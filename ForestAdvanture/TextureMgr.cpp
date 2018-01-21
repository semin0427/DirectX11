#include "stdafx.h"
#include "TextureMgr.h"
#include "Texture.h"
#include "SingleTexture.h"
#include "MultiTexture.h"

IMPLEMENT_SINGLETON(CTextureMgr)

CTextureMgr::CTextureMgr()
{

}

CTextureMgr::~CTextureMgr()
{

}

HRESULT CTextureMgr::InsertTexture(const wstring& wstrFilePath
								   , const wstring& wstrObjKey
								   , TEXTYPE _eType
								   , const wstring& wstrStateKey /* = L"" */
								   , const _int& iCount /* = 0 */)
{
	MAPTEX::iterator iter = m_mapTexture.find(wstrObjKey);

	m_wsrFullPath = wstrFilePath;

	if ( iter == m_mapTexture.end() )
	{
		CTexture* pTexture = NULL;

		switch (_eType)
		{
		case TEX_SINGLE:
			pTexture = CSingleTexture::Create(wstrFilePath, wstrStateKey, iCount);
			break;
		case TEX_MULTI:
			pTexture = CMultiTexture::Create(wstrFilePath, wstrStateKey, iCount);
			break;
		}

		if (NULL != pTexture)
		{
			m_mapTexture.insert( MAPTEX::value_type(wstrObjKey, pTexture));
		}
	}
	else
	{
			if(TEX_MULTI == _eType )
			{
				iter->second->InsertTexture(wstrFilePath, wstrStateKey, iCount);
			}

	}


	return S_OK;
}

HRESULT CTextureMgr::ReadImagePath(wstring wstrImgPath)
{
	wifstream			LoadFile;

	LoadFile.open(wstrImgPath.c_str(), ios::in);


	TCHAR		szObjKey[MIN_STR]	= L"";
	TCHAR		szStateKey[MIN_STR] = L"";
	TCHAR		szCount[MIN_STR]	= L"";
	TCHAR		szImgPath[MAX_PATH] = L"";

	wstring		wstrCombine = L"";

	while(!LoadFile.eof())
	{
		LoadFile.getline(szObjKey, MIN_STR, '|');
		LoadFile.getline(szStateKey, MIN_STR, '|');
		LoadFile.getline(szCount, MIN_STR, '|');	
		LoadFile.getline(szImgPath, MAX_PATH);	

		int	iCount = _ttoi(szCount);

		if(FAILED(InsertTexture(szImgPath, szObjKey, TEX_MULTI, szStateKey, iCount)))
		{
			return E_FAIL;
		}

	}

	LoadFile.close();

	return S_OK;
}

const TEXINFO* CTextureMgr::GetTexture(const wstring&  wstrObjKey/* = L"" */, const wstring& wstrStateKey, const _int& iCout /* = 0 */)
{
	MAPTEX::iterator iter = m_mapTexture.find(wstrObjKey);

	if ( iter == m_mapTexture.end())
		return NULL;

	return iter->second->GetTexture(wstrStateKey, iCout);
}

_int CTextureMgr::GetImageCount(const wstring& wstrObjKey, wstring wstrStateKey)
{
	MAPTEX::iterator iter = m_mapTexture.find(wstrObjKey);
	if ( iter == m_mapTexture.end())
		return -1;

	return ((CMultiTexture*)iter->second)->GetImageCount(wstrStateKey);
}

const	 std::wstring& CTextureMgr::GetFilePath(void)
{
	return m_wsrFullPath;
}

void CTextureMgr::SetName(wstring wstrName)
{
	m_wsrFullPath = wstrName;
}

_ulong CTextureMgr::Release(void)
{
	for_each( m_mapTexture.begin(), m_mapTexture.end(), DeleteMap() );
	m_mapTexture.clear();

	return 0;
}
