//#include "stdafx.h"
#include "SingleTexture.h"
#include "GraphicDev.h"

CSingleTexture::CSingleTexture(void)
	: CTexture()
{

}

CSingleTexture::~CSingleTexture(void)
{

}

HRESULT CSingleTexture::InsertTexture(const wstring& wstrFilePath , const wstring& wstrStateKey /* = L"" */, const _int& iCount /* = 0 */)
{
	m_pTexInfo = new TEXINFO;
	ZeroMemory(m_pTexInfo, sizeof(TEXINFO));

	if ( FAILED(D3DXGetImageInfoFromFile(wstrFilePath.c_str(), &m_pTexInfo->tImgInfo ) ) )
		return E_FAIL;

	if ( FAILED( D3DXCreateTextureFromFileEx( GETSINGLE(CGraphicDev)->GetDevice()
		, wstrFilePath.c_str()
		, m_pTexInfo->tImgInfo.Width
		, m_pTexInfo->tImgInfo.Height
		, m_pTexInfo->tImgInfo.MipLevels
		, 0, m_pTexInfo->tImgInfo.Format, D3DPOOL_MANAGED,
		D3DX_DEFAULT, D3DX_DEFAULT, NULL, &m_pTexInfo->tImgInfo, NULL, &m_pTexInfo->pTexture	) ))
		return E_FAIL;

	return S_OK;
}

const TEXINFO* CSingleTexture::GetTexture(const wstring& wstrStateKey /* = L"" */, const _int& iCout /* = 0 */)
{
	return m_pTexInfo;
}

CSingleTexture* CSingleTexture::Create(const wstring& wstrFilePath , const wstring& wstrStateKey /*= L"" */, const _int& iCount /*= 0 */)
{
	CSingleTexture* pInstance = new CSingleTexture;

	if ( FAILED(pInstance->InsertTexture(wstrFilePath,wstrStateKey, iCount)))
	{
		::Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CSingleTexture::Release()
{
	m_pTexInfo->pTexture->Release();

	::Safe_Delete(m_pTexInfo);

	delete this;

	return 0;
}
