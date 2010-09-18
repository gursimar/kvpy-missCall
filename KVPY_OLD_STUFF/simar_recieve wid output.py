import telephone
import time

start_ringing=0.0000
end_ringing=0.0000

print "\n\n\nSIMAR /... yo yo honey singh"

def newphonestate (stateInformation):
    print "\nI'm being calledback."

    
    newState = stateInformation[0]    
    
    if newState == telephone.EStatusRinging:
        start_ringing = time.clock()
        print "The new phone is ringing, call is from %s" %stateInformation[1]
        print start_ringing
        
    elif newState == telephone.EStatusDisconnecting:       
        end_ringing = time.clock()
        print "A call is being disconnected"
        print end_ringing
	simar()            

def simar():
    print "\n\nsimar is great"
    diff = (end_ringing-start_ringing)
    print diff

telephone.incoming_call()
telephone.call_state(newphonestate)

Python for S60 Version 2.0.0 svn3873
Capabilities Present: ('ReadDeviceData', 'WriteDeviceData', 'SwEvent', 'NetworkServices', 'LocalServices', 'ReadUserData', 'WriteUserData', 'Location', 'UserEnvironment')

Select:
"Options -> Run script" to run a script
"Options -> About" to view copyright




SIMAR /... yo yo honey singh

I'm being calledback.
The new phone is ringing, call is from +918146010177
269678.859375

I'm being calledback.
A call is being disconnected
269687.875


simar is great
0.0

I'm being calledback.


