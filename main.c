//
// Created by lbezborodov on 27.03.17.
//

#include "iconverter.h"

int main() {
    char* str = "Привет, мир!";
    char* to_buf;

    iconvert_("UTF8", "CP1251", str, &to_buf);
    printf("Result: %s\n", to_buf);

    info("/Users/lbezborodov/pg_iconv.log", "\n");

    free(to_buf);
    return 0;
}