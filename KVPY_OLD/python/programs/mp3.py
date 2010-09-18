import appuifw
import e32
import audio

def quit():
    app_lock.signal()
    song.stop()
    audio.Sound.close(song)

song = audio.Sound.open(u"e:\Sounds\Digital\mysong.mp3")    ## Open sound file
song.play(2)       ## Plays the song two times

appuifw.app.exit_key_hndler=quit
app_lock=e32.Ao_lock()
app_lock.wait()

