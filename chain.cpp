/**
 * File: chain.cpp
 * Description: student implementation for Chain functions
 *              for CPSC 221 2023W1 PA1
 *
 *              THIS FILE WILL BE SUBMITTED.
**/

#include <math.h>

#include "chain.h"
#include <vector>

/**
 * Constructs a Chain from an input image
 * Each Node will cover a nodedimension x nodedimension (in pixels)
 * region in the source image.
 * @param img the input image
 * @param nodedimension number of pixels along the width and height of the Block region
 * @pre input image's width and height are evenly divisible by nodedimension
**/
Chain::Chain(PNG& img, unsigned int nodedimension) {
	// complete your implementation below
	Block block;
	block.Build(img, 0, 0, nodedimension);
	Node *newNode = new Node(block);
	newNode->prev = NULL;
	newNode->next = NULL;
	NW = newNode;
	Node *temp = NW;
	SE = newNode;
	Node *prev = newNode;
	length_ = 0;

	for (unsigned int y = 0; y < img.height(); y += nodedimension) {
		for (unsigned int x = 0; x < img.width(); x += nodedimension) {
			Block b;
			b.Build(img, x, y, nodedimension);
			newNode = new Node(b);
			newNode->prev = prev;
			newNode->next = NULL;
			prev->next = newNode;
			prev = newNode;
			SE = newNode;
			length_++;
		}
	}
	NW = NW->next;
	NW->prev = NULL;
	delete temp;
	temp = NULL;
}

/**
 * Renders the Chain's pixel data into an output PNG.
 * The PNG should be sized according to the number of nodes
 * specified by the cols parameter.
 * It is possible that there may not be enough nodes to fill
 * the bottom row of the PNG. If this happens, just render
 * as many nodes as there are, from left to right, and leave
 * the rest of the row as opaque white pixels.
 * @pre this Chain is not empty
 * @param cols the number of Blocks to use for the width of the image
 * @param full whether to render all pixel data into a full-size image,
 *             or block averages into a tiny image
**/
PNG Chain::Render(unsigned int cols, bool full) {
	// replace the line below with your implementation
	unsigned int dimension = NW->data.Dimension();
	unsigned int width;
	unsigned int height;
	unsigned int rows = (length_ / cols);
	if (length_ % cols != 0) {
		rows++;
	}

	PNG png;
	Node *p = NW;

	if (full) {
		width =  dimension * cols;
		height = rows * dimension;
	} else {
		dimension = 1;
		width = cols;
		height = rows;
	}

	png = PNG(width, height);
	for (unsigned int y = 0; y < png.height(); y += dimension) {
		for (unsigned int x = 0; x < png.width(); x += dimension) {
			(*p).data.Render(png, x, y, full);
			p = p->next;
			if (p == NULL) {
				return png;
			}
		}
	}
	return png;
}

/**
 * Inserts a new Node containing ndata at the back of the Chain
**/
void Chain::InsertBack(const Block& ndata) {
	// complete your implementation below
    Node* newNode = new Node(ndata);
	SE->next = newNode;
	newNode->prev = SE;
	newNode->next = NULL;
	SE = newNode;
}

/**
 * Reverses the structure of the list. This must be done by
 * pointer assignments. You may not deallocate or allocate any Nodes.
 * Example:
 *	before:	NW -> A <-> B <-> C <-> D <-> E <-> F <-> G <-> H <- SE
 *  
 *  after:	NW -> H <-> G <-> F <-> E <-> D <-> C <-> B <-> A <- SE
**/
void Chain::Reverse() {
	// complete your implementation below
    Node *curr = NW;
	Node *temp = NW;
	Node *nextNode = NW->next;
	while (curr != NULL) {
		curr->next = curr->prev;
		curr->prev = nextNode;
		curr = nextNode;
		if (nextNode != NULL) {
			nextNode = nextNode->next;
		}
	}
	NW = SE;
	SE = temp;
}

/**
 * Rearranges the Node structure and internal pixel data to be flipped over a vertical axis.
 * This must be done using pointer assignments.
 * You may not deallocate or allocate any Nodes.
 * Example, length = 6:
 *  before, flipping with 3 rows (2 rows):
 *
 *		NW -> A> <-> B> <-> C> <->
 *            D> <-> E> <-> F> <- SE
 * 
 *  after, visualized with 3 rows (2 rows):
 * 
 *		NW -> <C <-> <B <-> <A <->
 *            <F <-> <E <-> <D <- SE
 * 
 * @param cols number of Blocks to use for the initial width of the image
 * @pre length_ is divisible by cols (i.e. no "partially-filled" rows)
**/
void Chain::FlipHorizontal(unsigned int cols) {
	// complete your implementation below

	// set up 2D vector of Nodes
    unsigned int rows = length_ / cols;
	if (length_ % cols != 0) {
		rows++;
	}
	vector<vector<Node *>> nodeVec;
	vector<Node *> nodeRow;
	Node *curr = NW;

	// adding Nodes to 2D vector
	for (unsigned int i = 0; i < rows; i++) {
		for (unsigned int j = 0; j < cols; j++) {
			nodeRow.push_back(curr);
			if (curr->next == NULL) {
				break;
			} else {
				curr = curr->next;
			}
		}
		nodeVec.push_back(nodeRow);
		nodeRow.clear();
	}

	// reverse rows in vector
	for (unsigned int i = 0; i < rows; i++) {
		for (unsigned int j=0; j < nodeVec[i].size()/2; j++) {
			swap(nodeVec[i][j], nodeVec[i][nodeVec[i].size()-1-j]);
		}
	}

	// reassign pointers in Nodes
	Node *p;
	for (unsigned int i = 0; i < rows; i++) {
		for (unsigned int j=0; j < cols; j++) {
			p = nodeVec[i][j];
			(*p).data.FlipHorizontal();
			
			if ((i == 0) && (j == 0)) {   // 1st element of new chain
				p->prev = NULL;
				NW = p;
			} else if (j == 0) {       // 1st of row
				p->prev = nodeVec[i-1][cols-1];
			} else {                                 // rest of row
				p->prev = nodeVec[i][j-1];
			}

			if ((i == rows-1) && (j == cols-1)) {   // last element
				p->next = NULL;
				SE = p;
				break;
			} else if (j == cols-1) {        // last of row
				p->next = nodeVec[i+1][0];
			} else {                         // rest of row
				p->next = nodeVec[i][j+1];
			}
		}
	}
}

/**
 * Rearranges the Node structure and internal pixel data to be flipped over a horizontal axis.
 * This must be done using pointer assignments.
 * You may not deallocate or allocate any Nodes.
 * Example, length = 6:
 *	before, flipping with 3 rows (2 rows):
 *                         
 *      NW -> A <-> B <-> C <->
 *            D <-> E <-> F <- SE
 * 
 *  after, visualized with 3 rows (2 rows):
 * 
 *      NW -> D <-> E <-> F <->
 *            A <-> B <-> C <- SE
 * 
 * @param cols number of Blocks to use for the initial width of the image
 * @pre length_ is divisible by cols (i.e. no "partially-filled" rows)
**/
void Chain::FlipVertical(unsigned int cols) {
	// complete your implementation below

    // set up 2D vector of Nodes
    unsigned int rows = length_ / cols;
	if (length_ % cols != 0) {
		rows++;
	}
	vector<vector<Node *>> nodeVec;
	vector<Node *> nodeRow;
	Node *curr = NW;

	// adding Nodes to 2D vector
	for (unsigned int i = 0; i < rows; i++) {
		for (unsigned int j = 0; j < cols; j++) {
			nodeRow.push_back(curr);
			if (curr->next == NULL) {
				break;
			} else {
				curr = curr->next;
			}
		}
		nodeVec.push_back(nodeRow);
		nodeRow.clear();
	}

	// reverse rows in vector
	for (unsigned int i = 0; i < rows/2; i++) {
		swap(nodeVec[i], nodeVec[rows-1-i]);
	}

	// reassign pointers in Nodes
	Node *p;
	for (unsigned int i = 0; i < rows; i++) {
		for (unsigned int j=0; j < cols; j++) {
			p = nodeVec[i][j];
			(*p).data.FlipVertical();
			
			if ((i == 0) && (j == 0)) {   // 1st element of new chain
				p->prev = NULL;
				NW = p;
			} else if (j == 0) {       // 1st of row
				p->prev = nodeVec[i-1][cols-1];
			} else {                                 // rest of row
				p->prev = nodeVec[i][j-1];
			}

			if ((i == rows-1) && (j == cols-1)) {   // last element
				p->next = NULL;
				SE = p;
				break;
			} else if (j == cols-1) {        // last of row
				p->next = nodeVec[i+1][0];
			} else {                         // rest of row
				p->next = nodeVec[i][j+1];
			}
		}
	}
}

/**
 * Produces a blocky, pixellated effect (in a rendered image) by replacing each
 * block's pixel data with the average color of the block.
**/
void Chain::Blockify() {
	// complete your implementation below
	Node *temp = NW;
	while (temp != NULL) {
		temp->data.FillAverage();
		temp = temp->next;
	}  
}

/**
 * Destroys all dynamically allocated memory associated with
 * this Chain object. Called by destructor and operator=.
 * You must complete its implementation for PA1.
**/
void Chain::Clear() {
	// complete your implementation below
	Node *temp = NW;
	while(NW != NULL) {
		NW = NW->next;
		delete temp;
		temp = NW;
	}
	delete temp;
	temp = NULL;
	SE = NULL;
}

/**
 * Copies the parameter other Chain into the current Chain.
 * Does not free any memory. Called by copy constructor and
 * operator=.
 * You must complete its implementation for PA1.
 * @param other The Chain to be copied.
**/
void Chain::Copy(const Chain& other) {
	// complete your implementation below
	length_ = other.Length();
	if (other.IsEmpty()) {
		return;
	}
	Block block;
	Node *newNode = new Node(block);
	newNode->prev = NULL;
	newNode->next = NULL;
	Node *temp = newNode;
	SE = newNode;

	Node *otherNode = other.NW;
	while (otherNode != NULL) {
		InsertBack((*otherNode).data);
		otherNode = otherNode->next;
	}
	NW = temp->next;
	delete temp;
	temp = NULL;
}

/**
 * If you have declared any private helper functions in chain_private.h,
 * add your completed implementations below.
**/

