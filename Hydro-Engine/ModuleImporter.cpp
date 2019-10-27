#include "Application.h"
#include "ModuleImporter.h"
#include "Globals.h"
#include "ModuleInput.h"

#include "DevIL/include/IL/il.h"
#include "DevIL/include/IL/ilu.h"
#include "DevIL/include/IL/ilut.h"

#pragma comment (lib, "DevIL/libx86/DevIL.lib")
#pragma comment (lib, "DevIL/libx86/ILU.lib")
#pragma comment (lib, "DevIL/libx86/ILUT.lib")

#include "GameObject.h"
#include "Component_Mesh.h"
#include "Component_Texture.h"






ModuleImporter::ModuleImporter(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleImporter::~ModuleImporter()
{
}

bool ModuleImporter::Start()
{
	//debugger
	aiLogStream stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
	LOG("aiAttachLogStream attached correctly the stream");
	ilInit(); 
	LOG("devil Inited");
	local_doc.assign("Assets/");
	
	return true;
}

int ModuleImporter::HowManyMeshes(const std::string & Filename)
{
	int ret = 0;
	Assimp::Importer Importer;

	const aiScene* pScene = Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcessPreset_TargetRealtime_MaxQuality); // el aiProcess_Triangulate sirve para transformar las caras cuadras en triangulos
	if (pScene) {
		ret = pScene->mNumMeshes;
	}
	else {
		printf("Error parsing '%s': '%s'\n", Filename.c_str(), Importer.GetErrorString());
	}
	return ret;
}

bool ModuleImporter::LoadFBX(const std::string & Filename, uint index, Component_Mesh* Ret)
{
	
	
	Assimp::Importer Importer;

	const aiScene* pScene = Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcessPreset_TargetRealtime_MaxQuality); // el aiProcess_Triangulate sirve para transformar las caras cuadras en triangulos
	if (pScene) {
		const aiMesh* sMesh = pScene->mMeshes[index];


		// First lets load

		LOG("Loading vertex from ASSIMP:")
		Ret->num_vertex = sMesh->mNumVertices;
		Ret->vertex = new float[Ret->num_vertex * 3]; 
		memcpy(Ret->vertex, sMesh->mVertices, sizeof(float) * Ret->num_vertex * 3); 


		glGenBuffers(1, &Ret->id_vertex);
		glBindBuffer(GL_ARRAY_BUFFER, Ret->id_vertex);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * Ret->num_vertex * 3, Ret->vertex, GL_STATIC_DRAW);
		
		LOG("New mesh with %d vertex", Ret->num_vertex);



		// copy faces
		if (sMesh->HasFaces())
		{
			LOG("Loading Mesh index from ASSIMP");
			Ret->num_index = sMesh->mNumFaces * 3;
			Ret->index = new uint[Ret->num_index]; 
			for (uint i = 0; i < sMesh->mNumFaces; ++i)
			{
				if (sMesh->mFaces[i].mNumIndices != 3)
				{
					LOG("WARNING, geometry face with != 3 indices!");
				}
				else
				{
					memcpy(&Ret->index[i * 3], sMesh->mFaces[i].mIndices, 3 * sizeof(uint));
					glGenBuffers(1, &Ret->id_index);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ret->id_index);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * (Ret->num_index), Ret->index, GL_STATIC_DRAW);
				}

			}
			LOG("New mesh with %d index", Ret->num_index);
		}

		if (sMesh->HasNormals())
		{
			Ret->normal = new float[sMesh->mNumVertices * 3];
			memcpy(Ret->normal, sMesh->mNormals, sizeof(aiVector3D) * sMesh->mNumVertices);
		}



		if (sMesh->HasTextureCoords(0))
		{
			LOG("Loading Texture UVS from ASSIMP");
			Ret->size = sMesh->mNumVertices * 3;
			Ret->text_uvs = new float[Ret->size];
			memcpy(Ret->text_uvs, sMesh->mTextureCoords[0], Ret->size * sizeof(float));
			glGenBuffers(1, (GLuint*) & (Ret->id_uvs));
			glBindBuffer(GL_ARRAY_BUFFER, Ret->id_uvs);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * Ret->size, Ret->text_uvs, GL_STATIC_DRAW);

			LOG("New mesh with %d texture uvs", Ret->size);
		}

		if (pScene->HasMaterials())
		{
			LOG("Loading Materials from ASSIMP")
			aiMaterial* material = pScene->mMaterials[sMesh->mMaterialIndex];
			uint numTextures = material->GetTextureCount(aiTextureType_DIFFUSE);

			aiString path;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &path);

			Ret->GO->texture_path = path.C_Str();

			Ret->GO->CreateComponent(TEXTURE);

			LOG("Mesh texture with path: %s", Ret->GO->texture_path.c_str());

		}

	}
	else {
		printf("Error parsing '%s': '%s'\n", Filename.c_str(), Importer.GetErrorString());
	}

	return true;
}




void ModuleImporter::LoadTexture(const std::string & Filename, Component_Texture* tex)
{
	ilutRenderer(ILUT_OPENGL);
	std::string R_Filename;


	if (tex->GO->path != "" && Filename.size() <= 25)
		R_Filename = SearchTheDoc(Filename, tex);
	else
	{

		R_Filename = CutTheDoc(Filename, tex);
	}
		


	LOG("Loading texture with the actual filename %s", R_Filename.c_str());
	ILuint text_nm = 0;

	ilGenImages(1, &text_nm);
	ilBindImage(text_nm);

	if (ilLoadImage(R_Filename.c_str()) == IL_FALSE)
	{
		ILenum er = ilGetError();
		tex->GO->texture = false;
		LOG("Error in the parsing of the string of the texture");
		/*LOG("ERROR: %s", iluErrorString(er));*/
	}

	else
	{

		tex->id_texture = ilutGLBindTexImage();
		tex->height = ilGetInteger(IL_IMAGE_WIDTH);
		tex->widht = ilGetInteger(IL_IMAGE_HEIGHT);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
		LOG("Texture correctly loaded %s", R_Filename.c_str());
	}

	ilDeleteImages(1, &text_nm);

}


std::string ModuleImporter::SearchTheDoc(const std::string & Filename, Component_Texture* tex)
{
	std::string doc;
	std::size_t found = tex->GO->path.find_last_of("/\\");
	doc = tex->GO->path.substr(0,found+1) + Filename;



	return doc;
}

std::string ModuleImporter::CutTheDoc(const std::string & Filename, Component_Texture * tex)
{
	std::string doc = Filename;

	std::size_t d = Filename.find_first_of("C:");
	if (d == std::string::npos) 
	{
		std::size_t helepr = Filename.find_last_of("/\\");
		std::size_t found = tex->GO->path.find_last_of("/\\");
		doc = tex->GO->path.substr(0, found + 1) + Filename.substr(helepr+1);
	}

	return doc;
}

bool ModuleImporter::CleanUp()
{
	//Debugger
	aiDetachAllLogStreams();
	return true;
}
