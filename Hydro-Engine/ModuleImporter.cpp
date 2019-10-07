#include "Application.h"
#include "ModuleImporter.h"
#include "Globals.h"




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

	LoadFBX("Assets/warrior.FBX");

	return true;
}

bool ModuleImporter::LoadFBX(const std::string & Filename)
{
	bool Ret = true;
	
	Assimp::Importer Importer;

	const aiScene* pScene = Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcessPreset_TargetRealtime_MaxQuality); // el aiProcess_Triangulate sirve para transformar las caras cuadras en triangulos

	if (pScene) {
		Ret = SceneToMesh(pScene);
	}
	else {
		printf("Error parsing '%s': '%s'\n", Filename.c_str(), Importer.GetErrorString());
	}

	return Ret;
}

bool ModuleImporter::SceneToMesh(const aiScene * FBXScene)
{
	//mNumMeshes sirve para entrar en el tamaño, y mMeshes sirve para entrar en la array de meshes de la escena.
	// Para ir una por una y separar las diversas meshes.
	for (uint i = 0; i < FBXScene->mNumMeshes; i++) {
		const aiMesh* sMesh = FBXScene->mMeshes[i];
		InitMesh(i, sMesh);
	}
	return true;
}

void ModuleImporter::InitMesh(uint Index, const aiMesh * sMesh)
{
	Mesh SceneMesh;

	SceneMesh.num_vertex = sMesh->mNumVertices;
	SceneMesh.vertex = new float[SceneMesh.num_vertex * 3]; // Lo multiplicamos por 3 porque hay 3 valores por vertice
	memcpy(SceneMesh.vertex, sMesh->mVertices, sizeof(float) * SceneMesh.num_vertex * 3); // Lo multiplicamos por el sizeof(float) yaque el memcpy trabaja en malditos bits.
	

	 // copy faces
	if (sMesh->HasFaces())
	{
		SceneMesh.num_index = sMesh->mNumFaces * 3;
		SceneMesh.index = new uint[SceneMesh.num_index]; // assume each face is a triangle // Que esto a la vez ya lo hacemos antes al cargar el fbx.
		for (uint i = 0; i < sMesh->mNumFaces; ++i)
		{
			if (sMesh->mFaces[i].mNumIndices != 3)
			{
				LOG("WARNING, geometry face with != 3 indices!");
			}
			else
				memcpy(&SceneMesh.index[i * 3], sMesh->mFaces[i].mIndices, 3 * sizeof(uint));
		}
	}
	scene_meshesh_xd.push_back(&SceneMesh);
}

update_status ModuleImporter::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleImporter::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleImporter::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleImporter::CleanUp()
{
	//Debugger
	aiDetachAllLogStreams();
	return true;
}
