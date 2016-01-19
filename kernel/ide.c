/**
 * Simple IDE read/write routines using PIO mode.
 * This code is very CPU intensive and not efficient
 * (if that's what you're after, use DMA mode instead).
 * Reference: http://wiki.osdev.org/ATA_PIO_Mode
 */

#include "ide.h"
#include "periph.h"

/**
 * Wait for the disk drive to be ready.
 */
static void wait_drive() {
    while((inb(0x1F7) & 0xC0) != 0x40);
}

/**
 * Prepare the disk drive for read/write at the specified sector in LBA mode.
 * @param sector the sector to read or write
 */
static void pio_prepare(int sector) {
    wait_drive();
    outb(0x1f2, 1);  // 1 sector
    outb(0x1f3, sector & 0xff);  // send bits 0-7 of LBA
    outb(0x1f4, (sector >> 8) & 0xff);  // send bits 8-15 of LBA
    outb(0x1f5, (sector >> 16) & 0xff);  // send bits 16-23 of LBA
    outb(0x1f6, ((sector >> 24) & 0x0f) | 0xe0);  // send bits 24-27 of LBA + set LBA mode; 0xe0 = 11100000b;
}

/**
 * Read sectors from the first disk.
 * @param first sector to read (0-indexed)
 * @param dst address to store to read data
 * Based on the assembly code at http://wiki.osdev.org/ATA_read/write_sectors
 */
void read_sector(int sector, void *dst) {
    pio_prepare(sector);

    outb(0x1f7, 0x20);  // command port: read with retry
    wait_drive();

    uint16_t *data = (uint16_t *)dst;
    for (int i = 0; i < SECTOR_SIZE/2; i++) {
        *data = inw(0x1f0);
        data++;
    }
}

/**
 * Write sectors from the first disk.
 * @param first sector to write (0-indexed)
 * @param src address of the data to be written
 */
void write_sector(int sector, void *src) {
    pio_prepare(sector);

    outb(0x1f7, 0x30);  // command port: write with retry
    wait_drive();

    uint16_t *data = (uint16_t *)src;
    for (int i = 0; i < SECTOR_SIZE/2; i++) {
        outw(0x1f0, *data);
        data++;
    }
}
