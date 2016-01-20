/// \file renderer.h 
/// \brief Definition of the renderer class CRenderer.

#pragma once

#include "spriteman.h"

#include <D3Dcompiler.h>

#include "WICTextureLoader.h"
#include "defines.h"


/// \brief The renderer.
///
/// The renderer class handles all the nasty Direct3D details that I don't
/// want readers to get into, or even be distracted by.

class CRenderer{
  friend void InitGame();
  friend class CShader;
  friend class C3DSprite;
  
  protected:
    ID3D11Device2* m_pDev2; ///< D3D device.
    ID3D11DeviceContext2* m_pDC2; ///< Device context.
    IDXGISwapChain2* m_pSwapChain2; ///< Swap chain.

    ID3D11RenderTargetView* m_pRTV; ///< Render target view.
    ID3D11DepthStencilView* m_pDSV; ///< Depth stencil view.

    XMMATRIX m_matWorld; ///< World matrix.
    XMMATRIX m_matView; ///< View matrix.
    XMMATRIX m_matProj; ///< Projection matrix.
    
    ID3D11BlendState1* m_pBlendState; ///< Blend state.

  protected:
    HRESULT CreateD3DDeviceAndSwapChain(HWND hwnd); ///< Create D3d device.
    HRESULT CreateDepthBuffer(); ///< Create Z buffer.
    HRESULT CreateRasterizer(); ///< Create rasterizer.
    void CreateViewport(); ///< Create viewport.

    void SetWorldMatrix(const Vector3& v, const float angle=0.0f, float xsize=1.0f, float ysize=1.0f); ///< Set the world matrix.

    ID3D11RasterizerState1* m_pRasterizerState; ///< Rasterizer state.
    D3D11_RASTERIZER_DESC1 m_rasterizerDesc; ///< Rasterizer description.

  protected:
    ID3D11Buffer* m_pBackgroundVB;  ///< Vertex buffer for background.
    ID3D11ShaderResourceView* m_pBackgroundTexture; ///< Texture for background.
    ID3D11Buffer* m_pConstantBuffer; ///< Constant buffer for shader.
    CShader* m_pShader; ///< Pointer to an instance of the shader class.

    CSpriteManager m_cSpriteManager; ///< Sprite manager.
    int m_nScreenWidth; ///< Screen width in pixels.
    int m_nScreenHeight; ///< Screen height in pixels.

  public:
    CRenderer(); ///< Constructor.
    BOOL Initialize(HINSTANCE hInstance, HWND hwnd); ///< Initialize Direct3D.
    void InitBackground(); ///< Initialize background.
    XMFLOAT4X4 CalculateWorldViewProjectionMatrix(); ///< Compute product of world, view, and projection matrices. 
    void DrawBackground(); ///< Draw the background to the screen.
    void LoadTexture(ID3D11ShaderResourceView* &v, char* fname,
      int* w=0, int* h=0); ///< Load texture from a file.
    void LoadBackground(); ///< Load background image from a file.
	void LoadSplash();
    void BeginFrame(); ///< Begin an animation frame.
    void EndFrame(); ///< End an animation frame.

    void Load(int sprite, char* name); ///< Load sprite.
    void draw(int sprite, const Vector3& s, float angle=0.0f, float xsize=1.0f, float ysize=1.0f); ///< Draw sprite.
	void drawAnimated(int* sprites, int betweenFrames, int frames, const Vector3& s, float angle = 0.0f, float xsize = 1.0f, float ysize = 1.0f);
	void drawMovement(int sprites[5][4], int betweenFrames, int frames, float movementSpeed, const Vector3& start, const Vector3& final, float angle = 0.0f, float xsize = 1.0f, float ysize = 1.0f);
    void drawtop(int sprite, float x, float y, float angle=0.0f, float size=1.0f); ///< Draw sprite at top of screen.

    void GetWorldSize(int &x, int &y); ///< Get the Render World width and height.
    void Release(); ///< Release textures and vertex buffer.
}; //CRenderer