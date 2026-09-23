/* Minimal Keil RL-ARM network type definitions required by FCARM output.
 * The project embeds the generated file table but does not use the network
 * stack itself, so pulling in the full middleware header is unnecessary. */
#ifndef NET_CONFIG_H
#define NET_CONFIG_H

#include <stdint.h>

typedef uint8_t  U8;
typedef uint32_t U32;

typedef struct {
    U32            id;
    const U8      *data;
} HTTP_FILE;

#endif /* NET_CONFIG_H */
