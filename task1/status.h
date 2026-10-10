#ifndef STATUS_H
#define STATUS_H

typedef enum status_code {
    STATUS_OK = 0,
    STATUS_INVALID_INPUT,  
    STATUS_INVALID_RANGE,   
    STATUS_OVERFLOW,        
    STATUS_MEMORY_ERROR,
    STATUS_NOT_FOUND
} status_code;

#endif 