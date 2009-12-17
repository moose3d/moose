/////////////////////////////////////////////////////////////////
#ifndef __PhoenixCore_h__
#define __PhoenixCore_h__
/////////////////////////////////////////////////////////////////
#include <sstream>
#include <iostream>
#include <ostream>
#include <vector>
#include <map>
#include <list>
#include "PhoenixAPI.h"
#ifdef WIN32
#include "PhoenixWindowsWrapper.h"
#else
#include <sys/time.h>
#endif
#include <math.h>
/////////////////////////////////////////////////////////////////
#include "PhoenixNullable.h"
#include "PhoenixEnableable.h"
#include "PhoenixFocusable.h"
#include "PhoenixHashTable.h"
#include "PhoenixNamed.h"
#include "PhoenixSelectable.h"
#include "PhoenixSingleton.h"
#include "PhoenixTimeStamp.h"
#include "PhoenixTimer.h"
#include "PhoenixFPSCounter.h"
#include "PhoenixTypeBase.h"
#include "PhoenixUniqueNameCreator.h"
#include "PhoenixViewable.h"
#include "PhoenixObjectCounter.h"
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
