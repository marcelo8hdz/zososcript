#include "Avail.h"

Avail::Avail() : current(0) {}

int Avail::next() {
    current++;
    return current;
}


void Avail::reset_counter() {
    current = -1;
}


void Avail::pushValueToMemory(int value) {
    values[current] = value;
}

