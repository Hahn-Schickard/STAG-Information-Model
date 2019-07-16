#ifndef _INFORMATION_MODEL_EXCEPTONS_HPP
#define _INFORMATION_MODEL_EXCEPTONS_HPP
#include <string>

namespace Model_Factory {

struct InvalidReferenceIdException : public std::exception {
  std::string desc;
  const char *what() const throw();
  InvalidReferenceIdException() { this->desc = "Invalid Reference Id"; }
  InvalidReferenceIdException(std::string desc) { this->desc = desc; }
};

struct ElementTypeMismatchException : public std::exception {
  std::string desc;
  const char *what() const throw();
  ElementTypeMismatchException() {}
  ElementTypeMismatchException(std::string desc) { this->desc = desc; }
};

struct FunctionElementException : public std::exception {
  std::string desc;
  const char *what() const throw();
  FunctionElementException() {}
  FunctionElementException(std::string desc) { this->desc = desc; }
};
}

#endif //_INFORMATION_MODEL_EXCEPTONS_HPP