types_and_names = [
	["Bool", "bool"],
	["Int8", "int8_t"],
	["UInt8", "uint8_t"],
	["Int16", "int16_t"],
	["UInt16", "uint16_t"],
	["Int32", "int32_t"],
	["UInt32", "uint32_t"],
	["Int64", "int64_t"],
	["UInt64", "uint64_t"],
	["Float32", "float"],
	["Float64", "double"],
	["String", "const char *"],
	["StaticString", "const char *"],
	["Pointer", "void *"],
	["Array", "struct DgArray *"],
	["Table", "struct DgTable *"],
]

with open("out.c", "w") as f:
	for v in types_and_names:
		f.write(f"""DgValue DgMake{v[0]}({v[1]} data) {{
	/**
	 * Make a value of the type {v[0]}.
	 * 
	 * @param Data that the generic value will contain
	 * @return Value
	 */
	
	DgValue v;
	DgValue{v[0]}(&v, data);
	return v;
}}

""")

for v in types_and_names:
	print(f"""DgValue DgMake{v[0]}({v[1]} data);""")
