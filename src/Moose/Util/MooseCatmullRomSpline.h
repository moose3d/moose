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
