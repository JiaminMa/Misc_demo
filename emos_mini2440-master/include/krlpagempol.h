#ifndef _KRL_PAGE_MEM_POOL_H_
#define _KRL_PAGE_MEM_POOL_H_
#include "types.h"
#include "spinlock.h"
#include "list.h"

#define KMEMPALCSZ_MIN 1
#define KMEMPALCSZ_MAX 0x400000
#define MPLHTY_PAGE 1
#define MPLHTY_OBJS 2
#define PAGE_SIZE (0x1000)
#define OBJSORPAGE 2048
#define KPMPORHALM (PAGE_SIZE*31)
#define PAGE_ALIGN(x) ALIGN(x,PAGE_SIZE)
#define OBJS_ALIGN(x) ALIGN(x,4)

typedef struct s_PGLMAP {
	unsigned int pgl_start;
	struct s_PGLMAP *pgl_next;
}pglmap_t;

typedef struct s_MPLHEAD {
	spinlock_t mh_lock;
	list_h_t mh_list;
	unsigned int mh_hedty;		/*�ڴ�ع���ͷ������*/
	unsigned int mh_start;  	/*�ڴ����ʼ��ַ*/
	unsigned int mh_end;		/*�ڴ�ؽ�����ַ*/
	unsigned int mh_firtfreadr;
	pglmap_t *mh_firtpmap;		/*ָ����һ������ҳ��ָ��*/
	unsigned int mh_objnr;		/*�ڴ�����ҳ����*/
	unsigned int mh_aliobsz;
	unsigned int mh_objsz;
	unsigned int mh_nxtpsz;
	unsigned int mh_afindx;		/*�Ѿ������ҳ��*/
	unsigned int mh_pmnr; 		/*ҳ״̬�����ṹ*/
	pglmap_t *mh_pmap;			/*ҳ״̬�����ṹ��ŵ�ַ*/
}mplhead_t;

typedef struct s_KMEMPOOL {
	spinlock_t mp_lock;
	list_h_t mp_list;
	unsigned int mp_stus;
	unsigned int mp_flgs;
	
	spinlock_t mp_pglock;
	spinlock_t mp_oblock;
	unsigned int mp_pgmplnr; /*ҳ���ڴ�صĸ��� Page mem pool nr*/
	unsigned int mp_obmplnr; /*Object mem pool nr*/
	list_h_t mp_pgmplmheadl;
	list_h_t mp_obmplmheadl;
	mplhead_t *mp_pgmplmhcach;	/*���浱ǰ��������ҳ���ڴ�ؽṹ*/
	mplhead_t *mp_obmplmhcach;
}kmempool_t;

void init_krlpagempol(void);
void kmempool_t_init(kmempool_t *initp);
unsigned int kmempool_new(size_t msize);
bool_t kmempool_delete(unsigned int fradr, size_t frsz);

#endif
