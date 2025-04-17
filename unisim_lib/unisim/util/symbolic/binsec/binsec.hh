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
#include <vector>
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
    void                    generate(std::ostream& sink, unsigned bytes, uint64_t address, bool with_opaque=false) const;

  private:
    std::map<Expr,unsigned> sestats;
  };

  struct Scope;

  struct ASExprNode : public ExprNode
  {
    virtual ConstNodeBase const* Simplify( Expr const& mask, Expr& expr ) const;
    virtual int GenCode( std::ostream& sink, Scope& scope ) const = 0;
    static  int GenerateCode( Expr const& expr, std::ostream& sink, Scope& scope );
    static  int GenConstCode( ConstNodeBase const* node, std::ostream& sink );
  };

  struct BitFilter : public ASExprNode
  {
    static Expr mksimple( Expr const& _input, unsigned _source, unsigned _rshift, unsigned _select, unsigned _extend, bool _sxtend );

    virtual BitFilter* Mutate() const override { return new BitFilter( *this ); }
    virtual ValueType GetType() const { return ValueType(extend == 1 ? ValueType::BOOL : ValueType::UNSIGNED, extend); }
    virtual int GenCode( std::ostream& sink, Scope& scope ) const override;
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
    int GenCode(std::ostream&, Scope&) const override;
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
    GetCode(Expr const& _expr, Scope& _scope, int _expected=-1)
      : expr(_expr), scope(_scope), expected(_expected)
    {}

    friend std::ostream& operator << ( std::ostream& sink, GetCode const& gc )
    {
      int size = ASExprNode::GenerateCode( gc.expr, sink, gc.scope );
      if (gc.expected >= 0 and gc.expected != size) { struct TypeSizeMisMatch {}; throw TypeSizeMisMatch(); }
      return sink;
    }

    Expr const& expr; Scope& scope; int expected;
  };

  struct RegReadBase : public ASExprNode
  {
    virtual void GetRegName( std::ostream& ) const = 0;
    virtual int GenCode( std::ostream& sink, Scope& scope ) const;
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

  struct SideEffect : public ExprNode
  {
    virtual void GenerateCode( std::ostream& sink, Scope& scope ) const = 0;
  };

  struct Assignment : public SideEffect
  {
    Assignment( Expr const& _value ) : value(_value) {}

    virtual ValueType GetType() const override { return NoValueType(); }
    virtual unsigned SubCount() const override { return 1; }
    virtual Expr const& GetSub(unsigned idx) const override { if (idx != 0) return ExprNode::GetSub(idx); return value; }
    virtual Expr SourceRead() const = 0;
    static int GenInputCode( Expr const& input, Scope& scope, std::ostream& sink );

    Expr value;
  };

  struct RegWriteBase : public Assignment
  {
    RegWriteBase( Expr const& _value, int _size ) : Assignment(_value), reg_size(_size), slice_base(0), slice_size(_size) {}
    RegWriteBase( Expr const& _value, int _size, int _slice_base, int _slice_size ) : Assignment(_value), reg_size(_size), slice_base(_slice_base), slice_size(_slice_size) {}

    virtual void GetRegName( std::ostream& ) const = 0;

    virtual void GenerateCode( std::ostream& sink, Scope& scope ) const override;
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
    RegWrite( RID _id, Expr const& _value ) : Super(_value, util::numeric::Numeric<typename RID::value_type>::bitsize), id(_id) {}
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
    virtual void GenerateCode( std::ostream& sink, Scope& scope ) const override;
    virtual Expr SourceRead() const override { return Expr(); }
  };

  struct CallBase : public Branch
  {
    CallBase(Expr const& target) : Branch(target) {}
    virtual dbx PrintRA() const = 0;
    virtual void annotate(std::ostream& sink) const override;
    virtual void Repr( std::ostream& sink ) const override;
  };

  template <typename T>
  struct Call : public CallBase
  {
    typedef Call<T> this_type;
    Call( Expr const& target, T ra ) : CallBase(target), return_address(ra) {}
    virtual this_type* Mutate() const override { return new this_type( *this ); }
    virtual int cmp( ExprNode const& rhs ) const override { return compare( dynamic_cast<this_type const&>( rhs ) ); }
    int compare( this_type const& rhs ) const { return return_address < rhs.return_address ? -1 : return_address > rhs.return_address ? +1 : 0; }
    virtual dbx PrintRA() const override { return dbx(sizeof(T), return_address); }

    T return_address;
  };

  struct Ret : public Branch
  {
    Ret( Expr const& target ) : Branch(target) {}
    virtual Ret* Mutate() const override { return new Ret( *this ); }
    virtual void Repr( std::ostream& sink ) const override;
    virtual void annotate(std::ostream& sink) const override;
  };

  struct AssertFalse : public SideEffect
  {
    AssertFalse() {}
    virtual AssertFalse* Mutate() const override { return new AssertFalse( *this ); }
    virtual void GenerateCode( std::ostream& sink, Scope& scope ) const override;
    virtual ValueType GetType() const override{ return NoValueType(); }

    virtual int cmp( ExprNode const& brhs ) const override { return 0; }
    virtual unsigned SubCount() const override { return 0; }
    virtual void Repr( std::ostream& sink ) const override;
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
    virtual int GenCode( std::ostream& sink, Scope& scope ) const override;
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
    virtual void GenerateCode( std::ostream& sink, Scope& scope ) const override;
  };

  struct UndefinedValueBase : public ASExprNode
  {
    virtual unsigned SubCount() const override { return 0; };
    virtual void Repr( std::ostream& sink ) const override;
    virtual int cmp( ExprNode const& rhs ) const override { return this > &rhs ? +1 : this < &rhs ? -1 : 0; }
    virtual int GenCode( std::ostream& sink, Scope& scope ) const override;
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

  struct OpaqueBase : public ASExprNode
  {
    OpaqueBase(std::initializer_list<Expr> l) : sources(l) {}

    virtual unsigned SubCount() const override { return sources.size(); }
    virtual Expr const& GetSub(unsigned idx) const { if (idx < sources.size()) return sources[idx]; return ExprNode::GetSub(idx); }
    virtual void Repr( std::ostream& sink ) const override;
    virtual int cmp( ExprNode const& rhs ) const override { return this > &rhs ? +1 : this < &rhs ? -1 : 0; }
    virtual int GenCode( std::ostream& sink, Scope& scope ) const override;

    std::vector<Expr> sources;
  };

  template <typename T>
  struct Opaque : public OpaqueBase
  {
    Opaque(std::initializer_list<Expr> l) : OpaqueBase(l) {}

    typedef Opaque<T> this_type;
    virtual this_type* Mutate() const override { return new this_type( *this ); }
    virtual ValueType GetType() const override { return CValueType(T()); }
  };

  template <typename VALUE_TYPE>
  Expr make_opaque(VALUE_TYPE, std::initializer_list<Expr> l)
  {
    return new Opaque<VALUE_TYPE>(l);
  }

  struct OpaqueBV : public OpaqueBase
  {
    OpaqueBV(int bits, std::initializer_list<Expr> l) : OpaqueBase(l), bitsize(bits) {}
    virtual OpaqueBV* Mutate() const override { return new OpaqueBV( *this ); }
    virtual ValueType GetType() const override { return ValueType(ValueType::UNSIGNED, bitsize); }
    int bitsize;
  };

} /* end of namespace binsec */
} /* end of namespace symbolic */
} /* end of namespace util */
} /* end of namespace unisim */

#endif // __UNISIM_UTIL_SYMBOLIC_BINSEC_BINSEC_HH__
