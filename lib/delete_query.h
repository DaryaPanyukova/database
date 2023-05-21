#pragma once

#include <stack>
#include "query.h"

class DeleteQuery : public Query {
public:
    void setRpnExp(const std::vector<rpn::lex>&);

    bool Execute(std::unordered_map<std::string, Table>&) override;

private:
    std::vector<rpn::lex> rpn_exp_;
};
