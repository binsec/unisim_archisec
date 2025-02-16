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

#ifndef __UNISIM_UTIL_SYMBOLIC_SYMBOLIC_HH__
#define __UNISIM_UTIL_SYMBOLIC_SYMBOLIC_HH__

#include <unisim/util/arithmetic/arithmetic.hh>
#include <unisim/util/arithmetic/integer.hh>
#include <unisim/util/identifier/identifier.hh>
#include <stdexcept>
#include <set>
#include <limits>
#include <typeinfo>
#include <cstring>
#include <iosfwd>
#include <inttypes.h>

namespace unisim {
namespace util {
namespace symbolic {

  template <typename Bool> struct AssertBool {};
  template <>              struct AssertBool<bool> { static void check() {} };

  template <bool test> struct StaticAssert {};
  template <> struct StaticAssert<true> { static void check() {}; };

  template <class T, class U>  struct CmpTypes { static bool const same = false; };

  template <class T>  struct CmpTypes<T,T> { static bool const same = true; };

  struct ValueType
  {
    enum Code { NA=0, BOOL, UNSIGNED, SIGNED, FLOAT };
    ValueType(Code _encoding, unsigned _bitsize) : encoding(_encoding), bitsize(_bitsize) {}
    int cmp(ValueType const& rhs) const { if (int delta = int(encoding) - int(rhs.encoding)) return delta; return int(bitsize) - int(rhs.bitsize); }
    void Repr(std::ostream& sink) const;
    Code encoding : 8;
    uint32_t bitsize : 24;
  };

  struct Op : public identifier::Identifier<Op>
  {
    enum Code
      {
        Xor, And, Or,
        Ror, Rol, Lsl, Asr, Lsr,
        Add, Sub, Mul, Div, Divu, Mod, Modu, Min, Max, Minu, Maxu,
        CMov, Inc, Dec,
        Tzero, Tnzero,
        Teq, Tne, Tge, Tgt, Tle, Tlt, Tgeu, Tgtu, Tleu, Tltu,
        BSwp, BSR, BSF, POPCNT, Not, Neg,
        FAdd, FSub, FDiv, FMul,
        FCmp, FSQB, FFZ, FNeg, FSqrt, FAbs, FDen, FMod, FPow,
        FCeil, FFloor, FTrunc, FRound, FNear, FMax, FMin,
        Cast, ReinterpretAs,
        end
      } code;

    char const* c_str() const
    {
      switch (code)
        {
        case    Xor: return "Xor";
        case    And: return "And";
        case     Or: return "Or";
        case    Add: return "Add";
        case    Sub: return "Sub";
        case    Div: return "Div";
        case   Divu: return "Divu";
        case    Mod: return "Mod";
        case   Modu: return "Modu";
        case    Mul: return "Mul";
        case    Min: return "Min";
        case    Max: return "Max";
        case   Minu: return "Minu";
        case   Maxu: return "Maxu";
        case   CMov: return "CMov";
        case    Ror: return "Ror";
        case    Rol: return "Rol";
        case    Lsl: return "Lsl";
        case    Asr: return "Asr";
        case    Lsr: return "Lsr";
        case    Inc: return "Inc";
        case    Dec: return "Dec";
        case  Tzero: return "Tzero";
        case Tnzero: return "Tnzero";
        case    Teq: return "Teq";
        case    Tne: return "Tne";
        case    Tge: return "Tge";
        case    Tgt: return "Tgt";
        case    Tle: return "Tle";
        case    Tlt: return "Tlt";
        case   Tgeu: return "Tgeu";
        case   Tgtu: return "Tgtu";
        case   Tleu: return "Tleu";
        case   Tltu: return "Tltu";
        case   FCmp: return "FCmp";
        case   BSwp: return "BSwp";
        case    BSR: return "BSR";
        case    BSF: return "BSF";
        case POPCNT: return "POPCNT";
        case    Not: return "Not";
        case    Neg: return "Neg";
        case   FAdd: return "FAdd";
        case   FSub: return "FSub";
        case   FMul: return "FMul";
        case   FDiv: return "FDiv";
        case   FSQB: return "FSQB";
        case    FFZ: return "FFZ";
        case   FNeg: return "FNeg";
        case  FSqrt: return "FSqrt";
        case   FAbs: return "FAbs";
        case   FDen: return "FDen";
        case   FMod: return "FMod";
        case   FPow: return "FPow";
        case FFloor: return "FFloor";
        case  FCeil: return "FCeil";
        case FRound: return "FRound";
        case FTrunc: return "FTrunc";
        case  FNear: return "FNear";
        case   FMax: return "FMax";
        case   FMin: return "FMin";
        case   Cast: return "Cast";
        case ReinterpretAs: return "ReinterpretAs";
        case    end: break;
        }
      return "NA";
    }

    Op() : code(end) {}
    Op( Code _code ) : code(_code) {}
    Op( char const* _code ) : code(end) { init( _code ); }
  };

  // TODO: Use c++-20 concepts and requirements ASAP.

  template <typename VALUE_TYPE>
  struct TypeInfo
  {
    enum { BITSIZE = 8*sizeof(VALUE_TYPE), ENCODING = std::is_signed<VALUE_TYPE>::value ? ValueType::SIGNED : ValueType::UNSIGNED };
    static constexpr Op::Code rsh_op() { return std::is_signed<VALUE_TYPE>::value ? Op::Asr : Op::Lsr; }
    static constexpr Op::Code add_op() { return Op::Add; }
    static constexpr Op::Code sub_op() { return Op::Sub; }
    static constexpr Op::Code mul_op() { return Op::Mul; }
    static constexpr Op::Code div_op() { return std::is_signed<VALUE_TYPE>::value ? Op::Div : Op::Divu; }
    static constexpr Op::Code mod_op() { return std::is_signed<VALUE_TYPE>::value ? Op::Mod : Op::Modu; }
    static constexpr Op::Code min_op() { return std::is_signed<VALUE_TYPE>::value ? Op::Min : Op::Minu; }
    static constexpr Op::Code max_op() { return std::is_signed<VALUE_TYPE>::value ? Op::Max : Op::Maxu; }
  };

  template <> struct TypeInfo<bool>
  {
    enum { BITSIZE = 1, ENCODING = ValueType::BOOL };
  };
  struct FloatTypeOps
  {
    static constexpr Op::Code add_op() { return Op::FAdd; }
    static constexpr Op::Code sub_op() { return Op::FSub; }
    static constexpr Op::Code mul_op() { return Op::FMul; }
    static constexpr Op::Code div_op() { return Op::FDiv; }
    static constexpr Op::Code min_op() { return Op::FMin; }
    static constexpr Op::Code max_op() { return Op::FMax; }
  };
  template <> struct TypeInfo<float> : public FloatTypeOps
  {
    enum { BITSIZE = 32, ENCODING = ValueType::FLOAT };
  };
  template <> struct TypeInfo<double> : public FloatTypeOps
  {
    enum { BITSIZE = 64, ENCODING = ValueType::FLOAT };
  };
  template <> struct TypeInfo<long double> : public FloatTypeOps
  {
    enum { BITSIZE = 80, ENCODING = ValueType::FLOAT };
  };

  template <typename T>
  ValueType CValueType( T const& )
  {
    return ValueType(ValueType::Code(TypeInfo<T>::ENCODING), TypeInfo<T>::BITSIZE);
  }

  ValueType CValueType(ValueType::Code encoding, unsigned bitsize);
  ValueType NoValueType();

  template <typename T>
  struct WithValueType
  {
    static ValueType GetType() { return CValueType(typename T::value_type()); }
  };

  struct Expr;

  struct ConstNodeBase;
  struct OpNodeBase;

  struct ExprNode
  {
    /* Resource management */
    ExprNode() : refs(0) {}
    ExprNode(ExprNode const&) : refs(0) {}
    virtual ~ExprNode() {}
    void Retain() const { ++refs; }
    void Release() const { if (--refs == 0) delete this; }
    virtual ExprNode* Mutate() const = 0;
    /* Expression-Tree accessors */
    virtual unsigned SubCount() const = 0;
    virtual Expr const& GetSub(unsigned idx) const { throw std::logic_error("out of bound sub expression"); }
    /* Value accessors */
    virtual int cmp( ExprNode const& ) const = 0;
    virtual ValueType GetType() const = 0;
    virtual ConstNodeBase const* AsConstNode() const { return 0; } /* Cannot allocate */
    virtual OpNodeBase const*    AsOpNode() const { return 0; } /* Cannot allocate */
    /* Debugging */
    virtual void Repr( std::ostream& sink ) const = 0;

  private:
    friend class Expr;

    virtual ConstNodeBase const* Eval( ConstNodeBase const** ) const { return 0; } /* may allocate */

    mutable uintptr_t refs;
  };

  struct Zero : public ExprNode
  {
    Zero(ValueType::Code encoding, unsigned bitsize) : type(encoding, bitsize) {}
    Zero(ValueType const& tp) : type(tp) {}
    virtual Zero* Mutate() const override { return new Zero( *this ); };
    virtual unsigned SubCount() const override { return 0; };
    virtual ConstNodeBase const* AsConstNode() const override;
    virtual ConstNodeBase const* Eval( ConstNodeBase const** ) const { return AsConstNode(); }
    virtual void Repr( std::ostream& sink ) const override;
    virtual ValueType GetType() const override { return type; }
    virtual int cmp( ExprNode const& rhs ) const override { return 0; }
    ValueType type;
  };

  struct ConstNodeBase : public ExprNode
  {
    virtual unsigned SubCount() const override { return 0; };
    virtual ConstNodeBase const* Eval( ConstNodeBase const** ) const override { return this; }
    ConstNodeBase const* AsConstNode() const override { return this; };
    virtual ConstNodeBase* apply( Op op, ConstNodeBase const** args ) const = 0;
    virtual float GetFloat( float ) const = 0;
    virtual double GetFloat( double ) const = 0;
    virtual long double GetFloat( long double ) const = 0;
    virtual uint64_t GetBits( unsigned ) const = 0;
    virtual void Repr( std::ostream& sink ) const override;
  };

  typedef uint8_t shift_type;

  template <typename VALUE_TYPE>
  VALUE_TYPE EvalInc( VALUE_TYPE l ) { return l + VALUE_TYPE(1); }
  inline bool EvalInc( bool l ) { return not l; }
  template <typename VALUE_TYPE>
  VALUE_TYPE EvalDec( VALUE_TYPE l ) { return l - VALUE_TYPE(1); }
  inline bool EvalDec( bool l ) { return not l; }
  template <typename VALUE_TYPE>
  VALUE_TYPE EvalMul( VALUE_TYPE l, VALUE_TYPE r ) { return l * r; }
  bool EvalMul( bool, bool );
  template <typename VALUE_TYPE>
  VALUE_TYPE EvalMod( VALUE_TYPE l, VALUE_TYPE r ) { return l % r; }
  long double EvalMod( long double l, long double r );
  double     EvalMod( double l, double r );
  float      EvalMod( float l, float r );
  template <typename VALUE_TYPE>
  VALUE_TYPE EvalXor( VALUE_TYPE l, VALUE_TYPE r ) { return l ^ r; }
  long double EvalXor( long double l, long double r );
  double     EvalXor( double l, double r );
  float      EvalXor( float l, float r );
  template <typename VALUE_TYPE>
  VALUE_TYPE EvalAnd( VALUE_TYPE l, VALUE_TYPE r ) { return l & r; }
  long double EvalAnd( long double l, long double r );
  double     EvalAnd( double l, double r );
  float      EvalAnd( float l, float r );
  template <typename VALUE_TYPE>
  VALUE_TYPE EvalOr( VALUE_TYPE l, VALUE_TYPE r ) { return l | r; }
  long double EvalOr( long double l, long double r );
  double     EvalOr( double l, double r );
  float      EvalOr( float l, float r );
  template <typename VALUE_TYPE>
  VALUE_TYPE EvalNot( VALUE_TYPE val ) { return ~val; }
  inline bool EvalNot( bool val ) { return not val; }
  long double EvalNot( long double val );
  double     EvalNot( double val );
  float      EvalNot( float val );

  template <typename VALUE_TYPE>
  VALUE_TYPE EvalSHL( VALUE_TYPE l, shift_type shift ) { return l << shift; }
  bool       EvalSHL( bool, shift_type );
  long double EvalSHL( long double, shift_type );
  double     EvalSHL( double, shift_type );
  float      EvalSHL( float, shift_type );
  template <typename VALUE_TYPE>
  VALUE_TYPE EvalSHR( VALUE_TYPE l, shift_type shift ) { return l >> shift; }
  long double EvalSHR( long double, shift_type );
  double     EvalSHR( double, shift_type );
  float      EvalSHR( float, shift_type );
  template <typename VALUE_TYPE>
  VALUE_TYPE EvalByteSwap( VALUE_TYPE v ) { throw std::logic_error( "No ByteSwap for this type" ); }
  uint32_t   EvalByteSwap( uint32_t v );
  uint16_t   EvalByteSwap( uint16_t v );
  template <typename VALUE_TYPE>
  VALUE_TYPE EvalBitScanReverse( VALUE_TYPE v ) { throw std::logic_error( "No BitScanReverse for this type" ); }
  uint32_t   EvalBitScanReverse( uint32_t v );
  uint32_t   EvalBitScanReverse( uint64_t v );
  template <typename VALUE_TYPE>
  VALUE_TYPE EvalBitScanForward( VALUE_TYPE v ) { throw std::logic_error( "No BitScanForward for this type" ); }
  uint32_t   EvalBitScanForward( uint32_t v );
  template <typename VALUE_TYPE>
  VALUE_TYPE EvalPopCount( VALUE_TYPE v ) { throw std::logic_error( "No PopCount for this type" ); }
  uint32_t   EvalPopCount( uint32_t v );
  uint64_t   EvalPopCount( uint64_t v );
  template <typename VALUE_TYPE>
  VALUE_TYPE EvalRotateRight( VALUE_TYPE v, shift_type shift ) { throw std::logic_error( "No RotateRight for this type" ); }
  uint32_t   EvalRotateRight( uint32_t v, shift_type shift );
  uint64_t   EvalRotateRight( uint64_t v, shift_type shift );
  template <typename VALUE_TYPE>
  VALUE_TYPE EvalRotateLeft( VALUE_TYPE v, shift_type shift ) { throw std::logic_error( "No RotateLeft for this type" ); }
  uint32_t   EvalRotateLeft( uint32_t v, shift_type shift );

  template <typename VALUE_TYPE>
  VALUE_TYPE EvalReinterpretAs( VALUE_TYPE res, ConstNodeBase const* cst )
  {
    if (std::is_floating_point<VALUE_TYPE>::value)
      {
        throw std::logic_error( "No ReinterpretAs for this type" );
      }
    if (CmpTypes<VALUE_TYPE,bool>::same)
      return cst->GetBits(0) & 1;
    return cst->GetBits(0);
  }

  template <unsigned CELLCOUNT, bool SIGNED>
  unisim::util::arithmetic::Integer<CELLCOUNT,SIGNED> EvalReinterpretAs( unisim::util::arithmetic::Integer<CELLCOUNT,SIGNED> res, ConstNodeBase const* cst )
  {
    if (CELLCOUNT & 1)
      res.cells[CELLCOUNT-1] = cst->GetBits(CELLCOUNT/2);

    for (unsigned idx = CELLCOUNT/2; idx-- > 0;)
      {
        uint64_t bits = cst->GetBits(idx);
        res.cells[idx+1] = bits >> 32;
        res.cells[idx+0] = bits >>  0;
      }

    return res;
  }

  struct Evaluator
  {
    struct Failure {};
    virtual ~Evaluator() {}
    virtual ConstNodeBase const* Simplify(unsigned, Expr&) const;
  };

  struct Expr
  {
    Expr() : node() {} ExprNode const* node;
    Expr( Expr const& expr ) : node( expr.node ) { if (node) node->Retain(); }
    Expr( Expr&& expr ) : node( expr.node ) { expr.node = 0; }
    Expr( ExprNode const* _node ) : node( _node ) { if (node) node->Retain(); }
    ~Expr() { if (node) node->Release(); }

    Expr&  operator = ( Expr const& expr )
    {
      if (expr.node) expr.node->Retain();
      ExprNode const* old_node = node;
      node = expr.node;
      if (old_node) old_node->Release();
      return *this;
    }

    ExprNode const* operator->() const { return node; }
    ExprNode const* operator->() { return node; }
    ExprNode const& operator* () const { return *node; }

    int compare( Expr const& rhs ) const
    {
      // Do not compare null expressions
      if (not rhs.node) return     node ?  1 : 0;
      if (not     node) return rhs.node ? -1 : 0;

      /* First compare actual types */
      const std::type_info* til = &typeid(*node);
      const std::type_info* tir = &typeid(*rhs.node);
      if (til < tir) return -1;
      if (til > tir) return +1;

      /* Same types, call derived comparator */
      if (int delta = node->cmp( *rhs.node ))
        return delta;

      /* Compare sub operands recursively */
      unsigned subcount = node->SubCount();
      if (int delta = int(subcount) - int(rhs.node->SubCount()))
        return delta;
      for (unsigned idx = 0; idx < subcount; ++idx)
        if (int delta = node->GetSub(idx).compare(rhs.node->GetSub(idx)))
          return delta;

      /* equal to us*/
      return 0;
    }

    bool operator != ( Expr const& rhs ) const { return compare( rhs ) != 0; }
    bool operator == ( Expr const& rhs ) const { return compare( rhs ) == 0; }
    bool operator  < ( Expr const& rhs ) const { return compare( rhs )  < 0; }
    bool operator  > ( Expr const& rhs ) const { return compare( rhs )  > 0; }

    ConstNodeBase const* ConstSimplify() { return Simplify(Evaluator()); }
    template <typename VALUE_TYPE> VALUE_TYPE Eval() const;

    ConstNodeBase const* Simplify( Evaluator const& evaluator );

    char const* dbgrepr() const;
    bool good() const { return node; }
    friend std::ostream& operator << (std::ostream&, Expr const&);
  };

  struct OpNodeBase : public ExprNode
  {
    OpNodeBase( Op _op ) : op(_op) { if (op.code == op.end) { struct Bad {}; throw Bad(); } }
    virtual OpNodeBase const* AsOpNode() const override { return this; }
    virtual ValueType GetType() const override
    {
      switch (op.code)
        {
        case Op::BSwp: case Op::Not: case Op::Neg:  case Op::BSR:   case Op::BSF:  case Op::POPCNT:
        case Op::FSQB: case Op::FFZ: case Op::FNeg: case Op::FSqrt: case Op::FAbs: case Op::FMod: case Op::FPow:
        case Op::FFloor: case Op::FCeil: case Op::FTrunc: case Op::FRound: case Op::FNear:
        case Op::FMax: case Op::FMin: case Op::FAdd: case Op::FSub: case Op::FMul: case Op::FDiv:
        case Op::Xor:  case Op::And: case Op::Or:
        case Op::Lsl:  case Op::Lsr: case Op::Asr:  case Op::Ror:   case Op::Rol:
        case Op::Add:  case Op::Sub: case Op::Min:  case Op::Max: case Op::Minu:  case Op::Maxu: case Op::CMov:
        case Op::Mul:  case Op::Div: case Op::Mod: case Op::Divu: case Op::Modu:
        case Op::ReinterpretAs: case Op::Inc: case Op::Dec:

          return GetSub(0)->GetType();

        case Op::FDen: case Op::Tzero: case Op::Tnzero:
        case Op::Teq: case Op::Tne:  case Op::Tleu: case Op::Tle:  case Op::Tltu:
        case Op::Tlt: case Op::Tgeu: case Op::Tge:  case Op::Tgtu: case Op::Tgt:
          return CValueType(bool());

        case Op::FCmp:
          return CValueType(int32_t());

        case Op::Cast:
           /* Should have been handled in derived type */
        case Op::end:
          throw std::logic_error("???");
        }
      return NoValueType();
    }

    virtual int cmp( ExprNode const& rhs ) const override { return compare( dynamic_cast<OpNodeBase const&>( rhs ) ); }
    int compare( OpNodeBase const& rhs ) const { return op.cmp( rhs.op ); }

    virtual void Repr( std::ostream& sink ) const override;

    Op op;
  };

  template <typename VALUE_TYPE>
  struct ConstNode : public ConstNodeBase
  {
    typedef ConstNode<VALUE_TYPE> this_type;

    ConstNode( VALUE_TYPE _value ) : value( _value ) {} VALUE_TYPE value;
    virtual this_type* Mutate() const override { return new this_type( *this ); };

    static VALUE_TYPE GetValue( ConstNodeBase const* cnb );

    ConstNodeBase*
    apply( Op op, ConstNodeBase const** args ) const override
    {
      switch (op.code)
        {
        case Op::BSwp:   return new this_type( EvalByteSwap( value ) );
        case Op::Not:    return new this_type( EvalNot( value ) );
        case Op::Neg:    return new this_type( - value );
        case Op::BSR:    return new this_type( EvalBitScanReverse( value ) );
        case Op::BSF:    return new this_type( EvalBitScanForward( value ) );
        case Op::POPCNT: return new this_type( EvalPopCount( value ) );
        case Op::Min:
        case Op::Minu:   return new this_type( std::min( value, GetValue( args[1] ) ) );
        case Op::Max:
        case Op::Maxu:   return new this_type( std::max( value, GetValue( args[1] ) ) );
        case Op::Xor:    return new this_type( EvalXor( value, GetValue( args[1] ) ) );
        case Op::And:    return new this_type( EvalAnd( value, GetValue( args[1] ) ) );
        case Op::Or:     return new this_type( EvalOr( value, GetValue( args[1] ) ) );
        case Op::CMov:   return new this_type( dynamic_cast<ConstNode<bool> const&>(*args[2]).value ? value : GetValue( args[1] ) );
        case Op::Lsl:    return new this_type( EvalSHL( value, dynamic_cast<ConstNode<shift_type> const&>(*args[1]).value ) );
        case Op::Lsr:
        case Op::Asr:    return new this_type( EvalSHR( value, dynamic_cast<ConstNode<shift_type> const&>(*args[1]).value ) );
        case Op::FAdd:
        case Op::Add:    return new this_type( value + GetValue( args[1] ) );
        case Op::FSub:
        case Op::Sub:    return new this_type( value - GetValue( args[1] ) );
        case Op::FMul:
        case Op::Mul:    return new this_type( EvalMul( value, GetValue( args[1] ) ) );
        case Op::FDiv: case Op::Divu:
        case Op::Div:   return new this_type( value / GetValue( args[1] ) );
        case Op::Mod:
        case Op::Modu:   return new this_type( EvalMod( value, GetValue( args[1] ) ) );
        case Op::Inc:    return new this_type( EvalInc( value ) );
        case Op::Dec:    return new this_type( EvalDec( value ) );

        case Op::Tnzero: return new ConstNode   <bool>   ( value != VALUE_TYPE() );
        case Op::Tzero:  return new ConstNode   <bool>   ( value == VALUE_TYPE() );
        case Op::Teq:    return new ConstNode   <bool>   ( value == GetValue( args[1] ) );
        case Op::Tne:    return new ConstNode   <bool>   ( value != GetValue( args[1] ) );
        case Op::Tleu:
        case Op::Tle:    return new ConstNode   <bool>   ( value <= GetValue( args[1] ) );
        case Op::Tltu:
        case Op::Tlt:    return new ConstNode   <bool>   ( value <  GetValue( args[1] ) );
        case Op::Tgeu:
        case Op::Tge:    return new ConstNode   <bool>   ( value >= GetValue( args[1] ) );
        case Op::Tgtu:
        case Op::Tgt:    return new ConstNode   <bool>   ( value >  GetValue( args[1] ) );
        case Op::Ror:    return new this_type( EvalRotateRight( value, dynamic_cast<ConstNode<shift_type> const&>(*args[1]).value ) );
        case Op::Rol:    return new this_type( EvalRotateLeft( value, dynamic_cast<ConstNode<shift_type> const&>(*args[1]).value ) );
        case Op::ReinterpretAs: return new this_type( EvalReinterpretAs( value, args[1] ) );

        case Op::FSQB:   break;
        case Op::FFZ:    break;
        case Op::FNeg:   break;
        case Op::FSqrt:  break;
        case Op::FAbs:   break;
        case Op::FDen:   break;
        case Op::FCmp:   break;
        case Op::FMod:   break;
        case Op::FPow:   break;
        case Op::FFloor: break;
        case Op::FCeil: break;
        case Op::FTrunc: break;
        case Op::FRound: break;
        case Op::FNear: break;
        case Op::FMax: break;
        case Op::FMin: break;

        case Op::Cast: /* Should have been handled elsewhere */
        case Op::end:
          break;
        }

      struct BadOperation {};
      throw BadOperation();
      return 0;
    }
    float GetFloat( float ) const override { return float(value); }
    double GetFloat( double ) const override { return double(value); }
    long double GetFloat( long double ) const override { typedef long double long_double; return long_double(value); }
    uint64_t GetBits(unsigned idx) const override
    {
      /* Return bits of infinitely precise encoding. */
      if (std::is_floating_point<VALUE_TYPE>::value)
        {
          struct Bad {};
          throw Bad();
          return 0;
        }
      else if (idx == 0)
        {
          return uint64_t(value);
        }
      else if (std::is_signed<VALUE_TYPE>::value)
        {
          return int64_t(value) >> 63;
        }
      else
        return 0;
    }
    ValueType GetType() const override { return CValueType(VALUE_TYPE()); }
    virtual int cmp( ExprNode const& rhs ) const override { return compare( dynamic_cast<this_type const&>( rhs ) ); }
    int compare( this_type const& rhs ) const { return (value < rhs.value) ? -1 : (value > rhs.value) ? +1 : 0; }
  };

  template <typename VALUE_TYPE>
  VALUE_TYPE Expr::Eval() const
  {
    struct : public Evaluator
    {
      ConstNodeBase const* Simplify(unsigned, Expr& expr) const override
      {
        if (auto res = expr.Simplify(*this))
          return res;
        throw Evaluator::Failure {};
        return 0;
      }
    } evaluator;

    Expr scratch = *this;
    return dynamic_cast<ConstNode<VALUE_TYPE> const&>(*evaluator.Simplify(0, scratch)).value;
  }

  template <typename VALUE_TYPE>
  VALUE_TYPE ConstNode<VALUE_TYPE>::GetValue( ConstNodeBase const* cnb ) { return dynamic_cast<ConstNode<VALUE_TYPE> const&>( *cnb ).value; }

  template <typename VALUE_TYPE>
  Expr make_const( VALUE_TYPE value ) { return Expr( new ConstNode<VALUE_TYPE>( value ) ); }

  template <unsigned SUBCOUNT>
  struct OpNode : public OpNodeBase
  {
    typedef OpNode<SUBCOUNT> this_type;

    OpNode( Op _op ) : OpNodeBase(_op) {}
    virtual this_type* Mutate() const override { return new this_type( *this ); }

    virtual ConstNodeBase const* Eval( ConstNodeBase const** cnbs ) const override { return cnbs[0]->apply( op, &cnbs[0] ); }

    virtual unsigned SubCount() const override { return SUBCOUNT; };
    virtual Expr const& GetSub(unsigned idx) const override
    {
      if (idx >= SUBCOUNT)
        return ExprNode::GetSub(idx);
      return subs[idx];
    };

    Expr subs[SUBCOUNT];
  };

  struct CastNodeBase : public OpNodeBase
  {
    CastNodeBase( Expr const& _src ) : OpNodeBase( Op::Cast ), src(_src) {}
    virtual ValueType GetSrcType() const = 0;
    virtual unsigned SubCount() const override { return 1; };
    virtual Expr const& GetSub(unsigned idx) const override { if (idx!= 0) return ExprNode::GetSub(idx); return src; }
    virtual void Repr( std::ostream& sink ) const override;
    virtual int cmp( ExprNode const& rhs ) const override { return compare( dynamic_cast<CastNodeBase const&>( rhs ) ); }
    int compare( CastNodeBase const& rhs ) const { return 0; }

    Expr src;
  };

  template <typename DST_VALUE_TYPE, typename SRC_VALUE_TYPE>
  struct CastNode : public CastNodeBase
  {
    typedef CastNode<DST_VALUE_TYPE,SRC_VALUE_TYPE> this_type;
    CastNode( Expr const& _src ) : CastNodeBase( _src ) {}
    virtual this_type* Mutate() const override { return new this_type( *this ); }
    virtual ValueType GetSrcType() const override { return CValueType(SRC_VALUE_TYPE()); }
    virtual ValueType GetType() const override { return CValueType(DST_VALUE_TYPE()); }
    virtual ConstNodeBase const* Eval( ConstNodeBase const** cnbs ) const override
    {
      return new ConstNode<DST_VALUE_TYPE>( DST_VALUE_TYPE(dynamic_cast<ConstNode<SRC_VALUE_TYPE> const&>(**cnbs).value) );
    }
  };

  /* 1 operand operation */
  inline Expr make_operation( Op op, Expr const& operand )
  {
    OpNode<1>* res = new OpNode<1>( op );
    res->subs[0] = operand;
    return res;
  }

  /* 2 operands operation */
  inline Expr make_operation( Op op, Expr const& left, Expr const& right )
  {
    OpNode<2>* res = new OpNode<2>( op );
    res->subs[0] = left; res->subs[1] = right;
    return res;
  }

  /* 3 operands operation */
  inline Expr make_operation( Op op, Expr const& arg0, Expr const& arg1, Expr const& arg2 )
  {
    OpNode<3>* res = new OpNode<3>( op );
    res->subs[0] = arg0;
    res->subs[1] = arg1;
    res->subs[2] = arg2;
    return res;
  }

  template <typename VALUE_TYPE>
  struct SmartValue
  {
    typedef VALUE_TYPE value_type;
    typedef SmartValue<value_type> this_type;
    typedef TypeInfo<value_type> info_type;
    static ValueType GetType() { return CValueType(value_type()); }

    Expr expr;

    SmartValue() : expr(0) {}

    SmartValue( Expr const& _expr )
      : expr( _expr )
    {
      expr.ConstSimplify();
    }

    explicit SmartValue( value_type value ) : expr( make_const( value ) ) {}

    template <typename SRC_VALUE_TYPE>
    explicit SmartValue( SmartValue<SRC_VALUE_TYPE> const& other )
    {
      if (CmpTypes<SRC_VALUE_TYPE,VALUE_TYPE>::same) {
        expr = other.expr;
      } else {
        expr = new CastNode<VALUE_TYPE,SRC_VALUE_TYPE>( other.expr );
        expr.ConstSimplify();
      }
    }

    static bool const is_signed = std::numeric_limits<value_type>::is_signed;
    static bool const is_float = std::is_floating_point<value_type>::value;

    this_type& operator = ( this_type const& other ) { expr = other.expr; return *this; }

    template <typename SHT>
    this_type operator << ( SHT sh ) const { return this_type( make_operation( Op::Lsl, expr, make_const<shift_type>(sh) ) ); }
    template <typename SHT>
    this_type operator >> ( SHT sh ) const { return this_type( make_operation( info_type::rsh_op(), expr, make_const<shift_type>(sh) ) ); }
    template <typename SHT>
    this_type& operator <<= ( SHT sh ) { expr = make_operation( Op::Lsl, expr, make_const<shift_type>(sh) ); return *this; }
    template <typename SHT>
    this_type& operator >>= ( SHT sh ) { expr = make_operation( info_type::rsh_op(), expr, make_const<shift_type>(sh) ); return *this; }

    template <typename SHT>
    this_type operator << ( SmartValue<SHT> const& sh ) const { return this_type( make_operation( Op::Lsl, expr, SmartValue<shift_type>(sh).expr ) ); }
    template <typename SHT>
    this_type operator >> ( SmartValue<SHT> const& sh ) const { return this_type( make_operation( info_type::rsh_op(), expr, SmartValue<shift_type>(sh).expr ) ); }
    template <typename SHT>
    this_type& operator <<= ( SmartValue<SHT> const& sh ) { expr = make_operation( Op::Lsl, expr, SmartValue<shift_type>(sh).expr ); return *this; }
    template <typename SHT>
    this_type& operator >>= ( SmartValue<SHT> const& sh ) { expr = make_operation( info_type::rsh_op(), expr, SmartValue<shift_type>(sh).expr ); return *this; }

    this_type operator - () const { return this_type( make_operation( Op::Neg, expr ) ); }
    this_type operator ~ () const { return this_type( make_operation( Op::Not, expr ) ); }

    this_type& operator += ( this_type const& other ) { expr = make_operation( info_type::add_op(), expr, other.expr ); return *this; }
    this_type& operator -= ( this_type const& other ) { expr = make_operation( info_type::sub_op(), expr, other.expr ); return *this; }
    this_type& operator *= ( this_type const& other ) { expr = make_operation( info_type::mul_op(), expr, other.expr ); return *this; }
    this_type& operator /= ( this_type const& other ) { expr = make_operation( info_type::div_op(), expr, other.expr ); return *this; }
    this_type& operator %= ( this_type const& other ) { expr = make_operation( info_type::mod_op(), expr, other.expr ); return *this; }
    this_type& operator ^= ( this_type const& other ) { expr = make_operation( Op::Xor, expr, other.expr ); return *this; }
    this_type& operator &= ( this_type const& other ) { expr = make_operation( Op::And, expr, other.expr ); return *this; }
    this_type& operator |= ( this_type const& other ) { expr =  make_operation( Op::Or, expr, other.expr ); return *this; }

    this_type operator + ( this_type const& other ) const { return this_type( make_operation( info_type::add_op(), expr, other.expr ) ); }
    this_type operator - ( this_type const& other ) const { return this_type( make_operation( info_type::sub_op(), expr, other.expr ) ); }
    this_type operator * ( this_type const& other ) const { return this_type( make_operation( info_type::mul_op(), expr, other.expr ) ); }
    this_type operator / ( this_type const& other ) const { return this_type( make_operation( info_type::div_op(), expr, other.expr ) ); }
    this_type operator % ( this_type const& other ) const { return this_type( make_operation( info_type::mod_op(), expr, other.expr ) ); }
    this_type operator ^ ( this_type const& other ) const { return this_type( make_operation( Op::Xor, expr, other.expr ) ); }
    this_type operator & ( this_type const& other ) const { return this_type( make_operation( Op::And, expr, other.expr ) ); }
    this_type operator | ( this_type const& other ) const { return this_type( make_operation( Op::Or, expr, other.expr ) ); }

    SmartValue<bool> operator == ( this_type const& other ) const { return SmartValue<bool>( make_operation( Op::Teq, expr, other.expr ) ); }
    SmartValue<bool> operator != ( this_type const& other ) const { return SmartValue<bool>( make_operation( Op::Tne, expr, other.expr ) ); }
    SmartValue<bool> operator <= ( this_type const& other ) const { return SmartValue<bool>( make_operation( is_signed ? Op::Tle : Op::Tleu, expr, other.expr ) ); }
    SmartValue<bool> operator >= ( this_type const& other ) const { return SmartValue<bool>( make_operation( is_signed ? Op::Tge : Op::Tgeu, expr, other.expr ) ); }
    SmartValue<bool> operator < ( this_type const& other ) const  { return SmartValue<bool>( make_operation( is_signed ? Op::Tlt : Op::Tltu, expr, other.expr ) ); }
    SmartValue<bool> operator > ( this_type const& other ) const  { return SmartValue<bool>( make_operation( is_signed ? Op::Tgt : Op::Tgtu, expr, other.expr ) ); }

    SmartValue<bool> operator ! () const
    { return SmartValue<bool>( make_operation( Op::Not, SmartValue<bool>( *this ).expr ) ); }

    SmartValue<bool> operator && ( SmartValue<bool> const& other ) const
    { AssertBool<value_type>::check(); return SmartValue<bool>( make_operation( Op::And, expr, other.expr ) ); }

    SmartValue<bool> operator || ( SmartValue<bool> const& other ) const
    { AssertBool<value_type>::check(); return SmartValue<bool>( Expr(  make_operation( Op::Or, expr, other.expr ) ) ); }
  };

  template <typename T> SmartValue<T> Minimum( SmartValue<T> const& l, SmartValue<T> const& r ) { return SmartValue<T>( make_operation( TypeInfo<T>::min_op(), l.expr, r.expr ) ); }
  template <typename T> SmartValue<T> Maximum( SmartValue<T> const& l, SmartValue<T> const& r ) { return SmartValue<T>( make_operation( TypeInfo<T>::max_op(), l.expr, r.expr ) ); }

  template <typename T> SmartValue<T> ConditionalMove(SmartValue<bool> cond, SmartValue<T> tval, SmartValue<T> fval) { return SmartValue<T>( make_operation( Op::CMov, tval.expr, fval.expr, cond.expr ) ); }

  template <typename UTP>
  UTP ByteSwap( UTP const& value ) { return UTP( make_operation( Op::BSwp, value.expr ) ); }

  template <typename UTP>
  UTP RotateRight( UTP const& value, shift_type sh ) { return UTP( make_operation( Op::Ror, value.expr, make_const<shift_type>(sh) ) ); }
  template <typename UTP, typename STP>
  UTP RotateRight( UTP const& value, STP const& sh ) { return UTP( make_operation( Op::Ror, value.expr, SmartValue<shift_type>(sh).expr ) ); }

  template <typename UTP>
  UTP RotateLeft( UTP const& value, shift_type sh ) { return UTP( make_operation( Op::Rol, value.expr, make_const<shift_type>(sh) ) ); }
  template <typename UTP, typename STP>
  UTP RotateLeft( UTP const& value, STP const& sh ) { return UTP( make_operation( Op::Rol, value.expr, SmartValue<shift_type>(sh).expr ) ); }

  template <typename UTP>
  UTP BitScanReverse( UTP const& value ) { return UTP( make_operation( Op::BSR, value.expr ) ); }

  template <typename UTP>
  UTP BitScanForward( UTP const& value ) { return UTP( make_operation( Op::BSF, value.expr ) ); }

  template <typename UTP>
  UTP PopCount(UTP const& v) { return UTP( make_operation( Op::POPCNT, v.expr ) ); }

  /*** Floating-Point ***/
  template <typename FTP>
  FTP power( FTP const& left, FTP const& right ) { return FTP( make_operation( Op::FPow, left.expr, right.expr ) ); }

  template <typename FTP>
  FTP fmodulo( FTP const& left, FTP const& right ) { return FTP( make_operation( Op::FMod, left.expr, right.expr ) ); }

  template <typename FTP>  FTP fabs( FTP const& value ) { return FTP( make_operation( Op::FAbs, value.expr ) ); }
  template <typename FTP>  FTP FAbs( FTP const& value ) { return fabs<FTP>( value ); }
  template <typename FTP>  FTP ceil( FTP const& value ) { return FTP( make_operation( Op::FCeil, value.expr ) ); }
  template <typename FTP>  FTP floor( FTP const& value ) { return FTP( make_operation( Op::FFloor, value.expr ) ); }
  template <typename FTP>  FTP trunc( FTP const& value ) { return FTP( make_operation( Op::FTrunc, value.expr ) ); }
  template <typename FTP>  FTP round( FTP const& value ) { return FTP( make_operation( Op::FRound, value.expr ) ); }
  template <typename FTP>  FTP nearbyint( FTP const& value ) { return FTP( make_operation( Op::FNear, value.expr ) ); }
  template <typename FTP>  FTP sqrt( FTP const& value ) { return FTP( make_operation( Op::FSqrt, value.expr ) ); }
  template <typename FTP>  FTP fmin( FTP const& l, FTP const& r ) { return FTP( make_operation( Op::FMin, l.expr, r.expr ) ); }
  template <typename FTP>  FTP fmax( FTP const& l, FTP const& r ) { return FTP( make_operation( Op::FMax, l.expr, r.expr ) ); }
  template <typename FTP>  FTP FMax( FTP const& l, FTP const& r ) { return fmax<FTP>( l, r ); }

  struct FP
  {
    struct DefaultNaNNode : public ExprNode
    {
      DefaultNaNNode( int _fsz ) : fsz( _fsz ) {} int fsz;
      virtual DefaultNaNNode* Mutate() const override { return new DefaultNaNNode( *this ); }
      virtual void Repr( std::ostream& sink ) const override;
      virtual unsigned SubCount() const override { return 0; };
      virtual ValueType GetType() const override { return CValueType(ValueType::FLOAT, fsz); }
      virtual int cmp( ExprNode const& rhs ) const override { return compare( dynamic_cast<DefaultNaNNode const&>( rhs ) ); }
      int compare( DefaultNaNNode const& rhs ) const { return fsz - rhs.fsz; }
    };

    static inline
    void SetDefaultNan( SmartValue<float>& result )
    { result = SmartValue<float>( Expr( new DefaultNaNNode( 32 ) ) ); }

    static inline
    void SetDefaultNan( SmartValue<double>& result )
    { result = SmartValue<double>( Expr( new DefaultNaNNode( 64 ) ) ); }

    template <typename FLOAT> static
    void SetQuietBit( FLOAT& op )
    {
      op = FLOAT( make_operation( Op::FSQB, op.expr ) );
    }

    template <typename FLOAT> static
    SmartValue<bool>
    FlushToZero( FLOAT& op, SmartValue<uint32_t> const& fpscr_val )
    {
      op = FLOAT( make_operation( Op::FFZ, op.expr ) );
      return SmartValue<bool>( make_operation( Op::FDen, op.expr ) );
    }

    template <typename FLOAT> static
    SmartValue<int32_t> Compare( FLOAT op1, FLOAT op2, SmartValue<uint32_t> const& fpscr_val )
    {
      return SmartValue<int32_t>( make_operation( Op::FCmp, op1.expr, op2.expr ) );
    }

    struct IsNaNNode : public ExprNode
    {
      IsNaNNode( Expr const& _src, bool _signaling, bool _quiet ) : src(_src), signaling(_signaling), quiet(_quiet) {} Expr src; bool signaling, quiet;
      virtual IsNaNNode* Mutate() const override { return new IsNaNNode( *this ); }
      virtual void Repr( std::ostream& sink ) const override;
      virtual int cmp( ExprNode const& rhs ) const override { return compare( dynamic_cast<IsNaNNode const&>( rhs ) ); }
      int compare( IsNaNNode const& rhs ) const { if (int delta = int(signaling) - int(rhs.signaling)) return delta; return int(quiet) - int(rhs.quiet); }
      virtual unsigned SubCount() const override { return 1; };
      virtual Expr const& GetSub(unsigned idx) const override { if (idx != 0) return ExprNode::GetSub(idx); return src; }
      virtual ValueType GetType() const override { return CValueType(bool()); }
    };

    template <typename FLOAT> static
    SmartValue<bool>
    IsSNaN( FLOAT const& op )
    {
      return SmartValue<bool>( Expr( new IsNaNNode( op.expr, true, false ) ) );
    }

    template <typename FLOAT> static
    SmartValue<bool>
    IsQNaN( FLOAT const& op )
    {
      return SmartValue<bool>( Expr( new IsNaNNode( op.expr, false, true ) ) );
    }

    template <typename FLOAT, class ARCH> static
    void Add( FLOAT& acc, FLOAT const& op2, ARCH& arch, SmartValue<uint32_t> const& fpscr_val )
    {
      acc = FLOAT( make_operation( Op::FAdd, acc.expr, op2.expr ) );
    }

    template <typename FLOAT, class ARCH> static
    void Sub( FLOAT& acc, FLOAT const& op2, ARCH& arch, SmartValue<uint32_t> const& fpscr_val )
    {
      acc = FLOAT( make_operation( Op::FSub, acc.expr, op2.expr ) );
    }

    template <typename FLOAT, class ARCH> static
    void Div( FLOAT& acc, FLOAT const& op2, ARCH& arch, SmartValue<uint32_t> const& fpscr_val )
    {
      acc = FLOAT( make_operation( Op::FDiv, acc.expr, op2.expr ) );
    }

    template <typename FLOAT, class ARCH> static
    void Mul( FLOAT& acc, FLOAT const& op2, ARCH& arch, SmartValue<uint32_t> const& fpscr_val )
    {
      acc = FLOAT( make_operation( Op::FMul, acc.expr, op2.expr ) );
    }

    struct MulAddNode : public ExprNode
    {
      MulAddNode( Expr const& _acc, Expr const& _left, Expr const& _right )
        : acc( _acc ), left( _left ), right( _right )
      {} Expr acc, left, right;
      virtual MulAddNode* Mutate() const override { return new MulAddNode( *this ); }

      virtual unsigned SubCount() const override { return 3; };
      virtual Expr const& GetSub(unsigned idx) const override { switch (idx) { case 0: return acc; case 1: return left; case 2: return right; } return ExprNode::GetSub(idx); };

      virtual void Repr( std::ostream& sink ) const override;
      virtual ValueType GetType() const override { return GetSub(0)->GetType(); }

      virtual int cmp( ExprNode const& rhs ) const override { return compare( dynamic_cast<MulAddNode const&>( rhs ) ); }
      int compare( MulAddNode const& rhs ) const { return 0; }
    };

    template <typename FLOAT, class ARCH> static
    void MulAdd( FLOAT& acc, FLOAT const& op1, FLOAT const& op2, ARCH& arch, SmartValue<uint32_t> const& fpscr_val )
    {
      acc = FLOAT( Expr( new MulAddNode( acc.expr, op1.expr, op2.expr ) ) );
    }

    struct IsInvalidMulAddNode : public ExprNode
    {
      IsInvalidMulAddNode( Expr const& _acc, Expr const& _left, Expr const& _right )
        : acc( _acc ), left( _left ), right( _right )
      {} Expr acc, left, right;
      virtual IsInvalidMulAddNode* Mutate() const override { return new IsInvalidMulAddNode( *this ); }
      virtual ValueType GetType() const override { return CValueType(bool()); }

      virtual unsigned SubCount() const override { return 3; };
      virtual Expr const& GetSub(unsigned idx) const override { switch (idx) { case 0: return acc; case 1: return left; case 2: return right; } return ExprNode::GetSub(idx); };

      virtual void Repr( std::ostream& sink ) const override;

      virtual int cmp( ExprNode const& rhs ) const override { return compare( dynamic_cast<IsInvalidMulAddNode const&>( rhs ) ); }
      int compare( IsInvalidMulAddNode const& rhs ) const { return 0; }
    };

    template <typename SOFTDBL> static
    SmartValue<bool>
    IsInvalidMulAdd( SOFTDBL const& acc, SOFTDBL const& op1, SOFTDBL const& op2, SmartValue<uint32_t> const& fpscr_val )
    {
      return SmartValue<bool>( Expr( new IsInvalidMulAddNode( acc.expr, op1.expr, op2.expr ) ) );
    }

    template <typename FLOAT> static
    void Neg( FLOAT& acc ) { acc = FLOAT( make_operation( Op::FNeg, acc.expr ) ); }

    template <typename FLOAT> static
    void Abs( FLOAT& acc ) { acc = FLOAT( make_operation( Op::FAbs, acc.expr ) ); }

    template <typename FLOAT, class ARCH> static
    void Sqrt( FLOAT& acc, ARCH& arch, SmartValue<uint32_t> const& fpscr_val ) { acc = FLOAT( make_operation( Op::FSqrt, acc.expr ) ); }

    struct FtoFNode : public ExprNode
    {
      FtoFNode( Expr const& _src, int _ssz, int _dsz )
        : src( _src ), ssz( _ssz ), dsz( _dsz )
      {} Expr src; int ssz; int dsz;
      virtual FtoFNode* Mutate() const override { return new FtoFNode( *this ); }

      virtual void Repr( std::ostream& sink ) const override;
      virtual unsigned SubCount() const override { return 1; }
      virtual Expr const& GetSub(unsigned idx) const override { if (idx != 0) return ExprNode::GetSub(idx); return src; }
      virtual ValueType GetType() const override { return CValueType(ValueType::FLOAT, dsz); }
      virtual int cmp( ExprNode const& rhs ) const override { return compare( dynamic_cast<FtoFNode const&>( rhs ) ); }
      int compare( FtoFNode const& rhs ) const
      {
        if (int delta = ssz - rhs.ssz) return delta;
        return dsz - rhs.dsz;
      }
    };

    template <typename ofpT, typename ifpT, class ARCH> static
    void FtoF( SmartValue<ofpT>& dst, SmartValue<ifpT> const& src, ARCH& arch, SmartValue<uint32_t> const& fpscr_val )
    {
      dst = SmartValue<ofpT>( Expr( new FtoFNode( src.expr, TypeInfo<ifpT>::BITSIZE, TypeInfo<ofpT>::BITSIZE ) ) );
    }

    struct FtoINodeBase : public ExprNode
    {
      FtoINodeBase( Expr const& _src, int _fb ) : src( _src ), fb( _fb ) {}
      virtual void Repr( std::ostream& sink ) const override;
      virtual unsigned SubCount() const override { return 1; }
      virtual Expr const& GetSub(unsigned idx) const override { if (idx != 0) return ExprNode::GetSub(idx); return src; }
      virtual int cmp( ExprNode const& rhs ) const override { return compare( dynamic_cast<FtoINodeBase const&>( rhs ) ); }
      int compare( FtoINodeBase const& rhs ) const { return fb - rhs.fb; }
      Expr src; int fb;
    };

    template <typename intT, typename fpT>
    struct FtoINode : public FtoINodeBase
    {
      FtoINode( Expr const& _src, int _fb ) : FtoINodeBase(_src, _fb) {}
      virtual FtoINode* Mutate() const override { return new FtoINode( *this ); }
      virtual ValueType GetType() const override { return CValueType(intT()); }
    };

    template <typename intT, typename fpT, class ARCH> static
    void FtoI( SmartValue<intT>& dst, SmartValue<fpT> const& src, int fracbits, ARCH& arch, SmartValue<uint32_t> const& fpscr_val )
    {
      dst = SmartValue<intT>( Expr( new FtoINode<intT,fpT>( src.expr, fracbits) ) );
    }

    struct ItoFNodeBase : public ExprNode
    {
      ItoFNodeBase( Expr const& _src, int _fb ) : src( _src ), fb( _fb ) {}
      virtual void Repr( std::ostream& sink ) const override;
      virtual unsigned SubCount() const override { return 1; }
      virtual Expr const& GetSub(unsigned idx) const override { if (idx != 0) return ExprNode::GetSub(idx); return src; }
      virtual int cmp( ExprNode const& rhs ) const override { return compare( dynamic_cast<ItoFNodeBase const&>( rhs ) ); }
      int compare( ItoFNodeBase const& rhs ) const { return fb - rhs.fb; }
      Expr src; int fb;
    };

    template <typename fpT, typename intT>
    struct ItoFNode : public ItoFNodeBase
    {
      ItoFNode( Expr const& src, int fb ) : ItoFNodeBase(src, fb) {}
      virtual ItoFNode* Mutate() const override { return new ItoFNode( *this ); }
      virtual ValueType GetType() const override { return CValueType(fpT()); }
    };

    template <typename fpT, typename intT, class ARCH> static
    void ItoF( SmartValue<fpT>& dst, SmartValue<intT> const& src, int fracbits, ARCH& arch, SmartValue<uint32_t> const& fpscr_val )
    {
      dst = SmartValue<fpT>( Expr( new ItoFNode<fpT,intT>( src.expr, fracbits ) ) );
    }

  };

  template <typename FTP>
  SmartValue<bool> isnan( FTP const& op )
  {
    return SmartValue<bool>( Expr( new FP::IsNaNNode( op.expr, true, true ) ) );
  }

  template <typename FTP> SmartValue<bool> IsNaN( FTP const& value ) { return isnan<FTP>( value ); }

  template <typename FTP>
  SmartValue<bool> issignaling( FTP const& op )
  {
    return SmartValue<bool>( Expr( new FP::IsNaNNode( op.expr, true, false ) ) );
  }

  template <class T>
  struct Choice
  {
    Choice() : nexts(), previous() {}
    ~Choice() { delete nexts[0]; delete nexts[1]; }

    T*      next(bool choice) { return nexts[choice]; }
    T*   getnext(bool choice) { if (not nexts[choice]) setnext(choice, new T); return nexts[choice]; }
    void setnext(bool choice, T* nxt) { nexts[choice] = nxt; nxt->previous = static_cast<T*>(this); }

    bool proceed()
    {
      if (getnext(false)->previous) return false;
      if (getnext(true)->previous)  return true;
      throw *this;                  return 0;
    }

    bool close()
    {
      if (T* p = previous)
        {
          previous = 0;
          if (this == p->nexts[1])
            return p->close();
          return false;
        }
      return true;
    }

    T*    nexts[2];
    T*    previous;
  };

  template <class T>
  struct Conditional : public Choice<T>
  {
    Conditional() : Choice<T>(), cond(), updates() {}

    bool  proceed( Expr const& _cond );
    void  add_update( Expr expr ) { expr.ConstSimplify(); updates.insert( expr ); }
    void  factorize();
    bool  merge(int, Expr const&, Expr const&) { return false; }

    Expr  cond;
    std::set<Expr> updates;
  };

  template <class T>
  bool
  Conditional<T>::proceed( Expr const& _cond )
  {
    if (not cond.good()) cond = _cond;
    else if (cond != _cond) throw cond;

    return Choice<T>::proceed();
  }

  template <class T>
  void
  Conditional<T>::factorize()
  {
    T* self = static_cast<T*>(this);
    std::set<Expr>::iterator
      lhi = self->nexts[0]->updates.begin(),
      lhe = self->nexts[0]->updates.end(),
      rhi = self->nexts[1]->updates.begin(),
      rhe = self->nexts[1]->updates.end();

    for (;;)
      {
        bool lstop = lhi == lhe;
        bool rstop = rhi == rhe;
        if (lstop and rstop)
          break;

        if (int delta = rstop ? -1 : lstop ? +1 : lhi->compare(*rhi))
          {
            if (self->merge(delta, lstop ? Expr() : *lhi, rstop ? Expr() : *rhi))
              {
                if (delta <= 0) self->nexts[0]->updates.erase( lhi++ );
                if (delta >= 0) self->nexts[1]->updates.erase( rhi++ );
              }
            else
              {
                if (delta < 0) ++lhi;
                else           ++rhi;
              }
          }
        else
          {
            self->updates.insert( *lhi );
            self->nexts[0]->updates.erase( lhi++ );
            self->nexts[1]->updates.erase( rhi++ );
          }
      }
  }

  template <class T>
  struct ExprScanner
  {
    void
    Flood( Expr const& e )
    {
      for (unsigned idx = 0, end = e->SubCount(); idx < end; ++idx)
        static_cast<T*>(this)->Process( e->GetSub(idx) );
    }
  };

} /* end of namespace symbolic */
} /* end of namespace util */
} /* end of namespace unisim */

#endif /* __UNISIM_UTIL_SYMBOLIC_SYMBOLIC_HH__ */
