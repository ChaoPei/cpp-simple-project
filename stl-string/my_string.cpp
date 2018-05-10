#include "my_string.hpp"
#include <vector>

int main() {
    Rocco::MyString s;
    s = Rocco::MyString("hello");
    std::vector<Rocco::MyString> vs;
    vs.push_back(Rocco::MyString("world"));

    getchar();
    return 0;
}