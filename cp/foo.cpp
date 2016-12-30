/*
 * foo.cpp
 *
 *  Created on: 16 juil. 2012
 *      Author: hsaturn
 */

#include "foo.h"

#include <iostream>

using namespace std;

int foo::iInstances=0;

foo::foo() {
	iInstances++;
	// TODO Auto-generated constructor stub

}

void foo::view()
{
	cout << "Instances : " << iInstances << endl;
}

