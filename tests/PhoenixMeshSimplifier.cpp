#include <Phoenix.h>
#include <iostream>
#include <list>
#include <vector>
#include <SDL.h>
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Graphics;
using namespace Phoenix::Core;
using namespace Phoenix::Math;
using namespace Phoenix::Spatial;
using namespace Phoenix::Window;
using namespace Phoenix::Graphics;
using namespace Phoenix::Scene;
using namespace Phoenix::Volume;
using namespace Phoenix::AI;
using namespace Phoenix::Data;
using std::cerr;
using std::endl; 
using std::list;
using std::vector;

#define ENORMOUS_COST -500.0f
#define QUITE_HUGE     200.0f
class Triangle;
/////////////////////////////////////////////////////////////////
class Vertex 
{
public:
  CVector3<float>	normal;
  CVector3<float>	position;  
  vector<size_t>        neighbors; // indices of vertices
  vector<size_t>	faces;     // indices of triangles that this belongs
  float			cost;
  size_t		mergeVertex;
  /////////////////////////////////////////////////////////////////
  Vertex()
  {
    cost = 0.0f;
  }
  /////////////////////////////////////////////////////////////////
  void AddNeighbor( size_t ind )
  {
    vector<size_t>::iterator it = neighbors.begin();
    for( ; it != neighbors.end(); it++)
    {
      // ok, vertex is already there 
      if ( (*it) == ind ) return;
    }
    // adds vertex as neighbor
    neighbors.push_back( ind );
  }
  /////////////////////////////////////////////////////////////////
  void AddFaceIndex( size_t ind )
  {
    faces.push_back(ind);
  }
  /////////////////////////////////////////////////////////////////  
  void CalculateNormal( vector<Triangle> & triangles, vector<Vertex> & vertices );
  
};

class Triangle
{
public:
  size_t v0, v1, v2;
  /////////////////////////////////////////////////////////////////
  CVector3<float>	GetNormal( vector<Vertex> & vertices ) 
  {
    CVector3<float> tmp1 = vertices[v1].position - vertices[v0].position;
    CVector3<float> tmp2 = vertices[v2].position - vertices[v0].position;
    return tmp1.Cross(tmp2).GetNormalized();
  }
  /////////////////////////////////////////////////////////////////  
  int HasEdge( size_t start, size_t end )
  {

    //cerr << "HasEdge: " << start << " vs. " << end << endl;
    return 
    ( v0 == start && v1 == end ) ||
    ( v1 == start && v2 == end ) ||
    ( v2 == start && v0 == end ) ||
    ( v0 == end && v1 == start ) ||
    ( v1 == end && v2 == start ) ||
    ( v2 == end && v0 == start );
  }
  /////////////////////////////////////////////////////////////////
  void ReplaceVertex( size_t prev, size_t curr )
  {
    if      ( v0 == prev ) {     v0 = curr;    } 
    else if ( v1 == prev ) {     v1 = curr;    } 
    else if ( v2 == prev ) {     v2 = curr;    }
  }
  /////////////////////////////////////////////////////////////////
  size_t GetVertexNotOnEdge( size_t one, size_t two )
  {
    if ( one == v0 )
    {
      if ( two == v1 )      return v2;
      else if ( two == v2 ) return v1;
    }
    else if ( two == v0 )
    {
      if ( one == v1 )      return v2;
      else if ( two == v2 ) return v1;
    }
    
    return v0;
    
  }
};
/////////////////////////////////////////////////////////////////
void 
Vertex::CalculateNormal( vector<Triangle> & triangles, vector<Vertex> & vertices )
{
  // calculate normal for vertex from surrounding triangle normals.
  normal = CVector3<float>(0,0,0);
  vector<size_t>::iterator triIt = faces.begin();    
  for( ; triIt != faces.end(); triIt++)
  {
    normal += triangles[*triIt].GetNormal(vertices);
  }
  normal.Normalize();
}
/////////////////////////////////////////////////////////////////
int SharedByTwoFaces( vector<Triangle> & triangles, vector<Vertex> &vertices, size_t start, size_t end  )
{
  int count = 0;
  Vertex & v = vertices[start];
  
  for( size_t i=0;i < v.faces.size();i++)
  {
    if ( triangles[v.faces[i]].HasEdge( start, end))
    {
      count++;
    }
  }

  return (count == 2);
}

void ConstructVertices( CVertexDescriptor *pVertexDescriptor, vector<Vertex> & vertices)
{
  // clear previous vertices.
  vertices.clear();
  // generate new set 
  for( size_t i=0;i<pVertexDescriptor->GetSize();i++)
  {
    Vertex vert;
    vert.position[0] = pVertexDescriptor->GetPointer<float>()[i*3];
    vert.position[1] = pVertexDescriptor->GetPointer<float>()[i*3+1];
    vert.position[2] = pVertexDescriptor->GetPointer<float>()[i*3+2];
    vert.mergeVertex = i;
    vert.cost = QUITE_HUGE;
    vertices.push_back(vert);
  }
}

// void ConstructFaces( CIndexArray * pIndices, vector<Triangle> & triangles )
// {
//   // Clears previous faces
//   triangles.clear();
  
//   for( size_t i=0;i<pIndices->GetNumIndices();i+=3)
//   {
//     Triangle tri;
//     tri.v0 = pIndices->GetPointer<unsigned short int>()[i];
//     tri.v1 = pIndices->GetPointer<unsigned short int>()[i+1];
//     tri.v2 = pIndices->GetPointer<unsigned short int>()[i+2];
//     triangles.push_back(tri);
//   }
// }

void CreateIndexVector( CIndexArray * pIndices, vector<size_t> & indices)
{
  for( size_t i=0;i<pIndices->GetNumIndices();i+=3)
  {
    indices.push_back(pIndices->GetPointer<unsigned short int>()[i]);
    indices.push_back(pIndices->GetPointer<unsigned short int>()[i+1]);
    indices.push_back(pIndices->GetPointer<unsigned short int>()[i+2]);
  }
}

void ConstructFaces( vector<size_t> & indices, vector<Triangle> & triangles )
{
  // Clears previous faces
  triangles.clear();
  
  for( size_t i=0;i<indices.size();i+=3)
  {
    Triangle tri;
    tri.v0 = indices[i];
    tri.v1 = indices[i+1];
    tri.v2 = indices[i+2];
    triangles.push_back(tri);
  }
}

void ResetVertexFacesAndNeighbors( vector<Vertex> & vertices )
{
  for( size_t i=0;i<vertices.size();i++)
  {
    vertices[i].neighbors.clear();
    vertices[i].faces.clear();
    vertices[i].cost = QUITE_HUGE;
  }
}

void ConstructVertexFacesAndNeighbors( vector<Vertex> & vertices, vector<Triangle> & triangles )
{
  /// check which vertices belong to which triangle
  for( size_t i=0;i<triangles.size();i++)
  {
    size_t index0 = triangles[i].v0;
    size_t index1 = triangles[i].v1;
    size_t index2 = triangles[i].v2;

    Vertex &vert0 = vertices[index0];
    Vertex &vert1 = vertices[index1];
    Vertex &vert2 = vertices[index2];
    
    vert0.AddFaceIndex(i);
    vert1.AddFaceIndex(i);
    vert2.AddFaceIndex(i);
    
    vert0.AddNeighbor( index1 );
    vert0.AddNeighbor( index2 );

    vert1.AddNeighbor( index0 );
    vert1.AddNeighbor( index2 );

    vert2.AddNeighbor( index1 );
    vert2.AddNeighbor( index0 );
  }
}

int ReduceVertex( vector<Vertex> &vertices, vector<Triangle> & triangles, vector<size_t> & indices  )
{
  
  int bWasAbleToRemove = 0;
  ConstructFaces( indices, triangles );
  ResetVertexFacesAndNeighbors( vertices );
  ConstructVertexFacesAndNeighbors( vertices, triangles );
  
  for( size_t i=0;i<vertices.size();i++)
  {
    Vertex &v = vertices[i];
    int bVertexCanBeRemoved = 1;
    if ( v.neighbors.size() == 0 ) continue;
      
    vector<size_t>::iterator it = v.neighbors.begin();
    for( ; it != v.neighbors.end();it++)
    {
      size_t ind = *it; 

      //cerr << "neighbor is : " << ind << endl;
      if ( ! SharedByTwoFaces( triangles, vertices, i, ind )) 
      {
	bVertexCanBeRemoved = 0;
      }
    }
    // if vertex can't be removed, try next
    if ( !bVertexCanBeRemoved ) 
    { 
      continue;
    }
    v.CalculateNormal( triangles, vertices );
    //cerr << "vertex has normal " << v.normal  << endl;
    
    it = v.neighbors.begin();
    for( ; it != v.neighbors.end();it++)
    {
      size_t ind = *it; 

      Vertex &n = vertices[ind];
      CVector3<float> vE = n.position - v.position ;
      CVector3<float> vD = v.normal.Cross(vE).GetNormalized();

      size_t vert1, vert2;
      
      int tri1found = 0;
      int tri2found = 0;
      float tri1side = 0.0f;
      float tri2side = 0.0f;
      size_t tri1_index, tri2_index;
      for( size_t t=0;t<v.faces.size();t++)
      {
	Triangle & tri = triangles[v.faces[t]];
	
	//	cerr << "tri has vertices: " << tri.v0 << "," << tri.v1 << "," << tri.v2 << endl;	  
	if ( tri.HasEdge( i,ind) )
	{

	  size_t v3 = tri.GetVertexNotOnEdge( i, ind);
	  if ( tri1found == 0)
	  {
	    vert1 = v3;
	    tri1found = 1;
	    tri1_index = t;
	  }
	  else 
	  {
	    vert2 = v3;
	    tri2found = 1;
	    tri2_index = t;
	    break;
	  }
	  
	}
      }
      //cerr << "indices were: " << vert1 << " and " << vert2 << endl;
      assert( tri1found == 1 && tri2found == 1 );
      
      tri1side = vD.Dot( vertices[vert1].position - v.position );
      tri2side = vD.Dot( vertices[vert2].position - v.position );
      
      
      // both vertices are on same side of edge, cannot remove 
      if ( (tri1side >= 0.0f) && (tri2side >= 0.0f) ) continue;
      if ( (tri1side < 0.0f)  && (tri2side < 0.0f) ) continue;

      CVector3<float> posnormal;
      CVector3<float> negnormal;

      if ( tri1side >= 0.0f ) 
      {
	posnormal = triangles[tri1_index].GetNormal( vertices );
	negnormal = triangles[tri2_index].GetNormal( vertices );
      }      
      else 
      {
	negnormal = triangles[tri1_index].GetNormal( vertices );
	posnormal = triangles[tri2_index].GetNormal( vertices );
      }      
      float d = QUITE_HUGE;

      // compare other faces that are related to this vertex, but
      // do not share the edge that is to be removed.
      for( size_t t=0;t<v.faces.size();t++)
      {
	Triangle & tri = triangles[v.faces[t]];
	
	if ( !tri.HasEdge( i, *it) )
	{
	  float val = 0.0f;
	  float newD = 0.0f;
	  size_t nTmpIndex = 0;

	  if ( tri.v0 != i ) { nTmpIndex = tri.v0; }
	  if ( tri.v1 != i ) { nTmpIndex = tri.v1; }
	  if ( tri.v2 != i ) { nTmpIndex = tri.v2; }
	  
	  val = vD.Dot( vertices[nTmpIndex].position - v.position );
	  if ( val > EPSILON ) 
	  {
	    newD = posnormal.Dot(tri.GetNormal(vertices));
	    d = newD < d ? newD : d;
	  } 
	  else if ( val < -EPSILON )
	  {
	    newD = negnormal.Dot(tri.GetNormal(vertices));
	    d = newD < d ? newD : d;
	  }
	} 
	else 
	{
	  d = 1.0f;
	}
      }
      float cost = (1.0f - d)*vE.Length();
      // compare cost to previous 
      if ( cost < v.cost )
      {
	v.cost = cost;
	v.mergeVertex = ind;
      }
      
    }
   
  }  
  // determine vertex with lowest merge cost
  size_t lowest = 0;
  for( size_t v=0;v<vertices.size();v++)
  {
    //cerr << "vertices[" << v << "]=" << vertices[v].cost << "\t" << vertices[v].position << endl;
    if( vertices[v].cost < vertices[lowest].cost )
    {
      lowest = v;
    }
  }
    
  if(  vertices[lowest].cost < QUITE_HUGE )
  {
    /*cerr << "lowest cost was in vertex " << lowest 
      << " with cost value : " << vertices[lowest].cost 
      << " merged to vertex " << vertices[lowest].mergeVertex << endl;*/
      
    // determine which triangles must be removed.
    vector<size_t> remove;
    for( size_t t=0;t<triangles.size();t++)
    {
      if ( triangles[t].HasEdge( lowest, vertices[lowest].mergeVertex ))
      {
	remove.push_back( t );
      }
    }
    assert( remove.size() == 2 );
    // "remove" triangles
    triangles.erase( triangles.begin() + remove[1] );
    triangles.erase( triangles.begin() + remove[0] );
      
    // replace removed vertices from triangles
    for( size_t t=0;t<triangles.size();t++)
    {
      triangles[t].ReplaceVertex( lowest, vertices[lowest].mergeVertex );
    }

    // recreate index vector
    indices.clear();
    for( size_t t=0;t<triangles.size();t++)
    {
      indices.push_back( triangles[t].v0 );
      indices.push_back( triangles[t].v1 );
      indices.push_back( triangles[t].v2 );
    }

    bWasAbleToRemove = 1;
  } 
  else
  {
    bWasAbleToRemove = 0;
  }
  //cerr << "remaining faces : " << indices.size()/3 << endl;
  //   for( size_t i=0;i<indices.size();i+=3)
  //   {
  //     cerr << "tri : " << indices[i] << ","<<  indices[i+1] << "," << indices[i+2] << endl;
  //   }
  return bWasAbleToRemove;

}

/////////////////////////////////////////////////////////////////
int main( int argc, char **argv )
{
  
  CMilkshapeLoader *pLoader = new CMilkshapeLoader();
  CVertexDescriptor *pVertexDescriptor = NULL; 
  CVertexDescriptor *pTexCoords = NULL; 
  assert(pLoader->Load("Resources/Models/1701-e.ms3d") == 0);
  pLoader->GenerateModelData( VERTEX_COMP_POSITION | VERTEX_COMP_TEXCOORD );
  
  pVertexDescriptor = pLoader->GetVertices();
  pLoader->ResetVertices();

  assert( pLoader->GetIndices().size() == 1 );
  CIndexArray *pIndices = pLoader->GetIndices()[0];
  pLoader->ResetIndices();

  pTexCoords = pLoader->GetTexCoords();
  pLoader->ResetTexCoords();
  delete pLoader;
  
  cerr << "vertices actually: " << pVertexDescriptor->GetSize() << endl;
  cerr << "indices actually: "  << pIndices->GetNumIndices()    << endl;


  
  
  /// Assign proper indices to vertices and set position
  vector<Vertex> vertices;
  /// Create triangles out from index vector
  vector<Triangle> triangles;
  /// Indices 
  vector<size_t>   indices;

  
  
  if (  CSDLScreen::GetInstance() == NULL )
  {
    cerr << "Can't open screen" << endl;
  }

  CCamera camera;
  camera.SetViewport( 0,0, CSDLScreen::m_SDLScreenParams.m_iWidth, CSDLScreen::m_SDLScreenParams.m_iHeight );
  camera.SetPosition( -20,0, 30.0f);
  camera.SetNearClipping(0.1f);
  camera.SetFarClipping(600.0f);
  camera.SetFieldOfView(45.0f);
  camera.RotateAroundY(-27.0f);
  SDL_Event event;
  int bLoop = 1;

  CIndexArray *pIndices2 = NULL;

  
  COglRenderer *pRenderer = new COglRenderer();
  CVector2<int> vMousePos((int)(camera.GetViewport()[2]*0.5f),  (int)(camera.GetViewport()[3]*0.5f)) ;
  int bMousePressed = 0;
  COglTexture *pTexture = pRenderer->CreateTexture( "Resources/Textures/sovereign.tga");
  size_t nDesiredFaces = pIndices->GetNumIndices()/3;
  while (  bLoop  )
  {
    while ( SDL_PollEvent(&event))
    {
      switch(event.type)
      {
      case SDL_KEYDOWN:
	if ( event.key.keysym.sym == SDLK_ESCAPE)	
	{
	  bLoop = 0;
	} 
	else if ( event.key.keysym.sym == SDLK_PLUS)
	{
	  if ( nDesiredFaces < (pIndices->GetNumIndices()/3)-2 )
	    nDesiredFaces+=2;
	  indices.clear();
	  ConstructVertices( pVertexDescriptor, vertices );
	  CreateIndexVector( pIndices, indices );
	  CTimer timer;
	  timer.Reset();
	  while (   indices.size()/3 > nDesiredFaces ){
	    if ( !ReduceVertex( vertices, triangles, indices ) )
	      break;
	  }
	  timer.Update();
	  cerr << timer.GetPassedTime() << endl;
	  cerr << "triangles remaining : " << indices.size()/3 << endl;
	  cerr << "triangles reduced : " << (pIndices->GetNumIndices() - indices.size())/3 << endl;

	  if ( pIndices2 != NULL ) delete pIndices2;
	  pIndices2 = new CIndexArray( PRIMITIVE_TRI_LIST, indices.size());

	  for( size_t idx =0 ; idx < indices.size(); idx++)
	  {
	    pIndices2->GetPointer<unsigned short int>()[idx] = indices[idx];
	  }
	}
	else if ( event.key.keysym.sym == SDLK_MINUS)
	{
	  if ( nDesiredFaces > 2 )
	    nDesiredFaces-=2;
	  indices.clear();
	  ConstructVertices( pVertexDescriptor, vertices );
	  CreateIndexVector( pIndices, indices );
	  CTimer timer;
	  timer.Reset();
	  while (   indices.size()/3 > nDesiredFaces ){
	    if ( !ReduceVertex( vertices, triangles, indices ) )
	      break;
	  }
	  timer.Update();
	  cerr << timer.GetPassedTime() << endl;
	  cerr << "triangles remaining : " << indices.size()/3 << endl;
	  cerr << "triangles reduced : " << (pIndices->GetNumIndices() - indices.size())/3 << endl;

	  if ( pIndices2 != NULL ) delete pIndices2;
	  pIndices2 = new CIndexArray( PRIMITIVE_TRI_LIST, indices.size());

	  for( size_t idx =0 ; idx < indices.size(); idx++)
	  {
	    pIndices2->GetPointer<unsigned short int>()[idx] = indices[idx];
	  }
	}
	break;
      case SDL_MOUSEBUTTONDOWN:
	bMousePressed = 1;
	break;
      case SDL_MOUSEBUTTONUP:
	bMousePressed = 0;
	break;
      case SDL_MOUSEMOTION:
	{
	  CVector2<int> vMousePosCurrent(event.motion.x, event.motion.y);
	  CVector2<int> vMouseDiff = vMousePos - vMousePosCurrent;
	  vMouseDiff[0] = -vMouseDiff[0];
	  //camera.RotateAroundUp(vMouseDiff[0]);
	  //camera.RotateAroundRight(vMouseDiff[1]);	
	  if ( bMousePressed ){
	    camera.VirtualTrackball( CVector3<float>(0,0,0),
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
    pRenderer->ClearBuffer( COLOR_BUFFER );
    pRenderer->ClearBuffer( DEPTH_BUFFER );
    pRenderer->CommitState( STATE_DEPTH_TEST );
    pRenderer->DisableState( STATE_FACECULLING );
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE );
    pRenderer->CommitCamera( camera );
    pRenderer->CommitColor( CVector4<unsigned char>(255,255,255,255));
    pRenderer->CommitTexture( 0, pTexture );
    pRenderer->CommitVertexDescriptor( pVertexDescriptor );
    pRenderer->CommitVertexDescriptor( pTexCoords );
    if ( pIndices2 != NULL )
      pRenderer->CommitPrimitive( pIndices2 );
    else 
      pRenderer->CommitPrimitive( pIndices );
    pRenderer->DisableTexture( 0, pTexture );
    pRenderer->Finalize();
    CSDLScreen::GetInstance()->SwapBuffers();

  }
  return 0;
}
/////////////////////////////////////////////////////////////////
