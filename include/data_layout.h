#ifndef NOPE_DATA_LAYOUT_H
#define NOPE_DATA_LAYOUT_H

#include <cstdint>
#include <string>

class DataLayout {
public:
    uintptr_t m_low_address;
    uintptr_t m_high_address;
    uintptr_t m_data_size;

    DataLayout(uintptr_t low_address, uintptr_t high_address, uintptr_t data_size);
    
    uintptr_t get_array_length() const;

    std::string to_string() const;
};

#endif // NOPE_DATA_LAYOUT_H