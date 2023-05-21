#pragma once

#include <filesystem>
#include <fstream>
#include <string>
#include "sqlparser.h"


class Database {
public:
    Database();

    void ImportTable
            (const std::string& table_name, const std::string& filename);

    Table Request(const std::string& input);

    void SaveToFile(const std::string& table_name, const std::string& filename);

    void SaveToFile(const Table& t, const std::string& filename);

    int isCorrect();

private:
    bool correct_;

    std::unordered_map<std::string, Table> tables_;
};
