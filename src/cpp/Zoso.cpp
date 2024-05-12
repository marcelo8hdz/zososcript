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


    Scanner* scanner = new Scanner(inputFile);
    Parser* parser = new Parser(scanner);

    SymbolTable* symbolTable = new SymbolTable(parser);


    parser -> Parse();

    coco_string_delete(inputFile);
    std::cout << "Parsing completed." << endl;

    return 0;
}