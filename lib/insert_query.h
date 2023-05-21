#pragma once

#include "query.h"
#include <vector>

class InsertQuery : public Query {
public:
    void AddColumn(std::string& column);

    void AddValue(const T& value);

    bool Execute(std::unordered_map<std::string, Table>&) override;

private:
    std::vector<std::string> columns_;
    std::vector<T> values_;
};
