/*
 *  Copyright (c) 2019,
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

#include <unisim/component/cxx/processor/intel/dba/arch.hh>
#include <iostream>

namespace unisim {
namespace component {
namespace cxx {
namespace processor {
namespace intel {
namespace dba {

ProcessorBase::ProcessorBase()
  : path(0)
  , next_insn_mode(ipjmp)
  , abort(false)
{
  for (FLAG reg; reg.next();)
    flagvalues[reg.idx()] = newRegRead( reg );
  for (FRegID reg; reg.next();)
    fregs[reg.idx()] = newRegRead( reg );
}

// void
// ProcessorBase::FTop::Repr( std::ostream& sink ) const
// {
//   sink << "FpuStackTop";
// }

unisim::util::symbolic::Expr&
ProcessorBase::fpaccess( unsigned reg, bool write )
{
  throw Unimplemented();
}

void
Compat32::IRegID::Repr(std::ostream& sink) const
{
  sink << c_str();
}

void
Intel64::IRegID::Repr(std::ostream& sink) const
{
  sink << c_str();
}

void
ProcessorBase::FLAG::Repr( std::ostream& sink ) const
{
  sink << c_str();
}

void
ProcessorBase::VRegID::Repr(std::ostream& sink) const
{
  sink << char(VmmValue::VPREFIX) << "mm" << std::dec << reg;
}

void
ProcessorBase::VmmIndirectReadBase::Repr( std::ostream& sink ) const
{
  GetVName(sink << "VmmIndirectReadRead<" << GetVSize() << ",");
  sink << ">(";
  for (unsigned idx = 0, end = SubCount()-1; idx < end; ++idx)
    sink << GetSub(idx) << ", ";
  sink << index << ")";
}

void
ProcessorBase::flagwrite( FLAG flag, bit_t fval, bit_t def )
{
  if (unisim::util::symbolic::ConstNodeBase const* cnode = def.expr.ConstSimplify())
    {
      flagvalues[flag.idx()] = dynamic_cast<unisim::util::symbolic::ConstNode<bool> const&>(*cnode).value ? fval.expr : unisim::util::symbolic::binsec::make_undefined_value(bool());
    }
  else
    {
      flagvalues[flag.idx()] = ConditionalMove(def, fval, bit_t(unisim::util::symbolic::binsec::make_undefined_value(bool()))).expr;
    }
}

void
ProcessorBase::finit()
{
  ftop = u8_t(0);
  fcw = u16_t(0x37f);
  for (auto flag : {FLAG::C0, FLAG::C1, FLAG::C2, FLAG::C3})
    flagwrite( flag, bit_t(0) );
}

void
ProcessorBase::fxam()
{
  Expr val = this->fread( 0 ).expr;

  flagwrite( FLAG::C1, bit_t(u64_t(Expr( new unisim::util::symbolic::vector::VTrans<u64_t>( val, 64, 0 ))) >> 63) );

  for (auto flag : {FLAG::C0, FLAG::C2, FLAG::C3})
    flagwrite( flag, bit_t( Expr( new unisim::util::symbolic::binsec::Opaque<bool>({val}) ) ) );
}

void
ProcessorBase::ftopdec()
{
  int last = FREGCOUNT-1;
  f64_t rot = fregs[last];
  for (int idx = last; idx > 0; --idx)
    fregs[idx] = fregs[idx-1];
  fregs[0] = rot;
  ftop = (ftop - u8_t(1)) % u8_t(FREGCOUNT);
}

void
ProcessorBase::ftopinc()
{
  int last = FREGCOUNT-1;
  f64_t rot = fregs[0];
  for (int idx = 0; idx < last; ++idx)
    fregs[idx] = fregs[idx+1];
  fregs[last] = rot;
  ftop = (ftop + u8_t(1)) % u8_t(FREGCOUNT);
}

namespace
{
  template <class OP>
  void
  noexec_error( OP const& op )
  {
    // std::cerr << "error: no execute method in `" << typeid(op).name() << "'\n" << std::hex << op.address << ":\t";
    // op.disasm( std::cerr );
    // std::cerr << '\n';
    throw ProcessorBase::Unimplemented();
  }
}

template <> void Processor<Compat32>::noexec( Processor<Compat32>::Operation const& op ) { noexec_error(op); }
template <> void Processor <Intel64>::noexec( Processor<Intel64>::Operation const& op ) { noexec_error(op); }

void show(unsigned idx, unisim::util::symbolic::ExprNode const* node)
{
  std::cerr << "[" << idx << "]: ";
  if (node)
    node->Repr(std::cerr);
  std::cerr << "\n";
}

} /* end of namespace dba */
} /* end of namespace intel */
} /* end of namespace processor */
} /* end of namespace cxx */
} /* end of namespace component */
} /* end of namespace unisim */
