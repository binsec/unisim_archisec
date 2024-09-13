/*
 *  Copyright (c) 2017,
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
#include <iostream>
#include <map>
#include <set>

namespace unisim {
namespace util {
namespace symbolic {
namespace binsec {

  typedef SmartValue<shift_type> USH;
  typedef SmartValue<uint8_t>    U8;
  typedef SmartValue<uint16_t>   U16;
  typedef SmartValue<uint32_t>   U32;
  typedef SmartValue<uint64_t>   U64;

  struct dbx
  {
    dbx( unsigned _bytes, uint64_t _value ) : value(_value << (64 - 8 * _bytes) >> (64 - 8 * _bytes)), bytes(_bytes) {} uint64_t value; unsigned bytes;
    friend std::ostream& operator << ( std::ostream& sink, dbx const& _ );
  };

  struct ActionNode : public Conditional<ActionNode>
  {
    ActionNode() : Conditional<ActionNode>(), sestats() {}

    void                    add_sink( Expr expr ) { updates.insert( expr ); }
    void                    simplify();
    bool                    merge( int& cmp, Expr const& t, Expr const& f );
    void                    commit_stats();
    std::map<Expr,unsigned> const& get_sestats() const { return sestats; }
    std::set<Expr> const&   get_sinks() const { return updates; }
    void                    generate(std::ostream& sink, unsigned bytes, uint64_t address) const;

  private:
    std::map<Expr,unsigned> sestats;
  };

  struct Instruction;

  struct Point
  {
    Point(Instruction* _insn) : insn(_insn) {}
    Point(Point const&) = default;
    Point& operator = (Point const&) = default;

    void connect( Point const& point );
    Point& append( Instruction* nins ) { Point np(nins); this->connect(np); return *this = np; }
    Point& prepend( Instruction* nins ) { Point np(nins); np.connect(*this); return *this = np; }

    Instruction* operator -> () const { return insn; }
    Instruction* get() const { return insn; }

    Instruction* insn;
  };

  typedef std::map<Expr,std::pair<std::string,int>> Variables;

  struct ASExprNode : public ExprNode
  {
    virtual ConstNodeBase const* Simplify( Expr const& mask, Expr& expr ) const;
    virtual int GenCode( std::ostream& sink, Variables& vars, Point& head ) const = 0;
    static  int GenerateCode( Expr const& expr, std::ostream& sink, Variables& vars, Point& head );
    static  int GenConstCode( ConstNodeBase const* node, std::ostream& sink );
  };

  struct BitFilter : public ASExprNode
  {
    static Expr mksimple( Expr const& _input, unsigned _source, unsigned _rshift, unsigned _select, unsigned _extend, bool _sxtend );

    virtual BitFilter* Mutate() const override { return new BitFilter( *this ); }
    virtual ValueType GetType() const { return ValueType(extend == 1 ? ValueType::BOOL : ValueType::UNSIGNED, extend); }
    virtual int GenCode( std::ostream& sink, Variables& vars, Point& head ) const override;
    virtual void Repr( std::ostream& sink ) const;
    virtual int cmp( ExprNode const& rhs ) const override { return compare( dynamic_cast<BitFilter const&>( rhs ) ); }
    int compare( BitFilter const& rhs ) const;
    virtual unsigned SubCount() const { return 1; }
    virtual Expr const& GetSub(unsigned idx) const { if (idx != 0) return ExprNode::GetSub(idx); return input; }

    virtual ConstNodeBase const* Eval( ConstNodeBase const** cnbs ) const override;
    virtual ConstNodeBase const* Simplify( Expr const&, Expr& ) const override;

    friend class BitSimplify;

  private:
    BitFilter(BitFilter const&) = default;
    BitFilter( Expr const& _input, unsigned _source, unsigned _rshift, unsigned _select, unsigned _extend, bool _sxtend )
      : input(_input), source(_source), pad0(), rshift(_rshift), pad1(), select(_select), pad2(), extend(_extend), sxtend(_sxtend)
    {}

    Expr     input;
    uint64_t source   : 15; // Source bit size
    uint64_t pad0     :  1;
    uint64_t rshift   : 15; // Source less significant bit selected
    uint64_t pad1     :  1;
    uint64_t select   : 15; // Source selected bit size
    uint64_t pad2     :  1;
    uint64_t extend   : 15; // Destination bit size
    uint64_t sxtend   :  1; // Destination is signed extended from `select` to `extend`
  };

  struct BitInsertNode : public ASExprNode
  {
    BitInsertNode(Expr const& _dst, Expr const& _src, unsigned _pos, unsigned _size) : dst(_dst), src(_src), pos(_pos), size(_size) {}
    unsigned SubCount() const override { return 2; };
    Expr const& GetSub(unsigned idx) const override { switch (idx) { case 0: return dst; case 1: return src; } return ExprNode::GetSub(idx); }
    void Repr(std::ostream& sink) const override;
    int cmp(ExprNode const& rhs) const override { return compare( dynamic_cast<BitInsertNode const&>(rhs) ); }
    int compare(BitInsertNode const& rhs) const { if (int delta = int(size) - int(rhs.size)) return delta; return int(pos) - int(rhs.pos); }
    ExprNode* Mutate() const { return new BitInsertNode(*this); }
    ValueType GetType() const override { return dst->GetType(); }
    int GenCode(std::ostream&, Variables&, Point&) const override;
    ConstNodeBase const* Simplify( Expr const&, Expr& ) const override;

    friend class BitSimplify;

  private:
    Expr dst, src;
    unsigned pos, size;
  };

  struct BitSimplify : public Evaluator
  {
    ConstNodeBase const* Process(Expr const& mask, Expr& expr) const;
    ConstNodeBase const* Simplify(unsigned idx, Expr& expr) const override { return Process(Expr(), expr); }
    static ConstNodeBase const* Do(Expr& expr) { return BitSimplify().Simplify(0,expr); }
    static Expr make_zero(ValueType);
  };

  struct GetCode
  {
    GetCode(Expr const& _expr, Variables& _vars, Point& _head, int _expected=-1)
      : expr(_expr), vars(_vars), head(_head), expected(-1)
    {}

    friend std::ostream& operator << ( std::ostream& sink, GetCode const& gc )
    {
      int size = ASExprNode::GenerateCode( gc.expr, sink, gc.vars, gc.head );
      if (gc.expected >= 0 and gc.expected != size) { struct TypeSizeMisMatch {}; throw TypeSizeMisMatch(); }
      return sink;
    }

    Expr const& expr; Variables& vars; Point& head; int expected;
  };

  struct RegReadBase : public ASExprNode
  {
    virtual void GetRegName( std::ostream& ) const = 0;
    virtual int GenCode( std::ostream& sink, Variables& vars, Point& head ) const;
    virtual void Repr( std::ostream& sink ) const;
    virtual unsigned SubCount() const { return 0; }
    virtual int cmp( ExprNode const& rhs ) const override { return compare( dynamic_cast<RegReadBase const&>( rhs ) ); }
    int compare( RegReadBase const& rhs ) const { return 0; }
  };

  template <typename RID>
  struct RegRead : public RegReadBase
  {
    typedef RegRead<RID> this_type;
    typedef RegReadBase Super;
    RegRead( RID _id ) : Super(), id(_id) {}
    virtual this_type* Mutate() const override { return new this_type( *this ); }
    virtual ValueType GetType() const override { return RID::GetType(); }
    virtual void GetRegName( std::ostream& sink ) const override { id.Repr(sink); }
    virtual int cmp( ExprNode const& rhs ) const override { return compare( dynamic_cast<RegRead const&>( rhs ) ); }
    int compare( RegRead const& rhs ) const { if (int delta = Super::compare(rhs)) return delta; return id.cmp( rhs.id ); }

    RID id;
  };

  struct Assignment : public ExprNode
  {
    Assignment( Expr const& _value ) : value(_value) {}

    virtual ValueType GetType() const override { return NoValueType(); }
    virtual unsigned SubCount() const override { return 1; }
    virtual Expr const& GetSub(unsigned idx) const override { if (idx != 0) return ExprNode::GetSub(idx); return value; }
    virtual void GenerateCode( std::ostream& sink, Variables& vars ) const = 0;
    virtual Expr SourceRead() const = 0;
    static int GenInputCode( Expr const& input, Variables& vars, std::ostream& sink );

    Expr value;
  };

  struct RegWriteBase : public Assignment
  {
    RegWriteBase( Expr const& _value, int _size ) : Assignment(_value), reg_size(_size), slice_base(0), slice_size(_size) {}
    RegWriteBase( Expr const& _value, int _size, int _slice_base, int _slice_size ) : Assignment(_value), reg_size(_size), slice_base(_slice_base), slice_size(_slice_size) {}

    virtual void GetRegName( std::ostream& ) const = 0;

    virtual void GenerateCode( std::ostream& sink, Variables& vars ) const override;
    virtual void Repr( std::ostream& sink ) const override;
    virtual int cmp( ExprNode const& rhs ) const override { return compare( dynamic_cast<RegWriteBase const&>( rhs ) ); }
    Expr source_read( RegReadBase* reg_read ) const;
    int compare( RegWriteBase const& rhs ) const
    {
      if (int delta = reg_size - rhs.reg_size) return delta;
      if (int delta = slice_base - rhs.slice_base) return delta;
      return slice_size - rhs.slice_size;
    }

    int reg_size, slice_base, slice_size;
  };

  template <typename RID>
  struct RegWrite : public RegWriteBase
  {
    typedef RegWrite<RID> this_type;
    typedef RegWriteBase Super;
    RegWrite( RID _id, Expr const& _value ) : Super(_value, TypeInfo<typename RID::value_type>::BITSIZE), id(_id) {}
    RegWrite( RID _id, int slice_base, int slice_size, Expr const& _value ) : Super(_value, RID::GetType().bitsize, slice_base, slice_size), id(_id) {}
    virtual this_type* Mutate() const override { return new this_type( *this ); }
    virtual void GetRegName( std::ostream& sink ) const override { id.Repr(sink); }
    virtual int cmp( ExprNode const& rhs ) const override { return compare( dynamic_cast<RegWrite const&>( rhs ) ); }
    virtual Expr SourceRead() const override { return source_read(new RegRead<RID>(id)); }
    int compare( RegWrite const& rhs ) const { if (int delta = id.cmp( rhs.id )) return delta; return Super::compare( rhs ); }

    RID id;
  };

  struct Branch : public Assignment
  {
    Branch( Expr const& value ) : Assignment( value ) {}
    virtual Branch* Mutate() const override { return new Branch( *this ); }
    virtual void annotate(std::ostream& sink) const {};
    virtual int cmp( ExprNode const& rhs ) const override { return 0; }
    virtual void Repr( std::ostream& sink ) const override;
    virtual void GenerateCode( std::ostream& sink, Variables& vars ) const override;
    virtual Expr SourceRead() const override { return Expr(); }
  };

  template <typename T>
  struct Call : public Branch
  {
    typedef Call<T> this_type;
    Call( Expr const& target, T ra ) : Branch(target), return_address(ra) {}
    virtual this_type* Mutate() const override { return new this_type( *this ); }
    virtual void annotate(std::ostream& sink) const override
    {
      sink << " // call (" << binsec::dbx(sizeof(T), return_address) << ",0)";
    }
    virtual void Repr( std::ostream& sink ) const
    {
      sink << "Call(";
      Branch::Repr(sink);
      sink << ", " << binsec::dbx(sizeof(T), return_address) << ")";
    }
    virtual int cmp( ExprNode const& rhs ) const override { return compare( dynamic_cast<this_type const&>( rhs ) ); }
    int compare( this_type const& rhs ) const
    {
      if (return_address < rhs.return_address) return -1;
      return int(return_address > rhs.return_address);
    }

    T return_address;
  };

  template <typename T>
  struct Ret : public Branch
  {
    typedef Ret<T> this_type;
    Ret( Expr const& target ) : Branch(target) {}
    virtual this_type* Mutate() const override { return new this_type( *this ); }
    virtual void annotate(std::ostream& sink) const override
    {
      sink << " // ret";
    }
    virtual void Repr( std::ostream& sink ) const
    {
      sink << "Ret(";
      Branch::Repr(sink);
      sink << ")";
    }
  };

  struct AssertFalse : public ASExprNode
  {
    AssertFalse() {}
    virtual AssertFalse* Mutate() const override { return new AssertFalse( *this ); }
    virtual int GenCode( std::ostream& sink, Variables& vars, Point& head ) const override
    {
      sink << "assert (false)";
      return 0;
    }
    virtual ValueType GetType() const override{ return NoValueType(); }

    virtual int cmp( ExprNode const& brhs ) const override { return 0; }
    virtual unsigned SubCount() const override { return 0; }
    virtual void Repr( std::ostream& sink ) const override { sink << "assert (false)"; }
  };

  struct MemAccess
  {
    MemAccess(Expr const& _addr, unsigned _size, unsigned _alignment, bool _bigendian)
      : addr(_addr), lastbyte(_size-1), alignment(_alignment), bigendian(_bigendian)
    {}
    int bytecount() const { return lastbyte+1; }
    virtual void Repr( std::ostream& sink ) const;
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

  struct Load : public MemAccess, ASExprNode
  {
    Load( Expr const& _addr, unsigned _size, unsigned _alignment, bool _bigendian )
      : MemAccess(_addr, _size, _alignment, _bigendian)
    {}
    Load(MemAccess const& ma) : MemAccess(ma) {}

    int cmp( ExprNode const& rhs ) const override { return compare( dynamic_cast<MemAccess const&>( rhs ) ); }
    virtual Load* Mutate() const override { return new Load(*this); }
    virtual ValueType GetType() const override { return CValueType(ValueType::UNSIGNED, 8*bytecount()); }
    virtual int GenCode( std::ostream& sink, Variables& vars, Point& head ) const override;
    virtual void Repr( std::ostream& sink ) const override { MemAccess::Repr(sink); }
    virtual unsigned SubCount() const override { return 1; }
    virtual Expr const& GetSub(unsigned idx) const override { if (idx != 0) return ExprNode::GetSub(idx); return addr; }
  };

  struct Store : public MemAccess, Assignment
  {
    Store( Expr const& _addr, Expr const& _value, unsigned _size, unsigned _alignment, bool _bigendian )
      : MemAccess(_addr, _size, _alignment, _bigendian), Assignment(_value)
    {}
    int cmp( ExprNode const& rhs ) const override { return compare( dynamic_cast<MemAccess const&>( rhs ) ); }
    virtual ValueType GetType() const override { return NoValueType(); }
    virtual Store* Mutate() const override { return new Store(*this); }
    virtual void Repr( std::ostream& sink ) const override;
    virtual unsigned SubCount() const override { return 2; }
    virtual Expr const& GetSub(unsigned idx) const override { switch (idx) { case 0: return value; case 1: return addr; } return ExprNode::GetSub(idx); }
    virtual Expr SourceRead() const override { return new Load( *this ); }
    virtual void GenerateCode( std::ostream& sink, Variables& vars ) const override;
  };

  struct UndefinedValueBase : public ASExprNode
  {
    virtual unsigned SubCount() const override { return 0; };
    virtual void Repr( std::ostream& sink ) const override;
    virtual int cmp( ExprNode const& rhs ) const override { return this > &rhs ? +1 : this < &rhs ? -1 : 0; }
    virtual int GenCode( std::ostream& sink, Variables& vars, Point& head ) const override;
  };

  template <typename T>
  struct UndefinedValue : public UndefinedValueBase
  {
    typedef UndefinedValue<T> this_type;

    virtual this_type* Mutate() const override { return new this_type( *this ); }
    virtual ValueType GetType() const override { return CValueType(T()); }
  };

  template <typename VALUE_TYPE>
  Expr make_undefined_value( VALUE_TYPE ) { return Expr( new UndefinedValue<VALUE_TYPE>() ); }

} /* end of namespace binsec */
} /* end of namespace symbolic */
} /* end of namespace util */
} /* end of namespace unisim */

#endif // __UNISIM_UTIL_SYMBOLIC_BINSEC_BINSEC_HH__
