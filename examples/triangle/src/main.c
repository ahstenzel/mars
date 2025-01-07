#include "mars.h"

int main() {
	CreateGame("Triangle");

	ResourceListDesc desc = {
		.resourceFile = "test.mrc",
		.resourcePassword = NULL
	};
	resourceList_id rsc = LoadResourceFile(desc);
	Texture2D* tex = GetResourceTexture2D(rsc, "group1/tex");
	if (tex) MARS_DEBUG_LOG("Texture properties: w=%u h=%u c=%u", tex->width, tex->height, tex->channels);
	
	UpdateGame();
	DestroyGame();
	return 0;
}