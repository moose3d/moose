#include <MooseQuad.h>
#include <MooseVector2.h>
#include <iostream>
using namespace Moose::Math;
using namespace std;
/////////////////////////////////////////////////////////////////
std::ostream&
Moose::Math::operator<<( std::ostream &stream, const CQuad & quad )
{
  stream << quad.GetPosition() << "," << quad.GetWidth() << "x" << quad.GetHeight() << endl;
  return stream;
}
////////////////////////////////////////////////////////////////
void
Moose::Math::CQuad::CreateFrom2DCoords( int iX1, int iY1, int iX2, int iY2 )
{
	int xMin, yMin, xMax, yMax;
	if ( iX1 < iX2 ) { xMin = iX1; xMax = iX2; }
	else						 { xMin = iX2; xMax = iX1; }
	if ( iY1 < iY2 ) { yMin = iY1; yMax = iY2; }
	else						 { yMin = iY2; yMax = iY1; }

	CVector2<float> vMin( xMin, yMin );
	CVector2<float> vMax( xMax, yMax );
	CVector2<float> vCenter = vMin + (vMax * 0.5f);
	CVector2<float> vDim     = vMax - vMin;
	SetPosition( vCenter[0], vCenter[1], 0.0f );
	SetWidth(  vDim[0] );
	SetHeight( vDim[1] );
}
////////////////////////////////////////////////////////////////
