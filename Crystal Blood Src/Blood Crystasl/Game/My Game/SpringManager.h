/// \file SpringManager.h
/// \brief Interface for the spring manager class CSpringManager.

#pragma once

#include "AbstractList.h"
#include "Particle.h"
#include "Spring.h"

/// \brief The spring manager.

class CSpringManager: public CAbstractList <CSpring>{
  public:
    CSpringManager(int size); ///< Constructor.
    CSpring* ConnectSpring(CParticle* v0, CParticle* v1, 
      CParticle* cntr, float restitution); ///< Add a spring.
    void Relax(int iterations);  ///< Gauss-Seidel relaxation.
    void move(); ///< Move springs.
}; //CSpringManager