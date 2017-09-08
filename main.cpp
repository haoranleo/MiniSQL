#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include "BufferManager.h"
#include "CatalogManager.h"
#include "RecordManager.h"
#include "IndexManager.h"
#include "Interpret.h"
#include "Sth.h"
#include "API.h"
#include "macro.h"

using namespace std;

RecordManager record;
IndexManager index;
CatalogManager catalog;
Interpret interprt;
BufferManager buf;
API api;

void quHuanHang(char* input);
int main()
{
    cout << "**************Welcome to use MiniSQL Version 1.0 O(∩_∩)O**************" << endl;
    vector<Condition> conditions;
    Table tableinfor;
    Index indexinfor;
    Row insertValue;
    Data datas;
    char command[COMLEN] = "";
    char input[INPUTLEN] = "";
    char word[WORDLEN] = "";
    short int ComEnd = 0;

    while (1)
    {
        string s;
        strcpy(command, "");
        ComEnd = 0;
        cout << "MiniSQL>";
		int flag = 0;
        while (!ComEnd)
        {   
			if (flag == 1)
				cout << "      ->";
			flag = 1;
            if (!api.fileflag) {               
                gets_s(input);
            }
            else {   
                if (fgets(input, INPUTLEN, api.fp) != NULL) {
                    quHuanHang(input);
                }
                else { //如果读到脚本末尾标记，则退出文件读取状态，设置flag=0;
                    api.fileflag = 0;
                    fclose(api.fp);
                    continue;
                }                
            }

            if (api.IsComEnd(input))
                ComEnd = 1;
            strcat(command, input);
            api.AddSeperator(command);
        }
        interprt.Parse(command);
        
        api.Execute();
    }
    getchar();
    return 0;
}

void quHuanHang(char* input) {
    unsigned int i = strlen(input) - 2;
    input[i] = '\0';
    return;
}
