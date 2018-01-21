
#ifndef MultiTexture_h__
#define MultiTexture_h__

#include "Texture.h"

class CMultiTexture : public CTexture
{
private:
	explicit CMultiTexture(void);
	~CMultiTexture(void);
public:
	virtual  HRESULT InsertTexture( const wstring& wstrFilePath , const wstring& wstrStateKey /* = L""  */, const _int& iCount /* = 0 */ );

	virtual const TEXINFO* GetTexture(const wstring& wstrStateKey /* = L""  */, const _int& iCout /* = 0 */);

	_int	GetImageCount(wstring wstrStateKey);

public:
	static CMultiTexture* Create( const wstring& wstrFilePath
		, const wstring& wstrStateKey = L""
		, const _int& iCount = 0 );
private:
	map<wstring, vector<TEXINFO*>>			m_pMapTexInfo;
public:
	virtual _ulong Release();

};

#endif // MultiTexture_h__
