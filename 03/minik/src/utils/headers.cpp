#include "headers.hpp"
#include <iostream>


void pass(const std::string &name)
{
    std::cerr
        << "\n"
        << "==========================================================\n"
        << "# pass: " << name << "\n"
        << "----------------------------------------------------------\n"
        << "\n";
}

void subpass(const std::string &title1, const std::string &title2)
{
    std::cerr
        << "\n"
        << "----------------------------------------------------------\n"
        << "# " << title1 << "\n";

    if (not title2.empty()) std::cerr
        << "# " << title2 << "\n";

    std::cerr
        << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - \n"
        << "\n";
}

void step(const std::string &title1, const std::string &title2)
{
    std::cerr
        << "\n"
        << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - \n"
        << title1 << "\n";

    if (not title2.empty()) std::cerr
        << title2 << "\n";

    std::cerr
        << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - \n"
        << "\n";
}

