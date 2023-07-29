import secrets

avail = [x for x in range(256)]

for j in range(256):
	val = secrets.choice(avail)
	
	avail.remove(val)
	
	print("{:02X}".format(val), end="")
	
	if (len(avail) % 16 == 0):
		print()
	else:
		print(" ", end="")
