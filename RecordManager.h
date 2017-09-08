#ifndef _RECORD_H_
#define _RECORD_H_
#include "BufferManager.h"
#include "Sth.h"
#include "Macro.h"
#include <stdio.h>
#include <vector>
extern BufferManager buf;
class RecordManager
{
public:
    bool Comparator(Table tableinfor, Row row, vector<Condition> conditions);
    Data select(Table& tableinfor);	
    Data select(Table tableinfor, vector<Condition> conditions);	
    void insertValue(Table& tableinfor, Row& splitedRow);	
    int deleteValue(Table tableinfor);	
    int deleteValue(Table tableinfor, vector<Condition> conditions);	
    void dropTable(Table tableinfo);	
    void createTable(Table tableinfo);
    Row splitRow(Table tableinfor, string row);	
    string connectRow(Table tableinfor, Row splitedRow);
    void showDatas(const Data& datas) const;
};
#endif






