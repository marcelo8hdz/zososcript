#include "SymbolTable.h"
#include "Parser.h"
#include <iostream>
#include <stdexcept>

namespace Zoso {

SymbolTable::SymbolTable(Parser* parser): undef(0), integer(1), boolean(2), decimal(3), var(0), function(1), scope(2) {
	std::cout << "symbolTable constructor" << std::endl;

	errors = parser -> errors;
	topScope = NULL;
	currentLevel = -1;
    
	undefObj = new Obj();
	undefObj -> name  = coco_string_create("undef"); 
    undefObj -> type = undef; 
    undefObj -> kind = var;
	undefObj -> address= 0; 
    undefObj -> level = 0; 
    undefObj -> next = NULL;
}

void SymbolTable::OpenScope () {
	std::cout << "symbolTable open scope" << std::endl;
	Obj *newScope = new Obj();
	
	newScope -> name = coco_string_create(""); 
	newScope -> kind = scope;
	newScope -> locals = NULL; 
	newScope -> nextAddress = 0;
	newScope -> next = topScope; 
	topScope = newScope;
	
	currentLevel++;
}

void SymbolTable::CloseScope () {
	std::cout << "symbolTable close scope" << std::endl;
	topScope = topScope -> next; 
	currentLevel--;
}
// new object node in current scope 
Obj* SymbolTable::NewObj (wchar_t* name, int kind, int type) {
	std::cout << "symbolTable new object" << std::endl;
	Obj *topScopeNode, *last, *newObject = new Obj();

	newObject -> name = coco_string_create(name); 
    newObject -> kind = kind; 
    newObject -> type = type;
	newObject -> level = currentLevel;

	topScopeNode = topScope -> locals; 
    last = NULL;
    
	while (topScopeNode != NULL) {
		std::cout << "symbolTable while" << std::endl;
		if (coco_string_equal(topScopeNode -> name, name)) throw std::invalid_argument("name already declared");
		last = topScopeNode; 
        topScopeNode = topScopeNode -> next;
	}

	if (last == NULL) topScope -> locals = newObject; 
    else last -> next = newObject;

	if (kind == var) newObject -> address = topScope -> nextAddress++;
	
	return newObject;

}

Obj* SymbolTable::Find (wchar_t* name) {
	std::cout << "symbolTable find" << std::endl;
	Obj *obj, *scope;
	scope = topScope;
	
	while (scope != NULL) {  // for all open scopes
		obj = scope->locals;
		while (obj != NULL) {  // for all objects in this scope
			if (coco_string_equal(obj->name, name)) return obj;
			obj = obj->next;
		}
		scope = scope->next;
	}

	wchar_t str[100];
	std::wstring wstr(str);

	coco_swprintf(str, 100, L"%ls is undeclared", name);
	
	throw std::invalid_argument("Tried to find undeclared variable"); // name is undeclared
	
	return undefObj;
}


}