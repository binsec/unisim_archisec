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

#include <unisim/component/cxx/processor/sparc/dba/arch32/decoder.hh>

#include <unisim/component/cxx/processor/sparc/asi.hh>
#include <unisim/component/cxx/processor/sparc/trap.hh>
#include <unisim/component/cxx/processor/sparc/isa/sv8/arch.hh>
#include <unisim/util/symbolic/binsec/binsec.hh>
#include <unisim/util/symbolic/symbolic.hh>
#include <unisim/component/cxx/processor/sparc/dba/sv8/sparc.tcc>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <set>

namespace unisim {
namespace component {
namespace cxx {
namespace processor {
namespace sparc {
namespace dba {
namespace arch32 {

struct Processor : public unisim::component::cxx::processor::sparc::isa::sv8::Arch<Processor>
{
  //   =====================================================================
  //   =                             Data Types                            =
  //   =====================================================================

  typedef unisim::util::symbolic::SmartValue<double>   F64;
  typedef unisim::util::symbolic::SmartValue<float>    F32;
  typedef unisim::util::symbolic::SmartValue<bool>     BOOL;
  typedef unisim::util::symbolic::SmartValue<uint8_t>  U8;
  typedef unisim::util::symbolic::SmartValue<uint16_t> U16;
  typedef unisim::util::symbolic::SmartValue<uint32_t> U32;
  typedef unisim::util::symbolic::SmartValue<uint64_t> U64;
  typedef unisim::util::symbolic::SmartValue<int8_t>   S8;
  typedef unisim::util::symbolic::SmartValue<int16_t>  S16;
  typedef unisim::util::symbolic::SmartValue<int32_t>  S32;
  typedef unisim::util::symbolic::SmartValue<int64_t>  S64;

  typedef unisim::util::symbolic::FP                   FP;
  typedef unisim::util::symbolic::Expr                 Expr;
  typedef unisim::util::symbolic::ValueType            ValueType;
  typedef unisim::util::symbolic::binsec::ActionNode   ActionNode;

  typedef unisim::component::cxx::processor::sparc::dba::sv8::Operation<Processor> Operation;
  typedef unisim::component::cxx::processor::sparc::ASI ASI;
  typedef unisim::component::cxx::processor::sparc::Trap_t::TrapType_t TrapType_t;

  template <typename RID>
  static Expr newRegRead( RID id ) { return new unisim::util::symbolic::binsec::RegRead<RID>( id ); }

  template <typename RID>
  static Expr newRegWrite( RID id, Expr const& value ) { return new unisim::util::symbolic::binsec::RegWrite<RID>( id, value ); }

  struct Flag
    : public unisim::util::identifier::Identifier<Flag>
    , public unisim::util::symbolic::WithValueType<Flag>
  {
    typedef bool value_type;
    enum Code { C, V, Z, N, end } code;

    char const* c_str() const
    {
      unsigned idx( code );
      static char const* names[] = {"c", "v", "z", "n"};
      return (idx < 4) ? names[idx] : "NA";
    }

    void Repr(std::ostream& sink) const { sink << c_str(); }

    Flag() : code(end) {}
    Flag( Code _code ) : code(_code) {}
    Flag( char const* _code ) : code(end) { init( _code ); }
  };

  struct GPR
    : public unisim::util::identifier::Identifier<GPR>
    , public unisim::util::symbolic::WithValueType<GPR>
  {
    typedef uint32_t value_type;

    enum Code { g0, g1, g2, g3, g4, g5, g6, g7, o0, o1, o2, o3, o4, o5, o6, o7, l0, l1, l2, l3, l4, l5, l6, l7, i0, i1, i2, i3, i4, i5, i6, i7, end } code;

    char const* c_str() const
    {
      switch (code)
        {
        case g0: return "g0"; case g1: return "g1"; case g2: return "g2"; case g3: return "g3"; case g4: return "g4"; case g5: return "g5"; case g6: return "g6"; case g7: return "g7";
        case o0: return "o0"; case o1: return "o1"; case o2: return "o2"; case o3: return "o3"; case o4: return "o4"; case o5: return "o5"; case o6: return "o6"; case o7: return "o7";
        case l0: return "l0"; case l1: return "l1"; case l2: return "l2"; case l3: return "l3"; case l4: return "l4"; case l5: return "l5"; case l6: return "l6"; case l7: return "l7";
        case i0: return "i0"; case i1: return "i1"; case i2: return "i2"; case i3: return "i3"; case i4: return "i4"; case i5: return "i5"; case i6: return "i6"; case i7: return "i7";
        case end: break;
        }
      return "NA";
    }

    void Repr( std::ostream& sink ) const { sink << c_str(); }

    GPR() : code(end) {}
    GPR( Code _code ) : code(_code) {}
    GPR( unsigned idx ) : code(Code(idx)) {}
    GPR( char const* _code ) : code(end) { init( _code ); }
  };

  struct YREG
    : public unisim::util::symbolic::WithValueType<YREG>
  {
    typedef uint32_t value_type;
    void Repr( std::ostream& sink ) const { sink << "y"; }
    int cmp( YREG const& rhs ) const { return 0; }
  };

  struct Conditions
  {
    Conditions(Processor& _scanner) : scanner(_scanner) {}
    bool Eval( BOOL&& res ) const { return scanner.Test(res); }
    BOOL F() const { return BOOL(false); }
    BOOL T() const { return BOOL(true); }
    BOOL N() const { return scanner.flags[Flag::N]; }
    BOOL Z() const { return scanner.flags[Flag::Z]; }
    BOOL V() const { return scanner.flags[Flag::V]; }
    BOOL C() const { return scanner.flags[Flag::C]; }
    Processor& scanner;
  };

  struct FConditions
  {
    FConditions(Processor& _scanner) : scanner(_scanner) {}
    bool Eval( BOOL&& res ) const { return scanner.Test(res); }
    BOOL F() const { return BOOL(false); }
    BOOL T() const { return BOOL(true); }
    BOOL E() const { throw 0; return BOOL(false); }
    BOOL G() const { throw 0; return BOOL(false); }
    BOOL L() const { throw 0; return BOOL(false); }
    BOOL U() const { throw 0; return BOOL(false); }
    Processor& scanner;
  };

  // //   =====================================================================
  // //   =                      Construction/Destruction                     =
  // //   =====================================================================

  Processor(U32 const& insn_addr)
    : path(0)
    , gpr()
    , yreg(newRegRead(YREG()))
    , flags()
    , current_instruction_address(insn_addr)
    , branch_addr()
    , branch_type(B_JMP)
    , branch_annul(false)
    , delay_slot(false)
    , unpredictable(false)
    , stores()
  {
    for (GPR reg; reg.next();)
      gpr[reg.idx()] = newRegRead(reg);
    for (Flag flag; flag.next();)
      flags[flag.idx()] = newRegRead(flag);
  }

  bool
  close( Processor const& ref, uint32_t linear_nia )
  {
    bool complete = path->close();

    if (branch_addr.expr.node)
      {
        if (branch_type == B_CALL)
          path->add_sink( new unisim::util::symbolic::binsec::Call<uint32_t>( branch_addr.expr, linear_nia ) );
        else
          path->add_sink( new unisim::util::symbolic::binsec::Branch( branch_addr.expr ) );
      }

    if (unpredictable)
      {
        path->add_sink( new unisim::util::symbolic::binsec::AssertFalse() );
        return complete;
      }

    for (GPR reg; reg.next();)
      if (gpr[reg.idx()].expr != ref.gpr[reg.idx()].expr)
        path->add_sink( newRegWrite( reg, gpr[reg.idx()].expr ) );

    if (yreg.expr != ref.yreg.expr)
      path->add_sink( newRegWrite( YREG(), yreg.expr ) );

    for (Flag flag; flag.next();)
      if (flags[flag.idx()].expr != ref.flags[flag.idx()].expr)
        path->add_sink( newRegWrite( flag, flags[flag.idx()].expr ) );

    for (std::set<Expr>::const_iterator itr = stores.begin(), end = stores.end(); itr != end; ++itr)
      path->add_sink( *itr );

    return complete;
  }

  //   =====================================================================
  //   =                 Internal Instruction Control Flow                 =
  //   =====================================================================

  struct Illegal {};
  struct Unimplemented {};
  void UndefinedInstruction(Operation const&) { throw Unimplemented(); }

  bool concretize( Expr cexp )
  {
    if (unisim::util::symbolic::ConstNodeBase const* cnode = cexp.ConstSimplify())
      return dynamic_cast<unisim::util::symbolic::ConstNode<bool> const&>(*cnode).value;

    bool predicate = path->proceed( cexp );
    path = path->next( predicate );
    return predicate;
  }

  template <typename T>
  bool Test( unisim::util::symbolic::SmartValue<T> const& cond )
  {
    if (not cond.expr.good())
      throw std::logic_error( "Not a valid condition" );

    return concretize( BOOL(cond).expr );
  }

  //   =====================================================================
  //   =             General Purpose Registers access methods              =
  //   =====================================================================

  void SetGPR( unsigned id, U32 const& value ) { if (id != 0) gpr[id] = value; }
  U32  GetGPR( unsigned id ) { return id == 0 ? U32(0) : gpr[id];  }
  U32  GetY() { return yreg; }
  void SetY(U32 const& value) { yreg = value; }

  //   =====================================================================
  //   =                  Arithmetic Flags access methods                  =
  //   =====================================================================

  void SetNZVC(BOOL const& N, BOOL const& Z, BOOL const& V, BOOL const& C)
  {
    flags[Flag::N] = N; flags[Flag::Z] = Z; flags[Flag::V] = V; flags[Flag::C] = C;
  }
  BOOL GetCarry() { return flags[Flag::C]; }
  BOOL GetN() { return flags[Flag::N]; }
  BOOL GetV() { return flags[Flag::V]; }
  BOOL GetET() { throw 0; return BOOL(); }
  void SetET(BOOL) { throw 0; }
  BOOL GetS() { throw 0; return BOOL(); }
  void SetS(BOOL) { throw 0; }
  BOOL GetPS() { throw 0; return BOOL(); }
  void SetPS(BOOL) { throw 0; }

  //   ====================================================================
  //   =                 Vector  Registers access methods
  //   ====================================================================

  void SetF32( unsigned id, F32 const& value ) { throw 0; }
  void SetF64( unsigned id, F64 const& value ) { throw 0; }
  void SetS32( unsigned id, S32 const& value ) { throw 0; }
  F32  GetF32( unsigned id ) { throw 0; return F32(); }
  F64  GetF64( unsigned id ) { throw 0; return F64(); }
  S32  GetS32( unsigned id ) { throw 0; return S32(); }
  void SetFCC( BOOL const& l, BOOL const& g ) { throw 0; }
  U32  GetFSR() { throw 0; return U32(); }
  void SetFSR( U32 const& ) { throw 0; }

  //   =====================================================================
  //   =              Special/System Registers access methods              =
  //   =====================================================================

  bool asr_perm(unsigned) { return false; }
  U32 rdasr(unsigned) { throw 0; return U32(); }
  void wrasr(unsigned, U32) { throw 0; }

  bool InvalidWindow(int) { return false; }
  void RotateWindow(int x) {
    /* always acts as if the old window were saved in the stack */
    /* it is a way to abstract away the complexity of windows which
       mix both exception and kernel handler */
    if (x < 0) { /* save operation */
      U32 s = GetGPR(GPR::o6);
      for (int i = 0; i < 16; i += 1) {
	MemWrite( ASI::user_data, s + U32(4 * i),
		  GetGPR( GPR::l0 + i ) );
      }
      for (int i = 0; i < 8; i += 1) {
	SetGPR( GPR::i0 + i, GetGPR( GPR::o0 + i ) );
	SetGPR( GPR::l0 + i, U32(0) );
	SetGPR( GPR::o0 + i, U32(0) );
      }
    } else { /* restore operation */
      U32 s = GetGPR(GPR::i6);
      for (int i = 0; i < 8; i += 1) {
	SetGPR( GPR::o0 + i, GetGPR( GPR::i0 + i ) );
      }
      for (int i = 0; i < 16; i += 1) {
	SetGPR( GPR::l0 + i, MemRead( U32(), ASI::user_data, s + U32(4 * i) ) );
      }
    }
  }
  unsigned nwindows() { throw 0; return 0; }

  bool IsSuper() const { return false; }

  U32 GetPSR() { throw 0; return U32(); }
  U32 GetWIM() { throw 0; return U32(); }
  U32 GetTBR() { throw 0; return U32(); }
  void SetPSR(U32) { throw 0; }
  void SetWIM(U32) { throw 0; }
  void SetTBR(U32) { throw 0; }

  //   =====================================================================
  //   =                      Control Transfer methods                     =
  //   =====================================================================

  U32  GetPC() { return current_instruction_address; }
  void DelayBranch(U32 target, branch_type_t bt )
  {
    branch_addr = target;
    branch_type = bt;
    delay_slot = true;
  }
  void SetAnnul(bool annul)
  {
    branch_annul = annul;
    delay_slot = true;
  }

  void Abort( TrapType_t trap ) { throw 0; }
  void StopFetch() { throw 0; }
  void SWTrap(U32) { throw 0; }

  //   =====================================================================
  //   =                       Memory access methods                       =
  //   =====================================================================

  template <typename T> T MemRead( T const&, ASI asi, U32 const& addr )
  {
    if (asi.code != asi.user_data) throw 0;
    auto tp = T::GetType();
    return T( Expr(new unisim::util::symbolic::binsec::Load( addr.expr, tp.bitsize/8, 0, true )) );
  }
  template <typename T> void MemWrite(ASI asi, U32 const& addr, T const& data)
  {
    if (asi.code != asi.user_data) throw 0;
    auto tp = T::GetType();
    stores.insert( new unisim::util::symbolic::binsec::Store( addr.expr, data.expr, tp.bitsize/8, 0, true ) );
  }

  template <typename T> T FMemRead( T const&, ASI asi, U32 const& addr ) { throw 0; return T(); }
  template <typename T> void FMemWrite( ASI asi, U32 const& addr, T const& ) { throw 0; }

  unisim::component::cxx::processor::sparc::ASI rqasi() const { return ASI::user_data; }

  ActionNode*      path;
  U32              gpr[32];
  U32              yreg;
  BOOL             flags[Flag::end];
  U32              current_instruction_address;
  U32              branch_addr;
  branch_type_t    branch_type;
  bool             branch_annul, delay_slot;
  bool             unpredictable;
  std::set<Expr>   stores;
};

struct Translator
{
  typedef unisim::component::cxx::processor::sparc::dba::sv8::Decoder<Processor> Decoder;
  typedef unisim::component::cxx::processor::sparc::dba::sv8::Operation<Processor> Operation;

  typedef unisim::util::symbolic::binsec::ActionNode ActionNode;

  Translator( uint32_t _addr, uint32_t _code0, uint32_t _code1 )
    : coderoot(new ActionNode), addr(_addr), code0(_code0), code1(_code1)
  {}
  ~Translator() { delete coderoot; }

  void
  translate( std::ostream& sink )
  {
    typedef unisim::util::symbolic::binsec::dbx print_dbx;
    sink << "(address . " << print_dbx(4, addr) << ")\n";

    struct Instruction
    {
      Instruction(uint32_t addr, uint32_t code)
        : operation(0)
      {
        Decoder decoder;
        operation = decoder.NCDecode( addr, code );
      }
      ~Instruction() { delete operation; }
      Operation* operator -> () { return operation; }
      void disassemble(std::ostream& sink) const
      {
        try { operation->disasm( sink, operation->GetAddr() ); }
        catch (...) { sink << "(bad)"; }
      }

      Operation* operation;
    };

    Instruction instruction0(addr, code0), instruction1(addr+4, code1);

    Processor::U32      insn_addr = unisim::util::symbolic::make_const(addr); //< concrete instruction address
    // Processor::U64      insn_addr = Expr(new InstructionAddress); //< symbolic instruction address
    Processor reference( insn_addr );

    struct
    {
      uint32_t size;
      Instruction& insn0;
      Instruction& insn1;
      uint32_t code0() { return insn0->GetEncoding(); }
      uint32_t code1() { return insn1->GetEncoding(); }
      void write(std::ostream& sink, char const* finish="")
      {
        switch (size)
          {
          default:
            sink << "(opcode . " << print_dbx(4, code0()) << ")\n";
            sink << "(mnemonic . \"";
            insn0.disassemble(sink);
            sink << "\")\n";
            break;
          case 8:
            sink << "(opcode . " << print_dbx(8, uint64_t(code0()) << 32 | code1()) << ")\n";
            sink << "(mnemonic . \"";
            insn0.disassemble(sink);
            sink << " | ";
            insn1.disassemble(sink);
            sink << "\")\n";
            break;
          }
        sink << "(size . " << size << ")\n" << finish;
      }
    } headers{4, instruction0, instruction1};

    // Get actions
    try
      {
        unsigned ds_state = 0;
        try
          {
            for (bool end = false; not end;)
              {
                Processor state( reference );
                state.path = coderoot;
                instruction0->execute( state );
                ds_state |= 1 << state.delay_slot;
                if (state.delay_slot)
                  {
                    if (not state.branch_addr.expr.node)
                      state.branch_addr = insn_addr + Processor::U32(8);
                    if (not state.branch_annul)
                      {
                        state.delay_slot = false;
                        instruction1->execute(state);
                        if (state.delay_slot)
                          throw Processor::Illegal();
                      }
                  }
                else
                  {
                    if (state.branch_addr.expr.node)
                      throw Processor::Illegal();
                    state.branch_addr = insn_addr + Processor::U32(4);
                  }
                end = state.close( reference, addr + 4 );
              }
          }
        catch (Processor::Illegal const&)
          {
            throw;
          }
        catch (...)
          {
            headers.write(sink, "(unimplemented)\n");
            return;
          }
        switch (ds_state)
          {
          case 1: break;
          case 2: headers.size = 8; break;
          default:
            throw Processor::Illegal();
          }
        coderoot->simplify();
        coderoot->commit_stats();
      }
    catch (...)
      {
        headers.write(sink, "(illegal)\n");
        return;
      }
    headers.write(sink);

    // Translate to DBA
    coderoot->generate(sink, 8, addr);
  }

  ActionNode*               coderoot;
  uint32_t                  addr;
  uint32_t                  code0, code1;
};

void
Decoder::process( std::ostream& sink, uint32_t addr, uint32_t code0, uint32_t code1 )
{
  Translator translator( addr, code0, code1 );

  translator.translate( sink );
}

} /* end of namespace arch32 */
} /* end of namespace dba */
} /* end of namespace sparc */
} /* end of namespace processor */
} /* end of namespace cxx */
} /* end of namespace component */
} /* end of namespace unisim */

