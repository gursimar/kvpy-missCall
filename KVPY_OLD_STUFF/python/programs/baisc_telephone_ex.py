import appuifw
import e32
import telephone



def main_menu_setup():
    appuifw.app.menu = [(u"dial", dialing),\
                        (u"hang-up", hangingup)]


def hangingup():
    telephone.hang_up()
    main_menu_setup()
    

def dialing():
    number = u'+1234566'
    telephone.dial(number)
    main_menu_setup()



def exit_key_handler():
    global script_lock
    script_lock.signal()
    


script_lock = e32.Ao_lock()


old_title = appuifw.app.title
appuifw.app.title = u"Dialer"
main_menu_setup()
appuifw.app.exit_key_handler = exit_key_handler
script_lock.wait()