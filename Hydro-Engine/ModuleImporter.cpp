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

void ModuleImporter::aiParentNode(const std::string & Filename)
{
	act_number_meshes = 0;
	Assimp::Importer Importer;

	const aiScene* pScene = Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcessPreset_TargetRealtime_MaxQuality); 
	if (pScene) {
		aiNode* root = pScene->mRootNode;
		App->scene_intro->CreateRootGameObject(root->mName.C_Str(), Filename);
	}
	else {
		printf("Error parsing '%s': '%s'\n", Filename.c_str(), Importer.GetErrorString());
	}

}

void ModuleImporter::NodeIterations(aiNode * parentNod, GameObject* act)
{
	aiVector3D translation, scaling;
	aiQuaternion rotation;
	// Transformation ------------
	parentNod->mTransformation.Decompose(scaling, rotation, translation);

	
	int _max = max(scaling.x, scaling.y);
	_max = max(_max, scaling.z);

	float3 pos(translation.x, translation.y, translation.z);
	float3 scale(scaling.x / _max, scaling.y / _max, scaling.z / _max);
	Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);

	if (act->DoIhave(TRANSFORM))
	{
		act->transform->LoadTransform(pos, scale, rot);
		act->transform->NewTransform();
	}
	// Transformation --------------
	

	for (uint i = 0; i < parentNod->mNumChildren; i++)
	{
		aiNode* child = parentNod->mChildren[i];
		for (uint i = 0; i < child->mNumMeshes; i++)
		{
			std::string name;
			name = child->mName.C_Str() + i;
			act->CreateChildren(name, act->path, act_number_meshes + i); //gtodo: si crea multiples meshes no aplicara la correspondiente transformacion.
		}
		if (child->mNumMeshes == 0)
		{
			std::string name;
			name = child->mName.C_Str() + i;
			act->CreateEmptyChild(name, act->path);

		}
		act_number_meshes += child->mNumMeshes;
		NodeIterations(child, act->childrens[i]);
	}
}

void ModuleImporter::CreateGO(const std::string & Filename, GameObject * act)
{
	Assimp::Importer Importer;

	const aiScene* pScene = Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcessPreset_TargetRealtime_MaxQuality);
	if (pScene) {
		aiNode* root = pScene->mRootNode;
		NodeIterations(root, act);
	}
	else {
		printf("Error parsing '%s': '%s'\n", Filename.c_str(), Importer.GetErrorString());
	}
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
			Ret->Has_normals = true;
			Ret->normal = new float[sMesh->mNumVertices * 3];
			memcpy(Ret->normal, sMesh->mNormals, sizeof(aiVector3D) * sMesh->mNumVertices);
		}



		if (sMesh->HasTextureCoords(0))
		{
			Ret->Has_tex_coords = true;
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

void ModuleImporter::ImportMeshOwnFile(const char * name, Component_Mesh * Mesh)
{
	// here we create data
	uint header[4];
	uint vert_num = 0, ind_num = 0;
	bool has_normals = false, has_texcoords = false;
	// ....

	// we load all the data
	if (Mesh->Has_normals)
		has_normals = true;

	if (Mesh->Has_tex_coords)
		has_texcoords = true;

	vert_num = Mesh->num_vertex;
	ind_num = Mesh->num_index;
	

	header[0] = vert_num;
	header[1] = ind_num;
	header[2] = has_normals;
	header[3] = has_texcoords;

	// Knowing the size of the file, we can create the buffer in which it all will be stored
	uint size = sizeof(header);					// header.
	size += sizeof(uint) * ind_num;				// index.
	size += sizeof(float) * (vert_num * 3);		// vertices.
	
	if (has_normals)
		size += sizeof(float) * (vert_num * 3); //normals.

	if (has_texcoords)
		size += sizeof(float) * (vert_num * 3); //texture coords.

	// ....

	char* data = new char[size];
	char* cursor = data;

	// First we store the header
	uint bytes = sizeof(header);
	memcpy(cursor, header, bytes);
	cursor += bytes;

	// Vertices
	const float* verti = Mesh->vertex;
	bytes = sizeof(float) * (vert_num * 3);
	memcpy(cursor, verti, bytes);
	cursor += bytes;

	// Index
	const uint* ind = Mesh->index;
	bytes = sizeof(uint) * Mesh->num_index;
	memcpy(cursor, ind, bytes);
	cursor += bytes;

	// Normals
	if (has_normals)
	{
		const float* normals = Mesh->normal;
		bytes = sizeof(float) * (vert_num * 3);
		memcpy(cursor, normals, bytes);
		cursor += bytes;
	}

	// Tex coords
	if (has_texcoords)
	{
		const float* tex_coords = Mesh->text_uvs;
		bytes = sizeof(float) * (vert_num * 3);
		memcpy(cursor, tex_coords, bytes);
	}

	std::string filename = name;



	App->file_system->GetActualName(filename);
	App->file_system->SaveUnique(output_file, data, size, LIBRARY_TEXTURES_FOLDER, "texture", "dds");


	delete data;
}

bool ModuleImporter::CleanUp()
{
	//Debugger
	aiDetachAllLogStreams();
	return true;
}
