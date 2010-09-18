import telephone

def newphonestate (stateInformation):
    newState = stateInformation[0]    
    if newState == telephone.EStatusUnknown:
        msg = "The new state is unknown"
    elif newState == telephone.EStatusIdle:
        msg = "The phone is idle"
    elif newState == telephone.EStatusDialling:
        msg = "The phone is dialling"
    elif newState == telephone.EStatusRinging:
        msg = "The new phone is ringing, call is from %s" % stateInformation[1]
    elif newState == telephone.EStatusAnswering:
        msg = "A call is being answered"
    elif newState == telephone.EStatusConnecting:
        msg = "A call is connecting"
    elif newState == telephone.EStatusConnected:
        msg = "A call has been connected"
    elif newState == telephone.EStatusReconnectPending:
        msg = "The channel has been lost and a reconnect is being attempted"
    elif newState == telephone.EStatusDisconnecting:
        msg = "A call is being disconnected"
    elif newState == telephone.EStatusHold:
        msg = "A call is being placed on hold"
    elif newState == telephone.EStatusTransferring:
        msg = "A call is being transferred"
    elif newState == telephone.EStatusTransferAlerting:
        msg = "The phone is alerting the remote phone about a transferred call"
    print "\nThe phone has changed states."
    print "   ",msg 

telephone.call_state(newphonestate)