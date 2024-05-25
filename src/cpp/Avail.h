#if !defined(AVAIL_H)
#define AVAIL_H
#include <string>
#include <map>


class Avail {
private:
    int current;
    std::map<int, std::string>  values;

public:
    Avail();
    int next();
    void reset_counter();
    void pushValueToMemory(int value);

    //free memory

};

#endif // AVAIL_H
