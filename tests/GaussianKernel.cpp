#include <math.h>
#include <iostream>
/////////////////////////////////////////////////////////////////
using std::cerr;
using std::endl;
/////////////////////////////////////////////////////////////////
/// Returns weight for point iX, iY in kernel with size iSize X iSize.
float GetWeight( unsigned int nX, unsigned int nY, unsigned int nSize, float fSigma = 1.0f)
{
  const float PI     = 3.14159265358979323f;
  const float fE     = 2.71828182846;

  int nCenter        = nSize/2;
  int nDiff[2]       = { nCenter - nX, nCenter - nY };
  //float fDistance    = sqrtf( nDiff[0] * nDiff[0]+  nDiff[1] * nDiff[1] );
  float fScale       = (1.0f / (sqrtf(2 * PI) * fSigma));  
  float fDistanceSq  = nDiff[0]*nDiff[0] + nDiff[1] * nDiff[1];
  float fSigmaSq_x2  = fSigma * fSigma * 2.0f ;
  float fPower       = -(fDistanceSq / fSigmaSq_x2);
  
  return fScale * powf(fE, fPower);
}
/////////////////////////////////////////////////////////////////
int main( int argc, char **argv )
{

  if ( argc != 3 )
  {
    cerr << "Usage: " << argv[0] << " <size> <sigma>" << endl;
    return 0;
  }
  
  int iSize = atoi(argv[1]);
  if ( iSize == 0 || iSize % 2 == 0)
  {
    cerr << "Second argument must be positive, odd integer." << endl;
    return 1; 
  }
  float fSigma = atof( argv[2] );
  if (fSigma <= 0.0f )
  {
    cerr << "Second argument must be positive float." << endl;
    return 1; 
  }
  
  // allocate proper kernel
  float * (*pKernel) = new float *[iSize];
  for( int i=0;i<iSize;i++)
  {
    pKernel[i] = new float[iSize];
  }
  // half size (index)
  int iHalfSize = (iSize/2)+1;
  for( int r=0;r<iHalfSize;r++)
  {
    for( int c=0;c<iHalfSize;c++)
    {
      pKernel[r][c] = GetWeight( r,c, iSize, fSigma);
      pKernel[r][iSize - c - 1] = pKernel[r][c];
      pKernel[iSize - r - 1 ][c] = pKernel[r][c];
      pKernel[iSize - r - 1 ][iSize - c - 1] = pKernel[r][c];
    }
  }
  // print kernel
  cerr << "= {" << endl;
  for( int r=0;r<iSize;r++)
  {
    cerr << "\t";
    for( int c=0;c<iSize;c++)
    {
      cerr << pKernel[r][c] << ",\t";
    }
    cerr << endl;
  }
  cerr << "};" << endl;
  
  cerr << "1D-convolution: " << endl << " = {";
  float *p1DKernel = new float[iSize];

  // calculate 1D convolution kernel
  for( int r=0;r<iSize;r++)
  {
    p1DKernel[r] = sqrtf(pKernel[r][r]);
    cerr << p1DKernel[r] << ", ";
  }
  cerr << "};" << endl;

  // test
  float fValue = 0.0f;
  for( int r=0;r<iSize;r++)
  {
    for( int c=0;c<iSize;c++)
    {
      fValue += pKernel[r][c];
    }
  }
  cerr << "2D kernel says: " << fValue << endl;
  // test
  fValue = 0.0f;
  for( int r=0;r<iSize;r++)
  {
    fValue += p1DKernel[r];
  }
  fValue *= fValue;
  cerr << "1D kernel says: " << fValue << endl;
  delete [] pKernel;
  delete p1DKernel;
  return 0;
}
/////////////////////////////////////////////////////////////////
