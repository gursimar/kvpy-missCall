import telephone
import e32
import appuifw
import time


number = 8146010177 #Enter the number you want to call
duration = appuifw.query(u"Enter the Durations",'number')#Enter the time

telephone.dial(str(number)) #make call 

#define the handler function 

start_time=0.0
end_time=0.0

def handle_hang_up(status):
    #test if the call was complete

    if status[0] == telephone.EStatusConnected:
	print "\nConnected"
	end_time=time.clock()
	print time.clock()
	diff = end_time-start_time
	print diff
        e32.ao_sleep(float(duration), telephone.hang_up)

    elif status[0] == telephone.EStatusConnecting:
	print "\nConnecting"
	start_time=time.clock()
	print time.clock()


    elif status[0] == telephone.EStatusDialing:
	print "\nDialing"
	print time.clock()

telephone.call_state(handle_hang_up) #set the handler function