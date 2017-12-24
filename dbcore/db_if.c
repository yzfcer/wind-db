/****************************************Copyright (c)**************************************************
**                                       清  风  海  岸
**文   件   名: db_if.c
**创   建   人: Jason Zhou
**最后修改日期: 2017/11/23
**描        述: 
**  
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: Jason Zhou
** 版  本: v1.0
** 日　期: 2017/11/23
** 描　述: 原始版本
**
**--------------当前版本修订----------------------------------------------------------------------------
** 修改人: 
** 日　期: 
** 描　述: 
**
**------------------------------------------------------------------------------------------------------
*******************************************************************************************************/
#include "db.h"
#include "db_if.h"
#include "db_entry.h"
w_int32_t wind_db_create(char *dbname,w_int32_t size)
{
    db_entry_create(dbname);
    return 0;
}
w_int32_t wind_db_distroy(char *dbname)
{
    db_entry_destroy(dbname);
    return 0;
}


w_int32_t wind_tb_create(char *tbname,db_item_info_s *info,w_int32_t item_cnt)
{
    
    return 0;
}

w_int32_t wind_tb_distroy(char *tbname)
{
    return 0;
}


w_int32_t wind_tb_insert(char *tbname,void *row_data,w_int32_t row_size)
{
    return 0;
}
w_int32_t wind_tb_delete(char *tbname,w_int32_t row_idx)
{
    return 0;
}
w_int32_t wind_tb_get_row_index(char *tbname,char *mbrname)
{
    return 0;
}
w_int32_t wind_tb_modify(char *tbname,w_int32_t tbindex,void *row_data,w_int32_t row_size)
{
    return 0;
}

w_int32_t wind_tb_modify_value(char *mbrbname,w_int32_t dataindex,void *data,w_int32_t size)
{
    return 0;
}

w_int32_t wind_tb_query_count(char *tbname,char *cond,w_int32_t *idxlist,w_int32_t cnt)
{
    return 0;
}
w_int32_t wind_tb_getdata(char *dbname,w_int32_t index,void *data)
{
    return 0;
}

