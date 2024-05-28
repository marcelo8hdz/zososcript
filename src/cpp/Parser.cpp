

#include <wchar.h>
#include "Parser.h"
#include "Scanner.h"


namespace Zoso {


void Parser::SynErr(int n) {
	if (errDist >= minErrDist) errors->SynErr(la->line, la->col, n);
	errDist = 0;
}

void Parser::SemErr(const wchar_t* msg) {
	if (errDist >= minErrDist) errors->Error(t->line, t->col, msg);
	errDist = 0;
}

void Parser::Get() {
	for (;;) {
		t = la;
		la = scanner->Scan();
		if (la->kind <= maxT) { ++errDist; break; }

		if (dummyToken != t) {
			dummyToken->kind = t->kind;
			dummyToken->pos = t->pos;
			dummyToken->col = t->col;
			dummyToken->line = t->line;
			dummyToken->next = NULL;
			coco_string_delete(dummyToken->val);
			dummyToken->val = coco_string_create(t->val);
			t = dummyToken;
		}
		la = t;
	}
}

void Parser::Expect(int n) {
	if (la->kind==n) Get(); else { SynErr(n); }
}

void Parser::ExpectWeak(int n, int follow) {
	if (la->kind == n) Get();
	else {
		SynErr(n);
		while (!StartOf(follow)) Get();
	}
}

bool Parser::WeakSeparator(int n, int syFol, int repFol) {
	if (la->kind == n) {Get(); return true;}
	else if (StartOf(repFol)) {return false;}
	else {
		SynErr(n);
		while (!(StartOf(syFol) || StartOf(repFol) || StartOf(0))) {
			Get();
		}
		return StartOf(syFol);
	}
}

void Parser::Ident(wchar_t* &name) {
		Expect(_ident);
		name = coco_string_create(t -> val); 
}

void Parser::AddOp(int& op) {
		if (la->kind == 5 /* "+" */) {
			Get();
			op = ADD; codeGenerator -> operatorStack.push(op); 
		} else if (la->kind == 6 /* "-" */) {
			Get();
			op = SUB; codeGenerator -> operatorStack.push(op); 
		} else SynErr(33);
}

void Parser::MulOp(int& op) {
		if (la->kind == 7 /* "*" */) {
			Get();
			op = MUL; codeGenerator -> operatorStack.push(op); 
		} else if (la->kind == 8 /* "/" */) {
			Get();
			op = DIV; codeGenerator -> operatorStack.push(op); 
		} else SynErr(34);
}

void Parser::RelOp(int& op) {
		if (la->kind == 9 /* "==" */) {
			Get();
			op = EQU; codeGenerator -> operatorStack.push(op); 
		} else if (la->kind == 10 /* "<" */) {
			Get();
			op = LSS; codeGenerator -> operatorStack.push(op); 
		} else if (la->kind == 11 /* ">" */) {
			Get();
			op = GTR; codeGenerator -> operatorStack.push(op); 
		} else if (la->kind == 12 /* "!=" */) {
			Get();
			op = NEQU; codeGenerator -> operatorStack.push(op); 
		} else SynErr(35);
}

void Parser::Type(int &type) {
		if (la->kind == 13 /* "int" */) {
			Get();
			type = integer; 
		} else if (la->kind == 14 /* "boolean" */) {
			Get();
			type = boolean; 
		} else if (la->kind == 15 /* "float" */) {
			Get();
			type = decimal; 
		} else if (la->kind == 16 /* "void" */) {
			Get();
			type = undef; 
		} else SynErr(36);
}

void Parser::VariableDeclaration() {
		wchar_t* name; int type; 
		Type(type);
		Ident(name);
		symbolTable -> NewObj(name, var, type); 
		while (la->kind == 17 /* "," */) {
			Get();
			Ident(name);
			symbolTable -> NewObj(name, var, type); 
		}
		Expect(18 /* ";" */);
}

void Parser::FunctionDeclaration() {
		wchar_t* name; int type; 
		Expect(19 /* "function" */);
		Type(type);
		Ident(name);
		symbolTable -> NewObj(name, function, type); 
		Expect(20 /* "(" */);
		Expect(21 /* ")" */);
		Expect(22 /* "{" */);
		while (StartOf(1)) {
			Statement();
		}
		Expect(23 /* "}" */);
}

void Parser::Statement() {
		if (la->kind == _ident) {
			VariableAssignation();
		} else if (la->kind == 25 /* "if" */) {
			IfCase();
		} else if (la->kind == 27 /* "while" */) {
			WhileLoop();
		} else if (la->kind == 28 /* "print" */) {
			Print();
		} else SynErr(37);
}

void Parser::VariableAssignation() {
		int type; wchar_t* name; Obj obj; 
		Ident(name);
		obj = symbolTable -> Find(name);
		type = obj.type;
		codeGenerator -> operandStack.push(obj.address);
		codeGenerator -> typeStack.push(type); 
		int newtype = undef;
		
		Expect(24 /* "=" */);
		codeGenerator -> operatorStack.push(ASSIGN); 
		LogicalExpresion(newtype);
		codeGenerator -> getAssignResultType(newtype); ; 
		Expect(18 /* ";" */);
}

void Parser::LogicalExpresion(int& type) {
		int nextType; int op;
		SimExpr(type);
		while (StartOf(2)) {
			RelOp(op);
			SimExpr(nextType);
			codeGenerator -> getRelOpResultType(nextType); 
			
		}
}

void Parser::IfCase() {
		int type; 
		Expect(25 /* "if" */);
		Expect(20 /* "(" */);
		LogicalExpresion(type);
		Expect(21 /* ")" */);
		Expect(22 /* "{" */);
		type = codeGenerator -> typeStack.top();
		codeGenerator -> typeStack.pop();
		
		int result = codeGenerator -> operandStack.top();
		codeGenerator -> operandStack.pop();
		
		codeGenerator -> code.push_back({GOTOF, result, 0, 0});
		codeGenerator -> jumpStack.push(static_cast<int>(codeGenerator -> code.size()) - 1);
		
		while (StartOf(1)) {
			Statement();
		}
		Expect(23 /* "}" */);
		if (la->kind == 26 /* "else" */) {
			Get();
			codeGenerator -> code.push_back({GOTO, 0, 0, 0});
			
			int gotoStack = codeGenerator -> jumpStack.top();
			codeGenerator -> jumpStack.pop();
			
			codeGenerator -> jumpStack.push(static_cast<int>(codeGenerator -> code.size()) - 1);
			
			codeGenerator -> code[gotoStack][3] = static_cast<int>(codeGenerator -> code.size());
			
			Expect(22 /* "{" */);
			while (StartOf(1)) {
				Statement();
			}
			Expect(23 /* "}" */);
		}
		int end = codeGenerator -> jumpStack.top();
		codeGenerator -> jumpStack.pop();
		codeGenerator -> code[end][3] = static_cast<int>(codeGenerator -> code.size());
		
}

void Parser::WhileLoop() {
		int type; 
		Expect(27 /* "while" */);
		codeGenerator -> jumpStack.push(static_cast<int>(codeGenerator -> code.size())); 
		Expect(20 /* "(" */);
		LogicalExpresion(type);
		Expect(21 /* ")" */);
		type = codeGenerator -> typeStack.top();
		codeGenerator -> typeStack.pop();
		
		int result = codeGenerator -> operandStack.top();
		codeGenerator -> operandStack.pop();
		
		codeGenerator -> code.push_back({GOTOF, result, 0, 0});
		codeGenerator -> jumpStack.push(static_cast<int>(codeGenerator -> code.size() - 1));
		
		Expect(22 /* "{" */);
		while (StartOf(1)) {
			Statement();
		}
		Expect(23 /* "}" */);
		int end = codeGenerator -> jumpStack.top();
		codeGenerator -> jumpStack.pop();
		
		int jumpBack = codeGenerator -> jumpStack.top(); 
		codeGenerator -> jumpStack.pop();
		
		codeGenerator -> code.push_back({GOTO, 0, 0, jumpBack});
		codeGenerator -> code[end][3] = static_cast<int>(codeGenerator -> code.size());
		
}

void Parser::Print() {
		int type; 
		Expect(28 /* "print" */);
		Expect(20 /* "(" */);
		if (la->kind == _string) {
			Get();
			int tempMemory = codeGenerator -> avail -> next();
			codeGenerator -> constantMap[tempMemory] = coco_string_create(t -> val);
			codeGenerator -> code.push_back({PRINT, 0, 0, tempMemory});
			
		} else if (StartOf(3)) {
			LogicalExpresion(type);
			codeGenerator -> code.push_back({PRINT, 0, 0, codeGenerator -> operandStack.top()});
			codeGenerator -> operandStack.pop();
			
		} else SynErr(38);
		Expect(21 /* ")" */);
		Expect(18 /* ";" */);
}

void Parser::SimExpr(int& type) {
		int nextType, op; 
		Term(type);
		while (la->kind == 5 /* "+" */ || la->kind == 6 /* "-" */) {
			AddOp(op);
			Term(nextType);
			codeGenerator -> getAddOpResultType(nextType);
			
		}
}

void Parser::Term(int& type) {
		int nextType, op; 
		Factor(type);
		while (la->kind == 7 /* "*" */ || la->kind == 8 /* "/" */) {
			MulOp(op);
			Factor(nextType);
			codeGenerator -> getMulOpResultType(nextType); 
			
		}
}

void Parser::Factor(int& type) {
		wchar_t* name;  Obj obj;  int numberReference; float decimalReference;
		switch (la->kind) {
		case _float: {
			type = undef; 
			Get();
			type = decimal; 
			wchar_t * pEnd;
			decimalReference = wcstof(t -> val, &pEnd); 
			int tempMemory = codeGenerator -> avail -> next();
			codeGenerator -> operandStack.push(tempMemory); 
			codeGenerator -> typeStack.push(type); 
			codeGenerator -> constantMap[tempMemory] = decimalReference;
			
			break;
		}
		case _number: {
			Get();
			type = integer; 
			swscanf(t -> val, L"%d", &numberReference); 
			int tempMemory = codeGenerator -> avail -> next();
			codeGenerator -> operandStack.push(tempMemory); 
			codeGenerator -> typeStack.push(type); 
			codeGenerator -> constantMap[tempMemory] = numberReference;
			
			break;
		}
		case 29 /* "false" */: {
			Get();
			type = boolean;
			int tempMemory = codeGenerator -> avail -> next();
			codeGenerator -> operandStack.push(tempMemory); 
			codeGenerator -> typeStack.push(type);   
			codeGenerator -> constantMap[tempMemory] = false;  
			
			break;
		}
		case 30 /* "true" */: {
			Get();
			type = boolean;
			int tempMemory = codeGenerator -> avail -> next();
			codeGenerator -> operandStack.push(tempMemory); 
			codeGenerator -> typeStack.push(type);   
			codeGenerator -> constantMap[tempMemory] = true;    
			
			break;
		}
		case _ident: {
			Ident(name);
			obj = symbolTable -> Find(name);
			type = obj.type;
			codeGenerator -> operandStack.push(obj.address);
			codeGenerator -> typeStack.push(obj.type);
			
			if (obj.kind != var) throw std::invalid_argument("identifier not a variable");  //use symbol table use else if when functions
			
			break;
		}
		case 20 /* "(" */: {
			Get();
			codeGenerator -> operatorStack.push(PARENTHESIS); 
			SimExpr(type);
			Expect(21 /* ")" */);
			codeGenerator -> operatorStack.pop(); 
			break;
		}
		case 6 /* "-" */: {
			Get();
			Factor(type);
			break;
		}
		default: SynErr(39); break;
		}
}

void Parser::Zoso() {
		wchar_t* name; InitDeclarations(); 
		Expect(31 /* "Program" */);
		Ident(name);
		Expect(18 /* ";" */);
		while (StartOf(4)) {
			if (la->kind == 19 /* "function" */) {
				FunctionDeclaration();
			} else {
				VariableDeclaration();
			}
		}
		while (StartOf(1)) {
			Statement();
		}
}




// If the user declared a method Init and a mehtod Destroy they should
// be called in the contructur and the destructor respctively.
//
// The following templates are used to recognize if the user declared
// the methods Init and Destroy.

template<typename T>
struct ParserInitExistsRecognizer {
	template<typename U, void (U::*)() = &U::Init>
	struct ExistsIfInitIsDefinedMarker{};

	struct InitIsMissingType {
		char dummy1;
	};
	
	struct InitExistsType {
		char dummy1; char dummy2;
	};

	// exists always
	template<typename U>
	static InitIsMissingType is_here(...);

	// exist only if ExistsIfInitIsDefinedMarker is defined
	template<typename U>
	static InitExistsType is_here(ExistsIfInitIsDefinedMarker<U>*);

	enum { InitExists = (sizeof(is_here<T>(NULL)) == sizeof(InitExistsType)) };
};

template<typename T>
struct ParserDestroyExistsRecognizer {
	template<typename U, void (U::*)() = &U::Destroy>
	struct ExistsIfDestroyIsDefinedMarker{};

	struct DestroyIsMissingType {
		char dummy1;
	};
	
	struct DestroyExistsType {
		char dummy1; char dummy2;
	};

	// exists always
	template<typename U>
	static DestroyIsMissingType is_here(...);

	// exist only if ExistsIfDestroyIsDefinedMarker is defined
	template<typename U>
	static DestroyExistsType is_here(ExistsIfDestroyIsDefinedMarker<U>*);

	enum { DestroyExists = (sizeof(is_here<T>(NULL)) == sizeof(DestroyExistsType)) };
};

// The folloing templates are used to call the Init and Destroy methods if they exist.

// Generic case of the ParserInitCaller, gets used if the Init method is missing
template<typename T, bool = ParserInitExistsRecognizer<T>::InitExists>
struct ParserInitCaller {
	static void CallInit(T *t) {
		// nothing to do
	}
};

// True case of the ParserInitCaller, gets used if the Init method exists
template<typename T>
struct ParserInitCaller<T, true> {
	static void CallInit(T *t) {
		t->Init();
	}
};

// Generic case of the ParserDestroyCaller, gets used if the Destroy method is missing
template<typename T, bool = ParserDestroyExistsRecognizer<T>::DestroyExists>
struct ParserDestroyCaller {
	static void CallDestroy(T *t) {
		// nothing to do
	}
};

// True case of the ParserDestroyCaller, gets used if the Destroy method exists
template<typename T>
struct ParserDestroyCaller<T, true> {
	static void CallDestroy(T *t) {
		t->Destroy();
	}
};

void Parser::Parse() {
	t = NULL;
	la = dummyToken = new Token();
	la->val = coco_string_create(L"Dummy Token");
	Get();
	Zoso();
	Expect(0);
}

Parser::Parser(Scanner *scanner) {
	maxT = 32;

	ParserInitCaller<Parser>::CallInit(this);
	dummyToken = NULL;
	t = la = NULL;
	minErrDist = 2;
	errDist = minErrDist;
	this->scanner = scanner;
	errors = new Errors();
}

bool Parser::StartOf(int s) {
	const bool T = true;
	const bool x = false;

	static bool set[5][34] = {
		{T,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x},
		{x,T,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,T,x,T, T,x,x,x, x,x},
		{x,x,x,x, x,x,x,x, x,T,T,T, T,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x},
		{x,T,x,T, T,x,T,x, x,x,x,x, x,x,x,x, x,x,x,x, T,x,x,x, x,x,x,x, x,T,T,x, x,x},
		{x,x,x,x, x,x,x,x, x,x,x,x, x,T,T,T, T,x,x,T, x,x,x,x, x,x,x,x, x,x,x,x, x,x}
	};



	return set[s][la->kind];
}

Parser::~Parser() {
	ParserDestroyCaller<Parser>::CallDestroy(this);
	delete errors;
	delete dummyToken;
}

Errors::Errors() {
	count = 0;
}

void Errors::SynErr(int line, int col, int n) {
	wchar_t* s;
	switch (n) {
			case 0: s = coco_string_create(L"EOF expected"); break;
			case 1: s = coco_string_create(L"ident expected"); break;
			case 2: s = coco_string_create(L"string expected"); break;
			case 3: s = coco_string_create(L"number expected"); break;
			case 4: s = coco_string_create(L"float expected"); break;
			case 5: s = coco_string_create(L"\"+\" expected"); break;
			case 6: s = coco_string_create(L"\"-\" expected"); break;
			case 7: s = coco_string_create(L"\"*\" expected"); break;
			case 8: s = coco_string_create(L"\"/\" expected"); break;
			case 9: s = coco_string_create(L"\"==\" expected"); break;
			case 10: s = coco_string_create(L"\"<\" expected"); break;
			case 11: s = coco_string_create(L"\">\" expected"); break;
			case 12: s = coco_string_create(L"\"!=\" expected"); break;
			case 13: s = coco_string_create(L"\"int\" expected"); break;
			case 14: s = coco_string_create(L"\"boolean\" expected"); break;
			case 15: s = coco_string_create(L"\"float\" expected"); break;
			case 16: s = coco_string_create(L"\"void\" expected"); break;
			case 17: s = coco_string_create(L"\",\" expected"); break;
			case 18: s = coco_string_create(L"\";\" expected"); break;
			case 19: s = coco_string_create(L"\"function\" expected"); break;
			case 20: s = coco_string_create(L"\"(\" expected"); break;
			case 21: s = coco_string_create(L"\")\" expected"); break;
			case 22: s = coco_string_create(L"\"{\" expected"); break;
			case 23: s = coco_string_create(L"\"}\" expected"); break;
			case 24: s = coco_string_create(L"\"=\" expected"); break;
			case 25: s = coco_string_create(L"\"if\" expected"); break;
			case 26: s = coco_string_create(L"\"else\" expected"); break;
			case 27: s = coco_string_create(L"\"while\" expected"); break;
			case 28: s = coco_string_create(L"\"print\" expected"); break;
			case 29: s = coco_string_create(L"\"false\" expected"); break;
			case 30: s = coco_string_create(L"\"true\" expected"); break;
			case 31: s = coco_string_create(L"\"Program\" expected"); break;
			case 32: s = coco_string_create(L"??? expected"); break;
			case 33: s = coco_string_create(L"invalid AddOp"); break;
			case 34: s = coco_string_create(L"invalid MulOp"); break;
			case 35: s = coco_string_create(L"invalid RelOp"); break;
			case 36: s = coco_string_create(L"invalid Type"); break;
			case 37: s = coco_string_create(L"invalid Statement"); break;
			case 38: s = coco_string_create(L"invalid Print"); break;
			case 39: s = coco_string_create(L"invalid Factor"); break;

		default:
		{
			wchar_t format[20];
			coco_swprintf(format, 20, L"error %d", n);
			s = coco_string_create(format);
		}
		break;
	}
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
	coco_string_delete(s);
	count++;
}

void Errors::Error(int line, int col, const wchar_t *s) {
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
	count++;
}

void Errors::Warning(int line, int col, const wchar_t *s) {
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
}

void Errors::Warning(const wchar_t *s) {
	wprintf(L"%ls\n", s);
}

void Errors::Exception(const wchar_t* s) {
	wprintf(L"%ls", s); 
	exit(1);
}

} // namespace

