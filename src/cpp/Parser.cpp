

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

void Parser::AddOp() {
		if (la->kind == 4 /* "+" */) {
			Get();
		} else if (la->kind == 5 /* "-" */) {
			Get();
		} else SynErr(25);
}

void Parser::MulOp() {
		if (la->kind == 6 /* "*" */) {
			Get();
		} else if (la->kind == 7 /* "/" */) {
			Get();
		} else SynErr(26);
}

void Parser::VariableDeclaration() {
		wchar_t* name; int type; 
		Type(type);
		Ident(name);
		symbolTable -> NewObj(name, var, type); 
		while (la->kind == 8 /* "," */) {
			Get();
			Ident(name);
			symbolTable -> NewObj(name, var, type); 
		}
		Expect(9 /* ";" */);
}

void Parser::Type(int &type) {
		if (la->kind == 10 /* "int" */) {
			Get();
			type = integer; 
		} else if (la->kind == 11 /* "bool" */) {
			Get();
			type = boolean; 
		} else if (la->kind == 12 /* "float" */) {
			Get();
			type = decimal; 
		} else if (la->kind == 13 /* "void" */) {
			Get();
			type = undef; 
		} else SynErr(27);
}

void Parser::FunctionDeclaration() {
		wchar_t* name; int type; 
		Expect(14 /* "function" */);
		Type(type);
		Ident(name);
		symbolTable -> NewObj(name, function, type); 
		Expect(15 /* "(" */);
		while (StartOf(1)) {
			VariableDeclaration();
		}
		Expect(16 /* ")" */);
		Expect(17 /* "{" */);
		while (StartOf(2)) {
			Statement();
		}
		Expect(18 /* "}" */);
}

void Parser::Statement() {
		if (la->kind == 19 /* "print" */) {
			Get();
			Expect(15 /* "(" */);
			while (StartOf(3)) {
				if (StartOf(4)) {
					Expr();
				} else if (StartOf(4)) {
					SimExpr();
				} else {
					Get();
				}
			}
			Expect(16 /* ")" */);
			Expect(9 /* ";" */);
		} else if (StartOf(4)) {
			Expr();
			Expect(9 /* ";" */);
		} else if (la->kind == 14 /* "function" */) {
			FunctionDeclaration();
		} else if (StartOf(1)) {
			VariableDeclaration();
		} else SynErr(28);
}

void Parser::Expr() {
		SimExpr();
		if (la->kind == 20 /* "==" */ || la->kind == 21 /* "<" */ || la->kind == 22 /* ">" */) {
			if (la->kind == 20 /* "==" */) {
				Get();
			} else if (la->kind == 21 /* "<" */) {
				Get();
			} else {
				Get();
			}
			SimExpr();
		}
}

void Parser::SimExpr() {
		Term();
		while (la->kind == 4 /* "+" */ || la->kind == 5 /* "-" */) {
			AddOp();
			Term();
		}
}

void Parser::Term() {
		Factor();
		while (la->kind == 6 /* "*" */ || la->kind == 7 /* "/" */) {
			MulOp();
			Factor();
		}
}

void Parser::Factor() {
		if (la->kind == _number) {
			Get();
		} else if (la->kind == _ident) {
			Get();
		} else if (la->kind == 15 /* "(" */) {
			Get();
			Expr();
			Expect(16 /* ")" */);
		} else if (la->kind == 4 /* "+" */ || la->kind == 5 /* "-" */) {
			AddOp();
			Factor();
		} else SynErr(29);
}

void Parser::Zoso() {
		wchar_t* name; InitDeclarations(); 
		Expect(23 /* "Zoso" */);
		symbolTable -> OpenScope(); 
		Expect(17 /* "{" */);
		while (StartOf(2)) {
			Statement();
		}
		Expect(18 /* "}" */);
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
	maxT = 24;

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

	static bool set[5][26] = {
		{T,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x},
		{x,x,x,x, x,x,x,x, x,x,T,T, T,T,x,x, x,x,x,x, x,x,x,x, x,x},
		{x,T,T,x, T,T,x,x, x,x,T,T, T,T,T,T, x,x,x,T, x,x,x,x, x,x},
		{x,T,T,T, T,T,x,x, x,x,x,x, x,x,x,T, x,x,x,x, x,x,x,x, x,x},
		{x,T,T,x, T,T,x,x, x,x,x,x, x,x,x,T, x,x,x,x, x,x,x,x, x,x}
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
			case 1: s = coco_string_create(L"number expected"); break;
			case 2: s = coco_string_create(L"ident expected"); break;
			case 3: s = coco_string_create(L"string expected"); break;
			case 4: s = coco_string_create(L"\"+\" expected"); break;
			case 5: s = coco_string_create(L"\"-\" expected"); break;
			case 6: s = coco_string_create(L"\"*\" expected"); break;
			case 7: s = coco_string_create(L"\"/\" expected"); break;
			case 8: s = coco_string_create(L"\",\" expected"); break;
			case 9: s = coco_string_create(L"\";\" expected"); break;
			case 10: s = coco_string_create(L"\"int\" expected"); break;
			case 11: s = coco_string_create(L"\"bool\" expected"); break;
			case 12: s = coco_string_create(L"\"float\" expected"); break;
			case 13: s = coco_string_create(L"\"void\" expected"); break;
			case 14: s = coco_string_create(L"\"function\" expected"); break;
			case 15: s = coco_string_create(L"\"(\" expected"); break;
			case 16: s = coco_string_create(L"\")\" expected"); break;
			case 17: s = coco_string_create(L"\"{\" expected"); break;
			case 18: s = coco_string_create(L"\"}\" expected"); break;
			case 19: s = coco_string_create(L"\"print\" expected"); break;
			case 20: s = coco_string_create(L"\"==\" expected"); break;
			case 21: s = coco_string_create(L"\"<\" expected"); break;
			case 22: s = coco_string_create(L"\">\" expected"); break;
			case 23: s = coco_string_create(L"\"Zoso\" expected"); break;
			case 24: s = coco_string_create(L"??? expected"); break;
			case 25: s = coco_string_create(L"invalid AddOp"); break;
			case 26: s = coco_string_create(L"invalid MulOp"); break;
			case 27: s = coco_string_create(L"invalid Type"); break;
			case 28: s = coco_string_create(L"invalid Statement"); break;
			case 29: s = coco_string_create(L"invalid Factor"); break;

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

