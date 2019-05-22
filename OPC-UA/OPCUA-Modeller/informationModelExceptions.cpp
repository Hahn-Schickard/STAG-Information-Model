#include <exception>

#include "informationModelExceptions.h"

const char * InvalidReferenceIdException::what() const throw ()
{
	std::string str = "Invalid ReferenceId: ";
	return (str + desc).c_str();
}