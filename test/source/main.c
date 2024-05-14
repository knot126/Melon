#include "util/melon.h"
#include "util/storage_void.h"
#include "util/storage_filesystem.h"

#ifndef MELON_CRYPTOGRAPHY_RANDOM
#error MELON_CRYPTOGRAPHY_RANDOM not defined
#endif

DgError DgCryptoCubeHasher_Test(void);
void DgCryptoCubeHashBytes_Test(void);

void TestString(void) {
	DgLog(DG_LOG_INFO, "TestString()");
	
	// Find function
	DgLog(DG_LOG_INFO, "%d", DgStringFind("fs://games/save.xml", "://", 0));
	DgLog(DG_LOG_INFO, "%d", DgStringFind("fs://games/save.xml, user://myfile.txt", "://", 1));
	DgLog(DG_LOG_INFO, "%d", DgStringFind("WONT FIND", "THIS", 0));
	DgLog(DG_LOG_INFO, "%d", DgStringFind("ABCDBEBBDBBEABBEABABROOEOKKA", "AB", 3));
	DgLog(DG_LOG_INFO, "%d", DgStringFind("", "asdf", 69));
	
	// Duplicate string up to
	DgLog(DG_LOG_INFO, "Up to 13 chars: %s", DgStringDuplicateUntil("Test string that is very long string!", 13));
	DgLog(DG_LOG_INFO, "Up to 13 chars: %s", DgStringDuplicateUntil("Hewwo! :3", 13));
	
	// Encoding functions
	char *a;
	
	a = DgStringEncodeBase64(13, "Hello, world!");
	DgLog(DG_LOG_INFO, "Base64 test 1: %s", a);
	DgFree(a);
	
	a = DgStringEncodeBase64(14, "Hello, world!!");
	DgLog(DG_LOG_INFO, "Base64 test 2: %s", a);
	DgFree(a);
	
	a = DgStringEncodeBase16(13, "Hello, world!");
	DgLog(DG_LOG_INFO, "Base16 test 1: %s", a);
	DgFree(a);
	
	a = DgStringEncodeBase16(14, "Hello, world!!");
	DgLog(DG_LOG_INFO, "Base16 test 2: %s", a);
	DgFree(a);
	
	DgLog(DG_LOG_SUCCESS, "TestString()");
}

void TestStorage(void) {
	DgLog(DG_LOG_INFO, "TestStorage()");
	
	char *proto, *file;
	
	// TEST 1
	DgLog(DG_LOG_INFO, "Testing path splitting function...");
	
	DgStorageSplitPathIntoParts("assets://stage/canyon.xml", &proto, &file);
	
	DgLog(DG_LOG_INFO, "Path: %s %s", proto, file);
	
	DgFree(proto);
	DgFree(file);
	
	DgStorageSplitPathIntoParts("fs://what/the/shit.fuck", &proto, NULL);
	
	DgLog(DG_LOG_INFO, "Path: %s", proto);
	
	DgFree(proto);
	
	DgLog(DG_LOG_SUCCESS, "TestStorage() - 1");
	
	// TEST 2
	DgLog(DG_LOG_INFO, "Adding a void pool and testing it...");
	
	DgLog(DG_LOG_INFO, "%x", DgStorageAddPool(NULL, DgVoidCreatePool("void")));
	
	DgLog(DG_LOG_INFO, "%x", DgStorageDelete(NULL, "void://help.txt"));
	
	DgStream s;
	
	if (DgStreamOpen(NULL, &s, "void://help.txt", DG_STREAM_READ | DG_STREAM_WRITE)) {
		DgLog(DG_LOG_INFO, "Failed to open file stream!");
		return;
	}
	
	DgStreamRead(&s, 0, NULL);
	DgStreamWrite(&s, 0, NULL);
	
	DgStreamClose(&s);
	
	DgLog(DG_LOG_SUCCESS, "TestStorage() - 2");
	
	// TEST 3
	DgLog(DG_LOG_INFO, "Real filesystem test...");
	
	DgLogError(DgStorageAddPool(NULL, DgFilesystemCreatePool("fs", ".")));
	
	char sample[] = "This is my lovely document! It's very nice!\n\n";
	
	DgFileAppend(NULL, "fs://hello.txt", sizeof(sample) - 1, sample);
	
	DgLog(DG_LOG_SUCCESS, "TestStorage() - 3");
}

void TestCryptoRandom(void) {
	char rand_bytes[16];
	
	for (size_t i = 0; i < 5; i++) {
		DgError error = DgRandom(16, rand_bytes);
		
		if (error) {
			DgLog(DG_LOG_ERROR, "DgRandom: %08x", error);
			return;
		}
		
		char *asBase16 = DgStringEncodeBase16(16, rand_bytes);
		DgLog(DG_LOG_INFO, "Secure random bytes as base16: %s", asBase16);
		DgFree(asBase16);
	}
	
	DgLog(DG_LOG_SUCCESS, "TestCryptoRandom");
}

void TestArray(void) {
	DgError error;
	
	DgLog(DG_LOG_INFO, "TestArray()");
	DgValue one = DgMakeInt64(1234);
	
	DgArray array;
	DgArrayInit(&array);
	DgArrayResize(&array, 20);
	DgArrayPut(&array, 3, &one);
	DgArrayResize(&array, 30);
	DgArrayPut(&array, 25, &one);
	DgArrayResize(&array, 40);
	DgArrayPut(&array, 35, &one);
	DgArrayResize(&array, 100);
	DgArrayPut(&array, 97, &one);
	DgArrayFree(&array, true);
	
	DgLog(DG_LOG_SUCCESS, "TestArray()");
}

void TestTerminal(void) {
	while (true) {
	const char *line = DgReadLine(">> ");
	
	if (line) {
		DgLog(DG_LOG_INFO, "Read a line: %s", line);
	}
	else {
		DgLog(DG_LOG_ERROR, "Read line failed! FUCK!");
	}
	
	DgMemoryFree(line);
	}
}

void TestTableAndSerialise(void) {
	DgError err;
	
	DgLog(DG_LOG_INFO, "TestTableAndSerialise()");
	
	DgTable table;
	
	DgLog(DG_LOG_INFO, "Initialise table");
	
	err = DgTableInit(&table);
	
	if (err) {
		DgLog(DG_LOG_ERROR, "%d", err);
	}
	
	DgValue key, value;
	
	DgLog(DG_LOG_INFO, "Value 1");
	DgValueStaticString(&key, "int64_test");
	DgValueInt64(&value, 324776765645);
	DgTablePut(&table, &key, &value);
	
	DgLog(DG_LOG_INFO, "Value 2");
	DgValueStaticString(&key, "uint32_test");
	DgValueUInt32(&value, 5001811);
	DgTablePut(&table, &key, &value);
	
	DgLog(DG_LOG_INFO, "Value 3");
	DgValueStaticString(&key, "float32_test");
	DgValueFloat32(&value, 1.5);
	DgTablePut(&table, &key, &value);
	
	DgValueStaticString(&key, "float32_test");
	DgValueFloat32(&value, 1.0);
	DgTablePut(&table, &key, &value);
	
	DgLog(DG_LOG_INFO, "Value 4");
	DgValueStaticString(&key, "nil_test");
	DgValueNil(&value);
	DgTablePut(&table, &key, &value);
	
	DgLog(DG_LOG_INFO, "Value 5");
	DgValueStaticString(&key, "ptr_test");
	DgValuePointer(&value, &table);
	DgTablePut(&table, &key, &value);
	
	DgLog(DG_LOG_INFO, "Value 6");
	DgValueStaticString(&key, "array_test");
	
	DgArray array;
	DgArrayInit(&array);
	DgArrayResize(&array, 5);
	DgValueArray(&value, &array);
	DgTablePut(&table, &key, &value);
	
	DgLog(DG_LOG_INFO, "Value 7");
	DgTable anTable;
	DgTableInit(&anTable);
	DgValueStaticString(&key, "subtable_test");
	DgValueTable(&value, &anTable);
	DgTablePut(&table, &key, &value);
	
	key = DgMakeStaticString("example");
	value = DgMakeInt64(123456);
	DgTablePut(&anTable, &key, &value);
	
	key = DgMakeStaticString("type");
	value = DgMakeStaticString("number");
	DgTablePut(&anTable, &key, &value);
	
	DgLog(DG_LOG_INFO, "Make table value for serialise test");
	DgValue table_val;
	DgValueTable(&table_val, &table);
	
	DgLog(DG_LOG_INFO, "Serialise");
	if ((err = DgSerialiseWrite(NULL, "fs://ser_test.dat", &table_val))) {
		DgLogError(err);
	}
	
	DgLog(DG_LOG_INFO, "Free the table");
	DgValueFree(&table_val);
}

int main(const int argc, const char *argv[]) {
	DgLog(DG_LOG_INFO, "Hello, world!");
	
	// TestString();
	TestStorage();
	// TestCryptoRandom();
	TestArray();
	TestTerminal();
	TestTableAndSerialise();
	// DgCryptoCubeHasher_Test();
	// DgCryptoCubeHashBytes_Test();
	
	return 0;
}
