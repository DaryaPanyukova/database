#include "sqlparser.h"

/*
const std::vector<Query>& SqlParser::getQueries() const {
    return queries_;
}*/



SqlParser::SqlParser(const std::string& input) {
    index_ = 0;
    input_ = input;
    correct_ = true;
}

bool SqlParser::isCorrect() const {
    return correct_;
}


inline bool isPunctuation(char c) {
    return c == ';' || c == ',' || c == '\'' || c == '(' || c == ')';
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

void SqlParser::GetChar() {
    for (; index_ < input_.size(); ++index_) {
        if (!std::isspace(input_[index_])) {
            break;
        }
    }
    if (index_ < input_.size()) {
        char_ = input_[index_];
    } else {
        char_ = '\0';
    }
}

void SqlParser::PutChar() {
    index_--;
}



// --------------------------------------------

void SqlParser::Query() {
    for (;;) {
        GetWord();
        if (word_ == "CREATE") {
            Create();
        } else if (word_ == "SELECT") {
            Select();
        } else if (word_ == "UPDATE") {
            Update();
        } else if (word_ == "INSERT") {
            Insert();
        } else if (word_ == "Delete") {
            Delete();
        } else if (word_.empty()) {
            return;
        } else {
            correct_ = false;
            return;
        }
    }
}

void SqlParser::Create() {
    GetWord();
    if (word_ != "TABLE") {
        correct_ = false;
        return;
    }
    GetChar();
    if (char_ != '(') {
        correct_ = false;
        return;
    }
    GetChar();
    if (char_ == ')') {
        return;
    }
    for (; char_ != '\0';) {
        GetChar();
        Name();

        GetWord();
        DataType();

        Constraint();
        GetChar();
        if (char_ == ')') {
            break;
        }
    }

    while (char_ == 'F') {
        PutChar();
        ForeignKey();
        GetChar();
    }

    if (char_ != ')') {
        correct_ = false;
    }
}

void SqlParser::DataType() {
    if (!(word_ == "FLOAT" || word_ == "INT" || word_ == "DOUBLE" ||
          word_ == "BOOL" || word_ == "VARCHAR")) {
        correct_ = false;
        return;
    }
    if (word_ == "VARCHAR") {
        VarcharType();
    }
}

void SqlParser::VarcharType() {

}

void SqlParser::Constraint() {
    GetWord();
    if (word_ == "PRIMARY") {
        GetWord();
        if (word_ != "KEY") {
            correct_ = false;
            return;
        }
    } else if (word_ == "NOT") {
        GetWord();
        if (word_ != "NULL") {
            correct_ = false;
            return;
        }
    } else if (word_ == "UNIQUE") {
        return;
    } else if (word_ == "DEFAULT") {
        Default();
    }
}

void SqlParser::Default() {

}

void SqlParser::ForeignKey() {

}

void SqlParser::Select() {

}

void SqlParser::Where() {

}

void SqlParser::From() {

}

void SqlParser::Join() {

}

void SqlParser::Update() {

}

void SqlParser::Insert() {

}

void SqlParser::Delete() {

}

void SqlParser::Drop() {

}

void SqlParser::KeyValue() {

}

void SqlParser::Name() {

}

void SqlParser::Value() {

}

void SqlParser::Bool() {

}

void SqlParser::Int() {

}

void SqlParser::Float() {

}

void SqlParser::Varchar() {

}

void SqlParser::Const() {

}

void SqlParser::Prior1() {

}

void SqlParser::Prior2() {

}

void SqlParser::Prior3() {

}

void SqlParser::Prior4() {

}

void SqlParser::Prior5() {

}

void SqlParser::Prior6() {

}

