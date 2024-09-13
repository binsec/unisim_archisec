/*
 *  Copyright (c) 2023,
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
 * Authors: Gilles Mouchard (gilles.mouchard@cea.fr)
 */

#include <unisim/util/floating_point/floating_point.hh>

//==============================================================================
//                            Forward declarations
//==============================================================================

namespace unisim {
namespace util {
namespace floating_point {

// Helper functions for native host floating-point numbers

template <typename INTEGER, typename FLOAT> static INTEGER _ToInt( FLOAT a, uint_fast8_t rmode, bool exact );
template <typename FLOAT> static FLOAT _DefaultNaN();
template <typename FLOAT> static FLOAT _FMulAdd( FLOAT a, FLOAT b, FLOAT c );
template <typename FLOAT> static FLOAT _FMin( FLOAT a, FLOAT b );
template <typename FLOAT> static FLOAT _FMinNumber( FLOAT a, FLOAT b );
template <typename FLOAT> static FLOAT _FMax( FLOAT a, FLOAT b );
template <typename FLOAT> static FLOAT _FMaxNumber( FLOAT a, FLOAT b );

} // end of namespace floating_point
} // end of namespace util
} // end of namespace unisim

//==============================================================================
//                               Definitions
//==============================================================================

#if HAVE_FLOAT16

// Functions for _Float16

template <> _Float16 FConvert<_Float16, float>( float a )
{
	_Float16 res;
	if( IsNaN( a ) )
	{
		UnpackedFloat unpacked;
		ToUnpacked( unpacked, a );
		unpacked.exp = 16;
		unpacked.frac.lo = (unpacked.frac.lo >> (unisim::util::floating_point::FloatingPointTypeInfo<float>::exp_pos - unisim::util::floating_point::FloatingPointTypeInfo<_Float16>::exp_pos)) | unisim::util::floating_point::FloatingPointTypeInfo<_Float16>::quiet_mask;
		unpacked.frac.hi = 0;
		res = FromUnpacked<_Float16>( unpacked );
	}
	else
	{
		res = _Float16( a );
	}
	return res;
}

template <> _Float16 FConvert<_Float16, double>( double a )
{
	_Float16 res;
	if( IsNaN( a ) )
	{
		UnpackedFloat unpacked;
		ToUnpacked( unpacked, a );
		unpacked.exp = 16;
		unpacked.frac.lo = (unpacked.frac.lo >> (unisim::util::floating_point::FloatingPointTypeInfo<double>::exp_pos - unisim::util::floating_point::FloatingPointTypeInfo<_Float16>::exp_pos)) | unisim::util::floating_point::FloatingPointTypeInfo<_Float16>::quiet_mask;
		unpacked.frac.hi = 0;
		res = FromUnpacked<_Float16>( unpacked );
	}
	else
	{
		res = _Float16( a );
	}
	return res;
}

template <> uint16_t ToInt<uint16_t, _Float16>( _Float16 a, uint_fast8_t rmode, bool exact ) { return unisim::util::floating_point::_ToInt<uint16_t, _Float16>( a, rmode, exact ); }
template <> uint32_t ToInt<uint32_t, _Float16>( _Float16 a, uint_fast8_t rmode, bool exact ) { return unisim::util::floating_point::_ToInt<uint32_t, _Float16>( a, rmode, exact ); }
template <> uint64_t ToInt<uint64_t, _Float16>( _Float16 a, uint_fast8_t rmode, bool exact ) { return unisim::util::floating_point::_ToInt<uint64_t, _Float16>( a, rmode, exact ); }
template <> int16_t  ToInt<int16_t , _Float16>( _Float16 a, uint_fast8_t rmode, bool exact ) { return unisim::util::floating_point::_ToInt<int16_t , _Float16>( a, rmode, exact ); }
template <> int32_t  ToInt<int32_t , _Float16>( _Float16 a, uint_fast8_t rmode, bool exact ) { return unisim::util::floating_point::_ToInt<int32_t , _Float16>( a, rmode, exact ); }
template <> int64_t  ToInt<int64_t , _Float16>( _Float16 a, uint_fast8_t rmode, bool exact ) { return unisim::util::floating_point::_ToInt<int64_t , _Float16>( a, rmode, exact ); }

template <> uint16_t ToIntRoundMinMag<uint16_t, _Float16>( _Float16 a, bool exact ) { return ToInt<uint16_t, _Float16>( a, round_minMag, exact); }
template <> uint32_t ToIntRoundMinMag<uint32_t, _Float16>( _Float16 a, bool exact ) { return ToInt<uint32_t, _Float16>( a, round_minMag, exact); }
template <> uint64_t ToIntRoundMinMag<uint64_t, _Float16>( _Float16 a, bool exact ) { return ToInt<uint64_t, _Float16>( a, round_minMag, exact); }
template <> int16_t  ToIntRoundMinMag<int16_t , _Float16>( _Float16 a, bool exact ) { return ToInt<int16_t , _Float16>( a, round_minMag, exact); }
template <> int32_t  ToIntRoundMinMag<int32_t , _Float16>( _Float16 a, bool exact ) { return ToInt<int32_t , _Float16>( a, round_minMag, exact); }
template <> int64_t  ToIntRoundMinMag<int64_t , _Float16>( _Float16 a, bool exact ) { return ToInt<int64_t , _Float16>( a, round_minMag, exact); }

_Float16 RoundToInt( _Float16 a, uint_fast8_t rmode, bool exact ) { return _Float16( RoundToInt( float(a), rmode, exact) ); }
_Float16 FMulAdd( _Float16 a, _Float16 b, _Float16 c) { return unisim::util::floating_point::_FMulAdd<_Float16>(a, b, c); }

_Float16 FRem( _Float16 a, _Float16 b )
{
	int r = fegetround();
	fesetround( FE_TONEAREST );
	_Float16 q = nearbyintf( a / b );
	_Float16 f = a - (q * b);
	fesetround( r );
	return f;
}

_Float16 FSqrt( _Float16 a ) { return sqrtf( a ); }
_Float16 FAbs( _Float16 a ) { return fabs( a ); }
_Float16 FMin( _Float16 a, _Float16 b ) { return unisim::util::floating_point::_FMin<_Float16>( a, b ); }
_Float16 FMinNumber( _Float16 a, _Float16 b ) { return unisim::util::floating_point::_FMinNumber<_Float16>( a, b ); }
_Float16 FMax( _Float16 a, _Float16 b ) { return unisim::util::floating_point::_FMax<_Float16>( a, b ); }
_Float16 FMaxNumber( _Float16 a, _Float16 b ) { return unisim::util::floating_point::_FMaxNumber<_Float16>( a, b ); }

template <> _Float16 DefaultNaN<_Float16>() { return unisim::util::floating_point::_DefaultNaN<_Float16>(); }

#endif

// Functions for float

template <> uint16_t ToInt<uint16_t, float>( float a, uint_fast8_t rmode, bool exact ) { return unisim::util::floating_point::_ToInt<uint16_t, float>( a, rmode, exact ); }
template <> uint32_t ToInt<uint32_t, float>( float a, uint_fast8_t rmode, bool exact ) { return unisim::util::floating_point::_ToInt<uint32_t, float>( a, rmode, exact ); }
template <> uint64_t ToInt<uint64_t, float>( float a, uint_fast8_t rmode, bool exact ) { return unisim::util::floating_point::_ToInt<uint64_t, float>( a, rmode, exact ); }
template <> int16_t  ToInt<int16_t , float>( float a, uint_fast8_t rmode, bool exact ) { return unisim::util::floating_point::_ToInt<int16_t , float>( a, rmode, exact ); }
template <> int32_t  ToInt<int32_t , float>( float a, uint_fast8_t rmode, bool exact ) { return unisim::util::floating_point::_ToInt<int32_t , float>( a, rmode, exact ); }
template <> int64_t  ToInt<int64_t , float>( float a, uint_fast8_t rmode, bool exact ) { return unisim::util::floating_point::_ToInt<int64_t , float>( a, rmode, exact ); }

template <> uint16_t ToIntRoundMinMag<uint16_t, float>( float a, bool exact ) { return ToInt<uint16_t, float>( a, round_minMag, exact); }
template <> uint32_t ToIntRoundMinMag<uint32_t, float>( float a, bool exact ) { return ToInt<uint32_t, float>( a, round_minMag, exact); }
template <> uint64_t ToIntRoundMinMag<uint64_t, float>( float a, bool exact ) { return ToInt<uint64_t, float>( a, round_minMag, exact); }
template <> int16_t  ToIntRoundMinMag<int16_t , float>( float a, bool exact ) { return ToInt<int16_t , float>( a, round_minMag, exact); }
template <> int32_t  ToIntRoundMinMag<int32_t , float>( float a, bool exact ) { return ToInt<int32_t , float>( a, round_minMag, exact); }
template <> int64_t  ToIntRoundMinMag<int64_t , float>( float a, bool exact ) { return ToInt<int64_t , float>( a, round_minMag, exact); }

float RoundToInt( float a, uint_fast8_t rmode, bool exact )
{
	float f;
	switch( rmode )
	{
		case round_near_maxMag:
		{
			f = roundf( a );
			if( exact && ( f != a ) ) feraiseexcept( FE_INEXACT );
			break;
		}
		case round_odd:
		{
			int r = fegetround();
			fesetround( FE_TONEAREST );
			f = ( float( 2.0 ) * nearbyintf( a / float( 2.0 ) ) ) + float( 1.0 );
			if( exact && ( f != a ) ) feraiseexcept( FE_INEXACT );
			fesetround( r );
			break;
		}
		default:
		{
			int r = fegetround();
			switch( rmode )
			{
				case round_near_even: fesetround( FE_TONEAREST  ); break;
				case round_minMag   : fesetround( FE_TOWARDZERO ); break;
				case round_min      : fesetround( FE_DOWNWARD   ); break;
				case round_max      : fesetround( FE_UPWARD     ); break;
			}
			f = nearbyintf( a );
			if( exact && ( f != a ) ) feraiseexcept( FE_INEXACT );
			fesetround( r );
			break;
		}
	}
	return f;
}

float FMulAdd( float a, float b, float c) { return unisim::util::floating_point::_FMulAdd<float>(a, b, c); }

float FRem( float a, float b )
{
	int r = fegetround();
	fesetround( FE_TONEAREST );
	float q = nearbyintf( a / b );
	float f = a - (q * b);
	fesetround( r );
	return f;
}

float FSqrt( float a ) { return sqrtf( a ); }
float FAbs( float a ) { return fabs( a ); }
float FMin( float a, float b ) { return unisim::util::floating_point::_FMin<float>( a, b ); }
float FMinNumber( float a, float b ) { return unisim::util::floating_point::_FMinNumber<float>( a, b ); }
float FMax( float a, float b ) { return unisim::util::floating_point::_FMax<float>( a, b ); }
float FMaxNumber( float a, float b ) { return unisim::util::floating_point::_FMaxNumber<float>( a, b ); }
bool IsSignaling( float a ) { return unisim::util::floating_point::_IsSignaling<float>( a ); }
template <> float DefaultNaN<float>() { return unisim::util::floating_point::_DefaultNaN<float>(); }

// Functions for double

template <> uint16_t ToInt<uint16_t, double>( double a, uint_fast8_t rmode, bool exact ) { return unisim::util::floating_point::_ToInt<uint16_t, double>( a, rmode, exact ); }
template <> uint32_t ToInt<uint32_t, double>( double a, uint_fast8_t rmode, bool exact ) { return unisim::util::floating_point::_ToInt<uint32_t, double>( a, rmode, exact ); }
template <> uint64_t ToInt<uint64_t, double>( double a, uint_fast8_t rmode, bool exact ) { return unisim::util::floating_point::_ToInt<uint64_t, double>( a, rmode, exact ); }
template <> int16_t  ToInt<int16_t , double>( double a, uint_fast8_t rmode, bool exact ) { return unisim::util::floating_point::_ToInt<int16_t , double>( a, rmode, exact ); }
template <> int32_t  ToInt<int32_t , double>( double a, uint_fast8_t rmode, bool exact ) { return unisim::util::floating_point::_ToInt<int32_t , double>( a, rmode, exact ); }
template <> int64_t  ToInt<int64_t , double>( double a, uint_fast8_t rmode, bool exact ) { return unisim::util::floating_point::_ToInt<int64_t , double>( a, rmode, exact ); }

template <> uint16_t ToIntRoundMinMag<uint16_t, double>( double a, bool exact ) { return ToInt<uint16_t, double>( a, round_minMag, exact); }
template <> uint32_t ToIntRoundMinMag<uint32_t, double>( double a, bool exact ) { return ToInt<uint32_t, double>( a, round_minMag, exact); }
template <> uint64_t ToIntRoundMinMag<uint64_t, double>( double a, bool exact ) { return ToInt<uint64_t, double>( a, round_minMag, exact); }
template <> int16_t  ToIntRoundMinMag<int16_t , double>( double a, bool exact ) { return ToInt<int16_t , double>( a, round_minMag, exact); }
template <> int32_t  ToIntRoundMinMag<int32_t , double>( double a, bool exact ) { return ToInt<int32_t , double>( a, round_minMag, exact); }
template <> int64_t  ToIntRoundMinMag<int64_t , double>( double a, bool exact ) { return ToInt<int64_t , double>( a, round_minMag, exact); }

double RoundToInt( double a, uint_fast8_t rmode, bool exact )
{
	double d;
	switch( rmode )
	{
		case round_near_maxMag:
		{
			d = round( a );
			if( exact && ( d != a ) ) feraiseexcept( FE_INEXACT );
			break;
		}
		case round_odd:
		{
			int r = fegetround();
			fesetround( FE_TONEAREST );
			d = ( double( 2.0 ) * nearbyint( a / double( 2.0 ) ) ) + double( 1.0 );
			if( exact && ( d != a ) ) feraiseexcept( FE_INEXACT );
			fesetround( r );
			break;
		}
		default:
		{
			int r = fegetround();
			switch( rmode )
			{
				case round_near_even: fesetround( FE_TONEAREST  ); break;
				case round_minMag   : fesetround( FE_TOWARDZERO ); break;
				case round_min      : fesetround( FE_DOWNWARD   ); break;
				case round_max      : fesetround( FE_UPWARD     ); break;
			}
			d = nearbyint( a );
			if( exact && ( d != a ) ) feraiseexcept( FE_INEXACT );
			fesetround( r );
			break;
		}
	}
	return d;
}

double FMulAdd( double a, double b, double c) { return unisim::util::floating_point::_FMulAdd<double>(a, b, c); }

double FRem( double a, double b )
{
	int r = fegetround();
	fesetround( FE_TONEAREST );
	double q = nearbyint( a / b );
	double d = a - (q * b);
	fesetround( r );
	return d;
}

double FSqrt( double a ) { return sqrt( a ); }
double FAbs( double a ) { return fabs( a ); }
double FMin( double a, double b ) { return unisim::util::floating_point::_FMin<double>( a, b ); }
double FMinNumber( double a, double b ) { return unisim::util::floating_point::_FMinNumber<double>( a, b ); }
double FMax( double a, double b ) { return unisim::util::floating_point::_FMax<double>( a, b ); }
double FMaxNumber( double a, double b ) { return unisim::util::floating_point::_FMaxNumber<double>( a, b ); }
bool IsSignaling( double a ) { return unisim::util::floating_point::_IsSignaling<double>( a ); }
template <> double DefaultNaN<double>() { return unisim::util::floating_point::_DefaultNaN<double>(); }

namespace unisim {
namespace util {
namespace floating_point {

// Helper functions for native host floating-point numbers

template <typename INTEGER, typename FLOAT>
static INTEGER _ToInt( FLOAT a, uint_fast8_t rmode, bool exact )
{
	INTEGER res = INTEGER();
	
	if( IsNaN( a ) )
	{
		feraiseexcept( FE_INVALID );
	}
	else
	{
		FLOAT f = RoundToInt( a, rmode, exact );
		fexcept_t excepts;
		fegetexceptflag( &excepts, FE_ALL_EXCEPT );
		if( f < FLOAT(std::numeric_limits<INTEGER>::min()) )
		{
			fesetexceptflag( &excepts, FE_ALL_EXCEPT );
			feraiseexcept( FE_INVALID );
			feclearexcept( FE_INEXACT );
			res = std::numeric_limits<INTEGER>::min();
		}
		else if( f > FLOAT(std::numeric_limits<INTEGER>::max()) )
		{
			fesetexceptflag( &excepts, FE_ALL_EXCEPT );
			feraiseexcept( FE_INVALID );
			feclearexcept( FE_INEXACT );
			res =  std::numeric_limits<INTEGER>::max();
		}
		else if( exact )
		{
			fesetexceptflag( &excepts, FE_ALL_EXCEPT );
			res = INTEGER(f);
		}
		else
		{
			res = INTEGER(f);
			fesetexceptflag( &excepts, FE_ALL_EXCEPT );
		}
		feclearexcept( FE_OVERFLOW );
	}
	
	return res;
}

template <typename FLOAT>
static FLOAT _DefaultNaN()
{
	return std::numeric_limits<FLOAT>::quiet_NaN();
}

template <typename FLOAT>
static FLOAT _FMulAdd( FLOAT a, FLOAT b, FLOAT c )
{
	// Note: on x86_64 host, selecting x86-64-v3 (-march=x86-64-v3) is sufficient for enabling fused multiply-add
	return (a * b) + c;
}

template <typename FLOAT>
static FLOAT _FMin( FLOAT a, FLOAT b )
{
	return (IsZero(a) && IsZero(b)) ? (IsNeg(a) ? a : b) : ((a < b) ? a : b);
}

template <typename FLOAT>
static FLOAT _FMinNumber( FLOAT a, FLOAT b )
{
	fexcept_t excepts;
	fegetexceptflag( &excepts, FE_ALL_EXCEPT );
	FLOAT res = IsNaN( a ) ? ( IsNaN( b ) ? a : b)
	                        : (IsNaN( b ) ? a
	                                      : ( ( IsZero( a ) && IsZero( b ) ) ? ( IsNeg( a ) ? a : b )
	                                                                         : ((a < b) ? a : b) ) );
	fesetexceptflag( &excepts, FE_ALL_EXCEPT );
	return res;
}

template <typename FLOAT>
static FLOAT _FMax( FLOAT a, FLOAT b )
{
	return (IsZero(a) && IsZero(b)) ? (IsNeg(a) ? b : a) : ((a < b) ? b : a);
}

template <typename FLOAT>
static FLOAT _FMaxNumber( FLOAT a, FLOAT b )
{
	fexcept_t excepts;
	fegetexceptflag( &excepts, FE_ALL_EXCEPT );
	FLOAT res = IsNaN( a ) ? ( IsNaN( b ) ? a : b)
	                        : (IsNaN( b ) ? a
	                                      : ( ( IsZero( a ) && IsZero( b ) ) ? ( IsNeg( a ) ? b : a )
	                                                                         : ((a < b) ? b : a) ) );
	fesetexceptflag( &excepts, FE_ALL_EXCEPT );
	return res;
}

} // end of namespace floating_point
} // end of namespace util
} // end of namespace unisim
