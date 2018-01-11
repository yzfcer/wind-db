/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
**��   ��   ��: tb_demo.c
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
#include "db_demo.h"
#include "db_entry.h"
//�������ݿ�ṹ

TABLE_START(tb_demo)
TABLE_ITEM_A(tb_demo,w_int32_t,key,DB_ATTR_KEY_ITEM)
TABLE_ITEM(tb_demo,w_int32_t,ademo)
TABLE_ITEM(tb_demo,w_int8_t,bdemo)
TABLE_ITEM(tb_demo,w_int16_t,cdemo)
TABLE_END
TABLE_DEF(tb_demo,DB_ATTR_SINGLE)

void create_tb_demo(void)
{
    int i;
	tb_demo demo;
	int idx[10] = {0};
	short c = 24;
	int cnt;
	demo.key =1;
	demo.ademo[0] = 2;
	demo.ademo[1] = 5;
	demo.bdemo = 3;
	demo.cdemo = 4;
    for (i = 0;i < 10;i ++)
        idx[i] = 0x12345670+i;
	wind_db_create("db1",DB_ATTR_RAMONLY);
    db_print_info("db1");
	wind_tb_create("db1.tb_demo",TABLE_PARA(tb_demo));
	wind_tb_insert("db1.tb_demo",&demo,sizeof(demo));
	demo.key = 2;
	wind_tb_insert("db1.tb_demo",&demo,sizeof(demo));
	demo.key = 3;
	wind_tb_insert("db1.tb_demo",&demo,sizeof(demo));

	wind_tb_query_count("db1.tb_demo",&cnt);

	wind_tb_getdata("db1.tb_demo",1,&demo,sizeof(demo));
	demo.ademo[1] = 23;
	wind_tb_modify("db1.tb_demo",1,&demo,sizeof(demo));
    db_print_info("db1");
	table_entry_print_info("db1.tb_demo");
    table_entry_print_data("db1.tb_demo");
}


