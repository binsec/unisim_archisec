/*
 *  Copyright (c) 2016-2023,
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
 *          Gilles Mouchard (gilles.mouchard@cea.fr)
 */

#ifndef __UNISIM_COMPONENT_CXX_PROCESSOR_ARM_ISA_ARM64_EXECUTE_HH__
#define __UNISIM_COMPONENT_CXX_PROCESSOR_ARM_ISA_ARM64_EXECUTE_HH__

#include <inttypes.h>
#include <unisim/component/cxx/processor/arm/isa/constants.hh>
#include <unisim/component/cxx/processor/arm/isa/execute.hh>
#include <sstream>

#define DEBUG_FP 0

namespace unisim {
namespace component {
namespace cxx {
namespace processor {
namespace arm {
namespace isa {
namespace arm64 {

template <class ARCH>
typename ARCH::U64
UnsignedMultiplyHigh64( ARCH&, typename ARCH::U64 opl, typename ARCH::U64 opr )
{
  typedef typename ARCH::U32 U32;
  typedef typename ARCH::U64 U64;
  
  U64 lhi = U64(opl >> 32), llo = U64(U32(opl)), rhi = U64(opr >> 32), rlo = U64(U32(opr));
  U64 hihi( lhi*rhi ), hilo( lhi*rlo), lohi( llo*rhi ), lolo( llo*rlo );
  return (((lolo >> 32) + U64(U32(hilo)) + U64(U32(lohi))) >> 32) + (hilo >> 32) + (lohi >> 32) + hihi;
}

template <class ARCH>
typename ARCH::S64
SignedMultiplyHigh64( ARCH&, typename ARCH::S64 opl, typename ARCH::S64 opr )
{
  typedef typename ARCH::U32 U32;
  typedef typename ARCH::U64 U64;

  U32 l[4], r[4];
  for (int idx = 0; idx < 4; ++idx)
    {
      l[idx] = U32(opl); opl >>= 32;
      r[idx] = U32(opr); opr >>= 32;
    }
  
  U64 res(0), sum(0);
  for (unsigned idx = 0; idx < 4; ++idx)
    {
      U64 carry(0);
      for (unsigned lidx = 0, lend = idx; lidx <= lend; lidx++)
        {
          U64 prod = U64(l[lidx])*U64(r[idx-lidx]);
          sum += U64(U32(prod));
          carry += prod >> 32;
        }
      res = (res >> 32) | (sum << 32);
      sum = carry + (sum >> 32);
    }

  return typename ARCH::S64(res);
}

// Poly32Mod2 on a bitstring does a polynomial Modulus over {0,1} operation
template <typename OUT, unsigned S, typename IN>
OUT PolyMod2(IN value, uint32_t _poly)
{
  IN poly(_poly);
  
  for (unsigned bit = 0; bit < S; ++bit)
    value = (value >> 1) ^ poly*(value & IN(1));
  
  return OUT(value);
}
  
  template <class ARCH, typename T>
  T Abs( ARCH& arch, T value, bool sat = false)
  {
    if (sat && arch.Test(value == std::numeric_limits<T>::min()))
      {
        arch.SetQC();
        return std::numeric_limits<T>::max();
      }
    else
      {
        return arch.Test(value >= T()) ? value : -value;
      }
  }

  template <class ARCH, typename T>
  T Neg( ARCH& arch, T value, bool sat = false)
  {
    if (std::numeric_limits<T>::is_signed && sat)
      {
        T res((std::numeric_limits<T>::is_signed && arch.Test(value < T(0))) ? std::numeric_limits<T>::max() : std::numeric_limits<T>::min());
        if (arch.Test(value == std::numeric_limits<T>::min())) arch.SetQC(); else res = -value;
        return res;
      }
    else
      {
        return -value;
      }
  }

  template <typename DST, class ARCH, typename SRC>
  DST SatNarrow( ARCH& core, SRC src )
  {
    if (std::numeric_limits<SRC>::is_signed)
      {
        if (std::numeric_limits<DST>::is_signed)
          {
            if (OverShift<DST>::size < OverShift<SRC>::size)
              {
                if (core.Test(src < SRC(std::numeric_limits<DST>::min())))
                  {
                    core.SetQC();
                    return std::numeric_limits<DST>::min();
                  }
                else if(core.Test(src > SRC(std::numeric_limits<DST>::max())))
                  {
                    core.SetQC();
                    return std::numeric_limits<DST>::max();
                  }
              }
          }
        else
          {
            if (core.Test(src < SRC(0)))
              {
                core.SetQC();
                return std::numeric_limits<DST>::min();
              }
            else if(OverShift<DST>::size < (OverShift<SRC>::size - 1) )
              {
                if(core.Test(src > SRC(std::numeric_limits<DST>::max())))
                  {
                    core.SetQC();
                    return std::numeric_limits<DST>::max();
                  }
              }
          }
      }
    else
      {
        if (std::numeric_limits<DST>::is_signed)
          {
            if (OverShift<DST>::size < (OverShift<SRC>::size - 1))
              {
                core.SetQC();
                return std::numeric_limits<DST>::max();
              }
          }
        else
          {
            if (OverShift<DST>::size < OverShift<SRC>::size )
              {
                if(core.Test(src > SRC(std::numeric_limits<DST>::max())))
                  {
                    core.SetQC();
                    return std::numeric_limits<DST>::max();
                  }
              }
          }
      }
      
    return DST(src);
  }
  
  template <class ARCH, typename OP>
  OP Div( ARCH& arch, OP op1, OP op2)
  {
    OP const zero(0);
    OP res(
      arch.Test( op2 == zero ) ? zero
                               : ( ( std::numeric_limits<OP>::is_signed and arch.Test( ( op1 == std::numeric_limits<OP>::min() ) and ( op2 == OP(-1) ) ) ) ? std::numeric_limits<OP>::min()
                                                                                                                                                           : (op1 / op2 ) )
    );

    return res;
  }

  template <class ARCH, typename OP, typename OP2>
  OP SatAdd( ARCH& arch, OP op1, OP2 op2)
  {
    if ( std::numeric_limits<OP>::is_signed )
      {
        if ( std::numeric_limits<OP2>::is_signed )
          {
            OP res( arch.Test(op1 <= OP(0)) ? std::numeric_limits<OP>::min() : std::numeric_limits<OP>::max() ), comp(res - op1);
            
            if (arch.Test(op1 <= OP(0)))
              {
                if (arch.Test(OP(op2) < comp)) arch.SetQC(); else res = op1 + OP(op2);
              }
            else
              {
                if (arch.Test(OP(op2) > comp)) arch.SetQC(); else res = op1 + OP(op2);
              }
            
            return res;
          }
        else
          {
            OP res( std::numeric_limits<OP>::max() ), comp(res - op1);
            
            if (arch.Test(op2 > OP2(comp))) arch.SetQC(); else res = op1 + OP(op2);
            
            return res;
          }
      }
    else
      {
        if( std::numeric_limits<OP2>::is_signed )
          {
            OP res( arch.Test(op2 < OP2(0)) ? std::numeric_limits<OP>::min() : std::numeric_limits<OP>::max() );
            
            if (arch.Test(op2 < OP2(0)))
              {
                if (arch.Test(OP(-op2) > op1)) arch.SetQC(); else res = op1 - OP(-op2);
              }
            else
              {
                if (arch.Test(OP(op2) > (res - op1))) arch.SetQC(); else res = op1 + OP(op2);
              }
              
            return res;
          }
        else
          {
            OP res( std::numeric_limits<OP>::max() ), comp(res - op1);
            
            if (arch.Test(OP(op2) > comp)) arch.SetQC(); else res = op1 + OP(op2);
            
            return res;
          }
      }
  }

  template <class ARCH, typename OP>
  OP SatSub( ARCH& arch, OP op1, OP op2)
  {
    if ( std::numeric_limits<OP>::is_signed )
      {
        OP res( arch.Test(op2 < OP(0)) ? std::numeric_limits<OP>::max() : std::numeric_limits<OP>::min() ), comp(res + op2);
        
        if (arch.Test(op2 < OP(0)))
          {
            if (arch.Test(op1 > comp)) arch.SetQC(); else res = op1 - op2;
          }
        else
          {
            if (arch.Test(op1 < comp)) arch.SetQC(); else res = op1 - op2;
          }
        
        return res;
      }
    else
      {
        OP res( 0 );
        
        if (arch.Test(op2 > op1)) arch.SetQC(); else res = op1 - op2;
        
        return res;
      }
  }
   
  template <class ARCH, typename FLOAT>
  FLOAT FPNaN( ARCH& arch, FLOAT value )
  {
    typedef typename ARCH::U32 U32;
    if ( arch.Test( DN.Get( arch.FPCR() ) != U32(0) ) )
    {
      FLOAT res = DefaultNaN<FLOAT>();
#if DEBUG_FP
      std::cerr << "default NaN: " << value << " (0x" << std::hex << ToPacked( value ) << std::dec << ") -> " << res << " (0x" << std::hex << ToPacked( res ) << std::dec << ")" << std::endl;
#endif
      return res;
    }
    FLOAT res = ClearSignaling( value );
#if DEBUG_FP
    std::cerr << "quiet NaN: " << value << " (0x" << std::hex << ToPacked( value ) << std::dec << ") -> " << res << " (0x" << std::hex << ToPacked( res ) << std::dec << ")" << std::endl;
#endif
    
    return res;
  }
  
  template <class ARCH, unsigned posT>
  void FPProcessException( ARCH& arch, unisim::util::arithmetic::BitField<posT,1> const& rf )
  {
    typedef typename ARCH::U32 U32;
    unisim::util::arithmetic::BitField<posT+8,1> const enable;
    if (arch.Test( enable.Get( arch.FPCR() ) != U32(0) ) )
      /*arch.FPTrap( posT )*/; /* FIXME */
    else
      rf.Set( arch.FPSR(), U32(1) );

#if DEBUG_FP
    {
      std::stringstream sstr;
      sstr << "FPSR=0x" << std::hex << arch.FPSR();
      std::cerr << sstr.str() << std::endl;
    }
#endif
  }
    
  template <typename operT>
  struct OutNaN
  {
    operT result;
    bool invalid;

    operator bool () const { return invalid; }
    operator operT () const { return result; }
  };
  
  template <typename ARCH, typename operT>
  OutNaN<operT> FPProcessNaNs(ARCH& arch, std::initializer_list<operT> l, bool fpexc)
  {
    OutNaN<operT> nan = {operT(), false};
    
    for (auto val : l)
      {
        if ( not arch.Test( IsNaN( val )) )
          continue;

        if ( arch.Test( IsSignaling( val ) ))
          {
            if ( fpexc )
              {
#if DEBUG_FP
                std::cerr << "IOC" << std::endl;
#endif
                FPProcessException( arch, IOC );
              }
              
            return {FPNaN( arch, val ), true};
          }

        if (not nan)
          nan = {FPNaN( arch, val ), true};
      }
    
    return nan;
  }

  template <typename ARCH, typename operT>
  OutNaN<operT> FPProcessSNaNs(ARCH& arch, std::initializer_list<operT> l, bool fpexc)
  {
    OutNaN<operT> nan = {operT(), false};
    
    for (auto val : l)
      {
        if (arch.Test( IsSignaling( val ) ))
          {
            if ( fpexc )
              {
#if DEBUG_FP
                std::cerr << "IOC" << std::endl;
#endif
                FPProcessException( arch, IOC );
              }
            
            return {FPNaN( arch, val ), true};
          }
      }
    
    return nan;
  }

  template <typename ARCH, typename operT>
  OutNaN<operT> FPProcessNaNsSignaling(ARCH& arch, std::initializer_list<operT> l, bool fpexc)
  {
    OutNaN<operT> nan = {operT(), false};
    
    for (auto val : l)
      {
        if ( arch.Test( IsNaN( val )) )
          {
            if ( fpexc )
              {
#if DEBUG_FP
                std::cerr << "IOC" << std::endl;
#endif
                FPProcessException( arch, IOC );
              }
            
            return {FPNaN( arch, val ), true};
          }
      }
    
    return nan;
  }
  
  struct FPRounding
  {
    enum Code {TIEEVEN, POSINF, NEGINF, ZERO, TIEAWAY, ODD} code;
  };
  
  struct NearestTieEven
  {
    template <typename T> static T roundint(T op, bool exact) { return RoundToInt(op, round_near_even, exact); }
    template <typename INTEGER, typename T> static INTEGER toint(T op, bool exact) { return ToInt<INTEGER>(op, round_near_even, exact); }
  };
  struct TowardPosInf
  {
    template <typename T> static T roundint(T op, bool exact) { return RoundToInt(op, round_max, exact); }
    template <typename INTEGER, typename T> static INTEGER toint(T op, bool exact) { return ToInt<INTEGER>(op, round_max, exact); }
  };
  struct TowardNegInf
  {
    template <typename T> static T roundint(T op, bool exact) { return RoundToInt(op, round_min, exact); }
    template <typename INTEGER, typename T> static INTEGER toint(T op, bool exact) { return ToInt<INTEGER>(op, round_min, exact); }
  };
  struct TowardZero
  {
    template <typename T> static T roundint(T op, bool exact) { return RoundToInt(op, round_minMag, exact); }
    template <typename INTEGER, typename T> static INTEGER toint(T op, bool exact) { return ToInt<INTEGER>(op, round_minMag, exact); }
  };
  struct NearestTieAway
  {
    template <typename T> static T roundint(T op, bool exact) { return RoundToInt(op, round_near_maxMag, exact); }
    template <typename INTEGER, typename T> static INTEGER toint(T op, bool exact) { return ToInt<INTEGER>(op, round_near_maxMag, exact); }
  };
  struct NearestTieOdd
  {
    template <typename T> static T roundint(T op, bool exact) { return RoundToInt(op, round_odd, exact); }
    template <typename INTEGER, typename T> static INTEGER toint(T op, bool exact) { return ToInt<INTEGER>(op, round_odd, exact); }
  };
  
  struct RoundingMode
  {
    FPRounding::Code rmode;
    
    template <typename ARCH> RoundingMode( ARCH& arch )
    {
      typedef typename ARCH::U32 U32;
      struct Bad {};
      U32 raw_rmode = RMode.Get( arch.FPCR() );
      if ( arch.Test( raw_rmode == U32(FPRounding::TIEEVEN) ) ) rmode = FPRounding::TIEEVEN;
      else if ( arch.Test( raw_rmode == U32(FPRounding::POSINF) ) ) rmode = FPRounding::POSINF;
      else if ( arch.Test( raw_rmode == U32(FPRounding::NEGINF) ) ) rmode = FPRounding::NEGINF;
      else if ( arch.Test( raw_rmode == U32(FPRounding::ZERO) ) ) rmode = FPRounding::ZERO;
      else throw Bad();
    }
    
    // RoundingMode( FPRounding::Code _mode ) : rmode(_mode) {}

    template <typename T> T roundint(T op, bool exact)
    {
#if DEBUG_FP
      std::cerr << "rmode: " << +rmode << std::endl;
#endif
      switch (rmode)
        {
        case FPRounding::TIEEVEN: return NearestTieEven::roundint(op, exact);
        case FPRounding::POSINF:  return TowardPosInf::roundint(op, exact);
        case FPRounding::NEGINF:  return TowardNegInf::roundint(op, exact);
        case FPRounding::ZERO:    return TowardZero::roundint(op, exact);
        case FPRounding::TIEAWAY: return NearestTieAway::roundint(op, exact);
        case FPRounding::ODD:     return NearestTieOdd::roundint(op, exact);
        }
      return op;
    }
    
    template <typename INTEGER, typename T> INTEGER toint(T op, bool exact)
    {
      switch (rmode)
        {
        case FPRounding::TIEEVEN: return NearestTieEven::toint<INTEGER>(op, exact);
        case FPRounding::POSINF:  return TowardPosInf::toint<INTEGER>(op, exact);
        case FPRounding::NEGINF:  return TowardNegInf::toint<INTEGER>(op, exact);
        case FPRounding::ZERO:    return TowardZero::toint<INTEGER>(op, exact);
        case FPRounding::TIEAWAY: return NearestTieAway::toint<INTEGER>(op, exact);
        case FPRounding::ODD:     return NearestTieOdd::toint<INTEGER>(op, exact);
        }
      return INTEGER();
    }
    
    unsigned roundingMode()
    {
      switch (rmode)
        {
        case FPRounding::TIEEVEN: return round_near_even;
        case FPRounding::POSINF:  return round_max;
        case FPRounding::NEGINF:  return round_min;
        case FPRounding::ZERO:    return round_minMag;
        case FPRounding::TIEAWAY: return round_near_maxMag;
        case FPRounding::ODD:     return round_odd;
        }
      return round_near_even;
    }
  };

  template <typename FLOAT>
  struct FlushToZero
  {
    unsigned ftz;
    
     template <typename ARCH> FlushToZero( ARCH& arch )
      : ftz( ( (  std::is_same<FLOAT, typename ARCH::F16>::value && arch.Test( FZ16.Get( arch.FPCR() ) != typename ARCH::U32(0) ) ) ||
               ( !std::is_same<FLOAT, typename ARCH::F16>::value && arch.Test( FZ.Get  ( arch.FPCR() ) != typename ARCH::U32(0) ) ) ) ? ftz_beforeRounding : ftz_never )
    {
    }
    
    unsigned flushToZero() const { return ftz; }
  };

  template <typename ARCH>
  void FPProcessExceptionFlags( ARCH& arch, unsigned eflags )
  {
    if( eflags & flag_inexact )
      {
#if DEBUG_FP
        std::cerr << "IXC" << std::endl;
#endif
        FPProcessException( arch, IXC );
      }

    if( eflags & flag_underflow )
      {
#if DEBUG_FP
        std::cerr << "UFC" << std::endl;
#endif
        FPProcessException( arch, UFC );
      }

    if( eflags & flag_overflow )
      {
#if DEBUG_FP
        std::cerr << "OFC" << std::endl;
#endif
        FPProcessException( arch, OFC );
      }

    if( eflags & flag_infinite )
      {
#if DEBUG_FP
      std::cerr << "DZC" << std::endl;
#endif
        FPProcessException( arch, DZC );
      }

    if( eflags & flag_invalid )
      {
#if DEBUG_FP
        std::cerr << "IOC" << std::endl;
#endif
        FPProcessException( arch, IOC );
      }
  }
  
  template <typename ARCH, typename FLOAT>
  FLOAT FPProcessDenormalInput(ARCH& arch, FLOAT op)
  {
    typedef typename ARCH::BOOL BOOL;
    typedef typename ARCH::U32 U32;
    BOOL is_denormal = IsDenormal( op );
#if DEBUG_FP
    if ( arch.Test( is_denormal ) )
    {
      std::cerr << "denormal (input): " << op << " (0x" << std::hex << ToPacked(op) << std::dec << ")" << std::endl;
    }
#endif
    if ( arch.Test( is_denormal ) &&
         ( ( std::is_same<FLOAT, typename ARCH::F16>::value && arch.Test( FZ16.Get( arch.FPCR() ) != U32(0) ) ) ||
           ( !std::is_same<FLOAT, typename ARCH::F16>::value &&
             ( arch.Test( FIZ.Get( arch.FPCR() ) != U32(0) ) ||
               ( arch.Test( AH.Get( arch.FPCR() ) == U32(0) ) &&
                 arch.Test( FZ.Get( arch.FPCR() ) != U32(0) )
               )
             )
           )
         )
       )
      {
        FLOAT res = Zeroes( op );
#if DEBUG_FP
        std::cerr << "flush-to-zero (input): " << op << " (0x" << std::hex << ToPacked(op) << std::dec << ") -> " << res << " (0x" << std::hex << ToPacked( res ) << std::dec << ")" << std::endl;
#endif
#if DEBUG_FP
        std::cerr << "IDC" << std::endl;
#endif
        if ( !std::is_same<FLOAT, typename ARCH::F16>::value )
          FPProcessException( arch, IDC );

        return res;
      }
      
    return op;
  }
  
  template <typename ARCH, typename FLOAT>
  FLOAT FPProcessDenormalOutput(ARCH& arch, FLOAT op)
  {
    typedef typename ARCH::BOOL BOOL;
    typedef typename ARCH::U32 U32;
    BOOL is_zero = op == FLOAT();
    BOOL is_denormal = IsDenormal( op );
#if DEBUG_FP
    if( arch.Test( is_zero ) )
    {
      std::cerr << "zero (output): " << op << " (0x" << std::hex << ToPacked(op) << std::dec << ")" << std::endl;
    }
    if( arch.Test( is_denormal ) )
    {
      std::cerr << "denormal (output): " << op << " (0x" << std::hex << ToPacked(op) << std::dec << ")" << std::endl;
    }
#endif
    if ( arch.Test( is_zero || is_denormal ) &&
          (
            (  std::is_same<FLOAT, typename ARCH::F16>::value && arch.Test( FZ16.Get( arch.FPCR() ) != U32(0) ) ) ||
            ( !std::is_same<FLOAT, typename ARCH::F16>::value && arch.Test( FZ.Get  ( arch.FPCR() ) != U32(0) ) )
          )
        )
      {
        FloatingPointStatusAndControl<FLOAT>::exceptionFlags( (FloatingPointStatusAndControl<FLOAT>::exceptionFlags() & ~flag_inexact) | (arch.Test(is_zero) ? 0 : flag_underflow ) );
        
        FLOAT res = Zeroes( op );
#if DEBUG_FP
        std::cerr << "flush-to-zero (output): " << op << " (0x" << std::hex << ToPacked(op) << std::dec << ") -> " << res << " (0x" << std::hex << ToPacked( res ) << std::dec << ")" << std::endl;
#endif
        return res;
      }
      
    return op;
  }

  template <typename FLOAT, typename ARCH>
  void CheckAlternativeHalfFloatPrecision( ARCH& arch)
  {
    typedef typename ARCH::U32 U32;
    if ( std::is_same<FLOAT, typename ARCH::F16>::value && arch.Test( AHP.Get( arch.FPCR() ) != U32(0) ) )
      {
        struct AlternativeHalfPrecisionFormatUnsupported : std::runtime_error
        {
          AlternativeHalfPrecisionFormatUnsupported() : std::runtime_error("Alternative half-precision format is unsupported") {}
        };
        
        throw AlternativeHalfPrecisionFormatUnsupported();
      }
  }
  
  template <typename ARCH, typename operT> operT FPRound( ARCH& arch, operT op )
  {
#if DEBUG_FP
    std::cerr << "output: " << op << " (0x" << std::hex << ToPacked(op) << std::dec << ")" << std::endl;
#endif
    operT res = FPProcessDenormalOutput( arch, op );

    FPProcessExceptionFlags( arch, FloatingPointStatusAndControl<operT>::exceptionFlags() );
    
    return arch.Test( IsNaN( res ) ) ? FAbs( FPNaN( arch, res ) ) : res;
  }

  template <typename ARCH, typename RMODE, typename FLOAT>
  FLOAT FPRoundInt(ARCH& arch, RMODE&& rmode, FLOAT op, bool exact)
  {
    CheckAlternativeHalfFloatPrecision<FLOAT>( arch );
#if DEBUG_FP
    std::cerr << "FPRoundInt" << std::endl;
    std::cerr << "input: " << op << " (0x" << std::hex << ToPacked(op) << std::dec << ")" << std::endl;
    std::cerr << "FPCR=0x" << std::hex << arch.FPCR() << std::dec << std::endl;
#endif
    FLOAT _op = FPProcessDenormalInput( arch, op );
    
    if (auto nan = FPProcessNaNs(arch, {op}, /* fpexc */ true))
      return nan;

    FloatingPointStatusAndControl<FLOAT>::exceptionFlags(0);
    FLOAT res = rmode.roundint(_op, exact);
#if DEBUG_FP
    std::cerr << "output: " << res << " (0x" << std::hex << ToPacked(res) << std::dec << ")" << std::endl;
#endif

    FPProcessExceptionFlags( arch, FloatingPointStatusAndControl<FLOAT>::exceptionFlags() );

    return res;
  }

  template <typename INTEGER, typename ARCH, typename RMODE, typename FLOAT>
  INTEGER FPToInt(ARCH& arch, RMODE&& rmode, FLOAT op, bool exact)
  {
    CheckAlternativeHalfFloatPrecision<FLOAT>( arch );
#if DEBUG_FP
    std::cerr << "FPToInt" << std::endl;
    std::cerr << "input: " << op << " (0x" << std::hex << ToPacked(op) << std::dec << ")" << std::endl;
    std::cerr << "FPCR=0x" << std::hex << arch.FPCR() << std::dec << std::endl;
#endif
    FLOAT _op = FPProcessDenormalInput( arch, op );

    FPProcessNaNsSignaling(arch, {op}, /* fpexc */ true);
    
    FloatingPointStatusAndControl<FLOAT>::exceptionFlags(0);
    INTEGER res = rmode.template toint<INTEGER>(_op, exact);
#if DEBUG_FP
    std::cerr << "output: " << res << std::endl;
#endif

    FPProcessExceptionFlags( arch, FloatingPointStatusAndControl<FLOAT>::exceptionFlags() );

    return res;
  }
  
  template <typename INTEGER, typename ARCH, typename FLOAT>
  INTEGER FPToFixedRoundTowardZero(ARCH& arch, FLOAT op, unsigned fbits, bool exact)
  {
    CheckAlternativeHalfFloatPrecision<FLOAT>( arch );
#if DEBUG_FP
    std::cerr << "FPToFixedRoundTowardZero" << std::endl;
    std::cerr << "input: " << op << " (0x" << std::hex << ToPacked(op) << std::dec << ")" << std::endl;
    std::cerr << "fbits: " << fbits << std::endl;
    std::cerr << "FPCR=0x" << std::hex << arch.FPCR() << std::dec << std::endl;
#endif
     FLOAT _op = FPProcessDenormalInput( arch, op );

    FPProcessNaNsSignaling(arch, {op}, /* fpexc */ true);
    
    FloatingPointStatusAndControl<FLOAT>::exceptionFlags(0);
    FloatingPointStatusAndControl<FLOAT>::roundingMode(round_minMag);
    
    INTEGER res;
    if ( arch.Test( IsNaN( op ) ) )
      {
        res = INTEGER();
      }
    else
      {
        typedef typename ARCH::F64 F64;
        F64 scale(uint64_t(1) << (fbits - 1));
        scale *= F64(2);
#if DEBUG_FP
        std::cerr << "scale: " << scale << " (0x" << std::hex << ToPacked(scale) << std::dec << ")" << std::endl;
#endif
        F64 scaled(F64(_op) * scale);
#if DEBUG_FP
    std::cerr << "scaled: " << scaled << " (0x" << std::hex << ToPacked(scaled) << std::dec << ")" << std::endl;
#endif
        FloatingPointStatusAndControl<FLOAT>::exceptionFlags(0);
        res = ToIntRoundMinMag<INTEGER>(scaled, exact);
      }
#if DEBUG_FP
    std::cerr << "output: " << res << std::endl;
#endif

    FPProcessExceptionFlags( arch, FloatingPointStatusAndControl<FLOAT>::exceptionFlags() );

    return res;
  }
  
  template <typename FLOAT, typename ARCH, typename INTEGER>
  FLOAT IntToFP(ARCH& arch, INTEGER op)
  {
    CheckAlternativeHalfFloatPrecision<FLOAT>( arch );
#if DEBUG_FP
    std::cerr << "IntToFP" << std::endl;
    std::cerr << "input: " << op << std::endl;
    std::cerr << "FPCR=0x" << std::hex << arch.FPCR() << std::dec << std::endl;
#endif
    typedef typename ARCH::U32 U32;
    FloatingPointStatusAndControl<FLOAT>::defaultNaN( arch.Test( DN.Get( arch.FPCR() ) != U32(0) ) );
    FloatingPointStatusAndControl<FLOAT>::exceptionFlags(0);
    FloatingPointStatusAndControl<FLOAT>::roundingMode( RoundingMode( arch ).roundingMode() );
    FloatingPointStatusAndControl<FLOAT>::detectTininess( tininess_beforeRounding );
    FLOAT res = FLOAT(op);
#if DEBUG_FP
    std::cerr << "output: " << res << " (0x" << std::hex << ToPacked(res) << std::dec << ")" << std::endl;
#endif

    FPProcessExceptionFlags( arch, FloatingPointStatusAndControl<FLOAT>::exceptionFlags() );

    return res;
  }
  
  template <typename FLOAT, typename ARCH, typename INTEGER>
  FLOAT FixedToFP(ARCH& arch, INTEGER op, unsigned fbits)
  {
    CheckAlternativeHalfFloatPrecision<FLOAT>( arch );
#if DEBUG_FP
    std::cerr << "FixedToFP" << std::endl;
    std::cerr << "input: " << op << std::endl;
    std::cerr << "fbits:" << fbits << std::endl;
    std::cerr << "FPCR=0x" << std::hex << arch.FPCR() << std::dec << std::endl;
#endif
    typedef typename ARCH::F64 F64;
    typedef typename ARCH::U32 U32;
    FloatingPointStatusAndControl<FLOAT>::defaultNaN( arch.Test( DN.Get( arch.FPCR() ) != U32(0) ) );
    FloatingPointStatusAndControl<FLOAT>::exceptionFlags(0);
    FloatingPointStatusAndControl<FLOAT>::roundingMode( RoundingMode( arch ).roundingMode() );
    FloatingPointStatusAndControl<FLOAT>::detectTininess( tininess_beforeRounding );
    FloatingPointStatusAndControl<FLOAT>::flushToZero( FlushToZero<FLOAT>( arch ).flushToZero() );
    F64 scale(uint64_t(1) << (fbits - 1));
    scale *= F64(2);
#if DEBUG_FP
    std::cerr << "scale: " << scale << " (0x" << std::hex << ToPacked(scale) << std::dec << ")" << std::endl;
#endif
    FLOAT res( F64(op) / scale );
    return FPRound(arch, res);
  }
  
  template <typename DST, typename ARCH, typename SRC>
  DST FPConvert( ARCH& arch, SRC op )
  {
    typedef typename ARCH::U32 U32;
    CheckAlternativeHalfFloatPrecision<SRC>( arch );
    CheckAlternativeHalfFloatPrecision<DST>( arch );
#if DEBUG_FP
    std::cerr << "FPConvert" << std::endl;
    std::cerr << "input: " << op << " (0x" << std::hex << ToPacked(op) << std::dec << ")" << std::endl;
    std::cerr << "FPCR=0x" << std::hex << arch.FPCR() << std::dec << std::endl;
#endif
    SRC _op = std::is_same<SRC, typename ARCH::F16>::value ? op : FPProcessDenormalInput( arch, op );
    
    typedef typename ARCH::U32 U32;
    FloatingPointStatusAndControl<DST>::defaultNaN( arch.Test( DN.Get( arch.FPCR() ) != U32(0) ) );
    FloatingPointStatusAndControl<DST>::exceptionFlags(0);
    FloatingPointStatusAndControl<DST>::roundingMode( RoundingMode( arch ).roundingMode() );
    FloatingPointStatusAndControl<DST>::detectTininess( tininess_beforeRounding );
    FloatingPointStatusAndControl<DST>::flushToZero( (std::is_same<SRC, typename ARCH::F16>::value or std::is_same<DST, typename ARCH::F16>::value) ? ftz_never : FlushToZero<DST>( arch ).flushToZero() );
    
    if (FPProcessNaNs(arch, {op}, /* fpexc */ true))
    {
      return FPNaN(arch, FConvert<DST>(op));
    }
    
    DST _res = FConvert<DST>(_op);
    DST res = (std::is_same<SRC, typename ARCH::F16>::value or std::is_same<DST, typename ARCH::F16>::value) ? _res : FPProcessDenormalOutput( arch, _res );
#if DEBUG_FP
    std::cerr << "output: " << res << " (0x" << std::hex << ToPacked(res) << std::dec << ")" << std::endl;
#endif
    FPProcessExceptionFlags( arch, FloatingPointStatusAndControl<DST>::exceptionFlags() );
    return arch.Test( IsNaN( res ) ) ? FAbs( FPNaN( arch, res ) ) : res;
  }

  template <typename DST, typename ARCH, typename SRC>
  DST FPConvertRoundOdd( ARCH& arch, SRC op )
  {
    typedef typename ARCH::U32 U32;
    CheckAlternativeHalfFloatPrecision<SRC>( arch );
    CheckAlternativeHalfFloatPrecision<DST>( arch );
#if DEBUG_FP
    std::cerr << "FPConvertRoundOdd" << std::endl;
    std::cerr << "input: " << op << " (0x" << std::hex << ToPacked(op) << std::dec << ")" << std::endl;
    std::cerr << "FPCR=0x" << std::hex << arch.FPCR() << std::dec << std::endl;
#endif
    SRC _op = std::is_same<SRC, typename ARCH::F16>::value ? op : FPProcessDenormalInput( arch, op );
    
    typedef typename ARCH::U32 U32;
    FloatingPointStatusAndControl<DST>::defaultNaN( arch.Test( DN.Get( arch.FPCR() ) != U32(0) ) );
    FloatingPointStatusAndControl<DST>::exceptionFlags(0);
    FloatingPointStatusAndControl<DST>::roundingMode( round_odd );
    FloatingPointStatusAndControl<DST>::detectTininess( tininess_beforeRounding );
    FloatingPointStatusAndControl<DST>::flushToZero( (std::is_same<SRC, typename ARCH::F16>::value or std::is_same<DST, typename ARCH::F16>::value) ? ftz_never : FlushToZero<DST>( arch ).flushToZero() );
    
    if (FPProcessNaNs(arch, {op}, /* fpexc */ true))
    {
      return FPNaN(arch, FConvert<DST>(op));
    }
    
    DST _res = FConvert<DST>(_op);
    DST res = (std::is_same<SRC, typename ARCH::F16>::value or std::is_same<DST, typename ARCH::F16>::value) ? _res : FPProcessDenormalOutput( arch, _res);
#if DEBUG_FP
    std::cerr << "output: " << res << " (0x" << std::hex << ToPacked(res) << std::dec << ")" << std::endl;
#endif
    FPProcessExceptionFlags( arch, FloatingPointStatusAndControl<DST>::exceptionFlags() );
    return arch.Test( IsNaN( res ) ) ? FAbs( FPNaN( arch, res ) ) : res;
  }
  
  template <typename operT, typename ARCH>
  operT FPSub( ARCH& arch, operT op1, operT op2 )
  {
    CheckAlternativeHalfFloatPrecision<operT>( arch );
#if DEBUG_FP
    std::cerr << "FPSub" << std::endl;
    std::cerr << "input1: " << op1 << " (0x" << std::hex << ToPacked(op1) << std::dec << ")" << std::endl;
    std::cerr << "input2: " << op2 << " (0x" << std::hex << ToPacked(op2) << std::dec << ")" << std::endl;
    std::cerr << "FPCR=0x" << std::hex << arch.FPCR() << std::dec << std::endl;
#endif
    operT _op1 = FPProcessDenormalInput( arch, op1 );
    operT _op2 = FPProcessDenormalInput( arch, op2 );
    if (auto nan = FPProcessNaNs(arch, {op1, op2}, /* fpexc */ true))
      {
#if DEBUG_FP
        std::cerr << "output: " << operT(nan) << " (0x" << std::hex << ToPacked(operT(nan)) << std::dec << ")" << std::endl;
#endif
        return nan;
      }

    typedef typename ARCH::U32 U32;
    FloatingPointStatusAndControl<operT>::defaultNaN( arch.Test( DN.Get( arch.FPCR() ) != U32(0) ) );
    FloatingPointStatusAndControl<operT>::exceptionFlags(0);
    FloatingPointStatusAndControl<operT>::roundingMode( RoundingMode( arch ).roundingMode() );
    FloatingPointStatusAndControl<operT>::detectTininess( tininess_beforeRounding );
    FloatingPointStatusAndControl<operT>::flushToZero( FlushToZero<operT>( arch ).flushToZero() );
    return FPRound(arch, _op1 - _op2 );
  }

  template <typename operT, typename ARCH>
  operT FPAdd( ARCH& arch, operT op1, operT op2 )
  {
    CheckAlternativeHalfFloatPrecision<operT>( arch );
#if DEBUG_FP
    std::cerr << "FPAdd" << std::endl;
    std::cerr << "input1: " << op1 << " (0x" << std::hex << ToPacked(op1) << std::dec << ")" << std::endl;
    std::cerr << "input2: " << op2 << " (0x" << std::hex << ToPacked(op2) << std::dec << ")" << std::endl;
    std::cerr << "FPCR=0x" << std::hex << arch.FPCR() << std::dec << std::endl;
#endif
    operT _op1 = FPProcessDenormalInput( arch, op1 );
    operT _op2 = FPProcessDenormalInput( arch, op2 );
    if (auto nan = FPProcessNaNs(arch, {op1, op2}, /* fpexc */ true))
      {
#if DEBUG_FP
        std::cerr << "output: " << operT(nan) << " (0x" << std::hex << ToPacked(operT(nan)) << std::dec << ")" << std::endl;
#endif
        return nan;
      }

    typedef typename ARCH::U32 U32;
    FloatingPointStatusAndControl<operT>::defaultNaN( arch.Test( DN.Get( arch.FPCR() ) != U32(0) ) );
    FloatingPointStatusAndControl<operT>::exceptionFlags(0);
    FloatingPointStatusAndControl<operT>::roundingMode( RoundingMode( arch ).roundingMode() );
    FloatingPointStatusAndControl<operT>::detectTininess( tininess_beforeRounding );
    FloatingPointStatusAndControl<operT>::flushToZero( FlushToZero<operT>( arch ).flushToZero() );
    return FPRound(arch, _op1 + _op2 );
  }

  template <typename operT, typename ARCH>
  operT FPMul( ARCH& arch, operT op1, operT op2 )
  {
    CheckAlternativeHalfFloatPrecision<operT>( arch );
#if DEBUG_FP
    std::cerr << "FPMul" << std::endl;
    std::cerr << "input1: " << op1 << " (0x" << std::hex << ToPacked(op1) << std::dec << ")" << std::endl;
    std::cerr << "input2: " << op2 << " (0x" << std::hex << ToPacked(op2) << std::dec << ")" << std::endl;
    std::cerr << "FPCR=0x" << std::hex << arch.FPCR() << std::dec << std::endl;
#endif
    operT _op1 = FPProcessDenormalInput( arch, op1 );
    operT _op2 = FPProcessDenormalInput( arch, op2 );
    if (auto nan = FPProcessNaNs(arch, {op1, op2}, /* fpexc */ true))
      {
#if DEBUG_FP
        std::cerr << "output: " << operT(nan) << " (0x" << std::hex << ToPacked(operT(nan)) << std::dec << ")" << std::endl;
#endif
        return nan;
      }

    typedef typename ARCH::U32 U32;
    FloatingPointStatusAndControl<operT>::defaultNaN( arch.Test( DN.Get( arch.FPCR() ) != U32(0) ) );
    FloatingPointStatusAndControl<operT>::exceptionFlags(0);
    FloatingPointStatusAndControl<operT>::roundingMode( RoundingMode( arch ).roundingMode() );
    FloatingPointStatusAndControl<operT>::detectTininess( tininess_beforeRounding );
    FloatingPointStatusAndControl<operT>::flushToZero( FlushToZero<operT>( arch ).flushToZero() );
    return FPRound(arch, _op1 * _op2 );
  }

  template <typename operT, typename ARCH>
  operT FPMulX( ARCH& arch, operT op1, operT op2 )
  {
    CheckAlternativeHalfFloatPrecision<operT>( arch );
#if DEBUG_FP
    std::cerr << "FPMulX" << std::endl;
    std::cerr << "input1: " << op1 << " (0x" << std::hex << ToPacked(op1) << std::dec << ")" << std::endl;
    std::cerr << "input2: " << op2 << " (0x" << std::hex << ToPacked(op2) << std::dec << ")" << std::endl;
    std::cerr << "FPCR=0x" << std::hex << arch.FPCR() << std::dec << std::endl;
#endif
    operT _op1 = FPProcessDenormalInput( arch, op1 );
    operT _op2 = FPProcessDenormalInput( arch, op2 );
    if (auto nan = FPProcessNaNs(arch, {op1, op2}, /* fpexc */ true))
      {
#if DEBUG_FP
        std::cerr << "output: " << operT(nan) << " (0x" << std::hex << ToPacked(operT(nan)) << std::dec << ")" << std::endl;
#endif
        return nan;
      }

    typedef typename ARCH::U32 U32;
    FloatingPointStatusAndControl<operT>::defaultNaN( arch.Test( DN.Get( arch.FPCR() ) != U32(0) ) );
    FloatingPointStatusAndControl<operT>::exceptionFlags(0);
    FloatingPointStatusAndControl<operT>::roundingMode( RoundingMode( arch ).roundingMode() );
    FloatingPointStatusAndControl<operT>::detectTininess( tininess_beforeRounding );
    FloatingPointStatusAndControl<operT>::flushToZero( FlushToZero<operT>( arch ).flushToZero() );
    
    operT res;
    
    if ( arch.Test( ( IsInf( _op1 ) && IsZero( _op2 ) ) || ( IsZero( _op1 ) && IsInf( _op2 ) ) ) )
      {
        res = FromUnpacked<operT>( { /* sign */ arch.Test(IsNeg(_op1) ^ IsNeg(_op2)), /* exp */ 1, /* frac */ { 0, 0 } } );
      }
    else
      {
        res = _op1 * _op2;
      }
      
    return FPRound(arch, res );
  }
  
  template <typename operT, typename ARCH>
  operT FPDiv( ARCH& arch, operT op1, operT op2 )
  {
    CheckAlternativeHalfFloatPrecision<operT>( arch );
#if DEBUG_FP
    std::cerr << "FPDiv" << std::endl;
    std::cerr << "input1: " << op1 << " (0x" << std::hex << ToPacked(op1) << std::dec << ")" << std::endl;
    std::cerr << "input2: " << op2 << " (0x" << std::hex << ToPacked(op2) << std::dec << ")" << std::endl;
    std::cerr << "FPCR=0x" << std::hex << arch.FPCR() << std::dec << std::endl;
#endif
    operT _op1 = FPProcessDenormalInput( arch, op1 );
    operT _op2 = FPProcessDenormalInput( arch, op2 );
    if (auto nan = FPProcessNaNs(arch, {op1, op2}, /* fpexc */ true))
      {
#if DEBUG_FP
        std::cerr << "output: " << operT(nan) << " (0x" << std::hex << ToPacked(operT(nan)) << std::dec << ")" << std::endl;
#endif
        return nan;
      }

    typedef typename ARCH::U32 U32;
    FloatingPointStatusAndControl<operT>::defaultNaN( arch.Test( DN.Get( arch.FPCR() ) != U32(0) ) );
    FloatingPointStatusAndControl<operT>::exceptionFlags(0);
    FloatingPointStatusAndControl<operT>::roundingMode( RoundingMode( arch ).roundingMode() );
    FloatingPointStatusAndControl<operT>::detectTininess( tininess_beforeRounding );
    FloatingPointStatusAndControl<operT>::flushToZero( FlushToZero<operT>( arch ).flushToZero() );
    return FPRound(arch, _op1 / _op2 );
  }
  
  template <typename operT, typename ARCH>
  operT FPMulAdd( ARCH& arch, operT op1, operT op2, operT op3 )
  {
    CheckAlternativeHalfFloatPrecision<operT>( arch );
#if DEBUG_FP
    std::cerr << "FPMulAdd" << std::endl;
    std::cerr << "input1: " << op1 << " (0x" << std::hex << ToPacked(op1) << std::dec << ")" << std::endl;
    std::cerr << "input2: " << op2 << " (0x" << std::hex << ToPacked(op2) << std::dec << ")" << std::endl;
    std::cerr << "input3: " << op3 << " (0x" << std::hex << ToPacked(op3) << std::dec << ")" << std::endl;
    std::cerr << "FPCR=0x" << std::hex << arch.FPCR() << std::dec << std::endl;
#endif
    operT _op1 = FPProcessDenormalInput( arch, op1 );
    operT _op2 = FPProcessDenormalInput( arch, op2 );
    operT _op3 = FPProcessDenormalInput( arch, op3 );
    auto nan = FPProcessNaNs(arch, {op1,op2,op3}, /* fpexc */ true);
      
    if ( arch.Test( IsNaN( op1 ) && !IsSignaling( op1 ) && // is quiet NaN
         ( ( IsInf( _op2 ) && IsZero( _op3 ) ) || ( IsZero( _op2 ) && IsInf( _op3 ) ) ) ) )
      {
#if DEBUG_FP
        std::cerr << "IOC" << std::endl;
#endif
        FPProcessException( arch, IOC );
        operT nan = DefaultNaN<operT>();
#if DEBUG_FP
        std::cerr << "output: " << operT(nan) << " (0x" << std::hex << ToPacked(operT(nan)) << std::dec << ")" << std::endl;
#endif
        return nan;
      }
    else if ( nan )
      {
#if DEBUG_FP
        std::cerr << "output: " << operT(nan) << " (0x" << std::hex << ToPacked(operT(nan)) << std::dec << ")" << std::endl;
#endif
        return nan;
      }

    typedef typename ARCH::U32 U32;
    FloatingPointStatusAndControl<operT>::defaultNaN( arch.Test( DN.Get( arch.FPCR() ) != U32(0) ) );
    FloatingPointStatusAndControl<operT>::exceptionFlags(0);
    FloatingPointStatusAndControl<operT>::roundingMode( RoundingMode( arch ).roundingMode() );
    FloatingPointStatusAndControl<operT>::detectTininess( tininess_beforeRounding );
    FloatingPointStatusAndControl<operT>::flushToZero( FlushToZero<operT>( arch ).flushToZero() );
    return FPRound(arch, FMulAdd(_op2, _op3, _op1 ) ); // (op2 * op3) + op1
  }
  
  template <typename operT, typename ARCH>
  operT FPMin( ARCH& arch, operT op1, operT op2 )
  {
#if DEBUG_FP
    std::cerr << "FPMin" << std::endl;
    std::cerr << "input1: " << op1 << " (0x" << std::hex << ToPacked(op1) << std::dec << ")" << std::endl;
    std::cerr << "input2: " << op2 << " (0x" << std::hex << ToPacked(op2) << std::dec << ")" << std::endl;
    std::cerr << "FPCR=0x" << std::hex << arch.FPCR() << std::dec << std::endl;
#endif
    operT _op1 = FPProcessDenormalInput( arch, op1 );
    operT _op2 = FPProcessDenormalInput( arch, op2 );
    if (auto nan = FPProcessNaNs(arch, {op1, op2}, /* fpexc */ true))
      {
#if DEBUG_FP
        std::cerr << "output: " << operT(nan) << " (0x" << std::hex << ToPacked(operT(nan)) << std::dec << ")" << std::endl;
#endif
        return nan;
      }

    typedef typename ARCH::U32 U32;
    FloatingPointStatusAndControl<operT>::defaultNaN( arch.Test( DN.Get( arch.FPCR() ) != U32(0) ) );
    FloatingPointStatusAndControl<operT>::exceptionFlags(0);
    FloatingPointStatusAndControl<operT>::roundingMode( RoundingMode( arch ).roundingMode() );
    FloatingPointStatusAndControl<operT>::detectTininess( tininess_beforeRounding );
    operT res  = FPRound( arch, FMin(_op1, _op2) );
    return res;
  }

  template <typename operT, typename ARCH>
  operT FPMinNumber( ARCH& arch, operT op1, operT op2 )
  {
#if DEBUG_FP
    std::cerr << "FPMinNumber" << std::endl;
    std::cerr << "input1: " << op1 << " (0x" << std::hex << ToPacked(op1) << std::dec << ")" << std::endl;
    std::cerr << "input2: " << op2 << " (0x" << std::hex << ToPacked(op2) << std::dec << ")" << std::endl;
    std::cerr << "FPCR=0x" << std::hex << arch.FPCR() << std::dec << std::endl;
#endif
    operT _op1 = FPProcessDenormalInput( arch, op1 );
    operT _op2 = FPProcessDenormalInput( arch, op2 );
    if (auto nan = FPProcessSNaNs(arch, {op1, op2}, /* fpexc */ true))
      {
#if DEBUG_FP
        std::cerr << "output: " << operT(nan) << " (0x" << std::hex << ToPacked(operT(nan)) << std::dec << ")" << std::endl;
#endif
        return nan;
      }

    typedef typename ARCH::U32 U32;
    FloatingPointStatusAndControl<operT>::defaultNaN( arch.Test( DN.Get( arch.FPCR() ) != U32(0) ) );
    FloatingPointStatusAndControl<operT>::exceptionFlags(0);
    FloatingPointStatusAndControl<operT>::roundingMode( RoundingMode( arch ).roundingMode() );
    FloatingPointStatusAndControl<operT>::detectTininess( tininess_beforeRounding );
    operT res = FMinNumber(_op1, _op2);
    return arch.Test( IsNaN( res ) ) ? FPNaN( arch, res ) : res;
  }
  
  template <typename operT, typename ARCH>
  operT FPMax( ARCH& arch, operT op1, operT op2 )
  {
#if DEBUG_FP
    std::cerr << "FPMax" << std::endl;
    std::cerr << "input1: " << op1 << " (0x" << std::hex << ToPacked(op1) << std::dec << ")" << std::endl;
    std::cerr << "input2: " << op2 << " (0x" << std::hex << ToPacked(op2) << std::dec << ")" << std::endl;
    std::cerr << "FPCR=0x" << std::hex << arch.FPCR() << std::dec << std::endl;
#endif
    operT _op1 = FPProcessDenormalInput( arch, op1 );
    operT _op2 = FPProcessDenormalInput( arch, op2 );
    if (auto nan = FPProcessNaNs(arch, {op1, op2}, /* fpexc */ true))
      {
#if DEBUG_FP
        std::cerr << "output: " << operT(nan) << " (0x" << std::hex << ToPacked(operT(nan)) << std::dec << ")" << std::endl;
#endif
        return nan;
      }

    typedef typename ARCH::U32 U32;
    FloatingPointStatusAndControl<operT>::defaultNaN( arch.Test( DN.Get( arch.FPCR() ) != U32(0) ) );
    FloatingPointStatusAndControl<operT>::exceptionFlags(0);
    FloatingPointStatusAndControl<operT>::roundingMode( RoundingMode( arch ).roundingMode() );
    FloatingPointStatusAndControl<operT>::detectTininess( tininess_beforeRounding );
    operT res  = FPRound( arch, FMax(_op1, _op2) );
    return res;
  }
  
  template <typename operT, typename ARCH>
  operT FPMaxNumber( ARCH& arch, operT op1, operT op2 )
  {
#if DEBUG_FP
    std::cerr << "FPMaxNumber" << std::endl;
    std::cerr << "input1: " << op1 << " (0x" << std::hex << ToPacked(op1) << std::dec << ")" << std::endl;
    std::cerr << "input2: " << op2 << " (0x" << std::hex << ToPacked(op2) << std::dec << ")" << std::endl;
    std::cerr << "FPCR=0x" << std::hex << arch.FPCR() << std::dec << std::endl;
#endif
    operT _op1 = FPProcessDenormalInput( arch, op1 );
    operT _op2 = FPProcessDenormalInput( arch, op2 );
    if (auto nan = FPProcessSNaNs(arch, {op1, op2}, /* fpexc */ true))
      {
#if DEBUG_FP
        std::cerr << "output: " << operT(nan) << " (0x" << std::hex << ToPacked(operT(nan)) << std::dec << ")" << std::endl;
#endif
        return nan;
      }

    typedef typename ARCH::U32 U32;
    FloatingPointStatusAndControl<operT>::defaultNaN( arch.Test( DN.Get( arch.FPCR() ) != U32(0) ) );
    FloatingPointStatusAndControl<operT>::exceptionFlags(0);
    FloatingPointStatusAndControl<operT>::roundingMode( RoundingMode( arch ).roundingMode() );
    FloatingPointStatusAndControl<operT>::detectTininess( tininess_beforeRounding );
    operT res = FMaxNumber(_op1, _op2);
    return arch.Test( IsNaN( res ) ) ? FPNaN( arch, res ) : res;
  }
  
  template <typename operT, typename ARCH>
  void FPQuietCompare( ARCH& arch, operT op1, operT op2 )
  {
    typedef typename ARCH::BOOL BOOL;
#if DEBUG_FP
    std::cerr << "FPQuietCompare" << std::endl;
    std::cerr << "input1: " << op1 << " (0x" << std::hex << ToPacked(op1) << std::dec << ")" << std::endl;
    std::cerr << "input2: " << op2 << " (0x" << std::hex << ToPacked(op2) << std::dec << ")" << std::endl;
#endif
    operT _op1 = FPProcessDenormalInput( arch, op1 );
    operT _op2 = FPProcessDenormalInput( arch, op2 );
    
    BOOL n, z, c, v;
    if ( arch.Test( IsNaN( op1 ) || IsNaN( op2 ) ) )
      {
        n = BOOL(0); z = BOOL(0); c = BOOL(1); v = BOOL(1);
        
        FPProcessSNaNs(arch, {op1, op2}, /* fpexc */ true);
      }
    else
      {
        BOOL le(_op1 <= _op2), ge(_op1 >= _op2);
        n = not ge and le; z = ge and le; c = ge or not le; v = not le and not ge;
      }
#if DEBUG_FP
    std::cerr << "output: NZCV=" << n << z << c << v << std::endl;
#endif
    arch.SetNZCV( n, z, c, v );
  }
  
  template <typename operT, typename ARCH>
  void FPSignalingCompare( ARCH& arch, operT op1, operT op2 )
  {
    typedef typename ARCH::BOOL BOOL;
#if DEBUG_FP
    std::cerr << "FPSignalingCompare" << std::endl;
    std::cerr << "input1: " << op1 << " (0x" << std::hex << ToPacked(op1) << std::dec << ")" << std::endl;
    std::cerr << "input2: " << op2 << " (0x" << std::hex << ToPacked(op2) << std::dec << ")" << std::endl;
#endif
    operT _op1 = FPProcessDenormalInput( arch, op1 );
    operT _op2 = FPProcessDenormalInput( arch, op2 );
    
    BOOL n, z, c, v;
    if ( arch.Test( IsNaN( op1 ) || IsNaN( op2 ) ) )
      {
        n = BOOL(0); z = BOOL(0); c = BOOL(1); v = BOOL(1);
        
        FPProcessNaNsSignaling(arch, {op1, op2}, /* fpexc */ true);
      }
    else
      {
        BOOL le(_op1 <= _op2), ge(_op1 >= _op2);
        n = not ge and le; z = ge and le; c = ge or not le; v = not le and not ge;
      }
#if DEBUG_FP
    std::cerr << "output: NZCV=" << n << z << c << v << std::endl;
#endif
    arch.SetNZCV( n, z, c, v );
   }

  template <typename operT, typename ARCH>
  typename ARCH::BOOL FPCompareEq( ARCH& arch, operT op1, operT op2 )
  {
#if DEBUG_FP
    std::cerr << "FPCompareEq" << std::endl;
    std::cerr << "input1: " << op1 << " (0x" << std::hex << ToPacked(op1) << std::dec << ")" << std::endl;
    std::cerr << "input2: " << op2 << " (0x" << std::hex << ToPacked(op2) << std::dec << ")" << std::endl;
#endif
    operT _op1 = FPProcessDenormalInput( arch, op1 );
    operT _op2 = FPProcessDenormalInput( arch, op2 );
    
    if ( arch.Test( IsNaN( op1 ) || IsNaN( op2 ) ) )
      {
        FPProcessSNaNs(arch, {op1, op2}, /* fpexc */ true);
        
        return typename ARCH::BOOL();
      }
    else
      {
        return _op1 == _op2;
      }
  }

  template <typename operT, typename ARCH>
  typename ARCH::BOOL FPCompareGe( ARCH& arch, operT op1, operT op2 )
  {
#if DEBUG_FP
    std::cerr << "FPCompareGe" << std::endl;
    std::cerr << "input1: " << op1 << " (0x" << std::hex << ToPacked(op1) << std::dec << ")" << std::endl;
    std::cerr << "input2: " << op2 << " (0x" << std::hex << ToPacked(op2) << std::dec << ")" << std::endl;
#endif
    operT _op1 = FPProcessDenormalInput( arch, op1 );
    operT _op2 = FPProcessDenormalInput( arch, op2 );
    
    if ( arch.Test( IsNaN( op1 ) || IsNaN( op2 )  ) )
      {
        FPProcessNaNsSignaling(arch, {op1, op2}, /* fpexc */ true);
        
        return typename ARCH::BOOL();
      }
    else
      {
        return _op1 >= _op2;
      }
  }

  template <typename operT, typename ARCH>
  typename ARCH::BOOL FPCompareGt( ARCH& arch, operT op1, operT op2 )
  {
#if DEBUG_FP
    std::cerr << "FPCompareGt" << std::endl;
    std::cerr << "input1: " << op1 << " (0x" << std::hex << ToPacked(op1) << std::dec << ")" << std::endl;
    std::cerr << "input2: " << op2 << " (0x" << std::hex << ToPacked(op2) << std::dec << ")" << std::endl;
#endif
    operT _op1 = FPProcessDenormalInput( arch, op1 );
    operT _op2 = FPProcessDenormalInput( arch, op2 );
    
    if ( arch.Test( IsNaN( op1 ) || IsNaN( op2 ) ) )
      {
        FPProcessNaNsSignaling(arch, {op1, op2}, /* fpexc */ true);
        
        return typename ARCH::BOOL();
      }
    else
      {
        return _op1 > _op2;
      }
  }
  
  template <typename operT, typename ARCH>
  operT FPSqrt( ARCH& arch, operT op )
  {
    CheckAlternativeHalfFloatPrecision<operT>( arch );
#if DEBUG_FP
    std::cerr << "FPSqrt" << std::endl;
    std::cerr << "input: " << op << " (0x" << std::hex << ToPacked(op) << std::dec << ")" << std::endl;
    std::cerr << "FPCR=0x" << std::hex << arch.FPCR() << std::dec << std::endl;
#endif
    operT _op = FPProcessDenormalInput( arch, op );
    if (auto nan = FPProcessNaNs(arch, {op}, /* fpexc */ true))
      {
#if DEBUG_FP
        std::cerr << "output: " << operT(nan) << " (0x" << std::hex << ToPacked(operT(nan)) << std::dec << ")" << std::endl;
#endif
        return nan;
      }

    typedef typename ARCH::U32 U32;
    FloatingPointStatusAndControl<operT>::defaultNaN( arch.Test( DN.Get( arch.FPCR() ) != U32(0) ) );
    FloatingPointStatusAndControl<operT>::exceptionFlags(0);
    FloatingPointStatusAndControl<operT>::roundingMode( RoundingMode( arch ).roundingMode() );
    FloatingPointStatusAndControl<operT>::detectTininess( tininess_beforeRounding );
    FloatingPointStatusAndControl<operT>::flushToZero( FlushToZero<operT>( arch ).flushToZero() );
    return FPRound(arch, FSqrt(_op) );
  }
  
} // end of namespace arm64
} // end of namespace isa
} // end of namespace arm
} // end of namespace processor
} // end of namespace cxx
} // end of namespace component
} // end of namespace unisim

#endif /* __UNISIM_COMPONENT_CXX_PROCESSOR_ARM_ISA_ARM64_EXECUTE_HH__ */

