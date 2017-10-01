#include "smart_ptr.hpp"

#include <cassert>
#include <string>
#include <iostream>

class TestBased {
public:
    virtual std::string name() const {
        return "TestBased";
    }

    virtual ~TestBased() {
    
    }
};


class TestDerived: public TestBased {
public:
    virtual std::string name() const {
        return "TestDerived";
    }
};

void test_copy_assignment() {
    Rocco::SmartPtr<int> sp1(new int(111));
    Rocco::SmartPtr<int> sp2;
    sp2 = sp1;

    assert(*sp1 == 111);
    assert(*sp1 == *sp2);

    std::cout << "test_copy_assignment pass." << std::endl;
}

void test_copy_construction() {
    Rocco::SmartPtr<int> sp1(new int(100));
    Rocco::SmartPtr<int> sp2(sp1);

    assert(*sp1 == 100);
    assert(*sp1 == *sp2);

    std::cout << "test_copy_construction pass." << std::endl;
}

void test_get() {
    Rocco::SmartPtr<int> sp(new int(100));
    
    assert(&(*sp) == sp.get());

    std::cout << "test_get pass." << std::endl;
}

void test_operator_less() {
    Rocco::SmartPtr<int> sp1(new int(10));
    Rocco::SmartPtr<int> sp2(new int(20));

    assert(&(*sp1) < &(*sp2) == sp1 < sp2);

    std::cout << "test_operator_less pass." << std::endl;
}

void test_pointer_constructor() {
    Rocco::SmartPtr<int> sp1(new int(10));
    assert(*sp1 == 10);

    Rocco::SmartPtr<TestBased> sp2(new TestBased());
    assert(sp2->name() == "TestBased");

    std::cout << "test_pointer_constructor pass." << std::endl;
}


void test_swap() {
    Rocco::SmartPtr<int> sp1(new int(1));
    Rocco::SmartPtr<int> sp2(new int(2));
    Rocco::swap(sp1, sp2);

    assert(*sp1 == 2);
    assert(*sp2 == 1);

    std::cout << "test_swap pass." << std::endl;
}

void test_deleter() {
    int val = 1;
    auto deleter = [&val] (int *ptr) -> void { val = 2; };
    {
        Rocco::SmartPtr<int> sp(new int(), deleter);
    }

    assert(val == 2);

    std::cout << "test_deleter pass." << std::endl;
}

void test_make_smart() {
    auto ptr = Rocco::make_smart<int>(10);
    assert(*ptr == 10);

    std::cout << "test_make_smart pass." << std::endl;
}

int main() {

    test_copy_assignment();
    test_copy_construction();
    test_get();
    test_operator_less();
    test_pointer_constructor();
    test_swap();
    test_deleter();
    test_make_smart();

    getchar();
    return 0;
}