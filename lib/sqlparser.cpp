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
    ++index_;
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
        } else if (word_ == "DELETE") {
            Delete();
        } else if (word_ == "DROP") {
            Drop();
        } else if (word_.empty()) {
            return;
        } else {
            correct_ = false;
            return;
        }
    }
}

// ------------------ CREATE ------------------

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
    GetChar();
    if (char_ != '(') {
        correct_ = false;
        return;
    }
    Int();
    if (char_ != ')') {
        correct_ = false;
        return;
    }
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
        Value();
    }
}

void SqlParser::ForeignKey() {
    GetWord();
    if (word_ != "FOREIGN") {
        correct_ = false;
        return;
    }

    GetWord();
    if (word_ != "KEY") {
        correct_ = false;
        return;
    }

    GetChar();
    if (char_ != '(') {
        correct_ = false;
        return;
    }
    Name();
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
    if (char_ != ')') {
        correct_ = false;
        return;
    }
}


// ------------------ SELECT ------------------
void SqlParser::Select() {
    GetChar();
    if (char_ != '*') {
        PutChar();
        Name();
        GetChar();
        while (char_ == ',') {
            Name();
            GetChar();
        }
        PutChar();
    }

    From();
    GetChar();
    if (char_ == 'W') {
        PutChar();
        Where();
    } else if (char_ != ';') {
        correct_ = false;
        return;
    }
}

void SqlParser::Where() {
    GetWord();
    if (word_ != "WHERE") {
        correct_ = false;
        return;
    }
    Prior6();
}

void SqlParser::From() {
    GetWord();
    if (word_ != "FROM") {
        correct_ = false;
        return;
    }
    Name();
    GetChar();
    if (char_ == 'I' || char_ == 'L' || char_ == 'R') {
        PutChar();
        Join();
    } else {
        PutChar();
    }
}

void SqlParser::Join() {
    GetWord();
    if (!(word_ == "INNER" || word_ == "LEFT" || word_ == "RIGHT")) {
        correct_ = false;
        return;
    }
    GetWord();
    if (word_ != "JOIN") {
        correct_ = false;
        return;
    }
    Name();
    GetWord();
    if (word_ != "ON") {
        correct_ = false;
        return;
    }

    Name();
    Compare();
    Name();
}

void SqlParser::Compare() {
    GetChar();
    if (char_ == '=') {
        return;
    } else if (char_ == '<') {
        GetChar();
        if (char_ != '>' || char_ != '=') {
            PutChar();
        }
    } else if (char_ == '>') {
        GetChar();
        if (char_ != '=') {
            PutChar();
        }
    } else {
        correct_ = false;
        return;
    }
}


// ------------------ UPDATE ------------------
void SqlParser::Update() {
    Name();
    GetWord();
    if (word_ != "SET") {
        correct_ = false;
        return;
    }
    KeyValue();
    GetChar();
    while (char_ == ',') {
        KeyValue();
        GetChar();
    }
    if (char_ == 'W') {
        PutChar();
        Where();
        GetChar();
    }
    if (char_ != ';') {
        correct_ = false;
    }
}

// ------------------ INSERT ------------------
void SqlParser::Insert() {
    GetWord();
    if (word_ != "INTO") {
        correct_ = false;
        return;
    }
    Name();
    GetChar();
    if (char_ != '(') {
        correct_ = false;
        return;
    }
    GetChar();
    while (char_ == ',') {
        GetWord();
        GetChar();
    }
    PutChar();
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
    Value();
    GetChar();
    while (char_ == ',') {
        Value();
        GetChar();
    }
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
void SqlParser::Delete() {
    GetWord();
    if (word_ != "FROM") {
        correct_ = false;
        return;
    }
    Name();
    GetChar();
    if (char_ == ';') {
        return;
    } else if (char_ == 'W') {
        PutChar();
        Where();
    } else {
        correct_ = false;
    }
}

// ------------------ DROP ------------------
void SqlParser::Drop() {
    GetWord();
    if (word_ != "TABLE") {
        correct_ = false;
        return;
    }
    Name();
    GetChar();
    if (char_ != ';') {
        correct_ = false;
    }
}

// --------------- Expressions --------------
void SqlParser::KeyValue() {
    Name();
    GetChar();
    if (char_ != '=') {
        correct_ = false;
        return;
    }
    Prior6();
}

void SqlParser::Name() {
    GetWord();
}

void SqlParser::Value() {
    GetChar();
    if (char_ == ',') {
        Varchar();
    } else if (char_ == 't' || char_ == 'f') {
        PutChar();
        Bool();
    } else {
        // todo int float
    }
}

void SqlParser::Bool() {
    GetWord();
    if (word_ != "true" && word_ != "false") {
        correct_ = false;
    }
}

void SqlParser::Int() {
    // todo
}

void SqlParser::Float() {
    // todo
}

void SqlParser::Varchar() {
    GetChar();
    while (char_ != '\'') {
        GetChar();
    }
    if (char_ != '\'') {
        correct_ = false;
    }
}

void SqlParser::Const() {
    GetChar();
    if (char_ == '-' || char_ == '+') {
        PutChar();
        Int(); // todo float?
    } else if (char_ == '\'') {
        Name();
    } else {
        PutChar();
        GetWord();
        if (word_ == "true" || word_ == "false") {
            // bool
        } else {
            correct_ = false;
        }
    }

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

