#ifndef DB_TABLE__ENTRY_H_
#define DB_TABLE__ENTRY_H_
#include "db.h"
#include "db_if.h"
#include "wind_type.h"
#include "db_adapter.h"
#include "db_entry.h"



w_err_t table_entry_create(char *tbname,tb_item_info_s *info,w_int32_t item_cnt);
w_err_t table_entry_insert(char *tbname,void *data,w_int32_t datasize);
w_err_t table_entry_delete(char *tbname,w_int32_t row_idx);

w_int32_t wind_tb_delete(char *tbname,w_int32_t row_idx);
w_int32_t wind_tb_get_row_index(char *tbname,char *mbrname);
w_int32_t wind_tb_modify(char *tbname,w_int32_t tbindex,void *row_data,w_int32_t row_size);

//数据字段名称格式为 dbname.tbname.mbrname
w_int32_t wind_tb_modify_value(char *mbrbname,w_int32_t dataindex,void *data,w_int32_t size);
//cond的格式为"mbrname1=value1&&mbr2=value2"类似的格式
w_int32_t wind_tb_query_count(char *tbname,char *cond,w_int32_t *idxlist,w_int32_t cnt);
w_int32_t wind_tb_getdata(char *dbname,w_int32_t index,void *data);

w_err_t table_print_data(char *tbname);
#endif


