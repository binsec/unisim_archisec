/*
 *  Copyright (c) 2025,
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

template <class ARCH, class VR>
struct AESDec : public Op3V<ARCH,VR>
{
  AESDec( OpBase<ARCH> const& opbase, RMOp<ARCH>&& rm, unsigned vn, unsigned gn ) : Op3V<ARCH,VR>(opbase, std::move(rm), vn, gn) {}

  typedef Op3V<ARCH,VR> op3v;
  using op3v::rm; using op3v::vn; using op3v::gn;

  void disasm( std::ostream& sink ) const { sink << op3v::vprefix() << "aesdec "; op3v::disasmVVW(sink); }

  void execute( ARCH& arch ) const
  {
    typedef typename ARCH::u128_t u128_t;
    arch.vmm_write( VR(), gn, 0, ARCH::aesdec( arch.vmm_read( VR(), vn, 0, u128_t() ), arch.vmm_read( VR(), rm, 0, u128_t() ) ) );
  }
};

/* AESDEC -- Perform One Round of an AES Decryption Flow */

template <class ARCH> struct DC<ARCH,AESDEC> { Operation<ARCH>* get( InputCode<ARCH> const& ic )
{
  if (ic.f0()) return 0;

  if (auto _ = match( ic, vex( "\x66\x0f\x38\xde" ) & RM() ))
    {
      unsigned gn = _.greg();
      if (not ic.vex())     return new AESDec<ARCH,SSE>(_.opbase(), _.rmop(), gn, gn);
      unsigned vn = ic.vreg();
      if (ic.vlen() == 128) return new AESDec<ARCH,XMM>(_.opbase(), _.rmop(), vn, gn);
    }

  return 0;
}};

template <class ARCH, class VR>
struct AESDecLast : public Op3V<ARCH,VR>
{
  AESDecLast( OpBase<ARCH> const& opbase, RMOp<ARCH>&& rm, unsigned vn, unsigned gn ) : Op3V<ARCH,VR>(opbase, std::move(rm), vn, gn) {}

  typedef Op3V<ARCH,VR> op3v;
  using op3v::rm; using op3v::vn; using op3v::gn;

  void disasm( std::ostream& sink ) const { sink << op3v::vprefix() << "aesdeclast "; op3v::disasmVVW(sink); }

  void execute( ARCH& arch ) const
  {
    typedef typename ARCH::u128_t u128_t;
    arch.vmm_write( VR(), gn, 0, ARCH::aesdeclast( arch.vmm_read( VR(), vn, 0, u128_t() ), arch.vmm_read( VR(), rm, 0, u128_t() ) ) );
  }
};

/* AESDECLAST -- Perform Last Round of an AES Decryption Flow */

template <class ARCH> struct DC<ARCH,AESDECLAST> { Operation<ARCH>* get( InputCode<ARCH> const& ic )
{
  if (ic.f0()) return 0;

  if (auto _ = match( ic, vex( "\x66\x0f\x38\xdf" ) & RM() ))
    {
      unsigned gn = _.greg();
      if (not ic.vex())     return new AESDecLast<ARCH,SSE>(_.opbase(), _.rmop(), gn, gn);
      unsigned vn = ic.vreg();
      if (ic.vlen() == 128) return new AESDecLast<ARCH,XMM>(_.opbase(), _.rmop(), vn, gn);
    }

  return 0;
}};

template <class ARCH, class VR>
struct AESEnc : public Op3V<ARCH,VR>
{
  AESEnc( OpBase<ARCH> const& opbase, RMOp<ARCH>&& rm, unsigned vn, unsigned gn ) : Op3V<ARCH,VR>(opbase, std::move(rm), vn, gn) {}

  typedef Op3V<ARCH,VR> op3v;
  using op3v::rm; using op3v::vn; using op3v::gn;

  void disasm( std::ostream& sink ) const { sink << op3v::vprefix() << "aesenc "; op3v::disasmVVW(sink); }

  void execute( ARCH& arch ) const
  {
    typedef typename ARCH::u128_t u128_t;
    arch.vmm_write( VR(), gn, 0, ARCH::aesenc( arch.vmm_read( VR(), vn, 0, u128_t() ), arch.vmm_read( VR(), rm, 0, u128_t() ) ) );
  }
};

/* AESENC -- Perform One Round of an AES Encryption Flow */

template <class ARCH> struct DC<ARCH,AESENC> { Operation<ARCH>* get( InputCode<ARCH> const& ic )
{
  if (ic.f0()) return 0;

  if (auto _ = match( ic, vex( "\x66\x0f\x38\xdc" ) & RM() ))
    {
      unsigned gn = _.greg();
      if (not ic.vex())     return new AESEnc<ARCH,SSE>(_.opbase(), _.rmop(), gn, gn);
      unsigned vn = ic.vreg();
      if (ic.vlen() == 128) return new AESEnc<ARCH,XMM>(_.opbase(), _.rmop(), vn, gn);
    }

  return 0;
}};

template <class ARCH, class VR>
struct AESEncLast : public Op3V<ARCH,VR>
{
  AESEncLast( OpBase<ARCH> const& opbase, RMOp<ARCH>&& rm, unsigned vn, unsigned gn ) : Op3V<ARCH,VR>(opbase, std::move(rm), vn, gn) {}

  typedef Op3V<ARCH,VR> op3v;
  using op3v::rm; using op3v::vn; using op3v::gn;

  void disasm( std::ostream& sink ) const { sink << op3v::vprefix() << "aesenclast "; op3v::disasmVVW(sink); }

  void execute( ARCH& arch ) const
  {
    typedef typename ARCH::u128_t u128_t;
    arch.vmm_write( VR(), gn, 0, ARCH::aesenclast( arch.vmm_read( VR(), vn, 0, u128_t() ), arch.vmm_read( VR(), rm, 0, u128_t() ) ) );
  }
};

/* AESENCLAST -- Perform Last Round of an AES Encryption Flow */

template <class ARCH> struct DC<ARCH,AESENCLAST> { Operation<ARCH>* get( InputCode<ARCH> const& ic )
{
  if (ic.f0()) return 0;

  if (auto _ = match( ic, vex( "\x66\x0f\x38\xdd" ) & RM() ))
    {
      unsigned gn = _.greg();
      if (not ic.vex())     return new AESEncLast<ARCH,SSE>(_.opbase(), _.rmop(), gn, gn);
      unsigned vn = ic.vreg();
      if (ic.vlen() == 128) return new AESEncLast<ARCH,XMM>(_.opbase(), _.rmop(), vn, gn);
    }

  return 0;
}};

template <class ARCH, class VR>
struct AESImc : public Operation<ARCH>
{
  AESImc( OpBase<ARCH> const& opbase, RMOp<ARCH>&& _rm, unsigned _gn ) : Operation<ARCH>(opbase), rm(std::move(_rm)), gn(_gn) {}

  void disasm( std::ostream& sink ) const { sink << (VR::vex()?"v":"") << "aesimc " << DisasmW( VR(), rm ) << ',' << DisasmV( VR(), gn ); }

  void execute( ARCH& arch ) const
  {
    typedef typename ARCH::u128_t u128_t;
    arch.vmm_write( VR(), gn, 0, ARCH::aesimc( arch.vmm_read( VR(), rm, 0, u128_t() ) ) );
  }

  RMOp<ARCH> rm; uint8_t gn;
};

/* AESIMC -- Perform the AES InvMixColumn Transformation */

template <class ARCH> struct DC<ARCH,AESIMC> { Operation<ARCH>* get( InputCode<ARCH> const& ic )
{
  if (ic.f0()) return 0;

  if (auto _ = match( ic, vex( "\x66\x0f\x38\xdb" ) & RM() ))
    {
      unsigned gn = _.greg();
      if (not ic.vex())     return new AESImc<ARCH,SSE>(_.opbase(), _.rmop(), gn);
      if (ic.vreg()) return 0;
      if (ic.vlen() == 128) return new AESImc<ARCH,XMM>(_.opbase(), _.rmop(), gn);
    }

  return 0;
}};

template <class ARCH, class VR>
struct AESKeyGenAssist : public Operation<ARCH>
{
  AESKeyGenAssist( OpBase<ARCH> const& opbase, RMOp<ARCH>&& _rm, uint8_t _gn, uint8_t _im ) : Operation<ARCH>(opbase), rm(std::move(_rm)), gn(_gn), im(_im) {}

  void disasm( std::ostream& sink ) const { sink << (VR::vex()?"v":"") << "aeskeygenassist " << DisasmI(im) << ',' << DisasmW( VR(), rm ) << ',' << DisasmV( VR(), gn ); }

  void execute( ARCH& arch ) const
  {
    typedef typename ARCH::u128_t u128_t;
    arch.vmm_write( VR(), gn, 0, ARCH::aeskeygenassist( arch.vmm_read( VR(), rm, 0, u128_t() ), im ) );
  }

  RMOp<ARCH> rm; uint8_t gn, im;
};

/* AESKEYGENASSIST -- Perform the AES InvMixColumn Transformation */

template <class ARCH> struct DC<ARCH,AESKEYGENASSIST> { Operation<ARCH>* get( InputCode<ARCH> const& ic )
{
  if (ic.f0()) return 0;

  if (auto _ = match( ic, vex( "\x66\x0f\x3a\xdf" ) & RM() & Imm<8>() ))
    {
      unsigned gn = _.greg();
      if (not ic.vex())     return new AESKeyGenAssist<ARCH,SSE>(_.opbase(), _.rmop(), gn, _.i(uint8_t()));
      if (ic.vreg()) return 0;
      if (ic.vlen() == 128) return new AESKeyGenAssist<ARCH,XMM>(_.opbase(), _.rmop(), gn, _.i(uint8_t()));
    }

  return 0;
}};

