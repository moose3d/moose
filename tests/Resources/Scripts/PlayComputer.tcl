proc FullThrust {} {
    global g_fSpeed g_fMaxSpeed

    if { $g_fSpeed < $g_fMaxSpeed } {
	Accelerate 0.25
    }
}

proc StopShip {} {
   global g_fSpeed
    if { $g_fSpeed > 0.0 } {
	Accelerate -0.5
    } else {
	Stop
    }
}
proc playComputer { } {

    global g_fHealth g_fSpeed g_fMaxSpeed g_fAccel g_bHasTarget \
	SHIP_ACTION_PASSIVE SHIP_ACTION_MOVING SHIP_ACTION_ATTACKING \
	SHIP_ACTION_GUARDING SHIP shipActionState

    if { $g_fHealth <= 0.0 } {

	if {  $g_fSpeed > 0.0 } {
	    Accelerate -0.5
	    Move
	}

	Rotate 0.0 2.5 1.0

    } else {

	switch -- $shipActionState  \
	$SHIP_ACTION_PASSIVE {
	    StopShip
	} \
	$SHIP_ACTION_MOVING {

	    FullThrust
	
	    if { $g_bHasTarget  } {
		TurnTowardsTarget
	    }
	    # if target is OutOfRange  
	    # move towards target.
	    Move
	    puts "moving"
	} \
	$SHIP_ACTION_ATTACKING {

	    if { [ TargetInWeaponsRange ] == "1" } {

		StopShip
		Move
		FirePhasers

	    } else {

		if { $g_bHasTarget  } {
		    TurnTowardsTarget
		}
		FullThrust
		Move
	    }
 	} \
	$SHIP_ACTION_GUARDING {
	    
	} \
	default {

	}
    	
    }    
    

}