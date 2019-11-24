#ifndef __MODULEFILESYSTEM_H__
#define __MODULEFILESYSTEM_H__

#include "Module.h"
#include <vector>
#include <string>
#include "Json/json.hpp"
#include "Globals.h"

struct SDL_RWops;
int close_sdl_rwops(SDL_RWops* rw);

struct aiFileIO;


class ModuleFileSystem : public Module
{
public:

	ModuleFileSystem(Application* app, bool start_enabled = true, const char* game_path = nullptr);

	// Destructor
	~ModuleFileSystem();


	// Called before quitting
	bool CleanUp() override;

	// Utility functions
	bool AddPath(const char* path_or_zip);
	bool Exists(const char* file) const;
	bool IsDirectory(const char* file) const;
	void CreateDirectory(const char* directory);
	void DiscoverFiles(const char* directory, std::vector<std::string>& file_list, std::vector<std::string>& dir_list) const;
	bool CopyFromOutsideFS(const char* full_path, const char* destination);
	bool Copy(const char* source, const char* destination);
	void SplitFilePath(const char* full_path, std::string* path, std::string* file = nullptr, std::string* extension = nullptr) const;
	void NormalizePath(char* full_path) const;
	void NormalizePath(std::string& full_path) const;

	// Open for Read/Write
	unsigned int Load(const char* path, const char* file, char** buffer) const;
	unsigned int Load(const char* file, char** buffer) const;
	SDL_RWops* Load(const char* file) const;
	void* BassLoad(const char* file) const;

	// IO interfaces for other libs to handle files via PHYSfs
	aiFileIO* GetAssimpIO();

	unsigned int Save(const char* file, const void* buffer, unsigned int size, bool append = false) const;
	bool SaveUnique(std::string& output, const void* buffer, uint size, const char* path, const char* prefix, const char* extension);
	bool Remove(const char* file);

	// Function to save json things.
	void SaveFile(const char* path, const nlohmann::json & to_save);


	const char* GetBasePath() const;
	const char* GetWritePath() const;
	const char* GetReadPaths() const;


	//new functions (Above are a spiral of destruction that i barelly understand) xd
	void GetActualName(std::string& path_name_extension);

	//this function cuts the extension of the filename
	void CutExtension(std::string& path_name_extension);

	//this function cuts the path of the filename
	void CutPath(std::string& path_name_extension);

	void NormalizeString(std::string& path_name_extension);

private:

	void CreateAssimpIO();

private:

	aiFileIO* AssimpIO = nullptr;

};

#endif // __MODULEFILESYSTEM_H__
