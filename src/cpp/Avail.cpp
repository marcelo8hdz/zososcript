#include "Avail.h"

Avail::Avail() : current(-1) {}

int Avail::next() {
    current++;
    return current;
}


void Avail::reset_counter() {
    current = -1;
}


