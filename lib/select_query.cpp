#include "select_query.h"


SelectQuery::SelectQuery() {
    rpn_where_.resize(1, {rpn::Group::BOOL, "true"});
}


void SelectQuery::setRpnWhere(const std::vector<rpn::lex>& rpnWhere) {
    rpn_where_ = rpnWhere;
}

void SelectQuery::AddColumn(const std::string& column) {
    columns_.insert(column);
}

void SelectQuery::setAll() {
    all_ = true;
}

void SelectQuery::SetJoin(const std::string& type) {
    if (type == "LEFT") {
        join = Join::left;
    } else if (type == "RIGHT") {
        join = Join::right;
    } else if (type == "INNER") {
        join = Join::inner;
    } else {
        join = Join::null;
    }
}

void SelectQuery::setOtherName(const std::string& otherTable) {
    other_name_ = otherTable;
}

void SelectQuery::setKeyLeft(const std::string& key) {
    key_left_ = key;
}

void SelectQuery::setKeyRight(const std::string& key) {
    key_right_ = key;
}


bool SelectQuery::Execute(std::unordered_map<std::string, Table>& tables) {
    if (tables.count(table_name_) == 0) {
        return false;
    }

    if (join == Join::null) {
        return ExecuteSimple(tables);
    } else {
        return ExecuteJoin(tables);
    }
}

bool SelectQuery::ExecuteSimple(std::unordered_map<std::string, Table>& tables) {
    Table& t = tables[table_name_];
    auto rows = t.getTable();
    auto columns = t.getColumns();

    if (!all_) {
        for (const auto& elem: columns) {
            if (columns_.count(elem.first) == 0) {
                t.RemoveColumn(elem.first);
            }
        }
    }

    for (size_t i = 0; i < rows.size(); ++i) {
        const auto& row = rows[i];
        auto res = rpn::Calculate(rpn_where_, row, columns);
        if (res.type != rpn::Group::BOOL) {
            return false;
        }
        if (res.value == "true") {
            continue;
        }
        t.RemoveRow(i);
    }
    return true;
}

bool SelectQuery::ExecuteJoin(std::unordered_map<std::string, Table>& tables) {
    if (tables.count(table_name_) == 0 || tables.count(other_name_) == 0) {
        return false;
    }
    std::set<std::string> columns_left;
    std::set<std::string> columns_right;

    for (const auto& col: columns_) {
        std::string table = col.substr(0, col.find('.'));
        std::string col_name = col.substr(col.find('.') + 1);
        if (table == table_name_) {
            columns_left.insert(col_name);
        } else {
            columns_right.insert(col_name);
        }
    }


    if (key_left_.substr(0, key_left_.find('.')) != table_name_) {
        std::swap(key_left_, key_right_);
    }

    Table& left = tables[table_name_];
    Table& right = tables[other_name_];

    key_left_ = key_left_.substr(key_left_.find('.') + 1);
    key_right_ = key_right_.substr(key_right_.find('.') + 1);
    size_t numk_left = left.GetColumnInfo(key_left_).number_;
    size_t numk_right = right.GetColumnInfo(key_right_).number_;

    Table new_table;
    size_t counter = 0;
    for (const auto& name: columns_left) {
        auto info = left.GetColumnInfo(name);
        info.number_ = counter;
        new_table.AddColumn(name, left.GetColumnInfo(name));
        counter++;
    }
    for (const auto& name: columns_right) {
        auto info = right.GetColumnInfo(name);
        info.number_ = counter;
        new_table.AddColumn(name, right.GetColumnInfo(name));
        counter++;
    }

    auto rows_left = left.getTable();
    auto rows_right = right.getTable();
    std::vector<bool> used(rows_right.size(), false);
    for (size_t i = 0; i < rows_left.size(); ++i) {
        bool match = false;
        for (size_t j = 0; j < rows_right.size(); ++j) {
            if (rows_left[i].GetCell(numk_left) ==
                rows_right[j].GetCell(numk_right)) {  // join
                match = true;
                used[j] = true;
                Row new_row(columns_left.size() + columns_right.size());

                // add left columns
                for (const auto& name: columns_left) {
                    size_t ind = left.GetColumnInfo(name).number_;
                    size_t new_ind = new_table.GetColumnInfo(name).number_;
                    T val = rows_left[i].GetCell(ind);
                    new_row.SetCell(val, new_ind);
                }

                // add right columns
                for (const auto& name: columns_right) {
                    size_t ind = right.GetColumnInfo(name).number_;
                    size_t new_ind = new_table.GetColumnInfo(name).number_;
                    T val = rows_right[j].GetCell(ind);
                    new_row.SetCell(val, new_ind);
                }

                new_table.AddRow(new_row);
            }
        }

        if (join == Join::left && !match) {
            Row new_row(columns_left.size() + columns_right.size());

            // add left columns
            for (const auto& name: columns_left) {
                size_t ind = left.GetColumnInfo(name).number_;
                size_t new_ind = new_table.GetColumnInfo(name).number_;
                T val = rows_left[i].GetCell(ind);
                new_row.SetCell(val, new_ind);
            }
            new_table.AddRow(new_row);
        }
    }

    if (join != Join::right) {
        left = new_table;
        return true;
    }

    for (size_t i = 0; i < used.size(); ++i) {
        if (!used[i]) {
            Row new_row(columns_left.size() + columns_right.size());
            for (const auto& name: columns_right) {
                size_t ind = right.GetColumnInfo(name).number_;
                size_t new_ind = new_table.GetColumnInfo(name).number_;
                T val = rows_right[i].GetCell(ind);
                new_row.SetCell(val, new_ind);
            }
            new_table.AddRow(new_row);
        }
    }

    left = new_table;
    return true;
}
