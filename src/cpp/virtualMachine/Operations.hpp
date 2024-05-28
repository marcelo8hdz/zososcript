#include <map>
#include <boost/variant.hpp>
#include <iostream>
#include <wchar.h>

using MemoryValue = boost::variant<int, float, bool, std::string>;

// Utility functions to perform arithmetic operations
MemoryValue add(const MemoryValue& a, const MemoryValue& b) {
    if (const int* ai = boost::get<int>(&a)) {
        if (const int* bi = boost::get<int>(&b)) return *ai + *bi;
        if (const float* bf = boost::get<float>(&b)) return *ai + *bf;
    } else if (const float* af = boost::get<float>(&a)) {
        if (const int* bi = boost::get<int>(&b)) return *af + *bi;
        if (const float* bf = boost::get<float>(&b)) return *af + *bf;
    }
    throw std::runtime_error("Invalid types for add operation");
}

MemoryValue sub(const MemoryValue& a, const MemoryValue& b) {
    if (const int* ai = boost::get<int>(&a)) {
        if (const int* bi = boost::get<int>(&b)) return *ai - *bi;
        if (const float* bf = boost::get<float>(&b)) return *ai - *bf;
    } else if (const float* af = boost::get<float>(&a)) {
        if (const int* bi = boost::get<int>(&b)) return *af - *bi;
        if (const float* bf = boost::get<float>(&b)) return *af - *bf;
    }
    throw std::runtime_error("Invalid types for sub operation");
}

MemoryValue mul(const MemoryValue& a, const MemoryValue& b) {
    if (const int* ai = boost::get<int>(&a)) {
        if (const int* bi = boost::get<int>(&b)) return *ai * *bi;
        if (const float* bf = boost::get<float>(&b)) return *ai * *bf;
    } else if (const float* af = boost::get<float>(&a)) {
        if (const int* bi = boost::get<int>(&b)) return *af * *bi;
        if (const float* bf = boost::get<float>(&b)) return *af * *bf;
    }
    throw std::runtime_error("Invalid types for mul operation");
}

MemoryValue div(const MemoryValue& a, const MemoryValue& b) {
    if (const int* ai = boost::get<int>(&a)) {
        if (const int* bi = boost::get<int>(&b)) {
            if (*bi == 0) throw std::runtime_error("Division by zero");
            return *ai / *bi;
        }
        if (const float* bf = boost::get<float>(&b)) {
            if (*bf == 0.0f) throw std::runtime_error("Division by zero");
            return *ai / *bf;
        }
    } else if (const float* af = boost::get<float>(&a)) {
        if (const int* bi = boost::get<int>(&b)) {
            if (*bi == 0) throw std::runtime_error("Division by zero");
            return *af / *bi;
        }
        if (const float* bf = boost::get<float>(&b)) {
            if (*bf == 0.0f) throw std::runtime_error("Division by zero");
            return *af / *bf;
        }
    }
    throw std::runtime_error("Invalid types for div operation");
}

MemoryValue lessThan(const MemoryValue& a, const MemoryValue& b) {
    if (const int* ai = boost::get<int>(&a)) {
        if (const int* bi = boost::get<int>(&b)) {
            return *ai < *bi;
        } else if (const float* bf = boost::get<float>(&b)) {
            return *ai < *bf;
        }
    } else if (const float* af = boost::get<float>(&a)) {
        if (const int* bi = boost::get<int>(&b)) {
            return *af < *bi;
        } else if (const float* bf = boost::get<float>(&b)) {
            return *af < *bf;
        }
    }
}

MemoryValue greaterThan(const MemoryValue& a, const MemoryValue& b) {
    if (const int* ai = boost::get<int>(&a)) {
        if (const int* bi = boost::get<int>(&b)) {
            return *ai > *bi;
        } else if (const float* bf = boost::get<float>(&b)) {
            return *ai > *bf;
        }
    } else if (const float* af = boost::get<float>(&a)) {
        if (const int* bi = boost::get<int>(&b)) {
            return *af > *bi;
        } else if (const float* bf = boost::get<float>(&b)) {
            return *af > *bf;
        }
    }
}

void print(const std::string value) {
    std::cout << "PRINT: " << value << std::endl;
}

void print(const std::wstring value) {
    std::wcout << L"PRINT: " << value << std::endl;
}

void print(const char* value) {
    std::cout << "const char";
    std::cout << "PRINT: " << value << std::endl;
    print(std::string(value));
}

void print(bool value) {
    std::cout << "PRINT: " << (value ? "true" : "false")<< std::endl;
}

void print(int value) {
    std::cout << "PRINT: " << value << std::endl;
}

void print(float value) {
    std::cout << "PRINT: " << value << std::endl;
}

void print(MemoryValue value) {
    if (const std::string* ai = boost::get<std::string>(&value)) print(*ai);
    else if (const int* ai = boost::get<int>(&value)) print(*ai);
    else if (const float* af = boost::get<float>(&value)) print(*af);
    else if (const bool* ab = boost::get<bool>(&value)) print(*ab);
}

