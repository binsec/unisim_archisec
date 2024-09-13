/*
 *  Copyright (c) 2009,
 *  Commissariat a l'Energie Atomique (CEA)
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

#ifndef PPC64DBA_TYPES_HH
#define PPC64DBA_TYPES_HH

#include <unisim/util/symbolic/binsec/binsec.hh>
#include <unisim/util/symbolic/symbolic.hh>
// #include <ppc64dec.hh>
// #include <iostream>
// #include <iomanip>
// #include <cstdlib>
// #include <set>
#include <inttypes.h>

namespace ppc64 {

  typedef unisim::util::symbolic::SmartValue<double>   F32;
  typedef unisim::util::symbolic::SmartValue<float>    F64;
  typedef unisim::util::symbolic::SmartValue<bool>     BOOL;
  typedef unisim::util::symbolic::SmartValue<uint8_t>  U8;
  typedef unisim::util::symbolic::SmartValue<uint16_t> U16;
  typedef unisim::util::symbolic::SmartValue<uint32_t> U32;
  typedef unisim::util::symbolic::SmartValue<uint64_t> U64;
  typedef unisim::util::symbolic::SmartValue<int8_t>   S8;
  typedef unisim::util::symbolic::SmartValue<int16_t>  S16;
  typedef unisim::util::symbolic::SmartValue<int32_t>  S32;
  typedef unisim::util::symbolic::SmartValue<int64_t>  S64;

  typedef U64 UINT;
  typedef S64 SINT;
  typedef U64 ADDRESS;

  struct Flags
  {
    typedef unisim::util::symbolic::Expr Expr;
    typedef unisim::util::symbolic::ExprNode ExprNode;

    struct RoundingMode : public Expr
    {
      RoundingMode(U64 const& rm) : Expr( rm.expr ) {}
      RoundingMode(unsigned rm) : Expr( unisim::util::symbolic::make_const<unsigned>( rm ) ) {}
    };

    Flags( RoundingMode const& _rm ) : rm(_rm) {}

    BOOL hasIncrementFraction(BOOL neg) const { return unisim::util::symbolic::make_opaque_operation(true, op); }
    BOOL isApproximate() const { return unisim::util::symbolic::make_opaque_operation(true, op); }         // FPSCR.FI
    BOOL isOverflow() const { return unisim::util::symbolic::make_opaque_operation(true, op); }               // FPSCR.OX
    BOOL isUnderflow() const { return unisim::util::symbolic::make_opaque_operation(true, op); }             // FPSCR.UX
    BOOL isDivisionByZero() const { return unisim::util::symbolic::make_opaque_operation(true, op); }   // FPSCR.ZX
    BOOL hasSNaNOperand() const { return unisim::util::symbolic::make_opaque_operation(true, op); }       // FPSCR.VXSNAN
    BOOL isInftyMinusInfty() const { return unisim::util::symbolic::make_opaque_operation(true, op); } // FPSCR.VXISI
    BOOL isInftyOnInfty() const { return unisim::util::symbolic::make_opaque_operation(true, op); }       // FPSCR.VXIDI
    BOOL isInftyMultZero() const { return unisim::util::symbolic::make_opaque_operation(true, op); }     // FPSCR.VXIMZ
    BOOL isZeroOnZero() const { return unisim::util::symbolic::make_opaque_operation(true, op); }           // FPSCR.VXZDZ

    void from( Expr const& _op ) { op = _op; }

    Expr rm, op;
  };

  struct SoftFloat;
  struct SoftDouble;

  template <class FP>
  U8 FPCompare( FP const& lhs, FP const& rhs )
  {
    return unisim::util::symbolic::make_opaque_operation(/*FCompare*/uint8_t(), lhs.expr, rhs.expr);
    // case Unordered: c = 1; break;
    // case Less: c = 8; break;
    // case Equal: c = 2; break;
    // case Greater: c = 4; break;
  }

  struct SoftFloat
  {
    typedef unisim::util::symbolic::Expr Expr;
    typedef unisim::util::symbolic::ExprNode ExprNode;

    enum ComparisonResult
      { CRNaN = 0, CRLess = 1, CRGreater = 2, CREqual = 3 };

    enum __FromRawBits__ { FromRawBits };
    SoftFloat( __FromRawBits__, U32 const& source ) : expr(unisim::util::symbolic::make_opaque_operation(float(), source.expr)) {}
    SoftFloat( SoftDouble const& source, Flags& flags );

    U32 queryRawBits() const { return Expr(unisim::util::symbolic::make_opaque_operation(/*FloatToRawU32*/uint32_t(), expr)); }

    void plusAssign(SoftFloat const& s, Flags& f) { expr = unisim::util::symbolic::make_opaque_operation(/*plus*/float(), s.expr, f.rm); f.from(expr); }
    void minusAssign(SoftFloat const& s, Flags& f) { expr = unisim::util::symbolic::make_opaque_operation(/*minus*/float(), s.expr, f.rm); f.from(expr); }
    void multAssign(SoftFloat const& s, Flags& f) { expr = unisim::util::symbolic::make_opaque_operation(/*mult*/float(), s.expr, f.rm); f.from(expr); }
    void divAssign(SoftFloat const& s, Flags& f) { expr = unisim::util::symbolic::make_opaque_operation(/*div*/float(), s.expr, f.rm); f.from(expr); }
    void multAndAddAssign(SoftFloat const& s1, SoftFloat const& s2, Flags& f)
    { expr = unisim::util::symbolic::make_opaque_operation(/*multAndAdd*/float(), s1.expr, s2.expr, f.rm); f.from(expr); }
    void multAndSubAssign(SoftFloat const& s1, SoftFloat const& s2, Flags& f)
    { expr = unisim::util::symbolic::make_opaque_operation(/*multAndSub*/float(), s1.expr, s2.expr, f.rm); f.from(expr); }
    void opposite() { expr = unisim::util::symbolic::make_opaque_operation(/*opposite*/float(),expr); }
    void setQuiet() { expr = unisim::util::symbolic::make_opaque_operation(/*setQuiet*/float(),expr); }
    void setPositive() { expr = unisim::util::symbolic::make_opaque_operation(/*setPos*/float(),expr); }

    //    ComparisonResult compare( SoftFloat const& rhs ) const { return ComparisonResult( FPCompare( *this, rhs ) ); }

    BOOL isNegative() const { return unisim::util::symbolic::make_opaque_operation(/*isNeg*/bool(),expr); }
    BOOL isNaN() const { return unisim::util::symbolic::make_opaque_operation(/*isNaN*/bool(), expr); }
    BOOL isQNaN() const { return unisim::util::symbolic::make_opaque_operation(/*isQNaN*/bool(), expr); }
    BOOL isZero() const { return unisim::util::symbolic::make_opaque_operation(/*isZero*/bool(),expr); }

    unisim::util::symbolic::Expr expr;
  };

  struct SoftDouble
  {
    typedef unisim::util::symbolic::Expr Expr;
    typedef unisim::util::symbolic::ExprNode ExprNode;

    enum ComparisonResult
      { CRNaN = 0, CRLess = 1, CRGreater = 2, CREqual = 3 };

    SoftDouble() : expr() {}

    enum __FromRawBits__ { FromRawBits };
    SoftDouble( __FromRawBits__, U64 const& source ) { fromRawBitsAssign(source); }

    SoftDouble(SoftFloat const& source, Flags& flags) { convertAssign(source, flags); }
    SoftDouble( S64 const& source, Flags& flags )
      : expr(unisim::util::symbolic::make_opaque_operation(/*S64ToDouble*/double(),  source.expr,  flags.rm))
    { flags.from( expr ); }

    SoftDouble& convertAssign( SoftFloat const& source, Flags& flags )
    {
      expr = unisim::util::symbolic::make_opaque_operation(/*FloatToDouble*/double(),  source.expr,  flags.rm);
      flags.from( expr );
      return *this;
    }
    SoftDouble& fromRawBitsAssign( U64 const& source )
    {
      expr = unisim::util::symbolic::make_opaque_operation(/*RawU64ToDouble*/double(), source.expr );
      return *this;
    }

    U64 queryRawBits() const { return unisim::util::symbolic::make_opaque_operation(/*DoubleToRawU64*/uint64_t(), expr ); }

    S64 queryS64( Flags& flags ) const
    {
      Expr res( unisim::util::symbolic::make_opaque_operation(/*DoubleToS64*/int64_t(),  expr,  flags.rm) );
      flags.from(res);
      return res;
    }
    S32 queryS32( Flags& flags ) const
    {
      Expr res( unisim::util::symbolic::make_opaque_operation(/*DoubleToS32*/int32_t(),  expr,  flags.rm) );
      flags.from(res);
      return res;
    }

    void plusAssign(SoftDouble const& s, Flags& f) { expr = unisim::util::symbolic::make_opaque_operation(/*plus*/double(), s.expr, f.rm); f.from(expr); }
    void minusAssign(SoftDouble const& s, Flags& f) { expr = unisim::util::symbolic::make_opaque_operation(/*minus*/double(), s.expr, f.rm); f.from(expr); }
    void multAssign(SoftDouble const& s, Flags& f) { expr = unisim::util::symbolic::make_opaque_operation(/*mult*/double(), s.expr, f.rm); f.from(expr); }
    void divAssign(SoftDouble const& s, Flags& f) { expr = unisim::util::symbolic::make_opaque_operation(/*div*/double(), s.expr, f.rm); f.from(expr); }
    void multAndAddAssign(SoftDouble const& s1, SoftDouble const& s2, Flags& f)
    { expr = unisim::util::symbolic::make_opaque_operation(/*multAndAdd*/double(), s1.expr, s2.expr, f.rm); f.from(expr); }
    void multAndSubAssign(SoftDouble const& s1, SoftDouble const& s2, Flags& f)
    { expr = unisim::util::symbolic::make_opaque_operation(/*multAndSub*/double(), s1.expr, s2.expr, f.rm); f.from(expr); }
    void opposite() { expr = unisim::util::symbolic::make_opaque_operation(/*opposite*/double(),expr); }
    void setQuiet() { expr = unisim::util::symbolic::make_opaque_operation(/*setQuiet*/double(),expr); }
    void setPositive() { expr = unisim::util::symbolic::make_opaque_operation(/*setPos*/double(),expr); }
    void setNegative() { expr = unisim::util::symbolic::make_opaque_operation(/*setNeg*/double(),expr); }
    void rSqrtEstimAssign() { expr = unisim::util::symbolic::make_opaque_operation(/*rSqrtEstim*/double(),expr); }

    void sqrtAssign(Flags& flags) { expr = unisim::util::symbolic::make_opaque_operation(/*sqrt*/double(), expr, flags.rm); flags.from(expr); }

    //    ComparisonResult compare( SoftDouble const& rhs ) const { return ComparisonResult( FPCompare( *this, rhs ) ); }

    BOOL isNegative() const { return unisim::util::symbolic::make_opaque_operation(/*isNeg*/bool(),expr); }
    BOOL isNaN() const { return unisim::util::symbolic::make_opaque_operation(/*isNaN*/bool(),expr); }
    BOOL isQNaN() const { return unisim::util::symbolic::make_opaque_operation(/*isQNaN*/bool(),expr); }
    BOOL isSNaN() const { return unisim::util::symbolic::make_opaque_operation(/*isSNaN*/bool(),expr); }
    BOOL isZero() const { return unisim::util::symbolic::make_opaque_operation(/*isZero*/bool(),expr); }
    BOOL isInfty() const { return unisim::util::symbolic::make_opaque_operation(/*isInfty*/bool(),expr); }

    unisim::util::symbolic::Expr expr;
  };

  inline SoftFloat::SoftFloat( SoftDouble const& source, Flags& flags )
    : expr(unisim::util::symbolic::make_opaque_operation(/*DoubleToFloat*/float(),  source.expr,  flags.rm))
  { flags.from( expr ); }

  template <typename T> void GenFPSCR_OX(U64& r, Flags const& f) { r.expr = unisim::util::symbolic::make_opaque_operation(/*GenFPSCR_OX*/uint64_t(), r.expr, f.op); }
  template <typename T> void GenFPSCR_UX(U64& r, Flags const& f) { r.expr = unisim::util::symbolic::make_opaque_operation(/*GenFPSCR_UX*/uint64_t(), r.expr, f.op); }
  template <typename T> void GenFPSCR_VXSNAN(U64& r, Flags const& f) { r.expr = unisim::util::symbolic::make_opaque_operation(/*GenFPSCR_VXSNAN*/uint64_t(), r.expr, f.op); }
  template <typename T> void GenFPSCR_VXISI(U64& r, Flags const& f) { r.expr = unisim::util::symbolic::make_opaque_operation(/*GenFPSCR_VXISI*/uint64_t(), r.expr, f.op); }
  template <typename T> void GenFPSCR_VXCVI(U64& r, Flags const& f) { r.expr = unisim::util::symbolic::make_opaque_operation(/*GenFPSCR_VXISI*/uint64_t(), r.expr, f.op); }
  template <typename T,typename D> void GenFPSCR_FR(U64& r, D const& d, Flags const& f) { r.expr = unisim::util::symbolic::make_opaque_operation(/*GenFPSCR_FR*/uint64_t(), r.expr, f.op); }
  template <typename T> void GenFPSCR_FI(U64& r, Flags const& f) { r.expr = unisim::util::symbolic::make_opaque_operation(/*GenFPSCR_FI*/uint64_t(), r.expr, f.op); }
  template <typename T,typename D> void GenFPSCR_FPRF(U64& r, D const& d) { r.expr = unisim::util::symbolic::make_opaque_operation(/*GenFPSCR_FI*/uint64_t(), r.expr, d.expr); }
  template <typename T> void GenFPSCR_FX(U64& r, U64& ar) { r.expr = unisim::util::symbolic::make_opaque_operation(/*GenFPSCR_FX*/uint64_t(), r.expr, ar.expr); }
  template <typename T> void GenFPSCR_VX(U64& r) { r.expr = unisim::util::symbolic::make_opaque_operation(/*GenFPSCR_VX*/uint64_t(),r.expr); }
  template <typename T> void GenFPSCR_FEX(U64& r) { r.expr = unisim::util::symbolic::make_opaque_operation(/*GenFPSCR_FEX*/uint64_t(),r.expr); }

} // end of namespace ppc64

#endif // PPC64DBA_TYPES_HH
