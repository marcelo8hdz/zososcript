#if !defined(CODEGENERATOR_H__)
#define CODEGENERATOR_H__

#include "Scanner.h"
#include "Avail.h"
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <vector>
#include <stack>
#include <map>

namespace Zoso {

class CodeGenerator {
    public:
        // opcodes
        int ADD, SUB, MUL, DIV, EQU, LSS, GTR, ASSIGN,
            LOAD, CONST, FCALL, RETURN, GOTO, GOTOF, STORE, READ, WRITE, PRINT, NEQU;

        int ERROR;
        int undef, integer, boolean, decimal; // types

        wchar_t* opcode[21];

        int programStart;		// address of first instruction of main program
        int programCounter;	// program counter

        Avail* avail;
        std::stack<int> operandStack;
        std::stack<int> operatorStack;
        std::stack<int> typeStack;

        std::vector<std::vector<int> > code;

        // std::map<int, int>  constIntAddress;
        // int constIntCounter;
        
        // std::map<int, float> constFloatAddress;
        // int constFloatCounter;
        
        // std::map<int, bool> constBoolAddress;
        // int constBoolCounter;
        
        // std::map<int, std::string>  constStringAddress;
        // int constStringCounter;
        
        
        CodeGenerator();
        ~CodeGenerator();

        //----- code generation methods -----

        void Emit(int op, int arg1, int arg2, int result);

        void getAddOpResultType(int& resultType);
        
        void getRelOpResultType(int& resultType);

        void getMulOpResultType(int& resultType);
    
        // THIS MAY WORK FOR .OBJ FILE???
        // void pushToConstIntMap(int value);
        // void pushToConstFloatMap(float value);
        // void pushToConstBoolMap(bool value);
        // void pushToConstStringMap(std::string value);
};

}; // namespace

#endif // !defined(CODEGENERATOR_H__)
