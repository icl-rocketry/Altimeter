#pragma once
#include <SPI.h>
#include <Arduino.h>
#include "as5f34g04snda.hpp"

class NANDFlash { // as5f34g04snda-08lin
    public:
        NANDFlash(SPIClass& spi , uint8_t cs, uint8_t wp, uint8_t hold);
        
        enum class Error : uint8_t {
            SUCCESS = 0,
            CACHE_MEMORY_ALLOCATION = 1,
            OOB_MEMORY_ALLOCATION = 2,
            SET_UP = 3,
            INVALID_BLOCK_ADDRESS = 4,
            INVALID_PAGE_ADDRESS = 5,
            ERASE_FAIL = 6,
            PROGRAM_FAIL = 7,
            ECC_FAIL = 8,
            READ_PAGE_TIMEOUT = 9
        };

        uint8_t* m_cache_ptr;

        Error setup();
        Error read_page(uint16_t block_address, uint8_t page_address);
        Error write_page(uint16_t block_address, uint8_t page_address);
        Error internal_transfer(uint16_t src_block_address, uint8_t src_page_address, uint16_t dst_block_address, uint8_t dst_page_address);
        void mark_bad_block(uint16_t block_address);
        Error erase_block(uint16_t block_address);
        bool is_block_bad(uint16_t block_address);
        void print_cache();
        void print_cache_short();
        void print_OOB();

        int debug();
        int debug1();
    private:
        AS5F34G04SNDA m_as5f34g04snda;
  
        uint8_t* m_OOB_ptr;

        Error wait_for_processing(unsigned long wait_time);
 
};