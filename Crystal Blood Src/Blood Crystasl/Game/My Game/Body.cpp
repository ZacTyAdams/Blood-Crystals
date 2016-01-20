/// \file body.cpp
/// \brief Code for the body class CBody.

#include "Body.h"

#include "objectworld.h"

extern int g_nScreenWidth, g_nScreenHeight;

CBody::CBody(CParticleManager* p, CSpringManager* s){ //constructor
  m_pPMan = p; m_pSMan = s;
  m_pPt = nullptr; m_nParticleCount = m_nParticleMax = 0;
  m_pSpring = nullptr; m_nSpringCount = m_nSpringMax = 0;
  m_pEdgeParticle = nullptr;
} //constructor

CBody::~CBody(){ //destructor
  delete [] m_pPt;
  delete [] m_pSpring;
} //destructor

/// Choose the kind of sprites used for this body, sticks or springs.
/// \param ball The sprite to be used for the particles.
/// \param spring The sprite to be used for the edges.
/// \param r The spring restitution value, used to decide whether this is a stick or a spring.

void CBody::ChooseSprites(SpriteType& ball, SpriteType& spring, float r){
  if(r > 0.49f){ //if coefficient of restitution is large, sticks and circles
    ball = WOODCIRCLE_SPRITE; spring = STICK_SPRITE;
  } //if
  else{ //springs and balls
    ball = BALL_SPRITE; spring = SPRING_SPRITE;
  } //else
} //ChooseSprites

/// Connect a spring between two particles.
/// \param p0 First particle.
/// \param p1 Second particle.
/// \param s The spring.
/// \param r Restitution value for the spring.

void CBody::ConnectSpring(int p0, int p1, int s, float r){
  m_pSpring[s] = m_pSMan->ConnectSpring(m_pPt[p0], m_pPt[p1], m_pCtr[s], r);
} //ConnectSpring

/// Create virtual particle for center of edge.
/// \param edge The edge in question.
/// \param sprite The type of sprite there.

void CBody::CreateEdgeCenter(int edge, SpriteType sprite){
  m_pCtr[edge] = m_pPMan->create(sprite, Vector2());
} //CreateEdgeCenter

/// Create particle for end of edge.
/// \param point Particle in question.
/// \param sprite Sprite to be drawn there.
/// \param v Location of particle.
/// \return Pointer to particle for end of edge.

CParticle* CBody::CreatePoint(int point, SpriteType sprite, Vector2 v){
  m_pPt[point] = m_pPMan->create(sprite, v);
  return m_pPt[point];
} //CreatePoint

/// Deliver impulse to body.
/// \param angle Orientation of impulse.
/// \param magnitude Magnitude of impulse.

void CBody::DeliverImpulse(float angle, float magnitude){
  for(int i=0; i<m_nParticleCount; i++)
    if(m_pPt[i])
      m_pPt[i]->DeliverImpulse(angle, magnitude);
} //DeliverImpulse

/// Deliver torque to body. This is achieved by delivering an impulse to a
/// particle on the outside of the body.
/// \param angle Orientation of torque.
/// \param magnitude Magnitude of torque.

void CBody::ApplyTorque(float angle, float magnitude){
  if(m_nParticleCount > 0 && m_pPt[1])
    m_pPt[1]->DeliverImpulse(angle, magnitude);
} //ApplyTorque

/// Teleport the body without moving any of the particle relative to each other.
/// \param xdelta Amount to move horizontally.
/// \param ydelta Amount to move vertically.

void CBody::Teleport(float xdelta, float ydelta){
 for(int i=0; i<m_nParticleCount; i++){
   m_pPt[i]->m_vPos.x += xdelta;
   m_pPt[i]->m_vPos.y += ydelta;
   m_pPt[i]->m_vOldPos.x += xdelta;
   m_pPt[i]->m_vOldPos.y += ydelta;
 } //for
} //Teleport

/// Move the body.
/// Since the components of the body (particles and springs) are responsible for moving themselves,
/// all we need to do here is to orient the particles at the ends and middle of the springs correctly.

void CBody::move(){
  for(int i=0; i<m_nSpringCount; i++)
    if(m_pSpring[i] && m_pSpring[i]->m_pCenter && m_pSpring[i]->m_pCenter->m_nSpriteType == STICK_SPRITE)
      m_pSpring[i]->m_pV0->m_fAngle = 
        m_pSpring[i]->m_pV1->m_fAngle = 
          m_pSpring[i]->m_pCenter->m_fAngle;
} //move

/// Make a chain of points in a straight line connected by springs.
/// \param count Number of points
/// \param radius Half the length of one of the springs.
/// \param r Coefficient of restitution of the springs.
/// \param angle Angle that the chain makes with the horizontal.

CParticle* CBody::MakeChain(int count, int radius, float r, float angle){
  if(count<2)return nullptr; //fail and bail

  //make space for particles and springs 
  m_nParticleCount = m_nParticleMax = count;
  m_pPt = new CParticle* [m_nParticleMax];
  m_nSpringCount = m_nSpringMax = count-1;
  m_pCtr = new CParticle* [m_nSpringMax];
  m_pSpring = new CSpring* [m_nSpringMax];

  //decide whether to draw springs or sticks
  SpriteType nVertexObject, nEdgeObject;
  ChooseSprites(nVertexObject, nEdgeObject, r);

  //x and y offsets between balls
  const float dx = 2.0f*radius*cos(angle); 
  const float dy = 2.0f*radius*sin(angle);

  //position for first ball
  Vector2 v = Vector2(
    g_nScreenWidth/2.0f - (count-1)*radius, //center chain on screen
    g_nScreenHeight/2.0f);  

  //first ball and spring
  CreateEdgeCenter(0, nEdgeObject);
  CreatePoint(0, nVertexObject, v);

  for(int i=1; i<count-1; i++){ //for the rest of the springs
    //add a ball and a spring, connect previous spring
    CreateEdgeCenter(i, nEdgeObject);
    v.x += dx; v.y += dy; //offset position
    CreatePoint(i, nVertexObject, v);
    ConnectSpring(i-1, i, i-1, r);
  } //for

  //last ball
  v.x += dx; v.y += dy;
  m_pPt[count-1] = m_pPMan->create(nVertexObject, v); 
  ConnectSpring(count-2, count-1, count-2, r);

  //clean up
  m_pEdgeParticle = m_pPt[0];
  delete [] m_pCtr;
  return m_pPt[0];
} //MakeChain

/// Make a triangle of points connected by springs.
/// \param radius Half the length of one of the springs.
/// \param r Coefficient of restitution of the springs.

CParticle* CBody::MakeTriangle(int radius, float r){ 
  //make space for particles and springs 
  m_nParticleCount = m_nParticleMax = 3;
  m_pPt = new CParticle* [m_nParticleMax];
  m_nSpringCount = m_nSpringMax = 3;
  m_pCtr = new CParticle* [m_nSpringMax];
  m_pSpring = new CSpring* [m_nSpringMax];
  
  //decide whether to draw springs or sticks
  SpriteType nVertexObject, nEdgeObject;
  ChooseSprites(nVertexObject, nEdgeObject, r);

  Vector2 v = Vector2( //default position
    g_nScreenWidth/2.0f,  g_nScreenHeight/2.0f + radius); 

  //edge objects
  for(int i=0; i<3; i++)
    CreateEdgeCenter(i, nEdgeObject);

  //balls
  CreatePoint(0, nVertexObject, v);
  v.x +=  radius;
  v.y -= radius * tan(XM_PI/3.0f);
  CreatePoint(1, nVertexObject, v);
  v.x -= 2.0f * radius;
  CreatePoint(2, nVertexObject, v);
  
  //tie springs to balls
  ConnectSpring(0, 1, 0, r); 
  ConnectSpring(1, 2, 1, r); 
  ConnectSpring(2, 0, 2, r); 

  //clean up
  m_pEdgeParticle = m_pPt[0];
  delete [] m_pCtr;
  return m_pPt[0];
} //MakeTriangle

/// Make a cross-braced square of points connected by springs.
/// \param radius Half the length of one of the springs.
/// \param r Coefficient of restitution of the springs.

CParticle* CBody::MakeSquare(int radius, float r){
  //make space for particles and springs 
  m_nParticleCount = m_nParticleMax = 4;
  m_pPt = new CParticle* [m_nParticleMax];
  m_nSpringCount = m_nSpringMax = 6;
  m_pCtr = new CParticle* [m_nSpringMax];
  m_pSpring = new CSpring* [m_nSpringMax];
  
 
  //decide whether to draw springs or sticks
  SpriteType nVertexObject, nEdgeObject;
  ChooseSprites(nVertexObject, nEdgeObject, r);

  Vector2 v = Vector2( //position
    g_nScreenWidth/2.0f - radius,  g_nScreenHeight/2.0f + radius); 

  //edge objects
  for(int i=0; i<6; i++)
    CreateEdgeCenter(i, nEdgeObject);

  //balls
  CreatePoint(0, nVertexObject, v);
  v.x += 2*radius; CreatePoint(1, nVertexObject, v);
  v.y -= 2*radius; CreatePoint(2, nVertexObject, v);
  v.x -= 2*radius; CreatePoint(3, nVertexObject, v);
  
  //tie springs to balls
  ConnectSpring(0, 1, 0, r); 
  ConnectSpring(1, 2, 1, r); 
  ConnectSpring(2, 3, 2, r); 
  ConnectSpring(3, 0, 3, r); 
  ConnectSpring(0, 2, 4, r); 
  ConnectSpring(1, 3, 5, r);
  
  //clean up
  delete [] m_pCtr;
  m_pEdgeParticle = m_pPt[0];
  return m_pPt[0];
} //MakeSquare

/// Make a cross-braced wheel of points connected by springs.
/// \param sides Number of sides to the wheel.
/// \param radius Half the length of one of the springs.
/// \param r Coefficient of restitution of the springs.

CParticle* CBody::MakeWheel(int sides, int radius, float r){

  //make space for particles and springs 
  m_nParticleCount = m_nParticleMax = sides+1;
  m_pPt = new CParticle* [m_nParticleMax];
  m_nSpringCount = m_nSpringMax = 2*sides;
  m_pCtr = new CParticle* [m_nSpringMax];
  m_pSpring = new CSpring* [m_nSpringMax];

  //decide whether to draw springs or sticks
  SpriteType nVertexObject, nEdgeObject;
  ChooseSprites(nVertexObject, nEdgeObject, r);

  Vector2 v = Vector2( //position
    g_nScreenWidth/2.0f,  g_nScreenHeight/2.0f + radius/2.0f); 

  //springs
  for(int i=0; i<2*sides; i++)
    CreateEdgeCenter(i, nEdgeObject);

  v = Vector2( //position
    g_nScreenWidth/2.0f,  g_nScreenHeight/2.0f + radius/2.0f);

  //balls
  CreatePoint(0, nVertexObject, v);
  for(int i=0; i<sides; i++){
    float theta = i * 2.0f * XM_PI/sides;
    Vector2 vBall = Vector2(v.x + radius*cos(theta), v.y + radius*sin(theta));    
    CreatePoint(i+1, nVertexObject, vBall);
  } //for

  //springs
  for(int i=0; i<sides; i++){
    ConnectSpring(0, i+1, i, r); //spoke
    ConnectSpring(i, i+1, sides+i-1, r); //rim
  } //for
  ConnectSpring(sides, 1, 2*sides-1, r); //rim
  
  //clean up
  delete [] m_pCtr;
  m_pEdgeParticle = m_pPt[1];
  return m_pPt[1];
} //MakeWheel

/// Make a cross-braced stick figure with spring constraints.

CParticle* CBody::MakeRagdoll(){ 
  m_nParticleCount = m_nParticleMax = NUM_POINTS;
  m_pPt = new CParticle* [m_nParticleMax];
  m_nSpringCount = m_nSpringMax = NUM_EDGES;
  m_pCtr = new CParticle* [m_nSpringMax];
  m_pSpring = new CSpring* [m_nSpringMax]; 

  //The springs and bracers that are not drawn don't have centers.
  for(int i=LEFT_HEAD_SPRING; i<=RIGHT_HAND_SPRING; i++) 
    m_pCtr[i] = nullptr;
  m_pCtr[HIP_BRACER] = nullptr;

  //root position where we start creating the ragdoll from
  Vector2 root = Vector2( //position
    g_nScreenWidth/2.0f,  0.5f*g_nScreenHeight); 

  //Various body measurements. Tune them to your requirements.
  const float TORSO_LENGTH = 170; //length of torso
  const float SHOULDER_LENGTH = 90; //width of collar bone
  const float SHOULDER_DROP = 40; //height of collar bone
  const float HIP_LENGTH = 42; //width of pelvis
  const float HIP_DROP = 40; //height of pelvis
  const float LIMB_LENGTH = 100; //length of limb segments
  const float NECK_LENGTH = 78; //length of neck, obviously
  const float ELBOW_OFFSET = 15; //elbows stick out this much
  const float LEG_SPREAD = 5; //feet spread out this much

  //Store point locations in a handy array for later.
  Vector2 vPointLoc[NUM_POINTS];

  vPointLoc[STERNUM_POINT] = 
    root + Vector2(0, TORSO_LENGTH);
  vPointLoc[HEAD_POINT] = 
    vPointLoc[STERNUM_POINT] + Vector2(0, NECK_LENGTH);
  vPointLoc[LEFT_SHOULDER_POINT] = 
    vPointLoc[STERNUM_POINT] + Vector2(-SHOULDER_LENGTH, -SHOULDER_DROP);
  vPointLoc[LEFT_ELBOW_POINT] = 
    vPointLoc[LEFT_SHOULDER_POINT] + Vector2(-ELBOW_OFFSET, -LIMB_LENGTH);
  vPointLoc[LEFT_HAND_POINT] = 
    vPointLoc[LEFT_ELBOW_POINT] + Vector2(0, -LIMB_LENGTH);  
  vPointLoc[RIGHT_SHOULDER_POINT] = 
    vPointLoc[STERNUM_POINT] + Vector2(SHOULDER_LENGTH, -SHOULDER_DROP);
  vPointLoc[RIGHT_ELBOW_POINT] = 
    vPointLoc[RIGHT_SHOULDER_POINT] + Vector2(ELBOW_OFFSET, -LIMB_LENGTH);
  vPointLoc[RIGHT_HAND_POINT] = 
    vPointLoc[RIGHT_ELBOW_POINT] + Vector2(0, -LIMB_LENGTH);  
  vPointLoc[LEFT_HIP_POINT] = 
    root + Vector2(-HIP_LENGTH, -HIP_DROP);
  vPointLoc[LEFT_KNEE_POINT] = 
    vPointLoc[LEFT_HIP_POINT] + Vector2(-LEG_SPREAD, -LIMB_LENGTH);
  vPointLoc[LFOOT_POINT] = 
    vPointLoc[LEFT_KNEE_POINT] + Vector2(-LEG_SPREAD, -LIMB_LENGTH);
  vPointLoc[RIGHT_HIP_POINT] = 
    root + Vector2(HIP_LENGTH, -HIP_DROP);
  vPointLoc[RIGHT_KNEE_POINT] = 
    vPointLoc[RIGHT_HIP_POINT] + Vector2(LEG_SPREAD, -LIMB_LENGTH);
  vPointLoc[RIGHT_FOOT_POINT] = 
    vPointLoc[RIGHT_KNEE_POINT] + Vector2(LEG_SPREAD, -LIMB_LENGTH);
 
  //create sticks with their end points
  //WARNING: Make sure to get the draw order right. It's a bit of a pain
  CreateEdgeCenter(LEFT_THIGH_BONE, STICK_SPRITE);
  CreateEdgeCenter(LEFT_SHIN_BONE, STICK_SPRITE);
  CreatePoint(LEFT_KNEE_POINT, WOODCIRCLE_SPRITE, vPointLoc[LEFT_KNEE_POINT]);
  CreatePoint(LFOOT_POINT, WOODCIRCLE_SPRITE, vPointLoc[LFOOT_POINT]);
  CreateEdgeCenter(RIGHT_THIGH_BONE, STICK_SPRITE);
  CreateEdgeCenter(RIGHT_SHIN_BONE, STICK_SPRITE);
  CreatePoint(RIGHT_KNEE_POINT, WOODCIRCLE_SPRITE, vPointLoc[RIGHT_KNEE_POINT]);
  CreatePoint(RIGHT_FOOT_POINT, WOODCIRCLE_SPRITE, vPointLoc[RIGHT_FOOT_POINT]);

  CreateEdgeCenter(NECK_BONE, STICK_SPRITE);

  //Make sure he'd not a pinhead. Zippy need not apply.
  CParticle* head = CreatePoint(HEAD_POINT, WOODCIRCLE_SPRITE, vPointLoc[HEAD_POINT]);
  const float fBigHead = 1.3f; //his head ball is this much bigger than a normal ball
  head->m_fXScale = head->m_fYScale = fBigHead; //looks bigger
  head->m_fRadius *= fBigHead; //is bigger

  CreateEdgeCenter(LEFT_SHOULDER_BRACER, STICK_SPRITE);
  CreateEdgeCenter(RIGHT_SHOULDER_BRACER, STICK_SPRITE);
  CreateEdgeCenter(SHOULDER_BRACER, STICK_SPRITE);
  CreateEdgeCenter(LEFT_COLLAR_BONE, STICK_SPRITE);
  CreateEdgeCenter(RIGHT_COLLAR_BONE, STICK_SPRITE);
  CreateEdgeCenter(LEFT_NECK_BRACER, STICK_SPRITE);
  CreateEdgeCenter(RIGHT_NECK_BRACER, STICK_SPRITE);
  CreatePoint(STERNUM_POINT, WOODCIRCLE_SPRITE, vPointLoc[STERNUM_POINT]);
  CreatePoint(LEFT_HIP_POINT, WOODCIRCLE_SPRITE, vPointLoc[LEFT_HIP_POINT]);
  CreatePoint(RIGHT_HIP_POINT, WOODCIRCLE_SPRITE, vPointLoc[RIGHT_HIP_POINT]);
  CreateEdgeCenter(LEFT_HUMERUS_BONE, STICK_SPRITE);
  CreatePoint(LEFT_SHOULDER_POINT, WOODCIRCLE_SPRITE, vPointLoc[LEFT_SHOULDER_POINT]);
  CreateEdgeCenter(LEFT_ULNUS_BONE, STICK_SPRITE);
  CreatePoint(LEFT_HAND_POINT, WOODCIRCLE_SPRITE, vPointLoc[LEFT_HAND_POINT]);
  CreatePoint(LEFT_ELBOW_POINT, WOODCIRCLE_SPRITE, vPointLoc[LEFT_ELBOW_POINT]);
  CreateEdgeCenter(RIGHT_HUMERUS_BONE, STICK_SPRITE);
  CreatePoint(RIGHT_SHOULDER_POINT, WOODCIRCLE_SPRITE, vPointLoc[RIGHT_SHOULDER_POINT]);
  CreateEdgeCenter(RIGHT_ULNUS_BONE, STICK_SPRITE);
  CreatePoint(RIGHT_HAND_POINT, WOODCIRCLE_SPRITE, vPointLoc[RIGHT_HAND_POINT]);
  CreatePoint(RIGHT_ELBOW_POINT, WOODCIRCLE_SPRITE, vPointLoc[RIGHT_ELBOW_POINT]);

  //connect objects at ends of sticks to center object representing the stick
  ConnectSpring(HEAD_POINT, STERNUM_POINT, NECK_BONE);
  ConnectSpring(STERNUM_POINT, LEFT_SHOULDER_POINT, LEFT_COLLAR_BONE);
  ConnectSpring(STERNUM_POINT, RIGHT_SHOULDER_POINT, RIGHT_COLLAR_BONE);
  ConnectSpring(LEFT_HIP_POINT, LEFT_KNEE_POINT, LEFT_THIGH_BONE);
  ConnectSpring(LEFT_KNEE_POINT, LFOOT_POINT, LEFT_SHIN_BONE);
  ConnectSpring(RIGHT_HIP_POINT, RIGHT_KNEE_POINT, RIGHT_THIGH_BONE);
  ConnectSpring(RIGHT_KNEE_POINT, RIGHT_FOOT_POINT, RIGHT_SHIN_BONE);
  ConnectSpring(LEFT_HIP_POINT, RIGHT_HIP_POINT, HIP_BRACER);
  ConnectSpring(RIGHT_HIP_POINT, STERNUM_POINT, RIGHT_SHOULDER_BRACER);
  ConnectSpring(LEFT_HIP_POINT, STERNUM_POINT, LEFT_SHOULDER_BRACER);
  ConnectSpring(LEFT_SHOULDER_POINT, RIGHT_SHOULDER_POINT, SHOULDER_BRACER);
  ConnectSpring(RIGHT_SHOULDER_POINT, LEFT_HIP_POINT, LEFT_NECK_BRACER);
  ConnectSpring(LEFT_SHOULDER_POINT, RIGHT_HIP_POINT, RIGHT_NECK_BRACER);
  ConnectSpring(LEFT_SHOULDER_POINT, LEFT_ELBOW_POINT, LEFT_HUMERUS_BONE);
  ConnectSpring(LEFT_ELBOW_POINT, LEFT_HAND_POINT, LEFT_ULNUS_BONE);
  ConnectSpring(RIGHT_SHOULDER_POINT, RIGHT_ELBOW_POINT, RIGHT_HUMERUS_BONE);
  ConnectSpring(RIGHT_ELBOW_POINT, RIGHT_HAND_POINT, RIGHT_ULNUS_BONE);

  //springs to keep limbs more or less in the right places
  ConnectSpring(HEAD_POINT, LEFT_SHOULDER_POINT, LEFT_HEAD_SPRING, 0.1f);
  ConnectSpring(HEAD_POINT, RIGHT_SHOULDER_POINT, RIGHT_HEAD_SPRING, 0.1f);

  ConnectSpring(LFOOT_POINT, RIGHT_FOOT_POINT, FEET_SPRING, 0.02f);
  ConnectSpring(LEFT_KNEE_POINT, RIGHT_HIP_POINT, LEFT_KNEE_SPRING, 0.06f);
  ConnectSpring(RIGHT_KNEE_POINT, LEFT_HIP_POINT, RIGHT_KNEE_SPRING, 0.06f);
  ConnectSpring(LFOOT_POINT, RIGHT_HIP_POINT, LEFT_FOOT_SPRING, 0.4f);
  ConnectSpring(RIGHT_FOOT_POINT, LEFT_HIP_POINT, RIGHT_FOOT_SPRING, 0.4f);
  
  ConnectSpring(LEFT_HAND_POINT, RIGHT_HAND_POINT, HANDS_SPRING, 0.01f);
  ConnectSpring(LEFT_ELBOW_POINT, RIGHT_SHOULDER_POINT, LEFT_ELBOW_SPRING, 0.01f);
  ConnectSpring(RIGHT_ELBOW_POINT, LEFT_SHOULDER_POINT, RIGHT_ELBOW_SPRING, 0.01f);
  ConnectSpring(LEFT_HAND_POINT, RIGHT_SHOULDER_POINT, LEFT_HAND_SPRING, 0.1f);
  ConnectSpring(RIGHT_HAND_POINT, LEFT_SHOULDER_POINT, RIGHT_HAND_SPRING, 0.1f);

  //clean up and exit  
  delete [] m_pCtr;
  m_pEdgeParticle = m_pPt[LEFT_SHOULDER_POINT];
  return m_pPt[HEAD_POINT];
} //MakeRagdoll