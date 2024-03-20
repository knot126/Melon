types_and_names = [
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
]

for v in types_and_names:
	print(f"""\tcase DG_TYPE_{v[0].upper()}: {{ return (value1->as{v[0]} == value2->as{v[0]}); }}""")
