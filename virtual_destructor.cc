#include <memory>

#include "utility_print-inl.h"
#include "utility_test_executor-inl.h"


namespace {

  class Base {
  public:
    Base() {
      utility::print("Base()");
    }

    virtual ~Base() {
      utility::print("~Base()");
    }
  };


  class BaseNoVirtualDtor {
  public:
    BaseNoVirtualDtor() {
      utility::print("BaseNoVirtualDtor()");
    }

    ~BaseNoVirtualDtor() {
      utility::print("~BaseNoVirtualDtor()");
    }
  };


  class DerivedFromBase : public Base {
  public:
    DerivedFromBase() {
      utility::print("DerivedFromBase()");
    }

    virtual ~DerivedFromBase() {
      utility::print("~DerivedFromBase()");
    }
  };


  class DerivedFromBaseNoVirtualDtor : public BaseNoVirtualDtor {
  public:
    DerivedFromBaseNoVirtualDtor() {
      utility::print("DerivedFromBaseNoVirtualDtor()");
    }

    virtual ~DerivedFromBaseNoVirtualDtor() {
      utility::print("~DerivedFromBaseNoVirtualDtor()");
    }
  };

}


void memoryLeakTest() {
  utility::runCase("CASE 1: Via Derived instance:", []() {
    DerivedFromBaseNoVirtualDtor derived;
  });

  utility::runCase("CASE 2: Via Derived pointer:", []() {
    std::unique_ptr<DerivedFromBaseNoVirtualDtor> derived{new DerivedFromBaseNoVirtualDtor{}};
  });

  utility::runCase("CASE 3: Via Base pointer (MEMORY LEAK IS DETECTED):", []() {
    std::unique_ptr<BaseNoVirtualDtor> BaseNoVirtualDtor{new DerivedFromBaseNoVirtualDtor{}};
  });

  utility::runCase("CASE 4: Via Base pointer (with virtual dtor everything is fine):", []() {
    std::unique_ptr<Base> Base{new DerivedFromBase{}};
  });
}


int main() {
  memoryLeakTest();
}
