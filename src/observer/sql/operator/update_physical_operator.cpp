#include "update_physical_operator.h"
#include "storage/trx/trx.h"

RC UpdatePhysicalOperator::open(Trx *trx)
{
  if (children_.empty()) {
    return RC::SUCCESS;
  }

  auto &child = children_[0];
  RC    rc    = child->open(trx);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to open child operator: %s", strrc(rc));
    return rc;
  }

  trx_ = trx;

  return RC::SUCCESS;
}
RC UpdatePhysicalOperator::next()
{
  RC rc = RC::SUCCESS;
  if (children_.empty()) {
    return RC::RECORD_EOF;
  }

  PhysicalOperator *child = children_[0].get();
  while (RC::SUCCESS == (rc = child->next())) {
    Tuple *tuple = child->current_tuple();
    if (nullptr == tuple) {
      LOG_WARN("failed to get current oldRecord: %s", strrc(rc));
      return rc;
    }

    RowTuple *row_tuple      = static_cast<RowTuple *>(tuple);
    Record   &oldRecord      = row_tuple->record();
    int       updatePosition = 0;
    auto     field_metas    = *table_->table_meta().field_metas();
    for (auto field : field_metas) {
      if (field.name() == column_name_) {   // resolve阶段做过检查，所以字段名一定能找到
        break;
      }
      updatePosition++;
    }

    // 构造新记录
    std::vector<Value> values;
    int                cell_num = row_tuple->cell_num();
    for (int i = 0; i < cell_num; i++) {
      Value value;
      if (i == updatePosition) {  // 只要求更新一个字段，其他字段都不变
        value = value_;
      } else {
        row_tuple->cell_at(i, value);
      }

      values.push_back(value);
    }
    Record newRecord;
    table_->make_record(cell_num, values.data(), newRecord);

    // 删除旧记录
    rc = trx_->delete_record(table_, oldRecord);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to delete  old oldRecord(update): %s", strrc(rc));
      return rc;
    }

    // 插入新记录
    rc = trx_->insert_record(table_, newRecord);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to insert new oldRecord(update): %s", strrc(rc));
      return rc;
    }
  }

  return RC::RECORD_EOF;
}
RC UpdatePhysicalOperator::close() { return RC::SUCCESS; }
