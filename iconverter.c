//
// Created by lbezborodov on 28.03.17.
//

#include "iconverter.h"

void throw_exception(char *cause) {
    fprintf( stderr, "iconv: %s\n", cause );
    info("/usr/local/var/log/pg_iconv.log", "Error: %s\n", cause);
    exit(EXIT_FAILURE);
}

static const iconv_t ICONV_INVALID_HANDLE = (iconv_t)(-1);

// Open iconv handle
// If ::iconv_open() failed, all fields in Converter could not change.
void open(iconverter* iconverter_, char *dest_enc, char *src_enc)
{
    info("/usr/local/var/log/pg_iconv.log", "Info: opening new iconverter %s -> %s\n", src_enc, dest_enc);
    iconv_t obj = iconv_open(dest_enc, src_enc);
    if (ICONV_INVALID_HANDLE == obj) {
        throw_exception("Initialization failed.");
    }
    if (ICONV_INVALID_HANDLE != iconverter_->iconv_) {
        iconv_close(iconverter_->iconv_);
        info("/usr/local/var/log/pg_iconv.log", "Info: closed obsolete iconverter %s -> %s\n", src_enc, dest_enc);
    }

    iconverter_->iconv_ = obj;
    iconverter_->dest_enc_ = dest_enc;
    iconverter_->src_enc_ = src_enc;
    info("/usr/local/var/log/pg_iconv.log", "Info: opened new iconverter %s -> %s\n", iconverter_->src_enc_, iconverter_->dest_enc_);
}

// c-tor
iconverter* new_iconverter(char *dest_enc, char *src_enc) {
    iconverter *converter_ = (iconverter*)malloc(sizeof(*converter_));
    converter_->iconv_ = ICONV_INVALID_HANDLE;
    open(converter_, dest_enc, src_enc);
    return converter_;
}

// d-tor
void delete_iconverter(iconverter *iconverter_)
{
    if (ICONV_INVALID_HANDLE != iconverter_->iconv_) {
        iconv_close(iconverter_->iconv_);
        info("/usr/local/var/log/pg_iconv.log", "Info: closed iconverter %s -> %s\n", iconverter_->src_enc_, iconverter_->dest_enc_);
    }
    free(iconverter_);
}

void iconvert_(char *src_enc, char *dst_enc, char *in, char **out) {
    iconverter* iconverter_ = new_iconverter(dst_enc, src_enc);
    info("/usr/local/var/log/pg_iconv.log", "Info: trying to iconvert string %s\n", in);

    size_t from_size = strlen(in) + 1;
    // Max length of a character is 6 bytes in UTF-8.
    size_t to_size = 6 * from_size + 1;

    // char *in = (char*)malloc(sizeof(char) * from_size);
    // strcpy(in, in);
    *out = (char*)malloc(sizeof(char) * to_size);

    // iconvert_
    size_t from_size_temp = from_size - 1;
    size_t to_size_temp = to_size - 1;
    char* from_ptr = in;
    char* to_ptr = *out;
    if (-1 == iconv(iconverter_->iconv_, &from_ptr, &from_size_temp, &to_ptr, &to_size_temp)) {
        throw_exception("Conversion failed.");
    }

    *to_ptr = '\0';
    info("/usr/local/var/log/pg_iconv.log", "Info: converted; got %s\n", *out);
    delete_iconverter(iconverter_);
}
