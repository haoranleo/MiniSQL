#ifndef _CatalogManager_h_
#define _CatalogManager_h_
#include "Macro.h"
#include "Sth.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

class CatalogManager{
public:
	vector<Table> tables;
	int tableNum;          
	vector<Index> indexes;       
	int indexNum;    

    CatalogManager() { InitialTalbeCatalog(); InitialIndexCatalog(); }
    ~CatalogManager() { StoreTalbeCatalog(); StoreIndexCatalog(); }

    void InitialTalbeCatalog();	
    void InitialIndexCatalog();	
    void StoreTalbeCatalog();
    void StoreIndexCatalog();
    void createTable(Table& table);
    void createIndex(Index index);		
    void dropTable(Table table);	
    void dropIndex(Index index);	
    void dropTable(string tablename);	
    void dropIndex(string index_name);	
    void update(Table& tableinfor);
    void update(Index& index);	
    bool ExistTable(string tablename);	
    bool ExistIndex(string tablename, int column);
    bool ExistIndex(string indexname);
    Table getTableInformation(string tablename);	
    Index getIndexInformation(string tablename, int column);
    Index getIndexInformation(string indexName);	
    int GetColumnNumber(Table& tableinfo, string columnname);
    int GetColumnAmount(Table& tableinfo);
};
#endif
