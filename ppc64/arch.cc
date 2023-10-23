/*
 *  Copyright (c) 2022-2023,
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

#include "arch.hh"
#include <iostream>
#include <iomanip>

namespace ppc64 {

  Arch::Arch( U64 const& insn_addr, Path& _path )
    : path(_path)
    , current_instruction_address(insn_addr)
    , next_instruction_address(insn_addr + U64(4))
    , branch_type(B_JMP)
    , cr(*this)
    , xer(*this)
    , fpscr(*this)
  {
    for (IRegID reg; reg.next();)
      regvalues[reg.idx()] = newRegRead( reg );
    for (FRegID reg; reg.next();)
      fregvalues[reg.idx()].expr = newRegRead( reg );
  }

  namespace
  {
    struct CRBIT : public unisim::util::symbolic::WithValueType<CRBIT>
    {
      typedef bool value_type;
      CRBIT(unsigned _bit) : bit(_bit) {}
      void Repr(std::ostream& sink) const { sink << "cr" << (bit / 4) << &"lt\0gt\0eq\0so"[(bit%4)*3]; }
      int cmp(CRBIT rhs) const { return int(bit) - int(rhs.bit); }
      unsigned bit;
    };
    
    struct BitRead : public Path::Source, public unisim::util::symbolic::binsec::RegRead<CRBIT>
    {
      BitRead( Path& path, CRBIT bit ) : Path::Source(path), unisim::util::symbolic::binsec::RegRead<CRBIT>(bit) {}
      virtual BitRead* Mutate() const override { return new BitRead(*this); }
    };
  }

  // Arch::CR::CR( Arch& arch ) : value(U32(0))
  // {
  //   for (unsigned idx = 32; idx-- > 0;) {
  //     Expr scratch = new BitRead( arch.path, CRBIT(31^idx) );
  //     scratch = unisim::util::symbolic::binsec::BitFilter::
  //       mksimple(scratch, 1, 0, 1, 32, false);
  //     value = value | (U32(scratch) << U32(idx));
  //   }
  // }

  unisim::util::symbolic::ConstNodeBase const*
  Arch::CR::Read::Simplify( Expr const& mask, Expr& expr ) const
  {
    if (not mask.good())
      return 0;

    uint32_t maskval = mask.Eval<uint32_t>();
    if (maskval & (maskval-1))
      return 0;

    unisim::util::symbolic::shift_type pos = __builtin_ctz(maskval);
    Expr scratch = new BitRead( path, CRBIT(31^pos) );
    scratch = unisim::util::symbolic::binsec::BitFilter::mksimple(scratch, 1, 0, 1, 32, false);
    expr = unisim::util::symbolic::make_operation(unisim::util::symbolic::Op::Lsl, scratch, unisim::util::symbolic::make_const(pos));

    return 0;
  }

  bool
  Arch::close( Arch const& ref, uint64_t linear_nia )
  {
    bool complete = path->close();
    /* TODO: branch_type */
    path->add_sink( Expr( new unisim::util::symbolic::binsec::Branch( next_instruction_address.expr ) ) );

    for (IRegID reg; reg.next();)
      if (regvalues[reg.idx()].expr != ref.regvalues[reg.idx()].expr)
        path->add_sink( newRegWrite( reg, regvalues[reg.idx()].expr ) );

    if (cr.value.expr != ref.cr.value.expr)
      {
        // cr.value.expr->Repr(std::cerr << "cr: ");
        // std::cerr << '\n';

        for (unsigned idx = 32; idx-- > 0;)
          {
            Expr
              new_crbit = unisim::util::symbolic::binsec::BitFilter::mksimple(cr.value.expr, 32, 31 ^ idx, 1, 1, false),
              old_crbit = unisim::util::symbolic::binsec::BitFilter::mksimple(ref.cr.value.expr, 32, 31 ^ idx, 1, 1, false);

            // unisim::util::symbolic::binsec::BitSimplify::Do(new_crbit);
            // unisim::util::symbolic::binsec::BitSimplify::Do(old_crbit);
            if (new_crbit == old_crbit)
              continue;
            path->add_sink( newRegWrite( CRBIT(idx), new_crbit ) );
            // new_crbit->Repr(std::cerr << "cr<" << idx << ">: ");
            // std::cerr << '\n';
          }
      }

    for (std::set<Expr>::const_iterator itr = stores.begin(), end = stores.end(); itr != end; ++itr)
      path->add_sink( *itr );

    if (xer.value.expr != ref.xer.value.expr) {
      unisim::util::symbolic::Expr x(((xer.value ^ ref.xer.value) != U64(0)).expr);
      if (auto c = unisim::util::symbolic::binsec::BitSimplify::Do(x)) {
	if (dynamic_cast<unisim::util::symbolic::ConstNode<bool> const&>(*c).value)
	  path->add_sink( newRegWrite( XER::ID(), xer.value.expr ) );
      } else
	path->add_sink( newRegWrite( XER::ID(), xer.value.expr ) );
    }

    if (fpscr.value.expr != ref.fpscr.value.expr)
      throw 0;

    return complete;
  }

  void
  Arch::IRegID::Repr(std::ostream& sink) const
  {
    sink << c_str();
  }

  void
  Arch::FRegID::Repr(std::ostream& sink) const
  {
    sink << c_str();
  }

  void ppc64::Arch::FPSCR::ID::Repr(std::ostream& sink) const { sink << "fpscr"; }
  void ppc64::Arch::XER::ID::Repr(std::ostream& sink) const { sink << "xer"; }
  void ppc64::Arch::CR::ID::Repr(std::ostream& sink) const { sink << "cr"; }
} // end of namespace ppc64

