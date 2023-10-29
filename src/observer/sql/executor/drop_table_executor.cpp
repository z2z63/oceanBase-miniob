#include "drop_table_executor.h"
#include "sql/stmt/drop_table_stmt.h"
#include "event/sql_event.h"
#include "session/session.h"
#include "event/session_event.h"
#include "storage/db/db.h"

RC DropTableExecutor::execute(SQLStageEvent *sql_event)
{
  Stmt    *stmt    = sql_event->stmt();
  Session *session = sql_event->session_event()->session();
  ASSERT(stmt->type() == StmtType::DROP_TABLE,
      "create table executor can not run this command: %d",
      static_cast<int>(stmt->type()));

  auto *drop_table_stmt = dynamic_cast<DropTableStmt *>(stmt);

  const char *table_name = drop_table_stmt->table_name().c_str();
  RC rc = session->get_current_db()->drop_table(table_name);    // 删除表交给db去做

  return rc;
}
