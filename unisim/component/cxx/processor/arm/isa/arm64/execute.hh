/*
 *  Copyright (c) 2016,
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

#ifndef __UNISIM_COMPONENT_CXX_PROCESSOR_ARM_ISA_ARM64_EXECUTE_HH__
#define __UNISIM_COMPONENT_CXX_PROCESSOR_ARM_ISA_ARM64_EXECUTE_HH__

#include <inttypes.h>

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

} // end of namespace arm64
} // end of namespace isa
} // end of namespace arm
} // end of namespace processor
} // end of namespace cxx
} // end of namespace component
} // end of namespace unisim

#endif /* __UNISIM_COMPONENT_CXX_PROCESSOR_ARM_ISA_ARM64_EXECUTE_HH__ */

