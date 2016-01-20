/// \file SpriteMan.h
/// \brief Interface for the sprite manager class CSpriteManager.

#pragma once

#include "defines.h"
#include "sprite.h"
#include "tinyxml2.h"


const int NUM_SPRITES = 512; ///< Maximum allowable number of sprites in the Sprite Manager.

/// \brief The sprite manager.

class CSpriteManager{
  private:
    C3DSprite* m_pSprite[NUM_SPRITES]; ///< Sprite pointers.
    char m_pBuffer[MAX_PATH]; ///< File name buffer.
    //LPDIRECT3DDEVICE9 m_pd3dDevice;
    ID3D11Device2* m_pDev2; ///< D3D device.
    ID3D11DeviceContext2* m_pDC2; ///< Device context.

    BOOL LoadFile(int sprite, const char* file); ///< Load sprite.
  public:
    CSpriteManager(); ///< Constructor
    ~CSpriteManager(); ///< Destructor
    void Load(int sprite, char* name); ///< Load sprite.
    void Draw(int sprite); ///< Draw sprite.
    void Release();  ///< Release textures.
    void SetDevice(ID3D11Device2* pDev2, ID3D11DeviceContext2* pDC2);  ///< Set the Direct3D device.
}; //CSpriteManager