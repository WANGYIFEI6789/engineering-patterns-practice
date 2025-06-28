#include "strutil.hpp"
#include <iostream>

int main(){
    std::string s = "AbcDef";
    std::cout << duan::strutil::upper(s) << std::endl;    // ABCDEF
    std::cout << duan::strutil::reverse(s) << std::endl;  // feDcbA
    return 0;
}