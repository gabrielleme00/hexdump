#include <iostream>
#include <fstream>
#include <iomanip>
#include <string.h>

typedef unsigned long ulong;
typedef unsigned int uint;

const uint FileNameBufferSize = 256;
const uint AsciiThreshold = 32; // Characters until code 32 cannot be shown
const uint BytesPerLine = 16;

using namespace std;

void showHeaders() {
	cout << "Offset   ";
	
	for (uint i = 0; i < BytesPerLine; ++i) {
		if(i % 8 == 0) cout << ' ';
		cout << setw(2) << i  << ' ';
	}
	
	cout << "  ASCII\n";
}

void showOffset(ulong offset) {
	cout << setw(8) << offset;
}

void showHexCodes(char* buffer, ulong offset, ulong bytesLeft) {
	for(uint i = 0; i < BytesPerLine; i++) {
        if(i % 8 == 0) cout << ' ';
        if(i < bytesLeft)
            cout << ' ' << setw(2) << (unsigned)buffer[i + offset];
        else 
            cout << "   ";
    }
}

void showCharacters(char* buffer, ulong offset) {
	cout << "  |";
	
    for(uint i = 0; i < BytesPerLine; i++) {
        if( (unsigned)buffer[i + offset] < AsciiThreshold )
			cout << '.';
        else
			cout << buffer[i + offset];
    }
	
	cout << "|\n";
}
 
int main(int argc, char *argv[]) {
	ifstream ifs;		// Input file stream
	ulong offset = 0;	// Address offset
	filebuf* pbuf;		// File stream buffer
	size_t size;		// File size
	char* buffer;		// Buffer for the file data
	char fileName[FileNameBufferSize];
	
	// Read arguments
	if (argc == 2) {
		strcpy(fileName, argv[1]);
	} else if (argc == 1) {
		cout << "File to dump: ";
		cin.get(fileName, FileNameBufferSize);
	} else if (argc > 2) {
		cout << "Please, provide only one file.\n";
		return 1;
	}
	
	ifs.open(fileName, ifstream::binary); 		// Open file in binary mode
	pbuf = ifs.rdbuf();				// Get pointer to associated buffer object
	size = pbuf->pubseekoff(0, ifs.end, ifs.in); 	// Get file size using buffer's members
	buffer = new char[size];			// Allocate memory to contain file data
	
	pbuf->pubseekpos(0, ifs.in);			// Set internal position pointer to absolute position
	pbuf-> sgetn (buffer, size);			// Get file data
	
	cout << hex << setfill('0');			// Set output to hexadecimal
	    
	showHeaders();
	
	// A line for every BytesPerLine
	for (ulong bytesRead = 0; bytesRead < size; bytesRead += BytesPerLine) {
		uint bytesLeft = size - bytesRead;
	    if ( bytesLeft > BytesPerLine )
			bytesLeft = BytesPerLine;
	    
		showOffset(offset);
		showHexCodes(buffer, offset, bytesLeft);
		showCharacters(buffer, offset);
		
	   	offset += BytesPerLine;
	}
	
	ifs.close();
	delete[] buffer;
	
	return 0;
}
