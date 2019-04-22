#include <iostream>
#include <memory>

namespace {

  class Base {
  public:
    Base() {
      std::cerr << "Base()" << std::endl;
    }

    virtual ~Base() {
      std::cerr << "~Base()" << std::endl;
    }
  };


  class BaseNoVirtualDtor {
  public:
    BaseNoVirtualDtor() {
      std::cerr << "BaseNoVirtualDtor()" << std::endl;
    }

    ~BaseNoVirtualDtor() {
      std::cerr << "~BaseNoVirtualDtor()" << std::endl;
    }
  };


  class DerivedFromBase : public Base {
  public:
    DerivedFromBase() {
      std::cerr << "DerivedFromBase()" << std::endl;
    }

    virtual ~DerivedFromBase() {
      std::cerr << "~DerivedFromBase()" << std::endl;
    }
  };


  class DerivedFromBaseNoVirtualDtor : public BaseNoVirtualDtor {
  public:
    DerivedFromBaseNoVirtualDtor() {
      std::cerr << "DerivedFromBaseNoVirtualDtor()" << std::endl;
    }

    virtual ~DerivedFromBaseNoVirtualDtor() {
      std::cerr << "~DerivedFromBaseNoVirtualDtor()" << std::endl;
    }
  };

}


void memoryLeakTest() {
  std::cerr << "Via Derived instance:" << std::endl;
  {
    DerivedFromBaseNoVirtualDtor derived;
  }

  std::cerr << std::endl << "Via Derived pointer:" << std::endl;
  {
    std::unique_ptr<DerivedFromBaseNoVirtualDtor> derived{new DerivedFromBaseNoVirtualDtor{}};
  }

  std::cerr << std::endl << "Via Base pointer (MEMORY LEAK IS DETECTED):" << std::endl;
  {
    std::unique_ptr<BaseNoVirtualDtor> BaseNoVirtualDtor{new DerivedFromBaseNoVirtualDtor{}};
  }

  std::cerr << std::endl << "Via Base pointer (with virtual dtor everything is fine):" << std::endl;
  {
    std::unique_ptr<Base> Base{new DerivedFromBase{}};
  }
}


int main() {
  memoryLeakTest();
}
