import telephone

def newphonestate (stateInformation):

    # Build the list of notes in a string, separated by "\n"
    noteString = ""

    newState = stateInformation[0]    
    if newState == telephone.EStatusUnknown:
        msg = "The new state is unknown"
	notestring = noteString + msg + "\n" 
    elif newState == telephone.EStatusIdle:
        msg = "The phone is idle"
	notestring = noteString + msg + "\n" 
    elif newState == telephone.EStatusDialling:
        msg = "The phone is dialling"
	notestring = noteString + msg + "\n" 
    elif newState == telephone.EStatusRinging:
        msg = "The new phone is ringing, call is from %s" % stateInformation[1]
	notestring = noteString + msg + "\n" 
    elif newState == telephone.EStatusAnswering:
        msg = "A call is being answered"
	notestring = noteString + msg + "\n" 
    elif newState == telephone.EStatusConnecting:
        msg = "A call is connecting"
	notestring = noteString + msg + "\n" 
    elif newState == telephone.EStatusConnected:
        msg = "A call has been connected"
	notestring = noteString + msg + "\n" 
    elif newState == telephone.EStatusReconnectPending:
        msg = "The channel has been lost and a reconnect is being attempted"
	notestring = noteString + msg + "\n" 
    elif newState == telephone.EStatusDisconnecting:
        msg = "A call is being disconnected"
	notestring = noteString + msg + "\n" 
    elif newState == telephone.EStatusHold:
        msg = "A call is being placed on hold"
	notestring = noteString + msg + "\n" 
    elif newState == telephone.EStatusTransferring:
        msg = "A call is being transferred"
	notestring = noteString + msg + "\n" 
    elif newState == telephone.EStatusTransferAlerting:
        msg = "The phone is alerting the remote phone about a transferred call"
	notestring = noteString + msg + "\n" 
    print "\nThe phone has changed states."
    print "   ",msg 
    # Now we build the topwindow window from the notes
    notes = noteString.split("\n")
    window = topwindow.TopWindow()
    window.size = (350, 40+(30*len(notes)))
    window.position = (10, 40)
    img = graphics.Image.new((310, 30*len(notes)))
    img.clear(0x99CCFF)
    position = 20
    for note in notes:
       img.text((20, position), note, font = 'title')
       position += 30
    window.add_image(img, (20, 20))
    window.background_color = 0xDDDDDD
    window.shadow = 4
    window.corner_type = 'corner5'
        
    # Display the window and sleep for 5 seconds.  Then hide it.
    window.show()
    e32.ao_sleep(5)
    window.hide()


telephone.call_state(newphonestate)