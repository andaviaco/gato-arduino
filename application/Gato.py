import os
import random
from boardcontroller import Board

from kivy.app import App
from kivy.uix.relativelayout import RelativeLayout
from kivy.uix.label import Label
from kivy.uix.button import Button
from kivy.core.window import Window

Window.size = (500,400)

PLAYER_1 = 'O'
PLAYER_2 = 'X'

ST_PLAY = '\x00'
ST_WON  = '\x01'
ST_DRAW = '\x02'

class MyWidget(RelativeLayout):
	def __init__(self, **kwargs):
		super(MyWidget, self).__init__(**kwargs)
		self.board = Board()

		if (self.board.connect()):
			print 'Dispositivo conectado'
		else:
			print 'No se pudo conectar al dispositivo'
		
		self.state  = ST_PLAY
		self.winner = ST_WON
		self.turn   = PLAYER_1

		self.update()

	def reactTo(self, x, y):
		if self.state == ST_PLAY:
			previous_turn = self.turn
			
			try:
				self.state, self.winner, self.turn = self.board.move(x - 1, y - 1)
			except:
				print 'Error: Verifica la conexion del dispositivo'
			else:
				btn = self.ids['matrix_' + str(x) + "_" + str(y)]
				btn.disabled = True
				
				btn.text = previous_turn

				if self.state == ST_WON:
					if self.winner == PLAYER_1:
						label = self.ids['score_1']
					else:
						label = self.ids['score_2']

					label.text = str(int(label.text) + 1)

			self.update()

	def update(self):
		text = ''

		if self.state == ST_PLAY:
			if self.turn == PLAYER_1:
				text = 'Turno: ' + PLAYER_1
			else:
				text = 'Turno: ' + PLAYER_2
		elif self.state == ST_DRAW:
			text = 'Empate!'
		elif self.state == ST_WON:
			text = 'Ganador ' + self.winner + '!'

		self.ids['turn'].text = text

	def resetGame(self):
		for x in xrange(1,4):
			btn = self.ids['matrix_' + str(x) + "_1"]
			btn.disabled = False
			btn.text = ''
			btn = self.ids['matrix_' + str(x) + "_2"]
			btn.text = ''
			btn.disabled = False
			btn = self.ids['matrix_' + str(x) + "_3"]
			btn.text = ''
			btn.disabled = False
		pass

		try:
			self.state, self.winner, self.turn = self.board.reset()
			self.update()
		except:
			print 'Error: Verifica la conexion del dispositivo'

class MyApp(App):
	title = 'Gato!'
	def build(self):
		return MyWidget()

if __name__ == '__main__':
	MyApp().run()