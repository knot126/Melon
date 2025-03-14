#!/usr/bin/env python3
import re

def get_lines_from_source(f):
	prev = ""
	comment = False
	lines = []
	
	for line in f.readlines():
		if "/*" in line:
			comment = True
			
			if "{" in prev:
				lines.append("<h3><ccode>" + prev.strip("{\t\r\n ") + "</ccode></h3>")
			else:
				comment = False
				continue
			
			lines.append(line.partition("/*")[2].strip("*\t\r\n "))
		elif "*/" in line:
			lines.append(line.partition("*/")[0].strip("*\t\r\n "))
			comment = False
		elif comment:
			lines.append(line.strip("*\t\r\n "))
		
		prev = line
	
	return lines

def format_codedoc(lines):
	class LineReader:
		def __init__(self, lines):
			self.lines = lines
		
		def consume(self):
			return self.lines.pop(0) if len(self.lines) > 0 else None
		
		def peek(self):
			return self.lines[0] if len(self.lines) > 0 else None
		
		def more(self):
			return len(self.lines) != 0
	
	lines = LineReader(lines)
	output = ""
	
	while lines.more():
		l = lines.peek()
		
		if l.startswith(("<", "@")):
			if l.startswith("@"):
				typ = l.split()[0][1:]
				match typ:
					case "param":
						output += "<ul>"
						while lines.peek().startswith("@param "):
							param = lines.consume()[7:]
							name = param.split()[0]
							desc = param[len(name)+1:]
							output += f'<li><strong><code>{name}</code></strong>: {desc}</li>'
						output += "</ul>"
					
					case _:
						output += f'<div class="{typ}"><strong>{typ.title()}:</strong> {l.partition(" ")[2]}</div>'
						lines.consume()
			else:
				output += lines.consume()
		elif l == "":
			lines.consume()
		else:
			p = []
			
			while lines.more() and not lines.peek().startswith(("<", "@")) and lines.peek() != "":
				p += [lines.consume()]
			
			output += f"<p>{' '.join(p)}</p>"
	
	return output

if __name__ == "__main__":
	import sys
	
	with open(sys.argv[1], "r") as f:
		print(format_codedoc(get_lines_from_source(f)))
