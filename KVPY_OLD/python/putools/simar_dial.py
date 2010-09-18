import telephone
import e32
import appuifw
number = appuifw.query(u"Enter Your Phone Number",'number') #Enter the number you want to call
duration = appuifw.query(u"Enter the Durations",'number')#Enter the time

telephone.dial(str(number)) #make call 

#define the handler function 
def handle_hang_up(status):
    #test if the call was complete
    if status[0] == telephone.EStatusConnected:
        #hangup after defined duration
        e32.ao_sleep(float(duration), telephone.hang_up)
    elif status[0] == telephone.EStatusConnecting
	print "connecting"

    elif status[0] == telephone.EStatusDialing
	print "dialing"

telephone.call_state(handle_hang_up) #set the handler function