#include "Application.h"
#include "ModuleImporter.h"
#include "Globals.h"
#include "ModuleInput.h"

#include "glmath.h"
//#include "MathGeoLib/include/MathGeoLib.h"
//#include "MathGeoLib/include/MathBuildConfig.h"
//#include "MathGeoLib/include/MathGeoLibFwd.h"




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

	//LoadFBX("Assets/warrior.FBX");

	return true;
}

bool ModuleImporter::LoadFBX(const std::string & Filename)
{
	bool Ret = true;
	
	Assimp::Importer Importer;

	const aiScene* pScene = Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcessPreset_TargetRealtime_MaxQuality); // el aiProcess_Triangulate sirve para transformar las caras cuadras en triangulos
	//const aiScene* aScene = aiImportFile(Filename.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
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
	

	glGenBuffers(1, &SceneMesh.id_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, SceneMesh.id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * SceneMesh.num_vertex * 3, SceneMesh.vertex, GL_STATIC_DRAW);

	

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
			{
				memcpy(&SceneMesh.index[i * 3], sMesh->mFaces[i].mIndices, 3 * sizeof(uint));
				glGenBuffers(1, &SceneMesh.id_index);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, SceneMesh.id_index);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * (SceneMesh.num_index), SceneMesh.index, GL_STATIC_DRAW);
			}
			
		}
	}

	if (sMesh->HasNormals())
	{
		SceneMesh.normal = new float[sMesh->mNumVertices*3];
		memcpy(SceneMesh.normal, sMesh->mNormals, sizeof(aiVector3D) * sMesh->mNumVertices);
	}


	// Normals.
	_amesh.push_back(SceneMesh);
}

void ModuleImporter::RenderAll()
{
	for (uint i = 0; i < _amesh.size(); i++) {

		glEnableClientState(GL_VERTEX_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, _amesh[i].id_vertex);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _amesh[i].id_index);

		glVertexPointer(3, GL_FLOAT, 0, NULL);

		glDrawElements(GL_TRIANGLES, _amesh[i].num_index, GL_UNSIGNED_INT, NULL);
		glDisableClientState(GL_VERTEX_ARRAY);

	}

	//RenderNormals();
	RenderFaceNormals();



}

void ModuleImporter::RenderNormals()
{
	for (uint z = 0; z < _amesh.size(); z++) {

	glColor3f(0.2f, 1.f, 0.25f);
	uint j = 0;
	float lenght = 2;

		for (uint i = 0; i < _amesh[z].num_vertex * 3; i += 3)
		{

			glBegin(GL_LINES);
			glVertex3f(_amesh[z].vertex[i], _amesh[z].vertex[i + 1], _amesh[z].vertex[i + 2]);
			glVertex3f(_amesh[z].vertex[i] + _amesh[z].normal[i], _amesh[z].vertex[i + 1] + _amesh[z].normal[i+1]*lenght, _amesh[z].vertex[i + 2] + _amesh[z].normal[i + 2]*lenght);
			++j;
			glEnd();
		}
	glColor3f(1, 1, 1);
	}
	

}

void ModuleImporter::RenderFaceNormals()
{
	int l = 2;
	for (uint m = 0; m < _amesh.size(); m++) // m de mesh xd.
	{
		for (uint i = 0; i < _amesh[m].num_index; i += 3) {
			// Calculo de vectores. ----------------------------------------------------------------------------------
			uint c_i = _amesh[m].index[i]*3; // Dentro de la mesh y dentro del cuadrado en la que estamos cojemos el primer indice del cuadrado.
			// Estas son las coordenadas del primer vector.
			vec3 a(_amesh[m].vertex[c_i], _amesh[m].vertex[c_i+1], _amesh[m].vertex[c_i+2]);
			
			c_i = _amesh[m].index[i + 1]*3; // aqui hacemos que el current index apunte al segundo indice del cuadrado;

			vec3 b(_amesh[m].vertex[c_i], _amesh[m].vertex[c_i + 1], _amesh[m].vertex[c_i + 2]);

			c_i = _amesh[m].index[i + 2]*3; // aqui hacemos que el current index apunte al tercer indice del cuadrado;

			vec3 c(_amesh[m].vertex[c_i], _amesh[m].vertex[c_i + 1], _amesh[m].vertex[c_i + 2]);
			//	Calculo de vectores. ----------------------------------------------------------------------------------
			
			// Calculo del centro del triangulo. -------------------------------------------------------
			vec3 tri_cen;
			tri_cen.x = (a.x + b.x + c.x) / 3;
			tri_cen.y = (a.y + b.y + c.y) / 3; // El centro del triangulo lo calculamos sumando todos sus puntos y diviendolo entre 3.
			tri_cen.z = (a.z + b.z + c.z) / 3;
			// Calculo del centro del triangulo. --------------------------------------------------------

			// Calculo del plano atraves de 2 vectores. -------------------------------------------------
			vec3 a_cen = a - tri_cen;
			vec3 b_cen = b - tri_cen; 
			// Ahora tenemos el vector a_cen que va desde el centro hasta el punto "a" y el b_cen que es lo mismo pero con "b".
			vec3 orth_vec = OrthogonalVect(a_cen, b_cen);
			// Calculo del plano atraves de 2 vectores. -------------------------------------------------
			orth_vec = normalize({ orth_vec.x,orth_vec.y,orth_vec.z });
			orth_vec *= l;
			
			glColor3f(1.f, 1.f, 0.25f);
			glBegin(GL_LINES);
			glVertex3f(tri_cen.x, tri_cen.y, tri_cen.z);
			glVertex3f(tri_cen.x - orth_vec.x, tri_cen.y - orth_vec.y, tri_cen.z - orth_vec.z);
			glEnd();


		}
		
	}

}

vec3 ModuleImporter::OrthogonalVect(const vec3 a, const vec3 b)
{
	vec3 ret;


	ret.x = ((a.y*b.z) - (b.y*a.z));
	ret.y = ((b.x*a.z) - (b.z*a.x));
	ret.z = ((a.x*b.y) - (a.y*b.x));


	return ret;
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
	
	RenderAll();
	return UPDATE_CONTINUE;
}

bool ModuleImporter::CleanUp()
{
	//Debugger
	aiDetachAllLogStreams();
	return true;
}
