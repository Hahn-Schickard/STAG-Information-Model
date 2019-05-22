#include <exception>

#include "informationModelExceptions.h"

const char * InvalidReferenceIdException::what() const throw ()
{
	std::string str = "Invalid ReferenceId: ";
	return (str + desc).c_str();
}

const char * ElementTypeMismatchException::what() const throw ()
{
	std::string str = "Element Types are mismatched: ";
	return (str + desc).c_str();
}