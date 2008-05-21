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
    
    template<typename OBJECTTYPE, typename HANDLE> class CResourceManager;
    ////////////////////
    /// Handle class. Handle is always null handle before it is Initialize()'d.
    /// \note 
    /// Handles must be Release()'d by CResourceManager before destroying them -
    /// Nullifying them is not enough - Otherwise BAD things are about to happen. 
    /// This is because CResourceManager keeps track of references (Handles) to resources 
    /// and does not know that handle does not exist anymore without Releasing.
    /// \note
    /// Handles can release themselves now during delete.
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
      ~CHandle();
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
      ////////////////////
      /// Comparison for handles by indices.
      /// \param handle Another handle.
      /// \returns boolean true if index is smaller index of given handle.
      bool operator<( const CHandle<TAG> & handle ) const
      {
	return ( GetIndex() < handle.GetIndex() );
      }
      
      TAG * operator*() const;
      TAG * operator->() const;
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
      /// \warning Remember to use ACTUAL handle that resides in final position in 
      /// \warning the memory.Otherwise, you WILL get nasty segfaults and handles 
      /// \warning start pointing where they wish - when you delete resources, that is.
      /// \param handle Reference to a handle.
      void ValidateHandle( HANDLE &handle)
      {
	if ( !handle.IsNull())
	  m_lstHandles.push_back( &handle );
      }
      ////////////////////
      /// Removes handle from list of handles and nullifies it.
      /// \warning Remember to use ACTUAL handle that resides in final position in 
      /// \warning the memory.Otherwise, you WILL get nasty segfaults and handles 
      /// \warning start pointing where they wish - when you delete resources, that is.
      /// \param handle Reference to a handle.
      void InvalidateHandle( HANDLE &handle )
      {
	if ( handle.IsNull()) return;

	typename std::list<HANDLE *>::iterator it = m_lstHandles.begin();
	for( ; it != m_lstHandles.end(); it++)
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
	  if ( *it == NULL ) continue;
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
      virtual ~CResourceManager();
    public:
      ////////////////////
      /// Initializes hash table with given size. If size is not given, it defaults to PHOENIX_MAGIC_NUMBER.
      /// \param nSize Number of hash table arrays.
      /// \returns non-zero on error, zero on success.
      int Initialize( size_t nSize = PHOENIX_MAGIC_NUMBER );
      ////////////////////
      /// Creates new resource to pointer, named strName - handle 
      /// is initialized refer to that object. 
      /// \warning Remember to use ACTUAL handle that resides in final position in the memory.
      /// \warning Otherwise, you WILL get nasty segfaults and handles start pointing
      /// \warning where they wish - when you delete resources, that is.
      /// \param pType Pointer to object.
      /// \param strName Name for this resource.
      /// \param Handle Reference to handle which will refer to this resource.
      /// \returns zero on success, non-zero on error.
      int Create( OBJECTTYPE *pType, const std::string &strName, HANDLE &Handle );
      ////////////////////
      /// Creates new resource to pointer, named strName - handle 
      /// is initialized refer to that object.
      /// \warning Remember to use ACTUAL handle that resides in final position in 
      /// \warning the memory.Otherwise, you WILL get nasty segfaults and handles 
      /// \warning start pointing where they wish - when you delete resources, that is.
      /// \param pType Pointer to object.
      /// \param szStrName Name for this resource.
      /// \param Handle Reference to handle which will refer to this resource.
      /// \returns zero on success, non-zero on error.
      int Create( OBJECTTYPE *pType, const char *szStrName, HANDLE &Handle );
      ////////////////////
      /// Creates new resource to pointer, named strName.
      /// \param pType Pointer to object.
      /// \param strName Name for this resource.
      /// \returns zero on success, non-zero on error.
      int Create( OBJECTTYPE *pType, const std::string &strName );
      ////////////////////
      /// Creates new resource to pointer, named strName.
      /// \param pType Pointer to object.
      /// \param szStrName Name for this resource.
      /// \returns zero on success, non-zero on error.
      int Create( OBJECTTYPE *pType, const char *szStrName );
      ////////////////////
      /// Attaches handle to resource.
      /// \warning Remember to use ACTUAL handle that resides in final position in the memory.
      /// \warning Otherwise, you WILL get nasty segfaults and handles start pointing
      /// \warning where they wish - when you delete resources, that is.
      /// \param strName Name of resource.
      /// \param handle Handle which will point to resource.
      /// \returns zero on success, non-zero on error.
      int AttachHandle(  const std::string &strName, HANDLE & handle  );
      ////////////////////
      /// Attaches handle to resource.
      /// \warning Remember to use ACTUAL handle that resides in final position in the memory.
      /// \warning Otherwise, you WILL get nasty segfaults and handles start pointing
      /// \warning where they wish - when you delete resources, that is.
      /// \param szName Name of resource.
      /// \param handle Handle which will point to resource.
      /// \returns zero on success, non-zero on error.
      int AttachHandle(  const char *szName, HANDLE & handle  );
      ////////////////////
      /// Duplicates handle.
      /// \warning Remember to use ACTUAL handle that resides in final position in the memory.
      /// \warning Otherwise, you WILL get nasty segfaults and handles start pointing
      /// \warning where they wish - when you delete resources, that is.
      /// \param hSrcHandle Handle that will be duplicated.
      /// \param handle Handle that will point to resource pointed by hSrcHandle.
      void DuplicateHandle(  const HANDLE &hSrcHandle, HANDLE & handle );
      ////////////////////
      /// Releases handle to this object.
      /// \warning Remember to use ACTUAL handle that resides in final position in the memory.
      /// \warning Otherwise, you WILL get nasty segfaults and handles start pointing
      /// \warning where they wish - when you delete resources, that is.      
      /// \param handle Handle to object.
      void Release( HANDLE &handle );
      ////////////////////
      /// Removes a resource from manager.
      /// \param strName Resource name.
      void Destroy( const std::string &strName ); 
      ////////////////////
      /// Removes a resource from manager.
      /// \param szStrName Resource name.
      void Destroy( const char *szStrName ); 
      ////////////////////
      /// Removes a resource from manager.
      /// \param handle Handle pointing to resource to be destroyed.
      void Destroy( HANDLE &handle ); 
      ////////////////////
      /// Returns pointer to resource.
      /// \param handle Handle to resource.
      /// \returns Pointer to object, if handle is valid. NULL otherwise.
      OBJECTTYPE *GetResource( const HANDLE &handle ) const;
      ////////////////////
      /// Returns pointer to resource.
      /// \param resName Registered resource name. 
      /// \returns Pointer to object, if handle is valid. NULL otherwise.
      OBJECTTYPE *GetResource( const std::string &resName ) const;
      ////////////////////
      /// Checks, if resource name is taken.
      /// \param resName Resource name. 
      /// \returns non-zero, if name is taken, zero otherwise.
      int HasResource( const std::string &resName ) const;
      ////////////////////
      /// Returns pointer to resource.
      /// \param szResName Registered resource name. 
      /// \returns Pointer to object, if handle is valid. NULL otherwise.
      OBJECTTYPE *GetResource( const char *szResName ) const;
      ////////////////////
      /// Gets number of currently managed objects.
      /// \returns Number of managed objects.
      unsigned int GetSize() const;
      ////////////////////
      /// Returns resource name where this handle refers to.
      /// \param handle Handle which resource name is retrieved.
      /// \returns String containing resource name. 
      /// \returns Empty string is returned if resource is not found.
      std::string GetResourceName( const HANDLE &handle ) const;
      ////////////////////
      /// Returns reference to Object vector.
      /// \returns Vector of managed objects.
      std::vector<CResource<OBJECTTYPE,HANDLE> *> & GetResources();
    private:
      void DeleteMemory();
      ////////////////////
      /// Removes a resource from manager.
      /// \param nIndex Index of resource array.
      void Destroy( unsigned int nIndex ); 
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
  return Create(pType, strName.c_str(), Handle );
}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
int
Phoenix::Core::CResourceManager<OBJECTTYPE,HANDLE>::Create( OBJECTTYPE *pType, 
							    const char *szStrName, 
							    HANDLE &Handle )
{
  // more comp
  std::string strName(szStrName);
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
int
Phoenix::Core::CResourceManager<OBJECTTYPE,HANDLE>::Create( OBJECTTYPE *pType, 
							    const std::string &strName)
{
  return Create(pType, strName.c_str() );
}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
int
Phoenix::Core::CResourceManager<OBJECTTYPE,HANDLE>::Create( OBJECTTYPE *pType, 
							    const char *szStrName)
{
  // more comp
  std::string strName(szStrName);
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
    return 0;
  } 
  
  return -1;
}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
int 
Phoenix::Core::CResourceManager<OBJECTTYPE,HANDLE>::AttachHandle(  const char *szName, HANDLE & handle  )
{
  return AttachHandle( std::string(szName), handle);
}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
int 
Phoenix::Core::CResourceManager<OBJECTTYPE,HANDLE>::AttachHandle(  const std::string &strName, HANDLE & handle  )
{
  // If resourcehash has not been created, we return error.
  if ( m_pResourceHash == NULL ) 
  { 
    handle.Nullify();
    return -1;
  }
  CHashItem<std::string,CResourceName> * hashItem = m_pResourceHash->Find( strName );

  // If such item is not found, we return error. 
  if ( hashItem == NULL )
  {
    handle.Nullify();
    return -1;
  }

  handle.Initialize(hashItem->GetObject().GetIndex());
  m_vecObjects[hashItem->GetObject().GetIndex()]->ValidateHandle( handle );
  return 0;
}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
void
Phoenix::Core::CResourceManager<OBJECTTYPE,HANDLE>::DuplicateHandle( const HANDLE &hSrcHandle, HANDLE & handle  )
{
  if ( hSrcHandle.IsNull() )
  {
    handle.Nullify();
  }
  else
  {
    // This requires also mutex, NOT thread-safe
    size_t nIndex = hSrcHandle.GetIndex();

    if ( nIndex >= m_vecObjects.size())
    {
      handle.Nullify();
    }
    else
    {
      handle.Initialize(nIndex);
      // Add handle to list of handles to be updated. 
      m_vecObjects[nIndex]->ValidateHandle( handle );  
    }
  }
  
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
inline OBJECTTYPE *
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
inline OBJECTTYPE *
Phoenix::Core::CResourceManager<OBJECTTYPE,HANDLE>::GetResource( const std::string &strName ) const
{
  if ( m_pResourceHash == NULL ) return NULL;

  CHashItem<std::string, CResourceName> *pHashItem = m_pResourceHash->Find(strName);
  if ( pHashItem == NULL ) return NULL;

  return m_vecObjects[pHashItem->GetObject().GetIndex()]->GetObject();
}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
inline int
Phoenix::Core::CResourceManager<OBJECTTYPE,HANDLE>::HasResource( const std::string &strName ) const
{
  if ( m_pResourceHash == NULL ) return 0;
  return (m_pResourceHash->Find(strName) != 0);
}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
inline OBJECTTYPE *
Phoenix::Core::CResourceManager<OBJECTTYPE,HANDLE>::GetResource( const char *szResName ) const
{
  std::string strName(szResName);
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
  if ( handle.IsNull() ) return;
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
  // Destroy by index.
  Destroy( nIndex );

}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
void
Phoenix::Core::CResourceManager<OBJECTTYPE,HANDLE>::Destroy( const char * szStrName )
{
  if ( m_pResourceHash == NULL ) return;
  std::string strName(szStrName);
  Destroy( strName );
}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
void
Phoenix::Core::CResourceManager<OBJECTTYPE,HANDLE>::Destroy( HANDLE & handle )
{
  // Null handles won't point to anything. 
  if ( handle.IsNull() ) return;
  // Destroy by index.
  Destroy( handle.GetIndex() );

}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
void
Phoenix::Core::CResourceManager<OBJECTTYPE,HANDLE>::Destroy( unsigned int nIndex )
{
  
  if ( nIndex < m_vecObjects.size() - 1 )
  {
    std::string strName = m_vecObjects[nIndex]->GetName();
    // Remove resource (invalidates also handles)
    delete m_vecObjects[nIndex];
    // Assign last resource into place of deleted resource.
    m_vecObjects[nIndex] = m_vecObjects.back();

    // Remove extra pointer.
    m_vecObjects.pop_back();    

    // Update handle indices.
    m_vecObjects[nIndex]->UpdateAllHandles(nIndex);

    // Remove key from hash table.
    m_pResourceHash->Delete( strName );
    
    // Update hashtable's resourcename.index for moved resource
    CHashItem<std::string, CResourceName> *pHashItem = m_pResourceHash->Find(m_vecObjects[nIndex]->GetName());
    pHashItem->GetObject().SetIndex( nIndex );

  }
  else if ( nIndex == m_vecObjects.size() - 1 )
  {
    std::string strName = m_vecObjects[nIndex]->GetName();
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
template<typename OBJECTTYPE,typename HANDLE>
std::vector<Phoenix::Core::CResource<OBJECTTYPE,HANDLE> *> & 
Phoenix::Core::CResourceManager<OBJECTTYPE,HANDLE>::GetResources() 
{
  return m_vecObjects;
}
/////////////////////////////////////////////////////////////////
template<typename TAG>
inline TAG *
Phoenix::Core::CHandle<TAG>::operator*() const
{
  return Phoenix::Core::CResourceManager<TAG, CHandle<TAG> >::GetInstance()->GetResource(*this);
}
/////////////////////////////////////////////////////////////////
template<typename TAG>
inline TAG *
Phoenix::Core::CHandle<TAG>::operator->() const
{
  assert ( !IsNull() && "Dereferencing NULL Handle!" );
  return Phoenix::Core::CResourceManager<TAG, CHandle<TAG> >::GetInstance()->GetResource(*this);
}
/////////////////////////////////////////////////////////////////
template<typename TAG>
inline
Phoenix::Core::CHandle<TAG>::~CHandle()
{
  // this has to be released, otherwise we'll go see the boogie man. :)
  Phoenix::Core::CResourceManager<TAG, CHandle<TAG> >::GetInstance()->Release(*this);
}
/////////////////////////////////////////////////////////////////
#endif