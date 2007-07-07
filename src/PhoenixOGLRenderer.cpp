/////////////////////////////////////////////////////////////////
#include <GL/GLee.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <ft2build.h>
#include <list>
#include <iostream>
#include "PhoenixGlobals.h"
/////////////////////////////////////////////////////////////////
using std::cerr;
using std::endl;
#include FT_FREETYPE_H
/////////////////////////////////////////////////////////////////
// globals 
const GLenum g_aGLMultiTexIds[8] = { GL_TEXTURE0_ARB,
				     GL_TEXTURE1_ARB,
				     GL_TEXTURE2_ARB,
				     GL_TEXTURE3_ARB,
				     GL_TEXTURE4_ARB,
				     GL_TEXTURE5_ARB,
				     GL_TEXTURE6_ARB,
				     GL_TEXTURE7_ARB };
/////////////////////////////////////////////////////////////////
COglRendererFontset::COglRendererFontset()
{
  m_nDisplayLists = 0;
  memset( m_ppTextures, 0, Phoenix::Graphics::MAX_FONT_CHARACTERS );
}
/////////////////////////////////////////////////////////////////
COglRendererFontset::~COglRendererFontset()
{
  COglTextureMgr *pOglTextureMgr = COglTextureMgr::GetInstance();
  // delete display lists
  glDeleteLists(m_nDisplayLists, Phoenix::Graphics::MAX_FONT_CHARACTERS);
  // delete textures as well
  for(unsigned int nTex=0;nTex<Phoenix::Graphics::MAX_FONT_CHARACTERS;nTex++)
  {
    if ( m_ppTextures[nTex] != NULL )
    {
      CTexture *pTexture = m_ppTextures[nTex]; 
      pOglTextureMgr->Delete( pTexture );
      m_ppTextures[nTex] = NULL;
    }
  }
}
/////////////////////////////////////////////////////////////////
GLuint &
COglRendererFontset::DisplayList()
{
  return m_nDisplayLists;
}
/////////////////////////////////////////////////////////////////
// Returns the pointer to array of texture pointers
CTexture **
COglRendererFontset::Textures()
{
  return m_ppTextures;
}
/////////////////////////////////////////////////////////////////
COglRendererFeatures::COglRendererFeatures()
{
  Init();
  // Check for required extensions:
  if ( GLEE_ARB_vertex_program	)  m_bARB_vertex_program  = 1;
  if ( GLEE_ARB_vertex_shader	)  m_bARB_vertex_shader   = 1;
  if ( GLEE_ARB_fragment_shader )  m_bARB_fragment_shader = 1;
  if ( GLEE_ARB_shader_objects	)  m_bARB_shader_objects  = 1;
  if ( GLEE_EXT_vertex_array	)  m_bEXT_vertex_array    = 1;
  if ( GLEE_ARB_multitexture	)  m_bARB_multitexture     = 1;

  if ( GLEE_ARB_vertex_buffer_object	) m_bARB_vertex_buffer_object = 1;
  glGetIntegerv( GL_MAX_LIGHTS,            &m_iMaxLights );
  glGetIntegerv( GL_MAX_ELEMENTS_VERTICES, &m_iMaxElementsVertices);
  glGetIntegerv( GL_MAX_ELEMENTS_INDICES,  &m_iMaxElementsIndices);
}
/////////////////////////////////////////////////////////////////
void
COglRendererFeatures::Init()
{
  m_bARB_vertex_program = 0;
  m_bARB_vertex_shader = 0;
  m_bARB_fragment_shader = 0;
  m_bEXT_vertex_array = 0;
  m_bARB_vertex_buffer_object = 0;
  m_bARB_multitexture = 0;
  m_bARB_shader_objects = 0;
  m_iMaxLights = 0;
}
/////////////////////////////////////////////////////////////////
char 
COglRendererFeatures::HasVertexProgram()
{
  return m_bARB_vertex_program;
}
/////////////////////////////////////////////////////////////////
char 
COglRendererFeatures::HasVertexShader()
{
  return m_bARB_vertex_shader;
}
/////////////////////////////////////////////////////////////////
char 
COglRendererFeatures::HasFragmentShader()
{
  return m_bARB_fragment_shader;
}
/////////////////////////////////////////////////////////////////
char 
COglRendererFeatures::HasVertexArray()
{
  return m_bEXT_vertex_array;
}
/////////////////////////////////////////////////////////////////
char 
COglRendererFeatures::HasVertexBufferObject()
{
  return m_bARB_vertex_buffer_object;
}
/////////////////////////////////////////////////////////////////
char 
COglRendererFeatures::HasMultitexture()
{
  return m_bARB_multitexture;
}
/////////////////////////////////////////////////////////////////
char 
COglRendererFeatures::HasShaderObjects()
{
  return m_bARB_shader_objects;
}
/////////////////////////////////////////////////////////////////
int
COglRendererFeatures::GetMaxLights()
{
  return m_iMaxLights;
}
/////////////////////////////////////////////////////////////////
int  
COglRendererFeatures::GetMaxElementsVertices()
{
  return m_iMaxElementsVertices;
}
/////////////////////////////////////////////////////////////////
int  
COglRendererFeatures::GetMaxElementsIndices()
{
  return m_iMaxElementsIndices;
}
/////////////////////////////////////////////////////////////////
std::ostream &operator<<(std::ostream &stream, COglRendererFeatures &obj)
{
  stream << "OpenGL extensions:" << std::endl;
  stream << "------------------" << std::endl;
  stream << "GL_ARB_vertex_program "  << ( obj.HasVertexProgram() ? "YES" : "NO" ) << endl;
  stream << "GL_ARB_vertex_shader "   << ( obj.HasVertexShader() ? "YES" : "NO" ) << endl;
  stream << "GL_ARB_fragment_shader " << ( obj.HasFragmentShader() ? "YES" : "NO" ) << endl;
  stream << "GL_EXT_vertex_array "    << ( obj.HasVertexArray() ? "YES" : "NO" ) << endl;
  stream << "GL_ARB_vertex_buffer_object " << ( obj.HasVertexBufferObject() ? "YES" : "NO" ) << endl;
  stream << "GL_ARB_multitexture " << ( obj.HasMultitexture() ? "YES" : "NO" ) << endl;
  stream << "GL_ARB_shader_objects " << ( obj.HasShaderObjects() ? "YES" : "NO" ) << endl;
  stream << "------------------" << std::endl;
  stream << "OpenGL environment variables:" << std::endl;
  stream << "------------------" << std::endl;
  stream << "GL_MAX_LIGHTS = " << obj.GetMaxLights() << endl;
  stream << "GL_MAX_ELEMENTS_VERTICES = " << obj.GetMaxElementsVertices() << std::endl;
  stream << "GL_MAX_ELEMENTS_INDICES  = " << obj.GetMaxElementsIndices() << std::endl;
  
  return stream;
}
/////////////////////////////////////////////////////////////////
COglRenderer::COglRenderer()
{
  m_pFeatures = new COglRendererFeatures();
  std::cerr <<  "OpenGL information:" << std::endl << *m_pFeatures );
  
  SetRenderPass(OGL_RENDER_PASS_ONLY_OPAQUE);
  m_pCurrentCamera  = NULL;
  m_bClearBuffers   = 1;
  m_sPadding	    = "";
  m_sArrow	    = "|->";

  m_bSphereInitialized = 0;
  m_bOBBInitialized    = 0;
  
  m_nHits = 0;
   
}
/////////////////////////////////////////////////////////////////
COglRenderer::~COglRenderer()
{
  // Delete all registered fontsets
  for( unsigned int nFontset = 0;nFontset<m_vFontsets.size();nFontset++)
  {
    delete m_vFontsets[nFontset];
  }
  m_vFontsets.clear();
  delete m_pFeatures;
}
/////////////////////////////////////////////////////////////////
void
COglRenderer::SetClearBuffers( char bFlag )
{
  m_bClearBuffers = bFlag;
}
/////////////////////////////////////////////////////////////////
Geometry::COrientedBox &
COglRenderer::GetBoundingBox()
{
  return m_obBoundingBox;
}
/////////////////////////////////////////////////////////////////
Geometry::CSphere &
COglRenderer::GetBoundingSphere()
{
  return m_BoundingSphere;
}
/////////////////////////////////////////////////////////////////
int 
COglRenderer::Enter( CGraphNode<NodeType> *pNode )
{

  int			iCulled = 0;

  switch ( m_iRenderPassType ) 
  {
  case OGL_RENDER_PASS_DEBUG_PRINT:
    iCulled = Handle_DebugPrint_RenderPass_Enter( pNode );
    break;
  case OGL_RENDER_PASS_GET_SELECTION:
    // Each new node causes an element to be added into the stack
    m_SelectionStack.push('o');
  case OGL_RENDER_PASS_ONLY_OPAQUE:
  case OGL_RENDER_PASS_ONLY_TRANSPARENT:
    iCulled = Handle_Drawing_RenderPass_Enter( pNode );
    break;
  }
  return iCulled;

}
/////////////////////////////////////////////////////////////////
int 
COglRenderer::Leave( CGraphNode<NodeType> *pNode )
{
  int			iRetval = 0;

  switch ( m_iRenderPassType )
  {

  case OGL_RENDER_PASS_DEBUG_PRINT:
    iRetval = Handle_DebugPrint_RenderPass_Leave( pNode );
    break;
  case OGL_RENDER_PASS_GET_SELECTION:
    // Pop the selection stack and if we have reached back to the
    // node we started from, we can retrieve the number of hits.
    m_SelectionStack.pop();
    if ( m_SelectionStack.empty())
    {
      m_nHits = glRenderMode( GL_RENDER );
    }
  case OGL_RENDER_PASS_ONLY_OPAQUE:
  case OGL_RENDER_PASS_ONLY_TRANSPARENT:
    iRetval = Handle_Drawing_RenderPass_Leave( pNode );
    break;
  }
 
  return iRetval;
}
/////////////////////////////////////////////////////////////////
void 
COglRenderer::ApplyLightParameters( CLightClusterNode *pNode )
{
  
  /////////////////////////////////////////////////////////////////
  CLight *pLight = NULL;
  GLenum iGLLightID = GL_LIGHT0;
  float aLightPosition[4];


  if ( !pNode->IsIlluminationEnabled())
  {
    glDisable(GL_LIGHTING);
    return;
  }
  glEnable(GL_LIGHTING);
  /////////////////////////////////////////////////////////////////
  for(unsigned int nLight=0;nLight<8;nLight++)
  {
    pLight = pNode->GetLight(nLight);
    iGLLightID = GL_LIGHT0+nLight;
    /////////////////////////////////////////////////////////////////
    // Enable the light, if needed.
    if ( pLight == NULL || !pLight->m_bEnabled)
    {
      glDisable(iGLLightID);
      continue;
    }

    /////////////////////////////////////////////////////////////////
    glEnable(iGLLightID);
    CDEBUG( "glEnable( GL_LIGHT" << nLight << ")")
    // set position
    // Convert position into float array of four elements
    aLightPosition[0] = pLight->GetPosition().m_pValues[CVector3::X];
    aLightPosition[1] = pLight->GetPosition().m_pValues[CVector3::Y];
    aLightPosition[2] = pLight->GetPosition().m_pValues[CVector3::Z];
    /////////////////////////////////////////////////////////////////
    switch ( pLight->m_iLightType )
    {
      /////////////////////////////////////////////////////////////////
    case CLight::DIRECTIONAL:

      CDEBUG("/* We have DIRECTIONAL light */");
      /////////////////////////////////////////////////////////////////
      // if we have directional light, the position parameter 
      // actually defines the direction
      aLightPosition[0] = -pLight->m_vDirection[CVector3::X];
      aLightPosition[1] = -pLight->m_vDirection[CVector3::Y];
      aLightPosition[2] = -pLight->m_vDirection[CVector3::Z];
      aLightPosition[3] = 0.0f;
      glLightf(iGLLightID, GL_SPOT_CUTOFF, 180.0);
      /////////////////////////////////////////////////////////////////
      CDEBUG( "glLightf(GL_LIGHT" << nLight << ", GL_SPOT_CUTOFF, 180.0)");
      CDEBUG( "glLightf(GL_LIGHT" << nLight << ", GL_LINEAR_ATTENUATION, 0.0) /* ignored*/");
      CDEBUG( "glLightf(GL_LIGHT" << nLight << ", GL_QUADRATIC_ATTENUATION, 0.0) /* ignored */");
      CDEBUG( "glLightf(GL_LIGHT" << nLight << ", GL_CONSTANT_ATTENUATION, 0.0) /* ignored */");
      break;
      /////////////////////////////////////////////////////////////////
    case CLight::SPOTLIGHT:

      CDEBUG( "/* We have SPOTLIGHT  */" );
      /////////////////////////////////////////////////////////////////
      aLightPosition[3] = 1.0f;
      /////////////////////////////////////////////////////////////////    
      glLightf( iGLLightID, GL_SPOT_CUTOFF,    pLight->m_fSpotAngle);
      glLightfv(iGLLightID, GL_SPOT_DIRECTION, pLight->m_vDirection.m_pValues);
      /////////////////////////////////////////////////////////////////
      CDEBUG("glLightf(GL_LIGHT" << nLight << ", GL_SPOT_CUTOFF, "
		<< pLight->m_fSpotAngle << ")" );
      CDEBUG("glLightf(GL_LIGHT" << nLight << ", GL_SPOT_DIRECTION, "
		<< pLight->m_vDirection << ")" );
      /////////////////////////////////////////////////////////////////
      // Set the attenuation parameters
      glLightf(iGLLightID, GL_LINEAR_ATTENUATION,    pLight->m_fLinearAttenuation);
      glLightf(iGLLightID, GL_QUADRATIC_ATTENUATION, pLight->m_fQuadraticAttenuation);
      glLightf(iGLLightID, GL_CONSTANT_ATTENUATION,  pLight->m_fConstantAttenuation);
      /////////////////////////////////////////////////////////////////
      CDEBUG( "glLightf(GL_LIGHT" << nLight << ", GL_LINEAR_ATTENUATION, "
		 << pLight->m_fLinearAttenuation  << ")" );
      CDEBUG("glLightf(GL_LIGHT" << nLight << ", GL_QUADRATIC_ATTENUATION, "
		<< pLight->m_fQuadraticAttenuation  << ")" );
      CDEBUG( "glLightf(GL_LIGHT" << nLight << ", GL_CONSTANT_ATTENUATION, "
		 << pLight->m_fConstantAttenuation << ")" );

      break;
      /////////////////////////////////////////////////////////////////
    case CLight::POINTLIGHT:

      CDEBUG( "/* We have POINTLIGHT  */" );
      /////////////////////////////////////////////////////////////////
      aLightPosition[3] = 1.0f;
      glLightf(iGLLightID, GL_SPOT_CUTOFF, 180.0f);
      CDEBUG( "glLightf(GL_LIGHT" << nLight << ", GL_SPOT_CUTOFF, 180.0)");
      /////////////////////////////////////////////////////////////////
      // Set the attenuation parameters
      glLightf(iGLLightID, GL_LINEAR_ATTENUATION,    pLight->m_fLinearAttenuation);
      glLightf(iGLLightID, GL_QUADRATIC_ATTENUATION, pLight->m_fQuadraticAttenuation);
      glLightf(iGLLightID, GL_CONSTANT_ATTENUATION,  pLight->m_fConstantAttenuation);
      /////////////////////////////////////////////////////////////////
      CDEBUG("glLightf(GL_LIGHT" << nLight << ", GL_LINEAR_ATTENUATION, "
		<< pLight->m_fLinearAttenuation  << ")" );
      CDEBUG("glLightf(GL_LIGHT" << nLight << ", GL_QUADRATIC_ATTENUATION, "
		<< pLight->m_fQuadraticAttenuation  << ")");
      CDEBUG("glLightf(GL_LIGHT" << nLight << ", GL_CONSTANT_ATTENUATION, "
		<< pLight->m_fConstantAttenuation << ")");
    
      break;
    }
    /////////////////////////////////////////////////////////////////
    // Set the light position
    glLightfv(iGLLightID, GL_POSITION, aLightPosition);
    /////////////////////////////////////////////////////////////////
    // Set the intensity distribution of the light.
    glLightf(iGLLightID, GL_SPOT_EXPONENT, pLight->m_fSpotExponent);
    /////////////////////////////////////////////////////////////////
    // Set the diffuse RGBA intensity 
    glLightfv(iGLLightID, GL_DIFFUSE, pLight->m_vDiffuseColor.m_pValues);
    /////////////////////////////////////////////////////////////////
    // Set the ambient RGBA intensity
    glLightfv(iGLLightID, GL_AMBIENT, pLight->m_vAmbientColor.m_pValues);
    /////////////////////////////////////////////////////////////////
    // Set the specular RGBA intensity
    glLightfv(iGLLightID, GL_SPECULAR, pLight->m_vSpecular.m_pValues);
    
    /////////////////////////////////////////////////////////////////
    // Debug output
    CDEBUG( "glLightf(GL_LIGHT" << nLight << ", GL_POSITION, "
	       << aLightPosition[0] << ","
	       << aLightPosition[1] << ","
	       << aLightPosition[2] << ","
	       << aLightPosition[3] << ")" );
    CDEBUG("glLightfv(GL_LIGHT" << nLight << ", GL_SPECULAR, " 
	      << pLight->m_vSpecular << ")");
    CDEBUG( "glLightf(GL_LIGHT" << nLight << ", GL_SPOT_EXPONENT, "
	       << pLight->m_fSpotExponent << ")" );
    CDEBUG( "glLightfv(GL_LIGHT" << nLight << ", GL_DIFFUSE, "
	       << pLight->m_vDiffuseColor << ")" );
    CDEBUG( "glLightfv(GL_LIGHT" << nLight << ", GL_AMBIENT, "
	       << pLight->m_vAmbientColor << ")");
    /////////////////////////////////////////////////////////////////
  }
  
  
}
/////////////////////////////////////////////////////////////////
void
COglRenderer::ApplyPerspective( CCamera *pCamera )
{  

  m_pCurrentCamera = pCamera;
 
  if ( pCamera == NULL )
  {
    CERR("Can't handle NULL's");
    return;
  }
  
  if ( pCamera->IsProjectionChanged())
  {
    pCamera->UpdateProjection();
  }
  if ( pCamera->IsViewChanged()) 
  {
    pCamera->UpdateView();
  }
  
  int *pViewport = pCamera->GetViewport();
  // Set the viewport
  glViewport(pViewport[0], pViewport[1], pViewport[2], pViewport[3]);
  // Reset the projection matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  ////////////////////
  // Selection occurs only on center of the viewport
  m_Selection.m_nMouseX = pViewport[0] + (unsigned int)(pViewport[2]*0.5);
  m_Selection.m_nMouseY = pViewport[1] + (unsigned int)(pViewport[3]*0.5);
  m_Selection.m_nWidth  = 4;
  m_Selection.m_nHeight = 4;
  
  if ( m_iRenderPassType == OGL_RENDER_PASS_GET_SELECTION )
  {
    gluPickMatrix( m_Selection.m_nMouseX, m_Selection.m_nMouseY,
		   m_Selection.m_nWidth, m_Selection.m_nHeight,
		   pViewport);
  } 
  
  if ( m_pCurrentCamera->IsOrthogonal() )
  {
    float *pOrthoPlanes = m_pCurrentCamera->GetOrthoPlanes();
    // Set the orthogonal mode
    glOrtho( pOrthoPlanes[0],		 pOrthoPlanes[1],
	     pOrthoPlanes[2],		 pOrthoPlanes[3],
	     pCamera->GetNearClipping(), pCamera->GetFarClipping());
    
  } 
  else 
  {
    // Set the perspective
    glMultMatrixf( pCamera->GetProjection().Transposed().GetArray());
    
  }
  // Set mode to Modelview matrix and reset
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glMultMatrixf( pCamera->GetView().Transposed().GetArray());
  
  // Extract frustum
  CMatrix4x4f mProj, mModelv, mClip;
  CPlane tmpPlane;
  
  glGetFloatv( GL_PROJECTION_MATRIX, mProj.GetArray() );
  glGetFloatv( GL_MODELVIEW_MATRIX,  mModelv.GetArray() );
  mClip = mModelv * mProj;
  //cerr << "OGL mv " << mModelv << endl;
  //cerr << "OGL proj " << mProj << endl;
  //cerr << "my mv " << pCamera->GetView() << endl;
  //cerr << "my proj " << pCamera->GetProjection() << endl;
  //exit(1);
  float *pClipArray = mClip.GetArray();
  
  // calculate RIGHT plane
  tmpPlane.m_pValues[CPlane::X] = pClipArray[3]  - pClipArray[0];
  tmpPlane.m_pValues[CPlane::Y] = pClipArray[7]  - pClipArray[4];
  tmpPlane.m_pValues[CPlane::Z] = pClipArray[11] - pClipArray[8];
  tmpPlane.m_pValues[CPlane::D] = pClipArray[15] - pClipArray[12];
  tmpPlane.Normalize();
  pCamera->Frustum().SetPlane( CFrustum::RIGHT, tmpPlane );
  
  // calculate LEFT plane
  tmpPlane.m_pValues[CPlane::X] = pClipArray[3]  + pClipArray[0];
  tmpPlane.m_pValues[CPlane::Y] = pClipArray[7]  + pClipArray[4];
  tmpPlane.m_pValues[CPlane::Z] = pClipArray[11] + pClipArray[8];
  tmpPlane.m_pValues[CPlane::D] = pClipArray[15] + pClipArray[12];
  tmpPlane.Normalize();
  pCamera->Frustum().SetPlane( CFrustum::LEFT, tmpPlane );

  // calculate BOTTOM plane
  tmpPlane.m_pValues[CPlane::X] = pClipArray[ 3] + pClipArray[ 1];
  tmpPlane.m_pValues[CPlane::Y] = pClipArray[ 7] + pClipArray[ 5];
  tmpPlane.m_pValues[CPlane::Z] = pClipArray[11] + pClipArray[ 9];
  tmpPlane.m_pValues[CPlane::D] = pClipArray[15] + pClipArray[13];
  tmpPlane.Normalize();
  pCamera->Frustum().SetPlane( CFrustum::BOTTOM, tmpPlane );

  // calculate TOP plane
  tmpPlane.m_pValues[CPlane::X] = pClipArray[ 3] - pClipArray[ 1];
  tmpPlane.m_pValues[CPlane::Y] = pClipArray[ 7] - pClipArray[ 5];
  tmpPlane.m_pValues[CPlane::Z] = pClipArray[11] - pClipArray[ 9];
  tmpPlane.m_pValues[CPlane::D] = pClipArray[15] - pClipArray[13];
  tmpPlane.Normalize();
  pCamera->Frustum().SetPlane( CFrustum::TOP, tmpPlane );

  // calculate FAR plane
  tmpPlane.m_pValues[CPlane::X] = pClipArray[ 3] - pClipArray[ 2];
  tmpPlane.m_pValues[CPlane::Y] = pClipArray[ 7] - pClipArray[ 6];
  tmpPlane.m_pValues[CPlane::Z] = pClipArray[11] - pClipArray[10];
  tmpPlane.m_pValues[CPlane::D] = pClipArray[15] - pClipArray[14];
  tmpPlane.Normalize();
  pCamera->Frustum().SetPlane( CFrustum::FAR, tmpPlane );
  
  // calculate NEAR plane
  tmpPlane.m_pValues[CPlane::X] = pClipArray[ 3] + pClipArray[ 2];
  tmpPlane.m_pValues[CPlane::Y] = pClipArray[ 7] + pClipArray[ 6];
  tmpPlane.m_pValues[CPlane::Z] = pClipArray[11] + pClipArray[10];
  tmpPlane.m_pValues[CPlane::D] = pClipArray[15] + pClipArray[14];
  tmpPlane.Normalize();
  pCamera->Frustum().SetPlane( CFrustum::NEAR, tmpPlane );
  
  pCamera->CalculateBoundingSphere();
  pCamera->CalculateBoundingCone();

}
/////////////////////////////////////////////////////////////////
void
COglRenderer::DisableMaterial( CMaterial *pMaterial )
{
  // NOP
}
/////////////////////////////////////////////////////////////////
void
COglRenderer::ApplyMaterial( GLenum iType, CMaterial *pMaterial )
{
  
  // Sanity check
  if ( pMaterial == NULL )
  {
    CWARN("Can't handle NULL's");
    return;
  }
  
  switch ( pMaterial->GetCacheMethod())
  {
  case CMaterial::CACHED_IN_DL:
    glCallList( pMaterial->Cache());
    return;
    break;
  case CMaterial::NO_CACHE:
    break;
  case CMaterial::REQUEST_DL_CACHE:
    /* generate list and compile properties in */
    pMaterial->Cache() = glGenLists(1);
    glNewList( pMaterial->Cache(), GL_COMPILE );
    break;
  case CMaterial::REGENERATE_DL_CACHE:

    glDeleteLists(pMaterial->Cache(), 1);
    pMaterial->Cache() = glGenLists(1);
    glNewList( pMaterial->Cache(), GL_COMPILE );
    break;
  case CMaterial::DELETE_DL_CACHE:
    glDeleteLists(pMaterial->Cache(),1);
    pMaterial->SetCacheMethod( CMaterial::NO_CACHE );
    break;
  } 
  
  glMaterialfv( iType, GL_DIFFUSE,   pMaterial->GetDiffuse().GetValues());
  glMaterialfv( iType, GL_AMBIENT,   pMaterial->GetAmbient().GetValues());
  glMaterialfv( iType, GL_SPECULAR,  pMaterial->GetSpecular().GetValues());
  glMaterialfv( iType, GL_EMISSION,  pMaterial->GetEmission().GetValues());
  glMaterialf(  iType, GL_SHININESS, pMaterial->GetShininess()*128.0f);

#ifdef DEBUG

  CLogger::Error() << "Material: " << pMaterial->GetName() << std::endl;
  CLogger::Error() << "glMaterialfv( iType, GL_AMBIENT, "
		      << pMaterial->GetAmbient() 
		      << ")" << std::endl
		      << "glMaterialfv( iType, GL_DIFFUSE, "
		      << pMaterial->GetDiffuse() << ")" << std::endl
		      << "glMaterialfv( iType, GL_SPECULAR, " 
		      << pMaterial->GetSpecular() << ")" << std::endl
		      << "glMaterialfv( iType, GL_EMISSION, " 
		      << pMaterial->GetEmission() << ")" << std::endl
		      << "glMaterialf( iType, GL_SHININESS, " 
		      << pMaterial->GetShininess()*128.0 << " )" << std::endl;
#endif
  
  if ( !glIsEnabled(GL_LIGHTING) )
  {
    glColor4fv(pMaterial->GetDiffuse().GetValues());
  }
  
  switch ( pMaterial->GetCacheMethod())
  {
  case CMaterial::CACHED_IN_DL:
  case CMaterial::NO_CACHE:
  case CMaterial::DELETE_DL_CACHE:
    break;
  case CMaterial::REQUEST_DL_CACHE:
  case CMaterial::REGENERATE_DL_CACHE:
    glEndList();
    pMaterial->SetCacheMethod( CMaterial::CACHED_IN_DL );
    glCallList( pMaterial->Cache());
    break;
  }
}
/////////////////////////////////////////////////////////////////
void
COglRenderer::SetupArrays( CGeometryData *pGeometryData ) 
{
  

  if ( pGeometryData == NULL ) 
  {
    CWARN("Can't handle NULL's");
    return;
  }

  int iNumElements  = pGeometryData->m_iNumElements;
  NormalArray	    aNormals   = pGeometryData->m_pNormals;
  ColorArray	    aColors    = pGeometryData->m_pColors;
  //
  // Disable previous Client States
  //
  glDisableClientState( GL_VERTEX_ARRAY );
  glDisableClientState( GL_NORMAL_ARRAY );
  glDisableClientState( GL_COLOR_ARRAY );
  CDEBUG("arrays disabled"); 
  for(int i =0;i<CMAX_TEXCOORD_ARRAYS;i++)
  {
    CDEBUG("disabling multitexarray"); 
    glClientActiveTextureARB(g_aGLMultiTexIds[i]);
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );
  }
  /////////////////////////////////////////////////////////////////
  // Enable Vertex Coordinate Array
  /////////////////////////////////////////////////////////////////
  if ( iNumElements > 0  ) {
    glEnableClientState( GL_VERTEX_ARRAY );
    // VBOs will be ignored while compiling a display list OR 
    // if they are not supported by HW
    if ( IS_COMPILING_DISPLAY_LIST() || !m_pFeatures->HasVertexBufferObject() )
    {
      glVertexPointer( 3, GL_FLOAT, 0, pGeometryData->m_pVertices);
    } 
    else
    {
      switch( pGeometryData->GetCacheMethod( CGeometryData::VERTEX_ARRAY ))
      {
      case CGeometryData::NO_CACHE:
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
	glVertexPointer( 3, GL_FLOAT, 0, pGeometryData->m_pVertices);
	break;
      case CGeometryData::REQUEST_VBO_CACHE:
	glGenBuffersARB(1, &(pGeometryData->Cache( CGeometryData::VERTEX_ARRAY )));
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, pGeometryData->Cache( CGeometryData::VERTEX_ARRAY));
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(float)*3*pGeometryData->m_iNumElements, pGeometryData->m_pVertices, GL_STATIC_DRAW_ARB);
	////////////////////
	// Prepare for the case if the buffer does not fit.
	if ( glGetError() == GL_OUT_OF_MEMORY )
	{
	  CERR("Couldn't create VBO for vertexbuffer.");
	  glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
	  glDeleteBuffersARB(1, &(pGeometryData->Cache( CGeometryData::VERTEX_ARRAY)));
	  glVertexPointer( 3, GL_FLOAT, 0, pGeometryData->m_pVertices);
	  pGeometryData->Cache( CGeometryData::VERTEX_ARRAY ) = 0;
	  pGeometryData->SetCacheMethod( CGeometryData::VERTEX_ARRAY, CGeometryData::NO_CACHE );
	} 
	else
	{
	  glVertexPointer( 3, GL_FLOAT, 0, 0);
	  pGeometryData->SetCacheMethod( CGeometryData::VERTEX_ARRAY, CGeometryData::CACHED_IN_VBO );
	}
	break;
      case CGeometryData::CACHED_IN_VBO:
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, pGeometryData->Cache(CGeometryData::VERTEX_ARRAY) );
	glVertexPointer( 3, GL_FLOAT, 0, 0);
	break;
      case CGeometryData::REGENERATE_VBO_CACHE:
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, pGeometryData->Cache(CGeometryData::VERTEX_ARRAY) );
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, 0, NULL, GL_STATIC_DRAW_ARB);
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(float)*3*pGeometryData->m_iNumElements, pGeometryData->m_pVertices, GL_STATIC_DRAW_ARB);
	glVertexPointer( 3, GL_FLOAT, 0, 0);
	pGeometryData->SetCacheMethod( CGeometryData::VERTEX_ARRAY, CGeometryData::CACHED_IN_VBO );
	break;
      case CGeometryData::DELETE_VBO_CACHE:
	glDeleteBuffersARB( 1, &(pGeometryData->Cache( CGeometryData::VERTEX_ARRAY )));
	pGeometryData->Cache( CGeometryData::VERTEX_ARRAY ) = 0;
	pGeometryData->SetCacheMethod( CGeometryData::VERTEX_ARRAY, CGeometryData::NO_CACHE );

	break; 
      }
    }
  }
  /////////////////////////////////////////////////////////////////
  // Enable Normal Vector Array
  /////////////////////////////////////////////////////////////////
  if ( aNormals != NULL ) 
  {
    
    glEnableClientState( GL_NORMAL_ARRAY );

    if ( IS_COMPILING_DISPLAY_LIST() || !m_pFeatures->HasVertexBufferObject())
    {
      glNormalPointer( GL_FLOAT, 0, aNormals );
    } 
    else 
    {
      switch( pGeometryData->GetCacheMethod( CGeometryData::NORMAL_ARRAY ))
      {
      case CGeometryData::NO_CACHE:
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
	glNormalPointer( GL_FLOAT, 0, aNormals );
	break;
      case CGeometryData::REQUEST_VBO_CACHE:
	glGenBuffersARB(1, &(pGeometryData->Cache( CGeometryData::NORMAL_ARRAY )));
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, pGeometryData->Cache( CGeometryData::NORMAL_ARRAY));
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(float)*3*pGeometryData->m_iNumElements, aNormals, GL_STATIC_DRAW_ARB);
	if ( glGetError() == GL_OUT_OF_MEMORY )
	{

	  CERR("Couldn't create VBO for normalbuffer.");
	  glDeleteBuffersARB( 1, &(pGeometryData->Cache( CGeometryData::NORMAL_ARRAY )));
	  pGeometryData->Cache( CGeometryData::NORMAL_ARRAY ) = 0;
	  pGeometryData->SetCacheMethod( CGeometryData::NORMAL_ARRAY, CGeometryData::NO_CACHE );
	  glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
	  glNormalPointer( GL_FLOAT, 0, aNormals );
	}
	else
	{
	  glNormalPointer( GL_FLOAT, 0, 0 );
	  pGeometryData->SetCacheMethod( CGeometryData::NORMAL_ARRAY, CGeometryData::CACHED_IN_VBO );
	}
	break;
      case CGeometryData::CACHED_IN_VBO:
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, pGeometryData->Cache(CGeometryData::NORMAL_ARRAY) );
	glNormalPointer( GL_FLOAT, 0, 0 );
	break;
      case CGeometryData::REGENERATE_VBO_CACHE:
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, pGeometryData->Cache(CGeometryData::NORMAL_ARRAY) );
	// clear previous
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, 0, NULL, GL_STATIC_DRAW_ARB);
	// send new ones in
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(float)*3*pGeometryData->m_iNumElements, aNormals, GL_STATIC_DRAW_ARB);
	glNormalPointer( GL_FLOAT, 0, 0 );
	pGeometryData->SetCacheMethod( CGeometryData::NORMAL_ARRAY, CGeometryData::CACHED_IN_VBO );
	break;
      case CGeometryData::DELETE_VBO_CACHE:
	glDeleteBuffersARB( 1, &(pGeometryData->Cache( CGeometryData::NORMAL_ARRAY )));
	pGeometryData->Cache( CGeometryData::NORMAL_ARRAY ) = 0;
	pGeometryData->SetCacheMethod( CGeometryData::NORMAL_ARRAY, CGeometryData::NO_CACHE );

	break; 
      }
    }
  }

  /////////////////////////////////////////////////////////////////
  // Enable color array
  /////////////////////////////////////////////////////////////////
  if ( aColors != NULL )
  {
    
    glEnableClientState( GL_COLOR_ARRAY );
    if ( IS_COMPILING_DISPLAY_LIST() || !m_pFeatures->HasVertexBufferObject() )
    {
      glColorPointer( 4, GL_FLOAT, 0, aColors );
    } 
    else 
    {
      switch( pGeometryData->GetCacheMethod( CGeometryData::COLOR_ARRAY ))
      {
      case CGeometryData::NO_CACHE:
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
	glColorPointer( 4, GL_FLOAT, 0, aColors );
	break;
      case CGeometryData::REQUEST_VBO_CACHE:
	glGenBuffersARB(1, &(pGeometryData->Cache( CGeometryData::COLOR_ARRAY )));
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, pGeometryData->Cache( CGeometryData::COLOR_ARRAY));
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(float)*4*pGeometryData->m_iNumElements, aColors, GL_STATIC_DRAW_ARB);
	if ( glGetError() == GL_OUT_OF_MEMORY )
	{
	  CERR("Couldn't create VBO for colorbuffer.");
	  glDeleteBuffersARB( 1, &(pGeometryData->Cache( CGeometryData::COLOR_ARRAY )));
	  pGeometryData->Cache( CGeometryData::COLOR_ARRAY ) = 0;
	  pGeometryData->SetCacheMethod( CGeometryData::COLOR_ARRAY, CGeometryData::NO_CACHE );
	  glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
	  glColorPointer( 4, GL_FLOAT, 0, aColors );
	}
	else
	{
	  glColorPointer( 4, GL_FLOAT, 0, 0 );
	  pGeometryData->SetCacheMethod( CGeometryData::COLOR_ARRAY, CGeometryData::CACHED_IN_VBO );
	}
	break;
      case CGeometryData::CACHED_IN_VBO:
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, pGeometryData->Cache(CGeometryData::COLOR_ARRAY) );
	glColorPointer( 4, GL_FLOAT, 0, 0 );
	break;
      case CGeometryData::REGENERATE_VBO_CACHE:
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, pGeometryData->Cache(CGeometryData::COLOR_ARRAY) );
	// clear previous
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, 0, NULL, GL_STATIC_DRAW_ARB);
	// send new ones in
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(float)*4*pGeometryData->m_iNumElements, aColors, GL_STATIC_DRAW_ARB);
	glColorPointer( 4, GL_FLOAT, 0, 0 );
	pGeometryData->SetCacheMethod( CGeometryData::COLOR_ARRAY, CGeometryData::CACHED_IN_VBO );
	break;
      case CGeometryData::DELETE_VBO_CACHE:
	glDeleteBuffersARB( 1, &(pGeometryData->Cache( CGeometryData::COLOR_ARRAY )));
	pGeometryData->Cache( CGeometryData::COLOR_ARRAY ) = 0;
	pGeometryData->SetCacheMethod( CGeometryData::COLOR_ARRAY, CGeometryData::NO_CACHE );
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
	break; 
      }
    }
  } 
  /////////////////////////////////////////////////////////////////
  // Enable Texture Coordinate Arrays
  /////////////////////////////////////////////////////////////////
  for(unsigned int i=0;i<CMAX_TEXCOORD_ARRAYS;i++){

    if ( pGeometryData->HasTexCoords(i) ) 
    {
      
      // Here we handle VERTEX ARRAYS, which are handled client-side
      glClientActiveTextureARB(g_aGLMultiTexIds[i]);
      glEnableClientState( GL_TEXTURE_COORD_ARRAY );   

      // determine cache key
      CGeometryData::CacheTarget_t nCacheKey = CGeometryData::TEXCOORD0_ARRAY;
      // Since the key is enumeration, we can do this and be moderately safe.
      nCacheKey= (CGeometryData::CacheTarget_t)(((int)nCacheKey) +i);

      if ( IS_COMPILING_DISPLAY_LIST() || !m_pFeatures->HasVertexBufferObject() )
      {
	glTexCoordPointer( 2, GL_FLOAT, 0, pGeometryData->m_pTexCoords[i] );
      } 
      else 
      {
	switch( pGeometryData->GetCacheMethod( nCacheKey ))
	{
	case CGeometryData::NO_CACHE:
	  glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
	  glTexCoordPointer( 2, GL_FLOAT, 0, pGeometryData->m_pTexCoords[i] );
	  break;
	case CGeometryData::REQUEST_VBO_CACHE:
	  glGenBuffersARB(1, &(pGeometryData->Cache( nCacheKey )));
	  glBindBufferARB( GL_ARRAY_BUFFER_ARB, pGeometryData->Cache( nCacheKey));
	  glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(float)*2*pGeometryData->m_iNumElements, pGeometryData->m_pTexCoords[i], GL_STATIC_DRAW_ARB);
	  if ( glGetError() == GL_OUT_OF_MEMORY )
	  {
	    CERR("Couldn't create VBO for texcoordbuffer " << i);
	    glDeleteBuffersARB( 1, &(pGeometryData->Cache( nCacheKey )));
	    pGeometryData->Cache( nCacheKey ) = 0;
	    pGeometryData->SetCacheMethod( nCacheKey, CGeometryData::NO_CACHE );
	    glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
	    glTexCoordPointer( 2, GL_FLOAT, 0, pGeometryData->m_pTexCoords[i] );
	  }
	  else
	  {
	    glTexCoordPointer( 2, GL_FLOAT, 0, 0 );
	    pGeometryData->SetCacheMethod( nCacheKey, CGeometryData::CACHED_IN_VBO );
	  }
	  break;
	case CGeometryData::CACHED_IN_VBO:
	  glBindBufferARB( GL_ARRAY_BUFFER_ARB, pGeometryData->Cache(nCacheKey) );
	  glTexCoordPointer( 2, GL_FLOAT, 0, 0 );
	  break;
	case CGeometryData::REGENERATE_VBO_CACHE:
	  glBindBufferARB( GL_ARRAY_BUFFER_ARB, pGeometryData->Cache(nCacheKey) );
	  // clear previous
	  glBufferDataARB( GL_ARRAY_BUFFER_ARB, 0, NULL, GL_STATIC_DRAW_ARB);
	  // send new ones in
	  glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(float)*2*pGeometryData->m_iNumElements,  pGeometryData->m_pTexCoords[i], GL_STATIC_DRAW_ARB);
	  glTexCoordPointer( 2, GL_FLOAT, 0, 0 );
	  pGeometryData->SetCacheMethod( nCacheKey, CGeometryData::CACHED_IN_VBO );
	  break;
	case CGeometryData::DELETE_VBO_CACHE:
	  glDeleteBuffersARB( 1, &(pGeometryData->Cache( nCacheKey )));
	  pGeometryData->Cache( nCacheKey ) = 0;
	  pGeometryData->SetCacheMethod( nCacheKey, CGeometryData::NO_CACHE );
	  glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
	  break; 
	}
      }
    }
  }
}
/////////////////////////////////////////////////////////////////
#define DRAW_RENDER_BATCHES() {					     \
        /* Disable IndexBuffer if it is supported, since batching */ \
        /* does not use it. */					     \
	if ( m_pFeatures->HasVertexBufferObject() )                  \
        {		                                             \
	  glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 );	     \
	}                                                            \
        std::list<CIndexBuffer::RenderBatch_t>::iterator it =     \
	  pIndexBuffer->RenderBatches().begin();                     \
	for(;it!=pIndexBuffer->RenderBatches().end();it++)           \
	{                                                            \
	  glDrawRangeElements( iPrimitive, it->m_nStart, it->m_nEnd, \
			       it->m_nCount, GL_UNSIGNED_SHORT,        \
			       it->m_pData);                         \
	}                                                            \
}
/////////////////////////////////////////////////////////////////
void
COglRenderer::DrawPrimitive( CIndexBuffer *pIndexBuffer )
{
  
  // To see later was the number of vertices in primitive valid
  char bValidPrimitive = 0;
  if ( pIndexBuffer == NULL )
  {
    CERR("Can't handle NULL's");
  }

  // To make the compiler happy
  GLenum iPrimitive = GL_TRIANGLES;

  switch ( pIndexBuffer->m_nVerticesInPrimitive ) 
  {
  case 3:
    iPrimitive = GL_TRIANGLES;
    bValidPrimitive = 1;
    break;
  case 4:
    iPrimitive = GL_QUADS;
    bValidPrimitive = 1;
    break;
  default:
    CERR("I don't know how to draw primitive with " 
	      << pIndexBuffer->m_nVerticesInPrimitive 
	      << " vertices ");
    bValidPrimitive = 0;
    return;
  }

  OptimizeVBOBatching(pIndexBuffer);

  // batching is disabled for particlesystem drawing,
  // since elementlist's indices are ordered on update so that  
  // the first m_nDrawIndices indices contain the particles still alive.
  // Optimizing the batches for (almost) every frame might not be what we want,
  // but it should be checked whether it would yield better performance. In the 
  // case of extremely large particlesystems it might do the trick.
  if ( IS_COMPILING_DISPLAY_LIST() || !pIndexBuffer->m_bBatchingEnabled )
  {
    // Disable element index buffer, if it is supported.
    if ( m_pFeatures->HasVertexBufferObject() )
    {
      glBindBuffer( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 );
    }
    glDrawElements( iPrimitive,  pIndexBuffer->m_nDrawIndices, GL_UNSIGNED_SHORT,
		    pIndexBuffer->m_pIndices);    
  } 
  else if ( !m_pFeatures->HasVertexBufferObject() )
  {
    // Draw stuff without buffering, the normal way
    DRAW_RENDER_BATCHES();
  } 
  else 
  {
    switch ( pIndexBuffer->GetCacheMethod()){
   
    case CIndexBuffer::NO_CACHE:
      glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 );
      DRAW_RENDER_BATCHES();
      break;
    case CIndexBuffer::DELETE_VBO_CACHE:
      glDeleteBuffersARB(1, &(pIndexBuffer->Cache()));
      glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 );      
      DRAW_RENDER_BATCHES();
      pIndexBuffer->SetCacheMethod(CIndexBuffer::NO_CACHE);
      break;
    case CIndexBuffer::REQUEST_VBO_CACHE:
      
      glGenBuffersARB( 1, &(pIndexBuffer->Cache()) );
      glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, pIndexBuffer->Cache());
      glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(unsigned int)*pIndexBuffer->m_nNumIndices, 
		       pIndexBuffer->m_pIndices, GL_STATIC_DRAW_ARB);
      ////////////////////
      // Prepare for the case if the buffer does not fit.
      if ( glGetError() == GL_OUT_OF_MEMORY )
      {
	CERR(__PRETTY_FUNCTION__ << "Couldn't create VBO for indexbuffer.");
	glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 );
	glDeleteBuffersARB(1, &(pIndexBuffer->Cache()));
	DRAW_RENDER_BATCHES();
	pIndexBuffer->SetCacheMethod( CIndexBuffer::NO_CACHE );
      }
      else 
      {
	glDrawElements( iPrimitive, pIndexBuffer->m_nDrawIndices,
			GL_UNSIGNED_SHORT, 0);
	pIndexBuffer->SetCacheMethod( CIndexBuffer::CACHED_IN_VBO);
      }
      glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
      break;
    case CIndexBuffer::CACHED_IN_VBO:
      glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, pIndexBuffer->Cache());
      glDrawElements( iPrimitive, 
		      pIndexBuffer->m_nDrawIndices,
		      GL_UNSIGNED_SHORT,
		      0);
      glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
      break;
    default:
      CWARN(__PRETTY_FUNCTION__ << "Unhandled state!");
      break;
    }
  }
}

/////////////////////////////////////////////////////////////////
void
COglRenderer::SetupVertexAttrib( CVertexAttribStruct *pVertexAttrib )
{
  if ( pVertexAttrib == NULL )
  {
    CERR( __PRETTY_FUNCTION__ << "Can't handle NULL's");
    return;
  }
  if ( m_pFeatures->HasVertexProgram() )
  {
    pVertexAttrib->Activate();
  }
  
}
/////////////////////////////////////////////////////////////////
void 
COglRenderer::FinishVertexAttrib( CVertexAttribStruct *pVertexAttrib )
{

  if ( pVertexAttrib == NULL )
  {
    CERR( __PRETTY_FUNCTION__ << "Can't handle NULL's");
    return;
  }
  if ( m_pFeatures->HasVertexProgram())
  {
    pVertexAttrib->Deactivate();
  }
  
}
/////////////////////////////////////////////////////////////////
void 
COglRenderer::Handle_ParticleSystemUpdate( CParticleSystemNode *pNode )
{

  
  // For this method, a camera is required so faces can be aligned correctly
  if ( m_pCurrentCamera != NULL )
  {
    
    /* if there's no particlesystem, updating isn't necessary either */
    CParticleSystemBase *ps = pNode->GetParticleSystem();
    if ( ps == NULL || !ps->IsAlive() ) return;

    CVector3 vX,vY;
    CVector3 vRight   = m_pCurrentCamera->GetRightVector();
    CVector3 vUp      = m_pCurrentCamera->GetUpVector();
    CGeometryData *pGeometryData = NULL;    
    CVector3 pos;
    CVector3 vNormal;

    unsigned int nNumVertices = 3;
    unsigned int nVertIndex = 0;
    unsigned int nVertIndex_By3 = 0;
    unsigned int nVertIndex_p1By3 = 0;
    unsigned int nVertIndex_p2By3 = 0;
    unsigned int nVertIndex_p3By3 = 0;

    float fWidth = 0.0f, fHeight = 0.0f;

    pGeometryData = pNode->GetGeometryData();
    // Let's be sure about that pointer...
    if ( pGeometryData == NULL ) 
    {
      CERR("GeometryData is not initialized!");
      return;      
    }
    // Updating is needed only for the alive particles.
    unsigned int nNumParticles = ps->GetAliveCount();
    const CParticle *pParticles = ps->GetParticles();
    // For each particle do
    for(unsigned int p=0;p<nNumParticles;p++)
    {
      // If the particle isn't alive, there's no need to update its data
      //if ( pParticles[p].m_fEnergy <= 0.0f) continue;
 
      nVertIndex = p* nNumVertices;
      nVertIndex_By3 = nVertIndex * nNumVertices;
      nVertIndex_p1By3 = (nVertIndex + 1) * nNumVertices;
      nVertIndex_p2By3 = (nVertIndex + 2) * nNumVertices;
      nVertIndex_p3By3 = (nVertIndex + 3) * nNumVertices;
      
      
      //CLogger::Error() << "particle " << p << std::endl;

      const CParticle *particle = &(pParticles[p]);
      fWidth  = particle->m_fSize;
      fHeight = particle->m_fSize;
      
      vX = fWidth * vRight;
      //vX.m_pValues[CVector3::X] = fWidth*vRight.m_pValues[CVector3::X];
      //vX.m_pValues[CVector3::Y] = fWidth*vRight.m_pValues[CVector3::Y];
      //vX.m_pValues[CVector3::Z] = fWidth*vRight.m_pValues[CVector3::Z];
      vY = fHeight * vUp;
      //vY.m_pValues[CVector3::X] = fHeight*vUp.m_pValues[CVector3::X];
      //vY.m_pValues[CVector3::Y] = fHeight*vUp.m_pValues[CVector3::Y];
      //vY.m_pValues[CVector3::Z] = fHeight*vUp.m_pValues[CVector3::Z];  
      //vX = CVector3::GetWorldX();
      //vY = CVector3::GetWorldY();

      //
      // Assign values to the vertex buffer
      // pos = particle.m_vPosition - (vX*0.5) - (vY*0.333);
      //
      // Inlining does a little help - optimizing by hand appears to be more efficient
      // 
      //if ( ps->m_ParticleBase.m_iParticlePrimitive == CParticleBase::TRIANGLE ){
      
      //pos.m_pValues[CVector3::X] = particle->m_vPosition.m_pValues[CVector3::X] - vX.m_pValues[CVector3::X] * 0.66f - vY.m_pValues[CVector3::X] * 0.33f;
      //pos.m_pValues[CVector3::Y] = particle->m_vPosition.m_pValues[CVector3::Y] - vX.m_pValues[CVector3::Y] * 0.66f - vY.m_pValues[CVector3::Y] * 0.33f;
      //pos.m_pValues[CVector3::Z] = particle->m_vPosition.m_pValues[CVector3::Z] - vX.m_pValues[CVector3::Z] * 0.66f - vY.m_pValues[CVector3::Z] * 0.33f;

      pos = particle->m_vPosition - vX * 0.66f;
      pos -= (vY * 0.33f);
      /*} else {

       pos.m_pValues[CVector3::X] = particle->m_vPosition.m_pValues[CVector3::X] - vX.m_pValues[CVector3::X] * 0.5 - vY.m_pValues[CVector3::X] * 0.5;
       pos.m_pValues[CVector3::Y] = particle->m_vPosition.m_pValues[CVector3::Y] - vX.m_pValues[CVector3::Y] * 0.5 - vY.m_pValues[CVector3::Y] * 0.5;
       pos.m_pValues[CVector3::Z] = particle->m_vPosition.m_pValues[CVector3::Z] - vX.m_pValues[CVector3::Z] * 0.5 - vY.m_pValues[CVector3::Z] * 0.5;

       }*/
      //pGeometryData->SetVertex(nVertIndex,   pos);
      pGeometryData->m_pVertices[(nVertIndex_By3)]   = pos.m_pValues[CVector3::X];
      pGeometryData->m_pVertices[(nVertIndex_By3)+1] = pos.m_pValues[CVector3::Y];
      pGeometryData->m_pVertices[(nVertIndex_By3)+2] = pos.m_pValues[CVector3::Z];

      // pos += vX;
      pos.m_pValues[CVector3::X] += vX.m_pValues[CVector3::X];
      pos.m_pValues[CVector3::Y] += vX.m_pValues[CVector3::Y];
      pos.m_pValues[CVector3::Z] += vX.m_pValues[CVector3::Z];
      
      //pGeometryData->SetVertex(nVertIndex+1, pos);
      pGeometryData->m_pVertices[(nVertIndex_p1By3)]   = pos.m_pValues[CVector3::X];
      pGeometryData->m_pVertices[(nVertIndex_p1By3)+1] = pos.m_pValues[CVector3::Y];
      pGeometryData->m_pVertices[(nVertIndex_p1By3)+2] = pos.m_pValues[CVector3::Z];
      // pos += vY;
      pos.m_pValues[CVector3::X] += vY.m_pValues[CVector3::X];
      pos.m_pValues[CVector3::Y] += vY.m_pValues[CVector3::Y];
      pos.m_pValues[CVector3::Z] += vY.m_pValues[CVector3::Z];
      
      //pGeometryData->SetVertex(nVertIndex+2, pos);
      pGeometryData->m_pVertices[nVertIndex_p2By3]   = pos.m_pValues[CVector3::X];
      pGeometryData->m_pVertices[nVertIndex_p2By3+1] = pos.m_pValues[CVector3::Y];
      pGeometryData->m_pVertices[nVertIndex_p2By3+2] = pos.m_pValues[CVector3::Z];
      vNormal = m_pCurrentCamera->GetPosition() - particle->m_vPosition;
      //vNormal.m_pValues[CVector3::X] = (m_pCurrentCamera->GetPosition()).m_pValues[CVector3::X] - particle->m_vPosition.m_pValues[CVector3::X];
      //vNormal.m_pValues[CVector3::Y] = (m_pCurrentCamera->GetPosition()).m_pValues[CVector3::Y] - particle->m_vPosition.m_pValues[CVector3::Y];
      //vNormal.m_pValues[CVector3::Z] = (m_pCurrentCamera->GetPosition()).m_pValues[CVector3::Z] - particle->m_vPosition.m_pValues[CVector3::Z];

      //pGeometryData->SetNormal( nVertIndex,   vNormal );
      //pGeometryData->SetNormal( nVertIndex+1, vNormal );
      //pGeometryData->SetNormal( nVertIndex+2, vNormal );

      pGeometryData->m_pNormals[ (nVertIndex_By3)  ] = vNormal.m_pValues[CVector3::X];
      pGeometryData->m_pNormals[ (nVertIndex_By3)+1] = vNormal.m_pValues[CVector3::Y];
      pGeometryData->m_pNormals[ (nVertIndex_By3)+2] = vNormal.m_pValues[CVector3::Z];
      
      pGeometryData->m_pNormals[ (nVertIndex_p1By3)  ] = vNormal.m_pValues[CVector3::X];
      pGeometryData->m_pNormals[ (nVertIndex_p1By3)+1] = vNormal.m_pValues[CVector3::Y];
      pGeometryData->m_pNormals[ (nVertIndex_p1By3)+2] = vNormal.m_pValues[CVector3::Z];

      pGeometryData->m_pNormals[ nVertIndex_p2By3  ] = vNormal.m_pValues[CVector3::X];
      pGeometryData->m_pNormals[ nVertIndex_p2By3+1] = vNormal.m_pValues[CVector3::Y];
      pGeometryData->m_pNormals[ nVertIndex_p2By3+2] = vNormal.m_pValues[CVector3::Z];
      
      pGeometryData->SetColor( p * 3, particle->m_vColor );
      pGeometryData->SetColor( 1 + p * 3, particle->m_vColor );
      pGeometryData->SetColor( 2 + p * 3, particle->m_vColor );
     //  // The fourth vertex will be update only if we have quad
//       if ( ps->m_ParticleBase.m_iParticlePrimitive == CParticleBase::QUAD )
//       {
// 	// pos -= vX;
// 	pos.m_pValues[CVector3::X] -= vX.m_pValues[CVector3::X];
// 	pos.m_pValues[CVector3::Y] -= vX.m_pValues[CVector3::Y];
// 	pos.m_pValues[CVector3::Z] -= vX.m_pValues[CVector3::Z];
// 	// pGeometryData->SetVertex(nVertIndex+3, pos);
// 	pGeometryData->m_pVertices[(nVertIndex_p3By3)]   = pos.m_pValues[CVector3::X];
// 	pGeometryData->m_pVertices[(nVertIndex_p3By3)+1] = pos.m_pValues[CVector3::Y];
// 	pGeometryData->m_pVertices[(nVertIndex_p3By3)+2] = pos.m_pValues[CVector3::Z];      
// 	// pGeometryData->SetNormal( nVertIndex+3, vNormal );    
// 	pGeometryData->m_pNormals[ (nVertIndex_p3By3) ]   = vNormal.m_pValues[CVector3::X];
// 	pGeometryData->m_pNormals[ (nVertIndex_p3By3) +1] = vNormal.m_pValues[CVector3::Y];
// 	pGeometryData->m_pNormals[ (nVertIndex_p3By3) +2] = vNormal.m_pValues[CVector3::Z];

//       }
    }
  }
}
/////////////////////////////////////////////////////////////////
void 
COglRenderer::ApplyGlobalLighting( CGlobalLightingNode *pNode )
{
  // change modes as specificed in node values
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT,      (const float *)pNode->GetGlobalAmbient().GetValues());
  // set two-sided lighting 
  glLightModeli( GL_LIGHT_MODEL_TWO_SIDE,     pNode->IsTwoSidedLightingEnabled());
  // set reflection angle calculation; if true then slower but more accurate
  glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, pNode->IsLocalViewerModeEnabled());
  // set the specular color handling; with  GL_LIGHT_MODEL_LOCAL_VIEWER this makes big difference.
  glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);


}
/////////////////////////////////////////////////////////////////
void
COglRenderer::Finalize()
{
  ////////////////////
  // ATI driver messes up lighting if glTexEnv is set to 
  // GL_REPLACE and not reverted to GL_MODULATE
  ////////////////////
  glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
  
  SDL_GL_SwapBuffers();
}
/////////////////////////////////////////////////////////////////
void
COglRenderer::RenderText( CTextNode *pTextNode )
{
  
  // If fontsets exist, we can do something
  if ( m_vFontsets.size() == 0 )
  {
    std::cerr << DEBUG_HEADER << "Error: No Fontsets created!" << endl;
    return;
  }
  if ( pTextNode->GetString() == NULL )
  {
    CLogger::Error() << DEBUG_HEADER << "I shall not render a NULL string!" << endl;
    return;
  }
  glPushAttrib( GL_LIST_BIT );
  unsigned int nFontsetIndex = pTextNode->GetFontset(); 
  if ( nFontsetIndex < m_vFontsets.size())
  {
    // Activate preferred fontset
    glListBase( m_vFontsets[nFontsetIndex]->DisplayList() );      
  }
  else
  {
    // Resort to default fontset
    glListBase( m_vFontsets[0]->DisplayList());
  }
    
  glPushMatrix();
  glCallLists( strlen(pTextNode->GetString()), GL_UNSIGNED_BYTE, pTextNode->GetString() );
  glPopMatrix();
  
  glPopAttrib();
  
  
}
/////////////////////////////////////////////////////////////////
int 
COglRenderer::Handle_DebugPrint_RenderPass_Enter( CGraphNode<NodeType> *pNode )
{
  int bCulled = 0;
  CLogger::Error() << m_sPadding <<  "|" << std::endl;
  
  switch ( pNode->GetType() )
  {
  case OCTREE:
    CLogger::Error() << m_sPadding <<  m_sArrow <<  "Octree"  << std::endl;
    break;
  case TRANSFORM:
    CLogger::Error() << m_sPadding <<  m_sArrow <<  "Transform"  << std::endl;
    //CLogger::Error() << ((CTransformNode *)pNode)->GetMatrix();
    break;
  case GEOMETRY:
    CLogger::Error() << m_sPadding <<  m_sArrow <<  "Geometry" ;
    break;
  case CAMERA:
    CLogger::Error() << m_sPadding<<  m_sArrow <<  "Camera"  << endl;
    //CLogger::Error() << ((CCameraNode *)pNode)->GetCamera()->GetView();
    break;
  case GROUP:
    CLogger::Error() << m_sPadding <<  m_sArrow <<  "Group" ;
    break;
  case LIGHT_CLUSTER:
    CLogger::Error() << m_sPadding <<  m_sArrow <<  "LightCluster" ;
    break;
  case FRONT_MATERIAL:
    CLogger::Error() << m_sPadding <<  m_sArrow <<  "FrontMaterial" ;
    break;
  case BACK_MATERIAL:
    CLogger::Error() << m_sPadding <<  m_sArrow <<  "BackMaterial" ;
    break;
  case INDEXBUFFER:
    CLogger::Error() << m_sPadding <<  m_sArrow <<  "IndexBuffer" ;
    break;
  case RENDERSTATE_CULLING:
    CLogger::Error() << m_sPadding <<  m_sArrow <<  "RenderStateCulling" ;
    break;
  case SHADER:
    CLogger::Error() << m_sPadding <<  m_sArrow <<  "Shader" ;
    break;
  case VERTEXATTRIB:
    CLogger::Error() << m_sPadding <<  m_sArrow <<  "VertexAttrib" ;
    break;
  case GLOBAL_LIGHTING:
    CLogger::Error() << m_sPadding <<  m_sArrow <<  "GlobalLighting" ;
    break;
  case PARTICLESYSTEM:
    CLogger::Error() << m_sPadding <<  m_sArrow <<  "ParticleSystem" ;
    break;
  case CACHE:
    CLogger::Error() << m_sPadding <<  m_sArrow <<  "Display list" ;
    break;
  case SELECTION:
    CLogger::Error() << m_sPadding <<  m_sArrow <<  "Selection name" ;
    break;
  case TEXT:
    CLogger::Error() << m_sPadding <<  m_sArrow <<  "Text string" ;
    break;
  case COLOR:
    CLogger::Error() << m_sPadding <<  m_sArrow <<  "Color" ;
    break;
  case ALPHA_TEST:
    CLogger::Error() << m_sPadding <<  m_sArrow <<  "AlphaTest" ;
    break;
  case RENDERSTATE_BLEND:
    CLogger::Error() << m_sPadding <<  m_sArrow <<  "BlendingState" ;
    break;
  case RENDERSTATE_DEPTHBUFFER:
    CLogger::Error() << m_sPadding <<  m_sArrow <<  "DepthBufferState" ;
    break;
  case RENDERSTATE_STACK:
    CLogger::Error() << m_sPadding <<  m_sArrow <<  "RenderStateStack" ;
    break;
  case TEXTURE:  
    CLogger::Error() << m_sPadding <<  m_sArrow <<  "Texture" ;
    break;
  case SCISSOR:
    CLogger::Error() << m_sPadding <<  m_sArrow <<  "ScissorTest" ;
    break;
  case CLEARBUFFER:
    CLogger::Error() << m_sPadding <<  m_sArrow <<  "ClearBuffer" ;
    break;
  case SHADEMODEL:
    CLogger::Error() << m_sPadding <<  m_sArrow <<  "ShadeModel" ;
    break;
  case POLYLINE:
    CLogger::Error() << m_sPadding <<  m_sArrow <<  "PolyLine" ;
    break;
  case KDOP:
    CLogger::Error() << m_sPadding <<  m_sArrow <<  "KDOP" ;
    break;
  case UNDEFINED:
    CLogger::Error() << m_sPadding <<  m_sArrow <<  "Undefined" ;
    break;
  case UTIL_SPHERE:
    CLogger::Error() << m_sPadding <<  m_sArrow <<  "Sphere" ;
    break;
  case UTIL_BOX:
    CLogger::Error() << m_sPadding <<  m_sArrow <<  "Box" ;
    break;
  case UTIL_CONE:
    CLogger::Error() << m_sPadding <<  m_sArrow <<  "Cone" ;
    break;
  case UTIL_LINESTRIP:
    CLogger::Error() << m_sPadding <<  m_sArrow <<  "LineStrip" ;
    break;
  case ROTATION:
    CLogger::Error() << m_sPadding <<  m_sArrow <<  "Rotation" ;
    break;
  case TRANSLATION:
    CLogger::Error() << m_sPadding <<  m_sArrow <<  "Translation" ;
    break;
  case ROOT:
    CLogger::Error() << m_sPadding <<  m_sArrow <<  "RenderRoot" ;
    break;
  case UTIL_QUAD:  
    CLogger::Error() << m_sPadding <<  m_sArrow <<  "UtilQuad" ;
    break;
  case POLYGONMODE:  
    CLogger::Error() << m_sPadding <<  m_sArrow <<  "PolygonMode" ;
    break;
  }
  CLogger::Error() << "(" << pNode << ")" << endl;
  
  m_sPadding.append(1,' ');
  m_sPadding.append(2,' ');
  return bCulled;
}
/////////////////////////////////////////////////////////////////
int 
COglRenderer::Handle_DebugPrint_RenderPass_Leave( CGraphNode<NodeType> *pNode )
{
  int iRetval = 0;
  m_sPadding.erase(0,3);
  return iRetval;
}
/////////////////////////////////////////////////////////////////
int
COglRenderer::HandleOglCache_Enter( COglCacheNode *pNode )
{

  
  int bCulled = 0;
 
  if ( pNode == NULL ){
    CLogger::Error() << "Can't handle NULLs!" << std::endl;
    return 0;
  }

  COglCache *pCache = pNode->GetCache();

  if ( pCache == NULL )
  {
    CLogger::Error() << "Can't handle NULL caches." << std::endl;
    return 0;
  }
  switch ( pCache->GetCacheMethod())
  {
  case COglCache::NO_CACHE:
    bCulled = 0;
    break;
  case COglCache::CACHED_IN_DL:
    glCallList( pCache->Cache());
    bCulled = 1; 
    break;
  case COglCache::REGENERATE_DL_CACHE:
    if ( !IS_COMPILING_DISPLAY_LIST()){
      glDeleteLists(pCache->Cache(), 1);
      break;
    } 
  case COglCache::REQUEST_DL_CACHE:
    if ( IS_COMPILING_DISPLAY_LIST())
    {
      break;
    }

    pCache->Cache()  = glGenLists( 1 );
    if ( pCache->Cache() == 0 )
    {
      CLogger::Error() << "Couldn't create display list!" << std::endl;
      
      pCache->SetCacheMethod( COglCache::NO_CACHE );
    } 
    else 
    {
      glNewList( pCache->Cache(), GL_COMPILE );
    }
    bCulled = 0;
    m_DisplayListStack.push('o');
    break;
  case COglCache::DELETE_DL_CACHE:
    if ( !IS_COMPILING_DISPLAY_LIST()){
      glDeleteLists(pCache->Cache(), 1);
      pCache->SetCacheMethod( COglCache::NO_CACHE );
    }
    break;
  }
  
  
  return bCulled;
}
/////////////////////////////////////////////////////////////////
int 
COglRenderer::HandleOglCache_Leave( COglCacheNode *pNode )
{
  int bRetval = 0;
  if ( pNode == NULL )
  {
    CWARN("Can't handle NULLs!");
    return 0;
  }

  COglCache *pCache = pNode->GetCache();

  if ( pCache == NULL )
  {
    CWARN("Can't handle NULL caches.");
    return 0;
  }
  switch ( pCache->GetCacheMethod())
  {
  case COglCache::NO_CACHE:
  case COglCache::CACHED_IN_DL:
  case COglCache::DELETE_DL_CACHE:
    break;
  case COglCache::REQUEST_DL_CACHE:
  case COglCache::REGENERATE_DL_CACHE:
    // end the GL_COMPILE
    glEndList();
    CDEBUG("glEndList( );");
    // Change cache message
    pCache->SetCacheMethod( COglCache::CACHED_IN_DL );
    // Call list so everything will be drawn also in this pass
    CDEBUG("glCallList( " << pCache->Cache() << ");");
    glCallList( pCache->Cache());
    m_DisplayListStack.pop();
    break;
  }
  return bRetval;
}
/////////////////////////////////////////////////////////////////
/// This method checks every face and determines in which batch 
/// the indices will belong to.
/// The size of the batch depends on hardware limits, and the optimization
/// will occur only once.
void 
COglRenderer::OptimizeVBOBatching( CIndexBuffer *pIndexBuffer )
{
  
  if ( pIndexBuffer == NULL )
  {
    CERR( __PRETTY_FUNCTION__ << "Can't handle NULL's.");
    return;
  }
  if ( !pIndexBuffer->RenderBatches().empty() || 
       !pIndexBuffer->m_bBatchingEnabled )
  {
    return;
  }
  unsigned int nTotalVertices = pIndexBuffer->m_nNumIndices;
  unsigned int nNumFaceVertices = pIndexBuffer->m_nVerticesInPrimitive;
  unsigned int nFaces = nTotalVertices / nNumFaceVertices;

  unsigned int nIndexCount   = 0;

  unsigned int nCurrentStart = pIndexBuffer->m_pIndices[0];
  unsigned int nCurrentEnd   = pIndexBuffer->m_pIndices[0];
  unsigned int nCurrentIndex = 0;
  unsigned int nTmpMinIndex  = 0;
  unsigned int nTmpMaxIndex  = 0;
  unsigned int nUnbatchedFaces = nFaces;
  unsigned short *pData = NULL;
  // while there remains unbatched faces do

  CIndexBuffer::RenderBatch_t renderBatch;

  nIndexCount  = 0;  
  // Clear previous batches, if there are any
  pIndexBuffer->RenderBatches().clear();

  while( nUnbatchedFaces > 0 )
  {
    nTmpMinIndex = nCurrentStart;
    nTmpMaxIndex = nCurrentEnd;
    // calculate new min and max index in the current face
    for(unsigned int nVert=0;nVert<nNumFaceVertices;nVert++)
    {
      STORE_MAX_MIN( pIndexBuffer->m_pIndices[nCurrentIndex+nVert], 
		     nTmpMaxIndex, nTmpMinIndex );
    }
    // This is for safety, each batch WILL have at least one face
    // although the vertex index range might be too large.
    if ( nIndexCount == 0 ) 
    {
        pData = &(pIndexBuffer->m_pIndices[nCurrentIndex]);

	nCurrentStart = nTmpMinIndex;
	nCurrentEnd   = nTmpMaxIndex;
	nUnbatchedFaces--;

	nIndexCount  += nNumFaceVertices;
	nCurrentIndex += nNumFaceVertices;

    } else {

      // if maximum vertices or index count is reached
      if ( (nIndexCount+nNumFaceVertices  > (unsigned int)m_pFeatures->GetMaxElementsIndices() ) ||
	   (nTmpMaxIndex-nTmpMinIndex + 1 > (unsigned int)m_pFeatures->GetMaxElementsVertices())){

	//cerr << "OptVBObatch: " << nIndexCount+nNumFaceVertices << " > " << m_pFeatures->GetMaxElementsIndices() << endl;
	//cerr << "OptVBObatch2: " << nTmpMaxIndex-nTmpMinIndex + 1 << " > " << m_pFeatures->GetMaxElementsVertices() << endl;
	// save batch data into IndexBuffer
	renderBatch.m_nStart = nCurrentStart;
	renderBatch.m_nEnd   = nCurrentEnd;
	renderBatch.m_nCount = nIndexCount;
	renderBatch.m_pData = pData;
	
	pIndexBuffer->RenderBatches().push_back( renderBatch );
	nIndexCount  = 0;	
	// Change index to next
	nCurrentStart = ++nCurrentEnd;
      } 
      else 
      {
	nIndexCount   += nNumFaceVertices;
	nCurrentIndex += nNumFaceVertices;
	nCurrentStart = nTmpMinIndex;
	nCurrentEnd   = nTmpMaxIndex;
	nUnbatchedFaces--;
      }
    }
  }

  // save final batch data into IndexBuffer
  renderBatch.m_nStart = nCurrentStart;
  renderBatch.m_nEnd   = nCurrentEnd;
  renderBatch.m_nCount = nIndexCount;
  renderBatch.m_pData = pData;
  
  pIndexBuffer->RenderBatches().push_back( renderBatch );
  CDEBUG("Renderbatches have been issued, size: " 
	    << pIndexBuffer->RenderBatches().size());

  
}
/////////////////////////////////////////////////////////////////
/// Handles the render passes of the normal drawing process.
int 
COglRenderer::Handle_Drawing_RenderPass_Enter( CGraphNode<NodeType> *pNode )
{
  
  int iCulled = 0;
  GLUquadric *quadric = NULL;
  
  float *pTmp = NULL;
  
  if ( pNode == NULL )
  {
    CDEBUG("Can't handle NULL's!");
    iCulled = 1;
  } 
  else 
  {
    ////////////////////
    // The normal rendering pass
    switch ( pNode->GetType() )
    {
    case OCTREE:
      DrawOctree( static_cast<COctreeNode *>(pNode)->GetOctree());
      break;
    case POLYLINE:
      DrawPolyline( static_cast<CPolyLineNode *>(pNode) );
      break;
    case TEXTURE:
      iCulled = HandleTexture( static_cast<CTextureNode *>(pNode) );
      break;
    case TRANSFORM:
      glPushMatrix();
      glMultMatrixf( (static_cast<CTransformNode *>(pNode))->GetMatrix().Transposed().GetArray()) ;
      break;
    case GEOMETRY: 
      SetupArrays( ( static_cast<CGeometryNode *>(pNode))->GetGeometryDataWorld() );
      break;
    case CAMERA:
      ApplyPerspective( ( static_cast<CCameraNode *>(pNode))->GetCamera());
      break;
    case GROUP:
      break;
    case LIGHT_CLUSTER: 
      ApplyLightParameters( static_cast<CLightClusterNode *>(pNode) );
      break;
    case POLYGONMODE:
      {
	switch(static_cast<CPolygonModeNode *>(pNode)->GetFrontFaceMode())
	{
	case CPolygonModeNode::FILL:
	  glPolygonMode( GL_FRONT, GL_FILL );
	  break;  
	case CPolygonModeNode::LINE:
	  glPolygonMode( GL_FRONT, GL_LINE );
	  break;  
	case CPolygonModeNode::POINT:
	  glPolygonMode( GL_FRONT, GL_POINT );
	  break;  
	}
	switch(static_cast<CPolygonModeNode *>(pNode)->GetBackFaceMode())
	{
	case CPolygonModeNode::FILL:
	  glPolygonMode( GL_BACK, GL_FILL );
	  break;  
	case CPolygonModeNode::LINE:
	  glPolygonMode( GL_BACK, GL_LINE );
	  break;  
	case CPolygonModeNode::POINT:
	  glPolygonMode( GL_BACK, GL_POINT );
	  break;  
	}
      }
      break;
    case FRONT_MATERIAL:
      {
 	CMaterial *pMat = ( static_cast<CMaterialNode *>(pNode))->GetMaterial();

 	if ( ( pMat->IsTransparent() && m_iRenderPassType == OGL_RENDER_PASS_ONLY_TRANSPARENT) ||
 	     (!pMat->IsTransparent() && m_iRenderPassType == OGL_RENDER_PASS_ONLY_OPAQUE) ||
 	     m_iRenderPassType == OGL_RENDER_PASS_GET_SELECTION){
 	  ApplyMaterial( GL_FRONT, pMat);
 	} else {
 	  iCulled = 1;
 	}
	
      }
      break;    
    case BACK_MATERIAL:
      {
	CMaterial *pMat = ( static_cast<CMaterialNode *>(pNode))->GetMaterial();
 	if ( ( pMat->IsTransparent() && m_iRenderPassType == OGL_RENDER_PASS_ONLY_TRANSPARENT) ||
 	     (!pMat->IsTransparent() && m_iRenderPassType == OGL_RENDER_PASS_ONLY_OPAQUE) ||
 	     m_iRenderPassType == OGL_RENDER_PASS_GET_SELECTION){
	  ApplyMaterial( GL_BACK, pMat);
 	} else {
 	  iCulled = 1;
 	}
      }

      break;
    case INDEXBUFFER:
      // Retrieve the IndexBuffer from the manager and pass it 
      DrawPrimitive( ( static_cast<CIndexBufferNode *>(pNode))->GetIndexBuffer());
      break;
    case RENDERSTATE_CULLING:
      Handle_Draw_CullingState( static_cast<CCullingStateNode *>(pNode));
      break;
    case RENDERSTATE_STACK:
      // Stores the defined attribute group
      HandleStackPush( static_cast<CRenderStateStackNode *>(pNode));
      break;
    case CLEARBUFFER:
      {
	
	CClearBufferNode *pClearBuf = static_cast<CClearBufferNode *>(pNode);
	if ( m_bClearBuffers )
	{
	  CColor &Color = pClearBuf->ClearColor();
	  glClearColor( Color[CColor::R],
			Color[CColor::G],
			Color[CColor::B],
			Color[CColor::A]);
	  
	  GLenum eClearBuffers = 0;
	  // Depth mask clearing requires that the glDepthMask is set to GL_TRUE.
	  if ( pClearBuf->IsDepthBufferCleared()  ) 
	  {
	    eClearBuffers |= GL_DEPTH_BUFFER_BIT;
	    /// The depth buffer will not be updated 
	    /// if the depth buffer writing is disabled.
	    glDepthMask( GL_TRUE );
	  }
	  if ( pClearBuf->IsColorBufferCleared()  ) eClearBuffers |= GL_COLOR_BUFFER_BIT;
	  if ( pClearBuf->IsStencilBufferCleared()) eClearBuffers |= GL_STENCIL_BUFFER_BIT;
	  if ( pClearBuf->IsAccumBufferCleared()  ) eClearBuffers |= GL_ACCUM_BUFFER_BIT;
	  
	  glClear( eClearBuffers );

	}
      }
      break;
    case SHADEMODEL:
      // Change the shade model
      switch( ( static_cast<CShadeModelNode *>(pNode))->GetShadeModel())
      {
      case CShadeModelNode::SMOOTH:
	glShadeModel(GL_SMOOTH);
	CDEBUG("glShadeModel(GL_SMOOTH);");
	break;
      case CShadeModelNode::FLAT:
	glShadeModel(GL_FLAT);
	CDEBUG("glShadeModel(GL_FLAT);");
	break;
      }
      
      break;
    case SHADER:
      {
	// Check for shader support and skip node if so
	if ( !m_pFeatures->HasVertexProgram()) break;

	CShaderNode *pShaderNode = static_cast<CShaderNode *>(pNode);
	COglShaderProgram *pShaderProgram = static_cast<COglShaderProgram *>(pShaderNode->GetShaderProgram());
      
	glPushAttrib( GL_LIGHTING_BIT );
	if ( pShaderNode->IsEnabled() )
	{
	  glDisable(GL_LIGHTING);
	  glUseProgram( pShaderProgram->GetProgramId() );
	  pShaderProgram->SetUniformParameters();
	}
	
      }
      break;  
    case VERTEXATTRIB:

      SetupVertexAttrib(( static_cast<CVertexAttribNode *>(pNode))->GetVertexAttrib());

      break;
    case GLOBAL_LIGHTING:
      
      ApplyGlobalLighting( static_cast<CGlobalLightingNode *>(pNode));
    
      break;
    case PARTICLESYSTEM:
      
      Handle_ParticleSystemUpdate( static_cast<CParticleSystemNode *>(pNode) );
      SetupArrays( static_cast<CParticleSystemNode *>(pNode)->GetGeometryData() );
      
      break;
    case UTIL_SPHERE:
      {quadric = gluNewQuadric();
      CUtilSphereNode *pSphere = static_cast<CUtilSphereNode *>(pNode);

      // Set the drawstyle
      switch ( pSphere->GetDrawMode())
      {
      case CUtilSphereNode::WIREFRAME:
	gluQuadricDrawStyle( quadric, GLU_LINE );
	break;
      case CUtilSphereNode::SILHOUETTE:
	gluQuadricDrawStyle( quadric, GLU_SILHOUETTE );
	break;
      default:
	gluQuadricDrawStyle( quadric, GLU_FILL );
	break;
      }
      // This sphere will never have lighting
      glPushAttrib(GL_ENABLE_BIT);
      //glDisable(GL_CULL_FACE);
      //glDisable(GL_DEPTH_TEST);
      glPushMatrix();
      glDisable(GL_LIGHTING);
      glColor4fv( pSphere->GetColor().GetValues());
      glTranslatef( pSphere->GetPosition().m_pValues[CVector3::X],
		    pSphere->GetPosition().m_pValues[CVector3::Y],
		    pSphere->GetPosition().m_pValues[CVector3::Z] );
    
      gluSphere(quadric,pSphere->GetRadius(), 16,16 );
    
      glPopMatrix();
      glPopAttrib();
      // cleanup
      gluDeleteQuadric(quadric);
      }
      break;
    case CACHE:
      iCulled = HandleOglCache_Enter( static_cast<COglCacheNode *>(pNode));
      break;
    case SELECTION:
      iCulled = 0;
      SetSelectionName( static_cast<CSelectionNameNode *>(pNode));
      break;
    case TEXT:
      iCulled = 0;
      RenderText( static_cast<CTextNode *>(pNode) );
      break;
    case COLOR:
      glColor4fv( ( static_cast<CColorNode *>(pNode))->m_pValues);
      iCulled = 0;
      break;
    case RENDERSTATE_DEPTHBUFFER:
      // Enable/disable depth test
      if ( static_cast<CDepthBufferStateNode *>(pNode)->IsDepthTest() )   
      {	
	glEnable(GL_DEPTH_TEST);  
      } 
      else
      {	
	glDisable(GL_DEPTH_TEST); 
      }
      // Enable disable writing into the depth buffer
      if ( static_cast<CDepthBufferStateNode *>(pNode)->IsDepthBufferWriting() )   
      { 
	glDepthMask( GL_TRUE  ); 
      }
      else 
      {
	glDepthMask( GL_FALSE ); 
      }
      break;
    case ALPHA_TEST:
      {
	CAlphaTestNode *pAlphaTest = static_cast<CAlphaTestNode *>(pNode);
	if ( pAlphaTest->IsEnabled())
	{
	  glEnable(GL_ALPHA_TEST);
	}
	else
	{
	  glDisable(GL_ALPHA_TEST);
	}
	switch( pAlphaTest->GetFunc())
	{
	case AlphaTest::ALWAYS:
	  glAlphaFunc(GL_ALWAYS, pAlphaTest->GetReference());
	  break;
	case AlphaTest::NEVER:
	  glAlphaFunc(GL_NEVER, pAlphaTest->GetReference());
	  break;
	case AlphaTest::LESS:
	  glAlphaFunc(GL_LESS, pAlphaTest->GetReference());
	  break;
	case AlphaTest::LEQUAL:
	  glAlphaFunc( GL_LEQUAL, pAlphaTest->GetReference() );
	  break;
	case AlphaTest::EQUAL:
	  glAlphaFunc( GL_EQUAL, pAlphaTest->GetReference() );
	  break;
	case AlphaTest::GEQUAL:
	  glAlphaFunc( GL_GEQUAL, pAlphaTest->GetReference() );
	  break;
	case AlphaTest::GREATER:
	  glAlphaFunc( GL_GREATER, pAlphaTest->GetReference() );
	  break;
	case AlphaTest::NOTEQUAL:
	  glAlphaFunc( GL_NOTEQUAL, pAlphaTest->GetReference() );
	  break;
	}
      }
      break;
    case RENDERSTATE_BLEND:
      {
	CBlendingStateNode *pBlendState = static_cast<CBlendingStateNode *>(pNode);
	if ( pBlendState->IsEnabled())
	{
	  glEnable(GL_BLEND);
	  glBlendFunc(pBlendState->GetSrcOp(), pBlendState->GetDestOp());
	}
	else
	  glDisable(GL_BLEND);
	iCulled = 0;
      }
      break;
    
    case SCISSOR:
      {
	CScissorTestNode *pScissorTestNode = static_cast<CScissorTestNode *>(pNode);
	
	if ( pScissorTestNode->IsEnabled())
	{
	  glEnable(GL_SCISSOR_TEST);
	  glScissor( pScissorTestNode->GetX(),
		     pScissorTestNode->GetY(),
		     pScissorTestNode->GetWidth(),
		     pScissorTestNode->GetHeight() );
	}
	else
	{
	  glDisable(GL_SCISSOR_TEST);
	}
      }
      break;
    case KDOP:
      {
	list<CVector3>		lstPoints;
	list<CVector3>::iterator	pointIt;
	list<CPlane>			lstTmpPlanes;

	CKDopUtilNode *pKDopUtil = static_cast<CKDopUtilNode *>(pNode);
	//CLogger::Error() << "Rendering KDOP " << endl << pKDopUtil->GetKDOP() << endl;	
	
	if ( pKDopUtil->GetKDOP().GetNumPlanes() > 2  ) 
	{
	  lstTmpPlanes = pKDopUtil->GetKDOP().Planes();
	  lstTmpPlanes.reverse();
	  list<CPlane>::iterator planeIt = lstTmpPlanes.begin();

	  CPlane far = *planeIt;
	  planeIt++;
	  CPlane near = *planeIt;
	  
	  lstTmpPlanes.pop_front();
	  lstTmpPlanes.pop_front();
	  
	  planeIt = lstTmpPlanes.begin();

	  // Calculate corners
	  for(; planeIt != lstTmpPlanes.end();planeIt++)
	  {
	    CPlane plane1 = *planeIt;
	    list<CPlane>::iterator planeItNext = planeIt;
	    planeItNext++;

	    if ( planeItNext == lstTmpPlanes.end())
	    {
	      planeItNext = lstTmpPlanes.begin();
	    }

	    CPlane plane2 = *planeItNext;
	    CVector3 vTmp1;
	    
	    if ( Geometry::PlaneIntersectionPoint( plane1, plane2, near, vTmp1 ) != Geometry::POINT )
	    {
	      
	      CERR("HELL BREAKS LOOSE, kDop planes do not intersect!!! (near)");
	      //abort();
	    } 
	    else 
	    {
	      lstPoints.push_back( vTmp1 );
	    }
	    
	    if ( Geometry::PlaneIntersectionPoint( plane1, plane2, far, vTmp1 ) != Geometry::POINT )
	    {
	      CERR("HELL BREAKS LOOSE, kDop planes do not intersect (far)!!!");
	      //abort();
	    } else {
	      lstPoints.push_back( vTmp1 );
	    }
	  }

	  pointIt = lstPoints.begin();
	  // Draw
	  
	  glPushAttrib( GL_CURRENT_BIT | GL_ENABLE_BIT | GL_POLYGON_BIT );
	  glDisable(GL_LIGHTING);
	  glColor3f(1.0, 1.0, 1.0);
	  glDisable(GL_CULL_FACE);
	  glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	  glBegin( GL_QUAD_STRIP );
	  //CLogger::Error() << "KDOP corner winners are: " << endl;
	  for(;pointIt!=lstPoints.end();pointIt++)
	  {
	    //CLogger::Error() << (*pointIt) << endl;
	    glVertex3fv( (*pointIt).m_pValues );
	  }
	  glEnd();

	  glPopAttrib();
	}
	
      }
      break;
    case ROTATION:
      glPushMatrix();
      // we need to transpose the matrix
      // before passing the array to glMultMatrixf
      glMultMatrixf( (static_cast<CRotationNode *>(pNode))->ToMatrix().Transposed().GetArray());            
      break;
    case TRANSLATION:
      // Store modelview matrix stack (or at least it should be stored)
      CDEBUG("glPushMatrix()");
      glPushMatrix();
      pTmp = (static_cast<CTranslationNode *>(pNode))->m_pValues;
      glTranslatef( pTmp[0], pTmp[1], pTmp[2]  );
      break;
    case UTIL_QUAD:
      Handle_Draw_UtilQuad( static_cast<CUtilQuadNode *>(pNode) );
      
      break;
    case UTIL_BOX:
      {
	CUtilBoxNode *pBox = static_cast<CUtilBoxNode *>(pNode);
	// This sphere will never have lighting
	glPushAttrib(GL_ENABLE_BIT | GL_POLYGON_BIT);
	// Set the drawstyle
	if ( pBox->IsWireframe())
	{  
	  glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); 
	} 
	else 
	{
	  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ); 
	}
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glColor4fv( pBox->GetColor().GetValues());

	glBegin( GL_QUAD_STRIP );
	glVertex3fv( pBox->GetCorner( COrientedBox::TOP_LEFT_BACK ) );
	glVertex3fv( pBox->GetCorner( COrientedBox::BOTTOM_LEFT_BACK ) );
	glVertex3fv( pBox->GetCorner( COrientedBox::TOP_RIGHT_BACK ) );
	glVertex3fv( pBox->GetCorner( COrientedBox::BOTTOM_RIGHT_BACK ) );
	glVertex3fv( pBox->GetCorner( COrientedBox::TOP_RIGHT_FRONT ) );
	glVertex3fv( pBox->GetCorner( COrientedBox::BOTTOM_RIGHT_FRONT ) );
	glVertex3fv( pBox->GetCorner( COrientedBox::TOP_LEFT_FRONT ) );
	glVertex3fv( pBox->GetCorner( COrientedBox::BOTTOM_LEFT_FRONT ) );
	glVertex3fv( pBox->GetCorner( COrientedBox::TOP_LEFT_BACK ) );
	glVertex3fv( pBox->GetCorner( COrientedBox::BOTTOM_LEFT_BACK ) );
	glEnd();

	glPopMatrix();
	glPopAttrib();
	// cleanup
	gluDeleteQuadric(quadric);
	quadric = NULL;

	
      }
      break;
    case UTIL_CONE:
      {
      
	quadric = gluNewQuadric();
	CUtilConeNode *pCone = static_cast<CUtilConeNode *>(pNode);

	// Set the drawstyle
	if ( pCone->IsWireframe())
	{
	  gluQuadricDrawStyle( quadric, GLU_SILHOUETTE );
	} 
	else
	{
	  gluQuadricDrawStyle( quadric, GLU_FILL );
	}

	// This sphere will never have lighting
	glPushAttrib(GL_ENABLE_BIT);
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glColor4fv( pCone->GetColor().GetValues());
      
	glTranslatef( pCone->GetPosition().m_pValues[CVector3::X],
		      pCone->GetPosition().m_pValues[CVector3::Y],
		      pCone->GetPosition().m_pValues[CVector3::Z] );
	
	float fAngle = acosf(pCone->GetDirection().Dot(CVector3::GetWorldZ()) / 
			     pCone->GetDirection().Length());
	//CLogger::Error() << "angle between " << pCone->GetDirection() << " and " << CVector3::GetWorldZ() << " is " << fAngle << std::endl;
	CVector3 vRotationVector = CVector3::GetWorldZ().Cross(pCone->GetDirection());
	glRotatef( RAD2DEG(fAngle), 
		   vRotationVector[CVector3::X],
		   vRotationVector[CVector3::Y],
		   vRotationVector[CVector3::Z]);
	
	gluCylinder(quadric, 0.0f, pCone->GetLength()*tan( pCone->GetAngle()), 
		    pCone->GetLength(), 32, 1 );
	
	glPopMatrix();
	glPopAttrib();
	// cleanup
	gluDeleteQuadric(quadric);
	quadric = NULL;

	
      }
      break;
    case UTIL_LINESTRIP:
      {
	CUtilLineStripNode *pLineStripNode = static_cast<CUtilLineStripNode *>(pNode);
	glPushAttrib( GL_ENABLE_BIT );
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glLineWidth(2.0);
	glColor3f( 1.0, 1.0, 1.0); // white
	glBegin( GL_LINE_STRIP );
	std::list<CVector3>::iterator it = pLineStripNode->Vertices().begin();
	for( ; it != pLineStripNode->Vertices().end(); it++)
	{
	  glVertex3fv( (*it).m_pValues );
	}
	glVertex3fv(pLineStripNode->Vertices().front().m_pValues);

	glEnd();
	glLineWidth(1.0);
	glPopAttrib();

	
      }
      break;
    case ROOT:
      break;
    case UNDEFINED:
      CLogger::Error() << "Node type is undefined!" << std::endl;
      break;
    }
  }

  return iCulled;
}
/////////////////////////////////////////////////////////////////
/// Handles the finish passes of the normal drawing process.
int 
COglRenderer::Handle_Drawing_RenderPass_Leave( CGraphNode<NodeType> *pNode )
{
  int iRetval = 0;
  
  switch ( pNode->GetType() )
  {
  case ROTATION:
  case TRANSLATION:
  case TRANSFORM:
    // Pop the previously stored matrix 
    CDEBUG("glPopMatrix();");
    glPopMatrix();
    break;
  case LIGHT_CLUSTER:
    {
      glDisable(GL_LIGHT0);
      glDisable(GL_LIGHT1);
      glDisable(GL_LIGHT2);
      glDisable(GL_LIGHT3);
      glDisable(GL_LIGHT4);
      glDisable(GL_LIGHT5);
      glDisable(GL_LIGHT6);
      glDisable(GL_LIGHT7);
      glDisable(GL_LIGHTING); 
    }
    break;
  case FRONT_MATERIAL:
  case BACK_MATERIAL:
    DisableMaterial( static_cast<CMaterialNode *>(pNode)->GetMaterial() );
    break;
  case SHADER:
    // revert to fixed functionality
    glUseProgram( 0 );
    glPopAttrib();
    break;
  case VERTEXATTRIB:
    FinishVertexAttrib( static_cast<CVertexAttribNode *>(pNode)->GetVertexAttrib());
    break;
  case CACHE:
    iRetval = HandleOglCache_Leave( static_cast<COglCacheNode *>(pNode) );
    break;
  case RENDERSTATE_STACK:
    // pops the stored attribute group
    glPopAttrib();
    break;
  case TEXTURE:
    {
      CTextureNode *pTexNode = static_cast<CTextureNode *>(pNode);
      if ( pTexNode->GetTexture() != NULL )
      { 
	glActiveTextureARB( g_aGLMultiTexIds[pTexNode->GetTexId()] );
	glDisable( GetGLType( pTexNode->GetTexture()->GetType()));
	pTexNode->GetTexture()->SetActive( pTexNode->GetTexture()->IsActive() ^ (1 << pTexNode->GetTexId()) );
      }
    }
    break;
  case RENDERSTATE_BLEND:
    {
      CBlendingStateNode *pBlendState = static_cast<CBlendingStateNode *>(pNode);
      if ( pBlendState->IsEnabled())
      {
	glDisable(GL_BLEND);
      }
    }
    break;
  case ALPHA_TEST:
    glDisable(GL_ALPHA_TEST);
    break;
  case GEOMETRY:
  case CAMERA:
  case GROUP:
  case UTIL_SPHERE:
  case UTIL_BOX:
  case UTIL_CONE:
  case UTIL_QUAD:
  case TEXT:
  case COLOR:
  case RENDERSTATE_DEPTHBUFFER:
  case SCISSOR:
  case CLEARBUFFER:
  case SHADEMODEL:
  case INDEXBUFFER:
  case PARTICLESYSTEM:
  case RENDERSTATE_CULLING:
  case ROOT:
  case GLOBAL_LIGHTING:
  case KDOP:
  case UTIL_LINESTRIP:
  case POLYLINE:
  case SELECTION:
  case OCTREE:
  case POLYGONMODE:
    break;
  case UNDEFINED:
    CLogger::Error() << "Node type is undefined!" << std::endl;
    break;
  }
  return iRetval;
}
/////////////////////////////////////////////////////////////////
void
COglRenderer::SetSelection ( unsigned int nMouseX, unsigned int nMouseY, 
				unsigned int nWidth , unsigned int nHeight  )
{
  m_Selection.m_nMouseX = nMouseX;
  m_Selection.m_nMouseY = nMouseY;
  m_Selection.m_nWidth = nWidth;
  m_Selection.m_nHeight = nHeight;

}
/////////////////////////////////////////////////////////////////
GLuint *
COglRenderer::GetSelectionBuffer()
{
  return m_aSelectionBuffer;
}
/////////////////////////////////////////////////////////////////
const unsigned int 
COglRenderer::GetNumHits()
{
  return m_nHits;
}
/////////////////////////////////////////////////////////////////
int 
COglRenderer::CreateFontset( const char *sPathToFontFile, unsigned int nFontSize)
{

#define WHITESPACE 32
  COglRendererFontset *pFontset;

  FT_Library ftLibrary;
  FT_Error ftError;
  FT_Face  ftFace;  

  COglTextureMgr *pOglTextureMgr = COglTextureMgr::GetInstance();

  // Initialize the font library
  ftError = FT_Init_FreeType( &ftLibrary ); 
  if ( ftError ) 
  { 
    CERR("Error initializing FT");
    return 1;
  }
  // Create new face
  ftError = FT_New_Face( ftLibrary,sPathToFontFile, 0, &ftFace);
  if ( ftError )
  {
    CERR("Error loading face");
    return 1;
  }
  // Set the character size
  ftError = FT_Set_Char_Size( ftFace, nFontSize * 64, 0, 0, 0 );        
  
  if ( ftError )
  {
    CERR("Error setting font size");
    return 1;
  }
  
  // create textures
  // glGenTextures( MAX_CHARS, Textures );
  
  pFontset = new COglRendererFontset();
  // create display lists 
  pFontset->DisplayList() = glGenLists(CMAX_FONT_CHARACTERS);
  
  for(unsigned int n=0;n<CMAX_FONT_CHARACTERS;n++)
  {

    if ( n == WHITESPACE )
    {
      glNewList(pFontset->DisplayList()+n,GL_COMPILE);
      glTranslatef(nFontSize*0.2, 0, 0);
      glEndList();
      continue;
    } 


    char bWidthFixApplied = 0;
    FT_UInt ftGlyphIndex;    
    ftGlyphIndex = FT_Get_Char_Index( ftFace, n );
    
    ftError = FT_Load_Glyph( ftFace, ftGlyphIndex, FT_LOAD_DEFAULT ); 
    if ( ftError ) continue; /* ignore errors */

    /* convert to an anti-aliased bitmap */  
    ftError = FT_Render_Glyph( ftFace->glyph, FT_RENDER_MODE_NORMAL ); 
    if ( ftError ) continue;
    
    if ( ftFace->glyph->bitmap.width == 0 ) continue;
    // Round dimensions up to closes power of two
    float fLog2Height = log(ftFace->glyph->bitmap.rows)/log(2);
    float fLog2Width  = log(ftFace->glyph->bitmap.width)/log(2);
    
    // Rounding error check
    if ( (fLog2Height - (int)fLog2Height) < 0.00001 ) fLog2Height = (int)fLog2Height;
    if ( (fLog2Width  - (int)fLog2Width)  < 0.00001 ) fLog2Width  = (int)fLog2Width;
    int iWidth  = (int)pow(2, ceil(fLog2Width));
    int iHeight = (int)pow(2, ceil(fLog2Height));

    // A fix for really narrow characters
    if ( iWidth == 1 ) 
    {
      bWidthFixApplied = 1;
      iWidth = 2;
    }

    // Create two-channel data
    GLubyte* expanded_data = new GLubyte[ 2 * iWidth * iHeight];
    for(int j=0; j <iHeight;j++) 
    {
      for(int i=0; i < iWidth; i++)
      {
	expanded_data[2*(i+j*iWidth)]= expanded_data[2*(i+j*iWidth)+1] = 
	(i>=ftFace->glyph->bitmap.width || j>=ftFace->glyph->bitmap.rows) ?
	0 : ftFace->glyph->bitmap.buffer[i + ftFace->glyph->bitmap.width*j];
      }
    }
    // Create and bind texture
    COglTexture *pTexture = new COglTexture();
    pTexture->SetType(TextureType::CTEX2D);
    glBindTexture  ( GL_TEXTURE_2D, pTexture->GetId());
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, iWidth, iHeight, 0,
		  GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data );
    
    delete [] expanded_data;
    // Let OglTextureMgr manage these
    pOglTextureMgr->Add(pTexture);
    // Add pointer to fontset's array as well 
    pFontset->Textures()[n] = pTexture;
    
    // --------------------------------------------------
    // face->glyph->bitmap->buffer;  // pointer to data
    // face->glyph->bitmap->width;   // pixels on a line
    // face->glyph->bitmap->rows;   // #lines
    // face->glyph->bitmap->pitch; // #|bytes| in line
    // --------------------------------------------------

    // Compile new display list
    glNewList(pFontset->DisplayList()+n,GL_COMPILE);
    // Bind texture
    glBindTexture( GL_TEXTURE_2D, pTexture->GetId());
    
    glPushMatrix();
    // move new char left so we have correct spacing
    glTranslatef( ftFace->glyph->bitmap_left, 0,0 );
    // fix g and y 
    glTranslatef(0,ftFace->glyph->bitmap_top-ftFace->glyph->bitmap.rows,0);

    float   fX = (float)ftFace->glyph->bitmap.width / (float)iWidth;
    float   fY = (float)ftFace->glyph->bitmap.rows / (float)iHeight;
    
    glBegin(GL_QUADS);
    glTexCoord2d(0,  0  ); glVertex2f( 0,ftFace->glyph->bitmap.rows );
    glTexCoord2d(0,  fY ); glVertex2f( 0,0 );
    glTexCoord2d(fX, fY ); glVertex2f( ftFace->glyph->bitmap.width, 0);
    glTexCoord2d(fX, 0  ); glVertex2f( ftFace->glyph->bitmap.width, ftFace->glyph->bitmap.rows );
    glEnd();
    
    glPopMatrix();

    // Advance along x-axis so next letter will be on correct place
    if ( bWidthFixApplied )   glTranslatef(ftFace->glyph->advance.x >> 7, 0, 0);
    else		      glTranslatef(ftFace->glyph->advance.x >> 6, 0, 0);
    
    glDisable(GL_TEXTURE_2D);
  
    glEndList();
    
  }
  // Insert our fontset to the list
  m_vFontsets.push_back( pFontset );
  
  FT_Done_Face    ( ftFace );
  FT_Done_FreeType( ftLibrary );
    
  return 0;
}
/////////////////////////////////////////////////////////////////
std::vector<COglRendererFontset *> &
COglRenderer::Fontsets()
{
  return m_vFontsets;
}
/////////////////////////////////////////////////////////////////
void 
COglRenderer::HandleStackPush( CRenderStateStackNode *pNode )
{
  
  GLenum eBits = 0;

  if ( pNode->IsStored( CRenderStateStackNode::ACCUM_BUFFER ))	eBits |= GL_ACCUM_BUFFER_BIT;
  if ( pNode->IsStored( CRenderStateStackNode::COLOR_BUFFER ))	eBits |= GL_COLOR_BUFFER_BIT;
  if ( pNode->IsStored( CRenderStateStackNode::CURRENT ))		eBits |= GL_CURRENT_BIT;
  if ( pNode->IsStored( CRenderStateStackNode::DEPTH_BUFFER ))	eBits |= GL_DEPTH_BUFFER_BIT;
  if ( pNode->IsStored( CRenderStateStackNode::ENABLE ))		eBits |= GL_ENABLE_BIT;
  if ( pNode->IsStored( CRenderStateStackNode::EVAL ))		eBits |= GL_EVAL_BIT;
  if ( pNode->IsStored( CRenderStateStackNode::FOG ))		eBits |= GL_FOG_BIT;
  if ( pNode->IsStored( CRenderStateStackNode::HINT ))		eBits |= GL_HINT_BIT;
  if ( pNode->IsStored( CRenderStateStackNode::LIGHTING ))	eBits |= GL_LIGHTING_BIT;
  if ( pNode->IsStored( CRenderStateStackNode::LINE ))		eBits |= GL_LINE_BIT;
  if ( pNode->IsStored( CRenderStateStackNode::LIST ))		eBits |= GL_LIST_BIT;
  if ( pNode->IsStored( CRenderStateStackNode::PIXEL_MODE ))	eBits |= GL_PIXEL_MODE_BIT;
  if ( pNode->IsStored( CRenderStateStackNode::POINT ))		eBits |= GL_POINT_BIT;
  if ( pNode->IsStored( CRenderStateStackNode::POLYGON ))		eBits |= GL_POLYGON_BIT;
  if ( pNode->IsStored( CRenderStateStackNode::POLYGON_STIPPLE )) eBits |= GL_POLYGON_STIPPLE_BIT;
  if ( pNode->IsStored( CRenderStateStackNode::SCISSOR ))		eBits |= GL_SCISSOR_BIT;
  if ( pNode->IsStored( CRenderStateStackNode::STENCIL_BUFFER ))  eBits |= GL_STENCIL_BUFFER_BIT;
  if ( pNode->IsStored( CRenderStateStackNode::TEXTURE ))		eBits |= GL_TEXTURE_BIT;
  if ( pNode->IsStored( CRenderStateStackNode::TRANSFORM ))	eBits |= GL_TRANSFORM_BIT;
  if ( pNode->IsStored( CRenderStateStackNode::VIEWPORT ))	eBits |= GL_VIEWPORT_BIT;

  glPushAttrib( eBits );
    
}
/////////////////////////////////////////////////////////////////
void 
COglRenderer::Handle_Draw_UtilQuad( CUtilQuadNode *pNode )
{

  CVector3 vPos = pNode->GetPosition() -
  (CVector3::GetWorldY() * pNode->GetHalfHeight()) -
  (CVector3::GetWorldX() * pNode->GetHalfWidth());


  glPushMatrix();
  glBegin(GL_QUADS);
       glTexCoord2f(pNode->GetTexCoord(0), pNode->GetTexCoord(1)); 
       glVertex3fv(vPos.m_pValues);

       vPos += CVector3::GetWorldX() * pNode->GetWidth();
       glTexCoord2f(pNode->GetTexCoord(2), pNode->GetTexCoord(3)); 
       glVertex3fv(vPos.m_pValues);
       
       vPos += CVector3::GetWorldY() * pNode->GetHeight();
       glTexCoord2f(pNode->GetTexCoord(4), pNode->GetTexCoord(5) ); 
       glVertex3fv(vPos.m_pValues);
	
       vPos -= CVector3::GetWorldX() * pNode->GetWidth();
       glTexCoord2f(pNode->GetTexCoord(6), pNode->GetTexCoord(7)); 
       glVertex3fv(vPos.m_pValues);
  glEnd();
  glPopMatrix();
    
}
/////////////////////////////////////////////////////////////////
void 
COglRenderer::Handle_Draw_CullingState( CCullingStateNode *pCullState )
{


      
  if ( !pCullState->IsFaceCullingEnabled() ||
       (!pCullState->IsFrontfaceCulled() && 
	!pCullState->IsBackfaceCulled()))
  {
    glDisable( GL_CULL_FACE );

  }
  else 
  {
    glEnable ( GL_CULL_FACE );
    if ( pCullState->IsFrontfaceCulled() && 
	 pCullState->IsBackfaceCulled())
    {
      glCullFace(GL_FRONT_AND_BACK);
    } 
    else if ( pCullState->IsBackfaceCulled() )
    {
      glCullFace(GL_BACK);
    } 
    else 
    {
      glCullFace(GL_FRONT);
    }

  }
      
    
}
/////////////////////////////////////////////////////////////////
void
COglRenderer::DrawPolyline( CPolyLineNode * pNode )
{
  // This would not produce any graphics anyway
  if ( pNode->GetPointCount() < 2 ) return;

  // This can also be done with shaders (probably more efficiently)
  
  glPushAttrib(GL_POLYGON_BIT);
  glDisable(GL_CULL_FACE);
  // T_0 = Z_0 X ( P_1 - P_0 )
  // T_k = Z_k X ( P_k-1 - P_k+1 ) | k in [1,n-1]
  // T_n = - (Z_n X ( P_n-1 - P_n ))

  CPolyLineNode &poly = (*pNode);
  unsigned int nP = 1;
  unsigned int nCount = poly.GetPointCount()-1;
  float    fTexCoordFraction = 1.0f / pNode->GetPointCount();
  float    fTexCoordY = 0.0f;
  float    fWidth = poly.GetWidth() * 0.5f;

  CVector3 vTangent = ( (poly[1] - poly[0]).Cross( m_pCurrentCamera->GetPosition()-poly[0])).ToUnit() * 0.5f * fWidth;
  
  glBegin( GL_QUAD_STRIP );  
  glVertex3fv( (poly[0]+vTangent).m_pValues );  glTexCoord2f( 0.0f, 0.0f );
  glVertex3fv( (poly[0]-vTangent).m_pValues );  glTexCoord2f( 1.0f, 0.0f );
  
  for(; nP < nCount; nP++)
  {
    fTexCoordY = fTexCoordFraction * (float)nP;
    
    vTangent = ((poly[nP+1] - poly[nP-1]).Cross( m_pCurrentCamera->GetPosition() - poly[nP])).ToUnit() * 0.5f * fWidth;
    
    glVertex3fv( (poly[nP] + vTangent).m_pValues );  glTexCoord2f( 0.0f, fTexCoordY);
    glVertex3fv( (poly[nP] - vTangent).m_pValues );  glTexCoord2f( 1.0f, fTexCoordY);
  }
  vTangent = ((poly[nP-1] - poly[nP]).Cross(m_pCurrentCamera->GetPosition() - poly[nP])).ToUnit() * 0.5f * fWidth;
  // The signs have been switched since last tangent points into reverse direction.
  glVertex3fv( (poly[nP] - vTangent).m_pValues );  glTexCoord2f( 0.0f, 1.0f );
  glVertex3fv( (poly[nP] + vTangent).m_pValues );  glTexCoord2f( 1.0f, 1.0f );
  
  glEnd();
  
  glPopAttrib();
}
/////////////////////////////////////////////////////////////////
void DrawOctreeRecursive( CGeometryOctree *pOctree )
{
  if ( pOctree == NULL )
  {
    return;
  }
  glBegin(GL_QUAD_STRIP);

  CVector3 vVertex = pOctree->GetPosition();
  // 1st
  vVertex[CVector3::X] -= pOctree->GetWidth() *0.5f;
  vVertex[CVector3::Y] += pOctree->GetHeight()*0.5f;
  vVertex[CVector3::Z] += pOctree->GetLength()*0.5f;
  glVertex3fv( vVertex.m_pValues );
  // 2nd
  vVertex[CVector3::Y] -= pOctree->GetHeight();       
  glVertex3fv( vVertex.m_pValues );
  // 3rd
  vVertex[CVector3::Y] += pOctree->GetHeight();
  vVertex[CVector3::X] += pOctree->GetWidth();
  glVertex3fv( vVertex.m_pValues );
  //4th
  vVertex[CVector3::Y] -= pOctree->GetHeight();       
  glVertex3fv( vVertex.m_pValues );
  // 5th
  vVertex[CVector3::Y] += pOctree->GetHeight();
  vVertex[CVector3::Z] -= pOctree->GetLength();
  glVertex3fv( vVertex.m_pValues );
  // 6th
  vVertex[CVector3::Y] -= pOctree->GetHeight();       
  glVertex3fv( vVertex.m_pValues );
  // 7th
  vVertex[CVector3::Y] += pOctree->GetHeight();
  vVertex[CVector3::X] -= pOctree->GetWidth();
  glVertex3fv( vVertex.m_pValues );
  // 8th
  vVertex[CVector3::Y] -= pOctree->GetHeight();
  glVertex3fv( vVertex.m_pValues );
  // 9th
  vVertex[CVector3::Y] += pOctree->GetHeight();
  vVertex[CVector3::Z] += pOctree->GetLength();
  glVertex3fv( vVertex.m_pValues );
  // 10th
  vVertex[CVector3::Y] -= pOctree->GetHeight();
  glVertex3fv( vVertex.m_pValues );

  glEnd();
  
  DrawOctreeRecursive(static_cast<CGeometryOctree *>(pOctree->GetNodes()[COctree::TOP_LEFT_FRONT]));
  DrawOctreeRecursive(static_cast<CGeometryOctree *>(pOctree->GetNodes()[COctree::TOP_LEFT_BACK]));
  DrawOctreeRecursive(static_cast<CGeometryOctree *>(pOctree->GetNodes()[COctree::TOP_RIGHT_FRONT]));
  DrawOctreeRecursive(static_cast<CGeometryOctree *>(pOctree->GetNodes()[COctree::TOP_RIGHT_BACK]));
  DrawOctreeRecursive(static_cast<CGeometryOctree *>(pOctree->GetNodes()[COctree::BOTTOM_LEFT_FRONT]));
  DrawOctreeRecursive(static_cast<CGeometryOctree *>(pOctree->GetNodes()[COctree::BOTTOM_LEFT_BACK]));
  DrawOctreeRecursive(static_cast<CGeometryOctree *>(pOctree->GetNodes()[COctree::BOTTOM_RIGHT_FRONT]));
  DrawOctreeRecursive(static_cast<CGeometryOctree *>(pOctree->GetNodes()[COctree::BOTTOM_RIGHT_BACK]));
  
}
/////////////////////////////////////////////////////////////////
void 
COglRenderer::DrawOctree( CGeometryOctree *pOctree )
{
  glDisable(GL_CULL_FACE);
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_TEXTURE_2D);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE );
  glColor4f( 1,1,1,1);
  DrawOctreeRecursive(pOctree);
}
/////////////////////////////////////////////////////////////////
using namespace TextureFilter;
/////////////////////////////////////////////////////////////////
int  
COglRenderer::HandleTexture( CTextureNode *pNode )
{

  if ( pNode->GetTexture() == NULL ) 
  {
    return 0;
  }
  
  if ( (pNode->GetTexture()->IsTransparent() && 
       GetRenderPass() == OGL_RENDER_PASS_ONLY_OPAQUE) ||
       (!pNode->GetTexture()->IsTransparent() && 
	GetRenderPass() == OGL_RENDER_PASS_ONLY_TRANSPARENT))
  {
    return 1;
  }

  COglTexture *pTexture = static_cast<COglTexture *>(pNode->GetTexture());

  // check enable bit for texture id
  //if ( ! (pTexture->IsActive() & (1 << pNode->GetTexId())) )
  // {
    // set this enabled.
    pTexture->SetActive( pTexture->IsActive() | 1 << pNode->GetTexId() );
    // Here we handle textures, which have been stored on server side
    glActiveTextureARB( g_aGLMultiTexIds[pNode->GetTexId()] );
    GLenum nTarget = GetGLType(pTexture->GetType());
    glEnable( nTarget );
    glBindTexture( nTarget, pTexture->GetId());
    std::list<TextureFilter::Type>::const_iterator itTexFilter;

    itTexFilter = pNode->GetValues().begin();

    for( ; itTexFilter != pNode->GetValues().end(); itTexFilter++)
    {
      switch( *itTexFilter )
      {
      case ENV_MODULATE:
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 
	break;
      case ENV_DECAL:
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL); 
	break;
      case ENV_REPLACE:
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); 
	break;
      case ENV_BLEND:
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
	break;
      case ENV_COLOR:
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, pNode->GetTexEnvColor().GetValues()); 
	break;
      case MIN_NEAREST:
	glTexParameteri( nTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	break;
      case MIN_LINEAR:
	glTexParameteri( nTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	break;
      case MIN_MIP_NEAREST:
	glTexParameteri( nTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	break;
      case MIN_MIP_LINEAR:
	glTexParameteri( nTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	break;
      case MAG_NEAREST:
	glTexParameteri( nTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	break;
      case MAG_LINEAR:
	glTexParameteri( nTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	break;
      case T_WRAP_REPEAT:
	glTexParameteri( nTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
	break;
      case S_WRAP_REPEAT:
	glTexParameteri( nTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
	break;
      case T_WRAP_CLAMP:
	glTexParameteri( nTarget, GL_TEXTURE_WRAP_T, GL_CLAMP);
	break;
      case S_WRAP_CLAMP:
	glTexParameteri( nTarget, GL_TEXTURE_WRAP_S, GL_CLAMP);
	break;
      case T_WRAP_CLAMP_TO_EDGE:
	glTexParameteri( nTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	break;
      case S_WRAP_CLAMP_TO_EDGE:
	glTexParameteri( nTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	break;
      }
    
    
    } 
    
    //  }
  return 0;
}
/////////////////////////////////////////////////////////////////
