#include <iostream>
#include "primitives.h"
#include "scene.h"


int main(int argc, char* argv[])
{
	if (argc != 2){
		cerr << "Input error. Abort." << endl;
		cerr << "\tinput: ./CDecompose [filepath] (.obj, .off, .json)" << endl;
	}
	char* name = argv[1];
	cout << "*************************" << endl;
	cout << "* CDecomposition starts *" << endl;
	cout << "*                  v0.5 *" << endl;	
	cout << "*************************" << endl;
	cout << "File: " << name << endl;

	Scene scene(name, 7611);	

	return 0;
}