#include "table.h"

std::ostream& operator<<(std::ostream& out, Table t) {
    for (const auto& col: t.columns_) {
        out << col.first << ' ';
    }
    out << '\n';

    for (const auto& row: t.table_) {
        for (const auto& col: t.columns_) {
            std::string tmp = row.GetCellAsString(col.first, t.columns_);
            if (tmp.empty()) {
                std::cout << "NULL";
            } else {
                std::cout << tmp;
            }
            out << ' ';
        }
        out << '\n';
    }
    return out;
}

bool Table::operator==(const Table& rhs) {
    const auto& rows_right = rhs.getTable();
    const auto& columns_right = rhs.getColumns();

    if (table_.size() != rows_right.size() ||
        columns_.size() != columns_right.size()) {
        return false;
    }

    for (const auto& col: columns_) {
        if (columns_right.count(col.first) == 0) {
            return false;
        }
    }

    for (size_t i = 0; i < table_.size(); ++i) {
        if (table_[i].getCells() != rows_right[i].getCells()) {
            return false;
        }
    }
    return true;
}

void Table::setColumns
        (const std::unordered_map<std::string, ColumnInfo>& columns) {
    columns_ = columns;
}

void Table::AddRow(Row& r) {
    table_.push_back(r);
}

ColumnInfo Table::GetColumnInfo(const std::string& name) {
    return columns_[name];
}

const std::unordered_map<std::string, ColumnInfo>& Table::getColumns() const {
    return columns_;
}

const std::vector<Row>& Table::getTable() const {
    return table_;
}

void Table::SetCell(size_t row_ind, size_t col_ind, std::string& value,
                    uint8_t type) {
    table_[row_ind].SetCellAsString(value, type, col_ind);
}

void Table::RemoveRow(size_t index) {
    table_.erase(table_.begin() + index);
}

void Table::RemoveColumn(const std::string& col) {
    columns_.erase(col);
}

void Table::AddColumn(const std::string& name, const ColumnInfo& info) {
    columns_.insert({name, info});
}
