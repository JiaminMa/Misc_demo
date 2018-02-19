#ifndef _MM_H_
#define _MM_H_

#include "list.h"
#include "spinlock.h"
#include "types.h"

#define BLKSZ_HEAD_MAX 6

#define DEV_TYPE_NOT 0xffffffff
#define ADRSPCE_NOT 0
#define ADRSPCE_IO 1
#define ADRSPCE_SDRAM 2
#define ADRSPCE_RAM 3
#define ADRSPCE_ROM 4
#define ADRSPCE_NORFLASH 5
#define ADRSPCE_NANDFLASH 6
#define PLFM_ADRSPCE_NR 29

#define MAPF_SZ_BIT 0
#define MAPONE_SIZE 0x400000
#define MAP_FLAGES_VAL(RV,SALLOCSZ,MSZ) (RV|SALLOCSZ|MSZ)
#define MAPF_ACSZ_4MB (6<<0)
#define MAPF_SZ_4MB (8<<0)

#define BLK128KB_SIZE (0x20000)
#define BLK256KB_SIZE (0x40000)
#define BLK512KB_SIZE (0x80000)
#define BLK1MB_SIZE (0x100000)
#define BLK2MB_SIZE (0x200000)
#define BLK4MB_SIZE (0x400000)

#define ADDT_EMTP_FLG 1
#define ADDT_FUEM_FLG 2
#define ADDT_FULL_FLG 3

#define MAPF_ACSZ_128KB (1<<0)
#define MAPF_ACSZ_256KB (2<<0)
#define MAPF_ACSZ_512KB (3<<0)
#define MAPF_ACSZ_1MB (4<<0)
#define MAPF_ACSZ_2MB (5<<0)
#define MAPF_ACSZ_4MB (6<<0)

#define BLK128KB_BITL (32)
#define BLK256KB_BITL (16)
#define BLK512KB_BITL (8)
#define BLK1MB_BITL (4)
#define BLK2MB_BITL (2)
#define BLK4MB_BITL (1)

#define BLK128KB_MASK (0xffffffff)
#define BLK256KB_MASK (0xffff)
#define BLK512KB_MASK (0xff)
#define BLK1MB_MASK (0xf)
#define BLK2MB_MASK (0x3)
#define BLK4MB_MASK (0x1)

typedef struct s_PHYADRSPCE {
	unsigned int ap_flgs;
	unsigned int ap_devtype;
	unsigned int ap_adrstart;
	unsigned int ap_adrend;
}phyadrspce_t;

/*描述内存块*/
typedef struct s_MMAPDSC {
	
	list_h_t map_list;
	spinlock_t map_lock;
	unsigned int map_phyadr;
	unsigned int map_phyadrend;
	unsigned int map_allcount;
	unsigned int map_flg;
		
}mmapdsc_t;

/*Alloc free list*/
typedef struct s_ALCFRELIST {
	spinlock_t afl_lock;
	size_t afl_sz;
	/*同类已经分配完的链表*/
	list_h_t afl_fulllsth;
	/*同类已经未分配的链表*/
	list_h_t afl_emtplsth;
	/*同类已经分配一部分的链表*/
	list_h_t afl_fuemlsth;
}alcfrelst_t;

typedef struct s_PHYMEM {
	list_h_t pmm_list;
	spinlock_t pmm_lock;
	unsigned int freeblks;
	unsigned int allcblks;
	alcfrelst_t pmm_sz_lsth[BLKSZ_HEAD_MAX];
}phymem_t;


void init_phymem(void);
void init_mm(void);
unsigned int mem_alloc_blks(size_t size);
bool_t mem_free_blks(unsigned int block, size_t size);
#endif

