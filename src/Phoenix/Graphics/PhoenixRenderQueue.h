#ifndef __PhoenixRenderQueue_h__
#define __PhoenixRenderQueue_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixOctree.h"
#include "PhoenixCamera.h"
#include "PhoenixOGLRenderer.h"
#include "PhoenixAPI.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Graphics
  {
    /////////////////////////////////////////////////////////////////
    /// Class for collecting and sorting renderable objects.
    template<typename TYPE>
    class PHOENIX_API CRenderQueue
    {
    protected:
      std::list<TYPE> m_lstObjects;
    public:
      ////////////////////
      /// Constructor.
      CRenderQueue();
      ////////////////////
      /// Destructor.
      ~CRenderQueue();
      ////////////////////
      /// Returns object list.
      /// \returns Reference to object list.
      std::list<TYPE> & GetObjectList();
      ////////////////////
      /// Renders current queue using given renderer.
      /// \param renderer OpenGL renderer.
      void Render( Phoenix::Graphics::COglRenderer &renderer );
      ////////////////////
      /// Clears current queue.
      void Clear();
      ////////////////////
      /// Performs sorting using adapter class.
      template<class ADAPTER_CLASS> void Sort( ADAPTER_CLASS & rAdapter );
    };
  }
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
Phoenix::Graphics::CRenderQueue<TYPE>::CRenderQueue()
{
  
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
Phoenix::Graphics::CRenderQueue<TYPE>::~CRenderQueue()
{
  
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
std::list<TYPE> & 
Phoenix::Graphics::CRenderQueue<TYPE>::GetObjectList()
{
  return m_lstObjects;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
void
Phoenix::Graphics::CRenderQueue<TYPE>::Clear()
{
  m_lstObjects.clear();
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
void 
Phoenix::Graphics::CRenderQueue<TYPE>::Render( Phoenix::Graphics::COglRenderer &renderer)
{
  typename std::list<TYPE>::iterator it = m_lstObjects.begin();
  for( ; it != m_lstObjects.end(); it++)
  {
    (*it)->Render(renderer); 
  }      
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
template<class ADAPTER_CLASS> 
void 
Phoenix::Graphics::CRenderQueue<TYPE>::Sort( ADAPTER_CLASS &rAdapter )
{
  rAdapter.Sort( m_lstObjects );
}
/////////////////////////////////////////////////////////////////

#endif
