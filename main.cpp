#include <iostream>

#include <localisation/system.hpp>

int main() {
    loc::system.load_from_directory("../example-resources");

    loc::system.set_default_language("en");
    loc::system.set_now_language("ru");

    std::string str32_test{loc::system.get_text("text")};
    std::cout << str32_test << std::endl;
}
