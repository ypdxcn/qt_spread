#if !defined(CONST_H)
#define CONST_H

#include <QString>

const QString gc_ErrCodeSus("00000000");  // 应答报文成功的标识
const QString gc_ErrCode_ConSverFail("CON00001");  // 应答报文成功的标识

const QString gc_YesNo_Yes("1");
const QString gc_YesNo_No("0");

const QString gc_EmptyStr("");

const QString gc_BSFlag_Buy("b");   // 买入
const QString gc_BSFlag_Sell("s");  // 卖出

const QString gc_OffsetFlag_Open("0");       // 开仓
const QString gc_OffsetFlag_Cov("1");        // 平仓
const QString gc_OffsetFlag_ForceCov("2");   // 强平
const QString gc_OffsetFlag_TodayCov("3");   // 平今 

#endif