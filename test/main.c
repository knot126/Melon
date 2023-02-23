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
	DgLog(DG_LOG_INFO, "Up to 13 chars: %s", DgStringDuplicateUntil("Test string that is very long string!", 13));
	DgLog(DG_LOG_INFO, "Up to 13 chars: %s", DgStringDuplicateUntil("Hewwo! :3", 13));
	
	DgLog(DG_LOG_SUCCESS, "TestString()");
}

/////// TEST POOL TYPE ////////

DgError DgVoid_Rename(DgStorage *storage, DgStoragePool *pool, DgStoragePath old_path, DgStoragePath new_path) {
	DgLog(DG_LOG_VERBOSE, "DgVoid_Rename() called!! %s -> %s", old_path, new_path);
	
	return DG_ERROR_SUCCESSFUL;
}

DgError DgVoid_Delete(DgStorage *storage, DgStoragePool *pool, DgStoragePath path) {
	DgLog(DG_LOG_VERBOSE, "DgVoid_Delete() called!! %s", path);
	
	return DG_ERROR_SUCCESSFUL;
}

DgError DgVoid_CreateFile(DgStorage *storage, DgStoragePool *pool, DgStoragePath path) {
	DgLog(DG_LOG_VERBOSE, "DgVoid_CreateFile() called!! %s", path);
	
	return DG_ERROR_SUCCESSFUL;
}

DgError DgVoid_CreateFolder(DgStorage *storage, DgStoragePool *pool, DgStoragePath path) {
	DgLog(DG_LOG_VERBOSE, "DgVoid_CreateFolder() called!! %s", path);
	
	return DG_ERROR_SUCCESSFUL;
}

DgError DgVoid_Type(DgStorage *storage, DgStoragePool *pool, DgStoragePath path, DgStorageObjectType *type) {
	DgLog(DG_LOG_VERBOSE, "DgVoid_Type() called!! %s", path);
	
	type[0] = DG_STORAGE_TYPE_REMOTE;
	
	return DG_ERROR_SUCCESSFUL;
}

DgError DgVoid_Open(DgStorage *storage, DgStoragePool *pool, DgStream *context, DgStoragePath path, DgStorageFlags flags) {
	DgLog(DG_LOG_VERBOSE, "DgVoid_Open() called!!");
	
	return DG_ERROR_SUCCESSFUL;
}

DgError DgVoid_Close(DgStorage *storage, DgStoragePool *pool, DgStream *context) {
	DgLog(DG_LOG_VERBOSE, "DgVoid_Close() called!!");
	
	return DG_ERROR_SUCCESSFUL;
}

DgError DgVoid_Read(DgStorage *storage, DgStoragePool *pool, DgStream *context, size_t size, void *buffer) {
	DgLog(DG_LOG_VERBOSE, "DgVoid_Read() called!!");
	
	return DG_ERROR_SUCCESSFUL;
}

DgError DgVoid_Write(DgStorage *storage, DgStoragePool *pool, DgStream *context, size_t size, void *buffer) {
	DgLog(DG_LOG_VERBOSE, "DgVoid_Write() called!!");
	
	return DG_ERROR_SUCCESSFUL;
}

DgError DgVoid_GetPosition(DgStorage *storage, DgStoragePool *pool, DgStream *context, size_t *position) {
	DgLog(DG_LOG_VERBOSE, "DgVoid_GetPosition() called!!");
	
	return DG_ERROR_SUCCESSFUL;
}

DgError DgVoid_SetPosition(DgStorage *storage, DgStoragePool *pool, DgStream *context, size_t position) {
	DgLog(DG_LOG_VERBOSE, "DgVoid_SetPosition() called!!");
	
	return DG_ERROR_SUCCESSFUL;
}

DgError DgVoid_Seek(DgStorage *storage, DgStoragePool *pool, DgStream *context, DgStorageSeekBase base, size_t offset) {
	DgLog(DG_LOG_VERBOSE, "DgVoid_Seek() called!!");
	
	return DG_ERROR_SUCCESSFUL;
}

DgStorageFunctions gVoidFunctions = {
	.create_file = &DgVoid_CreateFile,
	.create_folder = &DgVoid_CreateFolder,
	.type = &DgVoid_Type,
	.rename = &DgVoid_Rename,
	.delete = &DgVoid_Delete,
	.open = &DgVoid_Open,
	.close = &DgVoid_Close,
	.read = &DgVoid_Read,
	.write = &DgVoid_Write,
	.get_position = &DgVoid_GetPosition,
	.set_position = &DgVoid_SetPosition,
	.seek = &DgVoid_Seek,
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
	
	DgStream s;
	
	if (DgStreamOpen(NULL, &s, "void://help.txt", DG_STREAM_READ | DG_STREAM_WRITE)) {
		DgLog(DG_LOG_INFO, "Failed to open file stream!");
		return;
	}
	
	DgStreamRead(&s, 0, NULL);
	DgStreamWrite(&s, 0, NULL);
	
	DgStreamClose(&s);
	
	DgLog(DG_LOG_SUCCESS, "TestStorage()");
}

int main(const int argc, const char *argv[]) {
	DgLog(DG_LOG_INFO, "Hello, world!");
	
	TestString();
	TestStorage();
	
	return 0;
}
