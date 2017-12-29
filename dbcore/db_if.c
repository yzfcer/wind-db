/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
**��   ��   ��: db_if.c
**��   ��   ��: Jason Zhou
**����޸�����: 2017/11/23
**��        ��: 
**  
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: Jason Zhou
** ��  ��: v1.0
** �ա���: 2017/11/23
** �衡��: ԭʼ�汾
**
**--------------��ǰ�汾�޶�----------------------------------------------------------------------------
** �޸���: 
** �ա���: 
** �衡��: 
**
**------------------------------------------------------------------------------------------------------
*******************************************************************************************************/
#include "db.h"
#include "db_if.h"
#include "db_entry.h"
w_int32_t wind_db_create(char *dbname,w_int32_t size)
{
    db_create(dbname);
    return 0;
}

w_int32_t wind_db_distroy(char *dbname)
{
    db_destroy(dbname);
    db_print(dbname);
    return 0;
}


w_int32_t wind_tb_create(char *tbname,tb_item_info_s *item_info,w_int32_t item_cnt)
{
    table_entry_create(tbname,item_info,item_cnt);
    return 0;
}

w_int32_t wind_tb_distroy(char *tbname)
{
    return 0;
}


w_int32_t wind_tb_insert(char *tbname,void *row_data,w_int32_t row_size)
{
    return table_entry_insert(tbname,row_data,row_size);
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

