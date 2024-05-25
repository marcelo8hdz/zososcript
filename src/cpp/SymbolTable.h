#if !defined(SYMBOLTABLE_H__)
#define SYMBOLTABLE_H__

#include "Scanner.h"
#include <string>
#include <map>

namespace Zoso {

class Parser;
class Errors;

struct Obj {  // variable in symbol table
	wchar_t* name;		// name of the object
	int address;		// address in memory or start of function
	int type;		// type of the object 
	int kind;		// var, function
};

class SymbolTable {
public:
	std::map<std::wstring, Obj> variables;
	
	const int undef, integer, boolean, decimal;
	const int var, function;

	int integerAddress, booleanAddress, floatAddress, voidAddress;

	SymbolTable(Parser *parser);

	Obj NewObj (wchar_t* name, int kind, int type);

	Obj Find (wchar_t* name);
};

}; // namespace

#endif // !defined(SYMBOLTABLE_H__)
