#ifndef DB_TABLE__ENTRY_H_
#define DB_TABLE__ENTRY_H_
#include "db.h"
#include "db_if.h"
#include "wind_type.h"


#define TB_MAGIC 0xa527e397a167e268


typedef struct
{
    w_uint64_t magic;
    char dbname[DB_NAME_LEN];
    char tbname[TB_NAME_LEN];
    w_uint32_t base;//¾ø¶ÔµØÖ·
    w_uint32_t entry_size;
    w_uint16_t item_cnt;
    //Æ«ÒÆµØÖ·
    w_int16_t mbrname_offset;
    w_int16_t offset_offset;
    w_int16_t size_offset;
    w_int16_t attr_offset;
    w_int32_t data_offset;
}table_entry_s;
table_entry_s *table_entry_get_byname(char *tbname);
w_bool_t table_entry_create(char *tbname,db_item_info_s *info,w_int32_t item_cnt);
w_int32_t table_get_name_list(table_entry_s *tbentry,char**name);
w_int32_t table_get_offset_list(table_entry_s *tbentry,w_int16_t **offset);
w_int32_t table_get_size_list(table_entry_s *tbentry,w_int16_t **size);
w_int32_t table_get_attr_list(table_entry_s *tbentry,w_int16_t **attr);

#endif


