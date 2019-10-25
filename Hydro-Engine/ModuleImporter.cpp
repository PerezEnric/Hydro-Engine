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
	ilInit();
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
		//Lets start?
		Ret->num_vertex = sMesh->mNumVertices;
		Ret->vertex = new float[Ret->num_vertex * 3]; 
		memcpy(Ret->vertex, sMesh->mVertices, sizeof(float) * Ret->num_vertex * 3); 


		glGenBuffers(1, &Ret->id_vertex);
		glBindBuffer(GL_ARRAY_BUFFER, Ret->id_vertex);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * Ret->num_vertex * 3, Ret->vertex, GL_STATIC_DRAW);



		// copy faces
		if (sMesh->HasFaces())
		{
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
		}

		if (sMesh->HasNormals())
		{
			Ret->normal = new float[sMesh->mNumVertices * 3];
			memcpy(Ret->normal, sMesh->mNormals, sizeof(aiVector3D) * sMesh->mNumVertices);
		}



		if (sMesh->HasTextureCoords(0))
		{
			Ret->size = sMesh->mNumVertices * 3;
			Ret->text_uvs = new float[Ret->size];
			memcpy(Ret->text_uvs, sMesh->mTextureCoords[0], Ret->size * sizeof(float));
			glGenBuffers(1, (GLuint*) & (Ret->id_uvs));
			glBindBuffer(GL_ARRAY_BUFFER, Ret->id_uvs);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * Ret->size, Ret->text_uvs, GL_STATIC_DRAW);

		}

		if (pScene->HasMaterials())
		{
			aiMaterial* material = pScene->mMaterials[sMesh->mMaterialIndex];
			uint numTextures = material->GetTextureCount(aiTextureType_DIFFUSE);

			aiString path;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &path);

			Ret->GO->texture_path = path.C_Str();

			Ret->GO->CreateComponent(TEXTURE);

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
		R_Filename = Filename;

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

bool ModuleImporter::CleanUp()
{
	//Debugger
	aiDetachAllLogStreams();
	return true;
}
