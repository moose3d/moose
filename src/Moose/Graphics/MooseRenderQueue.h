#ifndef __MooseRenderQueue_h__
#define __MooseRenderQueue_h__
/////////////////////////////////////////////////////////////////
#include "MooseOctree.h"
#include "MooseCamera.h"
#include "MooseOGLRenderer.h"
#include "MooseAPI.h"
#include <vector>
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Graphics
  {
    /////////////////////////////////////////////////////////////////
    /// Class for collecting and sorting renderable objects.
    template<typename TYPE>
    class MOOSE_API CRenderQueue
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
      void Render( Moose::Graphics::COglRenderer &renderer );
      ////////////////////
      /// Clears current queue.
      void Clear();
      ////////////////////
      /// Performs sorting using adapter class.
      template<class SORTER> void Sort( SORTER & rAdapter );
    };
  }
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
Moose::Graphics::CRenderQueue<TYPE>::CRenderQueue()
{
  
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
Moose::Graphics::CRenderQueue<TYPE>::~CRenderQueue()
{
  
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
std::list<TYPE> & 
Moose::Graphics::CRenderQueue<TYPE>::GetObjectList()
{
  return m_lstObjects;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
void
Moose::Graphics::CRenderQueue<TYPE>::Clear()
{
  m_lstObjects.clear();
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
void 
Moose::Graphics::CRenderQueue<TYPE>::Render( Moose::Graphics::COglRenderer &renderer)
{
  typename std::list<TYPE>::iterator it = m_lstObjects.begin();
  for( ; it != m_lstObjects.end(); it++)
  {
    (*it)->Render(renderer); 
  }      
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
template<class SORTER> 
inline void 
Moose::Graphics::CRenderQueue<TYPE>::Sort( SORTER &rS )
{
  rS( m_lstObjects );
}
/////////////////////////////////////////////////////////////////

#endif
