#pragma once
#include <cmath>
struct bitmap
{
public:
	char*bitm; //el peso del struct al reiniciar archivo es diferente porque bitm no tiene tama;an definido
	int dbSize;

	bitmap(int size)
	{
		dbSize = size;
		
		bitm = new char[ceil(dbSize / 8)];
		initBitMap();
		
	}
	bitmap()
	{

	}

	int getFreePosBit() {
		const int SHIFT = 8 * sizeof(char) - 1;
		char MASK = 1 << SHIFT;

		for (int i = 0; i < dbSize / 8; i++)
		{
			char value;
			value = bitm[i];
			for (int c = 1; c <= SHIFT + 1; c++)
			{
				int bit = (value & MASK ? 1 : 0); //preguntar por este operador ternario
				if (bit == 0)
				{
					int freePos= (c - 1) + (i * 8);
					if (freePos > dbSize)
					{
						cout << "No queda espacio en disco ...." << endl;
						return -1;
					}
					return freePos;
				}
	
					
				else
					MASK = 1 << SHIFT -c;
			}
			MASK = 1 << SHIFT;
		}
		cout << "No queda espacio en disco ...." << endl;
		return -1;
	}

	void initBitMap()
	{
		for (int i = 0; i < dbSize / 8; i++)
		{
			bitm[i] = 0;
		}
	}

	void printBitMap()
	{
		const int SHIFT = 8 * sizeof(char) - 1;
		const char MASK = 1 << SHIFT;
		
		for (int i = 0; i < dbSize / 8; i++)
		{
			char value;
			value = bitm[i];
			for (int c = 1; c <= SHIFT + 1; c++)
			{
				cout << (value & MASK ? '1' : '0'); //preguntar por este operador ternario
				value <<= 1;

				if (c % 8 == 0)
					cout << ' ';
			}
		}
		cout << endl;

	}

	void setOn( int nBlock)
	{
		int positionByte = nBlock / 8;
		int iniPosition = (nBlock / 8) * 8;

		for (int i = iniPosition, x = 7; i < (positionByte * 8) + 8; i++, x--)
		{
			if (i == nBlock)
			{
				bitm[positionByte] |= 1 << x;
				break;
			}
		}

		save();
	}

	void setOff( int nBlock)
	{
		int positionByte = nBlock / 8;
		int iniPosition = (nBlock / 8) * 8;

		for (int i = iniPosition, x = 7; i < (positionByte * 8) + 8; i++, x--)
		{
			if (i == nBlock)
			{
				bitm[positionByte] &= ~(1 << x);
				break;
			}
		}
	}

	void save() {
		ofstream write("fs.dat", ios::binary | ios::in | ios::out);
		write.seekp(sizeof(metadata));
		write.write(reinterpret_cast<const char*> (this), sizeof(bitmap));
		write.close();

	}

};