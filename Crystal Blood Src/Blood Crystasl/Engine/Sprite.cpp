/// \file sprite.cpp
/// \brief Code for the sprite class C3DSprite.

#include "sprite.h"
#include "defines.h"
#include "debug.h"

extern int g_nScreenHeight, g_nScreenWidth;

C3DSprite::C3DSprite(ID3D11Device2* pDev2, ID3D11DeviceContext2* pDC2): 
m_pDev2(pDev2), m_pDC2(pDC2), m_pTexture(nullptr), m_pVertexBuffer(nullptr){
  m_pVertexBufferData = new BILLBOARDVERTEX[4];
} //constructor

C3DSprite::~C3DSprite(){ 
  delete m_pTexture;
  delete [] m_pVertexBufferData;
} //destructor

/// Load an image from a file into a D3D texture. 
/// \param v Pointer to D3D texture to receive the image
/// \param fname Name of the file containing the texture
/// \param w Pointer to a variable that receives the texture width
/// \param h Pointer to a variable that receives the texture height 

void C3DSprite::LoadTexture(char* fname, int* w, int* h){
  wchar_t  ws[100];
  swprintf(ws, 100, L"%hs", fname);
  CreateWICTextureFromFile(m_pDev2, m_pDC2, ws, nullptr, &m_pTexture, 0);

  //get texture width and height
  ID3D11Resource* r;
  D3D11_TEXTURE2D_DESC desc;
  m_pTexture->GetResource(&r);
  ((ID3D11Texture2D*)r)->GetDesc(&desc);

  if(w)*w = desc.Width;
  if(h)*h = desc.Height;
} //LoadTexture

/// Load the sprite image into a texture from a given file
/// name and create a vertex buffer for the billboard image containing 4 
/// corner vertices spaced apart the appropriate width and height.
/// \param filename The name of the image file

BOOL C3DSprite::Load(char* filename){
  int width, ht; //width and height of texture image
  LoadTexture(filename, &width, &ht);
  
  //load vertex buffer
  float w = width/2.0f;
  float h = ht/2.0f;
  
  //vertex information, first triangle in clockwise order
  m_pVertexBufferData[0].p = Vector3(w, h, 0.0f);
  m_pVertexBufferData[0].tu = 1.0f; m_pVertexBufferData[0].tv = 0.0f;
  
  m_pVertexBufferData[1].p = Vector3(w, -h, 0.0f);
  m_pVertexBufferData[1].tu = 1.0f; m_pVertexBufferData[1].tv = 1.0f;
  
  m_pVertexBufferData[2].p = Vector3(-w, h, 0.0f);
  m_pVertexBufferData[2].tu = 0.0f; m_pVertexBufferData[2].tv = 0.0f;
  
  m_pVertexBufferData[3].p = Vector3(-w, -h, 0.0f);
  m_pVertexBufferData[3].tu = 0.0f; m_pVertexBufferData[3].tv = 1.0f;
  
  //create vertex buffer
  D3D11_SUBRESOURCE_DATA subresourceData;
  subresourceData.pSysMem = m_pVertexBufferData;
  subresourceData.SysMemPitch = 0;
  subresourceData.SysMemSlicePitch = 0;
  
  D3D11_BUFFER_DESC d3d_vbDesc; //vertex buffer descriptor.
  d3d_vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  d3d_vbDesc.ByteWidth = sizeof(BILLBOARDVERTEX)* 4;
  d3d_vbDesc.CPUAccessFlags = 0;
  d3d_vbDesc.MiscFlags = 0;
  d3d_vbDesc.StructureByteStride = 0;
  d3d_vbDesc.Usage = D3D11_USAGE_IMMUTABLE;
    
  HRESULT hr = m_pDev2->CreateBuffer(&d3d_vbDesc, &subresourceData, &m_pVertexBuffer);
  
  return SUCCEEDED(hr); //successful
} //Load

/// Draw the sprite image.

void C3DSprite::Draw(){
  UINT offset = 0;
  UINT nVertexBufferStride = sizeof(BILLBOARDVERTEX);

  m_pDC2->PSSetShaderResources(0, 1, &m_pTexture); //set sprite texture
  m_pDC2->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &nVertexBufferStride, &offset);
  m_pDC2->Draw(4, 0);
} //Draw

/// Release sprite vertex buffer and textures.

void C3DSprite::Release(){ 
  SAFE_RELEASE(m_pVertexBuffer); //release vertex buffer
  SAFE_RELEASE(m_pTexture); //release texture
} //Release