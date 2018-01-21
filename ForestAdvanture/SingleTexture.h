#ifndef SingleTexture_h__
#define SingleTexture_h__

#include "Texture.h"

class CSingleTexture : public CTexture
{
private:
	explicit CSingleTexture(void);
	~CSingleTexture(void);
public:
	virtual  HRESULT InsertTexture( const wstring& wstrFilePath , const wstring& wstrStateKey /* = L""  */, const _int& iCount /* = 0 */ );

	virtual const TEXINFO* GetTexture(const wstring& wstrStateKey /* = L""  */, const _int& iCout /* = 0 */);

public:
	static CSingleTexture* Create( const wstring& wstrFilePath
		, const wstring& wstrStateKey = L""
		, const _int& iCount = 0 );
private:
	TEXINFO*			m_pTexInfo;
public:
	virtual _ulong Release();

};

#endif // SingleTexture_h__
