// #include "datalogger.h"


// DataLogger::DataLogger(NANDFlash& nand_flash, int LED_RED, int LED_GREEN, int LED_BLUE) 
//     : nand_flash(nand_flash), LED_RED(LED_RED), LED_GREEN(LED_GREEN), LED_BLUE(LED_BLUE) {}

// void DataLogger::setup() {
//     nand_flash.setup();

//     meta_ptr = (uint8_t *)malloc(2048 * sizeof(uint8_t));
//     if (meta_ptr == nullptr) {
//         // Handle allocation failure
//         Serial.println("Failed to allocate memory for cache array");
//     }

//     uint8_t interrupted_super_block = 0; // superblock that is half written
//     uint8_t old_file_number = 0; // file number to start checking where end of last data is
//     for (int i = 0; i < 64; i++) {
//         nand_flash.read_page(0, i); // read pages of block 0
        
//         if (nand_flash.cache_ptr[0] == 0xFF) { // if first byte is FF then superblock is unwritten/incomplete
//             interrupted_super_block = i;
//             Serial.println("Interrupted super block: " + String(interrupted_super_block) + " ofn: " + String(old_file_number));
//             current_super_block = nand_flash.update_meta_data(interrupted_super_block, old_file_number, meta_ptr);
//             break;
//         } else {
//             old_file_number = nand_flash.cache_ptr[0]; // if not empty then blocks ( (super_block_no) * 64 + 1) -> ( (page_no + 1) * 64 ) are full of data from this file number
//         }
//     }

//     nand_flash.read_page(1, current_super_block); // read bad_block meta block
//     memcpy(meta_ptr, nand_flash.cache_ptr, 2048); // copy the data from cache to meta_ptr
//     meta_ptr[0] = old_file_number + 1;
//     reset_block_number();
//     current_page = 0;
//     nand_flash.length_of_data = 1;
//     nand_flash.cache_ptr[0] = 0;

//     start_time = millis() + 60 * 1000; // start logging after 60 seconds
//     currentState = STARTUP;
//     digitalWrite(LED_BLUE, LOW);
//     logging_data = true;
//     digitalWrite(LED_GREEN, LOW);
//     data_dump_page_number = 128;
//     data_dump_telem_number = 0;
// }

// void DataLogger::log_data(uint8_t* data, uint16_t length) { // pushes data to the nand flash cache
//     if (length > 2048) {
//         Serial.println("Data too long to log");
//         return;
//     }

//     if (nand_flash.length_of_data + length > 2048) { // if data would wrap
//         nand_flash.write_page(current_super_block * 64 + current_block + 2, current_page, nand_flash.cache_ptr); // push to nand
//         current_page++;
//         if (current_page > 63) { 
//             increment_block();
//         }
//         nand_flash.length_of_data = 1;
//         nand_flash.cache_ptr[0] = 0;
//     }
//     memcpy(nand_flash.cache_ptr + nand_flash.length_of_data, data, length); //cpy from data to cache_ptr offset by length_of_data. length is no bytes copied from data to cache_ptr
//     nand_flash.length_of_data += length;
// };

// void DataLogger::increment_super_block() { 
//     meta_ptr[68] = 1; // block full
//     meta_ptr[67] = 63; // last page in block
//     int curr_file_number = meta_ptr[0];

//     nand_flash.write_page(0, current_super_block, meta_ptr);
//     current_super_block++;
//     nand_flash.read_page(1, current_super_block);
//     memcpy(meta_ptr, nand_flash.cache_ptr, 2048);

//     meta_ptr[0] = curr_file_number;
//     reset_block_number();
//     current_page = 0;
// };

// void DataLogger::increment_block() { 
//     current_page = 0;
//     current_block++;
//     if (current_block > 63) { 
//         increment_super_block();
//         return;
//     }
//     if (meta_ptr[current_block + 1] != 0xFF) { // if new block is bad
//         increment_block();
//     } 
//     meta_ptr[66] = current_block;
// };

// void DataLogger::reset_block_number() { 
//     current_block = 0; // start at 0
//     if (meta_ptr[current_block + 1] != 0xFF) { // if new block is bad go to next one
//         Serial.println("Bad block from meta");
//         increment_block();
//     } 
//     meta_ptr[66] = current_block;
// };

// void DataLogger::new_bad_block() {};

// void DataLogger::peep_the_cache() {
//     Serial.print("Current super block" + String(current_super_block));
//     Serial.print(" Current block" + String(current_block));
//     Serial.print(" Current page" + String(current_block));
//     Serial.print(" Cache: ");
//     for (int i = 0; i < nand_flash.length_of_data; i++) {
//         Serial.print(String(nand_flash.cache_ptr[i]) + " ");
//     }
//     Serial.println();
// }

// void DataLogger::format_data(SensorStructs::raw_measurements_t raw_data, SensorStructs::state_t estimator_data, uint8_t* data_ptr) {
//     floatToBytes(raw_data.accelgyro.ax, &data_ptr[0]);
//     floatToBytes(raw_data.accelgyro.ay, &data_ptr[4]);
//     floatToBytes(raw_data.accelgyro.az, &data_ptr[8]);
//     floatToBytes(raw_data.accel.ax, &data_ptr[12]);
//     floatToBytes(raw_data.accel.ay, &data_ptr[16]);
//     floatToBytes(raw_data.accel.az, &data_ptr[20]);
//     floatToBytes(raw_data.accelgyro.gx, &data_ptr[24]);
//     floatToBytes(raw_data.accelgyro.gy, &data_ptr[28]);
//     floatToBytes(raw_data.accelgyro.gz, &data_ptr[32]);
//     floatToBytes(raw_data.mag.mx, &data_ptr[36]);
//     floatToBytes(raw_data.mag.my, &data_ptr[40]);
//     floatToBytes(raw_data.mag.mz, &data_ptr[44]);
//     floatToBytes(raw_data.accelgyro.temp, &data_ptr[48]);
//     floatToBytes(raw_data.baro.alt, &data_ptr[52]);
//     floatToBytes(raw_data.baro.temp, &data_ptr[56]);
//     floatToBytes(raw_data.baro.press, &data_ptr[60]);
//     floatToBytes(estimator_data.eulerAngles[0], &data_ptr[64]);
//     floatToBytes(estimator_data.eulerAngles[1], &data_ptr[68]);
//     floatToBytes(estimator_data.eulerAngles[2], &data_ptr[72]);
//     floatToBytes(estimator_data.orientation.w(), &data_ptr[76]);
//     floatToBytes(estimator_data.orientation.x(), &data_ptr[80]);
//     floatToBytes(estimator_data.orientation.y(), &data_ptr[84]);
//     floatToBytes(estimator_data.orientation.z(), &data_ptr[88]);
//     floatToBytes(estimator_data.position[0], &data_ptr[92]);
//     floatToBytes(estimator_data.position[1], &data_ptr[96]);
//     floatToBytes(estimator_data.position[2], &data_ptr[100]);
//     floatToBytes(estimator_data.velocity[0], &data_ptr[104]);
//     floatToBytes(estimator_data.velocity[1], &data_ptr[108]);
//     floatToBytes(estimator_data.velocity[2], &data_ptr[112]);
//     floatToBytes(estimator_data.acceleration[0], &data_ptr[116]);
//     floatToBytes(estimator_data.acceleration[1], &data_ptr[120]);
//     floatToBytes(estimator_data.acceleration[2], &data_ptr[124]);
//     uint16ToBytes(raw_data.logicrail.volt, &data_ptr[128]);
//     uint16ToBytes(raw_data.logicrail.percent, &data_ptr[130]);
//     uint64ToBytes(micros(), &data_ptr[132]);
    
//     // Serial.print("Formatted serialized data: ");
//     // for (int i = 0; i < 140; i++) {
//     //     Serial.print(String(data_ptr[i]) + " ");
//     // }
//     // Serial.println();



// }

// void DataLogger::floatToBytes(float value, uint8_t* bytes) { // to reconvert back byte 0 is last and byte 4 is first
//     uint8_t* floatAsBytes = reinterpret_cast<uint8_t*>(&value);
//     for (int i = 0; i < 4; i++) {
//         bytes[i] = floatAsBytes[i];
//     }
// }

// union BytesToFloat {
//     uint8_t bytes[4];
//     float floatValue;
// };

// float DataLogger::bytesToFloat(uint8_t* bytes) {
//     BytesToFloat converter;
//     for (int i = 0; i < 4; i++) {
//         converter.bytes[i] = bytes[i];
//     }
//     return converter.floatValue;
// }

// void DataLogger::uint16ToBytes(uint16_t value, uint8_t* bytes) {
//     uint8_t* floatAsBytes = reinterpret_cast<uint8_t*>(&value);
//     for (int i = 0; i < 2; i++) {
//         bytes[i] = floatAsBytes[i];
//     }
// }

// uint16_t DataLogger::bytesToUint16(uint8_t* bytes) {
//     uint16_t value;
//     memcpy(&value, bytes, 2);
//     return value;
// }

// void DataLogger::uint64ToBytes(uint64_t value, uint8_t* bytes) {
//     uint8_t* floatAsBytes = reinterpret_cast<uint8_t*>(&value);
//     for (int i = 0; i < 8; i++) {
//         bytes[i] = floatAsBytes[i];
//     }
// }

// uint64_t DataLogger::bytesToUint64(uint8_t* bytes) {
//     uint64_t value;
//     memcpy(&value, bytes, 8);
//     return value;
// }

// void DataLogger::update(SensorStructs::raw_measurements_t raw_data, SensorStructs::state_t estimator_data, uint8_t* data_ptr) {
//     int current_time = millis();


//     switch(currentState) {
//         case STARTUP:
//             // Serial.println("STARTUP");
//             if (!logging_data) {
//                 currentState = IDLE;
//                 digitalWrite(LED_GREEN, HIGH);
//                 break;
//             }
//             if ((current_time > start_time)) {
//                 currentState = LOGGING;
//                 digitalWrite(LED_BLUE, HIGH);
//                 break;
//             }
            
//             break;
//         case IDLE:
//             // Serial.println("IDLE");

//             break;

//         case LOGGING:
//             Serial.println("Logging data");
//             format_data(raw_data, estimator_data, data_ptr);
//             log_data(data_ptr, 140);
//             break;

//         case ERASING:
//             Serial.println("ERASING");

//             break;
//     }
// }

// std::vector<DataLogger::MetaFileInfo> DataLogger::read_meta_data() {
//     nand_flash.read_page(0,0);
//     if (nand_flash.cache_ptr[0] == 0xFF) {
//         Serial.println("No meta data found");
//         return {};
//     }
//     int start_super_block = 0; // start block for first file
//     int end_super_block = 0; // end block for first file

//     std::vector<MetaFileInfo> files;
//     do {
//         if (nand_flash.cache_ptr[68] == 1) { // if superblock is full
//             end_super_block++;
//             nand_flash.read_page(0, end_super_block);
//         } else if (nand_flash.cache_ptr[68] == 0) { // if superblock is not full then this is last block in file
//             files.push_back({start_super_block * 64 + 2, (end_super_block) * 64 + 1 + nand_flash.cache_ptr[66] + 1, nand_flash.cache_ptr[0]});
//             start_super_block = end_super_block + 1;
//             end_super_block = start_super_block;
//             nand_flash.read_page(0, end_super_block);
//         } else {
//             // Serial.println("End of meta data");
//             break;
//         }
//     } while (true);

//     // Optionally, print file information
//     // for (const auto& file : files) {
//     //     Serial.print("File from block ");
//     //     Serial.print(file.start_block);
//     //     Serial.print(" to block ");
//     //     Serial.print(file.end_block);
//     //     Serial.print(" File number: ");
//     //     Serial.println(file.file_number);
//     // }
//     return files;

// }

// void DataLogger::erase_all_files() { // assumed only called when in idle
    
//     std::vector<MetaFileInfo> files = read_meta_data();
//     if (files.size() == 0) {
//         Serial.println("No files to erase");
//         return;
//     }
//     for (const auto& file : files) {
//         for (int i = file.start_block; i < file.end_block + 1; i++) {
//             nand_flash.read_page(i, 0);
//             if (nand_flash.cache_ptr[0] == 0xFF && nand_flash.cache_ptr[1] == 0xFF && nand_flash.cache_ptr[2] == 0xFF) { // if actually empty
//                 Serial.println("Erasing block " + String(i) + "but block is empty");
//             } else {
//                 nand_flash.block_erase(i);
//             }
//         }
//     }
//     bool update_bb_info = false;
//     for (int i = 0; i < 64; i++) {
//         nand_flash.read_page(0, i);
//         if (nand_flash.cache_ptr[65] != 0xFF) {
//             update_bb_info = true;
//             break;
//         }
//     }
//     if (update_bb_info) {
//         Serial.println("Updating bad block info");
//         nand_flash.block_erase(1); // delete old bb block

//         for (int i = 0; i < 64; i++) {

//             memset(meta_ptr, 0xFF, 2048);
//             nand_flash.read_page(0, i); // read new bb info

//             for (int j = 1; j < 65; j++) { // copy it over
//                 meta_ptr[j] = nand_flash.cache_ptr[j];
//             }
//             nand_flash.write_page(1, i, meta_ptr); // write it to new page
//         }
//     }
//     Serial.println("Deleting meta data");
//     nand_flash.block_erase(0);


// }

// void DataLogger::dump_next_page() {
//     int block = data_dump_page_number / 64;
//     int page = data_dump_page_number % 64;
//     nand_flash.read_page(block, page);
//     data_dump_page_number++;
// }

// void DataLogger::format_saved_data(SavedTelemetryPacket& telemetry, uint8_t* data_ptr, int block, int page, int telem_num) {
//     telemetry.ax = bytesToFloat(&data_ptr[0]);
//     telemetry.ay = bytesToFloat(&data_ptr[4]);
//     telemetry.az = bytesToFloat(&data_ptr[8]);
//     telemetry.h_ax = bytesToFloat(&data_ptr[12]);
//     telemetry.h_ay = bytesToFloat(&data_ptr[16]);
//     telemetry.h_az = bytesToFloat(&data_ptr[20]);
//     telemetry.gx = bytesToFloat(&data_ptr[24]);
//     telemetry.gy = bytesToFloat(&data_ptr[28]);
//     telemetry.gz = bytesToFloat(&data_ptr[32]);
//     telemetry.mx = bytesToFloat(&data_ptr[36]);
//     telemetry.my = bytesToFloat(&data_ptr[40]);
//     telemetry.mz = bytesToFloat(&data_ptr[44]);
//     // telemetry.accelgyro_temp = bytesToFloat(&data_ptr[48]);
//     telemetry.baro_alt = bytesToFloat(&data_ptr[52]);
//     telemetry.baro_temp = bytesToFloat(&data_ptr[56]);
//     telemetry.baro_press = bytesToFloat(&data_ptr[60]);
//     telemetry.roll = bytesToFloat(&data_ptr[64]);
//     telemetry.pitch = bytesToFloat(&data_ptr[68]);
//     telemetry.yaw = bytesToFloat(&data_ptr[72]);
//     telemetry.q0 = bytesToFloat(&data_ptr[76]);
//     telemetry.q1 = bytesToFloat(&data_ptr[80]);
//     telemetry.q2 = bytesToFloat(&data_ptr[84]);
//     telemetry.q3 = bytesToFloat(&data_ptr[88]);
//     telemetry.pn = bytesToFloat(&data_ptr[92]);
//     telemetry.pe = bytesToFloat(&data_ptr[96]);
//     telemetry.pd = bytesToFloat(&data_ptr[100]);
//     telemetry.vn = bytesToFloat(&data_ptr[104]);
//     telemetry.ve = bytesToFloat(&data_ptr[108]);
//     telemetry.vd = bytesToFloat(&data_ptr[112]);
//     telemetry.an = bytesToFloat(&data_ptr[116]);
//     telemetry.ae = bytesToFloat(&data_ptr[120]);
//     telemetry.ad = bytesToFloat(&data_ptr[124]);
//     telemetry.batt_voltage = bytesToUint16(&data_ptr[128]);
//     telemetry.batt_percent = bytesToUint16(&data_ptr[130]);
//     telemetry.system_time = bytesToUint64(&data_ptr[132]);
//     telemetry.block = block;
//     telemetry.page = page;
//     telemetry.telem_num = telem_num;

// }

// void DataLogger::cache_to_data_ptr(uint8_t* data_ptr, int data_dump_telem_number) {
// 	int offset = 1 + 140*data_dump_telem_number; // +1 as first byte in all pages is 0 and 140 as telem packet is that long

// 	for (int i = 0; i < 140; i++) {
//         data_ptr[i] = nand_flash.cache_ptr[offset + i];
//     }

// }