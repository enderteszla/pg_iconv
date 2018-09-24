//
// Created by lbezborodov on 28.03.17.
//

#include "iconverter.h"
#include "text.h"

#include "postgres.h"
#include "fmgr.h"
#include "utils/geo_decls.h"

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

PG_FUNCTION_INFO_V1(iconvert);

Datum
iconvert(PG_FUNCTION_ARGS) {
    text *from_text = PG_GETARG_TEXT_P(0);
    text *to_text = PG_GETARG_TEXT_P(1);
    text *in_text = PG_GETARG_TEXT_P(2);
    text *out_text;

    char *from = text_to_cstring(from_text);
    char *to = text_to_cstring(to_text);
    char *in = text_to_cstring(in_text);
    char *out;

    iconvert_(from, to, in, &out);

    out_text = cstring_to_text(out);

    PG_RETURN_TEXT_P(out_text);
}

PG_FUNCTION_INFO_V1(epgconvert);

Datum
epgconvert(PG_FUNCTION_ARGS) {
    text *in_text = PG_GETARG_TEXT_P(0);
    text *out_text;

    char *in = text_to_cstring(in_text);
    size_t in_size = strlen(in);
    size_t out_size = in_size + 1;
    char *out = (char*)malloc(sizeof(char) * out_size);

    unsigned char *uin = (unsigned char*)&in[0];
    unsigned char *uout = (unsigned char*)malloc(sizeof(unsigned char) * out_size);
    int in_index = 0;
    int out_index = 0;
    unsigned int cyrillic_from;
    unsigned int cyrillic_to;

    /* info("/usr/local/var/log/pg_iconv.log", "Info: > ");
    for(int i = 0; i < in_size; i ++) {
        info("/usr/local/var/log/pg_iconv.log", "%02X", in[i]);
    }
    info("/usr/local/var/log/pg_iconv.log", "\n"); */

    while(uin[in_index] != '\0') {
        if(((uin[in_index]>>4) & 0xF) == 0xD){ // Non-ASCII
            cyrillic_from = (((unsigned int)(uin[in_index] & 0xF))<<8) + uin[in_index+1];
            in_index += 2;
            if(cyrillic_from == 0x081) { // Ё
                cyrillic_to = 0x39D;
            } else if(cyrillic_from == 0x191) { // ё
                cyrillic_to = 0x39E;
            } else if(cyrillic_from >= 0x090 && cyrillic_from <= 0x0AF) { // А-Я
                cyrillic_to = cyrillic_from - 0x090 + 0x280;
            } else if(cyrillic_from >= 0x0B0 && cyrillic_from <= 0x0BF) { // а-п
                cyrillic_to = cyrillic_from - 0x0B0 + 0x3A0;
            } else if(cyrillic_from >= 0x180 && cyrillic_from <= 0x18F) { // р-я
                cyrillic_to = cyrillic_from - 0x180 + 0x3B0;
            } else {
                continue; // Non-ASCII, though not a letter? skipping.
            }
            uout[out_index ++] = (0xC<<4) | ((cyrillic_to>>8) & 0xF);
            uout[out_index ++] = cyrillic_to & 0xFF;
        } else { // ASCII
            uout[out_index ++] = uin[in_index ++];
        }
    }
    uout[out_index] = '\0';

    for(int i = 0; i < out_index; i ++) {
        out[i] = uout[i];
    }
    out[out_index] = '\0';

    /* info("/usr/local/var/log/pg_iconv.log", "Info: < ");
    for(int i = 0; i < out_index; i ++) {
        info("/usr/local/var/log/pg_iconv.log", "%02X", out[i]);
    }
    info("/usr/local/var/log/pg_iconv.log", "\n"); */

    out_text = cstring_to_text(out);

    PG_RETURN_TEXT_P(out_text);
}
