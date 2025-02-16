/*
 *  Copyright (c) 2018-2020,
 *  Commissariat a l'Energie Atomique (CEA)
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *   - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *   - Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 *   - Neither the name of CEA nor the names of its contributors may be used to
 *     endorse or promote products derived from this software without specific
 *     prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED.
 *  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY
 *  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 *  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors: Yves Lhuillier (yves.lhuillier@cea.fr)
 */

#ifndef __UNISIM_COMPONENT_CXX_PROCESSOR_SPARC_ISA_SV8_ARCH_HH__
#define __UNISIM_COMPONENT_CXX_PROCESSOR_SPARC_ISA_SV8_ARCH_HH__

#include <unisim/util/truth_table/truth_table.hh>
#include <inttypes.h>

namespace unisim {
namespace component {
namespace cxx {
namespace processor {
namespace sparc {
namespace isa {
namespace sv8 {

/** Base class for the SPARC family.
 *
 * This class is an helper base for processors the SPARCv8 family.  It
 * implements common instruction set functions based on assumed architectural implementation.
 */

template <typename ARCH_IMPL>
struct Arch
{
  enum branch_type_t { B_JMP = 0, B_CALL, B_RET };

  /** Check the given condition against the current PSR status.
   * Returns BOOL(true) if the condition holds, BOOL(false) otherwise.
   *
   * @param cond the condition to check
   *
   * @return BOOL(true) if the condition matches PSR status, BOOL(false) otherwise
   */
  bool TestCondition( uint32_t cond )
  {
    typename ARCH_IMPL::Conditions _(*static_cast<ARCH_IMPL*>(this));
    
    switch (cond)
      {
      case  0: return _.Eval(                        _.F()  ); //   n; never
      case  1: return _.Eval(                        _.Z()  ); //   e; equal
      case  2: return _.Eval(   (_.Z() or (_.N() xor _.V()))); //  le; less or equal
      case  3: return _.Eval(             (_.N() xor _.V()) ); //   l; less
      case  4: return _.Eval(             (_.C()  or _.Z()) ); // leu; less or equal unsigned
      case  5: return _.Eval(                        _.C()  ); //  cs; carry set
      case  6: return _.Eval(                        _.N()  ); // neg; negative
      case  7: return _.Eval(                        _.V()  ); //  vs; overflow set
      case  8: return _.Eval(                        _.T()  ); //  al; always
      case  9: return _.Eval(                    not _.Z()  ); //  ne; not equal
      case 10: return _.Eval(not(_.Z() or (_.N() xor _.V()))); //   g; greater
      case 11: return _.Eval(         not (_.N() xor _.V()) ); //  ge; greater or equal
      case 12: return _.Eval(         not (_.C()  or _.Z()) ); //  gu; greater unsigned
      case 13: return _.Eval(                    not _.C()  ); //  cc; carry clear
      case 14: return _.Eval(                    not _.N()  ); // pos; positive
      case 15: return _.Eval(                    not _.V()  ); //  vc; overflow clear
      }
    
    struct Bad {}; throw Bad ();
    return false;
  }

  bool TestFCondition(unsigned cond)
  {
    typename ARCH_IMPL::FConditions _(*static_cast<ARCH_IMPL*>(this));
    
    switch (cond)
      {
      case 0b0000: return _.Eval(          _.F() ); //   n; Never
      case 0b0001: return _.Eval(      not _.E() ); //  ne; Not equal
      case 0b0010: return _.Eval( _.L() or _.G() ); //  lg; Less or Greater
      case 0b0011: return _.Eval( _.U() or _.L() ); //  ul; Unordered or Less 
      case 0b0100: return _.Eval(          _.L() ); //   l; Less
      case 0b0101: return _.Eval( _.U() or _.G() ); //  ug; Unordered or Greater
      case 0b0110: return _.Eval(          _.G() ); //   g; Greater
      case 0b0111: return _.Eval(          _.U() ); //   u; Unordered
      case 0b1000: return _.Eval(          _.T() ); //   a; Always
      case 0b1001: return _.Eval(          _.E() ); //   e; Equal
      case 0b1010: return _.Eval( _.U() or _.E() ); //  ue; Unordered or Equal
      case 0b1011: return _.Eval( _.G() or _.E() ); //  ge; Greater or Equal
      case 0b1100: return _.Eval(      not _.L() ); // uge; Unordered or Greater or Equal
      case 0b1101: return _.Eval( _.L() or _.E() ); //  le; Less or Equal
      case 0b1110: return _.Eval(      not _.G() ); // ule; Unordered or Less or Equal
      case 0b1111: return _.Eval(      not _.U() ); //   o; Ordered
      }
      return false;
  }
};
	
} // end of namespace sv8
} // end of namespace isa
} // end of namespace sparc
} // end of namespace processor
} // end of namespace cxx
} // end of namespace component
} // end of namespace unisim

#define CP15ENCODE( CRN, OPC1, CRM, OPC2 ) ((OPC1 << 12) | (CRN << 8) | (CRM << 4) | (OPC2 << 0))

#endif // __UNISIM_COMPONENT_CXX_PROCESSOR_SPARC_ISA_SV8_ARCH_HH__
