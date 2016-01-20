/// \file Springs.cpp
/// \brief Code for the spring class CSpring.

#include "Spring.h"

extern int g_nScreenWidth; 
extern int g_nScreenHeight;

/// Clip function.
/// \param a Smallest allowable value.
/// \param b Value to be clipped.
/// \param c Largest allowable value.
/// \return The value b clipped between a and c.

inline float clip(float a, float b, float c){
  return max(a, min(b, c));
} //clip

CSpring::CSpring(){ //constructor
  m_pV0 = m_pV1 = m_pCenter = nullptr;
  m_fRestLength = 100.0f; //fair-sized
  m_fRestitution = 0.5f; //stick
} //constructor

/// Perform a single iteration of Gauss-Seidel relaxation to the spring.

void CSpring::Relax(){ //One round of Gauss-Seidel relaxation
  Vector2 vDelta = m_pV0->m_vPos - m_pV1->m_vPos;
  float fLength = vDelta.Length(); //stick current length

  if(fabs(fLength - m_fRestLength) > 0.5f){ //if different enough
    //relax
    vDelta *= 1.0f - m_fRestLength/fLength; //amount to change by
    vDelta *= m_fRestitution; //springiness
    m_pV0->m_vPos -= vDelta; //some from one end
    m_pV1->m_vPos += vDelta; //some from the other
    //edge collision response
    float r = m_pV0->m_fRadius;
    m_pV0->m_vPos.x = clip(r, m_pV0->m_vPos.x, g_nScreenWidth-r-1);
    m_pV0->m_vPos.y = clip(r, m_pV0->m_vPos.y, g_nScreenHeight-r-1);
    r = m_pV1->m_fRadius;
    m_pV1->m_vPos.x = clip(r, m_pV1->m_vPos.x, g_nScreenWidth-r-1);
    m_pV1->m_vPos.y = clip(r, m_pV1->m_vPos.y, g_nScreenHeight-r-1);
  } //if
} //Relax

/// The particles at the ends of the spring are moved and relaxed like any
/// sensible object should be. The center of the spring just gets dragged around.
/// This is where it catches up.

void CSpring::ComputeCenter(){
  if(!m_pCenter)return; //fail and bail
  Vector2 p0 = m_pV0->m_vPos; //p0 is the position of one end
  Vector2 p1 = m_pV1->m_vPos; //p1 is the other
  Vector2 v = p0 - p1; //vector from p1 to p0
  m_fAngle = m_pCenter->m_fAngle = atan2(v.y, v.x); //the angle between them
  m_pCenter->m_vOldPos = m_pCenter->m_vPos = (p0 + p1)/2.0f; //center position
  m_pCenter->m_fXScale = v.Length()/256.0f; //scale needed for sprite
} //ComputeCenter