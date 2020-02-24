#include "Vector3D.h"
#include "internalMapping.h"
void internalMapping::createMap() {
	map = "";
	//nodes = new sphereNode[nodesAmount];
	//delete[] nodes;
	for (int i = 0; i < nodesAmount; i++) {
		vector3D p = vector3D(r*i, 0, 0);
		map += p.serialize();//nodes[i].position.serialize();
		nodes[i] = sphereNode();
		nodes[i].position = p;

	}
}

String internalMapping::createMap(vector3D position) {
	map = "";

	nodesAmount = (int)ceil(position.x / r) +1; //Rounds upwards
	//nodes = new sphereNode[nodesAmount];
	if (nodesAmount > 10) {
		nodesAmount = 10;
	}
	//delete[] nodes;
	for (int i = 0; i < nodesAmount; i++) {
		vector3D p = vector3D(r*i, 0, 0);
		nodes[i].position = p;
		map += nodes[i].position.serialize();
	}
	return "reset";
}

void internalMapping:: unbound() {
	for (int i = 0; i < nodesAmount; i++) {
		nodes[i].traversable = true;
	}
}

void internalMapping:: object(vector3D point) {
	for (int i = 0; i < nodesAmount; i++) {
		if (nodes[i].bounded(point, r)) {
			nodes[i].traversable = false;
		}
	}
}

int internalMapping:: locator(vector3D point) {
	for (int i = 0; i < nodesAmount; i++) {
		if (nodes[i].bounded(point, r)) {
			return i;
		}
	}
	return -1;
}

String internalMapping::serialize() {
	return (String)"m_p" + map;
}