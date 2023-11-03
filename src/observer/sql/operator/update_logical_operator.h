#pragma once

#include "sql/operator/logical_operator.h"

class UpdateLogicalOperator : public LogicalOperator
{
public:
  explicit UpdateLogicalOperator(Table *table, std::string attribute_name, const Value &value)
      : table_(table), value_(value), attribute_name_(std::move(attribute_name)){};
  virtual ~UpdateLogicalOperator() = default;

  LogicalOperatorType type() const override { return LogicalOperatorType::UPDATE; }
  Table              *table() const { return table_; }
  std::string         attribute_name() const { return attribute_name_; }
  Value               value() const { return value_; }

private:
  Table      *table_ = nullptr;
  std::string attribute_name_;
  Value       value_;
};
