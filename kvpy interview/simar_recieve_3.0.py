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
    
    if diff<2:
	appuifw.note(u"hello", "info")
    elif diff<5:
	appuifw.note(u"listen to a song.. wait", "info")
	song.play(1)
    elif diff<8:
	appuifw.note(u"love  you", "info")
    elif diff<11:
	appuifw.note(u"i'm busy.. cya later", "info")
    elif diff<14:
	appuifw.note(u"i will call you later", "info")
    elif diff<17:
	appuifw.note(u"cheers!!", "info")
    elif diff<20:
	appuifw.note(u"how are you?", "info")
    elif diff<23:
	appuifw.note(u"absolutely fine", "info")
    elif diff<26:
	appuifw.note(u"have a good day", "info")
    elif diff<29:
	appuifw.note(u"regards", "info")
    elif diff<32:
	appuifw.note(u"i will c you later", "info")
    elif diff<35:
	appuifw.note(u"i'm free", "info")
    elif diff<38:
	appuifw.note(u"i'm sleeping", "info")
    elif diff<41:
	appuifw.note(u"hi", "info")
    elif diff<44:
	appuifw.note(u"shut up please.", "info")




#telephone.incoming_call()
telephone.call_state(newphonestate)

