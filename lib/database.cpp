#include "database.h"

void Database::ImportTable(const std::string& table_name, const std::string& filename) {
    // создать базу и добавить в мапу
}

Table Database::Request(const std::string& input) {
    SqlParser p(input);
    if (!p.isCorrect()) {
        // хз что делать, throw??
    }
    /*
     std::vector <Query> queries = p.GetQueries();
     for (size_t i = 0; i < queries.size(); ++i) {
        if (!queries[i].DO()) // исполнение запроса {
            // если запрос не возможно выполнить,
            // например несуществующее название столбца
            // хз что делать, throw?
        }
     }
     */

    // return table, которую мы меняли в последнем запросе
}

void Database::SaveToFile(const std::string& table_name, const std::string& filename) {}