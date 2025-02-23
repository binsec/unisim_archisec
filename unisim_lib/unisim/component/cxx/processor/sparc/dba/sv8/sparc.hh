/*** File generated by GenISSLib version 4.1.0: DO NOT MODIFY ***/
#ifndef __unisim_component_cxx_processor_sparc_dba_sv8_sparc_unisim__component__cxx__processor__sparc__dba__sv8_HH__
#define __unisim_component_cxx_processor_sparc_dba_sv8_sparc_unisim__component__cxx__processor__sparc__dba__sv8_HH__

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include <vector>
#include <inttypes.h>
#include <cstring>
namespace unisim { namespace component { namespace cxx { namespace processor { namespace sparc { namespace dba { namespace sv8 {
typedef uint32_t CodeType;

template <	typename	ARCH>
struct Operation;
template <	typename	ARCH>
struct DecodeTableEntry
{
	DecodeTableEntry(CodeType opcode, CodeType opcode_mask, Operation<	ARCH> *(*decode)(CodeType, uint32_t));
	CodeType opcode;
	CodeType opcode_mask;
	Operation<	ARCH> *(*decode)(CodeType code, uint32_t addr);
};
template <	typename	ARCH>
struct Decoder
{
	typedef Operation<	ARCH> operation_type;
	typedef uint32_t address_type;
	typedef CodeType code_type;
	enum { little_endian = 0, alignment = 32, minsize = 32, maxsize = 32 };
	Decoder();
	virtual ~Decoder();

	Operation<	ARCH>*NCDecode(uint32_t addr);
	Operation<	ARCH> *NCDecode(uint32_t addr, CodeType code);
	std::vector<DecodeTableEntry<	ARCH> > const& GetDecodeTable() const { return decode_table; };
	void SetLittleEndian();
	void SetBigEndian();
private:
	std::vector<DecodeTableEntry<	ARCH> > decode_table;
};

} } } } } } }

#include <iosfwd>
#include <inttypes.h>
namespace unisim { namespace component { namespace cxx { namespace processor { namespace sparc { namespace dba { namespace sv8 {
template <	typename	ARCH>
struct Operation
{
	Operation(CodeType code, uint32_t addr, const char *name);
	virtual ~Operation();
	inline uint32_t GetAddr() const { return addr; }
	inline void SetAddr(uint32_t _addr) { this->addr = _addr; }
	inline CodeType GetEncoding() const { return encoding; }
	inline unsigned int GetLength() const { return 32; }
	inline const char *GetName() const { return name; }
	inline bool Match(uint32_t _addr, CodeType _code) const { return (this->GetEncoding() ^ _code) == 0 and GetAddr() == _addr; }
	typedef Decoder<	ARCH> decoder_type;
	virtual void disasm( std::ostream& sink,
	uint32_t pc ) const;
	virtual void execute( ARCH& cpu ) const;
protected:
	const char *name;
	uint32_t addr;
	CodeType encoding;
};

} } } } } } }
#endif
