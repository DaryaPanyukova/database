#include "rpn.h"



std::map<std::string, int> operations = {
        {"_", 1}, {"*", 3}, {"/", 3}, {"%", 3},
        {"-", 4}, {"+", 4}, {"<", 4}, {">", 4}, {"<=", 4},
        {">=", 4}, {"=", 4}, {"<>", 4}, {"AND", 4},
        {"OR", 4}, {"NOT", 4}, {"IS NULL", 4},  {"IS NOT NULL", 4}
}; // todo приоритеты!!!


std::vector <std::string> binary_operations = { "*", "/", "%", "-", "+", "<", ">", "<=", ">=", "=", "<>", "AND", "OR" };
std::vector <std::string> unary_operations = { "_", "NOT", "IS NULL", "IS NOT NULL" };

int index_ = 0;
std::string word_;
char char_;

// ------------------------------------
const std::vector<char> punctuation_{ ';', ',', '\'', '(', ')', '+', '-',
                                      '/', '*', '%', '=', '>', '<'};
bool isPunctuation(char c) {
    return std::find(punctuation_.begin(), punctuation_.end(), c) !=
           punctuation_.end();
}

void GetWord(std::string& input_) {
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

void GetChar(std::string& input_) {
    for (; index_ < input_.size(); ++index_) {
        if (!std::isspace(input_[index_])) {
            break;
        }
    }
    if (index_ < input_.size()) {
        char_ = input_[index_];
    }
    else {
        char_ = '\0';
    }
    index_++;
}

void PutChar() {
    index_--;
}

// ----------------------------------

bool isOperation(std::string& op) {
    return operations.count(op) != 0;
}

bool isBinaryOperation(std::string& op) {
    return find(binary_operations.begin(), binary_operations.end(), op) != binary_operations.end();
}

bool isUnaryOperation(std::string& op) {
    return find(unary_operations.begin(), unary_operations.end(), op) != unary_operations.end();
}

bool ProcessBraces(std::string& element,
                   std::stack<std::string>& buffer,
                   std::stack<std::string>& result) {
    if (element == "(") {
        buffer.push(element);
        return false;
    } else if (element == ")") {
        while (!buffer.empty() && buffer.top() != "(") {
            std::string tmp = buffer.top();
            buffer.pop();
            result.push(tmp);
        }
        if (!buffer.empty() && buffer.top() == "(") {
            buffer.pop();
            return false;
        }
        else {
            return true;
        }
    }
}


void ProcessOperation(std::string& operation, std::stack<std::string>& buffer,
                      std::stack<std::string>& result) {

    if (operation == "IS NOT NULL" || operation == "IS NULL")
    {
        result.push(operation);
        return;
    }

    // compare operations priority
    if (operations[buffer.top()] <= operations[operation]) {
        while (!buffer.empty() && buffer.top() != "(" && operations.count(buffer.top()) != 0 &&
               operations[buffer.top()] <= operations[operation]) {
            result.push(buffer.top());
            buffer.pop();
        }
        buffer.push(operation);
    }
    else {
        buffer.push(operation);
    }
}

bool CheckString(std::vector <std::string>& input) {
    for (int i = 1; i < input.size(); ++i) {
        std::string& prev = input[i - 1];

        if (input[i] == "-" && (isBinaryOperation(prev) || prev == "(")) {
            input[i] = '_';
        } else if ((prev == "(" || isOperation(prev)) &&
                   !(input[i] == "(" || !isOperation(input[i]) || isUnaryOperation(input[i]))) {
            return false;
        } else if (prev == ")" || !isOperation(input[i]) &&
                                  !(input[i] == ")" || !isBinaryOperation(input[i]))) {
            return false;
        }
    }
    return true;
}


std::vector <std::string> ParseString(std::string& str) {
    std::vector<std::string> parsed_string;
    std::string word = "";

    parsed_string.push_back("(");

    std::string tmp;
    while (index_ < str.size()) {
        GetChar(str);
        tmp = char_;
        if (isOperation(tmp)) { // example: %, *, +
            parsed_string.push_back(tmp);
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
                parsed_string.push_back(tmp);
                continue;
            } else {
                continue;
                // todo некорректное выражение
            }
        }

        parsed_string.push_back(word_); // example: AND, OR and constants/names
    }
    parsed_string.push_back(")");
    return parsed_string;
}


std::stack <std::string> GetRpn(std::stack <std::string>& result) {
    std::stack <std::string> tmp;
    while (!result.empty()) {
        tmp.push(result.top());
        result.pop();
    }
    return tmp;
}


void Print(std::stack <std::string> s) {
    while (!s.empty()) {
        std::cout << s.top() << ' ';
        s.pop();
    }
    std::cout << '\n';
}

int main() {
    std::string str;
    getline(std::cin, str);

    std::vector <std::string> parsed_string = ParseString(str);
    if (!CheckString(parsed_string)) {
        std::cout << "Incorrect expression";
    }

    bool error = false;
    std::stack<std::string> result;
    std::stack<std::string> buffer;
    for (int i = 0; i < parsed_string.size(); ++i) {
        std::string element = parsed_string[i];

        if (element == "(" || element == ")") { // скобка
            if (ProcessBraces(element, buffer, result)) {
                error = true;
                exit(0); // todo
            }
        } else if (isOperation(element)) { // операция
            ProcessOperation(element, buffer, result);
        } else {
            result.push(element); // константы
        }

    }

    auto rpn = GetRpn(result);
    Print(rpn);
}