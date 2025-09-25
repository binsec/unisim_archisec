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

#ifndef __UNISIM_COMPONENT_CXX_PROCESSOR_POWERPC_DBA_ARCH64_ARCH_HH__
#define __UNISIM_COMPONENT_CXX_PROCESSOR_POWERPC_DBA_ARCH64_ARCH_HH__

#include <unisim/component/cxx/processor/powerpc/dba/types.hh>
#include <unisim/util/symbolic/binsec/binsec.hh>
#include <unisim/util/symbolic/symbolic.hh>
#include <inttypes.h>

namespace unisim {
namespace component {
namespace cxx {
namespace processor {
namespace powerpc {
namespace dba {
namespace arch64 {

  struct Path
  {
    using ActionNode = unisim::util::symbolic::binsec::ActionNode;

    struct Source
    {
      Source( Path& _path ) : path(_path) {} Path& path;
    };

    Path() : node() {}

    void reset(ActionNode* root) { node = root; }

    static bool Test( bool c ) { return c; }

    template <typename T>
    static bool Test( unisim::util::symbolic::SmartValue<T> const& cond )
    {
      if (not cond.expr.good())
        throw std::logic_error( "Not a valid condition" );

      return concretize( BOOL(cond).expr );
    }

    ActionNode* operator -> () { return node; }

  private:
    using Expr = unisim::util::symbolic::Expr;

    static bool concretize( Expr cexp )
    {
      if (unisim::util::symbolic::ConstNodeBase const* cnode = cexp.ConstSimplify())
        return dynamic_cast<unisim::util::symbolic::ConstNode<bool> const&>(*cnode).value;

      Path* path = SeekSource(cexp);
      if (not path)
        throw std::logic_error( "No concretization path" );

      return path->choose( cexp );
    }

    static Path* SeekSource( unisim::util::symbolic::Expr const& expr )
    {
      if (Source const* node = dynamic_cast<Source const*>( expr.node ))
        return &node->path;
      for (unsigned idx = 0, end = expr->SubCount(); idx < end; ++idx)
        if (Path* found = SeekSource( expr->GetSub(idx)))
          return found;
      return 0;
    }

    bool choose( Expr const& cexp )
    {
      bool predicate = node->proceed( cexp );
      node = node->next( predicate );
      return predicate;
    }

    ActionNode*     node;
  };

  struct Arch
  {
    static const bool HAS_FPU = true;
    static const bool HAS_FLOATING_POINT_GRAPHICS_INSTRUCTIONS = true;
    //   =====================================================================
    //   =                             Data Types                            =
    //   =====================================================================

    //    using FP = unisim::util::symbolic::FP;
    using Expr = unisim::util::symbolic::Expr;
    using ExprNode = unisim::util::symbolic::ExprNode;
    using ValueType = unisim::util::symbolic::ValueType;
    using ActionNode = unisim::util::symbolic::binsec::ActionNode;
    using BitInsertNode = unisim::util::symbolic::binsec::BitInsertNode;

    template <typename T>
    using SmartValue = unisim::util::symbolic::SmartValue<T>;

    template <typename RID>
    Expr newRegRead( RID id )
    {
      struct RegRead : public Path::Source, public unisim::util::symbolic::binsec::RegRead<RID>
      {
        RegRead( Path& path, RID id ) : Source(path), unisim::util::symbolic::binsec::RegRead<RID>(id) {}
        virtual RegRead* Mutate() const override { return new RegRead(*this); }
      };
      return new RegRead( path, id );
    }

    template <typename RID>
    Expr newRegWrite( RID id, Expr const& value ) { return new unisim::util::symbolic::binsec::RegWrite<RID>( id, value ); }

    struct Unimplemented {};

    Arch(U64 const& insn_addr, Path& path);

    bool  close( Arch const& ref, uint64_t linear_nia );

    //   =====================================================================
    //   =                         Condition Register                        =
    //   =====================================================================

    struct CR
    {
      template <unsigned IDX>
      struct CRn { struct LT { enum {pos=4*IDX+0}; }; struct GT { enum {pos=4*IDX+1}; }; struct EQ { enum {pos=4*IDX+2}; }; struct SO { enum {pos=4*IDX+3}; }; };
      typedef CRn<0> CR0; typedef CRn<1> CR1; typedef CRn<2> CR2; typedef CRn<3> CR3;
      typedef CRn<4> CR4; typedef CRn<5> CR5; typedef CRn<6> CR6; typedef CRn<7> CR7;

      template <typename PART> void Write(PART, Expr const& bit) { value = Expr(new BitInsertNode(value.expr, bit, 31 ^ PART::pos, 1 )); }
      template <unsigned IDX> void Write(CRn<IDX>, Expr const& bits) { value = Expr(new BitInsertNode(value.expr, bits, 4*(7 ^ IDX), 4 )); }
      template <typename PART,typename T> void Set( SmartValue<T> const& bit ) { Write(PART(), U32(bit).expr); }
      template <typename PART> void Set( uint32_t value ) { Write(PART(), unisim::util::symbolic::make_const(value) ); }
      // template <typename PART> U32 Get() { CRAccess(false); return cr_value; }
      operator U32 () { return value; }
      CR& operator= ( U32 const& _value ) { value = _value; return *this; }

      struct ID : public unisim::util::symbolic::WithValueType<ID> { typedef uint32_t value_type; void Repr(std::ostream& sink) const; int cmp(ID) const { return 0; } };

      struct Read : public Path::Source, public unisim::util::symbolic::binsec::RegRead<ID>
      {
        Read( Path& path ) : Source(path), unisim::util::symbolic::binsec::RegRead<ID>(ID()) {}
        virtual Read* Mutate() const override { return new Read(*this); }
        virtual unisim::util::symbolic::ConstNodeBase const* Simplify( Expr const& mask, Expr& expr ) const override;
      };

      CR( Arch& arch ) : value( new Read(arch.path) ) {}

      U32 value;
    };

    CR& GetCR() { return cr; }

    struct XER
    {
      struct SO { enum {pos = 32}; }; struct OV { enum {pos = 33}; }; struct CA { enum {pos = 34}; };
      struct _0_3 {};

      void Write(_0_3,Expr const& bits) { value = Expr(new BitInsertNode(value.expr, bits, 28, 4)); }
      template <typename PART> void Write(PART,Expr const& bit) { value = Expr(new BitInsertNode(value.expr, bit, 63 ^ PART::pos, 1)); }
      template <typename PART,typename T> void Set( SmartValue<T> const& bit ) { Write( PART(), U64(bit).expr ); }
      template <typename PART> void Set( uint64_t value ) { Set<PART,uint64_t>( unisim::util::symbolic::make_const(value) ); }
      template <typename PART> U64 Get() { return Read(PART()); }
      template <typename PART> U64 Read(PART) { return value >> (63 ^ PART::pos) & U64(1); }
      U64 Read(_0_3) { return value >> 28 & U64(0xf); }
      operator U64 () { return value; }
      XER& operator= ( U64 const& _value ) { value = _value; return *this; }

      struct ID : public unisim::util::symbolic::WithValueType<ID> { typedef uint64_t value_type; void Repr(std::ostream& sink) const; int cmp(ID) const { return 0; } };

      XER( Arch& arch ) : value( arch.newRegRead(ID()) ) {}

      U64 value;
    };

    XER& GetXER() { return xer; }

    struct MSR
    {
      struct FP {};
      template <typename PART> bool Get() { return Read(PART()); }
      bool Read(FP) { return true; }
    };

    MSR& GetMSR() { return msr; }

    //   =====================================================================
    //   =                         Integer registers                         =
    //   =====================================================================

    struct IRegID
      : public unisim::util::identifier::Identifier<IRegID>
      , public unisim::util::symbolic::WithValueType<IRegID>
    {
      typedef uint64_t value_type;

      enum Code {
        r0=0, r1,  r2,  r3,  r4,  r5,  r6,  r7,  r8,  r9, r10, r11, r12, r13, r14, r15,
        r16, r17, r18, r19, r20, r21, r22, r23, r24, r25, r26, r27, r28, r29, r30, r31,
        ctr, lr,
        end
      } code;

      char const* c_str() const
      {
        switch (code)
          {
          case r0:  return "r0";  case r1:  return "r1";  case r2:  return "r2";  case r3:  return "r3";
          case r4:  return "r4";  case r5:  return "r5";  case r6:  return "r6";  case r7:  return "r7";
          case r8:  return "r8";  case r9:  return "r9";  case r10: return "r10"; case r11: return "r11";
          case r12: return "r12"; case r13: return "r13"; case r14: return "r14"; case r15: return "r15";
          case r16: return "r16"; case r17: return "r17"; case r18: return "r18"; case r19: return "r19";
          case r20: return "r20"; case r21: return "r21"; case r22: return "r22"; case r23: return "r23";
          case r24: return "r24"; case r25: return "r25"; case r26: return "r26"; case r27: return "r27";
          case r28: return "r28"; case r29: return "r29"; case r30: return "r30"; case r31: return "r31";
          case ctr: return "ctr";          case lr:  return "lr";
          case end: break;
          }
        return "<bad>";
      }

      void Repr( std::ostream& sink ) const;

      IRegID() : code(end) {}
      IRegID( Code _code ) : code(_code) {}
      IRegID( unsigned idx ) : code(Code(idx)) {}
      IRegID( char const* _code ) : code(end) { init( _code ); }
    };

    typedef U64 CTR;
    // struct CTR
    // {
    //   CTR( Arch& _arch ) : arch( _arch ) {}

    //   Arch& arch;
    // };

    U64  GetGPR(unsigned idx) { return regvalues[idx]; }
    void SetGPR(unsigned idx, U64 value) { regvalues[idx] = value; }

    U64& GetCTR() { return regvalues[IRegID::ctr]; }
    U64& GetLR() { return regvalues[IRegID::lr]; }

    bool MoveFromSPR( unsigned idx, U64& value ) { throw Unimplemented(); }
    bool MoveToSPR( unsigned idx, U64 value ) { throw Unimplemented(); }

    //   =====================================================================
    //   =                     Floating-point registers                      =
    //   =====================================================================

    struct FRegID
      : public unisim::util::identifier::Identifier<FRegID>
      , public unisim::util::symbolic::WithValueType<FRegID>
    {
      typedef double value_type;

      enum Code {
        f0=0, f1,  f2,  f3,  f4,  f5,  f6,  f7,  f8,  f9, f10, f11, f12, f13, f14, f15,
        f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31,
        end
      } code;

      char const* c_str() const
      {
        switch (code)
          {
          case f0:  return "f0";  case f1:  return "f1";  case f2:  return "f2";  case f3:  return "f3";
          case f4:  return "f4";  case f5:  return "f5";  case f6:  return "f6";  case f7:  return "f7";
          case f8:  return "f8";  case f9:  return "f9";  case f10: return "f10"; case f11: return "f11";
          case f12: return "f12"; case f13: return "f13"; case f14: return "f14"; case f15: return "f15";
          case f16: return "f16"; case f17: return "f17"; case f18: return "f18"; case f19: return "f19";
          case f20: return "f20"; case f21: return "f21"; case f22: return "f22"; case f23: return "f23";
          case f24: return "f24"; case f25: return "f25"; case f26: return "f26"; case f27: return "f27";
          case f28: return "f28"; case f29: return "f29"; case f30: return "f30"; case f31: return "f31";
          case end: break;
          }
        return "<bad>";
      }

      void Repr( std::ostream& sink ) const;

      FRegID() : code(end) {}
      FRegID( Code _code ) : code(_code) {}
      FRegID( unsigned idx ) : code(Code(idx)) {}
      FRegID( char const* _code ) : code(end) { init( _code ); }
    };

    SoftDouble const& GetFPR(unsigned idx) { return fregvalues[idx]; }
    void SetFPR(unsigned idx, SoftDouble const& value) { fregvalues[idx] = value; }

    struct FPSCR
    {
      struct _0_3 {}; /*  0 ..  3 */
      struct FPRF {
        static uint32_t          QNAN    () { return 0x11; }
        static uint32_t NEGATIVE_INFINITY() { return 0x9; }
        static uint32_t NEGATIVE_NORMAL  () { return 0x8; }
        static uint32_t NEGATIVE_DENORMAL() { return 0x18; }
        static uint32_t NEGATIVE_ZERO    () { return 0x12; }
        static uint32_t POSITIVE_ZERO    () { return 0x2; }
        static uint32_t POSITIVE_DENORMAL() { return 0x14; }
        static uint32_t POSITIVE_NORMAL  () { return 0x4; }
        static uint32_t POSITIVE_INFINITY() { return 0x5; }
      };              /* 15 .. 19 : Floating-point result flags */
      struct   RN {}; /* 30 .. 31 : Floating-point rounding control*/

      template <unsigned BIT> struct FBit { enum { pos = BIT }; };
      typedef FBit<32+0> FX; /* Floating-point exception summary */
      typedef FBit<32+1> FEX; /* Floating-point enabled exception summary */
      typedef FBit<32+2> VX; /* Floating-point invalid operation exception summary */
      typedef FBit<32+3> OX; /* Floating-point overflow exception */
      typedef FBit<32+4> UX; /* Floating-point underflow exception */
      typedef FBit<32+5> ZX; /* Floating-point zero divide exception */
      typedef FBit<32+6> XX; /* Floating-point inexact exception */
      typedef FBit<32+7> VXSNAN; /* Floating-point invalid operation exception for SNaN */
      typedef FBit<32+8> VXISI; /* Floating-point invalid operation exception for infinity - infinity */
      typedef FBit<32+9> VXIDI; /* Floating-point invalid operation exception for infinity / infinity */
      typedef FBit<32+10> VXZDZ; /* Floating-point invalid operation exception for zero / zero */
      typedef FBit<32+11> VXIMZ; /* Floating-point invalid operation exception for infinity * zero */
      typedef FBit<32+12> VXVC; /* Floating-point invalid operation exception for invalid compare */
      typedef FBit<32+13> FR; /* Floating-point fraction rounded */
      typedef FBit<32+14> FI; /* Floating-point fraction inexact */
      typedef FBit<32+19> FPCC; /* Floating-Point Condition Code */
      typedef FBit<32+21> VXSOFT; /* Floating-point invalid operation exception for software request */
      typedef FBit<32+22> VXSQRT; /* Floating-point invalid operation exception for invalid square root */
      typedef FBit<32+23> VXCVI; /* Floating-point invalid operation exception for invalid integer convert */
      typedef FBit<32+24> VE; /* Floating-point invalid operation exception enable */
      typedef FBit<32+25> OE; /* IEEE floating-point overflow exception enable */
      typedef FBit<32+26> UE; /* IEEE floating-point underflow exception enable */
      typedef FBit<32+27> ZE; /* IEEE floating-point zero divide exception enable */
      typedef FBit<32+28> XE; /* Floating-point inexact exception enable */
      typedef FBit<32+29> NI; /* Floating-point non-IEEE mode */

      template <typename PART> void Write(PART, Expr const& bit) { value = Expr(new BitInsertNode(value.expr, bit, 63 ^ PART::pos, 1 )); }
      template <typename PART,typename T> void Set( SmartValue<T> const& bits ) { Write(PART(), U64(bits).expr); }
      // template <typename PART> void Set( uint32_t value ) { Set<PART,uint32_t>( unisim::util::symbolic::make_const(value) ); }

      U64 Read(_0_3) { return value >> 28 & U64(0xf); }
      U64 Read(RN) { return value >> 0 & U64(3); }
      template <typename PART> U64 Read(PART) { return value >> (63 ^ PART::pos) & U64(1); }
      template <typename PART> U64 Get() { return Read(PART()); }
      operator U64 () { return value; }
      FPSCR& operator= ( U64 const& _value ) { value = _value; return *this; }
      void SetInexact( BOOL i ) { throw 0; /*Register<FPSCR>::Set<FI>( U64(i) ); if (i) SetException( XX() );*/ }

      template <class FIELD>
      void SetInvalid( FIELD const& vxfield ) { throw 0; /*SetException( vxfield ); Register<FPSCR>::Set<VX>( U64(1) ); */ }

      template <class FIELD>
      void SetException( FIELD const& )
      {
        throw 0;
        // if (not Register<FPSCR>::Get<FIELD>())
        //   Register<FPSCR>::Set<FX>( U64(1) ); // Exception bit change from 0 to 1
        // Register<FPSCR>::Set<FIELD>( U64(1) );
        // // Check if exception is enable (enable bits locations are 22 bit
        // // upper than their exception bits counterparts)
        // struct Enable : Field<Enable, FIELD::offset1 + 22> {};
        // if (Get<Enable>() == U64(1))
        //   Register<FPSCR>::Set<FEX>( U64(1) );
      }

      struct ID : public unisim::util::symbolic::WithValueType<ID> { typedef uint32_t value_type; void Repr(std::ostream& sink) const; int cmp(ID) const { return 0; } };

      FPSCR( Arch& arch ) : value( arch.newRegRead(ID()) ) {}

      U64 value;
    };

    FPSCR& GetFPSCR() { return fpscr; }

    bool CheckFloatingPointException() {throw 0; return false;}

    //   =====================================================================
    //   =                      Memory Transfer methods                      =
    //   =====================================================================
    Expr newLoad( U64 const& addr, unsigned size )
    {
      struct Load : public Path::Source, public unisim::util::symbolic::binsec::Load
      {
        Load( Path& path, Expr const& addr, unsigned size ) : Source(path), unisim::util::symbolic::binsec::Load(addr, size, 0, true) {}
        virtual Load* Mutate() const override { return new Load(*this); }
      };
      return new Load( path, addr.expr, size );
    }

    bool  Int8Load (unsigned n, U64 addr) { SetGPR(n, UINT(U8 (newLoad(addr, 1))) ); return true; }
    bool  Int16Load(unsigned n, U64 addr) { SetGPR(n, UINT(U16(newLoad(addr, 2))) ); return true; }
    bool  Int32Load(unsigned n, U64 addr) { SetGPR(n, UINT(U32(newLoad(addr, 4))) ); return true; }
    bool  Int64Load(unsigned n, U64 addr) { SetGPR(n, UINT(U64(newLoad(addr, 8))) ); return true; }

    bool  SInt8Load (unsigned n, U64 addr) { SetGPR(n, UINT(S8 (newLoad(addr, 1))) ); return true; }
    bool  SInt16Load(unsigned n, U64 addr) { SetGPR(n, UINT(S16(newLoad(addr, 2))) ); return true; }
    bool  SInt32Load(unsigned n, U64 addr) { SetGPR(n, UINT(S32(newLoad(addr, 4))) ); return true; }
    bool  SInt64Load(unsigned n, U64 addr) { SetGPR(n, UINT(S64(newLoad(addr, 8))) ); return true; }

    bool  Int8Store (unsigned n, U64 addr) { stores.insert( new unisim::util::symbolic::binsec::Store( addr.expr, U8 (GetGPR(n)).expr, 1, 0, true ) ); return true; }
    bool  Int16Store(unsigned n, U64 addr) { stores.insert( new unisim::util::symbolic::binsec::Store( addr.expr, U16(GetGPR(n)).expr, 2, 0, true ) ); return true; }
    bool  Int32Store(unsigned n, U64 addr) { stores.insert( new unisim::util::symbolic::binsec::Store( addr.expr, U32(GetGPR(n)).expr, 4, 0, true ) ); return true; }
    bool  Int64Store(unsigned n, U64 addr) { stores.insert( new unisim::util::symbolic::binsec::Store( addr.expr, U64(GetGPR(n)).expr, 8, 0, true ) ); return true; }

    // bool Int16LoadByteReverse(unsigned n, ADDRESS const& address) { SetGPR(n, ByteSwap(U32(U16(MemRead<16>(address))))); return true; }
    // bool Int32LoadByteReverse(unsigned n, ADDRESS const& address) { SetGPR(n, ByteSwap(U32(MemRead<32>(address)))); return true; }

    // bool Int16StoreByteReverse(unsigned n, ADDRESS const& address ) { MemWrite<16>( address, ByteSwap(U16(GetGPR(n))) ); return true; }
    // bool Int32StoreByteReverse(unsigned n, ADDRESS const& address ) { MemWrite<32>( address, ByteSwap(U32(GetGPR(n))) ); return true; }

    bool Fp32Load(unsigned n, ADDRESS const& address) { throw 0; return true; }
    bool Fp64Load(unsigned n, ADDRESS const& addr) { throw 0; return true; }

    bool Fp32Store(unsigned n, ADDRESS const& addr) { throw 0; return true; }
    bool Fp64Store(unsigned n, ADDRESS const& addr) { throw 0; return true; }
    bool FpStoreLSW(unsigned n, ADDRESS const& addr) { throw 0; return true; }

    //   =====================================================================
    //   =                      Control Transfer methods                     =
    //   =====================================================================

    enum branch_type_t { B_JMP = 0, B_CALL, B_RET };
    void Branch(U64 const& npc, branch_type_t bt)
    {
      next_instruction_address = npc;
      branch_type = bt;
    }

    void Branch(U64 const& npc) { Branch(npc, B_JMP); }
    U64 GetCIA() const { return current_instruction_address; }

    struct ProgramInterrupt
    {
      struct IllegalInstruction {};
      struct UnimplementedInstruction {};
    };
    struct FloatingPointUnavailableInterrupt
    {
      struct FloatingPointUnavailable {};
    };
    template <typename T>  void ThrowException() { throw Unimplemented(); }

    Path&            path;
    U64              current_instruction_address;
    U64              next_instruction_address;
    branch_type_t    branch_type;
    U64              regvalues[IRegID::end];
    SoftDouble       fregvalues[FRegID::end];
    CR               cr;
    XER              xer;
    MSR              msr;
    FPSCR            fpscr;
    std::set<Expr>   stores;
  };

} /* end of namespace arch64 */
} /* end of namespace dba */
} /* end of namespace powerpc */
} /* end of namespace processor */
} /* end of namespace cxx */
} /* end of namespace component */
} /* end of namespace unisim */

#endif // __UNISIM_COMPONENT_CXX_PROCESSOR_POWERPC_DBA_ARCH64_ARCH_HH__
