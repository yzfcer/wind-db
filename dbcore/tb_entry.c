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


static w_int32_t get_table_entry_size(w_int32_t item_cnt)
{
    w_int32_t size = sizeof(tb_entry_s);
    size = (((size + 7) >> 3) << 3);
    size += MBR_NAME_LEN * item_cnt;//item name
    size = (((size + 7) >> 3) << 3);
    size += sizeof(w_int8_t) * item_cnt;//type
    size = (((size + 7) >> 3) << 3);
    size += sizeof(w_int8_t) * item_cnt;//count
    size = (((size + 7) >> 3) << 3);
    size += sizeof(w_int16_t) * item_cnt;//offset
    size = (((size + 7) >> 3) << 3);
    size += sizeof(w_int16_t) * item_cnt;//size
    size = (((size + 7) >> 3) << 3);
    size += sizeof(w_int16_t) * item_cnt;//attr
    size = (((size + 7) >> 3) << 3);
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

static w_uint16_t set_offset(w_uint16_t idx,w_uint16_t count,w_uint16_t *va,w_uint32_t size)
{
    w_int32_t si;
    *va = (((idx + 7) >> 3) << 3);
    si = (((size * count + 7) >> 3) << 3); 
    return si;
}

w_err_t table_entry_create(char *tbname,tb_item_info_s *item_info,w_int32_t item_cnt)
{
    w_int32_t i;
    w_err_t err;
    w_uint16_t offset;
    char *name_base;
    w_uint8_t *type,*count;
    w_uint16_t *pos,*psize,*pattr;
    w_int32_t size;
    tb_entry_s *entry;
    
    size = get_table_entry_size(item_cnt);
    entry = db_malloc(size);
    WIND_ASSERT_RETURN(entry != NULL,ERR_MEM);
    wind_memset(entry,0,size);
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
    offset = (((offset + 7) >> 3) << 3);
    offset += set_offset(offset,entry->item_cnt,&entry->mbrname_offset,MBR_NAME_LEN);
    offset += set_offset(offset,entry->item_cnt,&entry->type_offset,sizeof(item_info->type));
    offset += set_offset(offset,entry->item_cnt,&entry->count_offset,sizeof(item_info->count));
    offset += set_offset(offset,entry->item_cnt,&entry->offset_offset,sizeof(item_info->offset));
    offset += set_offset(offset,entry->item_cnt,&entry->size_offset,sizeof(item_info->size));
    offset += set_offset(offset,entry->item_cnt,&entry->attr_offset,sizeof(item_info->attr));
    
    name_base = (char*)db_get_addr(entry,entry->mbrname_offset);
    type = (w_uint8_t*)db_get_addr(entry,entry->type_offset);
    count = (w_uint8_t*)db_get_addr(entry,entry->count_offset);
    pos = (w_uint16_t*)db_get_addr(entry,entry->offset_offset);
    psize = (w_uint16_t*)db_get_addr(entry,entry->size_offset);
    pattr = (w_uint16_t*)db_get_addr(entry,entry->attr_offset);
    for(i = 0;i < entry->item_cnt;i ++)
    {
        wind_strncpy(name_base,item_info[i].name,MBR_NAME_LEN);
        name_base += MBR_NAME_LEN;
        type[i] = item_info[i].type;
        count[i] = item_info[i].count;
        pos[i] = item_info[i].offset;
        psize[i] = item_info[i].size;
        pattr[i] = item_info[i].attr;
    }
    //dlist_insert_tail(&entry->db->tblist,&entry->tbnode);
    err = db_insert_tb(entry->db,entry);
    WIND_ASSERT_RETURN(err == ERR_OK,ERR_COMMAN);
    return B_TRUE;
}

w_err_t table_entry_destroy(char *tbname)
{
    dnode_s *dnode;
    w_err_t err;
    tb_entry_s *entry;
    //wind_printf("************ERROR**************\r\n");
    entry = table_entry_get_byname(tbname);
    WIND_ASSERT_RETURN(entry != NULL,ERR_INVALID_PARAM);
    WIND_ASSERT_RETURN(entry->db != NULL,ERR_NULL_POINTER);
    err = db_remove_tb(entry->db,entry);
    WIND_ASSERT_RETURN(err == ERR_OK,ERR_COMMAN);
    foreach_node(dnode,&entry->data_list)
    {
        dnode = dlist_remove(&entry->data_list,dnode);
        db_free((void *)dnode);
    }
    db_free((void *)entry);
    return ERR_OK;
}

w_err_t table_entry_insert(char *tbname,void *data,w_int32_t datasize)
{
    w_int32_t size;
    tb_entry_s *entry;
    dnode_s *dnode;
    entry = table_entry_get_byname(tbname);
    WIND_ASSERT_RETURN(entry != NULL,ERR_INVALID_PARAM);
    WIND_ASSERT_RETURN(entry->data_cnt < 65535,ERR_COMMAN);
    size = sizeof(dnode_s) + entry->data_size;
    dnode = db_malloc(size);
    WIND_ASSERT_RETURN(dnode != 0,ERR_MEM);
    wind_memcpy(db_get_addr(dnode,sizeof(dnode_s)),data,entry->data_size);
    dlist_insert_tail(&entry->data_list,dnode);
    entry->data_cnt ++;
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
    WIND_ASSERT_RETURN(entry->data_cnt > 0,ERR_COMMAN);
    foreach_node(dnode,&entry->data_list)
    {
        if(idx == row_idx)
        {
            dnode = dlist_remove(&entry->data_list,dnode);
            db_free((void *)dnode);
            entry->data_cnt --;
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
    WIND_ASSERT_RETURN(data_size >= entry->data_size,ERR_INVALID_PARAM);
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
    WIND_ASSERT_RETURN(data_size >= entry->data_size,ERR_INVALID_PARAM);
    foreach_node(dnode,&entry->data_list)
    {
        if(idx == row_idx)
        {
            //dnode = dlist_remove(&entry->data_list,dnode);
            wind_memcpy(db_get_addr(dnode,sizeof(dnode_s)),data,entry->data_size);
            return ERR_OK;
        }
        idx ++;
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

w_err_t table_entry_print_info(char *tbname)
{
    tb_entry_s *entry = table_entry_get_byname(tbname);
    WIND_ASSERT_RETURN(entry != NULL,ERR_INVALID_PARAM);
    wind_printf("\r\ntable info:\r\n");
    wind_printf("DB    name :%s\r\n",entry->dbname);
    wind_printf("table name :%s\r\n",entry->tbname);
    wind_printf("entry size :%d\r\n",entry->entry_size);
    wind_printf("entry hash :%d\r\n",entry->hash);
    wind_printf("item count :%d\r\n",entry->item_cnt);
    wind_printf("data count :%d\r\n",entry->data_cnt);
    wind_printf("data  size :%d\r\n",entry->data_size);

    wind_printf("name   offset:%d\r\n",entry->mbrname_offset);
    wind_printf("type   offset:%d\r\n",entry->type_offset);
    wind_printf("count  offset:%d\r\n",entry->count_offset);
    wind_printf("offset offset:%d\r\n",entry->offset_offset);
    wind_printf("size   offset:%d\r\n",entry->size_offset);
    wind_printf("attr   offset:%d\r\n",entry->attr_offset);
}

w_err_t table_entry_print_data(char *tbname)
{
    w_int32_t idx = 0;
    dnode_s *dnode;
    w_uint8_t *data;
    tb_entry_s *entry = table_entry_get_byname(tbname);
    WIND_ASSERT_RETURN(entry != NULL,ERR_INVALID_PARAM);
    WIND_ASSERT_RETURN(entry->magic == TB_MAGIC,ERR_INVALID_PARAM);
    wind_printf("<DB name=%s.%s>\r\n",entry->dbname,entry->tbname);
    foreach_node(dnode,&entry->data_list)
    {
        data = (w_uint8_t *)&dnode[1];
        wind_printf("   <table index=%d>\r\n",idx);
        table_print_data(entry,data);
        wind_printf("   </table index=%d>\r\n",idx);
        idx ++;
    }
    wind_printf("</DB name=%s.%s>\r\n",entry->dbname,entry->tbname);
    return ERR_OK;
}

