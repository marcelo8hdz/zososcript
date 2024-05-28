#include "Scanner.h"
#include "CodeGenerator.h"
#include "SemanticCube.h"

#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <vector>
#include <map>
#include <boost/variant.hpp>

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
    int cont = 0;
    for (const auto& instruction : code) {
        std::wcout << cont << L". " 
                   << opcode[instruction[0]] << L" " 
                   << instruction[1] << L" " 
                   << instruction[2] << L" " 
                   << instruction[3] << L"\n";
        cont++;
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
        } else if (const std::wstring* wstrValue = boost::get<std::wstring>(&value)) {
            std::wcout << L"\"" << *wstrValue << L"\"\n";
        }
    }
}

void CodeGenerator::printConstantMapToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Could not open file for writing: " << filename << std::endl;
        return;
    }

    file << "#ifndef GENERATED_CONSTANTS_H\n";
    file << "#define GENERATED_CONSTANTS_H\n\n";
    file << "#include <map>\n";
    file << "#include <boost/variant.hpp>\n\n";
    file << "std::map<int, boost::variant<int, float, bool, std::string>> constantMap = {\n";
    for (const auto& [key, value] : constantMap) {
        file << "    {" << key << ", ";
        if (const int* intValue = boost::get<int>(&value)) {
            file<< "boost::variant<int, float, bool>("  << *intValue;
        } else if (const float* floatValue = boost::get<float>(&value)) {
            std::ostringstream oss;
            oss << "boost::variant<int, float, bool>(" << *floatValue;
            std::string floatString = oss.str();
            if (floatString.find('.') != std::string::npos) {
                floatString += 'f';
            }
            file << floatString;
        } else if (const bool* boolValue = boost::get<bool>(&value)) {
            file << "boost::variant<int, float, bool>(" << (*boolValue ? "true" : "false");
        } 
       else if (const std::wstring* wstrValue = boost::get<std::wstring>(&value)) {
            file << "(std::string)" << "\"" << std::string(wstrValue->begin(), wstrValue->end()) << "\"";
        }
        file << ")},\n";
    }
    file << "};\n\n";
    file << "#endif // GENERATED_CONSTANTS_H\n";

    file.close();
}

void CodeGenerator::printCodeVectorToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Could not open file for writing: " << filename << std::endl;
        return;
    }

    file << "#ifndef GENERATED_CODE_H\n";
    file << "#define GENERATED_CODE_H\n\n";
    file << "#include <vector>\n\n";
    file << "std::vector<std::vector<int>> code = {\n";
    for (const auto& instruction : code) {
        file << "    {" << instruction[0] << ", " 
             << instruction[1] << ", " 
             << instruction[2] << ", " 
             << instruction[3] << "},\n";
    }
    file << "};\n\n";

    // file << "std::vector<std::wstring> opcode = {\n";
    // for (const auto& op : opcode) {
    //     file << "    L\"" << op << "\",\n";
    // }
    // file << "};\n\n";
    file << "#endif // GENERATED_CODE_H\n";

    file.close();
}

}
