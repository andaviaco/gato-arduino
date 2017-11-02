import serial
from serial.tools import list_ports
from serial.serialutil import SerialTimeoutException
import time

MOVE  = 1
RESET = 0

class Board:
	def __init__(self):
		self.conection = None

	def connect(self, block = False):
		while True:
			try:
				port = list_ports.grep('arduino').next()
				self.conection = serial.Serial(port[0])
			except:
				if block:
					time.sleep(0.5)
				else:
					return False
			else:
				return True

	def response(self):
		data = self.conection.read(3)

		return data[0], data[1], data[2]

	def move(self, row, col):
		try:
			self.conection.write(bytearray([MOVE, row, col]))
		except SerialTimeoutException:
			print 'Tablero desconectado'
			raise

		return self.response()

	def reset(self):
		self.conection.write(bytearray([RESET, 0, 0]))

		return self.response()
