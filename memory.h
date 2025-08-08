
#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"

#define FRAME_SIZE 4096

typedef struct
{
    uint32_t *bitmap;
    uint32_t total_frames;
    uint32_t used_frames;
    uint32_t memory_size;
} physical_memory_t;

void init_memory_manager(uint32_t memory_size);

void *allocate_frame(void);
void free_frame(void *frame);

void show_memory_info(void);
uint32_t get_free_memory(void);

void *kmalloc(size_t size);
void kfree(void *ptr);

#endif
