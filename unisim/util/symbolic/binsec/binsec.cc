/*
 *  Copyright (c) 2017,
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
#include <unisim/util/symbolic/vector/vector.hh>
#include <ostream>
#include <sstream>
#include <iomanip>

namespace unisim {
namespace util {
namespace symbolic {
namespace binsec {

  struct Instruction
  {
    friend class ActionNode;

    virtual ~Instruction() {}
    Instruction() : index(-1) {}

    void connect(Instruction* next) { *nextp() = next; }
    virtual Instruction** nextp() { struct No {}; throw No (); return 0; }

    typedef std::map<unsigned, Instruction*> Sequence;
    void compute_indices(Sequence& head, Sequence& tail)
    {
      if (index >= 0 or not allocate(head, tail)) return;
      for (int idx = 0; Instruction* next = get_next(idx); ++idx)
        next->compute_indices(head, tail);
    }
    // Allocate index for current instruction and tell wether following
    // instructions need allocation
    virtual bool allocate(Sequence&, Sequence&) = 0;

    struct Printer
    {
      std::ostream& label(int idx) const { sink << "(" << addr << "," << idx << ") "; return sink; }
      std::ostream& sink; std::string addr;
    };
    virtual void print(Printer const&) const = 0;
    void release(std::set<Instruction*>& pool)
    {
      if (not pool.insert(this).second)
        return;
      for (int idx = 0; Instruction* next = get_next(idx); ++idx)
        next->release(pool);
    }
    virtual Instruction* get_next(int idx) const { return 0; }

    int index;
  };

  void Point::connect( Point const& np ) { insn->connect(np.insn); }

  struct SeqInstruction : Instruction
  {
    SeqInstruction() : next() {}

    virtual Instruction** nextp() { return &next; }
    virtual Instruction* get_next(int idx) const override { return idx == 0 ? next : 0; }

    Instruction* next;
  };

  struct Nop : SeqInstruction
  {
    virtual bool allocate(Sequence& head, Sequence& tail) override { index = head.size(); return true; }
    virtual void print(Printer const& printer) const override {};
  };

  struct Statement : SeqInstruction
  {
    Statement( std::string&& txt ) : text( std::move(txt) ) {}

    virtual bool allocate(Sequence& head, Sequence& tail) override { index = head.size(); head[index] = this; return true; }
    virtual void print(Printer const& printer) const override { printer.label(index) << text << "; goto " << next->index << '\n'; };

    std::string text;
  };

  struct ConditionalBranch : Instruction
  {
    ConditionalBranch( std::string&& cnd, Point const& target ) : nexts(), cond( std::move(cnd) ), dir(true) { connect(target.get()); dir = false; }
    ConditionalBranch( std::string&& cnd ) : nexts(), cond( std::move(cnd) ), dir(false) {}

    void follow(bool _dir) { dir = _dir; }
    virtual bool allocate(Sequence& head, Sequence& tail) override { index = head.size(); head[index] = this; return true; }
    virtual void print(Printer const& printer) const override
    {
      printer.label(index) << "if (" << cond << ") goto " << nexts[true]->index << " else goto " << nexts[false]->index << "\n";
    };
    virtual Instruction** nextp() override { return &nexts[dir]; }
    virtual Instruction* get_next(int idx) const override { return idx & -2 ? 0 : nexts[idx]; }

    Instruction* nexts[2];
    std::string cond;
    bool dir;
  };

  bool
  ActionNode::merge( int& cmp, Expr const& l, Expr const& r )
  {
    Assignment const* as[] = { dynamic_cast<Assignment const*>( l.node ), dynamic_cast<Assignment const*>( r.node ) };

    bool side = cmp>0; // side of the lowest ranked expression

    // For now, anything else than register writes are ignored (no branch, no stores)
    if (not dynamic_cast<RegWriteBase const*>(as[side]))
      return false;

    Expr source = as[side]->SourceRead();

    if (as[0] and as[1] and (&typeid(*as[0]) == &typeid(*as[1])) and (source == as[not side]->SourceRead()))
      {
        // If-conversion of two-way conditional assignments
        Assignment* assignment = dynamic_cast<Assignment*>(as[0]->Mutate());
        assignment->value = make_operation( Op::CMov, as[true]->value, as[false]->value, cond );
        add_sink(Expr(assignment));
        cmp = 0;
        return true;
      }

    // Force if-conversion of one-way contitional assignments
    Assignment* assignment = dynamic_cast<Assignment*>(as[side]->Mutate());
    Expr values[2] = {source, assignment->value};
    assignment->value = make_operation( Op::CMov, values[side], values[not side], cond );
    add_sink(Expr(assignment));
    return true;
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
            updates.insert(always->updates.begin(), always->updates.end());
            delete always;
          }
        else
          break;
      }

    {
      std::set<Expr> nsinks;
      for (auto const& sink : get_sinks())
        {
          Expr x(sink);
          BitSimplify::Do(x);
          nsinks.insert( x );
        }
      std::swap(nsinks, updates);
    }

    if (not cond.good())
      return;

    for (unsigned choice = 0; choice < 2; ++choice)
      if (ActionNode* next = nexts[choice])
        next->simplify();

    factorize();

    bool leaf = true;
    for (unsigned choice = 0; choice < 2; ++choice)
      if (ActionNode* next = nexts[choice])
        {
          if (next->cond.good() or next->updates.size()) leaf = false;
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
        // First level of expression is not functionnal (architectural side effect)
        for (auto const& sink : node->get_sinks())
          this->Flood( sink );
        for (unsigned choice = 0; choice < 2; ++choice)
          if (ActionNode* next = node->nexts[choice])
            { Sec sub(next,this); }
      }
      std::map<Expr,unsigned>& stats; Sec* up;
    } sec(this,0);
  }

  std::string const&
  mktemp(Variables& vars, Expr const& expr, unsigned size)
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

  void
  ActionNode::generate(std::ostream& sink, unsigned addrsize, uint64_t address) const
  {
    // Introducing a Finish instruction to capture branching code epilogue.
    struct Finish : Statement
    {
      Finish( std::string&& txt ) : Statement( std::move(txt) ) {}
      // The epilogue cannot share code with other epilogues as it
      // ends with its own goto => unshare remaining statements.
      virtual bool allocate(Sequence& head, Sequence& tail) override
      {
        int idx = tail.rbegin() != tail.rend() ? tail.rbegin()->first : 0;
        index = idx++;
        for (Statement const* insn = this; (insn = dynamic_cast<Statement*>(insn->next));)
          idx += 1;
        tail[idx] = this;
        return false;
      }
      virtual void print(Printer const& printer) const override
      {
        int idx = index;
        for (Statement const* insn = this; (insn = dynamic_cast<Statement*>(insn->next));)
          { printer.label(idx) << insn->text << "; goto " << idx+1 << '\n'; idx += 1; }
        printer.label(idx) << text << '\n';
      }
    };

    struct Scope
    {
      Scope() {}
      Scope( Scope* up ) : vars(up->vars) {}

      int GenTempCode(Expr const& expr, Point& head)
      {
        std::string tmp_src;
        int retsize;
        {
          std::ostringstream buffer;
          retsize = ASExprNode::GenerateCode( expr, buffer, vars, head );
          if (vars.count(expr)) return retsize;
          tmp_src = buffer.str();
        }
        std::ostringstream buffer;
        buffer << mktemp( vars, expr, retsize ) << " := " << tmp_src;
        head.append( new Statement( buffer.str() ) );
        return retsize;
      }

      void GenCode( ActionNode const* action_tree, Point head, Point tail )
      {
        Branch const* nia = 0;

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

          for (auto const& sestat : action_tree->get_sestats())
            {
              // At least 2 references to be considered a common subexpression
              if (sestat.second >= 2 and not this->vars.count(sestat.first))
                cse.Process(sestat.first);
            }

          // Assigning temporaries with each subexpression
          for (std::multimap<unsigned,Expr>::const_iterator itr = cse.begin(), end = cse.end(); itr != end; ++itr)
            {
              GenTempCode(itr->second, head);
            }
        }

        for (auto const& sink : action_tree->get_sinks())
          {

	    if (AssertFalse const* abort = dynamic_cast<AssertFalse const*>( sink.node ))
	      {
		std::ostringstream buffer;
                abort->GenCode(buffer, vars, tail);
                tail.prepend( new Statement( buffer.str() ) );
		continue;
	      }

            Assignment const& assignment = dynamic_cast<Assignment const&>( *sink.node );

            for (unsigned idx = 0, end = assignment.SubCount(); idx < end; ++idx)
              {
                Expr const& value = assignment.GetSub(idx);
                if (not value->AsConstNode() and not this->vars.count(value))
                  GenTempCode(value, head);
              }

            if (Branch const* branch = dynamic_cast<Branch const*>( &assignment ))
              nia = branch;
            else
              {
                std::ostringstream buffer;
                assignment.GenerateCode(buffer, vars);
                tail.prepend( new Statement( buffer.str() ) );
              }
          }

        if (nia)
          {
            std::ostringstream buffer;
            Expr const& target = nia->value;
            buffer << "goto (" << GetCode(target, this->vars, head) << (target->AsConstNode() ? ",0" : "") << ")";
            nia->annotate( buffer );
            tail.prepend( new Finish(buffer.str()) );
          }

        if (action_tree->cond.good())
          {
            std::ostringstream buffer;
            buffer << GetCode(action_tree->cond, this->vars, head);
            auto cbr = new ConditionalBranch(buffer.str());
            head.append( cbr );

            for (unsigned side = 0; side < 2; ++side)
              {
                cbr->follow(side);
                if (action_tree->nexts[side])
                  {
                    Scope nxt(this);
                    nxt.GenCode( action_tree->nexts[side], head, tail);
                  }
                else
                  head.connect( tail );
              }
          }
        else
          {
            if (action_tree->get_sinks().size() == 0)
              throw std::logic_error( "empty leaf" );
            head.connect( tail );
          }
      }

      Variables vars;
    };

    Nop entrypoint;
    Scope root;
    root.GenCode( this, Point(&entrypoint), Point(0) );

    Instruction::Sequence prologue, epilogue;
    entrypoint.compute_indices(prologue, epilogue);

    int tail = prologue.size();
    for (auto& line : epilogue)
      line.second->index += tail;

    std::ostringstream buf;
    buf << dbx(addrsize, address);
    Instruction::Printer printer{sink, buf.str()};

    for (auto line : prologue)
      line.second->print(printer);
    for (auto line : epilogue)
      line.second->print(printer);

    std::set<Instruction*> release_pool;
    entrypoint.next->release(release_pool);
    for (auto insn : release_pool)
      delete insn;
  }

  std::ostream&
  operator << ( std::ostream& sink, dbx const& _ )
  {
    std::ios_base::fmtflags f( sink.flags() );

    sink << "0x" << std::hex << std::setw(_.bytes*2) << std::setfill('0') << _.value;

    sink.flags( f );

    return sink;
  }

  int
  ASExprNode::GenConstCode(ConstNodeBase const* node, std::ostream& sink)
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
    return 0;
  }

  int
  ASExprNode::GenerateCode( Expr const& expr, std::ostream& sink, Variables& vars, Point& head )
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
        return GenConstCode(node, sink);
      }
    else if (OpNodeBase const* node = expr->AsOpNode())
      {
        switch (node->SubCount())
          {
          case 2: {
            auto infix = [&] (char const* op) -> int {
              int lhs_size = GenerateCode( node->GetSub(0), sink << '(', vars, head );
              sink << ' ' << op << ' ' << GetCode( node->GetSub(1), vars, head ) << ')';
              return lhs_size;
            };

            auto prefix = [&] (char const* op) -> int {
              int lhs_size = GenerateCode( node->GetSub(0), sink << op << '(', vars, head );
              sink << ", " << GetCode( node->GetSub(1), vars, head ) << ')';
              return lhs_size;
            };

            auto test = [&] (char const* op) -> int {
              sink << '(' << GetCode( node->GetSub(0), vars, head ) << ' ' << op << ' ' << GetCode( node->GetSub(1), vars, head ) << ')';
              return 1;
            };

            auto shift = [&] (char const* op) -> int {
              int lhs_size = GenerateCode( node->GetSub(0), sink << '(', vars, head );
              Expr rhs = node->GetSub(1);
              switch (lhs_size)
                {
                case  8: rhs =  U8(USH(rhs)).expr; BitSimplify::Do(rhs); break;
                case 16: rhs = U16(USH(rhs)).expr; BitSimplify::Do(rhs); break;
                case 32: rhs = U32(USH(rhs)).expr; BitSimplify::Do(rhs); break;
                case 64: rhs = U64(USH(rhs)).expr; BitSimplify::Do(rhs); break;
                }
              sink << ' ' << op << ' ' << GetCode( rhs, vars, head ) << ')';
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
		    sink << "if " << GetCode(node->GetSub(0), vars, head)
		         << '{' << i << "} then "
		         << i << '<' << bitsize << "> else (";
		  }
		  sink << bitsize - 1 << '<' << bitsize << '>';
		  for (unsigned i = 0; i < bitsize; i += 1) {
		    sink << ')';
		  }

                  // Point exit(new Nop());

                  // std::ostringstream buffer;
                  // buffer << "bsf_in<" << bitsize << "> := " << GetCode(node->GetSub(0), vars, head, bitsize);
                  // head.append( new Statement( buffer.str() ) );

                  // buffer = std::ostringstream();
                  // buffer << "bsf_out<" << bitsize << "> := 0<" << bitsize << ">";
                  // head.append( new Statement( buffer.str() ) );

                  // buffer = std::ostringstream();
                  // buffer << "bsf_in<" << bitsize << "> = 0<" << bitsize << ">";
                  // head.append( new ConditionalBranch( buffer.str(), exit ) );

                  // buffer = std::ostringstream();
                  // buffer << "(bsf_in<" << bitsize << "> rshiftu bsf_out<" << bitsize << ">){0,0}";
                  // head.append( new ConditionalBranch( buffer.str(), exit ) );
                  // auto loop = head;

                  // buffer = std::ostringstream();
                  // buffer << "bsf_out<" << bitsize << "> := bsf_out<" << bitsize << "> + 1<" << bitsize << ">";
                  // head.append( new Statement( buffer.str() ) );

                  // head.connect( loop );
                  // head = exit;

                  // sink << "bsf_out<" << bitsize << ">";

                  return bitsize;
                }

              case Op::BSR:
                {
                  unsigned bitsize = node->GetType().bitsize;
		  sink << "(";
		  for (unsigned i = bitsize - 1; i > 1; i -= 1) {
		    sink << "if " << GetCode(node->GetSub(0), vars, head)
		         << '{' << i << "} then "
		         << i << '<' << bitsize << "> else (";
		  }
		  sink << "extu ("
		       << GetCode(node->GetSub(0), vars, head)
		       << "{1}) " << bitsize;
		  for (unsigned i = bitsize; i > 1; i -= 1) {
		    sink << ')';
		  }

                  // Point exit(new Nop());

                  // std::ostringstream buffer;
                  // buffer << "bsr_in<" << bitsize << "> := " << GetCode(node->GetSub(0), vars, head, bitsize);
                  // head.append( new Statement( buffer.str() ) );

                  // buffer = std::ostringstream();
                  // buffer << "bsr_out<" << bitsize << "> := " << bitsize << "<" << bitsize << ">";
                  // head.append( new Statement( buffer.str() ) );

                  // buffer = std::ostringstream();
                  // buffer << "bsr_in<" << bitsize << "> = 0<" << bitsize << ">";
                  // head.append( new ConditionalBranch( buffer.str(), exit ) );

                  // buffer = std::ostringstream();
                  // buffer << "bsr_out<" << bitsize << "> := bsr_out<" << bitsize << "> - 1<" << bitsize << ">";
                  // head.append( new Statement( buffer.str() ) );
                  // auto loop = head;

                  // buffer = std::ostringstream();
                  // buffer << "(bsr_in<" << bitsize << "> rshiftu bsr_out<" << bitsize << ">){0,0}";
                  // head.append( new ConditionalBranch( buffer.str(), exit ) );

                  // head.connect( loop );
                  // head = exit;

                  // sink << "bsr_out<" << bitsize << ">";

                  return bitsize;
                }
              case Op::Tnzero:
                {
                  Expr const& src = node->GetSub(0);
                  sink << "(" << GetCode(src, vars, head) << " <> " << dbx(src->GetType().bitsize/8,0) << ")";
                  return 1;
                }
              case Op::Cast:
                {
                  CastNodeBase const& cnb = dynamic_cast<CastNodeBase const&>( *expr.node );
                  auto src = cnb.GetSrcType(), dst = cnb.GetType();

                  /* TODO: At this point, boolean casts should not remain */
                  if (dst.encoding == dst.BOOL)
                    {
                      sink << "(" << GetCode(cnb.src, vars, head) << " <> " << dbx(src.bitsize/8,0) << ")";
                    }
                  else
                    {
                      throw std::logic_error("Unexpected cast");
                      /* TODO: What to do with FP casts ? */
                      dst.Repr(sink);
                      sink << "( " << GetCode(cnb.src, vars, head) << " )";
                    }

                  return dst.bitsize;
                }
              }

            sink << '(' << operation << ' ';
            int retsz = GenerateCode( node->GetSub(0), sink, vars, head );
            sink << ')';
            return retsz;
          }
          case 3: {
            switch (node->op.code)
              {
              default: break;

              case Op::CMov:

                sink << "(if " << GetCode(node->GetSub(2), vars, head) << " then ";
                int retsz = GenerateCode( node->GetSub(0), sink, vars, head );
                sink << " else " << GetCode(node->GetSub(1), vars, head) << ")";

                return retsz;

              }
          }
          default:
            break;
          }
      }
    else if (auto vt = dynamic_cast<vector::VTransBase const*>( expr.node ))
      {
        unsigned srcsize = 8*vt->srcsize, dstsize = vt->GetType().bitsize, srcpos = 8*vt->srcpos;

        if (dstsize < srcsize)
          sink << "(" << GetCode(vt->src, vars, head) << " {" << std::dec << srcpos << ", " << (srcpos+dstsize-1) << "})";
        else
          sink << GetCode(vt->src, vars, head);
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
            retsize += GenerateCode( mix->l, sink, vars, head );
            sink << " :: ";
            mix = dynamic_cast<vector::VMix const*>( mix->r.node );
          }
        while (mix);
        retsize += GenerateCode( prev->r, sink, vars, head );
        sink << ")";
        return retsize;
      }
    else if (ASExprNode const* node = dynamic_cast<ASExprNode const*>( expr.node ))
      {
        return node->GenCode( sink, vars, head );
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
  BitFilter::GenCode( std::ostream& sink, Variables& vars, Point& head ) const
  {
    bool selection = source > select;
    if (extend == source and selection)
      {
        bool tail = true;
        if      (not rshift and not sxtend and select < 64)
          sink << "(" << GetCode(input, vars, head, source) << " and " << dbx(source/8, (1ull << select)-1) << ")";
        else if ((rshift + select) == extend)
          sink << "(" << GetCode(input, vars, head, source) << " rshift" << (sxtend?"s ":"u ") << dbx(source/8, rshift) << ")";
        else
          tail = false;

        if (tail)
          return extend;
      }

    bool extension = extend > select;
    if (extension)
      sink << '(' << (sxtend ? "exts " : "extu ");
    if (selection)
      sink << '(';

    int chksize = ASExprNode::GenerateCode( input, sink, vars, head );
    if (chksize != source) { struct TypeSizeMisMatch {}; throw TypeSizeMisMatch(); }

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
  RegReadBase::GenCode( std::ostream& sink, Variables& vars, Point& head ) const
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
  Assignment::GenInputCode( Expr const& input, Variables& vars, std::ostream& sink )
  {
    Expr constant = input;
    if (ConstNodeBase const* node = constant.ConstSimplify())
      return ASExprNode::GenConstCode(node, sink);
    auto const& var = vars.at(input);
    sink << var.first;
    return var.second;
  }

  void
  RegWriteBase::Repr( std::ostream& sink ) const
  {
    sink << "RegWrite( ";
    GetRegName( sink );
    sink << ", " << reg_size << ", " << slice_base << ", " << slice_size << ", ";
    value->Repr( sink );
    sink << " )";
  }

  Expr
  RegWriteBase::source_read( RegReadBase* reg_read ) const
  {
    if (slice_size != reg_size)
      return BitFilter::mksimple(reg_read, reg_size, slice_base, slice_size, slice_size, false);
    return reg_read;
  }

  void
  RegWriteBase::GenerateCode( std::ostream& sink, Variables& vars ) const
  {
    /* Name of the assigned register */
    GetRegName( sink );
    sink << '<' << reg_size << '>';
    if (slice_size != reg_size)
      sink << '{' << slice_base << ',' << (slice_base + slice_size - 1) << '}';

    sink << " := ";

    int value_size = GenInputCode( value, vars, sink );

    if (slice_size != value_size)
      throw std::logic_error( "error: size disagreement in register assignment." );
  }

  void
  Branch::Repr( std::ostream& sink ) const
  {
    sink << "Branch(";
    value->Repr( sink );
    sink << ")";
  }

  void
  Branch::GenerateCode( std::ostream& sink, Variables& vars ) const
  {
    struct ShouldNotBeHere {};
    throw ShouldNotBeHere ();
  }

  void
  Store::Repr( std::ostream& sink ) const
  {
    MemAccess::Repr( sink );
    sink << " := " << value;
  }

  void
  Store::GenerateCode( std::ostream& sink, Variables& vars ) const
  {
    /* TODO: exploit alignment info */
    sink << "@[";
    GenInputCode( addr, vars, sink );
    sink << ',' << (bigendian?"->":"<-") << ',' << bytecount() << "] := ";

    int value_size = GenInputCode( value, vars, sink );

    if (value_size != 8*bytecount())
      throw std::logic_error( "error: size disagreement in memory store." );
  }

  void
  MemAccess::Repr( std::ostream& sink ) const
  {
    sink << "[" << addr << ',' << bytecount() << ",^" << alignment << ',' << (bigendian ? "be" : "le") << "]";
  }

  int
  Load::GenCode( std::ostream& sink, Variables& vars, Point& head ) const
  {
    /* TODO: exploit alignment info */
    sink << "@[" << GetCode(addr, vars, head) << ',' << (bigendian?"->":"<-") << ',' << bytecount() << "]";
    return 8*bytecount();
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
  UndefinedValueBase::GenCode(std::ostream& sink, Variables& vars, Point& head) const
  {
    int retsize = GetType().bitsize;
    std::ostringstream buffer;
    std::string const& tmp = mktemp( vars, Expr(this), retsize );
    buffer << tmp << " := \\undef";
    head.append( new Statement( buffer.str() ) );
    sink << tmp;
    return retsize;
  }

  int
  BitInsertNode::GenCode(std::ostream& sink, Variables&, Point&) const
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
    struct FiltBitSimplify : public BitSimplify
    {
      FiltBitSimplify(Expr const& _mask, unsigned _filter) : mask(_mask), filter(_filter) {}
      ConstNodeBase const* Simplify(unsigned idx, Expr& expr) const override { return Process((filter >> idx & 1) ? mask : Expr(), expr); }
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
                if (auto cst = expr.Simplify(FiltBitSimplify(mask, (1<<subcount)-1)))
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
                            continue;
                          }

                        auto tp = expr->GetType();
                        Expr dmask = mask.good() ? mask : make_operation(Op::Not, make_zero(tp));
                        auto unsh = op == Op::Lsl ? Op::Lsr : Op::Lsl;
                        dmask = make_operation(unsh, dmask, cst);
                        if (auto cst = expr.Simplify(FiltBitSimplify(dmask.ConstSimplify(), 1)))
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
                            if (auto cst = expr.Simplify(FiltBitSimplify(dmask.ConstSimplify(), 1)))
                              return cst;
                          }
                        else
                          {
                            Expr dmask = make_operation(Op::Or, mask, make_operation(Op::Neg, mask));
                            if (auto cst = expr.Simplify(FiltBitSimplify(dmask.ConstSimplify(), 1)))
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

    return expr.Simplify(FiltBitSimplify(dmask.ConstSimplify(), 1));
  }

} /* end of namespace binsec */
} /* end of namespace symbolic */
} /* end of namespace util */
} /* end of namespace unisim */

