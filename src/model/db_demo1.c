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


