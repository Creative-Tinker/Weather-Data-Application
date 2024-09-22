#include "table.h"
#include "../dependencies/cpp-text-table/TextTable.h"

Table::Table() {}

void Table::init(size_t _values_height) {
    values_height = _values_height;
}

void Table::add(Column column) {
    columns.push_back(column);
}

std::string Table::format_table() {
    if (columns.size() == 0)
        return "";

    TextTable t;

    for (auto column : columns)
        t.add(column.header);
    t.endOfRow();

    for (auto column : columns)
        t.add(column.subheader);
    t.endOfRow();

    Column temp_column = *columns.begin();
    for (int i = 0; i < values_height; i++) {
        for (auto column : columns)
            t.add(column.format_value(i));
        t.endOfRow();
    }

    std::stringstream ss;
    ss << t;
    return ss.str();
}
