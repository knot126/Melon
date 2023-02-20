#include "util/log.h"
#include "util/string.h"
#include "util/storage.h"
#include "util/alloc.h"

void TestString(void) {
	DgLog(DG_LOG_INFO, "TestString()");
	
	// Find function
	DgLog(DG_LOG_INFO, "%d", DgStringFind("fs://games/save.xml", "://", 0));
	DgLog(DG_LOG_INFO, "%d", DgStringFind("fs://games/save.xml, user://myfile.txt", "://", 1));
	DgLog(DG_LOG_INFO, "%d", DgStringFind("WONT FIND", "THIS", 0));
	DgLog(DG_LOG_INFO, "%d", DgStringFind("ABCDBEBBDBBEABBEABABROOEOKKA", "AB", 3));
	DgLog(DG_LOG_INFO, "%d", DgStringFind("", "asdf", 69));
	
	// Duplicate string up to
	DgLog(DG_LOG_INFO, "Up to 10 chars: %s", DgStringDuplicateUntil("Test string that is very long string!", 13));
	DgLog(DG_LOG_INFO, "Up to 10 chars: %s", DgStringDuplicateUntil("Hewwo! :3", 13));
	
	DgLog(DG_LOG_SUCCESS, "TestString()");
}

/////// TEST POOL TYPE ////////

DgError DgVoid_Delete(DgStorage *storage, DgStoragePool *pool, DgStoragePath path) {
	DgLog(DG_LOG_VERBOSE, "DgVoid_Func() called!!");
	
	return DG_ERROR_SUCCESSFUL;
}

DgStorageFunctions gVoidFunctions = {
	.create_file = NULL,
	.create_folder = NULL,
	.type = NULL,
	.rename = NULL,
	.delete = &DgVoid_Delete,
	.open = NULL,
	.close = NULL,
	.read = NULL,
	.write = NULL,
	.get_position = NULL,
	.set_position = NULL,
	.seek = NULL,
	.free_specific_config = NULL,
};

DgStoragePool *DgVoidCreatePool(const char *protocol) {
	/**
	 * Create an empty pool void pool
	 */
	
	DgStoragePool *pool = DgAlloc(sizeof *pool);
	
	if (!pool) {
		return NULL;
	}
	
	pool->protocol = DgStringDuplicate(protocol);
	pool->functions = &gVoidFunctions;
	pool->specific_config = NULL;
	
	return pool;
}

/////// END TEST POOL TYPE ////////

void TestStorage(void) {
	DgLog(DG_LOG_INFO, "TestStorage()");
	
	char *proto, *file;
	
	// TEST 1
	DgStorageSplitPathIntoParts("assets://stage/canyon.xml", &proto, &file);
	
	DgLog(DG_LOG_INFO, "Path: %s %s", proto, file);
	
	DgFree(proto);
	DgFree(file);
	
	DgStorageSplitPathIntoParts("fs://what/the/shit.fuck", &proto, NULL);
	
	DgLog(DG_LOG_INFO, "Path: %s", proto);
	
	DgFree(proto);
	
	// TEST 2
	
	DgLog(DG_LOG_INFO, "%x", DgStorageAddPool(NULL, DgVoidCreatePool("void")));
	
	DgLog(DG_LOG_INFO, "%x", DgStorageDelete(NULL, "void://help.txt"));
	
	DgLog(DG_LOG_SUCCESS, "TestStorage()");
}

int main(const int argc, const char *argv[]) {
	DgLog(DG_LOG_INFO, "Hello, world!");
	
	TestString();
	TestStorage();
	
	return 0;
}
