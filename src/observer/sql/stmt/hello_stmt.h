#pragma once


#include <string>
#include <vector>

#include "sql/stmt/stmt.h"

class HelloStmt : public Stmt
{
public:
  HelloStmt()
  {}
  virtual ~HelloStmt() = default;

  StmtType type() const override { return StmtType::HELLO; }

  static RC create(Stmt *&stmt)
  {
    stmt = new HelloStmt();
    return RC::SUCCESS;
  }
};