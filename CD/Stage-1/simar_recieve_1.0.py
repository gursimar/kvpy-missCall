import telephone
import time


print "\n\n\nSIMAR /... Heya!!!"

def newphonestate (stateInformation):
    
    global start_ringing
    global end_ringing

    print "\nI'm being calledback."
    
    newState = stateInformation[0]    
    
    if newState == telephone.EStatusRinging:
        start_ringing = time.clock()
        print "::The new phone is ringing, call is from %s" %stateInformation[1]
        print start_ringing
        
    elif newState == telephone.EStatusDisconnecting:       
        end_ringing = time.clock()
        print "::A call is being disconnected"
        print end_ringing

    elif newState == telephone.EStatusIdle:
	print "::Idle"
	simar()       

def simar():
    print "\n\n Inside simar"
    print end_ringing
    print start_ringing
    diff = (end_ringing-start_ringing)
    print diff

telephone.incoming_call()
telephone.call_state(newphonestate)

