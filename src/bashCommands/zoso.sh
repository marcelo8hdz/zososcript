#!bin/sh 
./CocoSourcesCPP/Coco src/ZososcriptGrammar.cpp.atg -namespace Zoso -o ./src/cpp 
cd src/cpp 
g++ *.cpp -o Zoso -g -Wall
./Zoso Test.zozo
cd ../..