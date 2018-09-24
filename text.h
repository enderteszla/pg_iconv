//
// Created by lbezborodov on 28.03.17.
//

#ifndef PG_ICONV_TEXT_H
#define PG_ICONV_TEXT_H

#include "postgres.h"
#include "fmgr.h"
#include "utils/geo_decls.h"

/*
 * cstring_to_text
 *
 * Create a text value from a null-terminated C string.
 *
 * The new text value is freshly palloc'd with a full-size VARHDR.
 */
text *
cstring_to_text(char *s);

/*
 * cstring_to_text_with_len
 *
 * Same as cstring_to_text except the caller specifies the string length;
 * the string need not be null_terminated.
 */
text *
cstring_to_text_with_len(char *s, int len);

/*
 * ucstring_to_text_with_len
 *
 * Same as cstring_to_text except the caller specifies the string length;
 * the string need not be null_terminated.
 */
text *
ucstring_to_text_with_len(unsigned char *s, int len);

/*
 * text_to_cstring
 *
 * Create a palloc'd, null-terminated C string from a text value.
 *
 * We support being passed a compressed or toasted text value.
 * This is a bit bogus since such values shouldn't really be referred to as
 * "text *", but it seems useful for robustness.  If we didn't handle that
 * case here, we'd need another routine that did, anyway.
 */
char *
text_to_cstring(text *t);

/*
 * textlcpy --- exactly like strlcpy(), except source is a text value.
 *
 * Copy src to string dst of size siz.	At most siz-1 characters
 * will be copied.	Always NUL terminates (unless siz == 0).
 * Returns strlen(src); if retval >= siz, truncation occurred.
 *
 * We support being passed a compressed or toasted text value.
 * This is a bit bogus since such values shouldn't really be referred to as
 * "text *", but it seems useful for robustness.  If we didn't handle that
 * case here, we'd need another routine that did, anyway.
 */
size_t
textlcpy(char *dst, text *src, size_t siz);

#endif //PG_ICONV_TEXT_H
