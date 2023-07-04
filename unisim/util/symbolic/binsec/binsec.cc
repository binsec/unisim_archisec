/*
 *  Copyright (c) 2007-2023,
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

#include <unisim/util/symbolic/binsec/binsec.hh>
#include <ostream>
#include <sstream>
#include <iomanip>

namespace unisim {
namespace util {
namespace symbolic {
namespace binsec {

  std::ostream&
  operator << ( std::ostream& sink, dbx const& _ )
  {
    std::ios_base::fmtflags f( sink.flags() );

    sink << "0x" << std::hex << std::setw(_.bytes*2) << std::setfill('0') << _.value;

    sink.flags( f );

    return sink;
  }

  bool
  Label::subst_next( std::string& s, int next )
  {
    uintptr_t pos = s.find( "<next>" );
    if (pos == std::string::npos)
      return false;
    { std::ostringstream buf; buf << next; s.replace(pos, 6, buf.str()); }
    return true;
  }

  int
  ASExprNode::GenerateCode( Expr const& expr, Variables& vars, Label& label, std::ostream& sink )
  {
    /*** Pre expression process ***/
    Variables::iterator itr = vars.find( expr );
    if (itr != vars.end())
      {
        sink << itr->second.first;
        return itr->second.second;
      }

    /*** Sub expression process ***/
    Expr constant = expr;
    if (ConstNodeBase const* node = constant.ConstSimplify())
      {
        auto tp = node->GetType();
        switch (tp.encoding)
          {
          default: break;
          case ValueType::BOOL: sink << node->GetBits(0) << "<1>";  return 1;
          case ValueType::SIGNED: case ValueType::UNSIGNED:
            {
              unsigned bitsize = tp.bitsize;
              if (bitsize % 8) throw 0;

              {
                std::ios_base::fmtflags f( sink.flags() );
                sink << "0x" << std::hex << std::setfill('0');
                unsigned bytes = bitsize / 8;
                unsigned intro = ((bytes - 1) & 7) + 1;
                unsigned idx = (bytes - intro)/8;
                uint64_t intromask = uint64_t(-1) >> (-bitsize & 0x3f);
                sink << std::setw(intro*2) << (node->GetBits(idx) & intromask);
                while (idx-- > 0) sink << std::setw(16) << node->GetBits(idx);
                sink.flags( f );
              }

              return bitsize;
            }
          }
        throw std::logic_error("can't encode type");
      }
    else if (OpNodeBase const* node = expr->AsOpNode())
      {
        switch (node->SubCount())
          {
          case 2: {
            auto infix = [&] (char const* op) -> int {
              int lhs_size = GenerateCode( node->GetSub(0), vars, label, sink << '(' );
              sink << ' ' << op << ' ' << GetCode( node->GetSub(1), vars, label ) << ')';
              return lhs_size;
            };

            auto prefix = [&] (char const* op) -> int {
              int lhs_size = GenerateCode( node->GetSub(0), vars, label, sink << op << '(' );
              sink << ", " << GetCode( node->GetSub(1), vars, label ) << ')';
              return lhs_size;
            };

            auto test = [&] (char const* op) -> int {
              sink << '(' << GetCode( node->GetSub(0), vars, label ) << ' ' << op << ' ' << GetCode( node->GetSub(1), vars, label ) << ')';
              return 1;
            };

            auto shift = [&] (char const* op) -> int {
              int lhs_size = GenerateCode( node->GetSub(0), vars, label, sink << '(' );
              Expr rhs = node->GetSub(1);
              switch (lhs_size)
                {
                case 16: rhs = U16(U8(rhs)).expr; BitSimplify::Do(rhs); break;
                case 32: rhs = U32(U8(rhs)).expr; BitSimplify::Do(rhs); break;
                case 64: rhs = U64(U8(rhs)).expr; BitSimplify::Do(rhs); break;
                }
              sink << ' ' << op << ' ' << GetCode( rhs, vars, label ) << ')';
              return lhs_size;
            };
        
            switch (node->op.code)
              {
              default:          break;
              case Op::Add:     return infix("+");
              case Op::Sub:     return infix("-");
              case Op::Mul:     return infix("*");
              case Op::Mod:     return infix("mods");
              case Op::Modu:    return infix("modu");
              case Op::Div:     return infix("/s");
              case Op::Divu:    return infix("/u");

              case Op::Xor:     return infix("xor");
              case Op::Or:      return infix("or");
              case Op::And:     return infix("and");

              case Op::Teq:     return test("=");
              case Op::Tne:     return test("<>");

              case Op::Tle:     return test("<=s");
              case Op::Tleu:    return test("<=u");

              case Op::Tge:     return test(">=s");
              case Op::Tgeu:    return test(">=u");

              case Op::Tlt:     return test("<s");
              case Op::Tltu:    return test("<u");

              case Op::Tgt:     return test(">s");
              case Op::Tgtu:    return test(">u");


              case Op::Lsl:     return shift("lshift");
              case Op::Asr:     return shift("rshifts");
              case Op::Lsr:     return shift("rshiftu");
              case Op::Ror:     return shift("rrotate");

              case Op::Min:     return prefix("min");
              case Op::Max:     return prefix("max");
              }

            std::ostringstream buf;
            buf << "[" << node->op.c_str() << "]";
            return infix(buf.str().c_str());
          }

          case 1: {
            char const* operation = 0;

            switch (node->op.code)
              {
              default:              operation = node->op.c_str(); break;

              case Op::Not:    operation = "not "; break;
              case Op::Neg:    operation = "- "; break;

                // case Op::BSwp:  break;
              case Op::BSF:
                {
                  unsigned bitsize = node->GetType().bitsize;
		  sink << "(";
		  for (unsigned i = 0; i < bitsize - 1; i += 1) {
		    sink << "if " << GetCode(node->GetSub(0), vars, label)
			 << '{' << i << "} then "
			 << i << '<' << bitsize << "> else (";
		  }
		  sink << bitsize - 1 << '<' << bitsize << '>';
		  for (unsigned i = 0; i < bitsize; i += 1) {
		    sink << ')';
		  }
                  // Label head(label);
                  // int exit = label.allocate(), loop;

                  // std::ostringstream buffer;
                  // buffer << "bsf_in<" << bitsize << "> := " << GetCode(node->GetSub(0), vars, head) << " ; goto <next>";
                  // head.write( buffer.str() );

                  // buffer = std::ostringstream();
                  // buffer << "bsf_out<" << bitsize << "> := 0<" << bitsize << "> ; goto <next>";
                  // head.write( buffer.str() );

                  // buffer = std::ostringstream();
                  // buffer << "if (bsf_in<" << bitsize << "> = 0<" << bitsize << ">) goto " << exit << " else goto <next>";
                  // head.write( buffer.str() );

                  // buffer = std::ostringstream();
                  // buffer << "if ((bsf_in<" << bitsize << "> rshiftu bsf_out<" << bitsize << ">){0,0}) goto " << exit << " else goto <next>";
                  // loop = head.write( buffer.str() );

                  // buffer = std::ostringstream();
                  // buffer << "bsf_out<" << bitsize << "> := bsf_out<" << bitsize << "> + 1<" << bitsize << "> ; goto " << loop;
                  // head.write( buffer.str()  );

                  // sink << "bsf_out<" << bitsize << ">";

                  return bitsize;
                }

              case Op::BSR:
                {
                  unsigned bitsize = node->GetType().bitsize;
		  sink << "(";
		  for (unsigned i = bitsize - 1; i > 1; i -= 1) {
		    sink << "if " << GetCode(node->GetSub(0), vars, label)
			 << '{' << i << "} then "
			 << i << '<' << bitsize << "> else (";
		  }
		  sink << "extu ("
		       << GetCode(node->GetSub(0), vars, label)
		       << "{1}) " << bitsize;
		  for (unsigned i = bitsize; i > 1; i -= 1) {
		    sink << ')';
		  }
                  // Label head(label);
                  // int exit = label.allocate(), loop;

                  // std::ostringstream buffer;
                  // buffer << "bsr_in<" << bitsize << "> := " << GetCode(node->GetSub(0), vars, head) << " ; goto <next>";
                  // head.write( buffer.str() );

                  // buffer = std::ostringstream();
                  // buffer << "bsr_out<" << bitsize << "> := " << bitsize << "<" << bitsize << "> ; goto <next>";
                  // head.write( buffer.str() );

                  // buffer = std::ostringstream();
                  // buffer << "if (bsr_in<" << bitsize << "> = 0<" << bitsize << ">) goto " << exit << " else goto <next>";
                  // head.write( buffer.str() );

                  // buffer = std::ostringstream();
                  // buffer << "bsr_out<" << bitsize << "> := bsr_out<" << bitsize << "> - 1<" << bitsize << "> ; goto <next>";
                  // loop = head.write( buffer.str()  );

                  // buffer = std::ostringstream();
                  // buffer << "if ((bsr_in<" << bitsize << "> rshiftu bsr_out<" << bitsize << ">){0,0}) goto " << exit << " else goto " << loop;
                  // head.write( buffer.str() );

                  // sink << "bsr_out<" << bitsize << ">";

                  return bitsize;
                }
              case Op::Tnzero:
                {
                  Expr const& src = node->GetSub(0);
                  sink << "(" << GetCode(src, vars, label) << " <> " << dbx(src->GetType().bitsize/8,0) << ")";
                  return 1;
                }
              case Op::Cast:
                {
                  CastNodeBase const& cnb = dynamic_cast<CastNodeBase const&>( *expr.node );
                  auto src = cnb.GetSrcType(), dst = cnb.GetType();

                  /* TODO: At this point, boolean casts should not remain */
                  if (dst.encoding == dst.BOOL)
                    {
                      sink << "(" << GetCode(cnb.src, vars, label) << " <> " << dbx(src.bitsize/8,0) << ")";
                    }
                  else
                    {
                      throw std::logic_error("Unexpected cast");
                      /* TODO: What to do with FP casts ? */
                      dst.Repr(sink);
                      sink << "( " << GetCode(cnb.src, vars, label) << " )";
                    }

                  return dst.bitsize;
                }
              }

            sink << '(' << operation << ' ';
            int retsz = GenerateCode( node->GetSub(0), vars, label, sink );
            sink << ')';
            return retsz;
          }
          default:
            break;
          }
      }
    else if (auto vt = dynamic_cast<vector::VTransBase const*>( expr.node ))
      {
        unsigned srcsize = 8*vt->srcsize, dstsize = vt->GetType().bitsize, srcpos = 8*vt->srcpos;
        
        if (dstsize < srcsize)
          sink << "(" << GetCode(vt->src, vars, label) << " {" << std::dec << srcpos << ", " << (srcpos+dstsize-1) << "})";
        else
          sink << GetCode(vt->src, vars, label);
        return dstsize;
      }
    else if (auto mix = dynamic_cast<vector::VMix const*>( expr.node ))
      {
        decltype(mix) prev;
        sink << "(";
        int retsize = 0;
        do
          {
            prev = mix;
            retsize += GenerateCode( mix->l, vars, label, sink );
            sink << " :: ";
            mix = dynamic_cast<vector::VMix const*>( mix->r.node );
          }
        while (mix);
        retsize += GenerateCode(prev->r, vars, label, sink);
        sink << ")";
        return retsize;
      }
    else if (ASExprNode const* node = dynamic_cast<ASExprNode const*>( expr.node ))
      {
        return node->GenCode( label, vars, sink );
      }

    throw std::logic_error("No GenCode method");
    return 0;
  }

  ConstNodeBase const*
  ASExprNode::Simplify( Expr const& mask, Expr& expr ) const
  {
    return expr.Simplify(BitSimplify());
  }
  
  Expr
  BitFilter::mksimple( Expr const& input, unsigned source, unsigned rshift, unsigned select, unsigned extend, bool sxtend )
  {
    BitFilter bf( input, source, rshift, select, extend, sxtend );

    bf.Retain(); // Prevent deletion of this stack-allocated object
    
    Expr sbf = &bf;
    BitSimplify::Do(sbf);
    return (sbf.node == &bf) ? new BitFilter( bf ) : sbf.node;
  }

  int
  BitFilter::compare( BitFilter const& rhs ) const
  {
    if (int delta = int(source) - int(rhs.source)) return delta;
    if (int delta = int(rshift) - int(rhs.rshift)) return delta;
    if (int delta = int(select) - int(rhs.select)) return delta;
    if (int delta = int(extend) - int(rhs.extend)) return delta;
    return int(sxtend) - int(rhs.sxtend);
  }

  ConstNodeBase const*
  BitFilter::Eval( ConstNodeBase const** cnbs ) const
  {
    Expr scratch = cnbs[0];
    // if (rshift)
    //   scratch = make_operation(Op::Lsr, scratch, make_const<shift_type>(rshift));

    int shift = extend - (rshift+select);
    Zero dz(GetType()); dz.Retain(); // Prevent deletion of this stack-allocated object
    if (shift >= 0)
      {
        scratch = make_operation( Op::ReinterpretAs, &dz, scratch );
        if (shift)
          scratch = make_operation( Op::Lsl, scratch, make_const<shift_type>(shift) );
      }
    else
      {
        scratch = make_operation( Op::Lsr, scratch, make_const<shift_type>(-shift) );
        scratch = make_operation( Op::ReinterpretAs, &dz, scratch );
      }

    Expr xshift =  make_const<shift_type>(extend - select);
    ConstNodeBase const* args[2] = {scratch.ConstSimplify(), xshift.ConstSimplify()};
    return args[0]->apply(sxtend ? Op::Asr : Op::Lsr, args);
  }

  int
  BitFilter::GenCode( Label& label, Variables& vars, std::ostream& sink ) const
  {
    bool selection = source > select;
    if (extend == source and selection)
      {
        bool done = true;
        if      (not rshift and not sxtend and select < 64)
          sink << "(" << GetCode(input, vars, label, source) << " and " << dbx(source/8, (1ull << select)-1) << ")";
        else if ((rshift + select) == extend)
          sink << "(" << GetCode(input, vars, label, source) << " rshift" << (sxtend?"s ":"u ") << dbx(source/8, rshift) << ")";
        else
          done = false;

        if (done)
          return extend;
      }

    bool extension = extend > select;
    if (extension)
      sink << '(' << (sxtend ? "exts " : "extu ");
    if (selection)
      sink << '(';

    int chksize = ASExprNode::GenerateCode( input, vars, label, sink );
    if (chksize != source) throw 0;

    if (selection)
      sink << " {" << (rshift) << "," << (rshift+select-1) << "})";
    if (extension)
      sink << ' ' << extend << ')';

    return extend;
  }

  void
  BitFilter::Repr( std::ostream& sink ) const
  {
    sink << "BitFilter(" << input
         << ", " << source
         << ", " << rshift
         << ", " << select
         << ", " << extend
         << ", " << (sxtend ? "signed" : "unsigned")
         << ")";
  }

  int
  RegReadBase::GenCode( Label& label, Variables& vars, std::ostream& sink ) const
  {
    unsigned bitsize = GetType().bitsize;
    GetRegName( sink );
    sink << "<" << bitsize << ">";
    return bitsize;
  }

  void
  RegReadBase::Repr( std::ostream& sink ) const
  {
    sink << "RegRead( ";
    GetRegName( sink );
    sink << " )";
  }

  int
  RegWriteBase::GenerateCode( Label& label, Variables& vars, std::ostream& sink ) const
  {
    /* Name of the assigned register */
    int lhsize = size, rhsize;
    GetRegName( sink );
    sink << '<' << size << '>';
    if (rsize != size)
      {
        sink << '{' << rbase << ',' << (rbase + rsize - 1) << '}';
        lhsize = rsize;
      }
    sink << " := ";
    if (value->AsConstNode())
      rhsize = ASExprNode::GenerateCode( value, vars, label, sink );
    else
      {
        auto const& var = vars[value];
        if (var.first.empty())
          throw std::logic_error( "corrupted sink" );
        sink << var.first;
        rhsize = var.second;
      }
    if (lhsize != rhsize)
      throw std::logic_error( "none matching size in register assignment" );
    return rsize;
  }

  void RegWriteBase::Repr( std::ostream& sink ) const
  {
    sink << "RegWrite( ";
    GetRegName( sink );
    sink << ", " << size << ", " << rbase << ", " << rsize << ", ";
    value->Repr( sink );
    sink << " )";
  }

  void
  Branch::Repr( std::ostream& sink ) const
  {
    sink << "Branch(";
    value->Repr( sink );
    sink << ")";
  }

  int
  Load::GenCode( Label& label, Variables& vars, std::ostream& sink ) const
  {
    /* TODO: exploit alignment info */
    sink << "@[" << GetCode(addr, vars, label) << ',' << (bigendian?"->":"<-") << ',' << bytecount() << "]";
    return 8*bytecount();
  }

  int
  Store::GenCode( Label& label, Variables& vars, std::ostream& sink ) const
  {
    /* TODO: exploit alignment info */
    sink << "@[" << GetCode(addr, vars, label) << ',' << (bigendian?"->":"<-") << ',' << bytecount() << "] := " << GetCode(value, vars, label);
    return 0;
  }

  void
  MemAccess::Repr( std::ostream& sink ) const
  {
    sink << "[" << addr << ',' << bytecount() << ",^" << alignment << ',' << (bigendian ? "be" : "le") << "]";
  }

  void
  Store::Repr( std::ostream& sink ) const
  {
    MemAccess::Repr( sink );
    sink << " := " << value;
  }

  namespace {
    struct SinksMerger { void operator () ( std::set<Expr>& sinks, Expr const& l, Expr const& r ) { sinks.insert( l ); } };
  }

  void
  ActionNode::simplify()
  {
    while (cond.good())
      {
        if (ConstNodeBase const* c = BitSimplify::Do(cond))
          {
            //            std::cerr << "warning: post simplification of if-then-else condition\n";
            bool eval = dynamic_cast<ConstNode<bool> const&>(*c).value;
            ActionNode* always = nexts[eval];
            ActionNode* never = nexts[not eval];
            delete never;
            // Import always branch
            for (unsigned idx = 0; idx < 2; ++idx)
              {
                nexts[idx] = always->nexts[idx];
                always->nexts[idx] = 0;
              }
            cond = always->cond;
            sinks.insert(always->sinks.begin(), always->sinks.end());
            delete always;
          }
        else
          break;
      }

    {
      std::set<Expr> nsinks;
      for (std::set<Expr>::const_iterator itr = sinks.begin(), end = sinks.end(); itr != end; ++itr)
        {
          Expr x(*itr);
          BitSimplify::Do(x);
          nsinks.insert( x );
        }
      std::swap(nsinks, sinks);
    }

    if (not cond.good())
      return;

    for (unsigned choice = 0; choice < 2; ++choice)
      if (ActionNode* next = nexts[choice])
        next->simplify();

    factorize( sinks, nexts[0]->sinks, nexts[1]->sinks, SinksMerger() );

    bool leaf = true;
    for (unsigned choice = 0; choice < 2; ++choice)
      if (ActionNode* next = nexts[choice])
        {
          if (next->cond.good() or next->sinks.size()) leaf = false;
          else { delete next; nexts[choice] = 0; }
        }

    if (leaf)
      cond = Expr();
    else if (OpNodeBase const* onb = cond->AsOpNode())
      if (onb->op.code == onb->op.Not)
        {
          // If condition begins with a logical not, remove the not and
          //   swap if then else branches
          cond = onb->GetSub(0);
          std::swap( nexts[false], nexts[true] );
        }

  }

  void
  ActionNode::commit_stats()
  {
    struct Sec
    {
      void Flood( Expr const& e )
      {
        for (unsigned idx = 0, end = e->SubCount(); idx < end; ++idx)
          this->Process( e->GetSub(idx) );
      }
      void Process( Expr const& e )
      {
        if (not e->SubCount())
          return;
        bool cont = true;
        for (Sec* sec = this; sec; sec = sec->up)
          cont &= (sec->stats[e]++ == 0);
        if (cont)
          this->Flood( e );
      }
      Sec( ActionNode* node, Sec* _up )
        : stats(node->sestats), up(_up)
      {
        // First level of expression is not functionnal (architectual side effect)
        for (std::set<Expr>::const_iterator itr = node->sinks.begin(), end = node->sinks.end(); itr != end; ++itr)
          this->Flood( *itr );
        for (unsigned choice = 0; choice < 2; ++choice)
          if (ActionNode* next = node->nexts[choice])
            { Sec sub(next,this); }
      }
      std::map<Expr,unsigned>& stats; Sec* up;
    } sec(this,0);
  }

  void
  Program::Generate( ActionNode const* action_tree )
  {
    struct Context
    {
      Context() : upper(0) {}
      Context( Context* _up ) : upper(_up)
      {
        if (_up) next_tmp = _up->next_tmp;
        if (_up) vars = _up->vars;
      }

      void add_pending( RegWriteBase const* e ) { pendings.push_back(e); }
      bool has_pending() const { return pendings.size() > 0; }

      std::string const& mktemp(Expr const& expr, unsigned size, Assignment const* assignment)
      {
        auto itr = vars.lower_bound(expr);
        if (itr != vars.end() and itr->first == expr)
          throw std::logic_error( "multiple temporary definitions" );
        
        std::string name;
        {
          std::ostringstream buf;
          buf << "%%" << vars.size() << "<" << size << ">";
          name = buf.str();
        }
        
        itr = vars.emplace_hint(itr, std::piecewise_construct, std::forward_as_tuple(expr), std::forward_as_tuple(std::move(name), size) );
        return itr->second.first;
      }
      
      void GenCode( ActionNode const* action_tree, Label const& start, Label const& after )
      {
        Branch const* nia = 0;

        // Using a delayed writing mechanism so that the last code line
        // produced is linked to the next code line given by the upper
        // context instead of a fresh new one
        struct Head
        {
          Head( Label const& _start, int _after ) : cur(_start), after(_after) {}
          ~Head()
          {
            if (not pending.size()) return;
            Label::subst_next(pending, after);
            cur.write( pending );
            pending.clear();
          }

          Label& current() { flush(); return cur; }
          void write( std::string const& s )
          {
            flush();
            if (s.size()==0) throw std::runtime_error("xxx");
            pending = s;
          }
          void flush()
          {
            if (not pending.size()) return;
            cur.write( pending );
            pending.clear();
          }
          int GenCode(Expr const& expr, Context& context, Assignment const* rw)
          {
            std::string tmp_src, tmp_dst;
            int retsize;
            {
              std::ostringstream buffer;
              retsize = ASExprNode::GenerateCode( expr, context.vars, this->current(), buffer );
              tmp_src = buffer.str();
            }
            std::ostringstream buffer;
            buffer << context.mktemp( expr, retsize, rw ) << " := " << tmp_src << "; goto <next>";
            this->write( buffer.str() );
            return retsize;
          }
      
          Label cur;
          int after;
          std::string pending;
        } head( start, after.GetID() );

        {
          // Ordering Sub Expressions by size of expressions (so that
          // smaller expressions are factorized in larger ones)
          struct CSE : public std::multimap<unsigned,Expr>
          {
            void Process( Expr const& expr )
            {
              insert( std::make_pair( CountSubs( expr ), expr ) );
            }
            unsigned CountSubs( Expr const& expr )
            {
              unsigned sum = 1;
              for (unsigned idx = 0, end = expr->SubCount(); idx < end; ++idx)
                sum += CountSubs( expr->GetSub(idx) );
              return sum;
            }
          } cse;

          for (std::map<Expr,unsigned>::const_iterator itr = action_tree->sestats.begin(), end = action_tree->sestats.end(); itr != end; ++itr)
            {
              if (itr->second >= 2 and not this->vars.count(itr->first))
                cse.Process(itr->first);
            }

          // Keeping track of expressions involved in register
          // assignment (Clobbers). They require temporaries (which names
          // may differ from conventional pure CSE temporaries)
          std::map<Expr,Assignment const*> rtmps;
          for (std::set<Expr>::const_iterator itr = action_tree->sinks.begin(), end = action_tree->sinks.end(); itr != end; ++itr)
            {
              if (Assignment const* rw = dynamic_cast<Assignment const*>( itr->node ))
                if (not rw->value->AsConstNode() and not this->vars.count(rw->value))
                  rtmps[rw->value] = rw;
            }
          
          for (std::multimap<unsigned,Expr>::const_iterator itr = cse.begin(), end = cse.end(); itr != end; ++itr)
            {
              std::map<Expr,Assignment const*>::const_iterator rtmp = rtmps.find(itr->second);
              head.GenCode(itr->second, *this, rtmp != rtmps.end() ? rtmp->second : 0);
            }
        }

        for (std::set<Expr>::const_iterator itr = action_tree->sinks.begin(), end = action_tree->sinks.end(); itr != end; ++itr)
          {
            if (Assignment const* assignment = dynamic_cast<Assignment const*>( itr->node ))
              {
                Expr const  value = assignment->value;

                if (not value->AsConstNode() and not this->vars.count(value))
                  head.GenCode(value, *this, assignment);

                if (Branch const* branch = dynamic_cast<Branch const*>( assignment ))
                  { nia = branch; }
                else if (RegWriteBase const* rw = dynamic_cast<RegWriteBase const*>( assignment ))
                  { this->add_pending( rw ); }
                else
                  throw std::logic_error( "unknown assignment" );
              }
            else
              {
                std::ostringstream buffer;
                if (ASExprNode::GenerateCode( *itr, this->vars, head.current(), buffer ))
                  throw std::logic_error( "corrupted sink" );

                buffer << "; goto <next>";
                head.write( buffer.str() );
              }
          }

        if (not action_tree->cond.good())
          {
            if (action_tree->sinks.size() == 0)
              throw std::logic_error( "empty leaf" );
          }
        else
          {
            std::ostringstream buffer;
            Label ifinsn(head.current());
            buffer << "if " << GetCode(action_tree->cond, this->vars, ifinsn) << " ";

            // Preparing room for if then else code
            Label endif( after );
            if (nia or (after.valid() and (this->has_pending())))
              endif.allocate();

            if (not action_tree->nexts[0]) {
              Label ifthen(ifinsn);
              buffer << " goto " << ifthen.allocate() << " else goto " << endif.GetID();
              ifinsn.write( buffer.str() );
              Context nxt(this);
              nxt.GenCode( action_tree->nexts[1], ifthen, endif );
            } else if (not action_tree->nexts[1]) {
              Label ifelse(ifinsn);
              buffer << " goto " << endif.GetID() << " else goto " << ifelse.allocate();
              ifinsn.write( buffer.str() );
              Context nxt(this);
              nxt.GenCode( action_tree->nexts[0], ifelse, endif );
            } else {
              Label ifthen(ifinsn), ifelse(ifinsn);
              buffer << " goto " << ifthen.allocate() << " else goto " << ifelse.allocate();
              ifinsn.write( buffer.str() );
              {
                Context nxt(this);
                nxt.GenCode( action_tree->nexts[1], ifthen, endif );
              }
              {
                Context nxt(this);
                nxt.GenCode( action_tree->nexts[0], ifelse, endif );
              }
            }

            head.current() = endif;
          }

        if (not nia and not after.valid())
          return;

        for (RegWriteBase const* rw : this->pendings)
          {
            std::ostringstream buffer;
            rw->GenerateCode(head.current(), this->vars, buffer);
            buffer << "; goto <next>";
            head.write( buffer.str() );
          }

        if (not nia)
          return;

        Label current( head.current() );

        for (Context* uc = this->upper; uc; uc = uc->upper)
          {
            for (RegWriteBase const* rw : uc->pendings)
              {
                std::ostringstream buffer;
                rw->GenerateCode(current, this->vars, buffer);
                buffer << "; goto <next>";
                current.write( buffer.str() );
              }
          }

        std::ostringstream buffer;
        Expr const& target = nia->value;
        buffer << "goto (" << GetCode(target, this->vars, current) << (target->AsConstNode() ? ",0" : "") << ")";
        nia->annotate( buffer );
        current.write( buffer.str() );
      }

      Context* upper;
      typedef std::vector<RegWriteBase const*> Pendings;
      Pendings pendings;
      Variables vars;
      std::map<unsigned,unsigned> next_tmp;
    };

    Label beglabel(*this), endlabel(*this);
    beglabel.allocate();
    Context ctx;
    ctx.GenCode( action_tree, beglabel, endlabel );
  }

  void
  UndefinedValueBase::Repr( std::ostream& sink ) const
  {
    sink << "UndefinedValue<";
    GetType().Repr(sink);
    sink << ">(";
    for (unsigned idx = 0, end = this->SubCount(); idx < end; ++idx)
      {
        sink << (idx ? ", " : "");
        this->GetSub(idx)->Repr(sink);
      }
    sink << ")";
  }

  int
  UndefinedValueBase::GenCode(Label&, Variables&, std::ostream& sink) const
  {
    sink << "\\undef";
    return GetType().bitsize;
  }

  int
  BitInsertNode::GenCode(Label&, Variables&, std::ostream&) const
  {
    throw 0;
    return 0;
  }

  void
  BitInsertNode::Repr(std::ostream& sink) const
  {
    sink << "BitInsertNode(";
    dst->Repr(sink);
    sink << ',';
    src->Repr(sink);
    sink << ',' << pos << ',' << size << ")";
  }

  Expr BitSimplify::make_zero(ValueType tp) { return new Zero(tp.bitsize == 1 ? tp.BOOL : tp.UNSIGNED, tp.bitsize); }

  namespace
  {
    struct UniBitSimplify : public BitSimplify
    {
      UniBitSimplify(Expr const& _mask, unsigned _count) : mask(_mask), count(_count) {}
      ConstNodeBase const* Simplify(unsigned idx, Expr& expr) const override { return Process(idx < count ? mask : Expr(), expr); }
      Expr mask;
      unsigned count;
    };
    
    struct FiltBitSimplify : public BitSimplify
    {
      FiltBitSimplify(Expr const& _mask, unsigned _filter) : mask(_mask), filter(_filter) {}
      ConstNodeBase const* Simplify(unsigned idx, Expr& expr) const override { return Process((idx >> filter & 1) ? mask : Expr(), expr); }
      Expr mask;
      unsigned filter;
    };
  }

  ConstNodeBase const*
  BitSimplify::Process(Expr const& mask, Expr& expr) const
  {
    if (mask.good() and make_operation(Op::Tzero, mask).Eval<bool>())
      { expr = mask; return expr.ConstSimplify(); }

    /* Early simplification for twin inputs */
    do
      {
        if (expr->SubCount() != 2)
          break;
        Expr args[] = {expr->GetSub(0), expr->GetSub(1)};
        if (args[0] != args[1])
          break;
        auto const* node = expr->AsOpNode();
        if (not node)
          break;
        switch (node->op.code)
          {
          default: break;
          case Op::And: case Op::Or:
            expr = args[0];
            break;
            
          case Op::Xor: case Op::Sub:
            expr = BitSimplify::make_zero(args[0]->GetType());
            break;
          }
      }
    while (0);

    for (ExprNode const* original = 0; original != expr.node;)
      {
        original = expr.node;
        unsigned subcount = expr->SubCount();

        if (auto const* node = expr->AsOpNode())
          {
            switch (auto op = node->op.code)
              {
              default: break;

              case Op::Or:
              case Op::And:
                {
                  /* Compute significant bits for AND and ignored bits
                   * for OR. `umask` is for upstream bits and `dmask` is
                   * for downstream bits.
                   */
                  Expr umask = mask.good() ? mask : make_zero(expr->GetType());
                  if (mask.good() xor (op == Op::And))
                    umask = make_operation(Op::Not, umask);
                  Expr dmask = umask;

                  unsigned isvar = 0; /* keep track of non-const sub-expressions */
                  for (unsigned side = 0; side < subcount; ++side)
                    if (ConstNodeBase const* cst = expr->GetSub(side)->AsConstNode())
                      dmask = make_operation(op, dmask, make_operation(Op::ReinterpretAs, dmask, cst) );
                    else
                      isvar |= 1 << side;

                  if (isvar and not (isvar & (isvar-1)))
                    {
                      /* There is one non-const source left. See if we
                       * can make a simpler expression from it.
                       */
                      Expr const& src = expr->GetSub(arithmetic::BitScanForward(isvar));
                      dmask.ConstSimplify();
                      if (op == Op::And and make_operation(Op::Tzero, make_operation(Op::And, dmask, make_operation(Op::Inc, dmask))).Eval<bool>())
                        {
                          if (make_operation(Op::Tzero, dmask).Eval<bool>())
                            { expr = dmask; return expr->AsConstNode(); }

                          /* Make it a BitFilter */
                          unsigned bitsize = src->GetType().bitsize, select = 0;
                          {
                            ConstNodeBase const* cst = dmask.ConstSimplify();
                            
                            for (unsigned word = (bitsize+63)/64; word-->0;)
                              if (uint64_t bits = cst->GetBits(word))
                                { select = word*64 + unisim::util::arithmetic::BitScanReverse(bits) + 1; break; }
                            if (not select)
                              { struct Bad {}; throw Bad (); }
                          }

                          if (select >= bitsize)
                            expr = src;
                          else
                            expr = BitFilter::mksimple( src, bitsize, 0, select, bitsize, false );
                          continue;
                        }
                      if (make_operation( Op::Teq, umask, dmask ).Eval<bool>())
                        {
                          /* The AND/OR operation is not doing anything useful; its clearing/setting bits that will be ignored later. */
                          expr = src;
                          continue;
                        }
                    }

                  if (op == Op::Or)
                    dmask = make_operation(Op::Not, dmask);

                  if (auto cst = expr.Simplify(FiltBitSimplify(dmask, isvar)))
                    return cst;
                  continue;
                }
                break;

              case Op::Xor:
                /* TODO: Simplify trivial cases (xor 0 and xor -1) */
                if (auto cst = expr.Simplify(UniBitSimplify(mask, subcount)))
                  return cst;
                continue;
                break;

              case Op::Asr:
              case Op::Lsr:
              case Op::Lsl:
                if (subcount == 2)
                  {
                    if (ConstNodeBase const* cst = expr->GetSub(1)->AsConstNode())
                      {
                        if (op != Op::Lsl)
                          {
                            // Priority is given to BitFilter replacement
                            unsigned shift = dynamic_cast<ConstNode<shift_type> const&>(*cst).value;
                            Expr const& src = expr->GetSub(0);
                            unsigned bitsize = src->GetType().bitsize;
                            if (shift >= bitsize) { struct Bad {}; throw Bad(); }

                            expr = BitFilter::mksimple( src, bitsize, shift, bitsize - shift, bitsize, op == Op::Asr );
                          }

                        auto tp = expr->GetType();
                        Expr dmask = mask.good() ? mask : make_operation(Op::Not, make_zero(tp));
                        auto unsh = op == Op::Lsl ? Op::Lsr : Op::Lsl;
                        dmask = make_operation(unsh, dmask, cst);
                        if (auto cst = expr.Simplify(UniBitSimplify(dmask.ConstSimplify(), 1)))
                          return cst;
                        continue;
                      }
                    else if (mask.good())
                      {
                        if (op == Op::Lsl)
                          {
                            Expr dmask = make_operation(Op::BSR, mask);
                            unsigned shift = dmask.ConstSimplify()->GetBits(0);
                            dmask = make_operation(Op::Not, make_operation(Op::Inc, make_zero(expr->GetType())));
                            dmask = make_operation(Op::Not, make_operation(Op::Lsl, dmask, make_const<shift_type>(shift)));
                            if (auto cst = expr.Simplify(UniBitSimplify(dmask.ConstSimplify(), 1)))
                              return cst;
                          }
                        else
                          {
                            Expr dmask = make_operation(Op::Or, mask, make_operation(Op::Neg, mask));
                            if (auto cst = expr.Simplify(UniBitSimplify(dmask.ConstSimplify(), 1)))
                              return cst;
                          }
                        continue;
                      }
                  }
                break;

              case Op::Cast:
                if (subcount == 1)
                  {
                    CastNodeBase const& cnb = dynamic_cast<CastNodeBase const&>( *expr.node );
                    auto src = cnb.GetSrcType(), dst = cnb.GetType();
                    if (src.encoding != src.UNSIGNED and src.encoding != src.SIGNED and src.encoding != src.BOOL)
                      {
                        // Complex source
                        break;
                      }

                    if (dst.encoding == dst.BOOL)
                      {
                        if (src.encoding == src.BOOL)
                          expr = cnb.src;
                        else
                          expr = make_operation(Op::Tnzero, cnb.src);
                        continue;
                      }
                    if (dst.encoding != dst.UNSIGNED and dst.encoding != dst.SIGNED)
                      break; // Complex casts

                    unsigned src_bit_size = src.bitsize, dst_bit_size = dst.bitsize;

                    if (src_bit_size == dst_bit_size)
                      expr = cnb.src;
                    else
                      {
                        unsigned select = std::min(src_bit_size, dst_bit_size);
                        bool sextend = dst_bit_size > src_bit_size and src.encoding == src.SIGNED;
                        expr = BitFilter::mksimple( cnb.src, src_bit_size, 0, select, dst_bit_size, sextend );
                      }
                    continue;
                  }
                break;
              }
          }
        else if (ASExprNode const* node = dynamic_cast<ASExprNode const*>( expr.node ))
          {
            if (auto cst = node->Simplify(mask, expr))
              return cst;
            continue;
          }
        else if (auto const* node = expr->AsConstNode())
          {
            auto tp = node->GetType();
            if (tp.encoding == ValueType::SIGNED)
              {
                expr = make_operation(Op::ReinterpretAs, make_zero(tp), expr);
                return expr.ConstSimplify();
              }
            return node;
          }

        if (auto cst = expr.Simplify(BitSimplify()))
          return cst;
      }
    return 0;
  };

  ConstNodeBase const*
  BitInsertNode::Simplify( Expr const& mask, Expr& expr ) const
  {
    // Forward BitSimplify operation with optimized masks
    auto tp = GetType();
    Expr xmask[2];
    xmask[1] = make_operation(Op::Not, BitSimplify::make_zero(tp));
    xmask[1] = make_operation(Op::Not, make_operation(Op::Lsl, xmask[1], make_const<shift_type>(size)));
    Expr fgmask = make_operation(Op::Lsl, xmask[1].ConstSimplify(), make_const<shift_type>(pos));
    bool notinfg = mask.good() ? make_operation(Op::Tzero, make_operation(Op::And, fgmask, mask)).Eval<bool>() : true;
    xmask[0] = make_operation(Op::Not, fgmask);
    bool notinbg = mask.good() ? make_operation(Op::Tzero, make_operation(Op::And, xmask[0], mask)).Eval<bool>() : true;
    if (notinfg)
      {
        // Significant bits only found in background
        expr = dst;
        return 0;
      }
    else if (notinbg)
      {
        // Significant bits only found in foreground
        if (pos)
          expr = make_operation(Op::Lsl,src,make_const<shift_type>(pos));
        else
          expr = src;
        return 0;
      }
            
    struct BinBitSimplify : public BitSimplify
    {
      BinBitSimplify(Expr const* _masks) : masks(_masks) {}
      ConstNodeBase const* Simplify(unsigned idx, Expr& expr) const override { return Process(idx < 2 ? masks[idx] : Expr(), expr); }
      Expr const* masks;
    };
    return expr.Simplify(BinBitSimplify(&xmask[0]));
  }

  ConstNodeBase const*
  BitFilter::Simplify( Expr const& mask, Expr& expr ) const
  {
    // First check for local simplification, trivial and pipe hole
    if (rshift == 0 and source == select and select == extend)
      { expr = input; return 0; }

    auto tp = GetType();

    if (OpNodeBase const* onb = input->AsOpNode())
      {
        auto op = onb->op.code;
        if (op == Op::Lsl)
          {
            try
              {
                unsigned lshift = onb->GetSub(1).Eval<shift_type>();

                if (lshift <= rshift)
                  { expr = new BitFilter(onb->GetSub(0), this->source, this->rshift - lshift, this->select, this->extend, this->sxtend); return 0; }

                if (lshift >= unsigned(rshift + select))
                  { expr = BitSimplify::make_zero(tp); return 0; }
              }
            catch (Evaluator::Failure) {}
          }
        else if ((op == Op::And or op == Op::Xor or op == Op::Or) and select < source and not (select & (select-1)) and ((0x79 >> __builtin_ctz(select)) & 1))
          {
            /* Try to make bitwise manipulations work on bitvector as tiny as possible */
            unsigned subcount = onb->SubCount();
            Expr args[subcount];
            if (2 <= subcount and subcount <= 3)
              {
                for (unsigned idx = 0; idx < subcount; ++idx)
                  args[idx] = new BitFilter(onb->GetSub(idx), source, rshift, select, select, false);
                if      (subcount == 2)
                  expr = make_operation(op, args[0], args[1] );
                else if (subcount == 3)
                  expr = make_operation(op, args[0], args[1], args[2] );
                if (extend > select)
                  expr = new BitFilter(expr, select, 0, select, extend, sxtend);
                return 0;
              }
          }
      }
    else if (BitFilter const* bf = dynamic_cast<BitFilter const*>( input.node ))
      {
        if (source != bf->extend) /* Sanity check */
          { struct Bad {}; throw Bad (); }

        if (rshift >= bf->select)
          {
            // TODO: this should be optimized
          }
        else
          {
            unsigned new_rshift = bf->rshift + rshift;

            if (bf->select >= (rshift + select))
              { expr = new BitFilter( bf->input, bf->source, new_rshift, select, extend, sxtend ); return 0; }

            // rshift < bf->select < (rshift + select)
            if (sxtend or not bf->sxtend)
              { expr = new BitFilter( bf->input, bf->source, new_rshift, bf->select - rshift, extend, bf->sxtend ); return 0; }
          }
      }

    // Finally, perform classic BitSimplify operation with optimized masks

    Expr dmask = make_operation(Op::Not, BitSimplify::make_zero(tp));
    if (select < extend)
      dmask = make_operation(Op::Not, make_operation(Op::Lsl, dmask, make_const<shift_type>(select)));
    if (mask.good())
      dmask = make_operation(Op::And, dmask, mask);
    auto childtype = input->GetType();
    dmask = make_operation(Op::ReinterpretAs, BitSimplify::make_zero(childtype)->AsConstNode(), dmask);
    if (rshift)
      dmask = make_operation(Op::Lsl, dmask, make_const<shift_type>(rshift));

    return expr.Simplify(UniBitSimplify(dmask.ConstSimplify(), 1));
  }

} /* end of namespace binsec */
} /* end of namespace symbolic */
} /* end of namespace util */
} /* end of namespace unisim */

