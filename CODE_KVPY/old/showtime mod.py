import e32, time

def showtime():
  print time.clock()
  e32.ao_sleep(1, showtime)  # sleep then call itself again

showtime()  # start the loop

OUTPUT IN COMPUTER

>>> reset
>>> import time,e32
>>> def showtime():
...     print time.clock()
...     e32.ao_sleep(1,showtime)
...     
>>> showtime()
263523.390625
>>> 


OUTPUT IN PHONE - 5-sep-2010
Python for S60 Version 2.0.0 svn3873
Capabilities Present: ('ReadDeviceData', 'WriteDeviceData', 'SwEvent', 'NetworkServices', 'LocalServices', 'ReadUserData', 'WriteUserData', 'Location', 'UserEnvironment')

Select:
"Options -> Run script" to run a script
"Options -> About" to view copyright

263745.703125
263746.71875
263747.734375
263748.75
263749.765625
263750.78125
263751.796875
263752.8125
263753.828125
263754.84375
263755.859375
263756.875
263757.890625
263758.90625
263759.921875
263760.9375
263761.953125
263762.96875
263763.984375
263765.0
263766.015625
263767.03125
263768.046875
263769.0625
