/**
 * File: block.cpp
 * Description: student implementation for Block functions
 *              for CPSC 221 2023W1 PA1
 *
 *              THIS FILE WILL BE SUBMITTED.
**/

#include "block.h"

/**
 * Creates a square Block that is dimension x dimension pixels in size,
 * starting at (x, y) on img. Assumes that the requested
 * Block fits on the image (inputs are feasible)
**/
void Block::Build(PNG& img, unsigned int x, unsigned int y, unsigned int dimension) {
	// complete your implementation below
	RGBAPixel *p;
	vector<RGBAPixel> row;
	for (unsigned int a = 0; a < dimension; a++) {
		for (unsigned int b = 0; b < dimension; b++) {
			p = img.getPixel(x+b, y+a);
			row.push_back(*p); 
		}
		data.push_back(row);
		row.clear();
	}
}

/**
 * Renders this Block onto img with its upper-left corner at (x, y).
 * Assumes block fits on the image.
 * 
 * @param full - whether to render the entire contents to the target PNG,
 *               or the average color to a single pixel in the target PNG.
**/
void Block::Render(PNG& img, unsigned int x, unsigned int y, bool full) const {
	// complete your implementation below
	RGBAPixel *p;
	if (full) {
		for (unsigned long i = 0; i < data.size(); i++) {
			for (unsigned long j = 0; j < data.size(); j++) {
				p = img.getPixel(j+x, i+y);
				*p = data[i][j];
			}
		}
	} else {
		RGBAPixel avgColour = GetAverageColor();
		p = img.getPixel(x, y);
		*p = avgColour;
	}
}

/**
 * Flips the Block's pixel data horizontally by a simple reflection
 * across the vertical axis through its middle.
**/
void Block::FlipHorizontal() {
	// complete your implementation below
	for (unsigned long i = 0; i < data.size(); i++) {
		for (unsigned long j = 0; j < data.size()/2; j++) {
				swap(data[i][j], data[i][data.size()-1-j]);
			}
	}
    
}

/**
 * Flips the Block's pixel data vertically by a simple reflection
 * across the horizontal axis through its middle.
**/
void Block::FlipVertical() {
	// complete your implementation below
    for (unsigned long i = 0; i < data.size() / 2; i++) {
		swap(data[i], data[data.size()-1-i]);
	}
}

/**
 * Returns the dimension of the block (its width or height).
 */
unsigned int Block::Dimension() const {
	// replace the statement below with your implementation
	return data.size();
}

/**
 * Returns the average color of the pixels in the block's data.
**/
RGBAPixel Block::GetAverageColor() const {
	// replace the statement below with your implementation
	int avgRed, avgGreen, avgBlue;
	avgRed = avgGreen = avgBlue = 0;
	double avgAlpha = 0.0;
	for (unsigned long i = 0; i < data.size(); i++) {
		for (unsigned long j = 0; j < data.size(); j++) {
				avgRed += data[i][j].r;
				avgGreen += data[i][j].g;
				avgBlue += data[i][j].b;
				avgAlpha += data[i][j].a;
			}
	}
	avgRed = avgRed / (data.size() * data.size());
	avgGreen = avgGreen / (data.size() * data.size());
	avgBlue = avgBlue / (data.size() * data.size());
	avgAlpha = avgAlpha / (data.size() * data.size());
	RGBAPixel avgColour = RGBAPixel(avgRed, avgGreen, avgBlue, avgAlpha);
	return avgColour;
}

/**
 * Replaces all pixel data in the block with the block's average color.
**/
void Block::FillAverage() {
	// complete your implementation below
    RGBAPixel avgColour = GetAverageColor();
	for (unsigned long i = 0; i < data.size(); i++) {
		for (unsigned long j = 0; j < data.size(); j++) {
				data[i][j] = avgColour;
			}
	}
}