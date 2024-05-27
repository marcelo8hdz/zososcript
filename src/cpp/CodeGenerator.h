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
#include <iostream>
#include "boost/variant.hpp"

namespace Zoso {

class CodeGenerator {
    public:
        // opcodes
        int ADD, SUB, MUL, DIV, EQU, LSS, GTR, ASSIGN,
            LOAD, CONST, FCALL, RETURN, GOTO, GOTOF, STORE, READ, WRITE, PRINT, NEQU;

        int ERROR;
        int undef, integer, boolean, decimal; // types

        wchar_t* opcode[21];

        Avail* avail;
        std::stack<int> operandStack;
        std::stack<int> operatorStack;
        std::stack<int> typeStack;
        std::stack<int> jumpStack;
        
        std::vector<std::vector<int> > code;
        std::map<int, boost::variant<int, float, bool> > constantMap;
        
        
        CodeGenerator();
        ~CodeGenerator();

        void getAddOpResultType(int& resultType);
        void getRelOpResultType(int& resultType);
        void getMulOpResultType(int& resultType);
        void getAssignResultType(int& resultType);

        void printConstantMapToFile(const std::string& filename);
        void printCodeVectorToFile(const std::string& filename);

        
        void printQuads();
        void printConstantMap();
};

}; // namespace

#endif // !defined(CODEGENERATOR_H__)
