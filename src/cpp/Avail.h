#if !defined(AVAIL_H)
#define AVAIL_H
#include <string>

class Avail {
private:
    int current;

public:
    Avail();
    int next();
    void reset_counter();
    //free memory
};

#endif // AVAIL_H
