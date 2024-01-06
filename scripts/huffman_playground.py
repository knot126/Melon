 
def count_prob(string):
	probs = {}
	
	for c in string:
		if (c not in probs):
			probs[c] = 1
		else:
			probs[c] += 1
	
	return probs

def sort_dict(d):
	return dict(sorted(d.items(), key = lambda x: x[1], reverse = True))

class Node:
	"""
	Node in a huffman tree
	"""
	
	def __init__(self, value, prob, left = None, right = None):
		self.value = value
		self.prob = prob
		self.left = left
		self.right = right
	
	def __add__(self, other):
		"""
		Combine two huffman tree nodes
		"""
		
		return Node(None, self.prob + other.prob, self, other)
	
	def __format__(self, _unused):
		return self.stringify()
	
	def stringify(self, indent = 0):
		"""
		Format it for display
		"""
		
		tabs = indent * "| \t"
		
		s_left = ("\n" + self.left.stringify(indent + 1)) if self.left else ""
		s_right = ("\n" + self.right.stringify(indent + 1)) if self.right else ""
		
		return f"{tabs}* value = {self.value}\n{tabs}| prob = {self.prob}{s_left}{s_right}"

def probs_to_nodes(probs):
	"""
	Create the base nodes
	"""
	
	nodes = []
	
	for k, v in probs.items():
		nodes.append(Node(k, v))
	
	return nodes

def insert_node(nodes, new_node):
	"""
	Insert a node into a node array in the correct position
	"""
	
	for i in range(len(nodes)):
		if (new_node.prob >= nodes[i].prob):
			nodes.insert(0, new_node)
			break
	else:
		nodes.append(new_node)
	
	return nodes

def compute_tree(nodes):
	"""
	Compute the huffman tree given the list of base nodes
	"""
	
	while (len(nodes) > 1):
		new_node = nodes[-1] + nodes[-2]
		nodes = nodes[:-2]
		nodes = insert_node(nodes, new_node)
	
	return nodes[0]

if (__name__ == "__main__"):
	while True:
		d = input(">>> ")
		
		r = sort_dict(count_prob(d))
		
		print(r)
		
		r = compute_tree(probs_to_nodes(r))
		
		print(f"{r}")
