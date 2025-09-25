/*
 *  Copyright (c) 2019-2020,
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

#include <inttypes.h>
#include <iostream>
#include <sstream>

#include <unisim/component/cxx/processor/sparc/isa/sv8/disasm.hh>

namespace unisim {
namespace component {
namespace cxx {
namespace processor {
namespace sparc {
namespace isa {
namespace sv8 {

  std::ostream&
  operator << ( std::ostream& sink, DisasmObject const& dobj )
  {
    dobj( sink );
    return sink;
  }

  void DisasmHex::operator () ( std::ostream& sink ) const
  {
    sink << intro << "0x" << std::hex << value;
  }

  void DisasmRIAddress::operator () ( std::ostream& sink ) const
  {
    sink << DisasmGPR(rid) << DisasmHex(offset, " - ", " + ");
  }
  
  void DisasmRRAddress::operator () ( std::ostream& sink ) const
  {
    sink << DisasmGPR(rs1) << " + " << DisasmGPR(rs2);
  }

  void DisasmFCondition::operator () ( std::ostream& sink ) const
  {
    sink << cond_name(cond);
  }

  char const* DisasmFCondition::cond_name(unsigned code)
  {
    switch (code)
      {
      case 0b0000: return "n";   // Never
      case 0b0001: return "ne";  // Not equal
      case 0b0010: return "lg";  // Less or Greater
      case 0b0011: return "ul";  // Unordered or less 
      case 0b0100: return "l";   // Less 
      case 0b0101: return "ug";  // Unordered or Greater
      case 0b0110: return "g";   // Greater
      case 0b0111: return "u";   // Unordered
      case 0b1000: return "a";   // Always
      case 0b1001: return "e";   // Equal
      case 0b1010: return "ue";  // Unordered or Equal
      case 0b1011: return "ge";  // Greater or Equal
      case 0b1100: return "uge"; // Unordered or Greater or Equal
      case 0b1101: return "le";  // Less or Equal
      case 0b1110: return "ule"; // Unordered or Less or Equal
      case 0b1111: return "o";   // Ordered
      }
    return "?";
  }
  
  void DisasmCondition::operator () ( std::ostream& sink ) const
  {
    sink << cond_name(cond);
  }

  char const* DisasmCondition::cond_name(unsigned code)
  {
    switch (code)
      {
      case 0b0000: return "n";   // never
      case 0b0001: return "e";   // equal
      case 0b0010: return "le";  // less or equal
      case 0b0011: return "l";   // less
      case 0b0100: return "leu"; // less or equal unsigned
      case 0b0101: return "cs";  // carry set
      case 0b0110: return "neg"; // negative
      case 0b0111: return "vs";  // overflow set
      case 0b1000: return "a";   // always
      case 0b1001: return "ne";  // not equal
      case 0b1010: return "g";   // greater
      case 0b1011: return "ge";  // greater or equal
      case 0b1100: return "gu";  // greater unsigned
      case 0b1101: return "cc";  // carry clear
      case 0b1110: return "pos"; // positive
      case 0b1111: return "vc";  // overflow clear
      }
    return "?";
  }
  
} // end of namespace sv8
} // end of namespace isa
} // end of namespace sparc
} // end of namespace processor
} // end of namespace cxx
} // end of namespace component
} // end of namespace unisim
