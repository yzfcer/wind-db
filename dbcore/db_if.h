/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
**��   ��   ��: db_if.h
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
#ifndef DB_IF_H__
#define DB_IF_H__
#include "wind_type.h"

typedef struct
{
    char *name;
    w_uint16_t offset;
    w_uint16_t size;
    w_uint16_t attr;
}db_item_info_s;

//�ֶ����Զ���
#define DB_ATTR_KEY (0x01<<0)//�ɶ�
#define DB_ATTR_RD (0x01<<1)//�ɶ�
#define DB_ATTR_WR (0x01<<2) //��д
#define DB_ATTR_CRYPT (0x01<<3) //����
#define DB_ATTR_VISIBLE (0x01<<4) //�ɼ�
#define DB_ATTR_NOT_NULL (0x01<<5) //���ܿ�
#define DB_ATTR_FLUSH (0x01<<6) //���ܿ�

#define DB_ATTR_KEY_ITEM (DB_ATTR_KEY  | DB_ATTR_RD | DB_ATTR_VISIBLE | DB_ATTR_NOT_NULL)//��ֵ
#define DB_ATTR_DEFAULT_ITEM (DB_ATTR_RD | DB_ATTR_WR | DB_ATTR_VISIBLE)//Ĭ���ֶ�����
#define DB_ATTR_READONLY (DB_ATTR_RD | DB_ATTR_VISIBLE | DB_ATTR_NOT_NULL)
#define DB_ATTR_RAMONLY (DB_ATTR_RD | DB_ATTR_VISIBLE | DB_ATTR_NOT_NULL)



//�ж������Ƿ�����
#define TABLE_ATTR_EQ(attr,va) (((attr)&(va))==(va))
#define TABLE_IS_KEY(attr) TABLE_ATTR_EQ((attr),DB_ATTR_KEY)
#define TABLE_IS_RD(attr) TABLE_ATTR_EQ((attr),DB_ATTR_RD)
#define TABLE_IS_WR(attr) TABLE_ATTR_EQ((attr),DB_ATTR_WR)
#define TABLE_IS_CRYPT(attr) TABLE_ATTR_EQ((attr),DB_ATTR_CRYPT)
#define TABLE_IS_VISIBLE(attr) TABLE_ATTR_EQ((attr),DB_ATTR_VISIBLE)
#define TABLE_IS_NOT_NULL(attr) TABLE_ATTR_EQ((attr),DB_ATTR_NOT_NULL)


//���ݱ���
#define TABLE_OFFSET(tb_type,mbr) (w_int32_t)(&(((tb_type*)0)->mbr))
#define TABLE_DEF(tb_type) db_item_info_s db_info_list_##tb_type[] = {
#define TABLE_DECLARE(tb_type) extern db_item_info_s db_info_list_##tb_type[];
#define TABLE_ITEM(tb_type,mbr_type,mbr) {#mbr,TABLE_OFFSET(tb_type,mbr),sizeof(mbr_type),DB_ATTR_DEFAULT_ITEM},
#define TABLE_ITEM_A(tb_type,mbr_type,mbr,tb_attr) {#mbr,TABLE_OFFSET(tb_type,mbr),sizeof(mbr_type),tb_attr},
#define TABLE_END };
#define TABLE_PARA(tb_type) #tb_type,db_info_list_##tb_type,sizeof(db_info_list_##tb_type)/sizeof(db_item_info_s)


//���ݿ⺯��
w_int32_t wind_db_create(char *dbname,w_int32_t size);
w_int32_t wind_db_distroy(char *dbname);

//���ݱ���,���ݱ����Ƹ�ʽΪ dbname.tbname
w_int32_t wind_tb_create(char *tbname,db_item_info_s *info,w_int32_t item_cnt);
w_int32_t wind_tb_distroy(char *tbname);

//���ݲ�������,
w_int32_t wind_tb_insert(char *tbname,void *row_data,w_int32_t row_size);
w_int32_t wind_tb_delete(char *tbname,w_int32_t row_idx);
w_int32_t wind_tb_get_row_index(char *tbname,char *mbrname);
w_int32_t wind_tb_modify(char *tbname,w_int32_t tbindex,void *row_data,w_int32_t row_size);
//�����ֶ����Ƹ�ʽΪ dbname.tbname.mbrname
w_int32_t wind_tb_modify_value(char *mbrbname,w_int32_t dataindex,void *data,w_int32_t size);
//cond�ĸ�ʽΪ"mbrname1=value1&&mbr2=value2"���Ƶĸ�ʽ
w_int32_t wind_tb_query_count(char *tbname,char *cond,w_int32_t *idxlist,w_int32_t cnt);
w_int32_t wind_tb_getdata(char *dbname,w_int32_t index,void *data);

#endif

