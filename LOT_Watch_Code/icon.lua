cloudy_bits = string.char(
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x04, 0x81, 0x01, 0x00, 0x00, 0x04,
0x81, 0x00, 0x00, 0x00, 0x0C, 0x41, 0x00, 0x00, 0x00, 0x08, 0x60, 0x00,
0x00, 0x00, 0xF8, 0x2F, 0x00, 0x00, 0x00, 0x0C, 0x18, 0x00, 0x00, 0x7E,
0x06, 0x20, 0x00, 0x80, 0xC1, 0x13, 0x40, 0x0E, 0xC0, 0x00, 0xFF, 0xC0,
0x02, 0x60, 0x00, 0x82, 0x81, 0x00, 0x20, 0x00, 0x00, 0x83, 0x00, 0x30,
0x00, 0x00, 0x82, 0x01, 0x10, 0x00, 0x00, 0x8E, 0x01, 0x18, 0x00, 0x00,
0x90, 0x7F, 0x0C, 0x00, 0x00, 0xB0, 0x00, 0x04, 0x00, 0x00, 0xA0, 0x00,
0x02, 0x00, 0x00, 0xE0, 0x00, 0x02, 0x00, 0x00, 0xC0, 0x00, 0x02, 0x00,
0x00, 0x80, 0x00, 0x02, 0x00, 0x00, 0x80, 0x00, 0x02, 0x00, 0x00, 0x80,
0x00, 0x04, 0x00, 0x00, 0x80, 0x00, 0x0C, 0x00, 0x00, 0xC0, 0x00, 0x18,
0x00, 0x00, 0x60, 0x00, 0xE0, 0xFF, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)


--sunny
sunny_bits = string.char(
0x00,0x00,0x18,0x00,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x30,0x18,0x18,0x00,0x00,
0x30,0x18,0x1C,0x00,0x00,0x70,0x18,0x0C,0x00,0x00,0x60,0x10,0x0E,0x00,0xC0,0x60,
0x00,0x06,0x06,0xC0,0x01,0x00,0x00,0x07,0x80,0x03,0xFF,0x80,0x03,0x00,0xC3,0xFF,
0xC3,0x01,0x00,0xE0,0x81,0x0F,0x00,0x00,0x70,0x00,0x1C,0x00,0x0E,0x38,0x00,0x38,
0x70,0x3E,0x1C,0x00,0x70,0x7C,0x78,0x0E,0x00,0x60,0x1C,0x00,0x06,0x00,0xE0,0x00,
0x00,0x06,0x00,0xC0,0x00,0x00,0x03,0x00,0xC0,0x00,0x00,0x03,0x00,0xC0,0x00,0x1F,
0x03,0x00,0x80,0xF9,0x3F,0x03,0x00,0x80,0xF9,0x00,0x03,0x00,0xC0,0x01,0x00,0x03,
0x00,0xC0,0x00,0x00,0x07,0x00,0xC0,0x00,0x00,0x06,0x00,0xC0,0x00,0x70,0x0E,0x00,
0x60,0x0C,0x7C,0x0C,0x00,0x70,0x3C,0x1E,0x1C,0x00,0x38,0x78,0x04,0x38,0x00,0x1C,
0x40,0x00,0xF0,0x00,0x0F,0x00,0x00,0xC3,0xFF,0x87,0x00,0x80,0x83,0xFF,0xC1,0x01,
0xC0,0x01,0x00,0x80,0x03,0xE0,0x00,0x00,0x00,0x07,0x40,0x60,0x00,0x06,0x06,0x00,
0x60,0x18,0x0C,0x00,0x00,0x70,0x18,0x0C,0x00,0x00,0x30,0x18,0x1C,0x00,0x00,0x30,
0x18,0x18,0x00,0x00,0x00,0x18,0x00,0x00)


---rain
rain_bits = string.char(
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x01, 0x00, 0x00, 0x00,
0x1C, 0x07, 0x00, 0x00, 0x00, 0x06, 0xEC, 0x03, 0x00, 0x00, 0x03, 0x18,
0x04, 0x00, 0x00, 0x01, 0x00, 0x08, 0x00, 0x80, 0x00, 0x00, 0x08, 0x00,
0x80, 0x00, 0x00, 0x38, 0x00, 0xC0, 0x00, 0x00, 0xC0, 0x00, 0x60, 0x00,
0x00, 0x80, 0x00, 0x30, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x80,
0x01, 0x10, 0x00, 0x00, 0x00, 0x02, 0x10, 0x00, 0x00, 0x00, 0x06, 0x10,
0x00, 0x00, 0x00, 0x04, 0x10, 0x00, 0x00, 0x00, 0x04, 0x10, 0x00, 0x00,
0x00, 0x04, 0x20, 0x00, 0x00, 0x00, 0x06, 0xC0, 0x00, 0x00, 0x00, 0x03,
0x80, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18,
0x08, 0x0C, 0x00, 0x00, 0x38, 0x1C, 0x1C, 0x00, 0x00, 0x28, 0x2C, 0x14,
0x00, 0x00, 0x48, 0x28, 0x24, 0x00, 0x00, 0x78, 0x38, 0x3C, 0x00, 0x00,
0x00, 0x83, 0x01, 0x00, 0x00, 0x00, 0x87, 0x03, 0x00, 0x00, 0x00, 0x85,
0x06, 0x00, 0x00, 0x00, 0x8D, 0x05, 0x00, 0x00, 0x00, 0x07, 0x07, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)

--snow
snow_bits = string.char(
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x03, 0x00, 0x00, 0x00,
0x0C, 0x8C, 0x01, 0x00, 0x00, 0x06, 0xF8, 0x07, 0x00, 0x00, 0x02, 0x10,
0x08, 0x00, 0x00, 0x01, 0x00, 0x10, 0x00, 0x00, 0x01, 0x00, 0x10, 0x00,
0x00, 0x01, 0x00, 0xF0, 0x00, 0xC0, 0x01, 0x00, 0x80, 0x01, 0x40, 0x00,
0x00, 0x00, 0x01, 0x20, 0x00, 0x00, 0x00, 0x01, 0x30, 0x00, 0x00, 0x00,
0x03, 0x10, 0x00, 0x00, 0x00, 0x04, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x10,
0x00, 0x00, 0x00, 0x08, 0x30, 0x00, 0x00, 0x00, 0x08, 0x20, 0x00, 0x00,
0x00, 0x0C, 0x40, 0x00, 0x00, 0x00, 0x04, 0x80, 0x01, 0x00, 0x00, 0x03,
0x00, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04,
0x00, 0x20, 0x00, 0x00, 0x0F, 0x08, 0xF0, 0x00, 0x00, 0x1F, 0x6E, 0xF8,
0x00, 0x00, 0x0F, 0x3C, 0x70, 0x00, 0x00, 0x0F, 0x7F, 0xF0, 0x00, 0x00,
0x04, 0x3C, 0x20, 0x00, 0x00, 0x00, 0x6E, 0x00, 0x00, 0x00, 0x00, 0x08,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)

over_bits = string.char(
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0,
0x0F, 0x00, 0x00, 0x00, 0x70, 0x18, 0x00, 0x00, 0xE0, 0x1F, 0x60, 0x00,
0x00, 0xF0, 0xFF, 0x7F, 0x00, 0x00, 0x18, 0x00, 0xC0, 0x00, 0x00, 0x08,
0x00, 0x80, 0x00, 0x00, 0xFE, 0xFF, 0xFF, 0x00, 0x80, 0x01, 0x00, 0x80,
0x01, 0x80, 0x00, 0x00, 0x00, 0x02, 0x80, 0xFF, 0xFF, 0xFF, 0x07, 0xC0,
0x00, 0x00, 0x00, 0x04, 0x20, 0x00, 0x00, 0x00, 0x0C, 0xF0, 0xFF, 0xFF,
0xFF, 0x0F, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x10, 0x00, 0x00, 0x00, 0x04,
0xF0, 0xFF, 0xFF, 0xFF, 0x07, 0x20, 0x00, 0x00, 0x00, 0x02, 0x60, 0x00,
0x00, 0x80, 0x01, 0x80, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)



clear_bits = string.char(
0x00,0x00,0x07,0x00,0x00,0x00,0xF0,0x07,0x00,0x00,0x00,0x1C,0x01,0x00,0x00,0x00,
0x87,0x01,0x00,0x00,0x80,0xC1,0x00,0x00,0x00,0xC0,0x40,0x00,0x00,0x00,0x60,0x40,
0x00,0x00,0x00,0x30,0x60,0x00,0x00,0x00,0x18,0x20,0x00,0x00,0x00,0x08,0x30,0x00,
0x00,0x00,0x0C,0x10,0x00,0x00,0x00,0x06,0x10,0x00,0x00,0x00,0x02,0x10,0x00,0x00,
0x00,0x02,0x10,0x00,0x00,0x00,0x03,0x10,0x00,0x00,0x00,0x03,0x10,0x00,0x00,0x00,
0x01,0x10,0x00,0x00,0x00,0x01,0x10,0x00,0x00,0x00,0x01,0x10,0x00,0x00,0x00,0x01,
0x10,0x00,0x00,0x00,0x01,0x10,0x00,0x00,0x00,0x01,0x30,0x00,0x00,0x00,0x01,0x60,
0x00,0x00,0x00,0x03,0x60,0x00,0x00,0x00,0x02,0x40,0x00,0x00,0x00,0x02,0xC0,0x01,
0x00,0x00,0x02,0x00,0x01,0x00,0x00,0x06,0x00,0x03,0x00,0x00,0x04,0x00,0x06,0x00,
0x00,0x0C,0x00,0x1C,0x00,0x00,0x08,0x00,0x30,0x00,0x00,0x38,0x00,0xE0,0x01,0x00,
0x20,0x00,0x00,0x03,0x00,0xE0,0x00,0x00,0xFE,0x03,0xC0,0x00,0x00,0x00,0x03,0x80,
0x01,0x00,0xC0,0x01,0x00,0x07,0x00,0x60,0x00,0x00,0x1C,0x00,0x3C,0x00,0x00,0x70,
0x00,0x07,0x00,0x00,0xC0,0xFF,0x01,0x00)
