#ifndef STATUS_H
#define STATUS_H

typedef enum
{
    SUCCESS = 0,
    ERROR_INVALID_ARGUMENTS,
    ERROR_MEMORY_ALLOCATION,
    ERROR_OVERFLOW,
    ERROR_NO_RESULT
} status;

#endif