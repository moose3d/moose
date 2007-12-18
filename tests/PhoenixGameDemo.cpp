#include "Phoenix.h"
#include <iostream>
#include <SDL.h>
#include <sstream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <tcl.h>
#include "PhoenixEnergyBeam.h"
#include "TransformGraph.h"
using std::string;
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Core;
using namespace Phoenix::Math;
using namespace Phoenix::Spatial;
using namespace Phoenix::Window;
using namespace Phoenix::Graphics;
using namespace Phoenix::Scene;
using namespace Phoenix::Volume;
using std::cerr;
using std::endl; 
/////////////////////////////////////////////////////////////////
#define g_PhoenixModelManager (CResourceManager<CModel, CHandle<CModel> >::GetInstance())
typedef CHandle<CModel> MODEL_HANDLE;
/////////////////////////////////////////////////////////////////
enum OBJECT_TYPE 
{
  O_TYPE_ORDINARY = 0,
  O_TYPE_LOD = 1,
  O_TYPE_PS = 2
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
    //p.m_fSize = 1.5f;
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
typedef CParticleSystem<50,
			CCompletePolicy<CRotatingParticle, 
					CNullPolicy<CRotatingParticle>,
					CRandomSize<CRotatingParticle>,
					CRandomVelocity<CRotatingParticle>,
					CEnergyInitializer<CRotatingParticle>,
					CNullPolicy<CRotatingParticle> >,
			CCompletePolicy<CRotatingParticle, 
					CNullPolicy<CRotatingParticle>,
					CIncreaseSizePolicy<CRotatingParticle>,
					CNullPolicy<CRotatingParticle>,
					CEnergyPolicy<CRotatingParticle>,
					CPositionPolicy<CRotatingParticle> >,
			CRotatingParticle> CExplosion;
typedef CParticleSystem<150,
			CCompletePolicy<CRotatingParticle, 
					CNullPolicy<CRotatingParticle>,
					CConstantSize<CRotatingParticle>,
					CRandomVelocity<CRotatingParticle>,
					CEnergyInitializer<CRotatingParticle>,
					CNullPolicy<CRotatingParticle> >,
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

  CExplosion         m_Explosion;
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
    unsigned int nNumIndices = m_Explosion.GetMaxParticles()*4;

    m_pVertexDescriptor = new CVertexDescriptor(ELEMENT_TYPE_VERTEX_3F, nNumIndices);
    m_pShaderData = new CVertexDescriptor(ELEMENT_TYPE_ATTRIB_4F, nNumIndices);
    m_pIndices = new CIndexArray( PRIMITIVE_QUAD_LIST, nNumIndices);

    unsigned int nNumIndicesDebris  = m_ExplosionDebris.GetMaxParticles()*4;
    m_pVertexDescriptorDebris = new CVertexDescriptor(ELEMENT_TYPE_VERTEX_3F, nNumIndicesDebris);
    m_pShaderDataDebris = new CVertexDescriptor(ELEMENT_TYPE_ATTRIB_4F, nNumIndicesDebris);
    m_pIndicesDebris = new CIndexArray( PRIMITIVE_QUAD_LIST, nNumIndicesDebris);    

    // Set initial size 
    m_Explosion.m_InitializePolicy.m_SizePolicy.m_fSize = 1.0f;
    m_Explosion.m_ActionPolicy.m_EnergyPolicy.m_fEnergyDecrease = 0.35f;
    m_Explosion.m_InitializePolicy.m_VelocityPolicy.m_fMinSpeed = 0.5f;
    m_Explosion.m_InitializePolicy.m_VelocityPolicy.m_fMaxSpeed = 1.75f;
    
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
    for(size_t i=0;i<m_Explosion.GetAliveCount();i++)
    {
      // number of coords for one quad 4*3
      nIndex = i*12;
      // number of data slots for four quads 4*4
      nShaderDataIndex = i*16;
      
      m_pVertexDescriptor->GetPointer<float>()[nIndex]   = m_Explosion.GetParticles()[i].m_vPosition[0];
      m_pVertexDescriptor->GetPointer<float>()[nIndex+1] = m_Explosion.GetParticles()[i].m_vPosition[1];
      m_pVertexDescriptor->GetPointer<float>()[nIndex+2] = m_Explosion.GetParticles()[i].m_vPosition[2];
      m_pShaderData->GetPointer<float>()[nShaderDataIndex]    = -1;
      m_pShaderData->GetPointer<float>()[nShaderDataIndex+1]  = 1;
      m_pShaderData->GetPointer<float>()[nShaderDataIndex+2]  = m_Explosion.GetParticles()[i].m_fEnergy; 
      m_pShaderData->GetPointer<float>()[nShaderDataIndex+3]  = m_Explosion.GetParticles()[i].m_fSize;

      m_pVertexDescriptor->GetPointer<float>()[nIndex+3] = m_Explosion.GetParticles()[i].m_vPosition[0];
      m_pVertexDescriptor->GetPointer<float>()[nIndex+4] = m_Explosion.GetParticles()[i].m_vPosition[1];
      m_pVertexDescriptor->GetPointer<float>()[nIndex+5] = m_Explosion.GetParticles()[i].m_vPosition[2];
      m_pShaderData->GetPointer<float>()[nShaderDataIndex+4]  = -1;
      m_pShaderData->GetPointer<float>()[nShaderDataIndex+5]  = -1;
      m_pShaderData->GetPointer<float>()[nShaderDataIndex+6]  = m_Explosion.GetParticles()[i].m_fEnergy; 
      m_pShaderData->GetPointer<float>()[nShaderDataIndex+7]  = m_Explosion.GetParticles()[i].m_fSize;;

      m_pVertexDescriptor->GetPointer<float>()[nIndex+6] = m_Explosion.GetParticles()[i].m_vPosition[0];
      m_pVertexDescriptor->GetPointer<float>()[nIndex+7] = m_Explosion.GetParticles()[i].m_vPosition[1];
      m_pVertexDescriptor->GetPointer<float>()[nIndex+8] = m_Explosion.GetParticles()[i].m_vPosition[2];
      m_pShaderData->GetPointer<float>()[nShaderDataIndex+8]   =  1;
      m_pShaderData->GetPointer<float>()[nShaderDataIndex+9]   = -1;
      m_pShaderData->GetPointer<float>()[nShaderDataIndex+10]  = m_Explosion.GetParticles()[i].m_fEnergy; 
      m_pShaderData->GetPointer<float>()[nShaderDataIndex+11]  = m_Explosion.GetParticles()[i].m_fSize;;

      m_pVertexDescriptor->GetPointer<float>()[nIndex+9]  = m_Explosion.GetParticles()[i].m_vPosition[0];
      m_pVertexDescriptor->GetPointer<float>()[nIndex+10] = m_Explosion.GetParticles()[i].m_vPosition[1];
      m_pVertexDescriptor->GetPointer<float>()[nIndex+11] = m_Explosion.GetParticles()[i].m_vPosition[2];
      m_pShaderData->GetPointer<float>()[nShaderDataIndex+12]  = 1;
      m_pShaderData->GetPointer<float>()[nShaderDataIndex+13]  = 1;
      m_pShaderData->GetPointer<float>()[nShaderDataIndex+14]  = m_Explosion.GetParticles()[i].m_fEnergy; 
      m_pShaderData->GetPointer<float>()[nShaderDataIndex+15]  = m_Explosion.GetParticles()[i].m_fSize;

    }
    m_pIndices->SetDrawableCount( m_Explosion.GetAliveCount()*4 );
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
    m_Explosion.Update(nMS);
    m_ExplosionDebris.Update(nMS);
    m_nPassedTimeMS+=nMS;
    if ( m_Explosion.IsAlive() && !m_bSecondDone && m_nPassedTimeMS > 750 )
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
    m_Explosion.Init(5, vPosition );
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
    unsigned int nLength = (unsigned int)vDiff.Length();
    renderer.CommitTransform( pObject->GetWorldTransform() );
    if ( nLength < 140 || pObject->GetType() == O_TYPE_ORDINARY)
      renderer.CommitModel( *g_PhoenixModelManager->GetResource(pObject->GetModelHandle()));
    else
    {
      CModel &model = *g_PhoenixModelManager->GetResource( static_cast<const CGameLODObject *>(pObject)->GetModelHandle(0));
      // Retrieve resources
      COglTexture *pTexture = NULL;
      CVertexDescriptor *pTemp = NULL;
      CVertexDescriptor *pVertices = g_DefaultVertexManager->GetResource(model.GetVertexHandle());

      assert( pVertices != NULL );
      CIndexArray *pIndices = NULL;
  
      // Commit textures
      for( unsigned int i=0; i<TEXTURE_HANDLE_COUNT; i++)
      {
	pTemp    = g_DefaultVertexManager->GetResource(  model.GetTextureCoordinateHandle(i));
	pTexture = g_DefaultTextureManager->GetResource( model.GetTextureHandle(i) );
    
	// check that texcoord resources actually exist
	if ( pTemp     != NULL ) 
	{ 
	  renderer.CommitVertexDescriptor( pTemp, i ); 
	} 
	// check that texture resource exists
	if ( pTexture  != NULL ) 
	{ 
	  renderer.CommitTexture( i, pTexture ); 
	  // Apply texture filters.
	  std::vector<TEXTURE_FILTER> &vecFilters = model.GetTextureFilters(i);
      
	  for(unsigned int nFilter=0; nFilter<vecFilters.size(); nFilter++)
	  {
	    renderer.CommitFilter( vecFilters[nFilter], pTexture->GetType() );
	  }
	}
      }
      // if shader exists
      if ( model.GetShaderHandle().IsNull() == 0 )
      {
	CShader *pShader = g_DefaultShaderManager->GetResource(model.GetShaderHandle());
	renderer.CommitShader( pShader );
	CVertexDescriptor *pParam = NULL;
	// Go through all parameters and commit them
	for(unsigned int nSP=0; nSP< model.GetShaderParameters().size(); nSP++)
	{
	  pParam = g_DefaultVertexManager->GetResource( model.GetShaderParameters()[nSP].second );
	  if ( pParam != NULL )
	  {
	    renderer.CommitShaderParam( *pShader, model.GetShaderParameters()[nSP].first, *pParam );
	  }
	}
	// Go through all int parameters and commit them
	for(unsigned int nSP=0; nSP< model.GetShaderIntParameters().size(); nSP++)
	{
	  renderer.CommitUniformShaderParam( *pShader, model.GetShaderIntParameters()[nSP].first, model.GetShaderIntParameters()[nSP].second );
	}
	// Go through all float parameters and commit them
	for(unsigned int nSP=0; nSP< model.GetShaderFloatParameters().size(); nSP++)
	{
	  renderer.CommitUniformShaderParam( *pShader, model.GetShaderFloatParameters()[nSP].first, model.GetShaderFloatParameters()[nSP].second );
	}
      }
      // check and commit resources
      if ( pVertices != NULL ) 
      { 
	renderer.CommitVertexDescriptor ( pVertices ); 
      }
      CVertexDescriptor *pNormals = g_DefaultVertexManager->GetResource(model.GetNormalHandle());
      if ( pNormals != NULL ) 
      { 
	renderer.CommitVertexDescriptor( pNormals ); 
      }

      for(unsigned int n=0;n<model.GetIndexHandles().size();n++)
      {
	pIndices = g_DefaultIndexManager->GetResource( model.GetIndexHandles()[n] );
	if ( pIndices  != NULL ) 
	{ 
	  renderer.CommitPrimitive ( pIndices );         
	}
      }
      
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
public:

  ////////////////////
  /// Constructor.
  CSpaceShip() : m_fSpeed(0.0f)
  {
    
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
    g_PhoenixModelManager->AttachHandle( SOVEREIGN_RESOURCE, GetModelHandle() );
    SetType(O_TYPE_ORDINARY);
    GetBoundingSphere() = CSovereignClass::m_Sphere;
    GetBoundingBox() = CSovereignClass::m_Box;
    CModel *pModel = g_PhoenixModelManager->GetResource( SOVEREIGN_RESOURCE );
    pModel->AddTextureFilter(MIN_MIP_LINEAR);
    pModel->AddTextureFilter(MAG_LINEAR);
    SetShipClass( SHIP_CLASS_SOVEREIGN );

  }
  ////////////////////
  void Update( unsigned int nTimeMS )
  {
    m_fPassedTime = ((float)nTimeMS * 0.001f);
    if ( pInterp )
    {
      m_pCurrentObject = this;
      assert(Tcl_Eval(pInterp, "playComputer") == TCL_OK );
      
    }
    
  }
  ////////////////////
  /// Allocates proper resources for sovereign class ships.
  /// \param rOglRenderer Renderer for creating cache.
  static void CreateResources( COglRenderer & rOglRenderer)
  {
    Phoenix::Data::CMilkshapeLoader loader;
    std::string name("Resources/Models/1701-e.ms3d");
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
    
    pInterp = Tcl_CreateInterp();
    assert( pInterp != NULL );


    assert(Tcl_CreateObjCommand( pInterp, "Rotate",
				 Rotate, (ClientData)0,
				 NULL) != NULL );
    assert(Tcl_CreateObjCommand( pInterp, "MoveForward",
				 MoveForward, (ClientData)0,
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
    CVertexDescriptor *pTemp = g_DefaultVertexManager->GetResource( SOVEREIGN_VERTICES );
    CIndexArray *pIndices = g_DefaultIndexManager->GetResource( SOVEREIGN_INDICES );
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
  static int MoveForward( ClientData clientData, Tcl_Interp *pInterp, int objc, Tcl_Obj * CONST objv[])
  {
    if ( m_pCurrentObject != NULL )
    {
      double dSpeed;
      
      Tcl_GetDoubleFromObj( pInterp, objv[1], &dSpeed );
      
      m_pCurrentObject->GetLocalTransform().Move( m_pCurrentObject->GetForwardVector() * dSpeed * m_fPassedTime);
      
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
#define OMEGA_MODEL "Resources/Models/omega.ms3d"
#define OMEGA_VERTICES "omega_vertices"
#define OMEGA_TEXCOORDS "omega_texcoords"
#define OMEGA_TEXTURE "omega_texture"
#define OMEGA_NORMALS "omega_normals"
#define OMEGA_INDICES "omega_indices"
#define OMEGA_RESOURCE "omega_model"
/////////////////////////////////////////////////////////////////
class COmegaClass : public CSpaceShip
{
public:
  ////////////////////
  /// Constructor.
  COmegaClass()
  {
    g_PhoenixModelManager->AttachHandle( OMEGA_RESOURCE, GetModelHandle() );
    SetType(O_TYPE_ORDINARY);
    GetBoundingSphere() = m_Sphere;
    CModel *pModel = g_PhoenixModelManager->GetResource( OMEGA_RESOURCE );
    pModel->AddTextureFilter(MIN_MIP_LINEAR);
    pModel->AddTextureFilter(MAG_LINEAR);
    SetShipClass( SHIP_CLASS_OMEGA );
  }
  ////////////////////
  /// Allocates proper resources for omega class ships.
  /// \param rOglRenderer Renderer for creating cache.
  static void CreateResources( COglRenderer & rOglRenderer)
  {
    Phoenix::Data::CMilkshapeLoader loader;
    
    assert ( loader.Load( OMEGA_MODEL ) == 0 && "Could not open model file!");
    loader.GenerateModelData();
    //loader.Stripify();
    
    CModel *pSovereignModel = new CModel();

    assert(g_DefaultVertexManager->Create( loader.GetVertices(),  OMEGA_VERTICES,  pSovereignModel->GetVertexHandle() )            == 0);  
    assert(g_DefaultVertexManager->Create( loader.GetTexCoords(), OMEGA_TEXCOORDS, pSovereignModel->GetTextureCoordinateHandle())  == 0);
    assert(g_DefaultVertexManager->Create( loader.GetNormals(),   OMEGA_NORMALS,   pSovereignModel->GetNormalHandle() )            == 0);

    // Allow only one set of indices.
    assert( loader.GetIndices().size() == 1 );
    
    // add new index handle to model
    pSovereignModel->AddIndexHandle( INDEX_HANDLE() );
    // Create resource for these indices
    assert( g_DefaultIndexManager->Create( loader.GetIndices()[0], OMEGA_INDICES, pSovereignModel->GetIndexHandles().back()) == 0);
    
    // Create VBO cache for indices.
    rOglRenderer.CommitCache(*loader.GetIndices()[0]);
    // Create VBO cache for vertices.
    rOglRenderer.CommitCache(*loader.GetVertices());

    
    COglTexture *pTexture = rOglRenderer.CreateTexture( "Resources/Textures/omega.tga");
    assert(g_DefaultTextureManager->Create( pTexture, OMEGA_TEXTURE, pSovereignModel->GetTextureHandle()) == 0);
    assert( g_PhoenixModelManager->Create( pSovereignModel, OMEGA_RESOURCE ) == 0);
    
    // Calculate bounding sphere
    m_Sphere = CalculateBoundingSphereTight( *(loader.GetVertices()));

    // Tell loader not to free these resources, they are managed.
    loader.ResetVertices();
    loader.ResetTexCoords();
    loader.ResetNormals();
    loader.ResetIndices();

  }  
  ////////////////////
  /// Releases resources held by this class.
  /// \param rOglRenderer Renderer object for removing caches.
  static void ReleaseResources( COglRenderer & rOglRenderer )
  {
    // Release cache
    CVertexDescriptor *pTemp = g_DefaultVertexManager->GetResource( OMEGA_VERTICES );
    CIndexArray *pIndices = g_DefaultIndexManager->GetResource( OMEGA_INDICES );
    if ( pTemp )
      rOglRenderer.RollbackCache( *pTemp );    

    if ( pIndices )
      rOglRenderer.RollbackCache( *pIndices );    
    
    // release model itself
    g_PhoenixModelManager->Destroy( OMEGA_RESOURCE );
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
    switch( pShip->GetShipClass())
    {
    case SHIP_CLASS_SOVEREIGN:
      static_cast<CSovereignClass *>(pShip)->Update( nTimeMS );
      break;
    case SHIP_CLASS_OMEGA:
      //static_cast<COmegaClass *>(pShip)->Update( nTimeMS );
      break;
    }
    m_pGraph->Update( pShip );
  }
};
/////////////////////////////////////////////////////////////////
void 
DrawSurroundingQuad( COglRenderer * pOglRenderer, CCamera &camera, CGameObject<OBJECT_TYPE> *pObject )
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

  for( unsigned int i=1;i<8;i++)
  {
    vTemp = camera.WorldCoordinatesToScreen( box.GetCorner( (BBOX_CORNER_TYPE)i ));
    for(unsigned int n=0;n<2;n++)
    {
      if ( vTemp[n] > vMax[n] ) vMax[n] = vTemp[n];
      else if ( vTemp[n] < vMin[n] ) vMin[n] = vTemp[n];
    }
    
  }
  

  pOglRenderer->CommitColor( CVector4<unsigned char>(0,255,0,255));
  glPolygonMode( GL_FRONT, GL_LINE );
  glBegin(GL_QUADS);
  glVertex3fv(vMin.GetArray());
    glVertex3fv(CVector3<float>(vMax[0], vMin[1], 0.0f).GetArray()); 
    glVertex3fv(vMax.GetArray());
    glVertex3fv(CVector3<float>(vMin[0], vMax[1], 0.0f).GetArray());
    /*glVertex3f(100,100,0);
    glVertex3f(300,100,0);
    glVertex3f(300,300,0);
    glVertex3f(100,300,0);*/
  glEnd();
  glPolygonMode( GL_FRONT, GL_FILL );
  pOglRenderer->CommitColor( CVector4<unsigned char>(255,255,255,255));

}
enum TRANSFORM_TYPE 
{
  TRANSFORM_SHIP = 0,
  TRANSFORM_EXT = 1
};
/////////////////////////////////////////////////////////////////
class CShipTransform : public CTransformNode<TRANSFORM_TYPE, CHandle<CSpaceShip> >
{
  friend class CGraph<TRANSFORM_TYPE>;
public:
  CShipTransform()
  {
    SetType( TRANSFORM_SHIP );
  }
};
/////////////////////////////////////////////////////////////////
class CExtTransform : public CTransformNode<TRANSFORM_TYPE, CHandle<void> >,
		      public CTransformable
		
{
  friend class CGraph<CSpaceShip>;
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
int main()
{
  
  if ( !CSDLScreen::GetInstance() )
  {
    std::cerr << "Couldn't open screen" << std::endl;
    return 1;
  }
  
  CGraph<TRANSFORM_TYPE> *pTG	   = new CGraph<TRANSFORM_TYPE>();
  CExtTransform *pTGR              = pTG->CreateNode< CExtTransform >();
  CExtTransform *pScale		   = pTG->CreateNode< CExtTransform >();
  CShipTransform *pTSovereign      = pTG->CreateNode< CShipTransform >();
  CShipTransform *pTOmega          = pTG->CreateNode< CShipTransform >();
  CTransformUpdater trUpdater;

  pTGR->AddEdge( pTSovereign );
  pTSovereign->AddEdge( pScale );
  pScale->AddEdge(pTOmega );
  
  CCamera camera;
  camera.SetPosition( 0, 0.0f,75.0f);
  //camera.SetViewport( 480,340, 160, 120 );
  camera.SetViewport( 0,0, 640, 480 );
  camera.SetNearClipping( 0.1f);
  camera.SetFarClipping( 1500.0f );
  camera.SetFieldOfView( 43.0f);

  CCamera camera2;
  camera2.SetPosition( 0, 0.0f, 0.0f);
  camera2.SetViewport( 0,0, 640, 480 );
  camera2.SetNearClipping( -1.0f);
  camera2.SetFarClipping( 10.0f );
  camera2.SetViewOrtho( 0,640, 0, 480);
  //camera2.RotateAroundRight( -90.0 );
  SDL_Event event;
  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
  
  COglRenderer *pOglRenderer = new COglRenderer();
  float afAmbient[4] = { 0.3f, 0.3f, 0.3f, 1.0f };
  glLightModelfv( GL_LIGHT_MODEL_AMBIENT, afAmbient );
  
  CGameObjectOGLAdapter oglAdapter;
  
  Phoenix::Graphics::CRenderQueue<CGameObject<OBJECT_TYPE> *> *pRenderQueue = 
      new Phoenix::Graphics::CRenderQueue<CGameObject<OBJECT_TYPE> *>();

  CSpatialGraph *pSpatialGraph = new CSpatialGraph();
  /////////////////////////////////////////////////////////////////
  /// Allocate resources for ships
  COmegaClass::CreateResources( *pOglRenderer );
  CSovereignClass::CreateResources( *pOglRenderer );
  /////////////////////////////////////////////////////////////////
  /// Create ships 
  COmegaClass *pOmega = new COmegaClass();
  pOmega->GetLocalTransform().SetTranslation(0, -10 ,0);
  pOmega->GetLocalTransform().SetScaling(1.0f);
  pSpatialGraph->InsertObject( pOmega );
  
  CSovereignClass *pSovereign = new CSovereignClass();
  pSovereign->GetLocalTransform().SetTranslation( 10, 0,0);
  pSovereign->GetLocalTransform().SetScaling(3.0f);
  pSpatialGraph->InsertObject( pSovereign );
  cerr << "we're ok" << endl;

  pScale->GetLocalTransform().SetScaling( 0.333f);
  

  /////////////////////////////////////////////////////////////////
  COglTexture *pExplosionTexture = pOglRenderer->CreateTexture("Resources/Textures/explosion.tga");
  CShader *pPsShader = pOglRenderer->CreateShader( "Resources/Shaders/ps_vertex.glsl", "Resources/Shaders/ps_frag.glsl");  
  CFpsCounter fpsCounter;
  fpsCounter.Reset();
  unsigned int nCollected = 0;

  glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, 0 );    
  glLightModeli( GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR );
  SDL_WM_GrabInput(SDL_GRAB_ON);
  SDL_WarpMouse( (int)(camera.GetViewport()[2]*0.5f),  (int)(camera.GetViewport()[3]*0.5f) );
  
  CVector2<int> vMousePos((int)(camera.GetViewport()[2]*0.5f),  (int)(camera.GetViewport()[3]*0.5f)) ;
  CLaserBeam::CreateResources( *pOglRenderer );
  CLaserBeam *pBeam = new CLaserBeam();
  
  pBeam->Initialize( pSovereign->GetWorldTransform().GetTranslation(), pOmega->GetWorldTransform().GetTranslation(), 0.95f );


  //SHIP_HANDLE hEnterprise, hBackgammon;

  g_ShipManager->Create( pSovereign, "Enterprise");
  g_ShipManager->Create( pOmega,     "Backgammon");

  g_ShipManager->AttachHandle( "Enterprise", pTSovereign->GetHandle());
  g_ShipManager->AttachHandle( "Backgammon", pTOmega->GetHandle());
  
  CSpaceShipUpdaterAdapter shipAdapter( pSpatialGraph );
  
  CObjectUpdater<CSpaceShip> shipUpdater;
  shipUpdater.Manage( pTSovereign->GetHandle() );
  shipUpdater.Manage( pTOmega->GetHandle() );


  
  // Clear event queue
  while ( SDL_PollEvent(&event ));
  
  CGamePSObject *pParticleSystem = new CGamePSObject();
  pOglRenderer->CommitState( STATE_FACECULLING );
  
  CTimer timer;
  timer.Reset();
  int bChanged = 0;

  CPlane xzPlane(0,1,0,0);
  
  int bMousePressed = 0;
  while( g_bLoop )
  {
    fpsCounter.Update();
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
	else if ( event.key.keysym.sym == SDLK_RETURN )
	{
	  pParticleSystem->Init( pSovereign->GetWorldTransform().GetTranslation() + pSovereign->GetForwardVector());
	  pBeam->SetEnabled(1);
	}
	 else if ( event.key.keysym.sym == SDLK_RIGHT )
	{
	  pSovereign->GetLocalTransform().SetScaling(pSovereign->GetLocalTransform().GetScaling()-0.1f);
	} 
	else if ( event.key.keysym.sym == SDLK_LEFT )
	{
	  pSovereign->GetLocalTransform().SetScaling(pSovereign->GetLocalTransform().GetScaling()+0.1f);
	}
	break;
      case SDL_MOUSEMOTION:
	{
	  CVector2<int> vMousePosCurrent(event.motion.x, event.motion.y);
	  CVector2<int> vMouseDiff = vMousePos - vMousePosCurrent;
	  
	  
	  //camera.RotateAroundUp(vMouseDiff[0]);
	  //camera.RotateAroundRight(vMouseDiff[1]);	
	  camera.VirtualTrackball( CVector3<float>(0,0,0),
				   vMousePos,
				   vMousePosCurrent );

	  vMousePos = vMousePosCurrent;
	  }
	break;
      default:
	break;
      }
    }

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

    pOglRenderer->ClearBuffer( COLOR_BUFFER );
    pOglRenderer->ClearBuffer( DEPTH_BUFFER );
    pOglRenderer->CommitState( STATE_DEPTH_TEST );

    nCollected = pRenderQueue->CollectObjects( camera, *pSpatialGraph ) ;
    pOglRenderer->CommitCamera( camera );
    pOglRenderer->DisableState( STATE_LIGHTING );
    
    //pOglRenderer->CommitColor( CVector4<unsigned char>(255,255,255,255));
    //pOglRenderer->CommitTransform( pGameobject->GetTransform() );
    //pOglRenderer->CommitModel( *g_PhoenixModelManager->GetResource(gameobject.GetModelHandle()));
    pOglRenderer->CommitShader( NULL );    
    pRenderQueue->Render<CGameObjectOGLAdapter>( *pOglRenderer, oglAdapter );

    pOglRenderer->CommitState( STATE_BLENDING);
    pOglRenderer->DisableState( STATE_DEPTH_WRITE );
    //BLEND_SRC_ONE
    pOglRenderer->CommitBlending( BLEND_SRC_SRC_ALPHA, BLEND_DST_ONE );
    if ( pBeam->IsEnabled())
      pOglRenderer->CommitModel(*pBeam);

    //pOglRenderer->RollbackTransform();
    //pOglRenderer->CommitCamera( camera2);
    //pRenderQueue->Render<CGameObjectOGLAdapter>( *pOglRenderer, oglAdapter );
    //DrawFrustum( camera );

    

    if ( timer.HasPassedMS(5) )
    {
      pParticleSystem->Update(timer.GetPassedTimeMS());
      //pSovereign->Update(timer.GetPassedTimeMS());
      //pSpatialGraph->Update( pSovereign );
      shipUpdater.Update<CSpaceShipUpdaterAdapter>( shipAdapter, timer.GetPassedTimeMS());
      TravelDF<TRANSFORM_TYPE, CTransformUpdater>( static_cast<CGraphNode<TRANSFORM_TYPE> *>(pTGR), &trUpdater );
      timer.Reset();
      pBeam->IncreaseTime( timer.GetPassedTimeMS()*0.001f);
      if ( pBeam->IsEnabled())
      {
	pBeam->Initialize( pSovereign->GetWorldTransform().GetTranslation(), 
			   pOmega->GetWorldTransform().GetTranslation(), 0.95f );
      }

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

    pOglRenderer->DisableState( STATE_BLENDING);
    pOglRenderer->CommitState( STATE_DEPTH_WRITE );
    pOglRenderer->CommitShader( NULL );
    pOglRenderer->CommitCamera( camera2 );

    pOglRenderer->DisableState( STATE_DEPTH_TEST );
    
    pOglRenderer->DisableTexture( 0, pExplosionTexture );

    DrawSurroundingQuad( pOglRenderer, camera, pSovereign);
    
    pOglRenderer->CommitState( STATE_DEPTH_TEST );
    
    pOglRenderer->Finalize();
    pOglRenderer->CommitShader( NULL );    
    
    CSDLScreen::GetInstance()->SwapBuffers();
    fpsCounter++;

    if ( fpsCounter.HasPassedMS(1000) )
    {
      cerr << "FPS: " << fpsCounter << ", with visible objects #"<< nCollected <<endl;
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
  
