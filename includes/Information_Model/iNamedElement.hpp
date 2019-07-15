#ifndef _I_NAMED_ELEMENT_HPP
#define _I_NAMED_ELEMENT_HPP

#include <string>

class iNamedElement
{
public:
    virtual const std::string getElementName() = 0;
    virtual const std::string getElementDescription() = 0;
    virtual const std::string getElementRefId() = 0;

protected:
    iNamedElement();
};

#endif //_I_NAMED_ELEMENT_HPP