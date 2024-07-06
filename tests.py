import struct
import json
import pathlib
import os

BUILD_JSON = "build.json"
BUILD_DIR = "temp"
INVOKE_PATH = "tools/invoke"

### FILE STREAM ###
class FileStream():
	"""
	Nice way to read binary files
	"""
	
	def __init__(self, path, mode = "r+b"):
		self.f = open(path, mode)
		self.endian = "<"
	
	def close(self):
		self.f.close()
	
	def read(self, n):
		"""Read n bytes"""
		
		return self.f.read(n)
	
	def write(self, b):
		"""Write bytes"""
		
		self.f.write(b)
	
	def getpos(self):
		return self.f.tell()
	
	def setpos(self, pos):
		self.f.seek(pos, 0)
	
	def skip(self, count):
		self.f.seek(count, 1)
	
	def readFrom(self, pos, count):
		self.setpos(pos)
		return self.read(count)
	
	def readFormatted(self, fmt):
		fmt = f"{self.endian}{fmt}"
		return struct.unpack(fmt, self.read(struct.calcsize(fmt)))[0]
	
	def writeFormatted(self, fmt, val):
		fmt = f"{self.endian}{fmt}"
		self.write(struct.pack(fmt, val))
	
	def readInt8(self): return self.readFormatted("b")
	def readUInt8(self): return self.readFormatted("B")
	def readInt16(self): return self.readFormatted("h")
	def readUInt16(self): return self.readFormatted("H")
	def readInt32(self): return self.readFormatted("i")
	def readUInt32(self): return self.readFormatted("I")
	def readInt64(self): return self.readFormatted("q")
	def readUInt64(self): return self.readFormatted("Q")
	
	def readString(self):
		s = b""
		
		while (True):
			c = self.read(1)
			
			if (c == b"\x00"):
				break
			
			s += c
		
		return s.decode("latin-1")
	
	def writeInt8(self, val): self.writeFormatted("b", val)
	def writeUInt8(self, val): self.writeFormatted("B", val)
	def writeInt16(self, val): self.writeFormatted("h", val)
	def writeUInt16(self, val): self.writeFormatted("H", val)
	def writeInt32(self, val): self.writeFormatted("i", val)
	def writeUInt32(self, val): self.writeFormatted("I", val)
### FILE STREAM ###

class Object(): pass

def find_binary():
	"""
	Find the binary to use for tests
	"""
	
	build_info = json.loads(pathlib.Path(BUILD_JSON).read_text())
	return f"{BUILD_DIR}/" + build_info.get("linux").get("output", "app")

def read_symbols(path):
	"""
	Read symbols from the output file
	"""
	
	fs = FileStream(path, "rb")
	
	if (fs.read(4) != b"\x7fELF"):
		raise ValueError("Not an ELF file")
	
	if (fs.readUInt8() != 2):
		raise ValueError("32-bit ELF is not supported")
	
	if (fs.readUInt8() != 1):
		raise ValueError("Wrong type of snow.. I mean endianness")
	
	if (fs.readUInt8() != 1):
		raise ValueError("Wrong ELF version, you must live in the future!")
	
	fs.readUInt8() # abi
	fs.readUInt8() # abi
	fs.read(7) # pad
	
	e_type = fs.readUInt16()
	e_machine = fs.readUInt16()
	e_version = fs.readUInt32()
	e_entry = fs.readUInt64()
	e_phoff = fs.readUInt64()
	e_shoff = fs.readUInt64()
	e_flags = fs.readUInt32()
	e_ehsize = fs.readUInt16()
	e_phentsize = fs.readUInt16()
	e_phnum = fs.readUInt16()
	e_shentsize = fs.readUInt16()
	e_shnum = fs.readUInt16()
	e_shstrndx = fs.readUInt16()
	
	# Read program headers
	fs.setpos(e_phoff)
	
	phents = []
	
	for i in range(e_phnum):
		p = Object()
		p.type = fs.readUInt32()
		p.flags = fs.readUInt32()
		p.offset = fs.readUInt64()
		p.vaddr = fs.readUInt64()
		p.paddr = fs.readUInt64()
		p.filesz = fs.readUInt64()
		p.memsz = fs.readUInt64()
		p.align = fs.readUInt64()
		phents.append(p)
	
	# Find the dynamic one, that's what we want
	dynamic = None
	
	for ph in phents:
		if (ph.type == 0x2):
			if dynamic:
				raise ValueError("More than one PT_DYNAMIC!!!")
			
			dynamic = ph
	
	if not dynamic:
		raise ValueError("Could not find dynamic linker info")
	
	# Read the dynamic section
	fs.setpos(dynamic.offset)
	
	dyns = []
	
	while (True):
		dyn = Object()
		dyn.tag = fs.readInt64()
		dyn.val = fs.readUInt64()
		dyns.append(dyn)
		
		if (dyn.tag == 0):
			break
	
	strtab_offset = 0
	strtab_size = 0
	symtab_offset = 0
	symtab_count = 0
	
	for dyn in dyns:
		match dyn.tag:
			case 5: strtab_offset = dyn.val
			case 6: symtab_offset = dyn.val
			case 10: strtab_size = dyn.val
			case 4:
				fs.setpos(dyn.val)
				fs.readUInt32() # nbucket (ignored)
				symtab_count = fs.readUInt32() # nchain = number of symbol entries
			case 0x6ffffef5:
				# GNU's bullshit, because GNU had to be different than everyone
				# else.
				fs.setpos(dyn.val)
				nbuckets = fs.readUInt32()
				symoffset = fs.readUInt32()
				bloom_size = fs.readUInt32()
				bloom_shift = fs.readUInt32()
				fs.skip(0x8 * bloom_size)
				
				highest_chain_index = 0
				
				for i in range(nbuckets):
					highest_chain_index = max(highest_chain_index, fs.readUInt32())
				
				fs.skip(highest_chain_index)
				
				while True:
					highest_chain_index += 1
					
					s = fs.readUInt32()
					
					if (s & 1):
						break
				
				symtab_count = highest_chain_index
			case _:
				pass
	
	fs.setpos(symtab_offset)
	
	symbols = []
	
	for i in range(symtab_count):
		sym = Object()
		sym.name = fs.readUInt32()
		sym.info = fs.readUInt8()
		sym.other = fs.readUInt8()
		sym.shndx = fs.readUInt16()
		sym.value = fs.readUInt64()
		sym.size = fs.readUInt64()
		symbols.append(sym)
	
	symbol_names = []
	
	# parse out symbol names!
	for sym in symbols:
		if ((sym.info & 0xf) == 2):
			fs.setpos(strtab_offset + sym.name)
			sym.name = fs.readString()
			symbol_names.append(sym.name)
	
	return symbol_names

def filter_test_symbols(allsyms):
	testsyms = []
	
	for s in allsyms:
		if (s.startswith("Dg") and s.endswith("_Test")):
			testsyms.append(s)
	
	return testsyms

def run_test(binary_path, function_name):
	print(f"\x1b[1;38;5;69mRun test: {function_name}\x1b[0m")
	
	result = os.system(f"{INVOKE_PATH} {binary_path} {function_name}")
	
	if (not result):
		print(f"\x1b[1;38;5;34mPASS\x1b[0m")
	else:
		print(f"\x1b[1;38;5;161mFAIL\x1b[0m")

def main():
	binary_path = find_binary()
	
	test_symbols = filter_test_symbols(read_symbols(binary_path))
	
	if (len(test_symbols) == 0):
		print("No tests! Make sure you compiled with -rdynamic. Or maybe implement a better symbol table parser.")
	
	for symbol in test_symbols:
		run_test(binary_path, symbol)

if (__name__ == "__main__"):
	main()
