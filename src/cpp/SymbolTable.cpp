#include "SymbolTable.h"
#include "Parser.h"
#include <iostream>
#include <stdexcept>

namespace Zoso {

SymbolTable::SymbolTable(Parser* parser): undef(0), integer(1), boolean(2), decimal(3), var(0), function(1), scope(2) {
	errors = parser -> errors;
	topScope = NULL;
	curLevel = -1;
    
	undefObj = new Obj();
	undefObj -> name  = coco_string_create("undef"); 
    undefObj -> type = undef; 
    undefObj -> kind = var;
	undefObj -> adr = 0; 
    undefObj -> level = 0; 
    undefObj -> next = NULL;
}

// create a new object node in the current scope
Obj* SymbolTable::NewObj (wchar_t* name, int kind, int type) {
	Obj *p, *last, *obj = new Obj();

	obj -> name = coco_string_create(name); 
    obj -> kind = kind; 
    obj -> type = type;
	obj -> level = curLevel;

	p = topScope -> locals; 
    last = NULL;
    
	while (p != NULL) {
		if (coco_string_equal(p -> name, name)) throw std::invalid_argument("name already declared");
		last = p; 
        p = p -> next;
	}

	if (last == NULL) topScope -> locals = obj; 
    else last -> next = obj;
	if (kind == var) obj -> adr = topScope -> nextAdr++;
	return obj;

}

}