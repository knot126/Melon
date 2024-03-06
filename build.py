#!/usr/bin/python

import sys
import os, os.path
import shutil
import json
import hashlib
import pathlib

def load_build_config(profile = "default"):
	"""
	Load a build config file (controls linking, includes and may include
	compiler location information)
	"""
	
	with open("build.json", "r") as f:
		return json.load(f).get(profile, None)

def list_files_in_folder(d, ending = ".c"):
	"""
	List files in a folder
	"""
	
	lst = []
	outline = []
	
	for root, dirs, files in os.walk(d):
		root = str(root)
		
		for f in files:
			f = str(f)
			
			if (not f.endswith(ending)):
				continue
			
			lst.append(root + "/" + f)
		
		for d in dirs:
			outline.append(root + "/" + str(d))
	
	return (lst, outline)

def create_folder(d, mode = 0o755):
	"""
	Create a directory
	"""
	
	os.makedirs(d, mode = mode, exist_ok = True)

def hash_data(data):
	"""
	Find the hash of the given data using a good hashing function
	"""
	
	if (type(data) == str):
		data = data.encode('utf-8')
	
	return hashlib.blake2b(data).hexdigest()

def preprocess_file(filename, includes, seen = None):
	"""
	Preprocess file and return text
	"""
	
	if (not seen):
		seen = set()
	
	filename = filename.replace("\\", "/")
	
	# Has seen?
	if (filename in seen):
		# print(f"file already seen: {filename}")
		return ""
	
	# Exists?
	if (not os.path.exists(filename)):
		# print(f"no such file: {filename}")
		return ""
	
	# We should assert our own path too
	awuwuueu = "/".join(filename.split("/")[:-1])
	if (awuwuueu not in includes):
		includes.append(awuwuueu)
	
	data = ""
	
	f = open(filename, "r")
	
	while (True):
		line = f.readline()
		
		if (not line):
			break
		
		s = line.split()
		
		match (s[0] if len(s) >= 1 else "\n"):
			case "#include":
				basepath = s[1][1:-1]
				
				# print(f"process include: {basepath}")
				
				for cand in includes:
					got = preprocess_file(cand + "/" + basepath, includes, seen)
					data += got
					if (got):
						# print(f"got include: {basepath}")
						break
			
			case "#pragma":
				if (s[1] == "once"):
					seen.add(filename)
			
			case _:
				data += line
	
	return data

def hash_preprocessed_file(filename, includes):
	"""
	hash of a preprocessed file
	"""
	
	return hash_data(preprocess_file(filename, includes))

def hash_command_output(cmd):
	"""
	Use os.popen to read the output of a command and then hash it
	"""
	
	s = os.popen(cmd)
	d = s.read()
	h = hash_data(d)
	s.close()
	
	return h

def align_string(base, count = 8):
	"""
	Create an aligned string
	
	"7/12", count = 10 -> "7/12      " 
	"""
	
	string = [" "] * count
	
	for i in range(len(base)):
		string[i] = base[i]
	
	return "".join(string)

def which_cc():
	"""
	Get the best available c compiler name
	"""
	
	cc_names = ["clang", "gcc", "tcc", "cc"]
	
	for cc in cc_names:
		path = shutil.which(cc)
		
		if (path):
			return cc
	
	return None

def main():
	if (sys.platform == "win32"):
		os.system("cls")
	
	# Set up the profile
	profile = sys.argv[1] if len(sys.argv) > 1 else sys.platform
	config = load_build_config(profile)
	
	if (config == None):
		print(f"\033[31m[No such build profile: {profile}]\033[0m")
		return 1
	
	print(f"\033[35m[Using build profile {profile}]\033[0m")
	
	# Run prebuild commands
	for cmd in config.get("prebuild", []):
		print(f"\033[35m[Run command: {cmd}]\033[m")
		os.system(cmd)
	
	# Create the dirs we need if they don't exist
	create_folder("temp", mode = 0o755)
	create_folder("temp/outputs", mode = 0o755)
	
	# I don't know where else to put this stuff (includes and defines) so im
	# just putting them here.
	
	# Set up include dirs
	include = ""
	
	for incl in config["includes"]:
		include += f"-I\"{incl}\" "
	
	print(f"\033[35m[Includes: {include}]\033[0m")
	
	# Set up defines
	defines = ""
	
	for incl in config.get("defines", []):
		defines += f"-D{incl} "
	
	print(f"\033[35m[Defines: {defines}]\033[0m")
	
	# Get compiler info (TODO: hope we can make this somewhat automatic soon)
	compiler = config.get("compiler", which_cc())
	
	print(f"\033[35m[C compiler: {compiler}]\033[0m")
	
	# Enumerate files to build
	print(f"\033[36m[Enumerate and hash files]\033[m")
	
	files = []
	
	for folder in config.get("sources", ["source"]):
		filenames, outline = list_files_in_folder(folder)
		files += filenames
	
	# Enumerate the hashes of those files
	# ===================================
	# A clever trick we can use to make sure we don't have to deal with "what if
	# the header file changes" crap: just preprocess the file *before* taking the
	# hash. If the header changes, that will be reflected by a different file
	# hash!
	hashes = {}
	
	if (os.name != "nt"):
		for filename in files:
			print(f"\033[36m[Process file: \"{filename}\"]\033[m")
			hashes[filename] = hash_command_output(f"{compiler} -E -Wno-c2x-extensions {filename} {defines} {include}")
	else:
		# NT has slow Popen so fuck that
		for filename in files:
			print(f"\033[36m[Process file: \"{filename}\"]\033[m")
			hashes[filename] = hash_preprocessed_file(filename, config["includes"])
	
	# Get if the output object should be shared
	shared = config.get("shared", False)
	
	# Build changed files
	print(f"\033[36m[Build items]\033[m")
	
	item = 1
	
	for k in hashes:
		progress = align_string(
			f"{item}/{len(hashes)}",
			count = 2 * len(str(len(hashes))) + 1
		)
		
		infile = k
		outfile = f"temp/outputs/{hashes[k]}.out"
		
		# If the output is already built, then we skip it
		if (os.path.exists(outfile)):
			print(f"\033[36m[{progress} Skipping item: \"{infile}\"]\033[0m")
		# Otherwise, we need to build it!
		else:
			print(f"\033[36m[{progress} Building item: \"{infile}\"]\033[0m")
			
			status = os.system(f"{compiler} -c {defines} -o {outfile} {'-fpic' if shared else ''} -Wall -Wextra -Wno-missing-braces -Wno-unused-parameter {infile} {include}")
			
			if (status):
				print(f"\033[31m[Failed to build \"{infile}\"]\033[0m")
				sys.exit(status)
		
		item += 1
	
	# Set up linker
	include = ""
	
	for incl in config.get("links", []):
		# print(f"\033[36m[Adding linked library {incl}]\033[0m")
		include += f"-l{incl} "
	
	# output files
	object_files = ""
	
	for k in hashes:
		object_files += f"temp/outputs/{hashes[k]}.out "
	
	# Final binary name
	executable_name = config.get("output", "app")
	
	# Link!
	print(f"\033[32m[Linking binary]\033[0m")
	
	link_cmd = f"{compiler} -o temp/{executable_name} -std=c23 {'-shared' if shared else ''} -rdynamic {object_files} {include}"
	status = os.system(link_cmd)
	
	if (status):
		print(f"\033[31m[Failed to link binary]\033[0m")
	
	# Save to rune files
	# Disabled since it doesn't work right now
	# rune_count = len(list_files_in_folder("temp/runes")[0])
	# pathlib.Path(f"temp/runes/{executable_name}{rune_count}.rune").write_text(link_cmd)
	
	return status

if (__name__ == "__main__"):
	main()
