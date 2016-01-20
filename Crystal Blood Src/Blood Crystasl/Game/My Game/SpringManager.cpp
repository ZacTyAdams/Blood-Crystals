/// \file SpringManager.h
/// \brief Code for the spring manager CSpringManager.

#include "SpringManager.h"

CSpringManager::CSpringManager(int size):CAbstractList(size){
} //constructor

CSpring* CSpringManager::ConnectSpring(CParticle* v0, CParticle* v1,
  CParticle* cntr, float restitution)
{
  if(m_nCount >= m_nSize)return nullptr; //fail and bail
  CSpring* p = create(); //grab a new spring
  p->m_pV0 = v0; //set one end
  p->m_pV1 = v1; //set the other end
  p->m_pCenter = cntr; //set center

  //current length is rest length, so compute it and store
  Vector2 vDelta = v0->m_vPos - v1->m_vPos;
  p->m_fRestLength = vDelta.Length();

  p->m_fRestitution = restitution; //set restitution
  return p;
} //ConnectSpring

/// Perform Gauss-Seidel relaxation on a collection of springs. The more
/// iterations, the more stick-like the springs will be.
/// \param iterations Number of iterations of relaxation to perform.

void CSpringManager::Relax(int iterations){ // Gauss-Seidel relaxation
  for(int i=0; i<iterations; i++) //more iterations means more like a stick
    for(int j=0; j<m_nSize; j++){ //for each spring
      CSpring* p = m_pList[j];
      if(p)p->Relax();
    } //for
} //Relax

/// Springs don't move around in the normal way, instead they get dragged around
/// by their end points. All we need to do here is recompute their center points.

void CSpringManager::move(){
  for(int i=0; i<m_nSize; i++){
    CSpring* p = m_pList[i];
    if(p)p->ComputeCenter();
  } //for
} //move