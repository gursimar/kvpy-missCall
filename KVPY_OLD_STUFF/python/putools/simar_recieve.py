import telephone
import time

start_ringing=0.0000
end_ringing=0.0000
end_ringing_2=0
flag=0

print "\n\n\nSIMAR /... yo yo honey singh"

def newphonestate (stateInformation):
    print "\nI'm being calledback"
    newState = stateInformation[0]    
    if newState == telephone.EStatusUnknown:
        msg = "The new state is unknown"
        
    elif newState == telephone.EStatusIdle:
        msg = "The phone is idle"
        #end_ringing2=time.time()
        #print end_ringing_2-start_ringing
        # does not call after there is a miss call on the phone

    elif newState == telephone.EStatusDialling:
	print time.clock()
        msg = "The phone is dialling"
    
    elif newState == telephone.EStatusRinging:
        start_ringing = time.clock()
        msg = "The new phone is ringing, call is from %s" % stateInformation[1]
	print start_ringing
        
    elif newState == telephone.EStatusAnswering:
        msg = "A call is being answered"
    elif newState == telephone.EStatusConnecting:
	print time.clock()
        msg = "A call is connecting"
    elif newState == telephone.EStatusConnected:
	print time.clock()
        msg = "A call has been connected"
    elif newState == telephone.EStatusReconnectPending:
        msg = "The channel has been lost and a reconnect is being attempted"

    elif newState == telephone.EStatusDisconnecting:    	
        msg = "A call is being disconnected"
        end_ringing = time.clock()
	print "\nTime of call is " 
	print end_ringing
	flag=1
        #print (end_ringing-start_ringing)
        #print "simar ... bye"
        #not called when the 
        
        
    elif newState == telephone.EStatusHold:
        msg = "A call is being placed on hold"
    elif newState == telephone.EStatusTransferring:
        msg = "A call is being transferred"
    elif newState == telephone.EStatusTransferAlerting:
        msg = "The phone is alerting the remote phone about a transferred call"
    
    print "\nThe phone has changed states."
    print "   ",msg 

    if flag == 1:
	print "\n\n\n THE DURATION OF CALL IS"
        print end_ringing-start_ringing
	flag=0



telephone.call_state(newphonestate)