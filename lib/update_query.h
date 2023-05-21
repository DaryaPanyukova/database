#pragma once
#include "query.h"

class UpdateQuery : public Query {
public:
    UpdateQuery();
    void setRpnExp(const std::vector<rpn::lex>&);

    void AddColumn(const std::string&);

    void AddValueExp(const std::vector<rpn::lex>&);

    bool Execute(std::unordered_map <std::string, Table>&) override;

private:
    std::vector<std::vector<rpn::lex>> values_exp_;
    std::vector<std::string> columns_;
    std::vector<rpn::lex> rpn_where_;
};