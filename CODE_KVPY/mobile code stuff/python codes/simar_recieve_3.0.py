import telephone
import time
import e32
import appuifw
import audio

song = audio.Sound.open(u"e:\Sounds\Digital\Jazzy B- Romeo.MP3")

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
    print diff
    
    if diff<3:
	appuifw.note(u"hello", "info")
    elif diff<6:
	appuifw.note(u"listen to a song.. wait", "info")
	song.play(1)
    elif diff<9:
	appuifw.note(u"love  you", "info")
    elif diff<12:
	appuifw.note(u"i'm busy.. cya later", "info")
    elif diff<15:
	appuifw.note(u"i will call you later", "info")
    elif diff<18:
	appuifw.note(u"cheers!!", "info")
    elif diff<21:
	appuifw.note(u"how are you?", "info")
    elif diff<24:
	appuifw.note(u"absolutely fine", "info")
    elif diff<27:
	appuifw.note(u"have a good day", "info")
    elif diff<30:
	appuifw.note(u"regards", "info")
    elif diff<33:
	appuifw.note(u"i will c you later", "info")
    elif diff<36:
	appuifw.note(u"i'm free", "info")
    elif diff<39:
	appuifw.note(u"i'm sleeping", "info")
    elif diff<42:
	appuifw.note(u"hi", "info")
    elif diff<45:
	appuifw.note(u"shut up please.", "info")




#telephone.incoming_call()
telephone.call_state(newphonestate)

