typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

#define VGA ((volatile u16*)0xB8000)
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define COLOR 0x0F

static int row = 0;
static int col = 0;

static void put_char(char c) {
    if (c == '\n') { row++; col = 0; return; }
    if (row >= VGA_HEIGHT) return;
    VGA[row * VGA_WIDTH + col] = (u16)((COLOR << 8) | (u8)c);
    col++;
    if (col >= VGA_WIDTH) { col = 0; row++; }
}

static void print(const char *s) {
    while (*s) put_char(*s++);
}

static void clear_screen(void) {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; ++i) VGA[i] = (u16)((COLOR << 8) | ' ');
    row = 0;
    col = 0;
}

static void print_hex(u32 v) {
    const char *hex = "0123456789ABCDEF";
    print("0x");
    for (int i = 7; i >= 0; --i) put_char(hex[(v >> (i * 4)) & 0xF]);
}

void kernel_main(void) {
    clear_screen();
    print("TinyOS C kernel is running.\n");
    print("Mode: 32-bit protected mode\n");
    print("VGA text output: OK\n");
    print("Kernel loaded at: ");
    print_hex(0x00010000u);
    print("\n\n");
    print("This is a minimal modern educational boot path:\n");
    print("BIOS -> boot sector -> protected mode -> C kernel\n");
    print("\nNext upgrades: IDT, keyboard input, memory map, paging.\n");
    for (;;) {
        __asm__ __volatile__("hlt");
    }
}
