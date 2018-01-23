/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
**��   ��   ��: tb_demo1.c
**��   ��   ��: Jason Zhou
**����޸�����: 2017/11/23
**��        ��: 
**  
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: Jason Zhou
** ��  ��: v1.0
** �ա���: 2017/11/23
** �衡��: ԭʼ�汾
**
**--------------��ǰ�汾�޶�----------------------------------------------------------------------------
** �޸���: 
** �ա���: 
** �衡��: 
**
**------------------------------------------------------------------------------------------------------
*******************************************************************************************************/
#include "db_if.h"
#include "db_demo1.h"
#include "db_entry.h"
//�������ݿ�ṹ

TABLE_START(tb_demo1)
TABLE_ITEM_A(tb_demo1,w_int32_t,key,DB_ATTR_KEY_ITEM)
TABLE_ITEM(tb_demo1,w_int32_t,ademo1)
TABLE_ITEM(tb_demo1,w_int8_t,bdemo1)
TABLE_ITEM(tb_demo1,w_int16_t,cdemo1)
TABLE_ITEM(tb_demo1,w_bool_t,bodemo1)
TABLE_ITEM(tb_demo1,w_str64_t,strdemo)
TABLE_END
TABLE_DEF(tb_demo1,DB_ATTR_SINGLE)

void create_tb_demo1(void)
{
    int i;
	tb_demo1 demo1;
	int idx[10] = {0};
	short c = 24;
	int cnt;
	demo1.key =1;
	demo1.ademo1[0] = 2;
	demo1.ademo1[1] = 5;
	demo1.bdemo1 = 3;
	demo1.cdemo1 = 4;
    wind_memset(&demo1,0,sizeof(demo1));
    for (i = 0;i < 10;i ++)
        idx[i] = 0x12345670+i;
	wind_db_create("db1",DB_ATTR_RAMONLY);
    db_entry_print_info("db1");
	wind_tb_create("db1.tb_demo1",TABLE_PARA(tb_demo1));
	wind_tb_insert("db1.tb_demo1",&demo1,sizeof(demo1));
	demo1.key = 2;
    demo1.bodemo1 = B_FALSE;
    wind_strcpy(demo1.strdemo,"demo1 test.");
	wind_tb_insert("db1.tb_demo1",&demo1,sizeof(demo1));

	wind_tb_query_count("db1.tb_demo1",&cnt);

	wind_tb_getdata("db1.tb_demo1",1,&demo1,sizeof(demo1));
	demo1.ademo1[1] = 23;
	wind_tb_modify("db1.tb_demo1",1,&demo1,sizeof(demo1));
    demo1.bdemo1 = 125;
    wind_tb_modify_value("db1.tb_demo1","bdemo1",1,&demo1.bdemo1,sizeof(demo1.bdemo1));
    db_entry_print_info("db1");
	tb_entry_print_info("db1.tb_demo1");
}


