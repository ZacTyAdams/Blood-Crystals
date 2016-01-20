/// \file particle.cpp
/// \brief Code for the particle class CParticle.

#include "Particle.h"

#include "Sound.h"
#include "sndlist.h"
#include "debug.h"

extern int g_nScreenWidth; 
extern int g_nScreenHeight; 
extern CSoundManager* g_pSoundManager; 
extern BodyType g_nCurrentBody;

CParticle::CParticle(SpriteType sprite, Vector2 position){ //constructor
  m_nSpriteType = sprite; 
  m_fAngle = 0.0f; m_fXScale = m_fYScale = 1.0f;
  m_fRadius = 32.0f; // From the image file
  m_vPos = m_vOldPos = position;
} //constructor

CParticle::CParticle(){ //constructor
  m_nSpriteType = INVISIBLE_SPRITE; 
  m_vPos = m_vOldPos = Vector2(0, 0);
  m_fAngle = 0.0f; m_fXScale = m_fYScale = 1.0f;
  m_fRadius = 32.0f; // From the image file
} //constructor

/// Check for collision with an edge.
/// Collision and response for particle hitting an edge of the screen. 
/// Checks for a collision, and does the necessary housework for reflecting
/// a particle if it hits an edge. Function
/// backs off the particle so that it does not appear to overlap the edge.
/// \return Collision speed, if particle hits an edge.

float CParticle::EdgeCollision(){ 
  const float fRestitution = 0.6f; //how bouncy the edges are 
  Vector2 vDelta = m_vPos - m_vOldPos; //velocity

  float left, right, top, bottom;
  left = bottom = m_fRadius;
  right = g_nScreenWidth - m_fRadius;
  top = g_nScreenHeight - m_fRadius;

  float cspeed = 0.0f; //collision speed

  //left and right edges
  if(m_vPos.x < left || m_vPos.x > right){ 
    m_vPos.x = m_vPos.x < left? left: right; 
    vDelta.y = -vDelta.y;
    m_vOldPos = m_vPos + fRestitution * vDelta; 
    cspeed = fabs(vDelta.x);
  } //if

  //top and bottom edges
  if(m_vPos.y < bottom || m_vPos.y > top){
    m_vPos.y = m_vPos.y < bottom? bottom: top; 
    vDelta.x = -vDelta.x;
    m_vOldPos = m_vPos + fRestitution * vDelta; 
    cspeed = fabs(vDelta.y);
  } //if

  return cspeed;
} //EdgeCollision

/// Move the particle, apply collision and response.

void CParticle::move(){ //move particle using Verlet integration.
  Vector2 vTemp = m_vPos;
  m_vPos += m_vPos - m_vOldPos; 
  m_vOldPos = vTemp;
  m_vPos.y -= 0.2f; //gravity

  float cspeed = EdgeCollision(); //collision speed
  if(cspeed > 0.25f){ //edge collision
    if(g_nCurrentBody == RAGDOLL_BODY){
      if(cspeed > 5.0f)
        g_pSoundManager->play(OW_SOUND);
    } //if
    else if(m_nSpriteType == BALL_SPRITE){
      g_pSoundManager->play(BOING_SOUND);
      g_pSoundManager->volume(cspeed/10.0f);
    } //if
    else if(m_nSpriteType == WOODCIRCLE_SPRITE){
      g_pSoundManager->play(THUMP_SOUND);
      g_pSoundManager->volume(cspeed/10.0f);
    } //if
  } //if
} //move

/// Deliver an impulse to the particle, given the angle and magnitude.
/// \param angle Angle at which the impulse is to be applied.
/// \param magnitude Magnitude of the impulse to apply.

void CParticle::DeliverImpulse(float angle, float magnitude){ 
  m_vOldPos = m_vPos - magnitude * Vector2(cos(angle), sin(angle)); //Verlet again
} //DeliverImpulse