/// \file ParticleManager.cpp
/// \brief Code for the particle manager class CParticleManager.

#include "ParticleManager.h"

#include "Particle.h"
#include "Spring.h"
#include "renderworld.h"

extern CRenderWorld g_cRenderWorld;
extern int g_nScreenWidth;

CParticleManager::CParticleManager(int size):CAbstractList(size){
} //constructor

CParticle* CParticleManager::create(SpriteType sprite, Vector2 position){
  CParticle* p = CAbstractList::create();
  if(p){
    p->m_nSpriteType = sprite;
    p->m_vPos = p->m_vOldPos = position;
  } //if
  return p;
} //create

/// Move the game particles.

void CParticleManager::move(){ 
  for(int i=0; i<m_nSize; i++){ //for each particle slot
    CParticle* p = m_pList[i]; //handy particle pointer
    if(p)p->move(); //make it move
  } //for
} //move

/// Ask the Render World to draw all of the game particles.

void CParticleManager::draw(){ 
  for(int i=0; i<m_nSize; i++){ //for each particle slot
    CParticle* p = m_pList[i]; //handy particle pointer
    if(p){ //if there's a particle there
      Vector3 v = Vector3(p->m_vPos.x, p->m_vPos.y, (float)-i);
      g_cRenderWorld.draw(p->m_nSpriteType, v, 
        p->m_fAngle, p->m_fXScale, p->m_fYScale);
    } //if
  } //for
} //draw