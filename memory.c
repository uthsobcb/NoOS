/*
 * Memory Management for NoOS
 *
 * This module provides basic memory management functionality:
 * - Physical memory frame allocation using bitmap
 * - Simple heap allocator for kernel memory
 * - Memory information display
 */

#include "types.h"
#include "memory.h"

extern void printf(const char *format, ...);

static physical_memory_t pmm;

static uint8_t *heap_start;
static uint8_t *heap_end;
static uint8_t *heap_current;

void init_memory_manager(uint32_t memory_size)
{
    pmm.memory_size = memory_size;
    pmm.total_frames = memory_size / FRAME_SIZE;
    pmm.used_frames = 0;

    pmm.bitmap = (uint32_t *)0x200000;

    for (uint32_t i = 0; i < (pmm.total_frames / 32) + 1; i++)
    {
        pmm.bitmap[i] = 0;
    }

    uint32_t kernel_frames = (2 * 1024 * 1024) / FRAME_SIZE;
    for (uint32_t i = 0; i < kernel_frames; i++)
    {
        uint32_t byte = i / 32;
        uint32_t bit = i % 32;
        pmm.bitmap[byte] |= (1 << bit);
        pmm.used_frames++;
    }

    heap_start = (uint8_t *)0x300000;
    heap_end = (uint8_t *)0x800000;
    heap_current = heap_start;

    printf("Memory manager initialized: %d KB total, %d KB free\n",
           memory_size / 1024, get_free_memory() / 1024);
}

void *allocate_frame(void)
{
    for (uint32_t i = 0; i < pmm.total_frames; i++)
    {
        uint32_t byte = i / 32;
        uint32_t bit = i % 32;

        if (!(pmm.bitmap[byte] & (1 << bit)))
        {
            pmm.bitmap[byte] |= (1 << bit);
            pmm.used_frames++;
            return (void *)(i * FRAME_SIZE);
        }
    }
    return NULL; // Out of memory
}

void free_frame(void *frame)
{
    uint32_t frame_num = (uint32_t)frame / FRAME_SIZE;
    uint32_t byte = frame_num / 32;
    uint32_t bit = frame_num % 32;

    pmm.bitmap[byte] &= ~(1 << bit);
    pmm.used_frames--;
}

/*
 * Display current memory usage information
 */
void show_memory_info(void)
{
    uint32_t free_mem = get_free_memory();
    printf("Memory Info:\n");
    printf("  Total: %d KB\n", pmm.memory_size / 1024);
    printf("  Used:  %d KB\n", (pmm.memory_size - free_mem) / 1024);
    printf("  Free:  %d KB\n", free_mem / 1024);
    printf("  Frames: %d total, %d used\n", pmm.total_frames, pmm.used_frames);
}

uint32_t get_free_memory(void)
{
    return (pmm.total_frames - pmm.used_frames) * FRAME_SIZE;
}

void *kmalloc(size_t size)
{
    if (heap_current + size >= heap_end)
    {
        return NULL;
    }

    // Allocate memory at current position
    void *ptr = heap_current;
    heap_current += size;

    heap_current = (uint8_t *)(((uint32_t)heap_current + 3) & ~3);

    return ptr;
}

void kfree(void *ptr)
{
    // Simple bump allocator doesn't support free operation
    // This is just a placeholder for API compatibility
    (void)ptr;
}
