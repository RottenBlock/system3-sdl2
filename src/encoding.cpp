#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "encoding.h"

namespace {

#include "s2utbl.h"

} // namespace

class SjisEncoding : public Encoding {
public:
	int mblen(unsigned char first_byte) override
	{
		return is_2byte(first_byte) ? 2 : 1;
	}

	int next_codepoint(const unsigned char** s) override
	{
		int code = *(*s)++;
		if (is_2byte(code))
			code = (code << 8) | (uint8)*(*s)++;
		return sjis_to_unicode(code);
	}

	std::string fromUtf8(const char* str) override
	{
		unsigned char* src = (unsigned char*)str;
		std::string result;

		while (*src) {
			if (*src <= 0x7f) {
				result += *src++;
				continue;
			}

			int u;
			if (*src <= 0xdf) {
				u = (src[0] & 0x1f) << 6 | (src[1] & 0x3f);
				src += 2;
			} else if (*src <= 0xef) {
				u = (src[0] & 0xf) << 12 | (src[1] & 0x3f) << 6 | (src[2] & 0x3f);
				src += 3;
			} else {
				result += '?';
				do src++; while ((*src & 0xc0) == 0x80);
				continue;
			}

			if (u > 0xff60 && u <= 0xff9f) {
				result += u - 0xff60 + 0xa0;
			} else {
				int c = unicode_to_sjis(u);
				if (c) {
					result += c >> 8;
					result += c & 0xff;
				} else {
					result += '?';
				}
			}
		}
		return result;
	}

	std::string toUtf8(const char* str) override
	{
		unsigned char* src = (unsigned char*)str;
		std::string result;

		while (*src) {
			if (*src <= 0x7f) {
				result += *src++;
				continue;
			}

			int c;
			if (*src >= 0xa0 && *src <= 0xdf) {
				c = 0xff60 + *src - 0xa0;
				src++;
			} else {
				c = s2u[*src - 0x80][*(src+1) - 0x40];
				src += 2;
			}

			if (c <= 0x7f) {
				result += c;
			} else if (c <= 0x7ff) {
				result += 0xc0 | c >> 6;
				result += 0x80 | (c & 0x3f);
			} else {
				result += 0xe0 | c >> 12;
				result += 0x80 | (c >> 6 & 0x3f);
				result += 0x80 | (c & 0x3f);
			}
		}
		return result;
	}

private:
	bool is_2byte(unsigned char c) {
		return (0x81 <= c && c <= 0x9f) || 0xe0 <= c;
	}

	static int unicode_to_sjis(int u) {
		for (int b1 = 0x80; b1 <= 0xff; b1++) {
			if (b1 >= 0xa0 && b1 <= 0xdf)
				continue;
			for (int b2 = 0x40; b2 <= 0xff; b2++) {
				if (u == s2u[b1 - 0x80][b2 - 0x40])
					return b1 << 8 | b2;
			}
		}
		return 0;
	}

	static uint16 sjis_to_unicode(uint16 code)
	{
		// ASCII characters.
		if (code < 0x80)
			return code;
		// 1-byte kana characters.
		if (code >= 0xa0 && code <= 0xdf)
			return 0xff60 + code - 0xa0;
		// Gaiji characters.
		if (0xeb9f <= code && code <= 0xebfc)
			return code - 0xeb9f + GAIJI_FIRST;
		if (0xec40 <= code && code <= 0xec9e)
			return code - 0xec40 + 94 + GAIJI_FIRST;

		return s2u[(code >> 8) - 0x80][(code & 0xff) - 0x40];
	}
};

class Utf8Encoding : public Encoding {
public:
	int mblen(unsigned char first_byte) override
	{
		if (first_byte <= 0xbf)
			return 1;
		if (first_byte <= 0xdf)
			return 2;
		if (first_byte <= 0xef)
			return 3;
		return 4;
	}

	int next_codepoint(const unsigned char** str) override
	{
		int code;
		const unsigned char *s = *str;

		if (*s <= 0x7f) {
			code = *s++;
		} else if (*s <= 0xbf) {
			// Invalid UTF-8 sequence
			code = '?';
			s++;
		} else if (*s <= 0xdf) {
			code = (s[0] & 0x1f) << 6 | (s[1] & 0x3f);
			s += 2;
		} else if (*s <= 0xef) {
			code = (s[0] & 0xf) << 12 | (s[1] & 0x3f) << 6 | (s[2] & 0x3f);
			s += 3;
		} else if (*s <= 0xf7) {
			code = (s[0] & 0x7) << 18 | (s[1] & 0x3f) << 12 | (s[2] & 0x3f) << 6 | (s[3] & 0x3f);
			s += 4;
		} else {
			code = 0xfffd;  // REPLACEMENT CHARACTER
			s++;
			while (0x80 <= *s && *s <= 0xbf)
				s++;
		}
		*str = s;
		return code;
	}

	std::string fromUtf8(const char* s) override
	{
		return std::string(s);
	}

	std::string toUtf8(const char* s) override
	{
		return std::string(s);
	}
};

int Encoding::mbslen(const unsigned char* s)
{
	int len = 0;
	while (*s) {
		s += mblen(*s);
		len++;
	}
	return len;
}

std::unique_ptr<Encoding> Encoding::create(const char* name)
{
	if (!strcasecmp(name, "Shift_JIS") ||
		!strcasecmp(name, "Shift-JIS") ||
		!strcasecmp(name, "SJIS") ||
		!strcasecmp(name, "CP932"))
		return std::make_unique<SjisEncoding>();

	if (!strcasecmp(name, "UTF-8") ||
		!strcasecmp(name, "UTF8"))
		return std::make_unique<Utf8Encoding>();

	WARNING("Unrecognized encoding: \"%s\"", name);
	return std::make_unique<SjisEncoding>();
}
