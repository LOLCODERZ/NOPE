//
// Created by remy on 12/15/22.
//

#include <stdexcept>
#include <iostream>
#include <utility>
#include "data.h"

Data::Data(const uint8_t stack[STACK_SIZE], DataLayout layout): m_layout(layout), m_overflow(false) {
    for (uintptr_t i = layout.m_low_address; i < layout.m_high_address; i++) {
        m_data.push_back(stack[i]);
    }
}

Data::~Data() {
    //delete this->m_data;
}

Data Data::operator+(Data const& other) const {
    if (this->m_layout.get_array_length() + other.m_layout.get_array_length() > 2) {
        throw std::runtime_error("Cannot add arrays!");
    }

    if (this->m_layout.m_data_size != other.m_layout.m_data_size) {
        throw std::runtime_error("Cannot add data of different size!");
    }

    auto data_size = this->m_layout.m_high_address - this->m_layout.m_low_address;
    std::vector<uint8_t> ptr;
    uint8_t carry = 0;
    for (uintptr_t i = 0; i < data_size; i++) {
        uint8_t left = this->m_data[i];
        uint8_t right = other.m_data[i];
        uint16_t sum = (uint16_t) left + (uint16_t) right + (uint16_t) carry;
        ptr.push_back(sum & 0xFF);
        carry = (sum >> 8) & 0xFF;
    }

    bool overflow = carry != 0;

    return {this->m_layout, ptr, overflow};
}

Data Data::operator-(Data const& other) const {
    if (this->m_layout.get_array_length() + other.m_layout.get_array_length() > 2) {
        throw std::runtime_error("Cannot subtract arrays!");
    }

    if (this->m_layout.m_data_size != other.m_layout.m_data_size) {
        throw std::runtime_error("Cannot subtract data of different size!");
    }

    auto data_size = this->m_layout.m_high_address - this->m_layout.m_low_address;
    std::vector<uint8_t> ptr;
    uint8_t carry = 0;
    for (uintptr_t i = 0; i < data_size; i++) {
        uint8_t left = this->m_data[i];
        uint8_t right = other.m_data[i];

        if (left > right + carry) {
            ptr[i] = left - (right + carry);
            carry = 0;
        } else {
            ptr[i] = (0x100 - (right - left)) & 0xFF;
            carry = 1;
        }
        uint16_t sum = (uint16_t) left - (uint16_t) right - (uint16_t) carry;
        ptr[i] = sum & 0xFF;
        carry = (sum >> 8) & 0xFF;
    }

    bool overflow = carry != 0;

    return {this->m_layout, ptr, overflow};
}

Data Data::operator*(Data const& other) const {
    if (this->m_layout.get_array_length() + other.m_layout.get_array_length() > 2) {
        throw std::runtime_error("Cannot multiply arrays!");
    }

    if (this->m_layout.m_data_size != other.m_layout.m_data_size) {
        throw std::runtime_error("Cannot multiply data of different size!");
    }

    // TODO: Change to * (not -)

    auto data_size = this->m_layout.m_high_address - this->m_layout.m_low_address;
    std::vector<uint8_t> ptr;
    uint8_t carry = 0;
    for (uintptr_t i = 0; i < data_size; i++) {
        uint8_t left = this->m_data[i];
        uint8_t right = other.m_data[i];

        if (left > right + carry) {
            ptr[i] = left - (right + carry);
            carry = 0;
        } else {
            ptr[i] = (0x100 - (right - left)) & 0xFF;
            carry = 1;
        }
        uint16_t sum = (uint16_t) left - (uint16_t) right - (uint16_t) carry;
        ptr[i] = sum & 0xFF;
        carry = (sum >> 8) & 0xFF;
    }

    bool overflow = carry != 0;

    return {this->m_layout, ptr, overflow};
}

Data Data::operator/(Data const& other) const {
    if (this->m_layout.get_array_length() + other.m_layout.get_array_length() > 2) {
        throw std::runtime_error("Cannot divide arrays!");
    }

    if (this->m_layout.m_data_size != other.m_layout.m_data_size) {
        throw std::runtime_error("Cannot divide data of different size!");
    }

    // TODO: Change to / (not -)

    auto data_size = this->m_layout.m_high_address - this->m_layout.m_low_address;
    std::vector<uint8_t> ptr;
    uint8_t carry = 0;
    for (uintptr_t i = 0; i < data_size; i++) {
        uint8_t left = this->m_data[i];
        uint8_t right = other.m_data[i];

        if (left > right + carry) {
            ptr[i] = left - (right + carry);
            carry = 0;
        } else {
            ptr[i] = (0x100 - (right - left)) & 0xFF;
            carry = 1;
        }
        uint16_t sum = (uint16_t) left - (uint16_t) right - (uint16_t) carry;
        ptr[i] = sum & 0xFF;
        carry = (sum >> 8) & 0xFF;
    }

    bool overflow = carry != 0;

    return {this->m_layout, ptr, overflow};
}

bool Data::operator==(Data const& other) const {
    if (this->m_layout.get_array_length() + other.m_layout.get_array_length() > 2) {
        throw std::runtime_error("Cannot compare arrays! (for now)");
    }

    if (this->m_layout.m_data_size != other.m_layout.m_data_size) {
        throw std::runtime_error("Cannot compare data of different size!");
    }

    auto data_size = this->m_layout.m_high_address - this->m_layout.m_low_address;
    for (uintptr_t i = 0; i < data_size; i++) {
        uint8_t left = this->m_data[i];
        uint8_t right = other.m_data[i];

        if (left != right) {
            return false;
        }
    }

    return true;
}

Data::Data(DataLayout layout, std::vector<uint8_t> data, bool overflow) : m_layout(layout), m_data(std::move(data)), m_overflow(overflow) {

}

uintptr_t Data::as_uintptr() const {
    if (this->m_layout.get_array_length() > 1) {
        throw std::runtime_error("Cannot convert array to uintptr");
    }

    auto data_size = this->m_layout.m_high_address - this->m_layout.m_low_address;
    uintptr_t value = 0;
    for (uintptr_t i = 0; i < data_size; i++) {
        value = (value << 8) | (uintptr_t) this->m_data[data_size - i - 1];
    }
    return value;
}
