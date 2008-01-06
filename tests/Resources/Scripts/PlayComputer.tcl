proc playComputer { } {

    global g_fHealth g_fSpeed g_fMaxSpeed g_fAccel

    if { $g_fHealth <= 0.0 } {
	if { $g_fSpeed > 0.0 } {

	    if { $g_fAccel >= 5.0 } {
		Accelerate -5
	    } else {
		Accelerate -$g_fAccel
	    }
	    Move
	    Rotate 0.0 25 10.0
	} 

    } else {
	Accelerate 500
	Move
	Rotate 0.0 25 10.0
    }

}