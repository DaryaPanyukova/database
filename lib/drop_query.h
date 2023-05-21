#pragma once
#include "query.h"

class DropQuery : public Query {
public:
    bool Execute(std::unordered_map <std::string, Table>&) override;
};

