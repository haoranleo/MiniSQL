#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "CatalogManager.h"
#include "Macro.h"
#include "Sth.h"

void CatalogManager::InitialTalbeCatalog() {
    const string filename = "table.catlog";
    fstream  fin(filename.c_str(), ios::in);
    fin >> tableNum;
    for (int i = 0; i < tableNum; i++)
    {
        Table temp_table;
        fin >> temp_table.name;
        fin >> temp_table.attriNum;
        fin >> temp_table.blockNum;
        for (int j = 0; j < temp_table.attriNum; j++)
        {
            Attribute temp_attri;
            fin >> temp_attri.name;
            fin >> temp_attri.type;
            fin >> temp_attri.length;
            fin >> temp_attri.isPrimeryKey;
            fin >> temp_attri.isUnique;
            temp_table.attributes.push_back(temp_attri);
            temp_table.totalLength += temp_attri.length;
        }
        tables.push_back(temp_table);
    }
    fin.close();
}

void CatalogManager::InitialIndexCatalog() {
    const string filename = "index.catlog";
    fstream  fin(filename.c_str(), ios::in);
    fin >> indexNum;
    for (int i = 0; i < indexNum; i++)
    {
        Index temp_index;
        fin >> temp_index.index_name;
        fin >> temp_index.table_name;
        fin >> temp_index.column;
        fin >> temp_index.columnLength;
        fin >> temp_index.blockNum;
        indexes.push_back(temp_index);
    }
    fin.close();
}

void CatalogManager::StoreTalbeCatalog() {
    string filename = "table.catlog";
    fstream  fout(filename.c_str(), ios::out);

    fout << tableNum << endl;
    for (int i = 0; i < tableNum; i++)
    {
        fout << tables[i].name << " ";
        fout << tables[i].attriNum << " ";
        fout << tables[i].blockNum << endl;

        for (int j = 0; j < tables[i].attriNum; j++)
        {
            fout << tables[i].attributes[j].name << " ";
            fout << tables[i].attributes[j].type << " ";
            fout << tables[i].attributes[j].length << " ";
            fout << tables[i].attributes[j].isUnique << " ";
            fout << tables[i].attributes[j].isPrimeryKey << endl;
        }
    }
    fout.close();
}

void CatalogManager::StoreIndexCatalog() {
    string filename = "index.catlog";
    fstream  fout(filename.c_str(), ios::out);
    fout << indexNum << endl;
    for (int i = 0; i < indexNum; i++)
    {
        fout << indexes[i].index_name << " ";
        fout << indexes[i].table_name << " ";
        fout << indexes[i].column << " ";
        fout << indexes[i].columnLength << " ";
        fout << indexes[i].blockNum << endl;
    }
    fout.close();
}

void CatalogManager::createTable(Table& table) {
    tableNum++;
    for (int i = 0; i < table.attributes.size(); i++) {
        table.totalLength += table.attributes[i].length;
    }
    tables.push_back(table);
}

void CatalogManager::createIndex(Index index) {
    indexNum++;
    indexes.push_back(index);
}

void CatalogManager::dropTable(Table table) {
    dropTable(table.name);
}

void CatalogManager::dropIndex(Index index) {
    dropIndex(index.index_name);
}

void CatalogManager::dropTable(string tablename) {
    for (int i = tableNum - 1; i >= 0; i--) {
        if (tables[i].name == tablename) {
            tables.erase(tables.begin() + i);
            tableNum--;
        }
    }
    for (int i = indexNum - 1; i >= 0; i--) {
        if (indexes[i].table_name == tablename) {
            indexes.erase(indexes.begin() + i);
            indexNum--;
        }
    }
}

void CatalogManager::dropIndex(string index_name) {
    for (int i = indexNum - 1; i >= 0; i--) {
        if (indexes[i].index_name == index_name) {
            indexes.erase(indexes.begin() + i);
            indexNum--;
        }
    }
}

void CatalogManager::update(Table& tableinfor) {
    for (int i = 0; i < tableNum; i++) {
        if (tables[i].name == tableinfor.name) {
            tables[i].attriNum = tableinfor.attriNum;
            tables[i].blockNum = tableinfor.blockNum;
            tables[i].totalLength = tableinfor.totalLength;
            tables[i].attributes = tableinfor.attributes;
        }
    }
}

void CatalogManager::update(Index& index) {
    for (int i = 0; i< indexNum; i++) {
        if (indexes[i].index_name == index.index_name) {
            indexes[i].table_name = index.table_name;
            indexes[i].column = index.column;
            indexes[i].blockNum = index.blockNum;
        }
    }
}

bool CatalogManager::ExistTable(string tablename) {
    int i;
    for (i = 0; i<tables.size(); i++) {
        if (tables[i].name == tablename)
            return true;
    }
    return false;
}

bool CatalogManager::ExistIndex(string tablename, int column) {
    int i;
    for (i = 0; i < indexes.size(); i++) {
        if (indexes[i].table_name == tablename && indexes[i].column == column)
            break;//found it
    }
    if (i >= indexes.size()) return 0;
    else return 1;
}

bool CatalogManager::ExistIndex(string indexname) {
    int i;
    for (i = 0; i <indexes.size(); i++) {
        if (indexes[i].index_name == indexname)
            break;//found it
    }
    if (i >= indexes.size()) return 0;
    else return 1;
}

Table CatalogManager::getTableInformation(string tablename) {
    int i;
    Table temp;
    for (i = 0; i<tableNum; i++) {
        if ((tables[i].name) == tablename) {

            return tables[i];
        }
    }
    return temp;
}

Index CatalogManager::getIndexInformation(string tablename, int column) {
    int i;
    for (i = 0; i < indexes.size(); i++) {
        if (indexes[i].table_name == tablename && indexes[i].column == column)
            break;//found it
    }
    if (i >= indexNum) {
        Index tmpt;
        return tmpt;//indicate that table information not found
    }
    return indexes[i];
}

Index CatalogManager::getIndexInformation(string indexName) {
    int i;
    for (i = 0; i < tableNum; i++) {
        if (indexes[i].index_name == indexName)
            break;//found it
    }
    if (i >= indexNum) {
        Index tmpt;
        return tmpt;//indicate that table information not found
    }
    return indexes[i];
}

int CatalogManager::GetColumnNumber(Table& tableinfo, string columnname)
{
    for (int i = 0; i<tableinfo.attributes.size(); i++) {
        if (tableinfo.attributes[i].name == columnname) {
            return i;
        }
    }
    return -1;
}

int CatalogManager::GetColumnAmount(Table& tableinfo) {
    return tableinfo.attributes.size();
}
