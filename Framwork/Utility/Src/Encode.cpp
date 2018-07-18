// Encode.cpp: implementation of the CEncode class.
//
//////////////////////////////////////////////////////////////////////

#include "Encode.h"
#include "osdepend.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
static	char	*skey = "1234567890123456";
static	const	bytechar	ip_table[64] = {
	57, 49, 41, 33, 25, 17,  9,  1, 59, 51, 43, 35, 27, 19, 11, 3,
	61, 53, 45, 37, 29, 21, 13,  5, 63, 55, 47, 39, 31, 23, 15, 7,
	56, 48, 40, 32, 24, 16,  8,  0, 58, 50, 42, 34, 26, 18, 10, 2,
	60, 52, 44, 36, 28, 20, 12,  4, 62, 54, 46, 38, 30, 22, 14, 6
};

static	const	bytechar 	inv_ip_table[64] = {
	39,  7, 47, 15, 55, 23, 63, 31, 38,  6, 46, 14, 54, 22, 62, 30,
	37,  5, 45, 13, 53, 21, 61, 29, 36,  4, 44, 12, 52, 20, 60, 28,
	35,  3, 43, 11, 51, 19, 59, 27, 34,  2, 42, 10, 50, 18, 58, 26,
	33,  1, 41,  9, 49, 17, 57, 25, 32,  0, 40,  8, 48, 16, 56, 24
};

static	const	bytechar 	exp_table[48] = {
	31,  0,  1,  2,  3,  4,	3,   4,  5,  6,  7,  8,
	7,   8,  9, 10, 11, 12, 11, 12, 13, 14, 15, 16,
	15, 16, 17, 18, 19, 20, 19, 20, 21, 22, 23, 24,
	23, 24, 25, 26, 27, 28, 27, 28, 29, 30, 31,  0
};

static	const	bytechar 	perm_table[32] = {
	15,  6, 19, 20,	28, 11, 27, 16,
	 0, 14, 22, 25,  4, 17, 30,  9,
	 1,  7, 23, 13, 31, 26,  2,  8,
	18, 12, 29,  5, 21, 10,  3, 24
};

static	const	bytechar 	perm_choice1_C[28] = {
	56, 48, 40, 32, 24, 16,  8,
	 0, 57, 49, 41, 33, 25, 17,
	 9,  1, 58, 50, 42, 34, 26,
	18, 10,  2, 59, 51, 43, 35
};

static	const	bytechar 	perm_choice1_D[28] = {
	62, 54, 46, 38, 30, 22, 14,
	 6, 61, 53, 45, 37, 29, 21,
	13,  5, 60, 52, 44, 36, 28,
	20, 12,  4, 27, 19, 11,  3
};

static	const	bytechar 	perm_choice2[48] = {
	13, 16, 10, 23,  0,  4,  2, 27, 14,  5, 20,  9,
	22, 18, 11,  3, 25,  7, 15,  6, 26, 19, 12,  1,
	40, 51, 30, 36, 46, 54, 29, 39, 50, 44, 32, 47,
	43, 48, 38, 55, 33, 52, 45, 41, 49, 35, 28, 31
};

static	const	bytechar 	shifts_table[16] = {
	1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

static	const	bytechar 	sel_table[512] = {
	/* s1 */
	14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
	 0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
	 4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
	15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13,
	/* s2 */
	15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
	 3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
	 0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
	13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9,
	/* s3 */
	10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
	13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
	13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
	 1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12,
	/* s4 */
	 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
	13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
	10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
	 3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14,
	/* s5 */
	 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
	14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
	 4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
	11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3,
	/* s6 */
	12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
	10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
	 9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
	 4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13,
	/* s7 */
	 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
	13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
	 1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
	 6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12,
	/* s8 */
	13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
	 1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
	 7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
	 2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
};

static	const	bytechar 	bitmap[64] = {
	0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01,
	0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01,
	0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01,
	0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01,
	0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01,
	0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01,
	0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01,
	0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01
};

static	char	__keytab[96];

static	char	base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
		   "0123456789+/";



CEncode::CEncode()
{
	
}

CEncode::~CEncode()
{
	
}

int CEncode::Pwd3DesBase64Enc(char* pBufPwd,char* pBufEncoded,int nBufEncLen)
{
	int nPwdLen = strlen(pBufPwd);
	int n3DesEncLen = encryptchar(pBufPwd,nPwdLen,skey);

	if (nBufEncLen < (n3DesEncLen+2)/3 * 4 +1)
		return -1;

	return enbase64(pBufPwd,n3DesEncLen,pBufEncoded);
}

int CEncode::PwdBase64Dec3Des(char* pBufEncoded,char* pBufPwd,int nBufPwdLen)
{
	int nBase64EncLen = strlen(pBufEncoded);
	char* pBase64Dec = new char[nBase64EncLen/4*3+1];
	int nBase64DecLen = unbase64(pBufEncoded,nBase64EncLen,pBase64Dec);
	
	int n3DesDecLen = unencryptchar(pBase64Dec,nBase64DecLen,skey);
	if (nBufPwdLen < n3DesDecLen)
	{
		delete []pBase64Dec;
		return -1;
	}
	strcpy(pBufPwd,pBase64Dec);
	delete []pBase64Dec;
	return n3DesDecLen;
}

int	CEncode::ENC64(int ch)
{
	ch &= 0x3f;
	return	base64[ch];
}

int	CEncode::enbase64(char *buf, int n, char *outbuf)
{
	int	ch, cnt=0;
	char	*p;

	for (p = buf; n > 0; n -= 3, p += 3) {
		ch = *p >> 2;
		ch = ENC64(ch);
		outbuf[cnt++] = ch;

		ch = ((*p << 4) & 060) | ((p[1] >> 4) & 017);
		ch = ENC64(ch);
		outbuf[cnt++] = ch;

		ch = ((p[1] << 2) & 074) | ((p[2] >> 6) & 03);
		ch = (n < 2) ? '=' : ENC64(ch);
		outbuf[cnt++] = ch;

		ch = p[2] & 077;
		ch = (n < 3) ? '=' : ENC64(ch);
		outbuf[cnt++] = ch;
	}

	outbuf[cnt] = 0;

	return	cnt;
}

int	CEncode::DEC64(char c)
{
	if (c=='+')
		return	62;
	if (c=='/')
		return	63;
	if (isdigit(c))
		return	52+(c-0x30);
	if (isupper(c))
		return	(c-0x41);
	if (islower(c))
		return	26+(c-0x61);

	return	-1;
}

int	CEncode::unbase64(char *buf, int n, char *outbuf)
{
	int	ch, cnt=0;
	char	*p;

	while ('=' == buf[n-1])
		n--;

	for (p=buf ; n > 0; p += 4, n -= 4)
		if (n > 3) {
			ch = (DEC64(p[0]) << 2) | (DEC64(p[1]) >> 4);
			outbuf[cnt++] = ch;
			ch = (DEC64(p[1]) << 4) | (DEC64(p[2]) >> 2);
			outbuf[cnt++] = ch;
			ch = (DEC64(p[2]) << 6) | DEC64(p[3]);
			outbuf[cnt++] = ch;
		}
		else {
			if (n > 1) {
				ch = (DEC64(p[0]) << 2) | (DEC64(p[1]) >> 4);
				outbuf[cnt++] = ch;
			}
			if (n > 2) {
				ch = (DEC64(p[1]) << 4) | (DEC64(p[2]) >> 2);
				outbuf[cnt++] = ch;
			}
		}

	return	cnt;
}


void	CEncode::EncodePass(char *pass, char *xpass64)
{
	char	xpass[32];
	time_t	t;
	
	time(&t);
	memset(xpass, 0, sizeof(xpass));
	strncpy(xpass, pass, 6);
	xpass[6] = (t&0xff);
	xpass[7] = ((t>>8)&0xff);
	
	rmt_encode(skey, xpass, 8);
	
	enbase64(xpass, 8, xpass64); /* 8 ==> 11+1(=) */
}

void	CEncode::DecodePass(char *xpass64, char *pass)
{
	char	xpass[32];
	
	memset(xpass, 0, sizeof(xpass));
	unbase64(xpass64, 11, xpass); /* 11->8 */
	
	rmt_decode(skey, xpass, 8);
	
	memcpy(pass, xpass, 6);
	pass[6] = 0;
}

int	CEncode::CheckPass(char *xpass1, char *xpass2)
{
	char	pass1[6+1];
	char	pass2[6+1];
	int	eq;
	
	DecodePass(xpass1, pass1);
	DecodePass(xpass2, pass2);
	
	//printf("pass1=[%s -> %s] pass2=[%s -> %s]\n", xpass1, pass1, xpass2, pass2);
	
	eq = !memcmp(pass1, pass2, 7);
	
	memset(pass1, 0, sizeof(pass1));
	memset(pass2, 0, sizeof(pass2));
	
	return	eq;
}

int	CEncode::CheckPassB(char *xpass1, char *pass2)
{
	char	pass1[6+1];
	int	eq;
	
	DecodePass(xpass1, pass1);
	
	eq = !memcmp(pass1, pass2, 7);
	
	memset(pass1, 0, sizeof(pass1));
	
	return	eq;
}
/*
 *  remit system encode/decode function
    Import:
	NONE

    Export:
	void	rmt_encode(const char *key, const char *text, int len)
	void	rmt_decode(const char *key, const char *text, int len)

    Note:


    History:
	01/05/15    Wen	     1st version
*/


/* Chain-Encode should be used */
void	CEncode::rmt_encode(const char *key, char *text, int len)
{
	for (/*0*/; len>0; len -= 8, text += 8) {
		_3DES_encode(key, text, text);
	}
}
void	CEncode::rmt_decode(const char *key, char *text, int len)
{
	for (/*0*/; len>0; len -= 8, text += 8) {
		_3DES_decode(key, text, text);
	}
}



/**********************************************************************
Performs 'Initial Permutation' on input data block.
**********************************************************************/
void 	CEncode::do_ip(bytechar *input, bytechar *output, const bytechar *table)
{
	bytechar 	i, j;

	memset(output, 0, 8);

	for (i=0; i<64; i++) {
		j = table[i];
		if (input[j>>3] & bitmap[j])
			output[i>>3] |= bitmap[i];
	}
} /* do_ip() */

/**********************************************************************
Performs 'Cipher Function f' on input data block.
**********************************************************************/
void 	CEncode::function_rk(bytechar *input, bytechar *output, const bytechar *key)
{
	int	i;
	int 	row, column;
	int 	bits;
	bytechar 	temp[6];
	bytechar 	hold[4];

	memset(temp, 0, 6);
	memset(output, 0, 4);

	/* expand 32 bit input */
	for (i=0; i<48; i++) {
		row = exp_table[i];
		if (input[row>>3] & bitmap[row])
			temp[i>>3] |= bitmap[i];
	}

	/* XOR expanded data with permuted key */
	for (i=0; i<6; i++) {
		temp[i] ^= key[i];
	}

	/* calc 'select functions' */
	memset(hold, 0, 4);
	for (i=0; i<8; i++) {
		row = 0;
		switch (i) {
		case 0:
			if (temp[0] & 0x80) row = 2;
			if (temp[0] & 0x04) row++;
			column = ((temp[0] & 0x78) >> 3);
			break;
		case 1:
			if (temp[0] & 0x02) row = 2;
			if (temp[1] & 0x10) row++;
			column = ((temp[1] & 0xe0) >> 5);
			if (temp[0] & 0x01) column += 8;
			break;
		case 2:
			if (temp[1] & 0x08) row = 2;
			if (temp[2] & 0x40) row++;
			column = (((temp[1] & 0x07) << 1));
			if (temp[2] & 0x80) column++;
			break;
		case 3:
			if (temp[2] & 0x20) row = 2;
			if (temp[2] & 0x01) row++;
			column = ((temp[2] & 0x1e) >> 1);
			break;
		case 4:
			if (temp[3] & 0x80) row = 2;
			if (temp[3] & 0x04) row++;
			column = ((temp[3] & 0x78) >> 3);
			break;
		case 5:
			if (temp[3] & 0x02) row = 2;
			if (temp[4] & 0x10) row++;
			column = ((temp[4] & 0xe0) >> 5);
			if (temp[3] & 0x01) column += 8;
			break;
		case 6:
			if (temp[4] & 0x08) row = 2;
			if (temp[5] & 0x40) row++;
			column = ((temp[4] & 0x07) << 1);
			if (temp[5] & 0x80) column++;
			break;
		case 7:
			if (temp[5] & 0x20) row = 2;
			if (temp[5] & 0x01) row++;
			column = ((temp[5] & 0x1e) >> 1);
		}

		/* s*64 + row*16 + column */
		bits = sel_table[column + (i<<6) + (row<<4)];
		if ((i & 0x01) == 0) bits <<= 4;

		hold[i >> 1] |= bits;
	}

	/* run permutation function on 32 bit output */
	for (i=0; i<32; i++) {
		row = perm_table[i];
		if (hold[row>>3] & bitmap[row]) {
			/* set bit if needed */
			output[i>>3] |= bitmap[i];
		}
	}

} /* function_rk() */

/**********************************************************************
Rotates right 28 bits (in 2 words). 'Left shift' in DES standard.
**********************************************************************/
void 	CEncode::rotate(bytechar* ptr, bytechar times)
{

	int	bit, ntimes;

	ntimes = (8 - times);

	/* save LS Bit of lower word before shifting */
	bit = ((ptr[0] >> (ntimes-4)) & 0xf0);
	ptr[0] <<= times;
	ptr[0] |= (ptr[1] >> ntimes);
	ptr[1] <<= times;
	ptr[1] |= (ptr[2] >> ntimes);
	ptr[2] <<= times;
	ptr[2] |= (ptr[3] >> ntimes);
	ptr[3] &= 0xf0;
	ptr[3] <<= times;
	ptr[3] |= bit;
} /* rotate() */



/**********************************************************************
Generates 16 48-bit "Permuted Keys' from 64 bit (4 words) input key.
**********************************************************************/
void 	CEncode::genkeys_r(const char *key, char *perm_keys)
{

	bytechar 	data,i, k;
	bytechar 	c_block[4], d_block[4];

	if (!perm_keys)
		perm_keys = __keytab;

	/* generate 28 bit blocks using 'Permuted Choice 1' */
	/* generate 'C' block */
	memset(c_block, 0, 4);
	for (i=0; i<28; i++) {
		k = perm_choice1_C[i];
		if (key[k>>3] & bitmap[k]) {
			/* set bit if needed */
			c_block[i>>3] |= bitmap[i];
		}
	}

	/* generate 'D' block */
	memset(d_block, 0, 4);
	for (i=0; i<28; i++) {
		k = perm_choice1_D[i];
		if (key[k>>3] & bitmap[k]) {
			/* set bit if needed */
			d_block[i>>3] |= bitmap[i];
		}
	}

	memset(perm_keys, 0, 16*6);
	/* perform 'shifts' and generate 'Permuted Keys' */
	for (k=0; k<16; k++) {
		/* 'left shift' (rotate!) C block */
		rotate(c_block, shifts_table[k]);

		/* 'left shift' (rotate!) D block */
		rotate(d_block, shifts_table[k]);

		/* run 'Permuted Choice 2' to generate 48 bit 'Permuted Key' */
		for (i=0; i<48; i++) {
			data = perm_choice2[i];
			if (data < 28) {
				/* selected bit is in 'C' block */
				if (c_block[data>>3] & bitmap[data]) {
					perm_keys[k*6+(i>>3)] |= bitmap[i];
				}
			} else {
				/* selected bit is in 'D' block */
				data -= 28;
				if (d_block[data>>3] & bitmap[data]) {
					perm_keys[k*6+(i>>3)] |= bitmap[i];
				}
			}
		}
	}
}

/**********************************************************************
function : Ciphers 64 bit block of data.
input    : Pointers to 4 word input & output data arrays.
	   mode 0 = encrypt
	   mode 1 = decrypt
output	 : none.
call by  : user
**********************************************************************/
void	CEncode::cipher_r(const char *input, char *output, int decode, const char *perm_keys)
{
	int 	i;

	long	llast[2], ltemp[1];

	if (!perm_keys)
		perm_keys = __keytab;

	/* perform 'Initial Permutation' */
	do_ip((bytechar*)input, (bytechar*)llast, ip_table);

	/* Generate 'Preoutput' */
	for (i=0; i<16; i++) {
		if ( decode ) {
			function_rk((bytechar*)&llast[1], (bytechar*)ltemp,
				    (const bytechar*)&perm_keys[6*(15-i)]);
		} else {
			function_rk((bytechar*)&llast[1], (bytechar*)ltemp,
				    (const bytechar*)&perm_keys[6*i]);
		}

		ltemp[0] ^= llast[0];
		llast[0] = llast[1];
		llast[1] = ltemp[0];
	}

	ltemp[0] = llast[0];
	llast[0] = llast[1];
	llast[1] = ltemp[0];

	/* perform 'Inverse Initial Permutation' */
	do_ip((bytechar*)llast, (bytechar*)output, inv_ip_table);
}


void 	CEncode::genkeys(const char *key)
{
	genkeys_r(key, 0);
}
void	CEncode::cipher(const char *input, char *output, int decode)
{
	cipher_r(input, output, decode, 0);
}


void	CEncode::_3des_genkeys_r(const char *key16, char *perm_keys)
{
	genkeys_r(key16, perm_keys); /* Note: 0 not accepted. */
	genkeys_r(key16+8, perm_keys+96);
}
void	CEncode::_3des_cipher_r(const char *input, char *output, int decode, const char *perm_keys)
{
	cipher_r(input, output, decode, perm_keys); /*  */
	cipher_r(output, output, !decode, perm_keys+96); /*  */
	cipher_r(output, output, decode, perm_keys); /*  */
}


void	CEncode::_3DES_encode(const char *mkey, const char *text, char *xtext)
{
	char	keytab_l[96];
	char	keytab_r[96];

	genkeys_r(mkey, keytab_l);
	genkeys_r(mkey+8, keytab_r);

	cipher_r(text, xtext, 0, keytab_l); /* E */
	cipher_r(xtext, xtext, 1, keytab_r); /* D */
	cipher_r(xtext, xtext, 0, keytab_l); /* E */
}

void	CEncode::_3DES_decode(const char *mkey, const char *xtext, char *text)
{
	char	keytab_l[96];
	char	keytab_r[96];

	genkeys_r(mkey, keytab_l);
	genkeys_r(mkey+8, keytab_r);

	cipher_r(xtext, text, 1, keytab_l); /* D */
	cipher_r(text, text, 0, keytab_r); /* E */
	cipher_r(text, text, 1, keytab_l); /* D */
}

void	CEncode::PBOC(const char *mkey/*16*/, const char *id/*8*/, char *ikey/*16*/)
{
	char	notid[8];
	int	i;

	for (i=0; i<8; i++)
		notid[i] = ~id[i];
	_3DES_encode(mkey, id, ikey);

	_3DES_encode(mkey, notid, ikey+8);
}

void	CEncode::OWF2(const char *mkey/*16*/, const char *id/*8*/, char *mxkey/*16*/)
{
	char	nkey[16];

	memcpy(nkey, mkey+8, 8);
	memcpy(nkey+8, mkey, 8);

	_3DES_encode(mkey, id, mxkey);

	_3DES_encode(nkey, id, mxkey+8);
}

/*对字串进行加密*/
int CEncode::encryptchar(char *buf,int &buflen,char *key)
{
	int ret,ret1;
	char *p;
	char *pp;
	char tmp[9];
	char tmpout[17];
	char outbuf[8193];
	char key2[17];
	memset(tmp,0,sizeof(tmp));
	memset(key2,0,sizeof(key2));
	memset(outbuf,0,sizeof(outbuf));
	if (strlen(key)!=16)
	{
		return -1;
	}
	memcpy(key2,key,16);
	p = buf;
	pp = outbuf ;
	ret=0;
	ret1=0;
	while (ret<buflen)
	{
		memset(tmp,0,sizeof(tmp));
		if (ret +8 <buflen)
		{
			memcpy(tmp,p,8);
			tmp[8]=0;
		}
		else
		{
			memcpy(tmp,p,buflen-ret);
			tmp[buflen-ret]=0;
		}
		memset(tmpout,0,sizeof(tmpout));
		_3DES_encode(key2,tmp,tmpout);//调用加密
		tmpout[8]=0;
		memcpy(pp,tmpout,8);
		pp = pp + 8;
		p = p+8;
		ret = ret+8;
		ret1 = ret1 +8;
	}
	memcpy(buf,outbuf,ret1);
	buf[ret1]=0;
	buflen = ret1;
	return ret1;
	
}
/*对字串进行解密*/
int CEncode::unencryptchar(char *buf,int &buflen ,char *key)
{
	int ret;
	char *p;
	char *pp;
	char tmp[17];
	char tmpout[17];
	char outbuf[8193];
	char key2[17];
	memset(key2,0,sizeof(key2));
	memset(outbuf,0,sizeof(outbuf));
	if (strlen(key)!=16)
	{
		return -1;
	}
	memcpy(key2,key,16);
	p = buf;
	pp = outbuf ;
	ret=0;
	while (ret<buflen)
	{
		memset(tmp,0,sizeof(tmp));
		memset(tmpout,0,sizeof(tmpout));
		if (ret +8 <buflen)
		{
			memcpy(tmp,p,8);
			tmp[8]=0;
		}
		else
		{
			memcpy(tmp,p,buflen-ret);
			tmp[buflen-ret]=0;
		}
		_3DES_decode(key2,tmp,tmpout);//调用解密码
		tmp[8]=0;
		tmpout[8]=0;
		memcpy(pp,tmpout,8);
		pp = pp + 8;
		p = p+8;
		ret = ret+8;
		
	}    
	memcpy(buf,(char *)outbuf,strlen((char *)outbuf));
	buf[strlen((char *)outbuf)]=0;
	buflen = strlen(buf);
	return (strlen((char *)outbuf));
}