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
    std::cout << "read file" << std::endl;

    Scanner* scanner = new Scanner(inputFile);
    std::cout << "created scanner" << std::endl;
    Parser* parser = new Parser(scanner);
    std::cout << "created parser" << std::endl;
    

    parser -> symbolTable = new SymbolTable(parser);
    parser -> codeGenerator = new CodeGenerator();
    printf("created symbolTable");
    
    parser -> Parse();
    printf("parsed");

    coco_string_delete(inputFile);
    std::cout << "Parsing completed." << endl;

    parser -> codeGenerator -> printQuads();
    parser -> codeGenerator -> printConstantMap();

    return 0;
}