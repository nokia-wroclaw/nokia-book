#pragma once

#include <iostream>
#include <string>

namespace Msg {

inline void error(std::string val) {
    std::cerr << "Error: " << val << std::endl;
    exit(-1);
}

inline void warning(std::string val) {
    std::cerr << "Warning: " << val << std::endl;
}

inline void log(std::string val) {
    std::cerr << val << std::endl;
}

inline std::string cite(std::string cited) {
    return "'" + cited + "'";
}

}
