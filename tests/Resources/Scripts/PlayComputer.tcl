proc playComputer { } {

    global g_fHealth g_fSpeed g_fMaxSpeed g_fAccel

    if { $g_fHealth <= 0.0 } {

	#Accelerate -5
	#Move
	Rotate 0.0 25 10.0

    } else {
	if { $g_fSpeed < 1.0 } {
	    Accelerate 5
	}
	Move
	Rotate 0.0 150 0.0
    }

}