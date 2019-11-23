#pragma once

#include "Globals.h"
#include <string>


enum RESOURCE_TYPE
{
	R_MESH,
	R_TEXTURE,

	R_ERROR
};


class Resource
{
public:
	Resource(uint uuid, RESOURCE_TYPE type);
	~Resource();

	//This function will return the type of the current resource
	RESOURCE_TYPE GetType();

	//The same but with the uuid.
	uint GetUUID();

	//bools

	bool IsLoadedInMemory();
	virtual bool LoadToMemory() { return false; }
	virtual bool UnLoadFromMemory() { return false; }
	uint CountReferences();

	virtual void CleanUp() {}

	std::string path_to_original;
	std::string path_to_own;
	std::string original_name;

	int loaded = 0;

protected:

	uint my_uuid = 0;
	RESOURCE_TYPE my_type;
	

};
