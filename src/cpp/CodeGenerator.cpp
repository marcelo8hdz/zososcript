#include "Scanner.h"
#include "CodeGenerator.h"

#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>

namespace Zoso {
CodeGenerator::CodeGenerator() {
	// opcodes
    ADD = 0, SUB = 1, MUL = 2, DIV = 3, EQU = 4, LSS = 5, GTR = 6, ASSIGN = 7;

    opcode[ 0] = coco_string_create("ADD  ");
    opcode[ 1] = coco_string_create("SUB  ");
    opcode[ 2] = coco_string_create("MUL  ");
    opcode[ 3] = coco_string_create("DIV  ");
    opcode[ 4] = coco_string_create("EQU  ");
    opcode[ 5] = coco_string_create("LSS  ");
    opcode[ 6] = coco_string_create("GTR  ");
    opcode[ 7] = coco_string_create("ASSIGN");  
    
    // BELOW IS PROTOTYPE
    opcode[ 8] = coco_string_create("LOAD ");
    opcode[ 9] = coco_string_create("LOADG");
    opcode[10] = coco_string_create("CONST"); // NUMBERS, FLOATS, STRINGS?
    opcode[11] = coco_string_create("CALL "); // FUNCTION CALLS?
    opcode[12] = coco_string_create("RETURN");
    opcode[13] = coco_string_create("LEAVE");
    opcode[14] = coco_string_create("JMP  ");
    opcode[15] = coco_string_create("FJMP ");
    opcode[16] = coco_string_create("READ "); // READ VARIABLES?
    opcode[17] = coco_string_create("WRITE"); // ASSIGN?

    code    = new char[3000];
    globals = new int[100];
    stack   = new int[100];

    programStart = 0;

    programCounter = 1;

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
}

//----- code generation methods -----

void CodeGenerator::Emit(int op) {
    code[ programCounter++ ] = (char) op; // why char
}


void CodeGenerator::Emit (int op, int val) {
    Emit(op);
    Emit(val>>8); // why
    Emit(val);
}

void CodeGenerator::Patch(int adr, int val) {}

void CodeGenerator::Decode() {}

//----- interpreter methods -----

int CodeGenerator::Next () {}

int CodeGenerator::Next2 () {}

int CodeGenerator::Int(bool b) {}

void CodeGenerator::Push(int val) {}

int CodeGenerator::Pop(){}

int CodeGenerator::ReadInt(FILE* s){}

void CodeGenerator::Interpret(char* data) {}

}


