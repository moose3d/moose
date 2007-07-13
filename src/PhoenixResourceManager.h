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
    ////////////////////
    /// Handle class.
    template<typename TAG>
    class CHandle : public CNullable
    {
    private:
      unsigned int m_nIndex;
    public:
      CHandle() : CNullable(), m_nIndex(0) {  }
      ~CHandle() {}
      inline unsigned int GetIndex() const
      {
	return m_nIndex;
      }
      inline void Initialize( unsigned int nIndex )
      {
	m_nIndex = nIndex;
	SetNull(0);
      }
      inline void Nullify()
      {
	m_nIndex = 0;
	SetNull(1);
      }
      bool operator==( const CHandle<TAG> & handle ) const
      {
	return (handle.GetIndex() == GetIndex());
      }
    };
    ////////////////////
    /// Resource Name class.
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
    /// Resource Manager class.
    template<typename OBJECTTYPE, typename HANDLE>
    class CResourceManager
    {
    protected:
      CHashTable<std::string,ResourceName> *m_pResourceHash;
      std::vector<OBJECTTYPE *> m_vecObjects;
    public:
      ////////////////////
      /// Constructor.
      CResourceManager( unsigned int nSize ) 
      {
	m_pResourceHash = new CHashTable<std::string,ResourceName>(nSize);
      }
      ////////////////////
      /// Destructor.
      ~CResourceManager() 
      {
	delete m_pResourceHash;
      }
      ////////////////////
      /// Creates new resource to pointer, named strName - handle 
      /// is initialized refer to that object.
      /// \param pType Pointer to object.
      /// \param strName Name for this resource.
      /// \param pHandle Pointer to handle which will refer to this resource.
      /// \returns zero on success, non-zero on error.
      int Create( OBJECTTYPE *pType, const std::string &strName, HANDLE *pHandle );
      ////////////////////
      /// Releases handle to this object.
      /// \param handle Handle to object.
      void Release( HANDLE &handle );
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
      unsigned int GetSize() const;
      
    private:
      void DeleteMemory();
    };
  };
};
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
int
Phoenix::Core::CResourceManager::Create( OBJECTTYPE *pType, 
					 const str::string &strName, 
					 HANDLE *pHandle )
{
  
  if ( m_pResourceHash->Find(strName)==NULL )
  {

    //  insert object into manager and get its index
    m_vecObjects.push_back( pType );
    unsigned int nIndex = m_vecObjects.size() - 1;
    // Create object with name and index mapping
    CResourceName resourceName;
    resourceName.SetName( strName );
    resourceName.SetIndex( nIndex );
    
    // Put string-> resourcename into hashtable.
    CHashItem<std::string,ResourceName> hashItem;
    hashItem.SetKey(strName);
    hashItem.SetObject(resourceName);
    
    m_pResourceHash->Insert( hashItem );
    pHandle->Initialize( nIndex );

    return 0;
  } 
  else
  {
    pHandle->Nullify();
  }
  return -1;
}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
void
Phoenix::Core::CResourceManager::DeleteMemory()
{
  for(unsigned int n=0;n<GetSize();n++)
  {
    if ( m_vecObjects[i] != NULL )
      delete m_vecObjects[i];
    m_vecObjects[i] = NULL;
  }
  delete m_pResourceHash;
  m_pResourceHash = NULL;
}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
void
Phoenix::Core::CResourceManager::GetResource( const HANDLE &handle)
{
  if ( handle.GetIndex() >= GetSize() || handle.IsNull() )
  {
    return NULL;
  }
  return m_vecObjects[handle.GetIndex()];
}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
void
Phoenix::Core::CResourceManager::GetResource( const std::string &strName )
{
  CResourceName *pResName = m_pResourceHash->Find(strName);

  if ( pResName == NULL ) return NULL;

  return m_vecObjects[pResName->GetIndex()];
}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
inline unsigned int
Phoenix::Core::CResourceManager::GetSize()
{
  return m_vecObjects.size();
}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
inline void
Phoenix::Core::CResourceManager::Release( HANDLE &handle )
{
  handle.Nullify();
  ///  remove pointer to handle in ResourceName (future)
}
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
