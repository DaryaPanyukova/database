#pragma once

#include "query.h"

class SelectQuery : public Query {
public:
    SelectQuery();

    void setKeyRight(const std::string& key);

    void setKeyLeft(const std::string& key);

    void setOtherName(const std::string&);

    void setRpnWhere(const std::vector<rpn::lex>& rpnWhere);

    void AddColumn(const std::string&);

    void setAll();

    bool Execute(std::unordered_map<std::string, Table>&) override;

    void SetJoin(const std::string&);

private:
    enum class Join {
        left,
        right,
        inner,
        null
    };

    bool all_ = false;
    std::vector<rpn::lex> rpn_where_;
    Join join = Join::null;

    // left - table_name
    // right - other_table
    std::string other_name_;
    std::set <std::string> columns_;

    std::string key_left_, key_right_;

    bool ExecuteSimple(std::unordered_map<std::string, Table>&);

    bool ExecuteJoin(std::unordered_map<std::string, Table>&);
};
