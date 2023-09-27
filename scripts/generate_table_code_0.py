
bits = [8, 16, 32, 64]

for a in bits:
	A = f"""DgError DgValueInt{a}(DgTableValue * restrict value, int{a}_t data) {{
	/**
	 * Create a signed {a}-bit integer value.
	 * 
	 * @param value Value object
	 * @param data Data value to set to
	 * @return Error code
	 */
	
	value->data.asInt{a} = data;
	value->type = DG_TABLE_TYPE_INT{a};
	
	return DG_ERROR_SUCCESSFUL;
}}

DgError DgValueUInt{a}(DgTableValue * restrict value, uint{a}_t data) {{
	/**
	 * Create an unsigned {a}-bit integer value.
	 * 
	 * @param value Value object
	 * @param data Data value to set to
	 * @return Error code
	 */
	
	value->data.asUInt{a} = data;
	value->type = DG_TABLE_TYPE_UINT{a};
	
	return DG_ERROR_SUCCESSFUL;
}}

"""
	
	print(A)
