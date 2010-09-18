import telephone
import e32
import appuifw
import time


number = 9915326619 #Enter the number you want to call
duration = appuifw.query(u"Enter the Durations",'number')#Enter the time

telephone.dial(str(number)) #make call 

#define the handler function 

def handle_hang_up(status):
    #test if the call was complete

    if status[0] == telephone.EStatusConnected:
	print "\nConnected"
	print time.clock()
        e32.ao_sleep(float(duration), telephone.hang_up)

    elif status[0] == telephone.EStatusConnecting:
	print "\nConnecting"
	print time.clock()


    elif status[0] == telephone.EStatusDialing:
	print "\nDialing"
	print time.clock()

telephone.call_state(handle_hang_up) #set the handler function