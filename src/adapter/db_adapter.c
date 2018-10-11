/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
**
**                                       yzfcer@163.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: db_adapter.c
**��   ��   ��: Jason Zhou
**����޸�����: 2017.12.19
**��        ��: 
**              
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: 
** ��  ��: v1.0
** �ա���: 2017.12.19
** �衡��: ԭʼ�汾
**
**--------------��ǰ�汾�޶�----------------------------------------------------------------------------
** �޸���: 
** �ա���: 2017.12.19
** �衡��: 
**
**------------------------------------------------------------------------------------------------------
*******************************************************************************************************/
#include "db_adapter.h"
#include "malloc.h"
#define DB_MEM_FLAG 0xa6794d65
w_int8_t db_shm[1024000];

void *db_malloc(w_int32_t size)
{
    w_uint32_t *ptr = (w_uint32_t *)malloc(size + sizeof(w_uint32_t));
    if(ptr != NULL)
    {
        ptr[0] = DB_MEM_FLAG;
        return &ptr[1];
    }
    return NULL;
}

void db_free(void *ptr)
{
    w_addr_t *p;
    if(ptr != NULL)
    {
        p = (w_uint32_t*)((w_addr_t)ptr - sizeof(w_uint32_t));
        if(p[0] != DB_MEM_FLAG)
            return;
        free(p);
    }
}

