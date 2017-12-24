/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
**
**                                       yzfcer@163.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: db_entry.h
**��   ��   ��: Jason Zhou
**����޸�����: 2017.12.19
**��        ��: 
**              
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: 
** ��  ��: v1.0
** �ա���: 2017.12.19
** �衡��: ԭʼ�汾
**
**--------------��ǰ�汾�޶�----------------------------------------------------------------------------
** �޸���: 
** �ա���: 2017.12.19
** �衡��: 
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
    w_uint32_t base;//���Ե�ַ
    w_uint32_t next_offset;//��һ�����ݿ��ƫ�Ƶ�ַ
    w_int32_t  dbsize;//DB�Ŀռ��С���ֽ�Ϊ��λ
    w_uint16_t blk_cnt;//������
    w_uint16_t blk_size;//���С
    w_uint16_t attr;
    w_int32_t hash;
    w_uint16_t map_offset;//��ӳ���ƫ�Ƶ�ַ
};

#define DB_NEXT(dbinfo) (db_entry_s*)(dbinfo->base+dbinfo->next_offset)

//���ݿ⺯��
db_entry_s *db_entry_get_byname(char *dbname);
w_err_t db_entry_create(char *dbname);
w_err_t db_entry_destroy(char *dbname);
w_err_t db_entry_setattr(char *dbname,w_uint16_t attr);
w_err_t db_entry_getattr(char *dbname,w_uint16_t *attr);
w_err_t db_entry_print(char *dbname);


#endif


