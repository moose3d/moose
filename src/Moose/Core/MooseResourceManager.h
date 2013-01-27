/******************************************************************************/
 * Moose3D is a game development framework.
 *
 * Copyright 2006-2013 Anssi Gröhn / entity at iki dot fi.
 *
 * This file is part of Moose3D.
 *
 * Moose3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Moose3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Moose3D.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

#ifndef __MooseResourceManager_h__
#define __MooseResourceManager_h__
/////////////////////////////////////////////////////////////////
#include <string>
#include <vector>
#include <list>
#include <sstream>
#include "MooseAPI.h"
#include "MooseNullable.h"
#include "MooseHashTable.h"
#include "MooseSingleton.h"
#include "MooseGlobals.h"
#include "MooseExceptions.h"
#include <cstring>
#include <cassert>
#include <map>
/////////////////////////////////////////////////////////////////
namespace Moose
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
    class MOOSE_API CHandle : public CNullable
    {
    private:
      size_t m_nIndex;
    public:
      ////////////////////
      /// Constructor.
      CHandle() : CNullable(), m_nIndex(0) {  }
      ////////////////////
      /// Attempts to retrieve handle to given resource.
      /// \param szResourceName Resource name where this handle should point. If resource under such name is not available, then the handle will be null.
      CHandle( const char *szResourceName );
      ////////////////////
      /// Releases handle.
      ~CHandle();
      ////////////////////
      /// Releases handle.
      void Release();
      ////////////////////
      /// Returns index.
      /// \returns current index pointed by handle.
      inline size_t GetIndex() const
      {
      	return m_nIndex;
      }
      ////////////////////
      /// Initializes handle.
      /// \param nIndex Index where handle points.
      inline void Initialize( size_t nIndex )
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
      bool Equals( const CHandle<TAG> & handle ) const
      {
          return (handle.GetIndex() == GetIndex());
      }
#ifndef SWIG
      ////////////////////
      /// Comparison for handles by indices.
      /// \param handle Another handle.
      /// \returns boolean true if same, false otherwise.
      bool operator==( const CHandle<TAG> & handle ) const
      {
          return Equals(handle);
      }
      ////////////////////
      /// Comparison for handles by indices.
      /// \param handle Another handle.
      /// \returns boolean true if index is smaller index of given handle.
      bool operator<( const CHandle<TAG> & handle ) const
      {
	return ( GetIndex() < handle.GetIndex() );
      }
#endif
      TAG * operator*() const;
      TAG * operator->() const;
      ////////////////////
      /// Assignment operator, duplicates handles in manager.
      void operator=( const CHandle<TAG> & handle );
      ////////////////////
      /// Assignment operator via resource name directly.
      /// \param szResourceName Name of resource which will be attached to this handle.
      void operator=( const char *szResourceName );
    };
    ////////////////////
    /// Class for objects that know their position in ResourceManager.
    template< class OBJECT_TYPE >
    class MOOSE_API CHandled
    {
    protected:
      /// Handle to object itself.
      Moose::Core::CHandle< OBJECT_TYPE > m_hThis;
      virtual ~CHandled() { }
    public:
      ////////////////////
      /// Returns reference to object handle.
      /// \returns Reference to object handle.
      const Moose::Core::CHandle< OBJECT_TYPE > & GetObjectHandle() const
      {
	return m_hThis;
      }
      ////////////////////
      /// Returns reference to object handle.
      /// \returns Reference to object handle.
      Moose::Core::CHandle< OBJECT_TYPE > & GetObjectHandle()
      {
	return m_hThis;
      }
    };
    ////////////////////
    /// Resource Name class. This is used in Hash table.
    class MOOSE_API CResourceName : public CNullable
    {
    protected:
      /// Resource name.
      std::string m_strName;
      /// Resource index in manager array.
      size_t m_nIndex;
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
      /// Sets resource name.
      /// \param szName Name.
      void SetName(const char *szName )
      {
        m_strName = std::string(szName);
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
      void SetIndex( size_t nIndex )
      {
	m_nIndex = nIndex;
      }
      ////////////////////
      /// Returns index.
      /// \returns Resource index.
      size_t GetIndex() const
      {
	return m_nIndex;
      }
    };
    ////////////////////
    /// Resource class.
    template<typename OBJECTTYPE, typename HANDLE>
    class MOOSE_API CResource
    {
    protected:
      /// Handles which refer to this resource.
      std::list<HANDLE *> m_lstHandles;
      /// Actual resource object pointer.
      OBJECTTYPE *	  m_pObject;
      /// Name for this resource. Must be same as in CResourceName.m_strName.
      std::string	  m_strResourceName;
      /// Which slot this resource is in.
      size_t		  m_nIndex;
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
      void UpdateAllHandles( size_t nIndex )
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
    typedef std::map< std::string, std::string> ResourceMap;
    ////////////////////
    /// Resource Manager class.
    template<typename OBJECTTYPE, typename HANDLE>
    class MOOSE_API CResourceManager : public CSingleton<CResourceManager<OBJECTTYPE,HANDLE> >
    {
      friend class CSingleton<CResourceManager<OBJECTTYPE,HANDLE> >;
    protected:
      CHashTable<std::string,CResourceName> *m_pResourceHash;
      std::vector<CResource<OBJECTTYPE,HANDLE> *> m_vecObjects;
      ResourceMap                                 m_mapResources;
      ////////////////////
      /// Constructor.
      CResourceManager();
      ////////////////////
      /// Destructor.
      virtual ~CResourceManager();
    public:
      ////////////////////
      /// Initializes hash table with given size. If size is not given, it defaults to MOOSE_MAGIC_NUMBER.
      /// \param nSize Number of hash table arrays.
      /// \returns non-zero on error, zero on success.
      int Initialize( size_t nSize = Moose::Globals::MOOSE_MAGIC_NUMBER );
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
      /// Renames a resource. If newName exists already in manager, or szResName does not exist, an exception is thrown.
      /// \param resName Old resource name. 
      /// \param newName New name for resource. Such object must not exist.
      void Rename( const std::string & resName, const std::string & newName ) throw (Moose::Exceptions::CRenameException);
      ////////////////////
      /// Renames a resource. If newName exists already in manager, or szResName does not exist, an exception is thrown. 
      /// \param szResName Old resource name. 
      /// \param szNewName New name for resource. Such object must not exist.
      void Rename( const char *szResName, const char *szNewName ) throw (Moose::Exceptions::CRenameException);
      ////////////////////
      /// Renames a resource. If handle is null, or szNewName already exists, an exception is thrown. 
      /// \param handle Handle to resource to be renamed.
      /// \param szNewName New name for resource. Such object must not exist.
      void Rename( const HANDLE &handle, const std::string & strNewName ) throw (Moose::Exceptions::CRenameException);
#if !defined(SWIG)
      ////////////////////
      /// GetResource in more compact form.
      /// \param szResName Registered resource name.
      /// \returns Pointer to object, if handle is valid. NULL otherwise.
      OBJECTTYPE *operator()( const char *szResName) const;  
#endif
      ////////////////////
      /// Gets number of currently managed objects.
      /// \returns Number of managed objects.
      size_t GetSize() const;
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
      ////////////////////
      /// Releases object from manager without destroying it.
      /// \param handle Handle to managed object.
      void ReleaseObject( HANDLE & handle );
      ////////////////////
      /// Apply functor for each object in the manager.
      /// \param f Functor with operator() overwritten appropriately.
      template<typename Functor> void ForEachObject( Functor f );
      ////////////////////
      /// Apply functor for each object pointer in the manager.
      /// \param f Functor with operator() overwritten appropriately.
      template<typename Functor> void ForEachObjectPtr( Functor f );
      ////////////////////
      /// Registers a specific resource under a name.
      /// \param strPath path to resource file.
      /// \param name Name of stored resource.
      void RegisterPath( const std::string & strPath, const std::string & name );
      ////////////////////
      /// Checks whether a path is registered.
      /// \param strPath path to resource file.
      /// \return true if registered, false otherwise.
      bool IsPathRegistered( const std::string & strPath);
    private:
      void DeleteMemory();
      ////////////////////
      /// Removes a resource from manager.
      /// \param nIndex Index of resource array where resource is.
      void Destroy( size_t nIndex );
      ////////////////////
      /// Releases object from manager without destroying it.
      /// \param nIndex Index of resource array where object is.
      void ReleaseObject( size_t nIndex );
    };
  };
};
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
Moose::Core::CResourceManager<OBJECTTYPE,HANDLE>::CResourceManager() : m_pResourceHash(NULL)
{
  // NOP
}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
Moose::Core::CResourceManager<OBJECTTYPE,HANDLE>::~CResourceManager()
{
  DeleteMemory();
}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
int
Moose::Core::CResourceManager<OBJECTTYPE,HANDLE>::Initialize(size_t nSize)
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
Moose::Core::CResourceManager<OBJECTTYPE,HANDLE>::Create( OBJECTTYPE *pType,
							    const std::string &strName,
							    HANDLE &Handle )
{
  return Create(pType, strName.c_str(), Handle );
}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
int
Moose::Core::CResourceManager<OBJECTTYPE,HANDLE>::Create( OBJECTTYPE *pType,
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
    size_t nIndex = m_vecObjects.size() - 1;

    // Create object with name and index mapping
    CResourceName resourceName;
    resourceName.SetName( strName );
    resourceName.SetIndex( nIndex );

    // Put string-> resourcename into hashtable.
    CHashItem<std::string,CResourceName> hashItem;
    hashItem.SetKey(strName);
    hashItem.SetObject(resourceName);

    m_pResourceHash->Insert( hashItem );
    // release previously pointed resource (if such exists)
    Release(Handle);
    Handle.Initialize( nIndex );
    // Add handle to list of handles to be updated.
    m_vecObjects[nIndex]->ValidateHandle( Handle );
    return 0;
  }
  else
  {
    Handle.Nullify();
    Moose::Exceptions::CMooseRuntimeError err("");
    err << "Resource '" << szStrName << "' already exists!";
    throw err;
  }
  return -1;
}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
int
Moose::Core::CResourceManager<OBJECTTYPE,HANDLE>::Create( OBJECTTYPE *pType,
							    const std::string &strName)
{
  return Create(pType, strName.c_str() );
}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
int
Moose::Core::CResourceManager<OBJECTTYPE,HANDLE>::Create( OBJECTTYPE *pType,
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
    size_t nIndex = m_vecObjects.size() - 1;

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
  else 
  {
      Moose::Exceptions::CMooseRuntimeError err("");
      err << "Resource '" << szStrName << "' already exists!";
    throw err;
  }

  return -1;
}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
int
Moose::Core::CResourceManager<OBJECTTYPE,HANDLE>::AttachHandle(  const char *szName, HANDLE & handle  )
{
  return AttachHandle( std::string(szName), handle);
}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
int
Moose::Core::CResourceManager<OBJECTTYPE,HANDLE>::AttachHandle(  const std::string &strName, HANDLE & handle  )
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
  // Release previously pointed resource (if such exists).
  // This way duplicate AttachHandles() won't case trouble via handle pointers in
  // resource manager.
  Release(handle);

  handle.Initialize(hashItem->GetObject().GetIndex());
  m_vecObjects[hashItem->GetObject().GetIndex()]->ValidateHandle( handle );
  return 0;
}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
void
Moose::Core::CResourceManager<OBJECTTYPE,HANDLE>::DuplicateHandle( const HANDLE &hSrcHandle, HANDLE & handle  )
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
      // Release previously pointed resource.
      Release(handle);
      handle.Initialize(nIndex);
      // Add handle to list of handles to be updated.
      m_vecObjects[nIndex]->ValidateHandle( handle );
    }
  }

}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
void
Moose::Core::CResourceManager<OBJECTTYPE,HANDLE>::DeleteMemory()
{
  // Deletes resource pointers and objects inside them
  for( size_t nObj = 0;nObj<m_vecObjects.size();nObj++)
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
////////////////////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
inline void
Moose::Core::CResourceManager<OBJECTTYPE,HANDLE>::Rename( const std::string & resName, const std::string & newName ) throw (Moose::Exceptions::CRenameException)
{
  this->Rename( resName.c_str(), newName.c_str());
}
////////////////////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
inline void
Moose::Core::CResourceManager<OBJECTTYPE,HANDLE>::Rename( const char *szResName, const char * szNewName ) throw (Moose::Exceptions::CRenameException)
{
  // No operation on same name
  if ( strcmp(szResName, szNewName) == 0 ) return;

  if ( !HasResource(szResName) ) 
  {
    std::ostringstream s;
    s << "Cannot rename '" << szResName << "', no such object in manager.";
    throw Moose::Exceptions::CRenameException(s.str().c_str());
  }

  if ( HasResource(szNewName) ) 
  {
    std::ostringstream s;
    s << "Cannot rename '" << szResName << "' to '" << szNewName << "', new name points to already existing object in manager.";
    throw Moose::Exceptions::CRenameException(s.str().c_str());
  }

  // Get existing hash item for our managed object
  CHashItem<std::string, CResourceName> *pHashItem = m_pResourceHash->Find(szResName);
  size_t nIndex = pHashItem->GetObject().GetIndex();
  
  // Create mapping name->index
  CResourceName resourceName;
  resourceName.SetName( szNewName );
  resourceName.SetIndex( nIndex );
  
  // Create hash item for new name
  CHashItem<std::string, CResourceName> newHashItem;
  newHashItem.SetKey( szNewName);
  newHashItem.SetObject( resourceName );
  
  // Change name in existing resource (associated with object)
  m_vecObjects[nIndex]->SetName(szNewName);
  // Remove old hash information
  m_pResourceHash->Delete(szResName);
  // Add new name
  m_pResourceHash->Insert(newHashItem);
}
////////////////////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
inline void
Moose::Core::CResourceManager<OBJECTTYPE,HANDLE>::Rename( const HANDLE &handle, const std::string & strNewName ) throw (Moose::Exceptions::CRenameException)
{
  if ( handle.IsNull() ) 
  {
    throw Moose::Exceptions::CRenameException("Cannot rename, handle is null");
  }
  this->Rename(m_vecObjects[handle.GetIndex()]->GetName(), strNewName);
}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
inline OBJECTTYPE *
Moose::Core::CResourceManager<OBJECTTYPE,HANDLE>::operator()( const char *szResName ) const
{
    return this->GetResource(szResName);
}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
inline OBJECTTYPE *
Moose::Core::CResourceManager<OBJECTTYPE,HANDLE>::GetResource( const HANDLE &handle) const
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
Moose::Core::CResourceManager<OBJECTTYPE,HANDLE>::GetResource( const std::string &strName ) const
{
  if ( m_pResourceHash == NULL ) return NULL;

  CHashItem<std::string, CResourceName> *pHashItem = m_pResourceHash->Find(strName);
  if ( pHashItem == NULL ) return NULL;

  return m_vecObjects[pHashItem->GetObject().GetIndex()]->GetObject();
}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
inline int
Moose::Core::CResourceManager<OBJECTTYPE,HANDLE>::HasResource( const std::string &strName ) const
{
  if ( m_pResourceHash == NULL ) return 0;
  return (m_pResourceHash->Find(strName) != 0);
}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
inline OBJECTTYPE *
Moose::Core::CResourceManager<OBJECTTYPE,HANDLE>::GetResource( const char *szResName ) const
{
  std::string strName(szResName);
  if ( m_pResourceHash == NULL ) return NULL;

  CHashItem<std::string, CResourceName> *pHashItem = m_pResourceHash->Find(strName);
  if ( pHashItem == NULL ) return NULL;

  return m_vecObjects[pHashItem->GetObject().GetIndex()]->GetObject();
}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
inline size_t
Moose::Core::CResourceManager<OBJECTTYPE,HANDLE>::GetSize() const
{
  return m_vecObjects.size();
}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
inline void
Moose::Core::CResourceManager<OBJECTTYPE,HANDLE>::Release( HANDLE &handle )
{
  if ( handle.IsNull() ) return;
  m_vecObjects[handle.GetIndex()]->InvalidateHandle( handle );
  handle.Nullify();
}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
void
Moose::Core::CResourceManager<OBJECTTYPE,HANDLE>::Destroy( const std::string &strName )
{
  if ( m_pResourceHash == NULL ) return;
  CHashItem<std::string, CResourceName> *pHashItem = m_pResourceHash->Find(strName);

  if ( pHashItem == NULL ) return;
  size_t nIndex = pHashItem->GetObject().GetIndex();
  // Destroy by index.
  Destroy( nIndex );

}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
void
Moose::Core::CResourceManager<OBJECTTYPE,HANDLE>::Destroy( const char * szStrName )
{
  if ( m_pResourceHash == NULL ) return;
  std::string strName(szStrName);
  Destroy( strName );
}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
void
Moose::Core::CResourceManager<OBJECTTYPE,HANDLE>::Destroy( HANDLE & handle )
{
  // Null handles won't point to anything.
  if ( handle.IsNull() ) return;
  // Destroy by index.
  Destroy( handle.GetIndex() );

}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
void
Moose::Core::CResourceManager<OBJECTTYPE,HANDLE>::ReleaseObject( HANDLE & handle )
{
  // Null handles won't point to anything.
  if ( handle.IsNull() ) return;
  // Destroy by index.
  ReleaseObject( handle.GetIndex() );
}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
void
Moose::Core::CResourceManager<OBJECTTYPE,HANDLE>::Destroy( size_t nIndex )
{


  if ( nIndex < m_vecObjects.size() - 1 )
  {

    std::string strName = m_vecObjects[nIndex]->GetName();
    CHashItem<std::string, CResourceName>  *pTmp = m_pResourceHash->Find(strName);
    // Remove resource (invalidates also handles)
    delete m_vecObjects[nIndex];
    ///////////////////////
    // index might have become invalid at this stage, so we must retrieve it again.
    // This is because you could call CResourceManager::Destroy() inside a Destructor, which would shuffle indices
    // in m_vecObjects, and even move Resource pointer (while being deleted) into another location in the array.
    nIndex = pTmp->GetObject().GetIndex();

    if ( nIndex < m_vecObjects.size() - 1) // not the last object in array
    {
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
    else
    {
    	// We're last element in the array, so remove us without any hesitation.
    	m_vecObjects.pop_back();
    	// Remove key from hash table.
    	m_pResourceHash->Delete( strName );
    }

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
template<typename OBJECTTYPE, typename HANDLE>
void
Moose::Core::CResourceManager<OBJECTTYPE,HANDLE>::ReleaseObject( size_t nIndex )
{

  if ( nIndex < m_vecObjects.size() - 1 )
  {
    std::string strName = m_vecObjects[nIndex]->GetName();
    // Here we loose our track of the object.
    m_vecObjects[nIndex]->SetObject(NULL);
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
    // Here we loose our track of the object.
    m_vecObjects[nIndex]->SetObject(NULL);
    delete m_vecObjects[nIndex];
    m_vecObjects.pop_back();
    // Remove key from hash table.
    m_pResourceHash->Delete( strName );
  }

}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE,typename HANDLE>
std::string
Moose::Core::CResourceManager<OBJECTTYPE,HANDLE>::GetResourceName( const HANDLE &handle ) const
{
  if ( handle.IsNull() || handle.GetIndex() >= GetSize())
  {
    return std::string();
  }
  return m_vecObjects[handle.GetIndex()]->GetName();
}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE,typename HANDLE>
std::vector<Moose::Core::CResource<OBJECTTYPE,HANDLE> *> &
Moose::Core::CResourceManager<OBJECTTYPE,HANDLE>::GetResources()
{
  return m_vecObjects;
}
/////////////////////////////////////////////////////////////////
template<typename TAG>
inline TAG *
Moose::Core::CHandle<TAG>::operator*() const
{
  return Moose::Core::CResourceManager<TAG, CHandle<TAG> >::GetInstance()->GetResource(*this);
}
/////////////////////////////////////////////////////////////////
template<typename TAG>
inline TAG *
Moose::Core::CHandle<TAG>::operator->() const
{
  assert ( !IsNull() && "Dereferencing NULL Handle!" );
  return Moose::Core::CResourceManager<TAG, CHandle<TAG> >::GetInstance()->GetResource(*this);
}
/////////////////////////////////////////////////////////////////
template<typename TAG>
inline
Moose::Core::CHandle<TAG>::~CHandle()
{
  Release();
}
////////////////////////////////////////////////////////////////////////////////
template<typename TAG>
inline void
Moose::Core::CHandle<TAG>::Release()
{
  // this has to be released, otherwise we'll go see the boogie man. :)
  Moose::Core::CResourceManager<TAG, CHandle<TAG> >::GetInstance()->Release(*this);
}
////////////////////////////////////////////////////////////////////////////////
template<typename TAG>
inline
Moose::Core::CHandle<TAG>::CHandle(const char *szResourceName )
{
  Moose::Core::CResourceManager<TAG, CHandle<TAG> >::GetInstance()->AttachHandle( szResourceName, *this );
}
/////////////////////////////////////////////////////////////////
template<typename TAG>
inline void
Moose::Core::CHandle<TAG>::operator=( const Moose::Core::CHandle<TAG> & handle )
{
  Moose::Core::CResourceManager<TAG, CHandle<TAG> >::GetInstance()->Release( *this );
  Moose::Core::CResourceManager<TAG, CHandle<TAG> >::GetInstance()->DuplicateHandle( handle, *this );
}
/////////////////////////////////////////////////////////////////
template<typename TAG>
inline void
Moose::Core::CHandle<TAG>::operator=( const char *szResourceName )
{
  Moose::Core::CResourceManager<TAG, CHandle<TAG> >::GetInstance()->Release( *this );
  Moose::Core::CResourceManager<TAG, CHandle<TAG> >::GetInstance()->AttachHandle( szResourceName, *this );
}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE,typename HANDLE>
template<typename Functor>
inline void
Moose::Core::CResourceManager<OBJECTTYPE,HANDLE>::ForEachObjectPtr( Functor f )
{
	typename std::vector<CResource<OBJECTTYPE,HANDLE> *>::iterator it = m_vecObjects.begin();
	for( ; it != m_vecObjects.end(); it++)
	{
		f( (*it)->GetObject() );
	}
}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE,typename HANDLE>
template<typename Functor>
inline void
Moose::Core::CResourceManager<OBJECTTYPE,HANDLE>::ForEachObject( Functor f )
{
	typename std::vector<CResource<OBJECTTYPE,HANDLE> *>::iterator it = m_vecObjects.begin();
	for( ; it != m_vecObjects.end(); it++)
	{
		f( *(*it)->GetObject() );
	}
}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
void 
Moose::Core::CResourceManager<OBJECTTYPE,HANDLE>::RegisterPath( const std::string & strPath, 
                                                                const std::string & name )
{
  if ( m_mapResources.find(strPath) == m_mapResources.end() )
  {
    m_mapResources[strPath] = name;
  }
}
////////////////////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
bool 
Moose::Core::CResourceManager<OBJECTTYPE,HANDLE>::IsPathRegistered( const std::string & strPath )
{
  return ( m_mapResources.find(strPath) != m_mapResources.end());
}
////////////////////////////////////////////////////////////////////////////////
#endif
