#pragma once
#include <vector>

#include "MathGeoLib/include/Geometry/AABB.h"


class GameObject;

// Quad Tree Node abreviated.
struct QTN 
{
	//Constructor (0 game objects by now)
	QTN(AABB bound_limits);
	~QTN();

	// With this function we insert a gameobject to the node.
	bool InsertGO(GameObject* obj);

	// With this function we cut the current QTN to 4.
	void CutTo4();
	void Draw();
	void CleanUp();

	// Recursivity D:
	QTN* child[4];
	AABB my_box;
	int my_bucketsize;

	//This is important cause the leaf will be the last depth of QTN we have (where we really allocate the childs)
	bool im_leaf = false;
	std::vector<GameObject*> my_gos;

	void Intersect(std::vector<GameObject*>& did_i_found_intersects, Frustum primitive);
};

class QT
{
public:
	QT(AABB bound_limits, int bucketsize = 1);
	~QT();

	// We create a new QTN
	void Create(AABB limits, int bucket_size = 1);

	// Insert an object into the quadtree
	bool Insert(GameObject* object);		

	//Multiple Inserts of GO
	void Fill(std::vector<GameObject*> my_gos); 

	// We cleanUp the quadTree
	void CleanUp();
	void DebugDraw();

	QTN* root; // Root of the quadtree
	int bucketsize;	// How much items can be held in a QuadTreeNode

	void Intersect(std::vector<GameObject*>& did_i_found_intersects, Frustum primitive);

	/*void intersects();*/
};