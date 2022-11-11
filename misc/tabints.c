DgError DgValueInt8(DgTableValue * restrict value, int8_t data) {
	/**
	 * Create a signed 8-bit integer value.
	 * 
	 * @param value Value object
	 * @param data Data value to set to
	 * @return Error code
	 */
	
	value->data.asInt8 = data;
	value->type = DG_TABLE_TYPE_INT8;
	
	return DG_ERROR_SUCCESSFUL;
}

DgError DgValueUInt8(DgTableValue * restrict value, uint8_t data) {
	/**
	 * Create an unsigned 8-bit integer value.
	 * 
	 * @param value Value object
	 * @param data Data value to set to
	 * @return Error code
	 */
	
	value->data.asUInt8 = data;
	value->type = DG_TABLE_TYPE_UINT8;
	
	return DG_ERROR_SUCCESSFUL;
}

DgError DgValueInt16(DgTableValue * restrict value, int16_t data) {
	/**
	 * Create a signed 16-bit integer value.
	 * 
	 * @param value Value object
	 * @param data Data value to set to
	 * @return Error code
	 */
	
	value->data.asInt16 = data;
	value->type = DG_TABLE_TYPE_INT16;
	
	return DG_ERROR_SUCCESSFUL;
}

DgError DgValueUInt16(DgTableValue * restrict value, uint16_t data) {
	/**
	 * Create an unsigned 16-bit integer value.
	 * 
	 * @param value Value object
	 * @param data Data value to set to
	 * @return Error code
	 */
	
	value->data.asUInt16 = data;
	value->type = DG_TABLE_TYPE_UINT16;
	
	return DG_ERROR_SUCCESSFUL;
}

DgError DgValueInt32(DgTableValue * restrict value, int32_t data) {
	/**
	 * Create a signed 32-bit integer value.
	 * 
	 * @param value Value object
	 * @param data Data value to set to
	 * @return Error code
	 */
	
	value->data.asInt32 = data;
	value->type = DG_TABLE_TYPE_INT32;
	
	return DG_ERROR_SUCCESSFUL;
}

DgError DgValueUInt32(DgTableValue * restrict value, uint32_t data) {
	/**
	 * Create an unsigned 32-bit integer value.
	 * 
	 * @param value Value object
	 * @param data Data value to set to
	 * @return Error code
	 */
	
	value->data.asUInt32 = data;
	value->type = DG_TABLE_TYPE_UINT32;
	
	return DG_ERROR_SUCCESSFUL;
}

DgError DgValueInt64(DgTableValue * restrict value, int64_t data) {
	/**
	 * Create a signed 64-bit integer value.
	 * 
	 * @param value Value object
	 * @param data Data value to set to
	 * @return Error code
	 */
	
	value->data.asInt64 = data;
	value->type = DG_TABLE_TYPE_INT64;
	
	return DG_ERROR_SUCCESSFUL;
}

DgError DgValueUInt64(DgTableValue * restrict value, uint64_t data) {
	/**
	 * Create an unsigned 64-bit integer value.
	 * 
	 * @param value Value object
	 * @param data Data value to set to
	 * @return Error code
	 */
	
	value->data.asUInt64 = data;
	value->type = DG_TABLE_TYPE_UINT64;
	
	return DG_ERROR_SUCCESSFUL;
}


