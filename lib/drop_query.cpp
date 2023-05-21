#include "drop_query.h"

bool DropQuery::Execute(std::unordered_map<std::string, Table>& tables) {
    if (tables.count(table_name_) == 0) {
        return false;
    }
    tables.erase(table_name_);
    return true;
}
