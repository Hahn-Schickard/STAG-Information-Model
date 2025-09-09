#include "MockBuilder.hpp"

#include "ElementMock.hpp"

namespace Information_Model::testing {
using namespace std;
using namespace ::testing;

void MockBuilder::setDeviceInfo(
    const string& unique_id, const BuildInfo& element_info) {
  result_ = make_unique<DeviceMock>(
      unique_id, FullMetaInfo{element_info.name, element_info.description});
}

string MockBuilder::addGroup(const BuildInfo& element_info) {
  auto id = result_->generateID();
  auto group = make_shared<NiceMock<GroupMock>>(id);
  subgroups_.try_emplace(id, group);
  addElementMock(group, id, element_info);
  return id;
}

string MockBuilder::addGroup(
    const string& parent_id, const BuildInfo& element_info) {
  auto subgroup = getParentGroup(parent_id);
  auto id = subgroup->generateID();
  auto group = make_shared<NiceMock<GroupMock>>(id);
  subgroups_.try_emplace(id, group);
  addElementMock(group, id, element_info);
  return id;
}

string MockBuilder::addReadable(const BuildInfo& element_info,
    DataType data_type,
    const ReadCallback& read_cb) {
  auto id = result_->generateID();
  auto readable = make_shared<NiceMock<ReadableMock>>(data_type, read_cb);
  addElementMock(readable, id, element_info);
  return id;
}

string MockBuilder::addReadableMetric(const string& parent_id,
    const BuildInfo& element_info,
    DataType data_type,
    const ReadCallback& read_cb) {
  auto subgroup = getParentGroup(parent_id);
  auto id = subgroup->generateID();
  auto readable = make_shared<NiceMock<ReadableMock>>(data_type, read_cb);
  addElementMock(readable, id, element_info);
  return id;
}

string MockBuilder::addWritable(const BuildInfo& element_info,
    DataType data_type,
    const WriteCallback& write_cb,
    const ReadCallback& read_cb) {
  auto id = result_->generateID();
  auto writable =
      make_shared<NiceMock<WritableMock>>(data_type, read_cb, write_cb);
  addElementMock(writable, id, element_info);
  return id;
}

string MockBuilder::addWritableMetric(const string& parent_id,
    const BuildInfo& element_info,
    DataType data_type,
    const WriteCallback& write_cb,
    const ReadCallback& read_cb) {
  auto subgroup = getParentGroup(parent_id);
  auto id = subgroup->generateID();
  auto writable =
      make_shared<NiceMock<WritableMock>>(data_type, read_cb, write_cb);
  addElementMock(writable, id, element_info);
  return id;
}

pair<string, MockBuilder::NotifyCallback> MockBuilder::addObservable(
    const BuildInfo& element_info,
    DataType data_type,
    const ReadCallback& read_cb,
    const IsObservingCallback& observe_cb) {
  auto id = result_->generateID();
  auto observable = make_shared<NiceMock<ObservableMock>>(data_type, read_cb);
  observable->enableSubscribeFaking(observe_cb);
  addElementMock(observable, id, element_info);
  return make_pair(id, bind(&Observable::notify, observable, placeholders::_1));
}

pair<string, MockBuilder::NotifyCallback> MockBuilder::addObservable(
    const string& parent_id,
    const BuildInfo& element_info,
    DataType data_type,
    const ReadCallback& read_cb,
    const IsObservingCallback& observe_cb) {
  auto subgroup = getParentGroup(parent_id);
  auto id = subgroup->generateID();
  auto observable = make_shared<NiceMock<ObservableMock>>(data_type, read_cb);
  observable->enableSubscribeFaking(observe_cb);
  addElementMock(observable, id, element_info);
  return make_pair(id, bind(&Observable::notify, observable, placeholders::_1));
}

string MockBuilder::addCallable(const BuildInfo& element_info,
    DataType result_type,
    const AsyncExecuteCallback& async_execute_cb,
    const CancelCallback& cancel_cb,
    const ParameterTypes& parameter_types) {
  auto id = result_->generateID();
  auto callable = make_shared<NiceMock<CallableMock>>(
      result_type, async_execute_cb, cancel_cb, parameter_types);
  addElementMock(callable, id, element_info);
  return id;
}

string MockBuilder::addCallable(const BuildInfo& element_info,
    const ExecuteCallback& execute_cb,
    const ParameterTypes& parameter_types) {
  auto id = result_->generateID();
  auto callable =
      make_shared<NiceMock<CallableMock>>(execute_cb, parameter_types);
  addElementMock(callable, id, element_info);
  return id;
}

string MockBuilder::addCallable(const string& parent_id,
    const BuildInfo& element_info,
    const ExecuteCallback& execute_cb,
    const ParameterTypes& parameter_types) {
  auto subgroup = getParentGroup(parent_id);
  auto id = subgroup->generateID();
  auto callable =
      make_shared<NiceMock<CallableMock>>(execute_cb, parameter_types);
  addElementMock(callable, id, element_info);
  return id;
}

string MockBuilder::addCallable(const string& parent_id,
    const BuildInfo& element_info,
    DataType result_type,
    const ExecuteCallback& execute_cb,
    const AsyncExecuteCallback& async_execute_cb,
    const CancelCallback& cancel_cb,
    const ParameterTypes& parameter_types) {
  auto subgroup = getParentGroup(parent_id);
  auto id = subgroup->generateID();
  auto callable = make_shared<NiceMock<CallableMock>>(
      result_type, execute_cb, async_execute_cb, cancel_cb, parameter_types);
  addElementMock(callable, id, element_info);
  return id;
}

void MockBuilder::addElementMock(const ElementFunction& function,
    const std::string& id,
    const BuildInfo& element_info) {
  auto element = make_shared<NiceMock<ElementMock>>(
      function, id, FullMetaInfo{element_info.name, element_info.description});
  result_->addElement(element);
}

GroupMockPtr MockBuilder::getParentGroup(const string& parent_id) {
  if (auto it = subgroups_.find(parent_id); it != subgroups_.end()) {
    return it->second;
  } else {
    throw invalid_argument("No parent group with ID " + parent_id +
        " exists. Build parent group first");
  }
}

void MockBuilder::checkGroups() {
  if (result_->group()->size() == 0) {
    throw logic_error("Device root group is empty. Add elements to it");
  }
  for (const auto& [id, group] : subgroups_) {
    if (group->size() == 0) {
      throw logic_error("Device group " + id + " is empty. Add elements to it");
    }
  }
}

unique_ptr<Device> MockBuilder::result() {
  checkGroups();
  return move(result_);
}

} // namespace Information_Model::testing
