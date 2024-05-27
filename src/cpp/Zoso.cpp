#include <iostream>
#include <fstream>
#include "Parser.h"
#include "Scanner.h"
#include "SymbolTable.h"
using namespace Zoso;
using namespace std;

int main(int argc, char* argv[]) {
    
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <source_file>" << endl;
        return 1;
    }

    wchar_t *inputFile = coco_string_create(argv[1]);
    if (!inputFile) {
        cerr << "Error: Could not open the file '" << argv[1] << "' for reading." << endl;
        return 1;
    }
    cout << "read file" << endl;

    Scanner* scanner = new Scanner(inputFile);
    cout << "created scanner" << endl;
    Parser* parser = new Parser(scanner);
    cout << "created parser" << endl;
    

    parser -> symbolTable = new SymbolTable(parser);
    parser -> codeGenerator = new CodeGenerator();
    printf("created symbolTable\n");
    
    parser -> Parse();
    printf("parsed\n");

    coco_string_delete(inputFile);
    cout << "Parsing completed.\n" << endl;

    parser -> codeGenerator -> printQuads();
    parser -> codeGenerator -> printConstantMap();
    
    parser -> codeGenerator -> printConstantMapToFile("virtualMachine/ConstantMap.h");
    parser -> codeGenerator -> printCodeVectorToFile("virtualMachine/CodeVector.h");

    return 0;
}