#include "krlpagempol.h"
#include "krlglobal.h"
#include "mm.h"
#include "cpuctrl.h"
#include "printk.h"
#include "krlmm.h"
static unsigned int kmempool_pages_new(size_t msize);
void mplhead_t_init(mplhead_t *initp);
/*****************************************
 *���亯��
 *****************************************/
/******************************************
 *�ֽڷ���
 *1.�����ڴ�ռ��С��kmempool_t�в����ڴ��
 *2.���û���ҵ������½�һ���ڴ�
 *3.�����ڴ�ؽṹ
 *****************************************/
 /**
  *1.�����ڴ�ռ��С��kmempool_t�в����ڴ��
  */
static mplhead_t *objsnew_mplhead_isok(mplhead_t *mhp, size_t msize)
{
    if(mhp->mh_firtfreadr == NULL)
    {
        return NULL;
    }
    if(mhp->mh_hedty != MPLHTY_OBJS)
    {
        return NULL;
    }
    if(mhp->mh_aliobsz != msize)
    {
        return NULL;
    }
    if(mhp->mh_afindx >= mhp->mh_objnr)
    {
        return NULL;
    }
    return mhp;
} 

static mplhead_t *kmemplob_retn_mplhead(kmempool_t *kmplockp, 
				size_t msize)
{
	mplhead_t *retmhp;
	list_h_t *list;
	
	if (kmplockp->mp_obmplmhcach != NULL) {
		retmhp = kmplockp->mp_obmplmhcach;
		retmhp = objsnew_mplhead_isok(retmhp, msize);
		if (retmhp != NULL) {
			return retmhp;
		}
	}
	
	list_for_each(list, &kmplockp->mp_obmplmheadl) {
		retmhp = list_entry(list, mplhead_t, mh_list);
		retmhp = objsnew_mplhead_isok(retmhp, msize);
		if (retmhp != NULL) {
			kmplockp->mp_obmplmhcach = retmhp;
			return retmhp;
		}
	}
	
	return NULL;
}
/**
 *2.���û���ҵ������½�һ���ڴ��
 */
static mplhead_t *objs_mpool_init(kmempool_t *kmplockp, mplhead_t *initp,
				size_t msize, adr_t start, adr_t end)
{
	if(((start&0xfff) != 0) || ((end - start) < (PAGE_SIZE - 1))) {
        return NULL;
    }
	
	adr_t sadr = start + sizeof(mplhead_t);
	if((sadr & 3) != 0){
        hal_sysdie("objs_mpool_init:sadr not align dword");
        return NULL;
    }
	unsigned int objnr = 0;
	mplhead_t_init(initp);
	initp->mh_hedty = MPLHTY_OBJS;
	initp->mh_start = start;
	initp->mh_end = end;
	initp->mh_firtfreadr = sadr;
	initp->mh_aliobsz = msize;
	initp->mh_objsz = msize;
	initp->mh_nxtpsz = msize + sizeof(adr_t);
	adr_t *nexp = NULL;
	adr_t adrsz = msize;
	adr_t npsz = initp->mh_nxtpsz;
	
	if ((initp->mh_nxtpsz & 3) != 0) {
        hal_sysdie("objs_mpool_init:initp->mh_nxtpsz not align dword");
        return NULL;		
	}
	
	for(;;) {
		if ((sadr + npsz - 1) > end) {
			break;
		}
		nexp = (adr_t *)(sadr + adrsz);
		*nexp = (sadr + npsz);
		sadr += npsz;
		objnr++;
	}
	
	if (objnr == 0) {
        hal_sysdie("objs_mpool_init:objnr err");
        return NULL;		
	}
	sadr -= npsz;
	nexp = (adr_t *)(sadr + adrsz);
	*nexp = NULL;
	initp->mh_objnr = objnr;
	list_add(&initp->mh_list, &kmplockp->mp_obmplmheadl);
	return initp;
}

static mplhead_t * new_objs_mpool(kmempool_t *kmplockp, size_t msize)
{	
	mplhead_t *mphdp = NULL;
	adr_t pgadr = kmempool_pages_new(0x4000);
	if (pgadr == NULL) {
		return NULL;
	}
	mphdp = (mplhead_t *)pgadr;
	mphdp = objs_mpool_init(kmplockp, mphdp, msize, pgadr, (pgadr + 0x4000 - 1));
	if (mphdp == NULL) {
		printk("new objs mpool err");
		hal_sysdie("");
	}
	return mphdp;
}

/**
 *3.�����ڴ�ؽṹ
 */
static adr_t objs_new_on_mplhead(mplhead_t *mplhdp)
{
    if(mplhdp->mh_afindx>=mplhdp->mh_objnr||mplhdp->mh_firtfreadr==NULL) {
        return NULL;
    }
	adr_t retadr = NULL;
	adr_t *nextp = NULL;
	adr_t adrsz = (adr_t)(mplhdp->mh_aliobsz);
	retadr = mplhdp->mh_firtfreadr;
	nextp = (adr_t *)(retadr + adrsz);
	mplhdp->mh_firtfreadr = *nextp;
	*nextp = NULL;
	mplhdp->mh_afindx++;
	return retadr;
}

static adr_t kmempool_objsz_core_new(size_t msize)
{
    adr_t retadr=NULL;
    cpuflg_t cpufg;
    mplhead_t* mplhdp;
    kmempool_t* kmplp = &oskmempool;
    
	hal_spinlock_saveflg_cli(&kmplp->mp_oblock,&cpufg);
	mplhdp = kmemplob_retn_mplhead(kmplp, msize);
	if (mplhdp == NULL) {
		mplhdp = new_objs_mpool(kmplp, msize);
		if (mplhdp == NULL) {
			retadr = NULL;
			goto return_step;
		}
	}
	retadr = objs_new_on_mplhead(mplhdp);
return_step:
    hal_spinunlock_restflg_sti(&kmplp->mp_oblock,&cpufg);
    return retadr;
}

static unsigned int kmempool_objsz_new(size_t msize)
{
	size_t sz = OBJS_ALIGN(msize);
	if (sz > OBJSORPAGE) {
		return NULL;
	}
	return kmempool_objsz_core_new(sz);
}


/******************************************
 *�鼶����
 *****************************************/
static unsigned int kmempool_page_new_callhalmm(size_t msize) 
{
	if(msize <= BLK128KB_SIZE) {
		return mem_alloc_blks(BLK128KB_SIZE);
	}
	if(msize <= BLK256KB_SIZE) {
		return mem_alloc_blks(BLK256KB_SIZE);
	}
	if(msize <= BLK512KB_SIZE) {
		return mem_alloc_blks(BLK512KB_SIZE);
	}
	if(msize <= BLK1MB_SIZE) {
		return mem_alloc_blks(BLK1MB_SIZE);
	}
	if(msize <= BLK2MB_SIZE) {
		return mem_alloc_blks(BLK2MB_SIZE);
	}
	if(msize <= BLK4MB_SIZE) {
		return mem_alloc_blks(BLK4MB_SIZE);
	}
	return NULL;
}

/**
 *1.�����ڴ�ռ��С��oskemempool�ϲ��Һ��ʵ��ڴ��mplhead_t
 */
static mplhead_t *pagenew_mplhead_isok(mplhead_t *mhp, size_t msize)
{
	
	if (mhp->mh_firtpmap == NULL) {
		return NULL;
	}
	
	if (mhp->mh_hedty != MPLHTY_PAGE) {
		return NULL;
	}
	
	if (mhp->mh_aliobsz != msize) {
		return NULL;
	}
	
	if (mhp->mh_afindx >= mhp->mh_objnr) {
		return NULL;
	}
	return mhp;
}

static mplhead_t *kmemplpg_retn_mplhead(kmempool_t *kmplockp, size_t msize)
{
	mplhead_t *retmhp;
	list_h_t *list;
	
	/*����cache�����������ֱ�ӷ��ػ�����ڴ��*/
	if (kmplockp->mp_pgmplmhcach != NULL) {
		retmhp = kmplockp->mp_pgmplmhcach;
		retmhp = pagenew_mplhead_isok(retmhp, msize);
		if (retmhp != NULL) {
			return retmhp;
		}
	}
	/*���cache�����ϣ�������ڴ������*/
	list_for_each(list, &kmplockp->mp_pgmplmheadl) {
		retmhp = list_entry(list, mplhead_t, mh_list);
		retmhp = pagenew_mplhead_isok(retmhp, msize);
		if(retmhp != NULL) {
			kmplockp->mp_pgmplmhcach = retmhp;
			return retmhp;
		}
	}
	
	return NULL;
}

/**
 *2.���û���ҵ������½�һ���ڴ�
 */
void mplhead_t_init(mplhead_t *initp)
{
    hal_spinlock_init(&initp->mh_lock);
    list_init(&initp->mh_list);
    initp->mh_hedty = 0;
    initp->mh_start = 0;
    initp->mh_end = 0;
    initp->mh_firtfreadr = NULL;
    initp->mh_firtpmap = NULL;
    initp->mh_objnr = 0;
    initp->mh_aliobsz = 0;
    initp->mh_objsz = 0;
    initp->mh_nxtpsz = 0;
    initp->mh_afindx = 0;
    initp->mh_pmnr = 0;
    initp->mh_pmap=(pglmap_t*)(((unsigned int)initp) + sizeof(mplhead_t));
    return;
}
 
static mplhead_t* page_mpool_init(kmempool_t *kmplockp, mplhead_t *initp,
		size_t msize, unsigned int start, unsigned int end)
{
    if(((start & 0xfff) != 0) || ((end - start) < ((PAGE_SIZE * 2) - 1))){
        return NULL;
    }
	
	unsigned int sadr = start + PAGE_SIZE;
	unsigned int pi = 0, pnr = 0;
	mplhead_t_init(initp);
	initp->mh_hedty = MPLHTY_PAGE;
	initp->mh_start = start;
	initp->mh_end = end;
	initp->mh_aliobsz = msize;
	initp->mh_objsz = msize;
	
	/*ѭ�������ڴ����ҳ����������������ʼ��ַ*/
	for(;;) {
		if ((sadr + msize - 1) > end) 
			break;
		initp->mh_pmap[pi].pgl_start = sadr;
		initp->mh_pmap[pi].pgl_next = &(initp->mh_pmap[pi + 1]);
		sadr += msize;
		pi++;
	}
	
	/*�������һ��pglmap_t��pgl_nextΪ��*/
	if (pi > 0) {
		initp->mh_pmap[pi - 1].pgl_next = NULL;
		pnr = pi;
		initp->mh_firtpmap = &(initp->mh_pmap[0]);
		goto add_step;
	}
	initp->mh_pmap[pi].pgl_next = NULL;
	initp->mh_pmap[pi].pgl_start = NULL;
	pnr = pi;

add_step:
	/*һ���м���ҳ*/
	initp->mh_objnr = pnr;
	/*һ���м���pglmap_t���ݽṹ*/
	initp->mh_pmnr = pnr;
	list_add(&initp->mh_list, &kmplockp->mp_pgmplmheadl);
	kmplockp->mp_pgmplnr++;
	return initp;
}
 
static mplhead_t *new_page_mpool(kmempool_t *kmplockp, size_t msize)
{
	mplhead_t *mphdp = NULL;
	size_t pgnr = msize >> 12;
	unsigned int blkadr = NULL;
	
	if (pgnr < 1) {
		return NULL;
	}
	
	if (pgnr <= 2) {
		blkadr = mem_alloc_blks(BLK128KB_SIZE);
		if (blkadr == NULL) {
			return NULL;
		}
		mphdp = page_mpool_init(kmplockp, (mplhead_t *)blkadr,
						msize, blkadr, (blkadr + BLK128KB_SIZE - 1));
		if(mphdp == NULL) {
			printk("new pg mpool err 1\n");
			hal_sysdie("new pg mpool err 1");
		}
		
		return mphdp;
	}
	
	if (pgnr <= 4) {
		blkadr = mem_alloc_blks(BLK256KB_SIZE);
		if (blkadr == NULL) {
			return NULL;
		}
		mphdp = page_mpool_init(kmplockp, (mplhead_t *)blkadr,
						msize, blkadr, (blkadr + BLK256KB_SIZE - 1));
		if(mphdp == NULL) {
			printk("new pg mpool err 2\n");
			hal_sysdie("new pg mpool err 2");
		}
		
		return mphdp;
	}
	
    if(pgnr <= 8) {
        blkadr=mem_alloc_blks(BLK256KB_SIZE);
        if(blkadr==NULL)
        {
            return NULL;
        }
        mphdp=page_mpool_init(kmplockp,(mplhead_t*)blkadr,msize,blkadr,(blkadr+BLK256KB_SIZE-1));
        if(mphdp==NULL)
        {
			printk("new pg mpool err 3\n");
            hal_sysdie("new pg mpool err 3");
        }
        return mphdp;
    }
    
    if (pgnr <= 16) {
        blkadr=mem_alloc_blks(BLK256KB_SIZE);
        if(blkadr==NULL)
        {
            return NULL;
        }
        mphdp=page_mpool_init(kmplockp,(mplhead_t*)blkadr,msize,blkadr,(blkadr+BLK256KB_SIZE-1));
        if(mphdp==NULL)
        {
			printk("new pg mpool err 4\n");
            hal_sysdie("new pg mpool err 4");
        }
        return mphdp;
    }
    
    if (pgnr <= 31) {
        blkadr=mem_alloc_blks(BLK128KB_SIZE);
        if(blkadr==NULL)
        {
            return NULL;
        }
        mphdp=page_mpool_init(kmplockp,(mplhead_t*)blkadr,msize,blkadr,(blkadr+BLK128KB_SIZE-1));
        if(mphdp==NULL)
        {
			printk("new pg mpool err 5\n");
            hal_sysdie("new pg mpool err 5");
        }
        return mphdp;
    }
    
    return NULL;	
}

/**
 *3.���ڴ���Ϸ����ڴ�ռ�
 */
unsigned int page_new_on_mplhead(mplhead_t *mplhdp)
{
	pglmap_t *map;
	
	if (mplhdp->mh_afindx >= mplhdp->mh_objnr) {
		return NULL;
	}
	
	if (mplhdp->mh_firtpmap != NULL) {
		map = mplhdp->mh_firtpmap;
		mplhdp->mh_firtpmap = map->pgl_next;
		map->pgl_next = NULL;
		mplhdp->mh_afindx++;
		return map->pgl_start;
	}
	return NULL;
}
 
/**
 *1.�����ڴ�ռ��С��oskemempool�ϲ��Һ��ʵ��ڴ��mplhead_t
 *2.���û���ҵ������½�һ���ڴ��
 *3.���ڴ���Ϸ����ڴ�ռ�
 */
static unsigned int kmempool_pages_core_new(size_t msize)
{
	cpuflg_t cpufg;
	mplhead_t *mplhdp;
	unsigned int retadr = NULL;
	kmempool_t *kmplp = &oskmempool;

	hal_spinlock_saveflg_cli(&kmplp->mp_pglock, &cpufg);
	mplhdp = kmemplpg_retn_mplhead(kmplp, msize);
	if (mplhdp == NULL) {
		mplhdp = new_page_mpool(kmplp, msize);
		if (mplhdp == NULL) {
			hal_spinunlock_restflg_sti(&kmplp->mp_pglock, &cpufg);
			return NULL;
		}
	}
	retadr = page_new_on_mplhead(mplhdp);
	hal_spinunlock_restflg_sti(&kmplp->mp_pglock, &cpufg);
	return retadr;
}

static unsigned int kmempool_pages_new(size_t msize)
{
    size_t sz = PAGE_ALIGN(msize);
    if(sz > KPMPORHALM)
    {
        return kmempool_page_new_callhalmm(sz);
    }
    return kmempool_pages_core_new(sz);
}

static unsigned int kmempool_onsize_new(size_t msize) 
{
    if(msize > OBJSORPAGE)
    {
        return kmempool_pages_new(msize);
    }
    return kmempool_objsz_new(msize);
}

unsigned int kmempool_new(size_t msize) 
{
	if (msize < KMEMPALCSZ_MIN || msize > KMEMPALCSZ_MAX) {
		return NULL;
	}
	return kmempool_onsize_new(msize);
}


/**
 *�ͷź���
 */
/***********************************************
 *�ֽڼ��ڴ��ͷ�
 ***********************************************/ 
/**
 *1.��kmempool_t�����ڴ��
 */ 
static mplhead_t* objsdel_mplhead_isok(mplhead_t* mhp,adr_t fradr,size_t msize)
{
	/*�����ֽ��ڴ��*/
    if(mhp->mh_hedty != MPLHTY_OBJS) {
        return NULL;
    }
	/*��С����*/
    if(mhp->mh_aliobsz != msize) {
        return NULL;
    }
	
    if(mhp->mh_afindx == 0) {
        return NULL;
    }
	/*�����ֽ��ڴ�ط�Χ��*/
    if(fradr < (mhp->mh_start + sizeof(mplhead_t))
			|| (fradr + msize - 1) > mhp->mh_end) {
        return NULL;
    }
	
    return mhp;
}

static mplhead_t *kmempldelob_retn_mplhead(kmempool_t *kmplockp,
				adr_t fradr, size_t msize)
{
	mplhead_t *retmhp;
	list_h_t *list;
	if (kmplockp->mp_obmplmhcach != NULL) {
		/*���cache�ǵĻ��ͷ���cache���ڴ��*/
		retmhp = kmplockp->mp_obmplmhcach;
		retmhp = objsdel_mplhead_isok(retmhp, fradr, msize);
		if (retmhp != NULL) {
			return retmhp;
		}
	}
	/*��������ڴ��*/
	list_for_each(list,&kmplockp->mp_obmplmheadl) {
        retmhp = list_entry(list, mplhead_t, mh_list);
        retmhp=objsdel_mplhead_isok(retmhp, fradr, msize);
        if(retmhp != NULL) {
            return retmhp;
        }
    }
    return NULL;
}
 
/**
 *2.�ͷ��ڴ��
 */ 
static bool_t objs_delete_on_mplhead(mplhead_t *mplhdp, adr_t fradr)
{
    if(mplhdp->mh_afindx < 1 || fradr < (mplhdp->mh_start + sizeof(mplhead_t)) ||
		(fradr + mplhdp->mh_aliobsz) >= mplhdp->mh_end) {
        return FALSE;
    }
    adr_t retadr=NULL,*nextp=NULL,adrsz=(adr_t)(mplhdp->mh_aliobsz);
	nextp = (adr_t *)(fradr + adrsz);
	if (*nextp != NULL) {
		return FALSE;
	}
	retadr = mplhdp->mh_firtfreadr;
	mplhdp->mh_firtfreadr = fradr;
	*nextp = retadr;
	mplhdp->mh_afindx--;
	return TRUE;
}
 
/**
 *3.����ڴ��Ϊ�գ���ôɾ�����ڴ��
 */ 
static bool_t del_objs_mpool(kmempool_t *kmplockp, mplhead_t *mphdp)
{
	if (mphdp->mh_afindx > 0) {
		return TRUE;
	}
	
	size_t frsz = mphdp->mh_end - mphdp->mh_start + 1;
	adr_t fradr = mphdp->mh_start;
	list_del(&mphdp->mh_list);
	if (kmplockp->mp_obmplnr  < 1) {
        hal_sysdie("del_objs_mpool kmplockp->mp_obmplnr<1");		
	}
	kmplockp->mp_obmplnr--;
	if (kmplockp->mp_obmplmhcach == mphdp) {
		kmplockp->mp_obmplmhcach = NULL;
	}
	
    if(mem_free_blks(fradr,frsz)==FALSE)
    {
        hal_sysdie("del_objs_mpool page delete err");
    }
    return TRUE;
} 

static bool_t kmempool_objsz_core_delete(unsigned int fradr, size_t frsz)
{
	
    bool_t rets = FALSE;
    cpuflg_t cpufg;
    mplhead_t* mplhdp;
    kmempool_t* kmplp = &oskmempool;
    
	hal_spinlock_saveflg_cli(&kmplp->mp_oblock, &cpufg);
	mplhdp = kmempldelob_retn_mplhead(kmplp, fradr, frsz);
	if (mplhdp == NULL) {
		rets = FALSE;
		goto return_step;
	}
    if(objs_delete_on_mplhead(mplhdp,fradr) == FALSE) {
        rets=FALSE;
        goto return_step;
    }
    if(del_objs_mpool(kmplp,mplhdp) == FALSE) {
        rets=FALSE;
        goto return_step;
    }
    rets=TRUE;	
	
return_step:
    hal_spinunlock_restflg_sti(&kmplp->mp_oblock, &cpufg);
    return rets;	
}
 
static bool_t kmempool_objsz_delete(unsigned int fradr, size_t frsz) 
{
	size_t fsz = OBJS_ALIGN(frsz);
	if (fsz > OBJSORPAGE) {
		return FALSE;
	}
	return kmempool_objsz_core_delete(fradr, frsz);
}
 
 
/***********************************************
 *ҳ���ڴ��ͷ�
 ***********************************************/ 
/**
 *1.������Ӧ�ڴ�ҳ����ͷŵ�ַ�ʹ�С����kmempool_t�в����ڴ��
 */
static mplhead_t *pagedel_mplhead_isok(mplhead_t *mhp, 
						unsigned int fradr,size_t msize)
{
	/*����ҳ���ڴ�ؾͷ���*/
	if (mhp->mh_hedty != MPLHTY_PAGE) {
		return NULL;
	}
	/*�ڴ�صĴ�С����*/
	if (mhp->mh_aliobsz != msize) {
		return NULL;
	}
	/*�ڴ��Ϊ��*/
	if (mhp->mh_afindx == 0) {
		return NULL;
	}
	/*fradr�����ڴ����*/
	if (fradr < (mhp->mh_start + PAGE_SIZE) || (fradr + msize - 1) > mhp->mh_end){
		return NULL;
	}
	return mhp;
}
 
static mplhead_t *kmempldelpg_retn_mplhead(kmempool_t *kmplockp,
						unsigned int fradr, size_t msize)
{
	mplhead_t *retmhp;
	list_h_t *list;
	if (kmplockp->mp_pgmplmhcach != NULL) {
		retmhp = kmplockp->mp_pgmplmhcach;
		retmhp = pagedel_mplhead_isok(retmhp, fradr, msize);
		if (retmhp != NULL) {
			return retmhp;
		}
	}
	
	list_for_each(list, &kmplockp->mp_pgmplmheadl) {
		retmhp = list_entry(list, mplhead_t, mh_list);
		retmhp = pagedel_mplhead_isok(retmhp, fradr, msize);
		if (retmhp != NULL) {
			return retmhp;
		}
	}
	return NULL;
}
 
/**
 *2.�ͷŸ��ڴ��
 */
bool_t page_delete_on_mplhead(mplhead_t *mplhdp, unsigned int fradr)
{
	pglmap_t *map;
	unsigned int i;
	if (mplhdp->mh_afindx < 1 || mplhdp->mh_pmnr < 1) {
		return FALSE;
	}
	
	for(i = 0; i < mplhdp->mh_pmnr; i++) {
		if (fradr == mplhdp->mh_pmap[i].pgl_start) {
			map = &mplhdp->mh_pmap[i];
			if (map->pgl_next != NULL) {
				return FALSE;
			}
			goto del_step;
		}
	}
	return FALSE;

del_step:
    map->pgl_next = mplhdp->mh_firtpmap;
    mplhdp->mh_firtpmap = map;
    mplhdp->mh_afindx--;
    return TRUE;    
}

/**
 *3.������ڴ���Ѿ������ˣ�ɾ������ڴ�أ����֪ͨ�鼶
 *  �ڴ����ģ���ͷŵײ�������ڴ��
 */
static bool_t del_page_mpool(kmempool_t *kmplockp, mplhead_t *mphdp)
{
	if (mphdp->mh_afindx > 0) {
		return TRUE;
	}
	
	size_t frsz = mphdp->mh_end - mphdp->mh_start + 1;
	unsigned int fradr = mphdp->mh_start;
	list_del(&mphdp->mh_list);
	if (kmplockp->mp_pgmplnr < 1) {
		printk("del_page_mpool kmplockp->mp_pgmplnr < 1\n");
		hal_sysdie("del_page_mpool kmplockp->mp_pgmplnr < 1");
	}
	
	kmplockp->mp_pgmplnr--;
	if (kmplockp->mp_pgmplmhcach == mphdp) {
		kmplockp->mp_pgmplmhcach = NULL;
	}
	
	if (mem_free_blks(fradr, frsz) == FALSE)  {
		printk("del_page_mpool mem_free_blks\n");
		hal_sysdie("del_page_mpool mem_free_blks\n");		
	}
	return TRUE;	
}
 
/**
 *1.������Ӧ�ڴ�ҳ����ͷŵ�ַ�ʹ�С����kmempool_t�в����ڴ��
 *2.�ͷŸ��ڴ��
 *3.������ڴ���Ѿ������ˣ�ɾ������ڴ�أ����֪ͨ�鼶
 *  �ڴ����ģ���ͷŵײ�������ڴ��
 */
 
bool_t kmempool_pages_core_delete(unsigned int fradr, size_t frsz)
{
	cpuflg_t cpufg;
	mplhead_t *mplhdp;
	bool_t rets = FALSE;
	kmempool_t *kmplp = &oskmempool;
	
	hal_spinlock_saveflg_cli(&kmplp->mp_pglock, &cpufg);
	mplhdp = kmempldelpg_retn_mplhead(kmplp, fradr, frsz);
	if (mplhdp == NULL) {
		rets = FALSE;
		goto return_step;
	}
	
	if (page_delete_on_mplhead(mplhdp, fradr) == FALSE) {
		rets = FALSE;
		goto return_step;
	}
	
	if (del_page_mpool(kmplp, mplhdp) == FALSE) {
		rets = FALSE;
		goto return_step;
	}
	
return_step:
	hal_spinunlock_restflg_sti(&kmplp->mp_pglock, &cpufg);
	return rets;
}
 
bool_t kmempool_page_delete_callhalmm(unsigned int fradr,size_t frze)
{
    if(frze <= BLK128KB_SIZE) {
        return mem_free_blks(fradr, BLK128KB_SIZE);
    }
    if(frze <= BLK256KB_SIZE) {
        return mem_free_blks(fradr, BLK256KB_SIZE);
    }
    if(frze <= BLK512KB_SIZE) { 
        return mem_free_blks(fradr, BLK512KB_SIZE);
    }
    if(frze <= BLK1MB_SIZE) {
        return mem_free_blks(fradr, BLK1MB_SIZE);
    }
    if(frze <= BLK2MB_SIZE) {
        return mem_free_blks(fradr, BLK2MB_SIZE);
    }
    if(frze <= BLK4MB_SIZE) {
        return mem_free_blks(fradr, BLK4MB_SIZE);
    }
    return FALSE;
}
 
static bool_t kmempool_pages_delete(unsigned int fradr,size_t frsz)
{
    size_t sz = PAGE_ALIGN(frsz);
    if(sz > KPMPORHALM)
    {
        return kmempool_page_delete_callhalmm(fradr, sz);
    }
    return kmempool_pages_core_delete(fradr, sz);
}

static bool_t kmempool_onsize_delete(unsigned int fradr, size_t frsz) 
{
	if (frsz > OBJSORPAGE) {
		return kmempool_pages_delete(fradr, frsz);
	}
    return kmempool_objsz_delete(fradr,frsz);
}

bool_t kmempool_delete(unsigned int fradr, size_t frsz) 
{
	if (fradr == NULL || frsz < KMEMPALCSZ_MIN
	        || frsz > KMEMPALCSZ_MAX) {
		return FALSE;
	}
	return kmempool_onsize_delete(fradr, frsz);
}

#if 0
/**
 * ���Դ���
 */
typedef struct s_adrsz
{
    unsigned int adr;
    size_t sz; 
}adrsz_t;

static void testpgmpool()
{
    adrsz_t adsz[10];
    size_t alcsz=0x1000;
    adsz[0].sz=alcsz;
    adsz[0].adr=kmempool_new(alcsz);
    adsz[1].sz=alcsz;
    adsz[1].adr=kmempool_new(alcsz);
    alcsz=0x1500;
    adsz[2].sz=alcsz;
    adsz[2].adr=kmempool_new(alcsz);
    adsz[3].sz=alcsz;
    adsz[3].adr=kmempool_new(alcsz);
    alcsz=0x3000;
    adsz[4].sz=alcsz;
    adsz[4].adr=kmempool_new(alcsz);
    adsz[5].sz=alcsz;
    adsz[5].adr=kmempool_new(alcsz);
    alcsz=0x3200;
    adsz[6].sz=alcsz;
    adsz[6].adr=kmempool_new(alcsz);
    adsz[7].sz=alcsz;
    adsz[7].adr=kmempool_new(alcsz);
    alcsz=0x7000;
    adsz[8].sz=alcsz;
    adsz[8].adr=kmempool_new(alcsz);
    adsz[9].sz=alcsz;
    adsz[9].adr=kmempool_new(alcsz);
	int i;
    for(i=0;i<10;i++)
    {
        printk("adsz[%x] sz:%x adr:%x\n\r",i,adsz[i].sz,adsz[i].adr);
    }
    mplhead_t* retmhp;
    list_h_t* list;

    list_for_each(list,&oskmempool.mp_pgmplmheadl)
    {
        retmhp=list_entry(list,mplhead_t,mh_list);
        printk("mph_t_adr:%x mph_t.mh_end:%x mph_t.mh_objsz:%x mph_t.mh_objnr:%x\n\r",
                retmhp,retmhp->mh_end,retmhp->mh_objsz,retmhp->mh_objnr);
    }
    return;
}

static void cmp_adrsz(adrsz_t* assp,unsigned int nr)
{
	unsigned int i, j, k, h, l, m, n, o;
    for(i=0;i<nr;i++)
    {
        for(j=0;j<nr;j++)
        {
            if(i!=j)
            {
                if(assp[i].adr==assp[j].adr)
                {
                    hal_sysdie("cmp adr start err");
                }
            }
        }
    }

    for(k=0;k<nr;k++)
    {
        for(h=0;h<nr;h++)
        {
            if(k!=h)
            {
                if((assp[k].adr+assp[k].sz)==(assp[h].adr+assp[h].sz))
                {
                    hal_sysdie("cmp adr end err");
                }
            }
        }
    }

    for(l=0;l<nr;l++)
    {
        for(m=0;m<nr;m++)
        {
            if(l!=m)
            {
                if((assp[l].adr>=(assp[m].adr))&&((assp[l].adr+assp[l].sz)<=(assp[m].adr+assp[m].sz)))
                {
                    hal_sysdie("cmp adr in err");
                }
            }
        }
    }
    unsigned char* bytp=NULL,bytv=0;
    for(n=0;n<nr;n++)
    {
        bytp=(unsigned char*)(assp[n].adr);
        bytv=(unsigned char)(assp[n].adr&0xff);
        for(o=0;o<assp[n].sz;o++)
        {
            if(bytp[o]!=bytv)
            {
                hal_sysdie("cmp val err");
            }
        }
    }
    return;
}

static void testpagemgr()
{
	int i, j;
    adrsz_t adsz[30];
    size_t alcsz=0x1000;
    for(;alcsz<0x19000;alcsz+=0x1000)
    {
        for(i=0;i<30;i++)
        {
            adsz[i].sz=alcsz;
            adsz[i].adr=kmempool_new(alcsz);
            if(adsz[i].adr==NULL)
            {
                hal_sysdie("testpagemgr kmempool_new err");
            }
            printk("adsz[%x] sz:%x adr:%x\n\r",i,adsz[i].sz,adsz[i].adr);
        }
        cmp_adrsz(adsz,30);
        for(j=0;j<30;j++)
        {
            if(kmempool_delete(adsz[j].adr,adsz[j].sz)==FALSE)
            {
                hal_sysdie("testpagemgr kmempool_delete err");
            }
            printk("delete adsz[%x] sz:%x adr:%x\n\r",j,adsz[j].sz,adsz[j].adr);
        }
    }
    printk("oskmempool.mp_pgmplnr:%x\n\r",oskmempool.mp_pgmplnr);
    return;
}

static void testobjsmgr()
{
    adrsz_t adsz[64];
    size_t alcsz=0x20;
    unsigned char* adrbytp=NULL,bytval=0;
	int i,j, k;
    for(;alcsz<0x800;alcsz+=0x10)
    {
        for( i=0;i<64;i++)
        {
            adsz[i].sz=alcsz;
            adsz[i].adr=kmempool_new(alcsz);
            if(adsz[i].adr==NULL)
            {
                hal_sysdie("testobjsmgr kmempool_new err");
            }
            printk("objs alloc adsz[%x] sz:%x adr:%x\n\r",i,adsz[i].sz,adsz[i].adr);
            adrbytp=(unsigned char*)adsz[i].adr;
            bytval=(unsigned char)(adsz[i].adr&0xff);
            for( k=0;k<adsz[i].sz;k++)
            {
                adrbytp[k]=bytval;
            }
        }
        cmp_adrsz(adsz,64);
        for( j=0;j<64;j++)
        {
            if(kmempool_delete(adsz[j].adr,adsz[j].sz)==FALSE)
            {
                hal_sysdie("testobjsmgr kmempool_delete err");
            }
            printk("objs delete adsz[%x] sz:%x adr:%x\n\r",j,adsz[j].sz,adsz[j].adr);
        }
    }
    printk("oskmempool.mp_obmplnr:%x\n\r",oskmempool.mp_obmplnr);
    return;
}
#endif
/**
 * ��ʼ���ӿ�
 */
void init_krlpagempol()
{
	kmempool_t_init(&oskmempool);
#if 0
	testpagemgr();
	testobjsmgr();
#endif
}

void kmempool_t_init(kmempool_t *initp)
{
	hal_spinlock_init(&initp->mp_lock);
	list_init(&initp->mp_list);
	initp->mp_stus = 0;
	initp->mp_flgs = 0;
	hal_spinlock_init(&initp->mp_pglock);
	hal_spinlock_init(&initp->mp_oblock);
	initp->mp_pgmplnr = 0;
	initp->mp_obmplnr = 0;
	list_init(&initp->mp_pgmplmheadl);
	list_init(&initp->mp_obmplmheadl);
	initp->mp_pgmplmhcach = NULL;
	initp->mp_obmplmhcach = NULL;
}

adr_t krlnew(size_t mmsize)
{
    if(mmsize==MALCSZ_MIN||mmsize>MALCSZ_MAX)
    {
        return NULL;
    }
    return kmempool_new(mmsize);
}

bool_t krldelete(adr_t fradr,size_t frsz)
{
    if(fradr==NULL||frsz==MALCSZ_MIN||frsz>MALCSZ_MAX)
    {
        return FALSE;
    }
    return  kmempool_delete(fradr,frsz);
}

