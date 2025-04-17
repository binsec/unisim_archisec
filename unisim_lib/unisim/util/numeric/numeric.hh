/*
 *  Copyright (c) 2025,
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

#ifndef __UNISIM_UTIL_NUMERIC_NUMERIC_HH__
#define __UNISIM_UTIL_NUMERIC_NUMERIC_HH__

#include <cstdint>

namespace unisim {
namespace util {
namespace numeric {

template <typename T>
struct Numeric : T::numeric_type {};

template <>
struct Numeric<    bool> { enum nfo { is_signed = 0, is_integral = 1, is_floating_point = 0, bitsize = 1 }; };

template <>
struct Numeric< uint8_t> { enum nfo { is_signed = 0, is_integral = 1, is_floating_point = 0, bitsize = 8 }; };
template <>
struct Numeric<uint16_t> { enum nfo { is_signed = 0, is_integral = 1, is_floating_point = 0, bitsize = 16 }; };
template <>
struct Numeric<uint32_t> { enum nfo { is_signed = 0, is_integral = 1, is_floating_point = 0, bitsize = 32 }; };
template <>
struct Numeric<uint64_t> { enum nfo { is_signed = 0, is_integral = 1, is_floating_point = 0, bitsize = 64 }; };

template <>
struct Numeric<  int8_t> { enum nfo { is_signed = 1, is_integral = 1, is_floating_point = 0, bitsize =  8 }; };
template <>
struct Numeric< int16_t> { enum nfo { is_signed = 1, is_integral = 1, is_floating_point = 0, bitsize = 16 }; };
template <>
struct Numeric< int32_t> { enum nfo { is_signed = 1, is_integral = 1, is_floating_point = 0, bitsize = 32 }; };
template <>
struct Numeric< int64_t> { enum nfo { is_signed = 1, is_integral = 1, is_floating_point = 0, bitsize = 64 }; };

template <>
struct Numeric<   float> { enum nfo { is_signed = 1, is_integral = 0, is_floating_point = 1, bitsize = 32 }; };
template <>
struct Numeric<  double> { enum nfo { is_signed = 1, is_integral = 0, is_floating_point = 1, bitsize = 64 }; };

// This is extremely dangerous
template <>
struct Numeric<long double> { enum nfo { is_signed = 1, is_integral = 0, is_floating_point = 1, bitsize = 80 }; };

template <typename T>
struct Integers
{
  static constexpr T min() { return Numeric<T>::is_signed ? T(1) << (Numeric<T>::bitsize - 1) : T(0); }
  static constexpr T max() { return Numeric<T>::is_signed ? min() ^ T(-1) : T(-1); }
};

} // end of namespace numeric
} // end of namespace util
} // end of namespace unisim

#endif

