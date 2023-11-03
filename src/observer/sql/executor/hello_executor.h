#pragma once

#include "common/rc.h"
#include "sql/operator/string_list_physical_operator.h"
#include "event/sql_event.h"
#include "event/session_event.h"
#include "sql/executor/sql_result.h"
#include "session/session.h"


class HelloExecutor
{
public:
  HelloExecutor() = default;
  virtual ~HelloExecutor() = default;

  RC execute(SQLStageEvent *sql_event)
  {
    const char *strings = "Hello, OceanBase Competition!";

    auto oper = new StringListPhysicalOperator();
    oper->append(strings);

    SqlResult *sql_result = sql_event->session_event()->sql_result();
    sql_result->set_operator(std::unique_ptr<PhysicalOperator>(oper));

    return RC::SUCCESS;
  }
};