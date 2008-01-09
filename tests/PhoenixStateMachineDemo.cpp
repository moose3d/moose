#include <Phoenix.h>
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Core;
using namespace Phoenix::Math;
using namespace Phoenix::Spatial;
using namespace Phoenix::Window;
using namespace Phoenix::Graphics;
using namespace Phoenix::Scene;
using namespace Phoenix::Volume;
using namespace Phoenix::AI;
using namespace std;
/////////////////////////////////////////////////////////////////
enum NPC_STATE_MACHINE 
{
  NPC_STATE = 0
};
/////////////////////////////////////////////////////////////////
enum NPC_STATE_NAME
{
  NPC_UNCARING = 0,
  NPC_CURIOUS = 1,
  NPC_ANGRY,
  NPC_SCARED,
  NPC_DEAD,
  NPC_STATE_COUNT
};
/////////////////////////////////////////////////////////////////
enum INPUT_NAME
{
  INPUT_GREETING = 0,
  INPUT_DAMAGE = 1,
  INPUT_VITAL_HIT = 2
};
/////////////////////////////////////////////////////////////////
typedef CStateMachine< NPC_STATE_MACHINE, NPC_STATE_NAME, INPUT_NAME> CNPCStateMachine;
/////////////////////////////////////////////////////////////////
int main()
{
  CNPCStateMachine *pMachine = NULL;
  pMachine = new CNPCStateMachine( NPC_STATE_COUNT );
  pMachine->AddTransition( NPC_UNCARING, NPC_CURIOUS, INPUT_GREETING );
  pMachine->AddTransition( NPC_UNCARING, NPC_ANGRY, INPUT_DAMAGE );
  pMachine->AddTransition( NPC_CURIOUS,  NPC_ANGRY, INPUT_DAMAGE );

  if ( pMachine->StateTransition( NPC_UNCARING, INPUT_GREETING ) == NPC_CURIOUS )
  {
    cerr << "curious" << endl;
  }
  if ( pMachine->StateTransition( NPC_UNCARING, INPUT_VITAL_HIT ) == NPC_UNCARING )
  {
    cerr << "uncaring" << endl;
  }
  if ( pMachine->StateTransition( NPC_CURIOUS, INPUT_DAMAGE ) == NPC_ANGRY )
  {
    cerr << "angry" << endl;
  }
  return 0;
}
/////////////////////////////////////////////////////////////////
