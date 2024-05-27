#include "Scanner.h"
#include "CodeGenerator.h"
#include "SemanticCube.h"

#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include<vector>

namespace Zoso {
CodeGenerator::CodeGenerator() {
	// opcodes
    ADD = 0; SUB = 1; MUL = 2; DIV = 3; EQU = 4; LSS = 5; GTR = 6; ASSIGN = 7;
    
    LOAD = 8; CONST = 9; FCALL = 10; RETURN = 11; GOTO = 12; GOTOF = 13; STORE = 14; 
    READ = 15; WRITE = 16; PRINT = 17; NEQU = 18;
    ERROR = 404;
    undef = 0, integer = 1, boolean = 2, decimal = 3; // types

    opcode[ 0] = coco_string_create("ADD  ");
    opcode[ 1] = coco_string_create("SUB  ");
    opcode[ 2] = coco_string_create("MUL  ");
    opcode[ 3] = coco_string_create("DIV  ");
    opcode[ 4] = coco_string_create("EQU  ");
    opcode[ 5] = coco_string_create("LSS  ");
    opcode[ 6] = coco_string_create("GTR  ");
    opcode[ 7] = coco_string_create("ASSIGN");  
    
    opcode[ 8] = coco_string_create("LOAD ");
    opcode[ 9] = coco_string_create("CONST"); // NUMBERS, FLOATS, STRINGS, BOOLS
    opcode[10] = coco_string_create("FCALL"); // CALL FUNCTION
    opcode[11] = coco_string_create("RETURN");// RETURN FROM A FUNCTION
    opcode[12] = coco_string_create("GOTO ");
    opcode[13] = coco_string_create("GOTOF"); // GOTO WHEN FALSE
    opcode[14] = coco_string_create("FJUMP"); // JUMP TO A FUNCTION
    opcode[15] = coco_string_create("READ "); // READ VARIABLES
    opcode[16] = coco_string_create("WRITE"); // WRITE CONSTANTS ON MEMORY
    opcode[17] = coco_string_create("PRINT");  
    opcode[18] = coco_string_create("NEQU");  
    //add ERA (CHECK THERE'S MEMORY STACK AVAILABLE)load memory context for next instruction

    avail = new Avail();

    initializeSemanticCube();
}

CodeGenerator::~CodeGenerator() {
    coco_string_delete(opcode[ 0]);
    coco_string_delete(opcode[ 1]);
    coco_string_delete(opcode[ 2]);
    coco_string_delete(opcode[ 3]);
    coco_string_delete(opcode[ 4]);
    coco_string_delete(opcode[ 5]);
    coco_string_delete(opcode[ 6]);
    coco_string_delete(opcode[ 7]);
    coco_string_delete(opcode[ 8]);
    coco_string_delete(opcode[ 9]);
    coco_string_delete(opcode[10]);
    coco_string_delete(opcode[11]);
    coco_string_delete(opcode[12]);
    coco_string_delete(opcode[13]);
    coco_string_delete(opcode[14]);
    coco_string_delete(opcode[15]);
    coco_string_delete(opcode[16]);
    coco_string_delete(opcode[17]);
    coco_string_delete(opcode[18]);
}

void CodeGenerator::getAddOpResultType(int& resultType) {
    if (typeStack.empty() || operandStack.empty() || operatorStack.empty()) return; // maybe early returns should assign type?? NULL?
    if (operatorStack.top() != ADD && operatorStack.top() != SUB) return;
       
    int rightOperand = operandStack.top();
    operandStack.pop();
    
    int rightType = typeStack.top();
    typeStack.pop();
    
    int leftOperand = operandStack.top();
    operandStack.pop();
    
    int leftType = typeStack.top();
    typeStack.top();


    int addOperator = operatorStack.top();
    operatorStack.pop();

   resultType = SemanticCube.at(leftType).at(addOperator).at(rightType);

    if (resultType == ERROR) {
        throw std::invalid_argument("this types are not compatible for add operation!");
    }

    int result = avail -> next();
    code.push_back({addOperator, leftOperand, rightOperand, result});
    operandStack.push(result);
    typeStack.push(resultType);
    //If any operand were a temporal space,
    //return it to AVAIL
}

void CodeGenerator::getRelOpResultType(int& resultType) {
    if (typeStack.empty() || operandStack.empty() || operatorStack.empty()) return; // maybe early returns should assign type?? NULL?
    if (operatorStack.top() != EQU && operatorStack.top() != GTR && operatorStack.top() != LSS && operatorStack.top() !=  NEQU) return;
       
    int rightOperand = operandStack.top();
    operandStack.pop();
    
    int rightType = typeStack.top();
    typeStack.pop();
    
    int leftOperand = operandStack.top();
    operandStack.pop();
    
    int leftType = typeStack.top();
    typeStack.top();

    int relOperator = operatorStack.top();
    operatorStack.pop();

    resultType = SemanticCube.at(leftType).at(relOperator).at(rightType);

    if (resultType == ERROR) {
        throw std::invalid_argument("this types are not compatible for logical expression!");
    }

    int result = avail -> next();
    code.push_back({relOperator, leftOperand, rightOperand, result});
    operandStack.push(result);
    typeStack.push(resultType);

    //If any operand were a temporal space,
    //return it to AVAIL
}

void CodeGenerator::getMulOpResultType(int& resultType) {
    if (typeStack.empty() || operandStack.empty() || operatorStack.empty()) return; // maybe early returns should assign type?? NULL?
    if (operatorStack.top() != MUL && operatorStack.top() != DIV) return;
       
    int rightOperand = operandStack.top();
    operandStack.pop();
    
    int rightType = typeStack.top();
    typeStack.pop();
    
    int leftOperand = operandStack.top();
    operandStack.pop();
    
    int leftType = typeStack.top();
    typeStack.top();

    int mulOperator = operatorStack.top();
    operatorStack.pop();

    resultType = SemanticCube.at(leftType).at(mulOperator).at(rightType);

    if (resultType == ERROR) {

        throw std::invalid_argument("this types are not compatible for mul operation!");
    }

    int result = avail -> next();
    code.push_back({mulOperator, leftOperand, rightOperand, result});
    operandStack.push(result);
    typeStack.push(resultType);

    //If any operand were a temporal space,
    //return it to AVAIL
}

void CodeGenerator::getAssignResultType(int& resultType) {
    if (typeStack.empty() || operandStack.empty() || operatorStack.empty()) return; // maybe early returns should assign type?? NULL?
    if (operatorStack.top() != ASSIGN) return;
    //check this switched the order of popping right and left (works but dont know why)
    int leftOperand = operandStack.top();
    operandStack.pop();
    
    int leftType = typeStack.top();
    typeStack.top();

    int rightOperand = operandStack.top();
    operandStack.pop();
    
    int rightType = typeStack.top();
    typeStack.pop();

    int assignOperator = operatorStack.top();
    operatorStack.pop();

   resultType = SemanticCube.at(leftType).at(assignOperator).at(rightType);

    if (resultType == ERROR) {
        std::cout << leftType << rightType << std::endl;
        throw std::invalid_argument("this types are not compatible for assign operation!");
    }

    code.push_back({assignOperator, leftOperand, rightOperand, 0});
    //If any operand were a temporal space,
    //return it to AVAIL
}

void CodeGenerator::printQuads()  {
    for (const auto& instruction : code) {
        std::wcout << opcode[instruction[0]] << L" " 
                   << instruction[1] << L" " 
                   << instruction[2] << L" " 
                   << instruction[3] << L"\n";
    }
}
void CodeGenerator::printConstantMap() {
    for (const auto& [key, value] : constantMap) {
        std::cout << "Address: " << key << " Value: ";
        if (const int* intValue = boost::get<int>(&value)) {
            std::cout << *intValue << "\n";
        } else if (const float* floatValue = boost::get<float>(&value)) {
            std::cout << *floatValue << "\n";
        } else if (const bool* boolValue = boost::get<bool>(&value)) {
            std::cout << (*boolValue ? "true" : "false") << "\n";
        }
    }
}

}
