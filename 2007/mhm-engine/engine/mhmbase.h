//==========================================================================================
// Mayhem engine base include file AS PCH !!!
// Coded:	MozG
// Create:	7 may 2006
//==========================================================================================

#ifdef WIN32

#include <windows.h>
// DirectX vary block start
#ifdef MHM_USE_DX8
#include <d3d8.h>
#include <d3dx8.h>
#define MHMDirect3DDevice		IDirect3DDevice8
#define MHMDirect3D				IDirect3D8
#define MHMDirect3DIndexBuffer	IDirect3DIndexBuffer8
#define MHMDirect3DVertexBuffer	IDirect3DVertexBuffer8
#define MHMLight				D3DLIGHT8
#define MHMDirect3DTexture		IDirect3DTexture8
//#define MHMDirect3DBaseTexture	IDirect3DBaseTexture8
#define MHMMaterial				D3DMATERIAL8
#else
#include <d3d9.h>
#include <d3dx9.h>
#define MHMDirect3DDevice		IDirect3DDevice9
#define MHMDirect3D				IDirect3D9
#define MHMDirect3DIndexBuffer	IDirect3DIndexBuffer9
#define MHMDirect3DVertexBuffer	IDirect3DVertexBuffer9
#define MHMLight				D3DLIGHT9
#define MHMDirect3DTexture		IDirect3DTexture9
#define MHMMaterial				D3DMATERIAL9
#endif
// DirectX vary block end

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <typeinfo.h>

#endif

#include <stdio.h>
#include <stdarg.h>

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "mhmtypes.h"	// mayhem types definitions
#include "mhmException.h"
#include "mhmarray.h"

#include "mhmmacro.h"	// mayhem macros definitions
#include "mhmPool.h"	// mayhem memory pool class
#include "mhmlist.h"	// mayhem list template definition
#include "mhmstring.h"	// mayhem string class
#include "mhmfile.h"	// mayhem file class
#include "mhmtimer.h"
#include "mhmmath.h"	// mayhem macth classes


#include "mhmSoundManager.h"
#include "mhmini.h"
#include "mhmlog.h"
#include "mhmdevcfg.h"
#include "mhmdxutil.h"

#include "mhmVertexDeclaration.h"
#include "mhmd3dbufs.h"
#include "mhmresource.h"
#include "mhmdevice.h"

#include "mhmbasetex.h"
#include "mhmtextga.h"
#include "mhmTextureJPG.h"
#include "mhmTextureJP2.h"
#include "mhmVertexShader.h"
#include "mhmPixelShader.h"
#include "mhmBaseMaterial.h"
#include "mhmBaseMesh.h"
#include "mhmStaticMesh.h"
#include "mhmsimpbox.h"
#include "mhmStaticMeshMMH.h"
#include "mhmfont2d.h"
#include "mhmBaseLight.h"
#include "mhmBaseParticles.h"
#include "mhmSprite.h"
#include "mhmresman.h"
#include "mhmText.h"


#include "mhmVariable.h"
#include "mhmVarParser.h"

#include "mhmVar.h"

#include "mhmAnim.h"

#include "mhmActor.h"
#include "mhmCamera.h"
#include "mhmRenderer.h"
#include "mhmKeyboard.h"
#include "mhmMouse.h"
#include "mhmInput.h"
#include "mhmAnimation.h"
#include "mhmEntityFactory.h"
#include "mhmEntity.h"
//#include "mhmDataProvider.h"
#include "mhmEngine.h"
#include "mhmApp.h"

#include "mhmRectangle.h"

//#include "mhmWinMouse.h"

