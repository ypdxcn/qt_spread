/////////////////////////////////////////////////////////////////
//
//  MD5加密类
//
//  Bode
//  2007-01-29
//
//  参考资料：
//  Ronald L. Rivest (1992) "The MD5 Message-Digest Algorithm" 
//
/////////////////////////////////////////////////////////////////

#include "GESS.h"


// POINTER defines a generic pointer type
typedef unsigned char *POINTER;

// UINT2 defines a two byte word 
typedef unsigned short int UINT2;

// UINT4 defines a four byte word
typedef unsigned long int UINT4;

// MD5 context.
typedef struct {
    UINT4 state[4];             // state (ABCD) 
    UINT4 count[2];             // number of bits, modulo 2^64 (lsb first) 
    unsigned char buffer[64];   // input buffer
} MD5_CTX;

// MD5 Class
class UTIL_CLASS MD5
{
private:
    static unsigned char PADDING[64];
public:
    static void MD5Init (MD5_CTX *);
    static void MD5Update (MD5_CTX *, unsigned char *, unsigned int);
    static void MD5Final (unsigned char [16], MD5_CTX *);
private:
    static void MD5Transform (UINT4 [4], unsigned char [64]);
    static void Encode (unsigned char *, UINT4 *, unsigned int);
    static void Decode (UINT4 *, unsigned char *, unsigned int);
    static void MD5_memcpy (POINTER, POINTER, unsigned int);
    static void MD5_memset (POINTER, int, unsigned int);

    // added by Jerry Lee, 2011-3-30
    static string bytesToHexString(const unsigned char *, size_t); 
public:
    static void ToMD5(unsigned char [16],  void *, unsigned int);

    // added by Jerry Lee, 2011-3-30
    static string ToMD5(const char *, unsigned int);
};