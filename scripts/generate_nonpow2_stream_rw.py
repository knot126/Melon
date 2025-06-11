#!/usr/bin/env python3

from math import log, ceil

def generateFor(bits):
	minbits = 2 ** ceil(log(bits, 2))
	minbytes = minbits // 8
	
	rfunc = f"""uint{minbits}_t DgStreamReadUInt{bits}(DgStream *this, DgError *error) {{
	/**
	 * Read an {bits}-bit unsigned integer from the stream
	 */
	
	uint8_t data[{bits // 8}];
	
	DgError status = DgStreamRead(this, {bits // 8}, &data);
	
	if (status) {{
		if (error) {{
			*error = status;
		}}
		
		return 0;
	}}
	
	if (DgStreamGetEndian(this) == DG_ENDIAN_LITTLE) {{
		return {' | '.join([f'(data[{i}] << {8 * i})' for i in range(bits // 8)])};
	}}
	else {{
		return {' | '.join([f'(data[{(bits // 8) - (i + 1)}] << {bits - 8 * (i + 1)})' for i in range(bits // 8)])};
	}}
}}"""
	
	wfunc = f"""DgError DgStreamWriteUInt{bits}(DgStream *this, uint{minbits}_t value) {{
	/**
	 * Write a {bits}-bit integer to the stream
	 */
	
	uint8_t data[{bits // 8}];
	
	if (DgStreamGetEndian(this) == DG_ENDIAN_LITTLE) {{
		data = {{{', '.join([f'(value >> {8 * i})' for i in range(bits // 8)])}}};
	}}
	else {{
		data = {{{', '.join([f'(value >> {bits - 8 * (i + 1)})' for i in range(bits // 8)])}}};
	}}
	
	return DgStreamWrite(this, {bits // 8}, &data);
}}"""
	
	return rfunc + "\n\n" + wfunc + "\n\n"

def main():
	s = ""
	s += generateFor(24)
	s += generateFor(40)
	s += generateFor(48)
	s += generateFor(56)
	print(s)

if __name__ == "__main__":
	main()
