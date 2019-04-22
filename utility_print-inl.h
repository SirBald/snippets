#ifndef UTILITY_PRINT_INL_H_INCLUDED
#define UTILITY_PRINT_INL_H_INCLUDED

#include <iostream>
#include <sstream>

namespace utility {

  template <typename T>
  std::string quote(const T& what) {
    static const auto kSingleQuote = "'";

    std::ostringstream stream;
    stream << kSingleQuote << what << kSingleQuote;

    return stream.str();
  }

  template <typename T>
  void print(const T& last_arg) {
    std::cerr << last_arg << std::endl;
  }

  template <typename T, typename ...Args>
  void print(const T& first_arg, const Args&... args) {
    std::cerr << first_arg;
    print(args...);
  }

  void printEmptyLines(const size_t number_of_lines) {
    for (auto i = number_of_lines; i > 0; --i) {
      std::cerr << std::endl;
    }
  }

  void printEmptyLine() {
    printEmptyLines(1u);
  }

} // namespace utility

#endif // UTILITY_PRINT_INL_H_INCLUDED
