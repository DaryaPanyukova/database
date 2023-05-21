#include "insert_query.h"

void InsertQuery::AddColumn(std::string& column) {
    columns_.emplace_back(column);
}

void InsertQuery::AddValue(const T& value) {
    values_.emplace_back(value);
}

bool InsertQuery::Execute(std::unordered_map<std::string, Table>& tables) {
    if (tables.count(table_name_) == 0) {
        return false;
    }

    if (columns_.size() != values_.size()) {
        return false;
    }

    Table& t = tables[table_name_];
    const auto& columns = t.getColumns();
    const auto& rows = t.getTable();
    Row r(columns.size());

    // creating row
    for (size_t i = 0; i < columns_.size(); ++i) {
        ColumnInfo info = t.GetColumnInfo(columns_[i]);
        if (values_[i].index() == info.type_ ||
            values_[i].index() == INT && info.type_ == Type::DOUBLE ||
                values_[i].index() == DOUBLE && info.type_ == Type::INT) {
            size_t index = info.number_;
            r.SetCell(values_[i], index);
        } else {
            return false;
        }
    }

    // check constraints
    for (const auto& elem: columns) {
        size_t index = elem.second.number_;
        if (elem.second.unique_ || elem.second.primary_key_) {
            for (const auto& row: rows) {
                if (row.GetCell(index) == r.GetCell(index)) {
                    return false;
                }
            }
        }
        if (elem.second.not_null_ || elem.second.primary_key_) {
            if (r.GetCell(index) != Row::null) {
                continue;
            }

            if (elem.second.default_) {
                r.SetCell(elem.second.default_value_, index);
            } else {
                return false;
            }
        }
    }

    t.AddRow(r);
    return true;
}
