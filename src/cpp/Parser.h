

#if !defined(Zoso_COCO_PARSER_H__)
#define Zoso_COCO_PARSER_H__

#include "SymbolTable.h"
#include "CodeGenerator.h"
#include <wchar.h>
#include <iostream>


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
		_ident=1,
		_string=2,
		_number=3,
		_float=4
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

int plus, minus, times, slash, equals, lessThan, greaterThan, assign; // Operators
    int undef, integer, boolean, decimal; // types
    int var, function; //Object Kinds


    int ADD, SUB, MUL, DIV, EQU, LSS, GTR, ASSIGN, // operation codes
        LOAD, CONST, FCALL, RETURN, GOTO, GOTOF, STORE, READ, WRITE, PRINT, NEQU, PARENTHESIS;

    void InitDeclarations() {
        plus = 0, minus = 1, times = 2, slash = 3, equals = 4, lessThan = 5, greaterThan = 6, assign = 7;
        undef = 0, integer = 1, boolean = 2, decimal = 3;
        var = 0, function = 1;

        // Operational Codes
        ADD = 0, SUB = 1, MUL = 2, DIV = 3, EQU = 4, LSS = 5, GTR = 6, ASSIGN = 7;
        LOAD = 8; CONST = 9; FCALL = 10; RETURN = 11; GOTO = 12; GOTOF = 13; STORE = 14; 
        READ = 15; WRITE = 16; PRINT = 17; NEQU = 18, PARENTHESIS = 19;
    }

    SymbolTable* symbolTable;
    CodeGenerator* codeGenerator;



	Parser(Scanner *scanner);
	~Parser();
	void SemErr(const wchar_t* msg);

	void Ident(wchar_t* &name);
	void AddOp(int& op);
	void MulOp(int& op);
	void RelOp(int& op);
	void VariableDeclaration();
	void Type(int &type);
	void FunctionDeclaration();
	void Statement();
	void IfCase();
	void LogicalExpresion(int& type);
	void WhileLoop();
	void VariableAssignation();
	void SimExpr(int& type);
	void Term(int& type);
	void Factor(int& type);
	void Zoso();

	void Parse();

}; // end Parser

} // namespace


#endif

