// MD5Reg.cpp

#include "StdAfx.h"

#include "../../C/md5.h"

#include "../Common/MyCom.h"

#include "../7zip/ICoder.h"
#include "../7zip/Common/RegisterCodec.h"

#ifndef MD5_DIGEST_SIZE
#define MD5_DIGEST_SIZE 16
#endif

class CMD5Hasher:
  public IHasher,
  public CMyUnknownImp
{
  md5_state_t _md5;

public:
  CMD5Hasher() { Init(); };

  MY_UNKNOWN_IMP

  STDMETHOD_(void, Init)();
  STDMETHOD_(void, Update)(const void *data, UInt32 size);
  STDMETHOD_(void, Final)(Byte *digest);
  STDMETHOD_(UInt32, GetDigestSize)();
};

STDMETHODIMP_(void) CMD5Hasher::Init()
{
  md5_init(&_md5);
}

STDMETHODIMP_(void) CMD5Hasher::Update(const void *data, UInt32 size)
{
  md5_append(&_md5, (const Byte *)data, size);
}

STDMETHODIMP_(void) CMD5Hasher::Final(Byte *digest)
{
  md5_finish(&_md5, digest);
}

STDMETHODIMP_(UInt32) CMD5Hasher::GetDigestSize()
{
  return MD5_DIGEST_SIZE;
}

static IHasher *CreateHasher() { return new CMD5Hasher; }

static CHasherInfo g_HasherInfo = { CreateHasher, 0x8, L"MD5", MD5_DIGEST_SIZE };

REGISTER_HASHER(MD5)
