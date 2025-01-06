#include "mars/resource.h"
#include "mars/game.h"
#include "resource.h"
#include "external/stb/stb_image.h"

ResourceManager* _CreateResourceManager() {
	MARS_RETURN_CLEAR;
	ResourceManager* resourceManager = NULL;

	// Allocate resource manager
	resourceManager = MARS_CALLOC(1, sizeof(*resourceManager));
	if (!resourceManager) {
		MARS_ABORT(MARS_ERROR_CODE_BAD_ALLOC, "Failed to allocate resource manager!");
		goto create_resource_manager_fail;
	}

	// Create resource list container
	resourceManager->_resourceLists = unordered_map_create(ResourceList*);
	if (!resourceManager->_resourceLists) {
		MARS_ABORT(MARS_ERROR_CODE_BAD_ALLOC, "Failed to allocate resource list container!");
		goto create_resource_manager_fail;
	}

	return resourceManager;
create_resource_manager_fail:
	_DestroyResourceManager(resourceManager);
	return NULL;
}

void _DestroyResourceManager(ResourceManager *_resourceManager) {
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
		MARS_RETURN_SET(MARS_RETURN_CODE_INVALID_REFERENCE);
		goto create_resource_list_fail;
	}

	// Allocate resource list
	resourceList = MARS_CALLOC(1, sizeof(*resourceList));
	if (!resourceList) {
		MARS_ABORT(MARS_ERROR_CODE_BAD_ALLOC, "Failed to allocate resource list for (%s)!", _desc.resourceFile);
		goto create_resource_list_fail;
	}
	resourceList->_resourceFile = _mars_strdup(_desc.resourceFile);
	resourceList->_resourcePassword = _mars_strdup(_desc.resourcePassword);

	// Initialize caches
	resourceList->_cacheText = unordered_map_str_create(TextBuffer);
	if (!resourceList->_cacheText) {
		MARS_ABORT(MARS_ERROR_CODE_BAD_ALLOC, "Failed to allocate text buffer cache for (%s)!", _desc.resourceFile);
		goto create_resource_list_fail;
	}
	resourceList->_cacheData = unordered_map_str_create(DataBuffer);
	if (!resourceList->_cacheData) {
		MARS_ABORT(MARS_ERROR_CODE_BAD_ALLOC, "Failed to allocate data buffer cache for (%s)!", _desc.resourceFile);
		goto create_resource_list_fail;
	}
	resourceList->_cacheTexture2D = unordered_map_str_create(Texture2D);
	if (!resourceList->_cacheTexture2D) {
		MARS_ABORT(MARS_ERROR_CODE_BAD_ALLOC, "Failed to allocate texture cache for (%s)!", _desc.resourceFile);
		goto create_resource_list_fail;
	}

	// Load resource file
	fp = fopen(resourceList->_resourceFile, "rb");
	if (!fp) {
		MARS_DEBUG_WARN("Failed to open resource file (%s)!", resourceList->_resourceFile);
		MARS_RETURN_SET(MARS_RETURN_CODE_FILESYSTEM_FAILURE);
		goto create_resource_list_fail;
	}
	resourceList->_resourceFileBuffer = buffer_file_read(fp, 0);
	if (!resourceList->_resourceFileBuffer) {
		MARS_DEBUG_WARN("Failed to read resource file (%s)!", resourceList->_resourceFile);
		MARS_RETURN_SET(MARS_RETURN_CODE_FILESYSTEM_FAILURE);
		goto create_resource_list_fail;
	}
	fclose(fp);
	fp = NULL;

	// Verify file header
	char header[4] = {'\0'};
	buffer_get_str(resourceList->_resourceFileBuffer, 0, 4, &header[0]);
	if (strncmp(header, "MARS", 4) != 0) {
		MARS_DEBUG_WARN("Invalid resource file (%s)!", resourceList->_resourceFile);
		MARS_RETURN_SET(MARS_RETURN_CODE_FILESYSTEM_FAILURE);
		goto create_resource_list_fail;
	}

	// TODO Verify version

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

		// Clear caches
		for(unordered_map_str_it_t* it = unordered_map_str_it(_resourceList->_cacheText); it; unordered_map_str_it_next(it)) {
			TextBuffer* textBuffer = *(TextBuffer**)it->data;
			_DestroyResourceText(textBuffer);
		}
		for(unordered_map_str_it_t* it = unordered_map_str_it(_resourceList->_cacheData); it; unordered_map_str_it_next(it)) {
			DataBuffer* dataBuffer = *(DataBuffer**)it->data;
			_DestroyResourceData(dataBuffer);
		}
		for(unordered_map_str_it_t* it = unordered_map_str_it(_resourceList->_cacheTexture2D); it; unordered_map_str_it_next(it)) {
			Texture2D* texture2DBuffer = *(Texture2D**)it->data;
			_DestroyResourceTexture2D(texture2DBuffer);
		}

		// Clear structures
		unordered_map_str_destroy(_resourceList->_cacheText);
		unordered_map_str_destroy(_resourceList->_cacheData);
		unordered_map_str_destroy(_resourceList->_cacheTexture2D);
		buffer_destroy(_resourceList->_resourceFileBuffer);
		MARS_FREE(_resourceList);
	}
}

void _DestroyResourceText(TextBuffer *_text) {
	if (_text) {
		MARS_FREE(_text->data);
		MARS_FREE(_text);
	}
}

void _DestroyResourceData(DataBuffer *_buffer) {
	if (_buffer) {
		MARS_FREE(_buffer->data);
		MARS_FREE(_buffer);
	}
}

void _DestroyResourceTexture2D(Texture2D *_texture) {
	if (_texture) {
		MARS_FREE(_texture->data);
		MARS_FREE(_texture);
	}
}

uint64_t _SeekResourceInBuffer(buffer_t* _resourceBuffer, const char* _rsc) {
	// Load from resource file
	size_t max_len = strlen(_rsc);
	size_t rsc_len = 0;
	uint64_t file_table_off = buffer_get_u64(_resourceBuffer, 48);
	while(rsc_len < max_len) {
		// Tokenize resource name
		rsc_len = strcspn(_rsc, "/");
		if (rsc_len == 0) {
			MARS_RETURN_SET(MARS_RETURN_CODE_INVALID_PARAMETER);
			goto seek_resource_in_buffer_fail;
		}
		char* tok = _mars_strndup(_rsc, rsc_len - 1);

		// Parse file table header
		char file_table_sig[4] = {'\0'};
		buffer_get_str(_resourceBuffer, 0, 4, &file_table_sig[0]);
		if (strncmp(file_table_sig, "MRFT", 4) != 0) {
			MARS_RETURN_SET(MARS_RETURN_CODE_FILESYSTEM_FAILURE);
			goto seek_resource_in_buffer_fail;
		}
		uint32_t file_table_count = buffer_get_u32(_resourceBuffer, file_table_off + 4);
		uint32_t file_table_capacity = buffer_get_u32(_resourceBuffer, file_table_off + 8);

		// Deserialize file table
		char* file_table_ctrl_block = MARS_CALLOC(file_table_capacity, sizeof(*file_table_ctrl_block));
		if (!file_table_ctrl_block) {
			MARS_ABORT(MARS_ERROR_CODE_BAD_ALLOC, "Failed to allocate file table control block!");
			goto seek_resource_in_buffer_fail;
		}
		char* file_table_data_block = MARS_CALLOC(file_table_capacity * 40, sizeof(*file_table_data_block));
		if (!file_table_data_block) {
			MARS_FREE(file_table_ctrl_block);
			MARS_ABORT(MARS_ERROR_CODE_BAD_ALLOC, "Failed to allocate file table data block!");
			goto seek_resource_in_buffer_fail;
		}
		buffer_get_str(_resourceBuffer, file_table_off + 12, file_table_capacity, file_table_ctrl_block);
		buffer_get_str(_resourceBuffer, file_table_off + 12 + file_table_capacity, file_table_capacity * 40, file_table_data_block);

		// Search file table for token
		_umap_str_hash_t h = _umap_str_hash(tok);
		size_t pos = _umap_str_h1(h) & (file_table_capacity - 1);
		size_t first_pos = pos;
		do {
			uint8_t ctrl = file_table_ctrl_block[pos];
			_umap_str_hash_t h2 = _umap_str_h2(h);
			if (ctrl == h2) {
				char* key = &file_table_data_block[0] + (40 * pos);
				if (strncmp(key, tok, 32)) {
					file_table_off = buffer_get_u64(_resourceBuffer, file_table_off + 12 + file_table_capacity + (40 * pos) + 32);
					break;
				}
			}
			else if (ctrl == _UMAP_STR_EMPTY) {
				MARS_RETURN_SET(MARS_RETURN_CODE_INVALID_ID);
				MARS_FREE(tok);
				MARS_FREE(file_table_ctrl_block);
				MARS_FREE(file_table_data_block);
				goto seek_resource_in_buffer_fail;
			}
			else {
				pos = (pos + 1) & (file_table_capacity - 1);
			}
		} while(pos != first_pos);

		// Advance pointer
		_rsc += rsc_len;
		max_len -= rsc_len;
		MARS_FREE(tok);
		MARS_FREE(file_table_ctrl_block);
		MARS_FREE(file_table_data_block);
	}

	return file_table_off;
seek_resource_in_buffer_fail:
	return 0;
}
char* _GetResourceDataFromBuffer(buffer_t* _resourceBuffer, uint64_t _offset, const char* _type, size_t* _len) {
	char* data_block = NULL;

	// Read resource header
	char data_block_sig[4] = {'\0'};
	buffer_get_str(_resourceBuffer, _offset, 4, &data_block_sig[0]);
	if (strncmp(data_block_sig, _type, 4) != 0) {
		MARS_RETURN_SET(MARS_RETURN_CODE_FILESYSTEM_FAILURE);
		goto get_resource_data_from_buffer_fail;
	}
	uint32_t data_block_crc = buffer_get_u32(_resourceBuffer, _offset + 4);
	uint64_t data_block_next = buffer_get_u64(_resourceBuffer, _offset + 8);
	uint64_t data_block_prev = buffer_get_u64(_resourceBuffer, _offset + 16);
	uint64_t data_block_compressed_size = buffer_get_u64(_resourceBuffer, _offset + 24);
	uint64_t data_block_uncompressed_size = buffer_get_u64(_resourceBuffer, _offset + 32);
	_offset += 80; // Advance past data block header
	data_block = MARS_CALLOC(data_block_compressed_size, sizeof(*data_block));
	if (!data_block) {
		MARS_ABORT(MARS_ERROR_CODE_BAD_ALLOC, "Failed to allocate resource data buffer!");
		goto get_resource_data_from_buffer_fail;
	}
	buffer_get_str(_resourceBuffer, _offset, data_block_compressed_size, data_block);

	// Decompress data
	if (data_block_compressed_size != data_block_uncompressed_size) {
		char* decompressed_data_block = MARS_CALLOC(data_block_uncompressed_size, sizeof(*decompressed_data_block));
		if (!decompressed_data_block) {
			MARS_ABORT(MARS_ERROR_CODE_BAD_ALLOC, "Failed to allocate decompressed resource data buffer!");
			goto get_resource_data_from_buffer_fail;
		}
		if (LZ4_decompress_safe(data_block, decompressed_data_block, (int)data_block_compressed_size, (int)data_block_uncompressed_size) != data_block_uncompressed_size) {
			MARS_RETURN_SET(MARS_RETURN_CODE_RESOURCE_FAILURE);
			MARS_FREE(decompressed_data_block);
			goto get_resource_data_from_buffer_fail;
		}
		MARS_FREE(data_block);
		data_block = decompressed_data_block;
	}

	// Validate data
	if (CRC32Calculate(data_block, data_block_uncompressed_size, 0) != data_block_crc) {
		MARS_RETURN_SET(MARS_RETURN_CODE_FILESYSTEM_FAILURE);
		goto get_resource_data_from_buffer_fail;
	}

	*_len = data_block_uncompressed_size;
	return data_block;
get_resource_data_from_buffer_fail:
	MARS_FREE(data_block);
	*_len = 0;
	return NULL;
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
	void* loadedResourceList = unordered_map_insert(MARS_RESOURCES->_resourceLists, id, resourceList);
	MARS_ASSERT(loadedResourceList != NULL);
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
	return NULL;
}

DataBuffer* GetResourceData(resourceList_id _resourceList, const char* _data) {
	return NULL;
}

Texture2D* GetResourceTexture2D(resourceList_id _resourceList, const char* _texture) {
	MARS_RETURN_CLEAR;
	char* data_block = NULL;

	// Error check
	if (_resourceList == ID_NULL) { 
		MARS_DEBUG_WARN("NULL resource list ID!");
		MARS_RETURN_SET(MARS_RETURN_CODE_INVALID_ID);
		goto get_resource_texture_2d_fail;
	}
	if (_texture == NULL) { 
		MARS_DEBUG_WARN("NULL texture name!");
		MARS_RETURN_SET(MARS_RETURN_CODE_INVALID_REFERENCE);
		goto get_resource_texture_2d_fail;
	}

	// Get resource list
	void* data = unordered_map_find(MARS_RESOURCES->_resourceLists, _resourceList);
	if (!data) {
		MARS_DEBUG_WARN("Invalid resource list ID (%d)!", (int)_resourceList);
		MARS_RETURN_SET(MARS_RETURN_CODE_INVALID_PARAMETER);
		goto get_resource_texture_2d_fail;
	}
	ResourceList* resourceList = (ResourceList*)(data);

	// Check cache
	data = unordered_map_str_find(resourceList->_cacheTexture2D, _texture);
	if (data) { return (Texture2D*)(data); }

	// Get offset of data in buffer
	uint64_t resourceOff = _SeekResourceInBuffer(resourceList->_resourceFileBuffer, _texture);
	if (resourceOff == 0) {
		if (MARS_RETURN_CODE == MARS_RETURN_CODE_INVALID_ID) {
			MARS_DEBUG_WARN("Failed to find texture (%s)!", _texture);
		}
		else if (MARS_RETURN_CODE == MARS_RETURN_CODE_INVALID_PARAMETER) {
			MARS_DEBUG_WARN("Invalid texture name (%s)!", _texture);
		}
		else if (MARS_RETURN_CODE == MARS_RETURN_CODE_FILESYSTEM_FAILURE) {
			MARS_DEBUG_WARN("Invalid resource file (%s)!", resourceList->_resourceFile);
		}
		goto get_resource_texture_2d_fail;
	}

	// Get data from buffer
	size_t data_block_len = 0;
	data_block = _GetResourceDataFromBuffer(resourceList->_resourceFileBuffer, resourceOff, "MIMG", &data_block_len);
	if (!data_block) {
		if (MARS_RETURN_CODE == MARS_RETURN_CODE_RESOURCE_FAILURE) {
			MARS_DEBUG_WARN("Failed to decompress texture data (%s)!", _texture);
		}
		else if (MARS_RETURN_CODE == MARS_RETURN_CODE_FILESYSTEM_FAILURE) {
			MARS_DEBUG_WARN("Failed to validate texture data (%s)!", _texture);
		}
		goto get_resource_texture_2d_fail;
	}

	// Parse & cache data
	Texture2D texture = { 0 };
	texture.data = stbi_load_from_memory(data_block, (int)data_block_len, &texture.width, &texture.height, &texture.channels, 0);
	void* cached_data = unordered_map_str_insert(resourceList->_cacheTexture2D, _texture, &texture);
	MARS_ASSERT(cached_data != NULL);

	return cached_data;
get_resource_texture_2d_fail:
	MARS_FREE(data_block);
	return NULL;
}