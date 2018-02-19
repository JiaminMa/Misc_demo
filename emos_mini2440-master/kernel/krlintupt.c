#include "interrupt.h"
#include "types.h"
#include "krlmm.h"
#include "interrupt.h"
#include "krlintupt.h"
#include "cpuctrl.h"
/**
 *1.�ṩһЩ��Ϣ
 *2.����һ��intserdsc_t�ṹ�壬����Ϣ��������
 *3.�����interdsc_t�ṹ�ҵ���Ӧ��intfltdsc_t�ж�Դ��
 */
 
intserdsc_t *krladd_irqhandle(void *device, intflthandle_t handle,
					unsigned int phyline)
{
	if (device == NULL || handle == NULL) {
		return NULL;
	}
	
	intfltdsc_t *intp = retn_intfltdsc(phyline);
	if (intp == NULL) {
		return NULL;
	}
	
	intserdsc_t *serdscp = (intserdsc_t*)krlnew(sizeof(intserdsc_t));
	if (serdscp == NULL) {
		return NULL;
	}
	
	intserdsc_t_init(serdscp, 0, intp, device, handle);
	if (hal_add_ihandle(intp, serdscp) == FALSE) {
		if(krldelete((adr_t)serdscp, sizeof(intserdsc_t)) == FALSE) {
            hal_sysdie("krladd_irqhandle ERR");
        }
        return NULL;
	}
	return serdscp;
}

drvstus_t krlenable_intline(unsigned int ifdnr)
{
    
    return enable_intline(ifdnr);
}

drvstus_t krldisable_intline(unsigned int ifdnr)
{
 
    return disable_intline(ifdnr);

}

