#include "SymbolTable.h"
#include "Parser.h"
#include <iostream>
#include <stdexcept>

namespace Zoso {

SymbolTable::SymbolTable(Parser* parser): undef(0), integer(1), boolean(2), decimal(3), var(0), function(1), scope(2) {
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
	Obj *newScope = new Obj();
	
	newScope -> name = coco_string_create(""); 
	newScope -> kind = scope;
	newScope -> locals = NULL; 
	newScope -> nextAddress = 0;
	newScope -> next = topScope; 
	topScope = newScope;
	
	currentLevel++;
}

// new object node in current scope 
Obj* SymbolTable::NewObj (wchar_t* name, int kind, int type) {
	Obj *topScopeNode, *last, *newObject = new Obj();

	newObject -> name = coco_string_create(name); 
    newObject -> kind = kind; 
    newObject -> type = type;
	newObject -> level = currentLevel;

	topScopeNode = topScope -> locals; 
    last = NULL;
    
	while (topScopeNode != NULL) {
		if (coco_string_equal(topScopeNode -> name, name)) throw std::invalid_argument("name already declared");
		last = topScopeNode; 
        topScopeNode = topScopeNode -> next;
	}

	if (last == NULL) topScope -> locals = newObject; 
    else last -> next = newObject;

	if (kind == var) newObject -> address = topScope -> nextAddress++;
	
	return newObject;

}

}