// #pragma once
// #include "dhara/nand.h"
// #include "nandflash.h"
// #include <SPI.h>

// class TranslatedFlash : public NANDFlash {
//     public:
//         TranslatedFlash(SPIClass& spi, uint8_t cs, uint8_t wp, uint8_t hold);

//         // Dhara NAND Interface
//         static int dhara_is_bad(const struct dhara_nand *n, dhara_block_t b);
//         static void dhara_mark_bad(const struct dhara_nand *n, dhara_block_t b);
//         static int dhara_erase(const struct dhara_nand *n, dhara_block_t b, dhara_error_t *err);
//         static int dhara_prog(const struct dhara_nand *n, dhara_page_t p, const uint8_t *data, dhara_error_t *err);
//         static int dhara_is_free(const struct dhara_nand *n, dhara_page_t p);
//         static int dhara_read(const struct dhara_nand *n, dhara_page_t p, size_t offset, size_t length, uint8_t *data, dhara_error_t *err);
//         static int dhara_copy(const struct dhara_nand *n, dhara_page_t src, dhara_page_t dst, dhara_error_t *err);

//         // Return the Dhara NAND instance
//         struct dhara_nand *getDharaInstance();

//     private:
//         struct dhara_nand nand_instance;
// };



