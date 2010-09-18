import appuifw
import e32
import graphics

def quit():
    App_lock.signal()
appuifw.app.exit_key_handler = quit 

appuifw.app.title = u"FREE Remote worldwide - simar"
appuifw.app.screen='full'

canvas=appuifw.Canvas()
appuifw.app.body=canvas

## Clear the canvas with given color
canvas.clear(0x000000)		#rgb oxrrggbb


App_lock = e32.Ao_lock()
App_lock.wait()



