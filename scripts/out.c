DgValue DgMakeBool(bool data) {
	/**
	 * Make a value of the type Bool.
	 * 
	 * @param Data that the generic value will contain
	 * @return Value
	 */
	
	DgValue v;
	DgValueBool(&v, data);
	return v;
}

DgValue DgMakeInt8(int8_t data) {
	/**
	 * Make a value of the type Int8.
	 * 
	 * @param Data that the generic value will contain
	 * @return Value
	 */
	
	DgValue v;
	DgValueInt8(&v, data);
	return v;
}

DgValue DgMakeUInt8(uint8_t data) {
	/**
	 * Make a value of the type UInt8.
	 * 
	 * @param Data that the generic value will contain
	 * @return Value
	 */
	
	DgValue v;
	DgValueUInt8(&v, data);
	return v;
}

DgValue DgMakeInt16(int16_t data) {
	/**
	 * Make a value of the type Int16.
	 * 
	 * @param Data that the generic value will contain
	 * @return Value
	 */
	
	DgValue v;
	DgValueInt16(&v, data);
	return v;
}

DgValue DgMakeUInt16(uint16_t data) {
	/**
	 * Make a value of the type UInt16.
	 * 
	 * @param Data that the generic value will contain
	 * @return Value
	 */
	
	DgValue v;
	DgValueUInt16(&v, data);
	return v;
}

DgValue DgMakeInt32(int32_t data) {
	/**
	 * Make a value of the type Int32.
	 * 
	 * @param Data that the generic value will contain
	 * @return Value
	 */
	
	DgValue v;
	DgValueInt32(&v, data);
	return v;
}

DgValue DgMakeUInt32(uint32_t data) {
	/**
	 * Make a value of the type UInt32.
	 * 
	 * @param Data that the generic value will contain
	 * @return Value
	 */
	
	DgValue v;
	DgValueUInt32(&v, data);
	return v;
}

DgValue DgMakeInt64(int64_t data) {
	/**
	 * Make a value of the type Int64.
	 * 
	 * @param Data that the generic value will contain
	 * @return Value
	 */
	
	DgValue v;
	DgValueInt64(&v, data);
	return v;
}

DgValue DgMakeUInt64(uint64_t data) {
	/**
	 * Make a value of the type UInt64.
	 * 
	 * @param Data that the generic value will contain
	 * @return Value
	 */
	
	DgValue v;
	DgValueUInt64(&v, data);
	return v;
}

DgValue DgMakeFloat32(float data) {
	/**
	 * Make a value of the type Float32.
	 * 
	 * @param Data that the generic value will contain
	 * @return Value
	 */
	
	DgValue v;
	DgValueFloat32(&v, data);
	return v;
}

DgValue DgMakeFloat64(double data) {
	/**
	 * Make a value of the type Float64.
	 * 
	 * @param Data that the generic value will contain
	 * @return Value
	 */
	
	DgValue v;
	DgValueFloat64(&v, data);
	return v;
}

DgValue DgMakeString(const char * data) {
	/**
	 * Make a value of the type String.
	 * 
	 * @param Data that the generic value will contain
	 * @return Value
	 */
	
	DgValue v;
	DgValueString(&v, data);
	return v;
}

DgValue DgMakeStaticString(const char * data) {
	/**
	 * Make a value of the type StaticString.
	 * 
	 * @param Data that the generic value will contain
	 * @return Value
	 */
	
	DgValue v;
	DgValueStaticString(&v, data);
	return v;
}

DgValue DgMakePointer(void * data) {
	/**
	 * Make a value of the type Pointer.
	 * 
	 * @param Data that the generic value will contain
	 * @return Value
	 */
	
	DgValue v;
	DgValuePointer(&v, data);
	return v;
}

DgValue DgMakeArray(struct DgArray * data) {
	/**
	 * Make a value of the type Array.
	 * 
	 * @param Data that the generic value will contain
	 * @return Value
	 */
	
	DgValue v;
	DgValueArray(&v, data);
	return v;
}

DgValue DgMakeTable(struct DgTable * data) {
	/**
	 * Make a value of the type Table.
	 * 
	 * @param Data that the generic value will contain
	 * @return Value
	 */
	
	DgValue v;
	DgValueTable(&v, data);
	return v;
}

