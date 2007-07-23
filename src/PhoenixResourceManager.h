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
    class CResourceManager : public CSingleton<CResourceManager<OBJECTTYPE,HANDLE> >
    {
      friend class CSingleton<CResourceManager<OBJECTTYPE,HANDLE> >;
    protected:
      CHashTable<std::string,CResourceName> *m_pResourceHash;
      std::vector<OBJECTTYPE *> m_vecObjects;
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
Phoenix::Core::CResourceManager<OBJECTTYPE,HANDLE>::CResourceManager() : m_pResourceHash(NULL)
{
  // NOP
}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
Phoenix::Core::CResourceManager<OBJECTTYPE,HANDLE>::~CResourceManager() 
{
  if ( m_pResourceHash ) delete m_pResourceHash;
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

    //  insert object into manager and get its index
    m_vecObjects.push_back( pType );
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
  for(unsigned int n=0;n<GetSize();n++)
  {
    if ( m_vecObjects[n] != NULL )
      delete m_vecObjects[n];
    m_vecObjects[n] = NULL;
  }
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
  return m_vecObjects[handle.GetIndex()];
}
/////////////////////////////////////////////////////////////////
template<typename OBJECTTYPE, typename HANDLE>
OBJECTTYPE *
Phoenix::Core::CResourceManager<OBJECTTYPE,HANDLE>::GetResource( const std::string &strName ) const
{
  if ( m_pResourceHash == NULL ) return NULL;

  CHashItem<std::string, CResourceName> *pHashItem = m_pResourceHash->Find(strName);
  if ( pHashItem == NULL ) return NULL;

  return m_vecObjects[pHashItem->GetObject().GetIndex()];
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
  handle.Nullify();
  ///  remove pointer to handle in ResourceName (future)
}
/////////////////////////////////////////////////////////////////
#endif
