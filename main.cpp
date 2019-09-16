#include "INodeEntry.h"
#include "metadata.h"
#include <iostream>
#include <fstream>
#include "GlobalINodeEntry.h"
#include "fs.h"
#include <string>
#include <windows.h>

using namespace std;
int pos = 0;
void runCommand(string command, string value, fs* _fs, string value2)
{
	
	if (command == "mkdir")
		_fs->mkdir(pos, &value[0]);
	if (command == "print")
		_fs->readINodesEntries();
	if (command == "cd")
	{
		int tmp=	_fs->cd(pos, value);
		if (tmp != -1)
			pos = tmp;
	}
	if (command == "ls")
		_fs->ls(pos);

	if (command == "rm")
		_fs->rm2(pos, value);
	if (command == "bm")
		_fs->bm();

	if (command == "st")
		_fs->setOnBitMapBlock(stoi(value));

	if (command == "import")
		_fs->import(pos, &value[0]);

	if (command == "export")
		_fs->exportar(value, &value2[0]);
	if (command == "vfs")
		_fs->init();
}

void split(string command, fs * _fs) {
	string tmpcommand;
	string tmpvalue ;
	string tmpvalue2;
	char delimiter = ' ';
	bool found = false;
	bool found2 = false;
	for (int i = 0; i < command.length(); i++)
	{
		if (found == true && found2 == false) {
			if (command[i] == delimiter)
			{
				found2 = true;
			}else
			tmpvalue += command[i];
		}
		else
		if (found2 == true)
		{
			tmpvalue2 += command[i];
		}
		else {
			if (delimiter == command[i])
				found = true;
									
			else
				tmpcommand += command[i];
		}
	}

	
	runCommand(tmpcommand, tmpvalue, _fs,tmpvalue2);
		
}




int main() {
	ifstream reader("fs.dat", ios::binary);
	fs fs;
	if (!reader)
	{
		
		cout << "No hay ningun vfs creado" << endl;
		
	}
	else {
		fs.readMetadata();
		fs.readINodesEntries();
		cout << "Estas dentro del vfs: " << fs.md.name << endl;
	}
	


	string command;
	while (true) {
		cout << fs.getFullPath(pos) <<" >> ";
		getline(cin, command);
		split(command, &fs);
		Sleep(250);
	}
	


	

	return 0;
}




