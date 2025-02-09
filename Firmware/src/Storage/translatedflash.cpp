// #include "translatedflash.h"

// TranslatedFlash::TranslatedFlash(SPIClass& spi, uint8_t cs, uint8_t wp, uint8_t hold)
//     : NANDFlash(spi, cs, wp, hold) {
//     // Initialize the Dhara NAND instance
//     nand_instance.log2_page_size = 11; // Example: 512-byte pages (2^9)
//     nand_instance.log2_ppb = 6;      // Example: 64 pages per block (2^6)
//     nand_instance.num_blocks = 4096; // Example: 1024 blocks
// }

// struct dhara_nand *TranslatedFlash::getDharaInstance() {
//     return &nand_instance;
// }

// int TranslatedFlash::dhara_is_bad(const struct dhara_nand *n, dhara_block_t b) {
//     TranslatedFlash *self = (TranslatedFlash *)n;
//     return self->is_block_bad(b) ? 1 : 0;
// }

// void TranslatedFlash::dhara_mark_bad(const struct dhara_nand *n, dhara_block_t b) {
//     TranslatedFlash *self = (TranslatedFlash *)n;
//     self->mark_bad_block(b);
// }

// int TranslatedFlash::dhara_erase(const struct dhara_nand *n, dhara_block_t b, dhara_error_t *err) {
//     TranslatedFlash *self = (TranslatedFlash *)n;
//     uint8_t result = self->block_erase(b);
//     if (result != 0) {
//         *err = DHARA_E_BAD_BLOCK;
//         return -1;
//     }
//     return 0;
// }

// int TranslatedFlash::dhara_prog(const struct dhara_nand *n, dhara_page_t p, const uint8_t *data, dhara_error_t *err) {
//     TranslatedFlash *self = (TranslatedFlash *)n;
//     uint16_t block = p >> self->nand_instance.log2_ppb;
//     uint8_t page = p & ((1 << self->nand_instance.log2_ppb) - 1);
//     uint8_t result = self->write_page(block, page, const_cast<uint8_t *>(data));
//     if (result != 0) {
//         *err = DHARA_E_BAD_BLOCK;
//         return -1;
//     }
//     return 0;
// }

// int TranslatedFlash::dhara_is_free(const struct dhara_nand *n, dhara_page_t p) {
//     // Implement a check to determine if the page is erased
//     return 0; // Example: Always returns not free
// }

// int TranslatedFlash::dhara_read(const struct dhara_nand *n, dhara_page_t p, size_t offset, size_t length, uint8_t *data, dhara_error_t *err) {
//     TranslatedFlash *self = (TranslatedFlash *)n;
//     uint16_t block = p >> self->nand_instance.log2_ppb;
//     uint8_t page = p & ((1 << self->nand_instance.log2_ppb) - 1);
//     uint8_t result = self->read_page(block, page);
//     if (result != 0) {
//         *err = DHARA_E_ECC;
//         return -1;
//     }
//     memcpy(data, self->cache_ptr + offset, length);
//     return 0;
// }

// int TranslatedFlash::dhara_copy(const struct dhara_nand *n, dhara_page_t src, dhara_page_t dst, dhara_error_t *err) {
//     TranslatedFlash *self = (TranslatedFlash *)n;
//     uint16_t src_block = src >> self->nand_instance.log2_ppb;
//     uint8_t src_page = src & ((1 << self->nand_instance.log2_ppb) - 1);
//     uint16_t dst_block = dst >> self->nand_instance.log2_ppb;
//     uint8_t dst_page = dst & ((1 << self->nand_instance.log2_ppb) - 1);
//     return self->internal_transfer(src_block, src_page, dst_block, dst_page);
// }
