#ifndef __PhoenixResourceManager_h__
#define __PhoenixResourceManager_h__
/////////////////////////////////////////////////////////////////
#include <string>
#include "PhoenixCore.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Core
  {
    using namespace Phoenix::Core;
    ////////////////////
    /// Handle class. Handle is always null handle before it is Initialize()'d.
    template<typename TAG>
    class CHandle : public CNullable
    {
    private:
      unsigned int m_nIndex;
    public:
      ////////////////////
      /// Constructor.
      CHandle() : CNullable(), m_nIndex(0) {  }
      ////////////////////
      /// Destructor.
      ~CHandle() {}
      //////////////////// 
      /// Returns index.
      /// \returns current index pointed by handle.
      inline unsigned int GetIndex() const
      {
	return m_nIndex;
      }
      ////////////////////
      /// Initializes handle.
      /// \param nIndex Index where handle points.
      inline void Initialize( unsigned int nIndex )
      {
	m_nIndex = nIndex;
	SetNull(0);
      }
      ////////////////////
      /// Nullifies handle. Handle won't point anywhere.
      inline void Nullify()
      {
	m_nIndex = 0;
	SetNull(1);
      }
      ////////////////////
      /// Comparison for handles by indices.
      /// \param handle Another handle.
      /// \returns boolean true if same, false otherwise.
      bool operator==( const CHandle<TAG> & handle ) const
      {
	return (handle.GetIndex() == GetIndex());
      }
    };
    ////////////////////
    /// Resource Name class. This is used in Hash table.
    class CResourceName : public CNullable
    {
    protected:      
      /// Resource name.
      std::string m_strName;
      /// Resource index in manager array.
      unsigned int m_nIndex;
    public:
      ////////////////////
      /// Constructor.
      CResourceName() 
      {
	m_nIndex = 0;
      }
      ////////////////////
      /// Sets resource name.
      /// \param strName Name.
      void SetName(const std::string & strName )
      {
	m_strName = strName;
      }
      ////////////////////
      /// Gets resource name
      /// \returns Resource name.
      const std::string & GetName() const
      {
	return m_strName;
      }
      ////////////////////
      /// Sets index.
      /// \param Resource index.
      void SetIndex( unsigned int nIndex )
      {
	m_nIndex = nIndex;
      }
      ////////////////////
      /// Returns index.
      /// \returns Resource index.
      unsigned int GetIndex() const
      {
	return m_nIndex;
      }
    };
    ////////////////////
    /// Resource class.
    template<typename OBJECTTYPE, typename HANDLE>
    class CResource
    {
    protected:
      /// Handles which refer to this resource.
      std::list<HANDLE *> m_lstHandles;
      /// Actual resource object pointer.
      OBJECTTYPE *	  m_pObject;
      /// Name for this resource. Must be same as in CResourceName.m_strName.
      std::string	  m_strResourceName;
    public:
      ////////////////////
      /// Constructor.
      CResource() : m_pObject(NULL) {}
      ////////////////////
      /// Destructor.
      ~CResource() 
      {
	InvalidateAllHandles();
	if ( m_pObject != NULL ) 
	{
	  delete m_pObject;
	}
	m_pObject = NULL;
      }
      ////////////////////
      /// Assigns object.
      /// \param pObject Pointer to object.
      void SetObject( OBJECTTYPE *pObject )
      {
	m_pObject = pObject;
      }
      ////////////////////
      /// Retrieves object of this resource.
      /// \returns Pointer to object of resource.
      OBJECTTYPE *GetObject()
      {
	return m_pObject;
      }
      ////////////////////
      /// Returns resource name as string.
      /// \returns Resource name string.
      const std::string & GetName() const
      {
	return m_strResourceName;
      }
      ////////////////////
      /// Assigns resource name.
      /// \param strName Resource name string.
      void SetName(const std::string & strName)
      {
	m_strResourceName = strName;
      }
      ////////////////////
      /// Adds handle to this resource.
      /// \param handle Reference to a handle.
      void ValidateHandle( HANDLE &handle)
      {
	if ( !handle.IsNull())
	  m_lstHandles.push_back( &handle );
      }
      ////////////////////
      /// Removes handle from list of handles and nullifies it.
      void InvalidateHandle( HANDLE &handle )
      {
	if ( handle.IsNull()) return;

	typename std::list<HANDLE *>::iterator it = m_lstHandles.begin();
	for( ; it < m_lstHandles.end(); it++)
	{
	  // If handle address matches, nullify and delete.
	  if ( (*it) == &handle )
	  {
	    (*it)->Nullify();
	    m_lstHandles.erase(it);
	    break;
	  }
	}	
      }
      ////////////////////
      /// Nullifies all handles which point to this resource.
      void InvalidateAllHandles()
      {
	typename std::list<HANDLE *>::iterator it = m_lstHandles.begin();
	for( ; it != m_lstHandles.end(); it++)
	{
	  (*it)->Nullify();
	}
	m_lstHandles.clear();
      }
      ////////////////////
      /// Updates all handles to this resource. Before nullifiy
      /// \param nIndex New index for this resource in ResourceManager's array.
      void UpdateAllHandles( unsigned int nIndex )
      {
	typename std::list<HANDLE *>::iterator it  = m_lstHandles.begin();
	typename std::list<HANDLE *>::iterator end = m_lstHandles.end();
	/// Update valid handles and remove invalid from list.
	while( it != end )
	{
	  if( (*it)->IsNull() )
	  {
	    m_lstHandles.erase( it++ );
	  }
	  else
	  {
	    (*it)->Initialize(nIndex);
	    ++it;
	  }// else
	} // while( 
      }
    };
    ////////////////////
    /// Resource Manager class.
    template<typename OBJECTTYPE, typename HANDLE>
    class CResourceManager : public CSingleton<CResourceManager<OBJECTTYPE,HANDLE> >
    {
      friend class CSingleton<CResourceManager<OBJECTTYPE,HANDLE> >;
    protected:
      CHashTable<std::string,CResourceName> *m_pResourceHash;
      std::vector<CResource<OBJECTTYPE,HANDLE> *> m_vecObjects;
      ////////////////////
      /// Constructor.
      CResourceManager();
      ////////////////////
      /// Destructor.
      ~CResourceManager();
    public:
      ////////////////////
      /// Initializes hash table with given size. If size is not given, it defaults to PHOENIX_MAGIC_NUMBER.
      /// \param nSize Number of hash table arrays.
      /// \returns non-zero on error, zero on success.
      int Initialize( size_t nSize = PHOENIX_MAGIC_NUMBER );
      ////////////////////
      /// Creates new resource to pointer, named strName - handle 
      /// is initialized refer to that object.
      /// \param pType Pointer to object.
      /// \param strName Name for this resource.
      /// \param Handle Reference to handle which will refer to this resource.
      /// \returns zero on success, non-zero on error.
      int Create( OBJECTTYPE *pType, const std::string &strName, HANDLE &Handle );
      ////////////////////
      /// Releases handle to this object.
      /// \param handle Handle to object.
      void Release( HANDLE &handle );
      ////////////////////
      /// Removes a resource from manager.
      /// \param strName Resource name.
      void Destroy( const std::string &strName ); 
      ////////////////////
      /// Returns pointer to resource.
      /// \param handle Handle to resource.
      /// \returns Pointer to object, if handle is valid. NULL otherwise.
      OBJECTTYPE *GetResource( const HANDLE &handle ) const;
      ////////////////////
      /// Returns pointer to resource.
      /// \param name Registered resource name. 
      /// \returns Pointer to object, if handle is valid. NULL otherwise.
      OBJECTTYPE *GetResource( const std::string &resName ) const;
      ////////////////////
      /// Gets number of currently managed objects.
      /// \returns Number of managed objects.
      unsigned int GetSize() const;
      ////////////////////
      /// Returns resource name where this handle refers to.
      /// \param handle Handle which resource name is retrieved.
      /// \returns String containing resource name. Empty string is returned if resource is not found.
      std::string GetResourceName( const HANDLE &handle ) const;
      
    private:
      void DeleteMemory();
    };
  };
};
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
Phoenix::Core::CResourceManager<OBJECTTYPE,HANDLE>::CResourceManager() : m_pResourceHash(NULL)
{
  // NOP
}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
Phoenix::Core::CResourceManager<OBJECTTYPE,HANDLE>::~CResourceManager() 
{
  DeleteMemory();
}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
int
Phoenix::Core::CResourceManager<OBJECTTYPE,HANDLE>::Initialize(size_t nSize)
{
  m_pResourceHash = new CHashTable<std::string,CResourceName>(nSize);
  
  if ( m_pResourceHash == NULL )
  {
    return -1;
  }
  return 0;
}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
int
Phoenix::Core::CResourceManager<OBJECTTYPE,HANDLE>::Create( OBJECTTYPE *pType, 
							    const std::string &strName, 
							    HANDLE &Handle )
{
  if ( m_pResourceHash == NULL )
  {
    Initialize();
  }
  if ( m_pResourceHash->Find(strName)==NULL )
  {
    // Create resource object
    CResource<OBJECTTYPE,HANDLE> *pResource = new CResource<OBJECTTYPE,HANDLE>();
    pResource->SetObject(pType);
    pResource->SetName( strName );

    // Insert object into manager and get its index
    m_vecObjects.push_back( pResource );
    unsigned int nIndex = m_vecObjects.size() - 1;

    // Create object with name and index mapping
    CResourceName resourceName;
    resourceName.SetName( strName );
    resourceName.SetIndex( nIndex );

    // Put string-> resourcename into hashtable.
    CHashItem<std::string,CResourceName> hashItem;
    hashItem.SetKey(strName);
    hashItem.SetObject(resourceName);
    
    m_pResourceHash->Insert( hashItem );
    Handle.Initialize( nIndex );
    // Add handle to list of handles to be updated. 
    m_vecObjects[nIndex]->ValidateHandle( Handle );
    return 0;
  } 
  else
  {
    Handle.Nullify();
  }
  return -1;
}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
void
Phoenix::Core::CResourceManager<OBJECTTYPE,HANDLE>::DeleteMemory()
{
  // Deletes resource pointers and objects inside them
  for( unsigned int nObj = 0;nObj<m_vecObjects.size();nObj++)
  {
    if ( m_vecObjects[nObj] != NULL )
    {
      delete m_vecObjects[nObj];
      m_vecObjects[nObj] = NULL;
    }
  }
  m_vecObjects.clear();
  if ( m_pResourceHash != NULL )  
  {
    delete m_pResourceHash;
  }
  m_pResourceHash = NULL;
}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
OBJECTTYPE *
Phoenix::Core::CResourceManager<OBJECTTYPE,HANDLE>::GetResource( const HANDLE &handle) const
{
  if ( handle.GetIndex() >= GetSize() || handle.IsNull() )
  {
    return NULL;
  }
  return m_vecObjects[handle.GetIndex()]->GetObject();
}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
OBJECTTYPE *
Phoenix::Core::CResourceManager<OBJECTTYPE,HANDLE>::GetResource( const std::string &strName ) const
{
  if ( m_pResourceHash == NULL ) return NULL;

  CHashItem<std::string, CResourceName> *pHashItem = m_pResourceHash->Find(strName);
  if ( pHashItem == NULL ) return NULL;

  return m_vecObjects[pHashItem->GetObject().GetIndex()]->GetObject();
}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
inline unsigned int
Phoenix::Core::CResourceManager<OBJECTTYPE,HANDLE>::GetSize() const
{
  return m_vecObjects.size();
}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
inline void
Phoenix::Core::CResourceManager<OBJECTTYPE,HANDLE>::Release( HANDLE &handle )
{
  m_vecObjects[handle.GetIndex()]->InvalidateHandle( handle );
  handle.Nullify();
}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
void
Phoenix::Core::CResourceManager<OBJECTTYPE,HANDLE>::Destroy( const std::string &strName )
{
  if ( m_pResourceHash == NULL ) return;
  CHashItem<std::string, CResourceName> *pHashItem = m_pResourceHash->Find(strName);
  
  if ( pHashItem == NULL ) return;
  unsigned int nIndex = pHashItem->GetObject().GetIndex();
  
  if ( nIndex < m_vecObjects.size() - 1 )
  {
    // Remove resource (invalidates also handles)
    delete m_vecObjects[nIndex];    
    // Assign last resource into place of deleted resource.
    m_vecObjects[nIndex] = m_vecObjects.back();
    // Update handle indices.
    m_vecObjects[nIndex]->UpdateAllHandles(nIndex);
    // Remove extra pointer.
    m_vecObjects.pop_back();
    // Remove key from hash table.
    m_pResourceHash->Delete( strName );
    
    // Update hashtable's resourcename.index for moved resource
    pHashItem = m_pResourceHash->Find(m_vecObjects[nIndex]->GetName());
    pHashItem->GetObject().SetIndex( nIndex );
  }
  else
  {
    delete m_vecObjects[nIndex];
    m_vecObjects.pop_back();    
    // Remove key from hash table.
    m_pResourceHash->Delete( strName );
  }

}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE,typename HANDLE>
std::string
Phoenix::Core::CResourceManager<OBJECTTYPE,HANDLE>::GetResourceName( const HANDLE &handle ) const
{
  if ( handle.IsNull() || handle.GetIndex() >= GetSize())
  {
    return string();
  }
  return m_vecObjects[handle.GetIndex()]->GetName();
}
/////////////////////////////////////////////////////////////////
#endif
