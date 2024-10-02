class State:
	def __init__(self, name, accepting = False):
		self.name = name
		self.accepting = accepting

class Transition:
	def __init__(self, fro, to, symbol = None):
		self.fro = fro
		self.to = to
		self.symbol = symbol

class NFA:
	def __init__(self):
		self.states = []
		self.transitions = []
	
	def create_states(self, accept_map):
		for i in range(len(accept_map)):
			self.states.append(State(i, accept_map[i]))
	
	def add_transition(self, fro, to, symbol = None):
		self.transitions.append(Transition(fro, to, symbol))
	
	def eclosure(self, states, symbol):
		results = list(states)
		marks = [False] * len(results)
		
		for trans in self.transitions:
			if (trans.to not in results) and (trans.fro in states) and (self.symbol == symbol):
				results.append(trans.to)
				marks.append(False)
		
		return set(results)
