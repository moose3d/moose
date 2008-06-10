#ifndef __PhoenixEventHandler_h__
#define __PhoenixEventHandler_h__
/////////////////////////////////////////////////////////////////
#include <typeinfo>
#include <map>
/////////////////////////////////////////////////////////////////
class TypeInfo
{
private:
  const std::type_info & m_TypeInfo;
public:
  explicit TypeInfo(const std::type_info& info) : m_TypeInfo(info) {};

  bool operator < (const TypeInfo& rhs) const
  {
    return m_TypeInfo.before(rhs.m_TypeInfo) != 0;
  }
};
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Core
  {
    ////////////////////
    /// Base class for events.
    class CEvent 
    {
    public:
      virtual ~CEvent() {}
    };
    
    class CHandlerFunctionBase
    {
    public:
      virtual ~CHandlerFunctionBase() {}
      ////////////////////
      /// 
      inline void     Exec( const Phoenix::Core::CEvent * pEvent ) 
      { 
	Call(pEvent);
      }
      virtual void Call( const Phoenix::Core::CEvent *pEvent ) = 0;
    };


    template <class CLASS_TYPE, class EVENT_TYPE>
    class CMemberFunctionHandler : public CHandlerFunctionBase
    {
    private:
      CLASS_TYPE* m_pInstance;
      typedef void (CLASS_TYPE::* MemberFunc)(EVENT_TYPE *);
      MemberFunc m_Function;

    public:
      ////////////////////
      /// 
      CMemberFunctionHandler(CLASS_TYPE* pInstance, MemberFunc memFunc) : m_pInstance(pInstance), 
									  m_Function(memFunc) 
      {
	
      };
      ////////////////////
      ///
      void Call(const Phoenix::Core::CEvent* pEvent)
      {
	(m_pInstance->*m_Function)(static_cast<EVENT_TYPE*>(pEvent));
      }

    };
    /////////////////////////////////////////////////////////////////
    class CEventHandler
    {
    private:
      typedef std::map<TypeInfo, CHandlerFunctionBase*> HandlerMap;
      HandlerMap m_mapHandlers;
    public:
      ~CEventHandler();
      void HandleEvent(const Phoenix::Core::CEvent *pEvent );
      
      template <class CLASS_TYPE, class EVENT_TYPE>
      void RegisterHandler(CLASS_TYPE *, void (CLASS_TYPE::*memFunc)(EVENT_TYPE*));
    };
  } // Core
} // Phoenix
/////////////////////////////////////////////////////////////////
template <class CLASS_TYPE, class EVENT_TYPE>
void 
Phoenix::Core::CEventHandler::RegisterHandler(CLASS_TYPE* pObj, void (CLASS_TYPE::*memFunc)(EVENT_TYPE*))
{
  m_mapHandlers[TypeInfo(typeid(EVENT_TYPE))]= new Phoenix::Core::CMemberFunctionHandler<CLASS_TYPE, EVENT_TYPE>(pObj, memFunc);
}
/////////////////////////////////////////////////////////////////
#endif
