//
// Created by lbezborodov on 28.03.17.
//

#include "text.h"

/*
 * cstring_to_text
 *
 * Create a text value from a null-terminated C string.
 *
 * The new text value is freshly palloc'd with a full-size VARHDR.
 */
text *
cstring_to_text(char *s)
{
    return cstring_to_text_with_len(s, strlen(s));
}

/*
 * cstring_to_text_with_len
 *
 * Same as cstring_to_text except the caller specifies the string length;
 * the string need not be null_terminated.
 */
text *
cstring_to_text_with_len(char *s, int len)
{
    text	   *result = (text *) palloc(len + VARHDRSZ);

    SET_VARSIZE(result, len + VARHDRSZ);
    memcpy(VARDATA(result), s, len);

    return result;
}

/*
 * ucstring_to_text_with_len
 *
 * Same as cstring_to_text except the caller specifies the string length;
 * the string need not be null_terminated.
 */
text *
ucstring_to_text_with_len(unsigned char *s, int len)
{
    text	   *result = (text *) palloc(len + VARHDRSZ);

    SET_VARSIZE(result, len + VARHDRSZ);
    memcpy(VARDATA(result), s, len);

    return result;
}

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
text_to_cstring(text *t)
{
    char	   *result;
    text	   *tunpacked = pg_detoast_datum_packed((struct varlena *) t);
    int			len = VARSIZE_ANY_EXHDR(tunpacked);

    result = (char *) palloc(len + 1);
    memcpy(result, VARDATA_ANY(tunpacked), len);
    result[len] = '\0';

    if (tunpacked != t)
        pfree(tunpacked);

    return result;
}

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
textlcpy(char *dst, text *src, size_t siz)
{
    text	   *srcunpacked = pg_detoast_datum_packed((struct varlena *) src);
    size_t		srclen = VARSIZE_ANY_EXHDR(srcunpacked);

    if (siz > 0)
    {
        siz--;
        if (siz >= srclen)
            siz = srclen;
        else					/* ensure truncation is encoding-safe */
            siz = pg_mbcliplen(VARDATA_ANY(srcunpacked), srclen, siz);
        memcpy(dst, VARDATA_ANY(srcunpacked), siz);
        dst[siz] = '\0';
    }

    if (srcunpacked != src)
        pfree(srcunpacked);

    return srclen;
}
