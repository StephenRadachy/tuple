#include <iostream>
#include <string>
#include "tuple.hpp"

int main() {
    tuple t1 {1, 2.0, std::string("hello"), std::string("world")};
    tuple t2 {1, 2.0, std::string("hello"), std::string("world")};

    std::cout << get<0>(t1) << ", " << get<1>(t1) << ", " << get<2>(t1) << ", " << get<3>(t1) << std::endl;
    std::cout << "tuples are equal: " << std::boolalpha << (t1 == t2) << std::endl;

    return 0;
}