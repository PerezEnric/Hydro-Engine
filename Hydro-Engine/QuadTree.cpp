#include "QuadTree.h"
#include "GameObject.h"
#include "Component_Mesh.h"
#include "MathGeoLib/include/Geometry/AABB.h"



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
		delete[] root;

	//then we create a new root
	root = new QTN(limits);
	root->my_bucketsize = bucket_size;
	root->my_number_of_QTN = 1;

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

void QT::Draw()
{
	root->Draw();
}

void QT::Intersect(std::vector<GameObject*>& did_i_found_intersects, Frustum primitive) // gtodo i should swap this to a type var to be more generic
{
	root->Intersect(did_i_found_intersects, primitive);
}


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
	if (obj->b_mesh)
	{
		obj->my_mesh->recboubox();

		AABB obj_box = obj->my_mesh->obb_box;
	
		if (!my_box.Intersects(obj_box)) 	// If the gameobject is not in this node we do nothing
			return ret;

		if ((im_leaf && (my_gos.size() < my_bucketsize)) || my_number_of_QTN >= max_number_of_QTN) // Here we check if first we are a leaf and if we are full or not and if we have arrived at the limit of divisions
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
	child[0]->my_bucketsize = my_bucketsize;
	child[0]->my_number_of_QTN = my_number_of_QTN + 1;

	// Top right
	new_centre = center + float3(my_box.HalfSize().x / 2, 0, my_box.HalfSize().z / 2);
	split_box.SetFromCenterAndSize(new_centre, new_box_size);
	child[1] = new QTN(split_box);
	child[1]->my_bucketsize = my_bucketsize;
	child[1]->my_number_of_QTN = my_number_of_QTN + 1;

	//Bottom left
	new_centre = center + float3(-my_box.HalfSize().x / 2, 0, -my_box.HalfSize().z / 2);
	split_box.SetFromCenterAndSize(new_centre, new_box_size);
	child[2] = new QTN(split_box);
	child[2]->my_bucketsize = my_bucketsize;
	child[2]->my_number_of_QTN = my_number_of_QTN + 1;

	//Bottom right
	new_centre = center + float3(my_box.HalfSize().x / 2, 0, -my_box.HalfSize().z / 2);
	split_box.SetFromCenterAndSize(new_centre, new_box_size);
	child[3] = new QTN(split_box);
	child[3]->my_bucketsize = my_bucketsize;
	child[3]->my_number_of_QTN = my_number_of_QTN + 1;
}

void QTN::Draw()
{
	float3 down_l = { my_box.minPoint.x, my_box.minPoint.y,my_box.minPoint.z };
	float3 top_r = { my_box.maxPoint.x, my_box.maxPoint.y,my_box.maxPoint.z };


	glColor3f(0.0f, 1.0f, 1.0f);
	glBegin(GL_LINES);

	//the 6 faces of a Aabb:
	glVertex3f(down_l.x, down_l.y, down_l.z);		glVertex3f(top_r.x, down_l.y, down_l.z);
	glVertex3f(down_l.x, down_l.y, down_l.z);		glVertex3f(down_l.x, top_r.y, down_l.z);
	glVertex3f(top_r.x, down_l.y, down_l.z);		glVertex3f(top_r.x, top_r.y, down_l.z);
	glVertex3f(down_l.x, top_r.y, down_l.z);		glVertex3f(top_r.x, top_r.y, down_l.z);

	glVertex3f(top_r.x, top_r.y, top_r.z);			glVertex3f(down_l.x, top_r.y, top_r.z);
	glVertex3f(top_r.x, top_r.y, top_r.z);			glVertex3f(top_r.x, down_l.y, top_r.z);
	glVertex3f(down_l.x, top_r.y, top_r.z);			glVertex3f(down_l.x, down_l.y, top_r.z);
	glVertex3f(top_r.x, down_l.y, top_r.z);			glVertex3f(down_l.x, down_l.y, top_r.z);

	glVertex3f(down_l.x, down_l.y, down_l.z);		glVertex3f(down_l.x, down_l.y, top_r.z);
	glVertex3f(top_r.x, top_r.y, down_l.z);			glVertex3f(top_r.x, top_r.y, top_r.z);
	glVertex3f(top_r.x, down_l.y, down_l.z);		glVertex3f(top_r.x, down_l.y, top_r.z);
	glVertex3f(down_l.x, top_r.y, down_l.z);		glVertex3f(down_l.x, top_r.y, top_r.z);

	glEnd();
	glColor3f(1.0f, 1.0f, 1.0f);

	for (uint i = 0; i < 4; i++)
	{
		if (child[i] != nullptr)
			child[i]->Draw();
	}

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

void QTN::Intersect(std::vector<GameObject*>& did_i_found_intersects, Frustum primitive)
{
	if (my_box.Intersects(primitive)) {
		if (!im_leaf) {	// As always we only wanna test it on leafs.												
			for (int i = 0; i < 4; i++)
				child[i]->Intersect(did_i_found_intersects, primitive);
		}
		else {
			for (int i = 0; i < my_gos.size(); i++) {
				bool alredy = false;
				for (int j = 0; j < did_i_found_intersects.size(); j++)
				{
					if (did_i_found_intersects[j]->my_uuid == my_gos[i]->my_uuid)
						alredy = true;
				}
				if (!alredy)
					did_i_found_intersects.push_back(my_gos[i]);
			}
		}
	}
}
