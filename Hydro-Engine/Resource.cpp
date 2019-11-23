#include "Resource.h"

Resource::Resource(uint uuid, RESOURCE_TYPE type)
{
	my_uuid = uuid;
	my_type = type;
}

Resource::~Resource()
{
}

RESOURCE_TYPE Resource::GetType()
{
	return my_type;
}

uint Resource::GetUUID()
{
	return my_uuid;
}

bool Resource::IsLoadedInMemory()
{
	return loaded;
}

uint Resource::CountReferences()
{
	return loaded;
}
