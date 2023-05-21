#pragma once
#include <filesystem>
#include "sqlparser.h"
#include "table.h"

class Database {
private:
    // мапа где ключ это название таблицы, а значение - сама база
    // map <std::string, Table>
public:
    void ImportTable(const std::string& table_name, const std::string& filename);

    Table Request(const std::string& input);

    void SaveToFile(const std::string& table_name, const std::string& filename);
};

int Sum(int a, int b){
    return a + b;
}