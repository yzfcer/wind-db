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
#include "db.h"
#include "db_entry.h"
#include "tb_entry.h"
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

static dlist_s *get_db_list(void)
{
    return &db_list;
}


w_err_t db_entry_create(char *dbname,w_uint16_t attr)
{
    dlist_s *dblist;
    db_entry_s *entry;
    entry = db_get_byname(dbname);
    WIND_ASSERT_RETURN(entry == NULL,ERR_INVALID_PARAM);
    entry = (db_entry_s *)db_malloc(sizeof(db_entry_s));
    WIND_ASSERT_RETURN(entry != NULL,ERR_MEM);
    entry->magic = DB_MAGIC;
    wind_strncpy(entry->name,dbname,TB_NAME_LEN);
    DNODE_INIT(entry->dbnode);
    DLIST_INIT(entry->tblist);
    entry->base = (w_uint32_t)entry;
    entry->attr = attr;
    entry->hash = get_db_hash(entry->name);
    WIND_ASSERT_RETURN(entry->hash > 0,ERR_INVALID_PARAM);
    entry->tb_count = 0;
    dblist = get_db_list();
    dlist_insert_tail(dblist,&entry->dbnode);
    return B_TRUE;
}

w_err_t db_entry_destroy(char *dbname)
{
    dlist_s *dblist;
    dnode_s *node;
    tb_entry_s *tentry;
    db_entry_s *entry = db_get_byname(dbname);
    WIND_ASSERT_RETURN(entry != NULL,ERR_INVALID_PARAM);
    dblist = get_db_list();
    dlist_remove(dblist,&entry->dbnode);
    node = dlist_head(&entry->tblist);
    if(node != NULL)
    {
        foreach_node(node,&entry->tblist)
        {
            node = dlist_remove_head(&entry->tblist);
            tentry = DLIST_OBJ(node,tb_entry_s,tbnode);
            tb_entry_destroy(tentry->tbname);
        }
    }
    wind_memset(entry,0,sizeof(db_entry_s));
    db_free(entry);
    return ERR_OK;
}

db_entry_s *db_get_byname(char *dbname)
{
    dlist_s *dblist;
    db_entry_s *entry;
    dnode_s *dnode;
    w_int32_t hash = get_db_hash(dbname);
    WIND_ASSERT_RETURN(hash > 0,NULL);
    dblist = get_db_list();
    foreach_node(dnode,dblist)
    {
        entry = DLIST_OBJ(dnode,db_entry_s,dbnode);
        if(hash != entry->hash)
            continue;
        if(wind_strcmp(entry->name,dbname) == 0)
            return entry;
    }
    return NULL;
}

w_bool_t db_entry_exist(char *dbname)
{
    db_entry_s *entry;
    entry = db_get_byname(dbname);
    if(entry != NULL)
        return B_TRUE;
    return B_FALSE;
}

w_err_t db_entry_setattr(char *dbname,w_uint16_t attr)
{
    db_entry_s *entry = db_get_byname(dbname);
    WIND_ASSERT_RETURN(entry != NULL,ERR_INVALID_PARAM);
    entry->attr = attr;
    return ERR_OK;
}

w_err_t db_entry_getattr(char *dbname,w_uint16_t *attr)
{
    db_entry_s *entry = db_get_byname(dbname);
    WIND_ASSERT_RETURN(entry != NULL,ERR_INVALID_PARAM);
    *attr = entry->attr;
    return ERR_OK;
}

w_err_t db_entry_insert_tb(db_entry_s *db,tb_entry_s *tb)
{
    WIND_ASSERT_RETURN(db != NULL,ERR_NULL_POINTER);
    WIND_ASSERT_RETURN(tb != NULL,ERR_NULL_POINTER);
    dlist_insert_tail(&db->tblist,&tb->tbnode);
    db->tb_count ++;
    return ERR_OK;
}

w_err_t db_entry_remove_tb(db_entry_s *db,tb_entry_s *tb)
{
    WIND_ASSERT_RETURN(db != NULL,ERR_NULL_POINTER);
    WIND_ASSERT_RETURN(tb != NULL,ERR_NULL_POINTER);
    dlist_remove(&db->tblist,&tb->tbnode);
    db->tb_count --;
    return ERR_OK;
}


w_err_t db_entry_print_info(char *dbname)
{
    db_entry_s *entry = db_get_byname(dbname);
    WIND_ASSERT_RETURN(entry != NULL,ERR_INVALID_PARAM);
    wind_printf("\r\ndb info:\r\n");
    wind_printf("db name:%s\r\n",entry->name);
    wind_printf("table count:%d\r\n",entry->tb_count);
    return ERR_OK;
}

w_err_t db_entry_print_data(db_entry_s *entry)
{
    tb_entry_s *tbentry;
    dnode_s *dnode;
    wind_printf("|---<DB name=%s>\r\n",entry->name);
    foreach_node(dnode,&entry->tblist)
    {
        tbentry = DLIST_OBJ(dnode,tb_entry_s,tbnode);
        tb_entry_print_data(tbentry);
    }
    wind_printf("|---</DB name=%s>\r\n",entry->name);
    return ERR_OK;
}

w_err_t db_entry_print_db(char *dbname)
{
    dlist_s *dblist;
    db_entry_s *entry;
    dnode_s *dnode;
    dblist = get_db_list();
    foreach_node(dnode,dblist)
    {
        entry = DLIST_OBJ(dnode,tb_entry_s,tbnode);
        if(wind_strcmp(entry->name,dbname) == 0)
        {
            db_entry_print_data(entry);
            return ERR_OK;
        }
    }
    return ERR_COMMAN;
}

w_err_t db_entry_print_all(void)
{
    db_entry_s *entry;
    dnode_s *dnode;
    dlist_s *dblist;
    dblist = get_db_list();
    foreach_node(dnode,dblist)
    {
        entry = DLIST_OBJ(dnode,db_entry_s,dbnode);
        db_entry_print_data(entry);
    }
    return ERR_COMMAN;
}




