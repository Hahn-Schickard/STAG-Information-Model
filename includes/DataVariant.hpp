/* Copyright (C) 2018, Nikolai Wuttke. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __DATA_MODEL_DATA_VARIANT_HPP_
#define __DATA_MODEL_DATA_VARIANT_HPP_

#include <string>
#include <utility>
#include <variant>

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

using DataVariant = std::variant<bool, uint8_t, int16_t, int32_t, int64_t,
                                 float, double, std::string>;

namespace detail {

template <class... Ts> struct overloaded : Ts... { using Ts::operator()...; };

template <class... Ts> overloaded(Ts...)->overloaded<Ts...>;

} // namespace detail

/**
 * @brief CXX17 Data Type pattern matcher method based on visitor pattern
 * Takes in a variadic ammount of lambdas to visit each data type within a given
 * Variant type
 *
 * @tparam Variant - cpp17 variant data type
 * @tparam Matchers - Mathcer functor parameter pack
 * @param variant -  Input Data Variant to match against
 * @param matchers - Data type pattern matcher lambdas
 * @return auto
 */
template <typename Variant, typename... Matchers>
auto match(Variant &&variant, Matchers &&... matchers) {
  return std::visit(detail::overloaded{std::forward<Matchers>(matchers)...},
                    std::forward<Variant>(variant));
}

} // namespace Information_Model
#endif //__DATA_MODEL_DATA_VARIANT_HPP_