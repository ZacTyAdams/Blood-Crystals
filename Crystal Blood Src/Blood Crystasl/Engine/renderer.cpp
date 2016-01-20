/// \file renderer.cpp
/// \brief Direct3D rendering.
/// DirectX stuff that won't change much is hidden away in this file
/// so you won't have to keep looking at it.

#include "renderer.h"
#include "D2d1_1.h"
#include "d2d1effects.h"
#include "defines.h"
#include "abort.h"
#include "SpriteMan.h"
#include "Timer.h"
#include "tinyxml2.h"
#include "Shader.h"
#include "debug.h"
#include <iostream>

using namespace tinyxml2;

extern int g_nScreenWidth;
extern int g_nScreenHeight;
extern CTimer g_cTimer;
extern HWND g_HwndApp;
extern XMLElement* g_xmlSettings;

CRenderer::CRenderer():m_pDev2(nullptr){
  m_matWorld = XMMatrixIdentity();
  m_matView = XMMatrixIdentity();
  m_matProj = XMMatrixIdentity();
} //constructor

/// All textures used in the game are released - the release function is kind
/// of like a destructor for DirectX entities, which are COM objects.

void CRenderer::Release(){ 
  m_cSpriteManager.Release(); 
  SAFE_RELEASE(m_pRasterizerState);
  SAFE_RELEASE(m_pBackgroundTexture);
  SAFE_RELEASE(m_pBackgroundVB);
  SAFE_RELEASE(m_pBlendState); 
} //Release

/// Creates a D3D device and swap chain, sets the graphics mode, creates a vertex buffer 
/// for the background.
///  \param hInstance Instance handle
///  \param hwnd Window handle
///  \return TRUE if it succeeded

BOOL CRenderer::Initialize(HINSTANCE hInstance, HWND hwnd){ 
  HRESULT hr; //result from D3D function calls

  //create d3d device and swap chain for mode
  hr = CreateD3DDeviceAndSwapChain(hwnd);
  if(FAILED(hr)){ //bail if failed
    m_pDev2 = nullptr; return FALSE;
  } //if
  m_cSpriteManager.SetDevice(m_pDev2, m_pDC2);

  //graphics settings
  if(FAILED(CreateDepthBuffer()))
    return FALSE;
  if(FAILED(CreateRasterizer()))
    return FALSE;
  CreateViewport();
  
  //transformation matrices
  float w = g_nScreenWidth/2.0f;
  float h = g_nScreenHeight/2.0f;

  m_matView = XMMatrixLookAtLH(Vector3(w, h, -350.0f), Vector3(w, h, 1000.0f), Vector3(0, 1, 0));
  m_matProj = XMMatrixOrthographicOffCenterLH(-w, w, -h, h, 1.0f, 10000.0f);

  //set the blend state, to be used by sprites that have am alpha-channel
  D3D11_BLEND_DESC1 blendDesc;
  blendDesc.AlphaToCoverageEnable = FALSE;
  blendDesc.IndependentBlendEnable = FALSE;
  blendDesc.RenderTarget[0].BlendEnable = TRUE;
  blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
  blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
  blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
  blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
  blendDesc.RenderTarget[0].LogicOp = D3D11_LOGIC_OP_CLEAR;
  blendDesc.RenderTarget[0].LogicOpEnable = FALSE;
  blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
  blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
  blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;

  hr = m_pDev2->CreateBlendState1(&blendDesc, &m_pBlendState);
  
  m_nScreenWidth = g_nScreenWidth;
  m_nScreenHeight = g_nScreenHeight;
  
  return TRUE; //success exit
} //Initialize

/// Set the world matrix m_matWorld to a scale, then a rotation, then a translation.
/// \param v Translation vector.
/// \param angle Orientation angle, rotation about the Z axis.
/// \param xsize Scale in the x direction.
/// \param ysize Scale in the y direction.

void CRenderer::SetWorldMatrix(const Vector3& v, const float angle, float xsize, float ysize){
  m_matWorld = XMMatrixScaling(xsize, ysize, 1.0f) * XMMatrixRotationZ(angle) * XMMatrixTranslation(v.x, v.y, v.z);
} //SetWorldMatrix

/// Compose the world, view, and projection transformations.
/// \return Product of the world, view, and projection matrices.

XMFLOAT4X4 CRenderer::CalculateWorldViewProjectionMatrix(){
  XMFLOAT4X4 f;
  XMStoreFloat4x4(&f, XMMatrixTranspose(m_matWorld * m_matView * m_matProj));
  return f;
} //CalculateWorldViewProjectionMatrix

/// Initialize the vertex and constant buffers for the background.

void CRenderer::InitBackground(){
  HRESULT hr;

  //load vertex buffer
  float w = (float)g_nScreenWidth;
  float h = (float)g_nScreenHeight;
  
  //vertex information, first triangle in clockwise order
  BILLBOARDVERTEX pVertexBufferData[4]; 
  pVertexBufferData[0].p = Vector3(w, 0, 1500);
  pVertexBufferData[0].tu = 1.0f; pVertexBufferData[0].tv = 1.0f;

  pVertexBufferData[1].p = Vector3(0, 0, 1500);
  pVertexBufferData[1].tu = 0.0f; pVertexBufferData[1].tv = 1.0f;

  pVertexBufferData[2].p = Vector3(w, h, 1500);
  pVertexBufferData[2].tu = 1.0f; pVertexBufferData[2].tv = 0.0f;

  pVertexBufferData[3].p = Vector3(0, h, 1500);
  pVertexBufferData[3].tu = 0.0f; pVertexBufferData[3].tv = 0.0f;
  
  //create vertex buffer for background
  m_pShader = new CShader(2);
    
  m_pShader->AddInputElementDesc(0, DXGI_FORMAT_R32G32B32_FLOAT, "POSITION");
  m_pShader->AddInputElementDesc(12, DXGI_FORMAT_R32G32_FLOAT,  "TEXCOORD");
  m_pShader->VSCreateAndCompile(m_pDev2, L"VertexShader.hlsl", "main");
  m_pShader->PSCreateAndCompile(m_pDev2, L"PixelShader.hlsl", "main");
    
  // Create constant buffers.
  D3D11_BUFFER_DESC constantBufferDesc = { 0 };
  constantBufferDesc.ByteWidth = sizeof(ConstantBuffer);
  constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  constantBufferDesc.CPUAccessFlags = 0;
  constantBufferDesc.MiscFlags = 0;
  constantBufferDesc.StructureByteStride = 0;
    
  m_pDev2->CreateBuffer(&constantBufferDesc, nullptr, &m_pConstantBuffer);
    
  D3D11_BUFFER_DESC VertexBufferDesc;
  VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  VertexBufferDesc.ByteWidth = sizeof(BILLBOARDVERTEX)* 6;
  VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  VertexBufferDesc.CPUAccessFlags = 0;
  VertexBufferDesc.MiscFlags = 0;
  VertexBufferDesc.StructureByteStride = 0;
    
  D3D11_SUBRESOURCE_DATA subresourceData;
  subresourceData.pSysMem = pVertexBufferData;
  subresourceData.SysMemPitch = 0;
  subresourceData.SysMemSlicePitch = 0;
    
  hr = m_pDev2->CreateBuffer(&VertexBufferDesc, &subresourceData, &m_pBackgroundVB);
} //InitBackground

/// Load the background image after getting the file name from g_xmlSettings.

void CRenderer::LoadBackground(){
  char buffer[256];
  //image file names
  XMLElement* imagesTag = g_xmlSettings->FirstChildElement("images"); 
  if(imagesTag){
    XMLElement* imageSubTag = imagesTag->FirstChildElement("image");     
    while(imageSubTag && strcmp("background", imageSubTag->Attribute("name")))
      imageSubTag = imageSubTag->NextSiblingElement("image");
    if(imageSubTag){ //got "image" tag with right name
      int len = strlen(imageSubTag->Attribute("src")); //length of name string
      strncpy_s(buffer, 256, imageSubTag->Attribute("src"), len); //copy file name string
      buffer[len] = '\0'; //NULL at end of string
      LoadTexture(m_pBackgroundTexture, buffer);
    } //if image tag with correct name found
    else LoadTexture(m_pBackgroundTexture, "No background file specified in gamesettings.xml");
  } // if images tag found
} //LoadBackground

void CRenderer::LoadSplash(){
	char buffer[256];
	//image file names
	XMLElement* imagesTag = g_xmlSettings->FirstChildElement("images");
	if (imagesTag){
		XMLElement* imageSubTag = imagesTag->FirstChildElement("image");
		while (imageSubTag && strcmp("splash", imageSubTag->Attribute("name")))
			imageSubTag = imageSubTag->NextSiblingElement("image");
		if (imageSubTag){ //got "image" tag with right name
			int len = strlen(imageSubTag->Attribute("src")); //length of name string
			strncpy_s(buffer, 256, imageSubTag->Attribute("src"), len); //copy file name string
			buffer[len] = '\0'; //NULL at end of string
			LoadTexture(m_pBackgroundTexture, buffer);
		} //if image tag with correct name found
		else LoadTexture(m_pBackgroundTexture, "No background file specified in gamesettings.xml");
	} // if images tag found
} //LoadBackground

/// Draw the background, which is a simple image.

void CRenderer::DrawBackground(){

  SetWorldMatrix(Vector3(0.0f));

  UINT nVertexBufferOffset = 0;
  
  UINT nVertexBufferStride = sizeof(BILLBOARDVERTEX);
  m_pDC2->IASetVertexBuffers(0, 1, &m_pBackgroundVB, &nVertexBufferStride, &nVertexBufferOffset);
  m_pDC2->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
  m_pShader->SetShaders(m_pDC2);
  m_pDC2->PSSetShaderResources(0, 1, &m_pBackgroundTexture);  
  
  ConstantBuffer constantBufferData; 
  constantBufferData.wvp = CalculateWorldViewProjectionMatrix();
  m_pDC2->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &constantBufferData, 0, 0);
  m_pDC2->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
  m_pDC2->Draw(4, 0);
} //DrawBackground

void CRenderer::BeginFrame(){
  m_pDC2->OMSetRenderTargets(1, &m_pRTV, m_pDSV);
  float clearColor[] = { 1.0f, 1.0f, 1.0f, 0.0f };
  m_pDC2->ClearRenderTargetView(m_pRTV, clearColor);
  m_pDC2->ClearDepthStencilView(m_pDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
} //BeginFrame

/// End of frame processing, meaning that we preent the rendered image in video
/// memory to the monitor screen. Do not unchain from vertical retrace or else
/// bad things happen to the physics engine in the relaxation demo.

void CRenderer::EndFrame(){
  m_pSwapChain2->Present(1, 0); //present frame
} //EndFrame

/// Load a sprite using the Sprite Manager to do the heavy lifting.
/// \param sprite Index of sprite in Sprite Manager.
/// \param name Name of sprite in GameSettings.xml

void CRenderer::Load(int sprite, char* name){ // Load sprite.
  m_cSpriteManager.Load(sprite, name);
} //Load

/// Load an image from a file into a D3D texture. 
/// \param v Pointer to D3D texture to receive the image
/// \param fname Name of the file containing the texture
/// \param w Pointer to a variable that receives the texture width
/// \param h Pointer to a variable that receives the texture width 

void CRenderer::LoadTexture(ID3D11ShaderResourceView* &v, char* fname, int* w, int* h){
  wchar_t  ws[100];
  swprintf(ws, 100, L"%hs", fname);
  CreateWICTextureFromFile(m_pDev2, m_pDC2, ws, nullptr, &v, 0);

  //get texture width and height
  ID3D11Resource* r;
  D3D11_TEXTURE2D_DESC desc;
  v->GetResource(&r);
  ((ID3D11Texture2D*)r)->GetDesc(&desc);

  if(w)*w = desc.Width;
  if(h)*h = desc.Height;
} //LoadTexture

/// Draw a sprite, using the Sprite Manager to do the heavy lifting.
/// \param sprite Index of sprite in Sprite Manager.
/// \param x Horizontal coordinate in screen space.
/// \param y Vertical coordinate in screen space.
/// \param angle Orientation in radians.
/// \param xsize Width of sprite, 1.0 is actual size.
/// \param ysize Height of sprite, 1.0 is actual size.

void CRenderer::draw(int sprite, const Vector3& s, float angle, float xsize, float ysize){   
  SetWorldMatrix(Vector3(s.x, s.y, s.z), angle, xsize, ysize);
  XMFLOAT4X4 m = CalculateWorldViewProjectionMatrix();
    
  m_pShader->SetShaders(m_pDC2);

  m_pDC2->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
  m_pDC2->OMSetBlendState(m_pBlendState, nullptr, 0xffffffff);
  
  ConstantBuffer constantBufferData; 
  constantBufferData.wvp = CalculateWorldViewProjectionMatrix();
  m_pDC2->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &constantBufferData, 0, 0);
  m_pDC2->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);

  m_cSpriteManager.Draw(sprite);
} //draw

void CRenderer::drawAnimated(int* sprites, int betweenFrames, int frames, const Vector3& s, float angle, float xsize, float ysize) { //Allows for multiframe sprite

	int sprite; //Sprite to Draw
	int spriteRotation = frames * betweenFrames; //Takes total sprite time
	int currentSprite = g_cTimer.time() % spriteRotation; //takes current time and modulates it based on spriteRotation
	int frame = currentSprite / betweenFrames; //divides the spriteRotation by frames to get the frame number to call (int rounds down)
	sprite = sprites[frame]; //sets the sprite to the right frame

	SetWorldMatrix(Vector3(s.x, s.y, s.z), angle, xsize, ysize);
	XMFLOAT4X4 m = CalculateWorldViewProjectionMatrix();

	m_pShader->SetShaders(m_pDC2);

	m_pDC2->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	m_pDC2->OMSetBlendState(m_pBlendState, nullptr, 0xffffffff);

	ConstantBuffer constantBufferData;
	constantBufferData.wvp = CalculateWorldViewProjectionMatrix();
	m_pDC2->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &constantBufferData, 0, 0);
	m_pDC2->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	m_cSpriteManager.Draw(sprite);
} //draw

void CRenderer::drawMovement(int sprites[5][4], int betweenFrames, int frames, float movementSpeed, const Vector3& start, const Vector3& final, float angle, float xsize, float ysize) {
	Vector3 tempLocation = start;
	if (final.x > tempLocation.x) {
		while (final.x > tempLocation.x) {
			tempLocation.x += movementSpeed;
			drawAnimated(sprites[0], betweenFrames, frames, tempLocation);
		}
	}
	else if (final.x < tempLocation.x) {
		while (final.x < tempLocation.x) {
			tempLocation.x -= movementSpeed;
			drawAnimated(sprites[0], betweenFrames, frames, tempLocation);
		}
	}
	else {
		//do nothing
	}
	if (final.y > tempLocation.y) {
		while (final.y > tempLocation.y) {
			tempLocation.y += movementSpeed;
			drawAnimated(sprites[0], betweenFrames, frames, tempLocation);
		}
	}
	else if (final.y < tempLocation.y) {
		while (final.y < tempLocation.y) {
			tempLocation.y -= movementSpeed;
			drawAnimated(sprites[0], betweenFrames, frames, tempLocation);
		}
	}
	else {
		//do nothing
	}
}

/// Draw a sprite, using the Sprite Manager to do the heavy lifting. This time the vertical
/// coordinate is measured down from top of screen.
/// \param sprite Index of sprite in Sprite Manager.
/// \param x Horizontal coordinate in screen space.
/// \param y Vertical coordinate in screen space, measured from top of screen.
/// \param angle Orientation in radians.
/// \param size Size of sprite, 1.0 is actual size.

void CRenderer::drawtop(int sprite, float x, float y, float angle, float size){    
 // draw(sprite, x, g_nScreenHeight - y, angle, size);
} //draw

/// \param w Width.
/// \param h Height.

void CRenderer::GetWorldSize(int &w, int &h){
  w = m_nScreenWidth; h = m_nScreenHeight;
} //GetWorldSize

/// Set and initialize the device, device context, and swap chain.
/// Assign them to the member pointers m_pDev2, m_pDC2, and m_pSwapChain2.
/// \param hwnd Window handle
/// \param mode Display mode (width, height, color depth, refresh rate)
/// \return The error code returned by D3D

HRESULT CRenderer::CreateD3DDeviceAndSwapChain(HWND hwnd){ 
  //set up device specs data structure
  DXGI_SWAP_CHAIN_DESC d3dscd; //Swap chain specifications.
  ZeroMemory(&d3dscd, sizeof(DXGI_SWAP_CHAIN_DESC)); //null it out
  d3dscd.BufferCount = 1;
  d3dscd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //color mode
  d3dscd.BufferDesc.Height = g_nScreenHeight; //height
  d3dscd.BufferDesc.RefreshRate.Denominator = 1;
  d3dscd.BufferDesc.RefreshRate.Numerator = 60;
  d3dscd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
  d3dscd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
  d3dscd.BufferDesc.Width = g_nScreenWidth; //width
  d3dscd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  d3dscd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
  d3dscd.OutputWindow = hwnd;
  d3dscd.SampleDesc.Count = 1;
  d3dscd.SampleDesc.Quality = 0;
  d3dscd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
  d3dscd.Windowed = true;

  //now we can create the device and swap chain
  
  IDXGISwapChain* pSwapChain = nullptr; //Swap chain.
  ID3D11Device* pDev = nullptr; //D3D device.
  ID3D11DeviceContext* pDC = nullptr; //Device context.
  D3D_FEATURE_LEVEL fl; //Feature level.

  HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
    nullptr, 0, nullptr, 0, D3D11_SDK_VERSION,
    &d3dscd, &pSwapChain, &pDev, &fl, &pDC);
  
  m_pDev2 = (ID3D11Device2*)pDev;
  m_pDC2 = (ID3D11DeviceContext2*)pDC;
  m_pSwapChain2 = (IDXGISwapChain2*)pSwapChain;

  ID3D11Texture2D* backBuffer = nullptr;
  
  hr = pSwapChain->ResizeBuffers(1, g_nScreenWidth, g_nScreenHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
  hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
  hr = m_pDev2->CreateRenderTargetView(backBuffer, 0, &m_pRTV);
  SAFE_RELEASE(backBuffer);

  return hr;
} //CreateD3DDeviceAndSwapChain

/// Create the Z or depth buffer using a depth stencil.
/// This function assumes that m_pDev2 has been initialized.
/// \return The error code returned by D3D, 1 if m_pDev2 is NULL

HRESULT CRenderer::CreateDepthBuffer(){ 
  if(!m_pDev2)return 1; //bail and fail

  D3D11_TEXTURE2D_DESC depthStencilDesc;
  depthStencilDesc.Width = g_nScreenWidth;
  depthStencilDesc.Height = g_nScreenHeight;
  depthStencilDesc.MipLevels = 1;
  depthStencilDesc.ArraySize = 1;
  depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  depthStencilDesc.SampleDesc.Count = 1; // multisampling must match
  depthStencilDesc.SampleDesc.Quality = 0; // swap chain values.
  depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
  depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
  depthStencilDesc.CPUAccessFlags = 0;
  depthStencilDesc.MiscFlags = 0;

  ID3D11Texture2D* depthStencilBuffer = nullptr;
  m_pDSV = nullptr;
  m_pDev2->CreateTexture2D(&depthStencilDesc, 0, &depthStencilBuffer);
  m_pDev2->CreateDepthStencilView(depthStencilBuffer, 0, &m_pDSV);

  m_pDC2->OMSetRenderTargets(1, &m_pRTV, m_pDSV);
  
  ID3D11DepthStencilState* pDepthStencilState = nullptr;

  D3D11_DEPTH_STENCIL_DESC dhStencilDesc;

  // Depth test parameters
  dhStencilDesc.DepthEnable = true;
  dhStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
  dhStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

  // Stencil test parameters
  dhStencilDesc.StencilEnable = false;
  dhStencilDesc.StencilReadMask = 0xFF;
  dhStencilDesc.StencilWriteMask = 0xFF;

  // Stencil operations if pixel is front-facing
  dhStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  dhStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
  dhStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  dhStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

  // Stencil operations if pixel is back-facing.
  dhStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  dhStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
  dhStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  dhStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

  HRESULT hr = m_pDev2->CreateDepthStencilState(&dhStencilDesc, &pDepthStencilState);
  m_pDC2->OMSetDepthStencilState(pDepthStencilState, 1);

  return hr;
} //CreateDepthBuffer

/// Create the rasterizer and attach it to the device context.
/// This function assumes that m_pDev2 has been initialized.
/// \return The error code returned by D3D, 1 if m_pDev2 is NULL

HRESULT CRenderer::CreateRasterizer(){
  if(!m_pDev2)return 1; //bail and fail

  ZeroMemory(&m_rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC1));
  m_rasterizerDesc.CullMode = D3D11_CULL_BACK;
  m_rasterizerDesc.FillMode = D3D11_FILL_SOLID;
  m_rasterizerDesc.FrontCounterClockwise = false;

  m_pRasterizerState = nullptr;
  HRESULT hr = m_pDev2->CreateRasterizerState1(&m_rasterizerDesc, &m_pRasterizerState);
  m_pDC2->RSSetState(m_pRasterizerState);

  return hr;
} //CreateRasterizer

/// Create the viewport and attach it to the device context.
/// This function assumes that m_pDev2 has been initialized.

void CRenderer::CreateViewport(){
  if(!m_pDev2)return; //bail and fail
  
  D3D11_VIEWPORT vp;
  vp.Height = (float)g_nScreenHeight;
  vp.MaxDepth = 1.0f;
  vp.MinDepth = 0.0f;
  vp.TopLeftX = 0;
  vp.TopLeftY = 0;
  vp.Width = (float)g_nScreenWidth;

  m_pDC2->RSSetViewports(1, &vp);
} //CreateViewport
