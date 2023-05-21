#pragma once
#include <string>
#include "rpn.h"

class Query {
public:
    Query() = default;

    virtual ~Query() = default;

    void setTableName(const std::string& tableName) {
        table_name_ = tableName;
    }

    const std::string& getTableName() const {
        return table_name_;
    }

    virtual bool Execute(std::unordered_map <std::string, Table>&) = 0;

protected:
    std::string table_name_;
};