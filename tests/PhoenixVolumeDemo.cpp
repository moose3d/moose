#include "Phoenix.h"
#include <iostream>
#include <SDL.h>
#include <sstream>
using std::string;
using namespace std;
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Core;
using namespace Phoenix::Math;
using namespace Phoenix::Spatial;
using namespace Phoenix::Window;
using namespace Phoenix::Graphics;
using namespace Phoenix::Data;
using namespace Phoenix::Volume;
/////////////////////////////////////////////////////////////////
void PrintData( CVertexDescriptor *pVD )
{
  cerr << "DATA IS : " << endl;
  for( size_t i=0;i<pVD->GetSize()*3;i++)
  {
    cerr << pVD->GetPointer<float>()[i] << ",";
  }
  cerr << endl;
}
/////////////////////////////////////////////////////////////////
int main()
{
  CModel model;
  LoadMilkshapeModel( "Resources/Models/triangle.ms3d", "", model, OPT_VERTEX_NORMALS | OPT_VERTEX_TEXCOORDS | OPT_VERTEX_INDICES);
  COrientedBox obBox;
  CVertexDescriptor *pVD = NULL;
  pVD = g_DefaultVertexManager->GetResource(model.GetVertexHandle());
  PrintData( pVD );
  
  obBox  = CalculateOrientedBoundingBox( *(g_DefaultVertexManager->GetResource(model.GetVertexHandle())) );
  pVD = g_DefaultVertexManager->GetResource(model.GetVertexHandle());
  PrintData( pVD );
  cerr << obBox << endl;

  obBox  = CalculateOrientedBoundingBox( *(g_DefaultVertexManager->GetResource(model.GetVertexHandle())) );
  pVD = g_DefaultVertexManager->GetResource(model.GetVertexHandle());
  PrintData( pVD );
  cerr << obBox << endl;

  obBox  = CalculateOrientedBoundingBox( *(g_DefaultVertexManager->GetResource(model.GetVertexHandle())) );
  pVD = g_DefaultVertexManager->GetResource(model.GetVertexHandle());
  PrintData( pVD );
  cerr << obBox << endl;

  CSphere      sphere = CalculateBoundingSphereTight( *(g_DefaultVertexManager->GetResource(model.GetVertexHandle())) );
  pVD = g_DefaultVertexManager->GetResource(model.GetVertexHandle());
  PrintData( pVD );

  cerr << obBox << endl;
  cerr << sphere << endl;

  
  
  return 0;
}
/////////////////////////////////////////////////////////////////
