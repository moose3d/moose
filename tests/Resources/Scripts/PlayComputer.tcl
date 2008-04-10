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
	    if { $g_fSpeed > 0.0 } {
		Accelerate -1.5
		Move
	    }
	} \
	$SHIP_ACTION_MOVING {
	    if { $g_fSpeed < $g_fMaxSpeed } {
		Accelerate 0.25
	    }
	
	    if { $g_bHasTarget  } {
		TurnTowardsTarget
	    }
	    # if target is OutOfRange  
	    # move towards target.
	    Move
	    
	} \
	$SHIP_ACTION_ATTACKING {

	} \
	$SHIP_ACTION_GUARDING {

	} \
	default {

	}
    	
    }    
    

}