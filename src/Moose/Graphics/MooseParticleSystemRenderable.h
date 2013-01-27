/******************************************************************************/
 * Moose3D is a game development framework.
 *
 * Copyright 2006-2013 Anssi Gröhn / entity at iki dot fi.
 *
 * This file is part of Moose3D.
 *
 * Moose3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Moose3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Moose3D.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

#ifndef __MooseParticleSystemRenderable_h__
#define __MooseParticleSystemRenderable_h__
#include <MooseVertexDescriptor.h>
#include <MooseIndexArray.h>
#include <MooseRenderable.h>
#include <MooseTransform.h>
#include <MooseParticleSystem.h>
#include <algorithm>
namespace Moose
{
  namespace Graphics
  {
    template< class PARTICLE_SYSTEM >
    class MOOSE_API CParticleSystemRenderable : public Moose::Graphics::CRenderable
    {
    protected:
      PARTICLE_SYSTEM *m_pPS; ///!< Particle system.
      Moose::Graphics::CVertexDescriptor m_Vertices; ///!< Vertices for this ps
      Moose::Graphics::CVertexDescriptor m_ShaderData; ///!< Billboard shader data for this ps
      Moose::Graphics::CIndexArray m_Indices; ///!< Indices for this ps
    public:
      CParticleSystemRenderable( PARTICLE_SYSTEM *pPS );
        virtual ~CParticleSystemRenderable() {}
      void Render( Moose::Graphics::COglRenderer & r );
      void UpdateRenderableData();
    };
  } // Graphics 
} // Moose
////////////////////////////////////////////////////////////////////////////////
template<class PS>
inline 
Moose::Graphics::CParticleSystemRenderable<PS>::CParticleSystemRenderable( PS *pPS) :
  m_pPS(pPS),
  // 4 vertices per particle
  m_Vertices( Moose::Graphics::ELEMENT_TYPE_VERTEX_3F, pPS->GetMaxParticles()*4), 
  // Billboard data, 4 per particle
  m_ShaderData( Moose::Graphics::ELEMENT_TYPE_ATTRIB_4F, pPS->GetMaxParticles()*4),
  // 4 vertices per particle+ 2 for stiching, last does not need stitching => -2.
  m_Indices( Moose::Graphics::PRIMITIVE_TRI_STRIP, pPS->GetMaxParticles()*6-2)
{
  // Initialize index array
  int idx = 0;   
  for(unsigned short int i=0;i<m_pPS->GetMaxParticles()*4;i+=4)
  {
    m_Indices.GetPointer<unsigned short int>()[idx++]=i;
    m_Indices.GetPointer<unsigned short int>()[idx++]=i+1;
    m_Indices.GetPointer<unsigned short int>()[idx++]=i+2;
    m_Indices.GetPointer<unsigned short int>()[idx++]=i+3;
    
    if ( i < (m_pPS->GetMaxParticles()-1)*4 )
    {
      m_Indices.GetPointer<unsigned short int>()[idx++]=i+3;
      m_Indices.GetPointer<unsigned short int>()[idx++]=i+4;
    }
  }
  // Set up renderstate
  GetRenderState().GetBlendingOperation().SetOperation( BLEND_SRC_SRC_ALPHA, BLEND_DST_ONE );
  GetRenderState().GetBlendingOperation().SetEnabled(true);
  GetRenderState().GetDepthTest() = true;
  GetRenderState().GetDepthWrite() = false;
  GetRenderState().GetShaderHandle() = "moose_billboard_particle_shader";
assert( GetRenderState().GetShaderHandle().IsNull() == false);
  GetRenderState().AddShaderAttrib( "a_data",     &m_ShaderData);
  GetRenderState().AddShaderAttrib( "a_position", &m_Vertices);    
  GetRenderState().AddShaderUniform( "texture", 0);

}
////////////////////////////////////////////////////////////////////////////////
template<class PS>
inline void 
Moose::Graphics::CParticleSystemRenderable<PS>::Render( Moose::Graphics::COglRenderer & renderer )
{
    using namespace Moose::Math;
  // set renderstate first, because of lights, for example.
  renderer.CommitRenderState(GetRenderState());

  ////////////////////
  // Retrieve resources
  COglTexture *pTexture = NULL;

  for( unsigned int i=0; i<TEXTURE_HANDLE_COUNT; i++)
  {
    
    pTexture = *GetRenderState().GetTextureHandle(i);
    // check that texture resource exists
    if ( pTexture  != NULL )
    {
      renderer.CommitTexture( i, pTexture );
      renderer.CommitFilters( GetRenderState().GetTextureFilters(i), 
                              pTexture->GetType() );
    }
    else 
    { 
      renderer.DisableTexture(i, NULL);
    }
  } 


  CShader *pShader = *GetRenderState().GetShaderHandle();
  renderer.CommitShader( pShader );

  if ( !GetRenderState().GetShaderHandle().IsNull())
  {
    GetRenderState().GetShaderAttribs().Apply(renderer);
    GetRenderState().GetShaderUniforms().Apply(renderer);
    if ( renderer.GetCurrentCamera() )
    {
      // Update matrices 
      GetRenderState().GetShaderViewUniform().SetData(      &renderer.GetCurrentCamera()->GetViewMatrix());
      GetRenderState().GetShaderProjectionUniform().SetData(&renderer.GetCurrentCamera()->GetProjectionMatrix());

      if ( GetTransform() ) 
      {
        GetRenderState().GetShaderModelUniform().SetData( &GetTransform()->GetMatrix() );
      }
      else GetRenderState().GetShaderModelUniform().SetData( &CMatrix4x4<float>::Identity );

      // Send data to shader
      GetRenderState().GetShaderViewUniform().Apply(renderer);
      GetRenderState().GetShaderProjectionUniform().Apply(renderer);
      GetRenderState().GetShaderModelUniform().Apply(renderer);
    }
  }

  renderer.CommitPrimitive( &m_Indices );
  GetRenderState().GetShaderAttribs().Disable(renderer);

}
////////////////////////////////////////////////////////////////////////////////
template<class PS>
inline void 
Moose::Graphics::CParticleSystemRenderable<PS>::UpdateRenderableData( )
{
  for( size_t v=0, i=0; i<m_pPS->GetAliveCount(); i++, v+=4 )
  {
      const Moose::Graphics::CParticle & p = m_pPS->GetParticles()[i];
    m_Vertices.GetPointer<float>(v)[0] = p.m_vPosition[0];
    m_Vertices.GetPointer<float>(v)[1] = p.m_vPosition[1];
    m_Vertices.GetPointer<float>(v)[2] = p.m_vPosition[2];
    m_ShaderData.GetPointer<float>(v)[0] = -1;
    m_ShaderData.GetPointer<float>(v)[1] = 1;
    m_ShaderData.GetPointer<float>(v)[2] = p.m_fEnergy;
    m_ShaderData.GetPointer<float>(v)[3] = p.m_fSize;

    m_Vertices.GetPointer<float>(v+1)[0] = p.m_vPosition[0];
    m_Vertices.GetPointer<float>(v+1)[1] = p.m_vPosition[1];
    m_Vertices.GetPointer<float>(v+1)[2] = p.m_vPosition[2];   
    m_ShaderData.GetPointer<float>(v+1)[0] = -1;
    m_ShaderData.GetPointer<float>(v+1)[1] = -1;
    m_ShaderData.GetPointer<float>(v+1)[2] = p.m_fEnergy;
    m_ShaderData.GetPointer<float>(v+1)[3] = p.m_fSize;

    m_Vertices.GetPointer<float>(v+2)[0] = p.m_vPosition[0];
    m_Vertices.GetPointer<float>(v+2)[1] = p.m_vPosition[1];
    m_Vertices.GetPointer<float>(v+2)[2] = p.m_vPosition[2];   
    m_ShaderData.GetPointer<float>(v+2)[0] = 1;
    m_ShaderData.GetPointer<float>(v+2)[1] = 1;
    m_ShaderData.GetPointer<float>(v+2)[2] = p.m_fEnergy;
    m_ShaderData.GetPointer<float>(v+2)[3] = p.m_fSize;

    m_Vertices.GetPointer<float>(v+3)[0] = p.m_vPosition[0];
    m_Vertices.GetPointer<float>(v+3)[1] = p.m_vPosition[1];
    m_Vertices.GetPointer<float>(v+3)[2] = p.m_vPosition[2];   
    m_ShaderData.GetPointer<float>(v+3)[0] = 1;
    m_ShaderData.GetPointer<float>(v+3)[1] = -1;
    m_ShaderData.GetPointer<float>(v+3)[2] = p.m_fEnergy;
    m_ShaderData.GetPointer<float>(v+3)[3] = p.m_fSize;

  }
  // In case of VBOs, cache needs to be updated.
  if ( m_Vertices.IsCached()   ) m_Vertices.SetState( Moose::Core::CACHE_REFRESH);
  if ( m_ShaderData.IsCached() ) m_ShaderData.SetState( Moose::Core::CACHE_REFRESH);

  if ( m_pPS->GetAliveCount() > 0 )
    m_Indices.SetDrawableCount( m_pPS->GetAliveCount()*6-2 );
  else
    m_Indices.SetDrawableCount( 0 );
}
////////////////////////////////////////////////////////////////////////////////
#endif
