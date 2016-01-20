/// \file sprite.h
/// \brief Interface for the sprite class C3DSprite.

#pragma once

//#include <windows.h>
//#include <windowsx.h>

#include "Defines.h"
#include "Shader.h"
#include "WICTextureLoader.h"

//#include <d3d9.h>
//#include <d3dx9.h>

/// \brief The sprite. 
///
/// The sprite class contains basic information for drawing a
/// billboard sprite in Render World.

class C3DSprite{ //3D sprite
  friend class CSpriteManager;

  private:
    ID3D11ShaderResourceView* m_pTexture; ///< Pointer to texture containing the sprite image.

    ID3D11Buffer* m_pVertexBuffer; ///< Vertex buffer.
    BILLBOARDVERTEX* m_pVertexBufferData; ///< Vertex buffer data.

    ID3D11Device2* m_pDev2; ///< D3D device.
    ID3D11DeviceContext2* m_pDC2; ///< Device context.
    
    void LoadTexture(char* fname, int* w, int* h);

  public:
    C3DSprite(ID3D11Device2* pDev2, ID3D11DeviceContext2* pDC2); ///< Constructor.
    ~C3DSprite(); ///< Destructor.

    BOOL Load(char* filename); ///< Load texture image from file.
    void Draw(); ///< Draw sprite.
    void Release(); ///< Release sprite.
}; //C3DSprite