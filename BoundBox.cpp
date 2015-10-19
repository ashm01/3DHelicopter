#include "BoundBox.h"
#include <iostream>

using namespace::std;
BoundBox::BoundBox(double top, double bottom, double left, double right)
{
	this->top = top;
	this->bottom = bottom;
	this->left = left;
	this->right = right;

}
void BoundBox::updatePosition(double top, double bottom, double left, double right)
{
	this->top = top;
	this->bottom = bottom;
	this->left = left;
	this->right = right;
}
BoundBox::BoundBox()
{
	BoundBox(0, 0, 0, 0);
}

BoundBox::~BoundBox()
{
}

void BoundBox::draw()
{
	
}
