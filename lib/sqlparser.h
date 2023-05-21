#pragma once
#include <vector>
#include <string>

class SqlParser {
private:
    std::string input_;
    bool correct_;
    std::string word_;
    char char_;
    size_t index_;
  // std::vector <Query> queries_; // запросы во внутреннем представлении

    void GetWord();
    void GetChar();
    void PutChar();

    void Query();
    void Create();
    void DataType();
    void VarcharType();
    void Constraint();
    void Default();
    void ForeignKey();
    void Select();
    void Where();
    void From();
    void Join();
    void Update();
    void Insert();
    void Delete();
    void Drop();
    void KeyValue();
    void Name();
    void Value();
    void Bool();
    void Int();
    void Float();
    void Varchar();
    void Const();
    void Prior1();
    void Prior2();
    void Prior3();
    void Prior4();
    void Prior5();
    void Prior6();
public:
    SqlParser(const std::string& input);
    bool isCorrect() const;


    /*
    const std::vector<Query>& getQueries() const;
     */
};
