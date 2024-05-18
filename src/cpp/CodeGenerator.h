#if !defined(CODEGENERATOR_H__)
#define CODEGENERATOR_H__

#include "Scanner.h"
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>

namespace Zoso {

class CodeGenerator {
    public:
        // opcodes
        int ADD, SUB, MUL, DIV, EQU, LSS, GTR, ASSIGN;

        wchar_t* opcode[21];

        int programStart;		// address of first instruction of main program
        int programCounter;	// program counter
        char *code;

        // data for Interpret
        int *globals;
        int *stack;
        int top;	     // top of stack
        int basePointer; // base pointer


        CodeGenerator();

        ~CodeGenerator();

        //----- code generation methods -----

        void Emit (int op);

        void Emit (int op, int val);

        void Patch (int adr, int val);

        void Decode();

        //----- interpreter methods -----

        int Next();

        int Next2();

        int Int(bool b);

        void Push(int val);

        int Pop();

        int ReadInt(FILE* s);

        void Interpret(char* data);
};

}; // namespace

#endif // !defined(CODEGENERATOR_H__)
