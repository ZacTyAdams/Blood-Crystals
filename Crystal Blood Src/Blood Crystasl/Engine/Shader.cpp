/// \file Shader.cpp
/// \brief Code for the Shader class CShader.

#include "Shader.h"
//#include "gamerenderer.h"
#include "defines.h"
#include "Abort.h"
#include "debug.h"

//extern CGameRenderer GameRenderer;

/// Constructor, which initializes member variables and creates the  
/// input element descriptor array.
/// \param n Size of the input element descriptor array.

CShader::CShader(int n){
  m_pIEDesc = new D3D11_INPUT_ELEMENT_DESC[n];
  m_nMaxDescs = n;
  m_nNumDescs = 0;
  m_pVertexShader = nullptr;
  m_pPixelShader = nullptr;
} //constructor

CShader::~CShader(){
  delete [] m_pIEDesc;

  SAFE_RELEASE(m_pInputLayout);
  SAFE_RELEASE(m_pVertexShader);
  SAFE_RELEASE(m_pPixelShader);
} //destructor

/// Add an input element descriptor to the input element descriptor array.
/// \param offset Aligned byte offset.
/// \param fmt Color format.
/// \param name Semantic name.
/// \return true if succeeded, false if array full.

bool CShader::AddInputElementDesc(UINT offset, DXGI_FORMAT fmt, LPCSTR name){
  if(m_nNumDescs < m_nMaxDescs){
    m_pIEDesc[m_nNumDescs].AlignedByteOffset = offset;
    m_pIEDesc[m_nNumDescs].Format = fmt;
    m_pIEDesc[m_nNumDescs].InputSlot = 0;
    m_pIEDesc[m_nNumDescs].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    m_pIEDesc[m_nNumDescs].InstanceDataStepRate = 0;
    m_pIEDesc[m_nNumDescs].SemanticIndex = 0;
    m_pIEDesc[m_nNumDescs].SemanticName = name;

    m_nNumDescs++;

    return true;
  } //if
  else return false;
} //AddInputElementDesc

/// Create and compile a vertex shader.
/// \param fileName Name of file containing vertex shader.
/// \param entryPoint Name of function to call in that file.

bool CShader::VSCreateAndCompile(ID3D11Device2* pDev2, LPCWSTR fileName, LPCSTR entryPoint){
  HRESULT hr;
  ID3DBlob* code = nullptr;
  ID3DBlob* errorMsgs = nullptr;

  hr = D3DCompileFromFile(fileName, 0, 0, entryPoint, "vs_5_0",  0, 0, &code, &errorMsgs);

  if(FAILED(hr)){
    if(errorMsgs){
      ABORT("Vertex shader error: %s\n", (char*)errorMsgs->GetBufferPointer());
      errorMsgs->Release();
    } //if
    return false;
  } //if

  hr = pDev2->CreateVertexShader(code->GetBufferPointer(),
    code->GetBufferSize(), nullptr, &m_pVertexShader);

  if(FAILED(hr)){
    if(errorMsgs){
      ABORT("Vertex shader error: %s\n", (char*)errorMsgs->GetBufferPointer());
      errorMsgs->Release();
    } //if
    return false;
  } //if
  
  hr = pDev2->CreateInputLayout(m_pIEDesc, m_nMaxDescs,
    code->GetBufferPointer(), code->GetBufferSize(), &m_pInputLayout);

  SAFE_RELEASE(code);
  SAFE_RELEASE(errorMsgs);

  return true;
} //VSCreateAndCompile

/// Create and compile a pixel shader.
/// \param fileName Name of file containing pixel shader.
/// \param entryPoint Name of function to call in that file.

bool CShader::PSCreateAndCompile(ID3D11Device2* pDev2, LPCWSTR fileName, LPCSTR entryPoint){
  HRESULT hr;
  ID3DBlob* code;
  ID3DBlob* errorMsgs;

  hr = D3DCompileFromFile(fileName, 0, 0, entryPoint, "ps_4_0", 
    0, 0, &code, &errorMsgs);

  if(FAILED(hr)){
    if(errorMsgs){
      ABORT("Pixel shader error: %s\n", (char*)errorMsgs->GetBufferPointer());
      errorMsgs->Release();
    } //if
    return false;
  } //if

  hr = pDev2->CreatePixelShader(code->GetBufferPointer(), 
    code->GetBufferSize(), nullptr, &m_pPixelShader);

  if(FAILED(hr)){
    if(errorMsgs){
      ABORT("Pixel shader error: %s\n", (char*)errorMsgs->GetBufferPointer());
      errorMsgs->Release();
    } //if
    return false;
  } //if

  SAFE_RELEASE(code);
  SAFE_RELEASE(errorMsgs);

  return true;
} //PSCreateAndCompile

/// Set the renderer's vertex and pixel shaders.

void CShader::SetShaders(ID3D11DeviceContext2* pDC2){
  if(pDC2){
    pDC2->IASetInputLayout(m_pInputLayout);
    pDC2->VSSetShader(m_pVertexShader, nullptr, 0);
    pDC2->PSSetShader(m_pPixelShader, nullptr, 0);
  } //if
} //SetShaders