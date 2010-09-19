import telephone
import time
import e32
import appuifw

def quit():
    App_lock.signal()

appuifw.app.exit_key_handler = quit 

appuifw.app.screen='full'

canvas=appuifw.Canvas()
appuifw.app.body=canvas

## Clear the canvas with given color
canvas.clear(0xffffff)		#rgb oxrrggbb

def newphonestate (stateInformation):
    
    global start_ringing
    global end_ringing
    
    newState = stateInformation[0]    
    
    if newState == telephone.EStatusRinging:
        start_ringing = time.clock()
        
    elif newState == telephone.EStatusDisconnecting:       
        end_ringing = time.clock()

    elif newState == telephone.EStatusIdle:
	e32.ao_sleep(1,simar)       

def simar():
    diff = (end_ringing-start_ringing)
    
    if diff<2:
	canvas.clear(0x000000)		#black
    elif diff<5:
	print "\n:::::3 is sent::::::"
	appuifw.note(u":::::3 is sent::::::", "info")
    elif diff<8:
	canvas.clear(0xffffff)		#white
    elif diff<11:
	print "\n:::::9 is sent::::::"
	appuifw.note(u":::::9 is sent::::::", "info")
    elif diff<14:
	print "\n:::::12 is sent::::::"
	appuifw.note(u":::::12 is sent::::::", "info")
    elif diff<17:
	print "\n:::::15 is sent::::::"
	appuifw.note(u":::::15 is sent::::::", "info")
    elif diff<20:
	print "\n:::::18 is sent::::::"
	appuifw.note(u":::::18 is sent::::::", "info")
    elif diff<23:
	print "\n:::::21 is sent::::::"
	appuifw.note(u":::::21 is sent::::::", "info")
    elif diff<26:
	print "\n:::::24 is sent::::::"
	appuifw.note(u":::::24 is sent::::::", "info")
    elif diff<29:
	print "\n:::::27 is sent::::::"
	appuifw.note(u":::::27 is sent::::::", "info")
    elif diff<32:
	print "\n:::::30 is sent::::::"
	appuifw.note(u":::::30 is sent::::::", "info")
    elif diff<35:
	print "\n:::::33 is sent::::::"
	appuifw.note(u":::::33 is sent::::::", "info")
    elif diff<38:
	print "\n:::::36 is sent::::::"
	appuifw.note(u":::::36 is sent::::::", "info")
    elif diff<41:
	print "\n:::::39 is sent::::::"
	appuifw.note(u":::::39 is sent::::::", "info")
    elif diff<44:
	print "\n:::::42 is sent::::::"
	appuifw.note(u"::::42 is sent::::::", "info")


telephone.incoming_call()
telephone.call_state(newphonestate)

App_lock = e32.Ao_lock()
App_lock.wait()
