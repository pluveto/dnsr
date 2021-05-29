//
// Created by xqmmcqs on 2021/4/3.
//

#ifndef DNSR_UTIL_H
#define DNSR_UTIL_H

#include <stdio.h>

#include "config_jar.h"

extern FILE * log_file;

#define log_debug(args...) \
    if (LOG_MASK & 1) \
    { \
        if (log_file != stderr) \
            fprintf(log_file, "[DEBUG] %s:%d ", __FILE__, __LINE__); \
        else \
            fprintf(log_file, "\x1b[37m[DEBUG]\x1b[36m %s:%d \x1b[0m", __FILE__, __LINE__); \
        fprintf(log_file, args); \
        fprintf(log_file, "\n"); \
    }

#define log_info(args...) \
    if (LOG_MASK & 2) \
    { \
        if (log_file != stderr) \
            fprintf(log_file, "[INFO ] %s:%d ", __FILE__, __LINE__); \
        else \
            fprintf(log_file, "\x1b[34m[INFO ]\x1b[36m %s:%d \x1b[0m", __FILE__, __LINE__); \
        fprintf(log_file, args); \
        fprintf(log_file, "\n"); \
    }

#define log_error(args...) \
    if (LOG_MASK & 4) \
    { \
        if (log_file != stderr) \
            fprintf(log_file, "[ERROR] %s:%d ", __FILE__, __LINE__); \
        else \
            fprintf(log_file, "\x1b[33m[ERROR]\x1b[36m %s:%d \x1b[0m", __FILE__, __LINE__); \
        fprintf(log_file, args); \
        fprintf(log_file, "\n"); \
    }

#define log_fatal(args...) \
    if (LOG_MASK & 8) \
    { \
        if (log_file != stderr) \
            fprintf(log_file, "[FATAL] %s:%d ", __FILE__, __LINE__); \
        else \
            fprintf(log_file, "\x1b[31m[FATAL]\x1b[36m %s:%d \x1b[0m", __FILE__, __LINE__); \
        fprintf(log_file, args); \
        fprintf(log_file, "\n"); \
    }

#endif //DNSR_UTIL_H
