/// \file body.h
/// \brief Interface for the body class CBody.

#pragma once

#include "Spring.h"
#include "Particle.h"
#include "gamedefines.h"

/// \brief Enumerated type for points on the ragdoll.
enum PointType{
  HEAD_POINT, STERNUM_POINT, LEFT_HIP_POINT, RIGHT_HIP_POINT, LEFT_SHOULDER_POINT,
  RIGHT_SHOULDER_POINT, LEFT_ELBOW_POINT, RIGHT_ELBOW_POINT, LEFT_HAND_POINT, RIGHT_HAND_POINT,
  LEFT_KNEE_POINT, RIGHT_KNEE_POINT, LFOOT_POINT, RIGHT_FOOT_POINT, NUM_POINTS
}; //PointType

/// \brief Enumerated type for bones on the ragdoll.
enum EdgeType{
  NECK_BONE,
  LEFT_COLLAR_BONE, LEFT_HUMERUS_BONE, LEFT_ULNUS_BONE,
  RIGHT_COLLAR_BONE, RIGHT_HUMERUS_BONE, RIGHT_ULNUS_BONE,  
  LEFT_THIGH_BONE, LEFT_SHIN_BONE,
  RIGHT_THIGH_BONE, RIGHT_SHIN_BONE,

  HIP_BRACER, LEFT_SHOULDER_BRACER, RIGHT_SHOULDER_BRACER,
  SHOULDER_BRACER, LEFT_NECK_BRACER, RIGHT_NECK_BRACER,

  LEFT_HEAD_SPRING, RIGHT_HEAD_SPRING,
  FEET_SPRING,
  LEFT_FOOT_SPRING, RIGHT_FOOT_SPRING,
  LEFT_KNEE_SPRING, RIGHT_KNEE_SPRING,
  HANDS_SPRING,
  LEFT_ELBOW_SPRING, RIGHT_ELBOW_SPRING,
  LEFT_HAND_SPRING, RIGHT_HAND_SPRING,
  NUM_EDGES
}; //EdgeType

/// \brief The body.

class CBody{
  private:
    CParticle** m_pPt; ///< Array of pointers to particles in this body.
    int m_nParticleCount; ///< Number of particles in this body currently.
    int m_nParticleMax; ///< Maximum number of particles allowed in this body.

    CSpring** m_pSpring; ///< Array of pointers to springs in this body.
    CParticle** m_pCtr; ///< Array of pointers to spring centers.
    int m_nSpringCount; ///< Number of springs in this body currently.
    int m_nSpringMax; ///< Maximum number of springs allowed in this body.

    CParticle* m_pEdgeParticle;  ///< Pointer to an arbitrary particle on the edge of this body.

    CParticleManager* m_pPMan; ///< Pointer to the particle manager, for convenience.
    CSpringManager* m_pSMan; ///< Pointer to the spring manager, for convenience.

    void ChooseSprites(SpriteType& ball, SpriteType& spring, float r); ///< Choose sprites for particles and edges.
    void ConnectSpring(int p0, int p1, int s, float r=0.5f); ///< Connect spring to particles.

    void CreateEdgeCenter(int edge, SpriteType sprite); ///< Create sprite to represent edge. 
    CParticle* CreatePoint(int point, SpriteType sprite, Vector2 v); ///< Create sprite to represent particle.

  public:
    CBody(CParticleManager* p, CSpringManager* s); ///< Constructor.
    ~CBody(); ///< Destructor.

    CParticle* MakeChain(int count, int radius, float r, float angle); ///< Make a chain body.
    CParticle* MakeTriangle(int radius, float r); ///< Make a triangle body.
    CParticle* MakeSquare(int radius, float r); ///< Make a square body.
    CParticle* MakeWheel(int sides, int radius, float r); ///< Make a wheel body.
    CParticle* MakeRagdoll(); ///< Make a ragdoll body.

    void DeliverImpulse(float angle, float magnitude); ///< Deliver impulse to body.
    void ApplyTorque(float angle, float magnitude); ///< Apply a torque by delivering impulse to edge particle.
    void Teleport(float xdelta, float ydelta); ///< Magically teleport body by this amount.
    void move(); ///< Move body.
}; //CBody