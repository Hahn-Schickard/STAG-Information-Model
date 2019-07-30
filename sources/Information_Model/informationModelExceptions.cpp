#include <exception>

#include "informationModelExceptions.hpp"

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

const char * FunctionElementException::what() const throw ()
{
	std::string str = "FunctionElement caused an exception: ";
	return (str + desc).c_str();
}

const char * UndefinedElementTypeException::what() const throw ()
{
	std::string str = "Undefined ElementType: ";
	return (str + desc).c_str();
}


const char * GroupElementDoesNotExistException::what() const throw ()
{
	std::string str = "Group element does not exist : ";
	return (str + desc).c_str();
}