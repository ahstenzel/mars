#include "mars/resource.h"
#include "mars/game.h"

ResourceManager* _CreateResourceManager() {
	MARS_RETURN_CLEAR;
	ResourceManager* resourceManager = NULL;

	// Allocate resource manager
	resourceManager = MARS_CALLOC(1, sizeof(*resourceManager));
	if (!resourceManager) {
		MARS_ABORT(MARS_ERROR_STATUS_BAD_ALLOC, "Failed to allocate resource manager!");
		goto create_resource_manager_fail;
	}

	// Create resource list container
	resourceManager->_resourceLists = unordered_map_create(ResourceList*);
	if (!resourceManager->_resourceLists) {
		MARS_ABORT(MARS_ERROR_STATUS_BAD_ALLOC, "Failed to allocate resource list container!");
		goto create_resource_manager_fail;
	}

	return resourceManager;
create_resource_manager_fail:
	_DestroyResourceManager(resourceManager);
	return NULL;
}

void _DestroyResourceManager(ResourceManager* _resourceManager) {
	if (_resourceManager) {
		// Free all resource lists
		for(unordered_map_it_t* it = unordered_map_it(_resourceManager->_resourceLists); it; unordered_map_it_next(it)) {
			ResourceList* resourceList = *(ResourceList**)it->data;
			_DestroyResourceList(resourceList);
		}
		unordered_map_destroy(_resourceManager->_resourceLists);
		MARS_FREE(_resourceManager);
	}
}

ResourceList* _CreateResourceList(ResourceListDesc _desc) {
	MARS_RETURN_CLEAR;
	ResourceList* resourceList = NULL;
	FILE* fp = NULL;

	// Error check
	if (!_desc.resourceFile) {
		MARS_DEBUG_WARN("NULL resource file name!");
		MARS_RETURN_SET(MARS_RETURN_INVALID_REFERENCE);
		goto create_resource_list_fail;
	}

	// Allocate resource list
	resourceList = MARS_CALLOC(1, sizeof(*resourceList));
	if (!resourceList) {
		MARS_ABORT(MARS_ERROR_STATUS_BAD_ALLOC, "Failed to allocate resource list for (%s)!", _desc.resourceFile);
		goto create_resource_list_fail;
	}
	resourceList->_resourceFile = strdup(_desc.resourceFile);
	resourceList->_resourcePassword = strdup(_desc.resourcePassword);

	// Initialize caches
	resourceList->_cacheText = unordered_map_str_create(TextBuffer);
	if (!resourceList->_cacheText) {
		MARS_ABORT(MARS_ERROR_STATUS_BAD_ALLOC, "Failed to allocate text buffer cache for (%s)!", _desc.resourceFile);
		goto create_resource_list_fail;
	}
	resourceList->_cacheData = unordered_map_str_create(DataBuffer);
	if (!resourceList->_cacheData) {
		MARS_ABORT(MARS_ERROR_STATUS_BAD_ALLOC, "Failed to allocate data buffer cache for (%s)!", _desc.resourceFile);
		goto create_resource_list_fail;
	}
	resourceList->_cacheTexture2D = unordered_map_str_create(Texture2D);
	if (!resourceList->_cacheTexture2D) {
		MARS_ABORT(MARS_ERROR_STATUS_BAD_ALLOC, "Failed to allocate texture cache for (%s)!", _desc.resourceFile);
		goto create_resource_list_fail;
	}

	// Load resource file
	fp = fopen(resourceList->_resourceFile, "rb");
	if (!fp) {
		MARS_DEBUG_WARN("Failed to open resource file (%s)!", resourceList->_resourceFile);
		MARS_RETURN_SET(MARS_RETURN_FILESYSTEM_FAILURE);
		goto create_resource_list_fail;
	}
	resourceList->_resourceFileBuffer = buffer_file_read(fp, 0);
	if (!resourceList->_resourceFileBuffer) {
		MARS_DEBUG_WARN("Failed to read resource file (%s)!", resourceList->_resourceFile);
		MARS_RETURN_SET(MARS_RETURN_FILESYSTEM_FAILURE);
		goto create_resource_list_fail;
	}
	fclose(fp);

	// Verify file header
	char header[4] = {'\0'};
	buffer_get_str(resourceList->_resourceFileBuffer, 0, 4, &header[0]);
	if (strncmp(header, "MARS", 4) != 0) {
		MARS_DEBUG_WARN("Invalid resource file (%s)!", resourceList->_resourceFile);
		MARS_RETURN_SET(MARS_RETURN_FILESYSTEM_FAILURE);
		goto create_resource_list_fail;
	}

	// Decrypt contents
	char iv[32] = {'\0'};
	buffer_get_str(resourceList->_resourceFileBuffer, 16, 32, &iv[0]);
	if (resourceList->_resourcePassword) {
		size_t decrypted_len = resourceList->_resourceFileBuffer->_length - 48;
		struct AES_ctx ctx;
		AES_init_ctx_iv(&ctx, resourceList->_resourcePassword, &iv[0]);
		AES_CBC_decrypt_buffer(&ctx, &resourceList->_resourceFileBuffer->_buffer[48], decrypted_len);
	}

	return resourceList;
create_resource_list_fail:
	if (fp) fclose(fp);
	_DestroyResourceList(resourceList);
	return NULL;
}

void _DestroyResourceList(ResourceList* _resourceList) {
	if (_resourceList) {
		MARS_FREE(_resourceList->_resourceFile);
		MARS_FREE(_resourceList->_resourcePassword);
		unordered_map_str_destroy(_resourceList->_cacheText);
		unordered_map_str_destroy(_resourceList->_cacheData);
		unordered_map_str_destroy(_resourceList->_cacheTexture2D);
		buffer_destroy(_resourceList->_resourceFileBuffer);
		MARS_FREE(_resourceList);
	}
}

resourceList_id LoadResourceFile(ResourceListDesc _desc) {
	MARS_RETURN_CLEAR;

	// Load resource file
	ResourceList* resourceList = _CreateResourceList(_desc);
	if (!resourceList) {
		MARS_DEBUG_WARN("Failed to load resource file!");
		return ID_NULL;
	}

	// Add to global list
	resourceList_id id = _mars_id_generate();
	unordered_map_insert(MARS_RESOURCES->_resourceLists, id, resourceList);
	return id;
}

void UnloadResourceFile(resourceList_id _id) {
	if (_id != ID_NULL) {
		void* data = unordered_map_find(MARS_RESOURCES->_resourceLists, _id);
		if (data) {
			ResourceList* resourceList = *(ResourceList**)(data);
			_DestroyResourceList(resourceList);
			unordered_map_delete(MARS_RESOURCES->_resourceLists, _id);
		}
	}
}

TextBuffer* GetResourceText(resourceList_id _resourceList, const char* _text) {

}

DataBuffer* GetResourceData(resourceList_id _resourceList, const char* _data) {

}

Texture2D* GetResourceTexture2D(resourceList_id _resourceList, const char* _texture) {
	// Error check
	if (_resourceList == ID_NULL) { 
		MARS_DEBUG_WARN("NULL resource list ID!");
		MARS_RETURN_SET(MARS_RETURN_INVALID_ID);
		goto get_resource_texture_2d_fail;
	}
	if (_texture == NULL) { 
		MARS_DEBUG_WARN("NULL texture name!");
		MARS_RETURN_SET(MARS_RETURN_INVALID_REFERENCE);
		goto get_resource_texture_2d_fail;
	}

	// Get resource list
	void* data = unordered_map_find(MARS_RESOURCES->_resourceLists, _resourceList);
	if (!data) {
		MARS_DEBUG_WARN("Invalid resource list ID (%d)!", (int)_resourceList);
		MARS_RETURN_SET(MARS_RETURN_INVALID_PARAMETER);
		goto get_resource_texture_2d_fail;
	}
	ResourceList* resourceList = (ResourceList*)(data);

	// Check cache
	data = unordered_map_str_find(resourceList->_cacheTexture2D, _texture);
	if (data) { return (Texture2D*)(data); }

	// Load from resource file
	// TODO: 
	//  - Tokenize resource name
	//  - Deserialize file tables from resource file buffer
	//  - Walk file tables to get to resource data
	//  - Decompress resource data
	//  - Parse resource data into bitmap
get_resource_texture_2d_fail:
	return NULL;
}