/****************************************Copyright (c)**************************************************
**                                       清  风  海  岸
**
**                                       yzfcer@163.com
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: db_entry.h
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
#ifndef DB_ENTRY_H_
#define DB_ENTRY_H_
#include "wind_type.h"
#include "db.h"
#include "wind_dlist.h"
#define DB_MAGIC 0xa5253867a16725b6


typedef struct __db_entry_s db_entry_s;
struct __db_entry_s
{
    w_uint64_t magic;
    char name[DB_NAME_LEN];
    dnode_s dbnode;
    dlist_s tblist;
    w_uint32_t base;//绝对地址
    w_uint32_t next_offset;//下一个数据库的偏移地址
    w_int32_t  dbsize;//DB的空间大小，字节为单位
    w_uint16_t blk_cnt;//块数量
    w_uint16_t blk_size;//块大小
    w_uint16_t attr;
    w_int32_t hash;
    w_uint16_t map_offset;//块映射表偏移地址
};

#define DB_NEXT(dbinfo) (db_entry_s*)(dbinfo->base+dbinfo->next_offset)

//数据库函数
db_entry_s *db_entry_get_byname(char *dbname);
w_err_t db_entry_create(char *dbname);
w_err_t db_entry_destroy(char *dbname);
w_err_t db_entry_setattr(char *dbname,w_uint16_t attr);
w_err_t db_entry_getattr(char *dbname,w_uint16_t *attr);
w_err_t db_entry_print(char *dbname);


#endif


