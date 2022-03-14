/*
 *  Copyright (c) 2013,
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
 * Authors: Gilles Mouchard (gilles.mouchard@cea.fr)
 */

#include <unisim/util/debug/type.hh>
#include <iostream>
#include <sstream>
#include <queue>
#include <cassert>

namespace unisim {
namespace util {
namespace debug {

Type::Type()
	: type_class(T_UNKNOWN)
	, decl_location(0)
	, ref_count(0)
{
}

Type::Type(const DeclLocation *_decl_location)
	: type_class(T_UNKNOWN)
	, decl_location(_decl_location)
	, ref_count(0)
{
	if(decl_location) decl_location->Catch();
}

Type::Type(TYPE_CLASS _type_class)
	: type_class(_type_class)
	, decl_location(0)
	, ref_count(0)
{
}

Type::Type(TYPE_CLASS _type_class, const DeclLocation *_decl_location)
	: type_class(_type_class)
	, decl_location(_decl_location)
	, ref_count(0)
{
	if(decl_location) decl_location->Catch();
}

Type::~Type()
{
	if(decl_location) decl_location->Release();
}

TYPE_CLASS Type::GetClass() const
{
	return type_class;
}

bool Type::IsComposite() const
{
	return (type_class == unisim::util::debug::T_STRUCT) ||
	       (type_class == unisim::util::debug::T_UNION) ||
	       (type_class == unisim::util::debug::T_CLASS) ||
	       (type_class == unisim::util::debug::T_INTERFACE);
}

bool Type::IsBase() const
{
	return (type_class == unisim::util::debug::T_INTEGER) ||
	       (type_class == unisim::util::debug::T_CHAR) ||
	       (type_class == unisim::util::debug::T_FLOAT) ||
	       (type_class == unisim::util::debug::T_BOOL);
}

bool Type::IsNamed() const
{
	return IsBase() ||
	       IsComposite() ||
	       (type_class == T_TYPEDEF) ||
	       (type_class == T_ENUM);
}

const DeclLocation *Type::GetDeclLocation() const
{
	return decl_location;
}

void Type::DFS(const std::string& path, const TypeVisitor *visitor, bool follow_pointer) const
{
	visitor->Visit(path.c_str(), this, TINIT_TOK_LITERAL);
}

std::string Type::BuildCDecl(char const **identifier, bool collapsed) const
{
	return std::string("void");
}

void Type::Catch() const
{
	++ref_count;
}

void Type::Release() const
{
	if(ref_count)
	{
		if(--ref_count == 0)
		{
			delete this;
		}
	}
}

std::ostream& operator << (std::ostream& os, const Type& type)
{
	return os << type.BuildCDecl();
}

NamedType::NamedType(TYPE_CLASS _type_class, const char *_name)
	: Type(_type_class)
	, name(_name ? _name : "")
	, has_name(_name != 0)
{
}

NamedType::NamedType(TYPE_CLASS _type_class, const char *_name, const DeclLocation *_decl_location)
	: Type(_type_class, _decl_location)
	, name(_name ? _name : "")
	, has_name(_name != 0)
{
}

const std::string& NamedType::GetName() const
{
	return name;
}

bool NamedType::HasName() const
{
	return has_name;
}

BaseType::BaseType(const char *_name, unsigned int _bit_size)
	: NamedType(T_UNKNOWN, _name)
	, bit_size(_bit_size)
{
}

BaseType::BaseType(const char *_name, unsigned int _bit_size, const DeclLocation *_decl_location)
	: NamedType(T_UNKNOWN, _name, _decl_location)
	, bit_size(_bit_size)
{
}

BaseType::BaseType(TYPE_CLASS _type_class, const char *_name, unsigned int _bit_size)
	: NamedType(_type_class, _name)
	, bit_size(_bit_size)
{
}

BaseType::BaseType(TYPE_CLASS _type_class, const char *_name, unsigned int _bit_size, const DeclLocation *_decl_location)
	: NamedType(_type_class, _name, _decl_location)
	, bit_size(_bit_size)
{
}

BaseType::~BaseType()
{
}

unsigned int BaseType::GetBitSize() const
{
	return bit_size;
}

IntegerType::IntegerType(const char *_name, unsigned int _bit_size, bool _is_signed)
	: BaseType(T_INTEGER, _name, _bit_size)
	, is_signed(_is_signed)
{
}

IntegerType::IntegerType(const char *_name, unsigned int _bit_size, bool _is_signed, const DeclLocation *_decl_location)
	: BaseType(T_INTEGER, _name, _bit_size, _decl_location)
	, is_signed(_is_signed)
{
}

IntegerType::~IntegerType()
{
}

bool IntegerType::IsSigned() const
{
	return is_signed;
}

std::string IntegerType::BuildCDecl(char const **identifier, bool collapsed) const
{
	std::stringstream sstr;
	const std::string& name = GetName();
	if(name.empty())
	{
		sstr << (is_signed ? "s" : "u") << "int";
		uint64_t bit_size = GetBitSize();
		if(bit_size) sstr << bit_size;
	}
	else
	{
		sstr << name;
	}
	return sstr.str();
}

CharType::CharType(const char *_name, unsigned int _bit_size, bool _is_signed)
	: BaseType(T_CHAR, _name, _bit_size)
	, is_signed(_is_signed)
{
}

CharType::CharType(const char *_name, unsigned int _bit_size, bool _is_signed, const DeclLocation *_decl_location)
	: BaseType(T_CHAR, _name, _bit_size, _decl_location)
	, is_signed(_is_signed)
{
}

CharType::~CharType()
{
}

bool CharType::IsSigned() const
{
	return is_signed;
}

std::string CharType::BuildCDecl(char const **identifier, bool collapsed) const
{
	std::stringstream sstr;
	const std::string& name = GetName();
	if(name.empty())
	{
		sstr << (is_signed ? "s" : "u") << "char";
		uint64_t bit_size = GetBitSize();
		
		if(bit_size) sstr << bit_size;
	}
	else
	{
		sstr << name;
	}
	return sstr.str();
}

FloatingPointType::FloatingPointType(const char *_name, unsigned int bit_size)
	: BaseType(T_FLOAT, _name, bit_size)
{
}

FloatingPointType::FloatingPointType(const char *_name, unsigned int bit_size, const DeclLocation *_decl_location)
	: BaseType(T_FLOAT, _name, bit_size, _decl_location)
{
}

FloatingPointType::~FloatingPointType()
{
}

std::string FloatingPointType::BuildCDecl(char const **identifier, bool collapsed) const
{
	std::stringstream sstr;
	const std::string& name = GetName();
	if(name.empty())
	{
		sstr << "float";
		uint64_t bit_size = GetBitSize();
		if(bit_size) sstr << bit_size;
	}
	else
	{
		sstr << name;
	}
	
	return sstr.str();
}

BooleanType::BooleanType(const char *_name, unsigned int _bit_size)
	: BaseType(T_BOOL, _name, _bit_size)
{
}

BooleanType::BooleanType(const char *_name, unsigned int _bit_size, const DeclLocation *_decl_location)
	: BaseType(T_BOOL, _name, _bit_size, _decl_location)
{
}

BooleanType::~BooleanType()
{
}

std::string BooleanType::BuildCDecl(char const **identifier, bool collapsed) const
{
	std::stringstream sstr;
	const std::string& name = GetName();
	if(name.empty())
	{
		sstr << "bool";
		uint64_t bit_size = GetBitSize();
		if(bit_size) sstr << bit_size;
	}
	else
	{
		sstr << name;
	}
	return sstr.str();
}

Member::Member(const char *_name, const Type *_type, uint64_t _bit_size)
	: name(_name ? _name : "")
	, has_name(_name != 0)
	, type(_type)
	, bit_size(_bit_size)
{
	if(type)
	{
		type->Catch();
	}
}

Member::~Member()
{
	if(type)
	{
		type->Release();
	}
}

const char *Member::GetName() const
{
	return name.c_str();
}

bool Member::HasName() const
{
	return has_name;
}

const Type *Member::GetType() const
{
	return type;
}

uint64_t Member::GetBitSize() const
{
	return bit_size;
}

void Member::DFS(const std::string& path, const TypeVisitor *visitor, bool follow_pointer) const
{
	visitor->Visit(this);
	std::string member_path = path;
	if(has_name)
	{
		member_path += '.';
		member_path += name;
	}
	type->DFS(member_path, visitor, follow_pointer);
}

std::string Member::BuildCDecl() const
{
	std::stringstream sstr;
	char const *member_name = name.c_str();
	std::string s(type->BuildCDecl(&member_name, true));
	sstr << s;
	if(member_name)
	{
		if(!s.empty() && (s.back() != ' ') && (s.back() != '*')) sstr << " ";
		sstr << member_name;
	}
	if(bit_size)
	{
		sstr << ":" << bit_size;
	}
	return sstr.str();
}

CompositeType::CompositeType(TYPE_CLASS _type_class, const char *_name, bool _incomplete)
	: NamedType(_type_class, _name)
	, incomplete(_incomplete)
	, members()
{
}

CompositeType::CompositeType(TYPE_CLASS _type_class, const char *_name, bool _incomplete, const DeclLocation *_decl_location)
	: NamedType(_type_class, _name, _decl_location)
	, incomplete(_incomplete)
	, members()
{
}

CompositeType::~CompositeType()
{
	unsigned int member_count = members.size();
	unsigned int i;
	for(i = 0; i < member_count; i++)
	{
		delete members[i];
	}
}

void CompositeType::Add(const Member *_member)
{
	members.push_back(_member);
}

bool CompositeType::IsIncomplete() const
{
	return incomplete;
}

unsigned int CompositeType::GetMemberCount() const
{
	return members.size();
}

const Member *CompositeType::GetMember(unsigned int _idx) const
{
	unsigned int member_count = members.size();
	
	return (_idx < member_count) ? members[_idx] : 0;
}

void CompositeType::DFS(const std::string& path, const TypeVisitor *visitor, bool follow_pointer) const
{
	visitor->Visit(path.c_str(), this, TINIT_TOK_BEGIN_OF_STRUCT);
	unsigned int member_count = members.size();
	unsigned int i;
	for(i = 0; i < member_count; i++)
	{
		const Member *member = members[i];
		member->DFS(path, visitor, follow_pointer);
		if(i != (member_count - 1))
		{
			visitor->Visit(path.c_str(), this, TINIT_TOK_STRUCT_MEMBER_SEPARATOR);
		}
	}
	visitor->Visit(path.c_str(), this, TINIT_TOK_END_OF_STRUCT);
}

std::string CompositeType::BuildCDecl(char const **identifier, bool collapsed) const
{
	std::stringstream sstr;
	
	switch(GetClass())
	{
		case T_STRUCT:
			sstr << "struct";
			break;
		case T_UNION:
			sstr << "union";
			break;
		case T_CLASS:
			sstr << "class";
			break;
		case T_INTERFACE:
			sstr << "interface";
			break;
		default:
			assert(false);
	}
	if(HasName())
	{
		sstr << " " << GetName();
	}
	if((!collapsed || !HasName()) && !IsIncomplete())
	{
		sstr << " { ";
		unsigned int member_count = members.size();
		if(member_count)
		{
			unsigned int i;
			for(i = 0; i < member_count; i++)
			{
				Member const *member = members[i];
				Type const *member_type = member->GetType();
				if(member_type->GetClass() == T_ARRAY)
				{
					ArrayType const *array_type = dynamic_cast<ArrayType const *>(member_type);
					if((array_type->GetCount() == 0) && ((GetClass() != T_STRUCT) || (i != (member_count - 1))))
					{
						 continue;
					}
				}
				sstr << member->BuildCDecl() << "; ";
			}
		}
		sstr << "}";
	}
	return sstr.str();
}

ArrayType::ArrayType(const Type *_type_of_element, unsigned int _order, int64_t _lower_bound, int64_t _upper_bound)
	: Type(T_ARRAY)
	, type_of_element(_type_of_element)
	, order(_order)
	, lower_bound(_lower_bound)
	, upper_bound(_upper_bound)
{
	if(type_of_element)
	{
		type_of_element->Catch();
	}
}

ArrayType::ArrayType(const Type *_type_of_element, unsigned int _order, int64_t _lower_bound, int64_t _upper_bound, const DeclLocation *_decl_location)
	: Type(T_ARRAY, _decl_location)
	, type_of_element(_type_of_element)
	, order(_order)
	, lower_bound(_lower_bound)
	, upper_bound(_upper_bound)
{
	if(type_of_element)
	{
		type_of_element->Catch();
	}
}

ArrayType::~ArrayType()
{
	if(type_of_element)
	{
		type_of_element->Release();
	}
}

const Type *ArrayType::GetTypeOfElement() const
{
	return type_of_element;
}

unsigned int ArrayType::GetOrder() const
{
	return order;
}

int64_t ArrayType::GetLowerBound() const
{
	return lower_bound;
}

int64_t ArrayType::GetUpperBound() const
{
	return upper_bound;
}

int64_t ArrayType::GetCount() const
{
	return upper_bound - lower_bound + 1;
}

void ArrayType::DFS(const std::string& path, const TypeVisitor *visitor, bool follow_pointer) const
{
	visitor->Visit(path.c_str(), this, TINIT_TOK_BEGIN_OF_ARRAY);
	int64_t subscript;
	for(subscript = lower_bound; subscript <= upper_bound; subscript++)
	{
		std::stringstream element_path_sstr;
		element_path_sstr << path << '[' << subscript << ']';
		type_of_element->DFS(element_path_sstr.str(), visitor, follow_pointer);
		if(subscript != upper_bound)
		{
			visitor->Visit(path.c_str(), this, TINIT_TOK_ARRAY_ELEMENT_SEPARATOR);
		}
	}
	visitor->Visit(path.c_str(), this, TINIT_TOK_END_OF_ARRAY);
}

std::string ArrayType::BuildCDecl(char const **identifier, bool collapsed) const
{
	std::stringstream sstr;
	if(identifier && (*identifier))
	{
		sstr << (*identifier);
		*identifier = 0;
	}
	const ArrayType *a = 0;
	const Type *t = this; 
	do
	{
		a = (const ArrayType *) t;
		if(a->lower_bound <= a->upper_bound)
		{
			if(a->lower_bound)
				sstr << '[' << a->lower_bound << ".." << a->upper_bound << ']';
			else
				sstr << '[' << a->upper_bound + 1 << ']';
		}
		else
		{
			sstr << "[]";
		}
	}
	while((t = a->type_of_element)->GetClass() == T_ARRAY);
	std::string s(sstr.str());
	char const *s_cstr = s.c_str();
	
	std::stringstream sstr2;
	sstr2 << t->BuildCDecl(&s_cstr, true);
	if(s_cstr)
	{
		sstr2 << " " << s;
	}
	
	return sstr2.str();
}

PointerType::PointerType(const Type *_type_of_dereferenced_object)
	: Type(T_POINTER)
	, type_of_dereferenced_object(_type_of_dereferenced_object)
{
	if(type_of_dereferenced_object)
	{
		type_of_dereferenced_object->Catch();
	}
}

PointerType::PointerType(const Type *_type_of_dereferenced_object, const DeclLocation *_decl_location)
	: Type(T_POINTER, _decl_location)
	, type_of_dereferenced_object(_type_of_dereferenced_object)
{
	if(type_of_dereferenced_object)
	{
		type_of_dereferenced_object->Catch();
	}
}

PointerType::~PointerType()
{
	if(type_of_dereferenced_object)
	{
		type_of_dereferenced_object->Release();
	}
}

const Type *PointerType::GetTypeOfDereferencedObject() const
{
	return type_of_dereferenced_object;
}

bool PointerType::IsNullTerminatedStringPointer() const
{
	if(type_of_dereferenced_object->GetClass() == T_CHAR)
	{
		// char *
		return true;
	}
	else
	{
		if(type_of_dereferenced_object->GetClass() == T_CONST)
		{
			const ConstType *const_type = static_cast<const ConstType *>(type_of_dereferenced_object);
			if(const_type->GetType()->GetClass() == T_VOLATILE)
			{
				const VolatileType *volatile_type = static_cast<const VolatileType *>(const_type->GetType());
				if(volatile_type->GetType()->GetClass() == T_CHAR)
				{
					// char volatile const *
					return true;
				}
			}
			else
			{
				if(const_type->GetType()->GetClass() == T_CHAR)
				{
					// char const *
					return true;
				}
			}
		}
		else if(type_of_dereferenced_object->GetClass() == T_VOLATILE)
		{
			const VolatileType *volatile_type = static_cast<const VolatileType *>(type_of_dereferenced_object);
			if(volatile_type->GetType()->GetClass() == T_CONST)
			{
				const ConstType *const_type = static_cast<const ConstType *>(volatile_type->GetType());
				if(const_type->GetClass() == T_CHAR)
				{
					// char const volatile *
					return true;
				}
			}
			else
			{
				if(volatile_type->GetType()->GetClass() == T_CHAR)
				{
					// char volatile *
					return true;
				}
			}
		}
	}
	
	return false;
}

void PointerType::DFS(const std::string& path, const TypeVisitor *visitor, bool follow_pointer) const
{
	if(follow_pointer)
	{
		std::string dereferenced_object_path = std::string("*(") + std::string(path) + ')';
		type_of_dereferenced_object->DFS(dereferenced_object_path, visitor, follow_pointer);
	}
	else
	{
		visitor->Visit(path.c_str(), this, TINIT_TOK_LITERAL);
	}
}

std::string PointerType::BuildCDecl(char const **identifier, bool collapsed) const
{
	const Type *nested_type = type_of_dereferenced_object;
	do
	{
		if(nested_type->GetClass() == T_FUNCTION) break;
		if(nested_type->GetClass() == T_ARRAY) break;
		if(nested_type->GetClass() == T_CONST)
		{
			nested_type = static_cast<const ConstType *>(nested_type)->GetType();
		}
		if(nested_type->GetClass() == T_VOLATILE)
		{
			nested_type = static_cast<const VolatileType *>(nested_type)->GetType();
		}
		else
		{
			break;
		}
	}
	while(1);
	
	std::stringstream sstr;
	if((nested_type->GetClass() == T_FUNCTION) || (nested_type->GetClass() == T_ARRAY))
	{
		std::string s("(*");
		if(identifier && (*identifier))
		{
			s += (*identifier);
		}
		s += ')';
		char const *s_cstr = s.c_str();
		sstr << type_of_dereferenced_object->BuildCDecl(&s_cstr);
		if(identifier && (*identifier) && !s_cstr)
		{
			*identifier = 0;
		}
	}
	else
	{
		std::string s(type_of_dereferenced_object->BuildCDecl(0, true));
		sstr << s;
		if(!s.empty() && (s.back() != '*') && (s.back() != ' ')) sstr << " ";
		sstr << "*";
	}
	return sstr.str();
}

Typedef::Typedef(const Type *_type, const char *_name)
	: NamedType(T_TYPEDEF, _name)
	, type(_type)
{
	if(type)
	{
		type->Catch();
	}
}

Typedef::Typedef(const Type *_type, const char *_name, const DeclLocation *_decl_location)
	: NamedType(T_TYPEDEF, _name, _decl_location)
	, type(_type)
{
	if(type)
	{
		type->Catch();
	}
}

Typedef::~Typedef()
{
	if(type)
	{
		type->Release();
	}
}

const Type *Typedef::GetType() const
{
	return type;
}

std::string Typedef::BuildCDecl(char const **identifier, bool collapsed) const
{
	if(collapsed)
	{
		return std::string(GetName());
	}
	else
	{
		std::stringstream sstr;
		sstr << "typedef ";
		char const *typedef_name = GetName().c_str();
		sstr << type->BuildCDecl(&typedef_name, true);
		if(typedef_name)
		{
			sstr << " " << typedef_name;
		}
		return sstr.str();
	}
}

void Typedef::DFS(const std::string& path, const TypeVisitor *visitor, bool follow_pointer) const
{
	type->DFS(path, visitor, follow_pointer);
}

FormalParameter::FormalParameter(const char *_name, const Type *_type)
	: name(_name)
	, type(_type)
{
	if(type)
	{
		type->Catch();
	}
}

FormalParameter::~FormalParameter()
{
	if(type)
	{
		type->Release();
	}
}

const char *FormalParameter::GetName() const
{
	return name.c_str();
}

const Type *FormalParameter::GetType() const
{
	return type;
}

FunctionType::FunctionType(const Type *_return_type)
	: Type(T_FUNCTION)
	, return_type(_return_type)
	, formal_params()
{
	if(return_type)
	{
		return_type->Catch();
	}
}

FunctionType::FunctionType(const Type *_return_type, const DeclLocation *_decl_location)
	: Type(T_FUNCTION, _decl_location)
	, return_type(_return_type)
	, formal_params()
{
	if(return_type)
	{
		return_type->Catch();
	}
}

FunctionType::~FunctionType()
{
	unsigned int formal_param_count = formal_params.size();
	unsigned int i;
	for(i = 0; i < formal_param_count; i++)
	{
		delete formal_params[i];
	}
	if(return_type)
	{
		return_type->Release();
	}
}

void FunctionType::Add(const FormalParameter *formal_param)
{
	formal_params.push_back(formal_param);
}

std::string FunctionType::BuildCDecl(char const **identifier, bool collapsed) const
{
	std::stringstream sstr;
	
	if(return_type)
	{
		std::string s(return_type->BuildCDecl(0, true));
		sstr << s;
		if(!s.empty() && (s.back() != ' ') && (s.back() != '*')) sstr << " ";
	}
	else
	{
		sstr << "void ";
	}
	if(identifier && (*identifier))
	{
		sstr << (*identifier);
		*identifier = 0;
	}
	sstr << "(";
	unsigned int formal_param_count = formal_params.size();
	if(formal_param_count)
	{
		unsigned int i;
		for(i = 0; i < formal_param_count; i++)
		{
			sstr << formal_params[i]->GetType()->BuildCDecl(0, true);
			if(i != (formal_param_count - 1)) sstr << ", ";
		}
	}
	sstr << ")";
	return sstr.str();
}

ConstType::ConstType(const Type *_type)
	: Type(T_CONST)
	, type(_type)
{
	if(type)
	{
		type->Catch();
	}
}

ConstType::ConstType(const Type *_type, const DeclLocation *_decl_location)
	: Type(T_CONST, _decl_location)
	, type(_type)
{
	if(type)
	{
		type->Catch();
	}
}

ConstType::~ConstType()
{
	if(type)
	{
		type->Release();
	}
}

const Type *ConstType::GetType() const
{
	return type;
}

void ConstType::DFS(const std::string& path, const TypeVisitor *visitor, bool follow_pointer) const
{
	type->DFS(path, visitor, follow_pointer);
}

std::string ConstType::BuildCDecl(char const **identifier, bool collapsed) const
{
	if(type->GetClass() != T_ARRAY)
	{
		std::stringstream sstr;
	
		if(identifier && (*identifier))
		{
			std::string s("const");
			s += ' ';
			s += (*identifier);
			char const *s_cstr = s.c_str();
			sstr << type->BuildCDecl(&s_cstr, collapsed);
			if(!s_cstr)
			{
				*identifier = 0;
			}
		}
		else
		{
			sstr << type->BuildCDecl(0, collapsed);
			sstr << " const";
		}
		return sstr.str();
	}

	// Discarding const for arrays
	return type->BuildCDecl(identifier, collapsed);
}

Enumerator::Enumerator(const char *_name, int64_t _value)
	: name(_name)
	, sign(_value < 0)
	, value(sign ? (-_value) : _value)
{
}

Enumerator::Enumerator(const char *_name, uint64_t _value)
	: name(_name)
	, sign(false)
	, value(_value)
{
}

Enumerator::~Enumerator()
{
}

std::ostream& operator << (std::ostream& os, const Enumerator& enumerator)
{
	os << enumerator.name << " = ";
	if(enumerator.sign)
	{
		os << (int64_t)(-enumerator.value);
	}
	else
	{
		os << enumerator.value;
	}
	return os;
}

EnumType::EnumType(const char *_name)
	: NamedType(T_ENUM, _name)
	, enumerators()
{
}

EnumType::EnumType(const char *_name, const DeclLocation *_decl_location)
	: NamedType(T_ENUM, _name, _decl_location)
	, enumerators()
{
}

EnumType::~EnumType()
{
	unsigned int enumerator_count = enumerators.size();
	unsigned int i;
	for(i = 0; i < enumerator_count; i++)
	{
		delete enumerators[i];
	}
}

void EnumType::Add(const Enumerator *enumerator)
{
	enumerators.push_back(enumerator);
}

std::string EnumType::BuildCDecl(char const **identifier, bool collapsed) const
{
	std::stringstream sstr;
	
	sstr << "enum";
	if(HasName())
	{
		sstr << " " << GetName();
	}
	unsigned int enumerator_count = enumerators.size();
	if(enumerator_count && (!collapsed || !HasName()))
	{
		sstr << " { ";
		unsigned int i;
		for(i = 0; i < enumerator_count; i++)
		{
			if(i != 0) sstr << ", ";
			const Enumerator *enumerator = enumerators[i];
			sstr << (*enumerator);
		}
		sstr << " }";
	}
	return sstr.str();
}

UnspecifiedType::UnspecifiedType()
	: Type(T_VOID)
{
}

UnspecifiedType::UnspecifiedType(const DeclLocation *_decl_location)
	: Type(T_VOID, _decl_location)
{
}

UnspecifiedType::~UnspecifiedType()
{
}

std::string UnspecifiedType::BuildCDecl(char const **identifier, bool collapsed) const
{
	return std::string("void");
}

VolatileType::VolatileType(const Type *_type)
	: Type(T_VOLATILE)
	, type(_type)
{
	if(type)
	{
		type->Catch();
	}
}

VolatileType::VolatileType(const Type *_type, const DeclLocation *_decl_location)
	: Type(T_VOLATILE, _decl_location)
	, type(_type)
{
	if(type)
	{
		type->Catch();
	}
}

VolatileType::~VolatileType()
{
	if(type)
	{
		type->Release();
	}
}

const Type *VolatileType::GetType() const
{
	return type;
}

void VolatileType::DFS(const std::string& path, const TypeVisitor *visitor, bool follow_pointer) const
{
	type->DFS(path, visitor, follow_pointer);
}

std::string VolatileType::BuildCDecl(char const **identifier, bool collapsed) const
{
	if(type->GetClass() != T_ARRAY)
	{
		std::stringstream sstr;
	
		if(identifier && (*identifier))
		{
			std::string s("volatile");
			s += ' ';
			s += (*identifier);
			char const *s_cstr = s.c_str();
			sstr << type->BuildCDecl(&s_cstr, collapsed);
			if(!s_cstr)
			{
				*identifier = 0;
			}
		}
		else
		{
			sstr << type->BuildCDecl(0, collapsed);
			sstr << " volatile";
		}
		return sstr.str();
	}
	
	// Discarding volatile for arrays
	return type->BuildCDecl(identifier, collapsed);
}

} // end of namespace debug
} // end of namespace util
} // end of namespace unisim
