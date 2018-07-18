#include "HJGlobalFun.h"

#include <sstream> // 应用QStringstream需要包含该文件
#include <cmath>

#include <time.h>
#include <app.h>




#pragma execution_character_set("utf-8")


//// 配置列表外观
//void CHJGlobalFun::SetListCtrl( CGuiGridCtrl& listCtrl, int iItemHeight )
//{
//	listCtrl.SetTextFormat(DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
//	listCtrl.SetItemHeight(iItemHeight);
//    listCtrl.SetEvenColor(RGB(255, 255, 255));
//    listCtrl.SetOddColor(RGB(0xff, 0xff, 0xe0));
//    listCtrl.SetSelectedColor(RGB(0xc0, 0xf0, 0xec));
//    listCtrl.SetBorderColor(RGB(0xff, 0xff, 0xff));
//	((CGuiGridHeaderCtrl*)listCtrl.GetHeaderCtrl())->SetTextColor(RGB(0, 0, 0));
//	((CGuiGridHeaderCtrl*)listCtrl.GetHeaderCtrl())->SetBkgndColor(RGB(0xf0, 0xf3, 0xf5));
//	((CGuiGridHeaderCtrl*)listCtrl.GetHeaderCtrl())->SetSplitterColor(RGB(0xa9, 0xa9, 0xa9));
//}
//// 添加list的标题，并设置标题的宽度和对齐方式
//void CHJGlobalFun::AddListColumn( CGuiGridCtrl& listCtrl, int iCount, QString* psTitle,int* piAlign, int* piWidth )
//{
//	for(int i = 0; i < iCount; i++)
//	{
//		listCtrl.InsertColumn(i, *psTitle++, *piAlign++, *piWidth++);
//	}
//}
bool CHJGlobalFun::CheckFloat(QString &sFloat)
{
	int iLen = sFloat.length();
	if(iLen != 0)
	{
		QChar ch;
		bool bHasPoint = false;
		for(int i = 0; i < iLen; i++)
		{
			ch = sFloat.at(i);
			if(ch < '0' || ch > '9') // 如果不是数字
			{
				if(ch == '-' && i == 0 && iLen > 1) // 如果是负号，而且是第一个，并且字符串长度大于1，则通过
				{
					continue;
				}
				else if(ch == '.' && !bHasPoint) // 如果是小数点，而且还没出现过
				{
					// 不对小数位的位置做判断，解决服务器发送过来的".11"的错误
					bHasPoint = true;
					continue;

					/*bHasPoint = true;
					if(i == 0 || i == iLen -1)
					{
						return false;
					}
					else
					{
						continue;
					}*/
				}
				else
				{
					return false;
				}
			}
		}
	}
	else
	{
		return false;
	}

	return true;

}
// 格式化浮点数，添加了对黄金行情数据的分支处理
QString CHJGlobalFun::FormatFloat( QString strFloat, bool b45 /*= true */ )
{
	/*
	// 空处理
	if(strFloat == "" || strFloat == "-")//如果为空，则返回空
	{
		if(bQuotation)
		{
			return "0.00";
		}
		else
		{
			return strFloat;
			//return "";
		}
	}
	*/

	// 科学计数法处理，有的报文以这种形式
	int iFindIndexE = strFloat.indexOf('E');
	if(iFindIndexE != -1)
	{
		return FormatFloat(DealKeXue(strFloat, iFindIndexE));
	}

	// 如果不合法，则直接返回
	if(!CheckFloat(strFloat))
	{
		return strFloat;
	}

	QString zheng,xiao,temp;
	int index;

	QString cstr = strFloat;//转换成QString类型，为了方便处理

	//符号位处理，如果为整数，则为空，如果为负数，则处理的时候去掉负数，在最后加上
	QString fuhao = "";
	if(cstr.at(0) == '-')//如果为负数
	{
		//去掉负号，记录正负
		cstr = cstr.mid(1);
		fuhao = "-";
	}

	int len = cstr.length();//获得长度
	index =  cstr.indexOf(".");//获得小数点位置

	//处理小数部分
	if(index == -1)//如果为整数，则在后面添加默认小数位"00"
	{
		xiao = "00";
	}
	else//如果为小数
	{
		xiao = cstr.mid(index + 1);//获取小数部分
		if(len - index == 2)//如果为一位小数，则补齐一个0（要不要考虑只有小数点，没有小数位的）
		{
			xiao += "0";
		}	
		else if(len - index >= 4)
		{
			// 如果小数位大于3位，则截取2位
			if(b45)
			{
				cstr = Float45(QStringToQString(cstr.left(index)), QStringToQString(xiao), 2);
				// 由于处理四舍五入后数字可能存在变化，故重新查找
				index =  cstr.indexOf(".");//获得小数点位置
				xiao = cstr.mid(index + 1);//获取小数部分
			}
			else
			{
				xiao = xiao.left(2);
			}
		}
	}

	//处理整数部分，temp用来记录整数部分
	if(index == -1)
	{
		temp = cstr;
	}
	else
	{
		temp = cstr.left(index);//整数部分
		if(temp == "")//如果整数部分为空的话（处理-.23这种情况）
		{
			return fuhao + "0." + xiao;
		}
		len  = index;//记录整数部分的长度到len
	}

	//添加千位分隔符
	int count = -1;
	if(len % 3 != 0)//如果不为3整除
		count++;
	count = len/3 + count;
	zheng = temp.left(len - 3*count);
	while(count)
	{
		zheng += "," + temp.mid(len - 3*count, 3);
		count--;
	}
	return fuhao + zheng + "." + xiao;
}



// 格式化浮点数，添加了对黄金行情数据的分支处理
QString CHJGlobalFun::FormatFloatQuote(QString strFloat, bool b45 /*= true */)
{
	/*
	// 空处理
	if(strFloat == "" || strFloat == "-")//如果为空，则返回空
	{
	if(bQuotation)
	{
	return "0.00";
	}
	else
	{
	return strFloat;
	//return "";
	}
	}
	*/

	// 科学计数法处理，有的报文以这种形式
	int iFindIndexE = strFloat.indexOf('E');
	if (iFindIndexE != -1)
	{
		return FormatFloat(DealKeXue(strFloat, iFindIndexE));
	}

	// 如果不合法，则直接返回
	if (!CheckFloat(strFloat))
	{
		return strFloat;
	}

	QString zheng, xiao, temp;
	int index;

	QString cstr = strFloat;//转换成QString类型，为了方便处理

							//符号位处理，如果为整数，则为空，如果为负数，则处理的时候去掉负数，在最后加上
	QString fuhao = "";
	if (cstr.at(0) == '-')//如果为负数
	{
		//去掉负号，记录正负
		cstr = cstr.mid(1);
		fuhao = "-";
	}

	int len = cstr.length();//获得长度
	index = cstr.indexOf(".");//获得小数点位置

							  //处理小数部分
	if (index == -1)//如果为整数，则在后面添加默认小数位"00"
	{
		xiao = "00";
	}
	else//如果为小数
	{
		xiao = cstr.mid(index + 1);//获取小数部分
		if (len - index == 2)//如果为一位小数，则补齐一个0（要不要考虑只有小数点，没有小数位的）
		{
			xiao += "0";
		}
		else if (len - index >= 4)
		{
			// 如果小数位大于3位，则截取2位
			if (b45)
			{
				cstr = Float45(QStringToQString(cstr.left(index)), QStringToQString(xiao), 2);
				// 由于处理四舍五入后数字可能存在变化，故重新查找
				index = cstr.indexOf(".");//获得小数点位置
				xiao = cstr.mid(index + 1);//获取小数部分
			}
			else
			{
				xiao = xiao.left(2);
			}
		}
	}

	//处理整数部分，temp用来记录整数部分
	if (index == -1)
	{
		temp = cstr;
	}
	else
	{
		temp = cstr.left(index);//整数部分
		if (temp == "")//如果整数部分为空的话（处理-.23这种情况）
		{
			return fuhao + "0." + xiao;
		}
		len = index;//记录整数部分的长度到len
	}

	//添加千位分隔符
	//int count = -1;
	//if (len % 3 != 0)//如果不为3整除
	//	count++;
	//count = len / 3 + count;
	//zheng = temp.left(len - 3 * count);
	//while (count)
	//{
	//	zheng += "," + temp.mid(len - 3 * count, 3);
	//	count--;
	//}

	zheng = temp;
	return fuhao + zheng + "." + xiao;
}



QString CHJGlobalFun::DoubleToQString( double dValue, bool b45 /*= true*/ )
{
	//QStringstream   ss;   
	//ss << dValue;
	//return ss.str();

	if(b45) // 四舍五入
	{
		// 当数据是98705.144999999990，服务器为98705.15，这种方式格式为
		if( dValue > 0.0045 )
		{
			dValue += 0.005;
		}
		else
		{
			dValue -= 0.005;
		}
	}

	// 由于这里不对数据进行四舍五入
	if(dValue > -0.01 && dValue < 0.01)
	{
		dValue = 0.00;
	}

	QString sValue;
	char cTmp[64];
	sprintf(cTmp, "%f",dValue);
	sValue = cTmp;
	return sValue;
}

// 处理黄金服务器返回的科学计数法的转换
QString CHJGlobalFun::DealKeXue( QString &strFloatALL, int iFindIndexE )
{
#ifdef _VERSION_JH
	// 交行版本 "0E-10"
	int iWei;
	int iZheng;

	// 获得乘以10的个数，也就是E后面的数字
	int iLen = strFloatALL.length();
	QString strWei   = strFloatALL.substr(iFindIndexE+1, iLen-iFindIndexE);
	QStringstream ss(strWei); 
	ss >> iWei; 

	// 获得整数部分（即科学计数法E前面的数字）
	QStringstream ss2(strFloatALL.substr(0, iFindIndexE)); 
	ss2 >> iZheng;
	double dValue = pow((double)10, (double)iWei);

	// 获得原始的数据
	dValue *= iZheng;

	// 转换为字符串
	QStringstream ss3;
	ss3 << dValue;
	return ss3.str();
#else
	QString strZheng;
	QString strXiao;
	int iWei=0;
	QString strWei;

	int iLen = strFloatALL.length();

	//乘以10的个数
	strWei   = strFloatALL.mid(iFindIndexE+1, iLen-iFindIndexE);
	/*QStringstream ss(strWei); 
	ss >> iWei; */

	//获得还原后的整数部分和小数部分
	int iFindIndexD = strFloatALL.indexOf('.');
	int nXiaoBegin = iFindIndexD+1+iWei; // 获取小数位的开始位置
	if( nXiaoBegin < iFindIndexE ) // 解决没有小数位的情况，如："1.0E7"
	{
		strZheng = strFloatALL.mid(0, iFindIndexD) + strFloatALL.mid(iFindIndexD+1, iWei);
	}
	else
	{
		strZheng = strFloatALL.mid(0, iFindIndexD) + strFloatALL.mid(iFindIndexD+1, iFindIndexE-iFindIndexD-1);
		strZheng.insert(nXiaoBegin-iFindIndexE, '0');
	}
		

#ifdef _VERSION_GF
	/*
	 解决金联通卡余额的原始数据9.89709008E7展示为“9.8970900.8F”的问题
	*/
	
	// 判断是否超过E的位置
	if( nXiaoBegin < iFindIndexE )
		strXiao  = strFloatALL.substr(nXiaoBegin, iFindIndexE-nXiaoBegin);
	else
		strXiao = "00";
	
	return strZheng + "." + strXiao;
#else
	strXiao  = strFloatALL.mid(iFindIndexD+1+iWei, iFindIndexE-iFindIndexD-1);
	return Float45(strZheng, strXiao, 2);
#endif
	
#endif
}
//保留iReserve位小数，实现精确到最后一位
QString CHJGlobalFun::Float45( QString &strZheng, QString &strXiao, int iReserve )
{
	char   cTemp;
	QString strXiaoF;
	int    izhengF;

	////将整数部分转换为int型
	//QStringstream ss1(strZheng); 
	//ss1 >> izhengF; 
	//int iXiaoLen = strXiao.length();

	////获得保留位范围内的小数部分的字符串
	//strXiaoF = strXiao.substr(0, iReserve);

	////查找小数部分中iReserve位后第一个不是4的位置
	//int iCount = iReserve;
	//cTemp = strXiao.at(iCount);
	//while(cTemp == '4' && iCount < iXiaoLen-1)
	//{
	//	iCount++;
	//	cTemp = strXiao.at(iCount);
	//}

	////如果结束字符串大于4，则
	//if(cTemp > '4')
	//{
	//	izhengF += QStringXiaoAdd1(strXiaoF);
	//}

	//QStringstream ss;
	//ss << izhengF;
	//QString strFinal = ss.str();
	//strFinal += "." + strXiaoF;

	//return strFinal;
	return 1;
}
/*
将小数部分加1，如果有进位到整数的话，则返回1，否则返回0
比如：strFloat = "4445"，执行后strFloat为"4446"
*/
int CHJGlobalFun::QStringXiaoAdd1( QString &strFloat )
{
	int  iLen    = strFloat.length();
	int  iCount  = iLen -1;
	int  iReturn = 0;

	//找到从右边开始第一个不是9的数，并且将这之前的全部置为0，比如"8999"，则变为"9000"
	QChar cTemp  = strFloat.at(iCount);
	while(cTemp == '9' && iCount > 0)
	{
		// 将为9的数字置0，因为加1了
		strFloat[iCount] = '0'; //将该位置0
		iCount--;
		cTemp = strFloat.at(iCount);
	}

	//如果是第0个，且cTemp是9，则返回值为1，否则为0
	if(iCount == 0 && cTemp == '9')
	{
		strFloat[iCount]= '0';
		iReturn++;	
	}
	else//字符串加1
	{
		//cTemp++;
		strFloat[iCount] = cTemp;
	}

	return iReturn;
}
// 
QString CHJGlobalFun::QStringToQString( QString &cstr )
{
	QString str(cstr.data()); //QString to QString
	cstr.clear();
	return str;
}


// 将原始字符串Str以分割字符串SplitWord分割到Vector(Vec)里面
//void CHJGlobalFun::SplitStrToVector( QString Str, QString SplitWord , QVector<QString> &Vec, bool bAddSplit /*= true*/, bool bRemoveBlank /*= true*/ )
//{
//	int index; // 记录字符串第一个分隔符的位置
//	QString temp;
//	int iSplitterLen = SplitWord.length();
//
//	if(bAddSplit)
//	{
//		Str += SplitWord; // 添加分割符，不然可能遗漏掉最后一个，可能原始的字符串的字符串没有在最后面添加分隔符
//	}
//	
//	index = Str.indexOf(SplitWord);
//	while(index != -1)
//	{
//		// 获得分隔符之前的字符串，即目标字符串
//		temp = Str.left(index);
//
//		// 如果不需要去掉空格 或者 字符串不为空
//		if(!bRemoveBlank || temp != "")
//		{
//			Vec.push_back(temp);
//		}
//
//		// 获得分隔符后的字符串，作为新的处理字符串
//		Str   = Str.mid(index + iSplitterLen);
//
//		// 获得新的字符串的第一个分隔符的位置
//		index = Str.indexOf(SplitWord);
//	}
//}

void CHJGlobalFun::SplitStrToVector( QString Str, QString SplitWord , QVector<int> &Vec, bool bAddSplit /*= true*/, bool bRemoveBlank /*= true*/ )
{
	int index; // 记录字符串第一个分隔符的位置
	QString temp;
	int iSplitterLen = SplitWord.length();

	if(bAddSplit)
	{
		Str += SplitWord; // 添加分割符，不然可能遗漏掉最后一个，可能原始的字符串的字符串没有在最后面添加分隔符
	}

	index = Str.indexOf(SplitWord);
	while(index != -1)
	{
		temp = Str.left(index);
		if(!bRemoveBlank || temp != "")
		{
			int iValue = temp.toInt();
			Vec.push_back(iValue);
		}

		Str   = Str.mid(index + iSplitterLen); //注意SplitWord是字符串的话，要再这里添加字符串的长度
		index = Str.indexOf(SplitWord);
	}

}

void CHJGlobalFun::SplitStrToVector( QString &sOrg, const QString &sSplitter , QVector<QString> &Vec, bool bAddSplit /*= true*/, bool bRemoveBlank /*= true*/ )
{
	int index; // 记录字符串第一个分隔符的位置
	QString temp;

	int iSplitterLen = sSplitter.length();

	if(bAddSplit)
	{
		sOrg += sSplitter; // 添加分割符，不然可能遗漏掉最后一个，可能原始的字符串的字符串没有在最后面添加分隔符
	}

	index = sOrg.indexOf(sSplitter);
	while(index != -1)
	{
		temp = sOrg.mid(0, index);
		if( !bRemoveBlank || !temp.isEmpty() )
		{
			Vec.push_back(temp);
		}

		sOrg   = sOrg.mid(index + iSplitterLen); //注意SplitWord是字符串的话，要再这里添加字符串的长度
		index = sOrg.indexOf(sSplitter);
	}
}

// 判断一个字符串是否全部是由数字组成
bool CHJGlobalFun::IsNumber( const QString &sNum )
{
	// 判断是否是数字
	for(int i = 0; i < sNum.length(); i++)
	{
		// 利用0-9的数字对应一定范围的*码
		QChar mychar = sNum.at(i);// 获取QString某一个位置上的字符
		if(mychar < 48 || mychar > 57) // 判断是否在0-9之间
			return false;
	}

	return true;
}
// 功能：获取Datetimepicker控件的时间，并转换为"20100201"格式的字符串
//QString CHJGlobalFun::GetFormatTimeStr( CDateTimeCtrl &dtctrl )
//{
//	CTime tt;
//	dtctrl.GetTime(tt);
//	return tt.Format("%Y%m%d");
//}
/*
格式化时间字符串，使得为1980-02-01的格式
input：value必须是标准的格式形式19800201
return：1980-02-01的格式
*/
QString CHJGlobalFun::FormatDate( QString sDateOrg )
{
	if(sDateOrg.length() == 8)
		sDateOrg = sDateOrg.mid(0, 4) + "-"+ sDateOrg.mid(4, 2) + "-" + sDateOrg.mid(6, 2);

	return sDateOrg;
}
// 格式化日期字符串，将格式为120601的形式格式化为12:06:01的形式
QString CHJGlobalFun::FormatTime( QString sTimeOrg )
{
	if(sTimeOrg.length() == 6)
		sTimeOrg = sTimeOrg.mid(0, 2) + ":" + sTimeOrg.mid(2, 2) +  ":" + sTimeOrg.mid(4, 2);

	return sTimeOrg;
}
// 将一个日期字符串转换成CTime类型
QTime CHJGlobalFun::TranslateDateStrToCTime( QString sDate )
{
	QString cstrDate = sDate;
	if(cstrDate.length() != 8)
	{
		return QTime(0,0);
	}
	else
	{
		QTime timeTime;//(cstrDate.left(4).toInt(), atoi(cstrDate.mid(4, 2)), atoi(cstrDate.mid(6, 2)), 0, 0, 0);
		return timeTime;
	}
}

QTime CHJGlobalFun::TranslateDateStrToCTime( const QString &sDate )
{
	if(sDate.length() != 8)
	{
		return QTime(0,0,0,0);//("yyyy-MM-dd hh:mm:ss ddd"));
	}
	else
	{
		QTime timeTime;
		//timeTime.fromString()
		//(sDate.left(4).toInt(), sDate.mid(4, 2).toInt(), sDate.mid(6, 2).toInt(), 0, 0, 0);
		//timeTime.
		return timeTime;
	}
}

// 根据买卖方向ID获取买卖方向名称
QString CHJGlobalFun::GetBuySellName( const QString &sID )
{
	if(sID == "s")
	{
		return "卖";
	}
	else if(sID == "b")
	{
		return "买";
	}
	return sID;
}

QString CHJGlobalFun::IntToQString( int iValue )
{
	QString cstr= QString::number(iValue);
	//cstr.Format("%d", iValue);
	return cstr;
}

//void CHJGlobalFun::PairAdd( QVector< QPair<QString,QString> > & v, QString sTitle, QString sValue )
//{
//	QPair<QString,QString> p;
//	p.first  = sTitle;
//	p.second = sValue;
//	v.push_back(p);
//}

bool CHJGlobalFun::IsDoubleZero( double dValue )
{
	return dValue-0.0 < 1e-6 ? true : false;
}

// 格式化
double CHJGlobalFun::DoubleFormat( double dValue, int iScale /*= 2*/, EDoubleFormatMode eMode /*= ROUND_HALF_UP*/ )
{
	// 异常则返回原值
	if(iScale < 0)
	{
		return dValue;
	}

	// 比如dValue = 100.45434，iScale = 2 且是ROUND_HALF_UP，则iFactor=1000，UINT64 u64 = (UINT64)(dValue * iFactor) + 5 = 100459；u64 = u64 / 10=10045；
	int iFactor = 10;
	while(iScale-- > 0)
	{
		iFactor *= 10;
	}

	quint64 u64 = (quint64)(dValue * iFactor);

	bool bInvert = false;
	if(u64 < 0)
	{
		bInvert = true;
		u64 = -u64;
	}

	if(eMode ==  ROUND_HALF_UP)
	{
		u64 += 5;
	}
	else // ROUND_UP
	{
		u64 += 9;
	}

	// 去掉整数部分的最后一位
	u64 = u64 / 10;

	if(bInvert) u64 = -u64;

	return (double)u64 / (iFactor/10);
}

double CHJGlobalFun::DoubleFormatNoNine(double dValue, int iScale)
{
    // 异常则返回原值
    if(iScale < 0)
    {
        return dValue;
    }

    // 比如dValue = 100.45434，iScale = 2 且是ROUND_HALF_UP，则iFactor=1000，UINT64 u64 = (UINT64)(dValue * iFactor) + 5 = 100459；u64 = u64 / 10=10045；
    int iFactor = 10;
    while(iScale-- > 0)
    {
        iFactor *= 10;
    }

    quint64 u64 = (quint64)(dValue * iFactor);

    bool bInvert = false;
    if(u64 < 0)
    {
        bInvert = true;
        u64 = -u64;
    }

    int n = u64%10;
    if (n < 9 && n >= 5)
    {
        dValue += 0.005;
    }

    return dValue;
}

int CHJGlobalFun::DoubleToInt( double dValue )
{
	return (int)(dValue + 0.5);
}

// 判断一个字符串是否是连续的
bool CHJGlobalFun::IsContinuousNumber( const QString &sNum )
{
	int iLen = sNum.length()-1;

	// 如果小于2位，则视为不连续
	if(iLen < 1)
		return false;

	if(sNum.at(iLen) > sNum.at(0)) // 如果最后一位比第一位大，则视为可能的递增
	{
		for(int i = 0; i < iLen; i++)
		{
			// 当前位如果不比后面位小1，则不为连续
			if(sNum.at(i)+1 != sNum.at(i+1))
				return false;
		}
	}
	else
	{
		for(int i = iLen; i > 0; i--)
		{
			if(sNum.at(i)+1 != sNum.at(i-1))
				return false;
		}
	}

	return true;
}

// 判断一个字符串是否有iCount个连续的相同字符
bool CHJGlobalFun::IsHasContinuousSameWord( const QString &sNum, int iCount )
{
	if(sNum.length() < iCount)
		return false;

	int iFinalCount = 1;
	for(int i = 0; i < sNum.length()-1; i++)
	{
		// 
		if(sNum.at(i+1) == sNum.at(i))
		{
			iFinalCount++;

			if(iFinalCount >= iCount)
			{
				return true;
			}
		}
		else
		{
			iFinalCount=1;
		}	
	}

	return false;
}

int CHJGlobalFun::CompareDouble( double dValue1, double dValue2 )
{
	double dDif = fabs(dValue1 - dValue2);
	double dMinCom = 0.000001;

	if(dDif < dMinCom || dDif == dMinCom)
	{
		return 0;
	}
	else
	{
		return (dValue1 - dValue2 > 0.001) ? 1 : -1;
	}
}

bool CHJGlobalFun::IsEmail( const QString &strValue )
{
	//// 判断是否为空
	//if(strValue.IsEmpty())
	//{
	//	return true;
	//}

	int iPos = strValue.indexOf("@");
	if(iPos == -1)
	{
		return false;
	}
	else
	{
		// 判断是否有两个@
		int iPos2 = strValue.indexOf("@", iPos+1);
		if(iPos2 != -1)
		{
			return false;
		}
	}

	int iLen = strValue.length();

	// 判断"@"是否在第一位或者最后一位
	if(iPos == 0 || iPos == iLen-1)
	{
		return false;
	}

	return true;
}

bool CHJGlobalFun::IsPhone( const QString &strValue )
{
	//// 判断是否为空
	//if(strValue == "")
	//{
	//	return true;
	//}

	// 判断是否都是数字或者"-"号
	for( int i = 0; i < strValue.length(); i++ )
	{
		QChar cData = strValue.at(i);

		if(cData != '-')
		{
			if( cData < '0' || cData > '9' )
			{
				return false;
			}
		}
	}

	return true;
}

std::string CHJGlobalFun::GetRandNumber( short nCount )
{
	srand((unsigned int)time(NULL));

	// 获取7位的随机数
	unsigned int iFinalValue = 0;
	unsigned int iTemp = 1;
	for( short i = 0; i < nCount; i++ )
	{
		iFinalValue += rand()%(10)*iTemp;
		iTemp *= 10;
	}

	std::stringstream ss;
	ss << iFinalValue;
	return ss.str(); 
	//return "";
}

//2016.05.20 12:17:01.445 周五
//yyyy表示年；MM表示月；dd表示日； hh表示小时；mm表示分；ss表示秒；zzz表示毫秒；ddd表示周几
QString CHJGlobalFun::GetCurTimeFormat()
{
	// 获得当前时间，并且格式化
	QDateTime current_date_time = QDateTime::currentDateTime();
	//QString current_date = current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
	QString current_time = current_date_time.time().toString("hh:mm:ss");

	return current_time;
	/*SYSTEMTIME st;
	GetLocalTime(&st);
	QString csTime;
	csTime.Format("%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	return csTime.GetBuffer();
	return  "";*/
}

QString CHJGlobalFun::GetCurDateFormat()
{
	//// 获得当前时间，并且格式化
	//SYSTEMTIME st;
	//GetLocalTime(&st);
	//QString csTime;
	//csTime.Format("%04d%02d%02d", st.wYear, st.wMonth, st.wDay);

	//return csTime.GetBuffer();
	return "";
}

/*
根据exe文件的目录运行一个exe文件
input：fileName文件的完整路径
return：启动成功返回true，启动失败返回false
*/
bool CHJGlobalFun::RunFileExe( const QString &csFullPath )
{
	//QString csTemp(csFullPath);
	//// 尝试两种启动进程的方式，解决在某种环境下启动进程失败的问题
	//if( !RunCreateProcess(csTemp.GetBuffer()) )
	//{
	//	QString csExePath;
	//	QString csPara;
	//	int nPos = csFullPath.Find(" ");
	//	if( nPos != -1 )
	//	{
	//		csExePath = csFullPath.Left(nPos);
	//		csPara = csFullPath.mid(nPos+1);
	//	}
	//	else
	//	{
	//		csExePath = csFullPath;
	//	}

	//	ShellExecute( NULL, "open", csExePath, csPara, NULL, NULL );
	//}

	return true;
}

//bool CHJGlobalFun::RunCreateProcess( const LPSTR pPath )
//{
//	PROCESS_INFORMATION piProcInfo;
//
//	STARTUPINFO Info;
//	Info.cb          = sizeof(STARTUPINFO);
//	Info.lpReserved  = NULL;
//	Info.lpDesktop   = NULL;
//	Info.lpTitle     = NULL;
//	Info.cbReserved2 = 0;
//	Info.lpReserved2 = NULL;
//
//	BOOL bSuccess = CreateProcess(NULL, pPath, NULL, NULL,
//		FALSE, NULL, NULL, NULL, &Info, &piProcInfo);
//
//	return bSuccess ? true : false;
//}
//
//bool CHJGlobalFun::RunShellExecute( const char* pPath, const char* pPara /*= "" */ )
//{
//	ShellExecute( NULL, "open", pPath, pPara, NULL, NULL );
//
//	return true;
//}

QString CHJGlobalFun::getHostIpAddress()
{
	QString strIpAddress;
	auto ipAddressesList = QNetworkInterface::allAddresses();
	// 获取第一个本主机的IPv4地址  
	int nListSize = ipAddressesList.size();
	for (int i = 0; i < nListSize; ++i)
	{
		if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
			ipAddressesList.at(i).toIPv4Address()) {
			strIpAddress = ipAddressesList.at(i).toString();
			break;
		}
	}
	// 如果没有找到，则以本地IP地址为IP  
	if (strIpAddress.isEmpty())
		strIpAddress = QHostAddress(QHostAddress::LocalHost).toString();
	return strIpAddress;
}



 QString CHJGlobalFun::str2qstr(const std::string& str)
{
	return QString::fromLocal8Bit(str.data());
}

std::string CHJGlobalFun::qstr2str(const QString & qstr)
{
	QByteArray cdata = qstr.toLocal8Bit();
	return std::string(cdata);
}



void CHJGlobalFun::ReadUserChoose(const QString &csPath, const QString &csTitle, bool bName, QVector<QString> &vecUserNames)
{
	//
	QVector<QString> vecAllNames;
	QVector<int> vecColumnIndexs;
	//char buf[2000] = { 0 };
	QString  buf;
	QString csAllTitle = bName ? "AllColumnNames" : "AllColumnIDs";
	// 读取全部字段名称，并分割到vector
	//::GetPrivateProfileString("Info", csAllTitle, "", buf, sizeof(buf), csPath);
	App::GetPriProfileString(csPath, "Info", csAllTitle, buf);


	CHJGlobalFun::SplitStrToVector(buf, QUERY_INI_SPLITER, vecAllNames);

	// 读取位置字段，并且分割到vector
	//::GetPrivateProfileString("Info", csTitle, "", buf, sizeof(buf), csPath);
	App::GetPriProfileString(csPath, "Info", csTitle, buf);

	CHJGlobalFun::SplitStrToVector(buf, QUERY_INI_SPLITER, vecColumnIndexs);

	// 加载当前显示项
	for (int i = 0; i < vecColumnIndexs.size(); i++)
	{
		vecUserNames.push_back(vecAllNames.at(vecColumnIndexs.at(i)));
	}
}

//
void CHJGlobalFun::GetIniValueToVector(const QString &csPath, const QString &csMainTitle, const QString &csTitle, const QString &csSplitter, const QString &csDefaulut, QVector<QString> &vecValue)
{
	//
	QString buf;

	App::GetPriProfileString(csPath, csMainTitle, csTitle,  buf);

	CHJGlobalFun::SplitStrToVector(buf, csSplitter, vecValue);
}
