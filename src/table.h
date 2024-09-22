#pragma once

#include "column.h" // Include necessary headers

class Table {
public:
    Table();
    void init(size_t _values_height);
    void add(Column column);
    std::string format_table();

private:
    std::vector<Column> columns;
    size_t values_height;
};
