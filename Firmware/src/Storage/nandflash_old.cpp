// #include "nandflash.hpp"
// #include <string>
// #include <cstring>


// // as5f34g04snda-08lin
// NANDFlash::NANDFlash(SPIClass& spi , uint8_t cs, uint8_t wp, uint8_t hold)
//     : spi(spi), cs(cs), wp(wp), hold(hold) {
// }

// uint8_t NANDFlash::setup() { // used when not using translation layer
//     delay(3000);
//     cache_ptr = (uint8_t *)malloc(2048 * sizeof(uint8_t));
//     if (cache_ptr == nullptr) {
//         // Handle allocation failure
//         Serial.println("Failed to allocate memory for cache array");
//         return -1; // or take appropriate action
//     }
//     OOB_ptr = (uint8_t *)malloc(128 * sizeof(uint8_t));
//     if (cache_ptr == nullptr) {
//         // Handle allocation failure
//         Serial.println("Failed to allocate memory for meta array");
//         return -2; // or take appropriate action
//     }

//     digitalWrite(cs, LOW);

//     spi.transfer(0x9F); // Send Read JEDEC ID command
//     spi.transfer(0x00); // address byte

//     uint8_t manufacturer = spi.transfer(0x00); // Receive the manufacturer ID
//     uint8_t device = spi.transfer(0x00); // Receive the manufacturer ID
    
//     Serial.println(" IC Check:");
//     Serial.print("Manufacturer ID: ");
//     Serial.println(manufacturer, HEX);
//     Serial.print("Device ID: ");
//     Serial.println(device, HEX);

//     digitalWrite(cs, HIGH);

//     if (manufacturer != 0x52 || device != 0x3B)
//     {
//         Serial.println("IC not found!");
//         return -3;
//     }
//     else
//     {
//         Serial.println("IC found!");
//     }


    
//     update_config(0x00); // unlock all lock bits

//     return 0;
// }

// // for now first block will be meta data, each start up will read the latest page in the block (to get start of old block) and update the information (end of old block start of new block)
// // 64 pages per block
// // 4096 blocks
// // 262144 pages
// // 524288 values

// uint8_t NANDFlash::read_page(uint16_t block_address, uint8_t page_address) {
//     // Serial.println("Reading block: " + String(block_address) + " Page: " + String(page_address));
//     if (block_address > 4096 || page_address > 64)
//     {
//         Serial.println("Invalid block or page address read_page " + String(block_address) + " " + String(page_address));
//         return -11;
//     } 
//     uint32_t shifted_block_address = static_cast<uint32_t>(block_address) << 6;
//     uint32_t address = shifted_block_address | page_address;

//     read_to_cache(address >> 16 & 0xFF, address >> 8 & 0xFF, address & 0xFF);
    
//     uint8_t status = get_feature_loop(READ_TO_CACHE);

//     read_from_cache();

//     if (status != 0) {
//         Serial.println("Error reading page to cache, Block: " + String(block_address) + " Page: " + String(page_address));
//         return status;
//     }

//     return 0;

// }



// void NANDFlash::read_to_cache(uint8_t address1, uint8_t address2, uint8_t address3) {
//     digitalWrite(cs, LOW);
//     spi.transfer(0x13); // Send Read command
//     spi.transfer(address1); // address byte
//     spi.transfer(address2); // address byte
//     spi.transfer(address3); // address byte
//     digitalWrite(cs, HIGH);
// }

// void NANDFlash::read_from_cache() {
//     digitalWrite(cs, LOW);
//     spi.transfer(0x03); // Read instruction
//     spi.transfer(0x00); // Address byte 1
//     spi.transfer(0x00); // Address byte 2
//     spi.transfer(0x00); // Dummy byte
//     for (int i = 0; i < 2048; i++)
//     {
//         cache_ptr[i] = spi.transfer(0x00);
//     }
//     for (int i = 0; i < 128; i++)
//     {
//         OOB_ptr[i] = spi.transfer(0x00);
//     }
//     digitalWrite(cs, HIGH);
// }

// uint8_t NANDFlash::get_feature() {
//     digitalWrite(cs, LOW);
//     spi.transfer(0x0F); 
//     spi.transfer(0xC0); // Address byte 
//     int status = spi.transfer(0x00); // Dummy byte
//     digitalWrite(cs, HIGH);
//     return status;
// } 
// uint8_t NANDFlash::get_feature(uint8_t address) {
//     digitalWrite(cs, LOW);
//     spi.transfer(0x0F); 
//     spi.transfer(address); // Address byte 
//     int status = spi.transfer(0x00); // Dummy byte
//     digitalWrite(cs, HIGH);
//     return status;
// } 

// NANDFlash::NANDFlash_Errors NANDFlash::get_feature_loop(NANDFlash_Core_Processes process) {
//     uint8_t status;
//     bool ecc_caught = false;
//     bool timeout = false;

//     unsigned long current_time;
//     unsigned long start_time = micros();
//     unsigned long expected_time = start_time + 100; // the plus 100 is to give some leeway

//     switch (process) {
//         case READ_TO_CACHE:
//             expected_time += 70; // comes from data sheet
//             break;
//         // case READ_FROM_CACHE:
//         //     expected_time += 70;
//         //     break;
//         // case PROGRAM_LOAD:
//         //     expected_time += 600;
//         //     break;
//         case PROGRAM_EXECUTE:
//             expected_time += 600;
//             break;
//         case BLOCK_ERASE:
//             expected_time += 4000;
//             break;
//     }

//     do {
//         status = get_feature();
//         current_time = micros();
//         if (current_time > expected_time) {
//             timeout = true;
//         }
//     } while (status & 0x01 && current_time < (expected_time + 10000));
//     if (timeout) {
//         Serial.print("Timeout. Start time: ");
//         Serial.print(start_time);
//         Serial.print(" Current time: ");
//         Serial.print(current_time);
//         Serial.print(" Expected time: ");
//         Serial.println(expected_time);
//         return GET_FEATURE_TIMEOUT;
//     }
//     if (status & 0x04) {
//         Serial.println("E_FAIL ");
//         return GET_FEATURE_E_FAIL;
//     }
//     if (status & 0x08) {
//         Serial.println("P_FAIL ");
//         return GET_FEATURE_P_FAIL;
//     }

//     // ecc_0 and ecc_1 are not mutually exclusive
//     // if neither are set then the data is fine
//     // if both are set then data is ok but ecc limit is reached
//     // if ecc_0 is set then data is corrected
//     // if ecc_1 is set then data is uncorrectable
//     if (status & 0x10) {
//         Serial.print("ECC_0 ");
//         ecc_caught = true;
//     }
//     if (status & 0x20) {
//         Serial.print("ECC_1 ");
//         if (!ecc_caught) {
//             Serial.println("Error correction failed " + String(process));
//             return GET_FEATURE_ECC;
//         }
//     }
//     return GET_FEATURE_SUCCESS;
// }

// uint8_t NANDFlash::write_page(uint16_t block_address, uint8_t page_address, uint8_t* data) {
//     // Serial.println("Writing block: " + String(block_address) + " Page: " + String(page_address));
//     if (block_address > 4096 || page_address > 64)
//     {
//         Serial.println("Invalid block or page address write_page " + String(block_address) + " " + String(page_address));
//         return -11;
//     } 
//     uint32_t shifted_block_address = static_cast<uint32_t>(block_address) << 6;
//     uint32_t address = shifted_block_address | page_address;
//     write_enable();

//     program_load(data); // load data into cache
    
//     program_execute(address >> 16 & 0xFF, address >> 8 & 0xFF, address & 0xFF); // write from cache to memory

//     uint8_t status = get_feature_loop(PROGRAM_EXECUTE);

//     write_disable();

//     return status;

// }

// void NANDFlash::mark_bad_block(uint16_t block_address) {
//     Serial.println("Marking block as bad: " + String(block_address));
//     if (block_address > 4096)
//     {
//         Serial.println("Invalid block address mark_bad_block " + String(block_address));
//         return;
//     } 
//     uint32_t shifted_block_address = static_cast<uint32_t>(block_address) << 6;
//     uint32_t address = shifted_block_address;

//     write_enable();

//     program_load_bad_block_mark(); // load data into cache
    
//     program_execute(address >> 16 & 0xFF, address >> 8 & 0xFF, address & 0xFF); // write from cache to memory

//     get_feature_loop(PROGRAM_EXECUTE);

//     write_disable();
// }

// void NANDFlash::program_load(uint8_t* data) {
//     digitalWrite(cs, LOW);
//     spi.transfer(0x02); // Write instruction
//     spi.transfer(0x00); // Cache address byte 1
//     spi.transfer(0x00); // Cache address byte 2
//     for (int i = 0; i < 2048; i++)
//     {
//         spi.transfer(data[i]);
//     }
//     digitalWrite(cs, HIGH);
// }

// void NANDFlash::program_load_bad_block_mark() {
//     digitalWrite(cs, LOW);
//     spi.transfer(0x02); // Write instruction
//     spi.transfer(0x08); // Cache address byte 1
//     spi.transfer(0x00); // Cache address byte 2

//     for (int i = 0; i < 128; i++)
//     {
//         spi.transfer(0x00); // marking first spare area as bad block
//     }

//     digitalWrite(cs, HIGH);
// }

// void NANDFlash::program_execute(uint8_t address1, uint8_t address2, uint8_t address3) {
//     digitalWrite(cs, LOW);
//     spi.transfer(0x10); // Send Program command
//     spi.transfer(address1); // address byte
//     spi.transfer(address2); // address byte
//     spi.transfer(address3); // address byte
//     digitalWrite(cs, HIGH);
// }

// void NANDFlash::write_enable() {
//     digitalWrite(cs, LOW);
//     spi.transfer(0x06); 
//     digitalWrite(cs, HIGH);
// }

// void NANDFlash::write_disable() {
//     digitalWrite(cs, LOW);
//     spi.transfer(0x04); 
//     digitalWrite(cs, HIGH);
// }

// void NANDFlash::reset() {
//     write_disable();
// }

// void NANDFlash::update_config(uint8_t feature) {
//     switch (feature)
//     {
//         case 0x00: // unlock all lock bits
//             set_feature(0xA0, 0x00);
//             break;
//         case 0x01:
//             digitalWrite(cs, LOW);
//             spi.transfer(0x1F); 
//             spi.transfer(0x00); // Address byte 
//             spi.transfer(0x00); // Dummy byte
//             digitalWrite(cs, HIGH);
//             break;
//         case 0x02: // this disables ecc
//             set_feature(0xB0, 0x00);
//             break;
//         case 0x03: // this disables ecc
//             set_feature(0xB0, 0x10);
//             break;
//     }

// }

// void NANDFlash::set_feature(uint8_t address, uint8_t data) {
//     digitalWrite(cs, LOW);
//     spi.transfer(0x1F); 
//     spi.transfer(address); // Address byte 
//     spi.transfer(data); // Dummy byte
//     digitalWrite(cs, HIGH);
// }

// uint8_t NANDFlash::block_erase(uint16_t block_address) { // block erase also deletes the bad block marks fyi
//     Serial.println("Erasing block: " + String(block_address));
//     if (block_address > 4096)
//     {
//         Serial.println("Invalid block address block_erase " + String(block_address));
//         return -11;
//     } 
    
//     uint32_t shifted_block_address = static_cast<uint32_t>(block_address) << 6;
//     uint32_t address = shifted_block_address;

//     write_enable();

//     digitalWrite(cs, LOW);
//     spi.transfer(0xD8); // Send Erase command
//     spi.transfer(address >> 16 & 0xFF); // address byte
//     spi.transfer(address >> 8 & 0xFF); // address byte
//     spi.transfer(address & 0xFF); // address byte
//     digitalWrite(cs, HIGH);

//     uint8_t status = get_feature_loop(BLOCK_ERASE);

//     write_disable();
//     return status;
// }

// uint8_t NANDFlash::internal_transfer(uint16_t src_block_address, uint8_t src_page_address, uint16_t dst_block_address, uint8_t dst_page_address) {
//     Serial.println("Transferring block: " + String(src_block_address) + " Page: " + String(src_page_address) + " to block: " + String(dst_block_address) + " Page: " + String(dst_page_address));
//     if (src_block_address > 4096 || dst_block_address > 4096 || src_page_address > 64 || dst_page_address > 64)
//     {
//         Serial.println("Invalid block or page address internal_transfer " + String(src_block_address) + " " + String(src_page_address) + " " + String(dst_block_address) + " " + String(dst_page_address));
//         return -1;
//     } 
//     uint32_t shifted_src_block_address = static_cast<uint32_t>(src_block_address) << 6;
//     uint32_t src_address = shifted_src_block_address | src_page_address;

//     uint32_t shifted_dst_block_address = static_cast<uint32_t>(dst_block_address) << 6;
//     uint32_t dst_address = shifted_dst_block_address | dst_page_address;

//     read_to_cache(src_address >> 16 & 0xFF, src_address >> 8 & 0xFF, src_address & 0xFF);
    
//     uint8_t status = get_feature_loop(READ_TO_CACHE);
//     if (status != GET_FEATURE_SUCCESS) {
//         return status;
//     }

//     program_execute(dst_address >> 16 & 0xFF, dst_address >> 8 & 0xFF, dst_address & 0xFF); // write from cache to memory
//     status = get_feature_loop(PROGRAM_EXECUTE);
    
//     return status;

// }
// bool NANDFlash::is_block_bad(uint16_t block_address) {
//     read_page(block_address, 0);
//     return OOB_ptr[0] != 0xFF;
// }


// int NANDFlash::update_meta_data(uint8_t interrupted_super_block, uint8_t old_file_number, uint8_t* meta_ptr) {
//     // check interrupted super block isnt empty
//     uint16_t start_block = interrupted_super_block * 64 + 2; // first block in superblock
//     read_page(start_block, 0);
//     if (cache_ptr[0] == 0xFF) { // first page is empty so block is empty & superblock is empty
//         Serial.println("Interrupted Superblock empty");
//         return interrupted_super_block;
//     } else {
//         Serial.println("Interrupted Superblock not empty");
//     }
    
//     // if not empty then checks if prev block is related to the current file
//     if (interrupted_super_block > 0) { // if isb = 0 then obviously no prev block
//         read_page(0, interrupted_super_block - 1); // read bad_block meta block
//         if (cache_ptr[68] == 1) { // if last block is not full
//             Serial.println("Last block is related to current file");
//         } else {
//             Serial.println("Last block is not related to current file");
//             old_file_number++;
//         }
//     }

//     read_page(1, interrupted_super_block); // read bad_block meta block
//     memcpy(meta_ptr, cache_ptr, 2048); // copy the data from cache to meta_ptr

//     meta_ptr[0] = old_file_number; // last file number before empty to mark interrupted block as part of prev file
    
//     for (int i = 0; i < 64; i++) { // check for bad blocks in superblock
//         if (meta_ptr[i+1] == 0xFF) { // if not already marked as bad
//             if(is_block_bad(start_block + i)) { 
//                 meta_ptr[i+1] = 0; // add after file number the code of blocks in superblock that are bad
//                 meta_ptr[65] = 1; // new bad block marker
//             }
//         };
//     };


//     int current_block = 0;
//     for (int i = 0; i < 64; i++) {
//         read_page(start_block + i, 0);
//         if (cache_ptr[0] == 0xFF) { // if first byte is FF then unwritten block is free
//             current_block = i-1;
//             break;
//         }
//     }
//     int current_page = 0;
//     for (int i = 0; i < 64; i++) {
//         read_page(start_block + current_block, i);
//         if (cache_ptr[0] == 0xFF) { // if first byte is FF then unwritten
//             current_page = i - 1;
//             break;
//         }
//     }
//     if (current_page == 0 && current_block == 0) {
//         meta_ptr[66] = 63;
//         meta_ptr[67] = 63;
//         meta_ptr[68] = 1;
//     } else {
//         meta_ptr[66] = current_block;
//         meta_ptr[67] = current_page;
//         meta_ptr[68] = 0;
//     }
//     write_page(0, interrupted_super_block, meta_ptr);
//     return interrupted_super_block + 1;
// }