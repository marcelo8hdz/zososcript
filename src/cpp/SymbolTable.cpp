#include "SymbolTable.h"
#include "Parser.h"
#include <iostream>
#include <stdexcept>

namespace Zoso {

SymbolTable::SymbolTable(Parser* parser): undef(0), integer(1), boolean(2), decimal(3), var(0), function(1) {
	integerAddress = 5000 - 1;
	floatAddress = 4000 - 1;
	booleanAddress = 3000 - 1;
	voidAddress = 2000 - 1;

	// -1 for code readabililty (set address to intAddress++ in newObj)
}

// new object node in current scope 
Obj SymbolTable::NewObj (wchar_t* name, int kind, int type) {
	int address;
	if (type == integer) address = integerAddress++;
	else if (type == decimal) address = floatAddress++;
	else if (type == boolean) address = booleanAddress++;
	else if (type == undef) address = voidAddress++;
	
	if (variables.count(coco_string_create(name))) throw std::invalid_argument("This variable already exists!");

	variables[coco_string_create(name)] = Obj(coco_string_create(name), address, type, kind);
    
	return variables[coco_string_create(name)];

}

Obj SymbolTable::Find(wchar_t* name) {
	if (!variables.count(coco_string_create(name))) throw std::invalid_argument("Tried to find undeclared variable"); // name is undeclared
	
	return variables[name];
}

}