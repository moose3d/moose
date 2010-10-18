#ifndef __MooseAssetBundle_h__
#define __MooseAssetBundle_h__
////////////////////////////////////////////////////////////////////////////////
#include <string>
#include <set>
#include <MooseSingleton.h>
#include <MooseExceptions.h>
////////////////////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Core
  {
    ////////////////////
    /// Utility to get full asset paths just by typing
    /// asset filename.
    class CAssetBundle : public Moose::Core::CSingleton<Moose::Core::CAssetBundle>
    {
      friend class Moose::Core::CSingleton<Moose::Core::CAssetBundle>;
    protected:
      std::set<std::string> m_setPrefices;
      CAssetBundle();
      virtual ~CAssetBundle();
    public:
      void AddAssetRoot( const std::string & dir );
      void RemoveAssetRoot( const std::string & dir );
      std::string GetAssetPath( const std::string & assetFile ) throw (Moose::Exceptions::CNoSuchAssetException);
      std::string GetAssetPaths() const;
    };
  }
}
////////////////////////////////////////////////////////////////////////////////
#endif
