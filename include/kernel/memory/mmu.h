#ifndef __MMU_H__
#define __MMU_H__

void init_mmu(uint32_t mem_start, uint32_t mem_end, uint32_t kernel_end);

extern void invalidate_data_cache(void) __attribute__((optimize(3)));
extern void clean_data_cache(void) __attribute__((optimize(3)));

/* The below is not exactly efficient, but it's very descriptive */
typedef struct
{
    uint32_t type : 2;
    uint32_t sbz_1 : 1;
    uint32_t ns : 1;
    uint32_t sbz_2 : 1;
    uint32_t domain : 4;
    uint32_t imp : 1;
    uint32_t page_address : 22;
} page_table_t;

typedef struct
{
    uint32_t type : 2;    // Error (b00) Table (b01) / (Super)Section(b10) / Reserved Type (b11)
    uint32_t b : 1;       // Bufferable
    uint32_t c : 1;       // Cachable
    uint32_t xn : 1;      // Never Execute
    uint32_t domain : 4;  // Domain
    uint32_t imp : 1;     // Implementation Defined
    uint32_t ap : 2;      // Access Permissions
    uint32_t tex : 3;     // Region Attributes
    uint32_t apx : 1;     // Access Permissions Extended
    uint32_t s : 1;       // Sharable
    uint32_t ng : 1;      // Not Global
    uint32_t section : 1; // 0 Section / 1 Supersection
    uint32_t ns : 1;      // Not secure
    uint32_t base_address : 12;
} page_section_t;

typedef struct
{
    uint32_t type : 2;      // Error (b00) Table (b01) / (Super)Section(b10) / Reserved Type (b11)
    uint32_t b : 1;         // Bufferable
    uint32_t c : 1;         // Cachable
    uint32_t xn : 1;        // Never Execute
    uint32_t ex_base_1 : 4; // Extended Base [36:39]
    uint32_t imp : 1;       // Implementation Defined
    uint32_t ap : 2;        // Access Permissions
    uint32_t tex : 3;       // Region Attributes
    uint32_t apx : 1;       // Access Permissions Extended
    uint32_t s : 1;         // Sharable
    uint32_t ng : 1;        // Not Global
    uint32_t section : 1;   // 0 Section / 1 Supersection
    uint32_t ns : 1;        // Not secure
    uint32_t ex_base_2 : 4; // Extended Base [32:35]
    uint32_t base_address : 8;
} page_supersection_t;

typedef union
{
    uint32_t v;
    page_table_t t;
    page_section_t s;
    page_supersection_t ss;
} psu;

#define SECTION_KERNEL_T        \
    (psu)                       \
    {                           \
        .s = {.type = 0b10,     \
              .b = 0b1,         \
              .c = 0b1,         \
              .xn = 0b0,        \
              .domain = 0b0000, \
              .imp = 0b0,       \
              .ap = 0b01,       \
              .tex = 0b000,     \
              .apx = 0b0,       \
              .s = 0b1,         \
              .ng = 0b0,        \
              .section = 0b0,   \
              .ns = 0b1,        \
        }                       \
    }
#define SECTION_KERNEL (SECTION_KERNEL_T).v

#define SECTION_RAM_T           \
    (psu)                       \
    {                           \
        .s = {.type = 0b10,     \
              .b = 0b1,         \
              .c = 0b0,         \
              .xn = 0b1,        \
              .domain = 0b0000, \
              .imp = 0b0,       \
              .ap = 0b01,       \
              .tex = 0b000,     \
              .apx = 0b0,       \
              .s = 0b1,         \
              .ng = 0b0,        \
              .section = 0b0,   \
              .ns = 0b1,        \
        }                       \
    }
#define SECTION_RAM (SECTION_RAM_T).v

#define SECTION_PERIPH_T        \
    (psu)                       \
    {                           \
        .s = {.type = 0b10,     \
              .b = 0b0,         \
              .c = 0b0,         \
              .xn = 0b1,        \
              .domain = 0b0000, \
              .imp = 0b0,       \
              .ap = 0b01,       \
              .tex = 0b000,     \
              .apx = 0b0,       \
              .s = 0b0,         \
              .ng = 0b0,        \
              .section = 0b0,   \
              .ns = 0b1,        \
        }                       \
    }
#define SECTION_PERIPH (SECTION_PERIPH_T).v

#endif