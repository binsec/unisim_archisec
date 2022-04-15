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

#include "i128.hh"

namespace unisim {
namespace util {
namespace arithmetic {

  int128_t operator- (int128_t const&)
  { throw std::logic_error( "No - operator for int128_t." ); }
  int128_t operator+ (int128_t const&, int128_t const&)
  { throw std::logic_error( "No + operator for int128_t." ); }
  int128_t operator- (int128_t const&, int128_t const&)
  { throw std::logic_error( "No - operator for int128_t." ); }
  int128_t operator* (int128_t const&, int128_t const&)
  { throw std::logic_error( "No * operator for int128_t." ); }
  int128_t operator/ (int128_t const&, int128_t const&)
  { throw std::logic_error( "No / operator for int128_t." ); }
  int128_t operator% (int128_t const&, int128_t const&)
  { throw std::logic_error( "No % operator for int128_t." ); }

  int128_t operator~ (int128_t const&)
  { throw std::logic_error( "No ~ operator for int128_t." ); }

  int128_t operator^ (int128_t const&, int128_t const&)
  { throw std::logic_error( "No ^ operator for int128_t." ); }
  int128_t operator| (int128_t const&, int128_t const&)
  { throw std::logic_error( "No | operator for int128_t." ); }
  int128_t operator& (int128_t const&, int128_t const&)
  { throw std::logic_error( "No & operator for int128_t." ); }

  int128_t operator<< (int128_t const&, int8_t)
  { throw std::logic_error( "No << operator for int128_t." ); }
  int128_t operator>> (int128_t const&, int8_t)
  { throw std::logic_error( "No >> operator for int128_t." ); }

  bool operator== (int128_t const& x, int128_t const& y)
  { return mpz_cmp(x.value, y.value) == 0; }
  bool operator!= (int128_t const& x, int128_t const& y)
  { return mpz_cmp(x.value, y.value) != 0; }
  bool operator<= (int128_t const& x, int128_t const& y)
  { return mpz_cmp(x.value, y.value) <= 0; }
  bool operator< (int128_t const& x, int128_t const& y)
  { return mpz_cmp(x.value, y.value) < 0; }
  bool operator>= (int128_t const& x, int128_t const& y)
  { return mpz_cmp(x.value, y.value) >= 0; }
  bool operator> (int128_t const& x, int128_t const& y)
  { return mpz_cmp(x.value, y.value) > 0; }

  std::ostream& operator<< (std::ostream& os, int128_t const& v)
  { throw std::logic_error( "No pp operator for int128_t." ); }

} // end of namespace arithmetic
} // end of namespace util
} // end of namespace unisim
