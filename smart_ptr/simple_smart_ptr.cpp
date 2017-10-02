#include "simple_smart_ptr.hpp"

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
    Rocco::SimpleSmartPtr<int> sp1(new int(111));
    Rocco::SimpleSmartPtr<int> sp2;
    sp2 = sp1;

    assert(*sp1 == 111);
    assert(*sp1 == *sp2);

    std::cout << "test_copy_assignment pass." << std::endl;
}

void test_copy_construction() {
    Rocco::SimpleSmartPtr<int> sp1(new int(100));
    Rocco::SimpleSmartPtr<int> sp2(sp1);

    assert(*sp1 == 100);
    assert(*sp1 == *sp2);

    std::cout << "test_copy_construction pass." << std::endl;
}

void test_get() {
    Rocco::SimpleSmartPtr<int> sp(new int(100));
    
    assert(&(*sp) == sp.get());

    std::cout << "test_get pass." << std::endl;
}

void test_pointer_constructor() {
    Rocco::SimpleSmartPtr<int> sp1(new int(10));
    assert(*sp1 == 10);

    Rocco::SimpleSmartPtr<TestBased> sp2(new TestBased());
    assert(sp2->name() == "TestBased");

    std::cout << "test_pointer_constructor pass." << std::endl;
}





int main() {
    test_copy_assignment();
    test_copy_construction();
    test_get();
    test_pointer_constructor();

    getchar();
    return 0;
}