/*
 *  Copyright (c) 2017-2020,
 *  Commissariat a l'Energie Atomique (CEA),
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
 
#ifndef __UNISIM_UTIL_SYMBOLIC_VECTOR_VECTOR_HH__
#define __UNISIM_UTIL_SYMBOLIC_VECTOR_VECTOR_HH__

#include <unisim/util/symbolic/symbolic.hh>

namespace unisim {
namespace util {
namespace symbolic {
namespace vector {

  struct VUConfig
  {
    struct Byte
    {
      Byte() : sexp(), span() {}
      Byte( Expr const& _sexp, int _span ) : sexp(_sexp), span(_span) {}
      Byte(unsigned value) : sexp(unisim::util::symbolic::make_const(uint8_t(value))), span(1) {}

      void source( Expr const& _sexp, unsigned _span ) { sexp = _sexp; span = _span; }
      void repeat( unsigned dist ) { sexp = Expr(); span = dist; }

      unsigned is_source() const { return sexp.node ? span : 0; }
      unsigned is_repeat() const { return sexp.node ? 0 : span; }
      bool     is_none() const   { return not sexp.node and not span; }
      ExprNode const* get_node() const { return sexp.node; }
      unsigned size() const { if (not sexp.good()) { struct Bad {}; throw Bad(); } return span; }
      Expr const& expr() const { return sexp; }

    protected:
      Expr     sexp;
      unsigned span;
    };

    struct VMix : public ExprNode
    {
      VMix( Expr const& _l, Expr const& _r ) : l(_l), r(_r) {}
      virtual unsigned SubCount() const override { return 0; }
      virtual void Repr( std::ostream& sink ) const override;
      virtual int cmp( ExprNode const& brhs ) const override { return 0; }
      virtual VMix* Mutate() const override { return new VMix( *this ); }
      virtual ScalarType::id_t GetType() const override { return l->GetType(); }
      Expr l, r;
    };

    struct VTransBase : public Byte, public ExprNode
    {
      VTransBase( Byte const& byte, int _rshift ) : Byte( byte ), rshift(_rshift) {}
      virtual unsigned SubCount() const override { return 0; }
      virtual void Repr( std::ostream& sink ) const override;
      virtual int cmp( ExprNode const& brhs ) const override { return compare( dynamic_cast<VTransBase const&>(brhs) ); }
      int compare( VTransBase const& rhs ) const
      {
        if (int delta = int(Byte::span) - int(rhs.span)) return delta;
        return rshift - rhs.rshift;
      }
      int rshift;
    };
    
    template <typename T>
    struct VTrans : public VTransBase
    {
      VTrans( Byte const& byte, int rshift ) : VTransBase( byte, rshift ) {}
      typedef VTrans<T> this_type;
      virtual this_type* Mutate() const override { return new this_type( *this ); }
      virtual ScalarType::id_t GetType() const override { return T::GetType(); }
    };

    template <typename T>
    struct TypeInfo
    {
      enum { bytecount = sizeof (typename T::value_type) };
      static void ToBytes( Byte* dst, T& src )
      {
        dst->source( src.expr, bytecount );
        for (unsigned idx = 1, end = bytecount; idx < end; ++idx)
          dst[idx].repeat(idx);
      }
      static void FromBytes( T& dst, Byte const* byte )
      {
        struct CorruptedSource {};
        if (byte->is_none())
          {
            /* leave uninitialized */
            return;
          }

        if (int pos = byte->is_repeat())
          {
            Byte const* base = byte - pos;
            if (not base->is_source()) { struct Bad {}; throw Bad(); }
            dst = T(Expr(new VTrans<T>(*base, -pos)));
            return;
          }

        if (unsigned src_size = byte->is_source())
          {
            Expr res = new VTrans<T>(*byte, 0);
            for (unsigned next = src_size, idx = 1; next < bytecount; ++idx)
              {
                // Requested read is a concatenation of multiple source values
                for (;idx < next; ++idx)
                  if (byte[idx].get_node()) throw CorruptedSource();
                if (ExprNode const* node = byte[idx].get_node())
                  res = new VMix( new VTrans<T>(byte[idx], int(idx)), res );
                else
                  throw CorruptedSource(); // missing value
                if (unsigned span = byte[idx].is_source())
                  next = idx + span;
                else
                  throw CorruptedSource();
              }
            dst = T(res);
            return;
          }

        throw CorruptedSource();
      }
      static void Destroy( T& obj ) { obj.~T(); }
      static void Allocate( T& obj ) { new (&obj) T(); }
    };
  };

} /* end of namespace vector */
} /* end of namespace symbolic */
} /* end of namespace util */
} /* end of namespace unisim */


#endif // __UNISIM_UTIL_SYMBOLIC_VECTOR_VECTOR_HH__
