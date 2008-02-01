/******************************************************************
 *   Copyright(c) 2006,2007 eNtity/Anssi Gröhn
 * 
 *   This file is part of GSE.
 *
 *   GSE is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *    GSE is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with GSE; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 ******************************************************************/
/////////////////////////////////////////////////////////////////
#include <SDL.h>
#include "GSE_globals.h"
#include "GSE_Octree.h"
#include "GSE_GeometryData.h"
#include "GSE_Texture.h"
#include "GSE_OglRenderer.h"
#include "GSE_OglTexture.h"
#include "GSE_OglUtils.h"
#include "GSE_OglShaders.h"
#include "GSE_Logger.h"
#include "GSE_Light.h"
#include "GSE_Material.h"
#include "GSE_IndexBuffer.h"
#include "GSE_math.h"
#include "GSE_particlesystems.h"
#include "GSE_geometry.h"
#include "GSE_OglCache.h"
#include <GL/GLee.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <ft2build.h>
#include <list>
#include <iostream>
using std::cerr;
using std::endl;
#include FT_FREETYPE_H
/////////////////////////////////////////////////////////////////
#ifdef DEBUG
#define GSE_SHOW_OGL_ERRORS() {\
	Core::GSE_Logger::Error() << "GL_ERROR at " << __LINE__ << std::endl;\
	GSE_OglUtils::ShowGLErrors();\
}
#else
#define GSE_SHOW_OGL_ERRORS() {}
#endif
/////////////////////////////////////////////////////////////////
// namespace definitions 
using Geometry::GSE_Plane;
using Core::GSE_Logger;
using namespace RenderNode;
/////////////////////////////////////////////////////////////////
// globals 
const GLenum g_aGLMultiTexIds[GSE_MAX_TEXCOORD_ARRAYS] = { GL_TEXTURE0_ARB,
							   GL_TEXTURE1_ARB,
							   GL_TEXTURE2_ARB,
							   GL_TEXTURE3_ARB,
							   GL_TEXTURE4_ARB,
							   GL_TEXTURE5_ARB,
							   GL_TEXTURE6_ARB,
							   GL_TEXTURE7_ARB };
/////////////////////////////////////////////////////////////////
GLenum GetGLType( TextureType::Type type )
{
  switch ( type )
  {
  case TextureType::GSE_TEX1D:
    return GL_TEXTURE_1D;
    break;
  case TextureType::GSE_TEX2D:
    return GL_TEXTURE_2D;
    break;
  case TextureType::GSE_TEXR2D:
    return GL_TEXTURE_RECTANGLE_ARB;
    break;
  }
  return GL_TEXTURE_2D;
}
/////////////////////////////////////////////////////////////////
// Returns true if display lists is being compiled
#define IS_COMPILING_DISPLAY_LIST() ( !m_DisplayListStack.empty() )
/////////////////////////////////////////////////////////////////
GSE_OglRendererFontset::GSE_OglRendererFontset()
{
  m_nDisplayLists = 0;
  memset( m_ppTextures, 0, GSE_MAX_FONT_CHARACTERS );
}
/////////////////////////////////////////////////////////////////
GSE_OglRendererFontset::~GSE_OglRendererFontset()
{
  GSE_OglTextureMgr *pOglTextureMgr = GSE_OglTextureMgr::GetInstance();
  // delete display lists
  glDeleteLists(m_nDisplayLists, GSE_MAX_FONT_CHARACTERS);
  // delete textures as well
  for(unsigned int nTex=0;nTex<GSE_MAX_FONT_CHARACTERS;nTex++)
  {
    if ( m_ppTextures[nTex] != NULL )
    {
      GSE_Texture *pTexture = m_ppTextures[nTex]; 
      pOglTextureMgr->Delete( pTexture );
      m_ppTextures[nTex] = NULL;
    }
  }
}
/////////////////////////////////////////////////////////////////
GLuint &
GSE_OglRendererFontset::DisplayList()
{
  return m_nDisplayLists;
}
/////////////////////////////////////////////////////////////////
// Returns the pointer to array of texture pointers
GSE_Texture **
GSE_OglRendererFontset::Textures()
{
  return m_ppTextures;
}
/////////////////////////////////////////////////////////////////
GSE_OglRendererFeatures::GSE_OglRendererFeatures()
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
GSE_OglRendererFeatures::Init()
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
GSE_OglRendererFeatures::HasVertexProgram()
{
  return m_bARB_vertex_program;
}
/////////////////////////////////////////////////////////////////
char 
GSE_OglRendererFeatures::HasVertexShader()
{
  return m_bARB_vertex_shader;
}
/////////////////////////////////////////////////////////////////
char 
GSE_OglRendererFeatures::HasFragmentShader()
{
  return m_bARB_fragment_shader;
}
/////////////////////////////////////////////////////////////////
char 
GSE_OglRendererFeatures::HasVertexDescriptor()
{
  return m_bEXT_vertex_array;
}
/////////////////////////////////////////////////////////////////
char 
GSE_OglRendererFeatures::HasVertexBufferObject()
{
  return m_bARB_vertex_buffer_object;
}
/////////////////////////////////////////////////////////////////
char 
GSE_OglRendererFeatures::HasMultitexture()
{
  return m_bARB_multitexture;
}
/////////////////////////////////////////////////////////////////
char 
GSE_OglRendererFeatures::HasShaderObjects()
{
  return m_bARB_shader_objects;
}
/////////////////////////////////////////////////////////////////
int
GSE_OglRendererFeatures::GetMaxLights()
{
  return m_iMaxLights;
}
/////////////////////////////////////////////////////////////////
int  
GSE_OglRendererFeatures::GetMaxElementsVertices()
{
  return m_iMaxElementsVertices;
}
/////////////////////////////////////////////////////////////////
int  
GSE_OglRendererFeatures::GetMaxElementsIndices()
{
  return m_iMaxElementsIndices;
}
/////////////////////////////////////////////////////////////////
std::ostream &operator<<(std::ostream &stream, GSE_OglRendererFeatures &obj)
{
  stream << "OpenGL extensions:" << std::endl;
  stream << "------------------" << std::endl;
  stream << "GL_ARB_vertex_program "  << ( obj.HasVertexProgram() ? "YES" : "NO" ) << endl;
  stream << "GL_ARB_vertex_shader "   << ( obj.HasVertexShader() ? "YES" : "NO" ) << endl;
  stream << "GL_ARB_fragment_shader " << ( obj.HasFragmentShader() ? "YES" : "NO" ) << endl;
  stream << "GL_EXT_vertex_array "    << ( obj.HasVertexDescriptor() ? "YES" : "NO" ) << endl;
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
GSE_OglRenderer::GSE_OglRenderer()
{
  m_pFeatures = new GSE_OglRendererFeatures();
  GSE_LOG( "OpenGL information:" << endl << *m_pFeatures );
  
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
GSE_OglRenderer::~GSE_OglRenderer()
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
GSE_OglRenderer::SetRenderPass(GSE_OglRenderPass_t iRenderPass)
{
  //m_iRenderPassType = iRenderPass;
  GSE_MultiPassRenderer<GSE_OglRenderPass_t>::SetRenderPass(iRenderPass);

  switch ( GetRenderPass() )
  {
  case OGL_RENDER_PASS_GET_SELECTION:
    glSelectBuffer( GSE_MAX_SELECTION_OBJECTS, m_aSelectionBuffer );
    glRenderMode( GL_SELECT );
    glInitNames();
    glPushName(0);
    break;
  case OGL_RENDER_PASS_ONLY_OPAQUE:
  case OGL_RENDER_PASS_ONLY_TRANSPARENT:
    glRenderMode( GL_RENDER );
  default:
    break;
  } 
  
}
/////////////////////////////////////////////////////////////////
void
GSE_OglRenderer::SetClearBuffers( char bFlag )
{
  m_bClearBuffers = bFlag;
}
/////////////////////////////////////////////////////////////////
Geometry::GSE_OrientedBox &
GSE_OglRenderer::GetBoundingBox()
{
  return m_obBoundingBox;
}
/////////////////////////////////////////////////////////////////
Geometry::GSE_Sphere &
GSE_OglRenderer::GetBoundingSphere()
{
  return m_BoundingSphere;
}
/////////////////////////////////////////////////////////////////
int 
GSE_OglRenderer::Enter( GSE_GraphNode<NodeType> *pNode )
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
GSE_OglRenderer::Leave( GSE_GraphNode<NodeType> *pNode )
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
GSE_OglRenderer::ApplyLightParameters( GSE_LightClusterNode *pNode )
{
  
  /////////////////////////////////////////////////////////////////
  GSE_Light *pLight = NULL;
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
    GSE_DEBUG( "glEnable( GL_LIGHT" << nLight << ")")
    // set position
    // Convert position into float array of four elements
    aLightPosition[0] = pLight->GetPosition().m_pValues[GSE_Vector3::X];
    aLightPosition[1] = pLight->GetPosition().m_pValues[GSE_Vector3::Y];
    aLightPosition[2] = pLight->GetPosition().m_pValues[GSE_Vector3::Z];
    /////////////////////////////////////////////////////////////////
    switch ( pLight->m_iLightType )
    {
      /////////////////////////////////////////////////////////////////
    case GSE_Light::DIRECTIONAL:

      GSE_DEBUG("/* We have DIRECTIONAL light */");
      /////////////////////////////////////////////////////////////////
      // if we have directional light, the position parameter 
      // actually defines the direction
      aLightPosition[0] = -pLight->m_vDirection[GSE_Vector3::X];
      aLightPosition[1] = -pLight->m_vDirection[GSE_Vector3::Y];
      aLightPosition[2] = -pLight->m_vDirection[GSE_Vector3::Z];
      aLightPosition[3] = 0.0f;
      glLightf(iGLLightID, GL_SPOT_CUTOFF, 180.0);
      /////////////////////////////////////////////////////////////////
      GSE_DEBUG( "glLightf(GL_LIGHT" << nLight << ", GL_SPOT_CUTOFF, 180.0)");
      GSE_DEBUG( "glLightf(GL_LIGHT" << nLight << ", GL_LINEAR_ATTENUATION, 0.0) /* ignored*/");
      GSE_DEBUG( "glLightf(GL_LIGHT" << nLight << ", GL_QUADRATIC_ATTENUATION, 0.0) /* ignored */");
      GSE_DEBUG( "glLightf(GL_LIGHT" << nLight << ", GL_CONSTANT_ATTENUATION, 0.0) /* ignored */");
      break;
      /////////////////////////////////////////////////////////////////
    case GSE_Light::SPOTLIGHT:

      GSE_DEBUG( "/* We have SPOTLIGHT  */" );
      /////////////////////////////////////////////////////////////////
      aLightPosition[3] = 1.0f;
      /////////////////////////////////////////////////////////////////    
      glLightf( iGLLightID, GL_SPOT_CUTOFF,    pLight->m_fSpotAngle);
      glLightfv(iGLLightID, GL_SPOT_DIRECTION, pLight->m_vDirection.m_pValues);
      /////////////////////////////////////////////////////////////////
      GSE_DEBUG("glLightf(GL_LIGHT" << nLight << ", GL_SPOT_CUTOFF, "
		<< pLight->m_fSpotAngle << ")" );
      GSE_DEBUG("glLightf(GL_LIGHT" << nLight << ", GL_SPOT_DIRECTION, "
		<< pLight->m_vDirection << ")" );
      /////////////////////////////////////////////////////////////////
      // Set the attenuation parameters
      glLightf(iGLLightID, GL_LINEAR_ATTENUATION,    pLight->m_fLinearAttenuation);
      glLightf(iGLLightID, GL_QUADRATIC_ATTENUATION, pLight->m_fQuadraticAttenuation);
      glLightf(iGLLightID, GL_CONSTANT_ATTENUATION,  pLight->m_fConstantAttenuation);
      /////////////////////////////////////////////////////////////////
      GSE_DEBUG( "glLightf(GL_LIGHT" << nLight << ", GL_LINEAR_ATTENUATION, "
		 << pLight->m_fLinearAttenuation  << ")" );
      GSE_DEBUG("glLightf(GL_LIGHT" << nLight << ", GL_QUADRATIC_ATTENUATION, "
		<< pLight->m_fQuadraticAttenuation  << ")" );
      GSE_DEBUG( "glLightf(GL_LIGHT" << nLight << ", GL_CONSTANT_ATTENUATION, "
		 << pLight->m_fConstantAttenuation << ")" );

      break;
      /////////////////////////////////////////////////////////////////
    case GSE_Light::POINTLIGHT:

      GSE_DEBUG( "/* We have POINTLIGHT  */" );
      /////////////////////////////////////////////////////////////////
      aLightPosition[3] = 1.0f;
      glLightf(iGLLightID, GL_SPOT_CUTOFF, 180.0f);
      GSE_DEBUG( "glLightf(GL_LIGHT" << nLight << ", GL_SPOT_CUTOFF, 180.0)");
      /////////////////////////////////////////////////////////////////
      // Set the attenuation parameters
      glLightf(iGLLightID, GL_LINEAR_ATTENUATION,    pLight->m_fLinearAttenuation);
      glLightf(iGLLightID, GL_QUADRATIC_ATTENUATION, pLight->m_fQuadraticAttenuation);
      glLightf(iGLLightID, GL_CONSTANT_ATTENUATION,  pLight->m_fConstantAttenuation);
      /////////////////////////////////////////////////////////////////
      GSE_DEBUG("glLightf(GL_LIGHT" << nLight << ", GL_LINEAR_ATTENUATION, "
		<< pLight->m_fLinearAttenuation  << ")" );
      GSE_DEBUG("glLightf(GL_LIGHT" << nLight << ", GL_QUADRATIC_ATTENUATION, "
		<< pLight->m_fQuadraticAttenuation  << ")");
      GSE_DEBUG("glLightf(GL_LIGHT" << nLight << ", GL_CONSTANT_ATTENUATION, "
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
    GSE_DEBUG( "glLightf(GL_LIGHT" << nLight << ", GL_POSITION, "
	       << aLightPosition[0] << ","
	       << aLightPosition[1] << ","
	       << aLightPosition[2] << ","
	       << aLightPosition[3] << ")" );
    GSE_DEBUG("glLightfv(GL_LIGHT" << nLight << ", GL_SPECULAR, " 
	      << pLight->m_vSpecular << ")");
    GSE_DEBUG( "glLightf(GL_LIGHT" << nLight << ", GL_SPOT_EXPONENT, "
	       << pLight->m_fSpotExponent << ")" );
    GSE_DEBUG( "glLightfv(GL_LIGHT" << nLight << ", GL_DIFFUSE, "
	       << pLight->m_vDiffuseColor << ")" );
    GSE_DEBUG( "glLightfv(GL_LIGHT" << nLight << ", GL_AMBIENT, "
	       << pLight->m_vAmbientColor << ")");
    /////////////////////////////////////////////////////////////////
  }
  
  
}
/////////////////////////////////////////////////////////////////
void
GSE_OglRenderer::ApplyPerspective( GSE_Camera *pCamera )
{  

  m_pCurrentCamera = pCamera;
 
  if ( pCamera == NULL )
  {
    GSE_ERR("Can't handle NULL's");
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
  GSE_Matrix4x4f mProj, mModelv, mClip;
  GSE_Plane tmpPlane;
  
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
  tmpPlane.m_pValues[GSE_Plane::X] = pClipArray[3]  - pClipArray[0];
  tmpPlane.m_pValues[GSE_Plane::Y] = pClipArray[7]  - pClipArray[4];
  tmpPlane.m_pValues[GSE_Plane::Z] = pClipArray[11] - pClipArray[8];
  tmpPlane.m_pValues[GSE_Plane::D] = pClipArray[15] - pClipArray[12];
  tmpPlane.Normalize();
  pCamera->Frustum().SetPlane( GSE_Frustum::RIGHT, tmpPlane );
  
  // calculate LEFT plane
  tmpPlane.m_pValues[GSE_Plane::X] = pClipArray[3]  + pClipArray[0];
  tmpPlane.m_pValues[GSE_Plane::Y] = pClipArray[7]  + pClipArray[4];
  tmpPlane.m_pValues[GSE_Plane::Z] = pClipArray[11] + pClipArray[8];
  tmpPlane.m_pValues[GSE_Plane::D] = pClipArray[15] + pClipArray[12];
  tmpPlane.Normalize();
  pCamera->Frustum().SetPlane( GSE_Frustum::LEFT, tmpPlane );

  // calculate BOTTOM plane
  tmpPlane.m_pValues[GSE_Plane::X] = pClipArray[ 3] + pClipArray[ 1];
  tmpPlane.m_pValues[GSE_Plane::Y] = pClipArray[ 7] + pClipArray[ 5];
  tmpPlane.m_pValues[GSE_Plane::Z] = pClipArray[11] + pClipArray[ 9];
  tmpPlane.m_pValues[GSE_Plane::D] = pClipArray[15] + pClipArray[13];
  tmpPlane.Normalize();
  pCamera->Frustum().SetPlane( GSE_Frustum::BOTTOM, tmpPlane );

  // calculate TOP plane
  tmpPlane.m_pValues[GSE_Plane::X] = pClipArray[ 3] - pClipArray[ 1];
  tmpPlane.m_pValues[GSE_Plane::Y] = pClipArray[ 7] - pClipArray[ 5];
  tmpPlane.m_pValues[GSE_Plane::Z] = pClipArray[11] - pClipArray[ 9];
  tmpPlane.m_pValues[GSE_Plane::D] = pClipArray[15] - pClipArray[13];
  tmpPlane.Normalize();
  pCamera->Frustum().SetPlane( GSE_Frustum::TOP, tmpPlane );

  // calculate FAR plane
  tmpPlane.m_pValues[GSE_Plane::X] = pClipArray[ 3] - pClipArray[ 2];
  tmpPlane.m_pValues[GSE_Plane::Y] = pClipArray[ 7] - pClipArray[ 6];
  tmpPlane.m_pValues[GSE_Plane::Z] = pClipArray[11] - pClipArray[10];
  tmpPlane.m_pValues[GSE_Plane::D] = pClipArray[15] - pClipArray[14];
  tmpPlane.Normalize();
  pCamera->Frustum().SetPlane( GSE_Frustum::FAR, tmpPlane );
  
  // calculate NEAR plane
  tmpPlane.m_pValues[GSE_Plane::X] = pClipArray[ 3] + pClipArray[ 2];
  tmpPlane.m_pValues[GSE_Plane::Y] = pClipArray[ 7] + pClipArray[ 6];
  tmpPlane.m_pValues[GSE_Plane::Z] = pClipArray[11] + pClipArray[10];
  tmpPlane.m_pValues[GSE_Plane::D] = pClipArray[15] + pClipArray[14];
  tmpPlane.Normalize();
  pCamera->Frustum().SetPlane( GSE_Frustum::NEAR, tmpPlane );
  
  pCamera->CalculateBoundingSphere();
  pCamera->CalculateBoundingCone();

}
/////////////////////////////////////////////////////////////////
void
GSE_OglRenderer::DisableMaterial( GSE_Material *pMaterial )
{
  // NOP
}
/////////////////////////////////////////////////////////////////
void
GSE_OglRenderer::ApplyMaterial( GLenum iType, GSE_Material *pMaterial )
{
  
  // Sanity check
  if ( pMaterial == NULL )
  {
    GSE_WARN("Can't handle NULL's");
    return;
  }
  
  switch ( pMaterial->GetCacheMethod())
  {
  case GSE_Material::CACHED_IN_DL:
    glCallList( pMaterial->Cache());
    return;
    break;
  case GSE_Material::NO_CACHE:
    break;
  case GSE_Material::REQUEST_DL_CACHE:
    /* generate list and compile properties in */
    pMaterial->Cache() = glGenLists(1);
    glNewList( pMaterial->Cache(), GL_COMPILE );
    break;
  case GSE_Material::REGENERATE_DL_CACHE:

    glDeleteLists(pMaterial->Cache(), 1);
    pMaterial->Cache() = glGenLists(1);
    glNewList( pMaterial->Cache(), GL_COMPILE );
    break;
  case GSE_Material::DELETE_DL_CACHE:
    glDeleteLists(pMaterial->Cache(),1);
    pMaterial->SetCacheMethod( GSE_Material::NO_CACHE );
    break;
  } 
  
  glMaterialfv( iType, GL_DIFFUSE,   pMaterial->GetDiffuse().GetValues());
  glMaterialfv( iType, GL_AMBIENT,   pMaterial->GetAmbient().GetValues());
  glMaterialfv( iType, GL_SPECULAR,  pMaterial->GetSpecular().GetValues());
  glMaterialfv( iType, GL_EMISSION,  pMaterial->GetEmission().GetValues());
  glMaterialf(  iType, GL_SHININESS, pMaterial->GetShininess()*128.0f);

#ifdef DEBUG

  GSE_Logger::Error() << "Material: " << pMaterial->GetName() << std::endl;
  GSE_Logger::Error() << "glMaterialfv( iType, GL_AMBIENT, "
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
  case GSE_Material::CACHED_IN_DL:
  case GSE_Material::NO_CACHE:
  case GSE_Material::DELETE_DL_CACHE:
    break;
  case GSE_Material::REQUEST_DL_CACHE:
  case GSE_Material::REGENERATE_DL_CACHE:
    glEndList();
    pMaterial->SetCacheMethod( GSE_Material::CACHED_IN_DL );
    glCallList( pMaterial->Cache());
    break;
  }
}
/////////////////////////////////////////////////////////////////
void
GSE_OglRenderer::SetupArrays( GSE_GeometryData *pGeometryData ) 
{
  

  if ( pGeometryData == NULL ) 
  {
    GSE_WARN("Can't handle NULL's");
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
  GSE_DEBUG("arrays disabled"); 
  for(int i =0;i<GSE_MAX_TEXCOORD_ARRAYS;i++)
  {
    GSE_DEBUG("disabling multitexarray"); 
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
      switch( pGeometryData->GetCacheMethod( GSE_GeometryData::VERTEX_ARRAY ))
      {
      case GSE_GeometryData::NO_CACHE:
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
	glVertexPointer( 3, GL_FLOAT, 0, pGeometryData->m_pVertices);
	break;
      case GSE_GeometryData::REQUEST_VBO_CACHE:
	glGenBuffersARB(1, &(pGeometryData->Cache( GSE_GeometryData::VERTEX_ARRAY )));
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, pGeometryData->Cache( GSE_GeometryData::VERTEX_ARRAY));
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(float)*3*pGeometryData->m_iNumElements, pGeometryData->m_pVertices, GL_STATIC_DRAW_ARB);
	////////////////////
	// Prepare for the case if the buffer does not fit.
	if ( glGetError() == GL_OUT_OF_MEMORY )
	{
	  GSE_ERR("Couldn't create VBO for vertexbuffer.");
	  glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
	  glDeleteBuffersARB(1, &(pGeometryData->Cache( GSE_GeometryData::VERTEX_ARRAY)));
	  glVertexPointer( 3, GL_FLOAT, 0, pGeometryData->m_pVertices);
	  pGeometryData->Cache( GSE_GeometryData::VERTEX_ARRAY ) = 0;
	  pGeometryData->SetCacheMethod( GSE_GeometryData::VERTEX_ARRAY, GSE_GeometryData::NO_CACHE );
	} 
	else
	{
	  glVertexPointer( 3, GL_FLOAT, 0, 0);
	  pGeometryData->SetCacheMethod( GSE_GeometryData::VERTEX_ARRAY, GSE_GeometryData::CACHED_IN_VBO );
	}
	break;
      case GSE_GeometryData::CACHED_IN_VBO:
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, pGeometryData->Cache(GSE_GeometryData::VERTEX_ARRAY) );
	glVertexPointer( 3, GL_FLOAT, 0, 0);
	break;
      case GSE_GeometryData::REGENERATE_VBO_CACHE:
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, pGeometryData->Cache(GSE_GeometryData::VERTEX_ARRAY) );
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, 0, NULL, GL_STATIC_DRAW_ARB);
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(float)*3*pGeometryData->m_iNumElements, pGeometryData->m_pVertices, GL_STATIC_DRAW_ARB);
	glVertexPointer( 3, GL_FLOAT, 0, 0);
	pGeometryData->SetCacheMethod( GSE_GeometryData::VERTEX_ARRAY, GSE_GeometryData::CACHED_IN_VBO );
	break;
      case GSE_GeometryData::DELETE_VBO_CACHE:
	glDeleteBuffersARB( 1, &(pGeometryData->Cache( GSE_GeometryData::VERTEX_ARRAY )));
	pGeometryData->Cache( GSE_GeometryData::VERTEX_ARRAY ) = 0;
	pGeometryData->SetCacheMethod( GSE_GeometryData::VERTEX_ARRAY, GSE_GeometryData::NO_CACHE );

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
      switch( pGeometryData->GetCacheMethod( GSE_GeometryData::NORMAL_ARRAY ))
      {
      case GSE_GeometryData::NO_CACHE:
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
	glNormalPointer( GL_FLOAT, 0, aNormals );
	break;
      case GSE_GeometryData::REQUEST_VBO_CACHE:
	glGenBuffersARB(1, &(pGeometryData->Cache( GSE_GeometryData::NORMAL_ARRAY )));
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, pGeometryData->Cache( GSE_GeometryData::NORMAL_ARRAY));
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(float)*3*pGeometryData->m_iNumElements, aNormals, GL_STATIC_DRAW_ARB);
	if ( glGetError() == GL_OUT_OF_MEMORY )
	{

	  GSE_ERR("Couldn't create VBO for normalbuffer.");
	  glDeleteBuffersARB( 1, &(pGeometryData->Cache( GSE_GeometryData::NORMAL_ARRAY )));
	  pGeometryData->Cache( GSE_GeometryData::NORMAL_ARRAY ) = 0;
	  pGeometryData->SetCacheMethod( GSE_GeometryData::NORMAL_ARRAY, GSE_GeometryData::NO_CACHE );
	  glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
	  glNormalPointer( GL_FLOAT, 0, aNormals );
	}
	else
	{
	  glNormalPointer( GL_FLOAT, 0, 0 );
	  pGeometryData->SetCacheMethod( GSE_GeometryData::NORMAL_ARRAY, GSE_GeometryData::CACHED_IN_VBO );
	}
	break;
      case GSE_GeometryData::CACHED_IN_VBO:
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, pGeometryData->Cache(GSE_GeometryData::NORMAL_ARRAY) );
	glNormalPointer( GL_FLOAT, 0, 0 );
	break;
      case GSE_GeometryData::REGENERATE_VBO_CACHE:
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, pGeometryData->Cache(GSE_GeometryData::NORMAL_ARRAY) );
	// clear previous
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, 0, NULL, GL_STATIC_DRAW_ARB);
	// send new ones in
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(float)*3*pGeometryData->m_iNumElements, aNormals, GL_STATIC_DRAW_ARB);
	glNormalPointer( GL_FLOAT, 0, 0 );
	pGeometryData->SetCacheMethod( GSE_GeometryData::NORMAL_ARRAY, GSE_GeometryData::CACHED_IN_VBO );
	break;
      case GSE_GeometryData::DELETE_VBO_CACHE:
	glDeleteBuffersARB( 1, &(pGeometryData->Cache( GSE_GeometryData::NORMAL_ARRAY )));
	pGeometryData->Cache( GSE_GeometryData::NORMAL_ARRAY ) = 0;
	pGeometryData->SetCacheMethod( GSE_GeometryData::NORMAL_ARRAY, GSE_GeometryData::NO_CACHE );

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
      switch( pGeometryData->GetCacheMethod( GSE_GeometryData::COLOR_ARRAY ))
      {
      case GSE_GeometryData::NO_CACHE:
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
	glColorPointer( 4, GL_FLOAT, 0, aColors );
	break;
      case GSE_GeometryData::REQUEST_VBO_CACHE:
	glGenBuffersARB(1, &(pGeometryData->Cache( GSE_GeometryData::COLOR_ARRAY )));
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, pGeometryData->Cache( GSE_GeometryData::COLOR_ARRAY));
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(float)*4*pGeometryData->m_iNumElements, aColors, GL_STATIC_DRAW_ARB);
	if ( glGetError() == GL_OUT_OF_MEMORY )
	{
	  GSE_ERR("Couldn't create VBO for colorbuffer.");
	  glDeleteBuffersARB( 1, &(pGeometryData->Cache( GSE_GeometryData::COLOR_ARRAY )));
	  pGeometryData->Cache( GSE_GeometryData::COLOR_ARRAY ) = 0;
	  pGeometryData->SetCacheMethod( GSE_GeometryData::COLOR_ARRAY, GSE_GeometryData::NO_CACHE );
	  glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
	  glColorPointer( 4, GL_FLOAT, 0, aColors );
	}
	else
	{
	  glColorPointer( 4, GL_FLOAT, 0, 0 );
	  pGeometryData->SetCacheMethod( GSE_GeometryData::COLOR_ARRAY, GSE_GeometryData::CACHED_IN_VBO );
	}
	break;
      case GSE_GeometryData::CACHED_IN_VBO:
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, pGeometryData->Cache(GSE_GeometryData::COLOR_ARRAY) );
	glColorPointer( 4, GL_FLOAT, 0, 0 );
	break;
      case GSE_GeometryData::REGENERATE_VBO_CACHE:
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, pGeometryData->Cache(GSE_GeometryData::COLOR_ARRAY) );
	// clear previous
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, 0, NULL, GL_STATIC_DRAW_ARB);
	// send new ones in
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(float)*4*pGeometryData->m_iNumElements, aColors, GL_STATIC_DRAW_ARB);
	glColorPointer( 4, GL_FLOAT, 0, 0 );
	pGeometryData->SetCacheMethod( GSE_GeometryData::COLOR_ARRAY, GSE_GeometryData::CACHED_IN_VBO );
	break;
      case GSE_GeometryData::DELETE_VBO_CACHE:
	glDeleteBuffersARB( 1, &(pGeometryData->Cache( GSE_GeometryData::COLOR_ARRAY )));
	pGeometryData->Cache( GSE_GeometryData::COLOR_ARRAY ) = 0;
	pGeometryData->SetCacheMethod( GSE_GeometryData::COLOR_ARRAY, GSE_GeometryData::NO_CACHE );
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
	break; 
      }
    }
  } 
  /////////////////////////////////////////////////////////////////
  // Enable Texture Coordinate Arrays
  /////////////////////////////////////////////////////////////////
  for(unsigned int i=0;i<GSE_MAX_TEXCOORD_ARRAYS;i++){

    if ( pGeometryData->HasTexCoords(i) ) 
    {
      
      // Here we handle VERTEX ARRAYS, which are handled client-side
      glClientActiveTextureARB(g_aGLMultiTexIds[i]);
      glEnableClientState( GL_TEXTURE_COORD_ARRAY );   

      // determine cache key
      GSE_GeometryData::CacheTarget_t nCacheKey = GSE_GeometryData::TEXCOORD0_ARRAY;
      // Since the key is enumeration, we can do this and be moderately safe.
      nCacheKey= (GSE_GeometryData::CacheTarget_t)(((int)nCacheKey) +i);

      if ( IS_COMPILING_DISPLAY_LIST() || !m_pFeatures->HasVertexBufferObject() )
      {
	glTexCoordPointer( 2, GL_FLOAT, 0, pGeometryData->m_pTexCoords[i] );
      } 
      else 
      {
	switch( pGeometryData->GetCacheMethod( nCacheKey ))
	{
	case GSE_GeometryData::NO_CACHE:
	  glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
	  glTexCoordPointer( 2, GL_FLOAT, 0, pGeometryData->m_pTexCoords[i] );
	  break;
	case GSE_GeometryData::REQUEST_VBO_CACHE:
	  glGenBuffersARB(1, &(pGeometryData->Cache( nCacheKey )));
	  glBindBufferARB( GL_ARRAY_BUFFER_ARB, pGeometryData->Cache( nCacheKey));
	  glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(float)*2*pGeometryData->m_iNumElements, pGeometryData->m_pTexCoords[i], GL_STATIC_DRAW_ARB);
	  if ( glGetError() == GL_OUT_OF_MEMORY )
	  {
	    GSE_ERR("Couldn't create VBO for texcoordbuffer " << i);
	    glDeleteBuffersARB( 1, &(pGeometryData->Cache( nCacheKey )));
	    pGeometryData->Cache( nCacheKey ) = 0;
	    pGeometryData->SetCacheMethod( nCacheKey, GSE_GeometryData::NO_CACHE );
	    glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
	    glTexCoordPointer( 2, GL_FLOAT, 0, pGeometryData->m_pTexCoords[i] );
	  }
	  else
	  {
	    glTexCoordPointer( 2, GL_FLOAT, 0, 0 );
	    pGeometryData->SetCacheMethod( nCacheKey, GSE_GeometryData::CACHED_IN_VBO );
	  }
	  break;
	case GSE_GeometryData::CACHED_IN_VBO:
	  glBindBufferARB( GL_ARRAY_BUFFER_ARB, pGeometryData->Cache(nCacheKey) );
	  glTexCoordPointer( 2, GL_FLOAT, 0, 0 );
	  break;
	case GSE_GeometryData::REGENERATE_VBO_CACHE:
	  glBindBufferARB( GL_ARRAY_BUFFER_ARB, pGeometryData->Cache(nCacheKey) );
	  // clear previous
	  glBufferDataARB( GL_ARRAY_BUFFER_ARB, 0, NULL, GL_STATIC_DRAW_ARB);
	  // send new ones in
	  glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(float)*2*pGeometryData->m_iNumElements,  pGeometryData->m_pTexCoords[i], GL_STATIC_DRAW_ARB);
	  glTexCoordPointer( 2, GL_FLOAT, 0, 0 );
	  pGeometryData->SetCacheMethod( nCacheKey, GSE_GeometryData::CACHED_IN_VBO );
	  break;
	case GSE_GeometryData::DELETE_VBO_CACHE:
	  glDeleteBuffersARB( 1, &(pGeometryData->Cache( nCacheKey )));
	  pGeometryData->Cache( nCacheKey ) = 0;
	  pGeometryData->SetCacheMethod( nCacheKey, GSE_GeometryData::NO_CACHE );
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
        std::list<GSE_IndexBuffer::RenderBatch_t>::iterator it =     \
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
GSE_OglRenderer::DrawPrimitive( GSE_IndexBuffer *pIndexBuffer )
{
  
  // To see later was the number of vertices in primitive valid
  char bValidPrimitive = 0;
  if ( pIndexBuffer == NULL )
  {
    GSE_ERR("Can't handle NULL's");
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
    GSE_ERR("I don't know how to draw primitive with " 
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
   
    case GSE_IndexBuffer::NO_CACHE:
      glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 );
      DRAW_RENDER_BATCHES();
      break;
    case GSE_IndexBuffer::DELETE_VBO_CACHE:
      glDeleteBuffersARB(1, &(pIndexBuffer->Cache()));
      glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 );      
      DRAW_RENDER_BATCHES();
      pIndexBuffer->SetCacheMethod(GSE_IndexBuffer::NO_CACHE);
      break;
    case GSE_IndexBuffer::REQUEST_VBO_CACHE:
      
      glGenBuffersARB( 1, &(pIndexBuffer->Cache()) );
      glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, pIndexBuffer->Cache());
      glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(unsigned int)*pIndexBuffer->m_nNumIndices, 
		       pIndexBuffer->m_pIndices, GL_STATIC_DRAW_ARB);
      ////////////////////
      // Prepare for the case if the buffer does not fit.
      if ( glGetError() == GL_OUT_OF_MEMORY )
      {
	GSE_ERR(__PRETTY_FUNCTION__ << "Couldn't create VBO for indexbuffer.");
	glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 );
	glDeleteBuffersARB(1, &(pIndexBuffer->Cache()));
	DRAW_RENDER_BATCHES();
	pIndexBuffer->SetCacheMethod( GSE_IndexBuffer::NO_CACHE );
      }
      else 
      {
	glDrawElements( iPrimitive, pIndexBuffer->m_nDrawIndices,
			GL_UNSIGNED_SHORT, 0);
	pIndexBuffer->SetCacheMethod( GSE_IndexBuffer::CACHED_IN_VBO);
      }
      glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
      break;
    case GSE_IndexBuffer::CACHED_IN_VBO:
      glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, pIndexBuffer->Cache());
      glDrawElements( iPrimitive, 
		      pIndexBuffer->m_nDrawIndices,
		      GL_UNSIGNED_SHORT,
		      0);
      glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
      break;
    default:
      GSE_WARN(__PRETTY_FUNCTION__ << "Unhandled state!");
      break;
    }
  }
}

/////////////////////////////////////////////////////////////////
void
GSE_OglRenderer::SetupVertexAttrib( GSE_VertexAttribStruct *pVertexAttrib )
{
  if ( pVertexAttrib == NULL )
  {
    GSE_ERR( __PRETTY_FUNCTION__ << "Can't handle NULL's");
    return;
  }
  if ( m_pFeatures->HasVertexProgram() )
  {
    pVertexAttrib->Activate();
  }
  
}
/////////////////////////////////////////////////////////////////
void 
GSE_OglRenderer::FinishVertexAttrib( GSE_VertexAttribStruct *pVertexAttrib )
{

  if ( pVertexAttrib == NULL )
  {
    GSE_ERR( __PRETTY_FUNCTION__ << "Can't handle NULL's");
    return;
  }
  if ( m_pFeatures->HasVertexProgram())
  {
    pVertexAttrib->Deactivate();
  }
  
}
/////////////////////////////////////////////////////////////////
void 
GSE_OglRenderer::Handle_ParticleSystemUpdate( GSE_ParticleSystemNode *pNode )
{

  
  // For this method, a camera is required so faces can be aligned correctly
  if ( m_pCurrentCamera != NULL )
  {
    
    /* if there's no particlesystem, updating isn't necessary either */
    GSE_ParticleSystemBase *ps = pNode->GetParticleSystem();
    if ( ps == NULL || !ps->IsAlive() ) return;

    GSE_Vector3 vX,vY;
    GSE_Vector3 vRight   = m_pCurrentCamera->GetRightVector();
    GSE_Vector3 vUp      = m_pCurrentCamera->GetUpVector();
    GSE_GeometryData *pGeometryData = NULL;    
    GSE_Vector3 pos;
    GSE_Vector3 vNormal;

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
      GSE_ERR("GeometryData is not initialized!");
      return;      
    }
    // Updating is needed only for the alive particles.
    unsigned int nNumParticles = ps->GetAliveCount();
    const GSE_Particle *pParticles = ps->GetParticles();
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
      
      
      //GSE_Logger::Error() << "particle " << p << std::endl;

      const GSE_Particle *particle = &(pParticles[p]);
      fWidth  = particle->m_fSize;
      fHeight = particle->m_fSize;
      
      vX = fWidth * vRight;
      //vX.m_pValues[GSE_Vector3::X] = fWidth*vRight.m_pValues[GSE_Vector3::X];
      //vX.m_pValues[GSE_Vector3::Y] = fWidth*vRight.m_pValues[GSE_Vector3::Y];
      //vX.m_pValues[GSE_Vector3::Z] = fWidth*vRight.m_pValues[GSE_Vector3::Z];
      vY = fHeight * vUp;
      //vY.m_pValues[GSE_Vector3::X] = fHeight*vUp.m_pValues[GSE_Vector3::X];
      //vY.m_pValues[GSE_Vector3::Y] = fHeight*vUp.m_pValues[GSE_Vector3::Y];
      //vY.m_pValues[GSE_Vector3::Z] = fHeight*vUp.m_pValues[GSE_Vector3::Z];  
      //vX = GSE_Vector3::GetWorldX();
      //vY = GSE_Vector3::GetWorldY();

      //
      // Assign values to the vertex buffer
      // pos = particle.m_vPosition - (vX*0.5) - (vY*0.333);
      //
      // Inlining does a little help - optimizing by hand appears to be more efficient
      // 
      //if ( ps->m_ParticleBase.m_iParticlePrimitive == GSE_ParticleBase::TRIANGLE ){
      
      //pos.m_pValues[GSE_Vector3::X] = particle->m_vPosition.m_pValues[GSE_Vector3::X] - vX.m_pValues[GSE_Vector3::X] * 0.66f - vY.m_pValues[GSE_Vector3::X] * 0.33f;
      //pos.m_pValues[GSE_Vector3::Y] = particle->m_vPosition.m_pValues[GSE_Vector3::Y] - vX.m_pValues[GSE_Vector3::Y] * 0.66f - vY.m_pValues[GSE_Vector3::Y] * 0.33f;
      //pos.m_pValues[GSE_Vector3::Z] = particle->m_vPosition.m_pValues[GSE_Vector3::Z] - vX.m_pValues[GSE_Vector3::Z] * 0.66f - vY.m_pValues[GSE_Vector3::Z] * 0.33f;

      pos = particle->m_vPosition - vX * 0.66f;
      pos -= (vY * 0.33f);
      /*} else {

       pos.m_pValues[GSE_Vector3::X] = particle->m_vPosition.m_pValues[GSE_Vector3::X] - vX.m_pValues[GSE_Vector3::X] * 0.5 - vY.m_pValues[GSE_Vector3::X] * 0.5;
       pos.m_pValues[GSE_Vector3::Y] = particle->m_vPosition.m_pValues[GSE_Vector3::Y] - vX.m_pValues[GSE_Vector3::Y] * 0.5 - vY.m_pValues[GSE_Vector3::Y] * 0.5;
       pos.m_pValues[GSE_Vector3::Z] = particle->m_vPosition.m_pValues[GSE_Vector3::Z] - vX.m_pValues[GSE_Vector3::Z] * 0.5 - vY.m_pValues[GSE_Vector3::Z] * 0.5;

       }*/
      //pGeometryData->SetVertex(nVertIndex,   pos);
      pGeometryData->m_pVertices[(nVertIndex_By3)]   = pos.m_pValues[GSE_Vector3::X];
      pGeometryData->m_pVertices[(nVertIndex_By3)+1] = pos.m_pValues[GSE_Vector3::Y];
      pGeometryData->m_pVertices[(nVertIndex_By3)+2] = pos.m_pValues[GSE_Vector3::Z];

      // pos += vX;
      pos.m_pValues[GSE_Vector3::X] += vX.m_pValues[GSE_Vector3::X];
      pos.m_pValues[GSE_Vector3::Y] += vX.m_pValues[GSE_Vector3::Y];
      pos.m_pValues[GSE_Vector3::Z] += vX.m_pValues[GSE_Vector3::Z];
      
      //pGeometryData->SetVertex(nVertIndex+1, pos);
      pGeometryData->m_pVertices[(nVertIndex_p1By3)]   = pos.m_pValues[GSE_Vector3::X];
      pGeometryData->m_pVertices[(nVertIndex_p1By3)+1] = pos.m_pValues[GSE_Vector3::Y];
      pGeometryData->m_pVertices[(nVertIndex_p1By3)+2] = pos.m_pValues[GSE_Vector3::Z];
      // pos += vY;
      pos.m_pValues[GSE_Vector3::X] += vY.m_pValues[GSE_Vector3::X];
      pos.m_pValues[GSE_Vector3::Y] += vY.m_pValues[GSE_Vector3::Y];
      pos.m_pValues[GSE_Vector3::Z] += vY.m_pValues[GSE_Vector3::Z];
      
      //pGeometryData->SetVertex(nVertIndex+2, pos);
      pGeometryData->m_pVertices[nVertIndex_p2By3]   = pos.m_pValues[GSE_Vector3::X];
      pGeometryData->m_pVertices[nVertIndex_p2By3+1] = pos.m_pValues[GSE_Vector3::Y];
      pGeometryData->m_pVertices[nVertIndex_p2By3+2] = pos.m_pValues[GSE_Vector3::Z];
      vNormal = m_pCurrentCamera->GetPosition() - particle->m_vPosition;
      //vNormal.m_pValues[GSE_Vector3::X] = (m_pCurrentCamera->GetPosition()).m_pValues[GSE_Vector3::X] - particle->m_vPosition.m_pValues[GSE_Vector3::X];
      //vNormal.m_pValues[GSE_Vector3::Y] = (m_pCurrentCamera->GetPosition()).m_pValues[GSE_Vector3::Y] - particle->m_vPosition.m_pValues[GSE_Vector3::Y];
      //vNormal.m_pValues[GSE_Vector3::Z] = (m_pCurrentCamera->GetPosition()).m_pValues[GSE_Vector3::Z] - particle->m_vPosition.m_pValues[GSE_Vector3::Z];

      //pGeometryData->SetNormal( nVertIndex,   vNormal );
      //pGeometryData->SetNormal( nVertIndex+1, vNormal );
      //pGeometryData->SetNormal( nVertIndex+2, vNormal );

      pGeometryData->m_pNormals[ (nVertIndex_By3)  ] = vNormal.m_pValues[GSE_Vector3::X];
      pGeometryData->m_pNormals[ (nVertIndex_By3)+1] = vNormal.m_pValues[GSE_Vector3::Y];
      pGeometryData->m_pNormals[ (nVertIndex_By3)+2] = vNormal.m_pValues[GSE_Vector3::Z];
      
      pGeometryData->m_pNormals[ (nVertIndex_p1By3)  ] = vNormal.m_pValues[GSE_Vector3::X];
      pGeometryData->m_pNormals[ (nVertIndex_p1By3)+1] = vNormal.m_pValues[GSE_Vector3::Y];
      pGeometryData->m_pNormals[ (nVertIndex_p1By3)+2] = vNormal.m_pValues[GSE_Vector3::Z];

      pGeometryData->m_pNormals[ nVertIndex_p2By3  ] = vNormal.m_pValues[GSE_Vector3::X];
      pGeometryData->m_pNormals[ nVertIndex_p2By3+1] = vNormal.m_pValues[GSE_Vector3::Y];
      pGeometryData->m_pNormals[ nVertIndex_p2By3+2] = vNormal.m_pValues[GSE_Vector3::Z];
      
      pGeometryData->SetColor( p * 3, particle->m_vColor );
      pGeometryData->SetColor( 1 + p * 3, particle->m_vColor );
      pGeometryData->SetColor( 2 + p * 3, particle->m_vColor );
     //  // The fourth vertex will be update only if we have quad
//       if ( ps->m_ParticleBase.m_iParticlePrimitive == GSE_ParticleBase::QUAD )
//       {
// 	// pos -= vX;
// 	pos.m_pValues[GSE_Vector3::X] -= vX.m_pValues[GSE_Vector3::X];
// 	pos.m_pValues[GSE_Vector3::Y] -= vX.m_pValues[GSE_Vector3::Y];
// 	pos.m_pValues[GSE_Vector3::Z] -= vX.m_pValues[GSE_Vector3::Z];
// 	// pGeometryData->SetVertex(nVertIndex+3, pos);
// 	pGeometryData->m_pVertices[(nVertIndex_p3By3)]   = pos.m_pValues[GSE_Vector3::X];
// 	pGeometryData->m_pVertices[(nVertIndex_p3By3)+1] = pos.m_pValues[GSE_Vector3::Y];
// 	pGeometryData->m_pVertices[(nVertIndex_p3By3)+2] = pos.m_pValues[GSE_Vector3::Z];      
// 	// pGeometryData->SetNormal( nVertIndex+3, vNormal );    
// 	pGeometryData->m_pNormals[ (nVertIndex_p3By3) ]   = vNormal.m_pValues[GSE_Vector3::X];
// 	pGeometryData->m_pNormals[ (nVertIndex_p3By3) +1] = vNormal.m_pValues[GSE_Vector3::Y];
// 	pGeometryData->m_pNormals[ (nVertIndex_p3By3) +2] = vNormal.m_pValues[GSE_Vector3::Z];

//       }
    }
  }
}
/////////////////////////////////////////////////////////////////
void 
GSE_OglRenderer::ApplyGlobalLighting( GSE_GlobalLightingNode *pNode )
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
GSE_OglRenderer::SwapBuffers()
{
  // ATI driver messes up lighting if glTexEnv is set to 
  // GL_REPLACE and not reverted to GL_MODULATE
  glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
  SDL_GL_SwapBuffers();
}
/////////////////////////////////////////////////////////////////
void
GSE_OglRenderer::SetSelectionName( GSE_SelectionNameNode *pSelectionNameNode )
{
  glLoadName( pSelectionNameNode->GetSelectionName() );
}
/////////////////////////////////////////////////////////////////
void
GSE_OglRenderer::RenderText( GSE_TextNode *pTextNode )
{
  
  // If fontsets exist, we can do something
  if ( m_vFontsets.size() == 0 )
  {
    GSE_Logger::Error() << DEBUG_HEADER << "Error: No Fontsets created!" << endl;
    return;
  }
  if ( pTextNode->GetString() == NULL )
  {
    GSE_Logger::Error() << DEBUG_HEADER << "I shall not render a NULL string!" << endl;
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
GSE_OglRenderer::Handle_DebugPrint_RenderPass_Enter( GSE_GraphNode<NodeType> *pNode )
{
  int bCulled = 0;
  GSE_Logger::Error() << m_sPadding <<  "|" << std::endl;
  
  switch ( pNode->GetType() )
  {
  case OCTREE:
    GSE_Logger::Error() << m_sPadding <<  m_sArrow <<  "Octree"  << std::endl;
    break;
  case TRANSFORM:
    GSE_Logger::Error() << m_sPadding <<  m_sArrow <<  "Transform"  << std::endl;
    //GSE_Logger::Error() << ((GSE_TransformNode *)pNode)->GetMatrix();
    break;
  case GEOMETRY:
    GSE_Logger::Error() << m_sPadding <<  m_sArrow <<  "Geometry" ;
    break;
  case CAMERA:
    GSE_Logger::Error() << m_sPadding<<  m_sArrow <<  "Camera"  << endl;
    //GSE_Logger::Error() << ((GSE_CameraNode *)pNode)->GetCamera()->GetView();
    break;
  case GROUP:
    GSE_Logger::Error() << m_sPadding <<  m_sArrow <<  "Group" ;
    break;
  case LIGHT_CLUSTER:
    GSE_Logger::Error() << m_sPadding <<  m_sArrow <<  "LightCluster" ;
    break;
  case FRONT_MATERIAL:
    GSE_Logger::Error() << m_sPadding <<  m_sArrow <<  "FrontMaterial" ;
    break;
  case BACK_MATERIAL:
    GSE_Logger::Error() << m_sPadding <<  m_sArrow <<  "BackMaterial" ;
    break;
  case INDEXBUFFER:
    GSE_Logger::Error() << m_sPadding <<  m_sArrow <<  "IndexBuffer" ;
    break;
  case RENDERSTATE_CULLING:
    GSE_Logger::Error() << m_sPadding <<  m_sArrow <<  "RenderStateCulling" ;
    break;
  case SHADER:
    GSE_Logger::Error() << m_sPadding <<  m_sArrow <<  "Shader" ;
    break;
  case VERTEXATTRIB:
    GSE_Logger::Error() << m_sPadding <<  m_sArrow <<  "VertexAttrib" ;
    break;
  case GLOBAL_LIGHTING:
    GSE_Logger::Error() << m_sPadding <<  m_sArrow <<  "GlobalLighting" ;
    break;
  case PARTICLESYSTEM:
    GSE_Logger::Error() << m_sPadding <<  m_sArrow <<  "ParticleSystem" ;
    break;
  case CACHE:
    GSE_Logger::Error() << m_sPadding <<  m_sArrow <<  "Display list" ;
    break;
  case SELECTION:
    GSE_Logger::Error() << m_sPadding <<  m_sArrow <<  "Selection name" ;
    break;
  case TEXT:
    GSE_Logger::Error() << m_sPadding <<  m_sArrow <<  "Text string" ;
    break;
  case COLOR:
    GSE_Logger::Error() << m_sPadding <<  m_sArrow <<  "Color" ;
    break;
  case ALPHA_TEST:
    GSE_Logger::Error() << m_sPadding <<  m_sArrow <<  "AlphaTest" ;
    break;
  case RENDERSTATE_BLEND:
    GSE_Logger::Error() << m_sPadding <<  m_sArrow <<  "BlendingState" ;
    break;
  case RENDERSTATE_DEPTHBUFFER:
    GSE_Logger::Error() << m_sPadding <<  m_sArrow <<  "DepthBufferState" ;
    break;
  case RENDERSTATE_STACK:
    GSE_Logger::Error() << m_sPadding <<  m_sArrow <<  "RenderStateStack" ;
    break;
  case TEXTURE:  
    GSE_Logger::Error() << m_sPadding <<  m_sArrow <<  "Texture" ;
    break;
  case SCISSOR:
    GSE_Logger::Error() << m_sPadding <<  m_sArrow <<  "ScissorTest" ;
    break;
  case CLEARBUFFER:
    GSE_Logger::Error() << m_sPadding <<  m_sArrow <<  "ClearBuffer" ;
    break;
  case SHADEMODEL:
    GSE_Logger::Error() << m_sPadding <<  m_sArrow <<  "ShadeModel" ;
    break;
  case POLYLINE:
    GSE_Logger::Error() << m_sPadding <<  m_sArrow <<  "PolyLine" ;
    break;
  case KDOP:
    GSE_Logger::Error() << m_sPadding <<  m_sArrow <<  "KDOP" ;
    break;
  case UNDEFINED:
    GSE_Logger::Error() << m_sPadding <<  m_sArrow <<  "Undefined" ;
    break;
  case UTIL_SPHERE:
    GSE_Logger::Error() << m_sPadding <<  m_sArrow <<  "Sphere" ;
    break;
  case UTIL_BOX:
    GSE_Logger::Error() << m_sPadding <<  m_sArrow <<  "Box" ;
    break;
  case UTIL_CONE:
    GSE_Logger::Error() << m_sPadding <<  m_sArrow <<  "Cone" ;
    break;
  case UTIL_LINESTRIP:
    GSE_Logger::Error() << m_sPadding <<  m_sArrow <<  "LineStrip" ;
    break;
  case ROTATION:
    GSE_Logger::Error() << m_sPadding <<  m_sArrow <<  "Rotation" ;
    break;
  case TRANSLATION:
    GSE_Logger::Error() << m_sPadding <<  m_sArrow <<  "Translation" ;
    break;
  case ROOT:
    GSE_Logger::Error() << m_sPadding <<  m_sArrow <<  "RenderRoot" ;
    break;
  case UTIL_QUAD:  
    GSE_Logger::Error() << m_sPadding <<  m_sArrow <<  "UtilQuad" ;
    break;
  case POLYGONMODE:  
    GSE_Logger::Error() << m_sPadding <<  m_sArrow <<  "PolygonMode" ;
    break;
  }
  GSE_Logger::Error() << "(" << pNode << ")" << endl;
  
  m_sPadding.append(1,' ');
  m_sPadding.append(2,' ');
  return bCulled;
}
/////////////////////////////////////////////////////////////////
int 
GSE_OglRenderer::Handle_DebugPrint_RenderPass_Leave( GSE_GraphNode<NodeType> *pNode )
{
  int iRetval = 0;
  m_sPadding.erase(0,3);
  return iRetval;
}
/////////////////////////////////////////////////////////////////
int
GSE_OglRenderer::HandleOglCache_Enter( GSE_OglCacheNode *pNode )
{

  
  int bCulled = 0;
 
  if ( pNode == NULL ){
    GSE_Logger::Error() << "Can't handle NULLs!" << std::endl;
    return 0;
  }

  GSE_OglCache *pCache = pNode->GetCache();

  if ( pCache == NULL )
  {
    GSE_Logger::Error() << "Can't handle NULL caches." << std::endl;
    return 0;
  }
  switch ( pCache->GetCacheMethod())
  {
  case GSE_OglCache::NO_CACHE:
    bCulled = 0;
    break;
  case GSE_OglCache::CACHED_IN_DL:
    glCallList( pCache->Cache());
    bCulled = 1; 
    break;
  case GSE_OglCache::REGENERATE_DL_CACHE:
    if ( !IS_COMPILING_DISPLAY_LIST()){
      glDeleteLists(pCache->Cache(), 1);
      break;
    } 
  case GSE_OglCache::REQUEST_DL_CACHE:
    if ( IS_COMPILING_DISPLAY_LIST())
    {
      break;
    }

    pCache->Cache()  = glGenLists( 1 );
    if ( pCache->Cache() == 0 )
    {
      GSE_Logger::Error() << "Couldn't create display list!" << std::endl;
      
      pCache->SetCacheMethod( GSE_OglCache::NO_CACHE );
    } 
    else 
    {
      glNewList( pCache->Cache(), GL_COMPILE );
    }
    bCulled = 0;
    m_DisplayListStack.push('o');
    break;
  case GSE_OglCache::DELETE_DL_CACHE:
    if ( !IS_COMPILING_DISPLAY_LIST()){
      glDeleteLists(pCache->Cache(), 1);
      pCache->SetCacheMethod( GSE_OglCache::NO_CACHE );
    }
    break;
  }
  
  
  return bCulled;
}
/////////////////////////////////////////////////////////////////
int 
GSE_OglRenderer::HandleOglCache_Leave( GSE_OglCacheNode *pNode )
{
  int bRetval = 0;
  if ( pNode == NULL )
  {
    GSE_WARN("Can't handle NULLs!");
    return 0;
  }

  GSE_OglCache *pCache = pNode->GetCache();

  if ( pCache == NULL )
  {
    GSE_WARN("Can't handle NULL caches.");
    return 0;
  }
  switch ( pCache->GetCacheMethod())
  {
  case GSE_OglCache::NO_CACHE:
  case GSE_OglCache::CACHED_IN_DL:
  case GSE_OglCache::DELETE_DL_CACHE:
    break;
  case GSE_OglCache::REQUEST_DL_CACHE:
  case GSE_OglCache::REGENERATE_DL_CACHE:
    // end the GL_COMPILE
    glEndList();
    GSE_DEBUG("glEndList( );");
    // Change cache message
    pCache->SetCacheMethod( GSE_OglCache::CACHED_IN_DL );
    // Call list so everything will be drawn also in this pass
    GSE_DEBUG("glCallList( " << pCache->Cache() << ");");
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
GSE_OglRenderer::OptimizeVBOBatching( GSE_IndexBuffer *pIndexBuffer )
{
  
  if ( pIndexBuffer == NULL )
  {
    GSE_ERR( __PRETTY_FUNCTION__ << "Can't handle NULL's.");
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

  GSE_IndexBuffer::RenderBatch_t renderBatch;

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
  GSE_DEBUG("Renderbatches have been issued, size: " 
	    << pIndexBuffer->RenderBatches().size());

  
}
/////////////////////////////////////////////////////////////////
/// Handles the render passes of the normal drawing process.
int 
GSE_OglRenderer::Handle_Drawing_RenderPass_Enter( GSE_GraphNode<NodeType> *pNode )
{
  
  int iCulled = 0;
  GLUquadric *quadric = NULL;
  
  float *pTmp = NULL;
  
  if ( pNode == NULL )
  {
    GSE_DEBUG("Can't handle NULL's!");
    iCulled = 1;
  } 
  else 
  {
    ////////////////////
    // The normal rendering pass
    switch ( pNode->GetType() )
    {
    case OCTREE:
      DrawOctree( static_cast<GSE_OctreeNode *>(pNode)->GetOctree());
      break;
    case POLYLINE:
      DrawPolyline( static_cast<GSE_PolyLineNode *>(pNode) );
      break;
    case TEXTURE:
      iCulled = HandleTexture( static_cast<GSE_TextureNode *>(pNode) );
      break;
    case TRANSFORM:
      glPushMatrix();
      glMultMatrixf( (static_cast<GSE_TransformNode *>(pNode))->GetMatrix().Transposed().GetArray()) ;
      break;
    case GEOMETRY: 
      SetupArrays( ( static_cast<GSE_GeometryNode *>(pNode))->GetGeometryDataWorld() );
      break;
    case CAMERA:
      ApplyPerspective( ( static_cast<GSE_CameraNode *>(pNode))->GetCamera());
      break;
    case GROUP:
      break;
    case LIGHT_CLUSTER: 
      ApplyLightParameters( static_cast<GSE_LightClusterNode *>(pNode) );
      break;
    case POLYGONMODE:
      {
	switch(static_cast<GSE_PolygonModeNode *>(pNode)->GetFrontFaceMode())
	{
	case GSE_PolygonModeNode::FILL:
	  glPolygonMode( GL_FRONT, GL_FILL );
	  break;  
	case GSE_PolygonModeNode::LINE:
	  glPolygonMode( GL_FRONT, GL_LINE );
	  break;  
	case GSE_PolygonModeNode::POINT:
	  glPolygonMode( GL_FRONT, GL_POINT );
	  break;  
	}
	switch(static_cast<GSE_PolygonModeNode *>(pNode)->GetBackFaceMode())
	{
	case GSE_PolygonModeNode::FILL:
	  glPolygonMode( GL_BACK, GL_FILL );
	  break;  
	case GSE_PolygonModeNode::LINE:
	  glPolygonMode( GL_BACK, GL_LINE );
	  break;  
	case GSE_PolygonModeNode::POINT:
	  glPolygonMode( GL_BACK, GL_POINT );
	  break;  
	}
      }
      break;
    case FRONT_MATERIAL:
      {
 	GSE_Material *pMat = ( static_cast<GSE_MaterialNode *>(pNode))->GetMaterial();

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
	GSE_Material *pMat = ( static_cast<GSE_MaterialNode *>(pNode))->GetMaterial();
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
      DrawPrimitive( ( static_cast<GSE_IndexBufferNode *>(pNode))->GetIndexBuffer());
      break;
    case RENDERSTATE_CULLING:
      Handle_Draw_CullingState( static_cast<GSE_CullingStateNode *>(pNode));
      break;
    case RENDERSTATE_STACK:
      // Stores the defined attribute group
      HandleStackPush( static_cast<GSE_RenderStateStackNode *>(pNode));
      break;
    case CLEARBUFFER:
      {
	
	GSE_ClearBufferNode *pClearBuf = static_cast<GSE_ClearBufferNode *>(pNode);
	if ( m_bClearBuffers )
	{
	  GSE_Color &Color = pClearBuf->ClearColor();
	  glClearColor( Color[GSE_Color::R],
			Color[GSE_Color::G],
			Color[GSE_Color::B],
			Color[GSE_Color::A]);
	  
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
      switch( ( static_cast<GSE_ShadeModelNode *>(pNode))->GetShadeModel())
      {
      case GSE_ShadeModelNode::SMOOTH:
	glShadeModel(GL_SMOOTH);
	GSE_DEBUG("glShadeModel(GL_SMOOTH);");
	break;
      case GSE_ShadeModelNode::FLAT:
	glShadeModel(GL_FLAT);
	GSE_DEBUG("glShadeModel(GL_FLAT);");
	break;
      }
      
      break;
    case SHADER:
      {
	// Check for shader support and skip node if so
	if ( !m_pFeatures->HasVertexProgram()) break;

	GSE_ShaderNode *pShaderNode = static_cast<GSE_ShaderNode *>(pNode);
	GSE_OglShaderProgram *pShaderProgram = static_cast<GSE_OglShaderProgram *>(pShaderNode->GetShaderProgram());
      
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

      SetupVertexAttrib(( static_cast<GSE_VertexAttribNode *>(pNode))->GetVertexAttrib());

      break;
    case GLOBAL_LIGHTING:
      
      ApplyGlobalLighting( static_cast<GSE_GlobalLightingNode *>(pNode));
    
      break;
    case PARTICLESYSTEM:
      
      Handle_ParticleSystemUpdate( static_cast<GSE_ParticleSystemNode *>(pNode) );
      SetupArrays( static_cast<GSE_ParticleSystemNode *>(pNode)->GetGeometryData() );
      
      break;
    case UTIL_SPHERE:
      {quadric = gluNewQuadric();
      GSE_UtilSphereNode *pSphere = static_cast<GSE_UtilSphereNode *>(pNode);

      // Set the drawstyle
      switch ( pSphere->GetDrawMode())
      {
      case GSE_UtilSphereNode::WIREFRAME:
	gluQuadricDrawStyle( quadric, GLU_LINE );
	break;
      case GSE_UtilSphereNode::SILHOUETTE:
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
      glTranslatef( pSphere->GetPosition().m_pValues[GSE_Vector3::X],
		    pSphere->GetPosition().m_pValues[GSE_Vector3::Y],
		    pSphere->GetPosition().m_pValues[GSE_Vector3::Z] );
    
      gluSphere(quadric,pSphere->GetRadius(), 16,16 );
    
      glPopMatrix();
      glPopAttrib();
      // cleanup
      gluDeleteQuadric(quadric);
      }
      break;
    case CACHE:
      iCulled = HandleOglCache_Enter( static_cast<GSE_OglCacheNode *>(pNode));
      break;
    case SELECTION:
      iCulled = 0;
      SetSelectionName( static_cast<GSE_SelectionNameNode *>(pNode));
      break;
    case TEXT:
      iCulled = 0;
      RenderText( static_cast<GSE_TextNode *>(pNode) );
      break;
    case COLOR:
      glColor4fv( ( static_cast<GSE_ColorNode *>(pNode))->m_pValues);
      iCulled = 0;
      break;
    case RENDERSTATE_DEPTHBUFFER:
      // Enable/disable depth test
      if ( static_cast<GSE_DepthBufferStateNode *>(pNode)->IsDepthTest() )   
      {	
	glEnable(GL_DEPTH_TEST);  
      } 
      else
      {	
	glDisable(GL_DEPTH_TEST); 
      }
      // Enable disable writing into the depth buffer
      if ( static_cast<GSE_DepthBufferStateNode *>(pNode)->IsDepthBufferWriting() )   
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
	GSE_AlphaTestNode *pAlphaTest = static_cast<GSE_AlphaTestNode *>(pNode);
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
	GSE_BlendingStateNode *pBlendState = static_cast<GSE_BlendingStateNode *>(pNode);
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
	GSE_ScissorTestNode *pScissorTestNode = static_cast<GSE_ScissorTestNode *>(pNode);
	
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
	list<GSE_Vector3>		lstPoints;
	list<GSE_Vector3>::iterator	pointIt;
	list<GSE_Plane>			lstTmpPlanes;

	GSE_KDopUtilNode *pKDopUtil = static_cast<GSE_KDopUtilNode *>(pNode);
	//GSE_Logger::Error() << "Rendering KDOP " << endl << pKDopUtil->GetKDOP() << endl;	
	
	if ( pKDopUtil->GetKDOP().GetNumPlanes() > 2  ) 
	{
	  lstTmpPlanes = pKDopUtil->GetKDOP().Planes();
	  lstTmpPlanes.reverse();
	  list<GSE_Plane>::iterator planeIt = lstTmpPlanes.begin();

	  GSE_Plane far = *planeIt;
	  planeIt++;
	  GSE_Plane near = *planeIt;
	  
	  lstTmpPlanes.pop_front();
	  lstTmpPlanes.pop_front();
	  
	  planeIt = lstTmpPlanes.begin();

	  // Calculate corners
	  for(; planeIt != lstTmpPlanes.end();planeIt++)
	  {
	    GSE_Plane plane1 = *planeIt;
	    list<GSE_Plane>::iterator planeItNext = planeIt;
	    planeItNext++;

	    if ( planeItNext == lstTmpPlanes.end())
	    {
	      planeItNext = lstTmpPlanes.begin();
	    }

	    GSE_Plane plane2 = *planeItNext;
	    GSE_Vector3 vTmp1;
	    
	    if ( Geometry::PlaneIntersectionPoint( plane1, plane2, near, vTmp1 ) != Geometry::POINT )
	    {
	      
	      GSE_ERR("HELL BREAKS LOOSE, kDop planes do not intersect!!! (near)");
	      //abort();
	    } 
	    else 
	    {
	      lstPoints.push_back( vTmp1 );
	    }
	    
	    if ( Geometry::PlaneIntersectionPoint( plane1, plane2, far, vTmp1 ) != Geometry::POINT )
	    {
	      GSE_ERR("HELL BREAKS LOOSE, kDop planes do not intersect (far)!!!");
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
	  //GSE_Logger::Error() << "KDOP corner winners are: " << endl;
	  for(;pointIt!=lstPoints.end();pointIt++)
	  {
	    //GSE_Logger::Error() << (*pointIt) << endl;
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
      glMultMatrixf( (static_cast<GSE_RotationNode *>(pNode))->ToMatrix().Transposed().GetArray());            
      break;
    case TRANSLATION:
      // Store modelview matrix stack (or at least it should be stored)
      GSE_DEBUG("glPushMatrix()");
      glPushMatrix();
      pTmp = (static_cast<GSE_TranslationNode *>(pNode))->m_pValues;
      glTranslatef( pTmp[0], pTmp[1], pTmp[2]  );
      break;
    case UTIL_QUAD:
      Handle_Draw_UtilQuad( static_cast<GSE_UtilQuadNode *>(pNode) );
      
      break;
    case UTIL_BOX:
      {
	GSE_UtilBoxNode *pBox = static_cast<GSE_UtilBoxNode *>(pNode);
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
	glVertex3fv( pBox->GetCorner( GSE_OrientedBox::TOP_LEFT_BACK ) );
	glVertex3fv( pBox->GetCorner( GSE_OrientedBox::BOTTOM_LEFT_BACK ) );
	glVertex3fv( pBox->GetCorner( GSE_OrientedBox::TOP_RIGHT_BACK ) );
	glVertex3fv( pBox->GetCorner( GSE_OrientedBox::BOTTOM_RIGHT_BACK ) );
	glVertex3fv( pBox->GetCorner( GSE_OrientedBox::TOP_RIGHT_FRONT ) );
	glVertex3fv( pBox->GetCorner( GSE_OrientedBox::BOTTOM_RIGHT_FRONT ) );
	glVertex3fv( pBox->GetCorner( GSE_OrientedBox::TOP_LEFT_FRONT ) );
	glVertex3fv( pBox->GetCorner( GSE_OrientedBox::BOTTOM_LEFT_FRONT ) );
	glVertex3fv( pBox->GetCorner( GSE_OrientedBox::TOP_LEFT_BACK ) );
	glVertex3fv( pBox->GetCorner( GSE_OrientedBox::BOTTOM_LEFT_BACK ) );
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
	GSE_UtilConeNode *pCone = static_cast<GSE_UtilConeNode *>(pNode);

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
      
	glTranslatef( pCone->GetPosition().m_pValues[GSE_Vector3::X],
		      pCone->GetPosition().m_pValues[GSE_Vector3::Y],
		      pCone->GetPosition().m_pValues[GSE_Vector3::Z] );
	
	float fAngle = acosf(pCone->GetDirection().Dot(GSE_Vector3::GetWorldZ()) / 
			     pCone->GetDirection().Length());
	//GSE_Logger::Error() << "angle between " << pCone->GetDirection() << " and " << GSE_Vector3::GetWorldZ() << " is " << fAngle << std::endl;
	GSE_Vector3 vRotationVector = GSE_Vector3::GetWorldZ().Cross(pCone->GetDirection());
	glRotatef( RAD2DEG(fAngle), 
		   vRotationVector[GSE_Vector3::X],
		   vRotationVector[GSE_Vector3::Y],
		   vRotationVector[GSE_Vector3::Z]);
	
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
	GSE_UtilLineStripNode *pLineStripNode = static_cast<GSE_UtilLineStripNode *>(pNode);
	glPushAttrib( GL_ENABLE_BIT );
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glLineWidth(2.0);
	glColor3f( 1.0, 1.0, 1.0); // white
	glBegin( GL_LINE_STRIP );
	std::list<GSE_Vector3>::iterator it = pLineStripNode->Vertices().begin();
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
      GSE_Logger::Error() << "Node type is undefined!" << std::endl;
      break;
    }
  }

  return iCulled;
}
/////////////////////////////////////////////////////////////////
/// Handles the finish passes of the normal drawing process.
int 
GSE_OglRenderer::Handle_Drawing_RenderPass_Leave( GSE_GraphNode<NodeType> *pNode )
{
  int iRetval = 0;
  
  switch ( pNode->GetType() )
  {
  case ROTATION:
  case TRANSLATION:
  case TRANSFORM:
    // Pop the previously stored matrix 
    GSE_DEBUG("glPopMatrix();");
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
    DisableMaterial( static_cast<GSE_MaterialNode *>(pNode)->GetMaterial() );
    break;
  case SHADER:
    // revert to fixed functionality
    glUseProgram( 0 );
    glPopAttrib();
    break;
  case VERTEXATTRIB:
    FinishVertexAttrib( static_cast<GSE_VertexAttribNode *>(pNode)->GetVertexAttrib());
    break;
  case CACHE:
    iRetval = HandleOglCache_Leave( static_cast<GSE_OglCacheNode *>(pNode) );
    break;
  case RENDERSTATE_STACK:
    // pops the stored attribute group
    glPopAttrib();
    break;
  case TEXTURE:
    {
      GSE_TextureNode *pTexNode = static_cast<GSE_TextureNode *>(pNode);
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
      GSE_BlendingStateNode *pBlendState = static_cast<GSE_BlendingStateNode *>(pNode);
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
    GSE_Logger::Error() << "Node type is undefined!" << std::endl;
    break;
  }
  return iRetval;
}
/////////////////////////////////////////////////////////////////
void
GSE_OglRenderer::SetSelection ( unsigned int nMouseX, unsigned int nMouseY, 
				unsigned int nWidth , unsigned int nHeight  )
{
  m_Selection.m_nMouseX = nMouseX;
  m_Selection.m_nMouseY = nMouseY;
  m_Selection.m_nWidth = nWidth;
  m_Selection.m_nHeight = nHeight;

}
/////////////////////////////////////////////////////////////////
GLuint *
GSE_OglRenderer::GetSelectionBuffer()
{
  return m_aSelectionBuffer;
}
/////////////////////////////////////////////////////////////////
const unsigned int 
GSE_OglRenderer::GetNumHits()
{
  return m_nHits;
}
/////////////////////////////////////////////////////////////////
int 
GSE_OglRenderer::CreateFontset( const char *sPathToFontFile, unsigned int nFontSize)
{

#define WHITESPACE 32
  GSE_OglRendererFontset *pFontset;

  FT_Library ftLibrary;
  FT_Error ftError;
  FT_Face  ftFace;  

  GSE_OglTextureMgr *pOglTextureMgr = GSE_OglTextureMgr::GetInstance();

  // Initialize the font library
  ftError = FT_Init_FreeType( &ftLibrary ); 
  if ( ftError ) 
  { 
    GSE_ERR("Error initializing FT");
    return 1;
  }
  // Create new face
  ftError = FT_New_Face( ftLibrary,sPathToFontFile, 0, &ftFace);
  if ( ftError )
  {
    GSE_ERR("Error loading face");
    return 1;
  }
  // Set the character size
  ftError = FT_Set_Char_Size( ftFace, nFontSize * 64, 0, 0, 0 );        
  
  if ( ftError )
  {
    GSE_ERR("Error setting font size");
    return 1;
  }
  
  // create textures
  // glGenTextures( MAX_CHARS, Textures );
  
  pFontset = new GSE_OglRendererFontset();
  // create display lists 
  pFontset->DisplayList() = glGenLists(GSE_MAX_FONT_CHARACTERS);
  
  for(unsigned int n=0;n<GSE_MAX_FONT_CHARACTERS;n++)
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
    GSE_OglTexture *pTexture = new GSE_OglTexture();
    pTexture->SetType(TextureType::GSE_TEX2D);
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
    glEnable( GL_TEXTURE_2D);
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
std::vector<GSE_OglRendererFontset *> &
GSE_OglRenderer::Fontsets()
{
  return m_vFontsets;
}
/////////////////////////////////////////////////////////////////
void 
GSE_OglRenderer::HandleStackPush( GSE_RenderStateStackNode *pNode )
{
  
  GLenum eBits = 0;

  if ( pNode->IsStored( GSE_RenderStateStackNode::ACCUM_BUFFER ))	eBits |= GL_ACCUM_BUFFER_BIT;
  if ( pNode->IsStored( GSE_RenderStateStackNode::COLOR_BUFFER ))	eBits |= GL_COLOR_BUFFER_BIT;
  if ( pNode->IsStored( GSE_RenderStateStackNode::CURRENT ))		eBits |= GL_CURRENT_BIT;
  if ( pNode->IsStored( GSE_RenderStateStackNode::DEPTH_BUFFER ))	eBits |= GL_DEPTH_BUFFER_BIT;
  if ( pNode->IsStored( GSE_RenderStateStackNode::ENABLE ))		eBits |= GL_ENABLE_BIT;
  if ( pNode->IsStored( GSE_RenderStateStackNode::EVAL ))		eBits |= GL_EVAL_BIT;
  if ( pNode->IsStored( GSE_RenderStateStackNode::FOG ))		eBits |= GL_FOG_BIT;
  if ( pNode->IsStored( GSE_RenderStateStackNode::HINT ))		eBits |= GL_HINT_BIT;
  if ( pNode->IsStored( GSE_RenderStateStackNode::LIGHTING ))	eBits |= GL_LIGHTING_BIT;
  if ( pNode->IsStored( GSE_RenderStateStackNode::LINE ))		eBits |= GL_LINE_BIT;
  if ( pNode->IsStored( GSE_RenderStateStackNode::LIST ))		eBits |= GL_LIST_BIT;
  if ( pNode->IsStored( GSE_RenderStateStackNode::PIXEL_MODE ))	eBits |= GL_PIXEL_MODE_BIT;
  if ( pNode->IsStored( GSE_RenderStateStackNode::POINT ))		eBits |= GL_POINT_BIT;
  if ( pNode->IsStored( GSE_RenderStateStackNode::POLYGON ))		eBits |= GL_POLYGON_BIT;
  if ( pNode->IsStored( GSE_RenderStateStackNode::POLYGON_STIPPLE )) eBits |= GL_POLYGON_STIPPLE_BIT;
  if ( pNode->IsStored( GSE_RenderStateStackNode::SCISSOR ))		eBits |= GL_SCISSOR_BIT;
  if ( pNode->IsStored( GSE_RenderStateStackNode::STENCIL_BUFFER ))  eBits |= GL_STENCIL_BUFFER_BIT;
  if ( pNode->IsStored( GSE_RenderStateStackNode::TEXTURE ))		eBits |= GL_TEXTURE_BIT;
  if ( pNode->IsStored( GSE_RenderStateStackNode::TRANSFORM ))	eBits |= GL_TRANSFORM_BIT;
  if ( pNode->IsStored( GSE_RenderStateStackNode::VIEWPORT ))	eBits |= GL_VIEWPORT_BIT;

  glPushAttrib( eBits );
    
}
/////////////////////////////////////////////////////////////////
void 
GSE_OglRenderer::Handle_Draw_UtilQuad( GSE_UtilQuadNode *pNode )
{

  GSE_Vector3 vPos = pNode->GetPosition() -
  (GSE_Vector3::GetWorldY() * pNode->GetHalfHeight()) -
  (GSE_Vector3::GetWorldX() * pNode->GetHalfWidth());


  glPushMatrix();
  glBegin(GL_QUADS);
       glTexCoord2f(pNode->GetTexCoord(0), pNode->GetTexCoord(1)); 
       glVertex3fv(vPos.m_pValues);

       vPos += GSE_Vector3::GetWorldX() * pNode->GetWidth();
       glTexCoord2f(pNode->GetTexCoord(2), pNode->GetTexCoord(3)); 
       glVertex3fv(vPos.m_pValues);
       
       vPos += GSE_Vector3::GetWorldY() * pNode->GetHeight();
       glTexCoord2f(pNode->GetTexCoord(4), pNode->GetTexCoord(5) ); 
       glVertex3fv(vPos.m_pValues);
	
       vPos -= GSE_Vector3::GetWorldX() * pNode->GetWidth();
       glTexCoord2f(pNode->GetTexCoord(6), pNode->GetTexCoord(7)); 
       glVertex3fv(vPos.m_pValues);
  glEnd();
  glPopMatrix();
    
}
/////////////////////////////////////////////////////////////////
void 
GSE_OglRenderer::Handle_Draw_CullingState( GSE_CullingStateNode *pCullState )
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
GSE_OglRenderer::DrawPolyline( GSE_PolyLineNode * pNode )
{
  // This would not produce any graphics anyway
  if ( pNode->GetPointCount() < 2 ) return;

  // This can also be done with shaders (probably more efficiently)
  
  glPushAttrib(GL_POLYGON_BIT);
  glDisable(GL_CULL_FACE);
  // T_0 = Z_0 X ( P_1 - P_0 )
  // T_k = Z_k X ( P_k-1 - P_k+1 ) | k in [1,n-1]
  // T_n = - (Z_n X ( P_n-1 - P_n ))

  GSE_PolyLineNode &poly = (*pNode);
  unsigned int nP = 1;
  unsigned int nCount = poly.GetPointCount()-1;
  float    fTexCoordFraction = 1.0f / pNode->GetPointCount();
  float    fTexCoordY = 0.0f;
  float    fWidth = poly.GetWidth() * 0.5f;

  GSE_Vector3 vTangent = ( (poly[1] - poly[0]).Cross( m_pCurrentCamera->GetPosition()-poly[0])).ToUnit() * 0.5f * fWidth;
  
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
void DrawOctreeRecursive( GSE_GeometryOctree *pOctree )
{
  if ( pOctree == NULL )
  {
    return;
  }
  glBegin(GL_QUAD_STRIP);

  GSE_Vector3 vVertex = pOctree->GetPosition();
  // 1st
  vVertex[GSE_Vector3::X] -= pOctree->GetWidth() *0.5f;
  vVertex[GSE_Vector3::Y] += pOctree->GetHeight()*0.5f;
  vVertex[GSE_Vector3::Z] += pOctree->GetLength()*0.5f;
  glVertex3fv( vVertex.m_pValues );
  // 2nd
  vVertex[GSE_Vector3::Y] -= pOctree->GetHeight();       
  glVertex3fv( vVertex.m_pValues );
  // 3rd
  vVertex[GSE_Vector3::Y] += pOctree->GetHeight();
  vVertex[GSE_Vector3::X] += pOctree->GetWidth();
  glVertex3fv( vVertex.m_pValues );
  //4th
  vVertex[GSE_Vector3::Y] -= pOctree->GetHeight();       
  glVertex3fv( vVertex.m_pValues );
  // 5th
  vVertex[GSE_Vector3::Y] += pOctree->GetHeight();
  vVertex[GSE_Vector3::Z] -= pOctree->GetLength();
  glVertex3fv( vVertex.m_pValues );
  // 6th
  vVertex[GSE_Vector3::Y] -= pOctree->GetHeight();       
  glVertex3fv( vVertex.m_pValues );
  // 7th
  vVertex[GSE_Vector3::Y] += pOctree->GetHeight();
  vVertex[GSE_Vector3::X] -= pOctree->GetWidth();
  glVertex3fv( vVertex.m_pValues );
  // 8th
  vVertex[GSE_Vector3::Y] -= pOctree->GetHeight();
  glVertex3fv( vVertex.m_pValues );
  // 9th
  vVertex[GSE_Vector3::Y] += pOctree->GetHeight();
  vVertex[GSE_Vector3::Z] += pOctree->GetLength();
  glVertex3fv( vVertex.m_pValues );
  // 10th
  vVertex[GSE_Vector3::Y] -= pOctree->GetHeight();
  glVertex3fv( vVertex.m_pValues );

  glEnd();
  
  DrawOctreeRecursive(static_cast<GSE_GeometryOctree *>(pOctree->GetNodes()[GSE_Octree::TOP_LEFT_FRONT]));
  DrawOctreeRecursive(static_cast<GSE_GeometryOctree *>(pOctree->GetNodes()[GSE_Octree::TOP_LEFT_BACK]));
  DrawOctreeRecursive(static_cast<GSE_GeometryOctree *>(pOctree->GetNodes()[GSE_Octree::TOP_RIGHT_FRONT]));
  DrawOctreeRecursive(static_cast<GSE_GeometryOctree *>(pOctree->GetNodes()[GSE_Octree::TOP_RIGHT_BACK]));
  DrawOctreeRecursive(static_cast<GSE_GeometryOctree *>(pOctree->GetNodes()[GSE_Octree::BOTTOM_LEFT_FRONT]));
  DrawOctreeRecursive(static_cast<GSE_GeometryOctree *>(pOctree->GetNodes()[GSE_Octree::BOTTOM_LEFT_BACK]));
  DrawOctreeRecursive(static_cast<GSE_GeometryOctree *>(pOctree->GetNodes()[GSE_Octree::BOTTOM_RIGHT_FRONT]));
  DrawOctreeRecursive(static_cast<GSE_GeometryOctree *>(pOctree->GetNodes()[GSE_Octree::BOTTOM_RIGHT_BACK]));
  
}
/////////////////////////////////////////////////////////////////
void 
GSE_OglRenderer::DrawOctree( GSE_GeometryOctree *pOctree )
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
GSE_OglRenderer::HandleTexture( GSE_TextureNode *pNode )
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

  GSE_OglTexture *pTexture = static_cast<GSE_OglTexture *>(pNode->GetTexture());

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
// $Log: GSE_OglRenderer.cpp,v $
// Revision 1.58  2007/06/04 09:03:32  entity
// GLee fix
//
// Revision 1.57  2007/05/23 15:56:00  entity
// activetexture scheme
//
// Revision 1.56  2007/05/21 12:50:00  entity
// glew to GLee
//
// Revision 1.55  2007/05/18 08:08:09  entity
// Vector3 constructor check & fixes
//
// Revision 1.54  2007/05/16 13:03:36  entity
// UtilQuad with texture coords
//
// Revision 1.53  2007/05/10 11:08:37  entity
// polygonmodenode added
//
// Revision 1.52  2007/05/07 06:34:05  entity
// crash bug fix (glBindBuffer(...))
//
// Revision 1.51  2007/04/09 20:18:11  entity
// fixed lighting bug on ATI drivers
//
// Revision 1.50  2007/04/09 09:40:58  entity
// AlphaTest added
//
// Revision 1.49  2007/04/06 11:53:12  entity
// fixed nasty VBO bug
//
// Revision 1.48  2007/04/04 08:58:19  entity
// fixed particle positioning and update now only alive count
//
// Revision 1.47  2007/04/03 20:24:15  entity
// blending mode fix
//
// Revision 1.46  2007/04/03 12:56:23  entity
// code cleanups
//
// Revision 1.45  2007/04/03 06:57:40  entity
// comments and GL_LIGHTING is enabled only when lightcluster demands
//
// Revision 1.44  2007/04/02 12:55:50  entity
// added CLAMP_TO_EDGE filters
//
// Revision 1.43  2007/04/02 11:08:15  entity
// Texture disabling on _leave(), fixed
// culling by transparency
//
// Revision 1.42  2007/04/01 21:06:21  entity
// debug output tweaked
//
// Revision 1.41  2007/04/01 15:25:05  entity
// texture system overhaul, Elements->Indexbuffer rename
//
// Revision 1.40  2007/03/31 16:20:43  entity
// ObjStruct -> GeometryData rename
//
// Revision 1.39  2007/03/31 15:56:35  entity
// ElementList->IndexBuffer rename
//
// Revision 1.38  2007/03/30 12:57:29  entity
// static_cast added
//
// Revision 1.37  2007/03/30 09:13:34  entity
// Added some comments
//
/////////////////////////////////////////////////////////////////

