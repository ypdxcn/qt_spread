/// Encode.h: interface for the CEncode class.
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef AFX_ENCODE_H__5650495C_06A5_40D9_86DC_F4D25F73E58E__INCLUDED_
#define AFX_ENCODE_H__5650495C_06A5_40D9_86DC_F4D25F73E58E__INCLUDED_

#include "Gess.h"

typedef	unsigned char	bytechar;

class UTIL_CLASS CEncode  
{
public:
	CEncode();
	virtual ~CEncode();
public:
	static int Pwd3DesBase64Enc(char* pBufPwd,char* pBufEncoded,int nBufEncLen);
	static int PwdBase64Dec3Des(char* pBufEncoded,char* pBufPwd,int nBufPwdLen);

	/// 函数名: unencryptchar
	/// 编程  : 王明松 2007-12-1 10:51:52
	/// 返回  : int <0失败 >0解密后数据长度
	/// 输入输出参数  : char *buf  加密数据
	/// 输入输出参数  : int buflen  加密数据长度/解密后数据长度
	/// 输入参数  : char *key  16位长度密匙
	/// 描述  : 解密字符串
	static int unencryptchar(char *buf,int &buflen ,char *key);

	/// 函数名: encryptchar
	/// 编程  : 王明松 2007-12-1 10:51:25
	/// 返回  : int <0失败 >0加密后的数据长度
	/// 参数  : char *buf 要加密的数据
	/// 参数  : int buflen 要加密的数据长度/加密后的数据长度
	/// 参数  : char *key 16位长度密匙
	/// 描述  : 加密码字符串
	static int encryptchar(char *buf,int &buflen,char *key);

	static void	EncodePass(char *pass, char *xpass64);
	static void	DecodePass(char *xpass64, char *pass);
	static int	    CheckPass(char *xpass1, char *xpass2);
	static int	    CheckPassB(char *xpass1, char *pass2);
	static void	rmt_encode(const char *key, char *text, int len);
	static void	rmt_decode(const char *key, char *text, int len);

	static int	    enbase64(char *buf, int n, char *outbuf);
	static int	    unbase64(char *buf, int n, char *outbuf);
private:
	static void 	do_ip(bytechar *input, bytechar *output, const bytechar *table);
	static void 	function_rk(bytechar *input, bytechar *output, const bytechar *key);
	static void 	rotate(bytechar* ptr, bytechar times);
	static void 	genkeys_r(const char *key, char *perm_keys);
	static void	cipher_r(const char *input, char *output, int decode, const char *perm_keys);
	static void 	genkeys(const char *key);
	static void	cipher(const char *input, char *output, int decode);
	static void	_3des_genkeys_r(const char *key16, char *perm_keys);
	static void	_3des_cipher_r(const char *input, char *output, int decode, const char *perm_keys);
	static void	_3DES_encode(const char *mkey, const char *text, char *xtext);
	static void	_3DES_decode(const char *mkey, const char *xtext, char *text);
	static void	PBOC(const char *mkey/*16*/, const char *id/*8*/, char *ikey/*16*/);
	static void	OWF2(const char *mkey/*16*/, const char *id/*8*/, char *mxkey/*16*/);
	static int	ENC64(int ch);
	
	static int	DEC64(char c);
	

};

#endif /// !defined(AFX_ENCODE_H__5650495C_06A5_40D9_86DC_F4D25F73E58E__INCLUDED_)
