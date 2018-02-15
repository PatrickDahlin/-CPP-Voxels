#ifndef _OCTREE_HPP_
#define _OCTREE_HPP_

/*

	Very @TODO
	An Octree could improve memory efficiency for
	Voxel terrain so this is a future optimization
	But for now I'll just leave this here and use
	flat arrays to begin with

*/

template <typename T>
struct Octree_Node<T> {
	T*				value = nullptr;
	Octree_Node*	sub_nodes = nullptr; // pointer to array

	bool has_children();


};

template <typename T>
class Octree<T> {
public:
	Octree();
	~Octree();

	Octree_Node* get_root();

protected:

	Octree_Node*		root_node;

};

#endif