#include <unordered_map>
#include <string>
#include <iostream>

using TypeMap = std::unordered_map<int, int>;
using OperationMap = std::unordered_map<int, TypeMap>;
std::unordered_map<int, OperationMap> SemanticCube;


void initializeSemanticCube() {
    int undef, integer, boolean, decimal, error; // types
    undef = 0, integer = 1, boolean = 2, decimal = 3, error = 404;

    int ADD, SUB, MUL, DIV, EQU, LSS, GTR, ASSIGN, NEQU;
    ADD = 0, SUB = 1, MUL = 2, DIV = 3, EQU = 4, LSS = 5, GTR = 6, ASSIGN = 7, NEQU = 18;


    // Populate SemanticCube for "int"
    SemanticCube[integer][ASSIGN] = {{integer, integer}, {decimal, error}, {boolean, error}, {undef, error}};
    SemanticCube[integer][EQU] = {{integer, boolean}, {decimal, error}, {boolean, error}, {undef, error}};
    SemanticCube[integer][GTR] = {{integer, boolean}, {decimal, boolean}, {boolean, error}, {undef, error}};
    SemanticCube[integer][LSS] = {{integer, boolean}, {decimal, boolean}, {boolean, error}, {undef, error}};
    SemanticCube[integer][NEQU] = {{integer, boolean}, {decimal, boolean}, {boolean, error}, {undef, error}};
    SemanticCube[integer][MUL] = {{integer, integer}, {decimal, decimal}, {boolean, error}, {undef, error}};
    SemanticCube[integer][DIV] = {{integer, integer}, {decimal, decimal}, {boolean, error}, {undef, error}};
    SemanticCube[integer][ADD] = {{integer, integer}, {decimal, decimal}, {boolean, error}, {undef, error}};
    SemanticCube[integer][SUB] = {{integer, integer}, {decimal, decimal}, {boolean, error}, {undef, error}};

    // Populate SemanticCube for "float"
    SemanticCube[decimal][ASSIGN] = {{integer, decimal}, {decimal, decimal}, {boolean, error}, {undef, error}}; 
    SemanticCube[decimal][EQU] = {{integer, error}, {decimal, boolean}, {boolean, error}, {undef, error}};
    SemanticCube[decimal][GTR] = {{integer, boolean}, {decimal, boolean}, {boolean, error}, {undef, error}};
    SemanticCube[decimal][LSS] = {{integer, boolean}, {decimal, boolean}, {boolean, error}, {undef, error}};
    SemanticCube[decimal][NEQU] = {{integer, boolean}, {decimal, boolean}, {boolean, error}, {undef, error}};
    SemanticCube[decimal][MUL] = {{integer, decimal}, {decimal, decimal}, {boolean, error}, {undef, error}};
    SemanticCube[decimal][DIV] = {{integer, decimal}, {decimal, decimal}, {boolean, error}, {undef, error}};
    SemanticCube[decimal][ADD] = {{integer, decimal}, {decimal, decimal}, {boolean, error}, {undef, error}};
    SemanticCube[decimal][SUB] = {{integer, decimal}, {decimal, decimal}, {boolean, error}, {undef, error}};

    // Populate SemanticCube for "boolean"
    SemanticCube[boolean][ASSIGN] = {{integer, error}, {decimal, error}, {boolean, boolean}, {undef, error}};
    SemanticCube[boolean][EQU] = {{integer, error}, {decimal, error}, {boolean, boolean}, {undef, error}};
    SemanticCube[boolean][GTR] = {{integer, boolean}, {decimal, error}, {boolean, boolean}, {undef, error}};
    SemanticCube[boolean][LSS] = {{integer, error}, {decimal, error}, {boolean, boolean}, {undef, error}};
    SemanticCube[boolean][NEQU] = {{integer, error}, {decimal, error}, {boolean, error}, {undef, error}};
    SemanticCube[boolean][MUL] = {{integer, error}, {decimal, error}, {boolean, error}, {undef, error}};
    SemanticCube[boolean][DIV] = {{integer, error}, {decimal, error}, {boolean, error}, {undef, error}};
    SemanticCube[boolean][ADD] = {{integer, error}, {decimal, error}, {boolean, error}, {undef, error}};
    SemanticCube[boolean][SUB] = {{integer, error}, {decimal, error}, {boolean, error}, {undef, error}};
}
