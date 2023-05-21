#include "sqlparser.h"


SqlParser::SqlParser(const std::string& input) {
    index_ = 0;
    input_ = input;
    correct_ = true;
    Request();
}

bool SqlParser::isCorrect() {
    return correct_;
}


bool SqlParser::isPunctuation(char c) {
    return std::find(punctuation_.begin(), punctuation_.end(), c) !=
           punctuation_.end();
}

void SqlParser::GetWord() {
    word_ = "";
    for (; index_ < input_.size(); ++index_) {
        if (!std::isspace(input_[index_])) {
            break;
        }
    }
    for (; index_ < input_.size(); ++index_) {
        if (std::isspace(input_[index_]) || isPunctuation(input_[index_])) {
            break;
        }
        word_ += input_[index_];
    }
}

void SqlParser::GetChar(bool ignore = true) {
    if (ignore) {
        for (; index_ < input_.size(); ++index_) {
            if (!std::isspace(input_[index_])) {
                break;
            }
        }
    }
    if (index_ < input_.size()) {
        char_ = input_[index_];
    } else {
        char_ = '\0';
    }
    ++index_;
}

void SqlParser::PutChar() {
    index_--;
}


std::vector<Query*> SqlParser::getQueries() {
    return queries_;
}


// --------------------------------------------

void SqlParser::Request() {
    for (;;) {
        GetWord();
        if (word_ == "CREATE") {
            auto q = new СreateQuery;
            Create(q);
            queries_.push_back(q);
        } else if (word_ == "SELECT") {
            auto q = new SelectQuery;
            Select(q);
            queries_.push_back(q);
        } else if (word_ == "UPDATE") {
            auto q = new UpdateQuery;
            Update(q);
            queries_.push_back(q);
        } else if (word_ == "INSERT") {
            auto q = new InsertQuery;
            Insert(q);
            queries_.push_back(q);
        } else if (word_ == "DELETE") {
            auto q = new DeleteQuery;
            Delete(q);
            queries_.push_back(q);
        } else if (word_ == "DROP") {
            auto q = new DropQuery;
            Drop(q);
            queries_.push_back(q);
        } else if (word_.empty()) {
            return;
        } else {
            correct_ = false;
            return;
        }
    }
}

// ------------------ CREATE ------------------

void SqlParser::Create(СreateQuery* q) {
    GetWord();
    if (word_ != "TABLE") {
        correct_ = false;
        return;
    }
    Name();
    q->setTableName(word_);
    GetChar();
    if (char_ != '(') {
        correct_ = false;
        return;
    }

    size_t count = 0;
    do {
        ColumnInfo cur_column;
        Name();
        std::string col_name = word_;
        if (word_ == "FOREIGN") {
            GetWord();
            if (word_ == "KEY") {
                ForeignKey();
                GetChar();
                continue;
            }
        } else {
            GetWord();
        }
        cur_column.type_ = DataType();
        cur_column.number_ = count;
        Constraint(cur_column);
        GetChar();
        q->AddColumn(col_name, cur_column);
        count++;
    } while (char_ == ',');

    if (char_ != ')') {
        correct_ = false;
    }
}

Type SqlParser::DataType() {
    Type p;
    if (word_ == "INT") {
        p = INT;
    } else if (word_ == "DOUBLE") {
        p = DOUBLE;
    } else if (word_ == "BOOL") {
        p = BOOL;
    } else if (word_ == "VARCHAR") {
        p = VARCHAR;
    } else {
        correct_ = false;
    }
    return p;
}


void SqlParser::Constraint(ColumnInfo& info) {
    for (;;) {
        GetWord();
        if (word_ == "PRIMARY") {
            GetWord();
            if (word_ != "KEY") {
                correct_ = false;
                return;
            }
            info.primary_key_ = true;
        } else if (word_ == "NOT") {
            GetWord();
            if (word_ != "NULL") {
                correct_ = false;
                return;
            }
            info.not_null_ = true;
        } else if (word_ == "UNIQUE") {
            info.unique_ = true;
            continue;
        } else if (word_ == "DEFAULT") {
            info.default_ = true;
            info.default_value_ = Value();
        } else {
            return;
        }
    }
}

void SqlParser::ForeignKey() {
    GetChar();
    if (char_ != '(') {
        correct_ = false;
        return;
    }
    Name();
    GetChar();
    if (char_ != ')') {
        correct_ = false;
        return;
    }

    GetWord();
    if (word_ != "REFERENCES") {
        correct_ = false;
        return;
    }
    Name();
    GetChar();
    if (char_ != '(') {
        correct_ = false;
        return;
    }
    Name();
    GetChar();
    if (char_ != ')') {
        correct_ = false;
        return;
    }
}


// ------------------ SELECT ------------------
void SqlParser::Select(SelectQuery* q) {
    GetChar();
    if (char_ != '*') {
        PutChar();
        Name();
        q->AddColumn(word_);
        GetChar();
        while (char_ == ',') {
            Name();
            q->AddColumn(word_);
            GetChar();
        }
        PutChar();
    } else {
        q->setAll();
    }

    From(q);
    GetChar();
    if (char_ == 'W') {
        PutChar();
        q->setRpnWhere(Where());
    } else if (char_ != ';') {
        correct_ = false;
        return;
    }
}

std::vector<rpn::lex> SqlParser::Where() {
    GetWord();
    if (word_ != "WHERE") {
        correct_ = false;
        return {};
    }
    return Expression();
}

void SqlParser::From(SelectQuery* q) {
    GetWord();
    if (word_ != "FROM") {
        correct_ = false;
        return;
    }
    Name();
    q->setTableName(word_);
    GetChar();
    if (char_ == 'I' || char_ == 'L' || char_ == 'R') {
        PutChar();
        Join(q);
    } else {
        PutChar();
    }
}

void SqlParser::Join(SelectQuery* q) {
    GetWord();
    if (!(word_ == "INNER" || word_ == "LEFT" || word_ == "RIGHT")) {
        correct_ = false;
        return;
    }
    q->SetJoin(word_);
    GetWord();
    if (word_ != "JOIN") {
        correct_ = false;
        return;
    }
    Name();
    q->setOtherName(word_);
    GetWord();
    if (word_ != "ON") {
        correct_ = false;
        return;
    }

    Name();
    q->setKeyLeft(word_);
    GetChar();
    if (char_ != '=') {
        correct_ = false;
        return;
    }
    Name();
    q->setKeyRight(word_);
}



// ------------------ UPDATE ------------------
void SqlParser::Update(UpdateQuery* info) {
    Name();
    info->setTableName(word_);
    GetWord();
    if (word_ != "SET") {
        correct_ = false;
        return;
    }

    Name();
    KeyValue(info);
    GetChar();
    while (char_ == ',') {
        Name();
        if (word_ == "WHERE") {
            break;
        }
        KeyValue(info);
        GetChar();
    }

    if (word_ == "WHERE") {
        info->setRpnExp(Expression());
        GetChar();
    }
    if (char_ != ';') {
        correct_ = false;
    }
}

// ------------------ INSERT ------------------
void SqlParser::Insert(InsertQuery* info) {
    GetWord();
    if (word_ != "INTO") {
        correct_ = false;
        return;
    }
    Name();
    info->setTableName(word_);
    GetChar();
    if (char_ != '(') {
        correct_ = false;
        return;
    }

    do {
        GetWord();
        info->AddColumn(word_);
        GetChar();
    } while (char_ == ',');

    if (char_ != ')') {
        correct_ = false;
        return;
    }

    GetWord();
    if (word_ != "VALUES") {
        correct_ = false;
        return;
    }

    GetChar();
    if (char_ != '(') {
        correct_ = false;
        return;
    }
    do {
        auto v = Value();
        info->AddValue(v);
        GetChar();
    } while (char_ == ',');

    if (char_ != ')') {
        correct_ = false;
        return;
    }
    GetChar();
    if (char_ != ';') {
        correct_ = false;
    }
}

// ------------------ DELETE ------------------
void SqlParser::Delete(DeleteQuery* info) {
    GetWord();
    if (word_ != "FROM") {
        correct_ = false;
        return;
    }
    Name();
    info->setTableName(word_);
    GetChar();
    if (char_ == ';') {
        return;
    } else if (char_ == 'W') {
        PutChar();
        info->setRpnExp(Where());
    } else {
        correct_ = false;
    }
}

// ------------------ DROP ------------------
void SqlParser::Drop(DropQuery* info) {
    GetWord();
    if (word_ != "TABLE") {
        correct_ = false;
        return;
    }
    Name();
    info->setTableName(word_);
    GetChar();
    if (char_ != ';') {
        correct_ = false;
    }
}

// --------------- Expressions --------------
void SqlParser::KeyValue(UpdateQuery* info) {
    info->AddColumn(word_);
    GetChar();
    if (char_ != '=') {
        correct_ = false;
        return;
    }
    info->AddValueExp(Expression());
}

void SqlParser::Name() {
    GetWord();
    if (word_.empty()) {
        correct_ = false;
    }
}

T SqlParser::Value() {
    GetChar();
    if (char_ == '\'') {
        return Varchar();
    } else if (char_ == 't' || char_ == 'f') {
        PutChar();
        return Bool();
    } else {
        PutChar();
        return Number();
    }
}

bool SqlParser::Bool() {
    GetWord();
    if (word_ != "true" && word_ != "false") {
        correct_ = false;
    }
    return (word_ == "true");
}

T SqlParser::Number() {
    size_t beg = index_;
    GetChar();
    if (char_ == '+' || char_ == '-') { // SIGN
        GetChar();
    }
    if (!std::isdigit(char_)) {
        correct_ = false;
        return 0;
    }
    while (std::isdigit(char_)) {
        GetChar();
    }

    bool is_double  = false;
    if (char_ == '.') { // double
        is_double = true;
        GetChar();
        if (!std::isdigit(char_)) {
            correct_ = false;
        }
        while (std::isdigit(char_)) {
            GetChar();
        }
    }
    PutChar();
    size_t end = index_;
    std::string tmp = input_.substr(beg, end - beg);
    if (is_double) {
        return std::stod(tmp);
    } else {
        return std::stoi(tmp);
    }
}

std::string SqlParser::Varchar() {
    std::string result;
    GetChar(false);
    while (char_ != '\'' && char_ != '\0') {
        result += char_;
        GetChar(false);
    }
    if (char_ != '\'') {
        correct_ = false;
    }
    return result;
}

std::vector<rpn::lex> SqlParser::Expression() {
    size_t index = input_.size();
    for (size_t i = index_; i < input_.size(); ++i) {
        if (input_[i] == ';' || input_[i] == ',') {
            index = i;
            break;
        }
    }
    std::string expression = input_.substr(index_, index - index_);
    index_ = index;
    auto rpn_exp = rpn::GetRpn(expression);
    return rpn_exp;
}
