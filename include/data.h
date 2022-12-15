//
// Created by remy on 12/15/22.
//

#ifndef NOPE_DATA_H
#define NOPE_DATA_H

#include <vector>
#include "data_layout.h"
#include "constant.h"

class Data {
public:
    DataLayout m_layout;
    std::vector<uint8_t> m_data;
    bool m_overflow;

    Data(const uint8_t stack[STACK_SIZE], DataLayout layout);

    ~Data();

    Data operator+(Data const &other) const;
    Data operator-(Data const &other) const;
    Data operator*(Data const &other) const;
    Data operator/(Data const &other) const;
    bool operator==(Data const &other) const;

    uintptr_t as_uintptr() const;
private:
    Data(DataLayout layout, std::vector<uint8_t> data, bool overflow);
};

#endif //NOPE_DATA_H
