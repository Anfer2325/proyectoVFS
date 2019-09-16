#pragma once
#include <iostream>
#include <fstream>
#include "metadata.h";
#include "bitmap.h"
struct DataBlock {
public:
	char data[4092];
	int sig = -1;

	DataBlock() {

	};

	DataBlock(char _data[4092],int _sig) {
		memcpy(data, _data, 4092);
		sig = _sig;
	};
	

};