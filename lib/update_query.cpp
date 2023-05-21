#include "update_query.h"

UpdateQuery::UpdateQuery() {
    rpn_where_.resize(1, {rpn::Group::BOOL, "true"});
}


void UpdateQuery::setRpnExp(const std::vector<rpn::lex>& rpn_exp) {
    rpn_where_ = rpn_exp;
}

void UpdateQuery::AddColumn(const std::string& column) {
    columns_.emplace_back(column);
}

void UpdateQuery::AddValueExp(const std::vector<rpn::lex>& exp) {
    values_exp_.emplace_back(exp);
}

bool UpdateQuery::Execute(std::unordered_map<std::string, Table>& tables) {
    if (tables.count(table_name_) == 0) {
        return false;
    }
    Table& t = tables[table_name_];
    auto rows = t.getTable();
    auto columns = t.getColumns();
    for (size_t row_ind = 0; row_ind < rows.size(); ++row_ind) {
        auto row = rows[row_ind];
        auto res_where = rpn::Calculate(rpn_where_, row, columns);
        if (res_where.type != rpn::Group::BOOL) {
            return false;
        }
        if (res_where.value == "false") {
            continue;
        }

        for (size_t i = 0; i < values_exp_.size(); ++i) {
            auto exp = values_exp_[i];
            auto info = columns[columns_[i]];

            auto res_value = rpn::Calculate(exp, row, columns);
            if (res_value.value == "error") {
                return false;
            }

            if (res_value.type == info.type_ ||
                res_value.type == rpn::Group::INT &&
                info.type_ == Type::DOUBLE ||
                res_value.type == rpn::Group::DOUBLE &&
                info.type_ == Type::INT) {
                t.SetCell(row_ind, info.number_, res_value.value,
                          res_value.type);
            } else {
                return false;
            }
        }
    }


    for (const auto& elem: columns) {
        size_t index = elem.second.number_;
        if (!elem.second.unique_ && !elem.second.primary_key_) {
            continue;
        }
        for (size_t i = 0; i < rows.size(); ++i) {
            auto row1 = rows[i];
            for (size_t j = 0; j < rows.size(); ++j) {
                if (i == j) {
                    continue;
                }
                auto row2 = rows[j];
                if (row1.GetCell(index) == row2.GetCell(index)) {
                    return false;
                }
            }
        }
    }

    return true;
}
