#ifndef INFORMATIONMODELEXCEPTONS_H
#define INFORMATIONMODELEXCEPTONS_H
#include <string>

struct InvalidReferenceIdException : public std::exception {
	std::string desc;
	const char * what() const throw ();
	InvalidReferenceIdException(){}
	InvalidReferenceIdException(std::string desc)
	{
		this->desc = desc;
	}
};

#endif
