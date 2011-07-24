#include "MooseRocketRenderInterface.h"
#include "MooseDefaultEntities.h"
#include "MooseApplication.h"
#include "MooseRenderState.h"
#include "MooseVector2.h"
#include "MooseExceptions.h"
using namespace Moose::Scene;
using namespace Moose::Math;
using namespace Moose::Exceptions;
namespace prefix=Moose::Graphics;
////////////////////////////////////////////////////////////////////////////////
prefix::CRocketRenderInterface::CRocketRenderInterface(  ) : m_pRenderable(NULL)
{
  
}
////////////////////////////////////////////////////////////////////////////////
void
prefix::CRocketRenderInterface::SetRenderable( Moose::Graphics::CRenderable *pR )
{
  m_pRenderable = pR;
}
////////////////////////////////////////////////////////////////////////////////
void 
prefix::CRocketRenderInterface::RenderGeometry(Rocket::Core::Vertex* vertices, 
                       int num_vertices, 
                       int* indices, int num_indices, 
                       const Rocket::Core::TextureHandle texture, 
                       const Rocket::Core::Vector2f& translation)
{

  COglRenderer & renderer = CApplication::GetInstance()->GetRenderer();
  CRenderState & rs = m_pRenderable->GetRenderState();
  renderer.CommitRenderState( rs );
  glDisable(GL_TEXTURE_CUBE_MAP);		
  glDisable(GL_TEXTURE_RECTANGLE_ARB);  
  if ( texture )
  {
    glActiveTexture( GL_TEXTURE0);
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, texture );
  } 
  else 
  {
    glActiveTexture( GL_TEXTURE0);
    glDisable( GL_TEXTURE_2D );
  }


  renderer.CommitShader(*rs.GetShaderHandle());
  if ( rs.GetShaderHandle().IsNull()) 
  {
    throw CMooseRuntimeError("shader null");
  }
  
  CVertexDescriptor pos(ELEMENT_TYPE_ROCKET_DATA,num_vertices);
  pos.Copy(0, num_vertices, vertices);

  dynamic_cast<CShaderAttribRocketPos *>(rs.GetShaderAttrib("a_vertex"))->SetData( &pos);
  dynamic_cast<CShaderAttribRocketPos *>(rs.GetShaderAttrib("a_vertex"))->TriggerCacheUpdate();
  dynamic_cast<CShaderAttribRocketColor *>(rs.GetShaderAttrib("a_color"))->SetData( &pos);
  dynamic_cast<CShaderAttribRocketColor *>(rs.GetShaderAttrib("a_color"))->TriggerCacheUpdate();
  dynamic_cast<CShaderAttribRocketTexCoord *>(rs.GetShaderAttrib("a_texcoord"))->SetData( &pos);
  dynamic_cast<CShaderAttribRocketTexCoord *>(rs.GetShaderAttrib("a_texcoord"))->TriggerCacheUpdate();

  dynamic_cast<CShaderUniformVec2f *>(rs.GetShaderUniform("a_position"))->SetData( Moose::Math::CVector2<float>(translation.x, translation.y));
  dynamic_cast<CShaderUniformVec2f *>(rs.GetShaderUniform("a_position"))->TriggerCacheUpdate();
  rs.GetShaderAttribs().Apply(renderer);
  rs.GetShaderUniforms().Apply(renderer);

  CIndexArray arr(PRIMITIVE_TRI_LIST, num_indices);
  for(int i=0;i<num_indices;i++)
  {
    arr.GetPointer<unsigned short>()[i] = indices[i];
  }
  arr.CreateCache();
    
  if ( renderer.GetCurrentCamera() )
  {
    // Update matrices 
    rs.GetShaderViewUniform().SetData(      &renderer.GetCurrentCamera()->GetViewMatrix());
    rs.GetShaderProjectionUniform().SetData(&renderer.GetCurrentCamera()->GetProjectionMatrix());

    // Send data to shader
    rs.GetShaderViewUniform().Apply(renderer);
    rs.GetShaderProjectionUniform().Apply(renderer);
    
    renderer.CommitPrimitive( &arr );

  }

  
}
// Called by Rocket when it wants to compile geometry it believes will be static for the forseeable future.		
Rocket::Core::CompiledGeometryHandle 
prefix::CRocketRenderInterface::CompileGeometry(Rocket::Core::Vertex* ROCKET_UNUSED(vertices), 
                        int ROCKET_UNUSED(num_vertices), 
                        int* ROCKET_UNUSED(indices), 
                        int ROCKET_UNUSED(num_indices), 
                        const Rocket::Core::TextureHandle ROCKET_UNUSED(texture))
{
  return (Rocket::Core::CompiledGeometryHandle) NULL;
}

// Called by Rocket when it wants to render application-compiled geometry.		
void 
prefix::CRocketRenderInterface::RenderCompiledGeometry(Rocket::Core::CompiledGeometryHandle ROCKET_UNUSED(geometry), 
                               const Rocket::Core::Vector2f& ROCKET_UNUSED(translation))
{
}

// Called by Rocket when it wants to release application-compiled geometry.		
void 
prefix::CRocketRenderInterface::ReleaseCompiledGeometry(Rocket::Core::CompiledGeometryHandle ROCKET_UNUSED(geometry))
{
}

// Called by Rocket when it wants to enable or disable scissoring to clip content.		
void 
prefix::CRocketRenderInterface::EnableScissorRegion(bool enable)
{
  if (enable)
    glEnable(GL_SCISSOR_TEST);
  else
    glDisable(GL_SCISSOR_TEST);
}

// Called by Rocket when it wants to change the scissor region.		
void 
prefix::CRocketRenderInterface::SetScissorRegion(int x, int y, int width, int height)
{
  glScissor(x, 768 - (y + height), width, height);
}

// Set to byte packing, or the compiler will expand our struct, which means it won't read correctly from file
#pragma pack(1) 
struct TGAHeader 
{
  char  idLength;
  char  colourMapType;
  char  dataType;
  short int colourMapOrigin;
  short int colourMapLength;
  char  colourMapDepth;
  short int xOrigin;
  short int yOrigin;
  short int width;
  short int height;
  char  bitsPerPixel;
  char  imageDescriptor;
};
  // Restore packing
#pragma pack()

// Called by Rocket when a texture is required by the library.		
bool prefix::CRocketRenderInterface::LoadTexture(Rocket::Core::TextureHandle& texture_handle, 
                                                 Rocket::Core::Vector2i& texture_dimensions,
                                                 const Rocket::Core::String& source)
{
  Rocket::Core::FileInterface* file_interface = Rocket::Core::GetFileInterface();
  Rocket::Core::FileHandle file_handle = file_interface->Open(source);
  g_Log << "Opening " << source.CString() << "\n";
  if (!file_handle)
  {
    CMooseRuntimeError err("Failed to open file ");
    err << "'" << source.CString() << "'";
    throw err;
    return false;
  }
	
  file_interface->Seek(file_handle, 0, SEEK_END);
  size_t buffer_size = file_interface->Tell(file_handle);
  file_interface->Seek(file_handle, 0, SEEK_SET);
	
  char* buffer = new char[buffer_size];
  file_interface->Read(buffer, buffer_size, file_handle);
  file_interface->Close(file_handle);

  TGAHeader header;
  memcpy(&header, buffer, sizeof(TGAHeader));
	
  int color_mode = header.bitsPerPixel / 8;
  int image_size = header.width * header.height * 4; // We always make 32bit textures 
	
  if (header.dataType != 2)
  {
    Rocket::Core::Log::Message(Rocket::Core::Log::LT_ERROR, "Only 24/32bit uncompressed TGAs are supported.");
    return false;
  }
	
  // Ensure we have at least 3 colors
  if (color_mode < 3)
  {
    Rocket::Core::Log::Message(Rocket::Core::Log::LT_ERROR, "Only 24 and 32bit textures are supported");
    return false;
  }
	
  const char* image_src = buffer + sizeof(TGAHeader);
  unsigned char* image_dest = new unsigned char[image_size];
	
  // Targa is BGR, swap to RGB and flip Y axis
  for (long y = 0; y < header.height; y++)
  {
    long read_index = y * header.width * color_mode;
    long write_index = ((header.imageDescriptor & 32) != 0) ? read_index : (header.height - y - 1) * header.width * color_mode;
    for (long x = 0; x < header.width; x++)
    {
      image_dest[write_index] = image_src[read_index+2];
      image_dest[write_index+1] = image_src[read_index+1];
      image_dest[write_index+2] = image_src[read_index];
      if (color_mode == 4)
        image_dest[write_index+3] = image_src[read_index+3];
      else
        image_dest[write_index+3] = 255;
			
      write_index += 4;
      read_index += color_mode;
    }
  }

  texture_dimensions.x = header.width;
  texture_dimensions.y = header.height;
	
  bool success = GenerateTexture(texture_handle, image_dest, texture_dimensions);
	
  delete [] image_dest;
  delete [] buffer;
	
  return success;
}

// Called by Rocket when a texture is required to be built from an internally-generated sequence of pixels.
bool
prefix::CRocketRenderInterface::GenerateTexture(Rocket::Core::TextureHandle& texture_handle,
                             const Rocket::Core::byte* source, 
                             const Rocket::Core::Vector2i& source_dimensions)
{
  GLuint texture_id = 0;
  glGenTextures(1, &texture_id);
  if (texture_id == 0)
  {
    throw CMooseRuntimeError("Failed to generate textures\n");
    return false;
  }

  glBindTexture(GL_TEXTURE_2D, texture_id);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, source_dimensions.x, source_dimensions.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, source);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  texture_handle = (Rocket::Core::TextureHandle) texture_id;

  return true;
}

// Called by Rocket when a loaded texture is no longer required.		
void
prefix::CRocketRenderInterface::ReleaseTexture(Rocket::Core::TextureHandle texture_handle)
{
  glDeleteTextures(1, (GLuint*) &texture_handle);
}

