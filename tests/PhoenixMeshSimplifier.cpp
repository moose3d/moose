#include <Phoenix.h>
#include <iostream>
#include <list>
#include <vector>
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
#define QUITE_HUGE     594343.0f;
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
  Vertex()
  {
    cost = 0.0f;
  }

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
  
  void AddFaceIndex( size_t ind )
  {
    faces.push_back(ind);
  }

};

class Triangle
{
public:
  size_t v0, v1, v2;
  CVector3<float>	GetNormal( vector<Vertex> & vertices ) 
  {
    CVector3<float> tmp1 = vertices[v1].position - vertices[v0].position;
    CVector3<float> tmp2 = vertices[v2].position - vertices[v0].position;
    return tmp1.Cross(tmp2).GetNormalized();
  }
  
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
/////////////////////////////////////////////////////////////////
int main( int argc, char **argv )
{
  
  CMilkshapeLoader *pLoader = new CMilkshapeLoader();
  CVertexDescriptor *pVertexDescriptor = NULL; 

  assert(pLoader->Load("Resources/Models/TestMesh.ms3d") == 0);
  pLoader->GenerateModelData();

  pVertexDescriptor = pLoader->GetVertices();
  pLoader->ResetVertices();

  assert( pLoader->GetIndices().size() == 1 );
  CIndexArray *pIndices = pLoader->GetIndices()[0];
  pLoader->ResetIndices();
  delete pLoader;
  
  cerr << "vertices actually: " << pVertexDescriptor->GetSize() << endl;
  cerr << "indices actually: "  << pIndices->GetNumIndices()    << endl;
  
  /// Assign proper indices to vertices and set position
  vector<Vertex> vertices;
  /// Create triangles out from index vector
  vector<Triangle> triangles;


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
  
  for( size_t i=0;i<pIndices->GetNumIndices();i+=3)
  {
    Triangle tri;
    tri.v0 = pIndices->GetPointer<unsigned short int>()[i];
    tri.v1 = pIndices->GetPointer<unsigned short int>()[i+1];
    tri.v2 = pIndices->GetPointer<unsigned short int>()[i+2];
    triangles.push_back(tri);
  }

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

  
  for( size_t i=0;i<vertices.size();i++)
  {
    Vertex &v = vertices[i];

    // calculate normal for vertex from surrounding triangle normals.
    v.normal = CVector3<float>(0,0,0);
    vector<size_t>::iterator triIt = v.faces.begin();    
    for( ; triIt != v.faces.end(); triIt++)
    {
      v.normal += triangles[*triIt].GetNormal(vertices);
    }
    v.normal.Normalize();

    

    
    vector<size_t>::iterator it = v.neighbors.begin();
    for( ; it != v.neighbors.end();it++)
    {
      size_t ind = *it; 

      //cerr << "neighbor is : " << ind << endl;
      if ( ! SharedByTwoFaces( triangles, vertices, i, ind )) 
	continue;

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
	  }
	  
	}
      }
      //cerr << "indices were: " << vert1 << " and " << vert2 << endl;
      assert( tri1found == 1 && tri2found == 1 );
      
      tri1side = vD.Dot( vertices[vert1].position - v.position );
      tri2side = vD.Dot( vertices[vert2].position - v.position );
      
      
      // both vertices are on same side of edge, cannot remove 
      if ( (tri1side >= 0.0f) && (tri2side >= 0.0) ) continue;
      if ( (tri1side < 0.0f) && (tri2side < 0.0) ) continue;
      
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

      // compare other faces
      for( size_t t=0;t<v.faces.size();t++)
      {
	Triangle & tri = triangles[v.faces[t]];
	if ( !tri.HasEdge( i, *it) )
	{
	  float val = 0.0f;
	  if (tri.v0 != i )
	  {
	    val = vD.Dot( vertices[tri.v0].position - v.position );
	    if ( val > EPSILON ) 
	    {
	      float newD = posnormal.Dot(tri.GetNormal(vertices));
	      d = newD < d ? newD : d;
	    } 
	    else if ( val < -EPSILON )
	    {
	      float newD = negnormal.Dot(tri.GetNormal(vertices));
	      d = newD < d ? newD : d;
	    }
	  }

	  if ( tri.v1 != i )
	  {
	    val = vD.Dot( vertices[tri.v1].position - v.position );
	    if ( val > EPSILON ) 
	    {
	      float newD = posnormal.Dot(tri.GetNormal(vertices));
	      d = newD < d ? newD : d;
	    } 
	    else if ( val < -EPSILON )
	    {
	      float newD = negnormal.Dot(tri.GetNormal(vertices));
	      d = newD < d ? newD : d;
	    }
	  }

	  if ( tri.v2 != i )
	  {
	    val = vD.Dot( vertices[tri.v2].position - v.position );
	    if ( val > EPSILON ) 
	    {
	      float newD = posnormal.Dot(tri.GetNormal(vertices));
	      d = newD < d ? newD : d;
	    } 
	    else if ( val < -EPSILON )
	    {
	      float newD = negnormal.Dot(tri.GetNormal(vertices));
	      d = newD < d ? newD : d;
	    }
	  }
	  
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
  for( size_t v=1;v<vertices.size();v++)
  {
    cerr << "vertices[" << v << "]=" << vertices[v].cost << "\t" << vertices[v].position << endl;
    if( vertices[v].cost < vertices[lowest].cost )
    {
      lowest = v;
    }
  }
  cerr << "lowest cost was in vertex " << lowest << " with cost value : " << vertices[lowest].cost << endl;
  return 0;
}
  /////////////////////////////////////////////////////////////////
