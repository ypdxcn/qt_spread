#ifndef _ARRAY_LIST_MSG_H
#define _ARRAY_LIST_MSG_H

#include <cassert>
#include <string>
#include <sstream>
#include <iomanip> 
#include <vector>
#include "CommAp.h"
#include "Constant.h"

using namespace std;
using namespace Const;

///////////////////////////////////////////////////////////////////////////////
//根据程世禄交易服务器类修改
///////////////////////////////////////////////////////////////////////////////
class COMMAP_CLASS ArrayListMsg
{
	friend std::ostream& operator<<(std::ostream&, const ArrayListMsg&);
	friend std::istream& operator>>(std::istream&, ArrayListMsg&);
private:
	typedef enum 
	{
		E_STRING,
		E_ARRAYLIST
	} eMsgType;

	typedef struct tagPosInfo
	{
		eMsgType eType;
		size_t nPos;
	} POS_INFO;

public:
	ArrayListMsg();
	~ArrayListMsg();	
	//----------------------------------------------------------------
	// 根据字符串进行初始化解析的构造函数
	// 输入参数： pStrMsg  需要进行解析的字符串指针
	ArrayListMsg(const string& sStrMsg,int nLevel = 0);

	ArrayListMsg(const ArrayListMsg& oCopy);
	void operator=(const ArrayListMsg& oCopy);
	
	//----------------------------------------------------------------
	//函数功能：解析字符串
	//输入参数： pStrMsg  需要进行解析的字符串指针
	//返 回 值： 0：成功，其它失败
	int Parse(const string& sStrMsg,int v_iLevel = 0);

	//----------------------------------------------------------------
	//函数功能：获得当前集合中的元素个数
	//输入参数：无
	//返 回 值：集合中的元素个数
	size_t size() const;

	//容器容量
	size_t Capacity() const;

	//容器容量调整
	void Reserve(size_t nSize);

	//----------------------------------------------------------------
	//函数功能：将对象转换为字符串
	//输入参数：用于接收的字符串指针
	//返 回 值：无
	void   ToString(string& v_sOutMsg) const;
	string ToString() const;

	void clear();

	//----------------------------------------------------------------
	//函数功能：向当前集合中添加值
	//输入参数：tValue 需要添加的字符串值
	//返 回 值：无
	template <typename T>
	void AddValue(T tValue)
	{
		POS_INFO stInfo;
		stInfo.eType = E_STRING;
		stInfo.nPos = m_sStrValues.size();
		m_vecPosInfo.push_back(stInfo);

		std::stringstream ss;
		ss << setiosflags(ios::fixed) << setprecision(gc_ucPrecision) << tValue;
		m_sStrValues.push_back(ss.str());
	}

	void AddValue(ArrayListMsg& aRec);

	//----------------------------------------------------------------
	//函数功能：获得索引序号为 v_iIndex 位置的字符串元素
	//输入参数：需要获取位置的索引序号
	//返 回 值：如果指定索引位置有值，则返回相应指针，否则返回0
	template <typename T>
	T GetValue(size_t nIndex) const
	{
		assert(nIndex>=0 && nIndex < m_vecPosInfo.size() && m_vecPosInfo[nIndex].eType == E_STRING);
		assert(m_vecPosInfo[nIndex].nPos < m_sStrValues.size());

		std::stringstream ss;
		ss << m_sStrValues[m_vecPosInfo[nIndex].nPos];

		T tVal;
		ss >> tVal;
		return tVal;
	}

	ArrayListMsg GetValue(size_t nIndex) const;
	void GetValue(size_t nIndex,ArrayListMsg& aRec) const;
	//----------------------------------------------------------------
	//函数功能：向当前集合中添加值
	//输入参数：v_iIndex 需要进行添加的子集合索引
	//          v_sValue 需要添加的字符串值
	//返 回 值：无
	template <typename T>
	void AddValue(size_t nIndex,T tValue)
	{
		assert(nIndex>=0 && nIndex < m_vecPosInfo.size());
		assert(m_vecPosInfo[nIndex].eType == E_ARRAYLIST && m_vecPosInfo[nIndex].nPos < m_aListValues.size());

		std::stringstream ss;
		ss << tValue;
		m_aListValues[m_vecPosInfo[nIndex].nPos].AddValue(ss.str());
	}

	void AddValue(size_t nIndex,ArrayListMsg& aRec);
	//----------------------------------------------------------------
	//函数功能：获得记录索引号为nRecIndex，元素索引nEleIndex位置的值
	//输入参数：nRecIndex  记录索引
	//          nEleIndex 元素索引
	//返 回 值：
	template <typename T>
	T GetValue(size_t nRecIndex,size_t nEleIndex)  const
	{
		assert(nRecIndex>=0 && nRecIndex < m_vecPosInfo.size());
		assert(m_vecPosInfo[nRecIndex].eType == E_ARRAYLIST && m_vecPosInfo[nRecIndex].nPos < m_aListValues.size());

		T tVal = m_aListValues[m_vecPosInfo[nRecIndex].nPos].GetValue<T>(nEleIndex);
		return tVal;
	}

	// add by xiaorensong，解决stringstream空格截断的问题
	string GetStringEx(size_t nIndex)  const
	{
		assert(nIndex>=0 && nIndex < m_vecPosInfo.size() && m_vecPosInfo[nIndex].eType == E_STRING);
		assert(m_vecPosInfo[nIndex].nPos < m_sStrValues.size());

		return m_sStrValues[m_vecPosInfo[nIndex].nPos];
	}

	// add by xiaorensong，解决stringstream空格截断的问题
	string GetStringEx(size_t nRecIndex,size_t nEleIndex)  const
	{
		assert(nRecIndex>=0 && nRecIndex < m_vecPosInfo.size());
		assert(m_vecPosInfo[nRecIndex].eType == E_ARRAYLIST && m_vecPosInfo[nRecIndex].nPos < m_aListValues.size());

		return m_aListValues[m_vecPosInfo[nRecIndex].nPos].GetStringEx(nEleIndex);
	}

	ArrayListMsg GetValue(size_t nRecIndex,size_t nEleIndex) const;

	size_t SizeEx() const;

	size_t SizeEx2() const; // add by xrs，获取m_sStrValues的个数
private:
	void ToString(string& sOutMsg,int nIndex ) const;

	//用于保存元素为字符串的集合
	vector<string>        m_sStrValues;
	//用于保存元素为ArrayListMsg对象的集合     特别说明：在两个集合中，只会有一个集合有元素数据
	vector<ArrayListMsg> m_aListValues;
	//字符串格式
	string                m_sStrValue;

	int m_nLevel;
	vector<POS_INFO>	m_vecPosInfo;

};

#endif