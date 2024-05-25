#if !defined(SYMBOLTABLE_H__)
#define SYMBOLTABLE_H__

#include "Scanner.h"
#include <string>
#include <map>

namespace Zoso {

class Parser;
class Errors;

class Obj {  // variable in symbol table
public:
	wchar_t* name;		// name of the object
	int type;		// type of the object 
	int kind;		// var, function
	int address;		// address in memory or start of function

	Obj(wchar_t* name, int type, int kind, int address) {
		this -> name = name;
		this -> type = type;
		this -> kind = kind;
		this -> address = address;
	}

	~Obj() {
		coco_string_delete(name);
	}

	void PrintObj(int indentLevel);
};

class SymbolTable {
public:
	std::map<wchar_t*, Obj> variables;
	
	const int undef, integer, boolean, decimal;
	const int var, function;

	int integerAddress, booleanAddress, floatAddress, voidAddress;

	SymbolTable(Parser *parser);

	Obj NewObj (wchar_t* name, int kind, int type);

	Obj Find (wchar_t* name);
};

}; // namespace

#endif // !defined(SYMBOLTABLE_H__)
