// #include "translatedflash.hpp"

// // Global pointer to the NANDFlash instance
// // This assumes a single NANDFlash device; for multiple devices, you'd need a more sophisticated mechanism.
// static TranslatedFlash *flash_instance = nullptr;

// void set_flash_instance(TranslatedFlash *instance) {
//     flash_instance = instance;
// }

// // Helper to check if the instance is set
// TranslatedFlash *get_flash_instance() {
//     if (!flash_instance) {
//         // Handle error: instance not set
//     }
//     return flash_instance;
// }

// // Dhara Interface Implementations

// int dhara_nand_is_bad(const struct dhara_nand *n, dhara_block_t b) {
//     return get_flash_instance()->is_block_bad(b) ? 1 : 0;
// }

// void dhara_nand_mark_bad(const struct dhara_nand *n, dhara_block_t b) {
//     get_flash_instance()->mark_bad_block(b);
// }

// int dhara_nand_erase(const struct dhara_nand *n, dhara_block_t b, dhara_error_t *err) {
//     if (get_flash_instance()->block_erase(b) != 0) {
//         *err = DHARA_E_BAD_BLOCK;
//         return -1;
//     }
//     return 0;
// }

// int dhara_nand_prog(const struct dhara_nand *n, dhara_page_t p, const uint8_t *data, dhara_error_t *err) {
//     uint16_t block = p >> n->log2_ppb;
//     uint8_t page = p & ((1 << n->log2_ppb) - 1);
//     if (get_flash_instance()->write_page(block, page, const_cast<uint8_t *>(data)) != 0) {
//         *err = DHARA_E_BAD_BLOCK;
//         return -1;
//     }
//     return 0;
// }

// int dhara_nand_is_free(const struct dhara_nand *n, dhara_page_t p) {
//     return get_flash_instance()->is_page_free(p);
// }

// int dhara_nand_read(const struct dhara_nand *n, dhara_page_t p, size_t offset, size_t length, uint8_t *data, dhara_error_t *err) {
//     uint16_t block = p >> n->log2_ppb;
//     uint8_t page = p & ((1 << n->log2_ppb) - 1);
//     if (get_flash_instance()->read_page(block, page) != 0) {
//         *err = DHARA_E_ECC;
//         return -1;
//     }
//     memcpy(data, get_flash_instance()->cache_ptr + offset, length);
//     return 0;
// }

// int dhara_nand_copy(const struct dhara_nand *n, dhara_page_t src, dhara_page_t dst, dhara_error_t *err) {
//     uint16_t src_block = src >> n->log2_ppb;
//     uint8_t src_page = src & ((1 << n->log2_ppb) - 1);
//     uint16_t dst_block = dst >> n->log2_ppb;
//     uint8_t dst_page = dst & ((1 << n->log2_ppb) - 1);
//     return get_flash_instance()->internal_transfer(src_block, src_page, dst_block, dst_page);
// }
