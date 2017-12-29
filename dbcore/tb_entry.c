#include "tb_entry.h"
#include "wind_string.h"
#include "wind_debug.h"
#include "db_adapter.h"
#define TB_MAGIC 0xa527e397a167e268
static w_int32_t get_tb_hash(char *tbname)
{
    w_int32_t i,len;
    w_int32_t hash = 0;
    len = wind_strlen(tbname);
    WIND_ASSERT_RETURN(len < DB_NAME_LEN,-1);
    for(i = 0;i < len;i ++)
        hash += tbname[i];
    return hash;       
}

static w_err_t table_name_split(char *combine_name,char *dname,char *tname)
{
    w_int32_t len;
    char *offset = wind_strstr(combine_name,".");
    WIND_ASSERT_RETURN(offset != NULL,ERR_INVALID_PARAM);
    len  = (w_int32_t)(offset - combine_name);
    WIND_ASSERT_RETURN(len < DB_NAME_LEN,ERR_INVALID_PARAM);
    wind_memcpy(dname,combine_name,len);
    dname[len] = 0;
    offset ++;
    len = wind_strlen(offset);
    WIND_ASSERT_RETURN(len < TB_NAME_LEN,ERR_INVALID_PARAM);
    wind_strcpy(tname,offset,len);
    return ERR_OK;
}

w_int32_t get_table_entry_size(w_int32_t item_cnt)
{
    w_int32_t size = sizeof(tb_entry_s);
    size += MBR_NAME_LEN * item_cnt;//item name
    size += sizeof(w_int16_t) * item_cnt;//offset
    size += sizeof(w_int16_t) * item_cnt;//size
    size += sizeof(w_int16_t) * item_cnt;//attr
    return size;
}

tb_entry_s *table_entry_get_byname_from_db(char *dbname,char *tbname)
{
    db_entry_s *dbentry;
    tb_entry_s *tbentry;
    dnode_s *dnode;
    w_int32_t hash = get_tb_hash(tbname);
    WIND_ASSERT_RETURN(hash > 0,NULL);
    dbentry = db_get_byname(dbname);
    WIND_ASSERT_RETURN(dbentry != NULL,NULL);
    foreach_node(dnode,&dbentry->tblist)
    {
        tbentry = DLIST_OBJ(dnode,tb_entry_s,tbnode);
        if(hash != tbentry->hash)
            continue;
        if(wind_strcmp(tbentry->tbname,tbname) == 0)
            return tbentry;
    }
    return NULL;

}


tb_entry_s *table_entry_get_byname(char *combine_name)
{
    w_err_t err;
    char dname[TB_NAME_LEN];
    char tname[TB_NAME_LEN];
    err = table_name_split(combine_name,dname,tname);
    WIND_ASSERT_RETURN(err == ERR_OK,NULL);
    return table_entry_get_byname_from_db(dname,tname);
}

w_int32_t table_get_name_list(tb_entry_s *tbentry,char**name)
{
    *name = db_get_addr(tbentry,tbentry->mbrname_offset);
    return tbentry->item_cnt;
}

w_int32_t table_get_offset_list(tb_entry_s *tbentry,w_int16_t **offset)
{
    *offset = db_get_addr(tbentry,tbentry->offset_offset);
    return tbentry->item_cnt;
}

w_int32_t table_get_size_list(tb_entry_s *tbentry,w_int16_t **size)
{
    *size = db_get_addr(tbentry,tbentry->size_offset);
    return tbentry->item_cnt;
}

w_int32_t table_get_attr_list(tb_entry_s *tbentry,w_int16_t **attr)
{
    *attr = db_get_addr(tbentry,tbentry->attr_offset);
    return tbentry->item_cnt;
}


w_err_t table_entry_create(char *tbname,tb_item_info_s *item_info,w_int32_t item_cnt)
{
    w_int32_t i;
    w_err_t err;
    w_uint16_t offset;
    char *name_base;
    w_uint16_t *pos,*psize,*pattr;
    w_int32_t size;
    tb_entry_s *entry;
    
    size = get_table_entry_size(item_cnt);
    entry = db_malloc(size);
    WIND_ASSERT_RETURN(entry != NULL,ERR_MEM);
    
    entry->magic = TB_MAGIC;
    err = table_name_split(tbname,entry->dbname,entry->tbname);
    WIND_ASSERT_RETURN(err == ERR_OK,ERR_INVALID_PARAM);
    entry->base = (w_uint32_t)entry;
    entry->db = db_get_byname(entry->dbname);
    DNODE_INIT(entry->tbnode);


    entry->entry_size = size;
    entry->item_cnt = item_cnt;
    entry->hash = get_tb_hash(entry->tbname);

    DLIST_INIT(entry->data_list);
    entry->data_cnt = 0;
    entry->data_size = item_info[item_cnt-1].offset + item_info[item_cnt-1].size;
    
    offset = sizeof(tb_entry_s);
    entry->mbrname_offset = offset;
    offset += MBR_NAME_LEN * entry->item_cnt;
    entry->offset_offset = offset;
    offset += sizeof(item_info->offset) * entry->item_cnt;
    entry->size_offset = offset;
    offset += sizeof(item_info->size) * entry->item_cnt;
    entry->attr_offset = offset;
    offset += sizeof(item_info->attr) * entry->item_cnt;
    entry->entry_size = offset;
    
    name_base = (char*)db_get_addr(entry,entry->mbrname_offset);
    pos = (w_uint16_t*)db_get_addr(entry,entry->offset_offset);
    psize = (w_uint16_t*)db_get_addr(entry,entry->size_offset);
    pattr = (w_uint16_t*)db_get_addr(entry,entry->attr_offset);
    for(i = 0;i < entry->item_cnt;i ++)
    {
        wind_strncpy(name_base,item_info[i].name,MBR_NAME_LEN);
        name_base += MBR_NAME_LEN;
        pos[i] = item_info[i].offset;
        psize[i] = item_info[i].size;
        pattr[i] = item_info[i].attr;
    }
    dlist_insert_tail(&entry->db->tblist,&entry->tbnode);
    return B_TRUE;
}

w_err_t table_entry_insert(char *tbname,void *data,w_int32_t datasize)
{
    w_int32_t size;
    tb_entry_s *entry;
    dnode_s *dnode;
    entry = table_entry_get_byname(tbname);
    WIND_ASSERT_RETURN(entry != NULL,ERR_INVALID_PARAM);
    size = sizeof(dnode_s) + entry->data_size;
    dnode = db_malloc(size);
    WIND_ASSERT_RETURN(dnode != 0,ERR_MEM);
    wind_memcpy(db_get_addr(dnode,sizeof(dnode_s)),data,entry->data_size);
    dlist_insert_tail(&entry->data_list,dnode);
    return ERR_OK;
}

w_err_t table_entry_delete(char *tbname,w_int32_t row_idx)
{
    w_int32_t size;
    tb_entry_s *entry;
    dnode_s *dnode;
    w_int32_t idx = 0;
    WIND_ASSERT_RETURN(row_idx >= 0,ERR_INVALID_PARAM);
    entry = table_entry_get_byname(tbname);
    WIND_ASSERT_RETURN(entry != NULL,ERR_INVALID_PARAM);
    WIND_ASSERT_RETURN(row_idx < entry->data_cnt,ERR_INVALID_PARAM);
    foreach_node(dnode,&entry->data_list)
    {
        if(idx == row_idx)
        {
            dnode = dlist_remove(&entry->data_list,dnode);
            db_free((void *)dnode);
            return ERR_OK;
        }
    }
    return ERR_COMMAN;
}

w_err_t table_entry_get_data(char *tbname,w_int32_t row_idx,void *data,w_int32_t data_size)
{
    w_int32_t size;
    tb_entry_s *entry;
    dnode_s *dnode;
    w_int32_t idx = 0;
    WIND_ASSERT_RETURN(row_idx >= 0,ERR_INVALID_PARAM);
    entry = table_entry_get_byname(tbname);
    WIND_ASSERT_RETURN(entry != NULL,ERR_INVALID_PARAM);
    WIND_ASSERT_RETURN(row_idx < entry->data_cnt,ERR_INVALID_PARAM);
    WIND_ASSERT_RETURN(data_size == entry->data_size,ERR_INVALID_PARAM);
    foreach_node(dnode,&entry->data_list)
    {
        if(idx == row_idx)
        {
            dnode = dlist_remove(&entry->data_list,dnode);
            wind_memcpy(data,db_get_addr(dnode,sizeof(dnode_s)),entry->data_size);
            return ERR_OK;
        }
    }
    return ERR_COMMAN;
}

w_err_t table_entry_modify(char *tbname,w_int32_t row_idx,void *data,w_int32_t data_size)
{
    w_int32_t size;
    tb_entry_s *entry;
    dnode_s *dnode;
    w_int32_t idx = 0;
    WIND_ASSERT_RETURN(row_idx >= 0,ERR_INVALID_PARAM);
    entry = table_entry_get_byname(tbname);
    WIND_ASSERT_RETURN(entry != NULL,ERR_INVALID_PARAM);
    WIND_ASSERT_RETURN(row_idx < entry->data_cnt,ERR_INVALID_PARAM);
    WIND_ASSERT_RETURN(data_size == entry->data_size,ERR_INVALID_PARAM);
    foreach_node(dnode,&entry->data_list)
    {
        if(idx == row_idx)
        {
            dnode = dlist_remove(&entry->data_list,dnode);
            wind_memcpy(db_get_addr(dnode,sizeof(dnode_s)),data,entry->data_size);
            return ERR_OK;
        }
    }
    return ERR_COMMAN;
}
w_err_t table_entry_query_count(char *tbname,w_int32_t *count)
{
    w_int32_t size;
    tb_entry_s *entry;
    dnode_s *dnode;
    w_int32_t idx = 0;
    entry = table_entry_get_byname(tbname);
    WIND_ASSERT_RETURN(entry != NULL,ERR_INVALID_PARAM);
    *count = entry->data_cnt;
    return ERR_OK;
}



w_err_t table_entry_modify_value(char *tbname,char *mbrbname,w_int32_t row_idx,void *data,w_int32_t size)
{
    return ERR_COMMAN;
}

w_err_t table_entry_query_cond_count(char *tbname,char *cond,w_int32_t *idxlist,w_int32_t cnt)
{
    return ERR_COMMAN;
}

w_err_t table_print_data(char *tbname)
{
    char *name;
    tb_entry_s *entry = table_entry_get_byname(tbname);
    WIND_ASSERT_RETURN(entry != NULL,ERR_INVALID_PARAM);
    wind_printf("table=%s\r\n",entry->tbname);
    name = table_get_name_list(entry,*name);
    
}

