#pragma once
#include <iostream>
#include <fstream>
#include "metadata.h";
#include "bitmap.h"
#include "DataBlock.h"
using namespace std;
struct INodeEntry {
public:
	char name[25];
	char creation_date[8];
	int father;
	int first_son;
	int right_brother;
	char type;
	int size;
	int diskSize;
	bool occupied;
	int pos;
	int firstDataBlock;
	
	INodeEntry(const char * nam, const char * date,int _father,int _first_son,int _right_brother, char _type,int _size,bool _occupied,int _pos) {
		strcpy_s(name, nam);
		strcpy_s(creation_date, date);
		father = _father;
		first_son = _first_son;
		right_brother = _right_brother;
		type = _type;
		size = _size;
		occupied = _occupied;
		pos = _pos;
		diskSize = -1;
		firstDataBlock = -1;
	}
	//Nodo archivo
	INodeEntry(const char* nam, const char* date, int _father,int _right_brother, char _type, int _size, int _pos, int datablock)
	{
		strcpy_s(name, nam);
		strcpy_s(creation_date, date);
		father = _father;
		first_son =-1;
		right_brother = _right_brother;
		type = _type;
		size = _size;
		occupied = true;
		pos = _pos;
		diskSize = (size / 1024) * 1024;
		firstDataBlock = datablock;
	};
	INodeEntry(int _pos) {
		strcpy_s(name, "-1");
		strcpy_s(creation_date,"-1");
		father = -1;
		first_son = -1;
		right_brother = -1;
		type = ' ';
		size = 0;
		occupied = false;
		pos = _pos;
		firstDataBlock = -1;
	}
	INodeEntry() {
		strcpy_s(name, "-1");
		strcpy_s(creation_date, "-1");
		father = -1;
		first_son = -1;
		right_brother = -1;
		type = ' ';
		size = 0;
		occupied = false;
		pos = -1;
		firstDataBlock = -1;
	}
	void agregarData(char data[4092],int _size) {
		char dt[4092];
		memcpy(dt, data,_size);
		
		size += _size;
		ofstream write("video2.mp4", ios::binary | ios::app);
		write.write(reinterpret_cast<const char*> (dt), _size);
	}

	void rmDb() {
		ifstream readerMetadata("fs.dat", ios::binary);
		metadata mt;
		readerMetadata.seekg(0, ios::beg);
		readerMetadata.read(reinterpret_cast<char*> (&mt), sizeof(metadata));
		int totalINodes = mt.qty_entries;



		ifstream readerBitMap("fs.dat", ios::binary);
		bitmap bm(mt.qty_entries);
		readerBitMap.seekg(sizeof(metadata), ios::beg);
		readerBitMap.read(reinterpret_cast<char*> (&bm), sizeof(bm));
		




		DataBlock tmp;
		ifstream reader("fs.dat", ios::binary);
		reader.seekg(sizeof(metadata) + sizeof(bitmap) + (totalINodes * sizeof(INodeEntry) + (sizeof(DataBlock) * firstDataBlock)), ios::beg);
		reader.read(reinterpret_cast<char*> (&tmp), sizeof(DataBlock));


		ofstream write(name, ios::binary | ios::app);
		int escrito = 0;
		bm.setOff(firstDataBlock);
		while (true) {
			
				
			
			if (tmp.sig == -1)
				break;
			else
			{
				bm.setOff(tmp.sig);
				reader.read(reinterpret_cast<char*> (&tmp), sizeof(DataBlock));
				
			}

		}

		
		bm.save();
		write.close();
		reader.close();
		readerBitMap.close();
		readerMetadata.close();
	};

	void exportar(const char* name) {
		ifstream readerMetadata("fs.dat", ios::binary);
		metadata mt;
		readerMetadata.seekg(0, ios::beg);
		readerMetadata.read(reinterpret_cast<char*> (&mt), sizeof(metadata));
		int totalINodes = mt.qty_entries;


		DataBlock tmp;
		ifstream reader("fs.dat", ios::binary);
		reader.seekg(sizeof(metadata) + sizeof(bitmap) + (totalINodes * sizeof(INodeEntry)+(sizeof(DataBlock) * firstDataBlock)), ios::beg);
		reader.read(reinterpret_cast<char*> (&tmp), sizeof(DataBlock));


		ofstream write(name, ios::binary | ios::app);
		int escrito = 0;
		while (true) {
			cout << "siguiente : .... " << tmp.sig;
			if (escrito + 4092 < size)
			{
				write.write(reinterpret_cast<const char*> (tmp.data), 4092);
				escrito += 4092;
				
			}
			else {
				int residuo = size - escrito;
				write.write(reinterpret_cast<const char*> (tmp.data), residuo);
				escrito += residuo;
				break;
			}
			if (tmp.sig == -1)
				break;
			else
			{
				reader.read(reinterpret_cast<char*> (&tmp), sizeof(DataBlock));
			}

		}
			
		cout << "ESCRITO : " << escrito << endl;
		
		write.close();
		reader.close();
		readerMetadata.close();
	};
	void save() {
		ofstream write("fs.dat", ios::binary|ios::in | ios::out);
		write.seekp(sizeof(metadata)+ sizeof(bitmap) + (pos * sizeof(INodeEntry)));
		write.write(reinterpret_cast<const char*> (this), sizeof(INodeEntry));
		write.close();
	}
	void save(int _pos) {
		ofstream write("fs.dat", ios::binary | ios::in | ios::out);
		write.seekp(sizeof(metadata)+ sizeof(bitmap) + (_pos * sizeof(INodeEntry)));
		write.write(reinterpret_cast<const char*> (this), sizeof(INodeEntry));
		write.close();
	}
	void print() {
		cout << "_________________________________" << endl;
		cout << "Nombre: " << this->name << endl;
		cout << "Fecha de Creacion: " << this->creation_date << endl;
		cout << "Padre: " << this->father << endl;
		cout << "Primer hijo: " << this->first_son << endl;
		cout << "Hermano derecho: " << this->right_brother << endl;
		cout << "Tipo (D : DIRECTORIO  || A : ARCHIVO): " << this->type << endl;
		cout << "Tamano: " << this->size << endl;
		cout << "Ocupado: " << this->occupied<<endl;
		cout << "Posicion: " << this->pos << endl;
		cout << "First DataBlock: " << this->firstDataBlock << endl;
		cout << "_________________________________" << endl;

	}

	void reset() {
		int _pos = pos;
		strcpy_s(name, "-1");
		strcpy_s(creation_date, "-1");
		father = -1;
		first_son = -1;
		right_brother = -1;
		type = ' ';
		size = -1;
		occupied = false;
		pos = pos;
		save(_pos);
	}
};
