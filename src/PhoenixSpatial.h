/////////////////////////////////////////////////////////////////
#ifndef __PhoenixSpatial_h__
#define __PhoenixSpatial_h__
namespace Phoenix
{
  namespace Spatial
  {
    /////////////////////////////////////////////////////////////////
    /// The base class for Objects with position.
    class CPositional 
    {
    protected:
      /// Vector where position is stored.
      CVector3<float> m_vPosition;
      /// has the position changed.
      int	      m_bPositionChanged;
    public:
      ////////////////////
      /// Default constructor.
      CPositional() : m_vPosition(0.0f,0.0f,0.0f),m_bPositionChanged(0)
      {
	
      }
      ////////////////////
      /// Parametrized constructor.
      /// \param vPosition vector for location-
      CPositional( const CVector3<float> & vPosition ) : m_vPosition(vPosition),m_bPositionChanged(0)
      {
      }
      ////////////////////
      /// Assigns position.
      /// \param vPosition vector of which values are used.
      inline void SetPosition( const CVector3<float> &vPosition )
      {
	m_vPosition = vPosition;
	SetPositionChanged(1);
      }
      ////////////////////
      /// Assigns the position.
      /// \param fX position on X-axis.
      /// \param fY position on Y-axis.
      /// \param fZ position on Z-axis.
      inline void SetPosition( float fX, float fY, float fZ )
      {
	m_vPosition[0] = fX;
	m_vPosition[1] = fY;
	m_vPosition[2] = fZ;
	SetPositionChanged(1);
      }
      ////////////////////
      /// Returns position.
      /// \param current position.
      inline const CVector3<float> & GetPosition() const
      {
	return m_vPosition;
      }
      ////////////////////
      /// Adds vector to current position.
      /// \param vPosition the movement applied to current position.
      inline void Move( const CVector3<float> & vPosition )
      {
	m_vPosition = m_vPosition + vPosition;
	SetPositionChanged(1);
      }
      ////////////////////
      /// Returns true if position has changed.
      /// \returns boolean
      int IsPositionChanged() const
      {
	return m_bPositionChanged;
      }
      ////////////////////
      /// Sets the position change flag.
      /// \param bFlag true for enable, false for disable.
      inline void SetPositionChanged(int bFlag )
      {
	m_bPositionChanged = bFlag;
      }
    };
    /////////////////////////////////////////////////////////////////
    /// The class for single-directional object.
    class COneDirectional 
    {
    protected:
      /// Direction vector.
      CVector3<float> m_vDirection;
    public:
      ////////////////////
      /// Assigns the direction
      /// \param vDirection direction vector to be assigned.
      inline void SetDirection( const CVector3<float> & vDirection )
      {
	m_vDirection = vDirection;
      }
      ////////////////////
      /// Returns the direction.
      /// \returns Vector with current direction.
      inline const CVector3<float> & GetDirection() const
      {
	return m_vDirection;
      }
    };
    /////////////////////////////////////////////////////////////////



#ifndef _WIN32EXPORT 
    // WHOA!! This was probably the most mind-boggling thing to fix. This is 
    // related to the differences between UNIX shared object files (.so) and 
    // Windows's Dynamically Linked Libraries (.DLL) 
    // Using Singleton objects in a DLL and application using the DLL without 
    // the following extern declaration makes the compiler to create two instances
    // of the singleton; one inside the DLL and one inside the application binary.
    // And since the point of Singleton is to have a single instance of an object, so 
    // this nasty feature wreaks havoc on implementation. Following statements 
    // say that the CSingleton<CMaterialMgr> template has already been 
    // created in DLL and should not be created again.
#ifdef WIN32
    class CMaterialMgr;
    extern template class __declspec(dllimport) CSingleton<CMaterialMgr>;
#endif
#endif
    ////////////////////
    /// Vertex class for some cases where it is easier to handle things as a blob.
    class CVertex
    {
    public:
      /// Vertex position.
      CVector3<float> m_vPosition;
      /// Vertex normal.
      CVector3<float> m_vNormal;
      /// Vertex texture coordinates.
      CVector2<float> m_vTexCoord;
      /// Vertex color.
      CVector3<unsigned char> m_vColor;
      ////////////////////
      /// The equality comparison operator.
      /// \param vert CVertex object which this is compared against.
      bool operator==( CVertex vert)
      {
	return (m_vPosition == vert.m_vPosition &&
		m_vNormal   == vert.m_vNormal   &&
		m_vTexCoord == vert.m_vTexCoord && 
		m_vColor    == vert.m_vColor );
      }
      ////////////////////
      /// The less than comparison operator.
      bool operator< (CVertex vert)
      {
	return m_vPosition[CVector3::Y] < vert.m_vPosition[CVector3::Y];
      }
    };
  }; // namespace Spatial
}; // namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
