#include "QuadTree.h"

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
	return false;
}

void QTN::CutTo4()
{
}

void QTN::Draw()
{
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
	
}
