#include "mm.h"
#include "spinlock.h"
#include "mach.h"
#include "cpuctrl.h"
#include "global.h"
#include "printk.h"

/**
 *mmp:�ڴ�λͼ�ṹ��mmapdsc_tָ��
 *phyadr:λͼ����ʾ�������ڴ���ʼ��ַ
 *phyadre:λͼ����ʾ�������ڴ������ַ
 *allcount:�������
 *flgs:
 */
static void mmapdsc_t_init(mmapdsc_t* mmp, unsigned int phyadr, unsigned int phyadre, 
	unsigned int allcount, unsigned int flgs)
{
    list_init(&mmp->map_list);
    hal_spinlock_init(&mmp->map_lock);
    mmp->map_phyadr = phyadr;
    mmp->map_phyadrend = phyadre;
    mmp->map_allcount = allcount;
    mmp->map_flg = flgs;
}


/**
 *adrs:�����ڴ�ռ���ʼ��ַ
 *adre:�����ڴ�ռ������ַ
 *gmmp:λͼ����ʼ��ַ
 *curmindx:��ǰ��λͼ����
 */
static unsigned int init_core_mmapdsc(unsigned int adrs,unsigned int adre,
		mmapdsc_t* gmmp, unsigned int curmindx)
{
    unsigned int mindx = curmindx;
    unsigned int tmpadr = adre;
	/*ѭ����ʼ��4M�ڴ�λͼ�Ľṹ�壬���Ҽ���λͼ����*/
    for(;adrs < adre; adrs += MAPONE_SIZE, mindx++)
    {
        if((adrs + MAPONE_SIZE) < adre)
        {
            tmpadr = (adrs + MAPONE_SIZE) - 1;
        }else
        {
            tmpadr = adre;
        }
        
        mmapdsc_t_init(&gmmp[mindx] ,adrs, tmpadr, 0,
			MAP_FLAGES_VAL(0, MAPF_ACSZ_4MB, MAPF_SZ_4MB));
    }
    return mindx;
}

void init_mmapdsc(mach_t* mahp)
{
	unsigned int i;
    phyadrspce_t* adsp = mahp->mh_spedsc;
    unsigned int spnr = mahp->mh_spedscnr;
    unsigned int mindx = 0;
    for(i = 0; i < spnr; i++)
    {
        if(adsp[i].ap_flgs == ADRSPCE_SDRAM)
        {
			/*���ҵ���SDRAM������ռ�󣬶������м���*/
            mindx = init_core_mmapdsc(adsp[i].ap_adrstart, adsp[i].ap_adrend, 
					mahp->mh_mmapdscadr, mindx);
        }
    }
    mahp->mh_mmapdscnr = mindx;
    mahp->mh_kerinramend = (unsigned int)((unsigned int)mahp->mh_mmapdscadr
			+ ((sizeof(mmapdsc_t)) * mindx));
    return;
}

/**
 *��ʼ��alcfrelst_t�ṹ���е������spinlock
 */
static void alcfrelst_t_init(alcfrelst_t* initp,size_t bsz)
{
    hal_spinlock_init(&initp->afl_lock);
    initp->afl_sz=bsz;
    list_init(&initp->afl_fulllsth);
    list_init(&initp->afl_emtplsth);
    list_init(&initp->afl_fuemlsth);
    return;
}

static void phymem_t_init(phymem_t *initp)
{
	list_init(&initp->pmm_list);
	hal_spinlock_init(&initp->pmm_lock);
	alcfrelst_t_init(&initp->pmm_sz_lsth[0], BLK128KB_SIZE);
	alcfrelst_t_init(&initp->pmm_sz_lsth[1], BLK256KB_SIZE);
	alcfrelst_t_init(&initp->pmm_sz_lsth[2], BLK512KB_SIZE);
	alcfrelst_t_init(&initp->pmm_sz_lsth[3], BLK1MB_SIZE);
	alcfrelst_t_init(&initp->pmm_sz_lsth[4], BLK2MB_SIZE);
	alcfrelst_t_init(&initp->pmm_sz_lsth[5], BLK4MB_SIZE);
}

static void mapdsc_addto_memlst(alcfrelst_t* aflp, mmapdsc_t *mapp, 
				unsigned int atflg)
{
	switch(atflg) {
        case ADDT_EMTP_FLG:
            list_add_tail(&mapp->map_list, &aflp->afl_emtplsth);
            break;
        case ADDT_FUEM_FLG:
            list_add_tail(&mapp->map_list, &aflp->afl_fuemlsth);
            break;
        case ADDT_FULL_FLG:
            list_add_tail(&mapp->map_list, &aflp->afl_fulllsth);
            break;
        default:
            break;
	}
	
}

static void pmmlist_init(mach_t* mahp, phymem_t* pmp)
{
	unsigned int mr;
    mmapdsc_t* mapp = mahp->mh_mmapdscadr;
    unsigned int mapnr = mahp->mh_mmapdscnr;
    for(mr = 1; mr < mapnr; mr++) {
        mapdsc_addto_memlst(&pmp->pmm_sz_lsth[BLKSZ_HEAD_MAX - 1], &mapp[mr], ADDT_EMTP_FLG);  
    }

}

void onmmapdsc_inkrlram(mach_t* mahp,phymem_t* pmp)
{
    mmapdsc_t* mapp = mahp->mh_mmapdscadr;
    unsigned int ker_r_s = mahp->mh_kerinramstart;
	unsigned int ker_r_e = mahp->mh_kerinramend;
    if( (ker_r_e-ker_r_s) > BLK128KB_SIZE || ker_r_s < mapp[0].map_phyadr)
    {
        hal_sysdie("onmmapdsc_inkrlram err");
    }
    unsigned int cut = 1;
    unsigned int flg = MAP_FLAGES_VAL(0, MAPF_ACSZ_128KB, MAPF_SZ_4MB);
	/*��һ��λͼ�ṹ*/
    mapp[0].map_allcount = cut;
    mapp[0].map_flg = flg;
    mapdsc_addto_memlst(&pmp->pmm_sz_lsth[0], &mapp[0], ADDT_FUEM_FLG);
    return;
}

void init_phymem()
{
	phymem_t_init(&osphymem);
	pmmlist_init(&osmach, &osphymem);
}

void init_mm()
{
    init_mmapdsc(&osmach);
    init_phymem();
    onmmapdsc_inkrlram(&osmach, &osphymem);  

    return;
}

void print_mmapdsc(mach_t *mchp)
{
    mmapdsc_t* map=mchp->mh_mmapdscadr;
    unsigned int mnr=mchp->mh_mmapdscnr;
	unsigned int i;
    for(i=0;i<mnr;i++)
    {
        printk("mmapdsc[%x].map_phyadr:%x,map_phyadrend:%x\n\r",i,(unsigned int)map[i].map_phyadr,(unsigned int)map[i].map_phyadrend);
    }
    return;
}


/**
 *�鼶�ڴ����
 **/
 
static mmapdsc_t* hal_onalfl_findmapdsc(alcfrelst_t* allclh)
{
	mmapdsc_t *mp = NULL;
	/*���ڷ��������в���λͼ������Ѿ�����ʹ�ù���λͼ��ֱ�ӷ��ظ�λͼ*/
	if(list_is_empty_careful(&allclh->afl_fuemlsth) == FALSE) {
        mp = list_entry(allclh->afl_fuemlsth.next,mmapdsc_t,map_list);
        return mp;
    }
	
	/*����ڸ�alcfrelst_t����������û��λͼ�����ڿն��в���λͼ*/
	if(list_is_empty_careful(&allclh->afl_emtplsth) == FALSE)
    {
        mp=list_entry(allclh->afl_emtplsth.next,mmapdsc_t,map_list);
        return mp;
    }
	/*�������ڿն����л�û�����õ�λͼ���򷵻ؿ�*/
    return NULL;
}
 
static unsigned int hal_onmapdsc_allcblks(unsigned int mflg, unsigned int mask,unsigned int bitls,
			alcfrelst_t* aflp,alcfrelst_t* mvtoaflp)
{
	unsigned int retadr = NULL;
	mmapdsc_t *map = NULL;
	unsigned int bi = 0;
	unsigned int fg = 0xffffff0f;
	if (bitls > 32) {
		return NULL;
	}
	/*��ָ����alcfrelst_t����mmapdsc_t*/
	map = hal_onalfl_findmapdsc(aflp);
	if (map == NULL) {
		return NULL;
	}
	
	/*ɨ��λͼ*/
	for (; bi < bitls; bi++) {
        if(((map->map_allcount>>bi)&1)==0)
        {
            goto next_step;
        }
	}
	
	bi = 0xffffffff;
next_step:
	if (bi == 0xffffffff) {
		return NULL;
	}
	
	/*�������λͼ�������ַ��ƫ����*/
    retadr = map->map_phyadr + (mvtoaflp->afl_sz*bi);
    if(retadr < map->map_phyadr || retadr >= map->map_phyadrend) {
        return NULL;
    }
	/*��λͼ����Ӧλ��1*/
    map->map_allcount |= (1<<bi);
	/*���λͼ�е���ر�־λ*/
    map->map_flg &= fg;
	/*�������ñ�־λ*/
    map->map_flg |= mflg;
    /*�þ��ʾû�п����ڴ����*/
    if((map->map_allcount & mask) == mask)
    {
        list_move_tail(&map->map_list, &mvtoaflp->afl_fulllsth);
        return retadr;
    }
	/*�����Ƶ���������*/
    list_move_tail(&map->map_list, &mvtoaflp->afl_fuemlsth);
    return retadr;
}
 
 /**
  *allclh:���ڲ���mmapdsc_t��alcfrelst_t�ṹ
  *relaclh:������mmapdsc_t�󣬽�Ҫ�����ŵ����alcfrelst_t��
  **/
static unsigned int hal_onalcfrel_allocblks(alcfrelst_t *allclh, 
			alcfrelst_t *relaclh, size_t relalcsz)
{
	unsigned int retadr = NULL;
	switch(relalcsz) {
        case BLK128KB_SIZE:
            retadr = hal_onmapdsc_allcblks(MAPF_ACSZ_128KB,BLK128KB_MASK,BLK128KB_BITL,allclh,relaclh);
            break;
        case BLK256KB_SIZE:
            retadr = hal_onmapdsc_allcblks(MAPF_ACSZ_256KB,BLK256KB_MASK,BLK256KB_BITL,allclh,relaclh);
            break;
        case BLK512KB_SIZE:
            retadr = hal_onmapdsc_allcblks(MAPF_ACSZ_512KB,BLK512KB_MASK,BLK512KB_BITL,allclh,relaclh);
            break;
        case BLK1MB_SIZE:
            retadr = hal_onmapdsc_allcblks(MAPF_ACSZ_1MB,BLK1MB_MASK,BLK1MB_BITL,allclh,relaclh);
            break;
        case BLK2MB_SIZE:
            retadr = hal_onmapdsc_allcblks(MAPF_ACSZ_2MB,BLK2MB_MASK,BLK2MB_BITL,allclh,relaclh);
            break;
        case BLK4MB_SIZE:
            retadr = hal_onmapdsc_allcblks(MAPF_ACSZ_4MB,BLK4MB_MASK,BLK4MB_BITL,allclh,relaclh);
            break;
        default:
            retadr=NULL;
            break;    
	}
	return retadr;
}
 
static alcfrelst_t *hal_onblksz_findalcfrelst(alcfrelst_t **ret_alcfrl,
			size_t * ret_alcsz, size_t size)
{
	unsigned int i;
	phymem_t *memp = &osphymem;
	alcfrelst_t *aftp = NULL;
	alcfrelst_t *aft4mbp = &memp->pmm_sz_lsth[BLKSZ_HEAD_MAX - 1];

	for (i = 0; i < BLKSZ_HEAD_MAX; i++) {
	
		if (memp->pmm_sz_lsth[i].afl_sz == size) {
			aftp = &memp->pmm_sz_lsth[i];
			*ret_alcfrl = aftp;
			*ret_alcsz = memp->pmm_sz_lsth[i].afl_sz;
			goto next_step;
		}
	}
	aftp = NULL;
	*ret_alcfrl = NULL;
	*ret_alcsz = 0;
	
next_step:
	if (aftp == NULL) {
		return NULL;
	}
	
	if (list_is_empty_careful(&aftp->afl_emtplsth) == FALSE) {
		return aftp;
	}
	
	if (list_is_empty_careful(&aftp->afl_fuemlsth) == FALSE) {
		return aftp;
	}
	
	aftp = aft4mbp;
	
	if (list_is_empty_careful(&aftp->afl_emtplsth) == FALSE) {
		return aftp;
	}
	return NULL;
}
 
static unsigned int mem_alloc_blks_core(size_t size)
{
	phymem_t *memp = &osphymem;
	size_t retbsz = 0;
	cpuflg_t cpufg;
	unsigned int retadr = NULL;
	alcfrelst_t *allcflp = NULL;
	alcfrelst_t *aftp = NULL;
	
	hal_spinlock_saveflg_cli(&memp->pmm_lock, &cpufg);
	/*1.�ȸ�������������ڴ���С��osphymem���ҵ�
	 *һ��alcfrelst_t�ṹ
	 */
	aftp = hal_onblksz_findalcfrelst(&allcflp, &retbsz, size);
	if (aftp == NULL || allcflp == NULL || retbsz == 0) {
		retadr = NULL;
		goto return_step;
	}
	
	if(aftp->afl_sz != retbsz && aftp->afl_sz != BLK4MB_SIZE) {
		retadr = NULL;
		goto return_step;
	}
	
	/*2.��alcfrelst_t�в���λͼ��Ӧ���ڴ��������*/
	retadr = hal_onalcfrel_allocblks(aftp, allcflp, retbsz);
	
return_step:
	hal_spinunlock_restflg_sti(&memp->pmm_lock, &cpufg);
	return retadr;
}

mmapdsc_t* hal_free_findmapdsc(unsigned int frebadr,alcfrelst_t* allclh)
{
	mmapdsc_t *mp = NULL;
	list_h_t *tmplst = NULL;
	
	if(list_is_empty_careful(&allclh->afl_fuemlsth) == FALSE) {
		list_for_each(tmplst, &allclh->afl_fuemlsth) {
            mp = list_entry(tmplst,mmapdsc_t,map_list);
            if(frebadr < mp->map_phyadrend && frebadr >= mp->map_phyadr) {
                return mp;
            }
        }
	}
	
	if(list_is_empty_careful(&allclh->afl_fulllsth) == FALSE) {
        list_for_each(tmplst,&allclh->afl_fulllsth) {
            mp=list_entry(tmplst,mmapdsc_t,map_list);
            if(frebadr<mp->map_phyadrend&&frebadr>=mp->map_phyadr) {
                return mp;
            }
        }
    }
	
    return NULL;
}

static bool_t hal_onmapdsc_freeblks(unsigned int frebadr, 
			unsigned int mflg, unsigned int mask, alcfrelst_t *aflp,
			alcfrelst_t *mvtoaflp)
{
	mmapdsc_t *map = NULL;
	unsigned int bitnr;
	unsigned int fg = 0xffffff0f;
	map = hal_free_findmapdsc(frebadr, aflp);
	if (map == NULL) {
		return FALSE;
	}
	
	bitnr = (unsigned int)(frebadr - map->map_phyadr) 
			/(unsigned int)(aflp->afl_sz);
	if (bitnr > 31) {
		return FALSE;
	}
	
	if(((map->map_allcount>>bitnr)&1)!=1) {
       return FALSE;
    }
	
    map->map_allcount &= (~(1<<bitnr));	
	if ((map->map_allcount & mask) == 0) {
		map->map_flg &= fg;
		map->map_flg |= mflg;
		list_move(&map->map_list, &mvtoaflp->afl_emtplsth);
	}
	return TRUE;
}

static bool_t hal_onalcfrel_freeblks(unsigned int frebadr, 
		alcfrelst_t *allclh, alcfrelst_t *mvaflh, size_t relalcsz)
{
	bool_t retstus = FALSE;
    switch(relalcsz) {
        case BLK128KB_SIZE:
            retstus=hal_onmapdsc_freeblks(frebadr,MAPF_ACSZ_4MB,BLK128KB_MASK,allclh,mvaflh);
            break;
        case BLK256KB_SIZE:
            retstus=hal_onmapdsc_freeblks(frebadr,MAPF_ACSZ_4MB,BLK256KB_MASK,allclh,mvaflh);
            break;
        case BLK512KB_SIZE:
            retstus=hal_onmapdsc_freeblks(frebadr,MAPF_ACSZ_4MB,BLK512KB_MASK,allclh,mvaflh);
            break;
        case BLK1MB_SIZE:
            retstus=hal_onmapdsc_freeblks(frebadr,MAPF_ACSZ_4MB,BLK1MB_MASK,allclh,mvaflh);
            break;
        case BLK2MB_SIZE:
            retstus=hal_onmapdsc_freeblks(frebadr,MAPF_ACSZ_4MB,BLK2MB_MASK,allclh,mvaflh);
            break;
        case BLK4MB_SIZE:
            retstus=hal_onmapdsc_freeblks(frebadr,MAPF_ACSZ_4MB,BLK4MB_MASK,allclh,mvaflh);
            break;
        default:
            retstus=FALSE;
            break;    
    
    }
    return retstus;
}

static bool_t mem_free_blks_core(unsigned int block, size_t size)
{
	unsigned int fi;
	phymem_t *memp = &osphymem;
	bool_t retstus = FALSE;
	cpuflg_t cpufg;
	alcfrelst_t *aftp = NULL;
	alcfrelst_t *mvtoaftp = &memp->pmm_sz_lsth[BLKSZ_HEAD_MAX - 1];
	
	hal_spinlock_saveflg_cli(&memp->pmm_lock, &cpufg);
	for(fi = 0; fi < BLKSZ_HEAD_MAX; fi++){
		if(memp->pmm_sz_lsth[fi].afl_sz == size) {
			aftp = &memp->pmm_sz_lsth[fi];
			goto next_step;
		}
	}
	aftp = NULL;

next_step:
	if (aftp == NULL) {
		retstus = FALSE;
		goto return_step;
	}
	retstus = hal_onalcfrel_freeblks(block, aftp, mvtoaftp, size);

return_step:
	hal_spinunlock_restflg_sti(&memp->pmm_lock, &cpufg);
	return retstus;
}

/**
 *�鼶�ڴ����ӿ�
 **/
unsigned int mem_alloc_blks(size_t size)
{
	if (size < BLK128KB_SIZE || size > BLK4MB_SIZE) {
		return NULL;
	}
	return mem_alloc_blks_core(size);
}

bool_t mem_free_blks(unsigned int block, size_t size)
{
	if (block == NULL || size < BLK128KB_SIZE 
		|| size > BLK4MB_SIZE) {
		return FALSE;
	}
	return mem_free_blks_core(block, size);
}

