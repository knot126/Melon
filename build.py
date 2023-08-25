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
	
	try:
		os.makedirs(d, mode = mode)
	except FileExistsError:
		print(f"\033[33mWarning: Tried to created file \"{d}\" when it already exsists.\033[0m")

def hash_data(data):
	"""
	Find the hash of the given data using a good hashing function
	"""
	
	if (type(data) == str):
		data = data.encode('utf-8')
	
	return hashlib.shake_256(data).hexdigest(20)

def hash_file(path):
	"""
	Get the hash of a file
	"""
	
	return hash_data(pathlib.Path(path).read_text())

def align_string(base, count = 8):
	"""
	Create an aligned string
	
	"7/12", count = 10 -> "7/12      " 
	"""
	
	string = [" "] * count
	
	for i in range(len(base)):
		string[i] = base[i]
	
	return "".join(string)

def main():
	if (os.name == "nt"):
		os.system("cls")
	
	# Set up the profile
	profile = sys.argv[1] if len(sys.argv) > 1 else "default"
	config = load_build_config(profile)
	
	if (config == None):
		print(f"\033[31m[No such build profile: {profile}]\033[0m")
		return 1
	
	print(f"\033[35m[Using build profile {profile}]\033[0m")
	
	# Run prebuild commands
	for cmd in config.get("prebuild", []):
		print(f"\033[35m[Run command: {cmd}]\033[m")
		os.system(cmd)
	
	# Create some dirs
	# Caching requires that the temp folder is not cleared
	create_folder("temp", mode = 0o755)
	create_folder("temp/outputs", mode = 0o755)
	
	# Enumerate files to build
	print(f"\033[36m[Enumerate and hash files]\033[m")
	
	files = []
	
	for folder in config.get("folders", ["src"]):
		filename, outline_a = list_files_in_folder(folder)
		files += filename
	
	# Enumerate the hashes of those files
	hashes = {}
	
	for filename in files:
		hashes[filename] = hash_file(filename)
	
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
	
	# Build files
	compiler = config.get("compiler", "cc")
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
			
			status = os.system(f"{compiler} -c {defines} -o {outfile} -Wall -Wextra -Wno-missing-braces -Wno-unused-parameter {infile} {include}")
			
			if (status):
				print(f"\033[31m[Failed to build \"{infile}\"]\033[0m")
				sys.exit(status)
		
		item += 1
	
	# Set up linker
	include = ""
	
	for incl in config["links"]:
		print(f"\033[36m[Adding linked library {incl}]\033[0m")
		include += f"-l{incl} "
	
	# output files
	object_files = ""
	
	for k in hashes:
		object_files += f"temp/outputs/{hashes[k]}.out "
	
	# Final binary name
	executable_ext = {"posix": ".bin", "nt": ".exe"}.get(os.name, ".executable")
	executable_name = config.get("output", "app") + executable_ext
	
	# Link!
	print(f"\033[32m[Linking binary]\033[0m")
	
	status = os.system(f"{compiler} -o {executable_name} -std=c23 {object_files} {include}")
	
	if (status):
		print(f"\033[31m[Failed to link binary]\033[0m")
	
	return status

if (__name__ == "__main__"):
	main()
