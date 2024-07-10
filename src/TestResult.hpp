#ifndef TESTRESULT_HPP
#define TESTRESULT_HPP

#include <string>

struct TestResult {
  std::string testName;
  bool passed;
  std::string details;
};

#endif
