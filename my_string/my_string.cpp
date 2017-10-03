#include "my_string.hpp"
#include <vector>

int main() {
    MyString s;
    s = MyString("hello");
    std::vector<MyString> vs;
    vs.push_back(MyString("world"));

    getchar();
    return 0;
}