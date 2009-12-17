#ifndef PHOENIXPOLYTOPE_H_
#define PHOENIXPOLYTOPE_H_
/////////////////////////////////////////////////////////////////
#include "PhoenixSpatial.h"
#include "PhoenixMathGeometry.h"
#include "PhoenixIndexArray.h"
#include "PhoenixTransform.h"
#include <iostream>
#include <vector>
#include "PhoenixAPI.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Volume
  {
		/////////////////////////////////////////////////////////////////
    // A class for discrete oriented polytope, defines a convex bounding
    // volume by set of planes.
    class PHOENIX_API CPolytope
    {
    protected:
      std::vector<Phoenix::Math::CPlane> m_Planes;
    public:
      ////////////////////
      CPolytope( size_t nNumPlanes = 1)      { m_Planes.reserve(nNumPlanes); }
      ////////////////////
      ~CPolytope()      { m_Planes.clear();  }
      ////////////////////
      void AddPlane( Phoenix::Math::CPlane &Plane )
      {
      	m_Planes.push_back(Plane);
      }
      ////////////////////
      std::vector<Phoenix::Math::CPlane> &GetPlanes()
      {
      	return m_Planes;
      }
      ////////////////////
      const std::vector<Phoenix::Math::CPlane> & GetPlanes() const
      {
      	return m_Planes;
      }
      ////////////////////
      size_t GetNumPlanes()
      {
      	return m_Planes.size();
      }
      ////////////////////
      friend std::ostream& operator<<( std::ostream &stream, const Phoenix::Volume::CPolytope & polytope )
      {
				std::vector<Phoenix::Math::CPlane>::const_iterator it = polytope.m_Planes.begin();
				stream << "KDOP planes:" << std::endl;
				for( ; it != polytope.m_Planes.end(); it++)
				{
					stream << *it << std::endl;
				}
				return stream;
      }
    }; // CPolytope
  } // Volume
} // Phoenix
///////////////////////////////////////////////////////////////////////////////
#endif /* PHOENIXPOLYTOPE_H_ */
