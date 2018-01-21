/*!
 * \file GraphicDev.cpp
 * \date 2015/02/25 11:45
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef GraphicDev_h__
#define GraphicDev_h__

#include "Defines.h"


class CGraphicDev
{
public:
	DECLARE_SINGLETON(CGraphicDev)

public:
	enum WINMODE {MODE_FULL, MODE_WIN};
private:
	explicit CGraphicDev(void);
	~CGraphicDev(void);
public: // getter
	LPDIRECT3DDEVICE9 GetDevice(void) {
		return m_pDevice;}
public:	// setter
	void SetRenderState(_D3DRENDERSTATETYPE Type, const DWORD& dwFlag) {
		m_pDevice->SetRenderState(Type, dwFlag); }
	void SetSamplerState(_D3DSAMPLERSTATETYPE Type, const DWORD& dwFlag)	{		
		m_pDevice->SetSamplerState(0, Type, dwFlag);}
	void	Release_SDK(void) { ::Safe_Release(m_pSDK); }
public:
	HRESULT InitGraphicDev(WINMODE Mode, HWND hWnd, const WORD& wSizeX, const WORD& wSizeY);	

public:
	void Render_begin(void);
	void Render_end(void);

	LPD3DXSPRITE			GetSprite(void)  { return m_pSprite; }
	LPD3DXFONT			GetFont(void)  { return m_pFont; }
	LPD3DXLINE				GetLine(void)  { return m_pLine; }


private:
	LPDIRECT3D9							m_pSDK;
	LPDIRECT3DDEVICE9				m_pDevice; // 장치를 대표하는 객체다.

	LPD3DXSPRITE						m_pSprite;
	LPD3DXFONT						m_pFont;
	LPD3DXLINE							m_pLine;
private:
	void SetParameters(D3DPRESENT_PARAMETERS& d3dpp, WINMODE Mode, HWND hWnd, const WORD& wSizeX, const WORD& wSizeY);
public:
	_ulong Release(void);
};


#endif // GraphicDev_h__
