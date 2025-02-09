#pragma once
#include <SPI.h>
#include <Arduino.h>

class AS5F34G04SNDA { 
    // as5f34g04snda-08lin
    // 4096 blocks
    // 64 pages per block
    // 2048 bytes per page (+ 128 out of bounds bytes)

    public:
        AS5F34G04SNDA(SPIClass& spi , uint8_t cs, uint8_t wp, uint8_t hold);

        enum class Error : uint8_t {
            SUCCESS = 0,
            INITIALISATION_FAILURE = 1,
            ERROR_CHANGING_BLOCK_LOCK_FEATURE = 2,
        };

        AS5F34G04SNDA::Error setup();
        
        void get_feature(uint8_t address);
        void set_feature(uint8_t address, uint8_t data);
        void read_to_cache(uint8_t address1, uint8_t address2, uint8_t address3);
        void read_from_cache(uint8_t* cache_ptr, uint8_t* out_of_bounds_cache_ptr);
        void program_load(uint8_t* cache_ptr);
        void program_load_bad_block_mark();
        void program_execute(uint8_t address1, uint8_t address2, uint8_t address3);
        void block_erase(uint8_t address1, uint8_t address2, uint8_t address3);
        void write_enable();
        void write_disable();

        uint8_t get_block_lock_feature();
        uint8_t get_one_time_program_feature();
        uint8_t get_status_feature();


    private:
        SPIClass& m_spi;
        uint8_t m_cs;
        uint8_t m_wp; // initialised and held high as only should be used for error states
        uint8_t m_hold;

        uint8_t m_block_lock_feature;
        uint8_t m_one_time_program_feature;
        uint8_t m_status_feature;


        AS5F34G04SNDA::Error change_block_lock_feature(uint8_t new_val);

};