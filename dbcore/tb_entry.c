#include "tb_entry.h"
#include "wind_string.h"
#include "wind_debug.h"
static w_err_t tbentry_name_init(table_entry_s *entry,char *tbname)
{
    w_int32_t len;
    char *offset = wind_strstr(tbname,".");
    WIND_ASSERT_RETURN(offset != NULL,ERR_INVALID_PARAM);
    len  = (w_int32_t)(offset - tbname);
    WIND_ASSERT_RETURN(len < DB_NAME_LEN,ERR_INVALID_PARAM);
    wind_memcpy(entry->dbname,tbname,len);
    entry->dbname[len-1] = 0;
    offset ++;
    len = wind_strlen(offset);
    WIND_ASSERT_RETURN(len < TB_NAME_LEN,ERR_INVALID_PARAM);
    wind_strcpy(entry->tbname,offset,len);
    return ERR_OK;
}

w_bool_t table_entry_create(char *tbname,db_item_info_s *item_info,w_int32_t item_cnt)
{
    w_int32_t i;
    w_uint16_t offset;
    char *name_base;
    w_uint16_t *pos,*psize,*pattr;
    table_entry_s *entry = db_malloc(sizeof(table_entry_s));
    WIND_ASSERT_RETURN(entry != NULL,ERR_MEM);
    entry->magic = TB_MAGIC;
    //wind_strncpy(entry->tbname,tbname,TB_NAME_LEN);
    tbentry_name_init(entry,tbname);
    entry->base = (w_uint32_t)entry;
    entry->item_cnt = item_cnt;
    offset = sizeof(table_entry_s);
    entry->mbrname_offset = offset;
    offset += sizeof(item_info->name) * entry->item_cnt;
    entry->offset_offset = offset;
    offset += sizeof(item_info->name) * entry->item_cnt;
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
    return B_TRUE;
}

table_entry_s *table_entry_get_byname(char *tbname)
{
    
}

w_int32_t table_get_name_list(table_entry_s *tbentry,char**name)
{
    *name = db_get_addr(tbentry,tbentry->mbrname_offset);
    return tbentry->item_cnt;
}

w_int32_t table_get_offset_list(table_entry_s *tbentry,w_int16_t **offset)
{
    *offset = db_get_addr(tbentry,tbentry->offset_offset);
    return tbentry->item_cnt;
}

w_int32_t table_get_size_list(table_entry_s *tbentry,w_int16_t **size)
{
    *size = db_get_addr(tbentry,tbentry->size_offset);
    return tbentry->item_cnt;
}

w_int32_t table_get_attr_list(table_entry_s *tbentry,w_int16_t **attr)
{
    *attr = db_get_addr(tbentry,tbentry->attr_offset);
    return tbentry->item_cnt;
}


