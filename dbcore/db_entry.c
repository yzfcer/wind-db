/****************************************Copyright (c)**************************************************
**                                       清  风  海  岸
**
**                                       yzfcer@163.com
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: db_entry.c
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
#include "db_entry.h"
#include "wind_string.h"
#include "wind_debug.h"
#include "db_adapter.h"
#include "wind_debug.h"
static dlist_s db_list = {NULL,NULL};
static w_int32_t get_db_hash(char *dbname)
{
    w_int32_t i,len;
    w_int32_t hash = 0;
    len = wind_strlen(dbname);
    WIND_ASSERT_RETURN(len < DB_NAME_LEN,-1);
    for(i = 0;i < len;i ++)
        hash += dbname[i];
    return hash;       
}

w_err_t db_entry_create(char *dbname,w_int32_t cnt)
{
    db_entry_s *entry = db_malloc(sizeof(db_entry_s));
    WIND_ASSERT_RETURN(entry != NULL,ERR_MEM);
    entry->magic = DB_MAGIC;
    wind_strncpy(entry->name,dbname,TB_NAME_LEN);
    DNODE_INIT(entry->dbnode);
    DLIST_INIT(entry->tblist);
    entry->base = (w_uint32_t)entry;
    entry->next_offset = 0;
    entry->dbsize = cnt * DB_BLK_SIZE;
    entry->blk_cnt = cnt;
    entry->blk_size = DB_BLK_SIZE;
    entry->hash = get_db_hash(entry->name);
    WIND_ASSERT_RETURN(entry->hash > 0,ERR_INVALID_PARAM);
    dlist_insert_tail(&db_list,&entry->dbnode);
    return B_TRUE;
}

w_err_t db_entry_destroy(char *dbname)
{
    db_entry_s *entry = db_entry_get_byname(dbname);
    WIND_ASSERT_RETURN(entry != NULL,ERR_INVALID_PARAM);
    wind_memset(entry,0,sizeof(db_entry_s));
    db_free(entry);
    return ERR_OK;
}

db_entry_s *db_entry_get_byname(char *dbname)
{
    db_entry_s *entry;
    dnode_s *dnode;
    w_int32_t hash = get_db_hash(dbname);
    WIND_ASSERT_RETURN(hash > 0,NULL);
    foreach_node(dnode,&db_list)
    {
        entry = DLIST_OBJ(dnode,db_entry_s,dbnode);
        if(hash != entry->hash)
            continue;
        if(wind_strcmp(entry->name,dbname) == 0)
            return entry;
    }
    return NULL;
}

w_err_t db_entry_setattr(char *dbname,w_uint16_t attr)
{
    db_entry_s *entry = db_entry_get_byname(dbname);
    WIND_ASSERT_RETURN(entry != NULL,ERR_INVALID_PARAM);
    entry->attr = attr;
    return ERR_OK;
}

w_err_t db_entry_getattr(char *dbname,w_uint16_t *attr)
{
    db_entry_s *entry = db_entry_get_byname(dbname);
    WIND_ASSERT_RETURN(entry != NULL,ERR_INVALID_PARAM);
    *attr = entry->attr;
    return ERR_OK;
}

w_err_t db_entry_print(char *dbname)
{
    db_entry_s *entry = db_entry_get_byname(dbname);
    WIND_ASSERT_RETURN(entry != NULL,ERR_INVALID_PARAM);
    wind_printf("db info:\r\n");
    wind_printf("db name:%s\r\n",entry->name);
    wind_printf("db size:%d\r\n",entry->dbsize);
    wind_printf("db block count:%d\r\n",entry->blk_cnt);
    wind_printf("db block size:%d\r\n",entry->blk_size);
}




