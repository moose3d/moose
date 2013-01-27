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

#ifndef __MooseStateMachine_h__
#define __MooseStateMachine_h__
/////////////////////////////////////////////////////////////////
#include <MooseGraphEdge.h>
#include <MooseGraphNode.h>
#include <MooseGraph.h>
#include <MooseCore.h>
#include <MooseResourceManager.h>
#include <iomanip>
#include <algorithm>
#include <iostream>
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace AI
  {
    /////////////////////////////////////////////////////////////////
    /// Wrapper for state concept. Basically it is an integer, which reacts to 
    /// different inputs (integers) and changes state.
    class MOOSE_API CState : public Moose::Core::CTypeBase<int> 
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
    class MOOSE_API CStateMachine
    {
    protected:
      typedef std::vector< Moose::AI::CState *> StateVector;
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
	  Moose::AI::CState *pState = new Moose::AI::CState;
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
} // namespace Moose
#endif
