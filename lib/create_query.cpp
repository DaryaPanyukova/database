#include "create_query.h"

void СreateQuery::AddColumn(std::string& name, const ColumnInfo& col) {
    columns[name] = col;
}

bool СreateQuery::Execute(std::unordered_map <std::string, Table>& tables) {
    Table t;
    t.setColumns(columns);
    tables[table_name_] = t;
    return true;
}
