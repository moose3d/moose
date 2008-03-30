/////////////////////////////////////////////////////////////////
#ifndef __PhoenixSpatial_h__
#define __PhoenixSpatial_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixVector4.h"
#include "PhoenixVector3.h"
#include "PhoenixVector2.h"
#include "PhoenixQuaternion.h"
#include "PhoenixMatrix4x4.h"
#include "PhoenixMatrix3x3.h"
#include "PhoenixDefaultEntities.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Math 
  {
    void RotateVector( const CQuaternion &qRotation, CVector3<float> &vVector);
  }
  namespace Spatial
  {
    /////////////////////////////////////////////////////////////////
    /// The base class for Objects with position.
    class CPositional 
    {
    protected:
      /// Vector where position is stored.
      Phoenix::Math::CVector3<float> m_vPosition;
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
      CPositional( const Phoenix::Math::CVector3<float> & vPosition ) : m_vPosition(vPosition),m_bPositionChanged(0)
      {
      }
      ////////////////////
      /// Assigns position.
      /// \param vPosition vector of which values are used.
      inline void SetPosition( const Phoenix::Math::CVector3<float> &vPosition )
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
      inline const Phoenix::Math::CVector3<float> & GetPosition() const
      {
	return m_vPosition;
      }
      
      ////////////////////
      /// Adds vector to current position.
      /// \param vPosition the movement applied to current position.
      inline void Move( const Phoenix::Math::CVector3<float> & vPosition )
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
      Phoenix::Math::CVector3<float> m_vDirection;
    public:
      ////////////////////
      /// Assigns direction.
      /// \param vDirection direction vector to be assigned.
      inline void SetDirection( const Phoenix::Math::CVector3<float> & vDirection )
      {
	m_vDirection = vDirection;
      }
      ////////////////////
      /// Assigns direction.
      /// \param fX X-component of direction vector.
      /// \param fY Y-component of direction vector.
      /// \param fZ Z-component of direction vector.
      inline void SetDirection( float fX, float fY, float fZ )
      {
	m_vDirection[0] = fX;
	m_vDirection[1] = fY;
	m_vDirection[2] = fZ;
      }
      ////////////////////
      /// Returns direction.
      /// \returns Vector with current direction.
      inline Phoenix::Math::CVector3<float> & GetDirection() 
      {
	return m_vDirection;
      }
      ////////////////////
      /// Returns direction.
      /// \returns Vector with current direction.
      inline const Phoenix::Math::CVector3<float> & GetDirection() const
      {
	return m_vDirection;
      }
    };
    /////////////////////////////////////////////////////////////////




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

    //    class CMaterialMgr;
    //extern template class __declspec(dllimport) CSingleton<CMaterialMgr>;
    /// Vertex comparison flags
    enum VERTEX_COMP_FLAGS 
    {
      VERTEX_COMP_POSITION  = 1 << 0,
      VERTEX_COMP_TEXCOORD  = 1 << 1,
      VERTEX_COMP_COLOR     = 1 << 2,
      VERTEX_COMP_NORMAL    = 1 << 3
    };
    ////////////////////
    /// Vertex class for some cases where it is easier to handle things as a blob.
    class CVertex : public Phoenix::Spatial::CPositional
    {
    public:
      /// Vertex normal.
      Phoenix::Math::CVector3<float> m_vNormal;
      /// Vertex texture coordinates.
      Phoenix::Math::CVector2<float> m_vTexCoord[TEXTURE_HANDLE_COUNT];
      /// Vertex color.
      Phoenix::Math::CVector4<unsigned char> m_vColor;

      Phoenix::Math::CVector4<float> m_vAttrib4;
      Phoenix::Math::CVector3<float> m_vAttrib3;
      Phoenix::Math::CVector2<float> m_vAttrib2;
      float			     m_fAttrib1;

      ////////////////////
      /// Constructor.
      CVertex()
      {
	// init normal to neg z-axis
	m_vNormal[0] =  0.0f;
	m_vNormal[1] =  0.0f;
	m_vNormal[2] = -1.0f;
	// init texture coordiantes to zero
	for(int iT=0;iT<TEXTURE_HANDLE_COUNT;iT++)
	{
	  m_vTexCoord[iT][0] = 0.0f;
	  m_vTexCoord[iT][1] = 0.0f;
	}
	// Reset color to grayish
	m_vColor[0] = m_vColor[1] = m_vColor[2] = 200;
	m_vColor[3] = 255;
      }

      //       bool operator==( const CVertex & vert)
//       {
// 	for(int iT=0;iT<TEXTURE_HANDLE_COUNT;iT++)
// 	{
// 	  if ( !(m_vTexCoord[iT] == vert.m_vTexCoord[iT]) )  
// 	  {
// 	    return 0;  
// 	  }
// 	}
// 	if ( !(GetPosition() == vert.GetPosition())) 
// 	{
// 	  return 0;
// 	}
// 	if ( !(m_vNormal     == vert.m_vNormal))     { return 0; }
// 	if ( !(m_vColor      == vert.m_vColor ))     { return 0; }
// 	return 1;
//       }
      ////////////////////
      /// The equality comparison operator.
      /// \param vert CVertex object which this is compared against.
      /// \param iCompFlags Which parts of vertex data is compared. By default, all of them.
      bool Compare( const CVertex & vert, int iCompFlags = VERTEX_COMP_TEXCOORD | VERTEX_COMP_POSITION | VERTEX_COMP_NORMAL | VERTEX_COMP_COLOR )
      {
	// texture coordinate comparsion
	if ( iCompFlags & VERTEX_COMP_TEXCOORD )
	{
	  for(int iT=0;iT<TEXTURE_HANDLE_COUNT;iT++)
	  {
	    if ( !(m_vTexCoord[iT] == vert.m_vTexCoord[iT]) )  
	    {
	      return 0;  
	    }
	  }
	}

	// position comparision
	if ( iCompFlags & VERTEX_COMP_POSITION )
	{
	  if ( !(GetPosition() == vert.GetPosition())) 
	  {
	    return 0;
	  } 
	}

	// normal comparison
	if ( iCompFlags & VERTEX_COMP_NORMAL )
	{
	  if ( !(m_vNormal     == vert.m_vNormal))     { return 0; }
	}

	// color comparison
	if ( iCompFlags & VERTEX_COMP_COLOR )
	{
	  if ( !(m_vColor      == vert.m_vColor ))     { return 0; }
	}
	
	return 1;
      }
      ////////////////////
      /// The less than comparison operator.
      bool operator< (const CVertex & vert)
      {
	return m_vPosition[1] < vert.m_vPosition[1];
      }
      ////////////////////
      /// Assigns normal vector.
      /// \param vNormal Normal vector.
      inline void SetNormal( const Phoenix::Math::CVector3<float> &vNormal )
      {
	m_vNormal = vNormal;
      }
      ////////////////////
      /// Assigns normal vector.
      /// \param vNormal Normal vector.
      inline void SetNormal( float fX, float fY, float fZ )
      {
	m_vNormal[0] = fX;
	m_vNormal[1] = fY;
	m_vNormal[2] = fZ;
      }
      ////////////////////
      /// Returns normal vector.
      /// \returns Normal vector reference.
      inline const Phoenix::Math::CVector3<float> GetNormal() const
      {
	return m_vNormal;
      }
      ////////////////////
      /// Sets texture coordinate.
      /// \param fS S-coordinate.
      /// \param fT T-coordinate.
      /// \param nTexUnit To which texunit coordinates apply. By default, it is zero.
      void SetTextureCoordinates( float fS, float fT, unsigned int nTexUnit = 0 )
      {
	m_vTexCoord[nTexUnit][0] = fS;
	m_vTexCoord[nTexUnit][1] = fT;
      }
      ////////////////////
      /// Sets texture coordinate.
      /// \param vCoords Coordinates.
      /// \param nTexUnit To which texunit coordinates apply. By default, it is zero.
      void SetTextureCoordinates( Phoenix::Math::CVector2<float> vCoords, unsigned int nTexUnit = 0 )
      {
	m_vTexCoord[nTexUnit] = vCoords;
      }
      ////////////////////
      /// Returns texture coordinates.
      /// \param nTexUnit To which texunit coordinates apply. By default, it is zero.
      /// \returns Texture coordinate vector.
      const Phoenix::Math::CVector2<float> & GetTextureCoordinates( unsigned int nTexUnit = 0 ) const
      {
	return m_vTexCoord[nTexUnit];
      }
      ////////////////////
      /// Returns color vector.
      /// \return Color vector.
      const Phoenix::Math::CVector4<unsigned char> & GetColor() const
      {
	return m_vColor;
      }
      ////////////////////
      /// Returns color vector.
      /// \return Color vector.
      Phoenix::Math::CVector4<unsigned char> & GetColor() 
      {
	return m_vColor;
      }
      ////////////////////
      /// Assigns color vector.
      /// \param vColor Color to be assigned.
      void SetColor( const Phoenix::Math::CVector4<unsigned char> & vColor )
      {
	m_vColor = vColor;
      }
      ////////////////////
      /// Assigns vertex attribute values for shader.
      /// \param vValues Vector containing attrib values.
      void SetAttrib4( const Phoenix::Math::CVector4<float> & vValues )
      {
	m_vAttrib4 = vValues;
      }
      ////////////////////
      /// Assigns vertex attribute values for shader.
      /// \param vValues Vector containing attrib values.
      void SetAttrib3( const Phoenix::Math::CVector3<float> & vValues )
      {
	m_vAttrib3 = vValues;
      }
      ////////////////////
      /// Assigns vertex attribute values for shader.
      /// \param vValues Vector containing attrib values.
      void SetAttrib2( const Phoenix::Math::CVector2<float> & vValues )
      {
	m_vAttrib2 = vValues;
      }
      ////////////////////
      /// Assigns vertex attribute value for shader.
      /// \param fValue Attrib value.
      void SetAttrib1( const float & fValue )
      {
	m_fAttrib1 = fValue;
      }
      /////////////////////////////
      /// Returns vertex attribute.
      /// \returns vertex attribute
      const float & GetAttrib1() const
      {
	return m_fAttrib1;
      }
      /////////////////////////////
      /// Returns vertex attribute.
      /// \returns vertex attribute
      const Phoenix::Math::CVector2<float> & GetAttrib2() const
      {
	return m_vAttrib2;
      }
      /////////////////////////////
      /// Returns vertex attribute.
      /// \returns vertex attribute
      const Phoenix::Math::CVector3<float> & GetAttrib3() const
      {
	return m_vAttrib3;
      }
      /////////////////////////////
      /// Returns vertex attribute.
      /// \returns vertex attribute
      const Phoenix::Math::CVector4<float> & GetAttrib4() const
      {
	return m_vAttrib4;
      }
      ////////////////////
      /// For debugging.
      friend std::ostream & operator<<( std::ostream & stream, const CVertex & vertex)
      {
	stream << vertex.GetPosition() << ",";
	for(int iT=0;iT<TEXTURE_HANDLE_COUNT;iT++)
	{
	  stream << vertex.m_vTexCoord[iT] << ",";
	}
	stream << vertex.m_vNormal << ",";
	stream << vertex.m_vColor;
	return stream;
      }
    };
    /////////////////////////////////////////////////////////////////
    /// Triangle class for editors.
    class CTriangle 
    {
    protected:
      /// Vertices in a triangle
      CVertex			m_vVertices[3];
      /// Is this triangle selected.
      int			m_bSelected;
    public:
      ////////////////////
      /// Constructor.
      CTriangle() : m_bSelected(0)
      {
	
      }
      ////////////////////
      /// Is this triangle selected.
      /// \returns Selection status.
      int IsSelected() const
      {
	return m_bSelected;
      }
      ////////////////////
      /// Sets selection status.
      /// \param bFlag Boolean selection status, true for selection, false for unselection.
      void SetSelected( int bFlag )
      {
	m_bSelected = bFlag;
      }
      ////////////////////
      /// Returns vertex.
      /// \param nCorner Which corner vertex of triangle (0,1 or 2)
      /// \returns Reference to vertex.
      CVertex & GetVertex( unsigned int nCorner )
      {
	return m_vVertices[ nCorner % 3];
      }
      ////////////////////
      /// Returns vertex.
      /// \param nCorner Which corner vertex of triangle (0,1 or 2)
      /// \returns Reference to vertex.
      const CVertex & GetVertex( unsigned int nCorner ) const
      {
	return m_vVertices[ nCorner % 3];
      }
    };
    /////////////////////////////////////////////////////////////////
    /// Generic orientable object. Contains base vectors
    /// which are rotated using the provided methods.
    class COrientable
    {
    protected:

      /// The vector pointing right.
      Phoenix::Math::CVector3<float> m_vRight;
      /// The vector pointing upwards.
      Phoenix::Math::CVector3<float> m_vUpward;
      /// The vector pointing forward.
      Phoenix::Math::CVector3<float> m_vForward;
      /// The quaternion holding rotations. 
      Phoenix::Math::CQuaternion m_qRotation;
      /////////////////////////////////////////////////////////////////
      /// A boolean flag indicating the change of rotation.
      char	m_bRotationChanged;
    public:
      /////////////////////////////////////////////////////////////////
      /// The constructor.
      COrientable();
      /////////////////////////////////////////////////////////////////
      /// The destructor.
      ~COrientable() {   }
      /////////////////////////////////////////////////////////////////
      /// Rotates object given degrees over axis (1,0,0)
      /// \param fDegrees the degrees the object is rotated around axis.
      void RotateAroundX          ( float    fDegrees );
      /////////////////////////////////////////////////////////////////
      /// Rotates object given degrees over axis (0,1,0)
      /// \param fDegrees the degrees the object is rotated around axis.
      void RotateAroundY          ( float    fDegrees );
      /////////////////////////////////////////////////////////////////
      /// Rotates object given degrees over axis (0,0,1)
      /// \param fDegrees the degrees the object is rotated around axis.
      void RotateAroundZ          ( float    fDegrees );
      /////////////////////////////////////////////////////////////////
      /// Rotates object over world x,y,z axis by given degrees.
      /// \param vAngles the vector where angles for x,y,z axis rotations is given.
      void Rotate           ( const Phoenix::Math::CVector3<float>  &vAngles );
      /////////////////////////////////////////////////////////////////
      /// Rotates object over world x,y,z axis by given degrees.
      /// \param fAroundWorldXAngle The degrees by which the object is rotated over world x-axis.
      /// \param fAroundWorldYAngle The degrees by which the object is rotated over world y-axis.
      /// \param fAroundWorldZAngle The degrees by which the object is rotated over world z-axis.
      void Rotate           ( float fAroundWorldXAngle, 
			      float fAroundWorldYAngle, 
			      float fAroundWorldZAngle );
      /////////////////////////////////////////////////////////////////
      /// Rotates object given degrees over axis "right".
      /// \param fDegrees the degrees the object is rotated around axis.
      void RotateAroundRight   ( float    fDegrees );
      /////////////////////////////////////////////////////////////////
      /// Rotates object given degrees over axis "up".
      /// \param fDegrees the degrees the object is rotated around axis.
      void RotateAroundUp      ( float    fDegrees );
      /////////////////////////////////////////////////////////////////
      /// Rotates object given degrees over axis "forward".
      /// \param fDegrees the degrees the object is rotated around axis.
      void RotateAroundForward ( float    fDegrees );
      /////////////////////////////////////////////////////////////////
      /// Rotates object over local axes by given degrees.
      /// \param vAngles the vector where angles for right,up and forward axis rotations is given.
      void RotateLocal         ( Phoenix::Math::CVector3<float>  &vAngles );
      /////////////////////////////////////////////////////////////////
      /// Rotates object over local right,up,forward axis by given degrees.
      /// \param fAroundRightAngle The degrees by which the object is rotated over local right axis.
      /// \param fAroundUpAngle The degrees by which the object is rotated over local up axis.
      /// \param fAroundForwardAngle The degrees by which the object is rotated over local forward axis.
      void RotateLocal          ( float fAroundRightAngle, 
				  float fAroundUpAngle, 
				  float fAroundForwardAngle );
      /////////////////////////////////////////////////////////////////
      /// Initializes the object by assigning axis forward and up, the right
      /// axis is calculated by cross product of the two.
      /// Rotation is set to unit.
      void         SetDirectionForwardUp   ( Phoenix::Math::CVector3<float> vForward, 
					     Phoenix::Math::CVector3<float> vUp );
      /////////////////////////////////////////////////////////////////
      /// Return the right vector.
      /// \return CVector3<float> The vector currently used as local right axis.
      inline const Phoenix::Math::CVector3<float> & GetRightVector   () const
      {
	return m_vRight;
      }
      /////////////////////////////////////////////////////////////////
      /// Return the up vector.
      /// \return CVector3<float> The vector currently used as local up axis.
      inline const Phoenix::Math::CVector3<float> & GetUpVector      () const
      {
	return m_vUpward;
      }
      /////////////////////////////////////////////////////////////////
      /// Return the forward vector.
      /// \return CVector3<float> The vector currently used as local forward axis.
      inline const Phoenix::Math::CVector3<float> & GetForwardVector () const
      {
	return m_vForward;
      }
      /////////////////////////////////////////////////////////////////
      /// Returns the current rotation.
      /// \returns CQuaternion the rotation quaternion.
      inline const Phoenix::Math::CQuaternion	& GetRotationQuaternion() const
      {
	return m_qRotation;
      }
      /////////////////////////////////////////////////////////////////
      /// Initializes the orientation by given local axis and rotates them
      /// by given quaternion.
      /// \param vForward The vector for forward vector.
      /// \param vRight The vector for right vector.
      /// \param vUp The vector for up vector.
      /// \param qQuat The rotation quaternion which is applied to vectors.
      void			SetRotationFromInitialState( const Phoenix::Math::CVector3<float> &vForward,
							     const Phoenix::Math::CVector3<float> &vRight,
							     const Phoenix::Math::CVector3<float> &vUp,
							     const Phoenix::Math::CQuaternion & qQuat);
      /////////////////////////////////////////////////////////////////
      /// Rotates the orientation by given quaternion.
      /// \param qQuaternion The rotation which will be applied to the current rotation.
      void			AppendToRotation( const Phoenix::Math::CQuaternion &qQuaternion);
      /////////////////////////////////////////////////////////////////
      /// Returns boolean has the rotation changed.
      /// \returns boolean true if any rotations have been applied, false otherwise.
      inline int	       IsRotationChanged() const
      {
	return m_bRotationChanged;
      }
      /////////////////////////////////////////////////////////////////
      /// Sets the flag which indicates rotation change.
      /// \param bFlag boolean flag.
      inline void SetRotationChanged( int bFlag )
      {
	m_bRotationChanged = bFlag;
      }
      /////////////////////////////////////////////////////////////////
      /// Resets the orientation to original state.
      /// Forward = (0,0,-1)
      /// Up = ( 0,1,0)
      /// Right = ( 1,0,0)
      void			Reset();
      /////////////////////////////////////////////////////////////////
      /// Resets the orientation and applies rotation matrix mMatrix to vectors.
      /// \param mMatrix 4x4 rotation matrix which is applied to original vectors.
      void			SetRotation( const Phoenix::Math::CMatrix4x4<float> &mMatrix );
      /////////////////////////////////////////////////////////////////
      /// Resets the orientation and applies rotation qRotation to vectors.
      /// \param qRotation The quaternion which is applied to original vectors.
      void			SetRotation( const Phoenix::Math::CQuaternion &qRotation );
    protected:
      /////////////////////////////////////////////////////////////////
      /// Rotates all orientation vectors by q.
      /// \param q The quaternion which is applied to all vectors.
      inline void RotateAllDirections( const Phoenix::Math::CQuaternion &q )
      {
	Phoenix::Math::RotateVector(q, m_vRight);
	Phoenix::Math::RotateVector(q, m_vForward);
	Phoenix::Math::RotateVector(q, m_vUpward);
      }
    }; // COrientable

    ////////////////////
    /// A class which provides dimensions in 1D (or n-dimensional object with equivalent size in all dimensions.
    class CDimensional1D
    {
    protected:
      /// The width.
      float m_fWidth;
      /// Width * 0.5
      float m_fHalfWidth;
      ////////////////////
      /// Constructor. Initializes paramaters.
      CDimensional1D() : 
	m_fWidth(0.0f), 
	m_fHalfWidth(0.0f)
      {
    
      }
      ////////////////////
      /// The parametrized constructor. 
      /// \param fWidth The width of the object
      CDimensional1D( float fWidth )
      {
	SetWidth(fWidth);
      }
    public:
      ////////////////////
      /// Assigns the width.
      /// \param fWidth The value for width.
      inline void SetWidth ( float fWidth  )
      {  
	m_fWidth = fWidth;
	m_fHalfWidth = m_fWidth * 0.5f;
      }
      ////////////////////
      /// Returns the width.
      /// \returns The width.
      inline float GetWidth() const
      {	
	return m_fWidth;
      }
      ////////////////////
      /// Returns half of the width
      /// \returns half of the width
      inline float GetHalfWidth() const 
      {
	return m_fHalfWidth;
      }
      
    }; // CDimensional1D
    /////////////////////////////////////////////////////////////////
    /// A class which provides dimensions in 2D - width, height.
    class CDimensional2D : public CDimensional1D
    {
    protected:
      /// The height.
      float m_fHeight;
      /// Height * 0.5
      float m_fHalfHeight;
      ////////////////////
      /// The constructor. Initializes paramaters.
      CDimensional2D() : 
	CDimensional1D(),
	m_fHeight(0.0f), 
	m_fHalfHeight(0.0f)
      {
	
      }
      ////////////////////
      /// The parametrized constructor. 
      /// \param fWidth The width of the object
      /// \param fHeight The height of the object
      CDimensional2D( float fWidth, float fHeight) 
      {
	SetWidth(fWidth);
	SetHeight(fHeight);
      }
    public:
      ////////////////////
      /// Assigns the height.
      /// \param fHeight The value for height.
      inline void SetHeight( float fHeight )    
      {
	m_fHeight = fHeight;
	m_fHalfHeight = m_fHeight * 0.5f;
      }
      ////////////////////
      /// Returns the height.
      /// \returns The height.
      inline float GetHeight() const
      {	
	return m_fHeight;
      }
      ////////////////////
      /// Returns half of the height
      /// \returns half of the height
      inline float GetHalfHeight() const 
      {
	return m_fHalfHeight;
      }
    }; // CDimensional2D
    /////////////////////////////////////////////////////////////////
    /// A class which provides dimensions in 3D - width, height and length.
    class CDimensional3D : public CDimensional2D
    {
    protected:
      /// The length.
      float m_fLength;
      /// Length * 0.5
      float m_fHalfLength;
      ////////////////////
      /// The constructor. Initializes paramaters.
      CDimensional3D() : 
	CDimensional2D(),
	m_fLength(0.0f),
	m_fHalfLength(0.0f)
      {
    
      }
      ////////////////////
      /// The parametrized constructor. 
      /// \param fWidth The width of the object
      /// \param fHeight The height of the object
      /// \param fLength The length of the object
      CDimensional3D( float fWidth, float fHeight, float fLength ) 
      {
	SetWidth(fWidth);
	SetHeight(fHeight);
	SetLength(fLength);
      }
    public:
      ////////////////////
      /// Assigns the length.
      /// \param fLength The value for length.
      inline void SetLength( float fLength )
      {	
	m_fLength = fLength;
	m_fHalfLength = m_fLength * 0.5f;
      }
      ////////////////////
      /// Returns the length.
      /// \returns The length.
      inline float GetLength() const
      {	
	return m_fLength;
      }
      ////////////////////
      /// Returns half of the length
      /// \returns half of the length
      inline float GetHalfLength() const 
      {
	return m_fHalfLength;
      }
    }; // CDimensional3D
  }; // namespace Spatial
}; // namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
