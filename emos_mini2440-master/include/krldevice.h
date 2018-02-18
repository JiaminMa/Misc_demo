#ifndef _KERNEL_DEVICE_H_
#define _KERNEL_DEVICE_H_
#include "spinlock.h"
#include "list.h"
#include "types.h"
#include "sem.h"
#include "krlintupt.h"

#define NOT_DEVICE 0
#define AMBABUS_DEVICE 1
#define AHBBUS_DEVICE 2
#define APBBUS_DEVICE 3
#define BRIDGE_DEVICE 4
#define CPUCORE_DEVICE 5
#define RAMCONTER_DEVICE 6
#define RAM_DEVICE 7
#define USBHOSTCONTER_DEVICE 8
#define INTUPTCONTER_DEVICE 9
#define DMA_DEVICE 10
#define CLOCKPOWER_DEVICE 11
#define LCDCONTER_DEVICE 12
#define NANDFLASH_DEVICE 13
#define CAMERA_DEVICE 14
#define UART_DEVICE 15
#define TIMER_DEVICE 16
#define USB_DEVICE 17
#define WATCHDOG_DEVICE 18
#define IIC_DEVICE 19
#define IIS_DEVICE 20
#define GPIO_DEVICE 21
#define RTC_DEVICE 22
#define A_DCONVER_DEVICE 23
#define SPI_DEVICE 24
#define SD_DEVICE 25
#define AUDIO_DEVICE 26
#define TOUCH_DEVICE 27
#define NETWORK_DEVICE 28
#define VIR_DEVICE 29
#define FILESYS_DEVICE 30
#define SYSTICK_DEVICE 31
#define UNKNOWN_DEVICE 32

#define IOIF_CODE_OPEN 0 //对应于open操作
#define IOIF_CODE_CLOSE 1 //对应于close操作
#define IOIF_CODE_READ 2 //对应于read操作
#define IOIF_CODE_WRITE 3 //对应于write操作
#define IOIF_CODE_LSEEK 4 //对应于lseek操作
#define IOIF_CODE_IOCTRL 5 //对应于ioctrl操作
#define IOIF_CODE_DEV_START 6 //对应于start操作
#define IOIF_CODE_DEV_STOP 7 //对应于stop操作
#define IOIF_CODE_SET_POWERSTUS 8 //对应于powerstus操作
#define IOIF_CODE_ENUM_DEV 9 //对应于enum操作
#define IOIF_CODE_FLUSH 10 //对应于flush操作
#define IOIF_CODE_SHUTDOWN 11 //对应于shutdown操作
#define IOIF_CODE_MAX 12 //最大功能码

#define DEVFLG_EXCLU (1<<0)
#define DEVFLG_SHARE (1<<1)
#define DEVSTS_NORML (1<<0)
#define DEVSTS_FAILU (1<<1)
#define DIDFIL_IDN 1
#define DIDFIL_FLN 2

#define FSDEV_IOCTRCD_DELFILE 5
#define FSDEV_OPENFLG_NEWFILE 1
#define FSDEV_OPENFLG_OPEFILE 2

#define DIDFIL_IDN 1
typedef struct s_DEVID {
	unsigned int dev_mtype; /*Master type*/
	unsigned int dev_stype; 
	unsigned int dev_nr;
}devid_t;

typedef struct s_DEVICE{
	list_h_t dev_list;		/*设备链表节点*/
	list_h_t dev_indrvlst;	/*在驱动程序中挂载链表*/
	list_h_t dev_intbllst;	/*在设备表数据中对应的挂载链表*/
	spinlock_t dev_lock;
	unsigned int dev_count;
	sem_t dev_sem;
	unsigned int dev_stus;
	unsigned int dev_flgs;
	devid_t dev_id;
	unsigned int dev_intlnenr; 	/*设备中断服务例程个数*/
	list_h_t dev_intserlst;		/*设备中断服务例程的链表*/
	list_h_t dev_rqlist;		/*设备请求的服务链表*/
	unsigned int dev_rqlnr;		/*对设备请求服务个数*/
	sem_t dev_waitints;
	struct s_DRIVER *dev_drv;
	void *dev_attrb;
	void *dev_privdata;
	void *dev_userdata;
	void *dev_extdata;
	char *dev_name;
}device_t;

typedef drvstus_t (*drivcallfun_t)(device_t *, void *);
typedef drvstus_t (*drventyexit_t)(struct s_DRIVER*, unsigned int, void*);

/*驱动程序描述符*/
typedef struct s_DRIVER {
	spinlock_t drv_lock;
	list_h_t drv_list;		/*驱动程序链表节点*/
	unsigned int drv_stuts;
	unsigned int drv_flg;
	unsigned int drv_id;
	unsigned int drv_count;
	sem_t drv_sem;
	void *drv_safedsc;
	void *drv_attrb;
	void *drv_privdata;
	drivcallfun_t drv_dipfun[IOIF_CODE_MAX]; /*驱动分发数据函数指针数组*/
	list_h_t drv_alldevlist;
	drventyexit_t drv_entry; /*入口函数*/
	drventyexit_t drv_exit;
	void *drv_userdata;
	void *drv_extdata;
	char *drv_name;
}driver_t;

#define OBJN_TY_DEV 1
#define OBJN_TY_FIL 2
#define OBJN_TY_NUL 0
typedef struct s_OBJNODE {
	spinlock_t on_lock;
	list_h_t on_list;
	sem_t on_complesem;
	unsigned int on_flgs;
	unsigned int on_stus;
	int on_opercode;
	unsigned int on_objtype;
	void *on_objadr;
	unsigned int on_acsflgs;
	unsigned int on_acsstus;
	unsigned int on_currops;
	unsigned int on_len;
	unsigned int on_ioctrd;
	buf_t on_buf;
	unsigned int on_bufcurops;
	size_t on_bufsz;
	unsigned int on_count;
	void *on_safedsc;
	void *on_fname;
	void *on_finode;
	void *on_extp;
}objnode_t;

/*设备表*/
#define DEVICE_MAX 33
typedef struct s_DEVLIST{
	unsigned int dtl_type;
	unsigned int dtl_nr;
	list_h_t dtl_list;
}devtlst_t;

typedef struct s_DEVTABLE {
	list_h_t devt_list;
	spinlock_t devt_lock;
	list_h_t devt_devlist;
	list_h_t devt_drvlist;
	unsigned int devt_devnr;
	unsigned int devt_drvnr;
	devtlst_t devt_devclsl[DEVICE_MAX];
}devtable_t;


void devtlst_t_init(devtlst_t* initp,unsigned int dtype);
void devtable_t_init(devtable_t* initp);
void devid_t_init(devid_t* initp,unsigned int mty,unsigned int sty,unsigned int nr);
void device_t_init(device_t* initp);
void krlretn_driverid(driver_t* dverp);
void driver_t_init(driver_t* initp);
void init_krldevice();
drvstus_t krlrun_driverentry(drventyexit_t drventry);
void init_krldriver();
drvstus_t del_driver_dsc(driver_t* drvp);
driver_t* new_driver_dsc();
drvstus_t del_device_dsc(device_t* devp);
device_t* new_device_dsc();
drvstus_t drv_defalt_func(device_t* devp,void* iopack);
bool_t krlcmp_devid(devid_t* sdidp,devid_t* cdidp);
drvstus_t krldriver_add_system(driver_t* drvp);
drvstus_t krldev_add_driver(device_t* devp,driver_t* drvp);
drvstus_t krlnew_device(device_t* devp);
drvstus_t krldev_inc_devcount(device_t* devp);
drvstus_t krldev_dec_devcount(device_t* devp);
drvstus_t krldev_add_request(device_t *devp, objnode_t* request);
drvstus_t krldev_complete_request(device_t *devp, objnode_t* request);
drvstus_t krldev_retn_request(device_t *devp,unsigned int iocode,objnode_t** retreq);
drvstus_t krldev_wait_request(device_t *devp, objnode_t* request);
drvstus_t krldev_retn_rqueparm(void* request,buf_t* retbuf,unsigned int* retcops,unsigned int* retlen,unsigned int* retioclde,unsigned int* retbufcops,size_t* retbufsz);
device_t* krlonidfl_retn_device(void* dfname,unsigned int flgs);
drvstus_t krlnew_devhandle(device_t* devp,intflthandle_t handle,unsigned int phyiline);
drvstus_t krldev_io(objnode_t* nodep);
drvstus_t krldev_call_driver(device_t* devp,unsigned int iocode,unsigned int val1,unsigned int val2,void* p1,void* p2);
#endif


