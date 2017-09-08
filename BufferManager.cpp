#include "BufferManager.h"
#include <iostream>

void BufferManager::flashBack(int bufferNum) {
    if (!bufferBlock[bufferNum].isWritten) return;
    string filename = bufferBlock[bufferNum].filename;
    fstream fout(filename.c_str(), ios::in | ios::out);
    fout.seekp(BLOCKSIZE*bufferBlock[bufferNum].blockOffset, fout.beg);
    fout.write(bufferBlock[bufferNum].values, BLOCKSIZE);
    bufferBlock[bufferNum].initialize();
    fout.close();
}

int BufferManager::getbufferNum(string filename, int blockOffset) {
    int bufferNum = getIfIsInBuffer(filename, blockOffset);
    if (bufferNum == -1) {
        bufferNum = getEmptyBufferExcept(filename);
        readBlock(filename, blockOffset, bufferNum);
    }
    return bufferNum;
}

void BufferManager::readBlock(string filename, int blockOffset, int bufferNum) {
    bufferBlock[bufferNum].isValid = 1;
    bufferBlock[bufferNum].isWritten = 0;
    bufferBlock[bufferNum].filename = filename;
    bufferBlock[bufferNum].blockOffset = blockOffset;
    fstream  fin(filename.c_str(), ios::in);
    fin.seekp(BLOCKSIZE*blockOffset, fin.beg);
    fin.read(bufferBlock[bufferNum].values, BLOCKSIZE);
    fin.close();
}

void BufferManager::writeBlock(int bufferNum) {
    bufferBlock[bufferNum].isWritten = 1;
    useBlock(bufferNum);
}

void BufferManager::useBlock(int bufferNum) {
    for (int i = 1; i < MAXBLOCKNUMBER; i++)
    {
        if (i == bufferNum) {
            bufferBlock[bufferNum].LRUvalue = 0;
            bufferBlock[i].isValid = 1;
        }
        else bufferBlock[bufferNum].LRUvalue++;
    }
}

int BufferManager::getEmptyBuffer() {
    int bufferNum = 0;
    int highestLRUvalue = bufferBlock[0].LRUvalue;
    for (int i = 0; i < MAXBLOCKNUMBER; i++)
    {
        if (!bufferBlock[i].isValid) {
            bufferBlock[i].initialize();
            bufferBlock[i].isValid = 1;
            return i;
        }
        else if (highestLRUvalue < bufferBlock[i].LRUvalue)
        {
            highestLRUvalue = bufferBlock[i].LRUvalue;
            bufferNum = i;
        }
    }
    flashBack(bufferNum);
    bufferBlock[bufferNum].isValid = 1;
    return bufferNum;
}

int BufferManager::getEmptyBufferExcept(string filename) {
    int bufferNum = -1;
    int highestLRUvalue = bufferBlock[0].LRUvalue;
    for (int i = 0; i < MAXBLOCKNUMBER; i++)
    {
        if (!bufferBlock[i].isValid) {
            bufferBlock[i].isValid = 1;
            return i;
        }
        else if (highestLRUvalue < bufferBlock[i].LRUvalue && bufferBlock[i].filename != filename)
        {
            highestLRUvalue = bufferBlock[i].LRUvalue;
            bufferNum = i;
        }
    }
    if (bufferNum == -1) {
        cout << "All the buffers in the database system are used up. Sorry about that!" << endl;
        exit(0);
    }
    flashBack(bufferNum);
    bufferBlock[bufferNum].isValid = 1;
    return bufferNum;
}

insertPos BufferManager::getInsertPosition(Table& tableinfor) {
    insertPos ipos;
    if (tableinfor.blockNum == 0) {
        ipos.bufferNUM = addBlockInFile(tableinfor);
        ipos.position = 0;
        return ipos;
    }
    string filename = tableinfor.name + ".table";
    int length = tableinfor.totalLength + 1;
    int blockOffset = tableinfor.blockNum - 1;
    int bufferNum = getIfIsInBuffer(filename, blockOffset);
    if (bufferNum == -1) {
        bufferNum = getEmptyBuffer();
        readBlock(filename, blockOffset, bufferNum);
    }
    const int recordNum = BLOCKSIZE / length;
    for (int offset = 0; offset < recordNum; offset++) {
        int position = offset * length;
        char isEmpty = bufferBlock[bufferNum].values[position];
        if (isEmpty == EMPTY) {
            ipos.bufferNUM = bufferNum;
            ipos.position = position;
            return ipos;
        }
    }

    ipos.bufferNUM = addBlockInFile(tableinfor);
    ipos.position = 0;
    return ipos;
}

int BufferManager::addBlockInFile(Table& tableinfor) {
    int bufferNum = getEmptyBuffer();
    bufferBlock[bufferNum].initialize();
    bufferBlock[bufferNum].isValid = 1;
    bufferBlock[bufferNum].isWritten = 1;
    bufferBlock[bufferNum].filename = tableinfor.name + ".table";
    bufferBlock[bufferNum].blockOffset = tableinfor.blockNum++;
    return bufferNum;
}

int BufferManager::addBlockInFile(Index& indexinfor) {
    string filename = indexinfor.index_name + ".index";
    int bufferNum = getEmptyBufferExcept(filename);
    bufferBlock[bufferNum].initialize();
    bufferBlock[bufferNum].isValid = 1;
    bufferBlock[bufferNum].isWritten = 1;
    bufferBlock[bufferNum].filename = filename;
    bufferBlock[bufferNum].blockOffset = indexinfor.blockNum++;
    return bufferNum;
}

int BufferManager::getIfIsInBuffer(string filename, int blockOffset) {
    for (int bufferNum = 0; bufferNum < MAXBLOCKNUMBER; bufferNum++)
        if (bufferBlock[bufferNum].filename == filename && bufferBlock[bufferNum].blockOffset == blockOffset)	return bufferNum;
    return -1;
}

void BufferManager::scanIn(Table tableinfo) {
    string filename = tableinfo.name + ".table";
    fstream  fin(filename.c_str(), ios::in);
    for (int blockOffset = 0; blockOffset < tableinfo.blockNum; blockOffset++) {
        if (getIfIsInBuffer(filename, blockOffset) == -1) {
            int bufferNum = getEmptyBufferExcept(filename);
            readBlock(filename, blockOffset, bufferNum);
        }
    }
    fin.close();
}

void BufferManager::setInvalid(string filename) {
    for (int i = 0; i < MAXBLOCKNUMBER; i++)
    {
        if (bufferBlock[i].filename == filename) {
            bufferBlock[i].isValid = 0;
            bufferBlock[i].isWritten = 0;
        }
    }
}