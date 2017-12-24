/****************************************Copyright (c)**************************************************
**                                       清  风  海  岸
**文   件   名: db_demo.h
**创   建   人: Jason Zhou
**最后修改日期: 2017/11/23
**描        述: 
**  
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: Jason Zhou
** 版  本: v1.0
** 日　期: 2017/11/23
** 描　述: 原始版本
**
**--------------当前版本修订----------------------------------------------------------------------------
** 修改人: 
** 日　期: 
** 描　述: 
**
**------------------------------------------------------------------------------------------------------
*******************************************************************************************************/
#ifndef DB_DEMO_H__
#define DB_DEMO_H__
#include "db_if.h"
typedef struct 
{
	int k;
	int a[2];
	char b;
	short c;
}db_demo;

TABLE_DECLARE(db_demo);

#endif


