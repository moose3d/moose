#include "Phoenix.h"
#include <iostream>
#include <SDL.h>
#include <sstream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <tcl.h>
#include "PhoenixEnergyBeam.h"
using std::string;
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Core;
using namespace Phoenix::Math;
using namespace Phoenix::Spatial;
using namespace Phoenix::Window;
using namespace Phoenix::Graphics;
using namespace Phoenix::Scene;
using namespace Phoenix::Volume;
using namespace Phoenix::AI;
using namespace Phoenix::Data;
using namespace Phoenix::Collision;
using std::cerr;
using std::endl; 
using std::ostringstream;
int g_bMousePressed =0 ;
/////////////////////////////////////////////////////////////////
#define g_PhoenixModelManager (CResourceManager<CModel, CHandle<CModel> >::GetInstance())
typedef CHandle<CModel> MODEL_HANDLE;
//////////////////////////////////////////////////////////////
enum OBJECT_TYPE 
{
  O_TYPE_ORDINARY = 0,
  O_TYPE_LOD = 1,
  O_TYPE_PS = 2,
  O_TYPE_SPACESHIP
};
/////////////////////////////////////////////////////////////////
class CRotatingParticle : public CParticle
{
public:
  float m_fRotationAngle;
  CRotatingParticle() : m_fRotationAngle(0) {}
  
};
/////////////////////////////////////////////////////////////////
template <class PARTICLE_TYPE>
class CRandomVelocity
{
public:
  float	m_fMinSpeed;
  float m_fMaxSpeed;
public:
  /// Velocity vector.
  Phoenix::Math::CVector3<float> m_vVelocity ;
  /// Default constructor.
  CRandomVelocity() : m_fMinSpeed(0.0f), m_fMaxSpeed(0.0f), m_vVelocity(0,0,0){}

  /// Modifies given particle.
  /// \param p particle which values are to be modified.
  inline void operator()(PARTICLE_TYPE &p) const
  {
    float fVal1 = Phoenix::Math::GetRandom<float>(0.0f, 1.0f) < 0.5f ? -1.0f : 1.0f;
    float fVal2 = Phoenix::Math::GetRandom<float>(0.0f, 1.0f) < 0.5f ? -1.0f : 1.0f;
    float fVal3 = Phoenix::Math::GetRandom<float>(0.0f, 1.0f) < 0.5f ? -1.0f : 1.0f;
    //cerr << " values " << fVal1 << "," << fVal2 << "," << fVal3 << "," << endl;
    p.m_vVelocity[0] = Phoenix::Math::GetRandom<float>(m_fMinSpeed, m_fMaxSpeed) * ( fVal1 );
    p.m_vVelocity[1] = Phoenix::Math::GetRandom<float>(m_fMinSpeed, m_fMaxSpeed) * ( fVal2 ); 
    p.m_vVelocity[2] = Phoenix::Math::GetRandom<float>(m_fMinSpeed, m_fMaxSpeed) * ( fVal3 );
  } 
};
/////////////////////////////////////////////////////////////////
template <class PARTICLE_TYPE>
class CRandomSize
{
  
public:
  float m_fSize;
  /// Default constructor.
  CRandomSize() {}

  /// Modifies given particle.
  /// \param p particle which values are to be modified.
  inline void operator()(PARTICLE_TYPE &p) const
  {
    p.m_fSize = Phoenix::Math::GetRandom<float>(2.0f, 4.05f);
    //p.m_fSize = 10.5f;
  } 
};
/////////////////////////////////////////////////////////////////
template <class PARTICLE_TYPE>
class CConstantSize
{
public:

  float m_fSize;
  /// Default constructor.
  CConstantSize() : m_fSize(0.0f) { }

  /// Modifies given particle.
  /// \param p particle which values are to be modified.
  inline void operator()(PARTICLE_TYPE &p) const
  {
    p.m_fSize = m_fSize;
  } 
};
/////////////////////////////////////////////////////////////////
template <class PARTICLE_TYPE>
class CIncreaseSizePolicy : public CSecondPolicyBase
{
protected:

public:
  float m_fSize;
  /// Default constructor.
  CIncreaseSizePolicy() {}

  /// Modifies given particle.
  /// \param p particle which values are to be modified.
  inline void operator()(PARTICLE_TYPE &p) const
  {
    p.m_fSize += 3.25f * GetPassedTime();

  } 
};
/////////////////////////////////////////////////////////////////
template <class PARTICLE_TYPE>
class CEnergyPolicy : public CSecondPolicyBase
{
public:
  float m_fEnergyDecrease;
  /// Default constructor.
  CEnergyPolicy() {}
  ////////////////////
  /// Modifies given particle.
  /// \param p particle which values are to be modified.
  inline void operator()(PARTICLE_TYPE &p) const
  {
    p.m_fEnergy -= m_fEnergyDecrease*GetPassedTime();
  } 
};
/////////////////////////////////////////////////////////////////
template <class PARTICLE_TYPE>
class CPositionPolicy : public CSecondPolicyBase
{
public:
  /// Default constructor.
  CPositionPolicy() { }
  
  /// Modifies given particle.
  /// \param particle a particle which values are to be modified.
  inline void operator()(PARTICLE_TYPE &particle ) 
  {
    particle.m_vPosition += particle.m_vVelocity * GetPassedTime();
  }
}; // end of CPositionPolicy
/////////////////////////////////////////////////////////////////
template <class PARTICLE_TYPE>
class CPositionInitPolicy 
{
public:
  CVector3<float> m_vPosition;
  /// Default constructor.
  CPositionInitPolicy() { m_vPosition[0] = 0.0; m_vPosition[1] = 0.0; m_vPosition[2] = 0.0;}
  
  /// Modifies given particle.
  /// \param particle a particle which values are to be modified.
  inline void operator()(PARTICLE_TYPE &particle ) 
  {
    particle.m_vPosition = m_vPosition;
  }
}; // end of CPositionPolicy

/////////////////////////////////////////////////////////////////
typedef CParticleSystem<50,
			CCompletePolicy<CRotatingParticle, 
					CNullPolicy<CRotatingParticle>,
					CRandomSize<CRotatingParticle>,
					CRandomVelocity<CRotatingParticle>,
					CEnergyInitializer<CRotatingParticle>,
					CPositionInitPolicy<CRotatingParticle> >,
			CCompletePolicy<CRotatingParticle, 
					CNullPolicy<CRotatingParticle>,
					CIncreaseSizePolicy<CRotatingParticle>,
					CNullPolicy<CRotatingParticle>,
					CEnergyPolicy<CRotatingParticle>,
					CPositionPolicy<CRotatingParticle> >,
			CRotatingParticle> CExplosionFire;
typedef CParticleSystem<150,
			CCompletePolicy<CRotatingParticle, 
					CNullPolicy<CRotatingParticle>,
					CConstantSize<CRotatingParticle>,
					CRandomVelocity<CRotatingParticle>,
					CEnergyInitializer<CRotatingParticle>,
					CPositionInitPolicy<CRotatingParticle> >,
			CCompletePolicy<CRotatingParticle, 
					CNullPolicy<CRotatingParticle>,
					CNullPolicy<CRotatingParticle>,
					CNullPolicy<CRotatingParticle>,
					CEnergyPolicy<CRotatingParticle>,
					CPositionPolicy<CRotatingParticle> >,
			CRotatingParticle> CExplosionDebris;
/////////////////////////////////////////////////////////////////
/// 
class CGamePSObject : public Phoenix::Scene::CGameObject<OBJECT_TYPE>
{
private:
  CVertexDescriptor *m_pVertexDescriptor;
  CVertexDescriptor *m_pVertexDescriptorDebris;
  CVertexDescriptor *m_pShaderData;
  CVertexDescriptor *m_pShaderDataDebris;
  CIndexArray *m_pIndices;
  CIndexArray *m_pIndicesDebris;

  CExplosionFire     m_ExplosionFire;
  CExplosionDebris   m_ExplosionDebris;
  unsigned int	     m_nPassedTimeMS;
  unsigned int	     m_bSecondDone;
  CVector3<float>    m_vOrigPosition;
public:
  ////////////////////
  /// 
  CGamePSObject()  
  {
    SetType(O_TYPE_PS);
    unsigned int nNumIndices = m_ExplosionFire.GetMaxParticles()*4;

    m_pVertexDescriptor = new CVertexDescriptor(ELEMENT_TYPE_VERTEX_3F, nNumIndices);
    m_pShaderData = new CVertexDescriptor(ELEMENT_TYPE_ATTRIB_4F, nNumIndices);
    m_pIndices = new CIndexArray( PRIMITIVE_QUAD_LIST, nNumIndices);

    unsigned int nNumIndicesDebris  = m_ExplosionDebris.GetMaxParticles()*4;
    m_pVertexDescriptorDebris = new CVertexDescriptor(ELEMENT_TYPE_VERTEX_3F, nNumIndicesDebris);
    m_pShaderDataDebris = new CVertexDescriptor(ELEMENT_TYPE_ATTRIB_4F, nNumIndicesDebris);
    m_pIndicesDebris = new CIndexArray( PRIMITIVE_QUAD_LIST, nNumIndicesDebris);    

    // Set initial size 
    m_ExplosionFire.m_InitializePolicy.m_SizePolicy.m_fSize = 1.0f;
    m_ExplosionFire.m_ActionPolicy.m_EnergyPolicy.m_fEnergyDecrease = 0.15f;
    m_ExplosionFire.m_InitializePolicy.m_VelocityPolicy.m_fMinSpeed = 0.5f;
    m_ExplosionFire.m_InitializePolicy.m_VelocityPolicy.m_fMaxSpeed = 1.75f;
    
    m_ExplosionDebris.m_InitializePolicy.m_SizePolicy.m_fSize = 0.50f;
    m_ExplosionDebris.m_ActionPolicy.m_EnergyPolicy.m_fEnergyDecrease = 0.52f;
    m_ExplosionDebris.m_InitializePolicy.m_VelocityPolicy.m_fMinSpeed = 0.75f;
    m_ExplosionDebris.m_InitializePolicy.m_VelocityPolicy.m_fMaxSpeed = 7.15f;
    
    if ( m_pIndices->IsShortIndices())
    {
      for(unsigned short int i=0;i<nNumIndices;i++)
      {
	m_pIndices->GetPointer<unsigned short int>()[i]=i;
      }
    } 
    else 
    {
      for(size_t i=0;i<nNumIndices;i++)
      {
	m_pIndices->GetPointer<unsigned int>()[i]=i;
      }
    }
    if ( m_pIndicesDebris->IsShortIndices())
    {
      for(unsigned short int i=0;i<nNumIndicesDebris;i++)
      {
	m_pIndicesDebris->GetPointer<unsigned short int>()[i]=i;
      }
    } 
    else 
    {
      for(size_t i=0;i<nNumIndicesDebris;i++)
      {
	m_pIndicesDebris->GetPointer<unsigned int>()[i]=i;
      }
    }
    m_nPassedTimeMS = 0;
  }

  
  
  ////////////////////
  ///
  ~CGamePSObject( )
  {
    delete m_pVertexDescriptor;
    delete m_pShaderData;
    delete m_pIndices;
    delete m_pVertexDescriptorDebris;
    delete m_pShaderDataDebris;
    delete m_pIndicesDebris;
  }
  ////////////////////
  ///
  void UpdateRenderableData()
  {
    size_t nIndex = 0;
    size_t nShaderDataIndex = 0;
    for(size_t i=0;i<m_ExplosionFire.GetAliveCount();i++)
    {
      // number of coords for one quad 4*3
      nIndex = i*12;
      // number of data slots for four quads 4*4
      nShaderDataIndex = i*16;
      
      m_pVertexDescriptor->GetPointer<float>()[nIndex]   = m_ExplosionFire.GetParticles()[i].m_vPosition[0];
      m_pVertexDescriptor->GetPointer<float>()[nIndex+1] = m_ExplosionFire.GetParticles()[i].m_vPosition[1];
      m_pVertexDescriptor->GetPointer<float>()[nIndex+2] = m_ExplosionFire.GetParticles()[i].m_vPosition[2];
      m_pShaderData->GetPointer<float>()[nShaderDataIndex]    = -1;
      m_pShaderData->GetPointer<float>()[nShaderDataIndex+1]  = 1;
      m_pShaderData->GetPointer<float>()[nShaderDataIndex+2]  = m_ExplosionFire.GetParticles()[i].m_fEnergy; 
      m_pShaderData->GetPointer<float>()[nShaderDataIndex+3]  = m_ExplosionFire.GetParticles()[i].m_fSize;

      m_pVertexDescriptor->GetPointer<float>()[nIndex+3] = m_ExplosionFire.GetParticles()[i].m_vPosition[0];
      m_pVertexDescriptor->GetPointer<float>()[nIndex+4] = m_ExplosionFire.GetParticles()[i].m_vPosition[1];
      m_pVertexDescriptor->GetPointer<float>()[nIndex+5] = m_ExplosionFire.GetParticles()[i].m_vPosition[2];
      m_pShaderData->GetPointer<float>()[nShaderDataIndex+4]  = -1;
      m_pShaderData->GetPointer<float>()[nShaderDataIndex+5]  = -1;
      m_pShaderData->GetPointer<float>()[nShaderDataIndex+6]  = m_ExplosionFire.GetParticles()[i].m_fEnergy; 
      m_pShaderData->GetPointer<float>()[nShaderDataIndex+7]  = m_ExplosionFire.GetParticles()[i].m_fSize;

      m_pVertexDescriptor->GetPointer<float>()[nIndex+6] = m_ExplosionFire.GetParticles()[i].m_vPosition[0];
      m_pVertexDescriptor->GetPointer<float>()[nIndex+7] = m_ExplosionFire.GetParticles()[i].m_vPosition[1];
      m_pVertexDescriptor->GetPointer<float>()[nIndex+8] = m_ExplosionFire.GetParticles()[i].m_vPosition[2];
      m_pShaderData->GetPointer<float>()[nShaderDataIndex+8]   =  1;
      m_pShaderData->GetPointer<float>()[nShaderDataIndex+9]   = -1;
      m_pShaderData->GetPointer<float>()[nShaderDataIndex+10]  = m_ExplosionFire.GetParticles()[i].m_fEnergy; 
      m_pShaderData->GetPointer<float>()[nShaderDataIndex+11]  = m_ExplosionFire.GetParticles()[i].m_fSize;

      m_pVertexDescriptor->GetPointer<float>()[nIndex+9]  = m_ExplosionFire.GetParticles()[i].m_vPosition[0];
      m_pVertexDescriptor->GetPointer<float>()[nIndex+10] = m_ExplosionFire.GetParticles()[i].m_vPosition[1];
      m_pVertexDescriptor->GetPointer<float>()[nIndex+11] = m_ExplosionFire.GetParticles()[i].m_vPosition[2];
      m_pShaderData->GetPointer<float>()[nShaderDataIndex+12]  = 1;
      m_pShaderData->GetPointer<float>()[nShaderDataIndex+13]  = 1;
      m_pShaderData->GetPointer<float>()[nShaderDataIndex+14]  = m_ExplosionFire.GetParticles()[i].m_fEnergy; 
      m_pShaderData->GetPointer<float>()[nShaderDataIndex+15]  = m_ExplosionFire.GetParticles()[i].m_fSize;

    }
    m_pIndices->SetDrawableCount( m_ExplosionFire.GetAliveCount()*4 );
    /////////////////////////////////////////////////////////////////
    // debris
    /////////////////////////////////////////////////////////////////
    nIndex = 0;
    nShaderDataIndex = 0;
    for(size_t i=0;i<m_ExplosionDebris.GetAliveCount();i++)
    {
      // number of coords for one quad 4*3
      nIndex = i*12;
      // number of data slots for four quads 4*4
      nShaderDataIndex = i*16;
      
      m_pVertexDescriptorDebris->GetPointer<float>()[nIndex]   = m_ExplosionDebris.GetParticles()[i].m_vPosition[0];
      m_pVertexDescriptorDebris->GetPointer<float>()[nIndex+1] = m_ExplosionDebris.GetParticles()[i].m_vPosition[1];
      m_pVertexDescriptorDebris->GetPointer<float>()[nIndex+2] = m_ExplosionDebris.GetParticles()[i].m_vPosition[2];
      m_pShaderDataDebris->GetPointer<float>()[nShaderDataIndex]    = -1;
      m_pShaderDataDebris->GetPointer<float>()[nShaderDataIndex+1]  = 1;
      m_pShaderDataDebris->GetPointer<float>()[nShaderDataIndex+2]  = m_ExplosionDebris.GetParticles()[i].m_fEnergy; 
      m_pShaderDataDebris->GetPointer<float>()[nShaderDataIndex+3]  = m_ExplosionDebris.GetParticles()[i].m_fSize;

      m_pVertexDescriptorDebris->GetPointer<float>()[nIndex+3] = m_ExplosionDebris.GetParticles()[i].m_vPosition[0];
      m_pVertexDescriptorDebris->GetPointer<float>()[nIndex+4] = m_ExplosionDebris.GetParticles()[i].m_vPosition[1];
      m_pVertexDescriptorDebris->GetPointer<float>()[nIndex+5] = m_ExplosionDebris.GetParticles()[i].m_vPosition[2];
      m_pShaderDataDebris->GetPointer<float>()[nShaderDataIndex+4]  = -1;
      m_pShaderDataDebris->GetPointer<float>()[nShaderDataIndex+5]  = -1;
      m_pShaderDataDebris->GetPointer<float>()[nShaderDataIndex+6]  = m_ExplosionDebris.GetParticles()[i].m_fEnergy; 
      m_pShaderDataDebris->GetPointer<float>()[nShaderDataIndex+7]  = m_ExplosionDebris.GetParticles()[i].m_fSize;;

      m_pVertexDescriptorDebris->GetPointer<float>()[nIndex+6] = m_ExplosionDebris.GetParticles()[i].m_vPosition[0];
      m_pVertexDescriptorDebris->GetPointer<float>()[nIndex+7] = m_ExplosionDebris.GetParticles()[i].m_vPosition[1];
      m_pVertexDescriptorDebris->GetPointer<float>()[nIndex+8] = m_ExplosionDebris.GetParticles()[i].m_vPosition[2];
      m_pShaderDataDebris->GetPointer<float>()[nShaderDataIndex+8]   =  1;
      m_pShaderDataDebris->GetPointer<float>()[nShaderDataIndex+9]   = -1;
      m_pShaderDataDebris->GetPointer<float>()[nShaderDataIndex+10]  = m_ExplosionDebris.GetParticles()[i].m_fEnergy; 
      m_pShaderDataDebris->GetPointer<float>()[nShaderDataIndex+11]  = m_ExplosionDebris.GetParticles()[i].m_fSize;;

      m_pVertexDescriptorDebris->GetPointer<float>()[nIndex+9]  = m_ExplosionDebris.GetParticles()[i].m_vPosition[0];
      m_pVertexDescriptorDebris->GetPointer<float>()[nIndex+10] = m_ExplosionDebris.GetParticles()[i].m_vPosition[1];
      m_pVertexDescriptorDebris->GetPointer<float>()[nIndex+11] = m_ExplosionDebris.GetParticles()[i].m_vPosition[2];
      m_pShaderDataDebris->GetPointer<float>()[nShaderDataIndex+12]  = 1;
      m_pShaderDataDebris->GetPointer<float>()[nShaderDataIndex+13]  = 1;
      m_pShaderDataDebris->GetPointer<float>()[nShaderDataIndex+14]  = m_ExplosionDebris.GetParticles()[i].m_fEnergy; 
      m_pShaderDataDebris->GetPointer<float>()[nShaderDataIndex+15]  = m_ExplosionDebris.GetParticles()[i].m_fSize;
      
    }
    m_pIndicesDebris->SetDrawableCount( m_ExplosionDebris.GetAliveCount()*4 );
    
  }
  void Update( unsigned int nMS )
  {
    m_ExplosionFire.Update(nMS);
    m_ExplosionDebris.Update(nMS);
    m_nPassedTimeMS+=nMS;
    if ( m_ExplosionFire.IsAlive() && !m_bSecondDone && m_nPassedTimeMS > 750 )
    {
      m_ExplosionDebris.Init(25, m_vOrigPosition);
      m_bSecondDone = 1;
    }
      
  }
  CVertexDescriptor * GetVertices() { return m_pVertexDescriptor; }
  CIndexArray * GetIndices() { return m_pIndices; }
  CVertexDescriptor * GetShaderData() { return m_pShaderData; }

  CVertexDescriptor * GetVerticesDebris() { return m_pVertexDescriptorDebris; }
  CIndexArray * GetIndicesDebris() { return m_pIndicesDebris; }
  CVertexDescriptor * GetShaderDataDebris() { return m_pShaderDataDebris; }
  
  void Init( const CVector3<float> &vPosition )
  {
    m_nPassedTimeMS = 0;
    m_ExplosionFire.m_InitializePolicy.m_PositionPolicy.m_vPosition = vPosition;
    m_ExplosionDebris.m_InitializePolicy.m_PositionPolicy.m_vPosition = vPosition;
    m_ExplosionFire.Init(5, vPosition );
    m_ExplosionDebris.Init(65, vPosition );
    m_bSecondDone = 0;
    m_vOrigPosition = vPosition;
  }
};
/////////////////////////////////////////////////////////////////
class CGameLODObject : public Phoenix::Scene::CGameObject<OBJECT_TYPE>
{
private:
  CHandle<CModel> m_aLodHandle[5];
public:
  ////////////////////
  CGameLODObject()
  { 
    SetType(O_TYPE_LOD); 
  }
  ////////////////////
  inline CHandle<CModel> & GetModelHandle( unsigned int nLevel ) 
  {
    return m_aLodHandle[nLevel];
  }
  ////////////////////
  inline const CHandle<CModel> & GetModelHandle( unsigned int nLevel )  const
  {
    return m_aLodHandle[nLevel];
  }
};
/////////////////////////////////////////////////////////////////
int g_bLoop = 1;
class CSpatialGraph : public COctree<Phoenix::Scene::CGameObject<OBJECT_TYPE> *>
{
public:
  ////////////////////
  /// 
  CSpatialGraph() : COctree<Phoenix::Scene::CGameObject<OBJECT_TYPE> *>(4,1200){}
  ////////////////////
  /// 
  void InsertObject( CGameObject<OBJECT_TYPE> *pGameObject )
  {
    assert(pGameObject!=NULL);
    unsigned int nSpatialIndex = COctree<Phoenix::Scene::CGameObject<OBJECT_TYPE> *>::InsertObject( 
				  pGameObject, 
				  pGameObject->GetBoundingSphere().GetPosition() + pGameObject->GetWorldTransform().GetTranslation(),
				  pGameObject->GetBoundingSphere().GetRadius());
    cerr << "object  " << pGameObject << endl;
    pGameObject->SetSpatialIndex( nSpatialIndex );
    cerr << "object inserted into " << nSpatialIndex << endl;
    cerr << "object near " << m_pAllNodes[nSpatialIndex].GetPosition() << endl;
  }
  ////////////////////
  /// 
  void DeleteObject( CGameObject<OBJECT_TYPE> *pGameObject )
  {
    COctreeNode<Phoenix::Scene::CGameObject<OBJECT_TYPE> *> *pNode = COctree<Phoenix::Scene::CGameObject<OBJECT_TYPE> *>::GetNodeAt( pGameObject->GetSpatialIndex());
    if ( pNode != NULL )
    {
      pNode->DeleteObject( pGameObject );
    }
  }
  ////////////////////
  /// 
  void Update( Phoenix::Scene::CGameObject<OBJECT_TYPE> *pGameObject )
  {
    CVector3<float> vTmp = pGameObject->GetWorldTransform().GetTranslation();
    vTmp += pGameObject->GetBoundingSphere().GetPosition();

    unsigned int nLevel = COctree<Phoenix::Scene::CGameObject<OBJECT_TYPE> *>::GetObjectDepth( pGameObject->GetBoundingSphere().GetRadius() );
    unsigned int nIndex = COctree<Phoenix::Scene::CGameObject<OBJECT_TYPE> *>::GetIndex1D(nLevel, vTmp[0], vTmp[1], vTmp[2]);

    // If object has changed spatial location enough
    if ( pGameObject->GetSpatialIndex() != nIndex)
    {
      // Remove from  previous location
      GetNodeAt( pGameObject->GetSpatialIndex())->DeleteObject( pGameObject );
      // Insert into new
      GetNodeAt( nIndex )->AddObject( pGameObject );
      // Update child statuses up to root.
      PropagateChildrenStatus( GetNodeAt( nIndex ));
      // Store new spatial index.
      pGameObject->SetSpatialIndex( nIndex );

    }
  }
};
/////////////////////////////////////////////////////////////////
#define LASER_TEXTURE "EnergyBeam_LaserBeam"
/// Laser beam class.
class CLaserBeam : public CEnergyBeam 
{
private:
  
public:
  ////////////////////
  /// Constructor.
  CLaserBeam()
  {
    g_DefaultTextureManager->AttachHandle(LASER_TEXTURE, GetTextureHandle());
    AddTextureFilter( MIN_NEAREST );
    AddTextureFilter( MAG_LINEAR );
    AddTextureFilter( S_WRAP_REPEAT );
  }
  ////////////////////
  /// Creates necessary resources for this class.
  static void CreateResources( Phoenix::Graphics::COglRenderer &renderer )
  {
    COglTexture *pBeamTexture = renderer.CreateTexture( "Resources/Textures/beam2.tga");
    assert( g_DefaultTextureManager->Create( pBeamTexture, LASER_TEXTURE) == 0);
    CShader *pShader = g_DefaultShaderManager->GetResource("EnergyBeamShader");
    if ( pShader == NULL )
    {
      pShader = renderer.CreateShader( std::string("Resources/Shaders/line_vertex.glsl"), 
				       std::string("Resources/Shaders/line_frag.glsl") );
      assert( pShader != NULL ) ;
      assert(g_DefaultShaderManager->Create( pShader, "EnergyBeamShader") == 0 );
    }
  }
};
/////////////////////////////////////////////////////////////////
#define RESOURCE_ENERGYBOLT "__phoenix_energybolt"
#define RESOURCE_ENERGYBOLT_TEXTURE "__phoenix_energybolt_texture"
#define ENERGYBOLT_TEXTURE "Resources/Textures/energybolt.tga"
class CEnergyBolt 
{
private:
  CHandle<CModel> m_hModel;
public:
  ////////////////////
  /// Constructor.
  CEnergyBolt()
  {
    g_PhoenixModelManager->AttachHandle( RESOURCE_ENERGYBOLT, m_hModel);
  }
  ////////////////////
  /// Creates resources.
  static void CreateResources( COglRenderer & renderer )
  {
    CModel *pEnergyBolt = new CModel();
    assert( LoadMilkshapeModel("Resources/Models/energybolt.ms3d", RESOURCE_ENERGYBOLT, *pEnergyBolt, OPT_VERTEX_TEXCOORDS | OPT_VERTEX_INDICES ) == 0);
    g_PhoenixModelManager->Create( pEnergyBolt, RESOURCE_ENERGYBOLT);
    
    COglTexture *pTexture = renderer.CreateTexture(ENERGYBOLT_TEXTURE);
    assert( pTexture != NULL );
    g_DefaultTextureManager->Create(  pTexture, RESOURCE_ENERGYBOLT_TEXTURE, pEnergyBolt->GetTextureHandle() );

    pEnergyBolt->AddTextureFilter( MIN_MIP_LINEAR );
    pEnergyBolt->AddTextureFilter( MAG_LINEAR );
  }
  ////////////////////
  /// 
  CHandle<CModel> & GetModelHandle()
  {
    return m_hModel;
  }

};
/////////////////////////////////////////////////////////////////
enum SPACESHIP_CLASS
{
  SHIP_CLASS_SOVEREIGN = 0,
  SHIP_CLASS_OMEGA = 1
};
/////////////////////////////////////////////////////////////////
/// Baseclass for every spaceship in the system.
class CSpaceShip : public CGameObject<OBJECT_TYPE>,
		   public COrientable
{
protected:
  /// Default Bounding sphere for each ship.
  static Phoenix::Volume::CSphere	m_Sphere;
  /// Default bounding box for each ship
  static Phoenix::Volume::COrientedBox  m_Box;
  SPACESHIP_CLASS			m_nShipClass;
  float					m_fSpeed;
  float					m_fMaxSpeed;
  float					m_fHealth;
  float					m_fAcceleration;
public:

  ////////////////////
  /// Constructor.
  CSpaceShip( size_t nNumModels = 1) : CGameObject<OBJECT_TYPE>(nNumModels), m_fSpeed(0.0f), m_fMaxSpeed(10.0f), m_fHealth(1.0f), m_fAcceleration(0.0f)
  {
    SetType(O_TYPE_SPACESHIP);
  }
  ////////////////////
  /// Destructor.
  ~CSpaceShip() 
  {
    // Release model handle
    g_PhoenixModelManager->Release( GetModelHandle() );
  }
  ////////////////////
  /// Get Type
  SPACESHIP_CLASS GetShipClass() const
  {
    return m_nShipClass;
  }
  ////////////////////
  /// Set type.
  void SetShipClass( SPACESHIP_CLASS shipClass )
  {
    m_nShipClass = shipClass;
  }
  ////////////////////
  /// Returns current speed.
  float & GetSpeed() 
  {
    return m_fSpeed;
  }
  ////////////////////
  /// Returns maximum speed.
  const float & GetMaxSpeed() const
  {
    return m_fMaxSpeed;
  }
  ////////////////////
  /// Returns current health
  const float & GetHealth() const
  {
    return m_fHealth;
  }
  ////////////////////
  /// Decreases health by damage.
  /// \param fDamage Inflicted damage.
  void DecreaseHealth( float fDamage )
  {
    m_fHealth -= fDamage;
    if ( m_fHealth < 0.0f ) m_fHealth = 0.0f;
  }
  ////////////////////
  /// Increases ship health by given amount.
  /// \param fHealing Health increment.
  void IncreaseHealth( float fHealing )
  {
    m_fHealth += fHealing;
  }
  ////////////////////
  /// Returns acceleration 
  /// \returns Acceleration factor.
  const float & GetAcceleration() const
  {
    return m_fAcceleration;
  }
  ////////////////////
  /// Assigns acceleration factor.
  /// \param fValue New acceleration factor.
  void SetAcceleration( const float & fValue ) 
  {
    m_fAcceleration = fValue;
  }
  ////////////////////
  /// Checks is this ship alive
  int IsAlive() const
  {
    return (GetHealth() > 0.0f);
  }
};
/// Static members must be initialized.
CSphere CSpaceShip::m_Sphere = CSphere();
COrientedBox CSpaceShip::m_Box = COrientedBox();
/////////////////////////////////////////////////////////////////
#define SOVEREIGN_MODEL "Resources/Models/1701-e.ms3d"
#define SOVEREIGN_VERTICES "sovereign_vertices"
#define SOVEREIGN_TEXCOORDS "sovereign_texcoords"
#define SOVEREIGN_TEXTURE "sovereign_texture"
#define SOVEREIGN_NORMALS "sovereign_normals"
#define SOVEREIGN_INDICES "sovereign_indices"
#define SOVEREIGN_RESOURCE "sovereign_model"
/////////////////////////////////////////////////////////////////
/// Class for Sovereign class ships (E-class enterprise)
class CSovereignClass : public CSpaceShip
{
protected:
  static Tcl_Interp *pInterp;
  static CSovereignClass *m_pCurrentObject;
  static float m_fPassedTime;
public:
  ////////////////////
  /// Constructor.
  CSovereignClass()
  {
    m_fMaxSpeed = 15.0f;
    g_PhoenixModelManager->AttachHandle( SOVEREIGN_RESOURCE, GetModelHandle() );
    GetBoundingSphere() = CSovereignClass::m_Sphere;
    GetBoundingBox()	= CSovereignClass::m_Box;
    SetShipClass( SHIP_CLASS_SOVEREIGN );
  }
  ////////////////////
  void Update( unsigned int nTimeMS )
  {
    m_fPassedTime = ((float)nTimeMS * 0.001f);
    if ( pInterp )
    {
      m_pCurrentObject = this;

      Tcl_SetVar2Ex( pInterp, "g_fHealth",	NULL,	Tcl_NewDoubleObj(m_pCurrentObject->GetHealth()),	0);
      Tcl_SetVar2Ex( pInterp, "g_fAccel",	NULL,	Tcl_NewDoubleObj(m_pCurrentObject->GetAcceleration()),	0);
      Tcl_SetVar2Ex( pInterp, "g_fPassedTime",	NULL,	Tcl_NewDoubleObj(m_fPassedTime),			0);
      Tcl_SetVar2Ex( pInterp, "g_fSpeed",	NULL,	Tcl_NewDoubleObj(m_pCurrentObject->GetSpeed()),		0);
      Tcl_SetVar2Ex( pInterp, "g_fMaxSpeed",	NULL,	Tcl_NewDoubleObj(m_pCurrentObject->GetMaxSpeed()),	0);
      
      if ( Tcl_Eval(pInterp, "playComputer") != TCL_OK )
      {
	cerr << "Error in script: " << Tcl_GetStringResult(pInterp) << endl;
      }
    }
  }
  ////////////////////
  /// Allocates proper resources for sovereign class ships.
  /// \param rOglRenderer Renderer for creating cache.
  static void CreateResources( COglRenderer & rOglRenderer)
  {
    Phoenix::Data::CMilkshapeLoader loader;
    std::string name("Resources/Models/1701-e.ms3d");
    //std::string name("Resources/Models/triangle.ms3d");
    assert ( loader.Load( name ) == 0 && "Could not open model file!");
    loader.GenerateModelData();
    //loader.Stripify();
    
    CModel *pSovereignModel = new CModel();

    assert(g_DefaultVertexManager->Create( loader.GetVertices(),  SOVEREIGN_VERTICES,  pSovereignModel->GetVertexHandle() )            == 0);  
    assert(g_DefaultVertexManager->Create( loader.GetTexCoords(), SOVEREIGN_TEXCOORDS, pSovereignModel->GetTextureCoordinateHandle())  == 0);
    assert(g_DefaultVertexManager->Create( loader.GetNormals(),   SOVEREIGN_NORMALS,   pSovereignModel->GetNormalHandle() )            == 0);

    // Allow only one set of indices.
    assert( loader.GetIndices().size() == 1 );
    // add new index handle to model
    pSovereignModel->AddIndexHandle( INDEX_HANDLE() );
    // Create resource for these indices
    assert( g_DefaultIndexManager->Create( loader.GetIndices()[0], SOVEREIGN_INDICES, pSovereignModel->GetIndexHandles().back()) == 0);

    
    
    // Create VBO cache for indices.
    rOglRenderer.CommitCache(*loader.GetIndices()[0]);

    COglTexture *pTexture = rOglRenderer.CreateTexture("Resources/Textures/sovereign.tga");
    assert(g_DefaultTextureManager->Create( pTexture, SOVEREIGN_TEXTURE, pSovereignModel->GetTextureHandle()) == 0);
    assert( g_PhoenixModelManager->Create( pSovereignModel, SOVEREIGN_RESOURCE ) == 0);
    
    // Calculate bounding sphere
    CSovereignClass::m_Sphere = CalculateBoundingSphereTight( *(loader.GetVertices()));
    CSovereignClass::m_Box = CalculateOrientedBoundingBox( *(loader.GetVertices()));
    // Tell loader not to free these resources, they are managed.
    loader.ResetVertices();
    loader.ResetTexCoords();
    loader.ResetNormals();
    loader.ResetIndices();

    pSovereignModel->AddTextureFilter(MIN_MIP_LINEAR);
    pSovereignModel->AddTextureFilter(MAG_LINEAR);
    
    pInterp = Tcl_CreateInterp();
    assert( pInterp != NULL );

    assert(Tcl_CreateObjCommand( pInterp, "Rotate",
				 Rotate, (ClientData)0,
				 NULL) != NULL );
    assert(Tcl_CreateObjCommand( pInterp, "Accelerate",
				 Accelerate, (ClientData)0,
				 NULL) != NULL );
    assert(Tcl_CreateObjCommand( pInterp, "Move",
				 Move, (ClientData)0,
				 NULL) != NULL );
    
    int status = Tcl_EvalFile(pInterp, "Resources/Scripts/PlayComputer.tcl");
    assert( status == TCL_OK );
  }  
  ////////////////////
  /// Releases resources held by this class.
  /// \param rOglRenderer Renderer object for removing caches.
  static void ReleaseResources( COglRenderer & rOglRenderer )
  {
    // Release cache
    CVertexDescriptor *pTemp	= g_DefaultVertexManager->GetResource( SOVEREIGN_VERTICES );
    CIndexArray *pIndices	= g_DefaultIndexManager->GetResource( SOVEREIGN_INDICES );
    if ( pTemp )
      rOglRenderer.RollbackCache( *pTemp );    

    if ( pIndices )
      rOglRenderer.RollbackCache( *pIndices );    
    
    // release model itself
    g_PhoenixModelManager->Destroy( SOVEREIGN_RESOURCE );
    
    if ( pInterp )
      Tcl_DeleteInterp( pInterp );
  }
  /////////////////////////////////////////////////////////////////
  static int Accelerate( ClientData clientData, Tcl_Interp *pInterp, int objc, Tcl_Obj * CONST objv[])
  {
    if ( m_pCurrentObject != NULL )
    {
      double dAccel;
      Tcl_GetDoubleFromObj( pInterp, objv[1], &dAccel );

      m_pCurrentObject->SetAcceleration( dAccel );
    }
    Tcl_ResetResult( pInterp );
    return TCL_OK;
  }
  /////////////////////////////////////////////////////////////////
  static int Move( ClientData clientData, Tcl_Interp *pInterp, int objc, Tcl_Obj * CONST objv[] )
  {
    if ( m_pCurrentObject != NULL )
    {
      // Handle acceleration
      m_pCurrentObject->GetSpeed() =  m_pCurrentObject->GetAcceleration()* m_fPassedTime;
      // Move ship forward
      m_pCurrentObject->GetLocalTransform().Move( m_pCurrentObject->GetForwardVector() * 
						  m_pCurrentObject->GetSpeed() * 
						  m_fPassedTime);
    }
    Tcl_ResetResult( pInterp );
    return TCL_OK;
  }
  /////////////////////////////////////////////////////////////////
  static int Rotate( ClientData clientData, Tcl_Interp *pInterp, int objc, Tcl_Obj * CONST objv[])
  {
    if ( m_pCurrentObject != NULL )
    {
      double dX, dY, dZ;
      
      Tcl_GetDoubleFromObj( pInterp, objv[1], &dX );
      Tcl_GetDoubleFromObj( pInterp, objv[2], &dY );
      Tcl_GetDoubleFromObj( pInterp, objv[3], &dZ );
      CQuaternion qRot[4];

      qRot[0].CreateFromAxisAngle( m_pCurrentObject->GetRightVector(), dX * m_fPassedTime);
      qRot[1].CreateFromAxisAngle( m_pCurrentObject->GetUpVector(), dY * m_fPassedTime);
      qRot[2].CreateFromAxisAngle( m_pCurrentObject->GetForwardVector(), dZ * m_fPassedTime);
      qRot[3] = qRot[2] * qRot[1] * qRot[0];
      

      // Append rotation to direction vectors and transform
      m_pCurrentObject->GetLocalTransform().Rotate( qRot[3] );
      //m_pCurrentObject->GetLocalTransform().SetRotation( qRot[3] );
      m_pCurrentObject->AppendToRotation( qRot[3] );
      //m_pCurrentObject->SetRotation( qRot[3] );

    }
    Tcl_ResetResult( pInterp );
    return TCL_OK;
  }
};
CSovereignClass *CSovereignClass::m_pCurrentObject = NULL;
float CSovereignClass::m_fPassedTime = 0.0f;
Tcl_Interp * CSovereignClass::pInterp = NULL;
/////////////////////////////////////////////////////////////////
#define OMEGA_MODEL_HULL "Resources/Models/omega_hull.ms3d"
#define OMEGA_MODEL_ROT "Resources/Models/omega_rotator.ms3d"
#define OMEGA_VERTICES "omega_vertices"
#define OMEGA_TEXCOORDS "omega_texcoords"
#define OMEGA_TEXTURE "omega_texture"
#define OMEGA_NORMALS "omega_normals"
#define OMEGA_INDICES "omega_indices"

#define OMEGA_ROT_VERTICES "omega_rot_vertices"
#define OMEGA_ROT_TEXCOORDS "omega_rot_texcoords"
#define OMEGA_ROT_TEXTURE "omega_rot_texture"
#define OMEGA_ROT_NORMALS "omega_rot_normals"
#define OMEGA_ROT_INDICES "omega_rot_indices"

#define OMEGA_RESOURCE_HULL "omega_hullmodel"
#define OMEGA_RESOURCE_ROT "omega_rotmodel"
/////////////////////////////////////////////////////////////////
class COmegaClass : public CSpaceShip
{
private:
  CTransform m_RotatorTransform;
  float	     m_fAngleSpeed;
public:
  ////////////////////
  /// Constructor.
  COmegaClass() : CSpaceShip(2) 
  {
    g_PhoenixModelManager->AttachHandle( OMEGA_RESOURCE_HULL, GetModelHandle(0) );
    g_PhoenixModelManager->AttachHandle( OMEGA_RESOURCE_ROT,  GetModelHandle(1) );
    GetBoundingSphere() = m_Sphere;
    SetShipClass( SHIP_CLASS_OMEGA );
    m_RotatorTransform.SetRotation( 0, 0, 56.0f);
    m_fAngleSpeed = -10.0f;
  }
  ////////////////////
  /// Allocates proper resources for omega class ships.
  /// \param rOglRenderer Renderer for creating cache.
  static void CreateResources( COglRenderer & rOglRenderer)
  {
    CModel *pOmegaModel = new CModel();
    CModel *pOmegaRot   = new CModel();

    assert( LoadMilkshapeModel( OMEGA_MODEL_HULL, OMEGA_RESOURCE_HULL, *pOmegaModel, OPT_VERTEX_NORMALS | OPT_VERTEX_TEXCOORDS | OPT_VERTEX_INDICES  ) == 0);
    assert( LoadMilkshapeModel( OMEGA_MODEL_ROT , OMEGA_RESOURCE_ROT, *pOmegaRot,   OPT_VERTEX_NORMALS | OPT_VERTEX_TEXCOORDS | OPT_VERTEX_INDICES  ) == 0);    

    rOglRenderer.CommitCache( *g_DefaultVertexManager->GetResource(pOmegaModel->GetVertexHandle()));
    rOglRenderer.CommitCache( *g_DefaultVertexManager->GetResource(pOmegaModel->GetNormalHandle()));
    rOglRenderer.CommitCache( *g_DefaultVertexManager->GetResource(pOmegaModel->GetTextureCoordinateHandle()));
    rOglRenderer.CommitCache( *g_DefaultIndexManager->GetResource(pOmegaModel->GetIndexHandles().back()));
    
    rOglRenderer.CommitCache( *g_DefaultVertexManager->GetResource(pOmegaRot->GetVertexHandle()));
    rOglRenderer.CommitCache( *g_DefaultVertexManager->GetResource(pOmegaRot->GetNormalHandle()));
    rOglRenderer.CommitCache( *g_DefaultVertexManager->GetResource(pOmegaRot->GetTextureCoordinateHandle()));
    rOglRenderer.CommitCache( *g_DefaultIndexManager->GetResource(pOmegaRot->GetIndexHandles().back()));
    
    COglTexture *pTexture = rOglRenderer.CreateTexture( "Resources/Textures/omega.tga");

    assert( g_DefaultTextureManager->Create( pTexture,  OMEGA_TEXTURE, pOmegaModel->GetTextureHandle()) == 0);
    assert( g_PhoenixModelManager->Create( pOmegaModel, OMEGA_RESOURCE_HULL ) == 0);
    assert( g_PhoenixModelManager->Create( pOmegaRot,   OMEGA_RESOURCE_ROT  ) == 0);

    // Calculate bounding sphere
    m_Sphere = CalculateBoundingSphereTight( *(g_DefaultVertexManager->GetResource(pOmegaModel->GetVertexHandle())));
        
    pOmegaModel->AddTextureFilter(MIN_MIP_LINEAR);
    pOmegaModel->AddTextureFilter(MAG_LINEAR);
    pOmegaRot->AddTextureFilter(MIN_MIP_LINEAR);
    pOmegaRot->AddTextureFilter(MAG_LINEAR);

  }  
  ////////////////////
  /// Releases resources held by this class.
  /// \param rOglRenderer Renderer object for removing caches.
  static void ReleaseResources( COglRenderer & rOglRenderer )
  {
    // Release cache
    CVertexDescriptor *pTemp    = g_DefaultVertexManager->GetResource( OMEGA_VERTICES  );
    CVertexDescriptor *pNormals = g_DefaultVertexManager->GetResource( OMEGA_NORMALS   );
    CVertexDescriptor *pTexc    = g_DefaultVertexManager->GetResource( OMEGA_TEXCOORDS );
    CIndexArray *pIndices       = g_DefaultIndexManager->GetResource(  OMEGA_INDICES   );

    if ( pTemp )
      rOglRenderer.RollbackCache( *pTemp );    

    if ( pNormals )
      rOglRenderer.RollbackCache( *pNormals );

    if ( pTexc )
      rOglRenderer.RollbackCache( *pTexc );

    if ( pIndices )
      rOglRenderer.RollbackCache( *pIndices );    

    pTemp    = g_DefaultVertexManager->GetResource( OMEGA_ROT_VERTICES  );
    pNormals = g_DefaultVertexManager->GetResource( OMEGA_ROT_NORMALS   );
    pTexc    = g_DefaultVertexManager->GetResource( OMEGA_ROT_TEXCOORDS );
    pIndices = g_DefaultIndexManager->GetResource(  OMEGA_ROT_INDICES   );

    if ( pTemp )
      rOglRenderer.RollbackCache( *pTemp );    

    if ( pNormals )
      rOglRenderer.RollbackCache( *pNormals );

    if ( pTexc )
      rOglRenderer.RollbackCache( *pTexc );

    if ( pIndices )
      rOglRenderer.RollbackCache( *pIndices );    
    
    // release models themselves
    g_PhoenixModelManager->Destroy( OMEGA_RESOURCE_HULL );
    g_PhoenixModelManager->Destroy( OMEGA_RESOURCE_ROT );
  }
  ////////////////////
  /// Returns rotator transform.
  /// \returns Rotator transform reference.
  inline CTransform & GetRotatorTransform() 
  {
    return m_RotatorTransform;
  }
  ////////////////////
  /// Animates ship.
  void Update( unsigned int nTimeMS )
  {
    float m_fPassedTime = ((float)nTimeMS * 0.001f);
    CQuaternion rotation;
    rotation.CreateFromAxisAngle( 0,0,1, m_fPassedTime * m_fAngleSpeed );
    m_RotatorTransform.Rotate( rotation );
  }
};
/////////////////////////////////////////////////////////////////
#define g_ShipManager (CResourceManager<CSpaceShip, CHandle<CSpaceShip> >::GetInstance())
typedef CHandle<CSpaceShip> SHIP_HANDLE;
/////////////////////////////////////////////////////////////////
/// A class which works as an update adapter for SpaceShip objects.
class CSpaceShipUpdaterAdapter 
{
protected:
  CSpatialGraph *m_pGraph;
public:
  CSpaceShipUpdaterAdapter( CSpatialGraph *pGraph )
  {
    m_pGraph = pGraph;
  }
  inline void Update( CSpaceShip *pShip, unsigned int nTimeMS )
  {
    if ( pShip == NULL ) return;

    switch( pShip->GetShipClass())
    {
    case SHIP_CLASS_SOVEREIGN:
      static_cast<CSovereignClass *>(pShip)->Update( nTimeMS );
      break;
    case SHIP_CLASS_OMEGA:
      static_cast<COmegaClass *>(pShip)->Update( nTimeMS );
      break;
    }
    m_pGraph->Update( pShip );
  }
};
/////////////////////////////////////////////////////////////////
class CGameObjectOGLAdapter 
{
protected:
  CCamera *m_pCamera;
public:
  ////////////////////
  /// 
  CGameObjectOGLAdapter() : m_pCamera(NULL)
  { 
    
  }
  ////////////////////
  ///
  void Commit( COglRenderer &renderer, const CGameObject<OBJECT_TYPE> *pObject)
  {
    //renderer.CommitColor( CVector4<unsigned char>(255,255,255,255));
    CVector3<float> vDiff;
    if ( GetCamera() != NULL ) 
    {
      vDiff = pObject->GetWorldTransform().GetTranslation() - GetCamera()->GetPosition();
    } 
    renderer.CommitTransform( pObject->GetWorldTransform() );
    switch ( pObject->GetType())
    {
    case O_TYPE_SPACESHIP:
      {
	renderer.CommitModel( *g_PhoenixModelManager->GetResource(pObject->GetModelHandle()));
	if ( static_cast<const CSpaceShip *>(pObject)->GetShipClass() == SHIP_CLASS_OMEGA )
	{
	  COmegaClass *pTmp = static_cast<const COmegaClass *>(pObject);
	  renderer.CommitTransform( pTmp->GetRotatorTransform() );
	  renderer.CommitModel( *g_PhoenixModelManager->GetResource(pTmp->GetModelHandle(1)));
	  renderer.RollbackTransform();
	}
      }
      break;
    case O_TYPE_ORDINARY:
    case O_TYPE_PS:
    case O_TYPE_LOD:
      cerr << "Whatta hell" << endl;
      break;
    }
    renderer.RollbackTransform();
    //m_bFirstTime = 0;
    //glFlush();
  }
  ////////////////////
  //
  inline void SetCamera( CCamera & rCamera )
  {
    m_pCamera = &rCamera;
  }
  ////////////////////
  ///
  inline CCamera * GetCamera() 
  {
    return m_pCamera;
  }
};
/////////////////////////////////////////////////////////////////
void 
DrawSurroundingQuad( COglRenderer * pOglRenderer, CCamera &camera, CSpaceShip *pObject )
{
  COrientedBox box = pObject->GetBoundingBox();
  box.AppendToRotation( pObject->GetWorldTransform().GetRotation());
  box.SetPosition( box.GetPosition() + pObject->GetWorldTransform().GetTranslation());
  box.CalculatePlanes();
  box.CalculateCorners();
  CVector3<float> vMin, vMax;
  CVector3<float> vTemp;
  vMin = camera.WorldCoordinatesToScreen( box.GetCorner( Phoenix::Volume::TOP_LEFT_BACK ));


  vMax = vMin;
  vMin[2] = vMax[2] = 0.0f;
  // Project coordinates to screen and get min/max.
  for( unsigned int i=1;i<8;i++)
  {
    vTemp = camera.WorldCoordinatesToScreen( box.GetCorner( (BBOX_CORNER_TYPE)i ));
    for(unsigned int n=0;n<2;n++)
    {
      if ( vTemp[n] > vMax[n] ) vMax[n] = vTemp[n];
      else if ( vTemp[n] < vMin[n] ) vMin[n] = vTemp[n];
    }
    
  }
  
  // Render quad.
  pOglRenderer->CommitColor( CVector4<unsigned char>(0,255,0,255));
  glPolygonMode( GL_FRONT, GL_LINE );
  glBegin(GL_QUADS);
    glVertex3fv(vMin.GetArray());
    glVertex3fv(CVector3<float>(vMax[0], vMin[1], 0.0f).GetArray()); 
    glVertex3fv(vMax.GetArray());
    glVertex3fv(CVector3<float>(vMin[0], vMax[1], 0.0f).GetArray());
  glEnd();

  //pOglRenderer->CommitColor( CVector4<unsigned char>(0,255,0,255));
  pOglRenderer->CommitColor( CVector4<unsigned char>(255,255,255,255));
  ////////////////////
  // Draw healthbar
  glPolygonMode( GL_FRONT, GL_LINE );
  glLineWidth(1.0f);
  
  pOglRenderer->DisableState( STATE_DEPTH_TEST );
  float fWidth = 23.0f;
  float fHeight = 4.0f;

  
  CVector3<float> vPos = camera.WorldCoordinatesToScreen(pObject->GetWorldTransform().GetTranslation());
  vPos[1] += (vMax[1] - vMin[1]) * 0.75f;

  glBegin(GL_QUADS);
    glVertex3f(vPos[0] - fWidth, vPos[1], vPos[2]);
    glVertex3f(vPos[0] + fWidth, vPos[1], vPos[2]);
    glVertex3f(vPos[0] + fWidth, vPos[1]+fHeight, vPos[2]);
    glVertex3f(vPos[0] - fWidth, vPos[1]+fHeight, vPos[2]);
  glEnd();
  glPolygonMode( GL_FRONT, GL_FILL );  
  if ( pObject->GetHealth() > 0.80f )
  {
    pOglRenderer->CommitColor( CVector4<unsigned char>(0,255,0, 255));
  }
  else if ( pObject->GetHealth() > 0.4f )
  {
    pOglRenderer->CommitColor( CVector4<unsigned char>(255,255,0,255));
  }
  else 
  {
    pOglRenderer->CommitColor( CVector4<unsigned char>(255,0,0,255));
  }
  glBegin(GL_QUADS);
    glVertex3f(vPos[0] - fWidth, vPos[1], vPos[2]);
    glVertex3f(vPos[0] - fWidth + ((fWidth+fWidth)*pObject->GetHealth()), vPos[1], vPos[2]);
    glVertex3f(vPos[0] - fWidth + ((fWidth+fWidth)*pObject->GetHealth()), vPos[1]+fHeight, vPos[2]);
    glVertex3f(vPos[0] - fWidth, vPos[1]+fHeight, vPos[2]);
  glEnd();

  glLineWidth(1.0f);
  glPolygonMode( GL_FRONT, GL_FILL );  
  pOglRenderer->CommitColor( CVector4<unsigned char>(255,255,255,255));
}
/////////////////////////////////////////////////////////////////
enum TRANSFORM_TYPE 
{
  TRANSFORM_SHIP = 0,
  TRANSFORM_EXT = 1
};
/////////////////////////////////////////////////////////////////
class CShipTransform : public CTransformNode<TRANSFORM_TYPE, CSpaceShip, CHandle<CSpaceShip> >
{
  friend class CGraph<TRANSFORM_TYPE>;
public:
  CShipTransform()
  {
    SetType( TRANSFORM_SHIP );
  }
};
/////////////////////////////////////////////////////////////////
class CExtTransform : public CTransformNode<TRANSFORM_TYPE, CSpaceShip, CHandle<void> >,
		      public CTransformable
		
{
  friend class CGraph<TRANSFORM_TYPE>;
public:
  CExtTransform()
  {
    SetType( TRANSFORM_EXT );
  }
};
/////////////////////////////////////////////////////////////////
class CTransformUpdater 
{
public:
  ////////////////////
  /// Handles entering to node while traversing
  int Enter( CGraphNode<TRANSFORM_TYPE> *pNode )
  {
    int bRetval;
    switch ( pNode->GetType() )
    {
    case TRANSFORM_EXT:
      bRetval = HandleExtTransform( static_cast<CExtTransform *>(pNode));
      break;
    case TRANSFORM_SHIP:
      bRetval = HandleShipTransform( static_cast<CShipTransform *>(pNode));
      break;
    }
    return bRetval;
    
  }
  ////////////////////  
  /// Handles exiting  node while traversing
  void Leave( CGraphNode<TRANSFORM_TYPE> *pNode )
  {
    //NOP
  }
protected:
  ////////////////////
  int HandleExtTransform( CExtTransform * pNode )
  {
    
    CTransform *pParentWorldTransform = NULL;
    CSpaceShip *pParentShip = NULL;
    if ( !pNode->GetArrivingEdges().empty())
    {
      
      switch ( pNode->GetArrivingEdges().front()->GetFromNode()->GetType() )
      {
      case TRANSFORM_EXT:
	pParentWorldTransform = 
	             &static_cast<CExtTransform *>( pNode->GetArrivingEdges().front()->GetFromNode())->GetWorldTransform();
	break;
      case TRANSFORM_SHIP:
	pParentShip = g_ShipManager->GetResource( 
		     static_cast<CShipTransform *>( pNode->GetArrivingEdges().front()->GetFromNode() )->GetHandle() );
	// If there is no ship, skip handling and use local transform.
	if ( pParentShip == NULL ) 
	{
	  pNode->SetWorldTransform( pNode->GetLocalTransform());
	  return 0;
	}
	pParentWorldTransform = &pParentShip->GetWorldTransform();
	break;
      }
      
      Multiply( *pParentWorldTransform, 
		pNode->GetLocalTransform(), 
		pNode->GetWorldTransform());
      
    }
    else 
    {
      pNode->SetWorldTransform( pNode->GetLocalTransform());
    }
    return 0;
  }
  ////////////////////
  int HandleShipTransform( CShipTransform *pNode )
  {
    
    CSpaceShip *pShip = g_ShipManager->GetResource(pNode->GetHandle());
    if ( pShip == NULL) 
    {
      //cerr << "ship is null" << endl;
      return 0;
    }
    
    CTransform *pParentWorldTransform = NULL;
    CSpaceShip *pParentShip = NULL;
    if ( !pNode->GetArrivingEdges().empty())
    {
      
      switch ( pNode->GetArrivingEdges().front()->GetFromNode()->GetType() )
      {
      case TRANSFORM_EXT:
	pParentWorldTransform = 
	      &static_cast<CExtTransform *>( pNode->GetArrivingEdges().front()->GetFromNode())->GetWorldTransform();
	break;
      case TRANSFORM_SHIP:
	pParentShip = g_ShipManager->GetResource( 
		     static_cast<CShipTransform *>( pNode->GetArrivingEdges().front()->GetFromNode() )->GetHandle() );
	// If there is no ship, skip handling and use local transform.
	if ( pParentShip == NULL ) 
	{
	  pShip->SetWorldTransform( pShip->GetLocalTransform());
	  return 0;
	}
	pParentWorldTransform = &pParentShip->GetWorldTransform();
	break;
      }
      
      Multiply( *pParentWorldTransform, 
		pShip->GetLocalTransform(), 
		pShip->GetWorldTransform());
      
    }
    else 
    {
      pShip->SetWorldTransform( pShip->GetLocalTransform());
    }
    return 0;
  }
};
/////////////////////////////////////////////////////////////////
enum SPACESHIP_MESSAGES
{
  SHIP_INFLICT_DAMAGE = 0,
  SHIP_MESSAGE_COUNT
};
/////////////////////////////////////////////////////////////////
class CDamageMessage : public CMessage< CSpaceShip, SPACESHIP_MESSAGES >
{
protected:
  float		m_fDamage;
public:
  ////////////////////
  /// Constructor.
  CDamageMessage( const CHandle<CSpaceShip> & hSender, const CHandle<CSpaceShip> &hReceiver, float fDamage ) : m_fDamage( fDamage )
  {
    SetType( SHIP_INFLICT_DAMAGE );
    g_ShipManager->DuplicateHandle( hSender, GetSender());
    g_ShipManager->DuplicateHandle( hReceiver, GetReceiver());
  }
  ////////////////////
  /// Returns damage inflicted.
  const float & GetDamage() const 
  { 
    return m_fDamage; 
  }
};
/////////////////////////////////////////////////////////////////
class CMessageAdapter
{
public:
  ////////////////////
  /// Proceses all messages
  void Process( const CMessage<CSpaceShip, SPACESHIP_MESSAGES> &rMessage, CSpaceShip &rShip )
  {
    switch( rMessage.GetType() )
    {
      // Handles damage messages 
    case SHIP_INFLICT_DAMAGE:
      cerr << "Received damage message" << endl;
      rShip.DecreaseHealth( static_cast<const CDamageMessage & >(rMessage).GetDamage());
      break;
    default:
      cerr << "UNKNOWN MESSAGE" << endl;
      break;
    }
  }
};
/////////////////////////////////////////////////////////////////
class CSpaceShipMessageRouter : public CMessageRouter< CSpaceShip, SPACESHIP_MESSAGES>,
				public CSingleton<CSpaceShipMessageRouter>
{
  friend class CSingleton<CSpaceShipMessageRouter>;
  //friend class CMessageRouter< CSpaceShip, SPACESHIP_MESSAGES>;
private:
  CSpaceShipMessageRouter() : CMessageRouter<CSpaceShip, SPACESHIP_MESSAGES>( SHIP_MESSAGE_COUNT )
  {
  }
public:
  
};
/////////////////////////////////////////////////////////////////
#define g_ShipMsgRouter (CSpaceShipMessageRouter::GetInstance())
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
class CTailParticle : public CParticle 
{
public:
  CVector3<float> m_vOldPositions[4];
  float		  m_aEnergy[4];

};
/////////////////////////////////////////////////////////////////
template <class PARTICLE_TYPE>
class CConstantVelocityPolicy 
{
public:
  CVector3<float> m_vVelocity;
  /// Modifies given particle.
  /// \param p particle which values are to be modified.
  inline void operator()(PARTICLE_TYPE &p) const
  {
    p.m_vVelocity = m_vVelocity;
  } 
};
template <class PARTICLE_TYPE>
class CPositionTailInitPolicy 
{
public:
  CVector3<float> m_vPosition;
  /// Default constructor.
  CPositionTailInitPolicy() { m_vPosition[0] = 0.0; m_vPosition[1] = 0.0; m_vPosition[2] = 0.0;}
  
  /// Modifies given particle.
  /// \param particle a particle which values are to be modified.
  inline void operator()(PARTICLE_TYPE &particle ) 
  {
    particle.m_vPosition = m_vPosition;
    particle.m_vOldPositions[0] = m_vPosition;
    particle.m_vOldPositions[1] = m_vPosition;
    particle.m_vOldPositions[2] = m_vPosition;
    particle.m_vOldPositions[3] = m_vPosition;

    particle.m_aEnergy[0] = 0.75f;
    particle.m_aEnergy[1] = 0.50f;
    particle.m_aEnergy[2] = 0.25f;
    particle.m_aEnergy[3] = 0.0f;

  }
}; // end of CPositionPolicy

/////////////////////////////////////////////////////////////////

template <class PARTICLE_TYPE>
class CPositionTailPolicy : public CSecondPolicyBase
{
private:
  float m_fTailUpdateLimitTime;
  float m_fDecreaseFactor[4];
  float m_fEnergyInit[4];
  float m_fPassedTimeCumul;
  int   m_bMoveTail;
public:
  ////////////////////
  /// Default constructor.
  CPositionTailPolicy() : m_fTailUpdateLimitTime(0.02f), m_fPassedTimeCumul(0.0f)
  {
    m_fEnergyInit[0] = 0.75f;
    m_fEnergyInit[1] = 0.50f;
    m_fEnergyInit[2] = 0.25f;
    m_fEnergyInit[3] = 0.0f;

    m_fDecreaseFactor[0] = (1.0f             - m_fEnergyInit[0]) / m_fTailUpdateLimitTime;
    m_fDecreaseFactor[1] = (m_fEnergyInit[0] - m_fEnergyInit[1]) / m_fTailUpdateLimitTime;
    m_fDecreaseFactor[2] = (m_fEnergyInit[1] - m_fEnergyInit[2]) / m_fTailUpdateLimitTime;
    m_fDecreaseFactor[3] = (m_fEnergyInit[2] - m_fEnergyInit[3]) / m_fTailUpdateLimitTime;

    m_fPassedTimeCumul = 0.0f;    
    m_bMoveTail = 0;
  }
  ////////////////////
  /// Prepares policy for update
  inline void Prepare( unsigned int nPassedTimeInMS )
  {
    m_fPassedTimeInSec = (float)nPassedTimeInMS * 0.001f;
    m_fPassedTimeCumul += m_fPassedTimeInSec;
    if ( m_fPassedTimeCumul >= m_fTailUpdateLimitTime )
    {
      m_bMoveTail = 1;
      m_fPassedTimeCumul = 0.0f;
    } else {
      m_bMoveTail = 0;
    }
    
  }
  ////////////////////
  /// Modifies given particle.
  /// \param particle a particle which values are to be modified.
  inline void operator()(PARTICLE_TYPE &particle ) 
  {
    
    if ( m_bMoveTail )
    {
      particle.m_vOldPositions[3] = particle.m_vOldPositions[2];
      particle.m_vOldPositions[2] = particle.m_vOldPositions[1];
      particle.m_vOldPositions[1] = particle.m_vOldPositions[0];
      particle.m_vOldPositions[0] = particle.m_vPosition;
      
      particle.m_aEnergy[3] = 0.0f;
      particle.m_aEnergy[2] = 0.25f;
      particle.m_aEnergy[1] = 0.5f;
      particle.m_aEnergy[0] = 0.75f;

    }

    particle.m_vPosition += particle.m_vVelocity * GetPassedTime();

    particle.m_aEnergy[0] -= m_fDecreaseFactor[0] * GetPassedTime();
    particle.m_aEnergy[1] -= m_fDecreaseFactor[1] * GetPassedTime();
    particle.m_aEnergy[2] -= m_fDecreaseFactor[2] * GetPassedTime();
    particle.m_aEnergy[3] -= m_fDecreaseFactor[3] * GetPassedTime();

    particle.m_aEnergy[0] = particle.m_aEnergy[0] < 0.0f ? 0.0f : particle.m_aEnergy[0];
    particle.m_aEnergy[1] = particle.m_aEnergy[1] < 0.0f ? 0.0f : particle.m_aEnergy[1];
    particle.m_aEnergy[2] = particle.m_aEnergy[2] < 0.0f ? 0.0f : particle.m_aEnergy[2];
    particle.m_aEnergy[3] = particle.m_aEnergy[3] < 0.0f ? 0.0f : particle.m_aEnergy[3];
  }
}; // end of CPositionPolicy
/////////////////////////////////////////////////////////////////
typedef CParticleSystem<50,
			CCompletePolicy<CTailParticle, 
					CNullPolicy<CTailParticle>,
					CConstantSize<CTailParticle>,
					CConstantVelocityPolicy<CTailParticle>,
					CEnergyInitializer<CTailParticle>,
					CPositionTailInitPolicy<CTailParticle> >,
			CCompletePolicy<CTailParticle, 
					CNullPolicy<CTailParticle>,
					CNullPolicy<CTailParticle>,
					CNullPolicy<CTailParticle>,
					CEnergyPolicy<CTailParticle>,
					CPositionTailPolicy<CTailParticle> >,
			CTailParticle> CPulseWeaponParticleSystem;
/////////////////////////////////////////////////////////////////
class CPulseWeapon 
{
public:

  CPulseWeaponParticleSystem	m_PulseWeaponPS;
  CHandle<CShader>		m_hShaderHandle;
  CVertexDescriptor *		m_pVertexDescriptorHead;
  CVertexDescriptor *		m_pVertexDescriptorTail;
  CVertexDescriptor *		m_pTailColor;

  CVertexDescriptor *		m_pShaderData;
  CIndexArray *			m_pIndicesHead;
  CIndexArray *			m_pIndicesTail;
  ////////////////////
  /// Constructor.
  CPulseWeapon()
  {
    // Each particle is a quad.
    unsigned int nNumIndicesHead = m_PulseWeaponPS.GetMaxParticles()*4;
    // Each tail position is a point, current + 4 old ones + 2 for stitching.
    unsigned int nNumIndicesTail = m_PulseWeaponPS.GetMaxParticles()*7;
    
    m_pVertexDescriptorHead = new CVertexDescriptor( ELEMENT_TYPE_VERTEX_3F,	nNumIndicesHead );
    m_pVertexDescriptorTail = new CVertexDescriptor( ELEMENT_TYPE_VERTEX_3F,	nNumIndicesTail );
    m_pTailColor	    = new CVertexDescriptor( ELEMENT_TYPE_COLOR_4F,	nNumIndicesTail );
    m_pIndicesHead          = new CIndexArray(       PRIMITIVE_QUAD_LIST,	nNumIndicesHead );
    m_pIndicesTail	    = new CIndexArray(       PRIMITIVE_LINE_STRIP,	nNumIndicesTail );
    m_pShaderData           = new CVertexDescriptor( ELEMENT_TYPE_ATTRIB_4F,	nNumIndicesHead );
    ////////////////////
    // Set initial size.
    m_PulseWeaponPS.m_InitializePolicy.m_SizePolicy.m_fSize = 0.4780f;
    m_PulseWeaponPS.m_ActionPolicy.m_EnergyPolicy.m_fEnergyDecrease = 0.50f;

    // create indices for drawing the quads
    if ( m_pIndicesHead->IsShortIndices())
    {
      for(unsigned short int i=0;i<nNumIndicesHead;i++)
      {
	m_pIndicesHead->GetPointer<unsigned short int>()[i]=i;
      }
    } 
    else 
    {
      for(size_t i=0;i<nNumIndicesHead;i++)
      {
	m_pIndicesHead->GetPointer<unsigned int>()[i]=i;
      }
    }
    // create indices for drawing lines
    if ( m_pIndicesTail->IsShortIndices())
    {
      for(unsigned short int i=0;i<nNumIndicesTail;i++)
      {
	m_pIndicesTail->GetPointer<unsigned short int>()[i]=i;
      }
    } 
    else 
    {
      for(size_t i=0;i<nNumIndicesTail;i++)
      {
	m_pIndicesTail->GetPointer<unsigned int>()[i]=i;
      }
    }
    
  }
  ////////////////////
  /// Allocates resources.
  static void CreateResources( COglRenderer & renderer )
  {

  }
  ////////////////////
  //
  void Init( const CVector3<float> & vPosition, const CVector3<float> & vVelocity )
  {
    m_PulseWeaponPS.m_InitializePolicy.m_VelocityPolicy.m_vVelocity = vVelocity;
    m_PulseWeaponPS.m_InitializePolicy.m_PositionPolicy.m_vPosition = vPosition;
    m_PulseWeaponPS.Init( 1, vPosition);
  }
  void Update( unsigned int nMS )
  {
    m_PulseWeaponPS.Update(nMS);
  }
  void UpdateRenderableData()
  {
    size_t nIndex = 0;
    size_t nShaderDataIndex = 0;
    size_t nIndexTail = 0;
    size_t nIndexTailColor = 0;

    for(size_t i=0;i<m_PulseWeaponPS.GetAliveCount();i++)
    {
      // number of coords for one quad 4*3
      nIndex = i*12;
      // number of data slots for four quads 4*4
      nShaderDataIndex = i*16;
    

      m_pVertexDescriptorHead->GetPointer<float>()[nIndex]   = m_PulseWeaponPS.GetParticles()[i].m_vPosition[0];
      m_pVertexDescriptorHead->GetPointer<float>()[nIndex+1] = m_PulseWeaponPS.GetParticles()[i].m_vPosition[1];
      m_pVertexDescriptorHead->GetPointer<float>()[nIndex+2] = m_PulseWeaponPS.GetParticles()[i].m_vPosition[2];

      m_pShaderData->GetPointer<float>()[nShaderDataIndex]    = -1;
      m_pShaderData->GetPointer<float>()[nShaderDataIndex+1]  = 1;
      m_pShaderData->GetPointer<float>()[nShaderDataIndex+2]  = m_PulseWeaponPS.GetParticles()[i].m_fEnergy; 
      m_pShaderData->GetPointer<float>()[nShaderDataIndex+3]  = m_PulseWeaponPS.GetParticles()[i].m_fSize;

      m_pVertexDescriptorHead->GetPointer<float>()[nIndex+3] = m_PulseWeaponPS.GetParticles()[i].m_vPosition[0];
      m_pVertexDescriptorHead->GetPointer<float>()[nIndex+4] = m_PulseWeaponPS.GetParticles()[i].m_vPosition[1];
      m_pVertexDescriptorHead->GetPointer<float>()[nIndex+5] = m_PulseWeaponPS.GetParticles()[i].m_vPosition[2];

      m_pShaderData->GetPointer<float>()[nShaderDataIndex+4]  = -1;
      m_pShaderData->GetPointer<float>()[nShaderDataIndex+5]  = -1;
      m_pShaderData->GetPointer<float>()[nShaderDataIndex+6]  = m_PulseWeaponPS.GetParticles()[i].m_fEnergy; 
      m_pShaderData->GetPointer<float>()[nShaderDataIndex+7]  = m_PulseWeaponPS.GetParticles()[i].m_fSize;

      m_pVertexDescriptorHead->GetPointer<float>()[nIndex+6] = m_PulseWeaponPS.GetParticles()[i].m_vPosition[0];
      m_pVertexDescriptorHead->GetPointer<float>()[nIndex+7] = m_PulseWeaponPS.GetParticles()[i].m_vPosition[1];
      m_pVertexDescriptorHead->GetPointer<float>()[nIndex+8] = m_PulseWeaponPS.GetParticles()[i].m_vPosition[2];

      m_pShaderData->GetPointer<float>()[nShaderDataIndex+8]   =  1;
      m_pShaderData->GetPointer<float>()[nShaderDataIndex+9]   = -1;
      m_pShaderData->GetPointer<float>()[nShaderDataIndex+10]  = m_PulseWeaponPS.GetParticles()[i].m_fEnergy; 
      m_pShaderData->GetPointer<float>()[nShaderDataIndex+11]  = m_PulseWeaponPS.GetParticles()[i].m_fSize;

      m_pVertexDescriptorHead->GetPointer<float>()[nIndex+9]  = m_PulseWeaponPS.GetParticles()[i].m_vPosition[0];
      m_pVertexDescriptorHead->GetPointer<float>()[nIndex+10] = m_PulseWeaponPS.GetParticles()[i].m_vPosition[1];
      m_pVertexDescriptorHead->GetPointer<float>()[nIndex+11] = m_PulseWeaponPS.GetParticles()[i].m_vPosition[2];

      m_pShaderData->GetPointer<float>()[nShaderDataIndex+12]  = 1;
      m_pShaderData->GetPointer<float>()[nShaderDataIndex+13]  = 1;
      m_pShaderData->GetPointer<float>()[nShaderDataIndex+14]  = m_PulseWeaponPS.GetParticles()[i].m_fEnergy; 
      m_pShaderData->GetPointer<float>()[nShaderDataIndex+15]  = m_PulseWeaponPS.GetParticles()[i].m_fSize;


      /////////////////////////////////////////////////////////////////
      /// Tail stuff
      // number of coords for tail 7*3
      nIndexTail = i*21;
      // number of color components 7*4 : 7 points, RGBA
      nIndexTailColor = i*28;
      
      m_pVertexDescriptorTail->GetPointer<float>()[nIndexTail]   = m_PulseWeaponPS.GetParticles()[i].m_vPosition[0];
      m_pVertexDescriptorTail->GetPointer<float>()[nIndexTail+1] = m_PulseWeaponPS.GetParticles()[i].m_vPosition[1];
      m_pVertexDescriptorTail->GetPointer<float>()[nIndexTail+2] = m_PulseWeaponPS.GetParticles()[i].m_vPosition[2];

      m_pVertexDescriptorTail->GetPointer<float>()[nIndexTail+3] = m_PulseWeaponPS.GetParticles()[i].m_vPosition[0];
      m_pVertexDescriptorTail->GetPointer<float>()[nIndexTail+4] = m_PulseWeaponPS.GetParticles()[i].m_vPosition[1];
      m_pVertexDescriptorTail->GetPointer<float>()[nIndexTail+5] = m_PulseWeaponPS.GetParticles()[i].m_vPosition[2];

      m_pVertexDescriptorTail->GetPointer<float>()[nIndexTail+6] = m_PulseWeaponPS.GetParticles()[i].m_vOldPositions[0][0];
      m_pVertexDescriptorTail->GetPointer<float>()[nIndexTail+7] = m_PulseWeaponPS.GetParticles()[i].m_vOldPositions[0][1];
      m_pVertexDescriptorTail->GetPointer<float>()[nIndexTail+8] = m_PulseWeaponPS.GetParticles()[i].m_vOldPositions[0][2];      

      m_pVertexDescriptorTail->GetPointer<float>()[nIndexTail+9] = m_PulseWeaponPS.GetParticles()[i].m_vOldPositions[1][0];
      m_pVertexDescriptorTail->GetPointer<float>()[nIndexTail+10] = m_PulseWeaponPS.GetParticles()[i].m_vOldPositions[1][1];
      m_pVertexDescriptorTail->GetPointer<float>()[nIndexTail+11] = m_PulseWeaponPS.GetParticles()[i].m_vOldPositions[1][2];      

      m_pVertexDescriptorTail->GetPointer<float>()[nIndexTail+12] = m_PulseWeaponPS.GetParticles()[i].m_vOldPositions[2][0];
      m_pVertexDescriptorTail->GetPointer<float>()[nIndexTail+13] = m_PulseWeaponPS.GetParticles()[i].m_vOldPositions[2][1];
      m_pVertexDescriptorTail->GetPointer<float>()[nIndexTail+14] = m_PulseWeaponPS.GetParticles()[i].m_vOldPositions[2][2];      

      m_pVertexDescriptorTail->GetPointer<float>()[nIndexTail+15] = m_PulseWeaponPS.GetParticles()[i].m_vOldPositions[3][0];
      m_pVertexDescriptorTail->GetPointer<float>()[nIndexTail+16] = m_PulseWeaponPS.GetParticles()[i].m_vOldPositions[3][1];
      m_pVertexDescriptorTail->GetPointer<float>()[nIndexTail+17] = m_PulseWeaponPS.GetParticles()[i].m_vOldPositions[3][2];
            
      m_pVertexDescriptorTail->GetPointer<float>()[nIndexTail+18] = m_PulseWeaponPS.GetParticles()[i].m_vOldPositions[3][0];
      m_pVertexDescriptorTail->GetPointer<float>()[nIndexTail+19] = m_PulseWeaponPS.GetParticles()[i].m_vOldPositions[3][1];
      m_pVertexDescriptorTail->GetPointer<float>()[nIndexTail+20] = m_PulseWeaponPS.GetParticles()[i].m_vOldPositions[3][2];
      
      m_pTailColor->GetPointer<float>()[nIndexTailColor]   = 1.0f;
      m_pTailColor->GetPointer<float>()[nIndexTailColor+1] = 1.0f;
      m_pTailColor->GetPointer<float>()[nIndexTailColor+2] = 0.0f;
      m_pTailColor->GetPointer<float>()[nIndexTailColor+3] = 0.0f;

      m_pTailColor->GetPointer<float>()[nIndexTailColor+4] = 1.0f;
      m_pTailColor->GetPointer<float>()[nIndexTailColor+5] = 1.0f;
      m_pTailColor->GetPointer<float>()[nIndexTailColor+6] = 0.0f;
      m_pTailColor->GetPointer<float>()[nIndexTailColor+7] = 1.0f;

      m_pTailColor->GetPointer<float>()[nIndexTailColor+8]  = 1.0f;
      m_pTailColor->GetPointer<float>()[nIndexTailColor+9]  = 1.0f;
      m_pTailColor->GetPointer<float>()[nIndexTailColor+10] = 0.0f;
      m_pTailColor->GetPointer<float>()[nIndexTailColor+11] = m_PulseWeaponPS.GetParticles()[i].m_aEnergy[0];

      m_pTailColor->GetPointer<float>()[nIndexTailColor+12] = 1.0f;
      m_pTailColor->GetPointer<float>()[nIndexTailColor+13] = 1.0f;
      m_pTailColor->GetPointer<float>()[nIndexTailColor+14] = 0.0f;
      m_pTailColor->GetPointer<float>()[nIndexTailColor+15] = m_PulseWeaponPS.GetParticles()[i].m_aEnergy[1];

      m_pTailColor->GetPointer<float>()[nIndexTailColor+16] = 1.0f;
      m_pTailColor->GetPointer<float>()[nIndexTailColor+17] = 1.0f;
      m_pTailColor->GetPointer<float>()[nIndexTailColor+18] = 0.0f;
      m_pTailColor->GetPointer<float>()[nIndexTailColor+19] = m_PulseWeaponPS.GetParticles()[i].m_aEnergy[2];
      
      m_pTailColor->GetPointer<float>()[nIndexTailColor+20] = 1.0f;
      m_pTailColor->GetPointer<float>()[nIndexTailColor+21] = 1.0f;
      m_pTailColor->GetPointer<float>()[nIndexTailColor+22] = 0.0f;
      m_pTailColor->GetPointer<float>()[nIndexTailColor+23] = m_PulseWeaponPS.GetParticles()[i].m_aEnergy[3];

      m_pTailColor->GetPointer<float>()[nIndexTailColor+24] = 1.0f;
      m_pTailColor->GetPointer<float>()[nIndexTailColor+25] = 1.0f;
      m_pTailColor->GetPointer<float>()[nIndexTailColor+26] = 0.0f;
      m_pTailColor->GetPointer<float>()[nIndexTailColor+27] = 0.0f;
    }
    m_pIndicesHead->SetDrawableCount( m_PulseWeaponPS.GetAliveCount()*4 );
    m_pIndicesTail->SetDrawableCount( m_PulseWeaponPS.GetAliveCount()*7 );
  }
  CPulseWeaponParticleSystem	* GetPS() { return &m_PulseWeaponPS; }

};
/////////////////////////////////////////////////////////////////
int CheckForHits( CSovereignClass *pSovereign, CPulseWeaponParticleSystem *pPulseWeapon )
{
  CModel *pModel = g_PhoenixModelManager->GetResource(pSovereign->GetModelHandle());
  assert( pModel != NULL );
  CVertexDescriptor *pVertices = g_DefaultVertexManager->GetResource(pModel->GetVertexHandle());
  assert( pVertices );
  assert( pModel->GetIndexHandles().size() == 1);
  CIndexArray *pIndices = g_DefaultIndexManager->GetResource(pModel->GetIndexHandles().front());
  assert( pIndices );


  CVector3<float> vVertices[3];
  CLine line;
  CVector3<float> intersectPoint;
  CMatrix4x4<float> mInverse;

  if ( InverseMatrix( pSovereign->GetWorldTransform().GetMatrix(), mInverse))
  {
    cerr << "not inverible" << endl;
    return 0;
  }

  if ( pIndices->IsShortIndices())
  {
    // check does triangle intersect
    for( size_t p=0;p<pPulseWeapon->GetAliveCount();p++)
    {
      CVector3<float> vStart = Transform( pPulseWeapon->GetParticles()[p].m_vOldPositions[0], mInverse);
      CVector3<float> vEnd   = Transform( pPulseWeapon->GetParticles()[p].m_vPosition, mInverse);
      line.SetStart(vStart );
      line.SetEnd( vEnd );

      for( size_t i=0;i<pIndices->GetNumIndices();i+=3)
      {
	
	unsigned short int *pTmp = pIndices->GetPointer<unsigned short int>();
	
	vVertices[0][0] = pVertices->GetPointer<float>()[pTmp[i]*3];
	vVertices[0][1] = pVertices->GetPointer<float>()[pTmp[i]*3+1];
	vVertices[0][2] = pVertices->GetPointer<float>()[pTmp[i]*3+2];
	
	vVertices[1][0] = pVertices->GetPointer<float>()[pTmp[i+1]*3];
	vVertices[1][1] = pVertices->GetPointer<float>()[pTmp[i+1]*3+1];
	vVertices[1][2] = pVertices->GetPointer<float>()[pTmp[i+1]*3+2];
	
	vVertices[2][0] = pVertices->GetPointer<float>()[pTmp[i+2]*3];
	vVertices[2][1] = pVertices->GetPointer<float>()[pTmp[i+2]*3+1];
	vVertices[2][2] = pVertices->GetPointer<float>()[pTmp[i+2]*3+2];
      
	//cerr << "Checking: " << line.GetStart() << " and " << line.GetEnd() << endl;
	if ( LineIntersectsTriangle( line, vVertices[0], vVertices[1], vVertices[2], intersectPoint))
	{
	  const_cast<CTailParticle *>(pPulseWeapon->GetParticles())[p].m_fEnergy = 0.0f;
	  cerr << "Collides: " << line.GetStart() << " and " << line.GetEnd() << endl;
	}
       }
    }
  }
  else
  {
    assert( 0 );
  }
  return 0;
}
/////////////////////////////////////////////////////////////////
int CheckForLineHits( CSovereignClass *pSovereign, CLine &line )
{
  CModel *pModel = g_PhoenixModelManager->GetResource(pSovereign->GetModelHandle());
  assert( pModel != NULL );
  CVertexDescriptor *pVertices = g_DefaultVertexManager->GetResource(pModel->GetVertexHandle());
  assert( pVertices );
  assert( pModel->GetIndexHandles().size() == 1);
  CIndexArray *pIndices = g_DefaultIndexManager->GetResource(pModel->GetIndexHandles().front());
  assert( pIndices );


  CVector3<float> vVertices[3];
  CVector3<float> intersectPoint;
  CMatrix4x4<float> mInverse;

  if ( InverseMatrix( pSovereign->GetWorldTransform().GetMatrix(), mInverse))
  {
    cerr << "not inverible" << endl;
    return 0;
  }

  CVector3<float> vStart = Transform( line.GetStart(), mInverse);
  CVector3<float> vEnd   = Transform( line.GetEnd(), mInverse);

  //assert(pVertices->GetSize()==3);

  CLine tmpLine;
  tmpLine.SetStart(vStart );
  tmpLine.SetEnd( vEnd );
  // cerr << "line start = " << tmpLine.GetStart() << endl;
//   cerr << "line end = "   << tmpLine.GetEnd() << endl;

//   cerr << "vertex0 actual: " 
//        << pVertices->GetPointer<float>()[0] << "," 
//        << pVertices->GetPointer<float>()[1] << ","
//        << pVertices->GetPointer<float>()[2] << endl;

//   cerr << "vertex1 actual: " 
//        << pVertices->GetPointer<float>()[3] << "," 
//        << pVertices->GetPointer<float>()[4] << ","
//        << pVertices->GetPointer<float>()[5] << endl;

//   cerr << "vertex2 actual: " 
//        << pVertices->GetPointer<float>()[6] << "," 
//        << pVertices->GetPointer<float>()[7] << ","
//        << pVertices->GetPointer<float>()[8] << endl;

  if ( pIndices->IsShortIndices())
  {
    for( size_t i=0;i<pIndices->GetNumIndices();i+=3)
    {
      unsigned short int *pTmp = pIndices->GetPointer<unsigned short int>();

      vVertices[0][0] = pVertices->GetPointer<float>()[pTmp[i]*3];
      vVertices[0][1] = pVertices->GetPointer<float>()[pTmp[i]*3+1];
      vVertices[0][2] = pVertices->GetPointer<float>()[pTmp[i]*3+2];
      
      vVertices[1][0] = pVertices->GetPointer<float>()[pTmp[i+1]*3];
      vVertices[1][1] = pVertices->GetPointer<float>()[pTmp[i+1]*3+1];
      vVertices[1][2] = pVertices->GetPointer<float>()[pTmp[i+1]*3+2];
      
      vVertices[2][0] = pVertices->GetPointer<float>()[pTmp[i+2]*3];
      vVertices[2][1] = pVertices->GetPointer<float>()[pTmp[i+2]*3+1];
      vVertices[2][2] = pVertices->GetPointer<float>()[pTmp[i+2]*3+2];
      
      
      // cerr << "vertex 0 : " << vVertices[0] << endl;
      // cerr << "vertex 1 : " << vVertices[1] << endl;
      // cerr << "vertex 2 : " << vVertices[2] << endl;
      if ( LineIntersectsTriangle( tmpLine, vVertices[0], vVertices[1], vVertices[2], intersectPoint))
      {
	
	//cerr << "Collides: " << tmpLine.GetStart() << " and " << tmpLine.GetEnd() << endl;
	return 1;
      }
     
      
    }
  }
  else
  {
    assert( 0 );
  }
  return 0;
}
/////////////////////////////////////////////////////////////////
int main()
{

  CSDLScreen::m_SDLScreenParams.m_iWidth  = 800;
  CSDLScreen::m_SDLScreenParams.m_iHeight = 600;


  if ( !CSDLScreen::GetInstance() )
  {
    std::cerr << "Couldn't open screen" << std::endl;
    return 1;
  }
 

  
  CGraph<TRANSFORM_TYPE> *pTG	   = new CGraph<TRANSFORM_TYPE>();
  CExtTransform *pTGR              = pTG->CreateNode< CExtTransform >();
  CShipTransform *pTSovereign      = pTG->CreateNode< CShipTransform >();
  CShipTransform *pTOmega          = pTG->CreateNode< CShipTransform >();
  CTransformUpdater trUpdater;

  pTGR->AddEdge( pTSovereign );
  pTGR->AddEdge( pTOmega );
  
  
  CCamera camera;
  camera.SetPosition( 0, 0.0f,75.0f);
  //camera.SetViewport( 480,340, 160, 120 );
  camera.SetViewport( 0,0, CSDLScreen::m_SDLScreenParams.m_iWidth, CSDLScreen::m_SDLScreenParams.m_iHeight );
  camera.SetNearClipping( 0.1f);
  camera.SetFarClipping( 1500.0f );
  camera.SetFieldOfView( 43.0f);

  CCamera camera2;
  camera2.SetPosition( 0, 0.0f, 75.0f);
  camera2.SetViewport( CSDLScreen::m_SDLScreenParams.m_iWidth/2, 0,
		       CSDLScreen::m_SDLScreenParams.m_iWidth/2, CSDLScreen::m_SDLScreenParams.m_iHeight );
  camera2.SetNearClipping( 0.1f);
  camera2.SetFarClipping( 1500.0f );
  camera2.SetFieldOfView( 43.0f);

  CCamera cameraGUI;
  cameraGUI.SetPosition( 0, 0.0f, 0.0f);
  cameraGUI.SetViewport( 0, 0,
		       CSDLScreen::m_SDLScreenParams.m_iWidth, CSDLScreen::m_SDLScreenParams.m_iHeight );
  cameraGUI.SetNearClipping( -2.0f);
  cameraGUI.SetFarClipping( 3.0f );
  cameraGUI.SetViewOrtho(0,CSDLScreen::m_SDLScreenParams.m_iWidth, 0, CSDLScreen::m_SDLScreenParams.m_iHeight );


  SDL_Event event;
  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
  
  COglRenderer *pOglRenderer = new COglRenderer();
  float afAmbient[4] = { 0.3f, 0.3f, 0.3f, 1.0f };
  glLightModelfv( GL_LIGHT_MODEL_AMBIENT, afAmbient );
  
  CGameObjectOGLAdapter oglAdapter;
  
  CRenderQueue<CGameObject<OBJECT_TYPE> *> *pRenderQueue =  new CRenderQueue<CGameObject<OBJECT_TYPE> *>();
  
  CSpatialGraph *pSpatialGraph = new CSpatialGraph();
  /////////////////////////////////////////////////////////////////
  /// Allocate resources for ships
  COmegaClass::CreateResources( *pOglRenderer );
  CSovereignClass::CreateResources( *pOglRenderer );
  /////////////////////////////////////////////////////////////////
  /// Create ships 
  COmegaClass *pOmega = new COmegaClass();
  pOmega->GetLocalTransform().SetTranslation(0, -50 ,0);
  pOmega->GetLocalTransform().SetScaling(1.0f);
  pSpatialGraph->InsertObject( pOmega );
  
  CSovereignClass *pSovereign = new CSovereignClass();
  pSovereign->GetLocalTransform().SetTranslation( 0, 10,0);
  pSovereign->GetLocalTransform().SetScaling(1.0f);
  pSpatialGraph->InsertObject( pSovereign );
  cerr << "we're ok" << endl;

 
  
  /////////////////////////////////////////////////////////////////
  COglTexture *pExplosionTexture = pOglRenderer->CreateTexture("Resources/Textures/explosion.tga");
  CShader *pPsShader = pOglRenderer->CreateShader( "Resources/Shaders/ps_vertex.glsl", "Resources/Shaders/ps_frag.glsl");  
  CFpsCounter fpsCounter;
  fpsCounter.Reset();
  unsigned int nCollected = 0;

  glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, 0 );    
  glLightModeli( GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR );
  //SDL_WM_GrabInput(SDL_GRAB_ON);
  SDL_WarpMouse( (int)(camera.GetViewport()[2]*0.5f),  (int)(camera.GetViewport()[3]*0.5f) );
  
  CVector2<int> vMousePos((int)(camera.GetViewport()[2]*0.5f),  (int)(camera.GetViewport()[3]*0.5f)) ;
  CLaserBeam::CreateResources( *pOglRenderer );
  CLaserBeam *pBeam = new CLaserBeam();
  
  g_ShipManager->Create( pSovereign, "Enterprise" );
  g_ShipManager->Create( pOmega, "Backgammon");
  
  g_ShipManager->AttachHandle( "Enterprise", pTSovereign->GetHandle());
  g_ShipManager->AttachHandle( "Backgammon", pTOmega->GetHandle());
  
  CSpaceShipUpdaterAdapter shipAdapter( pSpatialGraph );
  
  CObjectUpdater<CSpaceShip> shipUpdater;
  shipUpdater.Manage( pTSovereign->GetHandle() );
  shipUpdater.Manage( pTOmega->GetHandle() );
  CMessageAdapter msgAdapter;
  
  g_ShipMsgRouter->RegisterReceiver( SHIP_INFLICT_DAMAGE, pTSovereign->GetHandle());


  CSkybox skybox;


  g_DefaultTextureManager->Create( pOglRenderer->CreateTexture("Resources/Textures/sb_nebula_top.tga"), 
				   "SKYBOX_TEXTURE0", 
				   skybox.GetTextureHandle(SKYBOX_CEILING) );
  g_DefaultTextureManager->Create( pOglRenderer->CreateTexture("Resources/Textures/sb_nebula_bottom.tga"), 
				   "SKYBOX_TEXTURE1", 
				   skybox.GetTextureHandle(SKYBOX_FLOOR) );
  g_DefaultTextureManager->Create( pOglRenderer->CreateTexture("Resources/Textures/sb_nebula_left.tga"), 
				   "SKYBOX_TEXTURE2", 
				   skybox.GetTextureHandle(SKYBOX_LEFT) );
  g_DefaultTextureManager->Create( pOglRenderer->CreateTexture("Resources/Textures/sb_nebula_right.tga"), 
				   "SKYBOX_TEXTURE3", 
				   skybox.GetTextureHandle(SKYBOX_RIGHT) );
  g_DefaultTextureManager->Create( pOglRenderer->CreateTexture("Resources/Textures/sb_nebula_front.tga"), 
				   "SKYBOX_TEXTURE4", 
				   skybox.GetTextureHandle(SKYBOX_FRONT) );
  g_DefaultTextureManager->Create( pOglRenderer->CreateTexture("Resources/Textures/sb_nebula_back.tga"), 
				   "SKYBOX_TEXTURE5", 
				   skybox.GetTextureHandle(SKYBOX_REAR) );
  // Clear event queue
  while ( SDL_PollEvent(&event ));
  
  CGamePSObject *pParticleSystem = new CGamePSObject();
  pOglRenderer->CommitState( STATE_FACECULLING );
  
  CTimer timer;
  timer.Reset();
  int bChanged = 0;

  CPlane xzPlane(0,1,0,0);
  
  int bMousePressed = 0;
  g_ShipMsgRouter->Prepare();


  string strFPS;
  CVector4<unsigned char> vWhite(255,255,255,255);
  CVector4<unsigned char> vEnergyRayColor(0,255,0,255);
  CFontset *pFontset = pOglRenderer->CreateFontset( "Resources/Fonts/trebuc.ttf", 15);

  ostringstream *pStream = new ostringstream();
  *pStream << pOglRenderer->GetFeatures();
  string strFeatures = pStream->str();
  delete pStream;
  pStream = NULL;

  CEnergyBolt::CreateResources( *pOglRenderer );
  CEnergyBolt *pEnergyBolt = new CEnergyBolt();
  CTransform transform;
  transform.SetTranslation( 0, 12.0, 0.0);

  
  CPulseWeapon *pPulseWeapon = new CPulseWeapon();
  COglTexture *pPulse = pOglRenderer->CreateTexture( "Resources/Textures/pulseweapon.tga" );
  assert( pPulse != 0);
  CVector3<float> m_vFireDirection(0,0,1);
  CQuaternion qUp, qDown;
  qUp.CreateFromAxisAngle( 1,0,0, 2.0f);
  qDown.CreateFromAxisAngle( 1,0,0, -2.0f);

  CLine energyRay;

  while( g_bLoop )
  {
    fpsCounter.Update();
    g_ShipMsgRouter->Update( msgAdapter);
    while ( SDL_PollEvent(&event ))
    {
      switch(event.type)
      {
      case SDL_KEYDOWN:
	if ( event.key.keysym.sym == SDLK_ESCAPE)	
	{
	  g_bLoop = 0;
	}
	else if ( event.key.keysym.sym == SDLK_w )
	{
	  camera.Move( 1.3f );
	}      
	else if ( event.key.keysym.sym == SDLK_s )
	{
	  camera.Move( -1.3f );
	} 
	else if ( event.key.keysym.sym == SDLK_a )
	{
	  camera.Strafe( -1.1f );
	}      
	else if ( event.key.keysym.sym == SDLK_d )
	{
	  camera.Strafe( 1.1f );
	} 
	else if ( event.key.keysym.sym == SDLK_r )
	{
	  camera.RotateAroundRight( -1.1f );
	}      
	else if ( event.key.keysym.sym == SDLK_f )
	{
	  camera.RotateAroundRight( 1.1f );

	} 
	else if ( event.key.keysym.sym == SDLK_PAGEUP )
	{
	  RotateVector( qUp, m_vFireDirection);
	}
	else if ( event.key.keysym.sym == SDLK_PAGEDOWN )
	{
	  RotateVector( qDown, m_vFireDirection);
	}
	else if ( event.key.keysym.sym == SDLK_RETURN )
	{
	  //pParticleSystem->Init( pSovereign->GetWorldTransform().GetTranslation() + pSovereign->GetForwardVector());
	  //pBeam->SetEnabled(1);
	  g_ShipMsgRouter->EnqueueMessage( new CDamageMessage( pTOmega->GetHandle(), pTSovereign->GetHandle(), 0.3f ));
	  pPulseWeapon->Init( CVector3<float>(0,0,0), m_vFireDirection*45.0f );
	  //pPulseWeapon->Init( CVector3<float>(0,0,0), pSovereign->GetWorldTransform().GetTranslation().GetNormalized()*45.0f );
	}
	 else if ( event.key.keysym.sym == SDLK_LEFT )
	{
	  camera.RotateAroundUp( 1.6f);
	  //pSovereign->GetLocalTransform().SetScaling(pSovereign->GetLocalTransform().GetScaling()-0.1f);
	} 
	else if ( event.key.keysym.sym == SDLK_RIGHT )
	{
	  //pSovereign->GetLocalTransform().SetScaling(pSovereign->GetLocalTransform().GetScaling()+0.1f);
	  camera.RotateAroundUp( -1.6f);
	}
	break;
      case SDL_MOUSEBUTTONDOWN:
	g_bMousePressed = 1;
	break;
      case SDL_MOUSEBUTTONUP:
	g_bMousePressed = 0;
	break;
      case SDL_MOUSEMOTION:
	{
	  CVector2<int> vMousePosCurrent(event.motion.x, event.motion.y);
	  CVector2<int> vMouseDiff = vMousePos - vMousePosCurrent;
	  vMouseDiff[0] = -vMouseDiff[0];
	  //camera.RotateAroundUp(vMouseDiff[0]);
	  //camera.RotateAroundRight(vMouseDiff[1]);	
	  if ( g_bMousePressed ){
	    camera.VirtualTrackball( pSovereign->GetWorldTransform().GetTranslation(),
				     vMousePos,
				     vMousePosCurrent );
	  }
	  
	  vMousePos = vMousePosCurrent;
	  
	  }
	break;
      default:
	break;
      }
    }
    
    // track sovereign with camera.
    //camera.Move( (pSovereign->GetWorldTransform().GetTranslation()-camera.GetPosition()).Dot(camera.GetUpVector())*camera.GetUpVector()      );
    //camera.Move( (pSovereign->GetWorldTransform().GetTranslation()-camera.GetPosition()).Dot(camera.GetRightVector())*camera.GetRightVector());
    
    pRenderQueue->Clear();
    //cerr << "Collected: " << nCollected << endl;
    
    if ( camera.IsProjectionChanged() )  {   bChanged = 1; camera.UpdateProjection();    }
    if ( camera.IsViewChanged() )     {      bChanged = 1; camera.UpdateView();          }
    if ( bChanged )
    {
      camera.CalculateFrustum();    
      camera.CalculateBoundingSphere();
      camera.CalculateBoundingCone();
      bChanged = 0;
    }
    

    //TravelDF<SPACESHIP_CLASS, CTransformUpdater>( pTGR, &trUpdater );

    //pOglRenderer->ClearBuffer( COLOR_BUFFER );
    pOglRenderer->ClearBuffer( COLOR_BUFFER );
    pOglRenderer->CommitState( STATE_DEPTH_TEST );

    nCollected = pRenderQueue->CollectObjects( camera, *pSpatialGraph ) ;
    pOglRenderer->CommitCamera( camera );
    pOglRenderer->DisableState( STATE_LIGHTING );
    
    pOglRenderer->CommitSkybox( skybox, camera );
    pOglRenderer->ClearBuffer( DEPTH_BUFFER );    
    
    pOglRenderer->CommitShader( NULL );    
    pRenderQueue->Render<CGameObjectOGLAdapter>( *pOglRenderer, oglAdapter );

    pOglRenderer->CommitState( STATE_BLENDING);
    pOglRenderer->DisableState( STATE_DEPTH_WRITE );
    //BLEND_SRC_ONE
    pOglRenderer->CommitBlending( BLEND_SRC_SRC_ALPHA, BLEND_DST_ONE );
    if ( pBeam->IsEnabled())
      pOglRenderer->CommitModel(*pBeam);

    if ( timer.HasPassed(0,5) )
    {
      pParticleSystem->Update(timer.GetPassedTime().GetMilliSeconds());
      // update weapon, if such is active
      pPulseWeapon->Update( timer.GetPassedTime().GetMilliSeconds() );
      
      shipUpdater.Update<CSpaceShipUpdaterAdapter>( shipAdapter, timer.GetPassedTime().GetMilliSeconds());
      TravelDF<CTransformUpdater, TRANSFORM_TYPE, std::string, int>( static_cast<CGraphNode<TRANSFORM_TYPE> *>(pTGR), &trUpdater );
      timer.Reset();
      pBeam->IncreaseTime( timer.GetPassedTime().GetMilliSeconds()*0.001f);
      if ( pBeam->IsEnabled())
      {
	pBeam->Initialize( pSovereign->GetWorldTransform().GetTranslation(), 
			   pOmega->GetWorldTransform().GetTranslation(), 0.95f );
      }
      
    }
    CheckForHits( pSovereign, pPulseWeapon->GetPS());      
    
    energyRay.SetStart( CVector3<float>(0,0,0));
    energyRay.SetEnd( m_vFireDirection*pSovereign->GetWorldTransform().GetTranslation().Length()*1.5f);

    if ( CheckForLineHits( pSovereign, energyRay ))
    {
      vEnergyRayColor = CVector4<unsigned char>(255,0,0,255);
    } 
    else 
    {
      
      //cerr << "does not hit" << endl;
      vEnergyRayColor = CVector4<unsigned char>(0,255,0,255);
    }
    

    pParticleSystem->UpdateRenderableData();
    //pOglRenderer->CommitBlending( BLEND_SRC_SRC_ALPHA, BLEND_DST_ONE_MINUS_SRC_ALPHA );
    pOglRenderer->CommitBlending( BLEND_SRC_SRC_ALPHA, BLEND_DST_ONE );
    pOglRenderer->CommitShader( pPsShader );    
    pOglRenderer->CommitTexture( 0, pExplosionTexture );
    pOglRenderer->CommitVertexDescriptor(pParticleSystem->GetVertices());
    pOglRenderer->CommitShaderParam( *pPsShader, "stuff", *pParticleSystem->GetShaderData());
    pOglRenderer->CommitUniformShaderParam( *pPsShader, "texture", 0);
    pOglRenderer->CommitPrimitive(pParticleSystem->GetIndices());

    pOglRenderer->CommitVertexDescriptor(pParticleSystem->GetVerticesDebris());
    pOglRenderer->CommitShaderParam( *pPsShader, "stuff", *pParticleSystem->GetShaderDataDebris());
    //pOglRenderer->CommitUniformShaderParam( *pPsShader, "texture", 0);
    pOglRenderer->CommitPrimitive(pParticleSystem->GetIndicesDebris());

    // render pulse weapon fire
    pPulseWeapon->UpdateRenderableData();
    // Render actual bolt
    pOglRenderer->CommitState( STATE_BLENDING);
    pOglRenderer->DisableState( STATE_DEPTH_WRITE );
    pOglRenderer->CommitShader( pPsShader );    
    pOglRenderer->CommitTexture( 0, pPulse );
    pOglRenderer->CommitVertexDescriptor( pPulseWeapon->m_pVertexDescriptorHead );
    pOglRenderer->CommitShaderParam( *pPsShader, "stuff", *pPulseWeapon->m_pShaderData);
    pOglRenderer->CommitPrimitive(pPulseWeapon->m_pIndicesHead);
    /////////////////////////////////////////////////////////////////
    // Render tail
    glLineWidth(2.0f);
    //glEnable(GL_LINE_SMOOTH );
    pOglRenderer->CommitState( STATE_BLENDING);
    pOglRenderer->DisableState( STATE_DEPTH_WRITE );
    pOglRenderer->CommitShader( NULL );
    pOglRenderer->DisableTexture( 0 );
    pOglRenderer->CommitVertexDescriptor( pPulseWeapon->m_pVertexDescriptorTail );
    pOglRenderer->CommitVertexDescriptor( pPulseWeapon->m_pTailColor );
    pOglRenderer->CommitPrimitive( pPulseWeapon->m_pIndicesTail );
    pOglRenderer->RollbackVertexDescriptor( pPulseWeapon->m_pTailColor );
    glLineWidth(1.0f);
    glDisable(GL_LINE_SMOOTH );

    pOglRenderer->CommitColor(vEnergyRayColor);
    glBegin(GL_LINES);
      glVertex3fv( energyRay.GetStart().GetArray());
      glVertex3fv( energyRay.GetEnd().GetArray());
    glEnd();
    pOglRenderer->CommitColor( vWhite );
    /////////////////////////////////////////////////////////////////
    /// Draw energybolt
    pOglRenderer->DisableState( STATE_BLENDING);
    pOglRenderer->CommitState( STATE_DEPTH_WRITE );
    pOglRenderer->CommitShader( NULL );
    
    transform.Move( 0,0,0.00130);
    pOglRenderer->CommitTransform( transform );
    pOglRenderer->DisableState( STATE_FACECULLING );
    pOglRenderer->DisableState( STATE_DEPTH_WRITE );
    pOglRenderer->CommitState( STATE_DEPTH_TEST );
    pOglRenderer->CommitState( STATE_BLENDING );
    CVector4<unsigned char> vYellow(255,200,100,255);
    pOglRenderer->CommitColor(vYellow);
    pOglRenderer->CommitBlending( BLEND_SRC_SRC_ALPHA, BLEND_DST_ONE );
    pOglRenderer->CommitModel( *g_PhoenixModelManager->GetResource(pEnergyBolt->GetModelHandle()) );
    pOglRenderer->DisableState( STATE_BLENDING );
    pOglRenderer->CommitState( STATE_DEPTH_WRITE );
    pOglRenderer->RollbackTransform();
    pOglRenderer->CommitColor(vWhite);
    ////////////////////
    // Draw GUI elements 
    pOglRenderer->CommitCamera( cameraGUI );
    pOglRenderer->DisableState( STATE_DEPTH_TEST );
    pOglRenderer->DisableTexture( 0, pExplosionTexture );
    DrawSurroundingQuad( pOglRenderer, camera, pSovereign);

    ////////////////////
    /// Render FPS + OpenGL information 
    pOglRenderer->DisableState( STATE_LIGHTING );
    pOglRenderer->DisableState( STATE_FACECULLING );
    pOglRenderer->CommitState( STATE_BLENDING );
    pOglRenderer->CommitBlending( BLEND_SRC_ONE, BLEND_DST_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc( GL_GREATER, 0.0f);
    pOglRenderer->CommitColor( vWhite );
    pOglRenderer->CommitString( *pFontset, 100, 100, strFPS.c_str());
    size_t prevpos = 0;
    size_t pos     = strFeatures.find("\n");
    if ( pos == string::npos )
    {
      pos = strFeatures.size();
    } 
    
    int offset = 0;
    while ( pos != string::npos )
    {
      pOglRenderer->CommitString( *pFontset, 100, 500+offset, strFeatures.substr(prevpos, pos-prevpos).c_str() );
      prevpos = pos+1;
      pos = strFeatures.find("\n", prevpos+1);
      offset -= 15;
    }
    
    pOglRenderer->CommitState( STATE_DEPTH_TEST );
    glDisable(GL_ALPHA_TEST);
    pOglRenderer->DisableState( STATE_BLENDING );

    pOglRenderer->CommitShader( NULL );    
    
//     pOglRenderer->CommitCamera( camera2 );


//     pOglRenderer->CommitColor( vWhite );
//     pOglRenderer->CommitModel( *g_PhoenixModelManager->GetResource(pSovereign->GetModelHandle()) );

//     CMatrix4x4<float> inv;
//     if ( InverseMatrix( pSovereign->GetWorldTransform().GetMatrix(), inv))
//     {
//       cerr << "not inverible" << endl;
//       return 0;
//     }
    
//     CVector3<float> vStart = Transform( energyRay.GetStart(), inv);
//     CVector3<float> vEnd   = Transform( energyRay.GetEnd(), inv);
//     pOglRenderer->CommitShader( NULL );    
//     pOglRenderer->CommitColor(CVector4<unsigned char>(255,0,0,255));
//     pOglRenderer->DisableTexture(0);
//     glBegin(GL_LINES);
//       glVertex3fv( vStart.GetArray());
//       glVertex3fv( vEnd.GetArray());
//     glEnd();
//     pOglRenderer->CommitColor( vWhite );
//     pOglRenderer->CommitShader( NULL );    
    pOglRenderer->Finalize();    
    CSDLScreen::GetInstance()->SwapBuffers();
    fpsCounter++;

    if ( fpsCounter.HasPassed(1,0) )
    {
      
      ostringstream str; 
      str << "FPS: " << fpsCounter << ", with visible objects #"<< nCollected;
      strFPS = str.str();
      //cerr << "Screen coords : " << camera.WorldCoordinatesToScreen(pSovereign->GetTransform().GetTranslation()) << endl;
      fpsCounter.Reset();

    }
    
  }

  CSovereignClass::ReleaseResources(*pOglRenderer);
  COmegaClass::ReleaseResources(*pOglRenderer);
  //pOglRenderer->RollbackCache( *pVD );
  CSDLScreen::DestroyInstance();
  SDL_WM_GrabInput(SDL_GRAB_OFF);
  return 0;
}

/* Point sprite particle code:
  float quadratic[] =  { 1.0f, 0.0f, 0.01f };
  glPointParameterfvARB( GL_POINT_DISTANCE_ATTENUATION_ARB, quadratic );
  glPointParameterfARB( GL_POINT_FADE_THRESHOLD_SIZE_ARB, 60.0f );
  glPointParameterfARB( GL_POINT_SIZE_MIN_ARB, 10.0f );
  glPointParameterfARB( GL_POINT_SIZE_MAX_ARB, 100.0f );
  glTexEnvf( GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE );
  glEnable( GL_POINT_SPRITE_ARB );
  glPointSize( 144.0f );
  pOglRenderer->CommitTexture( 0, pExplosionTexture );
  pOglRenderer->CommitVertexDescriptor( pParticleSystem->GetVertices());
  pOglRenderer->CommitPrimitive( pParticleSystem->GetIndices());
  glDisable(GL_POINT_SPRITE_ARB);
*/

//   // Shove in about 512 objects
//   //#define INSERT_OBJ
// #ifdef INSERT_OBJ
//   for(unsigned int w=0;w<8;w++)
//     for(unsigned int h=0;h<8;h++)
//       for(unsigned int d=0;d<8;d++)
//       {
// 	//CGameObject<OBJECT_TYPE> *pTemp = new CGameObject<OBJECT_TYPE>();
// 	CGameLODObject *pTemp = new CGameLODObject();
// 	pTemp->GetTransform().SetTranslation( -200.0f+w*145,-200.0f+h*145,-200.0f+d*145);  
// 	pTemp->GetBoundingSphere() = sphere;
// 	g_PhoenixModelManager->AttachHandle( "OmegaModel", static_cast<CGameObject<OBJECT_TYPE> *>(pTemp)->GetModelHandle());
// 	g_PhoenixModelManager->AttachHandle( "OmegaModelLod2", pTemp->GetModelHandle(0));
// 	pSpatialGraph->InsertObject(pTemp);
// 	//std::cerr << "inserting " << std::endl;
//       }
// #endif
  

