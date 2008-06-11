#ifndef __PhoenixStateMachine_h__
#define __PhoenixStateMachine_h__
/////////////////////////////////////////////////////////////////
#undef GetCurrentTime
#include <PhoenixGraph.h>
#include <PhoenixCore.h>
#include <PhoenixResourceManager.h>
#include <iomanip>
#include <algorithm>
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace AI
  {
    /////////////////////////////////////////////////////////////////    
    template <typename FSM_TYPE, typename STATE_NAME_TYPE, typename INPUT_NAME_TYPE>
    class CStateMachine;
    /////////////////////////////////////////////////////////////////    
    template <typename FSM_TYPE, typename STATE_NAME_TYPE, typename INPUT_NAME_TYPE>
    class CState : public Phoenix::Core::CGraphNode< FSM_TYPE, STATE_NAME_TYPE, INPUT_NAME_TYPE >
    {
      friend class CStateMachine< FSM_TYPE, STATE_NAME_TYPE, INPUT_NAME_TYPE >;
    public:
      CState()
      {
      }
    };
    ////////////////////
    /// Finite State Machine. Better implementation would probably be balanced bintree.
    template <typename FSM_TYPE, typename STATE_NAME_TYPE, typename INPUT_NAME_TYPE>
    class CStateMachine : public Phoenix::Core::CGraph< FSM_TYPE, STATE_NAME_TYPE, INPUT_NAME_TYPE >
    {
    protected:
      size_t m_nNumStates;
      /// Pointer to all states
      std::vector< CState<FSM_TYPE, STATE_NAME_TYPE, INPUT_NAME_TYPE> *>  m_vecStates;
      ////////////////////
      /// Returns reference to vector of states.
      inline std::vector< CState<FSM_TYPE, STATE_NAME_TYPE, INPUT_NAME_TYPE> *> & GetStates()
      {
	return m_vecStates;
      }
      ////////////////////
      /// Current machine state .
      STATE_NAME_TYPE m_CurrentState;
    public:
      ////////////////////
      /// Constructor. Creates given number of states for this FSM.
      /// All states are order by STATE_NAME_TYPE
      CStateMachine( size_t nNumStates) 
      {
	m_nNumStates = nNumStates;
	m_vecStates.reserve( nNumStates );
	SetCurrentState( (STATE_NAME_TYPE)0);

	for(size_t n=0;n<nNumStates;n++)
	{
	  // This does not work for some reason - complex template syntax, perhaps?
	  //m_vecStates[n] = Phoenix::Core::CGraph< FSM_TYPE, STATE_NAME_TYPE, 
	  // INPUT_NAME_TYPE>::CreateNode< CState< FSM_TYPE, STATE_NAME_TYPE, INPUT_NAME_TYPE> >();
	  m_vecStates.push_back( new Phoenix::AI::CState<FSM_TYPE, STATE_NAME_TYPE, INPUT_NAME_TYPE>());
	  Phoenix::Core::CGraph< FSM_TYPE, STATE_NAME_TYPE, INPUT_NAME_TYPE>::m_lstNodes.push_back( m_vecStates[n] );
	  m_vecStates[n]->m_pGraph = this;

	  m_vecStates[n]->SetName( (STATE_NAME_TYPE)n );
	}
      }
      ////////////////////
      /// Adds transition from one state to another with given input.
      /// \param nFromState Name of state where transition begins.
      /// \param nToState Name of state where transition leads to.
      /// \param nInput Name of input that ignites transition.
      void AddTransition( const STATE_NAME_TYPE & nFromState, const STATE_NAME_TYPE &nToState, const INPUT_NAME_TYPE &nInput )
      {
	Phoenix::Core::CGraph< FSM_TYPE, STATE_NAME_TYPE, INPUT_NAME_TYPE >::AddEdge( m_vecStates[nFromState], m_vecStates[nToState], nInput );
      }
      ////////////////////
      /// Returns name of next state on given input.
      /// \param nState Current state.
      /// \param nInput Input signal.
      /// \returns Name of state following the input.
      const STATE_NAME_TYPE & StateTransition( const STATE_NAME_TYPE & nState, const INPUT_NAME_TYPE &nInput )
      {
	typename std::list< Phoenix::Core::CGraphEdge< FSM_TYPE, STATE_NAME_TYPE, INPUT_NAME_TYPE > *>::iterator it;
	it = GetStates()[nState]->GetLeavingEdges().begin();

	for( ; it != GetStates()[nState]->GetLeavingEdges().end(); it++)
	{
	  if ( (*it)->GetName() == nInput )
	  {
	    return (*it)->GetToNode()->GetName();
	  }
	}
	return nState;
      }
      ///////////////////
      /// Processes given input and sets state accordingly.
      /// \param nInput Input where state machine reacts.
      void Process( const INPUT_NAME_TYPE & nInput )
      {
	SetCurrentState( StateTransition( GetCurrentState(), nInput));
      }
      ////////////////////
      /// Sets current state.
      /// \param state Next state.
      void SetCurrentState( const STATE_NAME_TYPE & state )
      {
	m_CurrentState = state;
      }
      ////////////////////
      /// Returns current state.
      /// \returns Current state.
      const STATE_NAME_TYPE & GetCurrentState()
      {
	return m_CurrentState;
      }
      ////////////////////
      /// Comparison operator for states directly.
      /// \param state State which current state is compared against.
      /// \returns Non-zero, if states are equal - zero otherwise.
      int operator==( const STATE_NAME_TYPE & state )
      {
	return ( GetCurrentState() == state );
      }
    };
  } // namespace AI
} // namespace Phoenix
#endif
