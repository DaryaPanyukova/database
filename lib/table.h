#pragma once

#include <iostream>
#include <map>
#include <unordered_map>
#include <variant>
#include <vector>

using T = std::variant<bool, int, std::string, double>;

enum Type {
    BOOL = 0,
    INT,
    VARCHAR,
    DOUBLE
};

struct ColumnInfo {
    bool unique_ = false, not_null_ = false,
            default_ = false, primary_key_ = false;
    Type type_;
    T default_value_;
    size_t number_;
};

class Row {
public:
    const std::vector<T>& getCells() const {
        return cells_;
    }

    inline static T null = "";

    T GetCell(size_t col) const {
        return cells_[col];
    }

    void SetCell(const T& val, size_t index) {
        cells_[index] = val;
    }

    void SetCellAsString(std::string val, uint8_t type, size_t index) {
        if (type == INT) {
            cells_[index] = std::stoi(val);
        } else if (type == DOUBLE) {
            cells_[index] = std::stod(val);
        } else if (type == BOOL) {
            cells_[index] = (val == "true");
        } else {
            cells_[index] = val;
        }
    }

    std::string GetCellAsString(const std::string& name,
                                std::unordered_map<std::string, ColumnInfo>& columns) const {
        size_t num = columns[name].number_;
        if (cells_[num] == null) {
            return "";
        }
        if (cells_[num].index() == BOOL) {
            return (std::get<bool>(cells_[num]) ? "true" : "false");
        } else if (cells_[num].index() == INT) {
            return std::to_string(std::get<int>(cells_[num]));
        } else if (cells_[num].index() == VARCHAR) {
            return std::get<std::string>(cells_[num]);
        } else if (cells_[num].index() == DOUBLE) {
            return std::to_string(std::get<double>(cells_[num]));
        }
    }

    explicit Row(size_t size) {
        cells_.resize(size, null);
    }

    Row(const Row& other) {
        cells_ = other.cells_;
    }

private:
    std::vector<T> cells_;
};


class Table {
private:
    std::unordered_map<std::string, ColumnInfo> columns_;
    std::vector<Row> table_;

public:
    const std::vector<Row>& getTable() const;

    const std::unordered_map<std::string, ColumnInfo>& getColumns() const;

    Table() = default;

    friend std::ostream& operator<<(std::ostream&, Table);

    void setColumns(const std::unordered_map<std::string, ColumnInfo>& columns);

    void AddRow(Row& r);

    void AddColumn(const std::string& name, const ColumnInfo& info);

    void SetCell
            (size_t row_ind, size_t col_ind, std::string& value, uint8_t type);

    ColumnInfo GetColumnInfo(const std::string& name);

    void RemoveRow(size_t index);

    void RemoveColumn(const std::string& col);

    bool operator==(const Table& rhs);
};

