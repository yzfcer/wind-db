/****************************************Copyright (c)**************************************************
**                                       清  风  海  岸
**
**                                       yzfcer@163.com
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: db_adapter.c
**创   建   人: Jason Zhou
**最后修改日期: 2017.12.19
**描        述: 
**              
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: 
** 版  本: v1.0
** 日　期: 2017.12.19
** 描　述: 原始版本
**
**--------------当前版本修订----------------------------------------------------------------------------
** 修改人: 
** 日　期: 2017.12.19
** 描　述: 
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


