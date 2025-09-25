#include "unisim/component/cxx/processor/sparc/dba/sv8/sparc.hh"

#include <unisim/component/cxx/processor/sparc/isa/sv8/disasm.hh>
#include <unisim/component/cxx/processor/sparc/asi.hh>
#include <unisim/component/cxx/processor/sparc/trap.hh>
#include <iostream>

using namespace unisim::component::cxx::processor::sparc::isa::sv8;

namespace unisim { namespace component { namespace cxx { namespace processor { namespace sparc { namespace dba { namespace sv8 {
template <	typename	ARCH>
Operation<	ARCH>::Operation(CodeType _code, uint32_t _addr, const char *_name)
: name(_name)
, addr(_addr)
, encoding(_code)
{
}

template <	typename	ARCH>
Operation<	ARCH>::~Operation()
{
}

template <	typename	ARCH>
void Operation<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "; Unknown instruction";
}}
template <	typename	ARCH>
void Operation<	ARCH>::execute( ARCH& cpu) const
{
	{
		cpu.UndefinedInstruction(*this);
}}
template <	typename	ARCH>
struct OpBc : public Operation<	ARCH>
{
	OpBc(CodeType code, uint32_t addr);
	uint8_t annul;
	uint8_t cond;
	int32_t disp22;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpFbc : public Operation<	ARCH>
{
	OpFbc(CodeType code, uint32_t addr);
	uint8_t annul;
	uint8_t cond;
	int32_t disp22;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpTc : public Operation<	ARCH>
{
	OpTc(CodeType code, uint32_t addr);
	uint8_t cond;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpTci : public Operation<	ARCH>
{
	OpTci(CodeType code, uint32_t addr);
	uint8_t cond;
	uint8_t rs1;
	uint8_t imm7;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpRdasr : public Operation<	ARCH>
{
	OpRdasr(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpWrasr : public Operation<	ARCH>
{
	OpWrasr(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpWrasri : public Operation<	ARCH>
{
	OpWrasri(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpLdsb : public Operation<	ARCH>
{
	OpLdsb(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t a;
	uint8_t rs1;
	uint8_t asi;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpLdsbi : public Operation<	ARCH>
{
	OpLdsbi(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpLdsh : public Operation<	ARCH>
{
	OpLdsh(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t a;
	uint8_t rs1;
	uint8_t asi;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpLdshi : public Operation<	ARCH>
{
	OpLdshi(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpLdub : public Operation<	ARCH>
{
	OpLdub(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t a;
	uint8_t rs1;
	uint8_t asi;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpLdubi : public Operation<	ARCH>
{
	OpLdubi(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpLduh : public Operation<	ARCH>
{
	OpLduh(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t a;
	uint8_t rs1;
	uint8_t asi;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpLduhi : public Operation<	ARCH>
{
	OpLduhi(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpLd : public Operation<	ARCH>
{
	OpLd(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t a;
	uint8_t rs1;
	uint8_t asi;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpLdi : public Operation<	ARCH>
{
	OpLdi(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpLdd : public Operation<	ARCH>
{
	OpLdd(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t a;
	uint8_t rs1;
	uint8_t asi;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpLddi : public Operation<	ARCH>
{
	OpLddi(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpLdf : public Operation<	ARCH>
{
	OpLdf(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpLdfi : public Operation<	ARCH>
{
	OpLdfi(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpLddf : public Operation<	ARCH>
{
	OpLddf(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpLddfi : public Operation<	ARCH>
{
	OpLddfi(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpLdfsr : public Operation<	ARCH>
{
	OpLdfsr(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpLdfsri : public Operation<	ARCH>
{
	OpLdfsri(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpLdc : public Operation<	ARCH>
{
	OpLdc(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpLdci : public Operation<	ARCH>
{
	OpLdci(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpLddc : public Operation<	ARCH>
{
	OpLddc(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpLddci : public Operation<	ARCH>
{
	OpLddci(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpLdcsr : public Operation<	ARCH>
{
	OpLdcsr(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpLdcsri : public Operation<	ARCH>
{
	OpLdcsri(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpStb : public Operation<	ARCH>
{
	OpStb(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t a;
	uint8_t rs1;
	uint8_t asi;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpStbi : public Operation<	ARCH>
{
	OpStbi(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpSth : public Operation<	ARCH>
{
	OpSth(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t a;
	uint8_t rs1;
	uint8_t asi;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpSthi : public Operation<	ARCH>
{
	OpSthi(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpSt : public Operation<	ARCH>
{
	OpSt(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t a;
	uint8_t rs1;
	uint8_t asi;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpSti : public Operation<	ARCH>
{
	OpSti(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpStd : public Operation<	ARCH>
{
	OpStd(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t a;
	uint8_t rs1;
	uint8_t asi;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpStdi : public Operation<	ARCH>
{
	OpStdi(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpStf : public Operation<	ARCH>
{
	OpStf(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpStfi : public Operation<	ARCH>
{
	OpStfi(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpStdf : public Operation<	ARCH>
{
	OpStdf(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpStdfi : public Operation<	ARCH>
{
	OpStdfi(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpStfsr : public Operation<	ARCH>
{
	OpStfsr(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpStfsri : public Operation<	ARCH>
{
	OpStfsri(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpStdfq : public Operation<	ARCH>
{
	OpStdfq(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpStdfqi : public Operation<	ARCH>
{
	OpStdfqi(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpStc : public Operation<	ARCH>
{
	OpStc(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpStci : public Operation<	ARCH>
{
	OpStci(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpStdc : public Operation<	ARCH>
{
	OpStdc(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpStdci : public Operation<	ARCH>
{
	OpStdci(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpStcsr : public Operation<	ARCH>
{
	OpStcsr(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpStcsri : public Operation<	ARCH>
{
	OpStcsri(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpStdcq : public Operation<	ARCH>
{
	OpStdcq(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpStdcqi : public Operation<	ARCH>
{
	OpStdcqi(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpLdstub : public Operation<	ARCH>
{
	OpLdstub(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t a;
	uint8_t rs1;
	uint8_t asi;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpLdstubi : public Operation<	ARCH>
{
	OpLdstubi(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpSwap : public Operation<	ARCH>
{
	OpSwap(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t a;
	uint8_t rs1;
	uint8_t asi;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpSwapi : public Operation<	ARCH>
{
	OpSwapi(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpFlush : public Operation<	ARCH>
{
	OpFlush(CodeType code, uint32_t addr);
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpFlushi : public Operation<	ARCH>
{
	OpFlushi(CodeType code, uint32_t addr);
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpSethi : public Operation<	ARCH>
{
	OpSethi(CodeType code, uint32_t addr);
	uint8_t rd;
	uint32_t imm22;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpAnd_ : public Operation<	ARCH>
{
	OpAnd_(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpAndi : public Operation<	ARCH>
{
	OpAndi(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpAndcc : public Operation<	ARCH>
{
	OpAndcc(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpAndcci : public Operation<	ARCH>
{
	OpAndcci(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpAndn : public Operation<	ARCH>
{
	OpAndn(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpAndni : public Operation<	ARCH>
{
	OpAndni(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpAndncc : public Operation<	ARCH>
{
	OpAndncc(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpAndncci : public Operation<	ARCH>
{
	OpAndncci(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpOr_ : public Operation<	ARCH>
{
	OpOr_(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpOri : public Operation<	ARCH>
{
	OpOri(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpOrcc : public Operation<	ARCH>
{
	OpOrcc(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpOrcci : public Operation<	ARCH>
{
	OpOrcci(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpOrn : public Operation<	ARCH>
{
	OpOrn(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpOrni : public Operation<	ARCH>
{
	OpOrni(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpOrncc : public Operation<	ARCH>
{
	OpOrncc(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpOrncci : public Operation<	ARCH>
{
	OpOrncci(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpXor_ : public Operation<	ARCH>
{
	OpXor_(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpXori : public Operation<	ARCH>
{
	OpXori(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpXorcc : public Operation<	ARCH>
{
	OpXorcc(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpXorcci : public Operation<	ARCH>
{
	OpXorcci(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpXnor : public Operation<	ARCH>
{
	OpXnor(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpXnori : public Operation<	ARCH>
{
	OpXnori(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpXnorcc : public Operation<	ARCH>
{
	OpXnorcc(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpXnorcci : public Operation<	ARCH>
{
	OpXnorcci(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpSll : public Operation<	ARCH>
{
	OpSll(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpSlli : public Operation<	ARCH>
{
	OpSlli(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t shcnt;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpSrl : public Operation<	ARCH>
{
	OpSrl(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpSrli : public Operation<	ARCH>
{
	OpSrli(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t shcnt;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpSra : public Operation<	ARCH>
{
	OpSra(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpSrai : public Operation<	ARCH>
{
	OpSrai(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t shcnt;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpAdd : public Operation<	ARCH>
{
	OpAdd(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpAddi : public Operation<	ARCH>
{
	OpAddi(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpAddcc : public Operation<	ARCH>
{
	OpAddcc(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpAddcci : public Operation<	ARCH>
{
	OpAddcci(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpAddx : public Operation<	ARCH>
{
	OpAddx(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpAddxi : public Operation<	ARCH>
{
	OpAddxi(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpAddxcc : public Operation<	ARCH>
{
	OpAddxcc(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpAddxcci : public Operation<	ARCH>
{
	OpAddxcci(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpTaddcc : public Operation<	ARCH>
{
	OpTaddcc(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpTaddcci : public Operation<	ARCH>
{
	OpTaddcci(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpTaddcctv : public Operation<	ARCH>
{
	OpTaddcctv(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpTaddcctvi : public Operation<	ARCH>
{
	OpTaddcctvi(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpSub : public Operation<	ARCH>
{
	OpSub(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpSubi : public Operation<	ARCH>
{
	OpSubi(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpSubcc : public Operation<	ARCH>
{
	OpSubcc(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpSubcci : public Operation<	ARCH>
{
	OpSubcci(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpSubx : public Operation<	ARCH>
{
	OpSubx(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpSubxi : public Operation<	ARCH>
{
	OpSubxi(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpSubxcc : public Operation<	ARCH>
{
	OpSubxcc(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpSubxcci : public Operation<	ARCH>
{
	OpSubxcci(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpTsubcc : public Operation<	ARCH>
{
	OpTsubcc(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpTsubcci : public Operation<	ARCH>
{
	OpTsubcci(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpTsubcctv : public Operation<	ARCH>
{
	OpTsubcctv(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpTsubcctvi : public Operation<	ARCH>
{
	OpTsubcctvi(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpMulscc : public Operation<	ARCH>
{
	OpMulscc(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpMulscci : public Operation<	ARCH>
{
	OpMulscci(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpUmul : public Operation<	ARCH>
{
	OpUmul(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpUmuli : public Operation<	ARCH>
{
	OpUmuli(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpSmul : public Operation<	ARCH>
{
	OpSmul(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpSmuli : public Operation<	ARCH>
{
	OpSmuli(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpUmulcc : public Operation<	ARCH>
{
	OpUmulcc(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpUmulcci : public Operation<	ARCH>
{
	OpUmulcci(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpSmulcc : public Operation<	ARCH>
{
	OpSmulcc(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpSmulcci : public Operation<	ARCH>
{
	OpSmulcci(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpUdiv : public Operation<	ARCH>
{
	OpUdiv(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpUdivi : public Operation<	ARCH>
{
	OpUdivi(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpSdiv : public Operation<	ARCH>
{
	OpSdiv(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpSdivi : public Operation<	ARCH>
{
	OpSdivi(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpUdivcc : public Operation<	ARCH>
{
	OpUdivcc(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpUdivcci : public Operation<	ARCH>
{
	OpUdivcci(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpSdivcc : public Operation<	ARCH>
{
	OpSdivcc(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpSdivcci : public Operation<	ARCH>
{
	OpSdivcci(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpSave : public Operation<	ARCH>
{
	OpSave(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpSavei : public Operation<	ARCH>
{
	OpSavei(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpRestore : public Operation<	ARCH>
{
	OpRestore(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpRestorei : public Operation<	ARCH>
{
	OpRestorei(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpBa : public Operation<	ARCH>
{
	OpBa(CodeType code, uint32_t addr);
	uint8_t annul;
	int32_t disp22;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpFba : public Operation<	ARCH>
{
	OpFba(CodeType code, uint32_t addr);
	uint8_t annul;
	int32_t disp22;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpCba : public Operation<	ARCH>
{
	OpCba(CodeType code, uint32_t addr);
	uint8_t annul;
	int32_t disp22;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpCbn : public Operation<	ARCH>
{
	OpCbn(CodeType code, uint32_t addr);
	uint8_t annul;
	int32_t disp22;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpCb3 : public Operation<	ARCH>
{
	OpCb3(CodeType code, uint32_t addr);
	uint8_t annul;
	int32_t disp22;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpCb2 : public Operation<	ARCH>
{
	OpCb2(CodeType code, uint32_t addr);
	uint8_t annul;
	int32_t disp22;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpCb23 : public Operation<	ARCH>
{
	OpCb23(CodeType code, uint32_t addr);
	uint8_t annul;
	int32_t disp22;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpCb1 : public Operation<	ARCH>
{
	OpCb1(CodeType code, uint32_t addr);
	uint8_t annul;
	int32_t disp22;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpCb13 : public Operation<	ARCH>
{
	OpCb13(CodeType code, uint32_t addr);
	uint8_t annul;
	int32_t disp22;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpCb12 : public Operation<	ARCH>
{
	OpCb12(CodeType code, uint32_t addr);
	uint8_t annul;
	int32_t disp22;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpCb123 : public Operation<	ARCH>
{
	OpCb123(CodeType code, uint32_t addr);
	uint8_t annul;
	int32_t disp22;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpCb0 : public Operation<	ARCH>
{
	OpCb0(CodeType code, uint32_t addr);
	uint8_t annul;
	int32_t disp22;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpCb03 : public Operation<	ARCH>
{
	OpCb03(CodeType code, uint32_t addr);
	uint8_t annul;
	int32_t disp22;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpCb02 : public Operation<	ARCH>
{
	OpCb02(CodeType code, uint32_t addr);
	uint8_t annul;
	int32_t disp22;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpCb023 : public Operation<	ARCH>
{
	OpCb023(CodeType code, uint32_t addr);
	uint8_t annul;
	int32_t disp22;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpCb01 : public Operation<	ARCH>
{
	OpCb01(CodeType code, uint32_t addr);
	uint8_t annul;
	int32_t disp22;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpCb013 : public Operation<	ARCH>
{
	OpCb013(CodeType code, uint32_t addr);
	uint8_t annul;
	int32_t disp22;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpCb012 : public Operation<	ARCH>
{
	OpCb012(CodeType code, uint32_t addr);
	uint8_t annul;
	int32_t disp22;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpCall : public Operation<	ARCH>
{
	OpCall(CodeType code, uint32_t addr);
	uint32_t disp30;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpJmpl : public Operation<	ARCH>
{
	OpJmpl(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t asi;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpJmpli : public Operation<	ARCH>
{
	OpJmpli(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpRett : public Operation<	ARCH>
{
	OpRett(CodeType code, uint32_t addr);
	uint8_t rs1;
	uint8_t asi;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpRetti : public Operation<	ARCH>
{
	OpRetti(CodeType code, uint32_t addr);
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpTa : public Operation<	ARCH>
{
	OpTa(CodeType code, uint32_t addr);
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpTai : public Operation<	ARCH>
{
	OpTai(CodeType code, uint32_t addr);
	uint8_t rs1;
	uint8_t imm7;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpRdy : public Operation<	ARCH>
{
	OpRdy(CodeType code, uint32_t addr);
	uint8_t rd;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpStbar : public Operation<	ARCH>
{
	OpStbar(CodeType code, uint32_t addr);
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpRdpsr : public Operation<	ARCH>
{
	OpRdpsr(CodeType code, uint32_t addr);
	uint8_t rd;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpRdwim : public Operation<	ARCH>
{
	OpRdwim(CodeType code, uint32_t addr);
	uint8_t rd;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpRdtbr : public Operation<	ARCH>
{
	OpRdtbr(CodeType code, uint32_t addr);
	uint8_t rd;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpWry : public Operation<	ARCH>
{
	OpWry(CodeType code, uint32_t addr);
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpWryi : public Operation<	ARCH>
{
	OpWryi(CodeType code, uint32_t addr);
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpWrpsr : public Operation<	ARCH>
{
	OpWrpsr(CodeType code, uint32_t addr);
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpWrpsri : public Operation<	ARCH>
{
	OpWrpsri(CodeType code, uint32_t addr);
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpWrwim : public Operation<	ARCH>
{
	OpWrwim(CodeType code, uint32_t addr);
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpWrwimi : public Operation<	ARCH>
{
	OpWrwimi(CodeType code, uint32_t addr);
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpWrtbr : public Operation<	ARCH>
{
	OpWrtbr(CodeType code, uint32_t addr);
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpWrtbri : public Operation<	ARCH>
{
	OpWrtbri(CodeType code, uint32_t addr);
	uint8_t rs1;
	int32_t simm13;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpUnimp : public Operation<	ARCH>
{
	OpUnimp(CodeType code, uint32_t addr);
	uint32_t const22;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpFitos : public Operation<	ARCH>
{
	OpFitos(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpFitod : public Operation<	ARCH>
{
	OpFitod(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpFitoq : public Operation<	ARCH>
{
	OpFitoq(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpFstoi : public Operation<	ARCH>
{
	OpFstoi(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpFdtoi : public Operation<	ARCH>
{
	OpFdtoi(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpFqtoi : public Operation<	ARCH>
{
	OpFqtoi(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpFstod : public Operation<	ARCH>
{
	OpFstod(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpFstoq : public Operation<	ARCH>
{
	OpFstoq(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpFdtos : public Operation<	ARCH>
{
	OpFdtos(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpFdtoq : public Operation<	ARCH>
{
	OpFdtoq(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpFqtos : public Operation<	ARCH>
{
	OpFqtos(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpFqtod : public Operation<	ARCH>
{
	OpFqtod(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpFmovs : public Operation<	ARCH>
{
	OpFmovs(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpFnegs : public Operation<	ARCH>
{
	OpFnegs(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpFabss : public Operation<	ARCH>
{
	OpFabss(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpFsqrts : public Operation<	ARCH>
{
	OpFsqrts(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpFsqrtd : public Operation<	ARCH>
{
	OpFsqrtd(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpFsqrtq : public Operation<	ARCH>
{
	OpFsqrtq(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpFadds : public Operation<	ARCH>
{
	OpFadds(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpFaddd : public Operation<	ARCH>
{
	OpFaddd(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpFaddq : public Operation<	ARCH>
{
	OpFaddq(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpFsubs : public Operation<	ARCH>
{
	OpFsubs(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpFsubd : public Operation<	ARCH>
{
	OpFsubd(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpFsubq : public Operation<	ARCH>
{
	OpFsubq(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpFmuls : public Operation<	ARCH>
{
	OpFmuls(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpFmuld : public Operation<	ARCH>
{
	OpFmuld(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpFmulq : public Operation<	ARCH>
{
	OpFmulq(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpFsmuld : public Operation<	ARCH>
{
	OpFsmuld(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpFdmulq : public Operation<	ARCH>
{
	OpFdmulq(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpFdivs : public Operation<	ARCH>
{
	OpFdivs(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpFdivd : public Operation<	ARCH>
{
	OpFdivd(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpFdivq : public Operation<	ARCH>
{
	OpFdivq(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpFcmps : public Operation<	ARCH>
{
	OpFcmps(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpFcmpd : public Operation<	ARCH>
{
	OpFcmpd(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpFcmpq : public Operation<	ARCH>
{
	OpFcmpq(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
struct OpFcmpes : public Operation<	ARCH>
{
	OpFcmpes(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpFcmped : public Operation<	ARCH>
{
	OpFcmped(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual
	void execute( ARCH& cpu ) const;
private:
};

template <	typename	ARCH>
struct OpFcmpeq : public Operation<	ARCH>
{
	OpFcmpeq(CodeType code, uint32_t addr);
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	virtual
	void disasm( std::ostream& sink,
	uint32_t pc ) const;
private:
};

template <	typename	ARCH>
void OpBc<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "b" << DisasmCondition(cond) << (annul ? ",a " : " ") << "" << DisasmHex(this->GetAddr() + disp22);
}}
template <	typename	ARCH>
void OpBc<	ARCH>::execute( ARCH& cpu) const
{
	{
		if (cpu.TestCondition(cond))
		cpu.DelayBranch(cpu.GetPC() + typename ARCH::U32(disp22), ARCH::B_JMP);
		else
		cpu.SetAnnul(annul);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpBc(CodeType code, uint32_t addr)
{
	return new OpBc<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpFbc<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "fb" << DisasmFCondition(cond) << (annul ? ",a " : " ") << "" << DisasmHex(pc + disp22);
}}
template <	typename	ARCH>
void OpFbc<	ARCH>::execute( ARCH& cpu) const
{
	{
		if (cpu.TestFCondition(cond))
		cpu.DelayBranch(cpu.GetPC() + typename ARCH::U32(disp22), ARCH::B_JMP);
		else
		cpu.SetAnnul(annul);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpFbc(CodeType code, uint32_t addr)
{
	return new OpFbc<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpTc<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "t" << DisasmCondition(cond) << " " << DisasmGPR(rs1) << " + " << DisasmGPR(rs2);
}}
template <	typename	ARCH>
void OpTc<	ARCH>::execute( ARCH& cpu) const
{
	{
		if (cpu.TestCondition(cond))
		cpu.SWTrap( cpu.GetGPR(rs1) + cpu.GetGPR(rs2) );
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpTc(CodeType code, uint32_t addr)
{
	return new OpTc<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpTci<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "t" << DisasmCondition(cond) << " " << DisasmGPR(rs1) << " + " << "" << DisasmHex(imm7);
}}
template <	typename	ARCH>
void OpTci<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;

		if (cpu.TestCondition(cond))
		cpu.SWTrap( cpu.GetGPR(rs1) + U32(imm7) );
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpTci(CodeType code, uint32_t addr)
{
	return new OpTci<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpRdasr<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "rd %asr" << DisasmI(rs1) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpRdasr<	ARCH>::execute( ARCH& cpu) const
{
	{
		if (rs1 < 16)
		cpu.Abort( Trap_t::illegal_instruction );
		else if (not cpu.asr_perm( rs1 ))
		cpu.Abort( Trap_t::privileged_instruction );
		else
		cpu.SetGPR(rd, cpu.rdasr( rs1 ));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpRdasr(CodeType code, uint32_t addr)
{
	return new OpRdasr<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpWrasr<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "wr " << DisasmGPR(rs1) << ", " << DisasmGPR(rs2) << ", %asr" << DisasmI(rd);
}}
template <	typename	ARCH>
void OpWrasr<	ARCH>::execute( ARCH& cpu) const
{
	{
		if (rd < 16)
		cpu.Abort( Trap_t::illegal_instruction );
		else if (not cpu.asr_perm( rd ))
		cpu.Abort( Trap_t::privileged_instruction );
		else
		cpu.wrasr( rd, cpu.GetGPR(rs1) ^ cpu.GetGPR(rs2) );
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpWrasr(CodeType code, uint32_t addr)
{
	return new OpWrasr<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpWrasri<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "wr " << DisasmGPR(rs1) << ", " << DisasmI(simm13) << ", %asr" << std::dec << int(rd);
}}
template <	typename	ARCH>
void OpWrasri<	ARCH>::execute( ARCH& cpu) const
{
	{
		if (rd < 16)
		cpu.Abort( Trap_t::illegal_instruction );
		else if (not cpu.asr_perm( rd ))
		cpu.Abort( Trap_t::privileged_instruction );
		else
		cpu.wrasr( rd, cpu.GetGPR(rs1) ^ typename ARCH::U32(simm13) );
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpWrasri(CodeType code, uint32_t addr)
{
	return new OpWrasri<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpLdsb<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "ldsb" << (a ? "a" : "") << " [" << DisasmRRAddress(rs1, rs2) << "]";
		if (a) sink << "" << DisasmHex(asi);
		sink << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpLdsb<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::S8 S8;
		typedef typename ARCH::U32 U32;

		if (a and not cpu.IsSuper())
		cpu.Abort( Trap_t::privileged_instruction );

		cpu.SetGPR(rd, U32(cpu.MemRead(S8(), a ? ASI::Code(asi) : cpu.rqasi(), cpu.GetGPR(rs1) + cpu.GetGPR(rs2))));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpLdsb(CodeType code, uint32_t addr)
{
	return new OpLdsb<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpLdsbi<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "ldsb [" << DisasmRIAddress(rs1, simm13) << "], " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpLdsbi<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::S8 S8;
		typedef typename ARCH::U32 U32;

		cpu.SetGPR(rd, U32(cpu.MemRead(S8(), cpu.rqasi(), cpu.GetGPR(rs1) + U32(simm13))));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpLdsbi(CodeType code, uint32_t addr)
{
	return new OpLdsbi<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpLdsh<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "ldsh" << (a ? "a" : "") << " [" << DisasmRRAddress(rs1, rs2) << "]";
		if (a) sink << "" << DisasmHex(asi);
		sink << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpLdsh<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::S16 S16;
		typedef typename ARCH::U32 U32;

		if (a and not cpu.IsSuper())
		cpu.Abort( Trap_t::privileged_instruction );
		cpu.SetGPR(rd, U32(cpu.MemRead(S16(), a ? ASI::Code(asi) : cpu.rqasi(), cpu.GetGPR(rs1) + cpu.GetGPR(rs2))));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpLdsh(CodeType code, uint32_t addr)
{
	return new OpLdsh<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpLdshi<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "ldsh [" << DisasmRIAddress(rs1, simm13) << "], " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpLdshi<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::S16 S16;
		typedef typename ARCH::U32 U32;

		cpu.SetGPR(rd, U32(cpu.MemRead(S16(), cpu.rqasi(), cpu.GetGPR(rs1) + U32(simm13))));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpLdshi(CodeType code, uint32_t addr)
{
	return new OpLdshi<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpLdub<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "ldub" << (a ? "a" : "") << " [" << DisasmRRAddress(rs1, rs2) << "]";
		if (a) sink << "" << DisasmHex(asi);
		sink << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpLdub<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U8 U8;
		typedef typename ARCH::U32 U32;

		if (a and not cpu.IsSuper())
		cpu.Abort( Trap_t::privileged_instruction );
		cpu.SetGPR(rd, U32(cpu.MemRead(U8(), a ? ASI::Code(asi) : cpu.rqasi(), cpu.GetGPR(rs1) + cpu.GetGPR(rs2))));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpLdub(CodeType code, uint32_t addr)
{
	return new OpLdub<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpLdubi<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "ldub [" << DisasmRIAddress(rs1, simm13) << "], " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpLdubi<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U8 U8;
		typedef typename ARCH::U32 U32;

		cpu.SetGPR(rd, U32(cpu.MemRead(U8(), cpu.rqasi(), cpu.GetGPR(rs1) + U32(simm13))));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpLdubi(CodeType code, uint32_t addr)
{
	return new OpLdubi<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpLduh<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "lduh" << (a ? "a" : "") << " [" << DisasmRRAddress(rs1, rs2) << "]";
		if (a) sink << "" << DisasmHex(asi);
		sink << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpLduh<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U16 U16;
		typedef typename ARCH::U32 U32;

		if (a and not cpu.IsSuper())
		cpu.Abort( Trap_t::privileged_instruction );
		cpu.SetGPR(rd, U32(cpu.MemRead(U16(), a ? ASI::Code(asi) : cpu.rqasi(), cpu.GetGPR(rs1) + cpu.GetGPR(rs2))));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpLduh(CodeType code, uint32_t addr)
{
	return new OpLduh<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpLduhi<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "lduh [" << DisasmRIAddress(rs1, simm13) << "], " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpLduhi<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U16 U16;
		typedef typename ARCH::U32 U32;

		cpu.SetGPR(rd, U32(cpu.MemRead(U16(), cpu.rqasi(), cpu.GetGPR(rs1) + U32(simm13))));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpLduhi(CodeType code, uint32_t addr)
{
	return new OpLduhi<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpLd<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "ld" << (a ? "a" : "") << " [" << DisasmRRAddress(rs1, rs2) << "]";
		if (a) sink << "" << DisasmHex(asi);
		sink << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpLd<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;

		if (a and not cpu.IsSuper())
		cpu.Abort( Trap_t::privileged_instruction );
		cpu.SetGPR(rd, cpu.MemRead(U32(), a ? ASI::Code(asi) : cpu.rqasi(), cpu.GetGPR(rs1) + cpu.GetGPR(rs2)));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpLd(CodeType code, uint32_t addr)
{
	return new OpLd<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpLdi<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "ld [" << DisasmRIAddress(rs1, simm13) << "], " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpLdi<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;

		cpu.SetGPR(rd, cpu.MemRead(U32(), cpu.rqasi(), cpu.GetGPR(rs1) + U32(simm13)));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpLdi(CodeType code, uint32_t addr)
{
	return new OpLdi<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpLdd<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "ldd" << (a ? "a" : "") << " [" << DisasmRRAddress(rs1, rs2) << "]";
		if (a) sink << "" << DisasmHex(asi);
		sink << ", " << DisasmGPR(rd & -2);
}}
template <	typename	ARCH>
void OpLdd<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::U64 U64;

		if (a and not cpu.IsSuper())
		cpu.Abort( Trap_t::privileged_instruction );
		U64 value = cpu.MemRead(U64(), a ? ASI::Code(asi) : cpu.rqasi(), cpu.GetGPR(rs1) + cpu.GetGPR(rs2));
		cpu.SetGPR((rd & -2) | 0, U32(value >> 32));
		cpu.SetGPR((rd & -2) | 1, U32(value >> 0));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpLdd(CodeType code, uint32_t addr)
{
	return new OpLdd<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpLddi<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "ldd [" << DisasmRIAddress(rs1, simm13) << "], " << DisasmGPR(rd & -2);
}}
template <	typename	ARCH>
void OpLddi<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::U64 U64;

		U64 value = cpu.MemRead(U64(), cpu.rqasi(), cpu.GetGPR(rs1) + U32(simm13));
		cpu.SetGPR((rd & -2) | 0, U32(value >> 32));
		cpu.SetGPR((rd & -2) | 1, U32(value >> 0));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpLddi(CodeType code, uint32_t addr)
{
	return new OpLddi<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpLdf<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "ld [" << DisasmRRAddress(rs1, rs2) << "], " << DisasmFPR(rd);
}}
template <	typename	ARCH>
void OpLdf<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::F32 F32;

		cpu.SetF32(rd, cpu.FMemRead(F32(), cpu.rqasi(), cpu.GetGPR(rs1) + cpu.GetGPR(rs2)));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpLdf(CodeType code, uint32_t addr)
{
	return new OpLdf<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpLdfi<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "ld [" << DisasmRIAddress(rs1, simm13) << "], " << DisasmFPR(rd);
}}
template <	typename	ARCH>
void OpLdfi<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::F32 F32;
		typedef typename ARCH::U32 U32;

		cpu.SetF32(rd, cpu.FMemRead(F32(), cpu.rqasi(), cpu.GetGPR(rs1) + U32(simm13)));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpLdfi(CodeType code, uint32_t addr)
{
	return new OpLdfi<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpLddf<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "ldd [" << DisasmRRAddress(rs1, rs2) << "], " << DisasmFPR(rd & -2);
}}
template <	typename	ARCH>
void OpLddf<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::F64 F64;

		cpu.SetF64(rd & -2, cpu.FMemRead(F64(), cpu.rqasi(), cpu.GetGPR(rs1) + cpu.GetGPR(rs2)));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpLddf(CodeType code, uint32_t addr)
{
	return new OpLddf<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpLddfi<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "ldd [" << DisasmRIAddress(rs1, simm13) << "], " << DisasmFPR(rd & -2);
}}
template <	typename	ARCH>
void OpLddfi<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::F64 F64;

		cpu.SetF64(rd & -2, cpu.FMemRead(F64(), cpu.rqasi(), cpu.GetGPR(rs1) + U32(simm13)));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpLddfi(CodeType code, uint32_t addr)
{
	return new OpLddfi<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpLdfsr<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "ld [" << DisasmRRAddress(rs1, rs2) << "], %fsr";
}}
template <	typename	ARCH>
void OpLdfsr<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;

		cpu.SetFSR( cpu.MemRead(U32(), cpu.rqasi(), cpu.GetGPR(rs1) + cpu.GetGPR(rs2)) );
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpLdfsr(CodeType code, uint32_t addr)
{
	return new OpLdfsr<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpLdfsri<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "ldd [" << DisasmRIAddress(rs1, simm13) << "], %fsr";
}}
template <	typename	ARCH>
void OpLdfsri<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;

		cpu.SetFSR( cpu.MemRead(U32(), cpu.rqasi(), cpu.GetGPR(rs1) + U32(simm13)) );
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpLdfsri(CodeType code, uint32_t addr)
{
	return new OpLdfsri<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpLdc<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "ld [" << DisasmRRAddress(rs1, rs2) << "], " << DisasmCPR(rd);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpLdc(CodeType code, uint32_t addr)
{
	return new OpLdc<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpLdci<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "ld [" << DisasmRIAddress(rs1, simm13) << "], " << DisasmCPR(rd);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpLdci(CodeType code, uint32_t addr)
{
	return new OpLdci<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpLddc<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "ldd [" << DisasmRRAddress(rs1, rs2) << "], " << DisasmCPR(rd & -2);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpLddc(CodeType code, uint32_t addr)
{
	return new OpLddc<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpLddci<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "ldd [" << DisasmRIAddress(rs1, simm13) << "], " << DisasmCPR(rd & -2);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpLddci(CodeType code, uint32_t addr)
{
	return new OpLddci<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpLdcsr<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "ld [" << DisasmRRAddress(rs1, rs2) << "], %csr";
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpLdcsr(CodeType code, uint32_t addr)
{
	return new OpLdcsr<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpLdcsri<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "ldd [" << DisasmRIAddress(rs1, simm13) << "], %csr";
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpLdcsri(CodeType code, uint32_t addr)
{
	return new OpLdcsri<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpStb<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "stb" << (a ? "a" : "") << " " << DisasmGPR(rd) << ", [" << DisasmRRAddress(rs1, rs2) << "]";
		if (a) sink << "" << DisasmHex(asi);
}}
template <	typename	ARCH>
void OpStb<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U8 U8;

		if (a and not cpu.IsSuper())
		cpu.Abort( Trap_t::privileged_instruction );
		cpu.MemWrite( a ? ASI::Code(asi) : cpu.rqasi(), cpu.GetGPR(rs1) + cpu.GetGPR(rs2), U8(cpu.GetGPR(rd)) );
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpStb(CodeType code, uint32_t addr)
{
	return new OpStb<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpStbi<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "stb " << DisasmGPR(rd) << ", [" << DisasmRIAddress(rs1, simm13) << "]";
}}
template <	typename	ARCH>
void OpStbi<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U8 U8;
		typedef typename ARCH::U32 U32;

		cpu.MemWrite( cpu.rqasi(), cpu.GetGPR(rs1) + U32(simm13), U8(cpu.GetGPR(rd)) );
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpStbi(CodeType code, uint32_t addr)
{
	return new OpStbi<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpSth<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "sth" << (a ? "a" : "") << " " << DisasmGPR(rd) << ", [" << DisasmRRAddress(rs1, rs2) << "]";
		if (a) sink << "" << DisasmHex(asi);
}}
template <	typename	ARCH>
void OpSth<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U16 U16;

		if (a and not cpu.IsSuper())
		cpu.Abort( Trap_t::privileged_instruction );
		cpu.MemWrite( a ? ASI::Code(asi) : cpu.rqasi(), cpu.GetGPR(rs1) + cpu.GetGPR(rs2), U16(cpu.GetGPR(rd)) );
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpSth(CodeType code, uint32_t addr)
{
	return new OpSth<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpSthi<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "sth " << DisasmGPR(rd) << ", [" << DisasmRIAddress(rs1, simm13) << "]";
}}
template <	typename	ARCH>
void OpSthi<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U16 U16;
		typedef typename ARCH::U32 U32;

		cpu.MemWrite( cpu.rqasi(), cpu.GetGPR(rs1) + U32(simm13), U16(cpu.GetGPR(rd)) );
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpSthi(CodeType code, uint32_t addr)
{
	return new OpSthi<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpSt<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "st" << (a ? "a" : "") << " " << DisasmGPR(rd) << ", [" << DisasmRRAddress(rs1, rs2) << "]";
		if (a) sink << "" << DisasmHex(asi);
}}
template <	typename	ARCH>
void OpSt<	ARCH>::execute( ARCH& cpu) const
{
	{
		if (a and not cpu.IsSuper())
		cpu.Abort( Trap_t::privileged_instruction );
		cpu.MemWrite( a ? ASI::Code(asi) : cpu.rqasi(), cpu.GetGPR(rs1) + cpu.GetGPR(rs2), cpu.GetGPR(rd) );
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpSt(CodeType code, uint32_t addr)
{
	return new OpSt<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpSti<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "st " << DisasmGPR(rd) << ", [" << DisasmRIAddress(rs1, simm13) << "]";
}}
template <	typename	ARCH>
void OpSti<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;

		cpu.MemWrite( cpu.rqasi(), cpu.GetGPR(rs1) + U32(simm13), cpu.GetGPR(rd) );
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpSti(CodeType code, uint32_t addr)
{
	return new OpSti<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpStd<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "std" << (a ? "a" : "") << " " << DisasmGPR(rd & -2) << ", [" << DisasmRRAddress(rs1, rs2) << "]";
		if (a) sink << "" << DisasmHex(asi);
}}
template <	typename	ARCH>
void OpStd<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U64 U64;

		if (a and not cpu.IsSuper())
		cpu.Abort( Trap_t::privileged_instruction );
		cpu.MemWrite( a ? ASI::Code(asi) : cpu.rqasi(), cpu.GetGPR(rs1) + cpu.GetGPR(rs2),
		U64(cpu.GetGPR((rd & -2) | 0)) << 32 | U64(cpu.GetGPR((rd & -2) | 1)) << 0 );
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpStd(CodeType code, uint32_t addr)
{
	return new OpStd<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpStdi<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "std " << DisasmGPR(rd & -2) << ", [" << DisasmRIAddress(rs1, simm13) << "]";
}}
template <	typename	ARCH>
void OpStdi<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::U64 U64;

		cpu.MemWrite( cpu.rqasi(), cpu.GetGPR(rs1) + U32(simm13),
		U64(cpu.GetGPR((rd & -2) | 0)) << 32 | U64(cpu.GetGPR((rd & -2) | 1)) << 0 );
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpStdi(CodeType code, uint32_t addr)
{
	return new OpStdi<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpStf<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "st " << DisasmFPR(rd) << ", [" << DisasmRRAddress(rs1, rs2) << "]";
}}
template <	typename	ARCH>
void OpStf<	ARCH>::execute( ARCH& cpu) const
{
	{
		cpu.FMemWrite( cpu.rqasi(), cpu.GetGPR(rs1) + cpu.GetGPR(rs2), cpu.GetF32(rd) );
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpStf(CodeType code, uint32_t addr)
{
	return new OpStf<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpStfi<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "st " << DisasmFPR(rd) << ", [" << DisasmRIAddress(rs1, simm13) << "]";
}}
template <	typename	ARCH>
void OpStfi<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;

		cpu.FMemWrite( cpu.rqasi(), cpu.GetGPR(rs1) + U32(simm13), cpu.GetF32(rd) );
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpStfi(CodeType code, uint32_t addr)
{
	return new OpStfi<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpStdf<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "std " << DisasmFPR(rd & -2) << ", [" << DisasmRRAddress(rs1, rs2) << "]";
}}
template <	typename	ARCH>
void OpStdf<	ARCH>::execute( ARCH& cpu) const
{
	{
		cpu.FMemWrite( cpu.rqasi(), cpu.GetGPR(rs1) + cpu.GetGPR(rs2), cpu.GetF64(rd) );
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpStdf(CodeType code, uint32_t addr)
{
	return new OpStdf<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpStdfi<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "std " << DisasmFPR(rd & -2) << ", [" << DisasmRIAddress(rs1, simm13) << "]";
}}
template <	typename	ARCH>
void OpStdfi<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;

		cpu.FMemWrite( cpu.rqasi(), cpu.GetGPR(rs1) + U32(simm13), cpu.GetF64(rd) );
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpStdfi(CodeType code, uint32_t addr)
{
	return new OpStdfi<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpStfsr<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "st %fsr, [" << DisasmRRAddress(rs1, rs2) << "]";
}}
template <	typename	ARCH>
void OpStfsr<	ARCH>::execute( ARCH& cpu) const
{
	{
		cpu.MemWrite( cpu.rqasi(), cpu.GetGPR(rs1) + cpu.GetGPR(rs2), cpu.GetFSR() );
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpStfsr(CodeType code, uint32_t addr)
{
	return new OpStfsr<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpStfsri<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "st %fsr, [" << DisasmRIAddress(rs1, simm13) << "]";
}}
template <	typename	ARCH>
void OpStfsri<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;

		cpu.MemWrite( cpu.rqasi(), cpu.GetGPR(rs1) + U32(simm13), cpu.GetFSR() );
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpStfsri(CodeType code, uint32_t addr)
{
	return new OpStfsri<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpStdfq<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "st %fq, [" << DisasmRRAddress(rs1, rs2) << "]";
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpStdfq(CodeType code, uint32_t addr)
{
	return new OpStdfq<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpStdfqi<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "st %fq, [" << DisasmRIAddress(rs1, simm13) << "]";
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpStdfqi(CodeType code, uint32_t addr)
{
	return new OpStdfqi<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpStc<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "st " << DisasmCPR(rd) << ", [" << DisasmRRAddress(rs1, rs2) << "]";
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpStc(CodeType code, uint32_t addr)
{
	return new OpStc<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpStci<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "st " << DisasmCPR(rd) << ", [" << DisasmRIAddress(rs1, simm13) << "]";
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpStci(CodeType code, uint32_t addr)
{
	return new OpStci<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpStdc<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "std " << DisasmCPR(rd & -2) << ", [" << DisasmRRAddress(rs1, rs2) << "]";
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpStdc(CodeType code, uint32_t addr)
{
	return new OpStdc<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpStdci<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "std " << DisasmCPR(rd & -2) << ", [" << DisasmRIAddress(rs1, simm13) << "]";
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpStdci(CodeType code, uint32_t addr)
{
	return new OpStdci<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpStcsr<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "st %csr, [" << DisasmRRAddress(rs1, rs2) << "]";
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpStcsr(CodeType code, uint32_t addr)
{
	return new OpStcsr<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpStcsri<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "st %csr, [" << DisasmRIAddress(rs1, simm13) << "]";
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpStcsri(CodeType code, uint32_t addr)
{
	return new OpStcsri<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpStdcq<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "st %cq, [" << DisasmRRAddress(rs1, rs2) << "]";
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpStdcq(CodeType code, uint32_t addr)
{
	return new OpStdcq<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpStdcqi<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "st %cq, [" << DisasmRIAddress(rs1, simm13) << "]";
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpStdcqi(CodeType code, uint32_t addr)
{
	return new OpStdcqi<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpLdstub<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "ldstub" << (a ? "a" : "") << " [" << DisasmRRAddress(rs1, rs2) << "]";
		if (a) sink << "" << DisasmHex(asi);
		sink << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpLdstub<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U8 U8;
		typedef typename ARCH::U32 U32;

		if (a and not cpu.IsSuper())
		cpu.Abort( Trap_t::privileged_instruction );
		U32 address = cpu.GetGPR(rs1) + cpu.GetGPR(rs2);
		U8 res = cpu.MemRead(U8(), a ? ASI::Code(asi) : cpu.rqasi(), address );
		cpu.MemWrite( cpu.rqasi(), address, U8(0xff) );

		cpu.SetGPR(rd, U32(res));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpLdstub(CodeType code, uint32_t addr)
{
	return new OpLdstub<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpLdstubi<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "ldstub [" << DisasmRIAddress(rs1, simm13) << "], " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpLdstubi<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U8 U8;
		typedef typename ARCH::U32 U32;

		U32 address = cpu.GetGPR(rs1) + U32(simm13);
		U8 res = cpu.MemRead(U8(), cpu.rqasi(), address );
		cpu.MemWrite( cpu.rqasi(), address, U8(0xff) );

		cpu.SetGPR(rd, U32(res));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpLdstubi(CodeType code, uint32_t addr)
{
	return new OpLdstubi<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpSwap<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "swap" << (a ? "a" : "") << " [" << DisasmRRAddress(rs1, rs2) << "]";
		if (a) sink << "" << DisasmHex(asi);
		sink << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpSwap<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;

		if (a and not cpu.IsSuper())
		cpu.Abort( Trap_t::privileged_instruction );
		U32 address = cpu.GetGPR(rs1) + cpu.GetGPR(rs2);
		ASI wasi = a ? ASI::Code(asi) : cpu.rqasi();
		U32 res = cpu.MemRead(U32(), wasi, address );
		cpu.MemWrite( wasi, address, cpu.GetGPR(rd) );
		cpu.SetGPR(rd, res);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpSwap(CodeType code, uint32_t addr)
{
	return new OpSwap<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpSwapi<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "swap [" << DisasmRIAddress(rs1, simm13) << "], " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpSwapi<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;

		U32 address = cpu.GetGPR(rs1) + U32(simm13);
		U32 res = cpu.MemRead(U32(), cpu.rqasi(), address );
		cpu.MemWrite( cpu.rqasi(), address, cpu.GetGPR(rd) );
		cpu.SetGPR(rd, res);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpSwapi(CodeType code, uint32_t addr)
{
	return new OpSwapi<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpFlush<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "flush " << DisasmRRAddress(rs1, rs2);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpFlush(CodeType code, uint32_t addr)
{
	return new OpFlush<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpFlushi<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "flush " << DisasmRIAddress(rs1, simm13);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpFlushi(CodeType code, uint32_t addr)
{
	return new OpFlushi<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpSethi<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		if( rd == 0 and imm22 == 0 ) sink << "nop";
		else sink << "sethi %hi(" << DisasmHex(imm22) << "), " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpSethi<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;

		cpu.SetGPR(rd, U32(imm22) /* | (cpu.GetGPR(rd) & 0x3ff) */);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpSethi(CodeType code, uint32_t addr)
{
	return new OpSethi<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpAnd_<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "and " << DisasmGPR(rs1) << ", " << DisasmGPR(rs2) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpAnd_<	ARCH>::execute( ARCH& cpu) const
{
	{
		cpu.SetGPR(rd, cpu.GetGPR(rs1) & cpu.GetGPR(rs2));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpAnd_(CodeType code, uint32_t addr)
{
	return new OpAnd_<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpAndi<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "and " << DisasmGPR(rs1) << ", " << DisasmI(simm13) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpAndi<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;

		cpu.SetGPR(rd, cpu.GetGPR(rs1) & U32(simm13));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpAndi(CodeType code, uint32_t addr)
{
	return new OpAndi<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpAndcc<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "andcc " << DisasmGPR(rs1) << ", " << DisasmGPR(rs2) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpAndcc<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::S32 S32;
		typedef typename ARCH::BOOL BOOL;

		U32 result = cpu.GetGPR(rs1) & cpu.GetGPR(rs2);
		cpu.SetNZVC( S32(result) < S32(0), result == U32(0), BOOL(false), BOOL(false) );
		cpu.SetGPR(rd, result);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpAndcc(CodeType code, uint32_t addr)
{
	return new OpAndcc<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpAndcci<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "andcc " << DisasmGPR(rs1) << ", " << DisasmI(simm13) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpAndcci<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::S32 S32;
		typedef typename ARCH::BOOL BOOL;

		U32 result = cpu.GetGPR(rs1) & U32(simm13);
		cpu.SetNZVC( S32(result) < S32(0), result == U32(0), BOOL(false), BOOL(false) );
		cpu.SetGPR(rd, result);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpAndcci(CodeType code, uint32_t addr)
{
	return new OpAndcci<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpAndn<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "andn " << DisasmGPR(rs1) << ", " << DisasmGPR(rs2) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpAndn<	ARCH>::execute( ARCH& cpu) const
{
	{
		cpu.SetGPR(rd, cpu.GetGPR(rs1) & ~(cpu.GetGPR(rs2)));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpAndn(CodeType code, uint32_t addr)
{
	return new OpAndn<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpAndni<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "andn " << DisasmGPR(rs1) << ", " << DisasmI(simm13) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpAndni<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;

		cpu.SetGPR(rd, cpu.GetGPR(rs1) & ~U32(simm13));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpAndni(CodeType code, uint32_t addr)
{
	return new OpAndni<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpAndncc<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "andncc " << DisasmGPR(rs1) << ", " << DisasmGPR(rs2) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpAndncc<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::S32 S32;
		typedef typename ARCH::BOOL BOOL;

		U32 result = cpu.GetGPR(rs1) & ~(cpu.GetGPR(rs2));
		cpu.SetNZVC( S32(result) < S32(0), result == U32(0), BOOL(false), BOOL(false) );
		cpu.SetGPR(rd, result);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpAndncc(CodeType code, uint32_t addr)
{
	return new OpAndncc<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpAndncci<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "andncc " << DisasmGPR(rs1) << ", " << DisasmI(simm13) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpAndncci<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::S32 S32;
		typedef typename ARCH::BOOL BOOL;

		U32 result = cpu.GetGPR(rs1) & ~U32(simm13);
		cpu.SetNZVC( S32(result) < S32(0), result == U32(0), BOOL(false), BOOL(false) );
		cpu.SetGPR(rd, result);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpAndncci(CodeType code, uint32_t addr)
{
	return new OpAndncci<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpOr_<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "or " << DisasmGPR(rs1) << ", " << DisasmGPR(rs2) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpOr_<	ARCH>::execute( ARCH& cpu) const
{
	{
		cpu.SetGPR(rd, cpu.GetGPR(rs1) | cpu.GetGPR(rs2));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpOr_(CodeType code, uint32_t addr)
{
	return new OpOr_<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpOri<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "or " << DisasmGPR(rs1) << ", " << DisasmI(simm13) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpOri<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;

		cpu.SetGPR(rd, cpu.GetGPR(rs1) | U32(simm13));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpOri(CodeType code, uint32_t addr)
{
	return new OpOri<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpOrcc<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		if (rd == 0) {
			sink << "tst " << DisasmGPR(rs1) << ", " << DisasmGPR(rs2);
			} else {
			sink << "orcc " << DisasmGPR(rs1) << ", " << DisasmGPR(rs2) << ", " << DisasmGPR(rd);
		}
}}
template <	typename	ARCH>
void OpOrcc<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::S32 S32;
		typedef typename ARCH::BOOL BOOL;

		U32 result = cpu.GetGPR(rs1) | cpu.GetGPR(rs2);
		cpu.SetNZVC( S32(result) < S32(0), result == U32(0), BOOL(false), BOOL(false) );
		cpu.SetGPR(rd, result);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpOrcc(CodeType code, uint32_t addr)
{
	return new OpOrcc<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpOrcci<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		if (rd == 0) {
			sink << "tst " << DisasmGPR(rs1) << ", " << DisasmI(simm13);
			} else {
			sink << "orcc " << DisasmGPR(rs1) << ", " << DisasmI(simm13) << ", " << DisasmGPR(rd);
		}
}}
template <	typename	ARCH>
void OpOrcci<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::S32 S32;
		typedef typename ARCH::BOOL BOOL;

		U32 result = cpu.GetGPR(rs1) | U32(simm13);
		cpu.SetNZVC( S32(result) < S32(0), result == U32(0), BOOL(false), BOOL(false) );
		cpu.SetGPR(rd, result);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpOrcci(CodeType code, uint32_t addr)
{
	return new OpOrcci<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpOrn<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "orn " << DisasmGPR(rs1) << ", " << DisasmGPR(rs2) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpOrn<	ARCH>::execute( ARCH& cpu) const
{
	{
		cpu.SetGPR(rd, cpu.GetGPR(rs1) | ~(cpu.GetGPR(rs2)));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpOrn(CodeType code, uint32_t addr)
{
	return new OpOrn<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpOrni<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "orn " << DisasmGPR(rs1) << ", " << DisasmI(simm13) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpOrni<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;

		cpu.SetGPR(rd, cpu.GetGPR(rs1) | ~U32(simm13));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpOrni(CodeType code, uint32_t addr)
{
	return new OpOrni<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpOrncc<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "orncc " << DisasmGPR(rs1) << ", " << DisasmGPR(rs2) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpOrncc<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::S32 S32;
		typedef typename ARCH::BOOL BOOL;

		U32 result = cpu.GetGPR(rs1) | ~(cpu.GetGPR(rs2));
		cpu.SetNZVC( S32(result) < S32(0), result == U32(0), BOOL(false), BOOL(false) );
		cpu.SetGPR(rd, result);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpOrncc(CodeType code, uint32_t addr)
{
	return new OpOrncc<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpOrncci<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "orncc " << DisasmGPR(rs1) << ", " << DisasmI(simm13) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpOrncci<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::S32 S32;
		typedef typename ARCH::BOOL BOOL;

		U32 result = cpu.GetGPR(rs1) | ~U32(simm13);
		cpu.SetNZVC( S32(result) < S32(0), result == U32(0), BOOL(false), BOOL(false) );
		cpu.SetGPR(rd, result);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpOrncci(CodeType code, uint32_t addr)
{
	return new OpOrncci<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpXor_<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "xor " << DisasmGPR(rs1) << ", " << DisasmGPR(rs2) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpXor_<	ARCH>::execute( ARCH& cpu) const
{
	{
		cpu.SetGPR(rd, cpu.GetGPR(rs1) ^ cpu.GetGPR(rs2));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpXor_(CodeType code, uint32_t addr)
{
	return new OpXor_<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpXori<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "xor " << DisasmGPR(rs1) << ", " << DisasmI(simm13) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpXori<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;

		cpu.SetGPR(rd, cpu.GetGPR(rs1) ^ U32(simm13));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpXori(CodeType code, uint32_t addr)
{
	return new OpXori<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpXorcc<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "xorcc " << DisasmGPR(rs1) << ", " << DisasmGPR(rs2) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpXorcc<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::S32 S32;
		typedef typename ARCH::BOOL BOOL;

		U32 result = cpu.GetGPR(rs1) ^ cpu.GetGPR(rs2);
		cpu.SetNZVC( S32(result) < S32(0), result == U32(0), BOOL(false), BOOL(false) );
		cpu.SetGPR(rd, result);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpXorcc(CodeType code, uint32_t addr)
{
	return new OpXorcc<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpXorcci<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "xorcc " << DisasmGPR(rs1) << ", " << DisasmI(simm13) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpXorcci<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::S32 S32;
		typedef typename ARCH::BOOL BOOL;

		U32 result = cpu.GetGPR(rs1) ^ U32(simm13);
		cpu.SetNZVC( S32(result) < S32(0), result == U32(0), BOOL(false), BOOL(false) );
		cpu.SetGPR(rd, result);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpXorcci(CodeType code, uint32_t addr)
{
	return new OpXorcci<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpXnor<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "xnor " << DisasmGPR(rs1) << ", " << DisasmGPR(rs2) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpXnor<	ARCH>::execute( ARCH& cpu) const
{
	{
		cpu.SetGPR(rd, cpu.GetGPR(rs1) ^ ~(cpu.GetGPR(rs2)));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpXnor(CodeType code, uint32_t addr)
{
	return new OpXnor<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpXnori<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "xnor " << DisasmGPR(rs1) << ", " << DisasmI(simm13) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpXnori<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;

		cpu.SetGPR(rd, cpu.GetGPR(rs1) ^ ~U32(simm13));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpXnori(CodeType code, uint32_t addr)
{
	return new OpXnori<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpXnorcc<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "xnorcc " << DisasmGPR(rs1) << ", " << DisasmGPR(rs2) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpXnorcc<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::S32 S32;
		typedef typename ARCH::BOOL BOOL;

		U32 result = cpu.GetGPR(rs1) ^ ~(cpu.GetGPR(rs2));
		cpu.SetNZVC( S32(result) < S32(0), result == U32(0), BOOL(false), BOOL(false) );
		cpu.SetGPR(rd, result);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpXnorcc(CodeType code, uint32_t addr)
{
	return new OpXnorcc<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpXnorcci<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "xnorcc " << DisasmGPR(rs1) << ", " << DisasmI(simm13) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpXnorcci<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::S32 S32;
		typedef typename ARCH::BOOL BOOL;

		U32 result = cpu.GetGPR(rs1) ^ ~U32(simm13);
		cpu.SetNZVC( S32(result) < S32(0), result == U32(0), BOOL(false), BOOL(false) );
		cpu.SetGPR(rd, result);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpXnorcci(CodeType code, uint32_t addr)
{
	return new OpXnorcci<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpSll<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "sll " << DisasmGPR(rs1) << ", " << DisasmGPR(rs2) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpSll<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;

		cpu.SetGPR(rd, cpu.GetGPR(rs1) << (cpu.GetGPR(rs2) & U32(0x1f)));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpSll(CodeType code, uint32_t addr)
{
	return new OpSll<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpSlli<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "sll " << DisasmGPR(rs1) << ", " << DisasmI(shcnt) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpSlli<	ARCH>::execute( ARCH& cpu) const
{
	{
		cpu.SetGPR(rd, cpu.GetGPR(rs1) << (shcnt & 0x1f));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpSlli(CodeType code, uint32_t addr)
{
	return new OpSlli<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpSrl<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "srl " << DisasmGPR(rs1) << ", " << DisasmGPR(rs2) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpSrl<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;

		cpu.SetGPR(rd, cpu.GetGPR(rs1) >> (cpu.GetGPR(rs2) & U32(0x1f)));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpSrl(CodeType code, uint32_t addr)
{
	return new OpSrl<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpSrli<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "srl " << DisasmGPR(rs1) << ", " << DisasmI(shcnt) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpSrli<	ARCH>::execute( ARCH& cpu) const
{
	{
		cpu.SetGPR(rd, cpu.GetGPR(rs1) >> (shcnt & 0x1f));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpSrli(CodeType code, uint32_t addr)
{
	return new OpSrli<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpSra<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "sra " << DisasmGPR(rs1) << ", " << DisasmGPR(rs2) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpSra<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::S32 S32;

		cpu.SetGPR(rd, U32(S32( cpu.GetGPR(rs1) ) >> (cpu.GetGPR(rs2) & U32(0x1f))));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpSra(CodeType code, uint32_t addr)
{
	return new OpSra<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpSrai<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "sra " << DisasmGPR(rs1) << ", " << DisasmI(shcnt) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpSrai<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::S32 S32;

		cpu.SetGPR(rd, U32(S32( cpu.GetGPR(rs1) ) >> shcnt));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpSrai(CodeType code, uint32_t addr)
{
	return new OpSrai<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpAdd<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "add " << DisasmGPR(rs1) << ", " << DisasmGPR(rs2) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpAdd<	ARCH>::execute( ARCH& cpu) const
{
	{
		cpu.SetGPR(rd, cpu.GetGPR(rs1) + cpu.GetGPR(rs2));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpAdd(CodeType code, uint32_t addr)
{
	return new OpAdd<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpAddi<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "add " << DisasmGPR(rs1) << ", " << DisasmI(simm13) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpAddi<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;

		cpu.SetGPR(rd, cpu.GetGPR(rs1) + U32(simm13));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpAddi(CodeType code, uint32_t addr)
{
	return new OpAddi<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpAddcc<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "addcc " << DisasmGPR(rs1) << ", " << DisasmGPR(rs2) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpAddcc<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::S32 S32;
		typedef typename ARCH::BOOL BOOL;

		U32 op1( cpu.GetGPR(rs1) ), op2( cpu.GetGPR(rs2) ), res = op1 + op2;
		BOOL n = S32(res) < S32(0);
		cpu.SetNZVC( n, res == U32(0), n xor (S32(op1) <= S32(~op2)), op1 > ~op2 );
		cpu.SetGPR(rd, res);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpAddcc(CodeType code, uint32_t addr)
{
	return new OpAddcc<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpAddcci<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "addcc " << DisasmGPR(rs1) << ", " << DisasmI(simm13) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpAddcci<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::S32 S32;
		typedef typename ARCH::BOOL BOOL;

		U32 op1( cpu.GetGPR(rs1) ), op2( simm13 ), res = op1 + op2;
		BOOL n = S32(res) < S32(0);
		cpu.SetNZVC( n, res == U32(0), n xor (S32(op1) <= S32(~op2)), op1 > ~op2 );
		cpu.SetGPR(rd, res);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpAddcci(CodeType code, uint32_t addr)
{
	return new OpAddcci<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpAddx<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "addx " << DisasmGPR(rs1) << ", " << DisasmGPR(rs2) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpAddx<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;

		cpu.SetGPR(rd, cpu.GetGPR(rs1) + cpu.GetGPR(rs2) + U32(cpu.GetCarry()));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpAddx(CodeType code, uint32_t addr)
{
	return new OpAddx<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpAddxi<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "addx " << DisasmGPR(rs1) << ", " << DisasmI(simm13) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpAddxi<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;

		cpu.SetGPR(rd, cpu.GetGPR(rs1) + U32(simm13) + U32(cpu.GetCarry()));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpAddxi(CodeType code, uint32_t addr)
{
	return new OpAddxi<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpAddxcc<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "addxcc " << DisasmGPR(rs1) << ", " << DisasmGPR(rs2) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpAddxcc<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::S32 S32;
		typedef typename ARCH::BOOL BOOL;

		BOOL carry( cpu.GetCarry() );
		U32 op1( cpu.GetGPR(rs1) ), op2( cpu.GetGPR(rs2) ), res = op1 + op2 + U32(carry);

		BOOL n = S32(res) < S32(0), z = res == U32(0);
		if (cpu.Test(carry))
		cpu.SetNZVC( n, z, n xor (S32(op1) <  S32(~op2)), op1 >= ~op2 );
		else
		cpu.SetNZVC( n, z, n xor (S32(op1) <= S32(~op2)), op1 >  ~op2 );

		cpu.SetGPR(rd, res);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpAddxcc(CodeType code, uint32_t addr)
{
	return new OpAddxcc<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpAddxcci<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "addxcc " << DisasmGPR(rs1) << ", " << DisasmI(simm13) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpAddxcci<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::S32 S32;
		typedef typename ARCH::BOOL BOOL;

		BOOL carry( cpu.GetCarry() );
		U32 op1( cpu.GetGPR(rs1) ), op2( simm13 ), res = op1 + op2 + U32(carry);

		BOOL n = S32(res) < S32(0), z = res == U32(0);
		if (cpu.Test(carry))
		cpu.SetNZVC( n, z, n xor (S32(op1) <  S32(~op2)), op1 >= ~op2 );
		else
		cpu.SetNZVC( n, z, n xor (S32(op1) <= S32(~op2)), op1 >  ~op2 );

		cpu.SetGPR(rd, res);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpAddxcci(CodeType code, uint32_t addr)
{
	return new OpAddxcci<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpTaddcc<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "taddcc " << DisasmGPR(rs1) << ", " << DisasmGPR(rs2) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpTaddcc<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::S32 S32;
		typedef typename ARCH::BOOL BOOL;

		U32 op1( cpu.GetGPR(rs1) ), op2( cpu.GetGPR(rs2) ), res = op1 + op2;

		BOOL n = S32(res) < S32(0), overflow = n xor (S32(op1) <= S32(~op2));
		overflow |= BOOL((op1 | op2) & U32(3));
		cpu.SetNZVC( n, res == U32(0), overflow, op1 > ~op2 );

		cpu.SetGPR(rd, res);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpTaddcc(CodeType code, uint32_t addr)
{
	return new OpTaddcc<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpTaddcci<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "taddcc " << DisasmGPR(rs1) << ", " << DisasmI(simm13) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpTaddcci<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::S32 S32;
		typedef typename ARCH::BOOL BOOL;

		U32 op1( cpu.GetGPR(rs1) ), op2( simm13 ), res = op1 + op2;

		BOOL n = S32(res) < S32(0), overflow = n xor (S32(op1) <= S32(~op2));
		overflow |= BOOL((op1 | op2) & U32(3));
		cpu.SetNZVC( n, res == U32(0), overflow, op1 > ~op2 );

		cpu.SetGPR(rd, res);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpTaddcci(CodeType code, uint32_t addr)
{
	return new OpTaddcci<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpTaddcctv<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "taddcctv " << DisasmGPR(rs1) << ", " << DisasmGPR(rs2) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpTaddcctv<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::S32 S32;
		typedef typename ARCH::BOOL BOOL;

		U32 op1( cpu.GetGPR(rs1) ), op2( cpu.GetGPR(rs2) ), res = op1 + op2;

		BOOL n = S32(res) < S32(0), overflow = n xor (S32(op1) <= S32(~op2));
		overflow |= BOOL((op1 | op2) & U32(3));
		if (cpu.Test(overflow))
		cpu.Abort( Trap_t::tag_overflow );
		cpu.SetNZVC( n, res == U32(0), overflow, op1 > ~op2 );

		cpu.SetGPR(rd, res);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpTaddcctv(CodeType code, uint32_t addr)
{
	return new OpTaddcctv<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpTaddcctvi<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "taddcctv " << DisasmGPR(rs1) << ", " << DisasmI(simm13) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpTaddcctvi<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::S32 S32;
		typedef typename ARCH::BOOL BOOL;

		U32 op1( cpu.GetGPR(rs1) ), op2( simm13 ), res = op1 + op2;

		BOOL n = S32(res) < S32(0), overflow = n xor (S32(op1) <= S32(~op2));
		overflow |= BOOL((op1 | op2) & U32(3));
		if (cpu.Test(overflow))
		cpu.Abort( Trap_t::tag_overflow );
		cpu.SetNZVC( n, res == U32(0), overflow, op1 > ~op2 );

		cpu.SetGPR(rd, res);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpTaddcctvi(CodeType code, uint32_t addr)
{
	return new OpTaddcctvi<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpSub<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "sub " << DisasmGPR(rs1) << ", " << DisasmGPR(rs2) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpSub<	ARCH>::execute( ARCH& cpu) const
{
	{
		cpu.SetGPR(rd, cpu.GetGPR(rs1) - cpu.GetGPR(rs2));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpSub(CodeType code, uint32_t addr)
{
	return new OpSub<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpSubi<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "sub " << DisasmGPR(rs1) << ", " << DisasmI(simm13) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpSubi<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;

		cpu.SetGPR(rd, cpu.GetGPR(rs1) - U32(simm13));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpSubi(CodeType code, uint32_t addr)
{
	return new OpSubi<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpSubcc<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		if (rd == 0) {
			sink << "cmp " << DisasmGPR(rs1) << ", " << DisasmGPR(rs2);
			} else {
			sink << "subcc " << DisasmGPR(rs1) << ", " << DisasmGPR(rs2) << ", " << DisasmGPR(rd);
		}
}}
template <	typename	ARCH>
void OpSubcc<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::S32 S32;
		typedef typename ARCH::BOOL BOOL;

		U32 op1( cpu.GetGPR(rs1) ), op2( cpu.GetGPR(rs2) ), res = op1 - op2;

		BOOL n = S32(res) < S32(0);
		cpu.SetNZVC( n, res == U32(0), n xor (S32(op1) < S32(op2)), op1 < op2 );
		cpu.SetGPR(rd, res);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpSubcc(CodeType code, uint32_t addr)
{
	return new OpSubcc<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpSubcci<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		if (rd == 0) {
			sink << "cmp " << DisasmGPR(rs1) << ", " << DisasmI(simm13);
			} else {
			sink << "subcc " << DisasmGPR(rs1) << ", " << DisasmI(simm13) << ", " << DisasmGPR(rd);
		}
}}
template <	typename	ARCH>
void OpSubcci<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::S32 S32;
		typedef typename ARCH::BOOL BOOL;

		U32 op1( cpu.GetGPR(rs1) ), op2( simm13 ), res = op1 - op2;

		BOOL n = S32(res) < S32(0);
		cpu.SetNZVC( n, res == U32(0), n xor (S32(op1) < S32(op2)), op1 < op2 );
		cpu.SetGPR(rd, res);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpSubcci(CodeType code, uint32_t addr)
{
	return new OpSubcci<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpSubx<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "subx " << DisasmGPR(rs1) << ", " << DisasmGPR(rs2) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpSubx<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;

		cpu.SetGPR(rd, cpu.GetGPR(rs1) - cpu.GetGPR(rs2) - U32(cpu.GetCarry()));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpSubx(CodeType code, uint32_t addr)
{
	return new OpSubx<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpSubxi<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "subx " << DisasmGPR(rs1) << ", " << DisasmI(simm13) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpSubxi<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;

		cpu.SetGPR(rd, cpu.GetGPR(rs1) - U32(simm13) - U32(cpu.GetCarry()));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpSubxi(CodeType code, uint32_t addr)
{
	return new OpSubxi<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpSubxcc<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "subxcc " << DisasmGPR(rs1) << ", " << DisasmGPR(rs2) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpSubxcc<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::S32 S32;
		typedef typename ARCH::BOOL BOOL;

		BOOL borrow( cpu.GetCarry() );
		U32 op1( cpu.GetGPR(rs1) ), op2( cpu.GetGPR(rs2) ), res = op1 - op2 - U32(borrow);

		BOOL n = S32(res) < S32(0), z = res == U32(0);
		if (cpu.Test(borrow))
		cpu.SetNZVC( n, z, n xor (S32(op1) <= S32(op2)), op1 <=  op2 );
		else
		cpu.SetNZVC( n, z, n xor (S32(op1) <  S32(op2)), op1 < op2 );

		cpu.SetGPR(rd, res);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpSubxcc(CodeType code, uint32_t addr)
{
	return new OpSubxcc<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpSubxcci<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "subxcc " << DisasmGPR(rs1) << ", " << DisasmI(simm13) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpSubxcci<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::S32 S32;
		typedef typename ARCH::BOOL BOOL;

		BOOL borrow( cpu.GetCarry() );
		U32 op1( cpu.GetGPR(rs1) ), op2( simm13 ), res = op1 - op2 - U32(borrow);

		BOOL n = S32(res) < S32(0), z = res == U32(0);
		if (cpu.Test(borrow))
		cpu.SetNZVC( n, z, n xor (S32(op1) <= S32(op2)), op1 <=  op2 );
		else
		cpu.SetNZVC( n, z, n xor (S32(op1) <  S32(op2)), op1 < op2 );

		cpu.SetGPR(rd, res);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpSubxcci(CodeType code, uint32_t addr)
{
	return new OpSubxcci<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpTsubcc<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "tsubcc " << DisasmGPR(rs1) << ", " << DisasmGPR(rs2) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpTsubcc<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::S32 S32;
		typedef typename ARCH::BOOL BOOL;

		U32 op1( cpu.GetGPR(rs1) ), op2( cpu.GetGPR(rs2) ), res = op1 - op2;

		BOOL n = S32(res) < S32(0), overflow = n xor (S32(op1) < S32(op2));
		overflow |= BOOL((op1 | op2) & U32(3));
		cpu.SetNZVC( n, res == U32(0), overflow, op1 < op2 );

		cpu.SetGPR(rd, res);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpTsubcc(CodeType code, uint32_t addr)
{
	return new OpTsubcc<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpTsubcci<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "tsubcc " << DisasmGPR(rs1) << ", " << DisasmI(simm13) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpTsubcci<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::S32 S32;
		typedef typename ARCH::BOOL BOOL;

		U32 op1( cpu.GetGPR(rs1) ), op2( simm13 ), res = op1 - op2;

		BOOL n = S32(res) < S32(0), overflow = n xor (S32(op1) < S32(op2));
		overflow |= BOOL((op1 | op2) & U32(3));
		cpu.SetNZVC( n, res == U32(0), overflow, op1 < op2 );

		cpu.SetGPR(rd, res);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpTsubcci(CodeType code, uint32_t addr)
{
	return new OpTsubcci<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpTsubcctv<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "tsubcctv " << DisasmGPR(rs1) << ", " << DisasmGPR(rs2) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpTsubcctv<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::S32 S32;
		typedef typename ARCH::BOOL BOOL;

		U32 op1( cpu.GetGPR(rs1) ), op2( cpu.GetGPR(rs2) ), res = op1 - op2;

		BOOL n = S32(res) < S32(0), overflow = n xor (S32(op1) < S32(op2));
		overflow |= BOOL((op1 | op2) & U32(3));
		if (cpu.Test(overflow))
		cpu.Abort( Trap_t::tag_overflow );
		cpu.SetNZVC( n, res == U32(0), overflow, op1 < op2 );

		cpu.SetGPR(rd, res);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpTsubcctv(CodeType code, uint32_t addr)
{
	return new OpTsubcctv<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpTsubcctvi<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "tsubcctv " << DisasmGPR(rs1) << ", " << DisasmI(simm13) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpTsubcctvi<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::S32 S32;
		typedef typename ARCH::BOOL BOOL;

		U32 op1( cpu.GetGPR(rs1) ), op2( simm13 ), res = op1 - op2;

		BOOL n = S32(res) < S32(0), overflow = n xor (S32(op1) < S32(op2));
		overflow |= BOOL((op1 | op2) & U32(3));
		if (cpu.Test(overflow))
		cpu.Abort( Trap_t::tag_overflow );
		cpu.SetNZVC( n, res == U32(0), overflow, op1 < op2 );

		cpu.SetGPR(rd, res);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpTsubcctvi(CodeType code, uint32_t addr)
{
	return new OpTsubcctvi<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpMulscc<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "mulscc " << DisasmGPR(rs1) << ", " << DisasmGPR(rs2) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpMulscc<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::S32 S32;
		typedef typename ARCH::BOOL BOOL;

		U32 op1( (U32(cpu.GetN() ^ cpu.GetV()) << 31) | (cpu.GetGPR(rs1) >> 1) ),
		op2( cpu.Test(cpu.GetY() & U32(1)) ? cpu.GetGPR(rs2) : U32(0) ),
		res = op1 + op2;

		BOOL n = S32(res) < S32(0);
		cpu.SetNZVC( n, res == U32(0), n xor (S32(op1) <= S32(~op2)), op1 > ~op2 );

		cpu.SetY( (cpu.GetGPR(rs1) << 31) | (cpu.GetY() >> 1) );
		cpu.SetGPR(rd, res);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpMulscc(CodeType code, uint32_t addr)
{
	return new OpMulscc<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpMulscci<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "mulscc " << DisasmGPR(rs1) << ", " << DisasmI(simm13) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpMulscci<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::S32 S32;
		typedef typename ARCH::BOOL BOOL;

		U32 op1( (U32(cpu.GetN() ^ cpu.GetV()) << 31) | (cpu.GetGPR(rs1) >> 1) ),
		op2( cpu.Test(cpu.GetY() & U32(1)) ? simm13 : 0 ),
		res = op1 + op2;

		BOOL n = S32(res) < S32(0);
		cpu.SetNZVC( n, res == U32(0), n xor (S32(op1) <= S32(~op2)), op1 > ~op2 );

		cpu.SetY( (cpu.GetGPR(rs1) << 31) | (cpu.GetY() >> 1) );
		cpu.SetGPR(rd, res);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpMulscci(CodeType code, uint32_t addr)
{
	return new OpMulscci<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpUmul<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "umul " << DisasmGPR(rs1) << ", " << DisasmGPR(rs2) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpUmul<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::U64 U64;

		U64 op1( U32( cpu.GetGPR(rs1) ) ), op2( U32( cpu.GetGPR(rs2) ) ), res = op1 * op2;
		cpu.SetY( U32(res >> 32) );
		cpu.SetGPR(rd, U32(res));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpUmul(CodeType code, uint32_t addr)
{
	return new OpUmul<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpUmuli<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "umul " << DisasmGPR(rs1) << ", " << DisasmI(unsigned(simm13)) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpUmuli<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::U64 U64;

		U64 op1( U32( cpu.GetGPR(rs1) ) ), op2 = U64( U32(simm13) ), res = op1 * op2;
		cpu.SetY( U32(res >> 32) );
		cpu.SetGPR(rd, U32(res));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpUmuli(CodeType code, uint32_t addr)
{
	return new OpUmuli<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpSmul<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "smul " << DisasmGPR(rs1) << ", " << DisasmGPR(rs2) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpSmul<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::S32 S32;
		typedef typename ARCH::S64 S64;

		S64 op1( S32( cpu.GetGPR(rs1) ) ), op2( S32( cpu.GetGPR(rs2) ) ), res = op1 * op2;

		cpu.SetY( U32(res >> 32) );
		cpu.SetGPR(rd, U32(res));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpSmul(CodeType code, uint32_t addr)
{
	return new OpSmul<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpSmuli<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "smul " << DisasmGPR(rs1) << ", " << DisasmI(simm13) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpSmuli<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::S32 S32;
		typedef typename ARCH::S64 S64;

		S64 op1( S32( cpu.GetGPR(rs1) ) ), op2 = S64( S32( simm13 ) ), res = op1 * op2;

		cpu.SetY( U32(res >> 32) );
		cpu.SetGPR(rd, U32(res));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpSmuli(CodeType code, uint32_t addr)
{
	return new OpSmuli<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpUmulcc<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "umulcc " << DisasmGPR(rs1) << ", " << DisasmGPR(rs2) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpUmulcc<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::U64 U64;
		typedef typename ARCH::S32 S32;
		typedef typename ARCH::BOOL BOOL;

		U64 op1( U32( cpu.GetGPR(rs1) ) ), op2( U32( cpu.GetGPR(rs2) ) ), res = op1 * op2;

		cpu.SetNZVC( S32(res) < S32(0), U32(res) == U32(0), BOOL(false), BOOL(false) );

		cpu.SetY( U32(res >> 32) );
		cpu.SetGPR(rd, U32(res));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpUmulcc(CodeType code, uint32_t addr)
{
	return new OpUmulcc<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpUmulcci<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "umulcc " << DisasmGPR(rs1) << ", " << DisasmI(unsigned(simm13)) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpUmulcci<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::U64 U64;
		typedef typename ARCH::S32 S32;
		typedef typename ARCH::BOOL BOOL;

		U64 op1( U32( cpu.GetGPR(rs1) ) ), op2 = U64( U32( simm13) ), res = op1 * op2;

		cpu.SetNZVC( S32(res) < S32(0), U32(res) == U32(0), BOOL(false), BOOL(false) );

		cpu.SetY( U32(res >> 32) );
		cpu.SetGPR(rd, U32(res));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpUmulcci(CodeType code, uint32_t addr)
{
	return new OpUmulcci<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpSmulcc<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "smulcc " << DisasmGPR(rs1) << ", " << DisasmGPR(rs2) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpSmulcc<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::S32 S32;
		typedef typename ARCH::S64 S64;
		typedef typename ARCH::BOOL BOOL;

		S64 op1( S32( cpu.GetGPR(rs1) ) ), op2( S32( cpu.GetGPR(rs2) ) ), res = op1 * op2;

		cpu.SetNZVC( S32(res) < S32(0), U32(res) == U32(0), BOOL(false), BOOL(false) );

		cpu.SetY( U32(res >> 32) );
		cpu.SetGPR(rd, U32(res));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpSmulcc(CodeType code, uint32_t addr)
{
	return new OpSmulcc<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpSmulcci<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "smulcc " << DisasmGPR(rs1) << ", " << DisasmI(simm13) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpSmulcci<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::S32 S32;
		typedef typename ARCH::S64 S64;
		typedef typename ARCH::BOOL BOOL;

		S64 op1( S32( cpu.GetGPR(rs1) ) ), op2( simm13 ), res = op1 * op2;

		cpu.SetNZVC( S32(res) < S32(0), U32(res) == U32(0), BOOL(false), BOOL(false) );

		cpu.SetY( U32(res >> 32) );
		cpu.SetGPR(rd, U32(res));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpSmulcci(CodeType code, uint32_t addr)
{
	return new OpSmulcci<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpUdiv<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "udiv " << DisasmGPR(rs1) << ", " << DisasmGPR(rs2) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpUdiv<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::U64 U64;
		typedef typename ARCH::BOOL BOOL;

		if (cpu.Test(cpu.GetGPR(rs2) == U32(0)))
		cpu.Abort( Trap_t::division_by_zero );

		U64 op1( U64( cpu.GetY() ) << 32 | U64(cpu.GetGPR(rs1)) ), op2( cpu.GetGPR(rs2) ), res64 = op1 / op2;

		BOOL overflow(U64(U32(res64)) != res64);
		U32 res = cpu.Test(overflow) ? U32(0xffffffff) : U32(res64);

		cpu.SetGPR(rd, res);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpUdiv(CodeType code, uint32_t addr)
{
	return new OpUdiv<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpUdivi<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "udiv " << DisasmGPR(rs1) << ", " << DisasmI(simm13) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpUdivi<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::U64 U64;
		typedef typename ARCH::BOOL BOOL;

		if (simm13 == 0)
		cpu.Abort( Trap_t::division_by_zero );

		U64 op1( U64( cpu.GetY() ) << 32 | U64(cpu.GetGPR(rs1)) ), op2 = U64( U32( simm13 ) ), res64 = op1 / op2;

		BOOL overflow(U64(U32(res64)) != res64);
		U32 res = cpu.Test(overflow) ? U32(0xffffffff) : U32(res64);

		cpu.SetGPR(rd, res);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpUdivi(CodeType code, uint32_t addr)
{
	return new OpUdivi<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpSdiv<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "sdiv " << DisasmGPR(rs1) << ", " << DisasmGPR(rs2) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpSdiv<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::U64 U64;
		typedef typename ARCH::S32 S32;
		typedef typename ARCH::S64 S64;
		typedef typename ARCH::BOOL BOOL;

		if (cpu.Test(cpu.GetGPR(rs2) == U32(0)))
		cpu.Abort( Trap_t::division_by_zero );

		S64 op1( U64( cpu.GetY() ) << 32 | U64( cpu.GetGPR(rs1) ) ), op2( S32(cpu.GetGPR(rs2)) ), res64 = op1 / op2;

		BOOL overflow(S64(S32(res64)) != res64);
		U32 res = cpu.Test(overflow) ? (U32(res64 >> 63) ^ U32(0x7fffffff)) : U32(res64);

		cpu.SetGPR(rd, res);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpSdiv(CodeType code, uint32_t addr)
{
	return new OpSdiv<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpSdivi<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "sdiv " << DisasmGPR(rs1) << ", " << DisasmI(simm13) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpSdivi<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::U64 U64;
		typedef typename ARCH::S32 S32;
		typedef typename ARCH::S64 S64;
		typedef typename ARCH::BOOL BOOL;

		if (simm13 == 0)
		cpu.Abort( Trap_t::division_by_zero );

		S64 op1( U64( cpu.GetY() ) << 32 | U64( cpu.GetGPR(rs1) ) ), op2( simm13 ), res64 = op1 / op2;

		BOOL overflow(S64(S32(res64)) != res64);
		U32 res = cpu.Test(overflow) ? (U32(res64 >> 63) ^ U32(0x7fffffff)) : U32(res64);

		cpu.SetGPR(rd, res);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpSdivi(CodeType code, uint32_t addr)
{
	return new OpSdivi<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpUdivcc<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "udivcc " << DisasmGPR(rs1) << ", " << DisasmGPR(rs2) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpUdivcc<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::U64 U64;
		typedef typename ARCH::S32 S32;
		typedef typename ARCH::BOOL BOOL;

		if (cpu.Test(cpu.GetGPR(rs2) == U32(0)))
		cpu.Abort( Trap_t::division_by_zero );

		U64 op1( U64( cpu.GetY() ) << 32 | U64(cpu.GetGPR(rs1)) ), op2( cpu.GetGPR(rs2) ), res64 = op1 / op2;

		BOOL overflow(U64(U32(res64)) != res64);
		U32 res = cpu.Test(overflow) ? U32(0xffffffff) : U32(res64);

		cpu.SetNZVC( S32(res) < S32(0), U32(res) == U32(0), overflow, BOOL(false) );

		cpu.SetGPR(rd, res);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpUdivcc(CodeType code, uint32_t addr)
{
	return new OpUdivcc<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpUdivcci<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "udivcc " << DisasmGPR(rs1) << ", " << DisasmI(simm13) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpUdivcci<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::U64 U64;
		typedef typename ARCH::S32 S32;
		typedef typename ARCH::BOOL BOOL;

		if (simm13 == 0)
		cpu.Abort( Trap_t::division_by_zero );

		U64 op1( U64( cpu.GetY() ) << 32 | U64(cpu.GetGPR(rs1)) ), op2 = U64( U32( simm13 ) ), res64 = op1 / op2;

		BOOL overflow(U64(U32(res64)) != res64);
		U32 res = cpu.Test(overflow) ? U32(0xffffffff) : U32(res64);

		cpu.SetNZVC( S32(res) < S32(0), U32(res) == U32(0), overflow, BOOL(false) );

		cpu.SetGPR(rd, res);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpUdivcci(CodeType code, uint32_t addr)
{
	return new OpUdivcci<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpSdivcc<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "sdivcc " << DisasmGPR(rs1) << ", " << DisasmGPR(rs2) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpSdivcc<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::U64 U64;
		typedef typename ARCH::S32 S32;
		typedef typename ARCH::S64 S64;
		typedef typename ARCH::BOOL BOOL;

		if (cpu.Test(cpu.GetGPR(rs2) == U32(0)))
		cpu.Abort( Trap_t::division_by_zero );

		S64 op1( U64( cpu.GetY() ) << 32 | U64( cpu.GetGPR(rs1) ) ), op2( S32(cpu.GetGPR(rs2)) ), res64 = op1 / op2;

		BOOL overflow(S64(S32(res64)) != res64);
		U32 res = cpu.Test(overflow) ? (U32(res64 >> 63) ^ U32(0x7fffffff)) : U32(res64);

		cpu.SetNZVC( S32(res) < S32(0), U32(res) == U32(0), overflow, BOOL(false) );

		cpu.SetGPR(rd, res);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpSdivcc(CodeType code, uint32_t addr)
{
	return new OpSdivcc<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpSdivcci<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "sdivcc " << DisasmGPR(rs1) << ", " << DisasmI(simm13) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpSdivcci<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::U64 U64;
		typedef typename ARCH::S32 S32;
		typedef typename ARCH::S64 S64;
		typedef typename ARCH::BOOL BOOL;

		if (simm13 == 0)
		cpu.Abort( Trap_t::division_by_zero );

		S64 op1( U64( cpu.GetY() ) << 32 | U64( cpu.GetGPR(rs1) ) ), op2( simm13 ), res64 = op1 / op2;

		BOOL overflow(S64(S32(res64)) != res64);
		U32 res = cpu.Test(overflow) ? (U32(res64 >> 63) ^ U32(0x7fffffff)) : U32(res64);

		cpu.SetNZVC( S32(res) < S32(0), U32(res) == U32(0), overflow, BOOL(false) );

		cpu.SetGPR(rd, res);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpSdivcci(CodeType code, uint32_t addr)
{
	return new OpSdivcci<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpSave<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "save " << DisasmGPR(rs1) << ", " << DisasmGPR(rs2) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpSave<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;

		if (cpu.InvalidWindow( -1 ))
		cpu.Abort( Trap_t::window_overflow );

		U32 result = cpu.GetGPR(rs1) + cpu.GetGPR(rs2);
		cpu.RotateWindow( -1 );
		cpu.SetGPR(rd, result);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpSave(CodeType code, uint32_t addr)
{
	return new OpSave<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpSavei<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "save " << DisasmRIAddress(rs1, simm13) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpSavei<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;

		if (cpu.InvalidWindow( -1 ))
		cpu.Abort( Trap_t::window_overflow );

		U32 result = cpu.GetGPR(rs1) + U32(simm13);
		cpu.RotateWindow( -1 );
		cpu.SetGPR(rd, result);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpSavei(CodeType code, uint32_t addr)
{
	return new OpSavei<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpRestore<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "restore " << DisasmGPR(rs1) << ", " << DisasmGPR(rs2) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpRestore<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;

		if (cpu.InvalidWindow( +1 ))
		cpu.Abort( Trap_t::window_underflow );

		U32 result = cpu.GetGPR(rs1) + cpu.GetGPR(rs2);
		cpu.RotateWindow( +1 );
		cpu.SetGPR(rd, result);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpRestore(CodeType code, uint32_t addr)
{
	return new OpRestore<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpRestorei<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "restore " << DisasmGPR(rs1) << ", " << DisasmHex(simm13,"-") << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpRestorei<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;

		if (cpu.InvalidWindow( +1 ))
		cpu.Abort( Trap_t::window_underflow );

		U32 result = cpu.GetGPR(rs1) + U32(simm13);
		cpu.RotateWindow( +1 );
		cpu.SetGPR(rd, result);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpRestorei(CodeType code, uint32_t addr)
{
	return new OpRestorei<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpBa<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "ba" << (annul ? ",a " : " ") << "" << DisasmHex(pc + disp22);
}}
template <	typename	ARCH>
void OpBa<	ARCH>::execute( ARCH& cpu) const
{
	{
		cpu.DelayBranch(cpu.GetPC() + typename ARCH::U32(disp22), ARCH::B_JMP);
		cpu.SetAnnul(annul);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpBa(CodeType code, uint32_t addr)
{
	return new OpBa<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpFba<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "fba" << (annul ? ",a" : "") << " " << "" << DisasmHex(pc + disp22);
}}
template <	typename	ARCH>
void OpFba<	ARCH>::execute( ARCH& cpu) const
{
	{
		cpu.DelayBranch(cpu.GetPC() + typename ARCH::U32(disp22), ARCH::B_JMP);
		cpu.SetAnnul(annul);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpFba(CodeType code, uint32_t addr)
{
	return new OpFba<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpCba<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "cba" << (annul ? ",a" : "") << " " << "" << DisasmHex(pc + disp22);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpCba(CodeType code, uint32_t addr)
{
	return new OpCba<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpCbn<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "cbn" << (annul ? ",a" : "") << " " << "" << DisasmHex(pc + disp22);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpCbn(CodeType code, uint32_t addr)
{
	return new OpCbn<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpCb3<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "cb3" << (annul ? ",a" : "") << " " << "" << DisasmHex(pc + disp22);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpCb3(CodeType code, uint32_t addr)
{
	return new OpCb3<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpCb2<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "cb2" << (annul ? ",a" : "") << " " << "" << DisasmHex(pc + disp22);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpCb2(CodeType code, uint32_t addr)
{
	return new OpCb2<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpCb23<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "cb23" << (annul ? ",a" : "") << " " << "" << DisasmHex(pc + disp22);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpCb23(CodeType code, uint32_t addr)
{
	return new OpCb23<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpCb1<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "cb1" << (annul ? ",a" : "") << " " << "" << DisasmHex(pc + disp22);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpCb1(CodeType code, uint32_t addr)
{
	return new OpCb1<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpCb13<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "cb13" << (annul ? ",a" : "") << " " << "" << DisasmHex(pc + disp22);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpCb13(CodeType code, uint32_t addr)
{
	return new OpCb13<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpCb12<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "cb12" << (annul ? ",a" : "") << " " << "" << DisasmHex(pc + disp22);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpCb12(CodeType code, uint32_t addr)
{
	return new OpCb12<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpCb123<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "cb123" << (annul ? ",a" : "") << " " << "" << DisasmHex(pc + disp22);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpCb123(CodeType code, uint32_t addr)
{
	return new OpCb123<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpCb0<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "cb0" << (annul ? ",a" : "") << " " << "" << DisasmHex(pc + disp22);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpCb0(CodeType code, uint32_t addr)
{
	return new OpCb0<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpCb03<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "cb03" << (annul ? ",a" : "") << " " << "" << DisasmHex(pc + disp22);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpCb03(CodeType code, uint32_t addr)
{
	return new OpCb03<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpCb02<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "cb02" << (annul ? ",a" : "") << " " << "" << DisasmHex(pc + disp22);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpCb02(CodeType code, uint32_t addr)
{
	return new OpCb02<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpCb023<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "cb023" << (annul ? ",a" : "") << " " << "" << DisasmHex(pc + disp22);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpCb023(CodeType code, uint32_t addr)
{
	return new OpCb023<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpCb01<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "cb01" << (annul ? ",a" : "") << " " << "" << DisasmHex(pc + disp22);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpCb01(CodeType code, uint32_t addr)
{
	return new OpCb01<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpCb013<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "cb013" << (annul ? ",a" : "") << " " << "" << DisasmHex(pc + disp22);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpCb013(CodeType code, uint32_t addr)
{
	return new OpCb013<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpCb012<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "cb012" << (annul ? ",a" : "") << " " << "" << DisasmHex(pc + disp22);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpCb012(CodeType code, uint32_t addr)
{
	return new OpCb012<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpCall<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "call " << "" << DisasmHex(disp30);
}}
template <	typename	ARCH>
void OpCall<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;

		cpu.DelayBranch( cpu.GetPC() + U32(disp30), ARCH::B_CALL );
		cpu.SetGPR(15, cpu.GetPC());
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpCall(CodeType code, uint32_t addr)
{
	return new OpCall<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpJmpl<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "jmpl " << DisasmGPR(rs1) << " + " << DisasmGPR(rs2) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpJmpl<	ARCH>::execute( ARCH& cpu) const
{
	{
		cpu.DelayBranch( cpu.GetGPR(rs1) + cpu.GetGPR(rs2), rd == 15 ? ARCH::B_CALL : ARCH::B_JMP );

		cpu.SetGPR(rd, cpu.GetPC());
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpJmpl(CodeType code, uint32_t addr)
{
	return new OpJmpl<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpJmpli<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "jmpl " << DisasmRIAddress(rs1, simm13) << ", " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpJmpli<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;

		cpu.DelayBranch(cpu.GetGPR(rs1) + U32(simm13), rd == 15 ? ARCH::B_CALL : ARCH::B_JMP);
		cpu.SetGPR(rd, cpu.GetPC());
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpJmpli(CodeType code, uint32_t addr)
{
	return new OpJmpli<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpRett<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "rett " << DisasmGPR(rs1) << " + " << DisasmGPR(rs2);
}}
template <	typename	ARCH>
void OpRett<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::BOOL BOOL;

		if (not cpu.IsSuper())
		cpu.Abort( Trap_t::privileged_instruction );

		if (cpu.Test(cpu.GetET()))
		cpu.Abort( Trap_t::illegal_instruction );

		if (cpu.InvalidWindow( +1 ))
		{
			cpu.StopFetch();
			cpu.Abort( Trap_t::window_underflow );
		}

		U32 address = cpu.GetGPR(rs1) + cpu.GetGPR(rs2);
		if (cpu.Test(address & U32(0x3)))
		{
			cpu.StopFetch();
			cpu.Abort( Trap_t::mem_address_not_aligned );
		}

		cpu.SetET( BOOL(true) );
		cpu.DelayBranch( address, ARCH::B_JMP );
		cpu.RotateWindow( +1 );
		cpu.SetS( cpu.GetPS() );
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpRett(CodeType code, uint32_t addr)
{
	return new OpRett<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpRetti<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "rett " << DisasmRIAddress(rs1, simm13);
}}
template <	typename	ARCH>
void OpRetti<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;
		typedef typename ARCH::BOOL BOOL;

		if (not cpu.IsSuper())
		cpu.Abort( Trap_t::privileged_instruction );

		if (cpu.Test(cpu.GetET()))
		cpu.Abort( Trap_t::illegal_instruction );

		if (cpu.InvalidWindow( +1 ))
		{
			cpu.StopFetch();
			cpu.Abort( Trap_t::window_underflow );
		}

		U32 address = cpu.GetGPR(rs1) + U32(simm13);
		if (cpu.Test(address & U32(0x3)))
		{
			cpu.StopFetch();
			cpu.Abort( Trap_t::mem_address_not_aligned );
		}

		cpu.SetET( BOOL(true) );
		cpu.DelayBranch( address, ARCH::B_JMP );
		cpu.RotateWindow( +1 );
		cpu.SetS( cpu.GetPS() );
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpRetti(CodeType code, uint32_t addr)
{
	return new OpRetti<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpTa<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "ta " << DisasmGPR(rs1) << " + " << DisasmGPR(rs2);
}}
template <	typename	ARCH>
void OpTa<	ARCH>::execute( ARCH& cpu) const
{
	{
		cpu.SWTrap( cpu.GetGPR(rs1) + cpu.GetGPR(rs2) );
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpTa(CodeType code, uint32_t addr)
{
	return new OpTa<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpTai<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "ta " << DisasmGPR(rs1) << " + " << "" << DisasmHex(imm7);
}}
template <	typename	ARCH>
void OpTai<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;

		cpu.SWTrap( cpu.GetGPR(rs1) + U32(imm7) );
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpTai(CodeType code, uint32_t addr)
{
	return new OpTai<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpRdy<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "rd %y, " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpRdy<	ARCH>::execute( ARCH& cpu) const
{
	{
		cpu.SetGPR(rd, cpu.GetY());
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpRdy(CodeType code, uint32_t addr)
{
	return new OpRdy<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpStbar<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "stbar";
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpStbar(CodeType code, uint32_t addr)
{
	return new OpStbar<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpRdpsr<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "rd %psr, " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpRdpsr<	ARCH>::execute( ARCH& cpu) const
{
	{
		if (not cpu.IsSuper())
		cpu.Abort( Trap_t::privileged_instruction );

		cpu.SetGPR(rd, cpu.GetPSR());
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpRdpsr(CodeType code, uint32_t addr)
{
	return new OpRdpsr<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpRdwim<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "rd %wim, " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpRdwim<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;

		if (not cpu.IsSuper())
		cpu.Abort( Trap_t::privileged_instruction );

		cpu.SetGPR(rd, cpu.GetWIM() & U32((uint64_t( 1 ) << cpu.nwindows())-1));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpRdwim(CodeType code, uint32_t addr)
{
	return new OpRdwim<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpRdtbr<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "rd %tbr, " << DisasmGPR(rd);
}}
template <	typename	ARCH>
void OpRdtbr<	ARCH>::execute( ARCH& cpu) const
{
	{
		if (not cpu.IsSuper())
		cpu.Abort( Trap_t::privileged_instruction );

		cpu.SetGPR(rd, cpu.GetTBR());
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpRdtbr(CodeType code, uint32_t addr)
{
	return new OpRdtbr<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpWry<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "wr " << DisasmGPR(rs1) << ", " << DisasmGPR(rs2) << ", %y";
}}
template <	typename	ARCH>
void OpWry<	ARCH>::execute( ARCH& cpu) const
{
	{
		cpu.SetY( cpu.GetGPR(rs1) ^ cpu.GetGPR(rs2) );
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpWry(CodeType code, uint32_t addr)
{
	return new OpWry<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpWryi<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "wr " << DisasmGPR(rs1) << ", " << DisasmI(simm13) << ", %y";
}}
template <	typename	ARCH>
void OpWryi<	ARCH>::execute( ARCH& cpu) const
{
	{
		cpu.SetY( cpu.GetGPR(rs1) ^ typename ARCH::U32(simm13) );
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpWryi(CodeType code, uint32_t addr)
{
	return new OpWryi<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpWrpsr<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "wr " << DisasmGPR(rs1) << ", " << DisasmGPR(rs2) << ", %psr";
}}
template <	typename	ARCH>
void OpWrpsr<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;

		if (not cpu.IsSuper())
		cpu.Abort( Trap_t::privileged_instruction );

		U32 res = cpu.GetGPR(rs1) ^ cpu.GetGPR(rs2);

		if (cpu.Test((res & U32(0x1f)) >= U32(cpu.nwindows())))
		cpu.Abort( Trap_t::illegal_instruction );

		cpu.SetPSR( res );
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpWrpsr(CodeType code, uint32_t addr)
{
	return new OpWrpsr<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpWrpsri<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "wr " << DisasmGPR(rs1) << ", " << DisasmI(simm13) << ", %psr";
}}
template <	typename	ARCH>
void OpWrpsri<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;

		if (not cpu.IsSuper())
		cpu.Abort( Trap_t::privileged_instruction );

		U32 res = cpu.GetGPR(rs1) ^ U32(simm13);
		if (cpu.Test((res & U32(0x1f)) >= U32(cpu.nwindows())))
		cpu.Abort( Trap_t::illegal_instruction );

		cpu.SetPSR( res );
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpWrpsri(CodeType code, uint32_t addr)
{
	return new OpWrpsri<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpWrwim<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "wr " << DisasmGPR(rs1) << ", " << DisasmGPR(rs2) << ", %wim";
}}
template <	typename	ARCH>
void OpWrwim<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;

		if (not cpu.IsSuper())
		cpu.Abort( Trap_t::privileged_instruction );

		cpu.SetWIM( (cpu.GetGPR(rs1) ^ cpu.GetGPR(rs2)) & U32((uint64_t( 1 ) << cpu.nwindows())-1) );
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpWrwim(CodeType code, uint32_t addr)
{
	return new OpWrwim<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpWrwimi<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "wr " << DisasmGPR(rs1) << ", " << DisasmI(simm13) << ", %wim";
}}
template <	typename	ARCH>
void OpWrwimi<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;

		if (not cpu.IsSuper())
		cpu.Abort( Trap_t::privileged_instruction );

		cpu.SetWIM( (cpu.GetGPR(rs1) ^ U32(simm13)) & U32((uint64_t( 1 ) << cpu.nwindows())-1) );
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpWrwimi(CodeType code, uint32_t addr)
{
	return new OpWrwimi<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpWrtbr<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "wr " << DisasmGPR(rs1) << ", " << DisasmGPR(rs2) << ", %tbr";
}}
template <	typename	ARCH>
void OpWrtbr<	ARCH>::execute( ARCH& cpu) const
{
	{
		if (not cpu.IsSuper())
		cpu.Abort( Trap_t::privileged_instruction );

		cpu.SetTBR((cpu.GetGPR(rs1) ^ cpu.GetGPR(rs2)));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpWrtbr(CodeType code, uint32_t addr)
{
	return new OpWrtbr<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpWrtbri<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "wr " << DisasmGPR(rs1) << ", " << DisasmI(simm13) << ", %tbr";
}}
template <	typename	ARCH>
void OpWrtbri<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::U32 U32;

		if (not cpu.IsSuper())
		cpu.Abort( Trap_t::privileged_instruction );

		cpu.SetTBR((cpu.GetGPR(rs1) ^ U32(simm13)) & U32((uint64_t( 1 ) << cpu.nwindows())-1));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpWrtbri(CodeType code, uint32_t addr)
{
	return new OpWrtbri<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpUnimp<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "unimp " << const22;
}}
template <	typename	ARCH>
void OpUnimp<	ARCH>::execute( ARCH& cpu) const
{
	{
		cpu.Abort( Trap_t::illegal_instruction );
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpUnimp(CodeType code, uint32_t addr)
{
	return new OpUnimp<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpFitos<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "fitos " << DisasmFPR(rs2) << ", " << DisasmFPR(rd);
}}
template <	typename	ARCH>
void OpFitos<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::F32 F32;
		typedef typename ARCH::S32 S32;

		S32 src = cpu.GetS32(rs2);
		cpu.SetF32(rd, F32( src ));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpFitos(CodeType code, uint32_t addr)
{
	return new OpFitos<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpFitod<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "fitod " << DisasmFPR(rs2 & -2) << ", " << DisasmFPR(rd & -2);
}}
template <	typename	ARCH>
void OpFitod<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::F64 F64;
		typedef typename ARCH::S32 S32;

		S32 src = cpu.GetS32(rs2);
		cpu.SetF64(rd & -2, F64( src ));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpFitod(CodeType code, uint32_t addr)
{
	return new OpFitod<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpFitoq<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "fitoq " << DisasmFPR(rs2 & -4) << ", " << DisasmFPR(rd & -4);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpFitoq(CodeType code, uint32_t addr)
{
	return new OpFitoq<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpFstoi<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "fstoi " << DisasmFPR(rs2) << ", " << DisasmFPR(rd);
}}
template <	typename	ARCH>
void OpFstoi<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::F32 F32;
		typedef typename ARCH::S32 S32;

		F32 src = cpu.GetF32(rs2);
		cpu.SetS32( rd, S32( src ) );
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpFstoi(CodeType code, uint32_t addr)
{
	return new OpFstoi<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpFdtoi<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "fdtoi " << DisasmFPR(rs2 & -2) << ", " << DisasmFPR(rd & -2);
}}
template <	typename	ARCH>
void OpFdtoi<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::F64 F64;
		typedef typename ARCH::S32 S32;

		F64 src = cpu.GetF64(rs2 & -2);
		cpu.SetS32( rd, S32( src ) );
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpFdtoi(CodeType code, uint32_t addr)
{
	return new OpFdtoi<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpFqtoi<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "fqtoi " << DisasmFPR(rs2 & -4) << ", " << DisasmFPR(rd & -4);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpFqtoi(CodeType code, uint32_t addr)
{
	return new OpFqtoi<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpFstod<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "fstod " << DisasmFPR(rs2) << ", " << DisasmFPR(rd & -2);
}}
template <	typename	ARCH>
void OpFstod<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::F32 F32;
		typedef typename ARCH::F64 F64;

		F32 src = cpu.GetF32(rs2);
		cpu.SetF64(rd & -2, F64( src ));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpFstod(CodeType code, uint32_t addr)
{
	return new OpFstod<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpFstoq<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "fstoq " << DisasmFPR(rs2) << ", " << DisasmFPR(rd & -4);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpFstoq(CodeType code, uint32_t addr)
{
	return new OpFstoq<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpFdtos<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "fdtos " << DisasmFPR(rs2 & -2) << ", " << DisasmFPR(rd);
}}
template <	typename	ARCH>
void OpFdtos<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::F32 F32;
		typedef typename ARCH::F64 F64;

		F64 src = cpu.GetF64(rs2 & -2);
		cpu.SetF32(rd, F32( src ));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpFdtos(CodeType code, uint32_t addr)
{
	return new OpFdtos<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpFdtoq<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "fdtoq " << DisasmFPR(rs2 & -2) << ", " << DisasmFPR(rd & -4);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpFdtoq(CodeType code, uint32_t addr)
{
	return new OpFdtoq<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpFqtos<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "fqtos " << DisasmFPR(rs2 & -4) << ", " << DisasmFPR(rd);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpFqtos(CodeType code, uint32_t addr)
{
	return new OpFqtos<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpFqtod<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "fqtod " << DisasmFPR(rs2 & -4) << ", " << DisasmFPR(rd & -2);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpFqtod(CodeType code, uint32_t addr)
{
	return new OpFqtod<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpFmovs<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "fmovs " << DisasmFPR(rs2) << ", " << DisasmFPR(rd);
}}
template <	typename	ARCH>
void OpFmovs<	ARCH>::execute( ARCH& cpu) const
{
	{
		cpu.SetF32(rd, cpu.GetF32(rs2));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpFmovs(CodeType code, uint32_t addr)
{
	return new OpFmovs<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpFnegs<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "fnegs " << DisasmFPR(rs2) << ", " << DisasmFPR(rd);
}}
template <	typename	ARCH>
void OpFnegs<	ARCH>::execute( ARCH& cpu) const
{
	{
		cpu.SetF32(rd, -cpu.GetF32(rs2));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpFnegs(CodeType code, uint32_t addr)
{
	return new OpFnegs<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpFabss<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "fabss " << DisasmFPR(rs2) << ", " << DisasmFPR(rd);
}}
template <	typename	ARCH>
void OpFabss<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::F32 F32;

		cpu.SetF32(rd, cpu.Test(cpu.GetF32(rs2) >= F32(0)) ? cpu.GetF32(rs2) : -cpu.GetF32(rs2));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpFabss(CodeType code, uint32_t addr)
{
	return new OpFabss<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpFsqrts<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "fsqrts " << DisasmFPR(rs2) << ", " << DisasmFPR(rd);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpFsqrts(CodeType code, uint32_t addr)
{
	return new OpFsqrts<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpFsqrtd<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "fsqrtd " << DisasmFPR(rs2 & -2) << ", " << DisasmFPR(rd & -2);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpFsqrtd(CodeType code, uint32_t addr)
{
	return new OpFsqrtd<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpFsqrtq<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "fsqrtq " << DisasmFPR(rs2 & -4) << ", " << DisasmFPR(rd & -4);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpFsqrtq(CodeType code, uint32_t addr)
{
	return new OpFsqrtq<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpFadds<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "fadds " << DisasmFPR(rs1) << ", " << DisasmFPR(rs2) << ", " << DisasmFPR(rd);
}}
template <	typename	ARCH>
void OpFadds<	ARCH>::execute( ARCH& cpu) const
{
	{
		cpu.SetF32(rd, cpu.GetF32(rs1) + cpu.GetF32(rs2));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpFadds(CodeType code, uint32_t addr)
{
	return new OpFadds<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpFaddd<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "faddd " << DisasmFPR(rs1 & -2) << ", " << DisasmFPR(rs2 & -2) << ", " << DisasmFPR(rd & -2);
}}
template <	typename	ARCH>
void OpFaddd<	ARCH>::execute( ARCH& cpu) const
{
	{
		cpu.SetF64(rd & -2, cpu.GetF64(rs1 & -2) + cpu.GetF64(rs2 & -2));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpFaddd(CodeType code, uint32_t addr)
{
	return new OpFaddd<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpFaddq<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "faddq " << DisasmFPR(rs1 & -4) << ", " << DisasmFPR(rs2 & -4) << ", " << DisasmFPR(rd & -4);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpFaddq(CodeType code, uint32_t addr)
{
	return new OpFaddq<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpFsubs<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "fsubs " << DisasmFPR(rs1) << ", " << DisasmFPR(rs2) << ", " << DisasmFPR(rd);
}}
template <	typename	ARCH>
void OpFsubs<	ARCH>::execute( ARCH& cpu) const
{
	{
		cpu.SetF32(rd, cpu.GetF32(rs1) - cpu.GetF32(rs2));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpFsubs(CodeType code, uint32_t addr)
{
	return new OpFsubs<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpFsubd<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "fsubd " << DisasmFPR(rs1 & -2) << ", " << DisasmFPR(rs2 & -2) << ", " << DisasmFPR(rd & -2);
}}
template <	typename	ARCH>
void OpFsubd<	ARCH>::execute( ARCH& cpu) const
{
	{
		cpu.SetF64(rd & -2, cpu.GetF64(rs1 & -2) - cpu.GetF64(rs2 & -2));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpFsubd(CodeType code, uint32_t addr)
{
	return new OpFsubd<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpFsubq<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "fsubq " << DisasmFPR(rs1 & -4) << ", " << DisasmFPR(rs2 & -4) << ", " << DisasmFPR(rd & -4);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpFsubq(CodeType code, uint32_t addr)
{
	return new OpFsubq<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpFmuls<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "fmuls " << DisasmFPR(rs1) << ", " << DisasmFPR(rs2) << ", " << DisasmFPR(rd);
}}
template <	typename	ARCH>
void OpFmuls<	ARCH>::execute( ARCH& cpu) const
{
	{
		cpu.SetF32(rd, cpu.GetF32(rs1) * cpu.GetF32(rs2));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpFmuls(CodeType code, uint32_t addr)
{
	return new OpFmuls<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpFmuld<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "fmuld " << DisasmFPR(rs1 & -2) << ", " << DisasmFPR(rs2 & -2) << ", " << DisasmFPR(rd & -2);
}}
template <	typename	ARCH>
void OpFmuld<	ARCH>::execute( ARCH& cpu) const
{
	{
		cpu.SetF64(rd & -2, cpu.GetF64(rs1 & -2) * cpu.GetF64(rs2 & -2));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpFmuld(CodeType code, uint32_t addr)
{
	return new OpFmuld<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpFmulq<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "fmulq " << DisasmFPR(rs1 & -4) << ", " << DisasmFPR(rs2 & -4) << ", " << DisasmFPR(rd & -4);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpFmulq(CodeType code, uint32_t addr)
{
	return new OpFmulq<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpFsmuld<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "fsmuld " << DisasmFPR(rs1) << ", " << DisasmFPR(rs2) << ", " << DisasmFPR(rd & -2);
}}
template <	typename	ARCH>
void OpFsmuld<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::F32 F32;
		typedef typename ARCH::F64 F64;

		F32 op1 = cpu.GetF32(rs1), op2 = cpu.GetF32(rs2);
		cpu.SetF64(rd & -2, F64( op1 ) * F64( op2 ));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpFsmuld(CodeType code, uint32_t addr)
{
	return new OpFsmuld<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpFdmulq<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "fdmulq " << DisasmFPR(rs1 & -2) << ", " << DisasmFPR(rs2 & -2) << ", " << DisasmFPR(rd & -4);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpFdmulq(CodeType code, uint32_t addr)
{
	return new OpFdmulq<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpFdivs<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "fdivs " << DisasmFPR(rs1) << ", " << DisasmFPR(rs2) << ", " << DisasmFPR(rd);
}}
template <	typename	ARCH>
void OpFdivs<	ARCH>::execute( ARCH& cpu) const
{
	{
		cpu.SetF32(rd, cpu.GetF32(rs1) / cpu.GetF32(rs2));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpFdivs(CodeType code, uint32_t addr)
{
	return new OpFdivs<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpFdivd<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "fdivd " << DisasmFPR(rs1 & -2) << ", " << DisasmFPR(rs2 & -2) << ", " << DisasmFPR(rd & -2);
}}
template <	typename	ARCH>
void OpFdivd<	ARCH>::execute( ARCH& cpu) const
{
	{
		cpu.SetF64(rd & -2, cpu.GetF64(rs1 & -2) / cpu.GetF64(rs2 & -2));
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpFdivd(CodeType code, uint32_t addr)
{
	return new OpFdivd<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpFdivq<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "fdivq " << DisasmFPR(rs1 & -4) << ", " << DisasmFPR(rs2 & -4) << ", " << DisasmFPR(rd & -4);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpFdivq(CodeType code, uint32_t addr)
{
	return new OpFdivq<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpFcmps<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "fcmps " << DisasmFPR(rs1) << ", " << DisasmFPR(rs2);
}}
template <	typename	ARCH>
void OpFcmps<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::F32 F32;
		typedef typename ARCH::BOOL BOOL;

		F32 a = cpu.GetF32(rs1), b = cpu.GetF32(rs2);
		BOOL le = a <= b, ge = a >= b;
		cpu.SetFCC(not le, not ge);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpFcmps(CodeType code, uint32_t addr)
{
	return new OpFcmps<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpFcmpd<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "fcmpd " << DisasmFPR(rs1 & -2) << ", " << DisasmFPR(rs2 & -2);
}}
template <	typename	ARCH>
void OpFcmpd<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::F64 F64;
		typedef typename ARCH::BOOL BOOL;

		F64 a = cpu.GetF64(rs1), b = cpu.GetF64(rs2);
		BOOL le = a <= b, ge = a >= b;
		cpu.SetFCC(not le, not ge);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpFcmpd(CodeType code, uint32_t addr)
{
	return new OpFcmpd<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpFcmpq<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "fcmpq " << DisasmFPR(rs1 & -4) << ", " << DisasmFPR(rs2 & -4);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpFcmpq(CodeType code, uint32_t addr)
{
	return new OpFcmpq<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpFcmpes<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "fcmpes " << DisasmFPR(rs1) << ", " << DisasmFPR(rs2);
}}
template <	typename	ARCH>
void OpFcmpes<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::F32 F32;
		typedef typename ARCH::BOOL BOOL;

		F32 a = cpu.GetF32(rs1), b = cpu.GetF32(rs2);
		BOOL le = a <= b, ge = a >= b;

		if (cpu.Test(not le and not ge))
		cpu.Abort( Trap_t::fp_exception );

		cpu.SetFCC(not le, not ge);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpFcmpes(CodeType code, uint32_t addr)
{
	return new OpFcmpes<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpFcmped<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "fcmped " << DisasmFPR(rs1 & -2) << ", " << DisasmFPR(rs2 & -2);
}}
template <	typename	ARCH>
void OpFcmped<	ARCH>::execute( ARCH& cpu) const
{
	{
		typedef typename ARCH::F64 F64;
		typedef typename ARCH::BOOL BOOL;

		F64 a = cpu.GetF64(rs1), b = cpu.GetF64(rs2);
		BOOL le = a <= b, ge = a >= b;

		if (cpu.Test(not le and not ge))
		cpu.Abort( Trap_t::fp_exception );

		cpu.SetFCC(not le, not ge);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpFcmped(CodeType code, uint32_t addr)
{
	return new OpFcmped<	ARCH>(code, addr);
}

template <	typename	ARCH>
void OpFcmpeq<	ARCH>::disasm( std::ostream& sink,
uint32_t pc) const
{
	{
		sink << "fcmpeq " << DisasmFPR(rs1 & -4) << ", " << DisasmFPR(rs2 & -4);
}}

template <	typename	ARCH>
static Operation<	ARCH> *DecodeOpFcmpeq(CodeType code, uint32_t addr)
{
	return new OpFcmpeq<	ARCH>(code, addr);
}

template <	typename	ARCH>
OpBc<	ARCH>::OpBc(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "bc")
{
	annul = uint8_t((code >> 29) & 0x1);
	cond = uint8_t((code >> 25) & 0xf);
	disp22 = ((int32_t)((code & 0x3fffff) << 10) >> 10) << 2;
}

template <	typename	ARCH>
OpFbc<	ARCH>::OpFbc(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "fbc")
{
	annul = uint8_t((code >> 29) & 0x1);
	cond = uint8_t((code >> 25) & 0xf);
	disp22 = ((int32_t)((code & 0x3fffff) << 10) >> 10) << 2;
}

template <	typename	ARCH>
OpTc<	ARCH>::OpTc(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "tc")
{
	cond = uint8_t((code >> 25) & 0xf);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpTci<	ARCH>::OpTci(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "tci")
{
	cond = uint8_t((code >> 25) & 0xf);
	rs1 = uint8_t((code >> 14) & 0x1f);
	imm7 = uint8_t(code & 0x7f);
}

template <	typename	ARCH>
OpRdasr<	ARCH>::OpRdasr(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "rdasr")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
}

template <	typename	ARCH>
OpWrasr<	ARCH>::OpWrasr(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "wrasr")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpWrasri<	ARCH>::OpWrasri(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "wrasri")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpLdsb<	ARCH>::OpLdsb(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "ldsb")
{
	rd = uint8_t((code >> 25) & 0x1f);
	a = uint8_t((code >> 23) & 0x1);
	rs1 = uint8_t((code >> 14) & 0x1f);
	asi = uint8_t((code >> 5) & 0xff);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpLdsbi<	ARCH>::OpLdsbi(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "ldsbi")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpLdsh<	ARCH>::OpLdsh(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "ldsh")
{
	rd = uint8_t((code >> 25) & 0x1f);
	a = uint8_t((code >> 23) & 0x1);
	rs1 = uint8_t((code >> 14) & 0x1f);
	asi = uint8_t((code >> 5) & 0xff);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpLdshi<	ARCH>::OpLdshi(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "ldshi")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpLdub<	ARCH>::OpLdub(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "ldub")
{
	rd = uint8_t((code >> 25) & 0x1f);
	a = uint8_t((code >> 23) & 0x1);
	rs1 = uint8_t((code >> 14) & 0x1f);
	asi = uint8_t((code >> 5) & 0xff);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpLdubi<	ARCH>::OpLdubi(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "ldubi")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpLduh<	ARCH>::OpLduh(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "lduh")
{
	rd = uint8_t((code >> 25) & 0x1f);
	a = uint8_t((code >> 23) & 0x1);
	rs1 = uint8_t((code >> 14) & 0x1f);
	asi = uint8_t((code >> 5) & 0xff);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpLduhi<	ARCH>::OpLduhi(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "lduhi")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpLd<	ARCH>::OpLd(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "ld")
{
	rd = uint8_t((code >> 25) & 0x1f);
	a = uint8_t((code >> 23) & 0x1);
	rs1 = uint8_t((code >> 14) & 0x1f);
	asi = uint8_t((code >> 5) & 0xff);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpLdi<	ARCH>::OpLdi(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "ldi")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpLdd<	ARCH>::OpLdd(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "ldd")
{
	rd = uint8_t((code >> 25) & 0x1f);
	a = uint8_t((code >> 23) & 0x1);
	rs1 = uint8_t((code >> 14) & 0x1f);
	asi = uint8_t((code >> 5) & 0xff);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpLddi<	ARCH>::OpLddi(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "lddi")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpLdf<	ARCH>::OpLdf(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "ldf")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpLdfi<	ARCH>::OpLdfi(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "ldfi")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpLddf<	ARCH>::OpLddf(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "lddf")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpLddfi<	ARCH>::OpLddfi(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "lddfi")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpLdfsr<	ARCH>::OpLdfsr(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "ldfsr")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpLdfsri<	ARCH>::OpLdfsri(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "ldfsri")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpLdc<	ARCH>::OpLdc(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "ldc")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpLdci<	ARCH>::OpLdci(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "ldci")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpLddc<	ARCH>::OpLddc(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "lddc")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpLddci<	ARCH>::OpLddci(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "lddci")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpLdcsr<	ARCH>::OpLdcsr(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "ldcsr")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpLdcsri<	ARCH>::OpLdcsri(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "ldcsri")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpStb<	ARCH>::OpStb(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "stb")
{
	rd = uint8_t((code >> 25) & 0x1f);
	a = uint8_t((code >> 23) & 0x1);
	rs1 = uint8_t((code >> 14) & 0x1f);
	asi = uint8_t((code >> 5) & 0xff);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpStbi<	ARCH>::OpStbi(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "stbi")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpSth<	ARCH>::OpSth(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "sth")
{
	rd = uint8_t((code >> 25) & 0x1f);
	a = uint8_t((code >> 23) & 0x1);
	rs1 = uint8_t((code >> 14) & 0x1f);
	asi = uint8_t((code >> 5) & 0xff);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpSthi<	ARCH>::OpSthi(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "sthi")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpSt<	ARCH>::OpSt(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "st")
{
	rd = uint8_t((code >> 25) & 0x1f);
	a = uint8_t((code >> 23) & 0x1);
	rs1 = uint8_t((code >> 14) & 0x1f);
	asi = uint8_t((code >> 5) & 0xff);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpSti<	ARCH>::OpSti(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "sti")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpStd<	ARCH>::OpStd(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "std")
{
	rd = uint8_t((code >> 25) & 0x1f);
	a = uint8_t((code >> 23) & 0x1);
	rs1 = uint8_t((code >> 14) & 0x1f);
	asi = uint8_t((code >> 5) & 0xff);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpStdi<	ARCH>::OpStdi(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "stdi")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpStf<	ARCH>::OpStf(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "stf")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpStfi<	ARCH>::OpStfi(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "stfi")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpStdf<	ARCH>::OpStdf(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "stdf")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpStdfi<	ARCH>::OpStdfi(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "stdfi")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpStfsr<	ARCH>::OpStfsr(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "stfsr")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpStfsri<	ARCH>::OpStfsri(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "stfsri")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpStdfq<	ARCH>::OpStdfq(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "stdfq")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpStdfqi<	ARCH>::OpStdfqi(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "stdfqi")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpStc<	ARCH>::OpStc(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "stc")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpStci<	ARCH>::OpStci(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "stci")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpStdc<	ARCH>::OpStdc(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "stdc")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpStdci<	ARCH>::OpStdci(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "stdci")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpStcsr<	ARCH>::OpStcsr(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "stcsr")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpStcsri<	ARCH>::OpStcsri(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "stcsri")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpStdcq<	ARCH>::OpStdcq(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "stdcq")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpStdcqi<	ARCH>::OpStdcqi(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "stdcqi")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpLdstub<	ARCH>::OpLdstub(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "ldstub")
{
	rd = uint8_t((code >> 25) & 0x1f);
	a = uint8_t((code >> 23) & 0x1);
	rs1 = uint8_t((code >> 14) & 0x1f);
	asi = uint8_t((code >> 5) & 0xff);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpLdstubi<	ARCH>::OpLdstubi(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "ldstubi")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpSwap<	ARCH>::OpSwap(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "swap")
{
	rd = uint8_t((code >> 25) & 0x1f);
	a = uint8_t((code >> 23) & 0x1);
	rs1 = uint8_t((code >> 14) & 0x1f);
	asi = uint8_t((code >> 5) & 0xff);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpSwapi<	ARCH>::OpSwapi(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "swapi")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpFlush<	ARCH>::OpFlush(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "flush")
{
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpFlushi<	ARCH>::OpFlushi(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "flushi")
{
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpSethi<	ARCH>::OpSethi(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "sethi")
{
	rd = uint8_t((code >> 25) & 0x1f);
	imm22 = uint32_t(code & 0x3fffff) << 10;
}

template <	typename	ARCH>
OpAnd_<	ARCH>::OpAnd_(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "and_")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpAndi<	ARCH>::OpAndi(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "andi")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpAndcc<	ARCH>::OpAndcc(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "andcc")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpAndcci<	ARCH>::OpAndcci(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "andcci")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpAndn<	ARCH>::OpAndn(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "andn")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpAndni<	ARCH>::OpAndni(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "andni")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpAndncc<	ARCH>::OpAndncc(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "andncc")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpAndncci<	ARCH>::OpAndncci(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "andncci")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpOr_<	ARCH>::OpOr_(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "or_")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpOri<	ARCH>::OpOri(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "ori")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpOrcc<	ARCH>::OpOrcc(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "orcc")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpOrcci<	ARCH>::OpOrcci(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "orcci")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpOrn<	ARCH>::OpOrn(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "orn")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpOrni<	ARCH>::OpOrni(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "orni")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpOrncc<	ARCH>::OpOrncc(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "orncc")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpOrncci<	ARCH>::OpOrncci(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "orncci")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpXor_<	ARCH>::OpXor_(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "xor_")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpXori<	ARCH>::OpXori(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "xori")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpXorcc<	ARCH>::OpXorcc(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "xorcc")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpXorcci<	ARCH>::OpXorcci(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "xorcci")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpXnor<	ARCH>::OpXnor(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "xnor")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpXnori<	ARCH>::OpXnori(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "xnori")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpXnorcc<	ARCH>::OpXnorcc(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "xnorcc")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpXnorcci<	ARCH>::OpXnorcci(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "xnorcci")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpSll<	ARCH>::OpSll(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "sll")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpSlli<	ARCH>::OpSlli(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "slli")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	shcnt = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpSrl<	ARCH>::OpSrl(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "srl")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpSrli<	ARCH>::OpSrli(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "srli")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	shcnt = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpSra<	ARCH>::OpSra(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "sra")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpSrai<	ARCH>::OpSrai(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "srai")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	shcnt = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpAdd<	ARCH>::OpAdd(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "add")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpAddi<	ARCH>::OpAddi(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "addi")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpAddcc<	ARCH>::OpAddcc(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "addcc")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpAddcci<	ARCH>::OpAddcci(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "addcci")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpAddx<	ARCH>::OpAddx(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "addx")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpAddxi<	ARCH>::OpAddxi(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "addxi")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpAddxcc<	ARCH>::OpAddxcc(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "addxcc")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpAddxcci<	ARCH>::OpAddxcci(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "addxcci")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpTaddcc<	ARCH>::OpTaddcc(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "taddcc")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpTaddcci<	ARCH>::OpTaddcci(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "taddcci")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpTaddcctv<	ARCH>::OpTaddcctv(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "taddcctv")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpTaddcctvi<	ARCH>::OpTaddcctvi(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "taddcctvi")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpSub<	ARCH>::OpSub(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "sub")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpSubi<	ARCH>::OpSubi(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "subi")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpSubcc<	ARCH>::OpSubcc(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "subcc")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpSubcci<	ARCH>::OpSubcci(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "subcci")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpSubx<	ARCH>::OpSubx(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "subx")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpSubxi<	ARCH>::OpSubxi(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "subxi")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpSubxcc<	ARCH>::OpSubxcc(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "subxcc")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpSubxcci<	ARCH>::OpSubxcci(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "subxcci")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpTsubcc<	ARCH>::OpTsubcc(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "tsubcc")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpTsubcci<	ARCH>::OpTsubcci(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "tsubcci")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpTsubcctv<	ARCH>::OpTsubcctv(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "tsubcctv")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpTsubcctvi<	ARCH>::OpTsubcctvi(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "tsubcctvi")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpMulscc<	ARCH>::OpMulscc(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "mulscc")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpMulscci<	ARCH>::OpMulscci(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "mulscci")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpUmul<	ARCH>::OpUmul(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "umul")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpUmuli<	ARCH>::OpUmuli(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "umuli")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpSmul<	ARCH>::OpSmul(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "smul")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpSmuli<	ARCH>::OpSmuli(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "smuli")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpUmulcc<	ARCH>::OpUmulcc(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "umulcc")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpUmulcci<	ARCH>::OpUmulcci(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "umulcci")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpSmulcc<	ARCH>::OpSmulcc(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "smulcc")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpSmulcci<	ARCH>::OpSmulcci(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "smulcci")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpUdiv<	ARCH>::OpUdiv(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "udiv")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpUdivi<	ARCH>::OpUdivi(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "udivi")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpSdiv<	ARCH>::OpSdiv(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "sdiv")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpSdivi<	ARCH>::OpSdivi(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "sdivi")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpUdivcc<	ARCH>::OpUdivcc(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "udivcc")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpUdivcci<	ARCH>::OpUdivcci(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "udivcci")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpSdivcc<	ARCH>::OpSdivcc(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "sdivcc")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpSdivcci<	ARCH>::OpSdivcci(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "sdivcci")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpSave<	ARCH>::OpSave(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "save")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpSavei<	ARCH>::OpSavei(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "savei")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpRestore<	ARCH>::OpRestore(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "restore")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpRestorei<	ARCH>::OpRestorei(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "restorei")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpBa<	ARCH>::OpBa(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "ba")
{
	annul = uint8_t((code >> 29) & 0x1);
	disp22 = ((int32_t)((code & 0x3fffff) << 10) >> 10) << 2;
}

template <	typename	ARCH>
OpFba<	ARCH>::OpFba(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "fba")
{
	annul = uint8_t((code >> 29) & 0x1);
	disp22 = ((int32_t)((code & 0x3fffff) << 10) >> 10) << 2;
}

template <	typename	ARCH>
OpCba<	ARCH>::OpCba(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "cba")
{
	annul = uint8_t((code >> 29) & 0x1);
	disp22 = ((int32_t)((code & 0x3fffff) << 10) >> 10) << 2;
}

template <	typename	ARCH>
OpCbn<	ARCH>::OpCbn(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "cbn")
{
	annul = uint8_t((code >> 29) & 0x1);
	disp22 = ((int32_t)((code & 0x3fffff) << 10) >> 10) << 2;
}

template <	typename	ARCH>
OpCb3<	ARCH>::OpCb3(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "cb3")
{
	annul = uint8_t((code >> 29) & 0x1);
	disp22 = ((int32_t)((code & 0x3fffff) << 10) >> 10) << 2;
}

template <	typename	ARCH>
OpCb2<	ARCH>::OpCb2(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "cb2")
{
	annul = uint8_t((code >> 29) & 0x1);
	disp22 = ((int32_t)((code & 0x3fffff) << 10) >> 10) << 2;
}

template <	typename	ARCH>
OpCb23<	ARCH>::OpCb23(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "cb23")
{
	annul = uint8_t((code >> 29) & 0x1);
	disp22 = ((int32_t)((code & 0x3fffff) << 10) >> 10) << 2;
}

template <	typename	ARCH>
OpCb1<	ARCH>::OpCb1(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "cb1")
{
	annul = uint8_t((code >> 29) & 0x1);
	disp22 = ((int32_t)((code & 0x3fffff) << 10) >> 10) << 2;
}

template <	typename	ARCH>
OpCb13<	ARCH>::OpCb13(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "cb13")
{
	annul = uint8_t((code >> 29) & 0x1);
	disp22 = ((int32_t)((code & 0x3fffff) << 10) >> 10) << 2;
}

template <	typename	ARCH>
OpCb12<	ARCH>::OpCb12(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "cb12")
{
	annul = uint8_t((code >> 29) & 0x1);
	disp22 = ((int32_t)((code & 0x3fffff) << 10) >> 10) << 2;
}

template <	typename	ARCH>
OpCb123<	ARCH>::OpCb123(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "cb123")
{
	annul = uint8_t((code >> 29) & 0x1);
	disp22 = ((int32_t)((code & 0x3fffff) << 10) >> 10) << 2;
}

template <	typename	ARCH>
OpCb0<	ARCH>::OpCb0(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "cb0")
{
	annul = uint8_t((code >> 29) & 0x1);
	disp22 = ((int32_t)((code & 0x3fffff) << 10) >> 10) << 2;
}

template <	typename	ARCH>
OpCb03<	ARCH>::OpCb03(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "cb03")
{
	annul = uint8_t((code >> 29) & 0x1);
	disp22 = ((int32_t)((code & 0x3fffff) << 10) >> 10) << 2;
}

template <	typename	ARCH>
OpCb02<	ARCH>::OpCb02(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "cb02")
{
	annul = uint8_t((code >> 29) & 0x1);
	disp22 = ((int32_t)((code & 0x3fffff) << 10) >> 10) << 2;
}

template <	typename	ARCH>
OpCb023<	ARCH>::OpCb023(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "cb023")
{
	annul = uint8_t((code >> 29) & 0x1);
	disp22 = ((int32_t)((code & 0x3fffff) << 10) >> 10) << 2;
}

template <	typename	ARCH>
OpCb01<	ARCH>::OpCb01(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "cb01")
{
	annul = uint8_t((code >> 29) & 0x1);
	disp22 = ((int32_t)((code & 0x3fffff) << 10) >> 10) << 2;
}

template <	typename	ARCH>
OpCb013<	ARCH>::OpCb013(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "cb013")
{
	annul = uint8_t((code >> 29) & 0x1);
	disp22 = ((int32_t)((code & 0x3fffff) << 10) >> 10) << 2;
}

template <	typename	ARCH>
OpCb012<	ARCH>::OpCb012(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "cb012")
{
	annul = uint8_t((code >> 29) & 0x1);
	disp22 = ((int32_t)((code & 0x3fffff) << 10) >> 10) << 2;
}

template <	typename	ARCH>
OpCall<	ARCH>::OpCall(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "call")
{
	disp30 = uint32_t(code & 0x3fffffff) << 2;
}

template <	typename	ARCH>
OpJmpl<	ARCH>::OpJmpl(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "jmpl")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	asi = uint8_t((code >> 5) & 0xff);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpJmpli<	ARCH>::OpJmpli(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "jmpli")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpRett<	ARCH>::OpRett(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "rett")
{
	rs1 = uint8_t((code >> 14) & 0x1f);
	asi = uint8_t((code >> 5) & 0xff);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpRetti<	ARCH>::OpRetti(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "retti")
{
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpTa<	ARCH>::OpTa(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "ta")
{
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpTai<	ARCH>::OpTai(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "tai")
{
	rs1 = uint8_t((code >> 14) & 0x1f);
	imm7 = uint8_t(code & 0x7f);
}

template <	typename	ARCH>
OpRdy<	ARCH>::OpRdy(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "rdy")
{
	rd = uint8_t((code >> 25) & 0x1f);
}

template <	typename	ARCH>
OpStbar<	ARCH>::OpStbar(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "stbar")
{
}

template <	typename	ARCH>
OpRdpsr<	ARCH>::OpRdpsr(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "rdpsr")
{
	rd = uint8_t((code >> 25) & 0x1f);
}

template <	typename	ARCH>
OpRdwim<	ARCH>::OpRdwim(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "rdwim")
{
	rd = uint8_t((code >> 25) & 0x1f);
}

template <	typename	ARCH>
OpRdtbr<	ARCH>::OpRdtbr(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "rdtbr")
{
	rd = uint8_t((code >> 25) & 0x1f);
}

template <	typename	ARCH>
OpWry<	ARCH>::OpWry(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "wry")
{
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpWryi<	ARCH>::OpWryi(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "wryi")
{
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpWrpsr<	ARCH>::OpWrpsr(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "wrpsr")
{
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpWrpsri<	ARCH>::OpWrpsri(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "wrpsri")
{
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpWrwim<	ARCH>::OpWrwim(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "wrwim")
{
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpWrwimi<	ARCH>::OpWrwimi(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "wrwimi")
{
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpWrtbr<	ARCH>::OpWrtbr(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "wrtbr")
{
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpWrtbri<	ARCH>::OpWrtbri(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "wrtbri")
{
	rs1 = uint8_t((code >> 14) & 0x1f);
	simm13 = ((int32_t)((code & 0x1fff) << 19) >> 19);
}

template <	typename	ARCH>
OpUnimp<	ARCH>::OpUnimp(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "unimp")
{
	const22 = uint32_t(code & 0x3fffff);
}

template <	typename	ARCH>
OpFitos<	ARCH>::OpFitos(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "fitos")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpFitod<	ARCH>::OpFitod(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "fitod")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpFitoq<	ARCH>::OpFitoq(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "fitoq")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpFstoi<	ARCH>::OpFstoi(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "fstoi")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpFdtoi<	ARCH>::OpFdtoi(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "fdtoi")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpFqtoi<	ARCH>::OpFqtoi(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "fqtoi")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpFstod<	ARCH>::OpFstod(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "fstod")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpFstoq<	ARCH>::OpFstoq(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "fstoq")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpFdtos<	ARCH>::OpFdtos(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "fdtos")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpFdtoq<	ARCH>::OpFdtoq(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "fdtoq")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpFqtos<	ARCH>::OpFqtos(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "fqtos")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpFqtod<	ARCH>::OpFqtod(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "fqtod")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpFmovs<	ARCH>::OpFmovs(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "fmovs")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpFnegs<	ARCH>::OpFnegs(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "fnegs")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpFabss<	ARCH>::OpFabss(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "fabss")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpFsqrts<	ARCH>::OpFsqrts(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "fsqrts")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpFsqrtd<	ARCH>::OpFsqrtd(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "fsqrtd")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpFsqrtq<	ARCH>::OpFsqrtq(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "fsqrtq")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpFadds<	ARCH>::OpFadds(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "fadds")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpFaddd<	ARCH>::OpFaddd(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "faddd")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpFaddq<	ARCH>::OpFaddq(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "faddq")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpFsubs<	ARCH>::OpFsubs(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "fsubs")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpFsubd<	ARCH>::OpFsubd(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "fsubd")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpFsubq<	ARCH>::OpFsubq(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "fsubq")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpFmuls<	ARCH>::OpFmuls(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "fmuls")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpFmuld<	ARCH>::OpFmuld(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "fmuld")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpFmulq<	ARCH>::OpFmulq(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "fmulq")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpFsmuld<	ARCH>::OpFsmuld(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "fsmuld")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpFdmulq<	ARCH>::OpFdmulq(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "fdmulq")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpFdivs<	ARCH>::OpFdivs(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "fdivs")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpFdivd<	ARCH>::OpFdivd(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "fdivd")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpFdivq<	ARCH>::OpFdivq(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "fdivq")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpFcmps<	ARCH>::OpFcmps(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "fcmps")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpFcmpd<	ARCH>::OpFcmpd(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "fcmpd")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpFcmpq<	ARCH>::OpFcmpq(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "fcmpq")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpFcmpes<	ARCH>::OpFcmpes(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "fcmpes")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpFcmped<	ARCH>::OpFcmped(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "fcmped")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
OpFcmpeq<	ARCH>::OpFcmpeq(CodeType code, uint32_t addr) : Operation<	ARCH>(code, addr, "fcmpeq")
{
	rd = uint8_t((code >> 25) & 0x1f);
	rs1 = uint8_t((code >> 14) & 0x1f);
	rs2 = uint8_t(code & 0x1f);
}

template <	typename	ARCH>
DecodeTableEntry<	ARCH>::DecodeTableEntry(CodeType opcode, CodeType opcode_mask, Operation<	ARCH> *(*decode)(CodeType, uint32_t))
{
	this->opcode = opcode;
	this->opcode_mask = opcode_mask;
	this->decode = decode;
}

template <	typename	ARCH>
Decoder<	ARCH>::Decoder()

{
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81a80ae0UL, 0xc1f83fe0UL, DecodeOpFcmpeq<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81a80ac0UL, 0xc1f83fe0UL, DecodeOpFcmped<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81a80aa0UL, 0xc1f83fe0UL, DecodeOpFcmpes<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81a80a60UL, 0xc1f83fe0UL, DecodeOpFcmpq<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81a80a40UL, 0xc1f83fe0UL, DecodeOpFcmpd<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81a80a20UL, 0xc1f83fe0UL, DecodeOpFcmps<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81a009e0UL, 0xc1f83fe0UL, DecodeOpFdivq<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81a009c0UL, 0xc1f83fe0UL, DecodeOpFdivd<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81a009a0UL, 0xc1f83fe0UL, DecodeOpFdivs<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81a00dc0UL, 0xc1f83fe0UL, DecodeOpFdmulq<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81a00d20UL, 0xc1f83fe0UL, DecodeOpFsmuld<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81a00960UL, 0xc1f83fe0UL, DecodeOpFmulq<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81a00940UL, 0xc1f83fe0UL, DecodeOpFmuld<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81a00920UL, 0xc1f83fe0UL, DecodeOpFmuls<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81a008e0UL, 0xc1f83fe0UL, DecodeOpFsubq<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81a008c0UL, 0xc1f83fe0UL, DecodeOpFsubd<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81a008a0UL, 0xc1f83fe0UL, DecodeOpFsubs<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81a00860UL, 0xc1f83fe0UL, DecodeOpFaddq<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81a00840UL, 0xc1f83fe0UL, DecodeOpFaddd<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81a00820UL, 0xc1f83fe0UL, DecodeOpFadds<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81a00560UL, 0xc1f83fe0UL, DecodeOpFsqrtq<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81a00540UL, 0xc1f83fe0UL, DecodeOpFsqrtd<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81a00520UL, 0xc1f83fe0UL, DecodeOpFsqrts<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81a00120UL, 0xc1f83fe0UL, DecodeOpFabss<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81a000a0UL, 0xc1f83fe0UL, DecodeOpFnegs<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81a00020UL, 0xc1f83fe0UL, DecodeOpFmovs<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81a01960UL, 0xc1f83fe0UL, DecodeOpFqtod<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81a018e0UL, 0xc1f83fe0UL, DecodeOpFqtos<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81a019c0UL, 0xc1f83fe0UL, DecodeOpFdtoq<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81a018c0UL, 0xc1f83fe0UL, DecodeOpFdtos<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81a019a0UL, 0xc1f83fe0UL, DecodeOpFstoq<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81a01920UL, 0xc1f83fe0UL, DecodeOpFstod<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81a01a60UL, 0xc1f83fe0UL, DecodeOpFqtoi<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81a01a40UL, 0xc1f83fe0UL, DecodeOpFdtoi<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81a01a20UL, 0xc1f83fe0UL, DecodeOpFstoi<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81a01980UL, 0xc1f83fe0UL, DecodeOpFitoq<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81a01900UL, 0xc1f83fe0UL, DecodeOpFitod<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81a01880UL, 0xc1f83fe0UL, DecodeOpFitos<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x0UL, 0xc1c00000UL, DecodeOpUnimp<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81982000UL, 0xc1f82000UL, DecodeOpWrtbri<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81980000UL, 0xc1f82000UL, DecodeOpWrtbr<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81902000UL, 0xc1f82000UL, DecodeOpWrwimi<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81900000UL, 0xc1f82000UL, DecodeOpWrwim<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81882000UL, 0xc1f82000UL, DecodeOpWrpsri<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81880000UL, 0xc1f82000UL, DecodeOpWrpsr<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81802000UL, 0xfff82000UL, DecodeOpWryi<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81800000UL, 0xfff82000UL, DecodeOpWry<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81580000UL, 0xc1f80000UL, DecodeOpRdtbr<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81500000UL, 0xc1f80000UL, DecodeOpRdwim<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81480000UL, 0xc1f80000UL, DecodeOpRdpsr<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x8143c000UL, 0xffffc000UL, DecodeOpStbar<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81400000UL, 0xc1ffc000UL, DecodeOpRdy<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x91d02000UL, 0xdff82000UL, DecodeOpTai<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x91d00000UL, 0xdff82000UL, DecodeOpTa<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81c82000UL, 0xc1f82000UL, DecodeOpRetti<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81c80000UL, 0xc1f82000UL, DecodeOpRett<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81c02000UL, 0xc1f82000UL, DecodeOpJmpli<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81c00000UL, 0xc1f82000UL, DecodeOpJmpl<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x40000000UL, 0xc0000000UL, DecodeOpCall<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x1fc00000UL, 0xdfc00000UL, DecodeOpCb012<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x1dc00000UL, 0xdfc00000UL, DecodeOpCb013<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x1bc00000UL, 0xdfc00000UL, DecodeOpCb01<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x19c00000UL, 0xdfc00000UL, DecodeOpCb023<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x17c00000UL, 0xdfc00000UL, DecodeOpCb02<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x15c00000UL, 0xdfc00000UL, DecodeOpCb03<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x13c00000UL, 0xdfc00000UL, DecodeOpCb0<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x3c00000UL, 0xdfc00000UL, DecodeOpCb123<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x5c00000UL, 0xdfc00000UL, DecodeOpCb12<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x7c00000UL, 0xdfc00000UL, DecodeOpCb13<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x9c00000UL, 0xdfc00000UL, DecodeOpCb1<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xbc00000UL, 0xdfc00000UL, DecodeOpCb23<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xdc00000UL, 0xdfc00000UL, DecodeOpCb2<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xfc00000UL, 0xdfc00000UL, DecodeOpCb3<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x1c00000UL, 0xdfc00000UL, DecodeOpCbn<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x11c00000UL, 0xdfc00000UL, DecodeOpCba<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x11800000UL, 0xdfc00000UL, DecodeOpFba<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x10800000UL, 0xdfc00000UL, DecodeOpBa<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81e82000UL, 0xc1f82000UL, DecodeOpRestorei<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81e80000UL, 0xc1f82000UL, DecodeOpRestore<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81e02000UL, 0xc1f82000UL, DecodeOpSavei<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81e00000UL, 0xc1f82000UL, DecodeOpSave<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80f82000UL, 0xc1f82000UL, DecodeOpSdivcci<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80f80000UL, 0xc1f82000UL, DecodeOpSdivcc<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80f02000UL, 0xc1f82000UL, DecodeOpUdivcci<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80f00000UL, 0xc1f82000UL, DecodeOpUdivcc<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80782000UL, 0xc1f82000UL, DecodeOpSdivi<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80780000UL, 0xc1f82000UL, DecodeOpSdiv<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80702000UL, 0xc1f82000UL, DecodeOpUdivi<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80700000UL, 0xc1f82000UL, DecodeOpUdiv<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80d82000UL, 0xc1f82000UL, DecodeOpSmulcci<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80d80000UL, 0xc1f82000UL, DecodeOpSmulcc<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80d02000UL, 0xc1f82000UL, DecodeOpUmulcci<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80d00000UL, 0xc1f82000UL, DecodeOpUmulcc<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80582000UL, 0xc1f82000UL, DecodeOpSmuli<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80580000UL, 0xc1f82000UL, DecodeOpSmul<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80502000UL, 0xc1f82000UL, DecodeOpUmuli<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80500000UL, 0xc1f82000UL, DecodeOpUmul<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81202000UL, 0xc1f82000UL, DecodeOpMulscci<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81200000UL, 0xc1f82000UL, DecodeOpMulscc<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81182000UL, 0xc1f82000UL, DecodeOpTsubcctvi<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81180000UL, 0xc1f82000UL, DecodeOpTsubcctv<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81082000UL, 0xc1f82000UL, DecodeOpTsubcci<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81080000UL, 0xc1f82000UL, DecodeOpTsubcc<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80e02000UL, 0xc1f82000UL, DecodeOpSubxcci<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80e00000UL, 0xc1f82000UL, DecodeOpSubxcc<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80602000UL, 0xc1f82000UL, DecodeOpSubxi<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80600000UL, 0xc1f82000UL, DecodeOpSubx<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80a02000UL, 0xc1f82000UL, DecodeOpSubcci<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80a00000UL, 0xc1f82000UL, DecodeOpSubcc<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80202000UL, 0xc1f82000UL, DecodeOpSubi<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80200000UL, 0xc1f82000UL, DecodeOpSub<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81102000UL, 0xc1f82000UL, DecodeOpTaddcctvi<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81100000UL, 0xc1f82000UL, DecodeOpTaddcctv<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81002000UL, 0xc1f82000UL, DecodeOpTaddcci<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81000000UL, 0xc1f82000UL, DecodeOpTaddcc<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80c02000UL, 0xc1f82000UL, DecodeOpAddxcci<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80c00000UL, 0xc1f82000UL, DecodeOpAddxcc<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80402000UL, 0xc1f82000UL, DecodeOpAddxi<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80400000UL, 0xc1f82000UL, DecodeOpAddx<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80802000UL, 0xc1f82000UL, DecodeOpAddcci<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80800000UL, 0xc1f82000UL, DecodeOpAddcc<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80002000UL, 0xc1f82000UL, DecodeOpAddi<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80000000UL, 0xc1f82000UL, DecodeOpAdd<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81382000UL, 0xc1f82000UL, DecodeOpSrai<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81380000UL, 0xc1f82000UL, DecodeOpSra<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81302000UL, 0xc1f82000UL, DecodeOpSrli<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81300000UL, 0xc1f82000UL, DecodeOpSrl<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81282000UL, 0xc1f82000UL, DecodeOpSlli<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81280000UL, 0xc1f82000UL, DecodeOpSll<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80b82000UL, 0xc1f82000UL, DecodeOpXnorcci<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80b80000UL, 0xc1f82000UL, DecodeOpXnorcc<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80382000UL, 0xc1f82000UL, DecodeOpXnori<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80380000UL, 0xc1f82000UL, DecodeOpXnor<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80982000UL, 0xc1f82000UL, DecodeOpXorcci<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80980000UL, 0xc1f82000UL, DecodeOpXorcc<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80182000UL, 0xc1f82000UL, DecodeOpXori<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80180000UL, 0xc1f82000UL, DecodeOpXor_<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80b02000UL, 0xc1f82000UL, DecodeOpOrncci<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80b00000UL, 0xc1f82000UL, DecodeOpOrncc<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80302000UL, 0xc1f82000UL, DecodeOpOrni<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80300000UL, 0xc1f82000UL, DecodeOpOrn<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80902000UL, 0xc1f82000UL, DecodeOpOrcci<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80900000UL, 0xc1f82000UL, DecodeOpOrcc<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80102000UL, 0xc1f82000UL, DecodeOpOri<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80100000UL, 0xc1f82000UL, DecodeOpOr_<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80a82000UL, 0xc1f82000UL, DecodeOpAndncci<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80a80000UL, 0xc1f82000UL, DecodeOpAndncc<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80282000UL, 0xc1f82000UL, DecodeOpAndni<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80280000UL, 0xc1f82000UL, DecodeOpAndn<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80882000UL, 0xc1f82000UL, DecodeOpAndcci<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80880000UL, 0xc1f82000UL, DecodeOpAndcc<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80082000UL, 0xc1f82000UL, DecodeOpAndi<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x80080000UL, 0xc1f82000UL, DecodeOpAnd_<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x1000000UL, 0xc1c00000UL, DecodeOpSethi<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81d82000UL, 0xc1f82000UL, DecodeOpFlushi<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81d80000UL, 0xc1f82000UL, DecodeOpFlush<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc0782000UL, 0xc1f82000UL, DecodeOpSwapi<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc0780000UL, 0xc1782000UL, DecodeOpSwap<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc0682000UL, 0xc1f82000UL, DecodeOpLdstubi<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc0680000UL, 0xc1782000UL, DecodeOpLdstub<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc1b02000UL, 0xc1f82000UL, DecodeOpStdcqi<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc1b00000UL, 0xc1f82000UL, DecodeOpStdcq<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc1a82000UL, 0xc1f82000UL, DecodeOpStcsri<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc1a80000UL, 0xc1f82000UL, DecodeOpStcsr<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc1b82000UL, 0xc1f82000UL, DecodeOpStdci<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc1b80000UL, 0xc1f82000UL, DecodeOpStdc<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc1a02000UL, 0xc1f82000UL, DecodeOpStci<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc1a00000UL, 0xc1f82000UL, DecodeOpStc<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc1302000UL, 0xc1f82000UL, DecodeOpStdfqi<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc1300000UL, 0xc1f82000UL, DecodeOpStdfq<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc1282000UL, 0xc1f82000UL, DecodeOpStfsri<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc1280000UL, 0xc1f82000UL, DecodeOpStfsr<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc1382000UL, 0xc1f82000UL, DecodeOpStdfi<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc1380000UL, 0xc1f82000UL, DecodeOpStdf<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc1202000UL, 0xc1f82000UL, DecodeOpStfi<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc1200000UL, 0xc1f82000UL, DecodeOpStf<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc0382000UL, 0xc1f82000UL, DecodeOpStdi<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc0380000UL, 0xc1782000UL, DecodeOpStd<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc0202000UL, 0xc1f82000UL, DecodeOpSti<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc0200000UL, 0xc1782000UL, DecodeOpSt<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc0302000UL, 0xc1f82000UL, DecodeOpSthi<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc0300000UL, 0xc1782000UL, DecodeOpSth<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc0282000UL, 0xc1f82000UL, DecodeOpStbi<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc0280000UL, 0xc1782000UL, DecodeOpStb<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc1882000UL, 0xc1f82000UL, DecodeOpLdcsri<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc1880000UL, 0xc1f82000UL, DecodeOpLdcsr<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc1982000UL, 0xc1f82000UL, DecodeOpLddci<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc1980000UL, 0xc1f82000UL, DecodeOpLddc<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc1802000UL, 0xc1f82000UL, DecodeOpLdci<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc1800000UL, 0xc1f82000UL, DecodeOpLdc<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc1082000UL, 0xc1f82000UL, DecodeOpLdfsri<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc1080000UL, 0xc1f82000UL, DecodeOpLdfsr<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc1182000UL, 0xc1f82000UL, DecodeOpLddfi<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc1180000UL, 0xc1f82000UL, DecodeOpLddf<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc1002000UL, 0xc1f82000UL, DecodeOpLdfi<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc1000000UL, 0xc1f82000UL, DecodeOpLdf<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc0182000UL, 0xc1f82000UL, DecodeOpLddi<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc0180000UL, 0xc1782000UL, DecodeOpLdd<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc0002000UL, 0xc1f82000UL, DecodeOpLdi<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc0000000UL, 0xc1782000UL, DecodeOpLd<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc0102000UL, 0xc1f82000UL, DecodeOpLduhi<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc0100000UL, 0xc1782000UL, DecodeOpLduh<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc0082000UL, 0xc1f82000UL, DecodeOpLdubi<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc0080000UL, 0xc1782000UL, DecodeOpLdub<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc0502000UL, 0xc1f82000UL, DecodeOpLdshi<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc0500000UL, 0xc1782000UL, DecodeOpLdsh<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc0482000UL, 0xc1f82000UL, DecodeOpLdsbi<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0xc0480000UL, 0xc1782000UL, DecodeOpLdsb<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81802000UL, 0xc1f82000UL, DecodeOpWrasri<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81800000UL, 0xc1f82000UL, DecodeOpWrasr<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81400000UL, 0xc1f80000UL, DecodeOpRdasr<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81d02000UL, 0xc1f82000UL, DecodeOpTci<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x81d00000UL, 0xc1f82000UL, DecodeOpTc<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x1800000UL, 0xc1c00000UL, DecodeOpFbc<	ARCH>));
	decode_table.push_back(DecodeTableEntry<	ARCH>(0x800000UL, 0xc1c00000UL, DecodeOpBc<	ARCH>));
}

template <	typename	ARCH>
Decoder<	ARCH>::~Decoder()
{
}

template <	typename	ARCH>
Operation<	ARCH> *Decoder<	ARCH>::NCDecode(uint32_t addr, CodeType code)
{
	Operation<	ARCH> *operation;
	unsigned int count = decode_table.size();
	unsigned int idx;
	for(idx = 0; idx < count; idx++)
	{
		if((code & decode_table[idx].opcode_mask) == decode_table[idx].opcode)
		{
			operation = decode_table[idx].decode(code, addr);
			return operation;
		}
	}
	operation = new Operation<	ARCH>(code, addr, "???");
	return operation;
}

} } } } } } }
