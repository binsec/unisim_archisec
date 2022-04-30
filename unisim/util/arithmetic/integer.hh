/*
 *  Copyright (c) 2007-2022,
 *  Commissariat a l'Energie Atomique (CEA),
 *  University of Perpignan (UPVD)
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
 
#ifndef __UNISIM_UTIL_ARITHMETIC_INTEGER_HH__
#define __UNISIM_UTIL_ARITHMETIC_INTEGER_HH__

#include <inttypes.h>
#include <unisim/util/inlining/inlining.hh>
#include <limits>

namespace unisim {
namespace util {
namespace arithmetic {

  template <unsigned CELLCOUNT, bool SIGNED>
  struct Integer
  {
    Integer() : cells() {}
    typedef Integer<CELLCOUNT,SIGNED> this_type;
    template <typename T>
    explicit
    Integer(T val)
    {
      for (unsigned idx = 0; idx < CELLCOUNT; ++idx)
        {
          cells[idx] = uint32_t(val);
          val >>= 32;
        }
    }

    explicit operator uint64_t () const { return uint64_t(cells[1]) << 32 | cells[0]; }
    explicit operator int64_t () const { return operator uint64_t(); }
    explicit operator uint32_t () const { return cells[0]; }
    explicit operator int32_t () const { return operator uint32_t(); }

    Integer operator * (this_type const& rhs) const
    {
      Integer result;
      for (unsigned l = 0; l < CELLCOUNT; l++)
        {
          uint64_t sub = 0;
          for (unsigned r = 0; r < CELLCOUNT; r++)
            {
              unsigned d = l+r;
              if (d >= CELLCOUNT) break;
              sub += uint64_t(result.cells[d]) + uint64_t(cells[l]) * uint64_t(rhs.cells[r]);
              result.cells[d] = sub;
              sub >>= 32;
            }
        }
      return result;
    }

    int32_t scell() const { return int32_t(cells[CELLCOUNT-1]); }
    uint32_t vcell(unsigned pos) const { return pos < CELLCOUNT ? cells[pos] : SIGNED ? uint32_t(scell() >> 31): 0; }

    Integer
    operator >> (unsigned amount) const
    {
      Integer result;
      unsigned csh = amount / 32;
      if (unsigned bsh = amount % 32)
        {
          unsigned rsh = 32 - bsh;
          uint32_t cell0, cell1 = vcell(csh);
          for (unsigned d = 0; d < CELLCOUNT; d++)
            {
              cell0 = cell1;
              cell1 = vcell(d+csh+1);
              result.cells[d] = (cell0 >> bsh) + (cell1 << rsh);
            }
        }
      else
        {
          for (unsigned d = 0; d < CELLCOUNT; d++)
            result.cells[d] = vcell(d+csh);
        }
      return result;
    }

    Integer operator >> (int amount) const { return operator>>(unsigned(amount)); }


    int cmp(this_type const& rhs) const
    {
      if (SIGNED)
        {
          if (int64_t delta = int64_t(scell()) - int64_t(rhs.scell()))
            return delta >> 32;
        }
      for (unsigned idx = CELLCOUNT; idx-->0;)
        {
          if (int64_t delta = int64_t(cells[idx]) - int64_t(rhs.cells[idx]))
            return delta >> 32;
        }
      return 0;
    }

    bool operator != (this_type const& rhs) const { return cmp(rhs) != 0; }
    bool operator == (this_type const& rhs) const { return cmp(rhs) == 0; }
    bool operator  < (this_type const& rhs) const { return cmp(rhs)  < 0; }
    bool operator >= (this_type const& rhs) const { return cmp(rhs) >= 0; }
    bool operator <= (this_type const& rhs) const { return cmp(rhs) <= 0; }
    bool operator  > (this_type const& rhs) const { return cmp(rhs)  > 0; }
    
    uint32_t cells[CELLCOUNT];
  };

} // end of namespace arithmetic
} // end of namespace util
} // end of namespace unisim

#endif // __UNISIM_UTIL_ARITHMETIC_INTEGER_HH__
