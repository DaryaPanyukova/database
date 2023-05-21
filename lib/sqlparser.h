#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include "create_query.h"
#include "drop_query.h"
#include "delete_query.h"
#include "insert_query.h"
#include "select_query.h"
#include "update_query.h"

class SqlParser {
public:
    SqlParser(const std::string& input);

    bool isCorrect();

    std::vector<Query*> getQueries();

private:
    const std::vector<char> punctuation_{';', ',', '\'', '(', ')', '+', '-',
                                         '/', '*', '%', '='};

    std::string input_;
    bool correct_;
    std::string word_;
    char char_;
    size_t index_;
    std::vector<Query*> queries_; // запросы во внутреннем представлении

    bool isPunctuation(char);

    void GetWord();
    void GetChar(bool);
    void PutChar();

    void Request();

    void Create(СreateQuery*);
    Type DataType();
    void Constraint(ColumnInfo&);
    void ForeignKey();

    void Select(SelectQuery*);
    std::vector<rpn::lex> Where();
    void From(SelectQuery*);
    void Join(SelectQuery*);

    void Update(UpdateQuery*);
    void Insert(InsertQuery*);
    void Delete(DeleteQuery*);
    void Drop(DropQuery*);

    void KeyValue(UpdateQuery*);
    void Name();
    T Value();

    bool Bool();
    T Number();
    std::string Varchar();

    std::vector<rpn::lex> Expression();
};
