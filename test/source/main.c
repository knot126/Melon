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
	DgMemoryFree(a);
	
	a = DgStringEncodeBase64(14, "Hello, world!!");
	DgLog(DG_LOG_INFO, "Base64 test 2: %s", a);
	DgMemoryFree(a);
	
	a = DgStringEncodeBase16(13, "Hello, world!");
	DgLog(DG_LOG_INFO, "Base16 test 1: %s", a);
	DgMemoryFree(a);
	
	a = DgStringEncodeBase16(14, "Hello, world!!");
	DgLog(DG_LOG_INFO, "Base16 test 2: %s", a);
	DgMemoryFree(a);
	
	DgLog(DG_LOG_SUCCESS, "TestString()");
}

void TestStorage(void) {
	DgLog(DG_LOG_INFO, "TestStorage()");
	
	// TEST 1
	DgLog(DG_LOG_INFO, "Testing path splitting function...");
	
	///
	const char *paths[] = {
		"assets://test1.txt",
		"assets://test/test2.txt",
		"assets://levels/canyon/canyon.lua",
		"assets://",
		"fs://other/test.bmp",
		"://blank/proto/test.nif",
		"://",
		"shouldHave/aNullProtocol.txt",
		NULL,
	};
	
	for (size_t i = 0; paths[i]; i++) {
		// Nither is NULL
		char *proto, *file;
		
		DgStorageSplitPathIntoParts(paths[i], &proto, &file);
		DgLog(DG_LOG_VERBOSE, "'%s' => '%s' '%s'", paths[i], proto ? proto : "<null>", file ? file : "<null>");
		
		DgMemoryFree(proto);
		DgMemoryFree(file);
		
		// File is NULL
		file = NULL; proto = NULL;
		
		DgStorageSplitPathIntoParts(paths[i], &proto, NULL);
		DgLog(DG_LOG_VERBOSE, "'%s' => '%s' '%s'", paths[i], proto ? proto : "<null>", file ? file : "<null>");
		
		DgMemoryFree(proto);
		
		// Proto are NULL
		file = NULL; proto = NULL;
		
		DgStorageSplitPathIntoParts(paths[i], NULL, &file);
		DgLog(DG_LOG_VERBOSE, "'%s' => '%s' '%s'", paths[i], proto ? proto : "<null>", file ? file : "<null>");
		
		DgMemoryFree(file);
	}
	
	// TEST 2
	DgLog(DG_LOG_INFO, "Adding a void pool and testing it...");
	
	DgLog(DG_LOG_INFO, "Result code %x", DgStorageAddPool(NULL, DgVoidCreatePool("void")));
	
	DgStream s;
	
	if (!DgStreamOpen(NULL, &s, "void://help.txt", DG_STREAM_READ | DG_STREAM_WRITE)) {
		DgStreamRead(&s, 0, NULL);
		DgStreamWrite(&s, 0, NULL);
		DgStreamClose(&s);
	}
	else {
		DgLog(DG_LOG_ERROR, "Failed to open file stream in void test!");
	}
	
	DgStorageObjectType ot;
	
	DgLogError(DgStorageDelete(NULL, "void://progress.xml"));
	DgLogError(DgStorageRename(NULL, "void://progress.xml", "void://test.xml"));
	DgLogError(DgStorageCreateFile(NULL, "void://progress.xml"));
	DgLogError(DgStorageCreateFolder(NULL, "void://more"));
	DgLogError(DgStorageType(NULL, "void://progress.xml", &ot));
	
	/// NULL protocol test
	DgLog(DG_LOG_INFO, "Adding a void pool with NULL protocol and testing...");
	DgLog(DG_LOG_INFO, "Result code %x", DgStorageAddPool(NULL, DgVoidCreatePool(NULL)));
	DgLogError(DgStorageType(NULL, "progress.xml", &ot));
	
	// TEST 3
	DgLog(DG_LOG_INFO, "Partially testing a filesystem pool...");
	
	DgLogError(DgStorageAddPool(NULL, DgFilesystemCreatePool("fs", ".")));
	
	char sample[] = "This is my *lovely* document! It's very nice!\n";
	
	DgLog(DG_LOG_INFO, "Create folder");
	DgLogError(DgStorageCreateFolder(NULL, "fs://testFiles/"));
	
	DgError error;
	
	DgLog(DG_LOG_INFO, "Basic write stream");
	if (!(error = DgStreamOpen(NULL, &s, "fs://testFiles/mytest.txt", DG_STREAM_READ | DG_STREAM_WRITE))) {
		DgStreamWrite(&s, DgStringLength(sample), sample);
		DgStreamClose(&s);
	}
	else {
		DgLog(DG_LOG_ERROR, "Failed to open filesystem stream %x", error);
	}
	
	DgLog(DG_LOG_INFO, "Create blank file");
	DgLogError(DgStorageCreateFile(NULL, "fs://testFiles/hyper_test/blank.txt"));
	
	DgLog(DG_LOG_INFO, "Delete test folder");
	DgLogError(DgStorageDelete(NULL, "fs://testFiles"));
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
		DgMemoryFree(asBase16);
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
	const char *line = DgReadLine(">> ");
	
	if (line) {
		DgLog(DG_LOG_INFO, "Read a line: %s", line);
	}
	else {
		DgLog(DG_LOG_ERROR, "Read line failed! FUCK!");
	}
	
	DgMemoryFree(line);
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

void TestCompressRLE(void) {
	DgStorageAddPool(NULL, DgFilesystemCreatePool(NULL, "."));
	
	DgLog(DG_LOG_INFO, "Test RLE compress");
	
	uint64_t example[] = {'E', 'x', 'a', 'm', 'p', 'l', 'e', 123456789014, '\x00'};
	size_t example_size = 8 * 9;
	size_t out_len; uint8_t *out_data;
	
	DgLogError(DgCompressRLE((uint8_t *) example, example_size, &out_data, &out_len));
	
	DgLog(DG_LOG_VERBOSE, "example_size = 0x%llx, out_len = 0x%llx, diff = 0x%llx", example_size, out_len, example_size - out_len);
	
	DgStorageSave(NULL, "example.rle", out_len, out_data);
}

int main(const int argc, const char *argv[]) {
	DgLog(DG_LOG_INFO, "Hello, world!");
	
	DgArgs args;
	DgArgParse(&args, argc, argv);
	
	if (DgArgGetFlag(&args, "string")) TestString();
	if (DgArgGetFlag(&args, "storage")) TestStorage();
	if (DgArgGetFlag(&args, "crypto-random")) TestCryptoRandom();
	if (DgArgGetFlag(&args, "array")) TestArray();
	if (DgArgGetFlag(&args, "terminal")) TestTerminal();
	if (DgArgGetFlag(&args, "table")) TestTableAndSerialise();
	if (DgArgGetFlag(&args, "cubehash1")) DgCryptoCubeHasher_Test();
	if (DgArgGetFlag(&args, "cubehash2")) DgCryptoCubeHashBytes_Test();
	if (DgArgGetFlag(&args, "compress-rle")) TestCompressRLE();
	
	DgArgFree(&args);
	
	return 0;
}
