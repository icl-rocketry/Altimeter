#include "as5f34g04snda.hpp"


// as5f34g04snda-08lin
AS5F34G04SNDA::AS5F34G04SNDA(SPIClass& spi , uint8_t cs, uint8_t wp, uint8_t hold)
    : m_spi(spi), m_cs(cs), m_wp(wp), m_hold(hold) {
}

AS5F34G04SNDA::Error AS5F34G04SNDA::setup() {

    digitalWrite(m_cs, LOW);

    m_spi.transfer(0x9F); // Send Read JEDEC ID command
    m_spi.transfer(0x00); // address byte

    uint8_t manufacturer = m_spi.transfer(0x00); // Receive the manufacturer ID
    uint8_t device = m_spi.transfer(0x00); // Receive the manufacturer ID

    digitalWrite(m_cs, HIGH);

    if (manufacturer != 0x52 || device != 0x3B) return AS5F34G04SNDA::Error::INITIALISATION_FAILURE;
    
    // unlock all lock bits
    if (AS5F34G04SNDA::Error::SUCCESS != change_block_lock_feature(0x00)) return AS5F34G04SNDA::Error::ERROR_CHANGING_BLOCK_LOCK_FEATURE;

    return AS5F34G04SNDA::Error::SUCCESS;
}

void AS5F34G04SNDA::get_feature(uint8_t address) {

    digitalWrite(m_cs, LOW);
    m_spi.transfer(0x0F); 
    m_spi.transfer(address); // Address byte 
    int status = m_spi.transfer(0x00); // Dummy byte
    digitalWrite(m_cs, HIGH);

    switch (address) {
        case 0xA0:
            m_block_lock_feature = status;
            break;
        case 0xB0:
            m_one_time_program_feature = status;
            break;
        case 0xC0:
            m_status_feature = status;
            break;
    }
} 

void AS5F34G04SNDA::set_feature(uint8_t address, uint8_t data) {
    digitalWrite(m_cs, LOW);
    m_spi.transfer(0x1F); 
    m_spi.transfer(address); // Address byte 
    m_spi.transfer(data); // Dummy byte
    digitalWrite(m_cs, HIGH);
}

void AS5F34G04SNDA::read_to_cache(uint8_t address1, uint8_t address2, uint8_t address3) {
    digitalWrite(m_cs, LOW);
    m_spi.transfer(0x13); // Send Read command
    m_spi.transfer(address1); // address byte
    m_spi.transfer(address2); // address byte
    m_spi.transfer(address3); // address byte
    digitalWrite(m_cs, HIGH);
}

void AS5F34G04SNDA::read_from_cache(uint8_t* cache_ptr, uint8_t* out_of_bounds_cache_ptr) {
    digitalWrite(m_cs, LOW);
    m_spi.transfer(0x03); // Read instruction
    m_spi.transfer(0x00); // Address byte 1
    m_spi.transfer(0x00); // Address byte 2
    m_spi.transfer(0x00); // Dummy byte
    for (int i = 0; i < 2048; i++)
    {
        cache_ptr[i] = m_spi.transfer(0x00);
    }
    for (int i = 0; i < 128; i++)
    {
        out_of_bounds_cache_ptr[i] = m_spi.transfer(0x00);
    }
    digitalWrite(m_cs, HIGH);
}

void AS5F34G04SNDA::program_load(uint8_t* cache_ptr) {
    digitalWrite(m_cs, LOW);
    m_spi.transfer(0x02); // Write instruction
    m_spi.transfer(0x00); // Cache address byte 1
    m_spi.transfer(0x00); // Cache address byte 2
    for (int i = 0; i < 2048; i++)
    {
        m_spi.transfer(cache_ptr[i]);
    }
    digitalWrite(m_cs, HIGH);
}

void AS5F34G04SNDA::program_load_bad_block_mark() {
    digitalWrite(m_cs, LOW);
    m_spi.transfer(0x02); // Write instruction
    m_spi.transfer(0x08); // Cache address byte 1
    m_spi.transfer(0x00); // Cache address byte 2

    for (int i = 0; i < 128; i++)
    {
        m_spi.transfer(0x00); // marking first spare area as bad block
    }

    digitalWrite(m_cs, HIGH);
}

void AS5F34G04SNDA::program_execute(uint8_t address1, uint8_t address2, uint8_t address3) {
    digitalWrite(m_cs, LOW);
    m_spi.transfer(0x10); // Send Program Execute command
    m_spi.transfer(address1); // address byte
    m_spi.transfer(address2); // address byte
    m_spi.transfer(address3); // address byte
    digitalWrite(m_cs, HIGH);
}

void AS5F34G04SNDA::block_erase(uint8_t address1, uint8_t address2, uint8_t address3) { // block erase also deletes the bad block marks fyi
    digitalWrite(m_cs, LOW);
    m_spi.transfer(0xD8); // Send Erase command
    m_spi.transfer(address1); // address byte
    m_spi.transfer(address2); // address byte
    m_spi.transfer(address3); // address byte
    digitalWrite(m_cs, HIGH);
}

void AS5F34G04SNDA::write_enable() {
    digitalWrite(m_cs, LOW);
    m_spi.transfer(0x06); 
    digitalWrite(m_cs, HIGH);
}

void AS5F34G04SNDA::write_disable() {
    digitalWrite(m_cs, LOW);
    m_spi.transfer(0x04); 
    digitalWrite(m_cs, HIGH);
}

AS5F34G04SNDA::Error AS5F34G04SNDA::change_block_lock_feature(uint8_t new_val) {
    m_block_lock_feature = new_val;
    set_feature(0xA0, m_block_lock_feature);
    
    get_feature(0xA0);

    if (m_block_lock_feature == new_val) {
        return AS5F34G04SNDA::Error::SUCCESS;
    } else {
        return AS5F34G04SNDA::Error::ERROR_CHANGING_BLOCK_LOCK_FEATURE;
    }
}

uint8_t AS5F34G04SNDA::get_block_lock_feature() { 
    get_feature(0xA0);
    return m_block_lock_feature; 
};
uint8_t AS5F34G04SNDA::get_one_time_program_feature() { 
    get_feature(0xB0);
    return m_one_time_program_feature; 
};
uint8_t AS5F34G04SNDA::get_status_feature() { 
    get_feature(0xA0);
    return m_status_feature; 
};

