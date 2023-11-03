#pragma once

#include "physical_operator.h"

#include <utility>

class UpdatePhysicalOperator : public PhysicalOperator
{
public:
  UpdatePhysicalOperator(Table *table, std::string column_name, const Value &value)
      : table_(table), value_(value), column_name_(std::move(column_name))
  {}

  virtual ~UpdatePhysicalOperator() = default;

  PhysicalOperatorType type() const override { return PhysicalOperatorType::UPDATE; }

  RC open(Trx *trx) override;
  RC next() override;
  RC close() override;

  Tuple *current_tuple() override { return nullptr; }

private:
  Table      *table_ = nullptr;
  Value       value_;
  std::string column_name_;
  Trx        *trx_ = nullptr;
};