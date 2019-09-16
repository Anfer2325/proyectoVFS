#pragma once
#include "GlobalINodeEntry.h";
#include "INodeEntry.h";
#include "metadata.h";
#include "bitmap.h";
#include <fstream>
#include <cmath>;
#include "DataBlock.h";
class fs {
public:
	metadata  md;

	fs() {

	};
	void init() {
		writeMetadata();

		writeINodeEntries();
		writeDataBlocks();
	};
	void writeDataBlocks() {
		ofstream write("fs.dat", ios::binary | ios::app);
		unsigned int numblocks = ceil(((md.qty_entries * 130) * 1024) / 4);
		for (int i = 1; i <= numblocks; i++)
		{	
			DataBlock tmp;
			
			write.write(reinterpret_cast<const char*> (&tmp), sizeof(DataBlock));
		}
		
	}
	//logica exportar archivo
	void import(int root,const char* name)
	{
		INodeEntry father = getNodeByPos(root);
		INodeEntry son = getEmptyNode();
		if (father.first_son == -1)
			father.first_son = son.pos;

		if (father.first_son != -1)
		{
			INodeEntry tmp = getNodeByPos(father.first_son);
			while (tmp.right_brother != -1)
			{
				tmp = getNodeByPos(tmp.right_brother);
			}
			tmp.right_brother = son.pos;
			tmp.save();
		}


		strcpy_s(son.name, name);
		son.father = father.pos;
		son.occupied = true;
		strcpy_s(son.creation_date, "2/2/2");
		son.type = 'A';
		father.save();
		

		//exportar data

		
		//getting metadata
		ifstream readerMetadata("fs.dat", ios::binary);
		metadata mt;
		readerMetadata.seekg(0, ios::beg);
		readerMetadata.read(reinterpret_cast<char*> (&mt), sizeof(metadata));
		int totalINodes = mt.qty_entries;

		//getting bm data
		ifstream readerBitMap("fs.dat", ios::binary);
		
		bitmap bm(mt.qty_entries);
		//cout << "TEST SIZE: " << sizeof(bitmap) << " TEST 2 SIZE: " << sizeof(bm) << endl;
		readerBitMap.seekg(sizeof(metadata), ios::beg);
		readerBitMap.read(reinterpret_cast<char*> (&bm), sizeof(bm));
		int freePos = bm.getFreePosBit();
		int firstPos = freePos;

		////////////////////////////////////////////////////////////////////////////

		ifstream reader(name, ios::binary); // pointer for reading importing file
		//ofstream write("test.docx", ios::binary | ios::app);  
		ofstream writer("fs.dat", ios::binary | ios::in | ios::out);
		
		int size = reader.seekg(0, ios::end).tellg();
		int leido = 0;
		char  data[4092];
		
		cout << "Size del archivo: " << size << endl;
		
		reader.seekg(0, ios::beg);
		while (!reader.eof()) {
			
			if (leido + 4092 <= size)
			{
				reader.read(reinterpret_cast<char*> (data), 4092);
				leido += 4092;
				//write.write(reinterpret_cast<const char*> (data), 4092);
				//son.agregarData(data,4092);

				if (leido < size)
				{
					writer.seekp(sizeof(metadata) + sizeof(bitmap) + (sizeof(INodeEntry) * totalINodes) + (freePos * sizeof(DataBlock)), ios::beg);
					bm.setOn(freePos);
					freePos = bm.getFreePosBit();
					DataBlock db(data, freePos);
					cout << "info bloque:  " << endl;
					cout << "pos : " << db.sig<<endl;

					writer.write(reinterpret_cast<const char*> (&db), sizeof(DataBlock));
				}
				else {
					writer.seekp(sizeof(metadata) + sizeof(bitmap) + (sizeof(INodeEntry) * totalINodes) + (freePos * sizeof(DataBlock)), ios::beg);
					bm.setOn(freePos);
					
					DataBlock db(data, -1);
					cout << "aqui esta el problema" << endl;
					writer.write(reinterpret_cast<const char*> (&db), sizeof(DataBlock));
				}
				
			}
			else
			{
				
				int bytesPorLeer = size - leido;
				char   dataRestante[4092];
				
				reader.read(reinterpret_cast<char*> (dataRestante), bytesPorLeer);

				writer.seekp(sizeof(metadata) + sizeof(bitmap) + (sizeof(INodeEntry) * totalINodes) + (freePos * sizeof(DataBlock)), ios::beg);
				bm.setOn(freePos);

				DataBlock db(dataRestante, -1);
				writer.write(reinterpret_cast<const char*> (&db), bytesPorLeer);
				//write.write(reinterpret_cast<const char*> (dataRestante), bytesPorLeer);
				//son.agregarData(dataRestante,bytesPorLeer);
				break;
			}
		}
		son.size = size;
		cout << "FIRST POS : " << firstPos << endl;
		son.firstDataBlock = firstPos;
		son.save();
		bm.save();
		reader.close();
		cout << "Size del archivo real >  " << son.size << endl;
		
		
			
	}

	void bm() {
		bitmap bm(md.qty_entries);
		ifstream reader("fs.dat", ios::binary);

		reader.seekg(sizeof(metadata), ios::beg);
		reader.read(reinterpret_cast<char*> (&bm), sizeof(bm));
		
		cout << "FREE POS : "<< bm.getFreePosBit()<< endl;
		bm.printBitMap();
		reader.close();
		
	}

	void setOnBitMapBlock(int block) {
		bitmap bm(md.qty_entries);
		ifstream reader("fs.dat", ios::binary);
		reader.seekg(sizeof(metadata), ios::beg);
		reader.read(reinterpret_cast<char*> (&bm), sizeof(bm));
		bm.setOn(block);
		
	};
	void writeBitMap( unsigned int size)
	{
		unsigned int numblocks =ceil(((size * 130) * 1024)/4);

		bitmap bm(numblocks);
		cout << bm.dbSize;
		ofstream write("fs.dat", ios::binary | ios::app);
		write.write(reinterpret_cast<const char*> (&bm), sizeof(bitmap));
		write.close();
	}
	void writeMetadata() {
		char name[30];
		cout << "Ingrese el nombre del vfs: " << endl;
		cin >> name;

		/*unsigned int size;
		cout << "Ingrese el size del vfs: " << endl;
		cin >> size;

		char date[8];
		cout << "Ingrese el date del vfs: " << endl;
		cin >> date;

		char owner[30];
		cout << "Ingrese el owner del vfs: " << endl;
		cin >> owner;*/

		unsigned int qty_entries;
		cout << "Ingrese el numero de entries del vfs: " << endl;
		cin >> qty_entries;

		/*unsigned int bitmap_size;
		cout << "Ingrese el bitmap size del vfs: " << endl;
		cin >> bitmap_size;

		unsigned int qty_direct_blocks;
		cout << "Ingrese la cantidad de bloques directos del vfs: " << endl;
		cin >> qty_direct_blocks;*/

		metadata tmpmd (name, qty_entries);
		md = tmpmd;
		ofstream write("fs.dat", ios::binary | ios::app);
		write.write(reinterpret_cast<const char*> (&tmpmd), sizeof(metadata));
		write.close();
		writeBitMap(qty_entries);
		
	};

	void writeINodeEntries() {
		ofstream write("fs.dat", ios::binary | ios::app);
		
		for (int i = 0; i < md.qty_entries; i++) {
			if (i == 0)
			{
				INodeEntry tmpEnt("/", "8/2/1", -1, -1, -1, 'D', -1, true, i);
				write.write(reinterpret_cast<const char*> (&tmpEnt), sizeof(INodeEntry));
			}
			else{
			INodeEntry tmpEntry(i);
			write.write(reinterpret_cast<const char*> (&tmpEntry), sizeof(INodeEntry));
			}
		}
		write.close();
	}

	void readMetadata() {
		ifstream reader("fs.dat", ios::binary);
		metadata tmpmd;
		reader.read(reinterpret_cast<char*> (&tmpmd), sizeof(metadata));
		md = tmpmd;
		cout << md.name<<endl;

		reader.close();

	}
	void exportar(string _nodename, const char * newFile) {
		ifstream reader("fs.dat", ios::binary);
		INodeEntry tmpnode;
		reader.seekg(sizeof(metadata) + sizeof(bitmap), ios::beg);
		reader.read(reinterpret_cast<char*> (&tmpnode), sizeof(INodeEntry));
		cout << "Numero de Entries a leer: " << md.qty_entries << endl;
		for (int i = 0; i < md.qty_entries; i++) {
			
			cout << tmpnode.name << endl;
			cout << _nodename << endl;
			if (tmpnode.name == _nodename && tmpnode.type=='A')
			{
				tmpnode.exportar(newFile);
				cout << "Archivo Exportado..." << endl;
				return;
			}
			reader.read(reinterpret_cast<char*> (&tmpnode), sizeof(INodeEntry));
			

		}

		cout << "No existe ese archivo..." << endl;
		reader.close();
	}
	void readINodesEntries() {
		
		ifstream reader("fs.dat", ios::binary);
		
		reader.seekg(sizeof(metadata)+ sizeof(bitmap), ios::beg);
		cout <<"Numero de Entries a leer: " <<md.qty_entries<<endl;
		for (int i = 0; i < md.qty_entries; i++) {
			INodeEntry tmpnode;
			reader.read(reinterpret_cast<char*> (&tmpnode), sizeof(INodeEntry));
			tmpnode.print();

		}
		reader.close();
	}

	void mkdir(int root, const char * name) {
		INodeEntry father =getNodeByPos(root);
		INodeEntry son = getEmptyNode();
		if(father.first_son == -1)
		father.first_son = son.pos;
		
		if (father.first_son != -1)
		{
			INodeEntry tmp= getNodeByPos(father.first_son);
			while (tmp.right_brother != -1)
			{
				tmp = getNodeByPos(tmp.right_brother);
			}
			tmp.right_brother = son.pos;
			tmp.save();
		}


		strcpy_s(son.name, name);
		son.father = father.pos;
		son.occupied = true;
		strcpy_s(son.creation_date, "2/2/2");
		son.type = 'D';

		father.save();
		son.save();

		cout << "Directorio creado " << endl;
			
	};

	INodeEntry getNodeByPos(int pos) {
		ifstream reader("fs.dat", ios::binary);
		reader.seekg(sizeof(metadata)+sizeof(bitmap)+ (pos * sizeof(INodeEntry)), ios::beg);
		INodeEntry tmp;
		reader.read(reinterpret_cast<char*> (&tmp), sizeof(INodeEntry));
		return tmp;
	};

	INodeEntry getEmptyNode() {
		ifstream reader("fs.dat", ios::binary);
		reader.seekg(sizeof(metadata) + sizeof(bitmap), ios::beg);
		for (int i = 0; i < md.qty_entries; i++) {
			INodeEntry tmpnode;
			reader.read(reinterpret_cast<char*> (&tmpnode), sizeof(INodeEntry));
			if (tmpnode.occupied == false)
				return tmpnode;

		}

		return NULL;
	}

	string getFullPath(int pos) {
		INodeEntry end = getNodeByPos(pos);
		string path= end.name;
		string delimiter = "/";
		while (end.father != -1)
		{
			end = getNodeByPos(end.father);
			if (end.name == "/")
				path = end.name + path;
			else 
				path =   end.name+ delimiter  + path;
		}
		return path;
	}

	int cd(int pos,string name) {

		if (name != "..")
		{

		
		INodeEntry father = getNodeByPos(pos);
		if (father.first_son == -1)
			cout << " No existe ese nombre" << endl;
		else {
			INodeEntry firstson = getNodeByPos(father.first_son);
			if (firstson.name == name && firstson.type !='A')
			{
				return firstson.pos;
			}
			else if(firstson.right_brother != -1) {
				INodeEntry tmp = getNodeByPos(firstson.right_brother);
				do{
					if (tmp.name == name && tmp.type != 'A')
						return tmp.pos;
					else
						tmp = getNodeByPos(tmp.right_brother);
				} while (tmp.right_brother != -1);

				cout << "No existe ese directorio o es un archivo" << endl;
				
			}
			else {
				cout << "No existe ese directorio  o es un archivo" << endl;
			}
		}
		return -1;
		}
		else {
			INodeEntry tmp = getNodeByPos(pos);
			if (tmp.father != -1)
			{
				INodeEntry goTo = getNodeByPos(tmp.father);
				return goTo.pos;
			}
			else {
				cout << "Ya estas al inicio del vfs"<< endl;
				return -1;
			}
		}
}


	void ls(int pos) {
		INodeEntry father = getNodeByPos(pos);
		if (father.first_son != -1)
		{
			INodeEntry son = getNodeByPos(father.first_son);
									
			while(son.right_brother !=-1)
			{
				cout << son.name << endl;
				son = getNodeByPos(son.right_brother);
			}
			cout << son.name << endl;

		}

	}



	void rm(int pos, string name) {
		INodeEntry father = getNodeByPos(pos);
		if (father.first_son != -1) {
			INodeEntry son = getNodeByPos(father.first_son);
			if (son.name == name)
			{
				if (son.right_brother != -1)
					father.first_son = son.right_brother;
				son.reset();
				
				return;
			}
			else {
				INodeEntry brother = getNodeByPos(son.right_brother);
				while (brother.right_brother != -1)
				{
					if (brother.name == name)
					{
						if (brother.right_brother != -1)
							son.right_brother = brother.right_brother;
						brother.reset();
						
						return;
					}
					else
					{
						son = getNodeByPos(son.right_brother);
						brother = getNodeByPos(brother.right_brother);
					}
				}

				if (brother.name == name)
				{
					if (brother.right_brother != -1)
						son.right_brother = brother.right_brother;
					brother.reset();
				
					return;
				}
			}
		}
		else
		{
			cout << "Directorio no existe.. " << endl;
			return;
		}

	}
	
	void rm2(int pos, string name) {
		INodeEntry father = getNodeByPos(pos);
		if (father.first_son != -1)
		{
			INodeEntry son = getNodeByPos(father.first_son);
			if (son.name == name)
			{
				
				if (son.first_son != -1)
				{
					rm2(son.first_son, son.pos);
				}
				if (son.right_brother != -1)
					father.first_son = son.right_brother;
				else
					father.first_son = -1;
				father.save();
				if (son.type == 'A')
				{
					son.rmDb();
				}
				son.reset();
				cout << "Directorio eliminado" << endl;
				return;
			}
			else if(son.right_brother != -1) {
				while (son.right_brother != -1)
				{
					INodeEntry brother = getNodeByPos(son.right_brother);
					if (brother.name == name)
					{
						
						if (brother.first_son != -1)
						{
							rm2(brother.first_son, brother.pos);
						}
						if (brother.right_brother != -1)
							son.right_brother = brother.right_brother;
						else
							son.right_brother = -1;
						son.save();
						if (brother.type == 'A')
						{
							brother.rmDb();
						}
						brother.reset();
						
						cout << "Directorio eliminado" << endl;
						return;
					}
					else {
						if (brother.right_brother != -1)
						{
							son = getNodeByPos(brother.right_brother);
						}
						else {
							cout << "Directorio no existe" << endl;
							return;
						}

						
					}
				}
			}
			else {
				cout << "Directorio no existe" << endl;
				return;
			}
		}
		else {
			cout << "Directorio no existe" << endl;
			return;
		}
	}

	void rm2(int pos, int father)
	{
		INodeEntry tmp = getNodeByPos(pos);
		if (tmp.first_son != -1)
			rm2(tmp.first_son, father);
		if (tmp.right_brother != -1)
			rm2(tmp.right_brother, father);

		cout << "Se elimino: " << tmp.name;
		tmp.reset();
		
	}
};