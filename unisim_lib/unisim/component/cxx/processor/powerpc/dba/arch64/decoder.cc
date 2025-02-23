/*
 *  Copyright (c) 2022,
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

#include <unisim/component/cxx/processor/powerpc/dba/arch64/decoder.hh>
#include <unisim/component/cxx/processor/powerpc/dba/arch64/arch.hh>
#include <unisim/component/cxx/processor/powerpc/dba/ppc64/ppc64.hh>

#include <unisim/util/symbolic/binsec/binsec.hh>

namespace unisim {
namespace component {
namespace cxx {
namespace processor {
namespace powerpc {
namespace dba {
namespace arch64 {

struct TBD {};

struct Translator
{
  typedef ppc64::Decoder Decoder;
  typedef ppc64::Operation Operation;

  typedef unisim::util::symbolic::binsec::ActionNode ActionNode;

  Translator( uint64_t _addr, uint32_t _code )
    : coderoot(new ActionNode), addr(_addr), code(_code)
  {}
  ~Translator() { delete coderoot; }

  void
  translate( std::ostream& sink )
  {
    sink << "(address . " << unisim::util::symbolic::binsec::dbx(8, addr) << ")\n";
    sink << "(opcode . " << unisim::util::symbolic::binsec::dbx(4, code) << ")\n";
    sink << "(size . 4)\n";

    struct Instruction
    {
      Instruction(uint32_t addr, uint32_t code)
        : operation(0)
      {
        try
          {
            Decoder decoder;
            operation = decoder.NCDecode( addr, code );
          }
        catch (TBD const&)
          { operation = 0; }
      }
      ~Instruction() { delete operation; }
      Operation* operator -> () { return operation; }

      Operation* operation;
    };

    Instruction instruction(addr, code);

    if (not instruction.operation)
      {
        sink << "(illegal)\n";
        return;
      }

    U64      insn_addr = unisim::util::symbolic::make_const(addr); //< concrete instruction address
    // Arch::U64      insn_addr = Expr(new InstructionAddress); //< symbolic instruction address
    Path path;
    Arch reference( insn_addr, path );

    // Disassemble
    sink << "(mnemonic . \"";
    try { instruction->disasm( sink ); }
    catch (...) { sink << "(bad)"; }
    sink << "\")\n";

    // Get actions
    try
      {
        for (bool end = false; not end;)
          {
            Arch state( reference );
	    path.reset(coderoot);
            instruction->execute( &state );
            end = state.close( reference, addr + 4 );
          }
        coderoot->simplify();
        coderoot->commit_stats();
      }
    catch (...)
      {
        sink << "(unimplemented)\n";
        return;
      }

    // Translate to DBA
    coderoot->generate(sink, 8, addr);
  }

  ActionNode*          coderoot;
  uint64_t             addr;
  uint32_t             code;
};

void
Decoder::process( std::ostream& sink, uint64_t addr, uint32_t code )
{
  Translator translator( addr, code );

  translator.translate( sink );
}

} /* end of namespace arch64 */
} /* end of namespace dba */
} /* end of namespace powerpc */
} /* end of namespace processor */
} /* end of namespace cxx */
} /* end of namespace component */
} /* end of namespace unisim */

