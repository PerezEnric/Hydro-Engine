#pragma once

#include "Globals.h"
#include <string>


enum RESOURCE_TYPE
{
	R_MESH,
	R_TEXTURE,
	R_SCENE,

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


	std::string GetOriginalPath();
	std::string GetOwnPath();

	//bools

	bool IsLoadedInMemory();
	virtual bool LoadToMemory();
	uint CountReferences();




	



protected:
	uint my_uuid = 0;

	std::string path_to_original;
	std::string path_to_own;

	RESOURCE_TYPE my_type;
	uint loaded = 0;



};
