

#if !defined(Zoso_COCO_PARSER_H__)
#define Zoso_COCO_PARSER_H__

#include "SymbolTable.h"


#include "Scanner.h"

namespace Zoso {


class Errors {
public:
	int count;			// number of errors detected

	Errors();
	void SynErr(int line, int col, int n);
	void Error(int line, int col, const wchar_t *s);
	void Warning(int line, int col, const wchar_t *s);
	void Warning(const wchar_t *s);
	void Exception(const wchar_t *s);

}; // Errors

class Parser {
private:
	enum {
		_EOF=0,
		_number=1,
		_ident=2,
		_string=3
	};
	int maxT;

	Token *dummyToken;
	int errDist;
	int minErrDist;

	void SynErr(int n);
	void Get();
	void Expect(int n);
	bool StartOf(int s);
	void ExpectWeak(int n, int follow);
	bool WeakSeparator(int n, int syFol, int repFol);

public:
	Scanner *scanner;
	Errors  *errors;

	Token *t;			// last recognized token
	Token *la;			// lookahead token

int undef, integer, boolean, decimal; // types
    int var, function; //Object Kinds


    // int ADD, SUB, MUL, DIV, EQU, LSS, GTR, ASSIGN; // operation codes

    // void InitDeclarations() {
    //     plus = 0, minus = 1, times = 2, slash = 3, equals = 4, lessThan = 5, greaterThan = 6, assign = 7
    //     integer = 1, boolean = 2;
    //     var = 0, function = 1;

    //     // Operational Codes
    //     ADD = 0, SUB = 1, MUL = 2, DIV = 3, EQU = 4, LSS = 5, GTR = 6, ASSIGN = 7;
    // }
    SymbolTable *symbolTable;



	Parser(Scanner *scanner);
	~Parser();
	void SemErr(const wchar_t* msg);

	void Ident(wchar_t* &name);
	void AddOp();
	void MulOp();
	void VariableDeclaration();
	void Type(int &type);
	void FunctionDeclaration();
	void Statement();
	void Expr();
	void SimExpr();
	void Term();
	void Factor();
	void Zoso();

	void Parse();

}; // end Parser

} // namespace


#endif

