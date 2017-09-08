#include "RecordManager.h"
#include <iostream>

bool RecordManager::Comparator(Table tableinfor, Row row, vector<Condition> conditions)
{
    for (int i = 0; i<conditions.size(); i++) {
        int colNum = conditions[i].columnNum;
        string value1 = "";
        string value2 = conditions[i].value;
        int length1 = 0;
        int length2 = value2.length();
        for (int k = 0; k < row.columns[colNum].length(); k++) {
            if (row.columns[colNum].c_str()[k] == EMPTY) {
                length1 = k;
                break;
            }
            value1 += row.columns[colNum].c_str()[k];
        }
        float fvalue1 = atof(value1.c_str());
        float fvalue2 = atof(value2.c_str());
        switch (tableinfor.attributes[colNum].type)
        {
        case CHAR:
            switch (conditions[i].op)
            {
            case Lt:
                if (value1 >= value2) return 0;
                break;
            case Le:
                if (value1 > value2) return 0;
                break;
            case Gt:
                if (value1 <= value2) return 0;
                break;
            case Ge:
                if (value1 < value2) return 0;
                break;
            case Eq:
                if (value1 != value2) return 0;
                break;
            case Ne:
                if (value1 == value2) return 0;
                break;
            }
            break;
        case INT:
            switch (conditions[i].op)
            {
            case Lt:
                if (length1 > length2) return 0;
                else if (length1 < length2) break;
                else if (value1 >= value2) return 0;
                break;
            case Le:
                if (length1 > length2) return 0;
                else if (length1 < length2) break;
                else if (value1 > value2) return 0;
                break;
            case Gt:
                if (length1 < length2) return 0;
                else if (length1 > length2) break;
                else if (value1 <= value2) return 0;
                break;
            case Ge:
                if (length1 < length2) return 0;
                else if (length1 > length2) break;
                else if (value1 < value2) return 0;
                break;
            case Eq:
                if (length1 != length2) return 0;
                else if (value1 != value2) return 0;
                break;
            case Ne:
                if (length1 != length2) break;
                else if (value1 == value2) return 0;
                break;
            }
            break;
        case FLOAT:
            switch (conditions[i].op)
            {
            case Lt:
                if (fvalue1 >= fvalue2) return 0;
                break;
            case Le:
                if (fvalue1 > fvalue2) return 0;
                break;
            case Gt:
                if (fvalue1 <= fvalue2) return 0;
                break;
            case Ge:
                if (fvalue1 < fvalue2) return 0;
                break;
            case Eq:
                if (fvalue1 != fvalue2) return 0;
                break;
            case Ne:
                if (fvalue1 == fvalue2) return 0;
                break;
            }
        }
    }
    return 1;
}

Data RecordManager::select(Table& tableinfor) {
    string filename = tableinfor.name + ".table";
    string stringrow;
    Row splitedRow;
    Data datas;
    int length = tableinfor.totalLength + 1;//加多一位来判断这条记录是否被删除了
    const int recordNum = BLOCKSIZE / length;

    for (int blockOffset = 0; blockOffset < tableinfor.blockNum; blockOffset++) {
        int bufferNum = buf.getIfIsInBuffer(filename, blockOffset);
        if (bufferNum == -1) {
            bufferNum = buf.getEmptyBuffer();
            buf.readBlock(filename, blockOffset, bufferNum);
        }
        for (int offset = 0; offset < recordNum; offset++) {
            int position = offset * length;
            stringrow = buf.bufferBlock[bufferNum].getvalues(position, position + length);
            if (stringrow.c_str()[0] == EMPTY) continue;//indicate that this row of record have been deleted
            stringrow.erase(stringrow.begin());//把第一位去掉
            splitedRow = splitRow(tableinfor, stringrow);
            datas.rows.push_back(splitedRow);
        }
    }
    return datas;
}

Data RecordManager::select(Table tableinfor, vector<Condition> conditions) {
    Data datas;
    if (conditions.size() == 0) {
        datas = select(tableinfor);
        return datas;
    }
    string filename = tableinfor.name + ".table";
    //buf.scanIn(tableinfor);
    string stringrow;
    Row splitedRow;
    int length = tableinfor.totalLength + 1;
    const int recordNum = BLOCKSIZE / length;

    for (int blockOffset = 0; blockOffset < tableinfor.blockNum; blockOffset++) {
        int bufferNum = buf.getIfIsInBuffer(filename, blockOffset);
        if (bufferNum == -1) {
            bufferNum = buf.getEmptyBuffer();
            buf.readBlock(filename, blockOffset, bufferNum);
        }
        for (int offset = 0; offset < recordNum; offset++) {
            int position = offset * length;
            stringrow = buf.bufferBlock[bufferNum].getvalues(position, position + length);
            if (stringrow.c_str()[0] != EMPTY) {
                stringrow.erase(stringrow.begin());
                splitedRow = splitRow(tableinfor, stringrow);
                if (Comparator(tableinfor, splitedRow, conditions)) {
                    datas.rows.push_back(splitedRow);
                }
            }
        }
    }
    return datas;
}

void RecordManager::insertValue(Table& tableinfor, Row& splitedRow) {
    string stringrow = connectRow(tableinfor, splitedRow);
    insertPos iPos = buf.getInsertPosition(tableinfor);
    buf.bufferBlock[iPos.bufferNUM].values[iPos.position] = NOTEMPTY;
	cout << "record:[ primary key= ";
    for (int i = 0; i < tableinfor.totalLength; i++) {
        buf.bufferBlock[iPos.bufferNUM].values[iPos.position + i + 1] = stringrow.c_str()[i];
		if(i<=9)
			cout << stringrow.c_str()[i] << "";
    }
    buf.bufferBlock[iPos.bufferNUM].isWritten = 1;
}

int RecordManager::deleteValue(Table tableinfor) {
    string filename = tableinfor.name + ".table";
    int count = 0;
    const int recordNum = BLOCKSIZE / (tableinfor.totalLength + 1);
    for (int blockOffset = 0; blockOffset < tableinfor.blockNum; blockOffset++) {
        int bufferNum = buf.getIfIsInBuffer(filename, blockOffset);
        if (bufferNum == -1) {
            bufferNum = buf.getEmptyBuffer();
            buf.readBlock(filename, blockOffset, bufferNum);
        }
        for (int offset = 0; offset < recordNum; offset++) {
            int position = offset * (tableinfor.totalLength + 1);
            if (buf.bufferBlock[bufferNum].values[position] != EMPTY) {
                buf.bufferBlock[bufferNum].values[position] = EMPTY;
                count++;
            }
        }
    }
    return count;
}

int RecordManager::deleteValue(Table tableinfor, vector<Condition> conditions) {
    string filename = tableinfor.name + ".table";
    string stringrow;
    Row splitedRow;
    int count = 0;
    const int recordNum = BLOCKSIZE / (tableinfor.totalLength + 1);
    for (int blockOffset = 0; blockOffset < tableinfor.blockNum; blockOffset++) {
        int bufferNum = buf.getIfIsInBuffer(filename, blockOffset);
        if (bufferNum == -1) {
            bufferNum = buf.getEmptyBuffer();
            buf.readBlock(filename, blockOffset, bufferNum);
        }
        for (int offset = 0; offset < recordNum; offset++) {
            int position = offset * (tableinfor.totalLength + 1);
            stringrow = buf.bufferBlock[bufferNum].getvalues(position, position + tableinfor.totalLength + 1);
            if (stringrow.c_str()[0] != EMPTY) {
                stringrow.erase(stringrow.begin());
                splitedRow = splitRow(tableinfor, stringrow);
                if (Comparator(tableinfor, splitedRow, conditions)) {
                    buf.bufferBlock[bufferNum].values[position] = DELETED;
                    count++;
                }
            }
        }
        buf.bufferBlock[bufferNum].isWritten = 1;
    }
    return count;
}

void RecordManager::dropTable(Table tableinfo) {
    string filename = tableinfo.name + ".table";
    if (remove(filename.c_str()) != 0)
        perror("Error deleting file");
    else
        buf.setInvalid(filename);
}

void RecordManager::createTable(Table tableinfo) {
    string filename = tableinfo.name + ".table";
    fstream  fout(filename.c_str(), ios::out);
    fout.close();
}

Row RecordManager::splitRow(Table tableinfor, string row) {
    Row splitedRow;
    int s_pos = 0, f_pos = 0;
    for (int i = 0; i < tableinfor.attriNum; i++) {
        s_pos = f_pos;
        f_pos += tableinfor.attributes[i].length;
        string col;
        for (int j = s_pos; j < f_pos; j++) {
            col += row[j];
        }
        splitedRow.columns.push_back(col);
    }
    return splitedRow;
}

string RecordManager::connectRow(Table tableinfor, Row splitedRow) {
    string tmptRow;
    string stringrow;
    for (int i = 0; i < splitedRow.columns.size(); i++) {
        tmptRow = splitedRow.columns[i];
        for (; tmptRow.length() < tableinfor.attributes[i].length; tmptRow += EMPTY);//补'@',定长存储
        stringrow += tmptRow;
    }
    return stringrow;
}

void RecordManager::showDatas(const Data& datas) const {
    if (datas.rows.size() == 0)
    {
        cout << "No Datas" << endl;
        return;
    }
    for (int i = 0; i< datas.rows.size(); i++) {
        cout << (i + 1) << " ";//序号
        for (int j = 0; j<datas.rows[i].columns.size(); j++) {
            for (int k = 0; k < datas.rows[i].columns[j].length(); k++)
                if (datas.rows[i].columns[j].c_str()[k] == EMPTY) break;
                else cout << datas.rows[i].columns[j].c_str()[k];
                cout << '\t';
        }
        cout << endl;
    }
}
