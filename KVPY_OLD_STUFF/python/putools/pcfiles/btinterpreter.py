"""Interpreter executes Python commands."""

import os
import sys
from code import InteractiveInterpreter
#import dispatcher
import wx.py.dispatcher
#import introspect
import wx
import time

# assuming phcomm is in ../libs relative to this file, now import finds it
# even if that directory is not in environment variable PYTHONPATH
sys.path.append( os.path.join( os.path.split(__file__)[0], '../libs' ) )
import phcomm
import glob

class BTcomm( phcomm.SvrCli ):

    #def __init__( self, verbose=1 ):
    def __init__( self, verbose=0 ):
        # connect to phone
        execfile( 'sync.config', globals(), globals() )
        sock = phcomm.connect_PC2phone( com_port=COM_PORT, verbose=verbose )
        phcomm.SvrCli.__init__( self, sock, verbose )

class SnapFrame( wx.Frame ):

    def __init__( self, parent ):
        wx.Frame.__init__( self, parent, -1, 'snapshot' )
        self.panel = wx.Panel( self, -1 )
        self.Bind( wx.EVT_KEY_DOWN, self.OnKeyDown )
        self.Show( False )
        #self.x = 1
        self.x = 2

    def update( self, fname = '', title = 'snapshot' ):
        if fname:
            self.fname = fname
        img = wx.Image( self.fname )
        if self.x != 1:
            img.Rescale( img.GetWidth() * self.x, img.GetHeight() * self.x )
        self.bmp = img.ConvertToBitmap()
        self.SetTitle( title )
        self.draw()
        
    def draw( self ):
        self.SetClientSize( (self.bmp.GetWidth(), self.bmp.GetHeight()) )
        wx.StaticBitmap( self.panel, -1, self.bmp )
        self.Show( True )
        self.Raise()
        self.Refresh()
        self.Update()

    def OnKeyDown( self, event ):
        keycode = event.GetKeyCode()
        print 'key', keycode
        if keycode == 43:   # pluskey
            self.x = min( self.x+1, 4 )
        elif keycode == 45: # minuskey
            self.x = max( self.x-1, 1 )
        self.update()
        
class Interpreter(InteractiveInterpreter):
    """Interpreter based on code.InteractiveInterpreter."""
    
    def __init__(self, owner=None, locals=None, rawin=None, 
                 stdin=sys.stdin, stdout=sys.stdout, stderr=sys.stderr):
        """Create an interactive interpreter object."""

        self.bt = BTcomm()
        
        InteractiveInterpreter.__init__(self, locals=locals)
        self.stdin = stdin
        self.stdout = stdout
        self.stderr = stderr
        if rawin:
            import __builtin__
            __builtin__.raw_input = rawin
            del __builtin__
        copyright = 'Type "help", "copyright", "credits" or "license"'
        copyright += ' for more information.'
        self.introText = 'Python %s on %s%s%s' % \
                         (sys.version, sys.platform, os.linesep, copyright)
        try:
            sys.ps1
        except AttributeError:
            sys.ps1 = '>>> '
        try:
            sys.ps2
        except AttributeError:
            sys.ps2 = '... '
        self.more = 0
        # List of lists to support recursive push().
        self.commandBuffer = []
        self.startupScript = None
        #self.first = True

        # the class that instantiates this, here phoneshell
        self.owner = owner
        # create a frame for snapshot images
        self.snapshot = SnapFrame( owner )

        # make sure we'll get a fresh interpreter at the phone end
        self.bt.send( 'reset' )

    def push(self, command):
        """Send command to the interpreter to be executed.
        
        Because this may be called recursively, we append a new list
        onto the commandBuffer list and then append commands into
        that.  If the passed in command is part of a multi-line
        command we keep appending the pieces to the last list in
        commandBuffer until we have a complete command. If not, we
        delete that last list."""

        # In case the command is unicode try encoding it
        if type(command) == unicode:
            try:
                command = command.encode(wx.GetDefaultPyEncoding())
            except UnicodeEncodeError:
                print >> self.stdout, 'UnicodeEncodeError'
                print >> self.stdout, 'Did not send', command, 'to phone'
                command = ''
        #print >> self.stdout, 'push', repr(command)
        if not self.more:
            try: del self.commandBuffer[-1]
            except IndexError: pass
        if not self.more: self.commandBuffer.append([])
        self.commandBuffer[-1].append(command)
        source = '\n'.join(self.commandBuffer[-1])
        more = self.more = self.runsource(source)
        wx.py.dispatcher.send( signal='Interpreter.push', sender=self,
                               command=command, more=more, source=source )
        return more
        
    def runsource(self, source):
        """Compile and run source code in the interpreter."""

        #print >> self.stdout, 'runsource', repr( source )
        if not source:
            # ignore empty lines
            return False

#         if self.first:
#             self.first = False
#             # let's not run pc startup script on the phone
#             if 'Startup script' in source:
#                 return False

        #print 'going to run', source
        if source == 'quit':
            self.bt.send( 'quit' )
            sys.exit()
        elif source == 'stop':
            # just stop the phone end
            self.bt.send( 'quit' )
            return False
        elif source == 'reset':
            self.bt.send( 'reset' )
            return False
        elif source == 'sync':
            self.synchronize()
            return False
        elif source == 'syncl':
            self.synchronize(1)
            return False
        elif source.split()[0] == 'snap':
            self.bt.send( 'snapshot' )
            self.bt.recv_file( 'snap.jpg' )
            self.snapshot.update( 'snap.jpg' )
            try:
                filename = source.split()[1]
                if os.path.splitext( filename )[1].lower() != '.jpg':
                    filename = filename + '.jpg'
                os.remove( filename )
                os.rename( 'snap.jpg', filename )
                print >> self.stdout, 'saved ' + filename
            except:
                pass
            return False
        elif source.split()[0] == 'view':
            filename = source.split()[1]
            self.bt.send( 'view %s' % filename )
            if int(self.bt.readline()):
                # yes, a file is coming
                filename = self.bt.readline().strip()
                tmpfilename = 'tmp' + os.path.splitext( filename )[1]
                self.bt.recv_file( tmpfilename )
                self.snapshot.update( tmpfilename, filename )
            else:
                # no, couldn't find the file or wrong type
                print >> self.stdout, 'cannot display %s' % filename
            return False
        elif source == 'runstartup':
            self.bt.send( 'runstartup' )
        else:
            #stdin, stdout, stderr = sys.stdin, sys.stdout, sys.stderr
            #sys.stdin, sys.stdout, sys.stderr = self.stdin, self.stdout, self.stderr
            self.bt.send( 'cmdline' )
            #print >> self.stdout, 'before send'
            #print >> self.stdout, source
            #print >> self.stdout, repr(source)
            self.bt.send_pyobj( source )
            #print >> self.stdout, 'after send'
        while 1:
            # don't want to block pc ui while waiting for response from the phone
            line = ''
            while not line:
                line = self.bt.readline_dontblock()
                wx.YieldIfNeeded()
            #print >> self.stdout, line
            head = line.split()[0]
            if head == 'output':
                output = self.bt.recv_data().rstrip()
                #print >> self.stdout, 'before output'
                print >> self.stdout, output
                print output
                #print >> self.stdout, 'after output'
            elif head == 'snap':
                #try:
                    self.bt.recv_file( 'snap.jpg' )
                    self.snapshot.update( 'snap.jpg', line.split()[1] )
                #except:
                #    print 'snap file receive error'
            elif head == 'more':
                return True
            elif head == 'nomore':
                return False
            wx.YieldIfNeeded()
        #sys.stdin, sys.stdout, sys.stderr = stdin, stdout, stderr
#         stdin, stdout, stderr = sys.stdin, sys.stdout, sys.stderr
#         sys.stdin, sys.stdout, sys.stderr = \
#                    self.stdin, self.stdout, self.stderr
#         more = InteractiveInterpreter.runsource(self, source)
#         # If sys.std* is still what we set it to, then restore it.
#         # But, if the executed source changed sys.std*, assume it was
#         # meant to be changed and leave it. Power to the people.
#         if sys.stdin == self.stdin:
#             sys.stdin = stdin
#         if sys.stdout == self.stdout:
#             sys.stdout = stdout
#         if sys.stderr == self.stderr:
#             sys.stderr = stderr
#         return more
        
    def getAutoCompleteKeys(self):
        """Return list of auto-completion keycodes."""
        return [ord('.')]

    def getAutoCompleteList(self, command='', *args, **kwds):
        """Return list of auto-completion options for a command.
        
        The list of options will be based on the locals namespace."""
        stdin, stdout, stderr = sys.stdin, sys.stdout, sys.stderr
        sys.stdin, sys.stdout, sys.stderr = \
                   self.stdin, self.stdout, self.stderr
        #l = introspect.getAutoCompleteList(command, self.locals,
        #                                   *args, **kwds)
        l = []
        sys.stdin, sys.stdout, sys.stderr = stdin, stdout, stderr
        return l

    def getCallTip(self, command='', *args, **kwds):
        """Return call tip text for a command.
        
        Call tip information will be based on the locals namespace."""
        #return introspect.getCallTip(command, self.locals, *args, **kwds)
        return ('', '', '')  # object name, argspec, tip text.

    def synchronize( self, reload=False ):
        # read in the config file
        # it should create variables COM_PORT and SYNC_FILES
        #print 'config file', self.owner.sync_config_file
        #execfile( self.owner.sync_config_file, {}, d )
        execfile( 'sync.config', globals(), globals() )
        # chdir to the directory of the config_file (if not already there)
        # so relative paths work
        cwd = os.getcwd()
        try:
            os.chdir( os.path.split( config_file )[0] )
        except:
            pass

        #print 'before sync'
        if reload:
            self.bt.send( 'syncl' )
        else:
            self.bt.send( 'sync' )
        #print 'after sync'

        # create lists of local and corresponding remote files and checksums
        remotefiles, localfiles, checksums = [], [], []
        for dest, srcs in SYNC_FROM_PC:
            # for each destination directory there can be several
            # source directories
            dpath = os.path.normpath( dest )
            if not isinstance(srcs, tuple) and not isinstance(srcs, list):
                # force sources to be a list
                srcs = [ srcs ]
            for src in srcs:
                # for each source location
                for f in glob.glob( src ):
                    # find matching files
                    file = os.path.split(f)[1]
                    # create remote name, local name, checksum
                    remotefiles.append( os.path.join( dpath, file ) )
                    localfiles.append( os.path.normpath( f ) )
                    checksums.append( phcomm.file_checksum( f ) )

        #print 'send upload files', repr(zip(remotefiles, localfiles, checksums))
        # send to phone a list of remote, local, checksum triplets
        self.bt.send_data( repr(zip(remotefiles, localfiles, checksums)) )

        #print 'send SYNC_TO_PC', repr( SYNC_TO_PC )
        # then send the info about the files that you want to download to pc
        self.bt.send_data( repr( SYNC_TO_PC ) )
        #print 'after download files'

        # now the phone loops over the files that it wants to get
        line = self.bt.readline()
        while line.split()[0] == 'getfile':
            filename = self.bt.recv_data()
            print filename
            data = open( filename, 'rb' ).read()
            print 'read data'
            self.bt.send_data( data )
            print 'sent it'
            print >> self.stdout, 'sent file: ' + filename
            line = self.bt.readline()

        # now the phone loops over the files that it wants to send
        while line.split()[0] == 'offerfile':
            crc      = int( line.split()[1] )
            filename = self.bt.recv_data()
            pc_crc   = phcomm.file_checksum( filename )
            if crc == pc_crc:
                # don't need this file
                self.bt.send( '0' )
            else:
                # yes, want this file
                self.bt.send( '1' )
                # create directory if needed
                dirpath = os.path.split( filename )[0]
                if not os.path.exists( dirpath ):
                    os.makedirs( dirpath )
                # read and write the file
                print >> self.stdout, 'getting file: ' + filename
                print 'getting file: ' + filename
                wx.YieldIfNeeded()
                #open( filename, 'wb' ).write( self.bt.recv_data() )
                self.bt.recv_file( filename )
                print >> self.stdout, 'received file: ' + filename
                print 'received file: ' + filename
                wx.YieldIfNeeded()
            line = self.bt.readline()

        # done, the last message is a printout that we indeed are done
        assert  line.split()[0] == 'output'
        output = self.bt.recv_data().rstrip()
        print >> self.stdout, output

        print 'done'

        # restore cwd
        os.chdir( cwd )
        
