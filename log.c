//
// Created by lbezborodov on 28.03.17.
//

#include "log.h"

void info(char* file, char* format, ...) {
    va_list args;
    va_start(args, format);
    FILE *fp = fopen(file, "a");
    vfprintf(fp, format, args);
    fclose(fp);
    va_end(args);
}
