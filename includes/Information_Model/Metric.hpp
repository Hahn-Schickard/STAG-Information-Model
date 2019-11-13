#ifndef METRIC_HPP
#define METRIC_HPP

#include "DataType.hpp"
#include "NamedElement.hpp"
#include <string>

namespace Information_Model
{

    class Metric : public NamedElement {
        protected:
            Metric( const std::string REF_ID, const std::string NAME,
                    const std::string DESC, Information_Model::DataType valueType): NamedElement(REF_ID, NAME, DESC) , value_type_(valueType) {}
        private:
            DataType value_type_;
        public:
        DataType getMetricValueType() { return value_type_; }
    };
} // namespace Information_Model
#endif //METRIC_HPP