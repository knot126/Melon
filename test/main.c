#include "util/log.h"
#include "util/string.h"
#include "util/storage.h"
#include "util/storage_void.h"
#include "util/storage_filesystem.h"
#include "util/file.h"
#include "util/alloc.h"
#include "util/crypto.h"

#ifndef MELON_CRYPTOGRAPHY_RANDOM
#error MELON_CRYPTOGRAPHY_RANDOM not defined
#endif

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
	
	DgLog(DG_LOG_INFO, "%x", DgStorageAddPool(NULL, DgFilesystemCreatePool("fs", ".")));
	
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
		
		const char *asBase16 = DgStringEncodeBase16(16, rand_bytes);
		DgLog(DG_LOG_INFO, "Secure random bytes as base16: %s", asBase16);
		DgFree(asBase16);
	}
	
	DgLog(DG_LOG_SUCCESS, "TestCryptoRandom");
}

void TestMemory(void) {
	//DgLog(DG_LOG_VERBOSE, "Allocated 0x%x bytes of memory over lifetime", DgMemoryAllocatedCount());
}

int main(const int argc, const char *argv[]) {
	DgLog(DG_LOG_INFO, "Hello, world!");
	
	TestString();
	TestStorage();
	TestMemory();
	TestCryptoRandom();
	
	return 0;
}
