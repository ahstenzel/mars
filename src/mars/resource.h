#ifndef MARS_RESOURCE_H
#define MARS_RESOURCE_H
/**
 * resource.h
 * Resource management functions.
*/
#include "mars/common.h"

typedef struct {
	uint8_t* data;
	unsigned int width;
	unsigned int height;
	unsigned int channels;
} Texture2D;

typedef struct {
	char* data;
	size_t size;
} TextBuffer;

typedef struct {
	uint8_t* data;
	size_t size;
} DataBuffer;

typedef struct {
	unordered_map_t* _resourceLists;
} ResourceManager;

typedef struct {
	unordered_map_str_t* _cacheText;
	unordered_map_str_t* _cacheData;
	unordered_map_str_t* _cacheTexture2D;
	buffer_t* _resourceFileBuffer;
	char* _resourceFile;
	char* _resourcePassword;
} ResourceList;

typedef struct {
	const char* resourceFile;
	const char* resourcePassword;
} ResourceListDesc;

typedef _MARS_ID_TYPE resourceList_id;

ResourceManager* _CreateResourceManager();

void _DestroyResourceManager(ResourceManager* _resourceManager);

ResourceList* _CreateResourceList(ResourceListDesc _desc);

void _DestroyResourceList(ResourceList* _resourceList);

void _DestroyResourceText(TextBuffer* _text);

void _DestroyResourceData(DataBuffer* _data);

void _DestroyResourceTexture2D(Texture2D* _texture);

uint64_t _SeekResourceInBuffer(buffer_t* _resourceBuffer, char* _rsc);

char* _GetResourceDataFromBuffer(buffer_t* _resourceBuffer, uint64_t _offset, char* _type, size_t* _len);


MARS_API resourceList_id LoadResourceFile(ResourceListDesc _desc);

MARS_API void UnloadResourceFile(resourceList_id _resourceList);

MARS_API TextBuffer* GetResourceText(resourceList_id _resourceList, char* _textName);

MARS_API DataBuffer* GetResourceData(resourceList_id _resourceList, char* _dataName);

MARS_API Texture2D* GetResourceTexture2D(resourceList_id _resourceList, char* _textureName);

#endif // MARS_RESOURCE_H