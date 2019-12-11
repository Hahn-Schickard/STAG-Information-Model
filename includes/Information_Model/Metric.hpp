#ifndef __METRIC_HPP
#define __METRIC_HPP

#include "DeviceElement.hpp"

#include <exception>

namespace Information_Model {

  /**
   * @enum DataTypeEnum
   * @brief DataType enumeration, specifying the supported data types 
   * 
   */
  typedef enum DataTypeEnum {
    BOOLEAN, /*!< bool */
    BYTE,    /*!< uint8_t */
    SHORT,   /*!< int16_t */
    INTEGER, /*!< int32_t */
    LONG,    /*!< int64_t */
    FLOAT,   /*!< float */
    DOUBLE,  /*!< doable */
    STRING,  /*!< std::string */
    UNKNOWN  /*!< fallback type */
  } DataType;

  /**
   * @brief Metric - a specialization of DeviceElement class. 
   * Metrics deal with Reading/Writing/Observing various data points
   * 
   */
  class Metric : public DeviceElement {
   protected:
    Metric(const std::string& REF_ID,
        const std::string& NAME,
        const std::string& DESC,
        ElementType type,
        DataType data_type)
        : DeviceElement(REF_ID, NAME, DESC, type)
        , type_(data_type) {
      if(type_ == DataType::UNKNOWN) {
	throw std::runtime_error("Could not instantiaite metric " + REF_ID + ":"
	                         + NAME
	                         + ". Given data type is not supported!");
      }
    }

   public:
    DataType getDataType() {
      return type_;
    };

   private:
    DataType type_;
  };
}   // namespace Information_Model

#endif   //__METRIC_HPP