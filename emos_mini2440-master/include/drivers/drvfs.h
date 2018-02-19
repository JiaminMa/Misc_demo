#ifndef _DRV_FS_H_
#define _DRV_FS_H_

#include "spinlock.h"
#include "types.h"
#include "krldevice.h"

#define DR_NM_MAX (128 - (sizeof(unsigned int) * 3))
#define RDR_NUL_TYPE 0
#define RDR_DIR_TYPE 1
#define RDR_FIL_TYPE 2
#define RDR_DEL_TYPE 5
#define FBLKS_MAX 32

#define FMD_NUL_TYPE 0
#define FMD_DIR_TYPE 1
#define FMD_FIL_TYPE 2
#define FMD_DEL_TYPE 5
#define FSMM_BLK 0x400000
#define FSDEV_OPENFLG_OPEFILE 2

/*�ļ�����ͷ*/

typedef struct s_FILBLKS {
	unsigned int fb_blkstart;
	unsigned int fb_blknr;
}filblks_t;

typedef struct s_fimgrhd {
	unsigned int fmd_stus;
	unsigned int fmd_type;
	unsigned int fmd_flg;
	unsigned int fmd_sfblk;
	unsigned int fmd_acss;
	unsigned int fmd_newtime;
	unsigned int fmd_acstime;
	unsigned int fmd_fileallbk;
	unsigned int fmd_filesz;
	unsigned int fmd_fileifstbkoff; /*�ļ������ٵ�һ���߼��洢���е�ƫ��*/
	unsigned int fmd_fileiendbkoff; /*�ļ����������һ���߼��洢���е�ƫ��*/
	unsigned int fmd_curfwritebk;	/*�ļ���ǰ��Ҫд����߼��洢��*/
	unsigned int fmd_curfinwbkoff; /*�ļ���ǰ��Ҫд����߼��洢���ƫ��*/
	filblks_t fmd_fleblk[FBLKS_MAX];
	unsigned int fmd_linkpblk;		/*ָ����һ���ļ�����ͷ���߼��洢��*/
	unsigned int fmd_linknblk;		/*ָ����һ���ļ�����ͷ���߼��洢��*/
}fimgrhd_t;

/*Ŀ¼*/
typedef struct s_RFSDIR {
	unsigned int rdr_stus;		/*Ŀ¼״̬*/
	unsigned int rdr_type;		/*Ŀ¼����*/
	unsigned int rdr_blknr;		/*ָ���ļ����ݹ���ͷ�Ŀ��*/
	char rdr_name[DR_NM_MAX];	/*��������*/
}rfsdir_t;

/*������*/
typedef struct s_RFSSUBLK {
	spinlock_t rsb_lock;	
	unsigned int rsb_mgic;		/*ħ�����ļ�ϵͳ��ʾ*/
	unsigned int rsb_vec;		/*�ļ�ϵͳ��ʶ*/
	unsigned int rsb_flg;		/*��ʶ*/
	unsigned int rsb_stus;		/*״̬*/
	size_t rsb_sz;				/*�����ݽṹ����Ĵ�С*/
	size_t rsb_sblksz;			/*�������С*/
	size_t rsb_dblksz;			/*�ļ�ϵͳ�߼����С*/
	unsigned int rsb_bmpbks;	/*λͼ��ʼ�߼��洢��*/
	unsigned int rsb_bmpbknr;	/*λͼռ���˶��ٸ��߼��洢��*/
	unsigned int rsb_fsysallblk; /*�ļ�ϵͳ�ж��ٸ��߼��洢��*/
	rfsdir_t rsb_rootdir;		/*��Ŀ¼*/
}rfssublk_t;

typedef struct s_RFSDEVEXT {
	spinlock_t rde_lock;
	list_h_t rde_list;
	unsigned int rde_flg;
	unsigned int rde_stus;
	void *rde_mstart;
	size_t rde_msize;
	void *rde_ext;
}rfsdevext_t;

#define FSYS_ALCBLKSZ 0x1000

bool_t create_bitmap(device_t *devp);
void rfsdir_t_init(rfsdir_t *initp);
void rfssublk_t_init(rfssublk_t *sbp);
bool_t create_superblk(device_t *devp);
void rfs_set_driver(driver_t* drvp);
void rfs_set_device(device_t* devp,driver_t* drvp);
drvstus_t rfs_entry(driver_t* drvp,unsigned int val,void* p);
drvstus_t rfs_exit(driver_t* drvp,unsigned int val,void* p);
drvstus_t rfs_open(device_t* devp,void* iopack);
drvstus_t rfs_close(device_t* devp,void* iopack);
drvstus_t rfs_read(device_t* devp,void* iopack);
drvstus_t rfs_write(device_t* devp,void* iopack);
drvstus_t rfs_lseek(device_t* devp,void* iopack);
drvstus_t rfs_ioctrl(device_t* devp,void* iopack);
drvstus_t rfs_dev_start(device_t* devp,void* iopack);
drvstus_t rfs_dev_stop(device_t* devp,void* iopack);
drvstus_t rfs_set_powerstus(device_t* devp,void* iopack);
drvstus_t rfs_enum_dev(device_t* devp,void* iopack);
drvstus_t rfs_flush(device_t* devp,void* iopack);
drvstus_t rfs_shutdown(device_t* devp,void* iopack);
drvstus_t rfs_new_file(device_t* devp,char* fname,unsigned int flg);
drvstus_t rfs_del_file(device_t* devp,char* fname,unsigned int flg);
drvstus_t rfs_read_file(device_t* devp,void* iopack);
drvstus_t rfs_write_file(device_t* devp,void* iopack);
drvstus_t rfs_open_file(device_t* devp,void* iopack);
drvstus_t rfs_close_file(device_t* devp,void* iopack);
void test_file(device_t* devp);
#endif
