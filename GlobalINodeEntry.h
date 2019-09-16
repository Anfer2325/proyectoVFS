#pragma once
#include "INodeEntry.h"
struct GlobalINodeEntry {
	INodeEntry* entries;
	GlobalINodeEntry() {
		entries = nullptr;

	}
	GlobalINodeEntry(int size) {
	//	entries = new INodeEntry[size];
	}
};