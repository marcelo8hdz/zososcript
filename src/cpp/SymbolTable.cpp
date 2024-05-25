#include "SymbolTable.h"
#include "Parser.h"
#include <iostream>
#include <stdexcept>

namespace Zoso {

SymbolTable::SymbolTable(Parser* parser): undef(0), integer(1), boolean(2), decimal(3), var(0), function(1) {
	integerAddress = 5000;
	floatAddress = 4000;
	booleanAddress = 3000;
	voidAddress = 2000;
}

// new object node in current scope 
Obj SymbolTable::NewObj (wchar_t* name, int kind, int type) {
	int address;
	if (type == integer) address = integerAddress++;
	else if (type == decimal) address = floatAddress++;
	else if (type == boolean) address = booleanAddress++;
	else if (type == undef) address = voidAddress++;

	
	if (variables.count(name)) throw std::invalid_argument("This variable already exists!");

	variables[name] = {name, address, type, kind};

	std::wcout << name << address << std::endl;
	return variables[name];

}

Obj SymbolTable::Find(wchar_t* name) {
	if (!variables.count(name)) throw std::invalid_argument("Tried to find undeclared variable"); // name is undeclared
	
	return variables[name];
}

}