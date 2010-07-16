/*
 * MooseSingleton.h
 *
 *  Created on: Dec 13, 2009
 *      Author: entity
 */

#ifndef MOOSESINGLETON_H_
#define MOOSESINGLETON_H_
#include "MooseAPI.h"
#include <cstdlib>
namespace Moose
{
	namespace Core
	{
	/////////////////////////////////////////////////////////////////
    /// Singleton template, which makes creating singleton objects easier.
    template <class T>
    class MOOSE_API CSingleton
    {

    private:
      /// Static pointer to only instance.
      static T *m_pSingleton;
      ////////////////////
      /// Copy constructor.
      CSingleton( const CSingleton &ref){}
    protected:
      ////////////////////
      /// Default constructor.
      CSingleton() {}
      ////////////////////
      /// Destructor.
      virtual ~CSingleton() {}
    public:
      ////////////////////
      /// Returns a pointer to this singleton.
      inline static T *GetInstance()
      {
	if ( m_pSingleton == NULL )
	{
	  m_pSingleton = new T();
	}
	return m_pSingleton;
      }
      /// Destroys this singleton.
      inline static void DestroyInstance()
      {
	if ( m_pSingleton != NULL )
	{
	  delete m_pSingleton;
	  m_pSingleton = NULL;
	}
      }
      /// Checks is this object instantiated
      static bool Exists()
      {
	return (m_pSingleton != NULL);
      }
    };
    /////////////////////////////////////////////////////////////////
    /// Initialize the static member in the derived classes.
    template<class T> T *CSingleton<T>::m_pSingleton = NULL;
	}
}
#endif /* MOOSESINGLETON_H_ */
