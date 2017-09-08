#ifndef _interpret_h_
#define _interpret_h_

class Interpret {
public:
	int m_operation; //²Ù×÷ºê
    char m_filename[30];
	string m_tabname;		
	string m_indname;		
	string m_colname;
	vector<Attribute> column;
	vector<Condition> condition;	
	Row row;	
	Table getTableInfo;
	Index getIndexInfo;
	int PrimaryKeyPosition;
	int UniquePostion;
	vector<Condition> UniqueCondition;
	friend void Execute();
	Interpret(){
		m_operation =UNKNOW;
		m_tabname = "";
		m_indname = "";
		m_colname="";
		PrimaryKeyPosition=-1;	
		UniquePostion=-1;
	}
	~Interpret(){}

	void Parse(char* command);
	void initcol();
	void initcond();
	void initvalue();
	void inittable(){
		getTableInfo.name="";
		getTableInfo.attriNum=getTableInfo.blockNum=getTableInfo.totalLength=0;
		getTableInfo.attributes.clear();
	}
	void initindex(){
		getIndexInfo.blockNum=getIndexInfo.column=-1;
		getIndexInfo.index_name="";
	}
	bool GetWord(string & src, string & des);
    bool GetFileName(string & src, char* des);

protected:
	
	void MakeInitilate(){
		m_operation =UNKNOW;
		m_tabname = "";
		m_indname = "";
		initcol();
		initcond();
		initvalue();
		inittable();
		initindex();
	}

	bool GetStr(string & src, string & des);

	short int IsInt(const char *);
	short int IsFloat(char *input);
};

#endif
