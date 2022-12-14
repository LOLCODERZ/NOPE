#include "data_layout.h"

DataLayout::DataLayout(uintptr_t low_address, uintptr_t high_address, uintptr_t data_size): m_low_address(low_address), m_high_address(high_address), m_data_size(data_size) {

}

uintptr_t DataLayout::get_array_length() const {
    uintptr_t address_span = (this->m_high_address - this->m_low_address);
    if (address_span < this->m_data_size) {
        return 0;
    }
    return address_span / this->m_data_size;
}

std::string DataLayout::to_string() const {
    std::string str {};

    str.append(std::to_string(this->m_data_size));
    str.push_back('[');
    str.append(std::to_string(this->get_array_length()));
    str.push_back(']');

    return str;
}