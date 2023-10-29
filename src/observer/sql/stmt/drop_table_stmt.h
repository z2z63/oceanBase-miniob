#pragma once

#include "sql/stmt/stmt.h"
#include "common/log/log.h"

class DropTableStmt : public Stmt
{
public:
  explicit DropTableStmt(const std::string &table_name) : table_name_(table_name) {}
  ~DropTableStmt() override = default;

  StmtType type() const override { return StmtType::DROP_TABLE; }

  static RC create(Db *db, const DropTableSqlNode &drop_table, Stmt *&stmt);

  const std::string &table_name() const { return table_name_; }

private:
  std::string table_name_;
};
