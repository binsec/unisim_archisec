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

#ifndef ARMD64DBA_ARCH_HH
#define ARMD64DBA_ARCH_HH

#include <unisim/component/cxx/processor/intel/isa/intel.hh>
#include <unisim/component/cxx/processor/intel/modrm.hh>
#include <unisim/component/cxx/processor/intel/disasm.hh>
#include <unisim/component/cxx/processor/intel/types.hh>
#include <unisim/component/cxx/processor/intel/vectorbank.hh>
#include <unisim/component/cxx/vector/vector.hh>
#include <unisim/util/symbolic/vector/vector.hh>
#include <unisim/util/symbolic/binsec/binsec.hh>
#include <unisim/util/symbolic/symbolic.hh>

// template <typename ARCH, unsigned OPSIZE> struct TypeFor {};

// template <typename ARCH> struct TypeFor<ARCH, 8> { typedef typename ARCH:: S8 S; typedef typename ARCH:: U8 U; };
// template <typename ARCH> struct TypeFor<ARCH,16> { typedef typename ARCH::S16 S; typedef typename ARCH::U16 U; };
// template <typename ARCH> struct TypeFor<ARCH,32> { typedef typename ARCH::S32 S; typedef typename ARCH::U32 U; typedef typename ARCH::F32 F; };
// template <typename ARCH> struct TypeFor<ARCH,64> { typedef typename ARCH::S64 S; typedef typename ARCH::U64 U; typedef typename ARCH::F64 F; };

struct VmmRegister
{
  enum { BYTECOUNT = 32 };
  struct value_type { char _[BYTECOUNT]; };
  VmmRegister() = default;
  VmmRegister(unisim::util::symbolic::Expr const& _expr) : expr(_expr) {}
  unisim::util::symbolic::Expr expr;
  static unisim::util::symbolic::ScalarType::id_t GetType() { return unisim::util::symbolic::ScalarType::VOID; }
};

struct ProcessorBase
{
  struct Undefined {};
  struct Unimplemented {};

  template <typename T> using SmartValue = unisim::util::symbolic::SmartValue<T>;
  template <typename T> using VectorTypeInfo = unisim::component::cxx::vector::VectorTypeInfo<T,0>;
  template <typename A, unsigned S> using TypeFor = typename unisim::component::cxx::processor::intel::TypeFor<A,S>;

  typedef SmartValue<uint8_t>     u8_t;
  typedef SmartValue<uint16_t>    u16_t;
  typedef SmartValue<uint32_t>    u32_t;
  typedef SmartValue<uint64_t>    u64_t;
  typedef SmartValue<int8_t>      s8_t;
  typedef SmartValue<int16_t>     s16_t;
  typedef SmartValue<int32_t>     s32_t;
  typedef SmartValue<int64_t>     s64_t;
  typedef SmartValue<bool>        bit_t;

  typedef SmartValue<float>       f32_t;
  typedef SmartValue<double>      f64_t;
  typedef SmartValue<long double> f80_t;

  typedef unisim::util::symbolic::Expr Expr;
  typedef unisim::util::symbolic::ExprNode ExprNode;
  typedef unisim::util::symbolic::ScalarType ScalarType;

  typedef unisim::util::symbolic::binsec::ActionNode   ActionNode;
  typedef unisim::util::symbolic::binsec::Load         Load;
  typedef unisim::util::symbolic::binsec::Store        Store;
  typedef unisim::util::symbolic::binsec::Branch       Br;

  ProcessorBase();

  /*** MEMORY ***/
  std::set<Expr> stores;

  /*** INTEGER REGISTERS ***/

  template <typename RID>
  struct RegRead : public unisim::util::symbolic::binsec::RegRead
  {
    typedef unisim::util::symbolic::binsec::RegRead Super;
    RegRead( RID _id ) : Super(), id(_id) {}
    virtual RegRead* Mutate() const override { return new RegRead( *this ); }
    virtual ScalarType::id_t GetType() const
    { return unisim::util::symbolic::TypeInfo<typename RID::register_type>::GetType(); }
    virtual void GetRegName( std::ostream& sink ) const { id.Repr(sink); }
    virtual int cmp( ExprNode const& rhs ) const override { return compare( dynamic_cast<RegRead const&>( rhs ) ); }
    int compare( RegRead const& rhs ) const { if (int delta = Super::compare(rhs)) return delta; return id.cmp( rhs.id ); }

    RID id;
  };

  template <typename RID>
  static Expr newRegRead( RID id ) { return new RegRead<RID>( id ); }

  template <typename RID>
  struct RegWrite : public unisim::util::symbolic::binsec::RegWrite
  {
    typedef RegWrite<RID> this_type;
    typedef unisim::util::symbolic::binsec::RegWrite Super;
    RegWrite( RID _id, Expr const& _value ) : Super(_value), id(_id) {}
    virtual this_type* Mutate() const override { return new this_type( *this ); }
    virtual void GetRegName( std::ostream& sink ) const { id.Repr(sink); }
    virtual int cmp( ExprNode const& rhs ) const override { return compare( dynamic_cast<RegWrite const&>( rhs ) ); }
    int compare( RegWrite const& rhs ) const { if (int delta = id.cmp( rhs.id )) return delta; return Super::compare( rhs ); }

    RID id;
  };

  template <typename RID>
  static Expr newRegWrite( RID id, Expr const& value ) { return new RegWrite<RID>( id, value ); }

  /*** FLOATING POINT REGISTERS ***/

  enum {FREGCOUNT=8};

  void                        fnanchk( f64_t value ) {}

  int                         fcwreadRC() const { return 0; }
  u16_t                       fcwread() const { throw Unimplemented(); /*FCW access*/ return u16_t(); }
  void                        fcwwrite( u16_t value ) { throw Unimplemented(); /*FCW access*/}
  void                        finit() { throw Unimplemented(); /*FCW access*/ }

  void                        fxam() { throw Unimplemented(); }

  struct FTop : public unisim::util::symbolic::ExprNode
  {
    virtual FTop* Mutate() const override { return new FTop(*this); }
    virtual unsigned SubCount() const { return 0; }
    virtual int cmp(ExprNode const&) const override { return 0; }
    virtual ScalarType::id_t GetType() const { return ScalarType::U8; }
    virtual void Repr( std::ostream& sink ) const;
  };

  struct FTopWrite : public unisim::util::symbolic::binsec::RegWrite
  {
    typedef unisim::util::symbolic::binsec::RegWrite Super;
    FTopWrite( u8_t const& ftop ) : Super( ftop.expr ) {}
    virtual FTopWrite* Mutate() const override { return new FTopWrite(*this); }
    virtual void GetRegName(std::ostream& sink) const;
    virtual int cmp(ExprNode const& rhs) const override { return Super::compare(dynamic_cast<Super const&>(rhs)); }
  };

  u16_t                       ftopread() { throw Unimplemented(); /*FCW access*/; return u16_t(); }
  unsigned                    ftop;

  Expr&                       fpaccess(unsigned r, bool w);
  bool                        fpdiff(unsigned r);

  Expr                        fpregs[8];
  void                        fpush( f64_t value ) { throw Unimplemented(); }
  void                        fwrite( unsigned idx, f64_t value ) { throw Unimplemented(); }
  f64_t                       fpop() { throw Unimplemented(); return f64_t(); }
  f64_t                       fread( unsigned idx ) { throw Unimplemented(); return f64_t(); }

  struct FRegID : public unisim::util::identifier::Identifier<FRegID>
  {
    typedef double register_type;
    enum Code { st0=0, st1, st2, st3, st4, st5, st6, st7, end } code;
    char const* c_str() const { return &"st0\0st1\0st2\0st3\0st4\0st5\0st6\0st7"[(unsigned(code) % 8)*4]; }
    FRegID() : code(end) {}
    FRegID( Code _code ) : code(_code) {}
    FRegID( char const* _code ) : code(end) { init( _code ); }
  };

  /*** VECTOR REGISTERS ***/
  
  // VRegRead should never be a binsec::node (no dba available for it)
  struct VRegRead : public unisim::util::symbolic::ExprNode
  {
    VRegRead( unsigned _reg ) : reg(_reg) {}
    virtual VRegRead* Mutate() const override { return new VRegRead( *this ); }
    virtual ScalarType::id_t GetType() const { return ScalarType::VOID; }
    virtual unsigned SubCount() const override { return 0; }
    virtual void Repr( std::ostream& sink ) const override;
    virtual int cmp( ExprNode const& rhs ) const override { return compare( dynamic_cast<VRegRead const&>( rhs ) ); }
    int compare( VRegRead const& rhs ) const { return int(reg) - int(rhs.reg); }
    unsigned reg;
  };

  struct VmmIndirectReadBase : public ExprNode
  {// Parent of VmmIndirectRead
    VmmIndirectReadBase(Expr const& _index) : index(_index) {}
    virtual void Repr( std::ostream& sink ) const override;
    virtual unsigned GetVSize() const = 0;
    virtual char const* GetVName() const = 0;
    Expr index;
  };

  /*** CONTROL FLOW  ***/

  struct Goto : public Br
  {
    Goto( Expr const& value ) : Br( value ) {}
    virtual Goto* Mutate() const override { return new Goto( *this ); }
    virtual void GetRegName( std::ostream& sink ) const override;
    virtual void annotate(std::ostream& sink) const override { return; }
  };

  /*** FLAGS ***/

  struct FLAG : public unisim::util::identifier::Identifier<FLAG>
  {
    typedef bool register_type;
    enum Code { CF = 0, PF, AF, ZF, SF, DF, OF, C0, C1, C2, C3, end } code;

    char const* c_str() const
    {
      switch (code)
        {
        case CF: return "cf";
        case PF: return "pf";
        case AF: return "af";
        case ZF: return "zf";
        case SF: return "sf";
        case DF: return "df";
        case OF: return "of";
        case C0: return "c0";
        case C1: return "c1";
        case C2: return "c2";
        case C3: return "c3";
        case end: break;
        }
      return "NA";
    }

    void Repr( std::ostream& sink ) const;

    FLAG() : code(end) {}
    FLAG( Code _code ) : code(_code) {}
    FLAG( char const* _code ) : code(end) { init(_code); }
  };

  Expr                        flagvalues[FLAG::end];

  bit_t                       flagread( FLAG flag ) { return bit_t(flagvalues[flag.idx()]); }
  void                        flagwrite( FLAG flag, bit_t fval ) { flagvalues[flag.idx()] = fval.expr; }

  /*** SEGMENTS ***/
  struct SegmentID : public unisim::util::identifier::Identifier<SegmentID>
  {
    enum Code { es, cs, ss, ds, fs, gs, end } code;

    char const* c_str() const
    {
      switch (code)
        {
        case  es: return "es";
        case  cs: return "cs";
        case  ss: return "ss";
        case  ds: return "ds";
        case  fs: return "fs";
        case  gs: return "gs";
        case end: break;
        }
      return "NA";
    }

    SegmentID() : code(end) {}
    SegmentID( Code _code ) : code(_code) {}
    SegmentID( char const* _code ) : code(end) { init( _code ); }
  };

  u16_t                       segregread( unsigned idx ) { throw Unimplemented(); return u16_t(); }
  void                        segregwrite( unsigned idx, u16_t value ) { throw Unimplemented(); }

  enum ipproc_t { ipjmp, ipcall, ipret };

  ActionNode*      path;
  ipproc_t         next_insn_mode;
  bool             abort;
};

template <class MODE>
struct Processor : public ProcessorBase
{
  typedef unisim::component::cxx::processor::intel::Operation<Processor> Operation;

  typedef unisim::component::cxx::processor::intel::GObLH GObLH;
  typedef unisim::component::cxx::processor::intel::GOb GOb;
  typedef unisim::component::cxx::processor::intel::GOw GOw;
  typedef unisim::component::cxx::processor::intel::GOd GOd;
  typedef unisim::component::cxx::processor::intel::GOq GOq;

  typedef unisim::component::cxx::processor::intel::SSE SSE;
  typedef unisim::component::cxx::processor::intel::XMM XMM;
  typedef unisim::component::cxx::processor::intel::YMM YMM;

  typedef unisim::component::cxx::processor::intel::RMOp<Processor> RMOp;

  typedef                typename MODE::IRegID  IRegID;
  typedef typename MODE::IRegID::register_type  nat_gr_type;
  typedef                    typename MODE::GR  GR;
  typedef                          nat_gr_type  nat_addr_t;
  typedef              SmartValue<nat_gr_type>  gr_type;
  typedef                              gr_type  addr_t;


  struct OpHeader { OpHeader( nat_addr_t _address ) : address( _address ) {} nat_addr_t address; };

  /*** SEGMENTS ***/
  struct SegBaseID : public SegmentID
  {
    typedef nat_addr_t register_type;
    void Repr(std::ostream& sink) const { sink << c_str() << "_base"; }
    SegBaseID() = default;
    SegBaseID( Code _code ) : SegmentID(_code) {}
    SegBaseID( char const* _code ) : SegmentID(_code) {}
  };
  
  Expr                        segment_bases[6];
  Expr GetSegBase( unsigned idx )
  {
    Expr& segbase = segment_bases[idx];
    if (ExprNode const* node = segbase.node)
      return node;
    return (segbase = newRegRead( SegBaseID( typename SegBaseID::Code(idx) ) ) );
  }

  /*** MEMORY ***/
  Expr PerformLoad( unsigned bytes, unsigned segment, addr_t addr )
  {
    if (segment >= 4) /*FS|GS relative addressing*/
      addr = addr_t(GetSegBase(segment)) + addr;
    return new Load( addr.expr, bytes, 0, false );
  }
  void PerformStore( unsigned bytes, unsigned segment, addr_t addr, Expr const& value )
  {
    if (segment >= 4) /*FS|GS relative addressing*/
      addr = addr_t(GetSegBase(segment)) + addr;
    stores.insert( Expr( new Store( addr.expr, value, bytes, 0, false ) ) );
  }

  template<unsigned OPSIZE>
  typename TypeFor<Processor,OPSIZE>::u
  memread( unsigned seg, addr_t const& addr )
  {
    typedef typename TypeFor<Processor,OPSIZE>::u u_type;
    return u_type( PerformLoad( OPSIZE/8, seg, addr.expr ) );
  }

  template <unsigned OPSIZE>
  void
  memwrite( unsigned seg, addr_t const& addr, typename TypeFor<Processor,OPSIZE>::u val )
  {
    PerformStore( OPSIZE/8, seg, addr, val.expr );
  }

  void                        fmemwrite32( unsigned seg, addr_t const& addr, f32_t val ) { throw Unimplemented(); }
  void                        fmemwrite64( unsigned seg, addr_t const& addr, f64_t val ) { throw Unimplemented(); }
  void                        fmemwrite80( unsigned seg, addr_t const& addr, f80_t val ) { throw Unimplemented(); }

  f32_t                       fmemread32( unsigned seg, addr_t const& addr ) { throw Unimplemented(); return f32_t(); }
  f64_t                       fmemread64( unsigned seg, addr_t const& addr ) { throw Unimplemented(); return f64_t(); }
  f80_t                       fmemread80( unsigned seg, addr_t const& addr ) { throw Unimplemented(); return f80_t(); }

  /*** CONTROL FLOW ***/

  struct Call : public Goto
  {
    Call( Expr const& value, nat_addr_t ra ) : Goto( value ), return_address( ra ) {}
    virtual Call* Mutate() const override { return new Call( *this ); }
    virtual void annotate(std::ostream& sink) const override;

    nat_addr_t return_address;
  };

  u64_t                       tscread() { throw Unimplemented(); return u64_t( 0 ); }

  typedef std::map<unsigned,unsigned> RegMap;

  enum { GREGSIZE = MODE::GREGSIZE, GREGCOUNT = MODE::GREGCOUNT, VREGCOUNT = MODE::VREGCOUNT };

  Expr                        eregread( unsigned reg, unsigned size, unsigned pos );
  void                        eregwrite( unsigned reg, unsigned size, unsigned pos, Expr const& xpr );

  Expr                        regvalues[GREGCOUNT][GREGSIZE];

  void                        eregsinks( unsigned reg );

  template <class GOP>
  typename TypeFor<Processor,GOP::SIZE>::u regread( GOP const&, unsigned idx )
  {
    return typename TypeFor<Processor,GOP::SIZE>::u( eregread( idx, GOP::SIZE / 8, 0 ) );
  }

  u8_t regread( GObLH const&, unsigned idx ) { return u8_t( eregread( idx%4, 1, (idx >> 2) & 1 ) ); }

  template <class GOP> void regwrite( GOP const&, unsigned idx, typename TypeFor<Processor,GOP::SIZE>::u const& val )
  { eregwrite( idx, GREGSIZE, 0, gr_type(val).expr ); }

  void regwrite( GObLH const&, unsigned idx, u8_t val ) { eregwrite( idx%4, 1, (idx>>2) & 1, val.expr ); }
  void regwrite( GOb const&, unsigned idx, u8_t val )   { eregwrite( idx, 1, 0, val.expr ); }
  void regwrite( GOw const&, unsigned idx, u16_t val )  { eregwrite( idx, 2, 0, val.expr ); }

  addr_t                      getnip() { return next_insn_addr; }
  void                        setnip( addr_t nip, ipproc_t ipproc = ipjmp )
  {
    next_insn_addr = nip;
    next_insn_mode = ipproc;
  }

  template <unsigned OPSIZE>
  typename TypeFor<Processor,OPSIZE>::u
  regread( unsigned idx )
  {
    typedef typename TypeFor<Processor,OPSIZE>::u u_type;

    if (OPSIZE==8)                    return u_type( eregread( idx%4, 1, (idx>>2) & 1 ) );
    if ((OPSIZE==16) or (OPSIZE==32)) return u_type( eregread( idx, OPSIZE/8, 0 ) );
    throw 0;
    return u_type(0);
  }

  template <unsigned OPSIZE>
  void
  regwrite( unsigned idx, typename TypeFor<Processor,OPSIZE>::u const& value )
  {
    if  (OPSIZE==8)                   return eregwrite( idx%4, 1, (idx>>2) & 1, value.expr );
    if ((OPSIZE==16) or (OPSIZE==32)) return eregwrite( idx, OPSIZE/8, 0, value.expr );
    throw 0;
  }

  template <unsigned OPSIZE>
  typename TypeFor<Processor,OPSIZE>::u
  pop()
  {
    // TODO: handle stack address size
    gr_type sptr = regread( GR(), 4 );
    regwrite( GR(), 4, sptr + gr_type( OPSIZE/8 ) );
    return memread<OPSIZE>( unisim::component::cxx::processor::intel::SS, addr_t(sptr) );
  }

  template <unsigned OPSIZE>
  void
  push( typename TypeFor<Processor,OPSIZE>::u const& value )
  {
    // TODO: handle stack address size
    gr_type sptr = regread( GR(), 4 ) - gr_type( OPSIZE/8 );
    memwrite<OPSIZE>( unisim::component::cxx::processor::intel::SS, addr_t(sptr), value );
    regwrite( GR(), 4, sptr );
  }

  void shrink_stack( addr_t const& offset ) { regwrite( GR(), 4, regread( GR(), 4 ) + offset ); }
  void grow_stack( addr_t const& offset ) { regwrite( GR(), 4, regread( GR(), 4 ) - offset ); }

  template <class GOP>
  typename TypeFor<Processor,GOP::SIZE>::u
  rmread( GOP const& g, RMOp const& rmop )
  {
    if (not rmop.is_memory_operand())
      return regread( g, rmop.ereg() );

    return memread<GOP::SIZE>( rmop->segment, rmop->effective_address( *this ) );
  }

  template <class GOP>
  void
  rmwrite( GOP const& g, RMOp const& rmop, typename TypeFor<Processor,GOP::SIZE>::u const& value )
  {
    if (not rmop.is_memory_operand())
      return regwrite( g, rmop.ereg(), value );

    return memwrite<GOP::SIZE>( rmop->segment, rmop->effective_address( *this ), value );
  }

  template <unsigned OPSIZE>
  typename TypeFor<Processor,OPSIZE>::f
  fpmemread( unsigned seg, addr_t const& addr )
  {
    throw Unimplemented();
    return typename TypeFor<Processor,OPSIZE>::f();
  }

  template <unsigned OPSIZE>
  typename TypeFor<Processor,OPSIZE>::f
  frmread( RMOp const& rmop )
  {
    typedef typename TypeFor<Processor,OPSIZE>::f f_type;
    if (not rmop.is_memory_operand()) return f_type( fread( rmop.ereg() ) );
    return this->fpmemread<OPSIZE>( rmop->segment, rmop->effective_address( *this ) );
  }

  template <unsigned OPSIZE>
  void
  fpmemwrite( unsigned seg, addr_t const& addr, typename TypeFor<Processor,OPSIZE>::f const& value )
  {
    throw Unimplemented();
  }

  template <unsigned OPSIZE>
  void
  frmwrite( RMOp const& rmop, typename TypeFor<Processor,OPSIZE>::f const& value )
  {
    if (not rmop.is_memory_operand()) return fwrite( rmop.ereg(), f64_t( value ) );
    fpmemwrite<OPSIZE>( rmop->segment, rmop->effective_address( *this ), value );
  }

  u32_t mxcsread() { throw Unimplemented(); /*mxcsr access*/; return u32_t(); }
  void mxcswrite( u32_t const& value ) { throw Unimplemented(); /*mxcsr access*/; }

  struct VUConfig : public unisim::util::symbolic::vector::VUConfig
  {
    static unsigned const BYTECOUNT = VmmRegister::BYTECOUNT;
    static unsigned const REGCOUNT = VREGCOUNT;
  };

  struct VmmBrick { char _[sizeof(u8_t)]; };
  typedef unisim::component::cxx::vector::VUnion<VUConfig> VUnion;
  VUnion umms[VUConfig::REGCOUNT];
  VmmBrick vmm_storage[VUConfig::REGCOUNT][VUConfig::BYTECOUNT];

  template <class VR> static unsigned vmm_wsize( VR const& vr ) { return VR::size() / 8; }
  static unsigned vmm_wsize( unisim::component::cxx::processor::intel::SSE const& ) { return VUConfig::BYTECOUNT; }

  template <class VR, class ELEM>
  struct VmmIndirectRead : public VmmIndirectReadBase
  {
    typedef unisim::util::symbolic::TypeInfo<typename ELEM::value_type> traits;
    enum { elemcount = VR::SIZE / 8 / traits::BYTECOUNT };
    
    VmmIndirectRead( ELEM const* elems, u8_t const& _index)
      : VmmIndirectReadBase(_index.expr)
    {
      for (unsigned idx = 0, end = elemcount; idx < end; ++idx)
        sources[idx] = elems[idx].expr;
    }
    typedef VmmIndirectRead<VR,ELEM> this_type;
    virtual this_type* Mutate() const override { return new this_type( *this ); }
    virtual unsigned GetVSize() const override { return VR::SIZE; }
    virtual char const* GetVName() const override { return ScalarType(GetType()).name; }
    virtual unsigned SubCount() const { return elemcount+1; }
    virtual Expr const& GetSub(unsigned idx) const
    {
      if (idx < elemcount) return sources[idx];
      if (idx == elemcount) return index;
      return ExprNode::GetSub(idx);
    }
    virtual ScalarType::id_t GetType() const override { return traits::GetType(); }
    virtual int cmp( ExprNode const& brhs ) const override { return compare( dynamic_cast<this_type const&>(brhs) ); }
    int compare( this_type const& rhs ) const { return 0; }
    
    Expr sources[elemcount];
  };

  template <class VR, class ELEM>
  ELEM vmm_read( VR const& vr, unsigned reg, u8_t const& sub, ELEM const& e )
  {
    ELEM const* elems = umms[reg].GetConstStorage( &vmm_storage[reg][0], e, vr.size() / 8 );
    return ELEM(Expr(new VmmIndirectRead<VR, ELEM>( elems, sub.expr )));
  }

  template <class VR, class ELEM>
  ELEM vmm_read( VR const& vr, unsigned reg, unsigned sub, ELEM const& e )
  {
    ELEM const* elems = umms[reg].GetConstStorage( &vmm_storage[reg][0], e, vr.size() / 8 );
    return elems[sub];
  }

  template <class VR, class ELEM>
  void vmm_write( VR const& vr, unsigned reg, unsigned sub, ELEM const& e )
  {
    ELEM* elems = umms[reg].GetStorage( &vmm_storage[reg][0], e, vmm_wsize( vr ) );
    elems[sub] = e;
  }

  template <class VR, class ELEM>
  ELEM vmm_read( VR const& vr, RMOp const& rmop, unsigned sub, ELEM const& e )
  {
    if (not rmop.is_memory_operand()) return vmm_read( vr, rmop.ereg(), sub, e );
    return vmm_memread( rmop->segment, rmop->effective_address( *this ) + addr_t(sub*VUConfig::template TypeInfo<ELEM>::bytecount), e );
  }

  template <class VR, class ELEM>
  void vmm_write( VR const& vr, RMOp const& rmop, unsigned sub, ELEM const& e )
  {
    if (not rmop.is_memory_operand()) return vmm_write( vr, rmop.ereg(), sub, e );
    return vmm_memwrite( rmop->segment, rmop->effective_address( *this ) + addr_t(sub*VUConfig::template TypeInfo<ELEM>::bytecount), e );
  }

  // Integer case
  template <class ELEM> ELEM vmm_memread( unsigned seg, addr_t addr, ELEM const& e )
  {
    typedef unisim::component::cxx::processor::intel::atpinfo<Processor,ELEM> atpinfo;
    return ELEM(memread<atpinfo::bitsize>(seg,addr));
  }

  f32_t vmm_memread( unsigned seg, addr_t addr, f32_t const& e ) { return fmemread32( seg, addr ); }
  f64_t vmm_memread( unsigned seg, addr_t addr, f64_t const& e ) { return fmemread64( seg, addr ); }
  f80_t vmm_memread( unsigned seg, addr_t addr, f80_t const& e ) { return fmemread80( seg, addr ); }

  // Integer case
  template <class ELEM> void vmm_memwrite( unsigned seg, addr_t addr, ELEM const& e )
  {
    typedef unisim::component::cxx::processor::intel::atpinfo<Processor,ELEM> atpinfo;
    memwrite<atpinfo::bitsize>(seg,addr,typename atpinfo::utype(e));
  }

  void vmm_memwrite( unsigned seg, addr_t addr, f32_t const& e ) { return fmemwrite32( seg, addr, e ); }
  void vmm_memwrite( unsigned seg, addr_t addr, f64_t const& e ) { return fmemwrite64( seg, addr, e ); }
  void vmm_memwrite( unsigned seg, addr_t addr, f80_t const& e ) { return fmemwrite80( seg, addr, e ); }

  //   =====================================================================
  //   =                      Construction/Destruction                     =
  //   =====================================================================

private:
  Processor( Processor const& );


public:

  Processor();
  ~Processor()
  {
    for (unsigned reg = 0; reg < VUConfig::REGCOUNT; ++reg)
      umms[reg].Clear(&vmm_storage[reg][0]);
  }

  bool close( Processor const& ref );

  void step( Operation const& op );

  void noexec( Operation const& op );

  void interrupt(int op, int code) { throw Unimplemented(); }

  void syscall() { throw Unimplemented(); }

  void unimplemented() { throw Unimplemented(); }

  void cpuid() { throw Unimplemented(); /*hardware*/ }

  void xgetbv() { throw Unimplemented(); /*hardware*/ }

  void stop() { throw Unimplemented(); /*hardware*/ }

  void _DE()  { abort = true; }

  //   =====================================================================
  //   =                 Internal Instruction Control Flow                 =
  //   =====================================================================

  bool concretize( Expr cexp )
  {
    if (unisim::util::symbolic::ConstNodeBase const* cnode = cexp.ConstSimplify())
      return cnode->Get( bool() );

    bool predicate = path->proceed( cexp );
    path = path->next( predicate );
    return predicate;
  }

  template <typename T>
  bool Test( unisim::util::symbolic::SmartValue<T> const& cond )
  {
    if (not cond.expr.good())
      throw std::logic_error( "Not a valid condition" );

    return concretize( bit_t(cond).expr );
  }
  
  static Operation* Decode(nat_addr_t address, uint8_t const* bytes);

  nat_addr_t       return_address;
  addr_t           next_insn_addr;
};

template <class MODE>
Processor<MODE>::Processor()
  : return_address()
  , next_insn_addr()
{
  for (IRegID reg; reg.next();)
    regvalues[reg.idx()][0] = newRegRead( reg );

  for (unsigned reg = 0; reg < VREGCOUNT; ++reg)
    {
      VmmRegister v( new VRegRead( reg ) );
      *(umms[reg].GetStorage( &vmm_storage[reg][0], v, VUConfig::BYTECOUNT )) = v;
    }
}

template <class MODE>
ProcessorBase::Expr
Processor<MODE>::eregread( unsigned reg, unsigned size, unsigned pos )
{
  using unisim::util::symbolic::ExprNode;
  using unisim::util::symbolic::make_const;

  struct
  {
    Expr ui( unsigned sz, Expr const& src ) const
    {
      switch (sz) {
      default: throw 0;
      case 1: return new unisim::util::symbolic::CastNode<uint8_t,nat_gr_type>( src );
      case 2: return new unisim::util::symbolic::CastNode<uint16_t,nat_gr_type>( src );
      case 4: return new unisim::util::symbolic::CastNode<uint32_t,nat_gr_type>( src );
      case 8: return new unisim::util::symbolic::CastNode<uint64_t,nat_gr_type>( src );
      }
      return 0;
    }
  } cast;

  if (not regvalues[reg][pos].node)
    {
      // requested read is in the middle of a larger value
      unsigned src = pos;
      do { src = src & (src-1); } while (not regvalues[reg][src].node);
      unsigned shift = 8*(pos - src);
      return cast.ui( size, make_operation( "Lsr", regvalues[reg][src], make_const( shift ) ) );
    }
  else if (not regvalues[reg][(pos|size)&(GREGSIZE-1)].node)
    {
      // requested read is in lower bits of a larger value
      return cast.ui( size, regvalues[reg][pos] );
    }
  else if ((size > 1) and (regvalues[reg][pos|(size >> 1)].node))
    {
      // requested read is a concatenation of multiple source values
      Expr concat = cast.ui( size, regvalues[reg][pos] );
      for (unsigned idx = 0; ++idx < size;)
        {
          if (not regvalues[reg][pos+idx].node)
            continue;
          concat = make_operation( "Or", make_operation( "Lsl", cast.ui( size, regvalues[reg][idx] ), make_const( 8*idx ) ), concat );
        }
      return concat;
    }

  // requested read is directly available
  return regvalues[reg][pos];
}

template <class MODE>
void
Processor<MODE>::eregwrite( unsigned reg, unsigned size, unsigned pos, Expr const& xpr )
{
  Expr nxt[GREGSIZE];

  for (unsigned ipos = pos, isize = size, cpos;
       cpos = (ipos^isize) & (GREGSIZE-1), (not regvalues[reg][ipos].node) or (not regvalues[reg][cpos].node);
       isize *= 2, ipos &= -isize
       )
    {
      nxt[cpos] = eregread( reg, isize, cpos );
    }

  for (unsigned ipos = 0; ipos < GREGSIZE; ++ipos)
    {
      if (nxt[ipos].node)
        regvalues[reg][ipos] = nxt[ipos];
    }

  regvalues[reg][pos] = xpr;

  for (unsigned rem = 1; rem < size; ++rem)
    {
      regvalues[reg][pos+rem] = 0;
    }
}

template <class MODE>
void
Processor<MODE>::eregsinks( unsigned reg )
{
  using unisim::util::symbolic::binsec::BitFilter;
  unsigned regsize = GREGSIZE;
  // using unisim::util::symbolic::make_const;

  { // Check for constant values
    Expr dr = unisim::util::symbolic::binsec::ASExprNode::Simplify( eregread( reg, regsize, 0 ) );
    if (dr.ConstSimplify())
      {
        path->add_sink( newRegWrite( IRegID(reg), dr ) );
        return;
      }
  }

  // Check for monolithic value
  if (not regvalues[reg][GREGSIZE/2].node)
    {
      path->add_sink( newRegWrite( IRegID(reg), eregread(reg,GREGSIZE,0) ) );
      return;
    }

  throw Unimplemented(); // Yet
  // // Requested read is a concatenation of multiple source values
  // struct _
  // {
  //   _( ProcessorBase& _core, unsigned _reg ) : core(_core), reg(_reg) { Process( 0, GREGSIZE ); } ProcessorBase& core; unsigned reg;
  //   void Process( unsigned pos, unsigned size )
  //   {
  //     unsigned half = size / 2, mid = pos+half;
  //     if (size > 1 and core.regvalues[reg][mid].node)
  //       {
  //         Process( pos, half );
  //         Process( mid, half );
  //       }
  //     else
  //       {
  //         unsigned begin = pos*8, end = begin+size*8;
  //         Expr value( new BitFilter( core.eregread(reg,size,pos), 64, size*8, size*8, false ) );
  //         core.path->add_sink( new GregPartialWrite( reg, begin, end, value ) );
  //       }
  //   }
  // } concat( *this, reg );
}

template <class MODE>
bool
Processor<MODE>::close( Processor<MODE> const& ref )
{
  bool complete = path->close();

  if (next_insn_mode == ipcall)
    path->add_sink( new Call( next_insn_addr.expr, return_address ) );
  else
    path->add_sink( new Goto( next_insn_addr.expr ) );

  if (abort)
    {
      path->add_sink( new unisim::util::symbolic::binsec::AssertFalse() );
      return complete;
    }

  for (int reg = GREGCOUNT; --reg >= 0;)
    {
      if (regvalues[reg][0] != ref.regvalues[reg][0])
        eregsinks(reg);
    }

  // Flags
  for (FLAG reg; reg.next();)
    if (flagvalues[reg.idx()] != ref.flagvalues[reg.idx()])
      path->add_sink( newRegWrite( reg, flagvalues[reg.idx()] ) );

  for (Expr const& store : stores)
    path->add_sink( store );

  return complete;
}

template <class MODE>
void
Processor<MODE>::step( Operation const& op )
{
  nat_addr_t insn_addr = op.address, nia = insn_addr + op.length;
  return_address = nia;
  next_insn_addr = addr_t(nia);
  op.execute( *this );
}

struct Intel64
{
  enum {GREGSIZE = 8, GREGCOUNT = 16, VREGCOUNT = 16};
  
  typedef unisim::component::cxx::processor::intel::GOq GR;
  
  struct IRegID : public unisim::util::identifier::Identifier<IRegID>
  {
    typedef uint64_t register_type;
    enum Code { rax = 0, rcx = 1, rdx = 2, rbx = 3, rsp = 4, rbp = 5, rsi = 6, rdi = 7,
                r8, r9, r10, r11, r12, r13, r14, r15,
                end } code;

    char const* c_str() const
    {
      switch (code)
        {
        case rax: return "rax";
        case rcx: return "rcx";
        case rdx: return "rdx";
        case rbx: return "rbx";
        case rsp: return "rsp";
        case rbp: return "rbp";
        case rsi: return "rsi";
        case rdi: return "rdi";
        case r8:  return "r8";
        case r9: return "r9";
        case r10: return "r10";
        case r11: return "r11";
        case r12: return "r12";
        case r13: return "r13";
        case r14: return "r14";
        case r15: return "r15";

        case end: break;
        }
      return "NA";
    }

    void Repr(std::ostream& sink) const;

    IRegID() : code(end) {}
    IRegID( Code _code ) : code(_code) {}
    IRegID( unsigned idx ) : code(Code(idx)) {}
    IRegID( char const* _code ) : code(end) { init( _code ); }
  };

};

struct Compat32
{
  enum {GREGSIZE = 4, GREGCOUNT = 8, VREGCOUNT = 8};

  typedef unisim::component::cxx::processor::intel::GOd GR;
  
  struct IRegID : public unisim::util::identifier::Identifier<IRegID>
  {
    typedef uint32_t register_type;
    enum Code { eax = 0, ecx = 1, edx = 2, ebx = 3, esp = 4, ebp = 5, esi = 6, edi = 7, end } code;

    char const* c_str() const
    {
      switch (code)
        {
        case eax: return "eax";
        case ecx: return "ecx";
        case edx: return "edx";
        case ebx: return "ebx";
        case esp: return "esp";
        case ebp: return "ebp";
        case esi: return "esi";
        case edi: return "edi";

        case end: break;
        }
      return "NA";
    }

    void Repr(std::ostream& sink) const;

    IRegID() : code(end) {}
    IRegID( Code _code ) : code(_code) {}
    IRegID( unsigned idx ) : code(Code(idx)) {}
    IRegID( char const* _code ) : code(end) { init( _code ); }
  };

};

#endif // ARMD64DBA_ARCH_HH
