#include "stdafx.h"
#include "MultiTexture.h"
#include "GraphicDev.h"

CMultiTexture::CMultiTexture(void)
	: CTexture()
{

}

CMultiTexture::~CMultiTexture(void)
{

}

HRESULT CMultiTexture::InsertTexture(const wstring& wstrFilePath , const wstring& wstrStateKey /* = L"" */, const _int& iCount /* = 0 */)
{
	TCHAR szPath[MAX_PATH] = L"";

	vector<TEXINFO*>		vecTexture;

	for ( int i = 0 ; i < iCount ; ++i )
	{
		wsprintf(szPath, wstrFilePath.c_str(), i);

		TEXINFO* m_pTexInfo = new TEXINFO;
		ZeroMemory(m_pTexInfo, sizeof(TEXINFO));

		if ( FAILED(D3DXGetImageInfoFromFile(szPath, &m_pTexInfo->tImgInfo ) ) )
			return E_FAIL;

		if ( FAILED( D3DXCreateTextureFromFileEx( GETSINGLE(CGraphicDev)->GetDevice()
			, szPath
			, m_pTexInfo->tImgInfo.Width
			, m_pTexInfo->tImgInfo.Height
			, m_pTexInfo->tImgInfo.MipLevels
			, 0, m_pTexInfo->tImgInfo.Format, D3DPOOL_MANAGED,
			D3DX_DEFAULT, D3DX_DEFAULT, NULL, &m_pTexInfo->tImgInfo, NULL, &m_pTexInfo->pTexture	) ))
			return E_FAIL;

		vecTexture.push_back(m_pTexInfo);
	}
	
	m_pMapTexInfo.insert( map<wstring, vector<TEXINFO*>>::value_type(wstrStateKey, vecTexture));

	return S_OK;
}

const TEXINFO* CMultiTexture::GetTexture(const wstring& wstrStateKey /* = L"" */, const _int& iCout /* = 0 */)
{
	map<wstring, vector<TEXINFO*>>::iterator iter = m_pMapTexInfo.find(wstrStateKey);

	if (iter == m_pMapTexInfo.end())
		return NULL;

	return iter->second[iCout];
}

_int CMultiTexture::GetImageCount(wstring wstrStateKey)
{
	map<wstring, vector<TEXINFO*>>::iterator iter = m_pMapTexInfo.find(wstrStateKey);

	if (iter == m_pMapTexInfo.end())
		return NULL;

	return iter->second.size();
}

CMultiTexture* CMultiTexture::Create(const wstring& wstrFilePath , const wstring& wstrStateKey /*= L"" */, const _int& iCount /*= 0 */)
{
	CMultiTexture* pInstance = new CMultiTexture;

	if ( FAILED(pInstance->InsertTexture(wstrFilePath,wstrStateKey, iCount)))
	{
		::Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CMultiTexture::Release()
{
	map<wstring, vector<TEXINFO*>>::iterator iter = m_pMapTexInfo.begin();
	map<wstring, vector<TEXINFO*>>::iterator iter_end = m_pMapTexInfo.end();

	for ( ; iter != iter_end ; ++iter )
	{
		vector<TEXINFO*>::iterator  veciter = iter->second.begin();
		vector<TEXINFO*>::iterator  veciter_end = iter->second.end();

		for ( ; veciter != veciter_end ; ++veciter )
			(*veciter)->pTexture->Release();

		for_each( iter->second.begin() , iter->second.end(), DeleteOBJ() );
		iter->second.clear();
		vector<TEXINFO*>().swap(iter->second);
	}
	
	m_pMapTexInfo.clear();
	
	delete this;

	return 0;
}
