/*
 *  Copyright (c) 20017-2018,
 *  Commissariat a l'Energie Atomique (CEA),
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification,
 *  are permitted provided that the following conditions are met:
 *
 *   - Redistributions of source code must retain the above copyright notice, this
 *     list of conditions and the following disclaimer.
 *
 *   - Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 *   - Neither the name of CEA nor the names of its contributors may be used to
 *     endorse or promote products derived from this software without specific prior
 *     written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED.
 *  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 *  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 *  OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 *  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors: Yves Lhuillier (yves.lhuillier@cea.fr)
 */
 
#ifndef __UNISIM_UTIL_SYMBOLIC_BINSEC_BINSEC_HH__
#define __UNISIM_UTIL_SYMBOLIC_BINSEC_BINSEC_HH__

#include <unisim/util/symbolic/symbolic.hh>
#include <map>
#include <set>
#include <vector>

namespace unisim {
namespace util {
namespace symbolic {
namespace binsec {
  
  typedef SmartValue<double>   F64;
  typedef SmartValue<float>    F32;
  typedef SmartValue<bool>     BOOL;
  typedef SmartValue<uint8_t>  U8;
  typedef SmartValue<uint16_t> U16;
  typedef SmartValue<uint32_t> U32;
  typedef SmartValue<uint64_t> U64;
  typedef SmartValue<int8_t>   S8;
  typedef SmartValue<int16_t>  S16;
  typedef SmartValue<int32_t>  S32;
  typedef SmartValue<int64_t>  S64;

  struct dbx
  {
    dbx( unsigned _bytes, uint64_t _value ) : value(_value), bytes(_bytes) {} uint64_t value; unsigned bytes;
    friend std::ostream& operator << ( std::ostream& sink, dbx const& _ );
  };
  
  struct ActionNode : public Conditional<ActionNode>
  {
    ActionNode() : Conditional<ActionNode>(), sinks(), sestats() {}

    void                    add_sink( Expr expr ) { expr.ConstSimplify(); sinks.insert( expr ); }
    void                    simplify();
    void                    commit_stats();
    
    std::set<Expr>          sinks;
    std::map<Expr,unsigned> sestats;
  };

  struct Program : public std::map<int,std::string>
  {
    typedef std::map<int,std::string> MapType;
    typedef MapType::iterator iterator;
    typedef MapType::const_iterator const_iterator;
    
    Program() : insn_count(0) {}
    
    void        Generate( ActionNode const* action_tree );
    
    int         next_insn() const { return insn_count; }
    int         allocate() { return insn_count++; }
    iterator    write(int idx, std::string const& s)
    {
      iterator itr = lower_bound(idx);
      if (itr != end() and itr->first == idx)
        throw std::runtime_error("overwriting statement");
      return insert( itr, std::make_pair(idx,s) );
    }
    
    int insn_count;
  };
  
  struct Label
  {
    Label( Program& _program ) : program(_program), id(-1) {}
    
    Label& operator= (Label const& l)
    {
      if (&program != &l.program) throw std::runtime_error("label programs must aggree");
      id = l.id;
      return *this;
    }
    
    int allocate() { return (id = program.allocate()); }
    
    bool valid() const { return id >= 0; }
    
    static bool subst_next( std::string& s, int next );
    
    int write( std::string const& src )
    {
      Program::iterator insn = program.write( id, src );
      if (subst_next(insn->second, program.next_insn()))
        id = program.allocate();
      return insn->first;
    }
    
    int GetID() const { return id; }
    
  private:
    Program& program;
    int id;
  };
  
  typedef std::map<Expr,Expr> Variables;
  
  struct ASExprNode : public ExprNode
  {
    virtual int GenCode( Label& label, Variables& vars, std::ostream& sink ) const = 0;
    static  int GenerateCode( Expr const& expr, Variables& vars, Label& label, std::ostream& sink );
    virtual Expr Simplify() const { return this; }
    static  Expr Simplify( Expr const& );
  };

  struct GetCode
  {
    GetCode(Expr const& _expr, Variables& _vars, Label& _label)
      : expr(_expr), vars(_vars), label(_label), expected(-1)
    {}
    GetCode(Expr const& _expr, Variables& _vars, Label& _label, int _expected)
      : expr(_expr), vars(_vars), label(_label), expected(_expected)
    {}
    
    friend std::ostream& operator << ( std::ostream& sink, GetCode const& gc )
    {
      int size = ASExprNode::GenerateCode( gc.expr, gc.vars, gc.label, sink );
      if (gc.expected >= 0 and gc.expected != size) { struct TypeSizeMisMatch {}; throw TypeSizeMisMatch(); }
      return sink;
    }
    Expr const& expr; Variables& vars; Label& label;
    int expected;
  };

  struct RegRead : public ASExprNode
  {
    virtual void GetRegName( std::ostream& ) const = 0;
    virtual int GenCode( Label& label, Variables& vars, std::ostream& sink ) const;
    virtual void Repr( std::ostream& sink ) const;
    virtual unsigned SubCount() const { return 0; }
    virtual int cmp( ExprNode const& rhs ) const override { return compare( dynamic_cast<RegRead const&>( rhs ) ); }
    int compare( RegRead const& rhs ) const { return 0; }
  };

  struct RegWrite : public ASExprNode
  {
    RegWrite( Expr const& _value ) : value(_value) {}
      
    virtual void GetRegName( std::ostream& ) const = 0;
    virtual ScalarType::id_t GetType() const { return ScalarType::VOID; }
    virtual int GenCode( Label& label, Variables& vars, std::ostream& sink ) const;
    virtual void Repr( std::ostream& sink ) const;
    virtual unsigned SubCount() const { return 1; }
    virtual Expr const& GetSub(unsigned idx) const { if (idx != 0) return ExprNode::GetSub(idx); return value; }
    virtual int cmp( ExprNode const& rhs ) const override { return compare( dynamic_cast<RegWrite const&>( rhs ) ); }
    int compare( RegWrite const& rhs ) const { return 0; /* Expr only */ }
    
    Expr value;
  };

  struct Branch : public RegWrite
  {
    Branch( Expr const& value ) : RegWrite( value ) {}
    virtual void annotate(std::ostream& sink) const = 0;
  };
  
  struct AssertFalse : public ASExprNode
  {
    AssertFalse() {}
    virtual AssertFalse* Mutate() const { return new AssertFalse( *this ); }
    virtual int GenCode( Label& label, Variables& vars, std::ostream& sink ) const
    {
      sink << "assert (false)";
      return 0;
    }
    virtual ScalarType::id_t GetType() const { return ScalarType::VOID; }

    virtual int cmp( ExprNode const& brhs ) const override { return 0; }
    virtual unsigned SubCount() const { return 0; }
    virtual void Repr( std::ostream& sink ) const { sink << "assert (false)"; }
  };

  struct BitFilter : public ASExprNode
  {
    BitFilter( Expr const& _input, unsigned _source, unsigned _rshift, unsigned _select, unsigned _extend, bool _sxtend )
      : input(_input), source(_source), pad0(), rshift(_rshift), pad1(), select(_select), pad2(), extend(_extend), sxtend(_sxtend)
    {}
    virtual BitFilter* Mutate() const { return new BitFilter( *this ); }
    Expr Simplify() const;
    virtual ScalarType::id_t GetType() const { return ScalarType::IntegerType( false, extend ); }
    virtual int GenCode( Label& label, Variables& vars, std::ostream& sink ) const;
    virtual void Repr( std::ostream& sink ) const;
    virtual int cmp( ExprNode const& rhs ) const override { return compare( dynamic_cast<BitFilter const&>( rhs ) ); }
    int compare( BitFilter const& rhs ) const;
    virtual unsigned SubCount() const { return 1; }
    virtual Expr const& GetSub(unsigned idx) const { if (idx != 0) return ExprNode::GetSub(idx); return input; }

    virtual ConstNodeBase const* Eval( unisim::util::symbolic::EvalSpace const& evs, ConstNodeBase const** cnbs ) const;
    
    Expr     input;
    uint64_t source   : 15;
    uint64_t pad0     :  1;
    uint64_t rshift   : 15;
    uint64_t pad1     :  1;
    uint64_t select   : 15;
    uint64_t pad2     :  1;
    uint64_t extend   : 15;
    uint64_t sxtend   :  1;
  };

  struct MemAccess : public ASExprNode
  {
    MemAccess(Expr const& _addr, unsigned _size, unsigned _alignment, bool _bigendian)
      : addr(_addr), lastbyte(_size-1), alignment(_alignment), bigendian(_bigendian)
    {}
    unsigned bytecount() const { return lastbyte+1; }
    virtual void Repr( std::ostream& sink ) const;
    int cmp( ExprNode const& rhs ) const override { return compare( dynamic_cast<MemAccess const&>( rhs ) ); }
    int compare( MemAccess const& rhs ) const
    {
      if (int delta = int(lastbyte) - int(rhs.lastbyte)) return delta;
      if (int delta = int(alignment) - int(rhs.alignment)) return delta;
      return (int(bigendian) - int(rhs.bigendian));
    }
    
    Expr addr;
    uint32_t lastbyte  : 16; // size in bytes [1,..;65536]
    uint32_t alignment :  4; // (log2) [1,2,4,8,...,32768] bytes
    uint32_t bigendian :  1; // 0=little-endian
    uint32_t reserved  : 11; // reserved
  };
    
  struct Load : public MemAccess
  {
    Load( Expr const& _addr, unsigned _size, unsigned _alignment, bool _bigendian )
      : MemAccess(_addr, _size, _alignment, _bigendian)
    {}
    virtual Load* Mutate() const { return new Load(*this); }
    virtual ScalarType::id_t GetType() const { return ScalarType::IntegerType(false, 8*bytecount()); }
    virtual int GenCode( Label& label, Variables& vars, std::ostream& sink ) const;
    virtual void Repr( std::ostream& sink ) const { MemAccess::Repr(sink); }
    virtual unsigned SubCount() const { return 1; }
    virtual Expr const& GetSub(unsigned idx) const { if (idx != 0) return ExprNode::GetSub(idx); return addr; }
  };

  struct Store : public MemAccess
  {
    Store( Expr const& _addr, Expr const& _value, unsigned _size, unsigned _alignment, bool _bigendian )
      : MemAccess(_addr, _size, _alignment, _bigendian), value(_value)
    {}
    virtual ScalarType::id_t GetType() const { return ScalarType::VOID; }
    virtual Store* Mutate() const { return new Store(*this); }
    virtual int GenCode( Label& label, Variables& vars, std::ostream& sink ) const;
    virtual void Repr( std::ostream& sink ) const;
    virtual unsigned SubCount() const { return 2; }
    virtual Expr const& GetSub(unsigned idx) const { switch (idx) { case 0: return addr; case 1: return value; } return ExprNode::GetSub(idx); }
      
    Expr value;
  };
    
} /* end of namespace binsec */
} /* end of namespace symbolic */
} /* end of namespace util */
} /* end of namespace unisim */

#endif // __UNISIM_UTIL_SYMBOLIC_BINSEC_BINSEC_HH__
