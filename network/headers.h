#ifndef HEADERS_H
#define HEADERS_H

#include <cstdint>

enum class MessageType : int32_t
{
    FullState = 1,
    Update    = 2
};

#endif // HEADERS_H
