#include "CreateVerificationCode.h"
#include <QTime>

// CCreateVerificationCode

CCreateVerificationCode::CCreateVerificationCode()
{
	CodeNum = 6;
}

CCreateVerificationCode::~CCreateVerificationCode()
{
}
// CCreateVerificationCode 成员函数

CCreateVerificationCode * CCreateVerificationCode::m_Unique = NULL;
// 去掉0和o，因为两个字符会混淆
char CCreateVerificationCode::code[] = 
{
	'1','2','3','4','5','6','7','8','9',
	'a','b','c','d','e','f','g','h','i','j','k','l','m','n','p','q','r','s','t','u','v','w','x','y','z',
	'A','B','C','D','E','F','G','H','I','J','K','L','M','N','P','Q','R','S','T','U','V','W','X','Y','Z',
};


CCreateVerificationCode * CCreateVerificationCode::GetInstance ()
{
	if (NULL == m_Unique)
	{
		m_Unique = new CCreateVerificationCode;
	}

	return m_Unique;
}

QString CCreateVerificationCode::GenerateVerificationCode() 
{
	QString str,str1;
	int i = 0, range;

	//kenny  2017/11/10
	//srand((unsigned)timeGetTime());  //产生随机数种子
	QTime time = QTime::currentTime();
	qsrand((uint)time.msec());
	//kenny  2017/11/14 
	//QString *str =tr("%1")).arg(rand() % 101);
	//srand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

	range = sizeof(code)/sizeof(code[0]);
	for (i = 0; i < CodeNum; i++)
	{
		str1.clear();
		str1 = code[rand()%range];
		str += str1;
	}

	return str;
}

void CCreateVerificationCode::SetCodeNum (int t)
{
	if (t >0 && t < 5)
	{
		CodeNum = t; 
	}
}
