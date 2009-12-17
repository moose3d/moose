#ifndef __PhoenixStateMachine_h__
#define __PhoenixStateMachine_h__
/////////////////////////////////////////////////////////////////
#include <PhoenixGraphEdge.h>
#include <PhoenixGraphNode.h>
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
    /// Wrapper for state concept. Basically it is an integer, which reacts to 
    /// different inputs (integers) and changes state.
    class PHOENIX_API CState : public Phoenix::Core::CTypeBase<int> 
    {
    protected:
      typedef std::map< int, int> InputToStateMap;       
      InputToStateMap m_mapTransitions;
    public:

      ////////////////////
      /// Registers input to specific state, overriding previous setting if it existed.
      /// \param iInput Input value
      /// \param iState State type index
      void AddTransition( int iInput, int iState )
      {
	m_mapTransitions[iInput] = iState;
      }
      ////////////////////
      /// Returns next state type index if it has been registered to given input.
      /// \param iInput Input to react with.
      /// \returns State type index. If transition is not available, then current one.
      int GetTransition( int iInput )
      {
	InputToStateMap::iterator it = m_mapTransitions.find( iInput);
	if ( it == m_mapTransitions.end())	return GetType();
	else					return it->second;
      }
      ////////////////////
      /// Checks whether input has been registered.
      /// \returns true if has been registered.
      /// \returns false if has not been registered.
      bool HasTransition( int iInput )
      {
	InputToStateMap::iterator it = m_mapTransitions.find( iInput);
	return ( it != m_mapTransitions.end() );
      }
      
    };
    ////////////////////////////////////////////////////////////////////////////
    /// Finite State Machine. 
    template<class STATE, class INPUT>
    class PHOENIX_API CStateMachine
    {
    protected:
      typedef std::vector< Phoenix::AI::CState *> StateVector;
      StateVector m_vecStates;
      
      ////////////////////
      /// Returns reference to vector of states.
      inline StateVector & GetStates()
      {
	return m_vecStates;
      }
      ////////////////////
      /// Current machine state .
      STATE m_CurrentState;
    public:
      ////////////////////
      /// Constructor. Creates given number of states for this FSM.
      /// All states are order by STATE
      CStateMachine( size_t nNumStates ) 
      {
	m_vecStates.reserve( nNumStates );
	SetCurrentState( static_cast<STATE>(0) );
	
	for(size_t n=0;n<nNumStates;n++)
	{
	  Phoenix::AI::CState *pState = new Phoenix::AI::CState;
	  pState->SetType( n );
	  m_vecStates.push_back( pState );
	}
      }
      ////////////////////
      /// Adds transition from one state to another with given input.
      /// \param nFromState Name of state where transition begins.
      /// \param nToState Name of state where transition leads to.
      /// \param nInput Name of input that ignites transition.
      void AddTransition( const STATE nFromState, const STATE nToState, const INPUT nInput )
      {
	m_vecStates[nFromState]->AddTransition( nInput, nToState );
      }
      ////////////////////
      /// Returns name of next state on given input.
      /// \param nState Current state.
      /// \param nInput Input signal.
      /// \returns Name of state following the input.
      const STATE StateTransition( const STATE nState, const INPUT nInput )
      {
	return static_cast<STATE>(GetStates()[nState]->GetTransition( nInput ));
      }
      ///////////////////
      /// Processes given input and sets state accordingly.
      /// \param nInput Input where state machine reacts.
      void Process( const INPUT nInput )
      {
	SetCurrentState( StateTransition( GetCurrentState(), nInput));
      }
      ////////////////////
      /// Sets current state.
      /// \param state Next state.
      void SetCurrentState( const STATE state )
      {
	m_CurrentState = state;
      }
      ////////////////////
      /// Returns current state.
      /// \returns Current state.
      const STATE GetCurrentState()
      {
	return m_CurrentState;
      }
      ////////////////////
      /// Comparison operator for states directly.
      /// \param state State which current state is compared against.
      /// \returns Non-zero, if states are equal - zero otherwise.
      bool operator==( const STATE state )
      {
	return ( GetCurrentState() == state );
      }
    };
  } // namespace AI
} // namespace Phoenix
#endif
