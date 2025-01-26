"""
hacky script
"""

import os
from pathlib import Path

OLD = """/**
 * Melon Software Framework is Copyright (C) 2021 - 2024 Knot126
 * 
 * ============================================================================="""
NEW = """/**
 * Melon Software Framework is Copyright (C) 2021 - 2025 Knot126
 * 
 * ============================================================================="""

list_dir = lambda path: [f for f in os.listdir(path) if os.path.isfile(os.path.join(path, f))]

files = list_dir(".")

for f in files:
	print(f"update '{f}'")
	content = Path(f).read_text()
	content = content.replace(OLD, NEW)
	Path(f).write_text(content)
