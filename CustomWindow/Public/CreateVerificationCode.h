#pragma once
#include <QString>
#include <QObject>

// CCreateVerificationCode 命令目标
class CCreateVerificationCode //: public QObject
{
protected:
	CCreateVerificationCode();
public:
	virtual ~CCreateVerificationCode();


	class CGarbo // 它的唯一工作就是在析构函数中删除CSingleton的实例
	{
	public:
		~CGarbo()
		{
			if (CCreateVerificationCode::m_Unique)
			{
				delete CCreateVerificationCode::m_Unique;
			}
		}
	};
	static CGarbo Garbo; // 定义一个静态成员，在程序结束时，系统会调用它的析构函数

public:
	static CCreateVerificationCode * GetInstance();
private:
	static CCreateVerificationCode * m_Unique;
	static char code[];
	int CodeNum;         //验证码的位数；

public:
	QString GenerateVerificationCode();
	void SetCodeNum(int t);

};


