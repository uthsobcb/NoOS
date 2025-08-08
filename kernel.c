#include "types.h"
#include "memory.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

static uint16_t *vga_buffer = (uint16_t *)VGA_MEMORY;
static size_t vga_row = 0;
static size_t vga_column = 0;
static uint8_t vga_color = 15;

void clear_screen(void);
void putchar(char c);
void print(const char *str);
void printf(const char *format, ...);
void print_number(uint32_t num);
void print_hex(uint32_t num);
void demonstrate_memory_operations(void);

void clear_screen(void)
{
    for (size_t y = 0; y < VGA_HEIGHT; y++)
    {
        for (size_t x = 0; x < VGA_WIDTH; x++)
        {
            const size_t index = y * VGA_WIDTH + x;
            vga_buffer[index] = ((uint16_t)vga_color << 8) | ' ';
        }
    }
    vga_row = 0;
    vga_column = 0;
}
void putchar(char c)
{
    if (c == '\n')
    {
        vga_column = 0;
        if (++vga_row == VGA_HEIGHT)
        {
            vga_row = VGA_HEIGHT - 1;
        }
    }
    else
    {
        const size_t index = vga_row * VGA_WIDTH + vga_column;
        vga_buffer[index] = ((uint16_t)vga_color << 8) | c;

        if (++vga_column == VGA_WIDTH)
        {
            vga_column = 0;
            if (++vga_row == VGA_HEIGHT)
            {
                vga_row = VGA_HEIGHT - 1;
            }
        }
    }
}

void print(const char *str)
{
    while (*str)
    {
        putchar(*str++);
    }
}

void print_number(uint32_t num)
{
    if (num == 0)
    {
        putchar('0');
        return;
    }

    char buffer[12];
    int i = 0;

    while (num > 0)
    {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }

    while (--i >= 0)
    {
        putchar(buffer[i]);
    }
}

void print_hex(uint32_t num)
{
    char hex_chars[] = "0123456789ABCDEF";
    char buffer[9];
    int i = 0;

    if (num == 0)
    {
        putchar('0');
        return;
    }

    while (num > 0)
    {
        buffer[i++] = hex_chars[num % 16];
        num /= 16;
    }
    while (--i >= 0)
    {
        putchar(buffer[i]);
    }
}

void printf(const char *format, ...)
{
    char *ptr = (char *)format;
    uint32_t *args = (uint32_t *)(&format + 1); // Get arguments after format string
    int arg_index = 0;

    while (*ptr)
    {
        if (*ptr == '%' && *(ptr + 1) == 'd')
        {
            // Handle %d format specifier - print integer
            print_number(args[arg_index++]);
            ptr += 2; // Skip %d
        }
        else
        {
            // Regular character - just print it
            putchar(*ptr);
            ptr++;
        }
    }
}
void demonstrate_memory_operations(void)
{
    print("\n=== Memory Operations Demo ===\n");

    print("1. Allocating memory blocks...\n");

    void *ptr1 = kmalloc(1024);
    void *ptr2 = kmalloc(2048);
    void *ptr3 = kmalloc(4096);

    print("   Block 1 (1KB) at address: 0x");
    print_hex((uint32_t)ptr1);
    print("\n");

    print("   Block 2 (2KB) at address: 0x");
    print_hex((uint32_t)ptr2);
    print("\n");

    print("   Block 3 (4KB) at address: 0x");
    print_hex((uint32_t)ptr3);
    print("\n");

    show_memory_info();

    print("\n2. Writing to allocated memory...\n");

    if (ptr1)
    {
        char *data1 = (char *)ptr1;
        data1[0] = 'H';
        data1[1] = 'i';
        data1[2] = '!';
        data1[3] = '\0';
        print("   Wrote 'Hi!' to block 1\n");
    }

    if (ptr2)
    {
        uint32_t *data2 = (uint32_t *)ptr2;
        data2[0] = 0xDEADBEEF; // Common debug value
        data2[1] = 0xCAFEBABE; // Another common debug value
        print("   Wrote magic numbers to block 2\n");
    }

    print("\n3. Reading from memory...\n");
    if (ptr1)
    {
        char *data1 = (char *)ptr1;
        print("   Block 1 contains: ");
        print(data1);
        print("\n");
    }

    if (ptr2)
    {
        uint32_t *data2 = (uint32_t *)ptr2;
        print("   Block 2[0] = 0x");
        print_hex(data2[0]);
        print("\n   Block 2[1] = 0x");
        print_hex(data2[1]);
        print("\n");
    }

    print("\n4. Freeing memory blocks...\n");

    if (ptr1)
    {
        kfree(ptr1);
        print("   Freed block 1\n");
    }

    if (ptr2)
    {
        kfree(ptr2);
        print("   Freed block 2\n");
    }

    if (ptr3)
    {
        kfree(ptr3);
        print("   Freed block 3\n");
    }

    print("\n5. Final memory status:\n");
    show_memory_info();

    print("\n=== Memory Demo Complete ===\n");
}

void kernel_main(void)
{
    clear_screen();

    print("====================\n");
    print("  Welcome to NO OS!\n\n");
    print("====================\n");
    print("This is NO OS that has no work! That does nothing currently but simulate-\n");
    print("- Display text on screen\n");
    print("- Handle basic memory management\n");
    print("\n");

    print("Initializing memory manager...\n");
    init_memory_manager(32 * 1024 * 1024);
    print("Memory manager initialized successfully!\n\n");

    demonstrate_memory_operations();

    print("\nSimulation of Memory management!!\n");
    print("Debug: NoOS is running successfully! The system is alive and operational.\n");
    while (1)
    {
        asm volatile("hlt");
    }
}
