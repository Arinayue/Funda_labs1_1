#ifndef STATUS_H
#define STATUS_H
typedef enum status_code{
    STATUS_OK = 0,
    STATUS_INVALID_INPUT,
    STATUS_OVERFLOW,
    STATUS_MAX_ITERATIONS
} status_code;
#endif