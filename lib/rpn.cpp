#include "rpn.h"


std::vector<rpn::lex> rpn::GetRpn(std::string& input) {
    rpn::RpnParser parser(input);
    return parser.GetRpn();
}

void rpn::CalculateBinary(const Row& row, std::vector<lex>& res, lex& cur,
                          std::unordered_map<std::string, ColumnInfo>& columns) {
    if (res.empty()) {
        throw std::exception{};
    }
    auto first = res.back();
    res.pop_back();
    if (res.empty()) {
        throw std::exception{};
    }
    auto second = res.back();
    res.pop_back();

    if (first.type == Group::NAME) {
        switch (columns[first.value].type_) {
            case BOOL:
                first.type = Group::BOOL;
                break;
            case INT:
                first.type = Group::INT;
                break;
            case VARCHAR:
                first.type = Group::VARCHAR;
                break;
            case DOUBLE:
                first.type = Group::DOUBLE;
        }
        first.value = row.GetCellAsString(first.value, columns);
        if (first.type == Group::VARCHAR) {
            first.value = '\'' + first.value + '\'';
        }
    }

    if (second.type == Group::NAME) {
        switch (columns[second.value].type_) {
            case BOOL:
                second.type = Group::BOOL;
                break;
            case INT:
                second.type = Group::INT;
                break;
            case VARCHAR:
                second.type = Group::VARCHAR;
                break;
            case DOUBLE:
                second.type = Group::DOUBLE;
        }
        second.value = row.GetCellAsString(second.value, columns);
        if (second.type == Group::VARCHAR) {
            second.value = '\'' + second.value + '\'';
        }
    }

    if (cur.value == "+" || cur.value == "-" ||
        cur.value == "*" || cur.value == "/") {
        if ((first.type == Group::INT || first.type == Group::DOUBLE) &&
            (second.type == Group::INT || second.type == Group::DOUBLE)) {
            double r;
            if (cur.value == "+") {
                r = std::stod(first.value) + std::stod(second.value);
            } else if (cur.value == "-") {
                r = std::stod(second.value) - std::stod(first.value);
            } else if (cur.value == "*") {
                r = std::stod(first.value) * std::stod(second.value);
            } else if (cur.value == "/") {
                r = std::stod(first.value) / std::stod(second.value);
            }
            if (first.type == Group::INT &&
                second.type == Group::INT) {
                res.emplace_back(Group::INT,
                                 std::to_string(static_cast<int>(r)));
            } else {
                res.emplace_back(Group::DOUBLE, std::to_string(r));
            }
        } else {
            throw std::exception{};
        }
    } else if (cur.value == "%") {
        if (first.type == Group::INT && second.type == Group::INT) {
            int32_t r;
            r = std::stoi(first.value) % std::stoi(second.value);
            res.emplace_back(Group::INT, std::to_string(static_cast<int>(r)));
        } else {
            throw std::exception{};
        }
    } else if (cur.value == "<" || cur.value == ">" ||
               cur.value == "<=" || cur.value == ">=") {
        if ((first.type == Group::INT || first.type == Group::DOUBLE) &&
            (second.type == Group::INT || second.type == Group::DOUBLE)) {
            bool r;
            if (cur.value == "<") {
                r = std::stod(first.value) < std::stod(second.value);
            } else if (cur.value == ">") {
                r = std::stod(second.value) > std::stod(first.value);
            } else if (cur.value == "<=") {
                r = std::stod(first.value) <= std::stod(second.value);
            } else if (cur.value == ">=") {
                r = std::stod(first.value) >= std::stod(second.value);
            }
            res.emplace_back(Group::BOOL, r ? "true" : "false");
        } else if (first.type == Group::VARCHAR &&
                   second.type == Group::VARCHAR) {
            bool r;
            if (cur.value == "<") {
                r = first.value < second.value;
            } else if (cur.value == ">") {
                r = second.value > first.value;
            } else if (cur.value == "<=") {
                r = first.value <= second.value;
            } else if (cur.value == ">=") {
                r = first.value >= second.value;
            }
            res.emplace_back(Group::BOOL, r ? "true" : "false");
        } else {
            throw std::exception{};
        }
    } else if (cur.value == "=" || cur.value == "<>") {
        bool r;
        if ((first.type == Group::INT || first.type == Group::DOUBLE) &&
            (second.type == Group::INT || second.type == Group::DOUBLE)) {
            r = std::stod(first.value) == std::stod(second.value);
        } else if (first.type == second.type) {
            r = (first.value == second.value);
        } else {
            throw std::exception{};
        }
        if (cur.value == "=") {
            res.emplace_back(Group::BOOL, r ? "true" : "false");
        } else {
            res.emplace_back(Group::BOOL, !r ? "true" : "false");
        }
    } else if (cur.value == "AND" || cur.value == "OR") {
        if (first.type != Group::BOOL || second.type != Group::BOOL) {
            throw std::exception{};
        } else {
            bool a = (first.value == "true");
            bool b = (second.value == "true");
            if (cur.value == "AND") {
                res.emplace_back(Group::BOOL, a && b ? "true" : "false");
            } else {
                res.emplace_back(Group::BOOL, a || b ? "true" : "false");
            }
        }
    }
}

void rpn::CalculateUnary(const Row& row, std::vector<lex>& res, lex& cur,
                         std::unordered_map<std::string, ColumnInfo>& columns) {
    auto elem = res.back();
    res.pop_back();
    if (elem.type == Group::NAME) {
        switch (columns[elem.value].type_) {
            case BOOL:
                elem.type = Group::BOOL;
                break;
            case INT:
                elem.type = Group::INT;
                break;
            case VARCHAR:
                elem.type = Group::VARCHAR;
                break;
            case DOUBLE:
                elem.type = Group::DOUBLE;
        }
        elem.value = row.GetCellAsString(elem.value, columns);
        if (elem.type == Group::VARCHAR && !elem.value.empty()) {
            elem.value = '\'' + elem.value + '\'';
        }
    }

    if (cur.value == "_") {
        if (elem.type == Group::INT) {
            int32_t r = -std::stoi(elem.value);
            res.emplace_back(Group::INT, std::to_string(r));
        } else if (elem.type == Group::DOUBLE) {
            double r = -std::stod(elem.value);
            res.emplace_back(Group::DOUBLE, std::to_string(r));
        } else {
            throw std::exception{};
        }
    } else if (cur.value == "NOT") {
        if (elem.type == Group::BOOL) {
            res.emplace_back(Group::BOOL,
                             elem.value == "true" ? "false" : "true");
        } else {
            throw std::exception{};
        }
    } else if (cur.value == "IS NULL") {
        res.emplace_back(Group::BOOL, elem.value.empty() ? "true" : "false");
    } else if (cur.value == "IS NOT NULL") {
        res.emplace_back(Group::BOOL, elem.value.empty() ? "false" : "true");
    }
}


rpn::lex rpn::Calculate(std::vector<lex> expr, const Row& row,
                        std::unordered_map<std::string, ColumnInfo>& columns) {
    try {
        std::vector<lex> res;

        while (!expr.empty()) {
            auto tmp = expr.back();
            expr.pop_back();

            if (tmp.type == Group::LOGICAl_OP ||
                tmp.type == Group::ARITHMETIC_OP) { // operation
                if (rpn::isBinaryOperation(tmp.value)) { // binary
                    CalculateBinary(row, res, tmp, columns);
                } else { // unary
                    CalculateUnary(row, res, tmp, columns);
                }
            } else {
                res.emplace_back(tmp);
            }
        }
        if (res.size() != 1) {
            throw std::exception{};
        }
        auto ans = res.back();
        if (ans.type == Group::BOOL || ans.type == Group::INT ||
            ans.type == Group::VARCHAR || ans.type == Group::DOUBLE) {
            return ans;
        } else {
            throw std::exception{};
        }
    } catch (const std::exception& e) {
        return {Group::NAME, "error"};
    }
}

inline bool rpn::isOperation(std::string& op) {
    return rpn::RpnParser::operations.find(op) !=
           rpn::RpnParser::operations.end();
}

bool rpn::isBinaryOperation(std::string& op) {
    return (find(rpn::RpnParser::binary_operations.begin(),
                 rpn::RpnParser::binary_operations.end(), op) !=
            rpn::RpnParser::binary_operations.end());
}

inline bool rpn::isArithmeticalOperation(std::string& op) {
    return (find(rpn::RpnParser::arithmetical_operations.begin(),
                 rpn::RpnParser::arithmetical_operations.end(), op) !=
            rpn::RpnParser::arithmetical_operations.end());
}

inline bool rpn::isLogicalOperation(std::string& op) {
    return (find(rpn::RpnParser::logical_operations.begin(),
                 rpn::RpnParser::logical_operations.end(), op) !=
            rpn::RpnParser::logical_operations.end());
}

// ----------------- parser ---------------------

rpn::RpnParser::RpnParser(std::string& input) {
    ParseString(input);
    SetTypes();
}

std::vector<rpn::lex> rpn::RpnParser::GetRpn() {
    std::vector<lex> result;
    std::vector<lex> buffer;
    for (int i = 0; i < input_.size(); ++i) {
        lex elem = input_[i];

        if (elem.value == "(" || elem.value == ")") { // скобка
            ProcessBraces(elem, buffer, result);
        } else if (elem.type == Group::ARITHMETIC_OP ||
                   elem.type == Group::LOGICAl_OP) { // операция
            ProcessOperation(elem, buffer, result);
        } else {
            result.emplace_back(elem); // константы
        }
    }

    std::vector<lex> tmp;
    while (!result.empty()) {
        tmp.emplace_back(result.back());
        result.pop_back();
    }
    return rpn_ = tmp;
}

// ------------------------------------

bool rpn::RpnParser::isPunctuation(char c) {
    return std::find(punctuation_.begin(), punctuation_.end(), c) !=
           punctuation_.end();
}

void rpn::RpnParser::GetWord(std::string& input_) {
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

void rpn::RpnParser::GetChar(std::string& input_, bool ignore = true) {
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
    index_++;
}

void rpn::RpnParser::PutChar() {
    index_--;
}

// ----------------------------------


void rpn::RpnParser::ProcessBraces(lex& element, std::vector<lex>& buffer,
                                   std::vector<lex>& result) {
    if (element.value == "(") {
        buffer.emplace_back(element);
    } else if (element.value == ")") {
        while (!buffer.empty() && buffer.back().value != "(") {
            lex tmp = buffer.back();
            buffer.pop_back();
            result.emplace_back(tmp);
        }
        if (!buffer.empty() && buffer.back().value == "(") {
            buffer.pop_back();
        }
    }
}


void rpn::RpnParser::ProcessOperation(lex& operation, std::vector<lex>& buffer,
                                      std::vector<lex>& result) {
    if (operation.value == "IS NOT NULL" || operation.value == "IS NULL") {
        result.emplace_back(operation);
        return;
    }

    // compare operations priority
    if (operations[buffer.back().value] <= operations[operation.value]) {
        while (!buffer.empty() && buffer.back().value != "(" &&
               isOperation(buffer.back().value) &&
               operations[buffer.back().value] <= operations[operation.value]) {
            result.emplace_back(buffer.back());
            buffer.pop_back();
        }
        buffer.emplace_back(operation);
    } else {
        buffer.emplace_back(operation);
    }
}

void rpn::RpnParser::ReplaceUnaryMinus() {
    for (int i = 1; i < input_.size(); ++i) {
        std::string& prev = input_[i - 1].value;
        if (input_[i].value == "-" &&
            (isBinaryOperation(prev) || prev == "(")) {
            input_[i].value = '_';
        }
    }
}


void rpn::RpnParser::ParseString(std::string& str) {
    std::string word;

    input_.emplace_back("(");

    std::string tmp;
    while (index_ < str.size()) {
        GetChar(str);
        tmp = char_;
        if (char_ == '\'') {
            GetChar(str, false);
            while (char_ != '\'' && char_ != '\0') {
                tmp += char_;
                GetChar(str, false);
            }
            tmp += '\'';
            input_.emplace_back(tmp);
            continue;
        }
        if (char_ == '<') {
            GetChar(str);
            if (char_ == '>') {
                input_.emplace_back("<>");
                continue;
            } else {
                PutChar();
            }
        }
        if (isOperation(tmp) || char_ == '(' ||
            char_ == ')') { // example: %, *, +
            input_.emplace_back(tmp);
            continue;
        }
        PutChar();
        GetWord(str);
        if (word_ == "IS") {
            tmp = word_;
            GetWord(str);
            if (word_ == "NOT") {
                tmp += " NOT";
                GetWord(str);
            }
            if (word_ == "NULL") {
                tmp += " NULL";
                input_.emplace_back(tmp);
                continue;
            } else {
                continue;
            }
        }

        input_.emplace_back(word_); // example: AND, OR and constants/names
    }
    input_.emplace_back(")");
    ReplaceUnaryMinus();
}

inline bool isInt(const std::string& str) {
    for (const auto& c: str) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    return true;
}


inline bool isDouble(const std::string& str) {
    size_t i = 0;
    for (; i < str.size(); ++i) {
        if (str[i] == '.') {
            break;
        }
        if (!std::isdigit(str[i])) {
            return false;
        }
    }

    if (i == str.size()) {
        return true;
    }
    ++i;
    if (i == str.size()) {
        return false;
    }
    for (; i < str.size(); ++i) {
        if (!std::isdigit(str[i])) {
            return false;
        }
    }
    return true;
}


void rpn::RpnParser::SetTypes() {
    for (auto& elem: input_) {
        if (isLogicalOperation(elem.value)) {
            elem.type = Group::LOGICAl_OP;
        } else if (isArithmeticalOperation(elem.value)) {
            elem.type = Group::ARITHMETIC_OP;
        } else if (elem.value[0] == '\'' &&
                   elem.value[elem.value.size() - 1] == '\'') {
            elem.type = Group::VARCHAR;
        } else if (elem.value == "true" || elem.value == "false") {
            elem.type = Group::BOOL;
        } else if (elem.value == "(" || elem.value == ")") {
            elem.type = Group::BRACES;
        } else if (isInt(elem.value)) {
            elem.type = Group::INT;
        } else if (isDouble(elem.value)) {
            elem.type = Group::DOUBLE;
        } else {
            elem.type = Group::NAME;
        }
    }
}
