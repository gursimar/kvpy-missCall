'''
# Symbian Foundation Example Code
#
# This software is in the public domain. No copyright is claimed, and you
# may use it for any purpose without license from the Symbian Foundation.
# No warranty for any purpose is expressed or implied by the authors or
# the Symbian Foundation.
'''

import telephone, contacts
import appuifw, e32, graphics, topwindow
import sys

#--------------------------------------------------------------------------------
#  This function is called when a phone call is made.  It does the
#  contact lookup and the display of the info window.
#--------------------------------------------------------------------------------

def displayNotes(infoTuple):

    # "window" must be global because quit() above uses it
    global window

    # Get the phone's state
    phoneState = infoTuple[0]
    
    # Only react when the phone is ringing
    if phoneState == telephone.EStatusRinging:
    
        # Start by opening the contacts database and looking up the phone number
        db = contacts.open()
        contactList = db.find(infoTuple[1])
        
        # Build the list of notes in a string, separated by "\n"
        noteString = ""
        if contactList:
            try:
                for note in contactList[0].find(type='note'):
                    if len(noteString)>0:
                        noteString = noteString + "\n" + note.value
                    else:
                        noteString = note.value
            except:
                noteString = "No notes available"
        else:
            noteString = "The phone number was not found"
            
        # Now we build the topwindow window from the notes
        notes = noteString.split("\n")
        window = topwindow.TopWindow()
        window.size = (350, 40+(30*len(notes)))
        window.position = (10, 40)
        img = graphics.Image.new((310, 30*len(notes)))
        img.clear(0x99CCFF)
        position = 20
        for note in notes:
            img.text((20, position), note, font = 'title')
            position += 30
        window.add_image(img, (20, 20))
        window.background_color = 0xDDDDDD
        window.shadow = 4
        window.corner_type = 'corner5'
        
        # Display the window and sleep for 5 seconds.  Then hide it.
        window.show()
        e32.ao_sleep(5)
        window.hide()

# Finally, install the function as the phone callback
telephone.call_state(displayNotes)
