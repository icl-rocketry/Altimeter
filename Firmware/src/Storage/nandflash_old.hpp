// #pragma once
// #include <SPI.h>
// #include <Arduino.h>

// class NANDFlash { // as5f34g04snda-08lin
//     public:
//         NANDFlash(SPIClass& spi , uint8_t cs, uint8_t wp, uint8_t hold);

//         SPIClass& spi;
//         uint8_t cs;
//         uint8_t wp;
//         uint8_t hold;
//         // variables used by the filesystem

       
//         uint8_t setup();
//         uint64_t active_blocks_a;
//         uint64_t active_blocks_b;
//         uint16_t meta_start_address; 
//         uint16_t data_start_address = 0; // block address
//         uint16_t data_page_address = 0; // block address
//         uint16_t length_of_data;
//         uint8_t* cache_ptr;
//         uint8_t* OOB_ptr;
//         uint16_t write_counter = 0;

//         enum NANDFlash_Core_Processes {
//             READ_TO_CACHE,
//             READ_FROM_CACHE,
//             PROGRAM_LOAD,
//             PROGRAM_EXECUTE,
//             BLOCK_ERASE
//         };
//         enum NANDFlash_High_Level_Processes {
//             READ_MAIN_DATA,
//             READ_OOB_DATA,
//             WRITE_PAGE,
//             MARK_BAD_BLOCK,
//             UPDATE_CONFIG,
//             INTERNAL_TRANSFER
//         };
//         enum NANDFlash_Errors {
//             GET_FEATURE_ECC = -1,
//             GET_FEATURE_BAD_BLOCK = -2,
//             GET_FEATURE_P_FAIL = -3,
//             GET_FEATURE_E_FAIL = -4,
//             GET_FEATURE_TIMEOUT = -5,
//             GET_FEATURE_SUCCESS = 0
//         };
        
//         uint8_t read_page(uint16_t block_address, uint8_t page_address);
//         void read_to_cache(uint8_t address1, uint8_t address2, uint8_t address3);
//         void read_from_cache();
//         uint8_t get_feature();
//         uint8_t get_feature(uint8_t address);
//         NANDFlash::NANDFlash_Errors get_feature_loop(NANDFlash_Core_Processes process);
//         uint8_t write_page(uint16_t block_address, uint8_t page_address, uint8_t* data);
//         void program_load(uint8_t* data);
//         void program_execute(uint8_t address1, uint8_t address2, uint8_t address3);
//         void write_enable();
//         void write_disable();
//         void reset();
//         void update_config(uint8_t feature);
//         void set_feature(uint8_t address, uint8_t data);
//         void mark_bad_block(uint16_t block_address);
//         void program_load_bad_block_mark();
//         uint8_t block_erase(uint16_t block_address);
//         uint8_t internal_transfer(uint16_t src_block_address, uint8_t src_page_address, uint16_t dst_block_address, uint8_t dst_page_address);
//         bool is_block_bad(uint16_t block_address);
//         int update_meta_data(uint8_t interrupted_super_block, uint8_t old_file_number, uint8_t* meta_ptr);
// };
// // hopefully should be data in block 1 page 0 and block 2 page zero and the meta data should reflect that