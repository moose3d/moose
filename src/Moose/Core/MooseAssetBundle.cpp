#include "MooseAssetBundle.h"
#include "MooseExceptions.h"
#include "MooseAPI.h"
#include <fstream>
#include <sstream>
using namespace std;
////////////////////////////////////////////////////////////////////////////////
namespace prefix=Moose::Core;
////////////////////////////////////////////////////////////////////////////////
prefix::CAssetBundle::CAssetBundle()
{
  AddAssetRoot("/usr/share/Moose/Assets");
}
////////////////////////////////////////////////////////////////////////////////
prefix::CAssetBundle::~CAssetBundle()
{

}
////////////////////////////////////////////////////////////////////////////////
void
prefix::CAssetBundle::AddAssetRoot( const std::string & dir )
{
  m_setPrefices.insert(dir);
}
////////////////////////////////////////////////////////////////////////////////
void
prefix::CAssetBundle::RemoveAssetRoot( const std::string & dir )
{
  std::set<std::string>::iterator pos = m_setPrefices.find(dir);
  if ( pos != m_setPrefices.end())
  {
    m_setPrefices.erase(pos);
  }
}
////////////////////////////////////////////////////////////////////////////////
std::string 
prefix::CAssetBundle::GetAssetPath( const std::string & assetFile ) throw (Moose::Exceptions::CNoSuchAssetException)
{
  std::set<std::string>::iterator it = m_setPrefices.begin();
  while ( it != m_setPrefices.end())
  {
    // Create full path
    ostringstream file;
    file << *it << "/" << assetFile;
    // Try to open
    ifstream s(file.str().c_str(), ifstream::in);
    // Was it readable?
    if ( s.good() ) {
      s.close();
      return file.str();
    }
    it++;
  }
  // no such asset
  throw (Moose::Exceptions::CNoSuchAssetException(assetFile.c_str()));  
}
////////////////////////////////////////////////////////////////////////////////
std::string 
prefix::CAssetBundle::GetAssetPaths() const
{

  ostringstream paths;
  std::set<std::string>::iterator it = m_setPrefices.begin();
  while ( it != m_setPrefices.end())
  {
    paths << *it << " ";
    it++;
  }
  return paths.str();
}
////////////////////////////////////////////////////////////////////////////////
