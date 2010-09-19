import telephone
import e32
import appuifw

#def quit():
#    App_lock.signal()
#appuifw.app.exit_key_handler = quit 

##app code starts
number = 9915326619 #Enter the number you want to call

appuifw.app.screen='normal'

txt = appuifw.Text()
appuifw.app.body = txt 
txt.color = 0x3A5FCD
txt.font = u"LatinBold25"
#txt.highlight_color = 0xc0c0c0
txt.style = appuifw.STYLE_BOLD
txt.style = appuifw.HIGHLIGHT_SHADOW
txt.add(u"SIMAR\n")

def loop():
    global duration
    M = [u"Hello",u"Play a song",u"Love you",u"I'm busy .. c ya later",u"I will call you later",u"cheers..",u"how are you",u"fine absolutely",u"have a good day",u"regards",u"i will c you later",u"i'm free ..",u"i'm sleeping",u"hi",u"Shup up"]

    index= appuifw.selection_list(choices=M, search_field=1)
    if index==0:
        duration = 0
    elif index==1:
        duration = 3
    elif index==2:
        duration = 6
    elif index==3:
        duration = 9
    elif index==4:
        duration = 12
    elif index==5:
        duration = 15
    elif index==6:
        duration = 18
    elif index==7:
        duration = 21
    elif index==8:
        duration = 24
    elif index==9:
        duration = 27
    elif index==10:
        duration = 30
    elif index==11:
        duration = 33
    elif index==12:
        duration = 36
    elif index==13:
        duration = 39
    elif index==14:
        duration = 42

    telephone.dial(str(number)) #make call 




#define the handler function 

def handle_hang_up(status):
    #test if the call was complete

    if status[0] == telephone.EStatusConnecting:
        e32.ao_sleep(float(duration), telephone.hang_up)

    elif status[0] == telephone.EStatusDisconnecting:
	e32.ao_sleep(1, loop)


telephone.call_state(handle_hang_up) #set the handler function

loop()

#appuifw.app.exit_key_handler=quit
#app_lock=e32.Ao_lock()
#app_lock.wait()