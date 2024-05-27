#include "CodeVector.h"
#include "ConstantMap.h"
#include "Operations.hpp"
#include <vector>
#include <map>
#include <iostream>
#include <stdexcept>
#include <wchar.h>

int main() {
    int instructionPointer = 0;
    while (instructionPointer < code.size()) {
        int op = code[instructionPointer][0];
        int arg1 = code[instructionPointer][1];
        int arg2 = code[instructionPointer][2];
        int result = code[instructionPointer][3];

        switch (op) {
            case 0:  // ADD
                constantMap[result] = add(constantMap[arg1], constantMap[arg2]);;
                break;
            case 1:  // SUB
                constantMap[result] = sub(constantMap[arg1], constantMap[arg2]);
                break;
            case 2:  // MUL
                constantMap[result] = mul(constantMap[arg1], constantMap[arg2]);
                break;
            case 3:  // DIV
                // if (constantMap[arg2] == 0) {
                //     throw std::runtime_error("Division by zero");
                // }
                constantMap[result] = div(constantMap[arg1], constantMap[arg2]);
                break;
            case 4:  // EQU
                constantMap[result] = constantMap[arg1] == constantMap[arg2];
                break;
            case 5:  // LSS
                constantMap[result] = lessThan(constantMap[arg1], constantMap[arg2]);
                break;
            case 6:  // GTR
                constantMap[result] = greaterThan(constantMap[arg1], constantMap[arg2]);
                break;
            case 7:  // ASSIGN
                constantMap[result] = constantMap[arg1];
                break;
            case 8:  // LOAD
                constantMap[result] = arg1;  // Assuming arg1 is a value to be loaded
                break;
            case 9:  // CONST
                constantMap[result] = arg1;  // Assuming arg1 is a constant value
                break;
            case 10:  // FCALL
                std::cout << "Function call not implemented\n";
                break;
            case 11:  // RETURN
                std::cout << "Return not implemented\n";
                break;
            case 12:  // GOTO
                instructionPointer = result;
                continue; 
            case 13:  // GOTOF
                if (const bool* condition = boost::get<bool>(&constantMap[arg1])) {
                    if (!*condition) instructionPointer = result;
                }
                continue;
            case 14:  // STORE
                std::cout << "STORE not implemented\n";
                break;
            case 15:  // READ
                std::cout << "Read not implemented\n";
                break;
            case 16:  // WRITE
                std::cout << "Write not implemented\n";
                break;
            case 17:  // PRINT
                std::wcout << L"PRINT: " << constantMap[arg1] << L"\n";
                break;
            case 18:  // NEQU
                constantMap[result] = (constantMap[arg1] != constantMap[arg2]);
                break;
            default:
                throw std::runtime_error("Unknown opcode");
        }
        instructionPointer++;
    }
    return 0;
}
