#include "stdlib.h"
#include "db_if.h"
#include "db_entry.h"
#include "tb_entry.h"
#include "db_demo.h"
#include "db_demo1.h"
#include "wind_string.h"


void create_tb_demo(void)
{
    int i;
	tb_demo demo;
	int idx[10] = {0};
	short c = 24;
	w_int32_t cnt;
	demo.key =1;
	demo.ademo[0] = 2;
	demo.ademo[1] = 5;
	demo.bdemo = 3;
	demo.cdemo = 4;
    for (i = 0;i < 10;i ++)
        idx[i] = 0x12345670+i;
	wind_db_create("db1",DB_ATTR_RAMONLY);
    db_entry_print_info("db1");
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
    demo.bdemo = 125;
    wind_tb_modify_value("db1.tb_demo","bdemo",1,&demo.bdemo,sizeof(demo.bdemo));
    db_entry_print_info("db1");
	tb_entry_print_info("db1.tb_demo");
}


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
    wind_strcpy(demo1.strdemo,"demo1 64 bytes string.");
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


void main(void)
{
	create_tb_demo();
	create_tb_demo1();
    db_entry_print_all();
        
	system("pause");
}
