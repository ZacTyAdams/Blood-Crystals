/// \file Spring.h
/// \brief Interface for the spring class CSpring.

#pragma once

#include "Particle.h"
#include "AbstractList.h"

/// \brief The spring.

class CSpring{ 
  friend class CAbstractList<CSpring>;
  friend class CSpringManager; 
  friend class CBody;
  private:
    CParticle* m_pV0; ///< Particle at one end.
    CParticle* m_pV1; ///< Particle at the other end.
    CParticle* m_pCenter; ///< Virtual particle at the center.

    float m_fRestLength; ///< Rest length.
    float m_fRestitution; ///< Springiness, between 0.0f and 0.5f. 
    float m_fAngle; ///< Orientation.
  public:
    CSpring(); ///< Constructor.
    void Relax();  ///< Gauss-Seidel relaxation.
    void ComputeCenter(); ///< Compute spring center.
}; //CSpring