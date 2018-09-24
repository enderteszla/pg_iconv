//
// Created by lbezborodov on 28.03.17.
//

#ifndef PG_ICONV_ICONVERTER_H
#define PG_ICONV_ICONVERTER_H

#include <iconv.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "log.h"

typedef struct {
    iconv_t iconv_;
    char *dest_enc_;
    char *src_enc_;
} iconverter;

iconverter* new_iconverter(char *dest_enc, char *src_enc);
void open(iconverter* iconverter_, char *dest_enc, char *src_enc);
void delete_iconverter(iconverter *iconverter_);

void iconvert_(char *src_enc, char *dst_enc, char *in, char **out);

void throw_exception(char *cause);

#endif //PG_ICONV_ICONVERTER_H
