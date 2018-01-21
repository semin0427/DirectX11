#ifndef TextureMgr_h__
#define TextureMgr_h__

#include "Defines.h"

class CTexture;
class CTextureMgr
{
	DECLARE_SINGLETON(CTextureMgr)
public:
	CTextureMgr();
	~CTextureMgr();
public:
	virtual  HRESULT				 InsertTexture( const wstring& wstrFilePath , const wstring& wstrObjKey, TEXTYPE _eType
		, const wstring& wstrStateKey = L"" 
		, const _int& iCount = 0  );
	HRESULT							ReadImagePath(wstring wstrImgPath);
public:
	virtual const TEXINFO*		GetTexture(const wstring& wstrObjKey, const wstring& wstrStateKey  = L"", const _int& iCout = 0);
	_int									GetImageCount(const wstring& wstrObjKey, wstring wstrStateKey);
	const	 wstring&					GetFilePath(void);
public:
	void									SetName(wstring wstrName);

private:
	typedef map<wstring, CTexture*>		MAPTEX;
	MAPTEX											m_mapTexture;
	wstring												m_wsrFullPath;
public:
	_ulong Release(void);
};

#endif // TextureMgr_h__
