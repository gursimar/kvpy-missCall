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
    elif diff<8:
	canvas.clear(0xffffff)		#white
    elif diff<11:
    elif diff<14:
    elif diff<17:
    elif diff<20:
    elif diff<23:
    elif diff<26:
    elif diff<29:
    elif diff<32:
    elif diff<35:
    elif diff<38:
    elif diff<41:
    elif diff<44:


telephone.incoming_call()
telephone.call_state(newphonestate)

App_lock = e32.Ao_lock()
App_lock.wait()
