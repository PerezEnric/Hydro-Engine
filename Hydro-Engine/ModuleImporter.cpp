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
	std::string R_Filename = local_doc + Filename;


	ILuint text_nm = 0;

	ilGenImages(1, &text_nm);
	ilBindImage(text_nm);

	if (ilLoadImage(R_Filename.c_str()) == IL_FALSE)
	{
		ILenum er = ilGetError();
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



void ModuleImporter::RenderNormals()
{
	/*int lenght = 2;

	for (uint z = 0; z < _amesh.size(); z++) {

	glColor3f(0.2f, 1.f, 0.25f);
	uint j = 0;
	

		for (uint i = 0; i < _amesh[z].num_vertex * 3; i += 3)
		{

			glBegin(GL_LINES);
			glVertex3f(_amesh[z].vertex[i], _amesh[z].vertex[i + 1], _amesh[z].vertex[i + 2]);
			glVertex3f(_amesh[z].vertex[i] + _amesh[z].normal[i], _amesh[z].vertex[i + 1] + _amesh[z].normal[i+1]*lenght, _amesh[z].vertex[i + 2] + _amesh[z].normal[i + 2]*lenght);
			++j;
			glEnd();
		}
	glColor3f(1, 1, 1);
	}*/
	

}

void ModuleImporter::RenderFaceNormals()
{
	//int l = 2;
	//for (uint m = 0; m < _amesh.size(); m++) // m de mesh xd.
	//{
	//	for (uint i = 0; i < _amesh[m].num_index; i += 3) {
	//		// Calculo de vectores. ----------------------------------------------------------------------------------
	//		uint c_i = _amesh[m].index[i]*3; // Dentro de la mesh y dentro del cuadrado en la que estamos cojemos el primer indice del cuadrado.
	//		// Estas son las coordenadas del primer vector.
	//		vec3 a(_amesh[m].vertex[c_i], _amesh[m].vertex[c_i+1], _amesh[m].vertex[c_i+2]);
	//		
	//		c_i = _amesh[m].index[i + 1]*3; // aqui hacemos que el current index apunte al segundo indice del cuadrado;

	//		vec3 b(_amesh[m].vertex[c_i], _amesh[m].vertex[c_i + 1], _amesh[m].vertex[c_i + 2]);

	//		c_i = _amesh[m].index[i + 2]*3; // aqui hacemos que el current index apunte al tercer indice del cuadrado;

	//		vec3 c(_amesh[m].vertex[c_i], _amesh[m].vertex[c_i + 1], _amesh[m].vertex[c_i + 2]);
	//		//	Calculo de vectores. ----------------------------------------------------------------------------------
	//		
	//		// Calculo del centro del triangulo. -------------------------------------------------------
	//		vec3 tri_cen;
	//		tri_cen.x = (a.x + b.x + c.x) / 3;
	//		tri_cen.y = (a.y + b.y + c.y) / 3; // El centro del triangulo lo calculamos sumando todos sus puntos y diviendolo entre 3.
	//		tri_cen.z = (a.z + b.z + c.z) / 3;
	//		// Calculo del centro del triangulo. --------------------------------------------------------
	//		
	//		// Calculo del plano atraves de 2 vectores. -------------------------------------------------
	//		vec3 a_cen = a - tri_cen;
	//		vec3 b_cen = b - tri_cen; 
	//		// Ahora tenemos el vector a_cen que va desde el centro hasta el punto "a" y el b_cen que es lo mismo pero con "b".
	//		vec3 orth_vec = OrthogonalVect(a_cen, b_cen);
	//		// Calculo del plano atraves de 2 vectores. -------------------------------------------------
	//		orth_vec = normalize({ orth_vec.x,orth_vec.y,orth_vec.z });
	//		orth_vec *= l;
	//		
	//		glColor3f(1.f, 1.f, 0.25f);
	//		glBegin(GL_LINES);
	//		glVertex3f(tri_cen.x, tri_cen.y, tri_cen.z);
	//		glVertex3f(tri_cen.x - orth_vec.x, tri_cen.y - orth_vec.y, tri_cen.z - orth_vec.z);
	//		glEnd();
	//		glColor3f(1, 1, 1);
	//	}
	//}

}

vec3 ModuleImporter::OrthogonalVect(const vec3 a, const vec3 b)
{
	vec3 ret;


	ret.x = ((a.y*b.z) - (b.y*a.z));
	ret.y = ((b.x*a.z) - (b.z*a.x));
	ret.z = ((a.x*b.y) - (a.y*b.x));


	return ret;
}

bool ModuleImporter::CleanUp()
{
	//Debugger
	aiDetachAllLogStreams();
	return true;
}
