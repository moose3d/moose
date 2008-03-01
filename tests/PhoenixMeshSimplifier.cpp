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
/////////////////////////////////////////////////////////////////
using std::cerr;
using std::endl; 
using std::list;
using std::vector;
/////////////////////////////////////////////////////////////////
#define QUITE_HUGE     200.0f
class Face;
/////////////////////////////////////////////////////////////////
class Vertex 
{
public:
  size_t		index;
  CVector3<float>	normal;
  CVector3<float>	position;  
  vector<Vertex *>      neighbors; // indices of vertices
  vector<Face *>	faces;     // indices of triangles that this belongs
  Vertex *		mergeVertex;
  float			cost;
  int			dirty;
  /////////////////////////////////////////////////////////////////
  Vertex()
  {
    dirty = 1;
    cost = 0.0f;
    mergeVertex = NULL;
  }
  /////////////////////////////////////////////////////////////////
  void AddNeighbor( Vertex *vertex )
  {
    // we won't add ourselves as neighbor.
    if ( vertex == this ) return;
    if ( vertex == NULL ) return;
    vector<Vertex *>::iterator it = neighbors.begin();
    for( ; it != neighbors.end(); it++)
    {
      // ok, vertex is already there 
      if ( (*it) == vertex ) return;
    }
    // adds vertex as neighbor
    neighbors.push_back( vertex );
  }
  void DeleteNeighbor( Vertex *vertex )
  {
    if ( vertex == NULL ) return;
    vector<Vertex *>::iterator it = neighbors.begin();
    for( ; it != neighbors.end(); it++)
    {
      // ok, vertex is already there 
      if ( (*it) == vertex ) {
	neighbors.erase( it );
	return;
      }
    }
  }

  /////////////////////////////////////////////////////////////////
  void AddFace( Face *face )
  {
    if ( face == NULL ) return;
    for( size_t t=0;t<faces.size();t++ )
    {
      if( faces[t] == face ) return;
    }
    faces.push_back(face);
  }
  void DeleteFace( Face *face )
  {
    if ( face == NULL ) return;
    for( size_t t=0;t<faces.size();t++ )
    {
      if( faces[t] == face )
      {
	faces.erase(faces.begin()+t);
	return ;
      }
    }
  }
  /////////////////////////////////////////////////////////////////  
  void CalculateNormal();
  void SetNeighborsDirty()
  {
    for( size_t i=0;i<neighbors.size();i++)
    {
      neighbors[i]->dirty = 1;
      //neighbors[i]->mergeVertex = NULL;
      neighbors[i]->cost = QUITE_HUGE;
    }    
  }
};

class Face
{
public:
  Vertex * v0, *v1, *v2;
  Face()
  {
    v0 = NULL;
    v1 = NULL;
    v2 = NULL;
  }
  /////////////////////////////////////////////////////////////////
  CVector3<float>	GetNormal() 
  {
    CVector3<float> tmp1 = v1->position - v0->position;
    CVector3<float> tmp2 = v2->position - v0->position;
    return tmp1.Cross(tmp2).GetNormalized();
  }
  /////////////////////////////////////////////////////////////////  
  int HasEdge( Vertex * start, Vertex * end )
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
  void ReplaceVertex( Vertex * prev, Vertex * curr )
  {
    assert( prev != NULL );
    assert( curr != NULL );
    if      ( v0 == prev ) {     v0 = curr;    } 
    else if ( v1 == prev ) {     v1 = curr;    } 
    else if ( v2 == prev ) {     v2 = curr;    }
    
  }
  /////////////////////////////////////////////////////////////////
  Vertex * GetVertexNotOnEdge( Vertex * one, Vertex * two )
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
Vertex::CalculateNormal()
{
  // calculate normal for vertex from surrounding triangle normals.
  normal = CVector3<float>(0,0,0);
  vector<Face *>::iterator triIt = faces.begin();    
  for( ; triIt != faces.end(); triIt++)
  {
    normal += (*triIt)->GetNormal();
  }
  normal.Normalize();
}

/////////////////////////////////////////////////////////////////
int SharedByTwoFaces( vector<Face *> & triangles, Vertex * start, Vertex * end  )
{
  int count = 0;
  
  for( size_t i=0;i < start->faces.size();i++)
  {
    if ( start->faces[i]->HasEdge( start, end))
    {
      count++;
    }
  }

  return (count == 2);
}

void ConstructVertices( CVertexDescriptor *pVertexDescriptor, vector<Vertex *> & vertices)
{
  // clear previous vertices.
  vertices.clear();

  // generate new set 
  for( size_t i=0;i<pVertexDescriptor->GetSize();i++)
  {
    Vertex *vert =  new Vertex();    
    vert->position[0] = pVertexDescriptor->GetPointer<float>()[i*3];
    vert->position[1] = pVertexDescriptor->GetPointer<float>()[i*3+1];
    vert->position[2] = pVertexDescriptor->GetPointer<float>()[i*3+2];
    vert->mergeVertex = NULL;
    vert->cost = QUITE_HUGE;
    vert->index = i;
    vertices.push_back(vert);
  }
}

void CreateIndexVector( CIndexArray * pIndices, vector<size_t> & indices)
{
  for( size_t i=0;i<pIndices->GetNumIndices();i+=3)
  {
    indices.push_back(pIndices->GetPointer<unsigned short int>()[i]);
    indices.push_back(pIndices->GetPointer<unsigned short int>()[i+1]);
    indices.push_back(pIndices->GetPointer<unsigned short int>()[i+2]);
  }
}

void ConstructFaces( vector<size_t> & indices, vector<Vertex *> vertices, vector<Face *> & triangles )
{
  // Clears previous faces
  triangles.clear();
  
  for( size_t i=0;i<indices.size();i+=3)
  {
    Face *tri = new Face();
    tri->v0 = vertices[indices[i]];
    tri->v1 = vertices[indices[i+1]];
    tri->v2 = vertices[indices[i+2]];
    triangles.push_back(tri);
  }
}

void ResetVertexFacesAndNeighbors( vector<Vertex *> & vertices )
{
  for( size_t i=0;i<vertices.size();i++)
  {
    vertices[i]->neighbors.clear();
    vertices[i]->faces.clear();
    vertices[i]->cost = QUITE_HUGE;
  }
}

void ConstructVertexFacesAndNeighbors( vector<Face *> & triangles )
{
  /// check which vertices belong to which triangle
  for( size_t i=0;i<triangles.size();i++)
  {
    
    Vertex *vert0 = triangles[i]->v0;
    Vertex *vert1 = triangles[i]->v1;
    Vertex *vert2 = triangles[i]->v2;

    vert0->AddFace( triangles[i] );
    vert1->AddFace( triangles[i] );
    vert2->AddFace( triangles[i] );
    
    vert0->AddNeighbor( triangles[i]->v1 );
    vert0->AddNeighbor( triangles[i]->v2 );

    vert1->AddNeighbor( triangles[i]->v0 );
    vert1->AddNeighbor( triangles[i]->v2 );

    vert2->AddNeighbor( triangles[i]->v1 );
    vert2->AddNeighbor( triangles[i]->v0 );
  }
}

void CreateIndexVector( vector<Face *> & triangles, vector<size_t> & indices )
{
  // recreate index vector
  indices.clear();
  for( size_t t=0;t<triangles.size();t++)
  {
    indices.push_back( triangles[t]->v0->index );
    indices.push_back( triangles[t]->v1->index );
    indices.push_back( triangles[t]->v2->index );
  }
}

int ReduceVertex( vector<Vertex *> &vertices, vector<Face *> & triangles, int bProcessAll = 0  )
{
  
  int bWasAbleToRemove = 0;
  for( size_t i=0;i<vertices.size();i++)
  {

    Vertex *v = vertices[i];
    
    int bVertexCanBeRemoved = 1;
    if ( v->dirty == 0 && !bProcessAll ) continue;      
    if ( v->neighbors.size() == 0 ) continue;

    vector<Vertex *>::iterator it = v->neighbors.begin();
    for( ; it != v->neighbors.end();it++)
    {
      Vertex *neighbor = *it; 
      if ( ! SharedByTwoFaces( triangles, v, neighbor )) 
      {
	bVertexCanBeRemoved = 0;
	break;
      }
    }
    // if vertex can't be removed, try next
    if ( !bVertexCanBeRemoved ) { continue;  }
    
    v->CalculateNormal();
    //cerr << "vertex has normal " << v.normal  << endl;
    
    it = v->neighbors.begin();
    for( ; it != v->neighbors.end();it++)
    {
      Vertex *n = *it;
      CVector3<float> vE = n->position - v->position ;
      CVector3<float> vD = v->normal.Cross(vE).GetNormalized();
      
      Vertex * vert1 = NULL, *vert2 = NULL;
      
      
      float tri1side = 0.0f;
      float tri2side = 0.0f;
      Face * tri1 = NULL;
      Face * tri2 = NULL;

      for( size_t t=0;t<v->faces.size();t++)
      {
	Face * tri = v->faces[t];
	
	//	cerr << "tri has vertices: " << tri.v0 << "," << tri.v1 << "," << tri.v2 << endl;	  
	if ( tri->HasEdge( v, n) )
	{
	  
	  Vertex * v3 = tri->GetVertexNotOnEdge( v, n);
	  if ( tri1 == NULL)
	  {
	    vert1 = v3;
	    tri1 = tri;
	  }
	  else 
	  {
	    vert2 = v3;
	    tri2 = tri;
	    break;
	  }
	  
	}
      }
      //cerr << "indices were: " << vert1 << " and " << vert2 << endl;
      assert( tri1 != NULL && tri2 != NULL );
      
      tri1side = vD.Dot( vert1->position - v->position );
      tri2side = vD.Dot( vert2->position - v->position );
      
      
      // both vertices are on same side of edge, cannot remove 
      if ( (tri1side >= 0.0f) && (tri2side >= 0.0f) ) continue;
      if ( (tri1side < 0.0f)  && (tri2side < 0.0f) ) continue;

      CVector3<float> posnormal;
      CVector3<float> negnormal;

      if ( tri1side >= 0.0f ) 
      {
	posnormal = tri1->GetNormal();
	negnormal = tri2->GetNormal();
      }      
      else 
      {
	negnormal = tri1->GetNormal();
	posnormal = tri2->GetNormal();
      }      
      float d = QUITE_HUGE;

      // compare other faces that are related to this vertex, but
      // do not share the edge that is to be removed.
      for( size_t t=0;t<v->faces.size();t++)
      {
	Face * tri = v->faces[t];
	
	if ( !tri->HasEdge( v, n) )
	{
	  float val = 0.0f;
	  float newD = 0.0f;
	  Vertex *tmp = NULL;

	  if ( tri->v0 != v ) { tmp = tri->v0; }
	  if ( tri->v1 != v ) { tmp = tri->v1; }
	  if ( tri->v2 != v ) { tmp = tri->v2; }
	  
	  val = vD.Dot( tmp->position - v->position );
	  if ( val > EPSILON ) 
	  {
	    newD = posnormal.Dot(tri->GetNormal());
	    d = newD < d ? newD : d;
	  } 
	  else if ( val < -EPSILON )
	  {
	    newD = negnormal.Dot(tri->GetNormal());
	    d = newD < d ? newD : d;
	  }
	} 
      }
      float cost = (1.0f - d)*vE.Length();
      v->dirty = 0;
      // compare cost to previous 
      if ( cost < v->cost )
      {
	v->cost = cost;
	v->mergeVertex = n;
      }
    }
  }
  
  // determine vertex with lowest merge cost
  size_t lowest = 0;
  for( size_t v=0;v<vertices.size();v++)
  {
    //cerr << "vertices[" << v << "]=" << vertices[v].cost << "\t" << vertices[v].position << endl;
    if( vertices[v]->cost < vertices[lowest]->cost )
    {
      lowest = v;
    }
  }
  // if there actually is a vertex that can be removed
  if(  vertices[lowest]->cost < QUITE_HUGE )
  {
    /*cerr << "lowest cost was in " << vertices[lowest] << " -> " 
      << vertices[lowest]->mergeVertex << ", with cost value : " << vertices[lowest]->cost << endl;
    */
    // determine which triangles must be removed.
    vector<size_t> remove;
    for( size_t t=0;t<triangles.size();t++)
    {
      if ( triangles[t]->HasEdge( vertices[lowest], vertices[lowest]->mergeVertex ))
      {
	remove.push_back( t );
      }
    }
    assert( remove.size() == 2 );

    Vertex *pDeadVert = vertices[lowest];

    // set vertices dirty that are affected by merge
    pDeadVert->SetNeighborsDirty();
    pDeadVert->mergeVertex->SetNeighborsDirty();

    /// remove deleted vertex from neighbors and add mergeVertex as neighbor
    for( size_t n=0;n<pDeadVert->neighbors.size();n++)
    {
      pDeadVert->neighbors[n]->DeleteNeighbor( pDeadVert );
      pDeadVert->neighbors[n]->AddNeighbor( pDeadVert->mergeVertex );
    }

    Face *pDead =  *(triangles.begin() + remove[1]);
    Face *pDead2 = *(triangles.begin() + remove[0]);

    pDead->v0->DeleteFace( pDead );
    pDead->v1->DeleteFace( pDead );
    pDead->v2->DeleteFace( pDead );

    pDead2->v0->DeleteFace( pDead2 );
    pDead2->v1->DeleteFace( pDead2 );
    pDead2->v2->DeleteFace( pDead2 );

    // remove triangles, order matters
    triangles.erase( triangles.begin() + remove[1] );
    triangles.erase( triangles.begin() + remove[0] );

    // replace removed vertices from triangles
    for( size_t t=0;t<triangles.size();t++)
    {

      triangles[t]->ReplaceVertex( pDeadVert, pDeadVert->mergeVertex );
    }
    
    pDeadVert->neighbors.clear();
    pDeadVert->faces.clear();
    // just to be on safe side
    delete pDead;     pDead = NULL;
    delete pDead2;    pDead2 = NULL;
    delete pDeadVert; pDeadVert = NULL;

    vertices.erase(vertices.begin()+lowest);
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
  
  size_t nDesiredFaces = 0;  
  CMilkshapeLoader *pLoader = new CMilkshapeLoader();
  CVertexDescriptor *pVertexDescriptor = NULL; 
  CVertexDescriptor *pTexCoords = NULL; 
  assert(pLoader->Load("Resources/Models/1701-e-high.ms3d") == 0);
  pLoader->GenerateModelData( VERTEX_COMP_POSITION   );
  
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
  
  if ( argc > 1 )
  {
    nDesiredFaces = atoi( argv[1]);
  }
  
  
  
  /// Assign proper indices to vertices and set position
  vector<Vertex *> vertices;
  /// Create triangles out from index vector
  vector<Face *> triangles;
  /// Indices 
  vector<size_t>   indices;
  
  
  ConstructVertices( pVertexDescriptor, vertices );
  CreateIndexVector( pIndices, indices );
  
  ConstructFaces( indices, vertices, triangles );
  ResetVertexFacesAndNeighbors( vertices );
  ConstructVertexFacesAndNeighbors( triangles );
  

  
  CTimer timer;
  timer.Reset();
  while (  triangles.size() > nDesiredFaces ){
    if ( !ReduceVertex( vertices, triangles ) )
    {
      ResetVertexFacesAndNeighbors( vertices );
      ConstructVertexFacesAndNeighbors( triangles );
      if ( !ReduceVertex( vertices, triangles, 1 ) )
      break;
    }
    
    if (triangles.size() % 100 == 0) cerr << "processing : " << triangles.size() << endl;
  }
  CreateIndexVector( triangles, indices );
  timer.Update();
  cerr << timer.GetPassedTime() << endl;
  cerr << "triangles remaining : " << indices.size()/3 << endl;
  cerr << "triangles reduced : " << (pIndices->GetNumIndices() - indices.size())/3 << endl;
  
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
  if ( pIndices2 != NULL ) delete pIndices2;
  pIndices2 = new CIndexArray( PRIMITIVE_TRI_LIST, indices.size());
  
  for( size_t idx =0 ; idx < indices.size(); idx++)
  {
    pIndices2->GetPointer<unsigned short int>()[idx] = indices[idx];
  }  
  
  COglRenderer *pRenderer = new COglRenderer();
  CVector2<int> vMousePos((int)(camera.GetViewport()[2]*0.5f),  (int)(camera.GetViewport()[3]*0.5f)) ;
  int bMousePressed = 0;
  COglTexture *pTexture = pRenderer->CreateTexture( "Resources/Textures/sovereign.tga");
  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
  int bSimplified = 0;
  CFpsCounter fpsCounter;
  fpsCounter.Reset();

  pRenderer->CommitCache( *pVertexDescriptor );
  pRenderer->CommitCache( *pTexCoords );
  pRenderer->CommitCache( *pIndices );
  pRenderer->CommitCache( *pIndices2 );
  while (  bLoop  )
  {
    fpsCounter.Update();
    while ( SDL_PollEvent(&event))
    {
      switch(event.type)
      {
      case SDL_KEYDOWN:
	if ( event.key.keysym.sym == SDLK_ESCAPE)	
	{
	  bLoop = 0;
	} 
	else if ( event.key.keysym.sym == SDLK_DOWN)
	{
	  camera.Move(camera.GetForwardVector()*-5.0f);
	}
	else if ( event.key.keysym.sym == SDLK_UP)
	{
	  camera.Move(camera.GetForwardVector()*5.0f);
	}
	else if ( event.key.keysym.sym == SDLK_RETURN)
	{
	  bSimplified = !bSimplified;
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
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL );
    pRenderer->CommitCamera( camera );
    pRenderer->CommitColor( CVector4<unsigned char>(255,255,255,255));
    pRenderer->CommitTexture( 0, pTexture );
    pRenderer->CommitVertexDescriptor( pVertexDescriptor );
    pRenderer->CommitVertexDescriptor( pTexCoords );
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE );
    //glDepthRange (0.01, 1.0); 
    if ( bSimplified )
      pRenderer->CommitPrimitive( pIndices2 );
    else 
      pRenderer->CommitPrimitive( pIndices );
    //pRenderer->CommitColor( CVector4<unsigned char>(0,0,0,255));
    // glDepthRange (0.0, 1.0); 
//     glPolygonMode(GL_FRONT_AND_BACK, GL_LINE );
//     if ( pIndices2 != NULL )
//       pRenderer->CommitPrimitive( pIndices2 );
//     else 
//       pRenderer->CommitPrimitive( pIndices );
//     glPolygonOffset( 0.0, 0.0f);
    //pRenderer->DisableTexture( 0, pTexture );
    pRenderer->Finalize();
    CSDLScreen::GetInstance()->SwapBuffers();
    fpsCounter++;
    if ( fpsCounter.HasPassed(1,0) )
    {
      cerr << "FPS: " << fpsCounter << endl;
      fpsCounter.Reset();
    }    
  }
  return 0;
}
/////////////////////////////////////////////////////////////////
