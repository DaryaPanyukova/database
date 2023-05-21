#pragma once
#include <vector>
#include "query.h"

class СreateQuery : public Query {
public:
    void AddColumn(std::string& name, const ColumnInfo& col);

    bool Execute(std::unordered_map <std::string, Table>&) override;

private:
    std::unordered_map<std::string, ColumnInfo> columns;

    std::string other_name_;
};

