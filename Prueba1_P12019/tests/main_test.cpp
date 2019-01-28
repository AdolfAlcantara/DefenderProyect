#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <sstream>
#include "doctest.h"
#include "vga.h"

#define MAKE_SYMBOL(f, b, c) ((((b) & 0xf) << 12) | (((f) & 0xf) << 8) | ((c) & 0xff))

inline uint16_t make_symbol(uint8_t f, uint8_t b, uint8_t ch) {
    return ((b & 0xf) << 12) | ((f & 0xf) << 8) | (ch & 0xff);
}

inline int get_vga_offset(int row, int col) {
    return (row * COL_COUNT) + col;
}

TEST_CASE("VGA clearscreen") {
    set_cursor(4, 5);
    set_color(2, 3);
    put_char('A');

    int off = get_vga_offset(4, 5);
    CHECK(vgafb[off] == make_symbol(2, 3, 'A'));

    clear_screen();
    for (int i = 0; i < ROW_COUNT; i++) {
        for (int j = 0; j < COL_COUNT; j++) {
            int off = get_vga_offset(i, j);
            REQUIRE(vgafb[off] == 0x0020);
        }
    }
}

void inline test_put_char(uint8_t row, uint8_t col, uint8_t ch) {
    set_cursor(row, col);
    set_color(2, 3);
    put_char(ch);

    int off = get_vga_offset(row, col);
    CHECK(vgafb[off] == make_symbol(2, 3, ch));
}

TEST_CASE("VGA putchar") {
    test_put_char(4, 5, 'A');
    test_put_char(11, 15, 'B');
    test_put_char(27, 35, 'C');
}


TEST_CASE("VGA putchar EOL") {
    set_cursor(4, 20);
    set_color(2, 3);
    put_char('\n');
    uint8_t row, col;

    get_cursor(&row, &col);

    CHECK(row == 5);
    CHECK(col == 0);
}