/*
 *  Copyright (c) 2007-2009,
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
 * Authors: Gilles Mouchard (gilles.mouchard@cea.fr)
 *          Yves Lhuillier (yves.lhuillier@cea.fr)
 *          Reda Nouacer (reda.nouacer@cea.fr)
 *          Sylvain Collange (sylvain.collange@univ-perp.fr)
 */

#ifndef __UNISIM_UTIL_ARITHMETIC_I128_T_HH__
#define __UNISIM_UTIL_ARITHMETIC_I128_T_HH__

#include <stdexcept>
#include <inttypes.h>
#include <gmp.h>

namespace unisim {
namespace util {
namespace arithmetic {

  typedef struct I128 int128_t;

  struct I128 {
    mpz_t value;

    I128() {
      mpz_init(value);
    }

    I128( float const& val)
    { throw std::logic_error( "No cast from float to int128_t." ); }
    I128( double const& val)
    { throw std::logic_error( "No cast from double to int128_t." ); }
    I128( long double const& val)
    { throw std::logic_error( "No cast from long double to int128_t." ); }

    I128( uint8_t const& val) { mpz_init(value); mpz_set_ui(value, val); }
    I128( uint16_t const& val) { mpz_init(value); mpz_set_ui(value, val); }
    I128( uint32_t const& val) { mpz_init(value); mpz_set_ui(value, val); }
    I128( uint64_t const& val) { mpz_init(value); mpz_set_ui(value, val); }

    I128( int8_t const& val) { mpz_init(value); mpz_set_si(value, val); }
    I128( int16_t const& val) { mpz_init(value); mpz_set_si(value, val); }
    I128( int32_t const& val) { mpz_init(value); mpz_set_si(value, val); }
    I128( int64_t const& val) { mpz_init(value); mpz_set_si(value, val); }

    I128( int128_t const& val) { mpz_init(value); mpz_set(value, val.value); }

    operator float() const
    { throw std::logic_error( "No cast from int128_t to float." ); }
    operator double() const
    { throw std::logic_error( "No cast from int128_t to double." ); }
    operator long double() const
    { throw std::logic_error( "No cast from int128_t to long double." ); }

    operator bool() const
    { throw std::logic_error( "No cast from int128_t to bool." ); }
    operator uint8_t() const { return mpz_get_ui(this->value); }
    operator uint16_t() const { return mpz_get_ui(this->value); }
    operator uint32_t() const { return mpz_get_ui(this->value); }
    operator uint64_t() const { return mpz_get_ui(this->value); }

    operator int8_t() const { return mpz_get_si(this->value); }
    operator int16_t() const { return mpz_get_si(this->value); }
    operator int32_t() const { return mpz_get_si(this->value); }
    operator int64_t() const { return mpz_get_si(this->value); }

  };

  int128_t operator- (int128_t const&);

  int128_t operator+ (int128_t const&, int128_t const&);
  int128_t operator- (int128_t const&, int128_t const&);
  int128_t operator* (int128_t const&, int128_t const&);
  int128_t operator/ (int128_t const&, int128_t const&);
  int128_t operator% (int128_t const&, int128_t const&);

  int128_t operator~ (int128_t const&);

  int128_t operator^ (int128_t const&, int128_t const&);
  int128_t operator| (int128_t const&, int128_t const&);
  int128_t operator& (int128_t const&, int128_t const&);

  int128_t operator<< (int128_t const&, int8_t);
  int128_t operator>> (int128_t const&, int8_t);

  bool operator== (int128_t const&, int128_t const&);
  bool operator!= (int128_t const&, int128_t const&);
  bool operator<= (int128_t const&, int128_t const&);
  bool operator< (int128_t const&, int128_t const&);
  bool operator>= (int128_t const&, int128_t const&);
  bool operator> (int128_t const&, int128_t const&);

  std::ostream& operator<< (std::ostream&, int128_t const&);


} // end of namespace arithmetic
} // end of namespace util
} // end of namespace unisim

typedef unisim::util::arithmetic::int128_t uint128_t;
typedef unisim::util::arithmetic::int128_t int128_t;

#endif
