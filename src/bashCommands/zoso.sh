#!bin/sh 
./CocoSourcesCPP/Coco src/Zososcript.cpp.atg -namespace Zoso -o ./src/cpp 
cd src/cpp 
g++ *.cpp -o Zoso -g -Wall
./Zoso Test.zozo
cd ../..


g++ -std=c++11 *.cpp -o Zoso -g -Wall -I/opt/homebrew/Cellar/boost/1.85.0/include/