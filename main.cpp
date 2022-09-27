#include <iostream>

#include "lib/loc/localizationSystem.hpp"

int main() {
    loc::system.loadFromDirectory("../test");

    loc::system.setDefaultLanguage("en");
    loc::system.setNowLanguage("en");

    std::u32string testStr{loc::system.getText("in")};
    for (int i = 0; i < testStr.size(); ++i) {
        std::cout << static_cast<char>(testStr[i]);
    }
    std::cout << std::endl;
}
