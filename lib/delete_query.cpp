#include "delete_query.h"

void DeleteQuery::setRpnExp(const std::vector<rpn::lex>& rpn_exp) {
    rpn_exp_ = rpn_exp;
}

bool DeleteQuery::Execute(std::unordered_map<std::string, Table>& tables) {
    if (tables.count(table_name_) == 0) {
        return false;
    }
    Table& t = tables[table_name_];
    auto rows = t.getTable();
    auto columns = t.getColumns();
    for (size_t i = 0; i < rows.size(); ++i) {
        const auto& row = rows[i];
        auto res = rpn::Calculate(rpn_exp_, row, columns);
        if (res.type != rpn::Group::BOOL) {
            return false;
        }
        if (res.value == "false") {
            continue;
        }
        t.RemoveRow(i);
    }
    return true;
}
