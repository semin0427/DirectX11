////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


//////////////
// INCLUDES //
//////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
//#include "colorshaderclass.h"
#include "TextureShaderClass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include "bitmapclass.h"
#include "TextClass.h"

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void ShutDown();
	bool Frame();

private:
	bool Render(float);

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	//ModelClass* m_Model;
	//TextureShaderClass* m_TextureShader;
	//LightShaderClass* m_LightShader;
	//LightClass* m_Light;
	//BitmapClass* m_Bitmap;
	TextClass* m_Text;
};

#endif
