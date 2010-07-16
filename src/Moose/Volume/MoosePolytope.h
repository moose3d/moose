#ifndef MOOSEPOLYTOPE_H_
#define MOOSEPOLYTOPE_H_
/////////////////////////////////////////////////////////////////
#include "MoosePlane.h"
#include "MoosePolytope.h"
#include "MooseIndexArray.h"
#include "MooseTransform.h"
#include <iostream>
#include <vector>
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Volume
  {
		/////////////////////////////////////////////////////////////////
    // A class for discrete oriented polytope, defines a convex bounding
    // volume by set of planes.
    class MOOSE_API CPolytope
    {
    protected:
      std::vector<Moose::Math::CPlane> m_Planes;
    public:
      ////////////////////
      CPolytope( size_t nNumPlanes = 1)      { m_Planes.reserve(nNumPlanes); }
      ////////////////////
      ~CPolytope()      { m_Planes.clear();  }
      ////////////////////
      void AddPlane( Moose::Math::CPlane &Plane )
      {
      	m_Planes.push_back(Plane);
      }
      ////////////////////
      std::vector<Moose::Math::CPlane> &GetPlanes()
      {
      	return m_Planes;
      }
      ////////////////////
      const std::vector<Moose::Math::CPlane> & GetPlanes() const
      {
      	return m_Planes;
      }
      ////////////////////
      size_t GetNumPlanes()
      {
      	return m_Planes.size();
      }
      ////////////////////
      friend std::ostream& operator<<( std::ostream &stream, const Moose::Volume::CPolytope & polytope )
      {
				std::vector<Moose::Math::CPlane>::const_iterator it = polytope.m_Planes.begin();
				stream << "KDOP planes:" << std::endl;
				for( ; it != polytope.m_Planes.end(); it++)
				{
					stream << *it << std::endl;
				}
				return stream;
      }
    }; // CPolytope
  } // Volume
} // Moose
///////////////////////////////////////////////////////////////////////////////
#endif /* MOOSEPOLYTOPE_H_ */
