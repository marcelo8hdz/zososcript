#if !defined(SYMBOLTABLE_H__)
#define SYMBOLTABLE_H__

#include "Scanner.h"
#include <string>

namespace Zoso {

class Parser;
class Errors;

class Obj {  // object describing a declared name
public:
	wchar_t* name;		// name of the object
	int type;		// type of the object 
	Obj* next;		// to next object in same scope
	int kind;		// var, function
	int address;		// address in memory or start of function
	int level;		// nesting level; 0=global, 1=local
	Obj* locals;		// scopes: to locally declared objects
	int nextAddress;	// scopes: next free address in this scope

	Obj() {
		name = NULL;
		type = 0;
		next = NULL;
		kind = 0;
		address = 0;
		level = 0;
		locals = NULL;
		nextAddress = 0;
	}

	~Obj() {
		coco_string_delete(name);
	}


};

class SymbolTable {
public:
	const int undef, integer, boolean, decimal;

	const int var, function, scope;

	int currentLevel;	// nesting level of current scope
	Obj *undefObj;	// object node for erroneous symbols
	Obj *topScope;	// topmost procedure scope

	Errors *errors;

	SymbolTable(Parser *parser);

	// open a new scope and make it the current scope (topScope)
	void OpenScope ();

	// close the current scope
	void CloseScope ();

	// create a new object node in the current scope
	Obj* NewObj (wchar_t* name, int kind, int type);

	// search the name in all open scopes and return its object node
	Obj* Find (wchar_t* name);

};

}; // namespace

#endif // !defined(SYMBOLTABLE_H__)
