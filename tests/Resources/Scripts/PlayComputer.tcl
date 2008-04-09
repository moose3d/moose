proc playComputer { } {

    global g_fHealth g_fSpeed g_fMaxSpeed g_fAccel g_bHasTarget

    if { $g_fHealth <= 0.0 } {

	#Accelerate -5
	#Move
	Rotate 0.0 25 10.0

    } else {

	if { $g_fSpeed < 0.010 } {
	    Accelerate 0.01
	}
	
	if { $g_bHasTarget  } {
	    TurnTowardsTarget
	}
	# if target is OutOfRange  
	# move towards target.
	Move

    }

}