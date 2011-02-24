import telephone
import time
import e32


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
	e32.ao_sleep(2,simar)       

def simar():
    print "\n\n Inside simar"
    print end_ringing
    print start_ringing
    diff = (end_ringing-start_ringing)
    print diff
    
    if diff<2:
	print "\n:::::0 is sent"
    elif diff<5:
	print "\n:::::3 is sent"
    elif diff<8:
	print "\n:::::6 is sent"
    elif diff<11:
	print "\n:::::9 is sent"
    elif diff<14:
	print "\n:::::12 is sent"
    elif diff<17:
	print "\n:::::15 is sent"
    elif diff<20:
	print "\n:::::18 is sent"
    elif diff<23:
	print "\n:::::21 is sent"
    elif diff<26:
	print "\n:::::24 is sent"
    elif diff<29:
	print "\n:::::27 is sent"
    elif diff<32:
	print "\n:::::30 is sent"
    elif diff<35:
	print "\n:::::33 is sent"
    elif diff<38:
	print "\n:::::36 is sent"
    elif diff<41:
	print "\n:::::39 is sent"
    elif diff<44:
	print "\n:::::42 is sent"


telephone.incoming_call()
telephone.call_state(newphonestate)

