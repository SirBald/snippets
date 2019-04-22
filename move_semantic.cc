#include <string>
#include <type_traits>
#include <typeinfo>
#include <utility>

#include "utility_print-inl.h"
#include "utility_test_executor-inl.h"


namespace {

  class MoveableStruct final {
  public:
    explicit MoveableStruct(const std::string& description)
      : description_{description} {

      utility::print("Ctor: ", utility::quote(description_));
    }

    MoveableStruct(const MoveableStruct& other) {
      utility::print("Copy ctor: ", utility::quote(description_), " ---> ", utility::quote(other.description_));
      description_ = other.description_;
    }

    MoveableStruct(MoveableStruct&& other)
      : description_{} {

      utility::print("Move ctor: ", utility::quote(description_), " ---> ", utility::quote(other.description_));
      description_ = std::move(other.description_);
    }

    MoveableStruct& operator=(MoveableStruct&& other) {
      utility::print("Move assignment: ", utility::quote(description_), " ---> ", utility::quote(other.description_));
      description_ = std::move(other.description_);

      return *this;
    }

  private:
    friend std::ostream& operator<<(std::ostream& stream, const MoveableStruct& moveable_struct);

    std::string description_;
  }; // struct MoveableStruct


  std::ostream& operator<<(std::ostream& stream, const MoveableStruct& moveable_struct) {
    stream << utility::quote(moveable_struct.description_);
    return stream;
  }


  MoveableStruct createMoveableStruct() {
    return MoveableStruct{"i am from createMoveableStruct"};
  }


  MoveableStruct returnLocalVariableByValue() {
    MoveableStruct local{"i am from returnLocalVariableByValue"};
    return local;
  }


  template <typename T>
  std::string getTypeName(T&& object) {
    std::string type_name{typeid(object).name()};

    if (std::is_lvalue_reference<decltype(object)>::value) {
      static const auto kLValueReference = " l-value reference";
      type_name += kLValueReference;
    } else if (std::is_rvalue_reference<decltype(object)>::value) {
      static const auto kRValueReference = " r-value reference";
      type_name += kRValueReference;
    }

    return type_name;
  }


  // gets object by forwarding reference
  template <typename T, typename Func>
  void forwardFurther(T&& object, const Func& function) {
    utility::print("forwardFurther uses T: ", getTypeName(std::forward<T>(object)));
    function(std::forward<T>(object));
  }


  // gets object by forwarding reference
  template <typename T, typename Func>
  void passFurtherAsIs(T&& object, const Func& function) {
    utility::print("passFurtherAsIs uses T: ", getTypeName(object));
    function(object);
  }


  // gets object by r-value reference
  template <typename T>
  void passTemplateByRValueRef(const T&& object) {
    utility::print("passTemplateByRValueRef has got T: ", getTypeName(std::move(object)));
  }


  void passByRef(const MoveableStruct& object) {
    utility::print("passByRef has got MoveableStruct: ", object);
  }


  void passByRValueRef(MoveableStruct&& object) {
    utility::print("COPY is done for MoveableStruct because named variable is l-value even if its type is r-value reference:");
    const auto copied = object;

    utility::print("Actual MOVE is done for MoveableStruct:");
    const auto moved = std::move(object);
  }


  void passByValue(MoveableStruct object) {
    utility::print("passByValue has got MoveableStruct: ", object);
  }

} // anonymous namespace


void forwardingReferenceTest() {
  utility::print(
    "forwardingReferenceTest: "\
    "Object is passed by l-value or r-value reference to a function which forwards it further or passes it further as is.");
  utility::printEmptyLine();

  utility::runCase("CASE 1.1: L-VALUE: FORWARDED to passByValue:", []() {
    MoveableStruct forwarded{"forwarded"};
    forwardFurther(forwarded, &passByValue);
  });
  utility::runCase("CASE 1.2: L-VALUE: PASSED AS IS to passByValue:", []() {
    MoveableStruct passed_as_is{"passed as is"};
    passFurtherAsIs(passed_as_is, &passByValue);
  });

  utility::runCase("CASE 2.1: R-VALUE: FORWARDED to passByValue:", []() {
    MoveableStruct forwarded{"forwarded"};
    forwardFurther(std::move(forwarded), &passByValue);
    forwardFurther(MoveableStruct{"temporarily constructed"}, &passByValue);
  });
  utility::runCase("CASE 2.2: R-VALUE: PASSED AS IS to passByValue:", []() {
    MoveableStruct passed_as_is{"passed as is"};
    passFurtherAsIs(std::move(passed_as_is), &passByValue);
    passFurtherAsIs(MoveableStruct{"temporarily constructed"}, &passByValue);
  });

  utility::runCase("CASE 3.1: L-VALUE: FORWARDED to passByRef:", []() {
    MoveableStruct forwarded{"forwarded"};
    forwardFurther(forwarded, &passByRef);
  });
  utility::runCase("CASE 3.2: L-VALUE: PASSED AS IS to passByRef:", []() {
    MoveableStruct passed_as_is{"passed as is"};
    passFurtherAsIs(passed_as_is, &passByRef);
  });

  utility::runCase("CASE 4.1: R-VALUE: FORWARDED to passByRef:", []() {
    MoveableStruct forwarded{"forwarded"};
    forwardFurther(std::move(forwarded), &passByRef);
    forwardFurther(MoveableStruct{"temporarily constructed"}, &passByRef);
  });
  utility::runCase("CASE 4.2: R-VALUE: PASSED AS IS to passByRef:", []() {
    MoveableStruct passed_as_is{"passed as is"};
    passFurtherAsIs(std::move(passed_as_is), &passByRef);
    passFurtherAsIs(MoveableStruct{"temporarily constructed"}, &passByRef);
  });

  // error: cannot bind rvalue reference of type ‘{anonymous}::MoveableStruct&&’ to lvalue of type ‘{anonymous}::MoveableStruct’
  //      function(std::forward<T>(object));
  //      ~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~
  // utility::runCase("CASE 5.1: L-VALUE: FORWARDED to passByRValueRef:", []() {
  //   MoveableStruct forwarded{"forwarded"};
  //   forwardFurther(forwarded, &passByRValueRef);
  // });
  // error: cannot bind rvalue reference of type ‘{anonymous}::MoveableStruct&&’ to lvalue of type ‘{anonymous}::MoveableStruct’
  //      function(object);
  //      ~~~~~~~~^~~~~~~~
  // utility::runCase("CASE 5.2: L-VALUE: PASSED AS IS to passByRValueRef:", []() {
  //   MoveableStruct passed_as_is{"passed as is"};
  //   passFurtherAsIs(passed_as_is, &passByRValueRef);
  // });

  utility::runCase("CASE 6.1: R-VALUE: FORWARDED to passByRValueRef:", []() {
    MoveableStruct forwarded{"forwarded"};
    forwardFurther(std::move(forwarded), &passByRValueRef);
    forwardFurther(MoveableStruct{"temporarily constructed"}, &passByRValueRef);
  });
  // error: cannot bind rvalue reference of type ‘{anonymous}::MoveableStruct&&’ to lvalue of type ‘{anonymous}::MoveableStruct’
  //      function(object);
  //      ~~~~~~~~^~~~~~~~
  // utility::runCase("CASE 6.2: R-VALUE: PASSED AS IS to passByRValueRef:", []() {
  //   MoveableStruct passed_as_is{"passed as is"};
  //   passFurtherAsIs(std::move(passed_as_is), &passByRValueRef);
  //   passFurtherAsIs(MoveableStruct{"temporarily constructed"}, &passByRValueRef);
  // });
}


void passByRValueReferenceTest() {
  utility::print(
    "passByRValueReferenceTest: Object is passed by r-value reference (instead of forwarding reference) to a template function.");
  utility::printEmptyLine();

  const MoveableStruct permanent{"permanently constructed"};

  // error: cannot bind rvalue reference of type ‘const {anonymous}::MoveableStruct&&’ to lvalue of type ‘{anonymous}::MoveableStruct’
  //    passTemplateByRValueRef(permanent);
  //                                     ^
  // BUT WORKS in forwardingReferenceTest because forwarding reference with const modifier (const T&&) is just a r-value reference.
  // passTemplateByRValueRef(permanent);

  passTemplateByRValueRef(std::move(permanent));
  passTemplateByRValueRef(MoveableStruct{"temporarily constructed"});
}


void passByValueTest() {
  utility::print("passByValueTest: Object is passed by value to a function.");
  utility::printEmptyLine();

  utility::runCase("CASE 1: MOVED (RVO):", []() {
    passByValue(createMoveableStruct());
  });

  utility::runCase("CASE 2: MOVED", []() {
    passByValue(std::move(createMoveableStruct()));
  });

  utility::runCase("CASE 3: MOVED (RVO)", []() {
    passByValue(MoveableStruct{"temporarily constructed"});
  });

  utility::runCase("CASE 4: MOVED", []() {
    passByValue(std::move(MoveableStruct{"temporarily constructed"}));
  });

  utility::runCase("CASE 5: COPIED", []() {
    MoveableStruct case_5{"permanently constructed"};
    passByValue(case_5);
  });

  utility::runCase("CASE 6: MOVED", []() {
    MoveableStruct case_6{"permanently constructed"};
    passByValue(std::move(case_6));
  });
}


void returnLocalVariableTest() {
  utility::print("returnLocalVariableTest: Object is created inside a function and returned by value.");
  utility::printEmptyLine();

  utility::runCase("CASE 1: MOVED", []() {
    MoveableStruct case_1{"i am waiting to be assigned"};
    case_1 = returnLocalVariableByValue();
  });

  utility::runCase("CASE 2: MOVED (RVO)", []() {
    const auto case_2 = returnLocalVariableByValue();
  });
}


int main() {
  utility::runSuite(&returnLocalVariableTest);

  utility::runSuite(&passByValueTest);

  utility::runSuite(&forwardingReferenceTest);

  passByRValueReferenceTest();
}
