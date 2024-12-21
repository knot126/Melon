#!/usr/bin/env python3
import os
from pathlib import Path

NOINCLUDE = {"crypto_hash.h", "crypto_hash_cubehash.h", "crypto_random.h",
	"crypto_symmetric.h", "storage_filesystem.h", "string_pattern.h"}

print("Generating melon.h header...")

files = sorted(os.listdir("./source"))

t = "/**\n * Auto-generated header for including all other Melon headers\n */\n\n#pragma once\n\n"

for f in files:
	if f.endswith(".h") and f not in NOINCLUDE:
		t += f'#include "{f}"\n'

Path("./source/melon.h").write_text(t)
