#ifndef Texture_h__
#define Texture_h__

#include "Defines.h"

class  CTexture
{
protected:
	explicit CTexture();
	virtual ~CTexture();

public:
	virtual HRESULT InsertTexture( const wstring& wstrFilePath
		, const wstring& wstrStateKey = L""
		, const _int& iCount = 0	)PURE;
	
	virtual const TEXINFO* GetTexture( const wstring& wstrStateKey = L""
		, const _int& iCout = 0)PURE;


public:
	virtual _ulong Release(void);
};

#endif // Texture_h__
