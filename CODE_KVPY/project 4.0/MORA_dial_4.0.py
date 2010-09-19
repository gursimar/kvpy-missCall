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
    M = [u"OFF",u"ON"]
    index= appuifw.selection_list(choices=M, search_field=1)
    if index==0:
        duration = 0

    if index==1:
        duration = 6
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