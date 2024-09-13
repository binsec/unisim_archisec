/*
 *  Copyright (c) 2015-2024,
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
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 *  OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 *  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors: Yves Lhuillier (yves.lhuillier@cea.fr)
 *          Gilles Mouchard (gilles.mouchard@cea.fr)
 */

#ifndef __UNISIM_COMPONENT_CXX_PROCESSOR_ARM_REGS64_CPU_HH__
#define __UNISIM_COMPONENT_CXX_PROCESSOR_ARM_REGS64_CPU_HH__

#include <unisim/component/cxx/vector/vector.hh>
#include <type_traits>

namespace unisim {
namespace component {
namespace cxx {
namespace processor {
namespace arm {
namespace regs64 {

template <typename TYPES, typename T> struct SetGType : public std::false_type {};
template <typename TYPES> struct SetGType<TYPES, typename TYPES::U32> : public std::true_type {};
template <typename TYPES> struct SetGType<TYPES, typename TYPES::U64> : public std::true_type {};

/// Implement the base register architecture of aarch64 (general purpose and SIMD)
template <typename CPU_IMPL, typename TYPES>
struct CPU
{
  //================ General Configuration and Constants ===============

  /// Control Transfer Type enumeration
  enum branch_type_t { B_JMP = 0, B_CALL, B_RET, B_EXC, B_ERET, B_DBG };

  /// Advanced SIMD layout
  struct VUConfig
  {
    static unsigned const BYTECOUNT = 16;
    template <typename T>
    using TypeInfo = typename TYPES::template VectorTypeInfo<T>;
    using Byte     = typename TYPES::VectorByte;
  };
  static unsigned const VECTORCOUNT = 32;
  static unsigned const GREGCOUNT = 32;

  typedef unisim::component::cxx::vector::VUnion<VUConfig> VectorView;
  struct Vector { typename TYPES::VectorByteShadow data[VUConfig::BYTECOUNT]; };


  /// Default Constructor (=default)
  CPU()
    : gpr()
    , vector_views()
    , vectors()
  {}

  /// Destructor (required vector destructions)
  ~CPU()
  {
    for (unsigned reg = 0; reg < VECTORCOUNT; ++reg)
      vector_views[reg].Clear(&vectors[reg]);
  }

  //============= General Purpose Registers access methods =============

  /// Get the value contained by a General-purpose or the Stack register.
  typename TYPES::U64 GetGSR(unsigned reg)
  {
    if (CPU_IMPL::report_gsr_access != CPU_IMPL::report_none) static_cast<CPU_IMPL*>(this)->report(CPU_IMPL::report_gsr_access, reg, false);
    return gpr[reg];
  }

  /// Get the value contained by a General-purpose or the Zero register.
  typename TYPES::U64 GetGZR(unsigned reg)
  {
    if (CPU_IMPL::report_gzr_access != CPU_IMPL::report_none) static_cast<CPU_IMPL*>(this)->report(CPU_IMPL::report_gzr_access, reg, false);
    return (reg != 31) ? gpr[reg] : typename TYPES::U64(0);
  }

  /// Set the value of a General-purpose or the Stack register
  template <typename T>
  void SetGSR(unsigned reg, T val)
  {
    static_assert(SetGType<TYPES, T>::value, "T must be U32 or U64");
    if (CPU_IMPL::report_gsr_access != CPU_IMPL::report_none) static_cast<CPU_IMPL*>(this)->report(CPU_IMPL::report_gsr_access, reg, true);
    gpr[reg] = typename TYPES::U64(val);
  }

  /// Set the value of a General-purpose or the Zero register
  template <typename T>
  void SetGZR(unsigned reg, T val)
  {
    static_assert(SetGType<TYPES, T>::value, "T must be U32 or U64");
    if (CPU_IMPL::report_gzr_access != CPU_IMPL::report_none) static_cast<CPU_IMPL*>(this)->report(CPU_IMPL::report_gzr_access, reg, true);
    if (reg != 31) gpr[reg] = typename TYPES::U64( val );
  }

  //================= Vector Registers access methods ==================

  template <typename T>
  T vector_read(unsigned reg, unsigned sub)
  {
    if (CPU_IMPL::report_simd_access != CPU_IMPL::report_none) static_cast<CPU_IMPL*>(this)->report(CPU_IMPL::report_simd_access, reg, false);
    return (vector_views[reg].GetConstStorage(&vectors[reg], T(), VUConfig::BYTECOUNT))[sub];
  }

  typename TYPES::U8  GetVU8 ( unsigned reg, unsigned sub ) { return vector_read<typename TYPES::U8> (reg, sub); }
  typename TYPES::U16 GetVU16( unsigned reg, unsigned sub ) { return vector_read<typename TYPES::U16>(reg, sub); }
  typename TYPES::U32 GetVU32( unsigned reg, unsigned sub ) { return vector_read<typename TYPES::U32>(reg, sub); }
  typename TYPES::U64 GetVU64( unsigned reg, unsigned sub ) { return vector_read<typename TYPES::U64>(reg, sub); }
  typename TYPES::S8  GetVS8 ( unsigned reg, unsigned sub ) { return vector_read<typename TYPES::S8> (reg, sub); }
  typename TYPES::S16 GetVS16( unsigned reg, unsigned sub ) { return vector_read<typename TYPES::S16>(reg, sub); }
  typename TYPES::S32 GetVS32( unsigned reg, unsigned sub ) { return vector_read<typename TYPES::S32>(reg, sub); }
  typename TYPES::S64 GetVS64( unsigned reg, unsigned sub ) { return vector_read<typename TYPES::S64>(reg, sub); }
  typename TYPES::F16 GetVF16( unsigned reg, unsigned sub ) { return vector_read<typename TYPES::F16>(reg, sub); }
  typename TYPES::F32 GetVF32( unsigned reg, unsigned sub ) { return vector_read<typename TYPES::F32>(reg, sub); }
  typename TYPES::F64 GetVF64( unsigned reg, unsigned sub ) { return vector_read<typename TYPES::F64>(reg, sub); }

  template <typename T>
  void vector_write(unsigned reg, unsigned sub, T value )
  {
    if (CPU_IMPL::report_simd_access != CPU_IMPL::report_none) static_cast<CPU_IMPL*>(this)->report(CPU_IMPL::report_simd_access, reg, true);
    (vector_views[reg].GetStorage(&vectors[reg], value, VUConfig::BYTECOUNT))[sub] = value;
  }

  void SetVU8 ( unsigned reg, unsigned sub, typename TYPES::U8  value ) { vector_write( reg, sub, value ); }
  void SetVU16( unsigned reg, unsigned sub, typename TYPES::U16 value ) { vector_write( reg, sub, value ); }
  void SetVU32( unsigned reg, unsigned sub, typename TYPES::U32 value ) { vector_write( reg, sub, value ); }
  void SetVU64( unsigned reg, unsigned sub, typename TYPES::U64 value ) { vector_write( reg, sub, value ); }
  void SetVS8 ( unsigned reg, unsigned sub, typename TYPES::S8  value ) { vector_write( reg, sub, value ); }
  void SetVS16( unsigned reg, unsigned sub, typename TYPES::S16 value ) { vector_write( reg, sub, value ); }
  void SetVS32( unsigned reg, unsigned sub, typename TYPES::S32 value ) { vector_write( reg, sub, value ); }
  void SetVS64( unsigned reg, unsigned sub, typename TYPES::S64 value ) { vector_write( reg, sub, value ); }
  void SetVF16( unsigned reg, unsigned sub, typename TYPES::F16 value ) { vector_write( reg, sub, value ); }
  void SetVF32( unsigned reg, unsigned sub, typename TYPES::F32 value ) { vector_write( reg, sub, value ); }
  void SetVF64( unsigned reg, unsigned sub, typename TYPES::F64 value ) { vector_write( reg, sub, value ); }

  template <typename T>
  void vector_write(unsigned reg, T value )
  {
    if (CPU_IMPL::report_simd_access != CPU_IMPL::report_none) static_cast<CPU_IMPL*>(this)->report(CPU_IMPL::report_simd_access, reg, true);
    *(vector_views[reg].GetStorage(&vectors[reg], value, VUConfig::template TypeInfo<T>::bytecount)) = value;
  }

  void SetVU8 ( unsigned reg, typename TYPES::U8 value )  { vector_write(reg, value); }
  void SetVU16( unsigned reg, typename TYPES::U16 value ) { vector_write(reg, value); }
  void SetVU32( unsigned reg, typename TYPES::U32 value ) { vector_write(reg, value); }
  void SetVU64( unsigned reg, typename TYPES::U64 value ) { vector_write(reg, value); }
  void SetVS8 ( unsigned reg, typename TYPES::S8 value )  { vector_write(reg, value); }
  void SetVS16( unsigned reg, typename TYPES::S16 value ) { vector_write(reg, value); }
  void SetVS32( unsigned reg, typename TYPES::S32 value ) { vector_write(reg, value); }
  void SetVS64( unsigned reg, typename TYPES::S64 value ) { vector_write(reg, value); }
  void SetVF16( unsigned reg, typename TYPES::F16 value ) { vector_write(reg, value); }
  void SetVF32( unsigned reg, typename TYPES::F32 value ) { vector_write(reg, value); }
  void SetVF64( unsigned reg, typename TYPES::F64 value ) { vector_write(reg, value); }

  void ClearHighV( unsigned reg, unsigned bytes )
  {
    if (CPU_IMPL::report_simd_access != CPU_IMPL::report_none) static_cast<CPU_IMPL*>(this)->report(CPU_IMPL::report_simd_access, reg, true);
    vector_views[reg].Truncate(bytes);
  }

  //===================== General Purpose Registers ====================

  typename TYPES::U64           gpr[GREGCOUNT];

  //========================= Vector Registers =========================

  VectorView    vector_views[VECTORCOUNT];
  Vector        vectors[VECTORCOUNT];
};

} // end of namespace regs64
} // end of namespace arm
} // end of namespace processor
} // end of namespace cxx
} // end of namespace component
} // end of namespace unisim

#endif // __UNISIM_COMPONENT_CXX_PROCESSOR_ARM_REGS64_CPU_HH__
