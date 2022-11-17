#ifndef __INFORMATION_MODEL_DEVICE_ELEMENT_GROUP_MOCK_HPP
#define __INFORMATION_MODEL_DEVICE_ELEMENT_GROUP_MOCK_HPP

#include "../DeviceElementGroup.hpp"
#include "Metric_MOCK.hpp"
#include "WritableMetric_MOCK.hpp"

#include "gmock/gmock.h"
#include <optional>
#include <unordered_map>

namespace Information_Model {
namespace testing {
/**
 * @brief DeviceElementGroup mock, use for testing only! Only use mocked
 * functions in your test cases!
 *
 */
class MockDeviceElementGroup : public DeviceElementGroup {
  using DeviceElementsMap =
      std::unordered_map<std::string, NonemptyDeviceElementPtr>;
  DeviceElementsMap elements_map_;
  size_t element_count_;
  std::string element_id_;

  /**
   * @brief Counts number of occurencies of a given pattern from a given cut of
   * marker. USED INTERNALLY
   *
   * @param input
   * @param pattern
   * @param cut_off
   * @return size_t
   */
  size_t countOccurrences(const std::string& input,
      const std::string& pattern,
      const std::string& cut_off) {
    size_t count = 0;
    std::string buffer = input.substr(input.find(cut_off) + 1, input.size());
    if (!buffer.empty()) {
      count++;
      size_t next_marker = buffer.find(pattern);
      while (next_marker != std::string::npos) {
        count++;
        next_marker = buffer.find(pattern, next_marker + pattern.size());
      }
    }
    return count;
  }

  /**
   * @brief Get the level of a current branch within a tree structured string.
   * USED INTERNALLY
   *
   * @param ref_id
   * @return size_t
   */
  size_t getTreeLevel(const std::string& ref_id) {
    return countOccurrences(ref_id, ".", ":");
  }

  /**
   * @brief get the position of a Nth occurrence of a given pattern inside  a
   * given string. USED INTERNALLY
   *
   * @param input
   * @param occurrence
   * @param pattern
   * @return size_t
   */
  size_t findNthSubstring(
      const std::string& input, size_t occurrence, const std::string& pattern) {
    if (0 == occurrence) {
      return std::string::npos;
    } else {
      size_t position, offset = 0;
      unsigned int i = 0;
      while (i < occurrence) {
        position = input.find(pattern, offset);
        if (std::string::npos == position) {
          break;
        } else {
          offset = position + pattern.size();
          i++;
        }
      }
      return position;
    }
  }

  /**
   * @brief Get the Next Element ID string. USED INTERNALLY
   *
   * @param child_ref_id
   * @param parent_level
   * @return std::string
   */
  std::string getNextElementID(
      const std::string& child_ref_id, size_t parent_level) {
    std::string tmp = child_ref_id.substr(
        0, findNthSubstring(child_ref_id, parent_level, "."));
    return tmp;
  }

  /**
   * @brief Generateds a new element reference ID based on the ID of this group
   *
   * @return std::string
   */
  std::string generateReferenceID() {
    auto base_id = element_id_;
    std::string sub_element_id =
        ((base_id.back() == ':') ? std::to_string(element_count_)
                                 : "." + std::to_string(element_count_));
    element_count_++;
    return base_id + sub_element_id;
  }

public:
  MockDeviceElementGroup(const std::string& ref_id)
      : DeviceElementGroup(), element_count_(0), element_id_(ref_id) {
    ON_CALL(*this, getSubelements).WillByDefault([this]() -> DeviceElements {
      std::vector<NonemptyDeviceElementPtr> subelements;
      // NOLINTNEXTLINE
      for (auto element_pair : elements_map_) {
        subelements.push_back(element_pair.second);
      }
      return subelements;
    });

    ON_CALL(*this, getSubelement)
        .WillByDefault([this](const std::string& ref_id) -> DeviceElementPtr {
          size_t target_level = getTreeLevel(ref_id) - 1;
          size_t current_level = getTreeLevel(element_id_);
          // Check if a given element is in a sub group
          if (target_level != current_level) {
            auto next_id = getNextElementID(ref_id, target_level);
            auto next_element = getSubelement(next_id);
            // Check if next element exists and is a group
            if (next_element) {
              auto next_group = std::get_if<NonemptyDeviceElementGroupPtr>(
                  &next_element->specific_interface);
              if (next_group)
                return (*next_group)->getSubelement(ref_id);
            }
          } // If not, check if it is in this group
          else if (elements_map_.find(ref_id) != elements_map_.end()) {
            return elements_map_.at(ref_id).base();
          }
          // If not, return an empty shared_ptr
          return DeviceElementPtr();
        });
  }

  /**
   * @brief Adds a new subgroup to this group.
   * Used by Information_Model::testing::DeviceMockBuilder, NOT FOR TESTING!
   *
   * @param name
   * @param desc
   * @return std::string
   */
  std::string addSubgroup(const std::string& name, const std::string& desc) {
    auto ref_id = generateReferenceID();
    NonemptyDeviceElementGroupPtr sub_group(NonemptyPointer::make_shared<
        ::testing::NiceMock<MockDeviceElementGroup>>(ref_id));
    std::pair<std::string, NonemptyDeviceElementPtr> element_pair(ref_id,
        NonemptyPointer::make_shared<DeviceElement>(
            ref_id, name, desc, sub_group));

    elements_map_.insert(element_pair);
    return ref_id;
  }

  /**
   * @brief Adds a new readable metric to this group.
   * Used by Information_Model::testing::DeviceMockBuilder, NOT FOR TESTING!
   *
   * @param name
   * @param desc
   * @param data_type
   * @param read_cb - read callback functor
   * @return std::string
   */
  std::string addReadableMetric(const std::string& name,
      const std::string& desc,
      DataType data_type,
      std::optional<std::function<DataVariant()>> read_cb) {
    auto ref_id = generateReferenceID();
    auto mock_metric =
        std::make_shared<::testing::NiceMock<MockMetric>>(data_type);
    NonemptyMetricPtr metric(mock_metric);
    if (read_cb.has_value()) {
      mock_metric->delegateToFake(read_cb.value());
    } else {
      mock_metric->delegateToFake();
    }

    std::pair<std::string, NonemptyDeviceElementPtr> element_pair(ref_id,
        NonemptyPointer::make_shared<DeviceElement>(
            ref_id, name, desc, metric));

    elements_map_.insert(element_pair);
    return ref_id;
  }

  /**
   * @brief Adds a new writable metric to this group.
   * Used by Information_Model::testing::DeviceMockBuilder, NOT FOR TESTING!
   *
   * @param name
   * @param desc
   * @param data_type
   * @param read_cb - read callback functor
   * @param write_cb - write callback functor
   * @return std::string
   */
  std::string addWritableMetric(const std::string& name,
      const std::string& desc,
      DataType data_type,
      std::optional<std::function<DataVariant()>> read_cb,
      std::optional<std::function<void(DataVariant)>> write_cb) {
    auto ref_id = generateReferenceID();
    auto mock_metric =
        std::make_shared<::testing::NiceMock<MockWritableMetric>>(data_type);
    NonemptyWritableMetricPtr metric(mock_metric);

    if (read_cb.has_value()) {
      if (write_cb.has_value()) {
        mock_metric->delegateToFake(read_cb.value(), write_cb.value());
      } else {
        mock_metric->delegateToFake(read_cb.value());
      }
    } else {
      mock_metric->delegateToFake();
    }

    std::pair<std::string, NonemptyDeviceElementPtr> element_pair(ref_id,
        NonemptyPointer::make_shared<DeviceElement>(
            ref_id, name, desc, metric));

    elements_map_.insert(element_pair);
    return ref_id;
  }

  MOCK_METHOD(DeviceElements, getSubelements, (), (override));
  MOCK_METHOD(DeviceElementPtr,
      getSubelement,
      (const std::string& /* ref_id */),
      (override));
};

using MockDeviceElementGroupPtr = std::shared_ptr<MockDeviceElementGroup>;
} // namespace testing
} // namespace Information_Model

#endif //__INFORMATION_MODEL_DEVICE_ELEMENT_GROUP_MOCK_HPP