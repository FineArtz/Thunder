#ifndef OTHERS
#define OTHERS

#include <string>
#include <sstream>

template<class T>
std::string toString(const T &a){
    std::ostringstream oss;
    oss << a;
    return oss.str();
}

#endif // OTHERS
