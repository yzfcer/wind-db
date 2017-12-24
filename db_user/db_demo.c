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

TABLE_DEF(db_demo)
TABLE_ITEM_A(db_demo,int,k,DB_ATTR_KEY_ITEM)
TABLE_ITEM(db_demo,int,a)
TABLE_ITEM(db_demo,char,b)
TABLE_ITEM(db_demo,short,c)
TABLE_END

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
	wind_db_create("db1",0x100000);
	wind_tb_create("db1",TABLE_PARA(db_demo));
	wind_tb_insert("db1.db_demo",&demo,sizeof(demo));
	demo.k = 2;
	wind_tb_insert("db1.db_demo",&demo,sizeof(demo));
	demo.k = 3;
	wind_tb_insert("db1.db_demo",&demo,sizeof(demo));

	cnt = wind_tb_query_count("db1.db_demo","",idx,10);

	wind_tb_getdata("db1.db_demo",1,&demo);
	demo.a[1] = 23;
	wind_tb_modify("db1.db_demo",1,&demo,sizeof(demo));
	
	wind_tb_modify("db1.db_demo.c",2,&demo.c,sizeof(demo.c));
}


