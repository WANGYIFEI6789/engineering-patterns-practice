#include "strutil.hpp"
#include <algorithm>
#include <cctype>

namespace duan {
    namespace strutil {
        std::string reverse(const std::string &s) {
            std::string r = s;
            std::reverse(r.begin(), r.end());
            return r;
        }
        std::string upper(const std::string &s) {
            std::string r = s;
            std::transform(r.begin(), r.end(), r.begin(),
                [](unsigned char c){ return std::toupper(c); });
            return r;
        }
    }
}