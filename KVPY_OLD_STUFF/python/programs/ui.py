
import appuifw
import e32

appuifw.app.title = u"Simar"

def onMenuitem1():
    screen_shot1()
    appuifw.note(u"Menu Item 1 Selected","info")
def onMenuitem2():
    appuifw.note(u"Menu Item 2 Selected","info")
def onMenuitem3():
    appuifw.note(u"Menu Item 3 Selected","info")


appuifw.app.menu=[(u"Menuitem1",onMenuitem1),

                  (u"Menuitem2",onMenuitem2),

                  (u"Menuitem3",onMenuitem3)]



def quit():
    app_lock.signal()
##your application code here

entries = [(u"Signal",u"signal strength"),(u"Battery",u"battery strength")]
#lb = appuifw.Listbox(entries, screen_shot1)

def screen_shot1():
    print "simar"

appuifw.app.screen='full'


appuifw.app.exit_key_handler=quit
app_lock=e32.Ao_lock()
app_lock.wait()