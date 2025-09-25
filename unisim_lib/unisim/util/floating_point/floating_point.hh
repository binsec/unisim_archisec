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

#ifndef __UNISIM_UTIL_FLOATING_POINT_FLOATING_POINT_HH__
#define __UNISIM_UTIL_FLOATING_POINT_FLOATING_POINT_HH__

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <cfenv>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <limits>

#if __cplusplus >= 201103L
#define __UNISIM_FP_CONSTEXPR constexpr
#else
#define __UNISIM_FP_CONSTEXPR
#endif

//==============================================================================
//                             Data type declarations
//==============================================================================

// Tininess detection mode
enum
{
	tininess_beforeRounding = 0,
	tininess_afterRounding  = 1
};

// Rounding mode
enum
{
	round_near_even   = 0,
	round_minMag      = 1,
	round_min         = 2,
	round_max         = 3,
	round_near_maxMag = 4,
  // we lost the 5th
	round_odd         = 6
};

// Floating-point exception flags
enum
{
	flag_inexact   = 1,
	flag_underflow = 2,
	flag_overflow  = 4,
	flag_infinite  = 8,
	flag_invalid   = 16
};

// Flush-to-zero
enum
{
	ftz_never          = 0,
	ftz_beforeRounding = 1,
	ftz_afterRounding  = 2
};

// Unpacked float representation
struct UnpackedFloat
{
	bool sign;     // sign
	int exp;       // exponent
	struct
	{
		uint64_t lo; // least significative bits
		uint64_t hi; // most significative bits
	} frac;        // fraction
};

//==============================================================================
//                            Forward declarations
//==============================================================================

namespace unisim {
namespace util {
namespace floating_point {

// Floating-point context save/restore
template <typename FPSC> struct Context;

// General description of floating-point format

template <typename FLOAT> struct FloatingPointTypeInfo {};

// Description of _Float16 type
#if HAVE_FLOAT16
template <> struct FloatingPointTypeInfo<_Float16>
{
	typedef uint16_t packed_type;
	typedef _Float16 float_type;
	static const packed_type sign_mask  = 0x8000;
	static const packed_type  exp_mask  = 0x7c00;
	static const unsigned      exp_pos  = 10;
	static const unsigned     exp_bias  = 15;
	static const packed_type quiet_mask = 0x0200;
	static const packed_type frac_mask  = 0x03ff;
	static __UNISIM_FP_CONSTEXPR float_type FromPacked(packed_type packed) { union { float_type v; packed_type packed; } u = {}; u.packed = packed; return u.v; }
	static __UNISIM_FP_CONSTEXPR packed_type ToPacked(float_type v) { union { float_type v; packed_type packed; } u = {}; u.v = v; return u.packed; }
};
#endif

// Description of float type
template <> struct FloatingPointTypeInfo<float>
{
	typedef uint32_t packed_type;
	typedef float float_type;
	static const packed_type sign_mask  = 0x80000000;
	static const packed_type  exp_mask  = 0x7f800000;
	static const unsigned      exp_pos  = 23;
	static const unsigned     exp_bias  = 127;
	static const packed_type quiet_mask = 0x00400000;
	static const packed_type frac_mask  = 0x007fffff;
	static __UNISIM_FP_CONSTEXPR float_type FromPacked(packed_type packed) { union { float_type v; packed_type packed; } u = {}; u.packed = packed; return u.v; }
	static __UNISIM_FP_CONSTEXPR packed_type ToPacked(float_type v) { union { float_type v; packed_type packed; } u = {}; u.v = v; return u.packed; }
};

// Description of double type
template <> struct FloatingPointTypeInfo<double>
{
	typedef uint64_t packed_type;
	typedef double float_type;
	static const packed_type sign_mask  = 0x8000000000000000ULL;
	static const packed_type  exp_mask  = 0x7ff0000000000000ULL;
	static const unsigned      exp_pos  = 52;
	static const unsigned     exp_bias  = 1023;
	static const packed_type quiet_mask = 0x0008000000000000ULL;
	static const packed_type frac_mask  = 0x000fffffffffffffULL;
	static __UNISIM_FP_CONSTEXPR float_type FromPacked(packed_type packed) { union { float_type v; packed_type packed; } u = {}; u.packed = packed; return u.v; }
	static __UNISIM_FP_CONSTEXPR packed_type ToPacked(float_type v) { union { float_type v; packed_type packed; } u = {}; u.v = v; return u.packed; }
};

// Floating-point status and control for native host floating-point numbers

template <typename FLOAT>
struct FloatingPointStatusAndControl
{
	// Floating-point context save/restore
	typedef unisim::util::floating_point::Context<FloatingPointStatusAndControl<FLOAT> > Context;

	// Status and control
	static void defaultNaN( bool flag );                     // setter
	static bool defaultNaN();                                // getter
	static void detectTininess( uint_fast8_t dt );           // setter
	static uint_fast8_t detectTininess();                    // getter
	static void roundingMode( uint_fast8_t rmode );          // setter
	static uint_fast8_t roundingMode();                      // getter
	static void exceptionFlags( uint_fast8_t eflags );       // setter
	static uint_fast8_t exceptionFlags();                    // getter
	static void extF80_roundingPrecision( uint_fast8_t rp ); // setter
	static uint_fast8_t extF80_roundingPrecision();          // getter
	static void flushToZero( uint_fast8_t ftz );             // setter
	static uint_fast8_t flushToZero();                       // getter
};

// Generic helper functions dependent from FloatingPointTypeInfo<> trait availability
template <typename FLOAT> FLOAT _FromUnpacked( UnpackedFloat const& unpacked );
template <typename FLOAT> __UNISIM_FP_CONSTEXPR typename FloatingPointTypeInfo<FLOAT>::float_type _FromPacked( typename FloatingPointTypeInfo<FLOAT>::packed_type a );
template <typename FLOAT> typename FloatingPointTypeInfo<FLOAT>::packed_type _ToPacked( FLOAT a );
template <typename FLOAT> void _ToUnpacked( UnpackedFloat& unpacked, FLOAT a );
template <typename FLOAT> bool _IsNaN( FLOAT a );
template <typename FLOAT> bool _IsDenormal( FLOAT a );
template <typename FLOAT> bool _IsZero( FLOAT a );
template <typename FLOAT> bool _IsNeg( FLOAT a );
template <typename FLOAT> bool _IsInf( FLOAT a );
template <typename FLOAT> bool _IsSignaling( FLOAT a );
template <typename FLOAT> FLOAT _FAbs( FLOAT a );
template <typename FLOAT> FLOAT _FNeg( FLOAT a );
template <typename FLOAT> FLOAT _ClearSignaling( FLOAT a );
template <typename FLOAT> FLOAT _Zeroes( FLOAT a );
template <typename FLOAT> FLOAT _FMin( FLOAT a, FLOAT b );
template <typename FLOAT> FLOAT _FMinNumber( FLOAT a, FLOAT b );
template <typename FLOAT> FLOAT _FMax( FLOAT a, FLOAT b );
template <typename FLOAT> FLOAT _FMaxNumber( FLOAT a, FLOAT b );

} // end of namespace floating_point
} // end of namespace util
} // end of namespace unisim

//==============================================================================
//                              Declarations
//==============================================================================

// Floating-point status and control
template <typename FLOAT>
struct FloatingPointStatusAndControl
{
	// Floating-point context save/restore
	typedef unisim::util::floating_point::Context<FloatingPointStatusAndControl<FLOAT> > Context;

	static void defaultNaN( bool dn );                       // setter
	static bool defaultNaN();                                // getter
	static void detectTininess( uint_fast8_t dt );           // setter
	static uint_fast8_t detectTininess();                    // getter
	static void roundingMode( uint_fast8_t rmode );          // setter
	static uint_fast8_t roundingMode();                      // getter
	static void exceptionFlags( uint_fast8_t eflags );       // setter
	static uint_fast8_t exceptionFlags();                    // getter
	static void extF80_roundingPrecision( uint_fast8_t rp ); // setter
	static uint_fast8_t extF80_roundingPrecision();          // getter
};

// Floating-point status and control traits for native host floating-point numbers

#if HAVE_FLOAT16
template <> struct FloatingPointStatusAndControl<_Float16> : unisim::util::floating_point::FloatingPointStatusAndControl<_Float16> {};
#endif
template <> struct FloatingPointStatusAndControl<float> : unisim::util::floating_point::FloatingPointStatusAndControl<float> {};
template <> struct FloatingPointStatusAndControl<double> : unisim::util::floating_point::FloatingPointStatusAndControl<double> {};

// template functions for any floating-point type
template <typename FLOAT> FLOAT FromUnpacked( UnpackedFloat const& unpacked );
template <typename FLOAT, typename PACKED> __UNISIM_FP_CONSTEXPR FLOAT FromPacked( PACKED a );
template <typename DST, typename SRC> DST FConvert( SRC a );
template <typename INTEGER, typename FLOAT> INTEGER ToInt( FLOAT a, uint_fast8_t rmode, bool exact );
template <typename INTEGER, typename FLOAT> INTEGER ToIntRoundMinMag( FLOAT a, bool exact );
template <typename FLOAT> FLOAT DefaultNaN();

#if HAVE_FLOAT16
// Functions for _Float16
template <> inline _Float16 FromUnpacked<_Float16>( UnpackedFloat const& unpacked ) { return unisim::util::floating_point::_FromUnpacked<_Float16>( unpacked ); }
template <> inline __UNISIM_FP_CONSTEXPR _Float16 FromPacked<_Float16, uint16_t>( uint16_t a ) { return unisim::util::floating_point::_FromPacked<_Float16>( a ); }
inline uint16_t ToPacked( _Float16 a ) { return unisim::util::floating_point::_ToPacked<_Float16>( a ); }
inline void ToUnpacked( UnpackedFloat& unpacked, _Float16 a ) { unisim::util::floating_point::_ToUnpacked<_Float16>( unpacked, a ); }
template <> inline _Float16 FConvert<_Float16, _Float16>( _Float16 a ) { return a; }
inline bool IsNaN( _Float16 a ) { return unisim::util::floating_point::_IsNaN<_Float16>( a ); }
inline bool IsDenormal( _Float16 a ) { return unisim::util::floating_point::_IsDenormal<_Float16>( a ); }
inline bool IsZero( _Float16 a ) { return unisim::util::floating_point::_IsZero<_Float16>( a ); }
inline bool IsNeg( _Float16 a ) { return unisim::util::floating_point::_IsNeg<_Float16>( a ); }
inline bool IsInf( _Float16 a ) { return unisim::util::floating_point::_IsInf<_Float16>( a ); }
_Float16 RoundToInt( _Float16 a, uint_fast8_t rmode, bool exact );
_Float16 FMulAdd( _Float16 a, _Float16 b, _Float16 c);
_Float16 FRem( _Float16 a, _Float16 b );
_Float16 FSqrt( _Float16 a );
_Float16 FAbs( _Float16 a );
_Float16 FMin( _Float16 a, _Float16 b );
_Float16 FMinNumber( _Float16 a, _Float16 b );
_Float16 FMax( _Float16 a, _Float16 b );
_Float16 FMaxNumber( _Float16 a, _Float16 b );
inline bool IsSignaling( _Float16 a ) { return unisim::util::floating_point::_IsSignaling<_Float16>( a ); }
inline _Float16 ClearSignaling( _Float16 a ) { return unisim::util::floating_point::_ClearSignaling<_Float16>( a ); }
inline _Float16 Zeroes( _Float16 a ) { return unisim::util::floating_point::_Zeroes<_Float16>( a ); }

inline std::ostream& operator << (std::ostream& stream, _Float16 const& f) { return stream << float(f); }
inline std::wostream& operator << (std::wostream& stream, _Float16 const& f) { return stream << float(f); }
inline std::istream& operator << (std::istream& stream, _Float16& f) { float _f; if(stream >> _f) { f = _Float16(_f); } return stream; }
inline std::wistream& operator << (std::wistream& stream, _Float16& f) { float _f; if(stream >> _f) { f = _Float16(_f); } return stream; }

#if !HAVE_STD_NUMERIC_LIMITS_FLOAT16
// std::numeric_limits trait for _Float16
template <>
class std::numeric_limits<_Float16>
{
public:
	static __UNISIM_FP_CONSTEXPR bool is_specialized = true;
	static __UNISIM_FP_CONSTEXPR bool is_signed = true;
	static __UNISIM_FP_CONSTEXPR bool is_integer = false;
	static __UNISIM_FP_CONSTEXPR bool is_exact = false;
	static __UNISIM_FP_CONSTEXPR bool has_infinity = true;
	static __UNISIM_FP_CONSTEXPR bool has_quiet_NaN = true;
	static __UNISIM_FP_CONSTEXPR bool has_signaling_NaN = true;
	static __UNISIM_FP_CONSTEXPR std::float_denorm_style has_denorm = std::denorm_present;
	static __UNISIM_FP_CONSTEXPR bool has_denorm_loss = false;
	static __UNISIM_FP_CONSTEXPR std::float_round_style round_style = std::round_to_nearest;
	static __UNISIM_FP_CONSTEXPR bool is_iec559 = true;
	static __UNISIM_FP_CONSTEXPR bool is_bounded = true;
	static __UNISIM_FP_CONSTEXPR bool is_modulo = false;
	static __UNISIM_FP_CONSTEXPR int digits = 11;
	static __UNISIM_FP_CONSTEXPR int digits10 = 3;
	static __UNISIM_FP_CONSTEXPR int max_digits10 = 5;
	static __UNISIM_FP_CONSTEXPR int radix = 2;
	static __UNISIM_FP_CONSTEXPR int min_exponent = -13;
	static __UNISIM_FP_CONSTEXPR int min_exponent10 = -4 ;
	static __UNISIM_FP_CONSTEXPR int max_exponent = 16;
	static __UNISIM_FP_CONSTEXPR int max_exponent10 = 4 ;
	static __UNISIM_FP_CONSTEXPR bool traps = false;
	static __UNISIM_FP_CONSTEXPR int tinyness_before = false;
	
	static __UNISIM_FP_CONSTEXPR _Float16 min()           noexcept { return FromPacked<_Float16>( uint16_t(0x0400) ); }
	static __UNISIM_FP_CONSTEXPR _Float16 lowest()        noexcept { return FromPacked<_Float16>( uint16_t(0xfbff) ); }
	static __UNISIM_FP_CONSTEXPR _Float16 max()           noexcept { return FromPacked<_Float16>( uint16_t(0x7bff) ); }
	static __UNISIM_FP_CONSTEXPR _Float16 epsilon()       noexcept { return FromPacked<_Float16>( uint16_t(0x1400) ); }
	static __UNISIM_FP_CONSTEXPR _Float16 round_error()   noexcept { return FromPacked<_Float16>( uint16_t(0x3800) ); }
	static __UNISIM_FP_CONSTEXPR _Float16 infinity()      noexcept { return FromPacked<_Float16>( uint16_t(0x7c00) ); }
	static __UNISIM_FP_CONSTEXPR _Float16 quiet_NaN()     noexcept { return FromPacked<_Float16>( uint16_t(0x7e00) ); }
	static __UNISIM_FP_CONSTEXPR _Float16 signaling_NaN() noexcept { return FromPacked<_Float16>( uint16_t(0x7d00) ); }
	static __UNISIM_FP_CONSTEXPR _Float16 denorm_min()    noexcept { return FromPacked<_Float16>( uint16_t(0x0001) ); }
};
#endif // HAVE_STD_NUMERIC_LIMITS_FLOAT16
#endif // HAVE_FLOAT16

// Functions for float
template <> inline float FromUnpacked<float>( UnpackedFloat const& unpacked ) { return unisim::util::floating_point::_FromUnpacked<float>( unpacked ); }
template <> inline __UNISIM_FP_CONSTEXPR float FromPacked<float, uint32_t>( uint32_t a ) { return unisim::util::floating_point::_FromPacked<float>( a ); }
inline uint32_t ToPacked( float a ) { return unisim::util::floating_point::_ToPacked<float>( a ); }
inline void ToUnpacked( UnpackedFloat& unpacked, float a ) { unisim::util::floating_point::_ToUnpacked<float>( unpacked, a ); }
#if HAVE_FLOAT16
template <> inline float FConvert<float, _Float16>( _Float16 a ) { return float( a ); }
#endif
template <> inline float FConvert<float, float>( float a ) { return a; }
template <> inline float FConvert<float, double>( double a ) { return float( a ); }
inline bool IsNaN( float a ) { return unisim::util::floating_point::_IsNaN<float>( a ); }
inline bool IsDenormal( float a ) { return unisim::util::floating_point::_IsDenormal<float>( a ); }
inline bool IsZero( float a ) { return unisim::util::floating_point::_IsZero<float>( a ); }
inline bool IsNeg( float a ) { return unisim::util::floating_point::_IsNeg<float>( a ); }
inline bool IsInf( float a ) { return unisim::util::floating_point::_IsInf<float>( a ); }
float RoundToInt( float a, uint_fast8_t rmode, bool exact );
float FMulAdd( float a, float b, float c);
float FRem( float a, float b );
float FSqrt( float a );
float FAbs( float a );
float FMin( float a, float b );
float FMinNumber( float a, float b );
float FMax( float a, float b );
float FMaxNumber( float a, float b );
bool IsSignaling( float a );
inline float ClearSignaling( float a ) { return unisim::util::floating_point::_ClearSignaling<float>( a ); }
inline float Zeroes( float a ) { return unisim::util::floating_point::_Zeroes<float>( a ); }

// Functions for double
template <> inline double FromUnpacked<double>( UnpackedFloat const& unpacked ) { return unisim::util::floating_point::_FromUnpacked<double>( unpacked ); }
template <> inline __UNISIM_FP_CONSTEXPR double FromPacked<double, uint64_t>( uint64_t a ) { return unisim::util::floating_point::_FromPacked<double>( a ); }
inline uint64_t ToPacked( double a ) { return unisim::util::floating_point::_ToPacked<double>( a ); }
inline void ToUnpacked( UnpackedFloat& unpacked, double a ) { unisim::util::floating_point::_ToUnpacked<double>( unpacked, a ); }
#if HAVE_FLOAT16
template <> inline double FConvert<double, _Float16>( _Float16 a ) { return double( a ); }
#endif
template <> inline double FConvert<double, float>( float a ) { return double( a ); }
template <> inline double FConvert<double, double>( double a ) { return a; }
inline bool IsNaN( double a ) { return unisim::util::floating_point::_IsNaN<double>( a ); }
inline bool IsDenormal( double a ) { return unisim::util::floating_point::_IsDenormal<double>( a ); }
inline bool IsZero( double a ) { return unisim::util::floating_point::_IsZero<double>( a ); }
inline bool IsNeg( double a ) { return unisim::util::floating_point::_IsNeg<double>( a ); }
inline bool IsInf( double a ) { return unisim::util::floating_point::_IsInf<double>( a ); }
double RoundToInt( double a, uint_fast8_t rmode, bool exact );
double FMulAdd( double a, double b, double c);
double FRem( double a, double b );
double FSqrt( double a );
double FAbs( double a );
double FMin( double a, double b );
double FMinNumber( double a, double b );
double FMax( double a, double b );
double FMaxNumber( double a, double b );
bool IsSignaling( double a );
inline double ClearSignaling( double a ) { return unisim::util::floating_point::_ClearSignaling<double>( a ); }
inline double Zeroes( double a ) { return unisim::util::floating_point::_Zeroes<double>( a ); }

//==============================================================================
//                               Definitions
//==============================================================================

namespace unisim {
namespace util {
namespace floating_point {

// Floating-point context save/restore
template <typename FPSC>
struct Context
{
  Context()
    : defaultNaN( FPSC::defaultNaN() )
    , detectTininess( FPSC::detectTininess() )
    , roundingMode( FPSC::roundingMode() )
    , exceptionFlags( FPSC::exceptionFlags() )
    , extF80_roundingPrecision( FPSC::extF80_roundingPrecision() ) {}

  ~Context()
  {
    FPSC::defaultNaN( defaultNaN );
    FPSC::detectTininess( detectTininess );
    FPSC::roundingMode( roundingMode );
    FPSC::exceptionFlags( exceptionFlags );
    FPSC::extF80_roundingPrecision( extF80_roundingPrecision );
  }
private:
  bool defaultNaN;
  uint_fast8_t detectTininess;
  uint_fast8_t roundingMode;
  uint_fast8_t exceptionFlags;
  uint_fast8_t extF80_roundingPrecision;
};

// Printable helper classes
struct DetectTininess
{
	DetectTininess( uint_fast8_t _detect_tininess ) : dt(_detect_tininess) {}
	friend std::ostream& operator << (std::ostream& stream, const DetectTininess& value);
private:
	uint_fast8_t dt;
};

struct RoundingMode
{
	RoundingMode( uint_fast8_t _rmode ) : rmode(_rmode) {}
	friend std::ostream& operator << (std::ostream& stream, const RoundingMode& value);
private:
	uint_fast8_t rmode;
};

struct ExceptionFlags
{
	ExceptionFlags( uint_fast8_t _eflags ) : eflags(_eflags) {}
	friend std::ostream& operator << (std::ostream& stream, const ExceptionFlags& value);
private:
	uint_fast8_t eflags;
};

struct FlushToZero
{
	FlushToZero( uint_fast8_t _flush_to_zero ) : ftz(_flush_to_zero) {}
	friend std::ostream& operator << (std::ostream& stream, const FlushToZero& value);
private:
	uint_fast8_t ftz;
};

inline std::ostream& operator << (std::ostream& stream, const DetectTininess& value)
{
	switch( value.dt )
	{
		case tininess_beforeRounding: return stream << "tininess before rounding";
		case tininess_afterRounding : return stream << "tininess after rounding";
	}
	return stream;
}

inline std::ostream& operator << (std::ostream& stream, const RoundingMode& value)
{
	switch( value.rmode )
	{
		case round_near_even  : return stream << "round near even";
		case round_minMag     : return stream << "round min mag";
		case round_min        : return stream << "round min";
		case round_max        : return stream << "round max";
		case round_near_maxMag: return stream << "round near max mag";
		case round_odd        : return stream << "round odd";
	}
	return stream;
}

inline std::ostream& operator << (std::ostream& stream, const ExceptionFlags& value)
{
	return stream
		<< "("
		<< "inexact="   << ( ( value.eflags & flag_inexact   ) != 0 ) << ":"
		<< "underflow=" << ( ( value.eflags & flag_underflow ) != 0 ) << ":"
		<< "overflow="  << ( ( value.eflags & flag_overflow  ) != 0 ) << ":"
		<< "infinite="  << ( ( value.eflags & flag_infinite  ) != 0 ) << ":"
		<< "invalid="   << ( ( value.eflags & flag_invalid   ) != 0 )
		<< ")";
}

inline std::ostream& operator << (std::ostream& stream, const FlushToZero& value)
{
	switch( value.ftz )
	{
		case ftz_never         : return stream << "flush-to-zero never";
		case ftz_beforeRounding: return stream << "flush-to-zero before rounding";
		case ftz_afterRounding : return stream << "flush-to-zero after rounding";
	}
	return stream;
}

// Floating-point status and control for native host floating-point numbers

template <typename FLOAT>
void FloatingPointStatusAndControl<FLOAT>::defaultNaN( bool flag ) {}

template <typename FLOAT>
bool FloatingPointStatusAndControl<FLOAT>::defaultNaN() { return false; }

template <typename FLOAT>
void FloatingPointStatusAndControl<FLOAT>::detectTininess( uint_fast8_t dt ) {}

template <typename FLOAT>
uint_fast8_t FloatingPointStatusAndControl<FLOAT>::detectTininess() { return tininess_beforeRounding; }

template <typename FLOAT>
void FloatingPointStatusAndControl<FLOAT>::roundingMode( uint_fast8_t rmode )
{
	switch( rmode )
	{
		case round_near_even  : fesetround( FE_TONEAREST  ); break;
		case round_minMag     : fesetround( FE_TOWARDZERO ); break;
		case round_min        : fesetround( FE_DOWNWARD   ); break;
		case round_max        : fesetround( FE_UPWARD     ); break;
		case round_near_maxMag: std::cerr << "WARNING! round_near_maxMag is unsupported." << std::endl; fesetround( FE_TONEAREST ); break;
		case round_odd        : std::cerr << "WARNING! round_odd is unsupported." << std::endl; fesetround( FE_TONEAREST ); break;
	}
}

template <typename FLOAT>
uint_fast8_t FloatingPointStatusAndControl<FLOAT>::roundingMode()
{
	switch( fegetround() )
	{
		case FE_TONEAREST: return round_near_even;
		case FE_TOWARDZERO: return round_minMag;
		case FE_DOWNWARD: return round_min;
		case FE_UPWARD: return round_max;
	}
	return round_near_even;
}

#ifndef EMSCRIPTEN

template <typename FLOAT>
void FloatingPointStatusAndControl<FLOAT>::exceptionFlags( uint_fast8_t eflags )
{
	fexcept_t excepts = ( ( ( eflags & flag_inexact   ) != 0 ) ? FE_INEXACT   : 0)
	                  | ( ( ( eflags & flag_underflow ) != 0 ) ? FE_UNDERFLOW : 0)
	                  | ( ( ( eflags & flag_overflow  ) != 0 ) ? FE_OVERFLOW  : 0)
	                  | ( ( ( eflags & flag_infinite  ) != 0 ) ? FE_DIVBYZERO : 0)
	                  | ( ( ( eflags & flag_invalid   ) != 0 ) ? FE_INVALID   : 0);
	fesetexceptflag( &excepts, FE_ALL_EXCEPT );
}

template <typename FLOAT>
uint_fast8_t FloatingPointStatusAndControl<FLOAT>::exceptionFlags()
{
	fexcept_t excepts;
	fegetexceptflag( &excepts, FE_ALL_EXCEPT );
	return ( ( ( excepts & FE_INEXACT   ) == FE_INEXACT   ) ? flag_inexact   : 0)
	     | ( ( ( excepts & FE_UNDERFLOW ) == FE_UNDERFLOW ) ? flag_underflow : 0)
	     | ( ( ( excepts & FE_OVERFLOW  ) == FE_OVERFLOW  ) ? flag_overflow  : 0)
	     | ( ( ( excepts & FE_DIVBYZERO ) == FE_DIVBYZERO ) ? flag_infinite  : 0)
	     | ( ( ( excepts & FE_INVALID   ) == FE_INVALID   ) ? flag_invalid   : 0);
}

#endif

template <typename FLOAT>
void FloatingPointStatusAndControl<FLOAT>::extF80_roundingPrecision( uint_fast8_t rp ) {}

template <typename FLOAT>
uint_fast8_t FloatingPointStatusAndControl<FLOAT>::extF80_roundingPrecision() { return 80; }

template <typename FLOAT>
void FloatingPointStatusAndControl<FLOAT>::flushToZero( uint_fast8_t ftz ) {}

template <typename FLOAT>
uint_fast8_t FloatingPointStatusAndControl<FLOAT>::flushToZero() { return ftz_never; }

// Generic helper functions dependent from FloatingPointTypeInfo<> trait availability

template <typename FLOAT>
FLOAT _FromUnpacked( UnpackedFloat const& unpacked )
{
	typename FloatingPointTypeInfo<FLOAT>::packed_type packed =
		(FloatingPointTypeInfo<FLOAT>::sign_mask * unpacked.sign)                                                                                               /* sign */
		| ((uint64_t(unpacked.exp + FloatingPointTypeInfo<FLOAT>::exp_bias) << FloatingPointTypeInfo<FLOAT>::exp_pos) & FloatingPointTypeInfo<FLOAT>::exp_mask) /* biased exponent */
		| (unpacked.frac.lo & FloatingPointTypeInfo<FLOAT>::frac_mask);                                                                                         /* fraction */
	return FloatingPointTypeInfo<FLOAT>::FromPacked( packed );
}

template <typename FLOAT>
__UNISIM_FP_CONSTEXPR typename FloatingPointTypeInfo<FLOAT>::float_type _FromPacked( typename FloatingPointTypeInfo<FLOAT>::packed_type a )
{
	return FloatingPointTypeInfo<FLOAT>::FromPacked( a );
}

template <typename FLOAT> typename FloatingPointTypeInfo<FLOAT>::packed_type _ToPacked( FLOAT a )
{
	return FloatingPointTypeInfo<FLOAT>::ToPacked( a );
}

template <typename FLOAT>
void _ToUnpacked( UnpackedFloat& unpacked, FLOAT a )
{
	typename FloatingPointTypeInfo<FLOAT>::packed_type packed = FloatingPointTypeInfo<FLOAT>::ToPacked( a );
	
	unpacked.sign = (packed & FloatingPointTypeInfo<FLOAT>::sign_mask) != 0;                                                                              /* sign */
	unpacked.exp = ((packed & FloatingPointTypeInfo<FLOAT>::exp_mask) >> FloatingPointTypeInfo<FLOAT>::exp_pos) - FloatingPointTypeInfo<FLOAT>::exp_bias; /* biased exponent */
	unpacked.frac.lo = packed & FloatingPointTypeInfo<FLOAT>::frac_mask;                                                                                  /* fraction */
	unpacked.frac.hi = 0;
}

template <typename FLOAT>
bool _IsNaN( FLOAT a )
{
	typename FloatingPointTypeInfo<FLOAT>::packed_type packed = FloatingPointTypeInfo<FLOAT>::ToPacked( a );
	return ( ( packed & FloatingPointTypeInfo<FLOAT>::exp_mask ) == FloatingPointTypeInfo<FLOAT>::exp_mask ) && ( ( packed & FloatingPointTypeInfo<FLOAT>::frac_mask ) != 0 );
}

template <typename FLOAT>
bool _IsDenormal( FLOAT a )
{
	typename FloatingPointTypeInfo<FLOAT>::packed_type packed = FloatingPointTypeInfo<FLOAT>::ToPacked( a );
	return ( ( packed & FloatingPointTypeInfo<FLOAT>::exp_mask ) == 0) && ( ( packed & FloatingPointTypeInfo<FLOAT>::frac_mask ) != 0 );
}

template <typename FLOAT>
bool _IsZero( FLOAT a )
{
	typename FloatingPointTypeInfo<FLOAT>::packed_type packed = FloatingPointTypeInfo<FLOAT>::ToPacked( a );
	return (packed & (FloatingPointTypeInfo<FLOAT>::exp_mask | FloatingPointTypeInfo<FLOAT>::frac_mask)) == 0;
}

template <typename FLOAT>
bool _IsNeg( FLOAT a )
{
	typename FloatingPointTypeInfo<FLOAT>::packed_type packed = FloatingPointTypeInfo<FLOAT>::ToPacked( a );
	return (packed & FloatingPointTypeInfo<FLOAT>::sign_mask) != 0;
}

template <typename FLOAT>
bool _IsInf( FLOAT a )
{
	typename FloatingPointTypeInfo<FLOAT>::packed_type packed = FloatingPointTypeInfo<FLOAT>::ToPacked( a );
	return ( ( packed & FloatingPointTypeInfo<FLOAT>::exp_mask ) == FloatingPointTypeInfo<FLOAT>::exp_mask ) && ( ( packed & FloatingPointTypeInfo<FLOAT>::frac_mask ) == 0 );
}

template <typename FLOAT>
bool _IsSignaling( FLOAT a )
{
	FLOAT qnan = DefaultNaN<FLOAT>();
	typename FloatingPointTypeInfo<FLOAT>::packed_type packed_qnan = FloatingPointTypeInfo<FLOAT>::ToPacked( qnan );
	typename FloatingPointTypeInfo<FLOAT>::packed_type packed = FloatingPointTypeInfo<FLOAT>::ToPacked( a );
	return ( ( packed & ( FloatingPointTypeInfo<FLOAT>::exp_mask | FloatingPointTypeInfo<FLOAT>::quiet_mask ) ) == ( packed_qnan & ( FloatingPointTypeInfo<FLOAT>::exp_mask | FloatingPointTypeInfo<FLOAT>::quiet_mask ) ) ) && ( ( packed & FloatingPointTypeInfo<FLOAT>::frac_mask & ~FloatingPointTypeInfo<FLOAT>::quiet_mask ) != 0 );
}

template <typename FLOAT>
FLOAT _FAbs( FLOAT a )
{
	return FloatingPointTypeInfo<FLOAT>::FromPacked( FloatingPointTypeInfo<FLOAT>::ToPacked( a ) & ~FloatingPointTypeInfo<FLOAT>::sign_mask );
}

template <typename FLOAT>
FLOAT _FNeg( FLOAT a )
{
	return FloatingPointTypeInfo<FLOAT>::FromPacked( FloatingPointTypeInfo<FLOAT>::ToPacked( a ) ^ FloatingPointTypeInfo<FLOAT>::sign_mask );
}

template <typename FLOAT>
FLOAT _ClearSignaling( FLOAT a )
{
	FLOAT qnan = DefaultNaN<FLOAT>();
	typename FloatingPointTypeInfo<FLOAT>::packed_type packed_qnan = FloatingPointTypeInfo<FLOAT>::ToPacked( qnan );
	typename FloatingPointTypeInfo<FLOAT>::packed_type packed = FloatingPointTypeInfo<FLOAT>::ToPacked( a );
	packed = (packed & ~FloatingPointTypeInfo<FLOAT>::quiet_mask) | (packed_qnan & FloatingPointTypeInfo<FLOAT>::quiet_mask);
	return FromPacked<FLOAT>( packed );
}

template <typename FLOAT>
FLOAT _Zeroes( FLOAT a )
{
	typename FloatingPointTypeInfo<FLOAT>::packed_type packed = FloatingPointTypeInfo<FLOAT>::ToPacked( a );
	packed &= FloatingPointTypeInfo<FLOAT>::sign_mask;
	return FloatingPointTypeInfo<FLOAT>::FromPacked( packed );
}

template <typename FLOAT>
FLOAT _FMin( FLOAT a, FLOAT b )
{
	return (IsZero(a) && IsZero(b)) ? (IsNeg(a) ? a : b) : ((a < b) ? a : b);
}

template <typename FLOAT>
FLOAT _FMinNumber( FLOAT a, FLOAT b )
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
FLOAT _FMax( FLOAT a, FLOAT b )
{
	return (IsZero(a) && IsZero(b)) ? (IsNeg(a) ? b : a) : ((a < b) ? b : a);
}

template <typename FLOAT>
FLOAT _FMaxNumber( FLOAT a, FLOAT b )
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

#endif // __UNISIM_UTIL_FLOATING_POINT_FLOATING_POINT_HH__
