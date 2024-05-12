#include "Scanner.h"
#include "CodeGenerator.h"

#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>

namespace Zoso {
CodeGenerator::CodeGenerator() {}

CodeGenerator::~CodeGenerator() {}

//----- code generation methods -----

void CodeGenerator::Emit(int op) {}


void CodeGenerator::Emit (int op, int val) {}

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


