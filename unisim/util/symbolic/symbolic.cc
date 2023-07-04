/*
 *  Copyright (c) 2017-2023,
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

#include <unisim/util/symbolic/symbolic.hh>
#include <unisim/util/arithmetic/arithmetic.hh>
#include <unisim/util/endian/endian.hh>
#include <iostream>
#include <iomanip>
#include <set>
#include <sstream>
#include <vector>
#include <algorithm>

namespace unisim {
namespace util {
namespace symbolic {

  char const*
  Expr::dbgrepr() const
  {
    static std::string result_string;
    std::ostringstream buf;
    node->Repr(buf);
    result_string = buf.str();
    return result_string.c_str();
  }

  void
  ValueType::Repr(std::ostream& sink) const
  {
    switch (encoding)
      {
      default:                  sink << "X"; break;
      case ValueType::FLOAT:    sink << 'F'; break;
      case ValueType::UNSIGNED: sink << "U"; break;
      case ValueType::SIGNED:   sink << "S"; break;
      case ValueType::BOOL:
        sink << "BOOL";
        if (bitsize == 1) return;
        break;
      }
    sink << bitsize;
  }

  ValueType CValueType(ValueType::Code encoding, unsigned bitsize)
  {
    typedef long double f80_t;
    switch (encoding)
      {
      case ValueType::FLOAT:
        switch (bitsize) {
        default: break;
        case 32: return CValueType(float());
        case 64: return CValueType(double());
        case 80: return CValueType(f80_t());
        } break;
      case ValueType::UNSIGNED:
        switch (bitsize) {
        default: break;;
        case  8: return CValueType(uint8_t());
        case 16: return CValueType(uint16_t());
        case 32: return CValueType(uint32_t());
        case 64: return CValueType(uint64_t());
        } break;
      case ValueType::SIGNED:
        switch (bitsize) {
        default: break;;
        case  8: return CValueType(int8_t());
        case 16: return CValueType(int16_t());
        case 32: return CValueType(int32_t());
        case 64: return CValueType(int64_t());
        } break;
      case ValueType::BOOL:
        if (bitsize != 1) break;
        return CValueType(bool());
      default:
        break;
      }
    struct Bad {}; throw Bad();
    return NoValueType();
  }

  ValueType NoValueType() { return ValueType(ValueType::NA, 0); }

  std::ostream&
  operator << (std::ostream& sink, Expr const& expr )
  {
    if (expr.good())
      expr->Repr( sink );
    else
      sink << "*null*";

    return sink;
  }

  ConstNodeBase const* Evaluator::Simplify( unsigned, Expr& expr ) const { return expr.Simplify(*this); }

  ConstNodeBase const*
  Expr::Simplify( Evaluator const& evaluator )
  {
    unsigned subcount = node->SubCount();
    Expr args[subcount];
    ConstNodeBase const* cnbs[subcount];
    bool const_args = true, simplified = false;
    for (unsigned idx = 0; idx < subcount; ++idx)
      {
        args[idx] = node->GetSub(idx);
        ExprNode const* original = args[idx].node;
        if (not (cnbs[idx] = evaluator.Simplify( idx, args[idx] )))
          const_args = false;
        if (args[idx].node != original)
          simplified = true;
      }

    if (const_args)
      {
        if (ConstNodeBase const* cn = node->Eval( &cnbs[0] ))
          {
            *this = Expr( cn );
            return cn;
          }
      }

    if (simplified)
      {
        ExprNode* nn = node->Mutate();
        for (unsigned idx = 0; idx < subcount; ++idx)
          const_cast<Expr&>( nn->GetSub(idx) ) = args[idx];
        *this = Expr( nn );
      }

    return 0;
  }

  void
  OpNodeBase::Repr( std::ostream& sink ) const
  {
    sink << op.c_str() << "( ";
    char const* sep = "";
    for (unsigned idx = 0, end = SubCount(); idx < end; sep = ", ", ++idx)
      sink << sep << GetSub(idx);
    sink << " )";
  }

  void
  ConstNodeBase::Repr( std::ostream& sink ) const
  {
    typedef long double f80_t;

    struct SaveFlags
    {
      SaveFlags( std::ostream& _sink ) : sink(_sink), flags(_sink.flags()) {} std::ostream& sink; std::ios_base::fmtflags flags;
      ~SaveFlags() { sink.flags(flags); }
    } save_flags(sink);

    ValueType tp = GetType();
    tp.Repr(sink);

    sink << '(';
    switch (tp.encoding)
      {
      case ValueType::UNSIGNED:
      case ValueType::SIGNED:
        {
          unsigned msb = tp.bitsize-1;
          bool neg = tp.encoding == ValueType::SIGNED and (GetBits(msb/64) >> (msb%64) & 1);

          struct
          {
            bool go(std::ostream& sink, ConstNodeBase const* cst, unsigned idx, unsigned last, bool neg, bool carry)
            {
              if (idx > last)
                {
                  sink << (neg ? "-" : "") << "0x" << std::hex << std::setfill('0');
                  return false;
                }
              uint64_t bits = (uint64_t(-neg) ^ cst->GetBits(idx)) + uint64_t(carry);
              if (go(sink, cst, idx+1, last, neg, carry and not bits))
                sink << std::setw(16) << bits;
              else if (bits or idx == 0)
                sink << std::setw(0) << bits;
              else
                return false;
              return true;
            }
          } print;
          print.go(sink, this, 0, msb/64, neg, neg );
        }
        break;

      case ValueType::FLOAT:
        sink << GetFloat(f80_t()) << ')';
        break;

      case ValueType::BOOL:
        sink << (GetBits(0) ? "true" : "false");
        break;

      default:
        break;
      }
    sink << ')';
  }

  void
  Zero::Repr( std::ostream& sink ) const
  {
    GetType().Repr(sink << "Zero.");
    sink << "()";
  }

  // ValueType const*
  // Zero::GetType() const
  // {

  //   static std::set<ZTP> type_descriptors;
  //   auto tp = type_descriptors.insert(ZTP(is_signed, bitsize)).first;

  //   return &*tp;
  // }

  ConstNodeBase const*
  Zero::AsConstNode() const
  {
    typedef long double f80_t;
    typedef unisim::util::arithmetic::Integer<4,false> uint128_t;
    typedef unisim::util::arithmetic::Integer<4,true>  int128_t;
    
    static Expr  F32Zero = make_const    <float>(0);
    static Expr  F64Zero = make_const   <double>(0);
    static Expr  F80Zero = make_const    <f80_t>(0);
    static Expr   U8Zero = make_const  <uint8_t>(0);
    static Expr  U16Zero = make_const <uint16_t>(0);
    static Expr  U32Zero = make_const <uint32_t>(0);
    static Expr  U64Zero = make_const <uint64_t>(0);
    static Expr U128Zero = make_const (uint128_t(0));
    static Expr   S8Zero = make_const   <int8_t>(0);
    static Expr  S16Zero = make_const  <int16_t>(0);
    static Expr  S32Zero = make_const  <int32_t>(0);
    static Expr  S64Zero = make_const  <int64_t>(0);
    static Expr S128Zero = make_const  (int128_t(0));
    static Expr BOOLZero = make_const     <bool>(0);

    ExprNode const* node = 0;
    switch (type.encoding)
      {
      case ValueType::FLOAT:
        switch (type.bitsize) {
        default: break;
        case  32: node =  F32Zero.node; break;
        case  64: node =  F64Zero.node; break;
        case  80: node =  F80Zero.node; break;
        } break;
      case ValueType::UNSIGNED:
        switch (type.bitsize) {
        default: break;;
        case   8: node =  U8Zero.node; break;
        case  16: node =  U16Zero.node; break;
        case  32: node =  U32Zero.node; break;
        case  64: node =  U64Zero.node; break;
        case 128: node =  U128Zero.node; break;
        } break;
      case ValueType::SIGNED:
        switch (type.bitsize) {
        default: break;;
        case   8: node =  S8Zero.node; break;
        case  16: node =  S16Zero.node; break;
        case  32: node =  S32Zero.node; break;
        case  64: node =  S64Zero.node; break;
        case 128: node =  S128Zero.node; break;
        } break;
      case ValueType::BOOL:
        if (type.bitsize == 1)
          node =  BOOLZero.node;
        break;
      default:
        break;
      }
    return static_cast<ConstNodeBase const*>(node);
  }

  void
  CastNodeBase::Repr( std::ostream& sink ) const
  {
    GetType().Repr(sink);
    sink << "( " << src << " )";
  }

  void FP::DefaultNaNNode::Repr( std::ostream& sink ) const { sink << "DefaultNaN()"; }
  void FP::IsNaNNode::Repr( std::ostream& sink ) const { sink << "IsNaN(" << src << ", " << int(signaling) << ", " << int(quiet) << ")"; }
  void FP::MulAddNode::Repr( std::ostream& sink ) const { sink << "MulAdd( " << acc << ", " << left << ", " << right << " )"; }
  void FP::IsInvalidMulAddNode::Repr( std::ostream& sink ) const { sink << "IsInvalidMulAdd( " << acc << ", " << left << ", " << right << " )"; }
  void FP::FtoFNode::Repr( std::ostream& sink ) const { sink << "FtoF( " << src << " )"; }
  void FP::FtoINodeBase::Repr( std::ostream& sink ) const { sink << "FtoI( " << src << " )"; }
  void FP::ItoFNodeBase::Repr( std::ostream& sink ) const { sink << "ItoF( " << src << " )"; }

  bool   EvalMul( bool, bool ) { throw std::logic_error( "No * for bool." ); }

  long double   EvalMod( long double l, long double r ) { throw std::logic_error( "No ^ for long double." ); }
  double   EvalMod( double l, double r ) { throw std::logic_error( "No ^ for double." ); }
  float    EvalMod( float l, float r ) { throw std::logic_error( "No ^ for float." ); }

  long double   EvalXor( long double l, long double r ) { throw std::logic_error( "No ^ for long double." ); }
  double   EvalXor( double l, double r ) { throw std::logic_error( "No ^ for double." ); }
  float    EvalXor( float l, float r ) { throw std::logic_error( "No ^ for float." ); }

  long double   EvalAnd( long double l, long double r ) { throw std::logic_error( "No & for long double." ); }
  double   EvalAnd( double l, double r ) { throw std::logic_error( "No & for double." ); }
  float    EvalAnd( float l, float r ) { throw std::logic_error( "No & for float." ); }

  long double   EvalOr( long double l, long double r ) { throw std::logic_error( "No | for long double." ); }
  double   EvalOr( double l, double r ) { throw std::logic_error( "No | for double." ); }
  float    EvalOr( float l, float r ) { throw std::logic_error( "No | for float." ); }

  long double   EvalNot( long double val ) { throw std::logic_error( "No ~ for long double." ); }
  double   EvalNot( double val ) { throw std::logic_error( "No ~ for double." ); }
  float    EvalNot( float val ) { throw std::logic_error( "No ~ for float." ); }

  bool   EvalSHL( bool, shift_type ) { throw std::logic_error( "No << for bool." ); }
  long double   EvalSHL( long double, shift_type ) { throw std::logic_error( "No << for long double." ); }
  double   EvalSHL( double, shift_type ) { throw std::logic_error( "No << for double." ); }
  float    EvalSHL( float, shift_type ) { throw std::logic_error( "No << for float." ); }

  long double   EvalSHR( long double, shift_type ) { throw std::logic_error( "No >> for long double." ); }
  double   EvalSHR( double, shift_type ) { throw std::logic_error( "No >> for double." ); }
  float    EvalSHR( float, shift_type ) { throw std::logic_error( "No >> for float." ); }

  uint32_t EvalByteSwap( uint32_t v ) { return unisim::util::endian::ByteSwap( v ); }
  uint16_t EvalByteSwap( uint16_t v ) { return unisim::util::endian::ByteSwap( v ); }

  uint32_t EvalBitScanReverse( uint32_t v ) { return unisim::util::arithmetic::BitScanReverse( v ); }
  uint32_t EvalBitScanReverse( uint64_t v ) { return unisim::util::arithmetic::BitScanReverse( v ); }

  uint32_t EvalBitScanForward( uint32_t v ) { return unisim::util::arithmetic::BitScanForward( v ); }

  uint32_t EvalPopCount( uint32_t v ) { return unisim::util::arithmetic::PopCount( v ); }
  uint64_t EvalPopCount( uint64_t v ) { return unisim::util::arithmetic::PopCount( v ); }

  uint32_t EvalRotateRight( uint32_t v, shift_type s ) { return unisim::util::arithmetic::RotateRight( v, s ); }
  uint64_t EvalRotateRight( uint64_t v, shift_type s ) { return unisim::util::arithmetic::RotateRight( v, s ); }

  uint32_t EvalRotateLeft( uint32_t v, shift_type s ) { return unisim::util::arithmetic::RotateLeft( v, s ); }
  
} /* end of namespace symbolic */
} /* end of namespace util */
} /* end of namespace unisim */

