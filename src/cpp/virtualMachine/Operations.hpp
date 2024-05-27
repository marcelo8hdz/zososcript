#include <map>
#include <boost/variant.hpp>

using MemoryValue = boost::variant<int, float, bool>;

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

