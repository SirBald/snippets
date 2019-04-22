#ifndef UTILITY_TEST_EXECUTOR_INL_H_INCLUDED
#define UTILITY_TEST_EXECUTOR_INL_H_INCLUDED

#include <string>

#include "utility_print-inl.h"

namespace utility {

  template <typename Func>
  void runCase(const std::string& message, const Func& case_to_run) {
    utility::print(message);
    case_to_run();
    utility::printEmptyLine();
  }


  template <typename Func>
  void runSuite(const Func& suite) {
    suite();
    utility::printEmptyLines(2u);
  }

} // namespace utility

#endif // UTILITY_TEST_EXECUTOR_INL_H_INCLUDED
