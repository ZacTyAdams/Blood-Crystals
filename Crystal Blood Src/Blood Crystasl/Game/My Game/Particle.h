/// \file particle.h
/// \brief Interface for the particle class CParticle.

#pragma once

#include "gamedefines.h"

/// \brief The particle. 

class CParticle{ //class for a particle
  friend class CParticleManager;
  friend class CSpring; 
  friend class CSpringManager;
  friend class CObjectWorld; 
  friend class CBody;

  private:
    SpriteType m_nSpriteType; ///< Sprite type.
    Vector2 m_vPos; ///< Current position.
    Vector2 m_vOldPos; ///< Previous position, needed for Verlet.
    float m_fRadius; ///< Radius of bounding circle.
    float m_fAngle; ///< Orientation angle.
    float m_fXScale; ///< Scale along x-axis.
    float m_fYScale; ///< Scale along y-axis.

    float EdgeCollision(); ///< Collision detection and response for edges of screen.

  public:
    CParticle(SpriteType sprite, Vector2 position); ///< Constructor.
    CParticle(); ///< Constructor.
    void move(); ///< Change position depending on time and velocity.
    void DeliverImpulse(float angle, float magnitude); ///< Deliver an impulse.
}; //CParticle