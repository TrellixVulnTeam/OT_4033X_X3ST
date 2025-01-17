
#include "typedefs.h"
#include "platform.h"
#include "blkdev.h"
#include "part.h"
#include "gfh.h"

#if CFG_PMT_SUPPORT
#include "pmt.h"
#endif

#define MOD "PART"

#define TO_BLKS_ALIGN(size, blksz)  (((size) + (blksz) - 1) / (blksz))

typedef union {
    part_hdr_t      part_hdr;
    gfh_file_info_t file_info_hdr;
} img_hdr_t;

int part_init(void)
{
    blkdev_t *bdev;
    part_t *part;
    u32 erasesz;
    unsigned long lastblk;

    #ifdef PL_PROFILING
    u32 profiling_time;
    profiling_time = 0;
    #endif

    cust_part_init();

    bdev = blkdev_get(CFG_BOOT_DEV);
    part = cust_part_tbl();

    if (!bdev || !part)
        return -1;

    erasesz = bdev->blksz;

    part->blks = TO_BLKS_ALIGN(part->size, erasesz);
    lastblk    = part->startblk + part->blks;

    while (1) {
        part++;
        if (!part->name)
            break;
        if (part->startblk == 0)
            part->startblk = lastblk;
        part->blks = TO_BLKS_ALIGN(part->size, erasesz);
        lastblk = part->startblk + part->blks;
    }
#if CFG_PMT_SUPPORT
    #ifdef PL_PROFILING
    profiling_time = get_timer(0);
    #endif
    pmt_init();

    #ifdef PL_PROFILING
    printf("#T#pmt_init=%d\n", get_timer(profiling_time));
    #endif
#endif
    return 0;
}

part_t *part_get(char *name)
{
    int index = 0;
    part_t *part = cust_part_tbl();

    while (part->name && index < PART_MAX_COUNT) {
        if (!strcmp(name, part->name)) {
        #if CFG_PMT_SUPPORT
            return pmt_get_part(part, index);
        #else
            return part;
        #endif
        }
        part++; index++;
    }
    return NULL;
}

int part_load(blkdev_t *bdev, part_t *part, u32 *addr, u32 offset, u32 size)
{
    int ret;
#ifdef DUMMY_AP_BOOTLOADER
    static img_hdr_t *hdr = (img_hdr_t*)(BOOT_ARGUMENT_ADDR + BOOT_ARGUMENT_MAX_SIZE);
#else
    img_hdr_t *hdr = (img_hdr_t*)COMMON_BUFFER_ADDR;
#endif
    part_hdr_t *part_hdr = &hdr->part_hdr;
    gfh_file_info_t *file_info_hdr = &hdr->file_info_hdr;

    /* specify the read offset */
    u64 src = part->startblk * bdev->blksz + offset;
    u32 dsize = 0, maddr = 0;
    u32 ms;

    /* retrieve partition header. */
    if (blkdev_read(bdev, src, sizeof(img_hdr_t), (u8*)hdr) != 0) {
        print("[%s] bdev(%d) read error (%s)\n", MOD, bdev->type, part->name);
        return -1;
    }

    if (part_hdr->info.magic == PART_MAGIC) {

        /* load image with partition header */
        part_hdr->info.name[31] = '\0';

        print("[%s] Image with part header\n", MOD);
        print("[%s] name : %s\n", MOD, part_hdr->info.name);
        print("[%s] addr : %xh\n", MOD, part_hdr->info.maddr);
        print("[%s] size : %d\n", MOD, part_hdr->info.dsize);
        print("[%s] magic: %xh\n", MOD, part_hdr->info.magic);

        maddr = part_hdr->info.maddr;
        dsize = part_hdr->info.dsize;
        src += sizeof(part_hdr_t);
#ifdef DUMMY_AP_BOOTLOADER
        print("[%s] hdr at 0x%x, 0x%x\n", MOD, hdr);
        //step forward 1 of sizeof(part_hdr_t)
        hdr = hdr + 1;
#endif

#if 0
    } else if ((file_info_hdr->m_gfh_hdr.m_type == GFH_FILE_INFO) &&
        (GFH_GET_MAGIC(file_info_hdr->m_gfh_hdr.m_magic_ver) == GFH_HDR_MAGIC)) {

        /* load image with GFH header */
        print("[%s] Image with GFH header\n", MOD);
        print("[%s] name : %s\n", MOD, file_info_hdr->m_id);
        print("[%s] addr : %xh\n", MOD, file_info_hdr->m_load_addr);
        print("[%s] size : %d\n", MOD, file_info_hdr->m_file_len);
        print("[%s] magic: %xh\n", MOD, file_info_hdr->m_gfh_hdr.m_magic_ver);

        maddr = file_info_hdr->m_load_addr;
        dsize = file_info_hdr->m_file_len;
        src += file_info_hdr->m_content_offset;
#endif
    } else {
#ifdef SLT_BOOTLOADER
        /* load full partition */
        maddr = *addr;
        dsize = part->size;

        print("[%s] Image without header\n", MOD);
        print("[%s] name : %s\n", MOD, part->name);
        print("[%s] addr : %xh\n", MOD, maddr);
        print("[%s] size : %d\n", MOD, part->size);
        print("[%s] magic: %xh\n", MOD, 0xFFFFFFFF);
#else
        print("[%s] <ASSERT> %s image doesn't exist\n", MOD, part->name);
        return -1;
#endif
    }

    if (*addr != PART_HEADER_MEMADDR)
        maddr = *addr;

    ms = get_timer(0);
    if (0 == (ret = blkdev_read(bdev, src, dsize, (u8*)maddr)))
        *addr = maddr;
    ms = get_timer(ms);

    print("\n[%s] load \"%s\" from 0x%llx (dev) to 0x%x (mem) [%s]\n", MOD,
        part->name, src, maddr, (ret == 0) ? "SUCCESS" : "FAILED");
    print("[%s] load speed: %dKB/s, %d bytes, %dms\n", MOD,
        ((dsize / ms) * 1000) / 1024, dsize, ms);

    return ret;
}

void part_dump(void)
{
    blkdev_t *bdev;
    part_t *part;
    u32 blksz;
    u64 start, end;

    bdev = blkdev_get(CFG_BOOT_DEV);
    part = cust_part_tbl();
    blksz = bdev->blksz;

    print("\n[%s] blksz: %dB\n", MOD, blksz);
    while (part->name) {
        start = (u64)part->startblk * blksz;
        end = (u64)(part->startblk + part->blks) * blksz - 1;
        print("[%s] [0x%llx-0x%llx] \"%s\" (%d blocks) \n", MOD, start, end,
            part->name, part->blks);
        part++;
    }
}
