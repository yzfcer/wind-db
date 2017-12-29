/****************************************Copyright (c)**************************************************
**                                       清  风  海  岸
**
**                                       yzfcer@163.com
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: db.c
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
#ifndef DB_H__
#define DB_H__

#include "wind_type.h"
#include "wind_dlist.h"
#define DB_BLK_SIZE 512
#define DB_NAME_LEN 12  //数据库名字长度
#define TB_NAME_LEN 12  //数据表名字长度
#define MBR_NAME_LEN 20  //数据名字长度


typedef struct __db_entry_s db_entry_s;
typedef struct __tb_entry_s tb_entry_s;


struct __db_entry_s
{
    w_uint64_t magic;
    char name[DB_NAME_LEN];
    dnode_s dbnode;
    dlist_s tblist;
    w_uint32_t base;//绝对地址
    w_uint16_t attr;
    w_int32_t hash;
    w_int32_t tb_count;
};

struct __tb_entry_s
{
    w_uint64_t magic;
    char dbname[DB_NAME_LEN];
    char tbname[TB_NAME_LEN];
    w_uint32_t base;//绝对地址
    db_entry_s *db;
    dnode_s tbnode;
    
    w_uint32_t entry_size;
    w_uint16_t item_cnt;
    w_int32_t hash;
    
    dlist_s data_list;
    w_uint16_t data_cnt;
    w_uint16_t data_size;
    //偏移地址
    w_int16_t mbrname_offset;
    w_int16_t offset_offset;
    w_int16_t size_offset;
    w_int16_t attr_offset;
};

typedef struct
{
    w_uint16_t msgid;
    char dbname[DB_NAME_LEN];
    w_int32_t dbsize;
}db_msg_s;


#define db_get_addr(base,offset) (w_addr_t)((offset)+(w_addr_t)(base))
#define db_get_offset(base,addr) (w_uint32_t)((w_addr_t)(addr)+(w_addr_t)(base))
#endif
