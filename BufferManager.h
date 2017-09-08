#ifndef _buffer_h_
#define _buffer_h_
#include "Sth.h"
#include "Macro.h"
#include <string> 
#include <fstream>
#include <vector>

class BufferManager
{
public:
	friend class RecordManager;
	friend class IndexManager;
	friend class Leaf;
	friend class Branch;
	friend class BPlusTree;
    buffer bufferBlock[MAXBLOCKNUMBER];
	BufferManager(){
		for(int i = 0; i< MAXBLOCKNUMBER; i++) bufferBlock[i].initialize();
	}
	~BufferManager(){
		for(int i = 0; i< MAXBLOCKNUMBER; i++)
			flashBack(i);
	}
	
    void flashBack(int bufferNum);	
    int getbufferNum(string filename, int blockOffset);	
    void readBlock(string filename, int blockOffset, int bufferNum);	
    void writeBlock(int bufferNum);	
    void useBlock(int bufferNum);	
    int getEmptyBuffer();	
    int getEmptyBufferExcept(string filename);
    insertPos getInsertPosition(Table& tableinfor);
    int addBlockInFile(Table& tableinfor);	
    int addBlockInFile(Index& indexinfor);
    int getIfIsInBuffer(string filename, int blockOffset);	
    void scanIn(Table tableinfo);	
    void setInvalid(string filename);
};

#endif
