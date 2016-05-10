import serial
import struct


port = serial.Serial("/dev/ttyUSB0", 38400)

# sync up with ST (art) bytes
# the format is STZzXxYy where big X Y Z are the high bytes
#and x y z are the low bytes

axes = ['Z','X','Y']

while(port.readline()!="ST"):
	pass

print("sync")

while (1):
	bytes = port.readline()
	print("Z"),
	print(str(word).rjust(10)),

	bytes = port.readline()
	print("X"),
	print(str(word).rjust(10)),

	bytes = port.readline()
	print("Y"),
	print(str(word).rjust(10)),

	print("\b"*14*3),