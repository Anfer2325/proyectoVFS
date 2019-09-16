#pragma once
#include <iostream>
using namespace std;
struct metadata {
public:
	char name[30];
	unsigned int size;
	char date[8];
	char owner[30];
	unsigned int qty_entries;
	unsigned int bitmap_size;
	unsigned int qty_direct_blocks;
	metadata(const char* nam, unsigned int _size, const char* _date, const char* _owner, unsigned int _qty_entries, unsigned int _bitmap_size, unsigned int _qty_direct_blocks) {
		strcpy_s(name, nam);
		size = _size;
		strcpy_s(date, _date);
		strcpy_s(owner, _owner);
		qty_entries = _qty_entries;
		bitmap_size = _bitmap_size;
		qty_direct_blocks = _qty_direct_blocks;

	}
	metadata() {
		strcpy_s(name, " ");
		size = -1;
		strcpy_s(date,"0/0/0");
		strcpy_s(owner, "-1");
		qty_entries = -1;
		bitmap_size = -1;
		qty_direct_blocks = -1;
	}
	metadata(const char* nam, unsigned int _qty_entries)
	{
		strcpy_s(name, nam);
		size = -1;
		strcpy_s(date, "0/0/0");
		strcpy_s(owner, "-1");
		qty_entries = _qty_entries;
		bitmap_size = -1;
		qty_direct_blocks = -1;
	};
};

