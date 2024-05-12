

#if !defined(Zoso_COCO_PARSER_H__)
#define Zoso_COCO_PARSER_H__



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



	Parser(Scanner *scanner);
	~Parser();
	void SemErr(const wchar_t* msg);

	void AddOp();
	void MulOp();
	void VariableDeclaration();
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

