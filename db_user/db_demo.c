/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
**��   ��   ��: db_demo.c
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
//�������ݿ�ṹ

TABLE_START(db_demo)
TABLE_ITEM_A(db_demo,int,k,DB_ATTR_KEY_ITEM)
TABLE_ITEM(db_demo,int,a)
TABLE_ITEM(db_demo,char,b)
TABLE_ITEM(db_demo,short,c)
TABLE_END
TABLE_DEF(db_demo,DB_ATTR_SINGLE)

void create_tb_demo(void)
{
    int i;
	db_demo demo;
	int idx[10] = {0};
	short c = 24;
	int cnt;
	demo.k =1;
	demo.a[0] = 2;
	demo.a[1] = 5;
	demo.b = 3;
	demo.c = 4;
    for (i = 0;i < 10;i ++)
        idx[i] = 0x12345670+i;
	wind_db_create("db1",DB_ATTR_RAMONLY);
	//wind_db_create("db1",DB_ATTR_RAMONLY);
    db_print_info("db1");
	wind_tb_create("db1.tb1",TABLE_PARA(db_demo));
	wind_tb_insert("db1.tb1",&demo,sizeof(demo));
	demo.k = 2;
	wind_tb_insert("db1.tb1",&demo,sizeof(demo));
	demo.k = 3;
	wind_tb_insert("db1.tb1",&demo,sizeof(demo));

	wind_tb_query_count("db1.tb1",&cnt);

	wind_tb_getdata("db1.tb1",1,&demo,sizeof(demo));
	demo.a[1] = 23;
	wind_tb_modify("db1.tb1",1,&demo,sizeof(demo));
    db_print_info("db1");
	table_entry_print_info("db1.tb1");
	//wind_tb_modify("db1.tb1",2,&demo.c,sizeof(demo.c));
}


