/// \file ParticleManager.h
/// \brief Interface for the particle manager class CParticleManager.

#pragma once

#include "Particle.h"
#include "AbstractList.h"

/// \brief The particle manager.
///
/// The particle manager is responsible for managing particles.

class CParticleManager: public CAbstractList <CParticle>{
  public:
    CParticleManager(int size); ///< Constructor.
    CParticle* create(SpriteType sprite, Vector2 position); ///< Create a particle.
    void move(); ///< Move all particles.
    void draw(); ///< Draw all particles.
}; //CParticleManager