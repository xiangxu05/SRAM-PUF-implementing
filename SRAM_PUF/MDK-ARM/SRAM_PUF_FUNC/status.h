#ifndef STATUS_H
#define STATUS_H

// 状态枚举
typedef enum {
    STATUS_DEFAULT = 0,
    STATUS_INIT,
    STATUS_SOURCE,
    STATUS_STRONG_SRAM,
    STATUS_SOURCE_SRAM,
    STATUS_DELAY,
    STATUS_FILE,
    STATUS_STATU,
    STATUS_UNKNOWN = 255
} Status_t;

#endif // STATUS_H
