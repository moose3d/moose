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

#ifndef __MooseCatmullRomSpline_h__
#define __MooseCatmullRomSpline_h__
////////////////////////////////////////////////////////////////////////////////
#include <vector>
#include <cstring>
#include <MooseVector3.h>
////////////////////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Util
  {
    class CCatmullRomSpline : protected std::vector< Moose::Math::CVector3<float> >
    {
    public:
      CCatmullRomSpline() {}
      CCatmullRomSpline( size_t nControlPoints )
      {
        for(size_t i=0;i<nControlPoints;i++) this->push_back( Moose::Math::CVector3<float>(0,0,0));
      }

      virtual ~CCatmullRomSpline()
      {
      
      }

      void Resize( size_t nSize )
      {
        this->resize(nSize);
      }

      void SetPoint( size_t index, const Moose::Math::CVector3<float> & pos )
      {
        this->at(index) = pos;
      }

      void Append( const Moose::Math::CVector3<float> & pos )
      {
        this->push_back( pos );
      }

      void Insert( size_t index, const Moose::Math::CVector3<float> & pos )
      {
        this->insert(this->begin()+index, pos);
      }

      const Moose::Math::CVector3<float> & GetPoint( size_t index ) const
      {
        return this->at(index);
      }

      Moose::Math::CVector3<float> & GetPoint( size_t index )
      {
        return this->at(index);
      }
      Moose::Math::CVector3<float> GetPosition( float t ) const;
     
#ifndef SWIG
      Moose::Math::CVector3<float> & operator[]( size_t nIndex) 
      { 
        return this->at(nIndex); 
      }
      const Moose::Math::CVector3<float> & operator[]( size_t nIndex) const 
      {
        return this->at(nIndex);
      }
      Moose::Math::CVector3<float> operator()( float t ) const 
      {
        return GetPosition(t); 
      }
#endif

    };
  }
}
////////////////////////////////////////////////////////////////////////////////
#endif
