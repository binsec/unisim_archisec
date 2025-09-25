/*
 *  Copyright (c) 2020,
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
#include <vector>

namespace unisim {
namespace util {
namespace symbolic {
namespace vector {

  /* Vector manipulation may involve bit-level type
   * reinterpretation. This are not casts where source type affects
   * destination type transformation.  Two operation are considered:
   * "VTrans" which reinterprets source bits (or a subset of) as a new
   * type and "VCat" which concatenates source bits of multiple
   * inputs and reinterprets the result as a new type.
   */

  struct VTransBase : public ExprNode
  {
    VTransBase( Expr const& _src, unsigned _srcsize, int _srcpos ) : src(_src), srcsize(_srcsize), srcpos(_srcpos) {}
    virtual unsigned SubCount() const override { return 1; }
    virtual Expr const& GetSub(unsigned idx) const override { if (idx!= 0) return ExprNode::GetSub(idx); return src; }
    virtual void Repr( std::ostream& sink ) const override;
    virtual int cmp( ExprNode const& brhs ) const override { return compare( dynamic_cast<VTransBase const&>(brhs) ); }
    int compare( VTransBase const& rhs ) const
    {
      if (int delta = int(srcsize) - int(srcsize)) return delta;
      return srcpos - rhs.srcpos;
    }
    Expr     src;     /* source */
    unsigned srcsize; /* size of source, in bytes */
    int      srcpos;  /* start position from source, in bytes */
  };

  template <typename T>
  struct VTrans : public VTransBase
  {
    VTrans( Expr const& src, unsigned srcsize, int srcpos ) : VTransBase( src, srcsize, srcpos ) {}
    typedef VTrans<T> this_type;
    virtual this_type* Mutate() const override { return new this_type( *this ); }
    virtual ValueType GetType() const override { return T::GetType(); }
  };

  struct VCatBase : public ExprNode
  {
    VCatBase( std::vector<Expr>&& _inputs, unsigned _subsize ) : inputs(std::move(_inputs)), subsize(_subsize) {}
    virtual unsigned SubCount() const override { return inputs.size(); }
    virtual Expr const& GetSub(unsigned idx) const override { if (idx < inputs.size()) return inputs[idx];  return ExprNode::GetSub(idx); };
    virtual void Repr( std::ostream& sink ) const override;
    int compare(VCatBase const& rhs) const { if (intptr_t delta = inputs.size() - rhs.inputs.size()) return delta > 0 ? +1 : -1; return 0; }
    virtual int cmp( ExprNode const& brhs ) const override { return compare( dynamic_cast<VCatBase const&>(brhs) ); }
    std::vector<Expr> inputs;
    unsigned subsize; /* size of sub parts in bytes*/
  };

  template <typename T>
  struct VCat : public VCatBase
  {
    VCat( std::vector<Expr>&& inputs, unsigned subsize ) : VCatBase(std::move(inputs), subsize) {}
    typedef VCat<T> this_type;
    virtual this_type* Mutate() const override { return new this_type( *this ); }
    virtual ValueType GetType() const override { return T::GetType(); }
  };

  struct VUConfig
  {
    struct Byte
    {
      Byte() : sexp(), span() {}
      Byte( Expr const& _sexp, int _span ) : sexp(_sexp), span(_span) {}
      Byte(unsigned value) : sexp(make_const(uint8_t(value))), span(1) {}

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
      static Expr mkVTrans( Expr const& src, unsigned srcsize, int srcpos )
      {
        return new VTrans<T>(src, srcsize, srcpos);
      }
      static Expr mkVCat( std::vector<Expr>&& inputs, unsigned subsize )
      {
        // // Try some recombinations
        // struct Failed {};
        // try
        //   {
        //     ExprNode const* head = inputs[0].node;
        //     if (auto vt = dynamic_cast<VTransBase const*>( head ))
        //       {
        //         unsigned start = vt->srcpos, end = start;
        //         for (auto const& input : inputs)
        //           {
        //             if (auto vti = dynamic_cast<VTransBase const*>( input.node ))
        //               {
        //                 if (vti->src != vt->src or vti->srcpos != end)
        //                   throw Failed();
        //               }
        //             else throw Failed();
        //             end += subsize;
        //           }
        //         return mkVTrans(vt->src, end - start, start);
        //       }
        //     if (auto vt = dynamic_cast<VCatBase const*>( head ))
        //       {
        //         std::vector<Expr> parts;
        //         parts.reserve((inputs.size()*subsize)/vt->subsize);
        //         for (auto const& input : inputs)
        //           {
        //             if (auto vti = dynamic_cast<VCatBase const*>( input.node ))
        //               {
        //                 if (vti->subsize != vt->subsize)
        //                   throw Failed();
        //                 for (auto const& part : vti->inputs)
        //                   parts.push_back(part);
        //               }
        //             else throw Failed();
        //           }
        //         return mkVCat(std::move(parts),vt->subsize);
        //       }
        //   }
        // catch (Failed const&) {}

        return new VCat<T>(std::move(inputs), subsize);
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
            unsigned size = base->size();
            if (unsigned(pos + bytecount) > size) throw CorruptedSource();
            dst = mkVTrans(base->expr(), size, pos);
            return;
          }

        if (unsigned src_size = byte->is_source())
          {
            if (src_size >= bytecount)
              {
                dst = mkVTrans(byte->expr(), byte->size(), 0);
                return;
            }

            // Requested read is a concatenation of multiple source values
            unsigned itemcount = bytecount / src_size;
            std::vector<Expr> parts(itemcount);
            for (unsigned idx = 0; idx < itemcount; ++idx)
              {
                unsigned start = idx*src_size;
                if (byte[start].is_source() != src_size)
                  throw CorruptedSource();
                parts[idx] = byte[start].get_node();
              }
            dst = mkVCat(std::move(parts), src_size);
            return;
          }

        throw CorruptedSource();
      }
      static void Destroy( T& obj ) { obj.~T(); }
      static void Allocate( T& obj ) { new (&obj) T(); }
    };
  };


  ExprNode const* corresponding_origin( Expr const& dst, unsigned dpos, unsigned spos );


} /* end of namespace vector */
} /* end of namespace symbolic */
} /* end of namespace util */
} /* end of namespace unisim */


#endif // __UNISIM_UTIL_SYMBOLIC_VECTOR_VECTOR_HH__
