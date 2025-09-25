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

/**************************************************************/
/* Disassembling methods                                      */
/**************************************************************/

#ifndef __UNISIM_COMPONENT_CXX_PROCESSOR_SPARC_ISA_SV8_DISASM_HH__
#define __UNISIM_COMPONENT_CXX_PROCESSOR_SPARC_ISA_SV8_DISASM_HH__

#include <iostream>
#include <inttypes.h>

namespace unisim {
namespace component {
namespace cxx {
namespace processor {
namespace sparc {
namespace isa {
namespace sv8 {

  struct DisasmObject
  {
    virtual void operator() ( std::ostream& sink ) const = 0;
    virtual ~DisasmObject() {};
    friend std::ostream& operator << ( std::ostream& sink, DisasmObject const& dobj );
  };
  
  struct DisasmGPR : public DisasmObject
  {
    DisasmGPR( unsigned _rid ) : rid(_rid) {} unsigned rid;
    void operator () ( std::ostream& sink ) const { sink << '%' << "goli"[rid / 8] << std::dec << (rid % 8); }
  };

  struct DisasmFPR : public DisasmObject
  {
    DisasmFPR( unsigned _rid ) : rid(_rid) {} unsigned rid;
    void operator () ( std::ostream& sink ) const { sink << "%f" << std::dec << rid; }
  };

  struct DisasmCPR : public DisasmObject
  {
    DisasmCPR( unsigned _rid ) : rid(_rid) {} unsigned rid;
    void operator () ( std::ostream& sink ) const { sink << "%c" << std::dec << rid; }
  };

  struct DisasmI : public DisasmObject
  {
    DisasmI(int32_t _value) : value(_value) {} int32_t value;
    void operator () ( std::ostream& sink ) const { sink << std::dec << value; }
  };

  struct DisasmHex : public DisasmObject
  {
    DisasmHex(uint32_t _value) : intro("-"), value(_value) {}
    DisasmHex(int32_t _value, char const* n, char const* p="") : intro(_value < 0 ? n : p), value(_value < 0 ? -_value : _value ) {}
    void operator () ( std::ostream& sink ) const;
    char const* intro;
    uint32_t value;
    
  };

  struct DisasmRIAddress : public DisasmObject
  {
    DisasmRIAddress( unsigned _rid, int _offset ) : rid(_rid), offset(_offset) {} unsigned rid; int offset;
    void operator () ( std::ostream& sink ) const;
  };
  
  struct DisasmRRAddress : public DisasmObject
  {
    DisasmRRAddress( unsigned _rs1, unsigned _rs2 ) : rs1(_rs1), rs2(_rs2) {} unsigned rs1; unsigned rs2;
    void operator () ( std::ostream& sink ) const;
  };

  struct DisasmCondition : public DisasmObject
  {
    DisasmCondition( unsigned _cond ) : cond(_cond) {} unsigned cond;
    static char const* cond_name(unsigned);
    void operator () (std::ostream& sink) const;
  };
  
  struct DisasmFCondition : public DisasmObject
  {
    DisasmFCondition( unsigned _cond ) : cond(_cond) {} unsigned cond;
    static char const* cond_name(unsigned);
    void operator () (std::ostream& sink) const;
  };
  
} // end of namespace sv8
} // end of namespace isa
} // end of namespace sparc
} // end of namespace processor
} // end of namespace cxx
} // end of namespace component
} // end of namespace unisim

#endif /* __UNISIM_COMPONENT_CXX_PROCESSOR_SPARC_ISA_SV8_DISASM_HH__ */
