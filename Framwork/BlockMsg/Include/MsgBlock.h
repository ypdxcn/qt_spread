#ifndef _MSG_BLOCK_H_
#define _MSG_BLOCK_H_
#include "MessageImpl.h"
#include "MsgDef.h"
#include "BlockMsg.h"

/*
 * MsgBlock 指块状数据结构的消息包。
 * 消息由一个或多个字段连续组成，有一个定长的字段指名了消息的标识。不同标识的消
 * 息可能具有不同字段组合。
 */
class CMessageImpl;
class BLOCKMSG_CLASS CMsgBlock
{
public:
	virtual ~CMsgBlock(void);
	CMsgBlock(CMessage& msg,size_t nDefaultWriteBufSize = 1024, size_t nMsgLengthBytes = 4);
	CMsgBlock(size_t nDefaultWriteBufSize = 1024, size_t nMsgLengthBytes = 4);
	CMsgBlock(const CMsgBlock& msgBlk);
	CMsgBlock& operator=(const CMsgBlock& msgBlk);

	const char * GetData(void);
	size_t DataLength(void);

	CMessage& GetMsg(){return m_Msg;}
protected:
	bool ReadInt8(int nFieldID);
	bool ReadInt16(int nFieldID);
	bool ReadInt32(int nFieldID);
	bool ReadOctStr(int nFieldID, size_t nSize);
	bool ReadBinary(int nFieldID, size_t nSize);
	bool ReadVLD8(int nFieldID);
	bool ReadVLD16(int nFieldID);
	bool ReadVLD32(int nFieldID);
	bool ReadMFS8(int nFieldID, size_t nFieldSize);
	bool ReadREC8(int nFieldID, size_t nRecSize);
	bool ReadREC16(int nFieldID, size_t nRecSize);
	bool ReadVLS(int nFieldID);

	bool WriteInt8(int nFieldID);
	bool WriteInt16(int nFieldID);
	bool WriteInt32(int nFieldID);
	bool WriteOctStr(int nFieldID, size_t nSize, char cBlank = '\0');
	bool WriteBinary(int nFieldID, size_t nSize, char cBlank = '\0');
	bool WriteVLD8(int nFieldID);
	bool WriteVLD16(int nFieldID);
	bool WriteVLD32(int nFieldID);
	bool WriteMFS8(int nFieldID, size_t nFieldSize);
	bool WriteREC8(int nFieldID, size_t nRecSize);
	bool WriteREC16(int nFieldID, size_t nRecSize);
	bool WriteVLS(int nFieldID);

	CMessageImpl  m_Msg;	
	
	const char * m_pReadBuf; // 用于解析的缓冲区地址
	char * m_pWriteBuf; // 用于组包的缓冲区地址

	size_t m_nOffset;
	size_t m_nBufLen;
	size_t m_nDefaultWriteBufSize; // 组包缓冲区的默认大小
	size_t m_nMsgLengthBytes; // 消息长度字段尺寸

	bool WriteBuf(const void * pData, size_t nSize, bool bPadding = false);
};

/*
 * 对各种类型的消息包进行定义的宏
 * Offset: 消息包标识字段在包中的偏移量
 * Length: 消息包标识字段的长度，假定不超过int的长度
 */
#define BEGIN_PKG_DECODE(Offset, Length) \
public: \
	void Decode(const char * pBuf, size_t nSize) \
	{ \
		m_nOffset = 0; \
		m_pReadBuf = pBuf; \
		m_nBufLen = nSize; \
		unsigned int uiCmdID = 0; \
		if (Length > sizeof(uiCmdID)) \
			return ; \
		memcpy(&uiCmdID, m_pReadBuf + Offset, Length); \
		uiCmdID = ntohl(uiCmdID);

// CmdID: 消息包标识 , pFunc: 对应的通信包处理函数
#define PKG_DECODE_HANDLE(CmdID, Func) \
		if (uiCmdID == CmdID) \
			Proc##Func(); 

#define END_PKG_DECODE() \
		return; \
	}

#define BEGIN_PKG_ENCODE \
public: \
	const char* Encode(size_t & nSize) \
	{ \
	    unsigned int uiCmdID = 0; \
	    m_Msg.GetField(MSG_CMD_ID, uiCmdID);

// CmdID: 消息包标识  Func: 对应的通信包处理函数
#define PKG_ENCODE_HANDLE(CmdID,Func) \
        if (CmdID == uiCmdID) \
		    return Encode##Func(nSize);

#define END_PKG_ENCODE() \
		return 0; \
	}

/*
 * 对每个通信包进行定义的宏
 * Func : 通信原语处理函数的名称
 */
#define BEGIN_PKG_FIELD(Func) \
private: \
	const char* Encode##Func(size_t & nSize) \
	{ \
	    nSize = 0; \
		if (m_pWriteBuf != NULL) \
		{ \
			nSize = m_nOffset; \
			return m_pWriteBuf; \
		} \
		m_nOffset = 0; \
		m_nBufLen = m_nDefaultWriteBufSize; \
		m_pWriteBuf = (char *)malloc(m_nBufLen); \
		bool bRet = Proc##Func(false); \
		if (!bRet) \
		{ \
			m_nOffset = 0; \
			nSize = 0; \
			if (0 != m_pWriteBuf) \
			{ \
				free(m_pWriteBuf); \
				m_pWriteBuf = 0; \
			} \
			return 0; \
		} \
		if (m_nMsgLengthBytes == 1) \
			*m_pWriteBuf = static_cast<unsigned char>(DataLength()); \
		else if (m_nMsgLengthBytes == 2) \
			*(unsigned short *)m_pWriteBuf = ntohs(static_cast<unsigned short>(DataLength())); \
		else if (m_nMsgLengthBytes == 4) \
			*(unsigned int *)m_pWriteBuf = ntohl(static_cast<unsigned int>(DataLength())); \
		nSize = m_nOffset; \
		return m_pWriteBuf; \
	} \
private: \
	bool Proc##Func(bool bDecode = true) \
	{ \
		bool bRet = true;

// 填充字段，解码时跳过填充段，编码时填'\0'
#define FIELD_PADDING(Length) \
		if (bDecode) \
		{ \
			if (m_nOffset + Length > m_nBufLen) \
				return false; \
			m_nOffset += Length; \
		} \
		else \
		{ \
			bRet = WriteBuf(NULL, Length, true); \
		} \
		if (!bRet) return false;

// 8位整型字段
#define FIELD_INT8(FieldID) \
		if (bDecode) \
			bRet = ReadInt8(FieldID); \
		else \
			bRet = WriteInt8(FieldID); \
		if (!bRet) return false;

// 16位整型字段
#define FIELD_INT16(FieldID) \
		if (bDecode) \
			bRet = ReadInt16(FieldID); \
		else \
			bRet = WriteInt16(FieldID); \
		if (!bRet) return false;

// 32位整型字段
#define FIELD_INT32(FieldID) \
		if (bDecode) \
			bRet = ReadInt32(FieldID); \
		else \
			bRet = WriteInt32(FieldID); \
		if (!bRet) return false;

// 多个8位字符构成的字段，不包含'\0'字符
#define FIELD_OCTSTR(FieldID, Length) \
		if (bDecode) \
			bRet = ReadOctStr(FieldID, Length); \
		else \
			bRet = WriteOctStr(FieldID, Length); \
		if (!bRet) return false;

// 多个8位字符构成的字段，可包含'\0'字符
#define FIELD_BINARY(FieldID, Length) \
		if (bDecode) \
			bRet = ReadBinary(FieldID, Length); \
		else \
			bRet = WriteBinary(FieldID, Length); \
		if (!bRet) return false;

// 可变长度字符串(Variable Length String)字段，字符串由'\0'结尾
#define FIELD_VLS(FieldID) \
		if (bDecode) \
		{ \
			bRet = ReadVLS(FieldID); \
		} \
		else \
		{ \
			bRet = WriteVLS(FieldID); \
		} \
		if (!bRet) return false;

// 可变长度数据(Variable Length Data)字段，长度由首字节8位整型表示
#define FIELD_VLD8(FieldID) \
		if (bDecode) \
		{ \
			bRet = ReadVLD8(FieldID); \
		} \
		else \
		{ \
			bRet = WriteVLD8(FieldID); \
		} \
		if (!bRet) return false;

// 可变长度数据(Variable Length Data)字段，长度由首字节16位整型表示
#define FIELD_VLD16(FieldID) \
		if (bDecode) \
		{ \
			bRet = ReadVLD16(FieldID); \
		} \
		else \
		{ \
			bRet = WriteVLD16(FieldID); \
		} \
		if (!bRet) return false;

// 可变长度数据(Variable Length Data)字段，长度由首字节32位整型表示
#define FIELD_VLD32(FieldID) \
		if (bDecode) \
		{ \
			bRet = ReadVLD32(FieldID); \
		} \
		else \
		{ \
			bRet = WriteVLD32(FieldID); \
		} \
		if (!bRet) return false;

/*
 * 多字段字符串(Multiple Fields String)字段，字段个数由首字节8位整型表示，每字
 * 段的长度固定。用,号拼接放入FieldID指定的字段中
 */
#define FIELD_MFS8(FieldID, Length) \
		if (bDecode) \
			bRet = ReadMFS8(FieldID, Length); \
		else \
			bRet = WriteMFS8(FieldID, Length); \
		if (!bRet) return false;

/*
 * 多记录字段，记录个数由首字节8位整型表示，每记录
 * 的长度固定为Length。
 */
#define FIELD_REC8(FieldID, Length) \
		if (bDecode) \
			bRet = ReadREC8(FieldID, Length); \
		else \
			bRet = WriteREC8(FieldID, Length); \
		if (!bRet) return false;

/*
 * 多记录字段，记录个数由首两字节整型表示，每记录
 * 的长度固定为Length。
 */
#define FIELD_REC16(FieldID, Length) \
		if (bDecode) \
			bRet = ReadREC16(FieldID, Length); \
		else \
			bRet = WriteREC16(FieldID, Length); \
		if (!bRet) return false;

// 自定义字段，由自定义处理函数自行处理长度与数据解析
#define FIELD_CUSTOM(FieldID, CustomFunc) \
		bRet = CustomFunc(FieldID, bDecode); \
		if (!bRet) return false;

#define END_PKG_FIELD() \
		return bRet; \
	}

#endif // _MSG_BLOCK_H_
