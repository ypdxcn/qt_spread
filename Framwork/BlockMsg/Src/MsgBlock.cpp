#include <string>
#include <cassert>
#include "MsgBlock.h"
#include "strutils.h"

using namespace std;

CMsgBlock::CMsgBlock(CMessage& msg,size_t nDefaultWriteBufSize, size_t nMsgLengthBytes)
{
	CMessageImpl&  oMsg = dynamic_cast<CMessageImpl&>(msg);
	m_Msg.Copy(oMsg,true);

	// 只接受1、2或4字节的长度字段
	assert(nMsgLengthBytes == 1 || nMsgLengthBytes == 2 || nMsgLengthBytes == 4);
	m_nMsgLengthBytes = nMsgLengthBytes;

	m_pReadBuf = NULL;
	m_pWriteBuf = NULL;
	m_nOffset = 0;
	m_nBufLen = 0;
	m_nDefaultWriteBufSize = nDefaultWriteBufSize;
}

CMsgBlock::CMsgBlock(size_t nDefaultWriteBufSize, size_t nMsgLengthBytes)
{

	// 只接受1、2或4字节的长度字段
	assert(nMsgLengthBytes == 1 || nMsgLengthBytes == 2 || nMsgLengthBytes == 4);
	m_nMsgLengthBytes = nMsgLengthBytes;

	m_pReadBuf = NULL;
	m_pWriteBuf = NULL;
	m_nOffset = 0;
	m_nBufLen = 0;
	m_nDefaultWriteBufSize = nDefaultWriteBufSize;
}

CMsgBlock::~CMsgBlock(void)
{
	if (m_pWriteBuf != NULL)
	{
		free(m_pWriteBuf);
		m_pWriteBuf = NULL;
	}
}

CMsgBlock::CMsgBlock(const CMsgBlock& msgBlk)
{
	m_Msg.Copy(msgBlk.m_Msg,true);
	m_pReadBuf = 0;

	m_pWriteBuf = 0;
	m_nOffset = 0;
	m_nBufLen = 0;
	m_nDefaultWriteBufSize = msgBlk.m_nDefaultWriteBufSize;
	m_nMsgLengthBytes = msgBlk.m_nMsgLengthBytes;
}

CMsgBlock& CMsgBlock::operator=(const CMsgBlock& msgBlk)
{
	m_Msg.Copy(msgBlk.m_Msg,true);
	m_pReadBuf = 0;

	if (0 != m_pWriteBuf)
	{
		free(m_pWriteBuf);
		m_pWriteBuf = 0;
	}

	m_nOffset = 0;
	m_nBufLen = 0;
	m_nDefaultWriteBufSize = msgBlk.m_nDefaultWriteBufSize;
	m_nMsgLengthBytes = msgBlk.m_nMsgLengthBytes;
	return *this;
}

const char * CMsgBlock::GetData(void)
{// 返回组包缓冲区的地址
	return m_pWriteBuf;
}

size_t CMsgBlock::DataLength(void)
{// 返回组包缓冲区内数据的长度
	return m_nOffset;
}

bool CMsgBlock::WriteBuf(const void * pData, size_t nSize, bool bPadding)
{
	if (m_nOffset + nSize > m_nBufLen)
	{// 重新分配缓冲区
		size_t nNewSize = m_nBufLen + m_nDefaultWriteBufSize;
		if (m_nOffset + nSize > nNewSize)
			nNewSize += nSize;
		m_pWriteBuf = static_cast<char *>(realloc(m_pWriteBuf, nNewSize));
		if (m_pWriteBuf == NULL)
			return false;
	}

	if (bPadding)
		memset(m_pWriteBuf + m_nOffset, 0, nSize);
	else
		memcpy(m_pWriteBuf + m_nOffset, pData, nSize);
	m_nOffset += nSize;
	return true;
}

bool CMsgBlock::ReadInt8(int nFieldID)
{
	if (m_nOffset + sizeof(unsigned char) > m_nBufLen)
		return false;

	unsigned char cVal = *(m_pReadBuf + m_nOffset);
	m_nOffset += sizeof(unsigned char);
	m_Msg.SetField(nFieldID, (unsigned short)cVal);
	return true;
}

bool CMsgBlock::WriteInt8(int nFieldID)
{
	unsigned short nVal = 0;

	m_Msg.GetField(nFieldID, nVal);
	unsigned char cVal = static_cast<unsigned char>(nVal);
	return WriteBuf(&cVal, sizeof(unsigned char));
}

bool CMsgBlock::ReadInt16(int nFieldID)
{
	if (m_nOffset + sizeof(unsigned short) > m_nBufLen)
		return false;

	unsigned short nVal = *((unsigned short *)(m_pReadBuf + m_nOffset));
	m_nOffset += sizeof(unsigned short);
	nVal = ntohs(nVal);
	m_Msg.SetField(nFieldID, nVal);
	return true;
}

bool CMsgBlock::WriteInt16(int nFieldID)
{
	unsigned short nVal = 0;

	m_Msg.GetField(nFieldID, nVal);
	nVal = htons(nVal);
	return WriteBuf(&nVal, sizeof(unsigned short));
}

bool CMsgBlock::ReadInt32(int nFieldID) 
{
	if (m_nOffset + sizeof(unsigned int) > m_nBufLen)
		return false;

	unsigned int ulVal = *((unsigned int *)(m_pReadBuf + m_nOffset));
	m_nOffset += sizeof(unsigned int);
	ulVal = ntohl(ulVal);
	m_Msg.SetField(nFieldID, ulVal);
	return true;
}

bool CMsgBlock::WriteInt32(int nFieldID)
{
	unsigned int ulVal = 0;

	m_Msg.GetField(nFieldID, ulVal);
	ulVal = htonl(ulVal);
	return WriteBuf(&ulVal, sizeof(unsigned int));
}

bool CMsgBlock::ReadVLD8(int nFieldID)
{
	if (m_nOffset + sizeof(char) > m_nBufLen)
		return false;

	unsigned char nSize = *(m_pReadBuf + m_nOffset);
	m_nOffset += sizeof(char);
	if (m_nOffset + nSize > m_nBufLen)
		return false;

	string str;
	str.assign(m_pReadBuf + m_nOffset, nSize);
	m_nOffset += nSize;
	m_Msg.SetBinaryField(nFieldID, str);
	return true;
}

bool CMsgBlock::WriteVLD8(int nFieldID)
{
	string sVal;
	m_Msg.GetBinaryField(nFieldID, sVal);

	if (sVal.length() > 0xFF)
		return false;

	unsigned char cLen = static_cast<unsigned char>(sVal.length());
	if (!WriteBuf(&cLen, sizeof(unsigned char)))
		return false;
	
	return WriteBuf(const_cast<char *>(sVal.c_str()), sVal.length());
}

bool CMsgBlock::ReadVLD16(int nFieldID)
{
	if (m_nOffset + sizeof(unsigned short) > m_nBufLen)
		return false;

	unsigned short nSize = *((unsigned short *)(m_pReadBuf + m_nOffset));
	m_nOffset += sizeof(unsigned short);
	nSize = ntohs(nSize);

	if (m_nOffset + nSize > m_nBufLen)
		return false;

	string str;
	str.assign(m_pReadBuf + m_nOffset, nSize);
	m_nOffset += nSize;
	m_Msg.SetBinaryField(nFieldID, str);
	return true;
}

bool CMsgBlock::WriteVLD16(int nFieldID)
{
	string sVal;
	m_Msg.GetBinaryField(nFieldID, sVal);

	if (sVal.length() > 0xFFFF)
		return false;

	unsigned short nLen = static_cast<unsigned short>(sVal.length());
	nLen = htons(nLen);
	if (!WriteBuf(&nLen, sizeof(unsigned short)))
		return false;
	
	return WriteBuf(const_cast<char *>(sVal.c_str()), sVal.length());
}

bool CMsgBlock::ReadVLD32(int nFieldID)
{
	if (m_nOffset + sizeof(unsigned int) > m_nBufLen)
		return false;

	unsigned int nSize = *((unsigned int *)(m_pReadBuf + m_nOffset));
	m_nOffset += sizeof(unsigned int);
	nSize = ntohl(nSize);

	if (m_nOffset + nSize > m_nBufLen)
		return false;

	string str;
	str.assign(m_pReadBuf + m_nOffset, nSize);
	m_nOffset += nSize;
	m_Msg.SetBinaryField(nFieldID, str);
	return true;
}

bool CMsgBlock::WriteVLD32(int nFieldID)
{
	string sVal;
	m_Msg.GetBinaryField(nFieldID, sVal);

	if (sVal.length() > 0xFFFF)
		return false;

	unsigned int nLen = static_cast<unsigned int>(sVal.length());
	nLen = htonl(nLen);
	if (!WriteBuf(&nLen, sizeof(unsigned int)))
		return false;
	
	return WriteBuf(const_cast<char *>(sVal.c_str()), sVal.length());
}

bool CMsgBlock::ReadOctStr(int nFieldID, size_t nSize)
{
	if (m_nOffset + nSize > m_nBufLen)
		return false;

	string str;
	if (*(m_pReadBuf + m_nOffset + nSize - 1) == '\0')
		str = m_pReadBuf + m_nOffset; // 数据区内有结束符，只复制结束符之前的部分
	else
		str.assign(m_pReadBuf + m_nOffset, nSize); // 数据区内无结束符，全部复制
	m_nOffset += nSize;
	m_Msg.SetField(nFieldID, str);
	return true;
}

bool CMsgBlock::ReadBinary(int nFieldID, size_t nSize)
{
	if (m_nOffset + nSize > m_nBufLen)
		return false;

	string str;
	str.assign(m_pReadBuf + m_nOffset, nSize);
	m_nOffset += nSize;
	m_Msg.SetBinaryField(nFieldID, str);
	return true;
}

bool CMsgBlock::WriteOctStr(int nFieldID, size_t nSize, char cBlank)
{
	string sVal;
	m_Msg.GetField(nFieldID, sVal);
	
	// 如果sVal的长度不足nSize，则右补cBlank
	if (nSize > sVal.length())
	{
		size_t nBlank = nSize - sVal.length();
		sVal.append(nBlank, cBlank);
	}
	return WriteBuf(const_cast<char *>(sVal.c_str()), nSize);
}

bool CMsgBlock::WriteBinary(int nFieldID, size_t nSize, char cBlank)
{
	string sVal;
	m_Msg.GetBinaryField(nFieldID, sVal);
	
	// 如果sVal的长度不足nSize，则右补cBlank
	if (nSize > sVal.length())
	{
		size_t nBlank = nSize - sVal.length();
		sVal.append(nBlank, cBlank);
	}
	return WriteBuf(const_cast<char *>(sVal.c_str()), nSize);
}

bool CMsgBlock::ReadMFS8(int nFieldID, size_t nFieldSize)
{
	string sVal;
	vector<string> vFields;
	unsigned char cCount = 0;

	cCount = *((unsigned char *)(m_pReadBuf + m_nOffset));
	m_nOffset += sizeof(unsigned char);

	for (int i = 0; i < cCount; i++)
	{
		string sField;
		sField.assign(m_pReadBuf + m_nOffset, nFieldSize);
		m_nOffset += nFieldSize;
		sVal = sField.c_str(); // 去掉结尾的'\0'填充
		vFields.push_back(sVal);
	}
	sVal = strutils::implode(",", vFields);
	m_Msg.SetField(nFieldID, sVal);

	return true;
}

bool CMsgBlock::WriteMFS8(int nFieldID, size_t nFieldSize)
{
	string sVal;
	unsigned char cCount = 0;

	m_Msg.GetField(nFieldID, sVal);
	vector<string> vFields = strutils::explode(",", sVal);
	if (vFields.size() < 1)
		return false;

	vector<string>::iterator it;

	unsigned char * pcFieldsCount = reinterpret_cast<unsigned char *>(m_pWriteBuf) + m_nOffset;
	if (!WriteBuf(&cCount, sizeof(unsigned char)))
		return false;

	int nCount = 0;
	for (it = vFields.begin(); it != vFields.end(); it++)
	{
		string sField = *it;
		if (!sField.empty())
		{
			nCount++;
			sField.append(nFieldSize - sField.length(), '\0'); // 右补'\0'
			if (!WriteBuf(const_cast<char *>(sField.data()), sField.length()))
				return false;
		}
	}
	*pcFieldsCount = static_cast<unsigned char>(nCount);

	return true;
}

bool CMsgBlock::ReadREC8(int nFieldID, size_t nRecSize)
{
	unsigned char cCount = 0;

	cCount = *((unsigned char *)(m_pReadBuf + m_nOffset));
	m_nOffset += sizeof(unsigned char);

	string sRecords;
	sRecords.assign(m_pReadBuf + m_nOffset, cCount * nRecSize);
		
	m_Msg.SetBinaryField(nFieldID, sRecords);
	return true;
}

bool CMsgBlock::WriteREC8(int nFieldID, size_t nRecSize)
{
	unsigned char cCount = 0;
	string sRecords;

	m_Msg.GetBinaryField(nFieldID, sRecords);	

	unsigned char * pcFieldsCount = reinterpret_cast<unsigned char *>(m_pWriteBuf + m_nOffset);
	
	if (!WriteBuf(&cCount, sizeof(unsigned char)))
		return false;

	if (!WriteBuf(const_cast<char *>(sRecords.data()), sRecords.length()))
		return false;

	int nCount = sRecords.length() / nRecSize;
	*pcFieldsCount = static_cast<unsigned char>(nCount);
	return true;
}

bool CMsgBlock::ReadREC16(int nFieldID, size_t nRecSize)
{
	unsigned short usCount = 0;

	usCount = *((unsigned short *)(m_pReadBuf + m_nOffset));
	m_nOffset += sizeof(unsigned short);

	string sRecords;
	sRecords.assign(m_pReadBuf + m_nOffset, usCount * nRecSize);
		
	m_Msg.SetBinaryField(nFieldID, sRecords);
	return true;
}

bool CMsgBlock::WriteREC16(int nFieldID, size_t nRecSize)
{
	unsigned short usCount = 0;
	string sRecords;

	m_Msg.GetBinaryField(nFieldID, sRecords);	

	unsigned short * pcFieldsCount = reinterpret_cast<unsigned short *>(m_pWriteBuf + m_nOffset);
	
	if (!WriteBuf(&usCount, sizeof(unsigned short)))
		return false;

	if (!WriteBuf(const_cast<char *>(sRecords.data()), sRecords.length()))
		return false;

	int nCount = sRecords.length() / nRecSize;
	*pcFieldsCount = static_cast<unsigned short>(nCount);
	return true;
}

bool CMsgBlock::ReadVLS(int nFieldID)
{
	if (m_nOffset + sizeof(char) > m_nBufLen)
		return false; // 至少有一个结尾'\0'的位置

	size_t nEnd = m_nOffset;
	for (; nEnd < m_nBufLen; nEnd++)
	{
		if (*(m_pReadBuf + nEnd) == '\0')
			break;
	}

	if (nEnd >= m_nBufLen)
		return false; // 在整个包中没有结束'\0'字符

	m_Msg.SetField(nFieldID, m_pReadBuf + m_nOffset);
	size_t nSize = nEnd - m_nOffset + 1; // 长度包括结尾'\0'字符, 跳过
	m_nOffset += nSize;
	return true;
}

bool CMsgBlock::WriteVLS(int nFieldID)
{
	string sVal;
	m_Msg.GetField(nFieldID, sVal);

	char szTail[1] = {0};
	if (WriteBuf(const_cast<char *>(sVal.c_str()), sVal.length()))
		return WriteBuf(szTail, sizeof(char));
	else
		return false;
}
