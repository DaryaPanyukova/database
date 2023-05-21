#pragma once

#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "table.h"


namespace rpn {
    enum Group {
        BOOL = 0,
        INT,
        VARCHAR,
        DOUBLE,
        NAME,
        LOGICAl_OP,
        ARITHMETIC_OP,
        BRACES
    };

    struct lex {
        lex() = default;

        lex(const std::string& val) {
            value = val;
        }

        lex(Group t, const std::string& val) {
            type = t;
            value = val;
        }

        Group type;
        std::string value;
    };

    std::vector<lex> GetRpn(std::string&);

    lex Calculate(std::vector<lex>, const Row&,
                  std::unordered_map<std::string, ColumnInfo>&);

    void CalculateBinary(const Row&, std::vector<lex>&, lex& cur,
                        std::unordered_map<std::string, ColumnInfo>&);

    void CalculateUnary(const Row& row, std::vector<lex>& res, lex& cur,
                             std::unordered_map<std::string, ColumnInfo>& columns);

    inline bool isOperation(std::string&);

    inline bool isBinaryOperation(std::string&);

    inline bool isArithmeticalOperation(std::string&);

    inline bool isLogicalOperation(std::string&);

    class RpnParser {
    public:
        RpnParser(std::string& input);

        std::vector<lex> GetRpn();


        inline static std::map<std::string, uint8_t> operations = {
                {"_",           1},
                {"*",           2},
                {"/",           2},
                {"%",           2},
                {"-",           3},
                {"+",           3},
                {"<",           4},
                {">",           4},
                {"<=",          4},
                {">=",          4},
                {"=",           4},
                {"<>",          4},
                {"IS NULL",     4},
                {"IS NOT NULL", 4},
                {"NOT",         5},
                {"AND",         6},
                {"OR",          7}
        };

        inline static std::vector<std::string> binary_operations = {
                "*", "/", "%", "-", "+", "<", ">",
                "<=", ">=", "=", "<>", "AND", "OR"
        };
        inline static std::vector<std::string> unary_operations = {
                "_", "NOT", "IS NULL", "IS NOT NULL"
        };
        inline static std::vector<char> punctuation_{';', ',', '(', ')', '+', '-',
                                       '/', '*', '%', '=', '>', '<'
        };
        inline static std::vector<std::string> logical_operations = {
                "<", ">", "<=", ">=", "=", "<>", "IS NULL",
                "IS NOT NULL", "NOT", "AND", "OR"
        };
        inline static std::vector<std::string> arithmetical_operations = {
                "_", "*", "/", "%", "-", "+"
        };
    private:

        size_t index_ = 0;
        std::string word_;
        char char_;
        std::vector<lex> input_;
        std::vector<lex> rpn_;

        bool isPunctuation(char);

        void GetWord(std::string&);

        void GetChar(std::string&, bool);

        void PutChar();

        // ----------------------------------


        void ProcessBraces(lex&, std::vector<lex>&, std::vector<lex>&);

        void ProcessOperation(lex&, std::vector<lex>&, std::vector<lex>&);

        void ReplaceUnaryMinus();

        void ParseString(std::string&);

        void SetTypes();


    };
}