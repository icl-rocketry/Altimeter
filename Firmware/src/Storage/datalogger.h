// #pragma once

// #include "nandflash.h"
// #include "Sensors/sensorStructs.h"
// #include "Commands/packets/SavedTelemetryPacket.h"

// class DataLogger {
//     public:
//         DataLogger(NANDFlash& nand_flash, int LED_RED, int LED_GREEN, int LED_BLUE);

//         uint8_t* meta_ptr;
//         // 0th byte is the number of the file
//         // 1st to 64th byte is for if each block is bad or not
//         // 65th byte represets if there are new bad blocks wrt block 1
//         // 66th byte is latest block in superblock with information
//         // 67th byte is latest page in block with information
//         // 67th byte represents if the block is full or not

//         uint8_t current_super_block;
//         uint8_t current_block;
//         uint8_t current_page;
//         void setup();
//         void peep_the_cache();
//         void update(SensorStructs::raw_measurements_t raw_data, SensorStructs::state_t estimator_data, uint8_t* data_ptr);
//         struct MetaFileInfo {
//             int start_block;
//             int end_block;
//             int file_number;
//         };
//         std::vector<MetaFileInfo> read_meta_data();
//         void erase_all_files();
//         enum nandState {
//             STARTUP,
//             IDLE,
//             LOGGING,
//             ERASING
//         };
//         nandState currentState;
//         bool logging_data;

//         void dump_next_page();
//         void format_saved_data(SavedTelemetryPacket& telemetry, uint8_t* data_ptr, int block, int page, int telem_num);
//         int data_dump_page_number;
//         int data_dump_telem_number;
//         void cache_to_data_ptr(uint8_t* data_ptr, int data_dump_telem_number);

//     private:
//         NANDFlash& nand_flash;
//         int LED_RED;
//         int LED_GREEN;
//         int LED_BLUE;
//         int start_time;
//         void increment_super_block();
//         void increment_block();
//         void new_bad_block();
//         void reset_block_number();
//         void log_data(uint8_t* data, uint16_t length);
//         void format_data(SensorStructs::raw_measurements_t raw_data, SensorStructs::state_t estimator_data, uint8_t* data_ptr);
//         void floatToBytes(float value, uint8_t* bytes);
//         float bytesToFloat(uint8_t* bytes);
//         void uint16ToBytes(uint16_t value, uint8_t* bytes);
//         uint16_t bytesToUint16(uint8_t* bytes);
//         void uint64ToBytes(uint64_t value, uint8_t* bytes);
//         uint64_t bytesToUint64(uint8_t* bytes);
// };;
