#ifndef __PhoenixStateMachine_h__
#define __PhoenixStateMachine_h__
/////////////////////////////////////////////////////////////////
#include <PhoenixGraph.h>
#include <PhoenixCore.h>
#include <PhoenixResourceManager.h>
#include <iomanip>
#include <algorithm>
#include <iostream>
#include "PhoenixAPI.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace AI
  {
    
    /////////////////////////////////////////////////////////////////    
    template <typename STATE_TYPE>
    class PHOENIX_API CState : public Phoenix::Core::TGraphNode< CState<STATE_TYPE> >,
		   public Phoenix::Core::CTypeBase<STATE_TYPE>
    {
    };

    template <class N, typename INPUT_TYPE>
    class PHOENIX_API CStateEdge : public Phoenix::Core::TGraphEdge< N >,
		       public Phoenix::Core::CTypeBase<INPUT_TYPE>
    {
      
    };
    ////////////////////
    /// Finite State Machine. 
    template <typename STATE_TYPE, typename INPUT_TYPE>
    class PHOENIX_API CStateMachine : public Phoenix::Core::TGraph< CState<STATE_TYPE> >
    {
    protected:
      typedef std::vector< CState< STATE_TYPE> *> StateVector;
      StateVector m_vecStates;
      ////////////////////
      /// Returns reference to vector of states.
      inline StateVector & GetStates()
      {
	return m_vecStates;
      }
      ////////////////////
      /// Current machine state .
      STATE_TYPE m_CurrentState;
    public:
      ////////////////////
      /// Constructor. Creates given number of states for this FSM.
      /// All states are order by STATE_NAME_TYPE
      CStateMachine( size_t nNumStates ) 
      {
	m_vecStates.reserve( nNumStates );
	SetCurrentState( (STATE_TYPE)0 );
	
	for(size_t n=0;n<nNumStates;n++)
	{
	  //Phoenix::AI::CState<STATE_TYPE> *pState = this->CreateNode< Phoenix::AI::CState<STATE_TYPE> >();
	  Phoenix::AI::CState<STATE_TYPE> *pState = new Phoenix::AI::CState<STATE_TYPE>();
	  RegisterNode(pState);
	  pState->SetType( (STATE_TYPE)n );
	  m_vecStates.push_back( pState );
	}
      }
      ////////////////////
      /// Adds transition from one state to another with given input.
      /// \param nFromState Name of state where transition begins.
      /// \param nToState Name of state where transition leads to.
      /// \param nInput Name of input that ignites transition.
      void AddTransition( const STATE_TYPE & nFromState, const STATE_TYPE &nToState, const INPUT_TYPE &nInput )
      {
	Phoenix::AI::CStateEdge<STATE_TYPE,INPUT_TYPE> *pEdge = this->AddEdge( m_vecStates[nFromState], m_vecStates[nToState] );
	pEdge->SetType(nInput);
      }
      ////////////////////
      /// Returns name of next state on given input.
      /// \param nState Current state.
      /// \param nInput Input signal.
      /// \returns Name of state following the input.
      const STATE_TYPE & StateTransition( const STATE_TYPE & nState, const INPUT_TYPE &nInput )
      {
	typename std::list< Phoenix::AI::CStateEdge<STATE_TYPE, INPUT_TYPE> *>::iterator it;
	it = GetStates()[nState]->GetLeavingEdges().begin();

	for( ; it != GetStates()[nState]->GetLeavingEdges().end(); it++)
	{

	  if ( static_cast< Phoenix::AI::CStateEdge<STATE_TYPE, INPUT_TYPE> *>(*it)->GetType() == nInput )
	  {
	    return static_cast< Phoenix::AI::CState<STATE_TYPE> *>((*it)->GetToNode())->GetType();
	  }
	}
	return nState;
      }
      ///////////////////
      /// Processes given input and sets state accordingly.
      /// \param nInput Input where state machine reacts.
      void Process( const INPUT_TYPE & nInput )
      {
	SetCurrentState( StateTransition( GetCurrentState(), nInput));
      }
      ////////////////////
      /// Sets current state.
      /// \param state Next state.
      void SetCurrentState( const STATE_TYPE & state )
      {
	m_CurrentState = state;
      }
      ////////////////////
      /// Returns current state.
      /// \returns Current state.
      const STATE_TYPE & GetCurrentState()
      {
	return m_CurrentState;
      }
      ////////////////////
      /// Comparison operator for states directly.
      /// \param state State which current state is compared against.
      /// \returns Non-zero, if states are equal - zero otherwise.
      bool operator==( const STATE_TYPE & state )
      {
	return ( GetCurrentState() == state );
      }
    };
  } // namespace AI
} // namespace Phoenix
#endif
