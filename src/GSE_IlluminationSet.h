#ifndef __GSE_IlluminationSet_h__
#define __GSE_IlluminationSet_h__
#include "GSE_Light.h"
#include <set>
#include <vector>
/////////////////////////////////////////////////////////////////
class GSE_LightClusterNode;
using std::set;
using std::vector;
/////////////////////////////////////////////////////////////////
/// The class which defines a set of lights affecting an object.
/// Each SceneNode will have this type of set 
// Author: eNtity/AG
/////////////////////////////////////////////////////////////////
class GSE_IlluminationSet 
{
  friend class GSE_IlluminationSetMgr;
 protected:
  /// The lights in this set.
  set<GSE_Light *> m_setLights;
  /// Sorted array of lights.
  vector<GSE_Light *> m_vSorted;
  /// Sorts the lights according to the brightness observed at
  /// given position.
  /// \param vPosition GSE_Vector3 position which is used to determine
  /// light ordering and compare brightness factor.
  void Sort( const GSE_Vector3 & vPosition );
  /// The default constructor.
  GSE_IlluminationSet(){}
 public:

  /// Removes given light from the set.
  /// \param pLight a pointer to GSE_Light which is to be removed.
  void RemoveLight( GSE_Light *pLight );
  /// Adds given light to the set. The same light is not added twice.
  /// \param pLight a pointer to GSE_Light which is to be added.
  void AddLight( GSE_Light *pLight );
  
  /// Applies the brightest lights (in respect to given position) to lightcluster.
  /// \param pLightCluster  where lights are enabled.
  /// \param vPosition position of the object which is lit.
  void Apply ( GSE_LightClusterNode *pLightCluster, const GSE_Vector3 & vPosition );
  /// Returns the number of lights in this set.
  inline unsigned int GetSize()
  {
    return m_setLights.size();
  }
};
/////////////////////////////////////////////////////////////////
class GSE_IlluminationSetMgr : public GSE_Singleton<GSE_IlluminationSetMgr>,
			       public GSE_Container<GSE_IlluminationSet>,
			       public GSE_Mapper<string,GSE_IlluminationSet>
{
  friend class GSE_Singleton<GSE_IlluminationSetMgr>;
 protected:
  GSE_IlluminationSetMgr(){}
  ~GSE_IlluminationSetMgr(){}
 public:
  GSE_IlluminationSet * CreateIlluminationSet( const char *szName );
};
			       
#endif
/////////////////////////////////////////////////////////////////
// History:
// $Log: GSE_IlluminationSet.h,v $
// Revision 1.6  2007/06/01 08:55:43  entity
// doxygen comments added
//
// Revision 1.5  2007/03/16 08:31:36  entity
// friend class declaration added
//
// Revision 1.4  2007/03/16 08:13:18  entity
// IlluminationSetMgr added
//
// Revision 1.3  2007/03/15 21:29:11  entity
// GetSize() added
//
// Revision 1.2  2007/03/15 21:02:21  entity
// Apply redefined
//
// Revision 1.1  2007/03/15 19:31:04  entity
// Initial import to CVS
//
/////////////////////////////////////////////////////////////////

