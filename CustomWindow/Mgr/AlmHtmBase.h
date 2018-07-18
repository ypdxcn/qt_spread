#pragma once

#include "ArrayListMsg.h"
#include "HashtableMsg.h"

#include <QString>
class CAlmHtmBase
{
public:
	CAlmHtmBase(void);
public:
	~CAlmHtmBase(void);

	// 初始化
	virtual bool Initialize( const ArrayListMsg *pAlm /*= NULL */ );

	// 写入数据到文件
	void WriteDataToFile( const QString &csValue, bool bHashTable );

	// 清空配置文件中相关信息
	void EmptyFileInfo( );

protected:
	// 从服务器加载码表
	virtual bool LoadDataFromServer( void ) = 0;

	// 从ArrayListMsg加载，由子类重写
	virtual void LoadFromAlm( const ArrayListMsg &alm ) = 0;

	// 从HashtableMsg加载，由子类重写
	virtual void LoadFromHtm( HashtableMsg &htm ) = 0;

private:
	// 从本地文件加载码表
	bool LoadDataFromFile( void );

	// 获取文件路径
	QString GetFilePath( );

	// 判断本地存储的数据的类型（是HashTable则为true，是ArrayListMsg则为false）
	bool IsHashTable( void );

	//
	QString GetData( void );

protected:
	QString m_csTitle;
	QString m_csTitleType;
	int m_iBufSize;

private:
	bool m_bInit;
};
