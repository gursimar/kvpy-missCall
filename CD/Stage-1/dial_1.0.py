import telephone
import e32
import appuifw


number = 9915326619 #Enter the number you want to call
duration = appuifw.query(u"Enter the Durations",'number')#Enter the time

telephone.dial(str(number)) #make call 

#define the handler function 

def handle_hang_up(status):
    #test if the call was complete

    if status[0] == telephone.EStatusConnecting:
	print "\nConnecting"
        e32.ao_sleep(float(duration), telephone.hang_up)

telephone.call_state(handle_hang_up) #set the handler function