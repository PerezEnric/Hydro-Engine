#include "QuadTree.h"
#include "GameObject.h"
#include "Component_Mesh.h"

QTN::QTN(AABB bound_limits)
{
	my_box = bound_limits;
	im_leaf = true; // always when we create a new QTN this will be leaf (when he cuts himself in 4 he degrades to branch)
	for (int i = 0; i < 4; i++)
		child[i] = nullptr;
}

QTN::~QTN()
{
}

bool QTN::InsertGO(GameObject * obj)
{
	bool ret = false;
	AABB obj_box = obj->my_mesh->mesh_bbox;
	
	if (!my_box.Intersects(obj_box)) 	// If the gameobject is not in this node we do nothing
		return ret;

	if (im_leaf && (my_gos.size() < my_bucketsize)) // Here we check if first we are a leaf and if we are full or not
		my_gos.push_back(obj);
	else {
		if (im_leaf) {						
			im_leaf = false; // first of all we will not be anymore a leaf D:

			CutTo4();// then we cut ourself in 4 parts

			for (int i = 0; i < my_gos.size(); i++)
				for (int j = 0; j < 4; j++)
					child[j]->InsertGO(my_gos[i]);	// If a child can already hold the object break the loop

			my_gos.clear();
		}

		for (int i = 0; i < 4; i++)									
			child[i]->InsertGO(obj); 	//if we aint a leaf we will pass the gameobject to our childs		
		
		ret = true;
	}

	return ret;
}

void QTN::CutTo4()
{
	
	float3 center = my_box.CenterPoint();


	AABB split_box;
	float3 new_centre;
	float3 size(my_box.Size());
	float3 new_box_size = float3(my_box.HalfSize().x, size.y, my_box.HalfSize().z);

	// Top left
	new_centre = center + float3(-my_box.HalfSize().x / 2, 0, my_box.HalfSize().z / 2);
	split_box.SetFromCenterAndSize(new_centre, new_box_size);
	child[0] = new QTN(split_box);

	// Top right
	new_centre = center + float3(my_box.HalfSize().x / 2, 0, my_box.HalfSize().z / 2);
	split_box.SetFromCenterAndSize(new_centre, new_box_size);
	child[1] = new QTN(split_box);

	//Bottom left
	new_centre = center + float3(-my_box.HalfSize().x / 2, 0, -my_box.HalfSize().z / 2);
	split_box.SetFromCenterAndSize(new_centre, new_box_size);
	child[2] = new QTN(split_box);

	//Bottom right
	new_centre = center + float3(my_box.HalfSize().x / 2, 0, -my_box.HalfSize().z / 2);
	split_box.SetFromCenterAndSize(new_centre, new_box_size);
	child[3] = new QTN(split_box);
}

void QTN::Draw()
{
	// gtodo

}

void QTN::CleanUp()
{
	if (!im_leaf)
	{
		for (int i = 0; i < 4; i++)
			child[i]->CleanUp();
		/*for (int i = 0; i < my_gos.size(); i++)
		{

		}*/ // I dont know if this is correct or not Gtest.
	}
}

QT::QT(AABB bound_limits, int bucketsize)
{
	Create(bound_limits, bucketsize);
}

QT::~QT()
{
}

void QT::Create(AABB limits, int bucket_size)
{
	//first we delete the root
	if (root != nullptr)
		delete root;

	//then we create a new root
	root = new QTN(limits);
	root->my_bucketsize = bucketsize;

}

bool QT::Insert(GameObject * object)
{
	bool ret = false;
	ret = root->InsertGO(object);
	return ret;
}

void QT::Fill(std::vector<GameObject*> my_gos)
{

	for (int i = 0; i < my_gos.size(); i++) 
	{
		Insert(my_gos[i]);
	}

}

void QT::CleanUp()
{
	if (root != nullptr)
	{
		root->CleanUp();
		root = nullptr;
	}
}

void QT::DebugDraw()
{
	root->Draw();
}