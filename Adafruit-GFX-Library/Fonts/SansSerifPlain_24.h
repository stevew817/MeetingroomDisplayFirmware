// Created by http://oleddisplay.squix.ch/ Consider a donation
// In case of problems make sure that you are using the font file with the correct version!
const uint8_t SansSerif_plain_24Bitmaps[] = {

	// Bitmap Data:
	0x00, // ' '
	0xFF,0xFF,0xFF,0x03,0xF0, // '!'
	0xCF,0x3C,0xF3,0xCF,0x3C,0xC0, // '"'
	0x03,0x0C,0x03,0x0C,0x03,0x18,0x03,0x18,0x06,0x18,0x7F,0xFF,0x7F,0xFF,0x06,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x60,0xFF,0xFE,0xFF,0xFE,0x18,0x60,0x18,0xC0,0x18,0xC0,0x10,0xC0,0x30,0xC0, // '#'
	0x04,0x00,0x80,0xFE,0x3F,0xEE,0x45,0x88,0x31,0x06,0x20,0xE4,0x0F,0xC0,0xFE,0x03,0xE0,0x4E,0x08,0xC1,0x18,0x23,0xC4,0xFF,0xF8,0xFE,0x02,0x00,0x40,0x08,0x00, // '$'
	0x3C,0x03,0x07,0xE0,0x20,0xE7,0x06,0x0C,0x30,0xC0,0xC3,0x0C,0x0C,0x31,0x80,0xE7,0x10,0x07,0xE3,0x00,0x3C,0x63,0xC0,0x06,0x7E,0x00,0xCE,0x70,0x08,0xC3,0x01,0x8C,0x30,0x30,0xC3,0x03,0x0C,0x30,0x60,0xE7,0x04,0x07,0xE0,0xC0,0x3C, // '%'
	0x0F,0xC0,0x0F,0xF0,0x0E,0x18,0x06,0x00,0x03,0x00,0x01,0x80,0x00,0x60,0x00,0x78,0x00,0x76,0x07,0x31,0x83,0x30,0x61,0x98,0x18,0xCC,0x0E,0xC6,0x03,0xE3,0x80,0xF0,0xE0,0xF8,0x3F,0xEE,0x07,0xC3,0x80, // '&'
	0xFF,0xFC, // '''
	0x18,0xC3,0x18,0x61,0x86,0x30,0xC3,0x0C,0x30,0xC3,0x0C,0x10,0x61,0x86,0x0C,0x30,0x60, // '('
	0x60,0xC3,0x04,0x18,0x61,0x83,0x0C,0x30,0xC3,0x0C,0x30,0xC6,0x18,0x61,0x0C,0x31,0x80, // ')'
	0x06,0x00,0x60,0x46,0x26,0x66,0x1F,0x80,0x60,0x1F,0x86,0x66,0x46,0x20,0x60,0x06,0x00, // '*'
	0x01,0x80,0x03,0x00,0x06,0x00,0x0C,0x00,0x18,0x00,0x30,0x3F,0xFF,0xFF,0xFF,0x01,0x80,0x03,0x00,0x06,0x00,0x0C,0x00,0x18,0x00,0x30,0x00,0x60,0x00, // '+'
	0x6D,0xAD,0x80, // ','
	0xFF,0xF0, // '-'
	0xFC, // '.'
	0x03,0x01,0x81,0x80,0xC0,0x60,0x60,0x30,0x18,0x0C,0x0C,0x06,0x03,0x03,0x01,0x80,0xC0,0xC0,0x60,0x30,0x18,0x18,0x0C,0x00, // '/'
	0x0F,0x03,0xFC,0x70,0xE6,0x06,0x60,0x6C,0x03,0xC0,0x3C,0x03,0xC0,0x3C,0x03,0xC0,0x3C,0x03,0xC0,0x36,0x06,0x60,0x67,0x0E,0x3F,0xC0,0xF0, // '0'
	0x3E,0x1F,0xC3,0xD8,0x03,0x00,0x60,0x0C,0x01,0x80,0x30,0x06,0x00,0xC0,0x18,0x03,0x00,0x60,0x0C,0x01,0x80,0x30,0x7F,0xEF,0xFC, // '1'
	0x3F,0x1F,0xF3,0x87,0x00,0x70,0x06,0x00,0xC0,0x18,0x06,0x01,0xC0,0x30,0x0C,0x03,0x00,0xC0,0x30,0x0C,0x03,0x00,0xFF,0xFF,0xFC, // '2'
	0x3F,0x07,0xFC,0x40,0xE0,0x06,0x00,0x60,0x06,0x00,0xC1,0xF8,0x1F,0xC0,0x0E,0x00,0x70,0x03,0x00,0x30,0x03,0x00,0x7C,0x0E,0xFF,0xC3,0xF0, // '3'
	0x01,0xC0,0x1E,0x00,0xF0,0x0D,0x80,0x6C,0x06,0x60,0x63,0x03,0x18,0x30,0xC3,0x86,0x18,0x31,0x81,0x8F,0xFF,0xFF,0xFC,0x03,0x00,0x18,0x00,0xC0,0x06,0x00, // '4'
	0x7F,0xCF,0xF9,0x80,0x30,0x06,0x00,0xC0,0x1F,0xC3,0xFC,0x41,0xC0,0x1C,0x01,0x80,0x30,0x06,0x00,0xC0,0x3E,0x0E,0xFF,0x8F,0xE0, // '5'
	0x07,0xC1,0xFE,0x38,0x26,0x00,0x60,0x0C,0x00,0xCF,0x8D,0xFC,0xF0,0xEE,0x07,0xE0,0x3C,0x03,0xC0,0x36,0x03,0x60,0x77,0x0E,0x3F,0xC0,0xF8, // '6'
	0xFF,0xFF,0xFC,0x03,0x00,0x60,0x1C,0x03,0x00,0x60,0x1C,0x03,0x00,0xE0,0x1C,0x03,0x00,0xE0,0x18,0x03,0x00,0xE0,0x18,0x03,0x00, // '7'
	0x1F,0x83,0xFC,0x70,0xE6,0x06,0x60,0x66,0x06,0x30,0xC1,0xF8,0x3F,0xC7,0x0E,0xE0,0x7C,0x03,0xC0,0x3C,0x03,0xE0,0x77,0x0E,0x7F,0xE1,0xF8, // '8'
	0x1F,0x03,0xFC,0x70,0xEE,0x06,0xC0,0x6C,0x03,0xC0,0x3C,0x07,0xE0,0x77,0x0F,0x3F,0xB1,0xF3,0x00,0x30,0x06,0x00,0x64,0x1C,0x7F,0x83,0xE0, // '9'
	0xFC,0x00,0x0F,0xC0, // ':'
	0x6D,0x80,0x00,0x01,0xB6,0xB6, // ';'
	0x00,0x00,0x00,0x1C,0x00,0xF8,0x0F,0xC0,0xFC,0x07,0xC0,0x3E,0x00,0x78,0x00,0x7E,0x00,0x3F,0x00,0x0F,0xC0,0x03,0xF0,0x01,0xE0,0x00,0x40, // '<'
	0xFF,0xFF,0xFF,0xFC,0x00,0x00,0x00,0x00,0x00,0x1F,0xFF,0xFF,0xFF,0x80, // '='
	0x00,0x01,0xC0,0x03,0xE0,0x01,0xF8,0x00,0x7E,0x00,0x1F,0x00,0x0F,0x80,0x0F,0x00,0xFC,0x07,0xE0,0x7E,0x07,0xE0,0x0F,0x00,0x10,0x00,0x00, // '>'
	0x3E,0x7F,0xB0,0xE0,0x30,0x18,0x0C,0x0C,0x0C,0x0C,0x0E,0x06,0x03,0x01,0x80,0x00,0x00,0x30,0x18,0x0C,0x00, // '?'
	0x00,0xFC,0x00,0x3F,0xF8,0x03,0xC0,0xF0,0x38,0x01,0xC3,0x80,0x06,0x38,0x00,0x19,0x87,0x80,0xCC,0x7F,0xC3,0xC3,0x0E,0x1E,0x30,0x30,0xF1,0x81,0x87,0x8C,0x0C,0x3C,0x60,0x63,0xE3,0x03,0x1B,0x8C,0x39,0x8C,0x7F,0xF8,0x71,0xE7,0x01,0xC0,0x00,0x07,0x00,0x00,0x1E,0x07,0x00,0x7F,0xF0,0x00,0xFC,0x00, // '@'
	0x01,0xC0,0x01,0xE0,0x00,0xF0,0x00,0x6C,0x00,0x66,0x00,0x33,0x80,0x38,0xC0,0x18,0x60,0x0C,0x18,0x0E,0x0C,0x06,0x06,0x03,0xFF,0x83,0xFF,0xC1,0x80,0x71,0xC0,0x18,0xE0,0x0C,0x60,0x07,0x70,0x01,0x80, // 'A'
	0xFF,0x0F,0xFC,0xC0,0xEC,0x06,0xC0,0x6C,0x06,0xC0,0xCF,0xFC,0xFF,0xCC,0x0E,0xC0,0x3C,0x03,0xC0,0x3C,0x03,0xC0,0x3C,0x0E,0xFF,0xEF,0xF8, // 'B'
	0x07,0xF0,0x7F,0xF3,0xC1,0xDC,0x01,0x60,0x03,0x80,0x0C,0x00,0x30,0x00,0xC0,0x03,0x00,0x0C,0x00,0x30,0x00,0xE0,0x01,0x80,0x07,0x00,0x4F,0x07,0x1F,0xFC,0x1F,0xC0, // 'C'
	0xFF,0x83,0xFF,0x8C,0x0F,0x30,0x0E,0xC0,0x1B,0x00,0x7C,0x00,0xF0,0x03,0xC0,0x0F,0x00,0x3C,0x00,0xF0,0x03,0xC0,0x1F,0x00,0x6C,0x03,0xB0,0x3C,0xFF,0xE3,0xFE,0x00, // 'D'
	0xFF,0xFF,0xFF,0x00,0x60,0x0C,0x01,0x80,0x30,0x07,0xFF,0xFF,0xF8,0x03,0x00,0x60,0x0C,0x01,0x80,0x30,0x06,0x00,0xFF,0xFF,0xFC, // 'E'
	0xFF,0xFF,0xFC,0x03,0x00,0xC0,0x30,0x0C,0x03,0xFF,0xFF,0xF0,0x0C,0x03,0x00,0xC0,0x30,0x0C,0x03,0x00,0xC0,0x30,0x00, // 'F'
	0x07,0xF0,0x3F,0xF8,0xF0,0x3B,0x80,0x16,0x00,0x1C,0x00,0x30,0x00,0x60,0x00,0xC0,0x7F,0x80,0xFF,0x00,0x1E,0x00,0x3E,0x00,0x6C,0x00,0xDC,0x01,0x9E,0x07,0x1F,0xFC,0x0F,0xE0, // 'G'
	0xC0,0x1E,0x00,0xF0,0x07,0x80,0x3C,0x01,0xE0,0x0F,0x00,0x7F,0xFF,0xFF,0xFE,0x00,0xF0,0x07,0x80,0x3C,0x01,0xE0,0x0F,0x00,0x78,0x03,0xC0,0x1E,0x00,0xC0, // 'H'
	0xFF,0xFF,0xFF,0xFF,0xF0, // 'I'
	0x0C,0x30,0xC3,0x0C,0x30,0xC3,0x0C,0x30,0xC3,0x0C,0x30,0xC3,0x0C,0x30,0xC3,0x1B,0xEF,0x00, // 'J'
	0xC0,0x73,0x03,0x8C,0x1C,0x30,0xE0,0xC7,0x03,0x38,0x0D,0xC0,0x3E,0x00,0xF0,0x03,0xE0,0x0D,0xC0,0x33,0x80,0xC7,0x03,0x0E,0x0C,0x1C,0x30,0x38,0xC0,0x73,0x00,0xE0, // 'K'
	0xC0,0x18,0x03,0x00,0x60,0x0C,0x01,0x80,0x30,0x06,0x00,0xC0,0x18,0x03,0x00,0x60,0x0C,0x01,0x80,0x30,0x06,0x00,0xFF,0xFF,0xFC, // 'L'
	0xE0,0x07,0xF0,0x0F,0xF0,0x0F,0xD8,0x1F,0xD8,0x1B,0xD8,0x1B,0xCC,0x33,0xCC,0x33,0xCE,0x73,0xC6,0x63,0xC6,0x63,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0xC1,0x83,0xC0,0x03,0xC0,0x03,0xC0,0x03, // 'M'
	0xE0,0x1F,0x80,0xFC,0x07,0xF0,0x3D,0x81,0xEE,0x0F,0x30,0x79,0xC3,0xC6,0x1E,0x18,0xF0,0xE7,0x83,0x3C,0x1D,0xE0,0x6F,0x03,0xF8,0x0F,0xC0,0x7E,0x01,0xC0, // 'N'
	0x07,0xE0,0x1F,0xF8,0x3C,0x3C,0x70,0x0E,0x60,0x06,0xE0,0x07,0xC0,0x03,0xC0,0x03,0xC0,0x03,0xC0,0x03,0xC0,0x03,0xC0,0x03,0xE0,0x07,0x60,0x06,0x70,0x0E,0x38,0x1C,0x1F,0xF8,0x07,0xE0, // 'O'
	0xFF,0x1F,0xFB,0x07,0x60,0x7C,0x07,0x80,0xF0,0x1E,0x07,0xC1,0xDF,0xFB,0xFC,0x60,0x0C,0x01,0x80,0x30,0x06,0x00,0xC0,0x18,0x00, // 'P'
	0x07,0xE0,0x1F,0xF8,0x3C,0x3C,0x70,0x0E,0x60,0x06,0xE0,0x07,0xC0,0x03,0xC0,0x03,0xC0,0x03,0xC0,0x03,0xC0,0x03,0xC0,0x03,0xE0,0x07,0x60,0x06,0x70,0x0E,0x38,0x1C,0x1F,0xF8,0x07,0xF0,0x00,0x38,0x00,0x38,0x00,0x1C, // 'Q'
	0xFF,0x03,0xFF,0x0C,0x0C,0x30,0x18,0xC0,0x63,0x01,0x8C,0x06,0x30,0x30,0xFF,0xC3,0xFE,0x0C,0x1C,0x30,0x30,0xC0,0xE3,0x01,0x8C,0x07,0x30,0x0C,0xC0,0x3B,0x00,0x60, // 'R'
	0x1F,0xC7,0xFE,0x70,0x6C,0x00,0xC0,0x0C,0x00,0xE0,0x07,0x80,0x3F,0x80,0x7E,0x00,0xE0,0x03,0x00,0x30,0x03,0x00,0x3E,0x0E,0xFF,0xE3,0xF8, // 'S'
	0xFF,0xFF,0xFF,0xFC,0x0C,0x00,0x18,0x00,0x30,0x00,0x60,0x00,0xC0,0x01,0x80,0x03,0x00,0x06,0x00,0x0C,0x00,0x18,0x00,0x30,0x00,0x60,0x00,0xC0,0x01,0x80,0x03,0x00,0x06,0x00, // 'T'
	0xC0,0x1E,0x00,0xF0,0x07,0x80,0x3C,0x01,0xE0,0x0F,0x00,0x78,0x03,0xC0,0x1E,0x00,0xF0,0x07,0x80,0x3C,0x01,0xE0,0x0D,0x80,0xCE,0x0E,0x3F,0xE0,0x7C,0x00, // 'U'
	0xE0,0x03,0x30,0x03,0x9C,0x01,0x8E,0x01,0xC3,0x00,0xE1,0xC0,0x60,0x60,0x70,0x30,0x30,0x1C,0x18,0x06,0x1C,0x03,0x8C,0x01,0xC6,0x00,0x67,0x00,0x3B,0x00,0x0F,0x80,0x07,0x80,0x03,0xC0,0x00,0xE0,0x00, // 'V'
	0x60,0x38,0x0E,0xE0,0x78,0x1D,0xC0,0xF0,0x31,0x81,0xE0,0x63,0x06,0xC1,0xC7,0x0C,0xC3,0x8E,0x19,0x86,0x0C,0x33,0x0C,0x18,0xC6,0x38,0x31,0x84,0x70,0x73,0x0C,0xC0,0x66,0x19,0x80,0xCC,0x33,0x01,0xB0,0x2E,0x03,0xE0,0x78,0x03,0xC0,0xF0,0x07,0x81,0xE0,0x0E,0x03,0xC0, // 'W'
	0x30,0x0E,0x38,0x0C,0x1C,0x1C,0x0C,0x38,0x0E,0x30,0x07,0x70,0x03,0x60,0x03,0xC0,0x01,0xC0,0x03,0xC0,0x03,0xE0,0x07,0x70,0x0E,0x30,0x0C,0x38,0x1C,0x18,0x38,0x0C,0x30,0x0E,0x70,0x06, // 'X'
	0x70,0x0E,0x30,0x0C,0x38,0x1C,0x1C,0x38,0x0C,0x30,0x0E,0x70,0x06,0x60,0x03,0xC0,0x03,0xC0,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80, // 'Y'
	0xFF,0xFF,0xFF,0xF0,0x01,0x80,0x0E,0x00,0x70,0x03,0x80,0x1C,0x00,0x70,0x03,0x80,0x1C,0x00,0xE0,0x03,0x00,0x1C,0x00,0xE0,0x07,0x00,0x18,0x00,0xFF,0xFF,0xFF,0xF0, // 'Z'
	0xFF,0xF1,0x8C,0x63,0x18,0xC6,0x31,0x8C,0x63,0x18,0xC6,0x31,0x8C,0x7F,0xE0, // '['
	0xC0,0x60,0x18,0x0C,0x06,0x01,0x80,0xC0,0x60,0x18,0x0C,0x06,0x03,0x00,0xC0,0x60,0x30,0x0C,0x06,0x03,0x01,0xC0,0x60,0x30, // '\'
	0xFF,0xC6,0x31,0x8C,0x63,0x18,0xC6,0x31,0x8C,0x63,0x18,0xC6,0x31,0xFF,0xE0, // ']'
	0x03,0xC0,0x03,0xE0,0x07,0x60,0x0E,0x30,0x1C,0x18,0x30,0x0C,0x60,0x06, // '^'
	0xFF,0xFF,0xFF, // '_'
	0xC1,0x83,0x06,0x18, // '`'
	0x3F,0x0F,0xF9,0x03,0x00,0x30,0x06,0x3F,0xDF,0xFF,0x03,0xC0,0x78,0x0F,0x03,0xF0,0xF7,0xF6,0x7C,0xC0, // 'a'
	0xC0,0x0C,0x00,0xC0,0x0C,0x00,0xC0,0x0C,0xF8,0xDF,0xCF,0x0E,0xE0,0x6C,0x03,0xC0,0x3C,0x03,0xC0,0x3C,0x03,0xC0,0x3E,0x06,0xF0,0xED,0xFC,0xCF,0x80, // 'b'
	0x0F,0xC7,0xFD,0xC1,0xB0,0x0C,0x01,0x80,0x30,0x06,0x00,0xC0,0x18,0x01,0x80,0x38,0x33,0xFE,0x1F,0x80, // 'c'
	0x00,0x30,0x03,0x00,0x30,0x03,0x00,0x31,0xF3,0x3F,0xB7,0x0F,0x60,0x7C,0x03,0xC0,0x3C,0x03,0xC0,0x3C,0x03,0xC0,0x36,0x07,0x70,0xF3,0xFB,0x1F,0x30, // 'd'
	0x0F,0x83,0xFC,0x70,0xE6,0x03,0xC0,0x3F,0xFF,0xFF,0xFC,0x00,0xC0,0x0C,0x00,0x60,0x07,0x03,0x3F,0xF0,0xFC, // 'e'
	0x0F,0x8F,0xCC,0x06,0x03,0x07,0xFB,0xFC,0x60,0x30,0x18,0x0C,0x06,0x03,0x01,0x80,0xC0,0x60,0x30,0x18,0x0C,0x00, // 'f'
	0x1F,0x33,0xFB,0x70,0xF6,0x07,0xC0,0x3C,0x03,0xC0,0x3C,0x03,0xC0,0x3C,0x03,0x60,0x77,0x0F,0x3F,0xB1,0xF3,0x00,0x30,0x06,0x40,0xE7,0xFC,0x3F,0x80, // 'g'
	0xC0,0x18,0x03,0x00,0x60,0x0C,0x01,0x9F,0x37,0xF7,0x86,0xE0,0x78,0x0F,0x01,0xE0,0x3C,0x07,0x80,0xF0,0x1E,0x03,0xC0,0x78,0x0F,0x01,0x80, // 'h'
	0xFC,0x3F,0xFF,0xFF,0xFC, // 'i'
	0x18,0xC6,0x00,0x0C,0x63,0x18,0xC6,0x31,0x8C,0x63,0x18,0xC6,0x31,0x9F,0xDC, // 'j'
	0xC0,0x0C,0x00,0xC0,0x0C,0x00,0xC0,0x0C,0x0E,0xC1,0xCC,0x38,0xC7,0x0C,0xC0,0xD8,0x0F,0x00,0xF8,0x0D,0xC0,0xCE,0x0C,0x70,0xC3,0x8C,0x1C,0xC0,0xE0, // 'k'
	0xFF,0xFF,0xFF,0xFF,0xFC, // 'l'
	0xCF,0x87,0xCD,0xFC,0xFE,0xF0,0xD8,0x6E,0x07,0x03,0xC0,0x60,0x3C,0x06,0x03,0xC0,0x60,0x3C,0x06,0x03,0xC0,0x60,0x3C,0x06,0x03,0xC0,0x60,0x3C,0x06,0x03,0xC0,0x60,0x3C,0x06,0x03, // 'm'
	0xCF,0x9B,0xFB,0xC3,0x70,0x3C,0x07,0x80,0xF0,0x1E,0x03,0xC0,0x78,0x0F,0x01,0xE0,0x3C,0x07,0x80,0xC0, // 'n'
	0x1F,0x83,0xFC,0x70,0xE6,0x06,0xC0,0x3C,0x03,0xC0,0x3C,0x03,0xC0,0x3C,0x03,0x60,0x67,0x0E,0x3F,0xC1,0xF8, // 'o'
	0xCF,0x8D,0xFC,0xF0,0xEE,0x06,0xC0,0x3C,0x03,0xC0,0x3C,0x03,0xC0,0x3C,0x03,0xE0,0x6F,0x0E,0xDF,0xCC,0xF8,0xC0,0x0C,0x00,0xC0,0x0C,0x00,0xC0,0x00, // 'p'
	0x1F,0x33,0xFB,0x70,0xF6,0x07,0xC0,0x3C,0x03,0xC0,0x3C,0x03,0xC0,0x3C,0x03,0x60,0x77,0x0F,0x3F,0xB1,0xF3,0x00,0x30,0x03,0x00,0x30,0x03,0x00,0x30, // 'q'
	0xCF,0xDF,0xF0,0xE0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0, // 'r'
	0x3F,0x9F,0xEE,0x0B,0x00,0xC0,0x38,0x07,0xE0,0x3E,0x01,0xC0,0x30,0x0F,0x07,0xFF,0x9F,0xC0, // 's'
	0x30,0x30,0x30,0x30,0xFF,0xFF,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x3F,0x1F, // 't'
	0x00,0x18,0x0F,0x01,0xE0,0x3C,0x07,0x80,0xF0,0x1E,0x03,0xC0,0x78,0x0F,0x01,0xE0,0x76,0x1E,0xFE,0xCF,0x98, // 'u'
	0x60,0x19,0xC0,0x63,0x03,0x8C,0x0C,0x38,0x30,0x61,0xC1,0x86,0x07,0x18,0x0C,0xE0,0x33,0x00,0xEC,0x01,0xE0,0x07,0x80,0x1E,0x00, // 'v'
	0xC1,0xC1,0xF8,0x70,0x66,0x1E,0x19,0x87,0x86,0x63,0x63,0x9C,0xC8,0xC3,0x33,0x30,0xCC,0xCC,0x36,0x37,0x0F,0x85,0x81,0xE1,0xE0,0x78,0x78,0x1C,0x1E,0x07,0x03,0x00, // 'w'
	0x70,0x38,0xE0,0xC1,0x86,0x03,0x38,0x0F,0xC0,0x1E,0x00,0x38,0x01,0xE0,0x07,0xC0,0x33,0x01,0xCE,0x0E,0x1C,0x30,0x31,0xC0,0x60, // 'x'
	0x60,0x19,0xC0,0x63,0x03,0x8C,0x0C,0x18,0x30,0x61,0x81,0xC6,0x03,0x38,0x0C,0xC0,0x1B,0x00,0x7C,0x01,0xE0,0x03,0x80,0x0C,0x00,0x30,0x01,0xC0,0x06,0x00,0xF8,0x03,0xC0,0x00, // 'y'
	0xFF,0xFF,0xF0,0x1C,0x0E,0x03,0x01,0x80,0xE0,0x70,0x18,0x0C,0x06,0x03,0x00,0xFF,0xFF,0xF0, // 'z'
	0x03,0xC1,0xF0,0xE0,0x30,0x0C,0x03,0x00,0xC0,0x30,0x0C,0x03,0x01,0xC3,0xE0,0xF8,0x07,0x00,0xC0,0x30,0x0C,0x03,0x00,0xC0,0x30,0x0E,0x03,0xF0,0x3C, // '{'
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC0, // '|'
	0xF0,0x3E,0x01,0xC0,0x30,0x0C,0x03,0x00,0xC0,0x30,0x0C,0x03,0x00,0xE0,0x1F,0x07,0xC3,0x80,0xC0,0x30,0x0C,0x03,0x00,0xC0,0x30,0x1C,0x3F,0x0F,0x00 // '}'
};
const GFXglyph SansSerif_plain_24Glyphs[] = {
// bitmapOffset, width, height, xAdvance, xOffset, yOffset
	  {     0,   1,   1,   9,    0,    0 }, // ' '
	  {     1,   2,  18,  11,    4,  -18 }, // '!'
	  {     6,   6,   7,  11,    2,  -18 }, // '"'
	  {    12,  16,  18,  21,    2,  -18 }, // '#'
	  {    48,  11,  22,  16,    2,  -19 }, // '$'
	  {    79,  20,  18,  23,    1,  -18 }, // '%'
	  {   124,  17,  18,  20,    2,  -18 }, // '&'
	  {   163,   2,   7,   7,    2,  -18 }, // '''
	  {   165,   6,  22,  10,    2,  -19 }, // '('
	  {   182,   6,  22,  10,    1,  -19 }, // ')'
	  {   199,  12,  11,  13,    0,  -18 }, // '*'
	  {   216,  15,  15,  22,    3,  -15 }, // '+'
	  {   245,   3,   6,   8,    2,   -3 }, // ','
	  {   248,   6,   2,   9,    1,   -8 }, // '-'
	  {   250,   2,   3,   9,    3,   -3 }, // '.'
	  {   251,   9,  21,   9,    0,  -18 }, // '/'
	  {   275,  12,  18,  17,    2,  -18 }, // '0'
	  {   302,  11,  18,  16,    2,  -18 }, // '1'
	  {   327,  11,  18,  16,    2,  -18 }, // '2'
	  {   352,  12,  18,  16,    1,  -18 }, // '3'
	  {   379,  13,  18,  16,    1,  -18 }, // '4'
	  {   409,  11,  18,  15,    1,  -18 }, // '5'
	  {   434,  12,  18,  17,    2,  -18 }, // '6'
	  {   461,  11,  18,  16,    2,  -18 }, // '7'
	  {   486,  12,  18,  17,    2,  -18 }, // '8'
	  {   513,  12,  18,  17,    2,  -18 }, // '9'
	  {   540,   2,  13,   9,    3,  -13 }, // ':'
	  {   544,   3,  16,   9,    2,  -13 }, // ';'
	  {   550,  15,  14,  22,    3,  -15 }, // '<'
	  {   577,  15,   7,  22,    3,  -11 }, // '='
	  {   591,  15,  14,  22,    3,  -15 }, // '>'
	  {   618,   9,  18,  14,    2,  -18 }, // '?'
	  {   639,  21,  22,  26,    2,  -18 }, // '@'
	  {   697,  17,  18,  17,    0,  -18 }, // 'A'
	  {   736,  12,  18,  17,    2,  -18 }, // 'B'
	  {   763,  14,  18,  17,    1,  -18 }, // 'C'
	  {   795,  14,  18,  18,    2,  -18 }, // 'D'
	  {   827,  11,  18,  16,    2,  -18 }, // 'E'
	  {   852,  10,  18,  14,    2,  -18 }, // 'F'
	  {   875,  15,  18,  19,    1,  -18 }, // 'G'
	  {   909,  13,  18,  18,    2,  -18 }, // 'H'
	  {   939,   2,  18,   7,    2,  -18 }, // 'I'
	  {   944,   6,  23,   7,   -2,  -18 }, // 'J'
	  {   962,  14,  18,  16,    2,  -18 }, // 'K'
	  {   994,  11,  18,  15,    2,  -18 }, // 'L'
	  {  1019,  16,  18,  21,    2,  -18 }, // 'M'
	  {  1055,  13,  18,  18,    2,  -18 }, // 'N'
	  {  1085,  16,  18,  19,    1,  -18 }, // 'O'
	  {  1121,  11,  18,  15,    2,  -18 }, // 'P'
	  {  1146,  16,  21,  19,    1,  -18 }, // 'Q'
	  {  1188,  14,  18,  17,    2,  -18 }, // 'R'
	  {  1220,  12,  18,  16,    2,  -18 }, // 'S'
	  {  1247,  15,  18,  16,    0,  -18 }, // 'T'
	  {  1281,  13,  18,  18,    2,  -18 }, // 'U'
	  {  1311,  17,  18,  17,    0,  -18 }, // 'V'
	  {  1350,  23,  18,  25,    0,  -18 }, // 'W'
	  {  1402,  16,  18,  17,    0,  -18 }, // 'X'
	  {  1438,  16,  18,  15,   -1,  -18 }, // 'Y'
	  {  1474,  14,  18,  17,    1,  -18 }, // 'Z'
	  {  1506,   5,  23,  10,    2,  -19 }, // '['
	  {  1521,   9,  21,   9,    0,  -18 }, // '\'
	  {  1545,   5,  23,  10,    2,  -19 }, // ']'
	  {  1560,  16,   7,  21,    2,  -18 }, // '^'
	  {  1574,  12,   2,  13,    0,    4 }, // '_'
	  {  1577,   6,   5,  13,    2,  -20 }, // '`'
	  {  1581,  11,  14,  15,    1,  -14 }, // 'a'
	  {  1601,  12,  19,  16,    2,  -19 }, // 'b'
	  {  1630,  11,  14,  14,    1,  -14 }, // 'c'
	  {  1650,  12,  19,  16,    1,  -19 }, // 'd'
	  {  1679,  12,  14,  15,    1,  -14 }, // 'e'
	  {  1700,   9,  19,  10,    1,  -19 }, // 'f'
	  {  1722,  12,  19,  16,    1,  -14 }, // 'g'
	  {  1751,  11,  19,  16,    2,  -19 }, // 'h'
	  {  1778,   2,  19,   7,    2,  -19 }, // 'i'
	  {  1783,   5,  24,   7,   -1,  -19 }, // 'j'
	  {  1798,  12,  19,  15,    2,  -19 }, // 'k'
	  {  1827,   2,  19,   7,    2,  -19 }, // 'l'
	  {  1832,  20,  14,  25,    2,  -14 }, // 'm'
	  {  1867,  11,  14,  16,    2,  -14 }, // 'n'
	  {  1887,  12,  14,  15,    1,  -14 }, // 'o'
	  {  1908,  12,  19,  16,    2,  -14 }, // 'p'
	  {  1937,  12,  19,  16,    1,  -14 }, // 'q'
	  {  1966,   8,  14,  11,    2,  -14 }, // 'r'
	  {  1980,  10,  14,  13,    1,  -14 }, // 's'
	  {  1998,   8,  18,  11,    1,  -18 }, // 't'
	  {  2016,  11,  15,  16,    2,  -15 }, // 'u'
	  {  2037,  14,  14,  15,    0,  -14 }, // 'v'
	  {  2062,  18,  14,  21,    1,  -14 }, // 'w'
	  {  2094,  14,  14,  15,    0,  -14 }, // 'x'
	  {  2119,  14,  19,  15,    0,  -14 }, // 'y'
	  {  2153,  10,  14,  13,    1,  -14 }, // 'z'
	  {  2171,  10,  23,  16,    2,  -19 }, // '{'
	  {  2200,   2,  25,   9,    3,  -19 }, // '|'
	  {  2207,  10,  23,  16,    2,  -19 } // '}'
};
const GFXfont SansSerif_plain_24 = {
(uint8_t  *)SansSerif_plain_24Bitmaps,(GFXglyph *)SansSerif_plain_24Glyphs,0x20, 0x7E, 29};