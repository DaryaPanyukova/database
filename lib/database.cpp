#include "database.h"

Database::Database() : correct_(true) {}

void Database::ImportTable(const std::string& table_name,
                           const std::string& filename) {
    Table t;

    std::ifstream in(filename);
    if (!in) {
        throw std::runtime_error("Can't open file " + filename);
    }

    size_t columns_num, rows_num;
    in >> columns_num >> rows_num;


    std::vector<std::pair<std::string, ColumnInfo>> header;
    for (size_t i = 0; i < columns_num; ++i) {
        std::string col;
        in >> col;
        header.emplace_back(col, ColumnInfo{});
        header[i].second.number_ = i;
    }
    for (size_t i = 0; i < columns_num; ++i) {
        char type;
        in >> type;
        if (type == '0') {
            header[i].second.type_ = BOOL;
        } else if (type == '1') {
            header[i].second.type_ = INT;
        } else if (type == '2') {
            header[i].second.type_ = VARCHAR;
        } else if (type == '3') {
            header[i].second.type_ = DOUBLE;
        }
    }
    for (size_t i = 0; i < columns_num; ++i) {
        char constraints[4];
        in >> constraints;
        header[i].second.unique_ = constraints[0] - '0';
        header[i].second.default_ = constraints[1] - '0';
        header[i].second.primary_key_ = constraints[2] - '0';
        header[i].second.not_null_ = constraints[3] - '0';
    }

    for (const auto& elem: header) {
        t.AddColumn(elem.first, elem.second);
    }

    for (size_t row = 0; row < rows_num; ++row) {
        Row r(columns_num);
        for (size_t col = 0; col < columns_num; ++col) {
            std::string cell;
            in >> cell;
            if (header[col].second.type_ == INT) {
                r.SetCell(std::stoi(cell), col);
            } else if (header[col].second.type_ == BOOL) {
                r.SetCell(cell == "true", col);
            } else if (header[col].second.type_ == DOUBLE) {
                r.SetCell(std::stod(cell), col);
            } else {
                r.SetCell(cell, col);
            }
        }
        t.AddRow(r);
    }
    tables_.insert({table_name, t});
}

Table Database::Request(const std::string& input) {
    SqlParser p(input);
    correct_ = correct_ && p.isCorrect();
    if (!p.isCorrect()) {
        return {};
    }

    std::string last;
    std::vector<Query*> queries = p.getQueries();
    for (auto & querie : queries) {
        Query* q = querie;
        last = q->getTableName();
        if (!querie->Execute(tables_)) {
            correct_ = false;
            return {};
        }
    }

    return tables_[last]; // которую мы меняли в последнем запросе
}

void Database::SaveToFile(const std::string& table_name,
                          const std::string& filename) {
    Table& t = tables_[table_name];
    SaveToFile(t, filename);
}

void Database::SaveToFile(const Table& t,
                          const std::string& filename) {
    std::ofstream out(filename);
    if (!out) {
        throw std::runtime_error("Can't open file " + filename);
    }
    out << t.getColumns().size() << ' ' << t.getTable().size() << '\n';
    for (const auto& col: t.getColumns()) {
        out << col.first << ' ';
    }
    out << '\n';

    for (const auto& col: t.getColumns()) {
        out << col.second.type_ << ' ';
    }
    out << '\n';

    for (const auto& col: t.getColumns()) {
        out << col.second.unique_ << col.second.default_
            << col.second.primary_key_ << col.second.not_null_ << ' ';
    }
    out << '\n';

    for (const auto& row: t.getTable()) {
        for (const auto& col: t.getColumns()) {
            out << row.GetCellAsString(col.first, const_cast
                    <std::unordered_map<std::string, ColumnInfo>&>(t.getColumns()));
            out << ' ';
        }
        out << '\n';
    }
}

int Database::isCorrect() {
    return correct_;
}

