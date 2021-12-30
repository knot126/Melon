#!/usr/bin/python

import glob

NAME = "util.h"

with open(NAME, "w") as f:
	d = glob.glob("*.h")
	
	f.write(
"""/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * NOTE: This is an auto-generated file. Please re-generate with "./zgen.py"
 */

#pragma once

"""
		)
	for h in d:
		if (d == NAME): continue
		f.write(f"#include \"{h}\"\n")
	
	f.close()
