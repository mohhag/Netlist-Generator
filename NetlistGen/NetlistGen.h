//Netlis Generator source code
//Written by Mohammad Hashem Haghbayan

//#include "test.h"
//#include "function.h"
//#include "HM_FaultSimulation.h"

#include<sstream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stack>
#include <windows.h>
#include<tchar.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <IO.h>
#include <fstream>

#include <iostream>

#include <conio.h>


using namespace std;


class node{
public:
	string s;
	node* next;
};

class GateStruct{
public:
	GateStruct* next;

	GateStruct();

	int Type; //0 buffer, 1 AND, 2 OR, 3 XOR, 4 FDCPE, 5, FTCPE, 6 FDDCPE, 7 FTDCPE, 8 LDCP, 9 Fanout//---------GATE TYPE-------------
	int brachNumber;
	string INS;
	string Output;
	node* input;
	bool invert; //0 does not invert allowed
	int inputNumber;
	long int idNumber;
	short int level;
	int levelNumber;
	int inputNumber_temp;
	string path;
	//---------------Flip Flop Properties------------------
	//FDCE
	string Q;
	string D;
	string C;
	string CLR;
	string CE;
	string PRE;
};

GateStruct::GateStruct(){

	input = NULL;
	next = NULL;
	Q = "";
	D = "";
	C = "";
	CLR  = "";
	CE = "";
	PRE = "";
	inputNumber = 0;
	inputNumber_temp = 0;
	idNumber = -1;
}



//----------------------------------------------GLOBAL FILES---------------------------------
ofstream ERR("Netllist_Error.txt");
ofstream WAR("Netlist_Warning.txt");
ofstream INF("Netlist_Info.txt");
//-------------------------------------------------------------------------------------------


class ModuleAnalysis{
protected:
	string ProjectName;
	string ModuleName;
	string rptFile;
	string Eq_after_WireAnalyzer_File;
	string erase_space(string);
	string ISEpath;
	node* EX_Inputs_1;
	node* EX_Outputs_1;
	node* EX_Inputs2;
	node* EX_Outputs2;
	node* Inputs;
	node* Outputs;
	node* InputsStr;
	node* OutputsStr;
	node* addedFiles;
	node* InOutOrder; //The order of inputs and outputs
	bool ConstructFlag;

	long int str2int(string);
	string int2str(int);
	string equationFile;
	int DeleteDirectory(const TCHAR*);
	int keywordFinder(string, string);
	int minNum(int, int, int, int, int, int, int, int, int);

	
private:

	int deletComment(string);
	void error_type1(int);
	void error_type2(int);
	void error_type3(int);
	void error_type10();
	void ISE_warning();
	int char2int(char);
	char int2char(int);
	bool IsDots(const TCHAR*);

public:
	ModuleAnalysis(){
	EX_Inputs_1 = NULL;
	EX_Outputs_1 = NULL;
	Inputs = NULL;
	Outputs = NULL;
	EX_Inputs2 = NULL;
	EX_Outputs2 = NULL;
	InOutOrder = NULL;
	InputsStr = NULL;
	OutputsStr = NULL;
	addedFiles = NULL;
	}
	~ModuleAnalysis();

	int readFile();
	int readInOut();
	void runISE();
	void PrintTCLFile();
	int RunBatchFile();
	string getModuleName() {return ModuleName;}

};

//returns minimum number
int ModuleAnalysis::minNum(int x1, int x2,int  x3, int x4, int x5, int x6, int x7, int x8, int x9){

	int temp = -1;
	if( x1 != -1 ){
		temp = x1;
	}else
	{
		if (x2 !=-1){
			temp = x2;
		}else{
			if (x3 != -1){
				temp = x3;
			}else{
				if (x4 != -1){
					temp = x4;
				}else{
					if (x5 != -1){
						temp = x5;
					}else{
						if (x6 != -1){
							temp = x6;
						}else{
							if (x7 != -1){
								temp = x7;
							}else{
								if (x8 != -1){
									temp = x8;
								}else{
									temp = x9;
								}
							}
						}
					}
				}
			}
		}
	}

	if (x2 < temp && x2 != -1) temp = x2;
	if (x3 < temp && x3 != -1) temp = x3;
	if (x4 < temp && x4 != -1) temp = x4;
	if (x5 < temp && x5 != -1) temp = x5;
	if (x6 < temp && x6 != -1) temp = x6;
	if (x7 < temp && x7 != -1) temp = x7;
	if (x8 < temp && x8 != -1) temp = x8;
	if (x9 < temp && x9 != -1) temp = x9;

	return temp;
}

int ModuleAnalysis::keywordFinder(string line, string key){
	
	string key1T = key;
	string key2T = key;
	string key3T = key;
	string key4T = key;
	string key5T = key;
	string key6T = key;
	string key7T = key;
	key1T.insert(key1T.length(), " ");
	key2T.insert(key2T.length(), "\t");
	key3T.insert(key3T.length(), ";");
	key7T.insert(key7T.length(), "[");

	if(line.find(key1T) == 0 || line.find(key2T) == 0 || line.find(key3T) == 0 || line.find(key7T) == 0) return 0;
	if(line == key) return 0;

	key4T.insert(0, " ");
	key5T.insert(0, "\t");
	key6T.insert(0, ";");

	int position = 0;

	while(line.find(key1T) != -1 || line.find(key2T) != -1 || line.find(key3T) != -1 || line.find(key4T) != -1 || line.find(key5T) != -1 || line.find(key6T) != -1 || line.find(key7T) != -1){
		if(line.find(key1T) != -1 && line.find(key1T) == minNum(line.find(key1T), line.find(key2T), line.find(key3T), line.find(key4T), line.find(key5T), line.find(key6T), line.find(key7T), -1, -1)){
			string lineT = line.substr(0, line.find(key1T)+key1T.length()-1);
			if(keywordFinder(lineT, key) == line.find(key1T)) return line.find(key1T) + position;
			else
				position = position + line.find(key1T)+key1T.length();
				line.erase(0, line.find(key1T)+key1T.length());
		}
		if(line.find(key2T) != -1 && line.find(key2T) == minNum(line.find(key1T), line.find(key2T), line.find(key3T), line.find(key4T), line.find(key5T), line.find(key6T), line.find(key7T), -1, -1)){
			string lineT = line.substr(0, line.find(key2T)+key2T.length()-1);
			if(keywordFinder(lineT, key) == line.find(key2T)) return line.find(key2T) + position;
			else
				position = position + line.find(key2T)+key2T.length();
				line.erase(0, line.find(key2T)+key2T.length());
		}
		if(line.find(key3T) != -1 && line.find(key3T) == minNum(line.find(key1T), line.find(key2T), line.find(key3T), line.find(key4T), line.find(key5T), line.find(key6T), line.find(key7T), -1, -1)){
			string lineT = line.substr(0, line.find(key3T)+key3T.length()-1);
			if(keywordFinder(lineT, key) == line.find(key3T)) return line.find(key3T) + position;
			else
				position = position + line.find(key3T)+key3T.length();
				line.erase(0, line.find(key3T)+key3T.length());
		}

		if(line.find(key4T) != -1 && line.find(key4T) == minNum(line.find(key1T), line.find(key2T), line.find(key3T), line.find(key4T), line.find(key5T), line.find(key6T), line.find(key7T), -1, -1)){
			string lineT = line.substr(line.find(key4T)+1, line.length());
			if(keywordFinder(lineT, key) == 0) return line.find(key4T)+1 + position;
			else
				position = position + line.find(key4T)+key4T.length();
				line.erase(0, line.find(key4T)+key4T.length());
		}
		if(line.find(key5T) != -1 && line.find(key5T) == minNum(line.find(key1T), line.find(key2T), line.find(key3T), line.find(key4T), line.find(key5T), line.find(key6T), line.find(key7T), -1, -1)){
			string lineT = line.substr(line.find(key5T)+1, line.length());
			if(keywordFinder(lineT, key) == 0) return line.find(key5T)+1 + position;
			else
				position = position + line.find(key5T)+key5T.length();
				line.erase(0, line.find(key5T)+key5T.length());
		}
		if(line.find(key6T) != -1 && line.find(key6T) == minNum(line.find(key1T), line.find(key2T), line.find(key3T), line.find(key4T), line.find(key5T), line.find(key6T), line.find(key7T), -1, -1)){
			string lineT = line.substr(line.find(key6T)+1, line.length());
			if(keywordFinder(lineT, key) == 0) return line.find(key6T)+1 + position;
			else
				position = position + line.find(key6T)+key6T.length();
				line.erase(0, line.find(key6T)+key6T.length());
		}
		if(line.find(key7T) != -1 && line.find(key7T) == minNum(line.find(key1T), line.find(key2T), line.find(key3T), line.find(key4T), line.find(key5T), line.find(key6T), line.find(key7T), -1, -1)){
			string lineT = line.substr(line.find(key7T)+1, line.length());
			if(keywordFinder(lineT, key) == 0) return line.find(key7T)+1 + position;
			else
				position = position + line.find(key7T)+key7T.length();
				line.erase(0, line.find(key7T)+key7T.length());
		}


		if(line == key) return position;
	}

	return -1;
}


int ModuleAnalysis::DeleteDirectory(const TCHAR* sPath) {
   HANDLE hFind;    // file handle
   WIN32_FIND_DATA FindFileData;

   TCHAR DirPath[MAX_PATH];
   TCHAR FileName[MAX_PATH];

   _tcscpy(DirPath,sPath);
   _tcscat(DirPath,"\\*");    // searching all files
   _tcscpy(FileName,sPath);
   _tcscat(FileName,"\\");

   // find the first file
   hFind = FindFirstFile(DirPath,&FindFileData);
   if(hFind == INVALID_HANDLE_VALUE) return FALSE;
   _tcscpy(DirPath,FileName);

   bool bSearch = true;
   while(bSearch) {    // until we find an entry
      if(FindNextFile(hFind,&FindFileData)) {
         if(IsDots(FindFileData.cFileName)) continue;
         _tcscat(FileName,FindFileData.cFileName);
         if((FindFileData.dwFileAttributes &
            FILE_ATTRIBUTE_DIRECTORY)) {

            // we have found a directory, recurse
            if(!DeleteDirectory(FileName)) {
                FindClose(hFind);
                return FALSE;    // directory couldn't be deleted
            }
            // remove the empty directory
            RemoveDirectory(FileName);
             _tcscpy(FileName,DirPath);
         }
         else {
            if(FindFileData.dwFileAttributes &
               FILE_ATTRIBUTE_READONLY)
               // change read-only file mode
                  _chmod(FileName, _S_IWRITE);
                  if(!DeleteFile(FileName)) {    // delete the file
                    FindClose(hFind);
                    return FALSE;
               }
               _tcscpy(FileName,DirPath);
         }
      }
      else {
         // no more files there
         if(GetLastError() == ERROR_NO_MORE_FILES)
         bSearch = false;
         else {
            // some error occurred; close the handle and return FALSE
               FindClose(hFind);
               return FALSE;
         }

      }

   }
   FindClose(hFind);// close the file handle

   int btemp = RemoveDirectory(sPath);     // remove the empty directory

   return btemp;

}
bool ModuleAnalysis::IsDots(const TCHAR* str){

	if(_tcscmp(str, ".") && _tcscmp(str, "..")) return FALSE;
	return TRUE;
}


ModuleAnalysis::~ModuleAnalysis(){
	stack<node*> s_temp;
	node* temp = EX_Inputs_1;

	while(temp){
		s_temp.push(temp);
		temp = temp->next;
	}



	while(!s_temp.empty()){

		delete(s_temp.top());
		s_temp.pop();

	}

	temp = EX_Outputs_1;

	while(temp){
		s_temp.push(temp);
		temp = temp->next;
	}



	while(!s_temp.empty()){

		delete(s_temp.top());
		s_temp.pop();

	}

	temp = EX_Outputs2;

	while(temp){
		s_temp.push(temp);
		temp = temp->next;
	}



	while(!s_temp.empty()){

		delete(s_temp.top());
		s_temp.pop();

	}

	temp = EX_Inputs2;

	while(temp){
		s_temp.push(temp);
		temp = temp->next;
	}



	while(!s_temp.empty()){

		delete(s_temp.top());
		s_temp.pop();

	}

	temp = Inputs;

	while(temp){
		s_temp.push(temp);
		temp = temp->next;
	}



	while(!s_temp.empty()){

		delete(s_temp.top());
		s_temp.pop();

	}

	temp = Outputs;

	while(temp){
		s_temp.push(temp);
		temp = temp->next;
	}



	while(!s_temp.empty()){

		delete(s_temp.top());
		s_temp.pop();

	}

	temp = OutputsStr;

	while(temp){
		s_temp.push(temp);
		temp = temp->next;
	}



	while(!s_temp.empty()){

		delete(s_temp.top());
		s_temp.pop();

	}

	temp = InputsStr;

	while(temp){
		s_temp.push(temp);
		temp = temp->next;
	}



	while(!s_temp.empty()){

		delete(s_temp.top());
		s_temp.pop();

	}

}

void ModuleAnalysis::runISE(){

	char slatchtemp[2];
	slatchtemp[0] = 92;
	slatchtemp[1] = 0;
	string slatch = slatchtemp;
	

	if(!ConstructFlag){

		ifstream ISEPATH("ISEPath.txt");
		if(!ISEPATH){

			cout<<"\nISE installation path request...";
			cout<<"\nIn this phase you should enter the location of the 'xtclsh.exe' file that is in your xilinx ISE installation directory.";
			cout<<"\nIf you used the default path for installation in Windows NT compatible OS (C:"<<slatchtemp<<"Xilinx"<<slatchtemp<<"12.2"<<slatchtemp<<"ISE_DS"<<slatchtemp<<"ISE"<<slatchtemp<<"bin"<<slatchtemp<<"nt"<<slatchtemp<<") press 'd' or 'D':";
			cin>>ISEpath;
			ofstream ISEPATH2("ISEPath.txt");
			

			if (ISEpath == "d" || ISEpath == "D") {
				ISEpath = "C:";
				ISEpath.insert(ISEpath.length(), slatch);
				ISEpath.insert(ISEpath.length(), "Xilinx");
				ISEpath.insert(ISEpath.length(), slatch);
				ISEpath.insert(ISEpath.length(), "12.2");
				ISEpath.insert(ISEpath.length(), slatch);
				ISEpath.insert(ISEpath.length(), "ISE_DS");
				ISEpath.insert(ISEpath.length(), slatch);
				ISEpath.insert(ISEpath.length(), "ISE");
				ISEpath.insert(ISEpath.length(), slatch);
				ISEpath.insert(ISEpath.length(), "bin");
				ISEpath.insert(ISEpath.length(), slatch);
				ISEpath.insert(ISEpath.length(), "nt");
				ISEpath.insert(ISEpath.length(), slatch);

			}

			ISEPATH2<<ISEpath<<endl;
		}else{
			char charTempP[10000];
			ISEPATH.getline(charTempP, 10000);
			string strTempP = charTempP;
			cout<<"\nISE installation path request...";
			cout<<"\nIn this phase you should enter the location of the 'xtclsh.exe' file that is in your xilinx ISE installation directory.";
			cout<<"\nHint: Usually the default ISE path is similar to (C:"<<slatchtemp<<"Xilinx"<<slatchtemp<<"12.2"<<slatchtemp<<"ISE_DS"<<slatchtemp<<"ISE"<<slatchtemp<<"bin"<<slatchtemp<<"nt"<<slatchtemp<<")";
			cout<<"\nThe current ISE path is ("<<strTempP<<"). If you want to modify the current ISE path enter the new path otherwise press 'd' or 'D':";
			cin>>ISEpath;

			if (ISEpath == "d" || ISEpath == "D") {
				ISEpath = strTempP;
			}else{
				remove("ISEPath.txt");
				ofstream ISEPATH2("ISEPath.txt");
				ISEPATH2<<ISEpath<<endl;
			}
		}
	
	}
	return;
}

void ModuleAnalysis::PrintTCLFile(){

	ofstream TEMP_FILE("TclFile.tcl");

	TEMP_FILE<<"#Module 0"<<endl;
	TEMP_FILE<<"project new "<<ProjectName<<".ise"<<endl;
	TEMP_FILE<<"project set family CoolRunner2"<<endl;
	TEMP_FILE<<"project set device XC2C512"<<endl;
	TEMP_FILE<<"project set package PQ208"<<endl;
	TEMP_FILE<<"project set speed -7"<<endl;

	node* Temp = addedFiles;

	if(Temp){
		TEMP_FILE<<"xfile add "<<Temp->s<<".v"<<endl;
		Temp = Temp->next;

		while(Temp){
			TEMP_FILE<<"xfile add "<<Temp->s<<endl;
			Temp = Temp->next;
		}
	}

	TEMP_FILE<<"set top_name "<<ModuleName<<endl;

	char k = '"';
	TEMP_FILE<<"process run "<<k<<"Synthesize - XST"<<k<<endl;
	TEMP_FILE<<"project close"<<endl;
	TEMP_FILE.close();

}

///////////////////////////////////////////////////////////////////////////////////////////////////
int ModuleAnalysis::RunBatchFile()
{

	STARTUPINFO startupInfo = {0};
	startupInfo.cb = sizeof(startupInfo);
	PROCESS_INFORMATION processInformation;

	string currentDIR = "";
	TCHAR Buffer[100];
	DWORD Dwret = GetCurrentDirectory(100, Buffer);
	cout<<"The current directory is: "<<Buffer<<endl;
	cout<<"Running ISE...\n";
	cout<<"\n**************************************************\n";
	cout<<"Please wait...\n";
	currentDIR = Buffer;

	string Tempxst = "";

	//--------------------------------------------
	char chartemp4[2];
	chartemp4[0] = '"';
	chartemp4[1] = 0;
	string dc = chartemp4;

	char slatchtemp[2];
	slatchtemp[0] = 92;
	slatchtemp[1] = 0;
	string slatch = slatchtemp;
	//--------------------------------------------
	currentDIR.insert(currentDIR.length(), slatch);


	string lpcommand = "";
	lpcommand = ISEpath;
	lpcommand.insert(lpcommand.length(), "xtclsh.exe");
	char chartemp1[512];
	for(int i=0; i<lpcommand.length();i++)
		chartemp1[i] = lpcommand[i];

	chartemp1[lpcommand.length()] = 0;





	if (!CreateProcess(chartemp1,  "source TclFile.tcl", NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS,
		NULL, NULL, &startupInfo, &processInformation))
	{
		cout<<"\nERROR : Running ISE failed.";
		cout<<"\n-Make sure the entered path is correct.";
		cout<<"\n-Make sure the default path in ISEPath.txt is correct.";
		char slatchtemp[2];
		slatchtemp[0] = 92;
		slatchtemp[1] = 0;
		string slatch = slatchtemp;
		cout<<"\n-Make sure the path is ended by "<<slatch<<".";
		cout<<"\nPress any key to end...";
		getch();
		exit(0);
		return -1;
	}
	WaitForSingleObject( processInformation.hProcess, INFINITE);
	CloseHandle (processInformation.hProcess);
	CloseHandle (processInformation.hThread);
	
	
	//-------------------------------------------

	/*Tempxst.insert(Tempxst.length(), "xst -ise ");
	Tempxst.insert(Tempxst.length(), dc);
	Tempxst.insert(Tempxst.length(), currentDIR);
	Tempxst.insert(Tempxst.length(), ModuleName);
	Tempxst.insert(Tempxst.length(), ".ise");
	Tempxst.insert(Tempxst.length(), dc);
	Tempxst.insert(Tempxst.length(), " -intstyle ise -ifn ");
	Tempxst.insert(Tempxst.length(), ModuleName);
	Tempxst.insert(Tempxst.length(), ".xst -ofn ");
	Tempxst.insert(Tempxst.length(), ModuleName);
	Tempxst.insert(Tempxst.length(), ".syr");


	char chartemp5[512];
	for(i=0; i<Tempxst.length();i++)
		chartemp5[i] = Tempxst[i];

	chartemp5[Tempxst.length()] = 0;

	if (!CreateProcess(NULL, chartemp5, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS,
		NULL, NULL, &startupInfo, &processInformation))
	{
		cout<<"ERROR : Running ISE failed, Make sure the path is correct...\n";
		return;
	}

	WaitForSingleObject( processInformation.hProcess, INFINITE);
	CloseHandle (processInformation.hProcess);
	CloseHandle (processInformation.hThread);


	string Tempngd = "";
	Tempngd.insert(Tempngd.length(), "ngdbuild -ise ");
	Tempngd.insert(Tempngd.length(), dc);
	Tempngd.insert(Tempngd.length(), currentDIR);
	Tempngd.insert(Tempngd.length(), ModuleName);
	Tempngd.insert(Tempngd.length(), ".ise");
	Tempngd.insert(Tempngd.length(), dc);
	Tempngd.insert(Tempngd.length(), " -intstyle ise -dd _ngo -i -p xbr ");
	Tempngd.insert(Tempngd.length(), dc);
	Tempngd.insert(Tempngd.length(), ModuleName);
	Tempngd.insert(Tempngd.length(), ".ngc");
	Tempngd.insert(Tempngd.length(), dc);
	Tempngd.insert(Tempngd.length(), " ");
	Tempngd.insert(Tempngd.length(), ModuleName);
	Tempngd.insert(Tempngd.length(), ".ngd");

	char chartemp6[512];
	for(i=0; i<Tempngd.length();i++)
		chartemp6[i] = Tempngd[i];

	chartemp6[Tempngd.length()] = 0;

	if (!CreateProcess(NULL, chartemp6, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS,
		NULL, NULL, &startupInfo, &processInformation))
	{
		cout<<"ERROR : Running ISE failed, Make sure the path is correct...\n";
		return;
	}

	WaitForSingleObject( processInformation.hProcess, INFINITE);
	CloseHandle (processInformation.hProcess);
	CloseHandle (processInformation.hThread);

	string Tempcpl = "";
	Tempcpl.insert(Tempcpl.length(), "cpldfit -ise ");
	Tempcpl.insert(Tempcpl.length(), dc);
	Tempcpl.insert(Tempcpl.length(), currentDIR);
	Tempcpl.insert(Tempcpl.length(), ModuleName);
	Tempcpl.insert(Tempcpl.length(), ".ise");
	Tempcpl.insert(Tempcpl.length(), dc);
	Tempcpl.insert(Tempcpl.length(), " -intstyle ise -p xc2c512-7-PQ208 -ofmt verilog -optimize density -htmlrpt -loc on -slew fast -init low -inputs 32 -inreg on -blkfanin 38 -pterms 28 -unused keeper -terminate keeper -iostd LVCMOS18 ");
	Tempcpl.insert(Tempcpl.length(), ModuleName);
	Tempcpl.insert(Tempcpl.length(), ".ngd");

	char chartemp7[512];
	for(i=0; i<Tempcpl.length();i++)
		chartemp7[i] = Tempcpl[i];

	chartemp7[Tempcpl.length()] = 0;

	if (!CreateProcess(NULL, chartemp7, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS,
		NULL, NULL, &startupInfo, &processInformation))
	{
		cout<<"ERROR : Running ISE failed, Make sure the path is correct...\n";
		return;
	}

	WaitForSingleObject( processInformation.hProcess, INFINITE);
	CloseHandle (processInformation.hProcess);
	CloseHandle (processInformation.hThread);

	
	*/
	char chtemp2[512];

	for(int i=0; i<ModuleName.length();i++)
		chtemp2[i] = ModuleName[i];
	chtemp2[ModuleName.length()] = '.';
	chtemp2[ModuleName.length()+1] ='s';
	chtemp2[ModuleName.length()+2] ='y';
	chtemp2[ModuleName.length()+3] ='r';
	chtemp2[ModuleName.length()+4] = 0;

	ifstream FILE_TEMP(chtemp2);
	if(!FILE_TEMP){
		error_type10();
		cout<<"\nPress any key to end...";
		getch();
		exit(0);
		return -1;
	} else{

		cout<<"\nCheck the "<<chtemp2<<" in your root directory for any possible errors.\n";
	}

	while(!FILE_TEMP.eof()){
		char str[1000];
		FILE_TEMP.getline(str, 1000);
		string str_temp = str;
		if(str_temp.find("ERROR") == 0) ISE_warning();
	}


	lpcommand = "";
	lpcommand = ISEpath;
	lpcommand.insert(lpcommand.length(), "ngc2edif.exe");
	lpcommand.insert(lpcommand.length(), " ");
	lpcommand.insert(lpcommand.length(), ModuleName);
	lpcommand.insert(lpcommand.length(), ".ngc -w");
	chartemp1[512];
	for(int i=0; i<lpcommand.length();i++)
		chartemp1[i] = lpcommand[i];

	chartemp1[lpcommand.length()] = 0;



	
	char charTemp6[256];
	for(int i = 0; i<ModuleName.length(); i++){
		charTemp6[i] = ModuleName[i];
	}
	charTemp6[ModuleName.length()] = '.';
	charTemp6[ModuleName.length()+1] = 'n';
	charTemp6[ModuleName.length()+2] = 'g';
	charTemp6[ModuleName.length()+3] = 'c';
	charTemp6[ModuleName.length()+4] = 0;

	if (!CreateProcess(NULL,  chartemp1, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS,
		NULL, NULL, &startupInfo, &processInformation))
	{
		cout<<"\nERROR : Running ISE failed.";
		cout<<"\n-Make sure the entered path is correct.";
		cout<<"\n-Make sure the default path in ISEPath.txt is correct.";
		char slatchtemp[2];
		slatchtemp[0] = 92;
		slatchtemp[1] = 0;
		string slatch = slatchtemp;
		cout<<"\n-Make sure the path is ended by "<<slatch<<".";
		cout<<"\nPress any key to end...";
		getch();
		exit(0);
		return -1;

	}

	WaitForSingleObject( processInformation.hProcess, INFINITE);
	CloseHandle (processInformation.hProcess);
	CloseHandle (processInformation.hThread);
	

	

	for(int i=0; i<ModuleName.length();i++)
		chtemp2[i] = ModuleName[i];
	chtemp2[ModuleName.length()] = '.';
	chtemp2[ModuleName.length()+1] ='b';
	chtemp2[ModuleName.length()+2] ='l';
	chtemp2[ModuleName.length()+3] ='d';
	chtemp2[ModuleName.length()+4] = 0;

	remove(chtemp2);


	for(int i=0; i<ModuleName.length();i++)
		chtemp2[i] = ModuleName[i];
	chtemp2[ModuleName.length()] = '.';
	chtemp2[ModuleName.length()+1] ='c';
	chtemp2[ModuleName.length()+2] ='x';
	chtemp2[ModuleName.length()+3] ='t';
	chtemp2[ModuleName.length()+4] = 0;
	remove(chtemp2);

	for(int i=0; i<ModuleName.length();i++)
		chtemp2[i] = ModuleName[i];
	chtemp2[ModuleName.length()] = '.';
	chtemp2[ModuleName.length()+1] ='c';
	chtemp2[ModuleName.length()+2] ='h';
	chtemp2[ModuleName.length()+3] ='k';
	chtemp2[ModuleName.length()+4] = 0;
	remove(chtemp2);

	for(int i=0; i<ModuleName.length();i++)
		chtemp2[i] = ModuleName[i];
	chtemp2[ModuleName.length()] = '.';
	chtemp2[ModuleName.length()+1] ='t';
	chtemp2[ModuleName.length()+2] ='x';
	chtemp2[ModuleName.length()+3] ='t';
	chtemp2[ModuleName.length()+4] = 0;
	remove(chtemp2);


	for(int i=0; i<ModuleName.length();i++)
		chtemp2[i] = ModuleName[i];
	chtemp2[ModuleName.length()] = '.';
	chtemp2[ModuleName.length()+1] ='v';
	chtemp2[ModuleName.length()+2] ='m';
	chtemp2[ModuleName.length()+3] ='6';
	chtemp2[ModuleName.length()+4] = 0;
	remove(chtemp2);


	for(int i=0; i<ModuleName.length();i++)
		chtemp2[i] = ModuleName[i];
	chtemp2[ModuleName.length()] = '.';
	chtemp2[ModuleName.length()+1] ='x';
	chtemp2[ModuleName.length()+2] ='s';
	chtemp2[ModuleName.length()+3] ='t';
	chtemp2[ModuleName.length()+4] = 0;
	remove(chtemp2);


	for(unsigned int i=0; i<ModuleName.length();i++)
		chtemp2[i] = ModuleName[i];
	chtemp2[ModuleName.length()] = '.';
	chtemp2[ModuleName.length()+1] ='p';
	chtemp2[ModuleName.length()+2] ='n';
	chtemp2[ModuleName.length()+3] ='x';
	chtemp2[ModuleName.length()+4] = 0;
	remove(chtemp2);


	for(unsigned int i=0; i<ModuleName.length();i++)
		chtemp2[i] = ModuleName[i];
	chtemp2[ModuleName.length()] = '.';
	chtemp2[ModuleName.length()+1] ='p';
	chtemp2[ModuleName.length()+2] ='a';
	chtemp2[ModuleName.length()+3] ='d';
	chtemp2[ModuleName.length()+4] = 0;
	remove(chtemp2);


	for(unsigned int i=0; i<ModuleName.length();i++)
		chtemp2[i] = ModuleName[i];
	chtemp2[ModuleName.length()] = '.';
	chtemp2[ModuleName.length()+1] ='n';
	chtemp2[ModuleName.length()+2] ='g';
	chtemp2[ModuleName.length()+3] ='r';
	chtemp2[ModuleName.length()+4] = 0;
	remove(chtemp2);


	for(unsigned int i=0; i<ModuleName.length();i++)
		chtemp2[i] = ModuleName[i];
	chtemp2[ModuleName.length()] = '.';
	chtemp2[ModuleName.length()+1] ='n';
	chtemp2[ModuleName.length()+2] ='g';
	chtemp2[ModuleName.length()+3] ='d';
	chtemp2[ModuleName.length()+4] = 0;
	remove(chtemp2);


	for(unsigned int i=0; i<ModuleName.length();i++)
		chtemp2[i] = ModuleName[i];
	chtemp2[ModuleName.length()] = '.';
	chtemp2[ModuleName.length()+1] ='n';
	chtemp2[ModuleName.length()+2] ='g';
	chtemp2[ModuleName.length()+3] ='a';
	chtemp2[ModuleName.length()+4] = 0;
	remove(chtemp2);


	for(unsigned int i=0; i<ModuleName.length();i++)
		chtemp2[i] = ModuleName[i];
	chtemp2[ModuleName.length()] = '.';
	chtemp2[ModuleName.length()+1] ='m';
	chtemp2[ModuleName.length()+2] ='p';
	chtemp2[ModuleName.length()+3] ='d';
	chtemp2[ModuleName.length()+4] = 0;
	remove(chtemp2);


	for(unsigned int i=0; i<ModuleName.length();i++)
		chtemp2[i] = ModuleName[i];
	chtemp2[ModuleName.length()] = '.';
	chtemp2[ModuleName.length()+1] ='m';
	chtemp2[ModuleName.length()+2] ='f';
	chtemp2[ModuleName.length()+3] ='d';
	chtemp2[ModuleName.length()+4] = 0;
	remove(chtemp2);


	for(unsigned int i=0; i<ModuleName.length();i++)
		chtemp2[i] = ModuleName[i];
	chtemp2[ModuleName.length()] = '.';
	chtemp2[ModuleName.length()+1] ='l';
	chtemp2[ModuleName.length()+2] ='s';
	chtemp2[ModuleName.length()+3] ='o';
	chtemp2[ModuleName.length()+4] = 0;
	remove(chtemp2);


	for(unsigned int i=0; i<ModuleName.length();i++)
		chtemp2[i] = ModuleName[i];
	chtemp2[ModuleName.length()] = '.';
	chtemp2[ModuleName.length()+1] ='g';
	chtemp2[ModuleName.length()+2] ='e';
	chtemp2[ModuleName.length()+3] ='d';
	chtemp2[ModuleName.length()+4] = 0;
	remove(chtemp2);


	for(unsigned int i=0; i<ModuleName.length();i++)
		chtemp2[i] = ModuleName[i];
	chtemp2[ModuleName.length()] = '.';
	chtemp2[ModuleName.length()+1] ='g';
	chtemp2[ModuleName.length()+2] ='y';
	chtemp2[ModuleName.length()+3] ='d';
	chtemp2[ModuleName.length()+4] = 0;
	remove(chtemp2);

	for(unsigned int i=0; i<ModuleName.length();i++)
		chtemp2[i] = ModuleName[i];
	chtemp2[ModuleName.length()] = '.';
	chtemp2[ModuleName.length()+1] ='d';
	chtemp2[ModuleName.length()+2] ='a';
	chtemp2[ModuleName.length()+3] ='t';
	chtemp2[ModuleName.length()+4] ='a';
	chtemp2[ModuleName.length()+5] = 0;
	remove(chtemp2);

	for(unsigned int i=0; i<ModuleName.length();i++)
		chtemp2[i] = ModuleName[i];
	chtemp2[ModuleName.length()] = '_';
	chtemp2[ModuleName.length()+1] ='p';
	chtemp2[ModuleName.length()+2] ='a';
	chtemp2[ModuleName.length()+3] ='d';
	chtemp2[ModuleName.length()+4] ='.';
	chtemp2[ModuleName.length()+5] ='t';
	chtemp2[ModuleName.length()+6] ='x';
	chtemp2[ModuleName.length()+7] ='t';
	chtemp2[ModuleName.length()+8] = 0;
	remove(chtemp2);

	for(unsigned int i=0; i<ModuleName.length();i++)
		chtemp2[i] = ModuleName[i];
	chtemp2[ModuleName.length()] = '_';
	chtemp2[ModuleName.length()+1] ='p';
	chtemp2[ModuleName.length()+2] ='a';
	chtemp2[ModuleName.length()+3] ='d';
	chtemp2[ModuleName.length()+4] ='.';
	chtemp2[ModuleName.length()+5] ='c';
	chtemp2[ModuleName.length()+6] ='s';
	chtemp2[ModuleName.length()+7] ='v';
	chtemp2[ModuleName.length()+8] = 0;
	remove(chtemp2);


	for(unsigned int i=0; i<ModuleName.length();i++)
		chtemp2[i] = ModuleName[i];
	chtemp2[ModuleName.length()] = '.';
	chtemp2[ModuleName.length()+1] ='c';
	chtemp2[ModuleName.length()+2] ='m';
	chtemp2[ModuleName.length()+3] ='d';
	chtemp2[ModuleName.length()+4] ='_';
	chtemp2[ModuleName.length()+5] ='l';
	chtemp2[ModuleName.length()+6] ='o';
	chtemp2[ModuleName.length()+7] ='g';
	chtemp2[ModuleName.length()+8] = 0;
	remove(chtemp2);

	for(unsigned int i=0; i<ModuleName.length();i++)
		chtemp2[i] = ModuleName[i];
	chtemp2[ModuleName.length()] = '.';
	chtemp2[ModuleName.length()+1] ='i';
	chtemp2[ModuleName.length()+2] ='s';
	chtemp2[ModuleName.length()+3] ='e';
	chtemp2[ModuleName.length()+4] ='_';
	chtemp2[ModuleName.length()+5] ='I';
	chtemp2[ModuleName.length()+6] ='S';
	chtemp2[ModuleName.length()+7] ='E';
	chtemp2[ModuleName.length()+8] ='_';
	chtemp2[ModuleName.length()+9] ='B';
	chtemp2[ModuleName.length()+10] ='a';
	chtemp2[ModuleName.length()+11] ='c';
	chtemp2[ModuleName.length()+12] ='k';
	chtemp2[ModuleName.length()+13] ='u';
	chtemp2[ModuleName.length()+14] = 'p';
	chtemp2[ModuleName.length()+15] = 0;
	remove(chtemp2);

	for(unsigned int i=0; i<ModuleName.length();i++)
		chtemp2[i] = ModuleName[i];
	chtemp2[ModuleName.length()] = '.';
	chtemp2[ModuleName.length()+1] ='r';
	chtemp2[ModuleName.length()+2] ='e';
	chtemp2[ModuleName.length()+3] ='s';
	chtemp2[ModuleName.length()+4] ='t';
	chtemp2[ModuleName.length()+5] ='o';
	chtemp2[ModuleName.length()+6] ='r';
	chtemp2[ModuleName.length()+7] ='e';
	chtemp2[ModuleName.length()+8] = 0;
	remove(chtemp2);

	for(unsigned int i=0; i<ModuleName.length();i++)
		chtemp2[i] = ModuleName[i];
	chtemp2[ModuleName.length()] = '.';
	chtemp2[ModuleName.length()+1] ='t';
	chtemp2[ModuleName.length()+2] ='i';
	chtemp2[ModuleName.length()+3] ='m';
	chtemp2[ModuleName.length()+4] = 0;
	remove(chtemp2);

	for(unsigned int i=0; i<ModuleName.length();i++)
		chtemp2[i] = ModuleName[i];
	chtemp2[ModuleName.length()] = '.';
	chtemp2[ModuleName.length()+1] ='j';
	chtemp2[ModuleName.length()+2] ='e';
	chtemp2[ModuleName.length()+3] ='d';
	chtemp2[ModuleName.length()+4] = 0;
	remove(chtemp2);

	for(unsigned int i=0; i<ModuleName.length();i++)
		chtemp2[i] = ModuleName[i];
	chtemp2[ModuleName.length()] = '.';
	chtemp2[ModuleName.length()+1] ='p';
	chtemp2[ModuleName.length()+2] ='r';
	chtemp2[ModuleName.length()+3] ='j';
	chtemp2[ModuleName.length()+4] = 0;
	remove(chtemp2);



	for(unsigned int i=0; i<ProjectName.length();i++)
		chtemp2[i] = ProjectName[i];
	chtemp2[ProjectName.length()] = '.';
	chtemp2[ProjectName.length()+1] ='r';
	chtemp2[ProjectName.length()+2] ='e';
	chtemp2[ProjectName.length()+3] ='s';
	chtemp2[ProjectName.length()+4] ='t';
	chtemp2[ProjectName.length()+5] ='o';
	chtemp2[ProjectName.length()+6] ='r';
	chtemp2[ProjectName.length()+7] ='e';
	chtemp2[ProjectName.length()+8] = 0;
	remove(chtemp2);

	for(unsigned int i=0; i<ProjectName.length();i++)
		chtemp2[i] = ProjectName[i];
	chtemp2[ProjectName.length()] = '.';
	chtemp2[ProjectName.length()+1] ='i';
	chtemp2[ProjectName.length()+2] ='s';
	chtemp2[ProjectName.length()+3] ='e';
	chtemp2[ProjectName.length()+4] ='_';
	chtemp2[ProjectName.length()+5] ='i';
	chtemp2[ProjectName.length()+6] ='s';
	chtemp2[ProjectName.length()+7] ='e';
	chtemp2[ProjectName.length()+8] ='_';
	chtemp2[ProjectName.length()+9] ='b';
	chtemp2[ProjectName.length()+10] ='a';
	chtemp2[ProjectName.length()+11] ='c';
	chtemp2[ProjectName.length()+12] ='k';
	chtemp2[ProjectName.length()+13] ='u';
	chtemp2[ProjectName.length()+14] ='p';
	chtemp2[ProjectName.length()+15] = 0;
	remove(chtemp2);

	for(unsigned int i=0; i<ProjectName.length();i++)
		chtemp2[i] = ProjectName[i];
	chtemp2[ProjectName.length()] = '.';
	chtemp2[ProjectName.length()+1] ='i';
	chtemp2[ProjectName.length()+2] ='s';
	chtemp2[ProjectName.length()+3] ='e';
	chtemp2[ProjectName.length()+4] = 0;
	remove(chtemp2);

	for(unsigned int i=0; i<ProjectName.length();i++)
		chtemp2[i] = ProjectName[i];
	chtemp2[ProjectName.length()] = '.';
	chtemp2[ProjectName.length()+1] ='x';
	chtemp2[ProjectName.length()+2] ='m';
	chtemp2[ProjectName.length()+3] ='l';
	chtemp2[ProjectName.length()+4] = 0;
	remove(chtemp2);

	remove("tmperr.err");

	for(unsigned int i=0; i<ProjectName.length();i++)
		chtemp2[i] = ModuleName[i];
	chtemp2[ModuleName.length()] = '.';
	chtemp2[ModuleName.length()+1] ='s';
	chtemp2[ModuleName.length()+2] ='t';
	chtemp2[ModuleName.length()+3] ='x';
	chtemp2[ModuleName.length()+4] = 0;

	remove(chtemp2);

	for(unsigned int i=0; i<ModuleName.length();i++)
		chtemp2[i] = ModuleName[i];
	chtemp2[ModuleName.length()] = '_';
	chtemp2[ModuleName.length()+1] ='b';
	chtemp2[ModuleName.length()+2] ='u';
	chtemp2[ModuleName.length()+3] ='i';
	chtemp2[ModuleName.length()+4] ='l';
	chtemp2[ModuleName.length()+5] ='d';
	chtemp2[ModuleName.length()+6] ='.';
	chtemp2[ModuleName.length()+7] ='x';
	chtemp2[ModuleName.length()+8] ='m';
	chtemp2[ModuleName.length()+9] ='l';
	chtemp2[ModuleName.length()+10] = 0;
	remove(chtemp2);

	for(unsigned int i=0; i<ModuleName.length();i++)
		chtemp2[i] = ModuleName[i];
	chtemp2[ModuleName.length()] = '.';
	chtemp2[ModuleName.length()+1] ='i';
	chtemp2[ModuleName.length()+2] ='s';
	chtemp2[ModuleName.length()+3] ='e';
	chtemp2[ModuleName.length()+4] = 0;
	remove(chtemp2);

	for(unsigned int i=0; i<ModuleName.length();i++)
		chtemp2[i] = ModuleName[i];
	chtemp2[ModuleName.length()] = '.';
	chtemp2[ModuleName.length()+1] ='x';
	chtemp2[ModuleName.length()+2] ='m';
	chtemp2[ModuleName.length()+3] ='l';
	chtemp2[ModuleName.length()+4] = 0;
	remove(chtemp2);

	for(unsigned int i=0; i<ModuleName.length();i++)
		chtemp2[i] = ModuleName[i];
	chtemp2[ModuleName.length()] = '.';
	chtemp2[ModuleName.length()+1] ='s';
	chtemp2[ModuleName.length()+2] ='t';
	chtemp2[ModuleName.length()+3] ='x';
	chtemp2[ModuleName.length()+4] = 0;
	remove(chtemp2);

	for(unsigned int i=0; i<ModuleName.length();i++)
		chtemp2[i] = ModuleName[i];
	chtemp2[ModuleName.length()] = '.';
	chtemp2[ModuleName.length()+1] ='m';
	chtemp2[ModuleName.length()+2] ='o';
	chtemp2[ModuleName.length()+3] ='d';
	chtemp2[ModuleName.length()+4] = 0;
	remove(chtemp2);


	remove("TclFile.tcl");

	string path1 = currentDIR;
	path1.insert(path1.length(), "_ngo");

	TCHAR* T_path1;
	for(unsigned int i=0; i<path1.length();i++)
		chtemp2[i] = path1[i];
	chtemp2[path1.length()] = 0;
	T_path1 = chtemp2;
	DeleteDirectory(T_path1);


	string path2 = currentDIR;
	path2.insert(path2.length(), "_xmsgs");
	for(unsigned int i=0; i<path2.length();i++)
		chtemp2[i] = path2[i];
	chtemp2[path2.length()] = 0;
	T_path1 = chtemp2;
	DeleteDirectory(T_path1);


	string path3 = currentDIR;
	path3.insert(path3.length(), ModuleName);
	path3.insert(path3.length(), "_html");
	for(unsigned int i=0; i<path3.length();i++)
		chtemp2[i] = path3[i];
	chtemp2[path3.length()] = 0;
	T_path1 = chtemp2;
	DeleteDirectory(T_path1);


	string path4 = currentDIR;
	path4.insert(path4.length(), "xst");
	for(unsigned int i=0; i<path4.length();i++)
		chtemp2[i] = path4[i];
	chtemp2[path4.length()] = 0;
	T_path1 = chtemp2;
	DeleteDirectory(T_path1);

	return 0;
}

void ModuleAnalysis::ISE_warning(){
	cout<<"\nERROR : There is an error during synthesis process. For more information check file "<<ModuleName<<".syr...\n";
	WAR<<"\nERROR : There is an error during synthesis process. For more information check file "<<ModuleName<<".syr...\n";
	return;
}

void ModuleAnalysis::error_type10(){
	cout<<"ERROR : ";
	cout<<"Make sure the project name is unique or the path of ISE is correct.\n";
	cout<<"\nPress any key to end...\n";
	getch();
	ERR<<"ERROR : ";
	ERR<<"Make sure the project name is unique or the path of ISE is correct.\n";
	exit(0);
	return;
}


//the erase space function
string ModuleAnalysis::erase_space(string s)
{



	while(s.find( " ", 0 ) != string::npos){
		s.erase(s.find(" ", 0), 1);
		}
	while(s.find( "\t", 0 ) != string::npos){
		s.erase(s.find("\t", 0), 1);
		}
	return s;
}

//error type1
void ModuleAnalysis::error_type1(int line){
	cout<<"ERROR : There is a template in your Verilog code which is not supported by our software parser\n";
	cout<<"The detected error is in line "<<line<<".\n";
	cout<<"Make sure that the input or output lines are near the left hand side of the code in the specified line.\n";
	cout<<"\nPress any key to end...\n";
	ERR<<"ERROR : There is a template in your Verilog code which is not supported by our software parser\n";
	ERR<<"The detected error is in line "<<line<<".\n";
	ERR<<"Make sure that the input or output lines are near the left hand side of the code in the specified line.\n";

	cout<<"\nPress any key to end...\n";
	getch();
	exit(0);

	return;
}

//error type2
void ModuleAnalysis::error_type2(int line){
	cout<<"ERROR : There is a template in your Verilog code which is not supported by our software parser\n";
	cout<<"The detected error is in line "<<line<<".\n";
	ERR<<"There is a template in your verilog code which is not supported by our oftware parser\n";
	ERR<<"The detected error is in line "<<line<<".\n";
	return;
}

//error type2
void ModuleAnalysis::error_type3(int line){
	cout<<"There is a template in your Verilog code which is not supported by our software parser\n";
	cout<<"The detected error is in line "<<line<<".\n";
	ERR<<"There is a template in your Verilog code which is not supported by our oftware parser\n";
	ERR<<"The detected eror is in line "<<line<<".\n";
	return;
}



//converting string to integer
int ModuleAnalysis::char2int(char c){
	switch (c){
	case '0': return 0;
	case '1': return 1;
	case '2': return 2;
	case '3': return 3;
	case '4': return 4;
	case '5': return 5;
	case '6': return 6;
	case '7': return 7;
	case '8': return 8;
	case '9': return 9;
	default: return -1;
	}

	return -1;
}

//converting string to integer
char ModuleAnalysis::int2char(int digit){
	switch (digit){
	case 0: return '0';
	case 1: return '1';
	case 2: return '2';
	case 3: return '3';
	case 4: return '4';
	case 5: return '5';
	case 6: return '6';
	case 7: return '7';
	case 8: return '8';
	case 9: return '9';
	default: return ' ';
	}

	return ' ';
}

//Converting integer to string
string ModuleAnalysis::int2str(int digit){

	string temp = "";
	string temp2= "";
	if(digit == 0) return "0";
	while(digit>0){
		int number = digit%10;
		digit = digit/10;
		temp = int2char(number);
		temp2.insert(0, temp);
	}

	return temp2;
}



//Converting string to integer
long int ModuleAnalysis::str2int(string str){

	long int digit=0;
	for(double i=0; i<str.length();i++)
		digit += pow(10, i)*char2int(str[str.length()-i-1]);

	return digit;
}




//reading Input Output of the design
int ModuleAnalysis::readInOut(){



	node* nodeFileTemp = addedFiles;
	char InputFile2[256];

	while(nodeFileTemp){
		int commentTemp = deletComment(addedFiles->s);

		if(commentTemp == -1) return -1;
		if(!commentTemp){


			long int lineCnt=0;
			
			InputFile2[0] = 'C';
			InputFile2[1] = 'o';
			InputFile2[2] = 'm';
			InputFile2[3] = 'm';
			InputFile2[4] = 'e';
			InputFile2[5] = 'n';
			InputFile2[6] = 't';
			InputFile2[7] = 'F';
			InputFile2[8] = 'r';
			InputFile2[9] = 'e';
			InputFile2[10] = 'e';
			InputFile2[11] = '_';
			for (int i=12; i<nodeFileTemp->s.length()+12; i++) InputFile2[i] = nodeFileTemp->s[i-12];
			InputFile2[nodeFileTemp->s.length()+12] = 0;

			ifstream fi_file(InputFile2);

			char str[1000];
			int flag = 0;
			while (!fi_file.eof())
			{

				fi_file.getline(str,10000);
				lineCnt++;
				string temp = str;

				if(keywordFinder(temp, "module") != -1){

					temp.erase(0, keywordFinder(temp, "module")+6);
					temp = erase_space(temp);
					string str_temp2 = temp.substr(0, temp.find("("));
					if(str_temp2 == ModuleName) flag = 1; else flag = 0;

					if (flag) {
						str_temp2 = temp;
						str_temp2.erase(0, temp.find("(")+1);
						if (str_temp2.find(")") == 0) {remove(InputFile2); return 0;}
						while(str_temp2.find(")") != 0){
							str_temp2 = erase_space(str_temp2);
							if(str_temp2.find(",") != -1){
								if( str_temp2.find(",") == 0 ) {str_temp2 = str_temp2.substr(1, str_temp2.length());}
								else{
									if (!InOutOrder) {
										InOutOrder = new node;
										InOutOrder->s = str_temp2.substr(0, str_temp2.find(","));
										InOutOrder->next=NULL;
										str_temp2.erase(0, str_temp2.find(",")+1);
									}
									else {
										node* tempPointer = InOutOrder;
										while(tempPointer->next){ tempPointer = tempPointer->next;}
										tempPointer->next = new node;
										tempPointer->next->s = str_temp2.substr(0, str_temp2.find(","));
										str_temp2.erase(0, str_temp2.find(",")+1);
										tempPointer->next->next = NULL;
									}
								}
							} else{
								if(str_temp2.find(")") != -1){
									if (!InOutOrder) {
										InOutOrder = new node;
										InOutOrder->s = str_temp2.substr(0, str_temp2.find(")"));
										InOutOrder->next=NULL;
										str_temp2.erase(0, str_temp2.find(")"));
									}
									else {
										node* tempPointer = InOutOrder;
										node*temp2Pointer = InOutOrder->next;
										while(tempPointer){ temp2Pointer = tempPointer; tempPointer = tempPointer->next;}
										temp2Pointer->next = new node;
										temp2Pointer->next->s = str_temp2.substr(0, str_temp2.find(")"));
										str_temp2.erase(0, str_temp2.find(")"));
										temp2Pointer->next->next = NULL;
									}
								}else{
									if (str_temp2.length()){
										if (!InOutOrder) {
											InOutOrder = new node;
											InOutOrder->s = str_temp2;
											InOutOrder->next=NULL;
											str_temp2.erase(0, str_temp2.length());
										}
										else {
											node* tempPointer = InOutOrder;
											while(tempPointer->next){ tempPointer = tempPointer->next;}
											tempPointer->next = new node;
											tempPointer->next->s = str_temp2;
											str_temp2.erase(0, str_temp2.length());
											tempPointer->next->next = NULL;
										}
									}
								}
							}


							if(!str_temp2.length()){
								fi_file.getline(str,10000);
								lineCnt++;
								str_temp2 = str;
								str_temp2 = erase_space(str_temp2);
							}
						}
					}
				}


				if((keywordFinder(temp, "input") != -1) && flag){
					if(temp.find(" reg ") != -1 || temp.find("\treg ") != -1 || temp.find(" reg\t") != -1 || temp.find("\treg\t") != -1) {error_type3(lineCnt); remove(InputFile2); return -1;}
					if (!InputsStr) {InputsStr = new node; InputsStr->s = temp.substr(0, temp.find(";")); InputsStr->next=NULL;}
					else {
						node* tempPointer = InputsStr;
						node*temp2Pointer = InputsStr->next;
						while(tempPointer){ temp2Pointer = tempPointer; tempPointer = tempPointer->next;}
						temp2Pointer->next = new node;
						temp2Pointer->next->s = temp.substr(0, temp.find(";"));
						temp2Pointer->next->next = NULL;
					}


					temp = temp.substr(temp.find("input"), temp.length());

					if(temp.find("input ")!=0 && temp.find("input\t")!=0 && temp.find("input[")!=0){

						error_type1(lineCnt);
						remove(InputFile2);
						exit(0);
						return -1;
					}
					else{
						temp = erase_space(temp);
						temp.erase(0, 5);
						if (temp.find(",") != -1) { error_type2(lineCnt); remove(InputFile2); exit(0); return -1;}
						if(temp.find("[") == -1){
							if (!EX_Inputs_1) {EX_Inputs_1 = new node; EX_Inputs_1->s = temp.substr(0, temp.find(";")); EX_Inputs_1->next=NULL;}
							else {
								node* tempPointer = EX_Inputs_1;
								node*temp2Pointer = EX_Inputs_1->next;
								while(tempPointer){ temp2Pointer = tempPointer; tempPointer = tempPointer->next;}
								temp2Pointer->next = new node;
								temp2Pointer->next->s = temp.substr(0, temp.find(";"));
								temp2Pointer->next->next = NULL;
							}

							if (!EX_Inputs2) {EX_Inputs2 = new node; EX_Inputs2->s = temp.substr(0, temp.find(";")); EX_Inputs2->next=NULL;}
							else {
								node* tempPointer = EX_Inputs2;
								node*temp2Pointer = EX_Inputs2->next;
								while(tempPointer){ temp2Pointer = tempPointer; tempPointer = tempPointer->next;}
								temp2Pointer->next = new node;
								temp2Pointer->next->s = temp.substr(0, temp.find(";"));
								temp2Pointer->next->next = NULL;
							}

							if (!Inputs) {Inputs = new node; Inputs->s = temp.substr(0, temp.find(";")); Inputs->next=NULL;}
							else {
								node* tempPointer = Inputs;
								node*temp2Pointer = Inputs->next;
								while(tempPointer){ temp2Pointer = tempPointer; tempPointer = tempPointer->next;}
								temp2Pointer->next = new node;
								temp2Pointer->next->s = temp.substr(0, temp.find(";"));
								temp2Pointer->next->next = NULL;
							}
						}
						else{
							temp.erase(0, 1);



							string left = temp.substr(0, temp.find(":"));
							temp.erase(0, temp.find(":")+1);
							string right = temp.substr(0, temp.find("]"));
							temp.erase(0, temp.find("]")+1);
							if(temp.find(";") == -1) {
								error_type1(lineCnt);
								remove(InputFile2);
								exit(0);
								return -1;
							}
							else temp = temp.substr(0, temp.find(";"));
							string s_temp = temp;

							if (!Inputs) {Inputs = new node; Inputs->s = temp; Inputs->next=NULL;}
							else {
								node* tempPointer = Inputs;
								node*temp2Pointer = Inputs->next;
								while(tempPointer){ temp2Pointer = tempPointer; tempPointer = tempPointer->next;}
								temp2Pointer->next = new node;
								temp2Pointer->next->s = temp.substr(0, temp.find(";"));
								temp2Pointer->next->next = NULL;
							}

							if(str2int(left) != -1 && str2int(right) != -1 ){
								if(str2int(left)>str2int(right)){
									for(int i=0 ; i<str2int(left)-str2int(right)+1;i++){
										temp = s_temp;
										temp.insert(temp.length(), "_");
										temp.insert(temp.length(), int2str(i));
										if (!EX_Inputs_1) {EX_Inputs_1 = new node; EX_Inputs_1->s = temp; EX_Inputs_1->next=NULL;}
										else {
											node* tempPointer = EX_Inputs_1;
											node*temp2Pointer = EX_Inputs_1->next;
											while(tempPointer){ temp2Pointer = tempPointer; tempPointer = tempPointer->next;}
											temp2Pointer->next = new node;
												temp2Pointer->next->s = temp.substr(0, temp.find(";"));
											temp2Pointer->next->next = NULL;
										}

										

									}

									for(int i=str2int(right) ; i<str2int(left)+1;i++){
										
										temp = s_temp;
										temp.insert(temp.length(), "[");
										temp.insert(temp.length(), int2str(i));
										temp.insert(temp.length(), "]");
										if (!EX_Inputs2) {EX_Inputs2 = new node; EX_Inputs2->s = temp; EX_Inputs2->next=NULL;}
										else {
											node* tempPointer = EX_Inputs2;
											node*temp2Pointer = EX_Inputs2->next;
											while(tempPointer){ temp2Pointer = tempPointer; tempPointer = tempPointer->next;}
											temp2Pointer->next = new node;
											temp2Pointer->next->s = temp.substr(0, temp.find(";"));
											temp2Pointer->next->next = NULL;
										}

									}
								}else{
									for(int i= 0; i<str2int(right)-str2int(left)+1;i++){
										temp = s_temp;
										temp.insert(temp.length(), "_");
										temp.insert(temp.length(), int2str(i));
										if (!EX_Inputs_1) {EX_Inputs_1 = new node; EX_Inputs_1->s = temp; EX_Inputs_1->next=NULL;}
										else {
											node* tempPointer = EX_Inputs_1;
											node*temp2Pointer = EX_Inputs_1->next;
											while(tempPointer){ temp2Pointer = tempPointer; tempPointer = tempPointer->next;}
											temp2Pointer->next = new node;
											temp2Pointer->next->s = temp.substr(0, temp.find(";"));
											temp2Pointer->next->next = NULL;
										}
									}

									for(int i= str2int(left); i<str2int(right)+1;i++){
										temp = s_temp;
										temp.insert(temp.length(), "[");
										temp.insert(temp.length(), int2str(i));
										temp.insert(temp.length(), "]");
										if (!EX_Inputs2) {EX_Inputs2 = new node; EX_Inputs2->s = temp; EX_Inputs2->next=NULL;}
										else {
											node* tempPointer = EX_Inputs2;
											node*temp2Pointer = EX_Inputs2->next;
											while(tempPointer){ temp2Pointer = tempPointer; tempPointer = tempPointer->next;}
											temp2Pointer->next = new node;
											temp2Pointer->next->s = temp.substr(0, temp.find(";"));
											temp2Pointer->next->next = NULL;
										}

									}
								}



							}
							else{
								error_type1(lineCnt);
								exit(0);
							}
						}
					}
				}


				if((keywordFinder(temp, "output") !=-1 ) && flag){

					if (!OutputsStr) {OutputsStr = new node; OutputsStr->s = temp.substr(0, temp.find(";")); OutputsStr->next=NULL;}
					else {
						node* tempPointer = OutputsStr;
						node*temp2Pointer = OutputsStr->next;
						while(tempPointer){ temp2Pointer = tempPointer; tempPointer = tempPointer->next;}
						temp2Pointer->next = new node;
						temp2Pointer->next->s = temp.substr(0, temp.find(";"));
						temp2Pointer->next->next = NULL;
					}

					temp = temp.substr(temp.find("output"), temp.length());

					if(temp.find("output ")!=0 && temp.find("output\t")!=0 && temp.find("output[")!=0){

						error_type1(lineCnt);
						remove(InputFile2);
						exit(0);
						return -1;
					}
					else{
						temp = erase_space(temp);
						temp.erase(0, 6);
						if (temp.find(",") != -1) { error_type2(lineCnt); remove(InputFile2); exit(0); return -1;}
						if(temp.find("[") == -1){
							if (!EX_Outputs_1) {EX_Outputs_1 = new node; EX_Outputs_1->s = temp.substr(0, temp.find(";")); EX_Outputs_1->next=NULL;}
							else {
								node* tempPointer = EX_Outputs_1;
								node*temp2Pointer = EX_Outputs_1->next;
								while(tempPointer){ temp2Pointer = tempPointer; tempPointer = tempPointer->next;}
								temp2Pointer->next = new node;
								temp2Pointer->next->s = temp.substr(0, temp.find(";"));
								temp2Pointer->next->next = NULL;
							}

							if (!EX_Outputs2) {EX_Outputs2 = new node; EX_Outputs2->s = temp.substr(0, temp.find(";")); EX_Outputs2->next=NULL;}
							else {
								node* tempPointer = EX_Outputs2;
								node*temp2Pointer = EX_Outputs2->next;
								while(tempPointer){ temp2Pointer = tempPointer; tempPointer = tempPointer->next;}
								temp2Pointer->next = new node;
								temp2Pointer->next->s = temp.substr(0, temp.find(";"));
								temp2Pointer->next->next = NULL;
							}

							if (!Outputs) {Outputs = new node; Outputs->s = temp.substr(0, temp.find(";")); Outputs->next=NULL;}
							else {
								node* tempPointer = Outputs;
								node*temp2Pointer = Outputs->next;
								while(tempPointer){ temp2Pointer = tempPointer; tempPointer = tempPointer->next;}
								temp2Pointer->next = new node;
								temp2Pointer->next->s = temp.substr(0, temp.find(";"));
								temp2Pointer->next->next = NULL;
							}
						}
						else{
							temp.erase(0, 1);



							string left = temp.substr(0, temp.find(":"));
							temp.erase(0, temp.find(":")+1);
							string right = temp.substr(0, temp.find("]"));
							temp.erase(0, temp.find("]")+1);
							if(temp.find(";") == -1) {
								error_type1(lineCnt);
								remove(InputFile2);
								exit(0);
								return -1;
							}
							else temp = temp.substr(0, temp.find(";"));
							string s_temp = temp;

							if (!Outputs) {Outputs = new node; Outputs->s = temp; Outputs->next=NULL;}
							else {
								node* tempPointer = Outputs;
								node*temp2Pointer = Outputs->next;
								while(tempPointer){ temp2Pointer = tempPointer; tempPointer = tempPointer->next;}
								temp2Pointer->next = new node;
								temp2Pointer->next->s = temp.substr(0, temp.find(";"));
								temp2Pointer->next->next = NULL;
							}

							if(str2int(left) != -1 && str2int(right) != -1 ){
								if(str2int(left)>str2int(right)){
									for(int i= 0; i<str2int(left)-str2int(right)+1;i++){
										temp = s_temp;
										temp.insert(temp.length(), "_");
										temp.insert(temp.length(), int2str(i));
										if (!EX_Outputs_1) {EX_Outputs_1 = new node; EX_Outputs_1->s = temp; EX_Outputs_1->next=NULL;}
										else {
											node* tempPointer = EX_Outputs_1;
											node*temp2Pointer = EX_Outputs_1->next;
											while(tempPointer){ temp2Pointer = tempPointer; tempPointer = tempPointer->next;}
											temp2Pointer->next = new node;
											temp2Pointer->next->s = temp.substr(0, temp.find(";"));
											temp2Pointer->next->next = NULL;
										}

										
									}

									for(int i= str2int(right); i<str2int(left)+1;i++){
										
										temp = s_temp;
										temp.insert(temp.length(), "[");
										temp.insert(temp.length(), int2str(i));
										temp.insert(temp.length(), "]");
										if (!EX_Outputs2) {EX_Outputs2 = new node; EX_Outputs2->s = temp; EX_Outputs2->next=NULL;}
										else {
											node* tempPointer = EX_Outputs2;
											node*temp2Pointer = EX_Outputs2->next;
											while(tempPointer){ temp2Pointer = tempPointer; tempPointer = tempPointer->next;}
											temp2Pointer->next = new node;
											temp2Pointer->next->s = temp.substr(0, temp.find(";"));
											temp2Pointer->next->next = NULL;
										}


									}
								}else{
									for(int i= 0; i<str2int(right)-str2int(left)+1;i++){
										temp = s_temp;
										temp.insert(temp.length(), "_");
										temp.insert(temp.length(), int2str(i));
										if (!EX_Outputs_1) {EX_Outputs_1 = new node; EX_Outputs_1->s = temp; EX_Outputs_1->next=NULL;}
										else {
											node* tempPointer = EX_Outputs_1;
											node*temp2Pointer = EX_Outputs_1->next;
											while(tempPointer){ temp2Pointer = tempPointer; tempPointer = tempPointer->next;}
											temp2Pointer->next = new node;
											temp2Pointer->next->s = temp.substr(0, temp.find(";"));
											temp2Pointer->next->next = NULL;
										}

										


									}

									for(int i= str2int(left); i<str2int(right)+1;i++){
										

										temp = s_temp;
										temp.insert(temp.length(), "[");
										temp.insert(temp.length(), int2str(i));
										temp.insert(temp.length(), "]");
										if (!EX_Outputs2) {EX_Outputs2 = new node; EX_Outputs2->s = temp; EX_Outputs2->next=NULL;}
										else {
											node* tempPointer = EX_Outputs2;
											node*temp2Pointer = EX_Outputs2->next;
											while(tempPointer){ temp2Pointer = tempPointer; tempPointer = tempPointer->next;}
											temp2Pointer->next = new node;
											temp2Pointer->next->s = temp.substr(0, temp.find(";"));
											temp2Pointer->next->next = NULL;
										}


									}
								}



							}
							else{
								error_type1(lineCnt);
								exit(0);
							}
						}
					}
				}
				if(temp.find("endmodule")!=-1 && flag) {remove(InputFile2); return 0;}
			}
			if(!flag){
				cout<<"\nERROR : Make sure the name of the top module is correct.";
				cout<<"\n-The name of the top module is case sensitive.";
				cout<<"\n-The name of the top module should be the same as the name of its file.";
				cout<<"\n-Inputs and outputs should be defined separately.";
				cout<<"\n-Please check software manual for more information.";
				cout<<"\nPress any key to end...\n";
				getch();
				exit(0);

				return -1;
			}
		}
		nodeFileTemp = nodeFileTemp->next;
	}

	
	remove(InputFile2);
	return 0;

}

int ModuleAnalysis::readFile(){
	cout<<"\n**************************************************\n";
	cout<<"This tool converts your Verilog input to a netlist. It uses a free version of ISE synthesis tool.\n";
	cout<<"Inputs and Outputs should be defined separately without comma.(as the old version of Verilog)\n";
	cout<<"\nFor synthesis, a correct installation of ISE synthesis tool should exist on your system.";
	cout<<"\nYou can add more than one file into your project, but the name of the top module should be the same as the name of the first file.";
	cout<<"\n**************************************************\n";
	if (!ConstructFlag){
		cout<<"\nChoose a name for your project : ";
		cin>>ProjectName;
	}
	char chartemp1[512];
	string strtemp6 = ProjectName;
	strtemp6.insert(strtemp6.length(), ".ise");
	for(int i = 0; i< strtemp6.length(); i++)
		chartemp1[i] = strtemp6[i];
	chartemp1[strtemp6.length()] = 0;
	remove(chartemp1);

	if (!ConstructFlag){
		cout<<"\n\nPlease enter the name of the module : ";
		cin>>ModuleName;
	}
	if (!ConstructFlag){
		addedFiles = NULL;
		addedFiles = new node;
		addedFiles->s = ModuleName;
		addedFiles->next = NULL;
		string temp;
		cout<<"\n\nIf you want to add other files to your project, type the name of the file (if not type 'N' or 'n'): ";
		cin>>temp;
		node* TempNode = addedFiles;
		while(temp != "N" && temp != "n"){
			TempNode->next = new node;
			TempNode->next->s = temp;
			TempNode->next->next = NULL;
			TempNode = TempNode->next;
			cout<<"\n\nIf you want to add other files to your project, type the name of the file (if not type 'N' or 'n'): ";
			cin>>temp;
		}
	}

	rptFile = ModuleName;
	equationFile = ModuleName;
	Eq_after_WireAnalyzer_File = ModuleName;
	Eq_after_WireAnalyzer_File.insert(0, "Equation_");
	Eq_after_WireAnalyzer_File.insert(Eq_after_WireAnalyzer_File.length(), ".v");

	equationFile.insert(0, "eq_");
	equationFile.insert(equationFile.length(), ".txt");
	rptFile.insert(rptFile.length(), ".rpt");


	return 0;

}








///////////////////////////////////////////////////////////////////////////////////////////////////


int ModuleAnalysis::deletComment(string s){

	cout<<"\n";
	char InputFile2[256];
	for (int i=0; i<s.length(); i++) InputFile2[i] = s[i];
	InputFile2[s.length()] = '.';
	InputFile2[s.length()+1] = 'v';
	InputFile2[s.length()+2] = 0;

	ifstream fi_file(InputFile2);

	if(!fi_file){
		cout<<"\nWe do not find any file matching the name -"<<s<<"- in the root directory.\nPress any key to end...";
		getch();
		exit(0);
		return -1;
	}

	s.insert(0, "CommentFree_");
	for (int i=0; i<s.length(); i++) InputFile2[i] = s[i];
	InputFile2[s.length()]  = 0;

	remove(InputFile2);
	ofstream fo_file(InputFile2);
	string st;
	char str[10000];
	int flag = 0;
	while (!fi_file.eof())
	{
		fi_file.getline(str,10000);
		st=str;
		while(st.find( "//", 0 ) != string::npos || st.find( "/*", 0 ) != string::npos || st.find( "/*", 0 ) != string::npos){
			if(st.find ( "//" , 0) != string::npos ){ st = st.substr(0 , st.find ( "//" , 0));}
			if(st.find ("/*" , 0) != string::npos ){
				flag = 1;
				if( st.find ("*/" , 0) != string::npos ) {
					string temp1 = st.substr(0 , st.find ( "/*" , 0));
					st.erase(0 , st.find ( "*/" , 0)+2);
					st.insert(0, temp1);
				}
				else {
					string temp1 = st.substr(0 , st.find ( "/*" , 0));
					fo_file<<temp1<<endl;
					while (st.find ("*/" , 0) == string::npos){
						fi_file.getline(str,10000);
						st=str;
					}

					st.erase(0, st.find ( "*/", 0)+2);
				}
			}
		}
		fo_file<<st<<endl;
	}

	fo_file.eof();

	fo_file.close();
	fi_file.close();

	return 0;
}



//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------EDIF MANAGER CLASS----------------------------
//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------











//----------------------------------NET Data Structures-------------------------------
struct netNode{
	string name;
	string preName;
	netNode* next;
};

class netWire{
public:
	netWire();
	string getNet(string s);
	void incNet() {netNumber++;}
	void decNet() {netNumber--;}
	void setPreversion(string s) { preVer = s;}
	netNode* cehckNet(string s);
	string checkRootNet(string s);
	void replaceCurNet(string s);
	string giveCurNet(){if(crNet) return crNet->name; else return "";}
	string addNet(string netT, string total_t, string preNet);
	string addNet(string netT, string total_t);
	
private:

	int char2int(char c);
	char int2char(int digit);
	string int2str(int digit);
	long int str2int(string str);
	

	netNode *netHead;
	netNode* crNet;
	string preVer;
	int netNumber;
	string netName;
};

void netWire::replaceCurNet(string s){
	if(crNet){
		crNet->name = s;
	}

	return;
}

netNode* netWire::cehckNet(string s){
	netNode* net_temp = netHead;
	while(net_temp){
		if(net_temp->preName == s) return net_temp;
		net_temp = net_temp->next;
	}
	return NULL;
}

string netWire::checkRootNet(string s){
	netNode* net_temp = netHead;
	while(net_temp){
		if(net_temp->preName.find(s) == 0) return net_temp->name;
		net_temp = net_temp->next;
	}
	return "";
}

string netWire::getNet(string s){
	string str_temp = netName;
	str_temp.insert(str_temp.length(), int2str(netNumber));

	/*if (str_temp == "intermediateXXXwire_2149"){
		int k;
		k++;
	}*/
	if(!netHead){
		netHead = new netNode;
		netHead->preName = s;
		netHead->name = str_temp;
		netHead->next = NULL;
		crNet = netHead;
	}else{
		crNet->next = new netNode;
		crNet->next->preName = s;
		crNet->next->name = str_temp;
		crNet->next->next = NULL;
		crNet = crNet->next;
	}
	return str_temp;
}
netWire::netWire(){

	netNumber = 0;
	netName = "intermediateXXXwire_";
	netHead = NULL;
	crNet = NULL;
}


//converting string to integer
int netWire::char2int(char c){
	switch (c){
	case '0': return 0;
	case '1': return 1;
	case '2': return 2;
	case '3': return 3;
	case '4': return 4;
	case '5': return 5;
	case '6': return 6;
	case '7': return 7;
	case '8': return 8;
	case '9': return 9;
	default: return -1;
	}

	return -1;
}

//converting string to integer
char netWire::int2char(int digit){
	switch (digit){
	case 0: return '0';
	case 1: return '1';
	case 2: return '2';
	case 3: return '3';
	case 4: return '4';
	case 5: return '5';
	case 6: return '6';
	case 7: return '7';
	case 8: return '8';
	case 9: return '9';
	default: return ' ';
	}

	return ' ';
}

//Converting integer to string
string netWire::int2str(int digit){

	string temp = "";
	string temp2= "";
	if(digit == 0) return "0";
	while(digit>0){
		int number = digit%10;
		digit = digit/10;
		temp = int2char(number);
		temp2.insert(0, temp);
	}

	return temp2;
}



//Converting string to integer
long int netWire::str2int(string str){

	long int digit=0;
	for(double i=0; i<str.length();i++)
		digit += pow(10, i)*char2int(str[str.length()-i-1]);

	return digit;
}

string netWire::addNet(string netT, string total_t, string preName){
	
	netNode* netTemp = netHead;
	string preNet = "";

	while(netTemp){
		if(netTemp->name == preName) netTemp->name = netT;
		netTemp = netTemp->next;
	}

	netTemp = netHead;
	if(!netTemp){
		netTemp = new netNode;
		netTemp->preName = total_t;
		netTemp->name = netT;
		netTemp->next = NULL;
		netHead = netTemp;
		crNet = netTemp;
	}else{
		int flag = 0;
		while(netTemp->next){
			if(netTemp->preName == total_t){
				preNet = netTemp->name;
				netTemp->name = netT;
				flag = 1;
			}
			netTemp = netTemp->next;
		}
		if(!flag){
			netTemp->next = new netNode;
			netTemp->next->name = netT;
			netTemp->next->preName = total_t;
			netTemp->next->next = NULL;
			crNet = netTemp->next;
		}
	}

	return preNet;
}

string netWire::addNet(string netT, string total_t){
	
	netNode* netTemp = netHead;
	string preNet = "";

	netTemp = netHead;
	if(!netTemp){
		netTemp = new netNode;
		netTemp->preName = total_t;
		netTemp->name = netT;
		netTemp->next = NULL;
		netHead = netTemp;
		crNet = netTemp;
	}else{
		int flag = 0;
		while(netTemp->next){
			if(netTemp->preName == total_t){
				preNet = netTemp->name;
				netTemp->name = netT;
				flag = 1;
			}
			netTemp = netTemp->next;
		}
		if(!flag){
			netTemp->next = new netNode;
			netTemp->next->name = netT;
			netTemp->next->preName = total_t;
			netTemp->next->next = NULL;
			crNet = netTemp->next;
		}
	}

	return preNet;
}


		




//------------------------------------------------------------------------------------



//---------------------------------------CELL----------------------

class portClass{
public:
	int array;
	string portName;
	string direction;
	portClass* next;
};
//-------------------------------

//-----------Cell/Library satruct Find Library---
class cellLib{
public:
	cellLib() {cellPointer = NULL; libPointer = NULL; next = NULL; cell = ""; Lib = "";}
	string cell;
	string Lib;
	node* cellPointer;
	node* libPointer;
	cellLib* next;
};


//--------------------------------------------------CLASS EDIF MANAGER------------------------------------------------
class EDIF_manager : public ModuleAnalysis{
public:

	void designHn (string total = "Design_");
	void readNDFfile();
	EDIF_manager();
	void print1();
protected:
	void assignHandler(string commandline, string total, long int eqNumber);
	node* cellFind(string Lib, string cell);

	void setIO(string);
	void netReplace(string, string);

	int minNum(int x1 = -1, int x2 = -1,int x3 = -1, int x4 = -1, int x5 = -1, int x6 = -1, int x7 = -1, int x8 = -1, int x9 = -1);

	int char2int(char c);
	char int2char(int digit);
	string int2str(int digit);
	long int str2int(string str);
	int getType(string cell);//0 buffer, 1 AND, 2 OR, 3 XOR, 4 FDCPE, 5, FTCPE, 6 FDDCPE, 7 FTDCPE, 8 LDCP, 9 Fanout//---------GATE TYPE-------------
	bool getInv(string cell);//0 buffer, 1 AND, 2 OR, 3 XOR, 4 FDCPE, 5, FTCPE, 6 FDDCPE, 7 FTDCPE, 8 LDCP, 9 Fanout//---------GATE TYPE-------------
	void cellError(string);

	node* cellRefHn(string total, string Lib, string cell, int top);

	
	node* cellRefHn1(string total, string Lib, string cell_s, node* nodeTempL, int top);
	node* cellRefHn2(string total, string Lib, string cell, node* nodeTempCell, int top);
	void insRefHn1( string total/*ID*/, node* nodeTempcell/*start of cell*/
			   , string portName/*port name*/,  string insRef);
	void instanceHn (string total = "Design_", node* insTemp = NULL, int top = 0);
	string netHn (string total, string cell, string Lib, node* insTemp, int top, string strNetTemp);
	string NETcellRefHn2(string total, string Lib, string cell, string portName, int top, string strNetTemp);
	string NETcellRefHn2(string total, string Lib, string cell, int top, string strNetTemp, string ioName, string ioArg);
	
	string NETcellRefHn1(string total, string Lib, string cell, string strIns, string portName, int top, string strNetTemp);
	string NETcellRefHn1(string total, string Lib, string cell, string strIns, int top, string strNetTemp, string ioName, string ioArg);


	node* sFind2 (string s, node* start = NULL);
	node* sFind (string s, node* start = NULL);
	node* libraryFind(string s);

	GateStruct* gateFind(string path);
	void deleteGatehead_t(GateStruct*);
	void deleteNode_t(node* nodeTemp);
	string creat_NOT(string st_temp2, long int eqNumber);
	int checkingWire(string s, long int eqNumber);
	void error_type5(long int eqLineCnt);
	string creat_BUFFER(string st_temp3, string st_temp2, long int eqNumber);
	int checkingGate(long int eqNumber);
	void warnning_type2(long int eqNumber);
	void warnning_type1(long int eqNumber);
	void error_type6(long int eqLineCnt);
	void deleteGate();
	int DFFHandler(string INS, string Q, string D, string C, string CLR, string PRE, string CE, string total);
	int wireFinder(string commandline, long int eqNumber);
	//void error_type5(long int eqLineCnt);
	string erase_space(string s);
	int wireFinder4reg(string commandline, long int eqNumber);
	int CommandReader(string total);
	string logicExtractor(string equation);
	void error_type8_FDCPE(long int gateCounter);
	string loadCellLibrary(string total, string total_t, string strNetTemp, string portName, string cell);
	void error_type4(long int rptLineCnt);
	void gateConverter();
	void wireAnalyzer_Gate();
	int checkingWire(string s, node* head);
	void replaceGatewire(string newWire, string oldWire, GateStruct* Temp);
	string erase_first_space(string s);
	



	//------------------------------------------------






	


	//--------------------------------------------------

	GateStruct* Gatehead;
	GateStruct* currentPointer;

	GateStruct* Gatehead_t;
	GateStruct* currentPointer_t;

	node* totalFile;

	netWire currentNet;

	string ioString;
	int ioStringFlag;

	cellLib* CLAddress; 
	cellLib* currentCLAddress;

	node* nodeHead_t;
	node* currentNode_t;

	node* EX_Inputs_1_t;
	node* EX_Outputs_1_t;
	node* currentInNode;
	node* currentOutNode;

	long int wireGateCnt_t;
	node* wiresHeadGate_t;
	node* currentGateWire_t;


};

EDIF_manager::EDIF_manager(){

	Gatehead = NULL;
	currentPointer = NULL;

	Gatehead_t = NULL;
	currentPointer_t = NULL;

	nodeHead_t = NULL;
	currentNode_t = NULL;


	totalFile = NULL;
	ioString = "";
	ioStringFlag = 0;
	CLAddress = NULL;
	currentCLAddress = NULL;

	EX_Inputs_1_t = NULL;
	EX_Outputs_1_t = NULL;
	currentInNode = 0;
	currentOutNode = 0;

	wireGateCnt_t = 0;
	wiresHeadGate_t = NULL;
	currentGateWire_t = NULL;

}

void EDIF_manager::error_type5(long int eqLineCnt){
	cout<<"\nThere is an error in your equation file.\n";
	cout<<"We do not detect any equation in line "<<eqLineCnt<<".\n";
	ERR<<"\nThere is an error in your equation file.\n";
	ERR<<"We do not detect any equation in line "<<eqLineCnt<<".\n .\n";
	return;
}

//the erase space function
string EDIF_manager::erase_space(string s)
{



	while(s.find( " ", 0 ) != string::npos){
		s.erase(s.find(" ", 0), 1);
		}
	while(s.find( "\t", 0 ) != string::npos){
		s.erase(s.find("\t", 0), 1);
		}
	return s;
}
//returns minimum number
int EDIF_manager::minNum(int x1, int x2,int  x3, int x4, int x5, int x6, int x7, int x8, int x9){

	int temp = -1;
	if( x1 != -1 ){
		temp = x1;
	}else
	{
		if (x2 !=-1){
			temp = x2;
		}else{
			if (x3 != -1){
				temp = x3;
			}else{
				if (x4 != -1){
					temp = x4;
				}else{
					if (x5 != -1){
						temp = x5;
					}else{
						if (x6 != -1){
							temp = x6;
						}else{
							if (x7 != -1){
								temp = x7;
							}else{
								if (x8 != -1){
									temp = x8;
								}else{
									temp = x9;
								}
							}
						}
					}
				}
			}
		}
	}

	if (x2 < temp && x2 != -1) temp = x2;
	if (x3 < temp && x3 != -1) temp = x3;
	if (x4 < temp && x4 != -1) temp = x4;
	if (x5 < temp && x5 != -1) temp = x5;
	if (x6 < temp && x6 != -1) temp = x6;
	if (x7 < temp && x7 != -1) temp = x7;
	if (x8 < temp && x8 != -1) temp = x8;
	if (x9 < temp && x9 != -1) temp = x9;

	return temp;
}




//-----------------------------------------------------------------------------------------------------------
void EDIF_manager::deleteNode_t(node* nodeTemp){
	if(nodeTemp){
		if(nodeTemp->next) deleteNode_t(nodeTemp->next);
		delete(nodeTemp);
	}
	return;
}
//-----------------------------------------------------------------------------------------------------------
void EDIF_manager::deleteGatehead_t(GateStruct* gateTemp){
	if(gateTemp){
		if(gateTemp->next) deleteGatehead_t(gateTemp->next);
		deleteNode_t(gateTemp->input);
		delete(gateTemp);
	}
}
	


//converting string to integer
int EDIF_manager::char2int(char c){
	switch (c){
	case '0': return 0;
	case '1': return 1;
	case '2': return 2;
	case '3': return 3;
	case '4': return 4;
	case '5': return 5;
	case '6': return 6;
	case '7': return 7;
	case '8': return 8;
	case '9': return 9;
	default: return -1;
	}

	return -1;
}
//converting string to integer
char EDIF_manager::int2char(int digit){
	switch (digit){
	case 0: return '0';
	case 1: return '1';
	case 2: return '2';
	case 3: return '3';
	case 4: return '4';
	case 5: return '5';
	case 6: return '6';
	case 7: return '7';
	case 8: return '8';
	case 9: return '9';
	default: return ' ';
	}

	return ' ';
}
//Converting integer to string
string EDIF_manager::int2str(int digit){

	string temp = "";
	string temp2= "";
	if(digit == 0) return "0";
	while(digit>0){
		int number = digit%10;
		digit = digit/10;
		temp = int2char(number);
		temp2.insert(0, temp);
	}

	return temp2;
}

void EDIF_manager::warnning_type1(long int eqNumber){
	cout<<"\nWarnning type1";
	cout<<"\nIn line number "<<eqNumber<<" duplicated gate is detected.\n";
	WAR<<"\nWarnning type1";
	WAR<<"\nIn line number "<<eqNumber<<" duplicated gate is detected.\n";
	return;
}
void EDIF_manager::warnning_type2(long int eqNumber){
	cout<<"\nWarnning type2";
	cout<<"\nIn line number "<<eqNumber<<" we detected some simboles which should be handled for fault simulation.\n";
	cout<<"The report file will show you where you can check the result on netlist format.\n";
	WAR<<"\nWarnning type2";
	WAR<<"\nIn line number "<<eqNumber<<" we detected some simboles which should be handled for fault simulation.\n";
	WAR<<"The report file will show you where you can check the result on netlist format.\n";
	return;
}

//-----------------------------------------------------------------------------------------------------

int EDIF_manager::checkingWire(string s, node* head){
	node* Temp = head;
	while(Temp){
		if(s == Temp->s){
			return 1;
		}
		Temp=Temp->next;
	}

		return 0;
}

int EDIF_manager::checkingWire(string s, long int eqNumber){
	GateStruct* Temp = Gatehead;
	while(Temp){
		if(s == Temp->Output){
			return 1;
			warnning_type1(eqNumber);
		}

		if((Temp->Type == 4 || Temp->Type == 5 || Temp->Type == 6 || Temp->Type == 7) && (s == Temp->Q)){
			return 1;
			warnning_type1(eqNumber);
		}
		if((Temp->Type == 4 || Temp->Type == 5 || Temp->Type == 6 || Temp->Type == 7) && (s == Temp->CE)){
			return 1;
			warnning_type1(eqNumber);
		}
		if((Temp->Type == 4 || Temp->Type == 5 || Temp->Type == 6 || Temp->Type == 7) && (s == Temp->CLR)){
			return 1;
			warnning_type1(eqNumber);
		}
		if((Temp->Type == 4 || Temp->Type == 5 || Temp->Type == 6 || Temp->Type == 7) && (s == Temp->D)){
			return 1;
			warnning_type1(eqNumber);
		}
		if((Temp->Type == 4 || Temp->Type == 5 || Temp->Type == 6 || Temp->Type == 7) && (s == Temp->PRE)){
			return 1;
			warnning_type1(eqNumber);
		}
		if((Temp->Type == 4 || Temp->Type == 5 || Temp->Type == 6 || Temp->Type == 7) && (s == Temp->C)){
			return 1;
			warnning_type1(eqNumber);
		}
		Temp=Temp->next;
	}

		return 0;
}
//creating not gate
string EDIF_manager::creat_NOT(string st_temp2, long int eqNumber){

	string st_temp = st_temp2;
	st_temp.insert(0, "NOT_");

	if(checkingWire(st_temp, eqNumber)) return st_temp;

	if(!currentPointer_t) {
		currentPointer_t = new GateStruct;
		currentPointer_t->input = new node;
		currentPointer_t->input->s = st_temp2;
		currentPointer_t->input->next = NULL;
		currentPointer_t->next = NULL;
		currentPointer_t->Output = st_temp;
		currentPointer_t->Type = 0;
		currentPointer_t->invert = 1;
		Gatehead_t = currentPointer_t;
		currentPointer_t->INS = "";
	}
	else{
		currentPointer_t->next = new GateStruct;
		currentPointer_t = currentPointer_t->next;
		currentPointer_t->input = new node;
		currentPointer_t->input->s = st_temp2;
		currentPointer_t->input->next = NULL;
		currentPointer_t->next = NULL;
		currentPointer_t->Output = st_temp;
		currentPointer_t->Type = 0;
		currentPointer_t->invert = 1;
		currentPointer_t->INS = "";
	}

	return st_temp;

}
//Converting string to integer
long int EDIF_manager::str2int(string str){

	long int digit=0;
	for(double i=0; i<str.length();i++)
		digit += pow(10, i)*char2int(str[str.length()-i-1]);

	return digit;
}


string EDIF_manager::creat_BUFFER(string st_temp3, string st_temp2, long int eqNumber){



	if(checkingWire(st_temp2, eqNumber)) return st_temp3;

	if(!currentPointer_t) {
		currentPointer_t = new GateStruct;
		currentPointer_t->input = new node;
		currentPointer_t->input->s = st_temp2;
		currentPointer_t->input->next = NULL;
		currentPointer_t->next = NULL;
		currentPointer_t->Output = st_temp3;
		currentPointer_t->Type = 0;
		currentPointer_t->invert = 0;
		Gatehead_t = currentPointer_t;
	}
	else{


		currentPointer_t->next = new GateStruct;
		currentPointer_t = currentPointer_t->next;
		currentPointer_t->input = new node;
		currentPointer_t->input->s = st_temp2;
		currentPointer_t->input->next = NULL;
		currentPointer_t->next = NULL;
		currentPointer_t->Output = st_temp3;
		currentPointer_t->Type = 0;
		currentPointer_t->invert = 0;
	}

	return st_temp3;

}

int EDIF_manager::checkingGate(long int eqNumber){
	GateStruct* Temp = Gatehead_t;
	if(!currentPointer_t->next){return 0;}else{
		while(Temp){
			if(currentPointer_t->next->Output == Temp->Output && currentPointer_t->next != Temp && currentPointer_t->next->invert == Temp->invert){
				return 1;
					warnning_type1(eqNumber);
			}
			Temp=Temp->next;
		}
	}
		return 0;
}



void EDIF_manager::error_type6(long int eqLineCnt){
	cout<<"\nThere is an error in your equation file.\n";
	cout<<"We do not detect any key words in line "<<eqLineCnt<<".\n";
	ERR<<"\nThere is an error in your equation file.\n";
	ERR<<"We do not detect any key words in line "<<eqLineCnt<<".\n";
	return;
}

void EDIF_manager::deleteGate(){
	stack<node*> s_temp;
	if(!currentPointer_t->next) return;
	node* temp = currentPointer_t->next->input;

	while(temp){
		s_temp.push(temp);
		temp = temp->next;
	}



	while(!s_temp.empty()){

		delete(s_temp.top());
		s_temp.pop();

	}

	delete(currentPointer_t->next);
	currentPointer_t->next = NULL;

	return;
}

int EDIF_manager::DFFHandler(string INS, string Q, string D, string C, string CLR, string PRE, string CE, string total){

	if(!currentPointer_t){
		currentPointer_t = new GateStruct;
		Gatehead_t = currentPointer_t;
		currentPointer_t->C = C;
		currentPointer_t->CE = CE;
		currentPointer_t->CLR = CLR;
		currentPointer_t->D = D;
		currentPointer_t->PRE = PRE;
		currentPointer_t->Q = Q;
		currentPointer_t->Type = 7;
		currentPointer_t->input = NULL;
		currentPointer_t->next = NULL;
		currentPointer_t->path = total;
		currentPointer_t->Output = "";
	}else{

		currentPointer_t->next = new GateStruct;
		currentPointer_t->next->C = C;
		currentPointer_t->next->CE = CE;
		currentPointer_t->next->CLR = CLR;
		currentPointer_t->next->D = D;
		currentPointer_t->next->PRE = PRE;
		currentPointer_t->next->Q = Q;
		currentPointer_t->next->Type = 7;
		currentPointer_t->next->input = NULL;
		currentPointer_t->next->next = NULL;
		currentPointer_t->next->path = total;
		currentPointer_t->next->Output = "";
		currentPointer_t = currentPointer_t->next;
	}


	return 0;
}

int EDIF_manager::wireFinder(string commandline, long int eqNumber)
{


	//QHandler();*************************handling ? option****************************
	int cmdCounter = 0;

	if(commandline.size() == 0) {error_type5(eqNumber); exit(0); return -1;}

	commandline = erase_space(commandline);
	int i= commandline.find("=");

	string Output_temp = commandline.substr(0, i);
	commandline.erase(0, i+1);

	stack<string> Stack_temp;
	stack<string> Stack1;


	int klll = Stack1.size();
	while( commandline.substr(0, 1) != ";"){
		if((commandline.substr(0, 1) == "!") || (commandline.substr(0, 1) == "(") || (commandline.substr(0, 1) == ")") || (commandline.substr(0, 1) == "^") || (commandline.substr(0, 1) == "?") || (commandline.substr(0, 1) == ":")){
			Stack1.push(commandline.substr(0, 1));
			commandline.erase(0, 1);
		} else{
			if((commandline.substr(0, 2) == "&&") || (commandline.substr(0, 2) == "||")){
				Stack1.push(commandline.substr(0, 2));
				commandline.erase(0, 2);
			}else{
				if((commandline.substr(0, 4) == "1'b0") || (commandline.substr(0, 4) == "1'b1") || (commandline.substr(0, 4) == "1'bZ")){
				Stack1.push(commandline.substr(0, 4));
				commandline.erase(0, 4);
				} else{
					int x1 = commandline.find("(");
					int x2 = commandline.find(")");
					int x3 = commandline.find("!");
					int x4 = commandline.find("&&");
					int x5 = commandline.find("||");
					int x6 = commandline.find("^");
					int x7 = commandline.find("?");
					int x8 = commandline.find(":");
					int x9 = commandline.find(";");
					if(minNum(x1, x2, x3, x4, x5, x6, x7, x8, x9) == -1) {error_type6(eqNumber); exit(0); return -1;}
					string strTemp = commandline.substr(0, minNum(x1, x2, x3, x4, x5, x6, x7, x8, x9));
					Stack1.push(strTemp);
					commandline.erase(0, minNum(x1, x2, x3, x4, x5, x6, x7, x8, x9));
				}
			}
		}
	}
	Stack1.push(";");
	stack<string> Stack_temp2;

	while(Stack1.size() != 0){
		Stack_temp2.push(Stack1.top());
		Stack1.pop();
	}
	Stack_temp2.push("=");

	Stack_temp2.push(Output_temp);


	while(Stack_temp2.size() != 0){
		int kkll = Stack_temp2.size();
		string Temp = Stack_temp2.top();
		if(!currentNode_t){
			currentNode_t = new node;
			currentNode_t->s = Stack_temp2.top();
			currentNode_t->next = NULL;
			nodeHead_t = currentNode_t;
		}else{
			currentNode_t->next = new node;
			currentNode_t->next->s = Stack_temp2.top();
			currentNode_t->next->next = NULL;
			currentNode_t = currentNode_t->next;
		}
		Stack_temp2.pop();
	}
	return 0;
}


//insert the input file into array
int EDIF_manager::wireFinder4reg(string commandline, long int eqNumber)
{

	stack<string> Stack1;

	commandline.erase(0, 3);
	Stack1.push("DFF");
	

	commandline = erase_space(commandline);


	while( commandline.substr(0, 1) != ";"){
		if((commandline.substr(0, 1) == "!") || (commandline.substr(0, 1) == "(") || (commandline.substr(0, 1) == ")") || (commandline.substr(0, 1) == "^") || (commandline.substr(0, 1) == ",")){
			Stack1.push(commandline.substr(0, 1));
			commandline.erase(0, 1);
		} else{
			if((commandline.substr(0, 2) == "&&") || (commandline.substr(0, 2) == "||")){
				Stack1.push(commandline.substr(0, 2));
				commandline.erase(0, 2);
			}else{
				if((commandline.substr(0, 4) == "1'b0") || (commandline.substr(0, 4) == "1'b1")|| (commandline.substr(0, 4) == "1'bZ")){
				Stack1.push(commandline.substr(0, 4));
				commandline.erase(0, 4);
				} else{
					int x1 = commandline.find("(");
					int x2 = commandline.find(")");
					int x3 = commandline.find("!");
					int x4 = commandline.find(",");//we changed this from wireFinder
					int x5 = commandline.find("||");
					int x6 = commandline.find("^");
					int x7 = commandline.find("?");
					int x8 = commandline.find(":");
					int x9 = commandline.find(";");
					if(minNum(x1, x2, x3, x4, x5, x6, x7, x8, x9) == -1) {error_type6(eqNumber); exit(0); return -1;}
					string strTemp = commandline.substr(0, minNum(x1, x2, x3, x4, x5, x6, x7, x8, x9));
					Stack1.push(strTemp);
					commandline.erase(0, minNum(x1, x2, x3, x4, x5, x6, x7, x8, x9));
				}
			}
		}
	}
	Stack1.push(";");
	stack<string> Stack_temp2;

	while(Stack1.size() != 0){
		Stack_temp2.push(Stack1.top());
		Stack1.pop();
	}

	while(Stack_temp2.size() != 0){
		int kkll = Stack_temp2.size();
		string Temp = Stack_temp2.top();
		if(!currentNode_t){
			currentNode_t = new node;
			currentNode_t->s = Stack_temp2.top();
			currentNode_t->next = NULL;
			nodeHead_t = currentNode_t;
		}else{
			currentNode_t->next = new node;
			currentNode_t->next->s = Stack_temp2.top();
			currentNode_t->next->next = NULL;
			currentNode_t = currentNode_t->next;
		}
		Stack_temp2.pop();
	}
	return 0;
}

string EDIF_manager::logicExtractor(string equation){



	stack<string> Stack1;
	stack<string> Stack_temp;
	string commandline = equation;

	while( commandline.substr(0, 1) != ";"){
		if((commandline.substr(0, 1) == "!") || (commandline.substr(0, 1) == "(") || (commandline.substr(0, 1) == ")") || (commandline.substr(0, 1) == "^")){
			Stack1.push(commandline.substr(0, 1));
			commandline.erase(0, 1);
		} else{
			if((commandline.substr(0, 2) == "&&") || (commandline.substr(0, 2) == "||")){
				Stack1.push(commandline.substr(0, 2));
				commandline.erase(0, 2);
			}else{
				if((commandline.substr(0, 4) == "1'b0") || (commandline.substr(0, 4) == "1'b1")  || (commandline.substr(0, 4) == "1'bZ")){
				Stack1.push(commandline.substr(0, 4));
				commandline.erase(0, 4);
				} else{
					int x1 = commandline.find("(");
					int x2 = commandline.find(")");
					int x3 = commandline.find("!");
					int x4 = commandline.find("&&");
					int x5 = commandline.find("||");
					int x6 = commandline.find("^");
					int x7 = commandline.find("?");
					int x8 = commandline.find(":");
					int x9 = commandline.find(";");
					if(minNum(x1, x2, x3, x4, x5, x6, x7, x8, x9) == -1) {return commandline;}
					string strTemp = commandline.substr(0, minNum(x1, x2, x3, x4, x5, x6, x7, x8, x9));
					Stack1.push(strTemp);
					commandline.erase(0, minNum(x1, x2, x3, x4, x5, x6, x7, x8, x9));
				}
			}
		}
	}


	//For one option
	if(Stack1.size() == 1){

		return Stack1.top();
	}

	stack<string> Stack_temp2;

	while(Stack1.size() != 0){
		Stack_temp2.push(Stack1.top());
		Stack1.pop();
	}

	//Handling inverters
	while(Stack_temp2.size() != 0){
		if(Stack_temp2.top() == "!"){
			Stack_temp2.pop();
			if(Stack_temp2.top() != "(" && Stack_temp2.top() != "1'b0" && Stack_temp2.top() != "1'b1" && Stack_temp2.top() != "1'bZ" && Stack_temp2.top() != "!"){

				string st_temp = Stack_temp2.top();
				Stack_temp2.pop();
				Stack_temp2.push(creat_NOT(st_temp, -1));

			}else{
				Stack1.push("!");
			}
		}
		Stack1.push(Stack_temp2.top());
		Stack_temp2.pop();
	}

	bool ValidGate = 0;

	int k = Stack1.size();

	int delete_flag = 0; GateStruct* Lastdeleted = NULL;

	while (Stack1.size() != 0){

		string Temp = Stack1.top();

		if(Temp == "(") {
			string Temp2 = Stack_temp.top();

			node* currentInput;

			if(!currentPointer_t){
				currentPointer_t = new GateStruct;
				Gatehead = currentPointer_t;
				currentPointer_t->input = new node;
				currentInput = currentPointer_t->input;
				currentPointer_t->input->next = NULL;
				currentPointer_t->input->s = Stack_temp.top();
				currentPointer_t->Output = Stack_temp.top();
				currentPointer_t->invert = 0;currentPointer_t->Type = 0;
				currentPointer_t->next=NULL;
				currentPointer_t->INS = "";
				ValidGate = 1;
			}else{
				currentPointer_t->next = new GateStruct;
				currentPointer_t->next->input = new node;
				currentInput = currentPointer_t->next->input;
				currentPointer_t->next->input->next = NULL;
				currentPointer_t->next->input->s = Stack_temp.top();
				currentPointer_t->next->Output = Stack_temp.top();
				currentPointer_t->next->invert = 0;currentPointer_t->next->Type = 0;
				currentPointer_t->next->next=NULL;
				currentPointer_t->next->INS = "";
				ValidGate = 1;
			}

			while(Temp2 != ")"){

				Temp2 = Stack_temp.top();
				Stack_temp.pop();


				//-------------------------------------------------------------------------
				if (Temp2 == "&&"){
					if(!currentPointer_t->next){
						currentPointer_t->Type = 1;
						currentPointer_t->Output.insert(0, "_AND_");
						currentInput->next = new node;
						currentInput = currentInput->next;
						currentInput->s = Stack_temp.top();
						currentInput->next = NULL;
						currentPointer_t->Output.insert(0, Stack_temp.top());
						currentPointer_t->next=NULL;
						Stack_temp.pop();
						Stack_temp.push(currentPointer_t->Output);
					}else{
						currentPointer_t->next->Type = 1;
						currentPointer_t->next->Output.insert(0, "_AND_");
						currentInput->next = new node;
						currentInput = currentInput->next;
						currentInput->s = Stack_temp.top();
						currentInput->next = NULL;
						currentPointer_t->next->Output.insert(0, Stack_temp.top());
						currentPointer_t->next->next=NULL;
						Stack_temp.pop();
						Stack_temp.push(currentPointer_t->next->Output);
					}
				}
				//--------------------------------------------------------------------------
				//-------------------------------------------------------------------------
				if (Temp2 == "||"){
					if(!currentPointer_t->next){
						currentPointer_t->Type = 2;
						currentPointer_t->Output.insert(0, "_OR_");
						currentInput->next = new node;
						currentInput = currentInput->next;
						currentInput->s = Stack_temp.top();
						currentInput->next = NULL;
						currentPointer_t->Output.insert(0, Stack_temp.top());
						currentPointer_t->next=NULL;
						Stack_temp.pop();
						Stack_temp.push(currentPointer_t->Output);
					}else{
						currentPointer_t->next->Type = 2;
						currentPointer_t->next->Output.insert(0, "_OR_");
						currentInput->next = new node;
						currentInput = currentInput->next;
						currentInput->s = Stack_temp.top();
						currentInput->next = NULL;
						currentPointer_t->next->Output.insert(0, Stack_temp.top());
						currentPointer_t->next->next=NULL;
						Stack_temp.pop();
						Stack_temp.push(currentPointer_t->next->Output);
					}
				}
				//--------------------------------------------------------------------------
				//-------------------------------------------------------------------------
				if (Temp2 == "^"){
					if(!currentPointer_t->next){
						currentPointer_t->Type = 3;
						currentPointer_t->Output.insert(0, "_XOR_");
						currentInput->next = new node;
						currentInput = currentInput->next;
						currentInput->s = Stack_temp.top();
						currentInput->next = NULL;
						currentPointer_t->Output.insert(0, Stack_temp.top());
						currentPointer_t->next=NULL;
						Stack_temp.pop();
						Stack_temp.push(currentPointer_t->Output);
					}else{
						currentPointer_t->next->Type = 3;
						currentPointer_t->next->Output.insert(0, "_XOR_");
						currentInput->next = new node;
						currentInput = currentInput->next;
						currentInput->s = Stack_temp.top();
						currentInput->next = NULL;
						currentPointer_t->next->Output.insert(0, Stack_temp.top());
						currentPointer_t->next->next=NULL;
						Stack_temp.pop();
						Stack_temp.push(currentPointer_t->next->Output);
					}
				}
				//--------------------------------------------------------------------------
			}
			if(!currentPointer_t->next){Stack_temp.push(currentPointer_t->Output);}else{Stack_temp.push(currentPointer_t->next->Output);}
		} else{
			if(Temp == "!")	{
				if(!currentPointer_t){
					string st_temp = creat_NOT(Stack_temp.top(), -1);
					Stack_temp.pop();
					Stack_temp.push(st_temp);

				}else{
					if(!currentPointer_t->next){
						if(!delete_flag){
							if(currentPointer_t->invert  == 1){
								currentPointer_t->invert = 0;
								if(currentPointer_t->Type == 0){
									currentPointer_t->Output.erase(0, 4);
									Stack_temp.pop();
									if(!currentPointer_t->next){Stack_temp.push(currentPointer_t->Output);}else{Stack_temp.push(currentPointer_t->next->Output);}
								}
							}
							else{
								if(currentPointer_t->Type == 0){
									currentPointer_t->Output.insert(0, "NOT_");
									Stack_temp.pop();
									if(!currentPointer_t->next){Stack_temp.push(currentPointer_t->Output);}else{Stack_temp.push(currentPointer_t->next->Output);}
								}
								currentPointer_t->invert = 1;
							}
						}else{
							if(Lastdeleted->invert  == 1){
								Lastdeleted->invert = 0;
								if(Lastdeleted->Type == 0){
									Lastdeleted->Output.erase(0, 4);
									Stack_temp.pop();
									if(!Lastdeleted->next){Stack_temp.push(Lastdeleted->Output);}else{Stack_temp.push(Lastdeleted->next->Output);}
								}
							}
							else{
								if(Lastdeleted->Type == 0){
									Lastdeleted->Output.insert(0, "NOT_");
									Stack_temp.pop();
									if(!Lastdeleted->next){Stack_temp.push(Lastdeleted->Output);}else{Stack_temp.push(Lastdeleted->next->Output);}
								}
								Lastdeleted->invert = 1;
							}
						}
					}
				}
			}else{

				Stack_temp.push(Temp);
			}
		}

		if(ValidGate) {
			if(checkingGate(-1)){
				if(!currentPointer_t->next){
					Lastdeleted = new GateStruct;
					node* nodeTemp = currentPointer_t->input;
					Lastdeleted->input = new node;
					node* nodeTemp2 =Lastdeleted->input;
					nodeTemp2->s = nodeTemp->s;
					nodeTemp2->next = NULL;
					nodeTemp = nodeTemp->next;
					while(nodeTemp){
						nodeTemp2->next = new node;
						nodeTemp2->next->s = nodeTemp->s;
						nodeTemp2->next->next = NULL;
						nodeTemp = nodeTemp->next;
						nodeTemp2 = nodeTemp2->next;
					}
					Lastdeleted->invert = currentPointer_t->invert;
					Lastdeleted->next = NULL;
					Lastdeleted->Output= currentPointer_t->Output;
					Lastdeleted->Type = currentPointer_t->Type;
				}else{
					Lastdeleted = new GateStruct;
					node* nodeTemp = currentPointer_t->next->input;
					Lastdeleted->input = new node;
					node* nodeTemp2 =Lastdeleted->input;
					nodeTemp2->s = nodeTemp->s;
					nodeTemp2->next = NULL;
					nodeTemp = nodeTemp->next;
					while(nodeTemp){
						nodeTemp2->next = new node;
						nodeTemp2->next->s = nodeTemp->s;
						nodeTemp2->next->next = NULL;
						nodeTemp = nodeTemp->next;
						nodeTemp2 = nodeTemp2->next;
					}
					Lastdeleted->invert = currentPointer_t->next->invert;
					Lastdeleted->next = NULL;
					Lastdeleted->Output= currentPointer_t->next->Output;
					Lastdeleted->Type = currentPointer_t->next->Type;
				}
				deleteGate();
				delete_flag = 1;
			}
			else {
				delete_flag = 0;
				if(!currentPointer_t->next){}else{
					delete_flag = 0;
					ValidGate = 0; currentPointer_t = currentPointer_t->next;
				}
			}
			ValidGate = 0;
		}
		Stack1.pop();

	}//End of While

	if(Stack_temp.size() == 1){
		if(delete_flag){
			currentPointer_t->next = Lastdeleted;
			currentPointer_t = currentPointer_t->next;
			return(Stack_temp.top());
		}else{
			delete(Lastdeleted);

			if(ValidGate) {
					if(checkingGate(-1)){
					if(!currentPointer_t->next){}else{
						currentPointer_t = currentPointer_t->next;
						return(Stack_temp.top());
					}
				} else{
					if(!currentPointer_t->next){}else{
						currentPointer_t = currentPointer_t->next;
						return(Stack_temp.top());
					}
				}
			} else{
				return(Stack_temp.top());
			}
		}

		ValidGate = 0;
	}

	if(Stack_temp.size() > 1){
	node* currentInput;
	string Temp2 = Stack_temp.top();
		if(!currentPointer_t){
			currentPointer_t = new GateStruct;
			Gatehead = currentPointer_t;
			currentPointer_t->input = new node;
			currentInput = currentPointer_t->input;
			currentPointer_t->input->next = NULL;
			currentPointer_t->input->s = Stack_temp.top();
			currentPointer_t->Output = Stack_temp.top();
			currentPointer_t->invert = 0;currentPointer_t->Type = 0;
			currentPointer_t->next=NULL;
			currentPointer_t->INS = "";
			ValidGate = 1;
		}else{
			currentPointer_t->next = new GateStruct;
			currentPointer_t->next->input = new node;
			currentInput = currentPointer_t->next->input;
			currentPointer_t->next->input->next = NULL;
			currentPointer_t->next->input->s = Stack_temp.top();
			currentPointer_t->next->Output = Stack_temp.top();
			currentPointer_t->next->invert = 0;currentPointer_t->next->Type = 0;
			currentPointer_t->next->next=NULL;
			currentPointer_t->next->INS = "";
			ValidGate = 1;
		}

		while(Stack_temp.size() > 1){

			Temp2 = Stack_temp.top();
			Stack_temp.pop();


			//-------------------------------------------------------------------------
			if (Temp2 == "&&"){
				if(!currentPointer_t->next){
					currentPointer_t->Type = 1;
					currentPointer_t->Output.insert(0, "_AND_");
					currentInput->next = new node;
					currentInput = currentInput->next;
					currentInput->s = Stack_temp.top();
					currentInput->next = NULL;
					currentPointer_t->Output.insert(0, Stack_temp.top());
					currentPointer_t->next=NULL;
					Stack_temp.pop();
					Stack_temp.push(currentPointer_t->Output);
				}else{
					currentPointer_t->next->Type = 1;
					currentPointer_t->next->Output.insert(0, "_AND_");
					currentInput->next = new node;
					currentInput = currentInput->next;
					currentInput->s = Stack_temp.top();
					currentInput->next = NULL;
					currentPointer_t->next->Output.insert(0, Stack_temp.top());
					currentPointer_t->next->next=NULL;
					Stack_temp.pop();
					Stack_temp.push(currentPointer_t->next->Output);
				}
			}
			//--------------------------------------------------------------------------
			//-------------------------------------------------------------------------
			if (Temp2 == "||"){
				if(!currentPointer_t->next){
					currentPointer_t->Type = 2;
					currentPointer_t->Output.insert(0, "_OR_");
					currentInput->next = new node;
					currentInput = currentInput->next;
					currentInput->s = Stack_temp.top();
					currentInput->next = NULL;
					currentPointer_t->Output.insert(0, Stack_temp.top());
					currentPointer_t->next=NULL;
					Stack_temp.pop();
					Stack_temp.push(currentPointer_t->Output);
				}else{
					currentPointer_t->next->Type = 2;
					currentPointer_t->next->Output.insert(0, "_OR_");
					currentInput->next = new node;
					currentInput = currentInput->next;
					currentInput->s = Stack_temp.top();
					currentInput->next = NULL;
					currentPointer_t->next->Output.insert(0, Stack_temp.top());
					currentPointer_t->next->next=NULL;
					Stack_temp.pop();
					Stack_temp.push(currentPointer_t->next->Output);
				}
			}
			//--------------------------------------------------------------------------
			//-------------------------------------------------------------------------
			if (Temp2 == "^"){
				if(!currentPointer_t->next){
					currentPointer_t->Type = 3;
					currentPointer_t->Output.insert(0, "_XOR_");
					currentInput->next = new node;
					currentInput = currentInput->next;
					currentInput->s = Stack_temp.top();
					currentInput->next = NULL;
					currentPointer_t->Output.insert(0, Stack_temp.top());
					currentPointer_t->next=NULL;
					Stack_temp.pop();
					Stack_temp.push(currentPointer_t->Output);
				}else{
					currentPointer_t->next->Type = 3;
					currentPointer_t->next->Output.insert(0, "_XOR_");
					currentInput->next = new node;
					currentInput = currentInput->next;
					currentInput->s = Stack_temp.top();
					currentInput->next = NULL;
					currentPointer_t->next->Output.insert(0, Stack_temp.top());
					currentPointer_t->next->next=NULL;
					Stack_temp.pop();
					Stack_temp.push(currentPointer_t->next->Output);
				}
			}
		//--------------------------------------------------------------------------
		}

		if(!currentPointer_t->next){Stack_temp.push(currentPointer_t->Output);}else{Stack_temp.push(currentPointer_t->next->Output);}


		if(ValidGate) {
			if(checkingGate(-1)){
				if(!currentPointer_t->next){}else{
					currentPointer_t = currentPointer_t->next;
					return(Stack_temp.top());
				}
			} //deleteGate();
			else{
				if(!currentPointer_t->next){}else{
					currentPointer_t = currentPointer_t->next;
					return(Stack_temp.top());
				}
			}
		} else{
			return(Stack_temp.top());
		}
		ValidGate = 0;
	}



	return commandline;

}

void EDIF_manager::error_type8_FDCPE(long int gateCounter){
	cout<<"\nThere is an error in your FDCPE number "<<gateCounter<<".\n";
	cout<<"We detected regular order for an FDCPE with 7 arguments.\n";
	ERR<<"\nThere is an error in your FDCPE number "<<gateCounter<<".\n";
	ERR<<"We detected regular order for an FDCPE with 7 arguments.\n";

	return;
}

void EDIF_manager::error_type4(long int rptLineCnt){
	cout<<"\nThere is an error in your EDIF file.\n";
	cout<<"We do not detect any equation.\n .\n";
	ERR<<"\nThere is an error in your EDIF file.\n";
	ERR<<"We do not detect any equation.\n .\n";
	return;
}


int EDIF_manager::CommandReader(string total){

	long int eqCounter = 0;
	int DFFcnt = 0;
	
	node* TempNode = nodeHead_t;

	while(TempNode){
		if( TempNode->s == "DFF" || TempNode->s == "dff"){
			DFFcnt++;
			string Arg1, Arg2, Arg3, Arg4, Arg5, Arg6;
			TempNode = TempNode->next;
			string INS = TempNode->s;
			//TempNode = TempNode->next;
			if(TempNode->s == "("){
				TempNode = TempNode->next;
				//---------
				string Temp_t = "";
				while(TempNode->s != ","){
					Temp_t.insert(Temp_t.length(), TempNode->s);
					TempNode = TempNode->next;
				}
				Temp_t.insert(Temp_t.length(), ";");
				Arg1 = logicExtractor(Temp_t);
				//----------
				if(TempNode->s == ","){
					TempNode = TempNode->next;
					//---------
					string Temp_t = "";
					while(TempNode->s != ","){
						Temp_t.insert(Temp_t.length(), TempNode->s);
						TempNode = TempNode->next;
					}
					Temp_t.insert(Temp_t.length(), ";");
					Arg2 = logicExtractor(Temp_t);
					//----------
					if(TempNode->s == ","){
						TempNode = TempNode->next;
						//---------
						string Temp_t = "";
						while(TempNode->s != ","){
							Temp_t.insert(Temp_t.length(), TempNode->s);
							TempNode = TempNode->next;
						}
						Temp_t.insert(Temp_t.length(), ";");
						Arg3 = logicExtractor(Temp_t);
						//----------
						if(TempNode->s == ","){
							TempNode = TempNode->next;
							//---------
							string Temp_t = "";
							while(TempNode->s != ","){
								Temp_t.insert(Temp_t.length(), TempNode->s);
								TempNode = TempNode->next;
							}
							Temp_t.insert(Temp_t.length(), ";");
							Arg4 = logicExtractor(Temp_t);
							//----------
							if(TempNode->s == ","){
								TempNode = TempNode->next;
								//---------
								string Temp_t = "";
								while(TempNode->s != ","){
									Temp_t.insert(Temp_t.length(), TempNode->s);
									TempNode = TempNode->next;
								}
								Temp_t.insert(Temp_t.length(), ";");
								Arg5 = logicExtractor(Temp_t);
								//----------
								if(TempNode->s == ","){
									TempNode = TempNode->next;
									//---------
									string Temp_t = "";
									while(TempNode->s != ")"){
										if(TempNode->s == ","){ cout<<"\nERROR : Can not read DFF port interface. DFF must have six port interface.";cout<<"\nPress any key to end...\n";getch();exit(0);}
										Temp_t.insert(Temp_t.length(), TempNode->s);
										TempNode = TempNode->next;
									}
									Temp_t.insert(Temp_t.length(), ";");
									Arg6 = logicExtractor(Temp_t);
									//----------
									if(TempNode->s == ")"){
										TempNode = TempNode->next;
										if(TempNode->s == ";"){
											TempNode = TempNode->next;
											DFFHandler(INS, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, total);
										}else{
											cout<<"\nERROR : Can not read DFF port interface";cout<<"\nPress any key to end...\n";getch();exit(0);
										}
									}else{
										cout<<"\nERROR : Can not read DFF port interface";cout<<"\nPress any key to end...\n";getch();exit(0);
									}
								}else{
									cout<<"\nERROR : Can not read DFF port interface";cout<<"\nPress any key to end...\n";getch();exit(0);
								}
							}else{
								cout<<"\nERROR : Can not read DFF port interface";cout<<"\nPress any key to end...\n";getch();exit(0);
							}
						}else{
							cout<<"\nERROR : Can not read DFF port interface";cout<<"\nPress any key to end...\n";getch();exit(0);
						}
					}else{
						cout<<"\nERROR : Can not read DFF port interface";cout<<"\nPress any key to end...\n";getch();exit(0);
					}
				}else{
					cout<<"\nERROR : Can not read DFF port interface";cout<<"\nPress any key to end...\n";getch();exit(0);
				}
			}else{
				cout<<"\nERROR : Can not read DFF port interface";cout<<"\nPress any key to end...\n";getch();exit(0);
			}
		}else{
			string commandline = "";
			int flag = 0;
			while(TempNode->s !=";"){
				commandline.insert(commandline.length(), TempNode->s);
				if(TempNode->s == "?") flag = 1;
				TempNode = TempNode->next;
			}
			if(TempNode->s == ";"){
				TempNode = TempNode->next;
				commandline.insert(commandline.length(), ";");
			}
			eqCounter++;
			//if(flag) //commandline = QHandler(commandline);
			assignHandler(commandline, total, eqCounter);
		}
		//if(TempNode) TempNode = TempNode->next; else break;
	}

	
	return 0;
}

void EDIF_manager::replaceGatewire(string newWire, string oldWire, GateStruct* Temp){
	
	while(Temp){
		if(oldWire == Temp->Output){
			Temp->Output = newWire;
		}
		node* Temp2 = Temp->input;
		while(Temp2){
			if(oldWire == Temp2->s){
				Temp2->s = newWire;
			}
			Temp2 = Temp2->next;
		}
		if(oldWire == Temp->D){
			Temp->D = newWire;
		}
		if(oldWire == Temp->Q){
			Temp->Q = newWire;
		}
		if(oldWire == Temp->CE){
			Temp->CE = newWire;
		}
		if(oldWire == Temp->PRE){
			Temp->PRE = newWire;
		}
		if(oldWire == Temp->CLR){
			Temp->CLR = newWire;
		}
		if(oldWire == Temp->C){
			Temp->C = newWire;
		}


		Temp=Temp->next;
	}
	return;
}

void EDIF_manager::wireAnalyzer_Gate(){

	cout<<"\n**************************************************\n";
	cout<<"\nAnalyzing wires for netlist generation...\n";
	string newWire = "Custom_Module_wire_";
	
	newWire.insert(newWire.length(), int2str(wireGateCnt_t));
	
	GateStruct* Temp = Gatehead_t;
	while(Temp){
		if(!checkingWire(Temp->Output, wiresHeadGate_t) && Temp->Output != "1'b0" && Temp->Output != "1'b1" && Temp->Output != "1'bZ" && !checkingWire(Temp->Output, EX_Inputs_1) && !checkingWire(Temp->Output, EX_Outputs_1)){
			int replaceFlag = 0;
			while(!replaceFlag){
				wireGateCnt_t++;
				newWire = "wire_";
				newWire.insert(newWire.length(), int2str(wireGateCnt_t));
				replaceGatewire(newWire, Temp->Output, Temp);
				if(!currentGateWire_t){
					currentGateWire_t = new node;
					currentGateWire_t->s = newWire;
					currentGateWire_t->next = NULL;
					wiresHeadGate_t = currentGateWire_t;
				}else{
					currentGateWire_t->next = new node;
					currentGateWire_t->next->s = newWire;
					currentGateWire_t->next->next = NULL;
					currentGateWire_t = currentGateWire_t->next;
				}
				replaceFlag = 1;
			}
		}
		node* Temp2 = Temp->input;

		while(Temp2){
			if(!checkingWire(Temp2->s, wiresHeadGate_t) && Temp2->s != "1'b0" && Temp2->s != "1'b1" && Temp2->s != "1'bZ" && !checkingWire(Temp2->s, EX_Inputs_1) && !checkingWire(Temp2->s, EX_Outputs_1)){
				int replaceFlag = 0;
				while(!replaceFlag){
					wireGateCnt_t++;
					newWire = "wire_";
					newWire.insert(newWire.length(), int2str(wireGateCnt_t));
					replaceGatewire(newWire, Temp2->s, Temp);
					if(!currentGateWire_t){
						currentGateWire_t = new node;
						currentGateWire_t->s = newWire;
						currentGateWire_t->next = NULL;
						wiresHeadGate_t = currentGateWire_t;
					}else{
						currentGateWire_t->next = new node;
						currentGateWire_t->next->s = newWire;
						currentGateWire_t->next->next = NULL;
						currentGateWire_t = currentGateWire_t->next;
					}
					replaceFlag = 1;
				}
			}
			Temp2 = Temp2->next;
		}
		Temp = Temp->next;
	}
	return;
}

string EDIF_manager::erase_first_space(string s){
	
	while(1){
		if (s[0] == ' ' || s[0] == '\t'){
			s = s.substr(1, s.length());
		}else
			return s;
	}
}




string EDIF_manager::loadCellLibrary(string total, string total_t, string strNetTemp, string portName, string cell){

	long int eqCounter = 0;
	int DFFcnt = 0;
	int rptlineCnt = 0;
	
	ifstream Input_cell_File("EDIF_Cell_Library.txt");

	if(!Input_cell_File){
		cout<<"\nERROR : Can not find any cell library file.\n";
		cout<<"\nCell "<<cell<<" referenced from xilinx synthesis tool is not supported by our default cells.";
		cout<<"\nFor creating a custom cell library file refer to NetlistGen manual.";
		cout<<"Press any key to end...\n";
		getch();
		exit(0);
		return "";
	}

	string str_temp = "";
	while(1){
		str_temp = erase_first_space(str_temp);
		if((str_temp.find("//cell ") == 0 ||  str_temp.find("//CELL ") == 0 || str_temp.find("//cell\t") == 0 ||  str_temp.find("//CELL\t") == 0)){
			string str_temp_t = str_temp;
			str_temp_t = erase_space(str_temp_t);
			str_temp_t = str_temp_t.substr(6, str_temp_t.length());
			if(cell == str_temp_t) break;
		}
		if(Input_cell_File.eof()){
			cout<<"\nERROR : We can not find cell -"<<cell<<"- in the EDIF_Cell_Library.txt.";
			cout<<"\nPress any key to end...\n";
			getch();
			exit(0);
			return "";
		}
		char temp[10000];
		Input_cell_File.getline(temp, 10000);
		rptlineCnt++;
		str_temp = temp;
		
	}

	int inputFlag = 0;
	int outputFlag = 0;

	
	while(1){
		char charTemp[10000];
		string str_temp;
		Input_cell_File.getline(charTemp, 10000);
		str_temp = charTemp;
		str_temp = erase_space(str_temp);
		if(str_temp != ""){

			if(str_temp == "//END CELL" || str_temp == "//end cell" || str_temp == "//EQUATIONS" ||str_temp == "//equations") {
				if(str_temp == "//END CELL" || str_temp == "//end cell" || (!outputFlag)){
					cout<<"\nERROR : Cell "<<cell<<" does not have correct IO. Each cell must have at least one input and one output.";
					cout<<"\nPress any key to end...\n";
					getch();
					exit(0);
					return "";
				}else
					break;
			}

		


			if(str_temp == "//inputs" || str_temp == "//INPUTS"){
				inputFlag = 1;
				outputFlag = 0;
			}else
			if(str_temp == "//OUTPUTS" || str_temp == "//outputs"){
				inputFlag = 0;
				outputFlag = 1;
			}else
			if(inputFlag){
				if(!EX_Inputs_1_t){
					EX_Inputs_1_t = new node;
					EX_Inputs_1_t->s = str_temp;
					EX_Inputs_1_t->next = NULL;
					currentInNode = EX_Inputs_1_t;
				}else{
					currentInNode->next = new node;
					currentInNode->next->s = str_temp;
					currentInNode->next->next = NULL;
					currentInNode = currentInNode->next;
				}
			}else

			if(outputFlag){
				if(!EX_Outputs_1_t){
					EX_Outputs_1_t = new node;
					EX_Outputs_1_t->s = str_temp;
					EX_Outputs_1_t->next = NULL;
					currentOutNode = EX_Outputs_1_t;
				}else{
					currentOutNode->next = new node;
					currentOutNode->next->s = str_temp;
					currentOutNode->next->next = NULL;
					currentOutNode = currentOutNode->next;
				}
			}
		}
	}


	while(1){

		char temp[10000];

		Input_cell_File.getline(temp, 10000);
		rptlineCnt++;
		str_temp = temp;
		str_temp = erase_first_space(str_temp);
		string commandline = "";
		
		if(str_temp == "//end cell" || str_temp == "//END CELL" || Input_cell_File.eof()) break;
		if(str_temp.find("assign ") == 0 || str_temp.find("assign\t") == 0){
			str_temp.erase(0, 7);
			commandline.insert(commandline.length(), str_temp);
			while(str_temp.find(";") != str_temp.length()-1){
				Input_cell_File.getline(temp, 10000);
				rptlineCnt++;
				str_temp = temp;
				commandline.insert(commandline.length(), str_temp);
			}
			eqCounter++;
			wireFinder(commandline, eqCounter);
		}

		if(str_temp.find("dff ") == 0 || str_temp.find("DFF ") == 0  || str_temp.find("dff\t") == 0 ||  str_temp.find("DFF\t") == 0  ||  str_temp.find("dff(") == 0 || str_temp.find("DFF(") == 0){
			str_temp = erase_space(str_temp);
			if(str_temp.find("dff(") == 0 || str_temp.find("DFF(") == 0){
				commandline.insert(commandline.length(), str_temp);
				while(str_temp.find(";") != str_temp.length()-1){
					Input_cell_File.getline(temp, 10000);
					rptlineCnt++;
					str_temp = temp;
					commandline.insert(commandline.length(), str_temp);
				}
				DFFcnt++;
				wireFinder4reg(commandline, eqCounter);
			}
		}
	}


	CommandReader(total);
	//wire
	//wireAnalyzer_Gate();

	string currNet = "";

	GateStruct* gateTemp = Gatehead_t;
	while(gateTemp){
		node* nodeTemp = gateTemp->input;
		while(nodeTemp){
			if(checkingWire(nodeTemp->s, EX_Inputs_1_t)){
				if(portName == nodeTemp->s){//in equal to port name
					if(strNetTemp == ""){
						currentNet.incNet();
						currNet = currentNet.getNet(total_t);
						replaceGatewire(currNet, nodeTemp->s, Gatehead_t);
						nodeTemp->s = currNet;
						strNetTemp = currNet;
					}else{
						replaceGatewire(strNetTemp, nodeTemp->s, Gatehead_t);
						nodeTemp->s = strNetTemp;
						currentNet.addNet(strNetTemp, total_t);
						currNet = strNetTemp;
					}
				}else{//--
					string str_temp_IO = total;
					str_temp_IO.insert(str_temp_IO.length(), nodeTemp->s);
					str_temp_IO.insert(str_temp_IO.length(), "/");
					netNode* net_temp = currentNet.cehckNet(str_temp);
					if(!net_temp){
						currentNet.incNet();
						string str_temp5 = currentNet.getNet(str_temp_IO);
						replaceGatewire(str_temp5, nodeTemp->s, Gatehead_t);
						nodeTemp->s = str_temp5;
					}
				}

			}


			nodeTemp = nodeTemp->next;
		} 
		
		if(checkingWire(gateTemp->C, EX_Inputs_1_t)){
			if(portName == gateTemp->C){//in equal to port name
				if(strNetTemp == ""){
					currentNet.incNet();
					currNet = currentNet.getNet(total_t);
					replaceGatewire(currNet, gateTemp->C, Gatehead_t);
					gateTemp->C = currNet;
					strNetTemp = currNet;
				}else{
					replaceGatewire(strNetTemp, gateTemp->C, Gatehead_t);
					gateTemp->C = strNetTemp;
					currentNet.addNet(strNetTemp, total_t);
					currNet = strNetTemp;
				}
			}else{//--
				string str_temp_IO = total;
				str_temp_IO.insert(str_temp_IO.length(), gateTemp->C);
				str_temp_IO.insert(str_temp_IO.length(), "/");
				netNode* net_temp = currentNet.cehckNet(str_temp);
				if(!net_temp){
					currentNet.incNet();
					string str_temp5 = currentNet.getNet(str_temp_IO);
					replaceGatewire(str_temp5, gateTemp->C, Gatehead_t);
					gateTemp->C = str_temp5;
				}
			}
		}
		if(checkingWire(gateTemp->CE, EX_Inputs_1_t)){
			if(portName == gateTemp->CE){//in equal to port name
				if(strNetTemp == ""){
					currentNet.incNet();
					currNet = currentNet.getNet(total_t);
					replaceGatewire(currNet, gateTemp->CE, Gatehead_t);
					gateTemp->CE = currNet;
					strNetTemp = currNet;
				}else{
					replaceGatewire(strNetTemp, gateTemp->CE, Gatehead_t);
					gateTemp->CE = strNetTemp;
					currentNet.addNet(strNetTemp, total_t);
					currNet = strNetTemp;
				}
			}else{//--
				string str_temp_IO = total;
				str_temp_IO.insert(str_temp_IO.length(), gateTemp->CE);
				str_temp_IO.insert(str_temp_IO.length(), "/");
				netNode* net_temp = currentNet.cehckNet(str_temp);
				if(!net_temp){
					currentNet.incNet();
					string str_temp5 = currentNet.getNet(str_temp_IO);
					replaceGatewire(str_temp5, gateTemp->CE, Gatehead_t);
					gateTemp->CE = str_temp5;
				}
			}
		}
		if(checkingWire(gateTemp->CLR, EX_Inputs_1_t)){
			if(portName == gateTemp->CLR){//in equal to port name
				if(strNetTemp == ""){
					currentNet.incNet();
					currNet = currentNet.getNet(total_t);
					replaceGatewire(currNet, gateTemp->CLR, Gatehead_t);
					gateTemp->CLR = currNet;
					strNetTemp = currNet;
				}else{
					replaceGatewire(strNetTemp, gateTemp->CLR, Gatehead_t);
					gateTemp->CLR = strNetTemp;
					currentNet.addNet(strNetTemp, total_t);
					currNet = strNetTemp;
				}
			}else{//--
				string str_temp_IO = total;
				str_temp_IO.insert(str_temp_IO.length(), gateTemp->CLR);
				str_temp_IO.insert(str_temp_IO.length(), "/");
				netNode* net_temp = currentNet.cehckNet(str_temp);
				if(!net_temp){
					currentNet.incNet();
					string str_temp5 = currentNet.getNet(str_temp_IO);
					replaceGatewire(str_temp5, gateTemp->CLR, Gatehead_t);
					gateTemp->CLR = str_temp5;
				}
			}
		} 
		if(checkingWire(gateTemp->D, EX_Inputs_1_t)){
			if(portName == gateTemp->D){//in equal to port name
				if(strNetTemp == ""){
					currentNet.incNet();
					currNet = currentNet.getNet(total_t);
					replaceGatewire(currNet, gateTemp->D, Gatehead_t);
					gateTemp->D = currNet;
					strNetTemp = currNet;
				}else{
					replaceGatewire(strNetTemp, gateTemp->D, Gatehead_t);
					gateTemp->D = strNetTemp;
					currentNet.addNet(strNetTemp, total_t);
					currNet = strNetTemp;
				}
			}else{//--
				string str_temp_IO = total;
				str_temp_IO.insert(str_temp_IO.length(), gateTemp->D);
				str_temp_IO.insert(str_temp_IO.length(), "/");
				netNode* net_temp = currentNet.cehckNet(str_temp);
				if(!net_temp){
					currentNet.incNet();
					string str_temp5 = currentNet.getNet(str_temp_IO);
					replaceGatewire(str_temp5, gateTemp->D, Gatehead_t);
					gateTemp->D = str_temp5;
				}
			}
		} 
		if(checkingWire(gateTemp->PRE, EX_Inputs_1_t)){
			if(portName == gateTemp->PRE){//in equal to port name
				if(strNetTemp == ""){
					currentNet.incNet();
					currNet = currentNet.getNet(total_t);
					replaceGatewire(currNet, gateTemp->PRE, Gatehead_t);
					gateTemp->PRE = currNet;
					strNetTemp = currNet;
				}else{
					replaceGatewire(strNetTemp, gateTemp->PRE, Gatehead_t);
					gateTemp->PRE = strNetTemp;
					currentNet.addNet(strNetTemp, total_t);
					currNet = strNetTemp;
				}
			}else{//--
				string str_temp_IO = total;
				str_temp_IO.insert(str_temp_IO.length(), gateTemp->PRE);
				str_temp_IO.insert(str_temp_IO.length(), "/");
				netNode* net_temp = currentNet.cehckNet(str_temp);
				if(!net_temp){
					currentNet.incNet();
					string str_temp5 = currentNet.getNet(str_temp_IO);
					replaceGatewire(str_temp5, gateTemp->PRE, Gatehead_t);
					gateTemp->PRE = str_temp5;
				}
			}
		}

		gateTemp = gateTemp->next;
	}

	gateTemp = Gatehead_t;

	while(gateTemp){

		if(checkingWire(gateTemp->Output, EX_Outputs_1_t)){
			if(portName == gateTemp->Output){//in equal to port name
				if(strNetTemp == ""){
					currentNet.incNet();
					currNet = currentNet.getNet(total_t);
					replaceGatewire(currNet, gateTemp->Output, Gatehead_t);
					gateTemp->Output = currNet;
					strNetTemp = currNet;
				}else{
					replaceGatewire(strNetTemp, gateTemp->Output, Gatehead_t);
					gateTemp->Output = strNetTemp;
					currentNet.addNet(strNetTemp, total_t);
					currNet = strNetTemp;
				}
			}else{//--
				string str_temp_IO = total;
				str_temp_IO.insert(str_temp_IO.length(), gateTemp->Output);
				str_temp_IO.insert(str_temp_IO.length(), "/");
				netNode* net_temp = currentNet.cehckNet(str_temp);
				if(!net_temp){
					currentNet.incNet();
					string str_temp5 = currentNet.getNet(str_temp_IO);
					replaceGatewire(str_temp5, gateTemp->Output, Gatehead_t);
					gateTemp->Output = str_temp5;
				}
			}
		} else {

			if(checkingWire(gateTemp->Q, EX_Outputs_1_t)){
				if(portName == gateTemp->Q){//in equal to port name
					if(strNetTemp == ""){
						currentNet.incNet();
						currNet = currentNet.getNet(total_t);
						replaceGatewire(currNet, gateTemp->Q, Gatehead_t);
						gateTemp->Q = currNet;
						strNetTemp = currNet;
					}else{
						replaceGatewire(strNetTemp, gateTemp->Q, Gatehead_t);
						gateTemp->Q = strNetTemp;
						currentNet.addNet(strNetTemp, total_t);
						currNet = strNetTemp;
					}
				}else{//--
					string str_temp_IO = total;
					str_temp_IO.insert(str_temp_IO.length(), gateTemp->Q);
					str_temp_IO.insert(str_temp_IO.length(), "/");
					netNode* net_temp = currentNet.cehckNet(str_temp);
					if(!net_temp){
						currentNet.incNet();
						string str_temp5 = currentNet.getNet(str_temp_IO);
						replaceGatewire(str_temp5, gateTemp->Q, Gatehead_t);
						gateTemp->Q = str_temp5;
					}
				}
			} else {
				if(gateTemp->Output != ""){
					string str_temp_IO = total;
					str_temp_IO.insert(str_temp_IO.length(), "-/intermediateHMWire/-");
					str_temp_IO.insert(str_temp_IO.length(), gateTemp->Output);
					netNode* net_temp = currentNet.cehckNet(str_temp);
					if(!net_temp){
						currentNet.incNet();
						string str_temp5 = currentNet.getNet(str_temp_IO);
						replaceGatewire(str_temp5, gateTemp->Output, Gatehead_t);
						gateTemp->Output = str_temp5;
					}
				}else {
					if(gateTemp->Q != ""){
						string str_temp_IO = total;
						str_temp_IO.insert(str_temp_IO.length(), "-/intermediateHMWire/-");
						str_temp_IO.insert(str_temp_IO.length(), gateTemp->Q);
						netNode* net_temp = currentNet.cehckNet(str_temp);
						if(!net_temp){
							currentNet.incNet();
							string str_temp5 = currentNet.getNet(str_temp_IO);
							replaceGatewire(str_temp5, gateTemp->Q, Gatehead_t);
							gateTemp->Q = str_temp5;
						}
					}
				}
			}
		}

		gateTemp = gateTemp->next;
	}

	if(currNet == ""){
		cout<<"\nERROR : Port declaration in EDIF_Cell_Library.txt does not match with the port map in EDIF file.";
		cout<<"\nPress any key to end...\n";
		getch();
		exit(0);
	}	
	

	gateConverter();
		
		

	

	return strNetTemp;
}






//insert the input file into array
//insert the input file into array
void EDIF_manager::assignHandler(string commandline, string total, long int eqNumber)
{
	int cmdCounter = 0;

	if(commandline.size() == 0) {error_type5(eqNumber);  exit(0); return;}

	commandline = erase_space(commandline);
	int i= commandline.find("=");

	string Output_temp = commandline.substr(0, i);
	commandline.erase(0, i+1);

	stack<string> Stack_temp;
	stack<string> Stack1;

	while( commandline.substr(0, 1) != ";"){
		if((commandline.substr(0, 1) == "!") || (commandline.substr(0, 1) == "(") || (commandline.substr(0, 1) == ")") || (commandline.substr(0, 1) == "^")){
			Stack1.push(commandline.substr(0, 1));
			commandline.erase(0, 1);
		} else{
			if((commandline.substr(0, 2) == "&&") || (commandline.substr(0, 2) == "||")){
				Stack1.push(commandline.substr(0, 2));
				commandline.erase(0, 2);
			}else{
				if((commandline.substr(0, 4) == "1'b0") || (commandline.substr(0, 4) == "1'b1")  || (commandline.substr(0, 4) == "1'bZ")){
				Stack1.push(commandline.substr(0, 4));
				commandline.erase(0, 4);
				} else{
					int x1 = commandline.find("(");
					int x2 = commandline.find(")");
					int x3 = commandline.find("!");
					int x4 = commandline.find("&&");
					int x5 = commandline.find("||");
					int x6 = commandline.find("^");
					int x7 = commandline.find("?");
					int x8 = commandline.find(":");
					int x9 = commandline.find(";");
					if(minNum(x1, x2, x3, x4, x5, x6, x7, x8, x9) == -1) {error_type6(eqNumber);  exit(0); return;}
					string strTemp = commandline.substr(0, minNum(x1, x2, x3, x4, x5, x6, x7, x8, x9));
					Stack1.push(strTemp);
					commandline.erase(0, minNum(x1, x2, x3, x4, x5, x6, x7, x8, x9));
				}
			}
		}
	}


	//For one option
	if(Stack1.size() == 1){

		if (Stack1.top() == "1'b0" && Stack1.top() == "1'b1" && Stack1.top() == "1'bZ" && Stack1.top().find("?") != -1) warnning_type2(eqNumber);
		creat_BUFFER(Output_temp, Stack1.top(), eqNumber);

		return;
	}

	stack<string> Stack_temp2;

	while(Stack1.size() != 0){
		Stack_temp2.push(Stack1.top());
		Stack1.pop();
	}

	//Handling inverters
	while(Stack_temp2.size() != 0){
		if(Stack_temp2.top() == "!"){
			Stack_temp2.pop();
			if(Stack_temp2.top() != "(" && Stack_temp2.top() != "1'b0" && Stack_temp2.top() != "1'b1" && Stack_temp2.top() != "1'bZ" && Stack_temp2.top() != "!"){

				string st_temp = Stack_temp2.top();
				Stack_temp2.pop();
				Stack_temp2.push(creat_NOT(st_temp, eqNumber));

			}else{
				Stack1.push("!");
			}
		}
		Stack1.push(Stack_temp2.top());
		Stack_temp2.pop();
	}

	bool ValidGate = 0;

	int k = Stack1.size();

	int delete_flag = 0; GateStruct* Lastdeleted = NULL;

	while (Stack1.size() != 0){

		string Temp = Stack1.top();

		if(Temp == "(") {
			string Temp2 = Stack_temp.top();

			node* currentInput;

			if(!currentPointer_t){
				currentPointer_t = new GateStruct;
				Gatehead_t = currentPointer_t;
				currentPointer_t->input = new node;
				currentPointer_t->path = total;
				currentInput = currentPointer_t->input;
				currentPointer_t->input->next = NULL;
				currentPointer_t->input->s = Stack_temp.top();
				currentPointer_t->Output = Stack_temp.top();
				currentPointer_t->invert = 0;currentPointer_t->Type = 0;
				currentPointer_t->next=NULL;
				currentPointer_t->INS = "";
				ValidGate = 1;
			}else{
				currentPointer_t->next = new GateStruct;
				currentPointer_t->next->input = new node;
				currentPointer_t->next->path = total;
				currentInput = currentPointer_t->next->input;
				currentPointer_t->next->input->next = NULL;
				currentPointer_t->next->input->s = Stack_temp.top();
				currentPointer_t->next->Output = Stack_temp.top();
				currentPointer_t->next->invert = 0;currentPointer_t->next->Type = 0;
				currentPointer_t->next->next=NULL;
				currentPointer_t->next->INS = "";
				ValidGate = 1;
			}

			while(Temp2 != ")"){

				Temp2 = Stack_temp.top();
				Stack_temp.pop();


				//-------------------------------------------------------------------------
				if (Temp2 == "&&"){
					if(!currentPointer_t->next){
						currentPointer_t->Type = 1;
						currentPointer_t->Output.insert(0, "_AND_");
						currentInput->next = new node;
						currentInput = currentInput->next;
						currentInput->s = Stack_temp.top();
						currentInput->next = NULL;
						currentPointer_t->Output.insert(0, Stack_temp.top());
						currentPointer_t->next=NULL;
						Stack_temp.pop();
						Stack_temp.push(currentPointer_t->Output);
					}else{
						currentPointer_t->next->Type = 1;
						currentPointer_t->next->Output.insert(0, "_AND_");
						currentInput->next = new node;
						currentInput = currentInput->next;
						currentInput->s = Stack_temp.top();
						currentInput->next = NULL;
						currentPointer_t->next->Output.insert(0, Stack_temp.top());
						currentPointer_t->next->next=NULL;
						Stack_temp.pop();
						Stack_temp.push(currentPointer_t->next->Output);
					}
				}
				//--------------------------------------------------------------------------
				//-------------------------------------------------------------------------
				if (Temp2 == "||"){
					if(!currentPointer_t->next){
						currentPointer_t->Type = 2;
						currentPointer_t->Output.insert(0, "_OR_");
						currentInput->next = new node;
						currentInput = currentInput->next;
						currentInput->s = Stack_temp.top();
						currentInput->next = NULL;
						currentPointer_t->Output.insert(0, Stack_temp.top());
						currentPointer_t->next=NULL;
						Stack_temp.pop();
						Stack_temp.push(currentPointer_t->Output);
					}else{
						currentPointer_t->next->Type = 2;
						currentPointer_t->next->Output.insert(0, "_OR_");
						currentInput->next = new node;
						currentInput = currentInput->next;
						currentInput->s = Stack_temp.top();
						currentInput->next = NULL;
						currentPointer_t->next->Output.insert(0, Stack_temp.top());
						currentPointer_t->next->next=NULL;
						Stack_temp.pop();
						Stack_temp.push(currentPointer_t->next->Output);
					}
				}
				//--------------------------------------------------------------------------
				//-------------------------------------------------------------------------
				if (Temp2 == "^"){
					if(!currentPointer_t->next){
						currentPointer_t->Type = 3;
						currentPointer_t->Output.insert(0, "_XOR_");
						currentInput->next = new node;
						currentInput = currentInput->next;
						currentInput->s = Stack_temp.top();
						currentInput->next = NULL;
						currentPointer_t->Output.insert(0, Stack_temp.top());
						currentPointer_t->next=NULL;
						Stack_temp.pop();
						Stack_temp.push(currentPointer_t->Output);
					}else{
						currentPointer_t->next->Type = 3;
						currentPointer_t->next->Output.insert(0, "_XOR_");
						currentInput->next = new node;
						currentInput = currentInput->next;
						currentInput->s = Stack_temp.top();
						currentInput->next = NULL;
						currentPointer_t->next->Output.insert(0, Stack_temp.top());
						currentPointer_t->next->next=NULL;
						Stack_temp.pop();
						Stack_temp.push(currentPointer_t->next->Output);
					}
				}
				//--------------------------------------------------------------------------
			}
			if(!currentPointer_t->next){Stack_temp.push(currentPointer_t->Output);}else{Stack_temp.push(currentPointer_t->next->Output);}
		} else{
			if(Temp == "!")	{
				if(!currentPointer_t){
					string st_temp = creat_NOT(Stack_temp.top(), eqNumber);
					Stack_temp.pop();
					Stack_temp.push(st_temp);

				}else{
					if(!currentPointer_t->next){
						if(!delete_flag){
							if(currentPointer_t->invert  == 1){
								currentPointer_t->invert = 0;
								if(currentPointer_t->Type == 0){
									currentPointer_t->Output.erase(0, 4);
									Stack_temp.pop();
									if(!currentPointer_t->next){Stack_temp.push(currentPointer_t->Output);}else{Stack_temp.push(currentPointer_t->next->Output);}
								}
							}
							else{
								if(currentPointer_t->Type == 0){
									currentPointer_t->Output.insert(0, "NOT_");
									Stack_temp.pop();
									if(!currentPointer_t->next){Stack_temp.push(currentPointer_t->Output);}else{Stack_temp.push(currentPointer_t->next->Output);}
								}
								currentPointer_t->invert = 1;
							}
						}else{
							if(Lastdeleted->invert  == 1){
								Lastdeleted->invert = 0;
								if(Lastdeleted->Type == 0){
									Lastdeleted->Output.erase(0, 4);
									Stack_temp.pop();
									if(!Lastdeleted->next){Stack_temp.push(Lastdeleted->Output);}else{Stack_temp.push(Lastdeleted->next->Output);}
								}
							}
							else{
								if(Lastdeleted->Type == 0){
									Lastdeleted->Output.insert(0, "NOT_");
									Stack_temp.pop();
									if(!Lastdeleted->next){Stack_temp.push(Lastdeleted->Output);}else{Stack_temp.push(Lastdeleted->next->Output);}
								}
								Lastdeleted->invert = 1;
							}
						}

					}else{
						if(currentPointer_t->next->invert  == 1){
							currentPointer_t->next->invert = 0;
							if(currentPointer_t->Type == 0){
								currentPointer_t->Output.erase(0, 4);
								Stack_temp.pop();
								if(!currentPointer_t->next){Stack_temp.push(currentPointer_t->Output);}else{Stack_temp.push(currentPointer_t->next->Output);}
							}
						}
						else{
							currentPointer_t->next->invert = 1;
							if(currentPointer_t->Type == 0){
								currentPointer_t->Output.insert(0, "NOT_");
								Stack_temp.pop();
								if(!currentPointer_t->next){Stack_temp.push(currentPointer_t->Output);}else{Stack_temp.push(currentPointer_t->next->Output);}
							}
						}
					}
				}
			}else{

				Stack_temp.push(Temp);
			}
		}

		if(ValidGate) {
			if(checkingGate(eqNumber)){
				if(!currentPointer_t->next){
					Lastdeleted = new GateStruct;
					node* nodeTemp = currentPointer_t->input;
					Lastdeleted->input = new node;
					node* nodeTemp2 =Lastdeleted->input;
					nodeTemp2->s = nodeTemp->s;
					nodeTemp2->next = NULL;
					nodeTemp = nodeTemp->next;
					while(nodeTemp){
						nodeTemp2->next = new node;
						nodeTemp2->next->s = nodeTemp->s;
						nodeTemp2->next->next = NULL;
						nodeTemp = nodeTemp->next;
						nodeTemp2 = nodeTemp2->next;
					}
					Lastdeleted->invert = currentPointer_t->invert;
					Lastdeleted->next = NULL;
					Lastdeleted->Output= currentPointer_t->Output;
					Lastdeleted->Type = currentPointer_t->Type;
					Lastdeleted->path = currentPointer_t->path;
				}else{
					Lastdeleted = new GateStruct;
					node* nodeTemp = currentPointer_t->next->input;
					Lastdeleted->input = new node;
					node* nodeTemp2 =Lastdeleted->input;
					nodeTemp2->s = nodeTemp->s;
					nodeTemp2->next = NULL;
					nodeTemp = nodeTemp->next;
					while(nodeTemp){
						nodeTemp2->next = new node;
						nodeTemp2->next->s = nodeTemp->s;
						nodeTemp2->next->next = NULL;
						nodeTemp = nodeTemp->next;
						nodeTemp2 = nodeTemp2->next;
					}
					Lastdeleted->invert = currentPointer_t->next->invert;
					Lastdeleted->next = NULL;
					Lastdeleted->Output= currentPointer_t->next->Output;
					Lastdeleted->Type = currentPointer_t->next->Type;
					Lastdeleted->path = currentPointer_t->next->path;
				}
				deleteGate();
				delete_flag = 1;
			}
			else {
				delete_flag = 0;
				if(!currentPointer_t->next){}else{
					delete_flag = 0;
					ValidGate = 0; currentPointer_t = currentPointer_t->next;
				}
			}
			ValidGate = 0;
		}
		Stack1.pop();

	}//End of While

	if(Stack_temp.size() == 1){
		if(delete_flag){
			currentPointer_t->next = Lastdeleted;
			currentPointer_t = currentPointer_t->next;
			currentPointer_t->Output = Output_temp;
		}else{
			delete(Lastdeleted);

			if(ValidGate) {
					if(checkingGate(eqNumber)){
					if(!currentPointer_t->next){}else{
						currentPointer_t = currentPointer_t->next;
						currentPointer_t->Output = Output_temp;
					}
				} else{
					if(!currentPointer_t->next){}else{
						currentPointer_t = currentPointer_t->next;
						currentPointer_t->Output = Output_temp;
					}
				}
			} else{
				currentPointer_t->Output = Output_temp;
			}
		}

		ValidGate = 0;
		return;
	}

	if(Stack_temp.size() > 1){
	node* currentInput;
	string Temp2 = Stack_temp.top();
		if(!currentPointer_t){
			currentPointer_t = new GateStruct;
			Gatehead_t = currentPointer_t;
			currentPointer_t->input = new node;
			currentInput = currentPointer_t->input;
			currentPointer_t->input->next = NULL;
			currentPointer_t->input->s = Stack_temp.top();
			currentPointer_t->Output = Stack_temp.top();
			currentPointer_t->invert = 0;currentPointer_t->Type = 0;
			currentPointer_t->path = total;
			currentPointer_t->next=NULL;
			currentPointer_t->INS = "";
			ValidGate = 1;
		}else{
			currentPointer_t->next = new GateStruct;
			currentPointer_t->next->input = new node;
			currentInput = currentPointer_t->next->input;
			currentPointer_t->next->input->next = NULL;
			currentPointer_t->next->input->s = Stack_temp.top();
			currentPointer_t->next->Output = Stack_temp.top();
			currentPointer_t->next->invert = 0;currentPointer_t->next->Type = 0;
			currentPointer_t->next->path = total;
			currentPointer_t->next->next=NULL;
			currentPointer_t->next->INS = "";
			ValidGate = 1;
		}

		while(Stack_temp.size() > 1){

			Temp2 = Stack_temp.top();
			Stack_temp.pop();


			//-------------------------------------------------------------------------
			if (Temp2 == "&&"){
				if(!currentPointer_t->next){
					currentPointer_t->Type = 1;
					currentPointer_t->Output.insert(0, "_AND_");
					currentInput->next = new node;
					currentInput = currentInput->next;
					currentInput->s = Stack_temp.top();
					currentInput->next = NULL;
					currentPointer_t->Output.insert(0, Stack_temp.top());
					currentPointer_t->next=NULL;
					Stack_temp.pop();
					Stack_temp.push(currentPointer_t->Output);
				}else{
					currentPointer_t->next->Type = 1;
					currentPointer_t->next->Output.insert(0, "_AND_");
					currentInput->next = new node;
					currentInput = currentInput->next;
					currentInput->s = Stack_temp.top();
					currentInput->next = NULL;
					currentPointer_t->next->Output.insert(0, Stack_temp.top());
					currentPointer_t->next->next=NULL;
					Stack_temp.pop();
					Stack_temp.push(currentPointer_t->next->Output);
				}
			}
			//--------------------------------------------------------------------------
			//-------------------------------------------------------------------------
			if (Temp2 == "||"){
				if(!currentPointer_t->next){
					currentPointer_t->Type = 2;
					currentPointer_t->Output.insert(0, "_OR_");
					currentInput->next = new node;
					currentInput = currentInput->next;
					currentInput->s = Stack_temp.top();
					currentInput->next = NULL;
					currentPointer_t->Output.insert(0, Stack_temp.top());
					currentPointer_t->next=NULL;
					Stack_temp.pop();
					Stack_temp.push(currentPointer_t->Output);
				}else{
					currentPointer_t->next->Type = 2;
					currentPointer_t->next->Output.insert(0, "_OR_");
					currentInput->next = new node;
					currentInput = currentInput->next;
					currentInput->s = Stack_temp.top();
					currentInput->next = NULL;
					currentPointer_t->next->Output.insert(0, Stack_temp.top());
					currentPointer_t->next->next=NULL;
					Stack_temp.pop();
					Stack_temp.push(currentPointer_t->next->Output);
				}
			}
			//--------------------------------------------------------------------------
			//-------------------------------------------------------------------------
			if (Temp2 == "^"){
				if(!currentPointer_t->next){
					currentPointer_t->Type = 3;
					currentPointer_t->Output.insert(0, "_XOR_");
					currentInput->next = new node;
					currentInput = currentInput->next;
					currentInput->s = Stack_temp.top();
					currentInput->next = NULL;
					currentPointer_t->Output.insert(0, Stack_temp.top());
					currentPointer_t->next=NULL;
					Stack_temp.pop();
					Stack_temp.push(currentPointer_t->Output);
				}else{
					currentPointer_t->next->Type = 3;
					currentPointer_t->next->Output.insert(0, "_XOR_");
					currentInput->next = new node;
					currentInput = currentInput->next;
					currentInput->s = Stack_temp.top();
					currentInput->next = NULL;
					currentPointer_t->next->Output.insert(0, Stack_temp.top());
					currentPointer_t->next->next=NULL;
					Stack_temp.pop();
					Stack_temp.push(currentPointer_t->next->Output);
				}
			}
		//--------------------------------------------------------------------------
		}

		if(!currentPointer_t->next){Stack_temp.push(currentPointer_t->Output);}else{Stack_temp.push(currentPointer_t->next->Output);}


		if(ValidGate) {
			if(checkingGate(eqNumber)){
				if(!currentPointer_t->next){
					currentPointer_t->Output = Output_temp;
				}else{
					currentPointer_t = currentPointer_t->next;
					currentPointer_t->Output = Output_temp;
				}
			} //deleteGate();
			else{
				if(!currentPointer_t->next){
					currentPointer_t->Output = Output_temp;
					
				}else{
					currentPointer_t = currentPointer_t->next;
					currentPointer_t->Output = Output_temp;
				}
			}
		} else{
			currentPointer_t->Output = Output_temp;
		}
		ValidGate = 0;
		return;
	}


return;


}//end getfile



//-------
int EDIF_manager::getType(string cell){//0 buffer, 1 AND, 2 OR, 3 XOR, 4 FDCE, 5, FTCPE, 6 FDDCPE, 7 FTDCPE, 8 LDCP, 9 Fanout//---------GATE TYPE-------------

	if(cell.find("AND") == 0) return 1;
	if(cell.find("NAND") == 0) return 1;
	if(cell.find("OR") == 0) return 2;
	if(cell.find("NOR") == 0) return 2;
	if(cell.find("XOR") == 0) return 3;
	if(cell.find("XNOR") == 0) return 3;
	if(cell.find("IBUF") == 0) return 0;
	if(cell.find("OBUF") == 0) return 0;
	if(cell.find("INV") == 0) return 0;
	if((cell.find("FDCE") == 0) && (cell.length() == 4)) return 4;
	if((cell.find("FD") == 0) && (cell.length() == 2) ) return 5;
	if((cell.find("FDC") == 0) && (cell.length() == 3) ) return 6;
	if((cell.find("FDCPE") == 0) && (cell.length() == 5) ) return 7;
	if(cell.find("GND") == 0) return 10;//10 GND, 11 VCC
	if(cell.find("VCC") == 0) return 11;//10 GND, 11 VDD
	 return -1;
}
//---------
bool EDIF_manager::getInv(string cell){//0 buffer, 1 AND, 2 OR, 3 XOR, 4 FDCPE, 5, FTCPE, 6 FDDCPE, 7 FTDCPE, 8 LDCP, 9 Fanout//---------GATE TYPE-------------

/*
AND2-AND8 AND Gates 
BUF Buffer
BUFE Tristate buffer (not available in XC9500XL designs)
BUFGSR Global set/reset input buffer
BUFGTS Global tristate control input buffer (uses clock-enable p-term in XC9500XL)
BUFG Global clock (FastCLK) input buffer
FDCE D-Type Flip-Flop with Clear and Clock Enable
FDCE_X D-Type Flip-Flop with Clear and Clock Enable
FDCP D-Type Flip-Flop with Asynchronous Clear and Preset
FDPE D-Type Flip-Flop with Preset and Clock Enable (uses clock-enable p-term in XC9500XL)
FDPE_X D-Type Flip-Flop with Preset and Clock Enable
IBUF Input Buffer
INV Inverter
IOBUFE Bi-Directional I/O Buffer
IOBUFE_F Bidirectional I/O Buffer--fast slew rate
IOBUFE_S Bidirectional I/O Buffer--slow slew rate
LD D-Type Latch
OBUF Output Buffer
OBUF_F Output Buffer--fast slew rate
OBUF_S Output Buffer--slow slew rate
OBUFE Tristate Output Buffer
OBUFE_F Tristate Output Buffer--fast slew rate
OBUFE_S Tristate Output Buffer--slow slew rate
OR2-OR8 OR Gates
XOR2-XOR8 XOR Gates
AND2 - AND8AND2 through AND8 are AND gates with 2 to 8 inputs.

Inferencing
The synthesizer uses these components when creating functions that require AND gates. 

Component Instantiation
U1: AND2 port map (O=>out,I1=>in2,I0=>in1);

BUF
BUF is a non-inverting buffer. 

Inferencing
The synthesizer does not use this component by inference. 

Component Instantiation
U1: BUF port map (O=>out_port, I=>in_port);

BUFE
BUFE is a non-inverting tristate buffer, with active-high enable. BUFE must not appear in XC9500XL designs.

Inferencing
The synthesizer uses these components when creating functions that require tristate buffers that drive internal signals. 

Component Instantiation
U1: BUFE port map (O=>ts_out, I=>inp, E=>enable);

BUFG
BUFG is an input buffer used to drive the Global clock signal ( GCK). 

BUFG signals may be used for active-high or active-low (inverted) clocking, and for any other logic functions in the design.

Inferencing
The synthesizer does not use this component by inference. 

Component Instantiation
U1: BUFG port map (O=>global_clk, I=>in_port);

BUFGSR
BUFGSR is an input buffer used to drive the Global set/reset signal. BUFGSR signals can drive the CLR or PRE input of any flip-flop components, and any other logic functions in the design.

Inferencing
The synthesizer does not use this component by inference. 

Component Instantiation
U1: BUFGSR port map (O=>global_sr, I=>in_port);

BUFGTS
BUFGTS is a an input buffer used to drive the global tristate control signal (GTS). BUFGTS may be used either active-high or active-low (inverted) to drive the E input of OBUFE and IOBUFE type components, and any other logic functions in the design.

Inferencing
The synthesizer does not use this component by inference. 

Component Instantiation
U1: BUFGTS port map (O=>global_oe, I=>in_port);

FDCE, FDCE_X
FDCE and FDCE_X are edge-triggered D-type flip-flops with clear and clock enable.

Inferencing
The synthesizer uses the FDCE_X component for all flip-flop functions requiring clock-enable, but not requiring asynchronous preset. The synthesizer does not use the FDCE component by inference.

Component Instantiation
U1: FDCE port map (Q=>out, D=>data, C=>clock, CLR=>async_clr, CE=>clk_enable);


FDCP
FDCP is an edge-triggered D-type flip-flop with preset and clear.

Inferencing
The synthesizer uses this component for all functions that require D-type registers, but not clock-enable.

Component Instantiation
U1: FDCP port map (Q=>out, D=>data, C=>clock, CLR=>async_clr, PRE=>async_set);

FDPE, FDPE_X
FDPE and FDPE_X are edge-triggered D-type flip-flops with preset and enable.

Inferencing
The synthesizer uses the FDPE_X component for all flip-flop functions requiring clock-enable and asynchronous preset. The synthesizer does not use the FDPE component by inference.

Component Instantiation
U1: FDPE port map (Q=>out, D=>data, C=>clock, PRE=>async_preset, CE=>clk_enable);


IBUF
IBUF is an input buffer.

Inferencing
The synthesizer uses these components to receive inputs from device pins. 

Component Instantiation
U1: IBUF port map (O=>received_signal,      I=>in_port);

INV
INV is an inverter.

Inferencing
The synthesizer uses this component for signal inversion. 

Component Instantiation
U1: INV port map (O=>not_in1, I=>in1);

IOBUFE, IOBUFE_F, IOBUFE_S
IOBUFE is a non-inverting tristate I/O buffer with active-high enable. Output slew rate is controlled by CPLD fitter options (default is fast).

IOBUFE_F is an I/O buffer with fast output slew rate.

IOBUFE_S is an I/O buffer with slow output slew rate.

Inferencing
The synthesizer uses these components to transfer signals to and from bidirectional device I/O pins.

Component Instantiation
U1: IOBUFE port map (O=>received_signal,      IO=>inout_port, I=>driving_signal,      E=>output_enable);

LD
LD is a D-type latch. 

Inferencing
The synthesizer uses LD for all transparent latches. This component can be used by inference.

Component Instantiation
U1: LD port map (Q=>out, D=>data,      G=>latch_enable);

OBUF, OBUF_F, OBUF_S
OBUF is an output buffer. Output slew rate is controlled by CPLD fitter options (default is fast).

OBUF_F is an output buffer with fast output slew rate.

OBUF_S is an output buffer with slow output slew rate.

Inferencing
The synthesizer uses this component when creating external outputs to device pins. 

Component Instantiation
U1: OBUF port map (O=>out_port,      I=>driving_signal);

OBUFE, OBUFE_F, OBUFE_S
OBUFE is a tristate output buffer with active-high enable. Output slew rate is controlled by CPLD fitter options (default is fast).

OBUFE_F is a tristate output buffer with fast output slew rate.

OBUFE_S is a tristate output buffer with slow output slew rate.

Inferencing
The synthesizer uses this component when creating tristate external outputs which connect to device pins. 

Component Instantiation
U1: OBUFE port map (O=>out_port,      I=>driving_signal, E=enable);

OR2 - OR8
OR2 through OR8 are OR gates with 2 to 8 inputs.

Inferencing
The synthesizer uses these components when creating functions that require OR gates. 

Component Instantiation
U1: OR2 port map (O=>out, I1=>in2, I0=>in1);

XOR2 - XOR8
XOR2 through XOR8 are XOR gates with 2 to 8 inputs.*/



	if(cell.find("AND") == 0) return 0;
	if(cell.find("NAND") == 0) return 1;
	if(cell.find("OR") == 0) return 0;
	if(cell.find("NOR") == 0) return 1;
	if(cell.find("XOR") == 0) return 0;
	if(cell.find("XNOR") == 0) return 1;
	if(cell.find("IBUF") == 0) return 0;
	if(cell.find("OBUF") == 0) return 0;
	if(cell.find("INV") == 0) return 1;
	if((cell.find("FDCE") == 0) && (cell.length() == 4)) return 0;
	if((cell.find("FD") == 0) && (cell.length() == 2) ) return 0;
	if((cell.find("FDC") == 0) && (cell.length() == 3) ) return 0;
	if((cell.find("FDCPE") == 0) && (cell.length() == 5) ) return 0;
	if(cell.find("GND") == 0) return 0;
	if(cell.find("VCC") == 0) return 0;
	 return 0;
}

void EDIF_manager::cellError(string cell){

	cout<<"\n**************************************************\n";
	cout<<"ERROR : Cell with the name of -"<<cell<<"- is not defined in our cell library."<<endl;
	cout<<"You should refer to EDIF file and define the function of this cell in our EDIF_Cell_Library.txt file.\n";
	cout<<"For more information about how you can define a custom cell in cellLibrary file, see the NetlistGen manual.\n";
	cout<<"You can find some predefined templates in the manual.\n";
	cout<<"\n**************************************************\n";
	cout<<"\nPress any key to continue...";
	
	
	ERR<<"\n\n************************************************************\n";
	ERR<<"ERROR : Cell with the name of -"<<cell<<"- is not defined in our library."<<endl;
	ERR<<"You should visit the EDIF file and define the faunction of this cell in our cellLibrary file.\n";
	ERR<<"For more information about how you can define a custom cell in cellLibrary file, see the NetlistGen manual.\n";
	ERR<<"You can find some predefined templates in cellLibrary file.\n";
	ERR<<"\n************************************************************\n";

	getch();

	return;
}



//--------------
node* EDIF_manager::libraryFind(string library){

	cellLib* pointerTemp = CLAddress;
	while(pointerTemp){
		if(pointerTemp->Lib == library)
			return pointerTemp->libPointer;
		pointerTemp = pointerTemp->next;
	}
		
	
	node* nodeTemp = totalFile;
	long int open = 1;
	long int close = 0;
	nodeTemp = nodeTemp->next;
	while(open != close){
		if((nodeTemp->s == "(library" && nodeTemp->next->s == library)||(nodeTemp->s == "(library" && nodeTemp->next->s == "(rename" && nodeTemp->next->next->s == library) ||(nodeTemp->s == "(external" && nodeTemp->next->s == library)){
			return nodeTemp;
		}
		if(nodeTemp->s.find("(") == 0) open++;
		if(nodeTemp->s.find(")") == 0) close++;
		nodeTemp = nodeTemp->next;
	}
	return NULL;
}
//----------

//-------------------------------------------CELL REF HANDELLER------------------------
node* EDIF_manager::cellFind(string Lib, string cell){
	cellLib* pointerTemp = CLAddress;
	while(pointerTemp){
		if(pointerTemp->cell == cell && pointerTemp->Lib == Lib)
			return pointerTemp->cellPointer;
		pointerTemp = pointerTemp->next;
	}
	
	node* libPointer = libraryFind(Lib);
	node* nodeTemp = libPointer;
	long int open = 1;
	long int close = 0;
	nodeTemp = nodeTemp->next;
	while(open != close){
		if((nodeTemp->s == "(cell" && nodeTemp->next->s == cell)||(nodeTemp->s == "(cell" && nodeTemp->next->s == "(rename" && nodeTemp->next->next->s == cell) ||(nodeTemp->s == "(external" && nodeTemp->next->s == cell)){
			if(!CLAddress){
				CLAddress = new cellLib;
				CLAddress->cellPointer = nodeTemp;
				CLAddress->libPointer = libPointer;
				CLAddress->cell = cell;
				CLAddress->Lib = Lib;
				CLAddress->next = NULL;
				currentCLAddress = CLAddress;
			}else{
				currentCLAddress->next = new cellLib;
				currentCLAddress->next->cell = cell;
				currentCLAddress->next->cellPointer = nodeTemp;
				currentCLAddress->next->libPointer = libPointer;
				currentCLAddress->next->cell = cell;
				currentCLAddress->next->Lib = Lib;
				currentCLAddress->next->next = NULL;
				currentCLAddress = currentCLAddress->next;
			}
			return nodeTemp;
		}
		if(nodeTemp->s.find("(") == 0) open++;
		if(nodeTemp->s.find(")") == 0) close++;
		nodeTemp = nodeTemp->next;
	}
	return NULL;
}
//--------------------------------------------
node* EDIF_manager::sFind (string s, node* start){

	node* nodeTemp = NULL;

	if(!start) nodeTemp = totalFile; else nodeTemp = start;


	while (nodeTemp){

		if (nodeTemp->s == s) return nodeTemp;
		nodeTemp = nodeTemp->next;
	}

	return 0;
}
//---------
node* EDIF_manager::sFind2 (string s, node* start){

	node* nodeTemp = NULL;

	if(!start){

		nodeTemp = totalFile;
		while (nodeTemp){

			if (nodeTemp->s == s) return nodeTemp;
			nodeTemp = nodeTemp->next;
		}
	}else {

		int open = 1;
		int close = 0;
		nodeTemp = start;


		nodeTemp = nodeTemp->next;

		while (open != close){

			if (nodeTemp->s == s) return nodeTemp;

			if(nodeTemp->s.find("(") == 0) open++;
			if(nodeTemp->s.find(")") == 0) close++;

			nodeTemp = nodeTemp->next;
		}
	}

	return 0;
}

//-----------------------------------------------------------------FIND GATE-------------------------------
GateStruct* EDIF_manager::gateFind(string path){
	GateStruct* Gate_t = Gatehead;
	while(Gate_t){
		if(Gate_t->path == path) return Gate_t;
		Gate_t = Gate_t->next;
	}
	return NULL;
}

//-----------------------------------------Setting primary I/Os----------------------------------
void EDIF_manager::setIO(string str_temp){
	GateStruct* Gate_temp = Gatehead;
	while(Gate_temp){
		if(Gate_temp->Output == str_temp){
			Gate_temp->Output = ioString;
		}
		if(Gate_temp->input){
			node* nodeTemp = Gate_temp->input;
			while(nodeTemp){
				if(nodeTemp->s == str_temp){
					nodeTemp->s = ioString;
				}
				nodeTemp = nodeTemp->next;
			}
		}
		Gate_temp = Gate_temp->next;
	}
	return;
}



//-----------------------------------------Setting primary I/Os----------------------------------
void EDIF_manager::netReplace(string str_tempPr, string str_tempFi){
	GateStruct* Gate_temp = Gatehead;
	while(Gate_temp){
		if(Gate_temp->Output == str_tempPr){
			Gate_temp->Output = str_tempFi;
		}
		if(Gate_temp->input){
			node* nodeTemp = Gate_temp->input;
			while(nodeTemp){
				if(nodeTemp->s == str_tempPr){
					nodeTemp->s = str_tempFi;
				}
				nodeTemp = nodeTemp->next;
			}
		}
		if(Gate_temp->Type == 4 || Gate_temp->Type == 5 || Gate_temp->Type == 6 || Gate_temp->Type == 7){
			if(Gate_temp->Q ==str_tempPr){Gate_temp->Q=str_tempFi;}
			if(Gate_temp->CE ==str_tempPr){Gate_temp->CE=str_tempFi;}
			if(Gate_temp->CLR ==str_tempPr){Gate_temp->CLR=str_tempFi;}
			if(Gate_temp->D ==str_tempPr){Gate_temp->D=str_tempFi;}
			if(Gate_temp->C ==str_tempPr){Gate_temp->C=str_tempFi;}
			if(Gate_temp->PRE ==str_tempPr){Gate_temp->PRE=str_tempFi;}
		}

		Gate_temp = Gate_temp->next;
	}
	return;
}


//---------------------------------------------------NET CELL REFERENCE HANDLLER------------------
string EDIF_manager::NETcellRefHn1(string total, string Lib, string cell, string strIns, string portName, int top, string strNetTemp){
	node* nodeTemp = cellFind(Lib, cell);
	int openC = 1;
	int closeC = 0;
	nodeTemp = nodeTemp->next;
	string strCell = "";
	string strLib = "";
	while (openC != closeC){
		if(nodeTemp->s == "(instance"){
			if((nodeTemp->next->s == "(rename" && nodeTemp->next->next->s == strIns) || (nodeTemp->next->s == strIns)){
				node* node_temp_t = sFind2("(viewRef", nodeTemp);
				if(!node_temp_t) {
					cout<<"\nERROR : can not find /reference/\n\n";
					return strNetTemp;
				}
				stack<string> S1;
				int open_t = 1;
				int close_t = 0;
				S1.push(node_temp_t->s);
				node_temp_t = node_temp_t->next;
				while (open_t != close_t){
					S1.push(node_temp_t->s);
					if(node_temp_t->s.find("(") == 0) open_t++;
					if(node_temp_t->s.find(")") == 0) close_t++;
					node_temp_t = node_temp_t->next;
				}
				stack<string> ST1, ST2, ST3;
				while(!S1.empty()){
					string temp = S1.top();
					S1.pop();
					ST1.push(temp);
				}
				node* Lib = NULL;
				while (ST1.size() != 1){
					while(ST1.top() != ")"){
						string temp = ST1.top();
						ST2.push(temp);
						ST1.pop();
					}
					int argCnt = 0;
					while(ST2.top().find("(") != 0){
						ST3.push(ST2.top());
						argCnt++;
						ST2.pop();
					}
					if(ST2.top() == "(libraryRef"){
						Lib = libraryFind(ST3.top());
						strLib = ST3.top();
						ST2.pop();
						ST1.pop();
						while(ST3.size()) ST3.pop();
					}
					if(ST2.top() == "(cellRef"){
						node* Cell = NULL;
						strCell = ST3.top();
						strNetTemp = NETcellRefHn2(total, strLib, strCell, portName, top, strNetTemp);
						ST2.pop();
						ST1.pop();
						while(ST3.size()) ST3.pop();
					}
				}
				return strNetTemp;
			}
		}
		if(nodeTemp->s.find("(") == 0) openC++;
		if(nodeTemp->s.find(")") == 0) closeC++;
		nodeTemp = nodeTemp->next;
	}
	return 0;
}


	
//---------------------------------------------------NET CELL REFERENCE HANDLLER------------------
string EDIF_manager::NETcellRefHn1(string total, string Lib, string cell, string strIns, int top, string strNetTemp, string ioName, string ioArg){
	node* nodeTemp = cellFind(Lib, cell);
	int openC = 1;
	int closeC = 0;
	nodeTemp = nodeTemp->next;
	string strCell = "";
	string strLib = "";
	while (openC != closeC){
		if(nodeTemp->s == "(instance"){
			if((nodeTemp->next->s == "(rename" && nodeTemp->next->next->s == strIns) || (nodeTemp->next->s == strIns)){
				node* node_temp_t = sFind2("(viewRef", nodeTemp);
				if(!node_temp_t) {
					cout<<"\nERROR : can not find /reference/\n\n";
					return strNetTemp;
				}
				stack<string> S1;
				int open_t = 1;
				int close_t = 0;
				S1.push(node_temp_t->s);
				node_temp_t = node_temp_t->next;
				while (open_t != close_t){
					S1.push(node_temp_t->s);
					if(node_temp_t->s.find("(") == 0) open_t++;
					if(node_temp_t->s.find(")") == 0) close_t++;
					node_temp_t = node_temp_t->next;
				}
				stack<string> ST1, ST2, ST3;
				while(!S1.empty()){
					string temp = S1.top();
					S1.pop();
					ST1.push(temp);
				}
				node* Lib = NULL;
				while (ST1.size() != 1){
					while(ST1.top() != ")"){
						string temp = ST1.top();
						ST2.push(temp);
						ST1.pop();
					}
					int argCnt = 0;
					while(ST2.top().find("(") != 0){
						ST3.push(ST2.top());
						argCnt++;
						ST2.pop();
					}
					if(ST2.top() == "(libraryRef"){
						Lib = libraryFind(ST3.top());
						strLib = ST3.top();
						ST2.pop();
						ST1.pop();
						while(ST3.size()) ST3.pop();
					}
					if(ST2.top() == "(cellRef"){
						node* Cell = NULL;
						strCell = ST3.top();
						strNetTemp = NETcellRefHn2(total, strLib, strCell, top, strNetTemp, ioName, ioArg);
						ST2.pop();
						ST1.pop();
						while(ST3.size()) ST3.pop();
					}
				}
				return strNetTemp;
			}
		}
		if(nodeTemp->s.find("(") == 0) openC++;
		if(nodeTemp->s.find(")") == 0) closeC++;
		nodeTemp = nodeTemp->next;
	}
	return 0;
}
//---------------------------------------------------------------------------------------------------------

void EDIF_manager::gateConverter(){

	if(!Gatehead)
		Gatehead = Gatehead_t;
	else{		
		currentPointer->next = Gatehead_t;
		currentPointer = currentPointer_t;
	}

	Gatehead_t = NULL;
	currentPointer_t = NULL;
		
	return;
}




	
//------------------------------------------------------------------MAKING GATE STRUCT---------------------
string EDIF_manager::NETcellRefHn2(string total, string Lib, string cell, string portName, int top, string strNetTemp){
	
	node* cellPointer = cellFind(Lib, cell);
	node* nodeTemp = cellPointer;
	int open = 1;
	int close = 0;
	int instFlag = 0;
	int netFlag = 0;
	
	node* cellTag = NULL;
	
	int openC = 1;
	int closeC = 0;
	nodeTemp = nodeTemp->next;
	while(openC != closeC){
		if(nodeTemp->s == "(net"){
			netFlag = 1;
			int portFindFlag = 0;
			node* nodeTemp4 = nodeTemp;
			int openN = 1;
			int closeN = 0;
			nodeTemp4 = nodeTemp4->next;
			while(openN != closeN){
				if(nodeTemp4->s == "(portRef" && nodeTemp4->next->s == portName) portFindFlag = 1;
				if(nodeTemp4->s.find("(") == 0) openN++;
				if(nodeTemp4->s.find(")") == 0) closeN++;
				nodeTemp4 = nodeTemp4->next;
			}
			if(portFindFlag){
				strNetTemp =  netHn (total, cell, Lib,  nodeTemp, top, strNetTemp);//------------------------
			}
		}
		if(nodeTemp->s == "(instance"){
			instFlag = 1;
		}
		if(nodeTemp->s.find("(") == 0) openC++;
		if(nodeTemp->s.find(")") == 0) closeC++;
		nodeTemp=nodeTemp->next;
	}

	string currNet = strNetTemp;
	if(!instFlag && !netFlag){
		int open = 1;
		int close = 0;
		nodeTemp = cellPointer;
		nodeTemp = nodeTemp->next;
		while(open != close){
			if((nodeTemp->s == "(port" && nodeTemp->next->s == "(rename" && nodeTemp->next->next->s == portName)||(nodeTemp->s == "(port" && nodeTemp->next->s == portName)){
				string total_t = total; 
				total_t.insert(total_t.length(), portName);
				total_t.insert(total_t.length(), "/");
				netNode* netTemp = currentNet.cehckNet(total_t);
				if(!netTemp){
					GateStruct* Gate_t = gateFind(total);
					if(!Gate_t){
						if(getType(cell) == -1) {
								cout<<"\nRefering to custom cell library...";
								currNet = loadCellLibrary(total, total_t, strNetTemp, portName, cell);
						}else{
							//------------------------------------------
							if(!Gatehead){
								Gatehead = new GateStruct;
								Gatehead->path = total;
								total = total_t;
								Gatehead->Type = getType(cell);
								Gatehead->invert = getInv(cell);
							
								//---------------------------------------------
								if(Gatehead->Type != 4  && Gatehead->Type != 5   && Gatehead->Type != 6 && Gatehead->Type != 7){
									if(sFind("(direction", nodeTemp)->next->s == "INPUT"){
										if(!Gatehead->input){
											Gatehead->input = new node;
											if(strNetTemp == ""){
												currentNet.incNet();
												currNet = currentNet.getNet(total);
												Gatehead->input->s = currNet;
											}else{
												Gatehead->input->s = strNetTemp;
												currentNet.addNet(strNetTemp, total_t);
												currNet = strNetTemp;
											}
											Gatehead->input->next = NULL;
										}else{
											node* node_t = Gatehead->input;
											while(node_t->next) node_t = node_t->next;
											node_t->next = new node;
											if(strNetTemp == ""){
												currentNet.incNet();
												currNet = currentNet.getNet(total);
												node_t->next->s = currNet;
											}else{
												node_t->next->s = strNetTemp;
												currentNet.addNet(strNetTemp, total_t);
												currNet = strNetTemp;
											}
											node_t->next->next = NULL;
										}
									}else{
										if(sFind("(direction", nodeTemp)->next->s == "OUTPUT"){
											if(strNetTemp == ""){
												currentNet.incNet();
												currNet = currentNet.getNet(total);
												Gatehead->Output = currNet;
											}else{
												Gatehead->Output = strNetTemp;
												currentNet.addNet(strNetTemp, total_t);
												currNet = strNetTemp;
											}
										}
									}
									Gatehead->next = NULL;
									currentPointer = Gatehead;
								}
								else{
									if(portName == "Q"){
										if(strNetTemp == ""){
											currentNet.incNet();
											currNet = currentNet.getNet(total);
											Gatehead->Q = currNet;
										}else{
											Gatehead->Q = strNetTemp;
											currentNet.addNet(strNetTemp, total_t);
											currNet = strNetTemp;
										}
									}
									if(portName == "D"){
										if(strNetTemp == ""){
											currentNet.incNet();
											currNet = currentNet.getNet(total);
											Gatehead->D = currNet;
										}else{
											Gatehead->D = strNetTemp;
											currentNet.addNet(strNetTemp, total_t);
											currNet = strNetTemp;
										}
									}
									if(portName == "C"){
										if(strNetTemp == ""){
											currentNet.incNet();
											currNet = currentNet.getNet(total);
											Gatehead->C = currNet;
										}else{
											Gatehead->C = strNetTemp;
											currentNet.addNet(strNetTemp, total_t);
											currNet = strNetTemp;
										}
									}
									if(portName == "CLR"){
										if(strNetTemp == ""){
											currentNet.incNet();
											currNet = currentNet.getNet(total);
											Gatehead->CLR = currNet;
										}else{
											Gatehead->CLR = strNetTemp;
											currentNet.addNet(strNetTemp, total_t);
											currNet = strNetTemp;
										}
									}
									if(portName == "PRE"){
										if(strNetTemp == ""){
											currentNet.incNet();
											currNet = currentNet.getNet(total);
											Gatehead->PRE = currNet;
										}else{
											Gatehead->PRE = strNetTemp;
											currentNet.addNet(strNetTemp, total_t);
											currNet = strNetTemp;
										}
									}
									
									if(portName == "CE"){
										if(strNetTemp == ""){
											currentNet.incNet();
											currNet = currentNet.getNet(total);
											Gatehead->CE = currNet;
										}else{
											Gatehead->CE = strNetTemp;
											currentNet.addNet(strNetTemp, total_t);
											currNet = strNetTemp;
										}
									}	

									Gatehead->next = NULL;
									currentPointer = Gatehead;
								}
							}else{
								currentPointer->next = new GateStruct;
								currentPointer->next->path = total;
								total = total_t;
								currentPointer->next->Type = getType(cell);
								if(currentPointer->next->Type != 4 && currentPointer->next->Type != 5  && currentPointer->next->Type != 6 && currentPointer->next->Type != 7){
									currentPointer->next->invert = getInv(cell);
									if(sFind("(direction", nodeTemp)->next->s == "INPUT"){
										if(!currentPointer->next->input){
											currentPointer->next->input = new node;
											if(strNetTemp == ""){
												currentNet.incNet();
												currNet = currentNet.getNet(total);
												currentPointer->next->input->s = currNet;
											}else{
												currentPointer->next->input->s = strNetTemp;
												currentNet.addNet(strNetTemp, total_t);
												currNet = strNetTemp;
											}
											currentPointer->next->input->next = NULL;
										}else{
											node* node_t = currentPointer->next->input;
											while(node_t->next) node_t = node_t->next;
											node_t->next = new node;
											if(strNetTemp == ""){
												currentNet.incNet();
												currNet = currentNet.getNet(total);
												node_t->next->s = currNet;
											}else{
												node_t->next->s = strNetTemp;
												currentNet.addNet(strNetTemp, total_t);
												currNet = strNetTemp;
											}
											node_t->next->next = NULL;
										}
									}else{
										if(sFind("(direction", nodeTemp)->next->s == "OUTPUT"){
											if(strNetTemp == ""){
												currentNet.incNet();
												currNet = currentNet.getNet(total);
												currentPointer->next->Output = currNet;
											}else{
												currentPointer->next->Output = strNetTemp;
												currentNet.addNet(strNetTemp, total_t);
												currNet = strNetTemp;
											}
										}
									}
									currentPointer->next->next = NULL;
									currentPointer = currentPointer->next;
								}else{
									if(portName == "Q"){
										if(strNetTemp == ""){
											currentNet.incNet();
											currNet = currentNet.getNet(total);
											currentPointer->next->Q = currNet;
										}else{
											currentPointer->next->Q = strNetTemp;
											currentNet.addNet(strNetTemp, total_t);
											currNet = strNetTemp;
										}
									}
									if(portName == "D"){
										if(strNetTemp == ""){
											currentNet.incNet();
											currNet = currentNet.getNet(total);
											currentPointer->next->D = currNet;
										}else{
											currentPointer->next->D = strNetTemp;
											currentNet.addNet(strNetTemp, total_t);
											currNet = strNetTemp;
										}
									}
									if(portName == "C"){
										if(strNetTemp == ""){
											currentNet.incNet();
											currNet = currentNet.getNet(total);
											currentPointer->next->C = currNet;
										}else{
											currentPointer->next->C = strNetTemp;
											currentNet.addNet(strNetTemp, total_t);
											currNet = strNetTemp;
										}
									}
									if(portName == "CLR"){
										if(strNetTemp == ""){
											currentNet.incNet();
											currNet = currentNet.getNet(total);
											currentPointer->next->CLR = currNet;
										}else{
											currentPointer->next->CLR = strNetTemp;
											currentNet.addNet(strNetTemp, total_t);
											currNet = strNetTemp;
										}
									}
									if(portName == "PRE"){
										if(strNetTemp == ""){
											currentNet.incNet();
											currNet = currentNet.getNet(total);
											currentPointer->next->PRE = currNet;
										}else{
											currentPointer->next->PRE = strNetTemp;
											currentNet.addNet(strNetTemp, total_t);
											currNet = strNetTemp;
										}
									}
									if(portName == "CE"){
										if(strNetTemp == ""){
											currentNet.incNet();
											currNet = currentNet.getNet(total);
											currentPointer->next->CE = currNet;
										}else{
											currentPointer->next->CE = strNetTemp;
											currentNet.addNet(strNetTemp, total_t);
											currNet = strNetTemp;
										}
									}	

									currentPointer->next->next = NULL;
									currentPointer = currentPointer->next;
								}
							}
						}
					}else{
						//------------------------------------------
						total = total_t;
						if(Gate_t->Type != 4 && Gate_t->Type != 5 && Gate_t->Type != 6 && Gate_t->Type != 7){
							if(sFind("(direction", nodeTemp)->next->s == "INPUT"){
								if(!Gate_t->input){
									Gate_t->input = new node;
									if(strNetTemp == ""){
										currentNet.incNet();
										currNet = currentNet.getNet(total);
										Gate_t->input->s = currNet;
									}else{
										Gate_t->input->s = strNetTemp;
										currentNet.addNet(strNetTemp, total_t);
										currNet = strNetTemp;
									}
									Gate_t->input->next = NULL;
								}else{
									node* node_t = Gate_t->input;
									while(node_t->next) node_t = node_t->next;
									node_t->next = new node;
									if(strNetTemp == ""){
										currentNet.incNet();
										currNet = currentNet.getNet(total);
										node_t->next->s = currNet;
									}else{
										node_t->next->s = strNetTemp;
										currentNet.addNet(strNetTemp, total_t);
										currNet = strNetTemp;
									}
									node_t->next->next = NULL;
								}
							}else{
								if(sFind("(direction", nodeTemp)->next->s == "OUTPUT"){
									if(strNetTemp == ""){
										currentNet.incNet();
										currNet = currentNet.getNet(total);
										Gate_t->Output = currNet;
									}else{
										Gate_t->Output = strNetTemp;
										currentNet.addNet(strNetTemp, total_t);
										currNet = strNetTemp;
									}
								}
							}
						}else{
							if(portName == "Q"){
								if(strNetTemp == ""){
									currentNet.incNet();
									currNet = currentNet.getNet(total);
									Gate_t->Q = currNet;
								}else{
									Gate_t->Q = strNetTemp;
									currentNet.addNet(strNetTemp, total_t);
									currNet = strNetTemp;
								}
							}
							if(portName == "D"){
								if(strNetTemp == ""){
									currentNet.incNet();
									currNet = currentNet.getNet(total);
									Gate_t->D = currNet;
								}else{
									Gate_t->D = strNetTemp;
									currentNet.addNet(strNetTemp, total_t);
									currNet = strNetTemp;
								}
							}
							if(portName == "C"){
								if(strNetTemp == ""){
									currentNet.incNet();
									currNet = currentNet.getNet(total);
									Gate_t->C = currNet;
								}else{
									Gate_t->C = strNetTemp;
									currentNet.addNet(strNetTemp, total_t);
									currNet = strNetTemp;
								}
							}
							if(portName == "CLR"){
								if(strNetTemp == ""){
									currentNet.incNet();
									currNet = currentNet.getNet(total);
									Gate_t->CLR = currNet;
								}else{
									Gate_t->CLR = strNetTemp;
									currentNet.addNet(strNetTemp, total_t);
									currNet = strNetTemp;
								}
							}
							if(portName == "PRE"){
								if(strNetTemp == ""){
									currentNet.incNet();
									currNet = currentNet.getNet(total);
									Gate_t->PRE = currNet;
								}else{
									Gate_t->PRE = strNetTemp;
									currentNet.addNet(strNetTemp, total_t);
									currNet = strNetTemp;
								}
							}
							if(portName == "CE"){
								if(strNetTemp == ""){
									currentNet.incNet();
									currNet = currentNet.getNet(total);
									Gate_t->CE = currNet;
								}else{
									Gate_t->CE = strNetTemp;
									currentNet.addNet(strNetTemp, total_t);
									currNet = strNetTemp;
								}
							}
						}
					}
				}else{
					if(strNetTemp == ""){
						currNet = netTemp->name;
						currentNet.addNet(netTemp->name, total_t);
					}else{
						string str_temp5 = netTemp->name;
						netReplace(str_temp5, strNetTemp);
						currentNet.addNet(strNetTemp, total_t, str_temp5);
						currNet = strNetTemp;
					}
				}
			} 
		
			if(nodeTemp->s.find("(") == 0) open++;
			if(nodeTemp->s.find(")") == 0) close++;
			nodeTemp=nodeTemp->next;
		}
	}

	

	
	if(!instFlag && netFlag){
		nodeTemp = cellPointer;
		int open = 1;
		int close = 0;
		int instFlag = 0;
		nodeTemp = nodeTemp->next;
		while (open != close){
			if((nodeTemp->s == "(port" && nodeTemp->next->s == "(rename" && nodeTemp->next->next->s == portName)||(nodeTemp->s == "(port" && nodeTemp->next->s == portName)){
		
				string total_t = total;
				total_t.insert(total_t.length(), portName);
				total_t.insert(total_t.length(), "/");
				netNode* netTemp = currentNet.cehckNet(total_t);
				if(!netTemp){
					GateStruct* Gate_t = gateFind(total);
					if(!Gate_t){
					//------------------------------------------
						if(!Gatehead){
							Gatehead = new GateStruct;
							Gatehead->path = total;
							total = total_t;
							Gatehead->Type = 0;
							Gatehead->invert = 0;
							if(sFind("(direction", nodeTemp)->next->s == "INPUT"){
								if(!Gatehead->input){
									Gatehead->input = new node;
									if(strNetTemp == ""){
										currentNet.incNet();
										currNet = currentNet.getNet(total);
										Gatehead->input->s = currNet;
									}else{
										Gatehead->input->s = strNetTemp;
										currentNet.addNet(strNetTemp, total_t);
										currNet = strNetTemp;
									}
									Gatehead->input->next = NULL;
								}else{
									node* node_t = Gatehead->input;
									while(node_t->next) node_t = node_t->next;
									node_t->next = new node;
									if(strNetTemp == ""){
										currentNet.incNet();
										currNet = currentNet.getNet(total);
										node_t->next->s = currNet;
									}else{
										node_t->next->s = strNetTemp;
										currentNet.addNet(strNetTemp, total_t);
										currNet = strNetTemp;
									}
									node_t->next->next = NULL;
								}
							}else{
								if(sFind("(direction", nodeTemp)->next->s == "OUTPUT"){
									if(strNetTemp == ""){
										currentNet.incNet();
										currNet = currentNet.getNet(total);
										Gatehead->Output = currNet;
									}else{
										Gatehead->Output = strNetTemp;
										currentNet.addNet(strNetTemp, total_t);
										currNet = strNetTemp;
									}
								}
							}
							Gatehead->next = NULL;
							currentPointer = Gatehead;
						}else{
							currentPointer->next = new GateStruct;
							currentPointer->next->path = total;
							total = total_t;
							currentPointer->next->Type = 0;
							currentPointer->next->invert = 0;
							if(sFind("(direction", nodeTemp)->next->s == "INPUT"){
								if(!currentPointer->next->input){
									currentPointer->next->input = new node;
									if(strNetTemp == ""){
										currentNet.incNet();
										currNet = currentNet.getNet(total);
										currentPointer->next->input->s = currNet;
									}else{
										currentPointer->next->input->s = strNetTemp;
										currentNet.addNet(strNetTemp, total_t);
										currNet = strNetTemp;
									}
									currentPointer->next->input->next = NULL;
								}else{
									node* node_t = currentPointer->next->input;
									while(node_t->next) node_t = node_t->next;
									node_t->next = new node;
									if(strNetTemp == ""){
										currentNet.incNet();
										currNet = currentNet.getNet(total);
										node_t->next->s = currNet;
									}else{
										node_t->next->s = strNetTemp;
										currentNet.addNet(strNetTemp, total_t);
										currNet = strNetTemp;
									}
									node_t->next->next = NULL;
								}
							}else{
								if(sFind("(direction", nodeTemp)->next->s == "OUTPUT"){
									if(strNetTemp == ""){
										currentNet.incNet();
										currNet = currentNet.getNet(total);
										currentPointer->next->Output = currNet;
									}else{
										currentPointer->next->Output = strNetTemp;
										currentNet.addNet(strNetTemp, total_t);
										currNet = strNetTemp;
									}
								}
							}
							currentPointer->next->next = NULL;
							currentPointer = currentPointer->next;
						}
					}else{
						//------------------------------------------
						total = total_t;
						if(sFind("(direction", nodeTemp)->next->s == "INPUT"){
							if(!Gate_t->input){
								Gate_t->input = new node;
								if(strNetTemp == ""){
									currentNet.incNet();
									currNet = currentNet.getNet(total);
									Gate_t->input->s = currNet;
								}else{
									Gate_t->input->s = strNetTemp;
									currentNet.addNet(strNetTemp, total_t);
									currNet = strNetTemp;
								}
								Gate_t->input->next = NULL;
							}else{
								node* node_t = Gate_t->input;
								while(node_t->next) node_t = node_t->next;
								node_t->next = new node;
								if(strNetTemp == ""){
									currentNet.incNet();
									currNet = currentNet.getNet(total);
									node_t->next->s = currNet;
								}else{
									node_t->next->s = strNetTemp;
									currentNet.addNet(strNetTemp, total_t);
									currNet = strNetTemp;
								}
								node_t->next->next = NULL;
							}
						}else{
							if(sFind("(direction", nodeTemp)->next->s == "OUTPUT"){
								if(strNetTemp == ""){
									currentNet.incNet();
									currNet = currentNet.getNet(total);
									Gate_t->Output = currNet;
								}else{
									Gate_t->Output = strNetTemp;
									currentNet.addNet(strNetTemp, total_t);
									currNet = strNetTemp;
								}
							}
						}
					}
				}else{
					if(strNetTemp == ""){
						currNet = netTemp->name;
						currentNet.addNet(netTemp->name, total_t);
					}else{
						string str_temp5 = netTemp->name;
						netReplace(str_temp5, strNetTemp);
						currentNet.addNet(strNetTemp, total_t, str_temp5);
						currNet = strNetTemp;
					}
				}
			}
		
			if(nodeTemp->s.find("(") == 0) open++;
			if(nodeTemp->s.find(")") == 0) close++;
			nodeTemp = nodeTemp->next;
		}
	}

	/*if (strNetTemp == "intermediateXXXwire_398"){
		int n;
		n++;
	}*/


	/*ofstream FILE("TestTest");
	GateStruct* gategate = Gatehead;
	while(gategate){
		FILE<<gategate->Output<<endl;
		gategate = gategate->next;
	}*/


	return currNet;
}

//------------------------------------------------------------------ARRAY TYPE-----------------------------
//------------------------------------------------------------------MAKING GATE STRUCT---------------------
string EDIF_manager::NETcellRefHn2(string total, string Lib, string cell, int top, string strNetTemp, string ioName, string ioArg){
	node* nodeTemp = cellFind(Lib, cell);
	int instFlag = 0;
	int netFlag = 0;
	nodeTemp = nodeTemp->next;
	
	int openC = 1;
	int closeC = 0;
	while(openC != closeC){
		if(nodeTemp->s == "(net"){
			netFlag = 1;
			int portFindFlag = 0;
			node* nodeTemp4 = nodeTemp;
			int openN = 1;
			int closeN = 0;
			nodeTemp4 = nodeTemp4->next;
			while(openN != closeN){
				if(nodeTemp4->s == "(portRef" && nodeTemp4->next->next->s == ioName && nodeTemp4->next->next->next->s == ioArg) portFindFlag = 1;
				if(nodeTemp4->s.find("(") == 0) openN++;
				if(nodeTemp4->s.find(")") == 0) closeN++;
				nodeTemp4 = nodeTemp4->next;
			}
			if(portFindFlag){
				strNetTemp =  netHn (total, cell, Lib,  nodeTemp, top, strNetTemp);//------------------------
			}
		}
		if(nodeTemp->s.find("(") == 0) openC++;
		if(nodeTemp->s.find(")") == 0) closeC++;
		nodeTemp=nodeTemp->next;
	}

	string currNet = strNetTemp;
	if(!instFlag && !netFlag){
		cout<<"\n**************************************************\n";
		cout<<"ERROR : There is an array definition in UNISIM library which is not handled by this software...\n";

	}

	return currNet;
}
//--------------------------------------------NET HANDLLER-----------------------------------------
string EDIF_manager::netHn (string total, string cell, string strLib, node* nodeTemp, int top, string strNetTemp){
	
	
	nodeTemp = sFind2("(joined", nodeTemp);
	if(!nodeTemp) {
		cout<<"\nERROR: can not find /reference/\n\n";
		return "";
	}
	stack<string> S1;
	int open = 1;
	int close = 0;
	S1.push(nodeTemp->s);
	nodeTemp = nodeTemp->next;
	while (open != close){
		S1.push(nodeTemp->s);
		if(nodeTemp->s.find("(") == 0) open++;
		if(nodeTemp->s.find(")") == 0) close++;
		nodeTemp = nodeTemp->next;
	}
	stack<string> ST1, ST2, ST3;
	while(!S1.empty()){
		string temp = S1.top();
		S1.pop();
		ST1.push(temp);
	}
	string insRef = "";
	int insRefFlag = 0;
	int arrayFlag = 0;
	string SinsTemp = "";
	string total_t = "";
	string ioName;
	string ioArg;
	while (ST1.size() != 1){
		while(ST1.top() != ")"){
			string temp = ST1.top();
			ST2.push(temp);
			ST1.pop();
		}
		int argCnt = 0;
		while(ST2.top().find("(") != 0){
			ST3.push(ST2.top());
			argCnt++;
			ST2.pop();
		}
		if(ST2.top() == "(instanceRef"){
			insRefFlag = 1;
			total_t = total;
			total_t.insert(total_t.length(), ST3.top());
			SinsTemp = ST3.top();
			total_t.insert(total_t.length(), "/");
			ST2.pop();
			ST1.pop();
			while(ST3.size()) ST3.pop();
		}
		if(ST2.top() == "(member"){

			arrayFlag = 1;
			ioName = ST3.top();
			ST3.pop();
			ioArg = ST3.top();
			ST2.pop();
			ST1.pop();
			while(ST3.size()) ST3.pop();
		} else	if(ST2.top() == "(portRef"){
			node* Cell = NULL;
			if(arrayFlag && !insRefFlag){
				if(!top) {
					ioString = ioName;
					ioString.insert(ioString.length(), "_");
					ioString.insert(ioString.length(), ioArg);
					ioStringFlag = 1;
				}
				ST2.pop();
				ST1.pop();
				while(ST3.size()) ST3.pop();
				arrayFlag = 0;
			} else if(!arrayFlag && !insRefFlag){
				if(!top){
					ioString = ST3.top();
					ioStringFlag = 1;
				}
				ST2.pop();
				ST1.pop();
				while(ST3.size()) ST3.pop();
			} else if(insRefFlag && !arrayFlag) {
				//string strTemp = currentNet.checkRootNet(total_t);
				//if(strTemp == "") 
					strNetTemp = NETcellRefHn1(total_t, strLib, cell, SinsTemp, ST3.top(), 1, strNetTemp);
				//else strNetTemp = strTemp;
				ST2.pop();
				ST1.pop();
				while(ST3.size()) ST3.pop();
				insRefFlag = 0;
			} else if(insRefFlag && arrayFlag){
				//string strTemp = currentNet.checkRootNet(total_t);
				//if(strTemp == "") 
					strNetTemp = NETcellRefHn1(total_t, strLib, cell, SinsTemp, 1, strNetTemp, ioName, ioArg);

				//else strNetTemp = strTemp;
				ST2.pop();
				ST1.pop();
				while(ST3.size()) ST3.pop();
				insRefFlag = 0;
				arrayFlag = 0;
			}
		}
	}
	return strNetTemp;
}


//----------------------------------INS------------------------------------------------
void EDIF_manager::instanceHn (string total, node* insTemp, int top){
	string SinsTemp = "";
	if(insTemp->s != "(instance") {cout<<"ERROR : Can not find instance...\n"; return;}
	if(insTemp->next->s == "(rename") SinsTemp = insTemp->next->next->s; else SinsTemp = insTemp->next->s;
	node* nodeTemp = sFind2("(viewRef", insTemp);
	if(!nodeTemp) {
		cout<<"\nERROR : can not find /reference/\n\n";
		return;
	}
	stack<string> S1;
	int open = 1;
	int close = 0;
	S1.push(nodeTemp->s);
	nodeTemp = nodeTemp->next;
	while (open != close){
		S1.push(nodeTemp->s);
		if(nodeTemp->s.find("(") == 0) open++;
		if(nodeTemp->s.find(")") == 0) close++;
		nodeTemp = nodeTemp->next;
	}
	stack<string> ST1, ST2, ST3;
	while(!S1.empty()){
		string temp = S1.top();
		S1.pop();
		ST1.push(temp);
	}
	node* Lib = NULL;
	string strLib = "";
	string strCell = "";
	while (ST1.size() != 1){
		while(ST1.top() != ")"){
			string temp = ST1.top();
			ST2.push(temp);
			ST1.pop();
		}
		int argCnt = 0;
		while(ST2.top().find("(") != 0){
			ST3.push(ST2.top());
			argCnt++;
			ST2.pop();
		}
		if(ST2.top() == "(libraryRef"){
			Lib = libraryFind(ST3.top());
			strLib = ST3.top();
			ST2.pop();
			ST1.pop();
			while(ST3.size()) ST3.pop();
		}
		if(ST2.top() == "(cellRef"){
			node* Cell = NULL;
			string total_t = total;
			total_t.insert(total_t.length(), SinsTemp);
			total_t.insert(total_t.length(), "/");
			strCell = ST3.top();
			Cell = cellRefHn(total_t, strLib, strCell, 1);
			ST2.pop();
			ST1.pop();
			while(ST3.size()) ST3.pop();
		}
	}
}

//-----------------------------------------CELL REF HANDELLER 2----------------------------------
node* EDIF_manager::cellRefHn(string total, string Lib, string cell, int top){
	node* nodeTemp = cellFind(Lib, cell);
	int open = 1;
	int close = 0;
	int instFlag = 0;
	nodeTemp = nodeTemp->next;
	portClass* ports = NULL; portClass* currentPort = NULL;
	while (open != close){
		if(nodeTemp->s == "(port"){
			if(nodeTemp->next->s =="(rename"){
				if(!ports){
					ports = new portClass;
					ports->portName = nodeTemp->next->next->s;
					ports->direction = sFind("(direction", nodeTemp)->next->s;
					ports->array = 1;
					ports->next = NULL;
					currentPort = ports;
				}else{
					currentPort->next = new portClass;
					currentPort->next->portName = nodeTemp->next->next->s;
					ports->array = 1;
					currentPort->next->direction = sFind("(direction", nodeTemp)->next->s;
					currentPort->next->next = NULL;
					currentPort = currentPort->next;
				}
			} else if(nodeTemp->next->s =="(array"){
				if(nodeTemp->next->next->s =="(rename"){
					if(!ports){

						ports = new portClass;
						ports->portName = nodeTemp->next->next->next->s;
						ports->direction = sFind("(direction", nodeTemp)->next->s;
						ports->array = str2int(nodeTemp->next->next->next->next->next->next->s);
						ports->next = NULL;
						currentPort = ports;
					}else{
						currentPort->next = new portClass;
						currentPort->next->portName = nodeTemp->next->next->next->s;
						ports->array = str2int(nodeTemp->next->next->next->next->next->next->s);
						currentPort->next->direction = sFind("(direction", nodeTemp)->next->s;
						currentPort->next->next = NULL;
						currentPort = currentPort->next;
					}
				
				} else {
					
					if(!ports){

						ports = new portClass;
						ports->portName = nodeTemp->next->next->s;
						ports->direction = sFind("(direction", nodeTemp)->next->s;
						ports->array = str2int(nodeTemp->next->next->next->s);
						ports->next = NULL;
						currentPort = ports;
					}else{
						currentPort->next = new portClass;
						currentPort->next->portName = nodeTemp->next->next->s;
						ports->array = str2int(nodeTemp->next->next->next->s);
						currentPort->next->direction = sFind("(direction", nodeTemp)->next->s;
						currentPort->next->next = NULL;
						currentPort = currentPort->next;
					}
				}
			}else{
				if(!ports){

					ports = new portClass;
					ports->portName = nodeTemp->next->s;
					ports->direction = sFind("(direction", nodeTemp)->next->s;
					ports->array = 1;
					ports->next = NULL;
					currentPort = ports;
				}else{
					currentPort->next = new portClass;
					currentPort->next->portName = nodeTemp->next->s;
					ports->array = 1;
					currentPort->next->direction = sFind("(direction", nodeTemp)->next->s;
					currentPort->next->next = NULL;
					currentPort = currentPort->next;
				}
			}
		}
		
		//-----------------------------------------
		if(nodeTemp->s == "(instance"){
			instanceHn(total, nodeTemp, top);
			instFlag = 1;
		}
		
		if(nodeTemp->s == "(net"){

			/*if(nodeTemp->next->s == "y_7_OBUF"){
				int l;
				l++;
			}*/

			netHn (total, cell, Lib,  nodeTemp, top, "");
			if(!top && ioStringFlag){
				ioStringFlag = 0;
				setIO(currentNet.giveCurNet());
				currentNet.replaceCurNet(ioString);
				currentNet.decNet();
			}
		}
		if(nodeTemp->s.find("(") == 0) open++;
		if(nodeTemp->s.find(")") == 0) close++;
		nodeTemp = nodeTemp->next;
	}
	return 0;
}


//---------Design Handller-----------------------
void EDIF_manager::designHn (string total){

	cout<<"\n**************************************************\n";
	cout<<"\nAnalyzing EDIF file, Please wait...\n";
	node* nodeTemp = sFind("(design");
	if(!nodeTemp) {
		cout<<"\nERROR : can not find /design/\n\n";
		return;
	}
	stack<string> S1;
	int open = 1;
	int close = 0;
	S1.push(nodeTemp->s);
	nodeTemp = nodeTemp->next;
	while (open != close){
		S1.push(nodeTemp->s);
		if(nodeTemp->s.find("(") == 0) open++;
		if(nodeTemp->s.find(")") == 0) close++;
		nodeTemp = nodeTemp->next;
	}
	stack<string> ST1, ST2, ST3;
	while(!S1.empty()){
		string temp = S1.top();
		S1.pop();
		ST1.push(temp);
	}
	node* Lib = NULL;
	string strLib = "";
	string strCell = "";
	while (ST1.size() != 1){
		while(ST1.top() != ")"){
			string temp = ST1.top();
			ST2.push(temp);
			ST1.pop();
		}
		int argCnt = 0;
		while(ST2.top().find("(") != 0){
			ST3.push(ST2.top());
			argCnt++;
			ST2.pop();
		}
		if(ST2.top() == "(libraryRef"){

			Lib = libraryFind(ST3.top());
			strLib = ST3.top();
			ST2.pop();
			ST1.pop();
			while(ST3.size()) ST3.pop();
		}
		if(ST2.top() == "(cellRef"){
			node* Cell = NULL;
			strCell = ST3.top();
			cellRefHn(total, strLib, strCell, 0);
			ST2.pop();
			ST1.pop();
			while(ST3.size()) ST3.pop();
			return;
		}
	}
}
//-----------------------Read input file------------------------------------
void EDIF_manager::readNDFfile(){

	
	char chartemp1[512];
	for(int i = 0; i< ModuleName.length(); i++)
		chartemp1[i] = ModuleName[i];
	chartemp1[ModuleName.length()] = '.';
	chartemp1[ModuleName.length()+1] = 'n';
	chartemp1[ModuleName.length()+2] = 'd';
	chartemp1[ModuleName.length()+3] = 'f';
	chartemp1[ModuleName.length()+4] = 0;
	
	ifstream IFLE(chartemp1);
	node* currentNode = NULL;
	while(!IFLE.eof()){
		char charTemp[100000];
		IFLE.getline(charTemp, 100000);
		string str_temp = charTemp;
		while (str_temp.length()){
			if(str_temp.find(" ") == -1 && str_temp.find("(") == -1 && str_temp.find(")") == -1){
				if(!totalFile){
					totalFile = new node;
					totalFile->s = str_temp;
					totalFile->next = NULL;
					currentNode = totalFile;
				}else{
					currentNode->next = new node;
					currentNode->next->s = str_temp;
					currentNode->next->next = NULL;
					currentNode = currentNode->next;
				}
				break;
			}
			if((minNum(str_temp.find(" "), str_temp.find("("), str_temp.find(")")) == str_temp.find(" ")) && str_temp.find(" ") != -1){
				//space
				if(!str_temp.find(" "))
					str_temp.erase(0, str_temp.find(" ")+1);
				else{
					if(!totalFile){
						totalFile = new node;
						totalFile->s = str_temp.substr(0, str_temp.find(" "));
						str_temp.erase(0, str_temp.find(" "));
						totalFile->next = NULL;
						currentNode = totalFile;
					}else{
						currentNode->next = new node;
						currentNode->next->s = str_temp.substr(0, str_temp.find(" "));
						str_temp.erase(0, str_temp.find(" "));
						currentNode->next->next = NULL;
						currentNode = currentNode->next;
					}
				}
			}
			if((minNum(str_temp.find(" "), str_temp.find("("), str_temp.find(")")) == str_temp.find("(")) && str_temp.find("(") != -1){
				//"("
				if(!totalFile){
					totalFile = new node;
					str_temp.erase(0, str_temp.find("(")+1);
					totalFile->s = str_temp.substr(0, minNum(str_temp.find(" "), str_temp.find("("), str_temp.find(")")));
					totalFile->s.insert(0, "(");
					str_temp.erase(0, minNum(str_temp.find(" "), str_temp.find("("), str_temp.find(")")));
					totalFile->next = NULL;
					currentNode = totalFile;
				}else{
					currentNode->next = new node;
					str_temp.erase(0, str_temp.find("(")+1);
					currentNode->next->s = str_temp.substr(0, minNum(str_temp.find(" "), str_temp.find("("), str_temp.find(")")));
					currentNode->next->s.insert(0, "(");
					str_temp.erase(0, minNum(str_temp.find(" "), str_temp.find("("), str_temp.find(")")));
					currentNode->next->next = NULL;
					currentNode = currentNode->next;
				}
			}
			if((minNum(str_temp.find(" "), str_temp.find("("), str_temp.find(")")) == str_temp.find(")")) && str_temp.find(")") != -1){
				//")"
				if(!str_temp.find(")")){
					if(!totalFile){
						totalFile = new node;
						totalFile->s = ")";
						str_temp = str_temp.erase(0, 1);
						totalFile->next = NULL;
						currentNode = totalFile;
					}else{
						currentNode->next = new node;
						currentNode->next->s = ")";
						str_temp = str_temp.erase(0, 1);
						currentNode->next->next = NULL;
						currentNode = currentNode->next;
					}
				}
				else{
					if(!totalFile){
						totalFile = new node;
						totalFile->s = str_temp.substr(0, str_temp.find(")"));
						str_temp.erase(0, str_temp.find(")"));
						totalFile->next = NULL;
						currentNode = totalFile;
					}else{
						currentNode->next = new node;
						currentNode->next->s = str_temp.substr(0, str_temp.find(")"));
						str_temp.erase(0, str_temp.find(")"));
						currentNode->next->next = NULL;
						currentNode = currentNode->next;
					}
					if(!totalFile){
						totalFile = new node;
						totalFile->s = ")";
						str_temp = str_temp.erase(0, 1);
						totalFile->next = NULL;
						currentNode = totalFile;
					}else{
						currentNode->next = new node;
						currentNode->next->s = ")";
						str_temp = str_temp.erase(0, 1);
						currentNode->next->next = NULL;
						currentNode = currentNode->next;
					}
				}
			}
		}
	}
}
//--------------------------------------------------
void EDIF_manager::print1(){

	node* nodeTemp = totalFile;

	ofstream FILE("Test1");
	while(nodeTemp){

		FILE<<nodeTemp->s<<endl;
		nodeTemp = nodeTemp->next;
	}

	return;
}
//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------
/*struct GateStruct
{
	int Type; //0 buffer, 1 AND, 2 OR, 3 XOR, 4 FDCPE, 5, FTCPE, 6 FDDCPE, 7 FTDCPE, 8 LDCP, 9 Fanout//---------GATE TYPE-------------
	int brachNumber;
	string INS;
	string Output;
	node* input;
	bool invert; //0 does not invert allowed
	GateStruct* next;
};*/

class wireAnalyzer: public EDIF_manager {
protected:
	node* wiresHead;
	node* currentWire;
	node* nodeHead;
	node* currentNode;
	string GlobalReset;
	string serialInwire;
	string NbarT;



	long int wireCnt;
	long int insCnt;

	long int FDCPEcnt;
	long int FDDCPEcnt;
	long int LDCPcnt;
	long int FTDCPEcnt;
	long int FTCPEcnt;

	void error_type4(long int rptLineCnt);
	void error_type5(long int);
	void error_type6(long int);

	void information_type2(string, string);
	void information_type3(string);
	void information_type4(string);
	void information_type5(string);

	void warnning_type1(long int);


	int wireFinder(string, long int);
	int wireFinder4reg(string, long int);
	int checkingWire(string, node*);
	void replace(string, string, node*);
	void AddingGlobalReset();
	void wireMakerForverilogSafe();
	void wireMakerForverilogNotSafe();
	int minNum(int, int,int, int, int, int, int, int, int);
	int checkingWire(string, long int);


public:
	void wireMakerForverilog();
	void wireMakerFornetlist();
	wireAnalyzer(){nodeHead = NULL; wireCnt = 0; insCnt = 0; currentNode = NULL; wiresHead = NULL; currentWire = NULL;}
	int Pre_CommandReader();
	void printVerilogEq();
};

void wireAnalyzer::warnning_type1(long int eqNumber){
	cout<<"\nWarnning type1";
	cout<<"\nIn line number "<<eqNumber<<" duplicated gate is detected.\n";
	WAR<<"\nWarnning type1";
	WAR<<"\nIn line number "<<eqNumber<<" duplicated gate is detected.\n";
	return;
}

void wireAnalyzer::information_type2(string newWire, string oldWire){

	cout<<"wire "<<oldWire<<" replaced by wire "<<newWire<<endl;
	INF<<"wire "<<oldWire<<" replaced by wire "<<newWire<<endl;
	return;
}

void wireAnalyzer::information_type3(string newWire){

	cout<<"we detected wire "<<newWire<<" in your design.\n";
	cout<<"To make all the wire's neme unique we ignore this wire.\n";
	INF<<"we detected wire "<<newWire<<" in your design.\n";
	INF<<"To make all the wire's neme unique we ignore this wire.\n";
	return;
}

void wireAnalyzer::information_type4(string input){

	cout<<"we detected output "<<input<<" in your design.\n";
	INF<<"we detected output "<<input<<" in your design.\n";
	return;
}

void wireAnalyzer::information_type5(string output){

	cout<<"we detected input "<<output<<" in your design.\n";
	INF<<"we detected input "<<output<<" in your design.\n";
	return;
}


void wireAnalyzer::error_type4(long int rptLineCnt){
	cout<<"\nThere is an error in your EDIF file.\n";
	cout<<"We do not detect any equation.\n";
	ERR<<"\nThere is an error in your EDIF file.\n";
	ERR<<"We do not detect any equation.\n";
	return;
}

void wireAnalyzer::error_type5(long int eqLineCnt){
	cout<<"\nThere is an error in your equation file.\n";
	cout<<"We do not detect any equation in line "<<eqLineCnt<<"  .\n";
	ERR<<"\nThere is an error in your equation file.\n";
	ERR<<"We do not detect any equation in line "<<eqLineCnt<<".\n ";
	return;
}

void wireAnalyzer::error_type6(long int eqLineCnt){
	cout<<"\nThere is an error in your equation file.\n";
	cout<<"We do not detect any key words in line "<<eqLineCnt<<".\n ";
	ERR<<"\nThere is an error in your equation file.\n";
	ERR<<"We do not detect any key words in line "<<eqLineCnt<<".\n";
	return;
}



int wireAnalyzer::checkingWire(string s, node* head){
	node* Temp = head;
	while(Temp){
		if(s == Temp->s){
			return 1;
		}
		Temp=Temp->next;
	}

		return 0;
}

void wireAnalyzer::wireMakerForverilogNotSafe(){

	AddingGlobalReset();
	string newWire = "MHMH_";
	newWire.insert(newWire.length(), int2str(wireCnt));

	string newIns = "INS_";
	newIns.insert(newIns.length(), int2str(insCnt));


	node* Temp = nodeHead;
	while(Temp){
		if(Temp->s != "=" && Temp->s != "||" && Temp->s != "&&" && Temp->s != "^" && Temp->s != "(" && Temp->s != ")" && Temp->s != "?" && Temp->s != "!" && Temp->s != ";" && Temp->s != "," && Temp->s != "FDCPE"  && Temp->s != "FTCPE"  && Temp->s != "FDDCPE"  && Temp->s != "FTDCPE"  && Temp->s != "LDCP"   && Temp->s != "1'b0"   && Temp->s != "1'b1"   && Temp->s != "1'bZ" && Temp->s != GlobalReset){
			if(!checkingWire(Temp->s, EX_Inputs2)){
				if(!checkingWire(Temp->s, EX_Outputs2)){
					if(!checkingWire(Temp->s, wiresHead)){
						int replaceFlag = 0;
						while(!replaceFlag){
							wireCnt++;
							newWire = "MHMH_";
							newWire.insert(newWire.length(), int2str(wireCnt));
							//if(!checkingWire(newWire, nodeHead) && !checkingWire(newWire, EX_Inputs2) && !checkingWire(newWire, EX_Outputs2) && !checkingWire(newWire, Inputs) && !checkingWire(newWire, Outputs)&& !checkingWire(newWire, EX_Inputs_1) && !checkingWire(newWire, EX_Outputs_1)){
							information_type2(newWire, Temp->s);
							replace(newWire, Temp->s, Temp);
							if(!currentWire){
								currentWire = new node;
								currentWire->s = newWire;
								currentWire->next = NULL;
								wiresHead = currentWire;
							}else{
								currentWire->next = new node;
								currentWire->next->s = newWire;
								currentWire->next->next = NULL;
								currentWire = currentWire->next;
							}

							replaceFlag = 1;
							//} else{
							//	information_type3(newWire);
							//	replaceFlag = 0;
							//}
						}
					}
				}else{
					information_type4(Temp->s);//Output information
				}
			}
			else{
				information_type5(Temp->s);//input Information
			}
		}

		if(Temp->s == "FDCPE"  || Temp->s == "FTCPE"  || Temp->s == "FDDCPE"  || Temp->s == "FTDCPE"  || Temp->s == "LDCP"){
			int replaceFlag = 0;
			while(!replaceFlag){

				insCnt++;
				newIns = "INS_";
				newIns.insert(newIns.length(), int2str(insCnt));
				if(!checkingWire(newIns, nodeHead) && !checkingWire(newIns, EX_Inputs2) && !checkingWire(newIns, EX_Outputs2)  && !checkingWire(newIns, EX_Inputs_1) && !checkingWire(newIns, EX_Outputs_1) && !checkingWire(newIns, Inputs) && !checkingWire(newIns, Outputs)){
					information_type2(newWire, Temp->s);
					Temp = Temp->next;
					Temp->s = newIns;
					replaceFlag = 1;
				}else{
					information_type3(newWire);
					replaceFlag = 0;
				}
			}
		}
		Temp = Temp->next;
	}
	return;
}


void wireAnalyzer::wireMakerForverilogSafe(){

	AddingGlobalReset();
	string newWire = "iwire_";
	newWire.insert(newWire.length(), int2str(wireCnt));

	string newIns = "INS_";
	newIns.insert(newIns.length(), int2str(insCnt));


	node* Temp = nodeHead;
	while(Temp){
		if(Temp->s != "=" && Temp->s != "||" && Temp->s != "&&" && Temp->s != "^" && Temp->s != "(" && Temp->s != ")" && Temp->s != "?" && Temp->s != "!" && Temp->s != ";" && Temp->s != "," && Temp->s != "FDCPE"  && Temp->s != "FTCPE"  && Temp->s != "FDDCPE"  && Temp->s != "FTDCPE"  && Temp->s != "LDCP"   && Temp->s != "1'b0"   && Temp->s != "1'b1"   && Temp->s != "1'bZ" && Temp->s != GlobalReset && Temp->s != ":" && Temp->s != "?"){
			if(!checkingWire(Temp->s, EX_Inputs2)){
				if(!checkingWire(Temp->s, EX_Outputs2)){
					if(!checkingWire(Temp->s, wiresHead)){
						int replaceFlag = 0;
						while(!replaceFlag){
							wireCnt++;
							newWire = "iwire_";
							newWire.insert(newWire.length(), int2str(wireCnt));
							if(!checkingWire(newWire, nodeHead) && !checkingWire(newWire, EX_Inputs2) && !checkingWire(newWire, EX_Outputs2) && !checkingWire(newWire, Inputs) && !checkingWire(newWire, Outputs)&& !checkingWire(newWire, EX_Inputs_1) && !checkingWire(newWire, EX_Outputs_1)){
								information_type2(newWire, Temp->s);
								replace(newWire, Temp->s, Temp);
								if(!currentWire){
									currentWire = new node;
									currentWire->s = newWire;
									currentWire->next = NULL;
									wiresHead = currentWire;
								}else{
									currentWire->next = new node;
									currentWire->next->s = newWire;
									currentWire->next->next = NULL;
									currentWire = currentWire->next;
								}

								replaceFlag = 1;
							} else{
								information_type3(newWire);
								replaceFlag = 0;
							}
						}
					}
				}else{
					information_type4(Temp->s);//Output information
				}
			}
			else{
				information_type5(Temp->s);//input Information
			}
		}

		if(Temp->s == "FDCPE"  || Temp->s == "FTCPE"  || Temp->s == "FDDCPE"  || Temp->s == "FTDCPE"  || Temp->s == "LDCP"){
			int replaceFlag = 0;
			while(!replaceFlag){

				insCnt++;
				newIns = "INS_";
				newIns.insert(newIns.length(), int2str(insCnt));
				if(!checkingWire(newIns, nodeHead) && !checkingWire(newIns, EX_Inputs2) && !checkingWire(newIns, EX_Outputs2)  && !checkingWire(newIns, EX_Inputs_1) && !checkingWire(newIns, EX_Outputs_1) && !checkingWire(newIns, Inputs) && !checkingWire(newIns, Outputs)){
					information_type2(newWire, Temp->s);
					Temp = Temp->next;
					Temp->s = newIns;
					replaceFlag = 1;
				}else{
					information_type3(newWire);
					replaceFlag = 0;
				}
			}
		}
		Temp = Temp->next;
	}
	return;
}

void wireAnalyzer::wireMakerForverilog(){

	wireMakerForverilogSafe();

	//cout<<endl<<"For pre analyzing the wires of the design there are two methods: 1. Safe 2. Unsafe."<<endl;
	//cout<<"In safe method, which is slow, we analyze the entire design for wire replacement process."<<endl;
	//cout<<"In unsafe method you should be aware that in your RPT file there are not any wires' name the same as our generated wires."<<endl;
	//cout<<"for using unsafe method be sure that the wires' names in the design are not started with 'MHMH'."<<endl;
	//cout<<"Which process do you suggest?(S/U)";
	//bool flag = 0;
	//while(!flag){
	//	char Temp;
	//	cin>>Temp;
	//	switch(Temp){
	//	case 'S': {wireMakerForverilogSafe(); flag = 1; break;}
	//	case 'U': {wireMakerForverilogNotSafe(); flag = 1;break;}
	//	case 's': {wireMakerForverilogSafe(); flag = 1;break;}
	//	case 'u': {wireMakerForverilogNotSafe(); flag = 1;break;}
	//	default: {cout<<endl<<"{Please make your decision. We are waiting for you...\nSafe or Unsafe (S/U)?";break;}
	//	}
	//}


	return;
}


void wireAnalyzer::AddingGlobalReset(){
	GlobalReset = "global_reset";
	long int counter = 0;

	int replaceFlag = 0;
	while(!replaceFlag){
		if(!checkingWire(GlobalReset, nodeHead) && !checkingWire(GlobalReset, EX_Inputs2) && !checkingWire(GlobalReset, EX_Outputs2) && !checkingWire(GlobalReset, Inputs) && !checkingWire(GlobalReset, Outputs)&& !checkingWire(GlobalReset, EX_Inputs_1) && !checkingWire(GlobalReset, EX_Outputs_1)) replaceFlag = 1;
		else{
			GlobalReset.insert(GlobalReset.length(), int2str(counter));
			counter++;
		}
	}

	cout<<endl<<"The global reset is "<<GlobalReset<<".\n";

	serialInwire = "Si";
	NbarT = "NbarT";
	counter = 0;
	replaceFlag = 0;

	while(!replaceFlag){
		if(!checkingWire(serialInwire, nodeHead) && !checkingWire(serialInwire, EX_Inputs2) && !checkingWire(serialInwire, EX_Outputs2) && !checkingWire(serialInwire, Inputs) && !checkingWire(serialInwire, Outputs)&& !checkingWire(serialInwire, EX_Inputs_1) && !checkingWire(serialInwire, EX_Outputs_1)) replaceFlag = 1;
		else{
			serialInwire.insert(serialInwire.length(), int2str(counter));
			counter++;
		}
	}

	counter = 0;
	replaceFlag = 0;

	while(!replaceFlag){
		if(!checkingWire(NbarT, nodeHead) && !checkingWire(NbarT, EX_Inputs2) && !checkingWire(NbarT, EX_Outputs2) && !checkingWire(NbarT, Inputs) && !checkingWire(NbarT, Outputs)&& !checkingWire(NbarT, EX_Inputs_1) && !checkingWire(NbarT, EX_Outputs_1)) replaceFlag = 1;
		else{
			NbarT.insert(NbarT.length(), int2str(counter));
			counter++;
		}
	}

	cout<<endl<<"The name of serial input wire for scan chain is "<<serialInwire<<".\n";
	cout<<endl<<"The wire for selecting test mode is "<<NbarT<<".\n";
	INF<<endl<<"The name of serial input wire for scan chain is "<<serialInwire<<".\n";
	INF<<endl<<"The wire for selecting test mode is "<<NbarT<<".\n";


	node* Temp = nodeHead;
	while(Temp){
		if(Temp->s == "FDDCPE"  || Temp->s == "FTDCPE"  || Temp->s == "LDCP"){
			node* nodeTemp = new node;
			nodeTemp->s = ",";
			nodeTemp->next = new node;
			nodeTemp->next->s=GlobalReset;
			while(Temp->next->s != ")"){
				Temp = Temp->next;
			}
			nodeTemp->next->next=Temp->next;
			Temp->next=nodeTemp;
		}
		else if(Temp->s == "FDCPE"  || Temp->s == "FTCPE"){
			node* nodeTemp = new node;
			nodeTemp->s = ",";
			nodeTemp->next = new node;
			nodeTemp->next->s=GlobalReset;
			while(Temp->next->s != ")"){
				Temp = Temp->next;
			}
			nodeTemp->next->next=Temp->next;
			Temp->next=nodeTemp;
		}
		Temp = Temp->next;
	}
	return;
}


void wireAnalyzer::wireMakerFornetlist(){
	string newWire = "iwire_";
	newWire.insert(newWire.length(), int2str(wireCnt));

	node* Temp = nodeHead;
	while(Temp){
		if(Temp->s != "=" && Temp->s != "||" && Temp->s != "&&" && Temp->s != "^" && Temp->s != "(" && Temp->s != ")" && Temp->s != "?" && Temp->s != "!" && Temp->s != ";" && Temp->s != "," && Temp->s != "FDCPE"  && Temp->s != "FTCPE"  && Temp->s != "FDDCPE"  && Temp->s != "FTDCPE"  && Temp->s != "LDCP"   && Temp->s != "1'b0"   && Temp->s != "1'b1"   && Temp->s != "1'bZ"){
			if(!checkingWire(Temp->s, EX_Inputs2)){
				if(!checkingWire(Temp->s, EX_Outputs2)){
					if(!checkingWire(Temp->s, wiresHead)){
						int replaceFlag = 0;
						while(!replaceFlag){
							wireCnt++;
							newWire = "iwire_";
							newWire.insert(newWire.length(), int2str(wireCnt));
							if(!checkingWire(newWire, nodeHead) && !checkingWire(newWire, EX_Inputs2) && !checkingWire(newWire, EX_Outputs2)){
								information_type2(newWire, Temp->s);
								replace(newWire, Temp->s, Temp);
								if(!currentWire){
									currentWire = new node;
									currentWire->s = newWire;
									currentWire->next = NULL;
									wiresHead = currentWire;
								}else{
									currentWire->next = new node;
									currentWire->next->s = newWire;
									currentWire->next->next = NULL;
									currentWire = currentWire->next;
								}

								replaceFlag = 1;
							} else{
								information_type3(newWire);
								replaceFlag = 0;
							}
						}
					}
				}else{
					information_type4(Temp->s);//Output information
				}
			}
			else{
				information_type5(Temp->s);//input Information
			}
		}
		Temp = Temp->next;
	}
	return;
}




void wireAnalyzer::replace(string newWire, string oldWire, node* Temp){

	while(Temp){
		if(oldWire == Temp->s){
			Temp->s = newWire;
		}
		Temp=Temp->next;
	}
	return;
}




int wireAnalyzer::Pre_CommandReader(){

	long int eqCounter = 0;
	FDCPEcnt = 0;
	FDDCPEcnt = 0;
	LDCPcnt = 0;
	FTDCPEcnt = 0;
	FTCPEcnt = 0;

	char rptFilet[30];
	for(int i=0; i<30; i++) rptFilet[i] = rptFile[i];
	ifstream Input_rpt_File(rptFilet);

	if(!Input_rpt_File){
		cout<<"\nWe do not find any EDIF file matching your design name.";
		cout<<"\nPress any key to end...\n";
		getch();
		exit(0);
		
	}


	char eqFilet[30];
	for(int i=0; i<30; i++) eqFilet[i] = equationFile[i];
	ofstream eqFile(eqFilet);
	long int rptlineCnt=0;
	char temp[10000];
	string str_temp;
	string commandline = "";

	while(str_temp.find("Equations") == -1){
		if(Input_rpt_File.eof()){error_type4(rptlineCnt); exit(0); return -1;}
		Input_rpt_File.getline(temp, 10000);
		rptlineCnt++;
		str_temp = temp;
	}


	while(!Input_rpt_File.eof()){

		Input_rpt_File.getline(temp, 10000);
		rptlineCnt++;
		str_temp = temp;
		commandline = "";
		

		if(str_temp.find("assign ") == 0 || str_temp.find("assign\t") == 0){
			str_temp.erase(0, 7);
			commandline.insert(commandline.length(), str_temp);
			while(str_temp.find(";") != str_temp.length()-1){
				Input_rpt_File.getline(temp, 10000);
				rptlineCnt++;
				str_temp = temp;
				commandline.insert(commandline.length(), str_temp);
			}
			eqFile<<commandline<<endl;


			eqCounter++;

			wireFinder(commandline, eqCounter);

		}

		if(str_temp.find("FDCPE ") == 0 || str_temp.find("FDCPE\t") == 0){
			commandline.insert(commandline.length(), str_temp);
			while(str_temp.find(";") != str_temp.length()-1){
				Input_rpt_File.getline(temp, 10000);
				rptlineCnt++;
				str_temp = temp;
				commandline.insert(commandline.length(), str_temp);
			}
			eqFile<<commandline<<endl;
			FDCPEcnt++;
			wireFinder4reg(commandline, eqCounter);

		}

		if(str_temp.find("FDDCPE ") == 0 || str_temp.find("FDDCPE\t") == 0){
			commandline.insert(commandline.length(), str_temp);
			while(str_temp.find(";") != str_temp.length()-1){
				Input_rpt_File.getline(temp, 10000);
				rptlineCnt++;
				str_temp = temp;
				commandline.insert(commandline.length(), str_temp);
			}
			eqFile<<commandline<<endl;
			FDDCPEcnt++;
			wireFinder4reg(commandline, eqCounter);

		}

		if(str_temp.find("FTCPE ") == 0 || str_temp.find("FTCPE\t") == 0){
			commandline.insert(commandline.length(), str_temp);
			while(str_temp.find(";") != str_temp.length()-1){
				Input_rpt_File.getline(temp, 10000);
				rptlineCnt++;
				str_temp = temp;
				commandline.insert(commandline.length(), str_temp);
			}
			eqFile<<commandline<<endl;
			FTCPEcnt++;
			wireFinder4reg(commandline, eqCounter);

		}

		if(str_temp.find("FTDCPE ") == 0 || str_temp.find("FTDCPE\t") == 0){
			commandline.insert(commandline.length(), str_temp);
			while(str_temp.find(";") != str_temp.length()-1){
				Input_rpt_File.getline(temp, 10000);
				rptlineCnt++;
				str_temp = temp;
				commandline.insert(commandline.length(), str_temp);
			}
			eqFile<<commandline<<endl;
			FTDCPEcnt++;
			wireFinder4reg(commandline, eqCounter);

		}

		if(str_temp.find("LDCP ") == 0 || str_temp.find("LDCP\t") == 0){
			commandline.insert(commandline.length(), str_temp);
			while(str_temp.find(";") != str_temp.length()-1){
				Input_rpt_File.getline(temp, 10000);
				rptlineCnt++;
				str_temp = temp;
				commandline.insert(commandline.length(), str_temp);
			}
			eqFile<<commandline<<endl;
			LDCPcnt++;
			wireFinder4reg(commandline, eqCounter);

		}

	}

	INF<<"\n\n************************************************************\n";
	INF<<"****                                                    ****\n";
	INF<<"****             Netlist Generator report file          ****\n";
	//INF<<"****                  CAD RESERAC GROUP                 ****\n";
	//INF<<"****                 UNIVERSITY OF TEHRAN               ****\n";
	INF<<"****                                                    ****\n";
	INF<<"************************************************************\n\n";

	INF<<"The process of extracting equations is completed.\n";
	INF<<"This design has "<<eqCounter<<" non extracted equations.\n";
	INF<<"This design has "<<FDCPEcnt<<" FDCPE(s) storage memory.\n";
	INF<<"This design has "<<FDDCPEcnt<<" FDDCPE(s) storage memory."<<endl;
	INF<<"This design has "<<FTCPEcnt<<" FTCPE(s) storage memory."<<endl;
	INF<<"This design has "<<FTDCPEcnt<<" FTDCPE(s) storage memory."<<endl;
	INF<<"This design has "<<LDCPcnt<<" LDCP(s) storage memory.\n";
	INF<<"you can check the equations in "<<equationFile<<" File.\n\n\n";
	INF<<"************************************************************\n\n";

	cout<<"\n\n************************************************************\n";
	cout<<"****                                                    ****\n";
	cout<<"****             Netlist Generator report file          ****\n";
	//cout<<"****                  CAD RESEARCH GROUP                ****\n";
	//cout<<"****                 UNIVERSITY OF TEHRAN               ****\n";
	cout<<"****                                                    ****\n";
	cout<<"************************************************************\n\n";




	cout<<"\n\n************************************************************\n";
	cout<<"The process of extracting equations is completed.\n";
	cout<<"This design has "<<eqCounter<<" non extracted equations.\n";
	cout<<"This design has "<<FDCPEcnt<<" FDCPE(s) storage memory.\n";
	cout<<"This design has "<<FDDCPEcnt<<" FDDCPE(s) storage memory."<<endl;
	cout<<"This design has "<<FTCPEcnt<<" FTCPE(s) storage memory."<<endl;
	cout<<"This design has "<<FTDCPEcnt<<" FTDCPE(s) storage memory."<<endl;
	cout<<"This design has "<<LDCPcnt<<" LDCP(s) storage memory.\n";
	cout<<"you can check the equations in "<<equationFile<<" File.\n";


	eqFile<<eqCounter<<" eguations."<<endl;
	eqFile<<FDCPEcnt<<" FDCPE(s)."<<endl;
	eqFile<<FDDCPEcnt<<" FDDCPE(s)."<<endl;
	eqFile<<FTCPEcnt<<" FTCPE(s)."<<endl;
	eqFile<<FTDCPEcnt<<" FTDCPE(s)."<<endl;
	eqFile<<LDCPcnt<<" LDCP(s).";
	eqFile.close();

	return 0;
}

//returns minimum number
int wireAnalyzer::minNum(int x1, int x2,int  x3, int x4, int x5, int x6, int x7, int x8, int x9){

	int temp = -1;
	if( x1 != -1 ){
		temp = x1;
	}else
	{
		if (x2 !=-1){
			temp = x2;
		}else{
			if (x3 != -1){
				temp = x3;
			}else{
				if (x4 != -1){
					temp = x4;
				}else{
					if (x5 != -1){
						temp = x5;
					}else{
						if (x6 != -1){
							temp = x6;
						}else{
							if (x7 != -1){
								temp = x7;
							}else{
								if (x8 != -1){
									temp = x8;
								}else{
									temp = x9;
								}
							}
						}
					}
				}
			}
		}
	}

	if (x2 < temp && x2 != -1) temp = x2;
	if (x3 < temp && x3 != -1) temp = x3;
	if (x4 < temp && x4 != -1) temp = x4;
	if (x5 < temp && x5 != -1) temp = x5;
	if (x6 < temp && x6 != -1) temp = x6;
	if (x7 < temp && x7 != -1) temp = x7;
	if (x8 < temp && x8 != -1) temp = x8;
	if (x9 < temp && x9 != -1) temp = x9;

	return temp;
}



//insert the input file into array
int wireAnalyzer::wireFinder(string commandline, long int eqNumber)
{


	//QHandler();*************************handling ? option****************************
	int cmdCounter = 0;

	if(commandline.size() == 0) {error_type5(eqNumber); exit(0); return -1;}

	commandline = erase_space(commandline);
	int i= commandline.find("=");

	string Output_temp = commandline.substr(0, i);
	commandline.erase(0, i+1);

	stack<string> Stack_temp;
	stack<string> Stack1;


	int klll = Stack1.size();
	while( commandline.substr(0, 1) != ";"){
		if((commandline.substr(0, 1) == "!") || (commandline.substr(0, 1) == "(") || (commandline.substr(0, 1) == ")") || (commandline.substr(0, 1) == "^") || (commandline.substr(0, 1) == "?") || (commandline.substr(0, 1) == ":")){
			Stack1.push(commandline.substr(0, 1));
			commandline.erase(0, 1);
		} else{
			if((commandline.substr(0, 2) == "&&") || (commandline.substr(0, 2) == "||")){
				Stack1.push(commandline.substr(0, 2));
				commandline.erase(0, 2);
			}else{
				if((commandline.substr(0, 4) == "1'b0") || (commandline.substr(0, 4) == "1'b1") || (commandline.substr(0, 4) == "1'bZ")){
				Stack1.push(commandline.substr(0, 4));
				commandline.erase(0, 4);
				} else{
					int x1 = commandline.find("(");
					int x2 = commandline.find(")");
					int x3 = commandline.find("!");
					int x4 = commandline.find("&&");
					int x5 = commandline.find("||");
					int x6 = commandline.find("^");
					int x7 = commandline.find("?");
					int x8 = commandline.find(":");
					int x9 = commandline.find(";");
					if(minNum(x1, x2, x3, x4, x5, x6, x7, x8, x9) == -1) {error_type6(eqNumber); exit(0); return -1;}
					string strTemp = commandline.substr(0, minNum(x1, x2, x3, x4, x5, x6, x7, x8, x9));
					Stack1.push(strTemp);
					commandline.erase(0, minNum(x1, x2, x3, x4, x5, x6, x7, x8, x9));
				}
			}
		}
	}
	Stack1.push(";");
	stack<string> Stack_temp2;

	while(Stack1.size() != 0){
		Stack_temp2.push(Stack1.top());
		Stack1.pop();
	}
	Stack_temp2.push("=");

	Stack_temp2.push(Output_temp);


	while(Stack_temp2.size() != 0){
		int kkll = Stack_temp2.size();
		string Temp = Stack_temp2.top();
		if(!currentNode){
			currentNode = new node;
			currentNode->s = Stack_temp2.top();
			currentNode->next = NULL;
			nodeHead = currentNode;
		}else{
			currentNode->next = new node;
			currentNode->next->s = Stack_temp2.top();
			currentNode->next->next = NULL;
			currentNode = currentNode->next;
		}
		Stack_temp2.pop();
	}
	return 0;
}


//insert the input file into array
int wireAnalyzer::wireFinder4reg(string commandline, long int eqNumber)
{


	//QHandler();*************************handling ? option****************************
	int cmdCounter = 0;

	if(commandline.size() == 0) {error_type5(eqNumber); exit(0); return -1;}

	stack<string> Stack1;
	if(commandline.find("FDCPE ") == 0 || commandline.find("FDCPE\t") == 0){
		commandline.erase(0, 6);
		Stack1.push("FDCPE");
	}

	if(commandline.find("FDDCPE ") == 0 || commandline.find("FDDCPE\t") == 0){
		commandline.erase(0, 7);
		Stack1.push("FDDCPE");
	}
	if(commandline.find("FTCPE ") == 0 || commandline.find("FTCPE\t") == 0){
		commandline.erase(0, 6);
		Stack1.push("FTCPE");
	}
	if(commandline.find("FTDCPE ") == 0 || commandline.find("FTDCPE\t") == 0){
		commandline.erase(0, 7);
		Stack1.push("FTDCPE");
	}
	if(commandline.find("LDCP ") == 0 || commandline.find("LDCP\t") == 0){
		commandline.erase(0, 5);
		Stack1.push("LDCP");
	}


	commandline = erase_space(commandline);


	while( commandline.substr(0, 1) != ";"){
		if((commandline.substr(0, 1) == "!") || (commandline.substr(0, 1) == "(") || (commandline.substr(0, 1) == ")") || (commandline.substr(0, 1) == "^") || (commandline.substr(0, 1) == ",")){
			Stack1.push(commandline.substr(0, 1));
			commandline.erase(0, 1);
		} else{
			if((commandline.substr(0, 2) == "&&") || (commandline.substr(0, 2) == "||")){
				Stack1.push(commandline.substr(0, 2));
				commandline.erase(0, 2);
			}else{
				if((commandline.substr(0, 4) == "1'b0") || (commandline.substr(0, 4) == "1'b1")|| (commandline.substr(0, 4) == "1'bZ")){
				Stack1.push(commandline.substr(0, 4));
				commandline.erase(0, 4);
				} else{
					int x1 = commandline.find("(");
					int x2 = commandline.find(")");
					int x3 = commandline.find("!");
					int x4 = commandline.find(",");//we changed this from wireFinder
					int x5 = commandline.find("||");
					int x6 = commandline.find("^");
					int x7 = commandline.find("?");
					int x8 = commandline.find(":");
					int x9 = commandline.find(";");
					if(minNum(x1, x2, x3, x4, x5, x6, x7, x8, x9) == -1) {error_type6(eqNumber); exit(0); return -1;}
					string strTemp = commandline.substr(0, minNum(x1, x2, x3, x4, x5, x6, x7, x8, x9));
					Stack1.push(strTemp);
					commandline.erase(0, minNum(x1, x2, x3, x4, x5, x6, x7, x8, x9));
				}
			}
		}
	}
	Stack1.push(";");
	stack<string> Stack_temp2;

	while(Stack1.size() != 0){
		Stack_temp2.push(Stack1.top());
		Stack1.pop();
	}

	while(Stack_temp2.size() != 0){
		int kkll = Stack_temp2.size();
		string Temp = Stack_temp2.top();
		if(!currentNode){
			currentNode = new node;
			currentNode->s = Stack_temp2.top();
			currentNode->next = NULL;
			nodeHead = currentNode;
		}else{
			currentNode->next = new node;
			currentNode->next->s = Stack_temp2.top();
			currentNode->next->next = NULL;
			currentNode = currentNode->next;
		}
		Stack_temp2.pop();
	}
	return 0;
}






void wireAnalyzer::printVerilogEq(){

	char eqFilet[256];
	for(int i=0; i<Eq_after_WireAnalyzer_File.length(); i++) eqFilet[i] = Eq_after_WireAnalyzer_File[i];
	eqFilet[Eq_after_WireAnalyzer_File.length()] = 0;

	remove(eqFilet);
	ofstream FO_TEMP(eqFilet);

	FO_TEMP<<endl<<"module "<<ModuleName<<"_eq("<<GlobalReset<<", ";

	node* Temp = NULL;
	if(Inputs){

		Temp = Inputs;
		while(Temp->next){
			FO_TEMP<<Temp->s<<","<<endl;
			Temp = Temp->next;
		}
		if(Outputs){ FO_TEMP<<Temp->s<<","<<endl;}
	}

	if(Outputs){

		Temp = Outputs;
		while(Temp->next){
			FO_TEMP<<Temp->s<<","<<endl;
			Temp = Temp->next;
		}
	}

	if(Outputs) FO_TEMP<<Temp->s<<");"<<endl;

	FO_TEMP<<"input "<<GlobalReset<<";"<<endl;

	Temp = InputsStr;

	if(Temp){
		while(Temp->next){
			FO_TEMP<<Temp->s<<";"<<endl;
			Temp = Temp->next;
		}
	}

	if(InputsStr) FO_TEMP<<Temp->s<<";"<<endl<<endl;

	Temp = OutputsStr;

	if(Temp){
		while(Temp->next){
			FO_TEMP<<Temp->s<<";"<<endl;
			Temp = Temp->next;
		}
	}
	if(OutputsStr) FO_TEMP<<Temp->s<<";"<<endl<<endl;





	Temp = wiresHead;
	if(Temp){FO_TEMP<<"wire"<<endl;}

	if(Temp){
		while(Temp->next){
			FO_TEMP<<Temp->s<<","<<endl;
			Temp = Temp->next;
		}
	}

	if(wiresHead) FO_TEMP<<Temp->s<<";"<<endl<<endl;


	Temp = nodeHead;


	if(Temp){
		if(Temp->s != "FDCPE" && Temp->s != "FTCPE" && Temp->s != "FDDCPE" && Temp->s != "FTDCPE" && Temp->s != "LDCP"){ FO_TEMP<<"assign ";}
	}
	while(Temp){
		if(Temp->s == ";"){
			if(Temp->next){
				if(Temp->next->s != "FDCPE" && Temp->next->s != "FTCPE" && Temp->next->s != "FDDCPE" && Temp->next->s != "FTDCPE" && Temp->next->s != "LDCP"){
					FO_TEMP<<Temp->s<<endl<<"assign ";
				}
				else{
					FO_TEMP<<Temp->s<<endl;
				}
			}
			else{
				FO_TEMP<<Temp->s<<endl;
			}
		}

		else FO_TEMP<<Temp->s<<" ";
		Temp = Temp->next;
	}

	FO_TEMP<<endl<<"endmodule"<<endl;


	FO_TEMP.close();

	return;
}


//-------------------------------------------------------------------


/*struct GateStruct
{
	int Type; //0 buffer, 1 AND, 2 OR, 3 XOR

	string Output;
	node* input;
	bool invert; //0 does not invert allowed
	GateStruct* next;
};*/




class netlistGenerator: public wireAnalyzer{
public:
	netlistGenerator() {
		ConstructFlag = 0; andCnt = 0; gateCnt = 0; FDCPEcnt = 0; FDDCPEcnt = 0; LDCPcnt = 0; FTDCPEcnt = 0; FTCPEcnt = 0; Gatehead = NULL; currentPointer=NULL; wiresHeadGate = NULL; currentGateWire = NULL; wireGateCnt = 0;
	}
	netlistGenerator(node*, string, string, string);	int CommandReader();
	void printGate();
	void bufferRmv();
	void wireAnalyzer_Gate();
	void printGate_preGateWireAnalyzer();
	void changingInOut();
	void benchGenerator();
	void benchGeneratorV2();
	void FanoutGenerator();
	int verilogGeneratorV1();
	int verilogGeneratorV2();
	void printingTestForNetlist();
	void printEQ_Components();
	void printNet_Components();
	void printMem_module();
	void outputFanoutHandller();
	void wireAnalyzer_Gate2();
	void AddingGlobalReset2();
	void clkEnableHn();
	int checkInOut();
	void optimizingNetlist();
	

	//Fault Simulation
	
	


protected:

	long int andCnt;
	long int nandCnt;
	long int orCnt;
	long int norCnt;
	long int xorCnt;
	long int xnorCnt;
	long int notCnt;
	long int bufferCnt;
	long int FDCPEcnt;
	long int FDDCPEcnt;
	long int LDCPcnt;
	long int FTDCPEcnt;
	long int FTCPEcnt;
	long int fanoutCnt;
	long int gateCnt;
	long int dffCnt;

	node* wiresHeadGate;
	node* currentGateWire;
	long int wireGateCnt;
	string equation_file;

	//GateStruct* Gatehead;
	//GateStruct* currentPointer;

	string createIntermediatewire(string);
	void insertIntermediate(string des, string source);



	string changeInOut(string, long int);


	GateStruct* findingFanout(string);


	void replaceGatewire(string, string, GateStruct*);
	void replaceGatewire(string, string);
	int checkingWire(string, node*);
	void assignHandler(string, long int);
	int FDCPEHandler(string, string, string, string, string, string, string, string);
	int FTCPEHandler(string, string, string, string, string, string, string, string);
	int FDDCPEHandler(string, string, string, string, string, string, string, string);
	int LDCPHandler(string, string, string, string, string, string, string);
	int minNum(int, int, int, int, int, int, int, int, int);
	int checkingWire(string, long int);
	int checkingGate(long int);
	void deleteGate();
	string creat_NOT(string, long int);
	string creat_BUFFER(string, string, long int);
	string QHandler(string);
	string logicExtractor(string);

	void error_type4(long int);
	void error_type5(long int);
	void error_type6(long int);
	void error_type7(long int);
	void error_type8_FDCPE(long int);
	void error_type8_FTCPE(long int);
	void error_type8_FDDCPE(long int);
	void error_type8_LDCP(long int);
	void error_type9_input();
	void error_type9_output();

	void warnning_type1(long int);
	void warnning_type2(long int);

	void fanout_information(string, string, string);
	void information();

	int checkingFloatBuf(string s);

	//fault simulation
	int FSNetlistGeneratorV2();

	void deleteGate(GateStruct* gatePtr);
	GateStruct* IsSoloNotOutput(string s);


};

netlistGenerator::netlistGenerator(node* AddedFilest, string ProjectNamet, string ModuleNamet, string ISEpatht){
	EX_Inputs_1 = NULL;
	EX_Outputs_1 = NULL;
	Inputs = NULL;
	Outputs = NULL;
	EX_Inputs2 = NULL;
	EX_Outputs2 = NULL;
	InputsStr = NULL;
	OutputsStr = NULL;
	addedFiles = NULL;
	ProjectName = ProjectNamet;
	addedFiles = AddedFilest;
	ModuleName = ModuleNamet;
	ISEpath = ISEpatht;
	andCnt = 0;
	gateCnt = 0;
	FDCPEcnt = 0;
	FDDCPEcnt = 0;
	LDCPcnt = 0;
	FTDCPEcnt = 0;
	FTCPEcnt = 0;
	Gatehead = NULL;
	currentPointer=NULL;
	wiresHeadGate = NULL;
	currentGateWire = NULL;
	wireGateCnt = 0;
	ConstructFlag = 1;	
}

void netlistGenerator::error_type4(long int rptLineCnt){
	cout<<"\nThere is an error in your EDIF file.\n";
	cout<<"We do not detect any equation.\n .\n";
	ERR<<"\nThere is an error in your EDIF file.\n";
	ERR<<"We do not detect any equation.\n .\n";
	return;
}

void netlistGenerator::error_type5(long int eqLineCnt){
	cout<<"\nThere is an error in your equation file.\n";
	cout<<"We do not detect any equation in line "<<eqLineCnt<<".\n .\n";
	ERR<<"\nThere is an error in your equation file.\n";
	ERR<<"We do not detect any equation in line "<<eqLineCnt<<".\n .\n";
	return;
}

void netlistGenerator::error_type6(long int eqLineCnt){
	cout<<"\nThere is an error in your equation file.\n";
	cout<<"We do not detect any key words in line "<<eqLineCnt<<".\n .\n";
	ERR<<"\nThere is an error in your equation file.\n";
	ERR<<"We do not detect any key words in line "<<eqLineCnt<<".\n .\n";
	return;
}

void netlistGenerator::error_type7(long int gateCounter){
	cout<<"\nThere is an error in your gate number "<<gateCounter<<".\n";
	cout<<"We detected '[' in the inputs, when we detect an '[' we look for ']'\n";
	ERR<<"\nThere is an error in your gate number "<<gateCounter<<".\n";
	ERR<<"We detected '[' in the inputs, when we detect an '[' we look for ']'\n";

	return;
}

void netlistGenerator::error_type8_FDCPE(long int gateCounter){
	cout<<"\nThere is an error in your FDCPE number "<<gateCounter<<".\n";
	cout<<"We detected regular order for an FDCPE with 7 arguments.\n";
	ERR<<"\nThere is an error in your FDCPE number "<<gateCounter<<".\n";
	ERR<<"We detected regular order for an FDCPE with 7 arguments.\n";

	return;
}

void netlistGenerator::error_type8_FTCPE(long int gateCounter){
	cout<<"\nThere is an error in your FTCPE number "<<gateCounter<<".\n";
	cout<<"We detected regular order for an FTCPE with 7 arguments.\n";
	ERR<<"\nThere is an error in your FTCPE number "<<gateCounter<<".\n";
	ERR<<"We detected regular order for an FTCPE with 7 arguments.\n";

	return;
}

void netlistGenerator::error_type8_FDDCPE(long int gateCounter){
	cout<<"\nThere is an error in your FDDCPE number "<<gateCounter<<".\n";
	cout<<"We detected regular order for an FDDCPE with 7 arguments.\n";
	ERR<<"\nThere is an error in your FDDCPE number "<<gateCounter<<".\n";
	ERR<<"We detected regular order for an FDDCPE with 7 arguments.\n";

	return;
}

void netlistGenerator::error_type8_LDCP(long int gateCounter){
	cout<<"\nThere is an error in your LDCP number "<<gateCounter<<".\n";
	cout<<"We detected regular order for an LDCP with 7 arguments.\n";
	ERR<<"\nThere is an error in your LDCP number "<<gateCounter<<".\n";
	ERR<<"We detected regular order for an LDCP with 7 arguments.\n";

	return;
}

void netlistGenerator::error_type9_input(){
	cout<<"\nThere is an error in your verilog or EDIF file. Extracted inputs do not match the real inputs.\n";
	ERR<<"\nThere is an error in your verilog or EDIF file. Extracted inputs do not match the real inputs.\n";
	return;
}

void netlistGenerator::error_type9_output(){
	cout<<"\nThere is an error in your verilog or EDIF file. Extracted outputs do not match the real oputs.\n";
	ERR<<"\nThere is an error in your verilog or EDIF file. Extracted outputs do not match the real oputs.\n";
	return;
}

void netlistGenerator::warnning_type1(long int eqNumber){
	cout<<"\nWarnning type1";
	cout<<"\nIn line number "<<eqNumber<<" duplicated gate is detected.\n";
	WAR<<"\nWarnning type1";
	WAR<<"\nIn line number "<<eqNumber<<" duplicated gate is detected.\n";
	return;
}
void netlistGenerator::warnning_type2(long int eqNumber){
	cout<<"\nWarnning type2";
	cout<<"\nIn line number "<<eqNumber<<" we detected some simboles which should be handled for fault simulation.\n";
	cout<<"The report file will show you where you can check the result on netlist format.\n";
	WAR<<"\nWarnning type2";
	WAR<<"\nIn line number "<<eqNumber<<" we detected some simboles which should be handled for fault simulation.\n";
	WAR<<"The report file will show you where you can check the result on netlist format.\n";
	return;
}



void netlistGenerator::information(){
	cout<<"\nIn this phase the netlist of your design is being extracted by the output format of ISE synthesis tool.\n";
	cout<<"The EDIF file of specified design is expected in the root directory.\n .\n";
	INF<<"\nIn this phase the netlist of your design is being extracted by the output format of ISE synthesis tool.\n";
	INF<<"The EDIF file of specified design is expected in the root directory.\n  ";
	return;
}

void netlistGenerator::fanout_information(string fan_number, string fan_input, string fan_iteration){
	cout<<"\n"<<fan_number<<"th FAN-OUT with "<<fan_iteration<<" branches was generated.\n";
	return;
}


int netlistGenerator::checkingWire(string s, node* head){
	node* Temp = head;
	while(Temp){
		if(s == Temp->s){
			return 1;
		}
		Temp=Temp->next;
	}

		return 0;
}

void netlistGenerator::benchGenerator(){

	string benchFile = ModuleName;
	benchFile.insert(benchFile.length(), ".bench");
	char bench[256];
	for(int i=0; i<256; i++) bench[i] = benchFile[i];
	bench[benchFile.length()];

	remove(bench);


	ofstream FO_TEMP(bench);
	FO_TEMP<<endl;
	FO_TEMP<<"#Bench file of module "<<ModuleName;
	FO_TEMP<<endl;


	node* nodeTemp = EX_Inputs_1;
	while(nodeTemp){
		FO_TEMP<<"INPUT ("<<nodeTemp->s<<")"<<endl;
		nodeTemp = nodeTemp->next;
	}
	FO_TEMP<<endl;

	nodeTemp = EX_Outputs_1;
	while(nodeTemp){
		FO_TEMP<<"OUTPUT ("<<nodeTemp->s<<")"<<endl;
		nodeTemp = nodeTemp->next;
	}

	FO_TEMP<<endl;

	GateStruct* Temp = Gatehead;
	while(Temp){
		if(Temp->Type == 4 || Temp->Type == 6){
			FO_TEMP<<Temp->Output<<" = DFF("<<Temp->input->s<<")"<<endl<<endl;
		}
		Temp = Temp->next;
	}

	Temp = Gatehead;
	while(Temp){
		if(Temp->Type == 5 || Temp->Type == 7){
			FO_TEMP<<Temp->Output<<" = DFF("<<Temp->Output<<"_xor_"<<Temp->input->s<<")"<<endl;
			FO_TEMP<<Temp->Output<<"_xor_"<<Temp->input->s<<" = XOR("<<Temp->input->s<<", "<<Temp->Output<<")"<<endl<<endl;
		}
		Temp = Temp->next;
	}

	Temp = Gatehead;
	while(Temp){
		if(Temp->Type != 4 && Temp->Type != 5 && Temp->Type != 6 && Temp->Type != 7 && Temp->Type != 8){
			FO_TEMP<<Temp->Output<<" = ";
			if(Temp->Type ==0 && Temp->invert == 0){FO_TEMP<<"BUF(";}
			if(Temp->Type ==0 && Temp->invert == 1){FO_TEMP<<"NOT(";}
			if(Temp->Type ==1 && Temp->invert == 0){FO_TEMP<<"AND(";}
			if(Temp->Type ==1 && Temp->invert == 1){FO_TEMP<<"NAND(";}
			if(Temp->Type ==2 && Temp->invert == 0){FO_TEMP<<"OR(";}
			if(Temp->Type ==2 && Temp->invert == 1){FO_TEMP<<"NOR(";}
			if(Temp->Type ==3 && Temp->invert == 0){FO_TEMP<<"XOR(";}
			if(Temp->Type ==3 && Temp->invert == 1){FO_TEMP<<"XNOR(";}

			
			if(Temp->input){

				FO_TEMP<<Temp->input->s;

				node* currentInput = Temp->input->next;

				while(currentInput){
					FO_TEMP<<", "<<currentInput->s;
					currentInput = currentInput->next;
				}

				FO_TEMP<<")"<<endl;
			}
		}

		Temp = Temp->next;
	}

	FO_TEMP<<endl<<"END"<<endl;

	FO_TEMP.close();
	return;


}

void netlistGenerator::benchGeneratorV2(){

	string benchFile = ModuleName;
	benchFile.insert(benchFile.length(), ".bench");
	char bench[256];
	for(int i=0; i<benchFile.length(); i++) {
		
		bench[i] = benchFile[i];
	}
	bench[benchFile.length()] =  0;

	//remove(bench);


	ofstream FO_TEMP(bench);
	FO_TEMP<<endl;
	FO_TEMP<<"#Bench file of module "<<ModuleName;
	FO_TEMP<<endl;


	node* nodeTemp = EX_Inputs_1;
	while(nodeTemp){
		FO_TEMP<<"INPUT ("<<nodeTemp->s<<")"<<endl;
		nodeTemp = nodeTemp->next;
	}
	FO_TEMP<<endl;

	nodeTemp = EX_Outputs_1;
	while(nodeTemp){
		FO_TEMP<<"OUTPUT ("<<nodeTemp->s<<")"<<endl;
		nodeTemp = nodeTemp->next;
	}

	FO_TEMP<<endl;

	GateStruct* Temp = Gatehead;
	while(Temp){
		if(Temp->Type == 4 || Temp->Type == 5 || Temp->Type == 6 || Temp->Type == 7){
			FO_TEMP<<Temp->Q<<" = DFF("<<Temp->D<<")"<<endl<<endl;
		}
		Temp = Temp->next;
	}

	Temp = Gatehead;
	/*while(Temp){
		if(Temp->Type == 5 || Temp->Type == 7){
			FO_TEMP<<Temp->Output<<" = DFF("<<Temp->Output<<"_xor_"<<Temp->input->s<<")"<<endl;
			FO_TEMP<<Temp->Output<<"_xor_"<<Temp->input->s<<" = XOR("<<Temp->input->s<<", "<<Temp->Output<<")"<<endl<<endl;
		}
		Temp = Temp->next;
	}*/

	Temp = Gatehead;
	while(Temp){
		if(Temp->Type != 4 && Temp->Type != 5 && Temp->Type != 6 && Temp->Type != 7 && Temp->Type != 8){
			FO_TEMP<<Temp->Output<<" = ";
			if(Temp->Type ==0 && Temp->invert == 0){FO_TEMP<<"BUF(";}
			if(Temp->Type ==0 && Temp->invert == 1){FO_TEMP<<"NOT(";}
			if(Temp->Type ==1 && Temp->invert == 0){FO_TEMP<<"AND(";}
			if(Temp->Type ==1 && Temp->invert == 1){FO_TEMP<<"NAND(";}
			if(Temp->Type ==2 && Temp->invert == 0){FO_TEMP<<"OR(";}
			if(Temp->Type ==2 && Temp->invert == 1){FO_TEMP<<"NOR(";}
			if(Temp->Type ==3 && Temp->invert == 0){FO_TEMP<<"XOR(";}
			if(Temp->Type ==3 && Temp->invert == 1){FO_TEMP<<"XNOR(";}
			if(Temp->Type ==10 && Temp->invert == 0){FO_TEMP<<"BUF(";}
			if(Temp->Type ==11 && Temp->invert == 0){FO_TEMP<<"BUF(";}

			
			if(Temp->input){

				FO_TEMP<<Temp->input->s;

				node* currentInput = Temp->input->next;

				while(currentInput){
					FO_TEMP<<", "<<currentInput->s;
					currentInput = currentInput->next;
				}

				FO_TEMP<<")"<<endl;
			}else{
				FO_TEMP<<endl;
			}

		}

		Temp = Temp->next;
	}
	

	FO_TEMP<<endl<<"END"<<endl;

	FO_TEMP.close();
	return;


}





void netlistGenerator::wireAnalyzer_Gate(){

	cout<<"\n**************************************************\n";
	cout<<"\nAnalyzing wires for netlist generation...\n";
	string newWire = "wire_";
	newWire.insert(newWire.length(), int2str(wireGateCnt));

	GateStruct* Temp = Gatehead;
	while(Temp){
		if(!checkingWire(Temp->Output, EX_Inputs2)){
			if(!checkingWire(Temp->Output, EX_Outputs2)){
				if(!checkingWire(Temp->Output, wiresHeadGate) && Temp->Output != "1'b0" && Temp->Output != "1'b1" && Temp->Output != "1'bZ" && Temp->Output != GlobalReset ){
					int replaceFlag = 0;
					while(!replaceFlag){
						wireGateCnt++;
						newWire = "wire_";
						newWire.insert(newWire.length(), int2str(wireGateCnt));
						if(!checkingWire(newWire, EX_Inputs2) && !checkingWire(newWire, EX_Outputs2)){
							information_type2(newWire, Temp->Output);
							replaceGatewire(newWire, Temp->Output, Temp);
							if(!currentGateWire){
								currentGateWire = new node;
								currentGateWire->s = newWire;
								currentGateWire->next = NULL;
								wiresHeadGate = currentGateWire;
							}else{
								currentGateWire->next = new node;
								currentGateWire->next->s = newWire;
								currentGateWire->next->next = NULL;
								currentGateWire = currentGateWire->next;
							}
								replaceFlag = 1;
						} else{
							information_type3(newWire);
							replaceFlag = 0;
						}
					}
				}
			}else{
				information_type4(Temp->Output);//Output information
			}
		}else{
			information_type5(Temp->Output);//input Information
		}

		node* Temp2 = Temp->input;
		while(Temp2){
			if(!checkingWire(Temp2->s, EX_Inputs2)){
				if(!checkingWire(Temp2->s, EX_Outputs2)){
					if(!checkingWire(Temp2->s, wiresHeadGate) && Temp2->s != "1'b0" && Temp2->s != "1'b1" && Temp2->s != "1'bZ" && Temp2->s != GlobalReset ){
						int replaceFlag = 0;
						while(!replaceFlag){
							wireGateCnt++;
							newWire = "wire_";
							newWire.insert(newWire.length(), int2str(wireGateCnt));
							if(!checkingWire(newWire, EX_Inputs2) && !checkingWire(newWire, EX_Outputs2)){
								information_type2(newWire, Temp2->s);
								replaceGatewire(newWire, Temp2->s, Temp);
								if(!currentGateWire){
									currentGateWire = new node;
									currentGateWire->s = newWire;
									currentGateWire->next = NULL;
									wiresHeadGate = currentGateWire;
								}else{
									currentGateWire->next = new node;
									currentGateWire->next->s = newWire;
									currentGateWire->next->next = NULL;
									currentGateWire = currentGateWire->next;
								}
								replaceFlag = 1;
							} else{
								information_type3(newWire);
								replaceFlag = 0;
							}
						}
					}
				}else{
					information_type4(Temp2->s);//Output information
				}
			}else{
				information_type5(Temp2->s);//input Information
			}

			Temp2 = Temp2->next;
		}
		Temp = Temp->next;
	}
	return;
}

void netlistGenerator::wireAnalyzer_Gate2(){

	cout<<"\n**************************************************\n";
	cout<<"\nAnalyzing wires for netlist generation...\n";
	string newWire = "wire_";
	newWire.insert(newWire.length(), int2str(wireGateCnt));
	
	int aTemp = 0;
	GateStruct* Temp = Gatehead;
	while(Temp){
		
		if (Temp->Output.find("intermediateXXXwire_") == 0){
			wireGateCnt++;
			newWire = "wire_";
			newWire.insert(newWire.length(), int2str(wireGateCnt));
			information_type2(newWire, Temp->Output);
			cout<<"(Gate path) : "<<Temp->path<<endl;
			INF<<"(Gate path) : "<<Temp->path<<endl;
			netReplace(Temp->Output, newWire);

			
			if(!currentGateWire){
				currentGateWire = new node;
				currentGateWire->s = newWire;
				currentGateWire->next = NULL;
				wiresHeadGate = currentGateWire;
			}else{
				currentGateWire->next = new node;
				currentGateWire->next->s = newWire;
				currentGateWire->next->next = NULL;
				currentGateWire = currentGateWire->next;
			}
		}
		if (Temp->C.find("intermediateXXXwire_") == 0){
			wireGateCnt++;
			newWire = "wire_";
			newWire.insert(newWire.length(), int2str(wireGateCnt));
			information_type2(newWire, Temp->C);
			cout<<"(Gate path) : "<<Temp->path<<endl;
			INF<<"(Gate path) : "<<Temp->path<<endl;
			netReplace(Temp->C, newWire);
			
			
			if(!currentGateWire){
				currentGateWire = new node;
				currentGateWire->s = newWire;
				currentGateWire->next = NULL;
				wiresHeadGate = currentGateWire;
			}else{
				currentGateWire->next = new node;
				currentGateWire->next->s = newWire;
				currentGateWire->next->next = NULL;
				currentGateWire = currentGateWire->next;
			}
		}
		if (Temp->CE.find("intermediateXXXwire_") == 0){
			wireGateCnt++;
			newWire = "wire_";
			newWire.insert(newWire.length(), int2str(wireGateCnt));
			information_type2(newWire, Temp->CE);
			cout<<"(Gate path) : "<<Temp->path<<endl;
			INF<<"(Gate path) : "<<Temp->path<<endl;
			netReplace(Temp->CE, newWire);
			
			
			if(!currentGateWire){
				currentGateWire = new node;
				currentGateWire->s = newWire;
				currentGateWire->next = NULL;
				wiresHeadGate = currentGateWire;
			}else{
				currentGateWire->next = new node;
				currentGateWire->next->s = newWire;
				currentGateWire->next->next = NULL;
				currentGateWire = currentGateWire->next;
			}
		}
		if (Temp->CLR.find("intermediateXXXwire_") == 0){
			wireGateCnt++;
			newWire = "wire_";
			newWire.insert(newWire.length(), int2str(wireGateCnt));
			information_type2(newWire, Temp->CLR);
			cout<<"(Gate path) : "<<Temp->path<<endl;
			INF<<"(Gate path) : "<<Temp->path<<endl;
			netReplace(Temp->CLR, newWire);
			
			
			if(!currentGateWire){
				currentGateWire = new node;
				currentGateWire->s = newWire;
				currentGateWire->next = NULL;
				wiresHeadGate = currentGateWire;
			}else{
				currentGateWire->next = new node;
				currentGateWire->next->s = newWire;
				currentGateWire->next->next = NULL;
				currentGateWire = currentGateWire->next;
			}
		}
		if (Temp->D.find("intermediateXXXwire_") == 0){
			wireGateCnt++;
			newWire = "wire_";
			newWire.insert(newWire.length(), int2str(wireGateCnt));
			information_type2(newWire, Temp->D);
			cout<<"(Gate path) : "<<Temp->path<<endl;
			INF<<"(Gate path) : "<<Temp->path<<endl;
			netReplace(Temp->D, newWire);
			
			
			if(!currentGateWire){
				currentGateWire = new node;
				currentGateWire->s = newWire;
				currentGateWire->next = NULL;
				wiresHeadGate = currentGateWire;
			}else{
				currentGateWire->next = new node;
				currentGateWire->next->s = newWire;
				currentGateWire->next->next = NULL;
				currentGateWire = currentGateWire->next;
			}
		}
		if (Temp->PRE.find("intermediateXXXwire_") == 0){
			wireGateCnt++;
			newWire = "wire_";
			newWire.insert(newWire.length(), int2str(wireGateCnt));
			information_type2(newWire, Temp->PRE);
			cout<<"(Gate path) : "<<Temp->path<<endl;
			INF<<"(Gate path) : "<<Temp->path<<endl;
			netReplace(Temp->PRE, newWire);
			
			
			if(!currentGateWire){
				currentGateWire = new node;
				currentGateWire->s = newWire;
				currentGateWire->next = NULL;
				wiresHeadGate = currentGateWire;
			}else{
				currentGateWire->next = new node;
				currentGateWire->next->s = newWire;
				currentGateWire->next->next = NULL;
				currentGateWire = currentGateWire->next;
			}
		}
		if (Temp->Q.find("intermediateXXXwire_") == 0){
			wireGateCnt++;
			newWire = "wire_";
			newWire.insert(newWire.length(), int2str(wireGateCnt));
			information_type2(newWire, Temp->Q);
			cout<<"(Gate path) : "<<Temp->path<<endl;
			INF<<"(Gate path) : "<<Temp->path<<endl;
			netReplace(Temp->Q, newWire);
			
			
			if(!currentGateWire){
				currentGateWire = new node;
				currentGateWire->s = newWire;
				currentGateWire->next = NULL;
				wiresHeadGate = currentGateWire;
			}else{
				currentGateWire->next = new node;
				currentGateWire->next->s = newWire;
				currentGateWire->next->next = NULL;
				currentGateWire = currentGateWire->next;
			}
		}

		node* nodeTemp = Temp->input;
		while(nodeTemp){
			if (nodeTemp->s.find("intermediateXXXwire") == 0){
				wireGateCnt++;
				newWire = "wire_";
				newWire.insert(newWire.length(), int2str(wireGateCnt));
				information_type2(newWire, nodeTemp->s);
				cout<<"(Gate path) : "<<Temp->path<<endl;
				INF<<"(Gate path) : "<<Temp->path<<endl;
				netReplace(nodeTemp->s, newWire);

				
				
				if(!currentGateWire){
					currentGateWire = new node;
					currentGateWire->s = newWire;
					currentGateWire->next = NULL;
					wiresHeadGate = currentGateWire;
				}else{
					currentGateWire->next = new node;
					currentGateWire->next->s = newWire;
					currentGateWire->next->next = NULL;
					currentGateWire = currentGateWire->next;
				}
			}
			nodeTemp = nodeTemp->next;
		}

		/*
		if(Temp->Type!= 4 && Temp->Type != 5 && Temp->Type != 6 && Temp->Type != 7){

			if(Temp->Output != "1'b0" && Temp->Output != "1'b1" && Temp->Output != "1'bZ" && Temp->Output != GlobalReset && !checkingWire(Temp->Output, EX_Inputs_1) && !checkingWire(Temp->Output, EX_Outputs_1) ){
				int replaceFlag = 0;
				while(!replaceFlag){
					wireGateCnt++;
					newWire = "wire_";
					newWire.insert(newWire.length(), int2str(wireGateCnt));
					if(!checkingWire(newWire, aTemp) && !checkingWire(newWire, aTemp)){
						information_type2(newWire, Temp->Output);
						replaceGatewire(newWire, Temp->Output);
						if(!currentGateWire){
							currentGateWire = new node;
							currentGateWire->s = newWire;
							currentGateWire->next = NULL;
							wiresHeadGate = currentGateWire;
						}else{
							currentGateWire->next = new node;
							currentGateWire->next->s = newWire;
							currentGateWire->next->next = NULL;
							currentGateWire = currentGateWire->next;
						}
							replaceFlag = 1;
					} else{
						information_type3(newWire);
						replaceFlag = 0;
					}
				}
			}

			//-----------------------------------------------------------------------------------------

			node* Temp2 = Temp->input;
			while(Temp2){
				if(Temp2->s != "1'b0" && Temp2->s != "1'b1" && Temp2->s != "1'bZ" && Temp2->s != GlobalReset && !checkingWire(Temp2->s, EX_Inputs_1) && !checkingWire(Temp2->s, EX_Outputs_1) ){
					int replaceFlag = 0;
					while(!replaceFlag){
						wireGateCnt++;
						newWire = "wire_";
						newWire.insert(newWire.length(), int2str(wireGateCnt));
						if(!checkingWire(newWire, aTemp) && !checkingWire(newWire, aTemp)){
							information_type2(newWire, Temp2->s);
							replaceGatewire(newWire, Temp2->s);
							if(!currentGateWire){
								currentGateWire = new node;
								currentGateWire->s = newWire;
								currentGateWire->next = NULL;
								wiresHeadGate = currentGateWire;
							}else{
								currentGateWire->next = new node;
								currentGateWire->next->s = newWire;
								currentGateWire->next->next = NULL;
								currentGateWire = currentGateWire->next;
							}
							replaceFlag = 1;
						} else{
							information_type3(newWire);
							replaceFlag = 0;
						}
					}
						
				}
				Temp2 = Temp2->next;
			}
		}else{
			

			//-------------------------------------------

			if(Temp->D != "1'b0" && Temp->D != "1'b1" && Temp->D != "1'bZ" && Temp->D != GlobalReset && !checkingWire(Temp->D, EX_Inputs_1) && !checkingWire(Temp->D, EX_Outputs_1) ){
				int replaceFlag = 0;
				while(!replaceFlag){
					wireGateCnt++;
					newWire = "wire_";
					newWire.insert(newWire.length(), int2str(wireGateCnt));
					if(!checkingWire(newWire, aTemp) && !checkingWire(newWire, aTemp)){
						information_type2(newWire, Temp->D);
						replaceGatewire(newWire, Temp->D);
						if(!currentGateWire){
							currentGateWire = new node;
							currentGateWire->s = newWire;
							currentGateWire->next = NULL;
							wiresHeadGate = currentGateWire;
						}else{
							currentGateWire->next = new node;
							currentGateWire->next->s = newWire;
							currentGateWire->next->next = NULL;
							currentGateWire = currentGateWire->next;
						}
							replaceFlag = 1;
					} else{
						information_type3(newWire);
						replaceFlag = 0;
					}
				}
			}
					
			
			//-------------------------------------------

			if(Temp->Q != "1'b0" && Temp->Q != "1'b1" && Temp->Q != "1'bZ" && Temp->Q != GlobalReset && !checkingWire(Temp->Q, EX_Inputs_1) && !checkingWire(Temp->Q, EX_Outputs_1) ){
				int replaceFlag = 0;
				while(!replaceFlag){
					wireGateCnt++;
					newWire = "wire_";
					newWire.insert(newWire.length(), int2str(wireGateCnt));
					if(!checkingWire(newWire, aTemp) && !checkingWire(newWire, aTemp)){
						information_type2(newWire, Temp->Q);
						replaceGatewire(newWire, Temp->Q);
						if(!currentGateWire){
							currentGateWire = new node;
							currentGateWire->s = newWire;
							currentGateWire->next = NULL;
							wiresHeadGate = currentGateWire;
						}else{
							currentGateWire->next = new node;
							currentGateWire->next->s = newWire;
							currentGateWire->next->next = NULL;
							currentGateWire = currentGateWire->next;
						}
							replaceFlag = 1;
					} else{
						information_type3(newWire);
						replaceFlag = 0;
					}
				}
			}

						//-------------------------------------------

			if(Temp->C != "1'b0" && Temp->C != "1'b1" && Temp->C != "1'bZ" && Temp->C != GlobalReset && !checkingWire(Temp->C, EX_Inputs_1) && !checkingWire(Temp->C, EX_Outputs_1) ){
				int replaceFlag = 0;
				while(!replaceFlag){
					wireGateCnt++;
					newWire = "wire_";
					newWire.insert(newWire.length(), int2str(wireGateCnt));
					if(!checkingWire(newWire, aTemp) && !checkingWire(newWire, aTemp)){
						information_type2(newWire, Temp->C);
						replaceGatewire(newWire, Temp->C);
						if(!currentGateWire){
							currentGateWire = new node;
							currentGateWire->s = newWire;
							currentGateWire->next = NULL;
							wiresHeadGate = currentGateWire;
						}else{
							currentGateWire->next = new node;
							currentGateWire->next->s = newWire;
							currentGateWire->next->next = NULL;
							currentGateWire = currentGateWire->next;
						}
							replaceFlag = 1;
					} else{
						information_type3(newWire);
						replaceFlag = 0;
					}
				}
			}

						//-------------------------------------------

			if(Temp->CLR != "1'b0" && Temp->CLR != "1'b1" && Temp->CLR != "1'bZ" && Temp->CLR != GlobalReset && !checkingWire(Temp->CLR, EX_Inputs_1) && !checkingWire(Temp->CLR, EX_Outputs_1) ){
				int replaceFlag = 0;
				while(!replaceFlag){
					wireGateCnt++;
					newWire = "wire_";
					newWire.insert(newWire.length(), int2str(wireGateCnt));
					if(!checkingWire(newWire, aTemp) && !checkingWire(newWire, aTemp)){
						information_type2(newWire, Temp->CLR);
						replaceGatewire(newWire, Temp->CLR);
						if(!currentGateWire){
							currentGateWire = new node;
							currentGateWire->s = newWire;
							currentGateWire->next = NULL;
							wiresHeadGate = currentGateWire;
						}else{
							currentGateWire->next = new node;
							currentGateWire->next->s = newWire;
							currentGateWire->next->next = NULL;
							currentGateWire = currentGateWire->next;
						}
							replaceFlag = 1;
					} else{
						information_type3(newWire);
						replaceFlag = 0;
					}
				}
			}
	

						//-------------------------------------------

			if(Temp->CE != "1'b0" && Temp->CE != "1'b1" && Temp->CE != "1'bZ" && Temp->CE != GlobalReset && !checkingWire(Temp->CE, EX_Inputs_1) && !checkingWire(Temp->CE, EX_Outputs_1) ){
				int replaceFlag = 0;
				while(!replaceFlag){
					wireGateCnt++;
					newWire = "wire_";
					newWire.insert(newWire.length(), int2str(wireGateCnt));
					if(!checkingWire(newWire, aTemp) && !checkingWire(newWire, aTemp)){
						information_type2(newWire, Temp->CE);
						replaceGatewire(newWire, Temp->CE);
						if(!currentGateWire){
							currentGateWire = new node;
							currentGateWire->s = newWire;
							currentGateWire->next = NULL;
							wiresHeadGate = currentGateWire;
						}else{
							currentGateWire->next = new node;
							currentGateWire->next->s = newWire;
							currentGateWire->next->next = NULL;
							currentGateWire = currentGateWire->next;
						}
							replaceFlag = 1;
					} else{
						information_type3(newWire);
						replaceFlag = 0;
					}
				}
			}
		}*/
			
			
	
	


		Temp = Temp->next;
	}
	return;
}


string netlistGenerator::changeInOut(string Temp, long int GateCounter){



	if(Temp.find("[") != -1){
		cout<<"\n In gate #"<<GateCounter<<", "<<Temp;
		Temp.insert(Temp.find("["), "_");
		Temp.erase(Temp.find("["), 1);
		if(Temp.find("]") == -1) {
			error_type7(GateCounter);
			return "";
		}
		Temp.erase(Temp.find("]"), 1);
		cout<<" replaced by "<<Temp<<endl;
	}else{return Temp;}
	return Temp;
}

void netlistGenerator::changingInOut(){

	cout<<"\n**************************************************\n";
	cout<<"\nChanging Inputs and Outputs into regular format...\n";
	long int GateCounter = 0;

	GateStruct* Temp = Gatehead;
	while(Temp){
		if(checkingWire(Temp->Output, EX_Inputs2)){
			Temp->Output = changeInOut(Temp->Output, GateCounter);
		}
		if(checkingWire(Temp->Output, EX_Outputs2)){
			Temp->Output = changeInOut(Temp->Output, GateCounter);
		}
		node* Temp2 = Temp->input;
		while(Temp2){

			if(checkingWire(Temp2->s, EX_Inputs2)){
				Temp2->s = changeInOut(Temp2->s, GateCounter);
			}
			if(checkingWire(Temp2->s, EX_Outputs2)){
				Temp2->s = changeInOut(Temp2->s, GateCounter);
			}

			Temp2 = Temp2->next;
		}
		Temp = Temp->next;
		GateCounter++;
	}
	return;
}

void netlistGenerator::replaceGatewire(string newWire, string oldWire, GateStruct* Temp){
	
	while(Temp){
		if(oldWire == Temp->Output){
			Temp->Output = newWire;
		}
		node* Temp2 = Temp->input;
		while(Temp2){
			if(oldWire == Temp2->s){
				Temp2->s = newWire;
			}
			Temp2 = Temp2->next;
		}
		if(oldWire == Temp->D){
			Temp->D = newWire;
		}
		if(oldWire == Temp->Q){
			Temp->Q = newWire;
		}
		if(oldWire == Temp->CE){
			Temp->CE = newWire;
		}
		if(oldWire == Temp->PRE){
			Temp->PRE = newWire;
		}
		if(oldWire == Temp->CLR){
			Temp->CLR = newWire;
		}
		if(oldWire == Temp->C){
			Temp->C = newWire;
		}


		Temp=Temp->next;
	}
	return;
}


void netlistGenerator::replaceGatewire(string newWire, string oldWire){
	
	 GateStruct* Temp = Gatehead; 
	while(Temp){
		if(oldWire == Temp->Output){
			Temp->Output = newWire;
		}
		node* Temp2 = Temp->input;
		while(Temp2){
			if(oldWire == Temp2->s){
				Temp2->s = newWire;
			}
			Temp2 = Temp2->next;
		}
		if(oldWire == Temp->D){
			Temp->D = newWire;
		}
		if(oldWire == Temp->Q){
			Temp->Q = newWire;
		}
		if(oldWire == Temp->PRE){
			Temp->PRE = newWire;
		}
		if(oldWire == Temp->CE){
			Temp->CE = newWire;
		}
		if(oldWire == Temp->CLR){
			Temp->CLR = newWire;
		}
		if(oldWire == Temp->C){
			Temp->C = newWire;
		}


		Temp=Temp->next;
	}
	return;
}



string netlistGenerator::creat_NOT(string st_temp2, long int eqNumber){

	string st_temp = st_temp2;
	st_temp.insert(0, "NOT_");

	if(checkingWire(st_temp, eqNumber)) return st_temp;

	if(!currentPointer) {
		currentPointer = new GateStruct;
		currentPointer->input = new node;
		currentPointer->input->s = st_temp2;
		currentPointer->input->next = NULL;
		currentPointer->next = NULL;
		currentPointer->Output = st_temp;
		currentPointer->Type = 0;
		currentPointer->invert = 1;
		Gatehead = currentPointer;
		currentPointer->INS = "";
	}
	else{
		currentPointer->next = new GateStruct;
		currentPointer = currentPointer->next;
		currentPointer->input = new node;
		currentPointer->input->s = st_temp2;
		currentPointer->input->next = NULL;
		currentPointer->next = NULL;
		currentPointer->Output = st_temp;
		currentPointer->Type = 0;
		currentPointer->invert = 1;
		currentPointer->INS = "";
	}

	return st_temp;

}

string netlistGenerator::creat_BUFFER(string st_temp3, string st_temp2, long int eqNumber){



	if(checkingWire(st_temp2, eqNumber)) return st_temp3;

	if(!currentPointer) {
		currentPointer = new GateStruct;
		currentPointer->input = new node;
		currentPointer->input->s = st_temp2;
		currentPointer->input->next = NULL;
		currentPointer->next = NULL;
		currentPointer->Output = st_temp3;
		currentPointer->Type = 0;
		currentPointer->invert = 0;
		Gatehead = currentPointer;
	}
	else{


		currentPointer->next = new GateStruct;
		currentPointer = currentPointer->next;
		currentPointer->input = new node;
		currentPointer->input->s = st_temp2;
		currentPointer->input->next = NULL;
		currentPointer->next = NULL;
		currentPointer->Output = st_temp3;
		currentPointer->Type = 0;
		currentPointer->invert = 0;
	}

	return st_temp3;

}

int netlistGenerator::checkingGate(long int eqNumber){
	GateStruct* Temp = Gatehead;
	if(!currentPointer->next){return 0;}else{
		while(Temp){
			if(currentPointer->next->Output == Temp->Output && currentPointer->next != Temp && currentPointer->next->invert == Temp->invert){
				return 1;
					warnning_type1(eqNumber);
			}
			Temp=Temp->next;
		}
	}
		return 0;
}

void netlistGenerator::deleteGate(){
	stack<node*> s_temp;
	if(!currentPointer->next) return;
	node* temp = currentPointer->next->input;

	while(temp){
		s_temp.push(temp);
		temp = temp->next;
	}



	while(!s_temp.empty()){

		delete(s_temp.top());
		s_temp.pop();

	}

	delete(currentPointer->next);
	currentPointer->next = NULL;

	return;
}

int netlistGenerator::checkingWire(string s, long int eqNumber){
	GateStruct* Temp = Gatehead;
	while(Temp){
		if(s == Temp->Output){
			return 1;
			warnning_type1(eqNumber);
		}

		if((Temp->Type == 4 || Temp->Type == 5 || Temp->Type == 6 || Temp->Type == 7) && (s == Temp->Q)){
			return 1;
			warnning_type1(eqNumber);
		}
		if((Temp->Type == 4 || Temp->Type == 5 || Temp->Type == 6 || Temp->Type == 7) && (s == Temp->CE)){
			return 1;
			warnning_type1(eqNumber);
		}
		if((Temp->Type == 4 || Temp->Type == 5 || Temp->Type == 6 || Temp->Type == 7) && (s == Temp->CLR)){
			return 1;
			warnning_type1(eqNumber);
		}
		if((Temp->Type == 4 || Temp->Type == 5 || Temp->Type == 6 || Temp->Type == 7) && (s == Temp->D)){
			return 1;
			warnning_type1(eqNumber);
		}
		if((Temp->Type == 4 || Temp->Type == 5 || Temp->Type == 6 || Temp->Type == 7) && (s == Temp->PRE)){
			return 1;
			warnning_type1(eqNumber);
		}
		if((Temp->Type == 4 || Temp->Type == 5 || Temp->Type == 6 || Temp->Type == 7) && (s == Temp->C)){
			return 1;
			warnning_type1(eqNumber);
		}
		Temp=Temp->next;
	}

		return 0;
}

//returns minimum number
int netlistGenerator::minNum(int x1, int x2,int  x3, int x4, int x5, int x6, int x7, int x8, int x9){

	int temp = -1;
	if( x1 != -1 ){
		temp = x1;
	}else
	{
		if (x2 !=-1){
			temp = x2;
		}else{
			if (x3 != -1){
				temp = x3;
			}else{
				if (x4 != -1){
					temp = x4;
				}else{
					if (x5 != -1){
						temp = x5;
					}else{
						if (x6 != -1){
							temp = x6;
						}else{
							if (x7 != -1){
								temp = x7;
							}else{
								if (x8 != -1){
									temp = x8;
								}else{
									temp = x9;
								}
							}
						}
					}
				}
			}
		}
	}

	if (x2 < temp && x2 != -1) temp = x2;
	if (x3 < temp && x3 != -1) temp = x3;
	if (x4 < temp && x4 != -1) temp = x4;
	if (x5 < temp && x5 != -1) temp = x5;
	if (x6 < temp && x6 != -1) temp = x6;
	if (x7 < temp && x7 != -1) temp = x7;
	if (x8 < temp && x8 != -1) temp = x8;
	if (x9 < temp && x9 != -1) temp = x9;

	return temp;
}


int netlistGenerator::FDCPEHandler(string INS, string Arg1, string Arg2, string Arg3, string Arg4, string Arg5, string Arg6, string Arg7){

	if(!currentPointer){
		currentPointer = new GateStruct;
		Gatehead = currentPointer;
		currentPointer->input = new node;
		currentPointer->input->next = NULL;
		currentPointer->input->s = Arg2;

		currentPointer->input->next = new node;
		currentPointer->input->next->s = Arg3;
		currentPointer->input->next->next=NULL;


		currentPointer->input->next->next = new node;
		currentPointer->input->next->next->s = Arg4;
		currentPointer->input->next->next->next=NULL;


		currentPointer->input->next->next->next = new node;
		currentPointer->input->next->next->next->s = Arg5;
		currentPointer->input->next->next->next->next=NULL;


		currentPointer->input->next->next->next->next = new node;
		currentPointer->input->next->next->next->next->s = Arg6;
		currentPointer->input->next->next->next->next->next=NULL;

		currentPointer->input->next->next->next->next->next = new node;
		currentPointer->input->next->next->next->next->next->s = Arg7;
		currentPointer->input->next->next->next->next->next->next=NULL;

		currentPointer->Output = Arg1;
		currentPointer->invert = 0;
		currentPointer->Type = 4;
		currentPointer->INS = INS;
		currentPointer->next=NULL;
	}else{
		currentPointer->next = new GateStruct;
		currentPointer->next->input = new node;
		currentPointer->next->input->next = NULL;
		currentPointer->next->input->s = Arg2;

		currentPointer->next->input->next = new node;
		currentPointer->next->input->next->s = Arg3;
		currentPointer->next->input->next->next=NULL;


		currentPointer->next->input->next->next = new node;
		currentPointer->next->input->next->next->s = Arg4;
		currentPointer->next->input->next->next->next=NULL;


		currentPointer->next->input->next->next->next = new node;
		currentPointer->next->input->next->next->next->s = Arg5;
		currentPointer->next->input->next->next->next->next=NULL;


		currentPointer->next->input->next->next->next->next = new node;
		currentPointer->next->input->next->next->next->next->s = Arg6;
		currentPointer->next->input->next->next->next->next->next=NULL;

		currentPointer->next->input->next->next->next->next->next = new node;
		currentPointer->next->input->next->next->next->next->next->s = Arg7;
		currentPointer->next->input->next->next->next->next->next->next=NULL;


		currentPointer->next->Output = Arg1;
		currentPointer->next->invert = 0;
		currentPointer->next->Type = 4;
		currentPointer->next->next=NULL;
		currentPointer->next->INS = INS;
		currentPointer = currentPointer->next;
	}





	return 0;
}



int netlistGenerator::FTCPEHandler(string INS, string Arg1, string Arg2, string Arg3, string Arg4, string Arg5, string Arg6, string Arg7){

	if(!currentPointer){
		currentPointer = new GateStruct;
		Gatehead = currentPointer;
		currentPointer->input = new node;
		currentPointer->input->next = NULL;
		currentPointer->input->s = Arg2;

		currentPointer->input->next = new node;
		currentPointer->input->next->s = Arg3;
		currentPointer->input->next->next=NULL;


		currentPointer->input->next->next = new node;
		currentPointer->input->next->next->s = Arg4;
		currentPointer->input->next->next->next=NULL;


		currentPointer->input->next->next->next = new node;
		currentPointer->input->next->next->next->s = Arg5;
		currentPointer->input->next->next->next->next=NULL;


		currentPointer->input->next->next->next->next = new node;
		currentPointer->input->next->next->next->next->s = Arg6;
		currentPointer->input->next->next->next->next->next=NULL;

		currentPointer->input->next->next->next->next->next = new node;
		currentPointer->input->next->next->next->next->next->s = Arg7;
		currentPointer->input->next->next->next->next->next->next=NULL;

		currentPointer->Output = Arg1;
		currentPointer->invert = 0;
		currentPointer->Type = 5;
		currentPointer->next=NULL;
		currentPointer->INS = INS;
	}else{
		currentPointer->next = new GateStruct;
		currentPointer->next->input = new node;
		currentPointer->next->input->next = NULL;
		currentPointer->next->input->s = Arg2;

		currentPointer->next->input->next = new node;
		currentPointer->next->input->next->s = Arg3;
		currentPointer->next->input->next->next=NULL;


		currentPointer->next->input->next->next = new node;
		currentPointer->next->input->next->next->s = Arg4;
		currentPointer->next->input->next->next->next=NULL;


		currentPointer->next->input->next->next->next = new node;
		currentPointer->next->input->next->next->next->s = Arg5;
		currentPointer->next->input->next->next->next->next=NULL;


		currentPointer->next->input->next->next->next->next = new node;
		currentPointer->next->input->next->next->next->next->s = Arg6;
		currentPointer->next->input->next->next->next->next->next=NULL;

		currentPointer->next->input->next->next->next->next->next = new node;
		currentPointer->next->input->next->next->next->next->next->s = Arg7;
		currentPointer->next->input->next->next->next->next->next->next=NULL;


		currentPointer->next->Output = Arg1;
		currentPointer->next->invert = 0;
		currentPointer->next->Type = 5;
		currentPointer->next->next=NULL;
		currentPointer->next->INS = INS;
		currentPointer = currentPointer->next;
	}
	return 0;
}


int netlistGenerator::FDDCPEHandler(string INS, string Arg1, string Arg2, string Arg3, string Arg4, string Arg5, string Arg6, string Arg7){

	if(!currentPointer){
		currentPointer = new GateStruct;
		Gatehead = currentPointer;
		currentPointer->input = new node;
		currentPointer->input->next = NULL;
		currentPointer->input->s = Arg2;

		currentPointer->input->next = new node;
		currentPointer->input->next->s = Arg3;
		currentPointer->input->next->next=NULL;


		currentPointer->input->next->next = new node;
		currentPointer->input->next->next->s = Arg4;
		currentPointer->input->next->next->next=NULL;


		currentPointer->input->next->next->next = new node;
		currentPointer->input->next->next->next->s = Arg5;
		currentPointer->input->next->next->next->next=NULL;


		currentPointer->input->next->next->next->next = new node;
		currentPointer->input->next->next->next->next->s = Arg6;
		currentPointer->input->next->next->next->next->next=NULL;

		currentPointer->input->next->next->next->next->next = new node;
		currentPointer->input->next->next->next->next->next->s = Arg7;
		currentPointer->input->next->next->next->next->next->next=NULL;

		currentPointer->Output = Arg1;
		currentPointer->invert = 0;
		currentPointer->Type = 6;
		currentPointer->next=NULL;
		currentPointer->INS = INS;
	}else{
		currentPointer->next = new GateStruct;
		currentPointer->next->input = new node;
		currentPointer->next->input->next = NULL;
		currentPointer->next->input->s = Arg2;

		currentPointer->next->input->next = new node;
		currentPointer->next->input->next->s = Arg3;
		currentPointer->next->input->next->next=NULL;


		currentPointer->next->input->next->next = new node;
		currentPointer->next->input->next->next->s = Arg4;
		currentPointer->next->input->next->next->next=NULL;


		currentPointer->next->input->next->next->next = new node;
		currentPointer->next->input->next->next->next->s = Arg5;
		currentPointer->next->input->next->next->next->next=NULL;


		currentPointer->next->input->next->next->next->next = new node;
		currentPointer->next->input->next->next->next->next->s = Arg6;
		currentPointer->next->input->next->next->next->next->next=NULL;

		currentPointer->next->input->next->next->next->next->next = new node;
		currentPointer->next->input->next->next->next->next->next->s = Arg7;
		currentPointer->next->input->next->next->next->next->next->next=NULL;


		currentPointer->next->Output = Arg1;
		currentPointer->next->invert = 0;
		currentPointer->next->Type = 6;
		currentPointer->next->next=NULL;
		currentPointer->next->INS = INS;
		currentPointer = currentPointer->next;
	}
	return 0;
}


int netlistGenerator::LDCPHandler(string INS, string Arg1, string Arg2, string Arg3, string Arg4, string Arg5, string Arg6){

	if(!currentPointer){
		currentPointer = new GateStruct;
		Gatehead = currentPointer;
		currentPointer->input = new node;
		currentPointer->input->next = NULL;
		currentPointer->input->s = Arg2;

		currentPointer->input->next = new node;
		currentPointer->input->next->s = Arg3;
		currentPointer->input->next->next=NULL;


		currentPointer->input->next->next = new node;
		currentPointer->input->next->next->s = Arg4;
		currentPointer->input->next->next->next=NULL;


		currentPointer->input->next->next->next = new node;
		currentPointer->input->next->next->next->s = Arg5;
		currentPointer->input->next->next->next->next=NULL;


		currentPointer->input->next->next->next->next = new node;
		currentPointer->input->next->next->next->next->s = Arg6;
		currentPointer->input->next->next->next->next->next=NULL;

		currentPointer->Output = Arg1;
		currentPointer->invert = 0;
		currentPointer->Type = 8;
		currentPointer->next=NULL;
		currentPointer->INS = INS;
	}else{
		currentPointer->next = new GateStruct;
		currentPointer->next->input = new node;
		currentPointer->next->input->next = NULL;
		currentPointer->next->input->s = Arg2;

		currentPointer->next->input->next = new node;
		currentPointer->next->input->next->s = Arg3;
		currentPointer->next->input->next->next=NULL;


		currentPointer->next->input->next->next = new node;
		currentPointer->next->input->next->next->s = Arg4;
		currentPointer->next->input->next->next->next=NULL;


		currentPointer->next->input->next->next->next = new node;
		currentPointer->next->input->next->next->next->s = Arg5;
		currentPointer->next->input->next->next->next->next=NULL;


		currentPointer->next->input->next->next->next->next = new node;
		currentPointer->next->input->next->next->next->next->s = Arg6;
		currentPointer->next->input->next->next->next->next->next=NULL;

		currentPointer->next->Output = Arg1;
		currentPointer->next->invert = 0;
		currentPointer->next->Type = 8;
		currentPointer->next->next=NULL;
		currentPointer->next->INS = INS;
		currentPointer = currentPointer->next;
	}
	return 0;
}




//insert the input file into array
//insert the input file into array
void netlistGenerator::assignHandler(string commandline, long int eqNumber)
{


	int cmdCounter = 0;

	if(commandline.size() == 0) {error_type5(eqNumber); return;}

	commandline = erase_space(commandline);
	int i= commandline.find("=");

	string Output_temp = commandline.substr(0, i);
	commandline.erase(0, i+1);

	stack<string> Stack_temp;
	stack<string> Stack1;

	while( commandline.substr(0, 1) != ";"){
		if((commandline.substr(0, 1) == "!") || (commandline.substr(0, 1) == "(") || (commandline.substr(0, 1) == ")") || (commandline.substr(0, 1) == "^")){
			Stack1.push(commandline.substr(0, 1));
			commandline.erase(0, 1);
		} else{
			if((commandline.substr(0, 2) == "&&") || (commandline.substr(0, 2) == "||")){
				Stack1.push(commandline.substr(0, 2));
				commandline.erase(0, 2);
			}else{
				if((commandline.substr(0, 4) == "1'b0") || (commandline.substr(0, 4) == "1'b1")  || (commandline.substr(0, 4) == "1'bZ")){
				Stack1.push(commandline.substr(0, 4));
				commandline.erase(0, 4);
				} else{
					int x1 = commandline.find("(");
					int x2 = commandline.find(")");
					int x3 = commandline.find("!");
					int x4 = commandline.find("&&");
					int x5 = commandline.find("||");
					int x6 = commandline.find("^");
					int x7 = commandline.find("?");
					int x8 = commandline.find(":");
					int x9 = commandline.find(";");
					if(minNum(x1, x2, x3, x4, x5, x6, x7, x8, x9) == -1) {error_type6(eqNumber); return;}
					string strTemp = commandline.substr(0, minNum(x1, x2, x3, x4, x5, x6, x7, x8, x9));
					Stack1.push(strTemp);
					commandline.erase(0, minNum(x1, x2, x3, x4, x5, x6, x7, x8, x9));
				}
			}
		}
	}


	//For one option
	if(Stack1.size() == 1){

		if (Stack1.top() == "1'b0" && Stack1.top() == "1'b1" && Stack1.top() == "1'bZ" && Stack1.top().find("?") != -1) warnning_type2(eqNumber);
		creat_BUFFER(Output_temp, Stack1.top(), eqNumber);

		return;
	}

	stack<string> Stack_temp2;

	while(Stack1.size() != 0){
		Stack_temp2.push(Stack1.top());
		Stack1.pop();
	}

	//Handling inverters
	while(Stack_temp2.size() != 0){
		if(Stack_temp2.top() == "!"){
			Stack_temp2.pop();
			if(Stack_temp2.top() != "(" && Stack_temp2.top() != "1'b0" && Stack_temp2.top() != "1'b1" && Stack_temp2.top() != "1'bZ" && Stack_temp2.top() != "!"){

				string st_temp = Stack_temp2.top();
				Stack_temp2.pop();
				Stack_temp2.push(creat_NOT(st_temp, eqNumber));

			}else{
				Stack1.push("!");
			}
		}
		Stack1.push(Stack_temp2.top());
		Stack_temp2.pop();
	}

	bool ValidGate = 0;

	int k = Stack1.size();

	int delete_flag = 0; GateStruct* Lastdeleted = NULL;

	while (Stack1.size() != 0){

		string Temp = Stack1.top();

		if(Temp == "(") {
			string Temp2 = Stack_temp.top();

			node* currentInput;

			if(!currentPointer){
				currentPointer = new GateStruct;
				Gatehead = currentPointer;
				currentPointer->input = new node;
				currentInput = currentPointer->input;
				currentPointer->input->next = NULL;
				currentPointer->input->s = Stack_temp.top();
				currentPointer->Output = Stack_temp.top();
				currentPointer->invert = 0;currentPointer->Type = 0;
				currentPointer->next=NULL;
				currentPointer->INS = "";
				ValidGate = 1;
			}else{
				currentPointer->next = new GateStruct;
				currentPointer->next->input = new node;
				currentInput = currentPointer->next->input;
				currentPointer->next->input->next = NULL;
				currentPointer->next->input->s = Stack_temp.top();
				currentPointer->next->Output = Stack_temp.top();
				currentPointer->next->invert = 0;currentPointer->next->Type = 0;
				currentPointer->next->next=NULL;
				currentPointer->next->INS = "";
				ValidGate = 1;
			}

			while(Temp2 != ")"){

				Temp2 = Stack_temp.top();
				Stack_temp.pop();


				//-------------------------------------------------------------------------
				if (Temp2 == "&&"){
					if(!currentPointer->next){
						currentPointer->Type = 1;
						currentPointer->Output.insert(0, "_AND_");
						currentInput->next = new node;
						currentInput = currentInput->next;
						currentInput->s = Stack_temp.top();
						currentInput->next = NULL;
						currentPointer->Output.insert(0, Stack_temp.top());
						currentPointer->next=NULL;
						Stack_temp.pop();
						Stack_temp.push(currentPointer->Output);
					}else{
						currentPointer->next->Type = 1;
						currentPointer->next->Output.insert(0, "_AND_");
						currentInput->next = new node;
						currentInput = currentInput->next;
						currentInput->s = Stack_temp.top();
						currentInput->next = NULL;
						currentPointer->next->Output.insert(0, Stack_temp.top());
						currentPointer->next->next=NULL;
						Stack_temp.pop();
						Stack_temp.push(currentPointer->next->Output);
					}
				}
				//--------------------------------------------------------------------------
				//-------------------------------------------------------------------------
				if (Temp2 == "||"){
					if(!currentPointer->next){
						currentPointer->Type = 2;
						currentPointer->Output.insert(0, "_OR_");
						currentInput->next = new node;
						currentInput = currentInput->next;
						currentInput->s = Stack_temp.top();
						currentInput->next = NULL;
						currentPointer->Output.insert(0, Stack_temp.top());
						currentPointer->next=NULL;
						Stack_temp.pop();
						Stack_temp.push(currentPointer->Output);
					}else{
						currentPointer->next->Type = 2;
						currentPointer->next->Output.insert(0, "_OR_");
						currentInput->next = new node;
						currentInput = currentInput->next;
						currentInput->s = Stack_temp.top();
						currentInput->next = NULL;
						currentPointer->next->Output.insert(0, Stack_temp.top());
						currentPointer->next->next=NULL;
						Stack_temp.pop();
						Stack_temp.push(currentPointer->next->Output);
					}
				}
				//--------------------------------------------------------------------------
				//-------------------------------------------------------------------------
				if (Temp2 == "^"){
					if(!currentPointer->next){
						currentPointer->Type = 3;
						currentPointer->Output.insert(0, "_XOR_");
						currentInput->next = new node;
						currentInput = currentInput->next;
						currentInput->s = Stack_temp.top();
						currentInput->next = NULL;
						currentPointer->Output.insert(0, Stack_temp.top());
						currentPointer->next=NULL;
						Stack_temp.pop();
						Stack_temp.push(currentPointer->Output);
					}else{
						currentPointer->next->Type = 3;
						currentPointer->next->Output.insert(0, "_XOR_");
						currentInput->next = new node;
						currentInput = currentInput->next;
						currentInput->s = Stack_temp.top();
						currentInput->next = NULL;
						currentPointer->next->Output.insert(0, Stack_temp.top());
						currentPointer->next->next=NULL;
						Stack_temp.pop();
						Stack_temp.push(currentPointer->next->Output);
					}
				}
				//--------------------------------------------------------------------------
			}
			if(!currentPointer->next){Stack_temp.push(currentPointer->Output);}else{Stack_temp.push(currentPointer->next->Output);}
		} else{
			if(Temp == "!")	{
				if(!currentPointer){
					string st_temp = creat_NOT(Stack_temp.top(), eqNumber);
					Stack_temp.pop();
					Stack_temp.push(st_temp);

				}else{
					if(!currentPointer->next){
						if(!delete_flag){
							if(currentPointer->invert  == 1){
								currentPointer->invert = 0;
								if(currentPointer->Type == 0){
									currentPointer->Output.erase(0, 4);
									Stack_temp.pop();
									if(!currentPointer->next){Stack_temp.push(currentPointer->Output);}else{Stack_temp.push(currentPointer->next->Output);}
								}
							}
							else{
								if(currentPointer->Type == 0){
									currentPointer->Output.insert(0, "NOT_");
									Stack_temp.pop();
									if(!currentPointer->next){Stack_temp.push(currentPointer->Output);}else{Stack_temp.push(currentPointer->next->Output);}
								}
								currentPointer->invert = 1;
							}
						}else{
							if(Lastdeleted->invert  == 1){
								Lastdeleted->invert = 0;
								if(Lastdeleted->Type == 0){
									Lastdeleted->Output.erase(0, 4);
									Stack_temp.pop();
									if(!Lastdeleted->next){Stack_temp.push(Lastdeleted->Output);}else{Stack_temp.push(Lastdeleted->next->Output);}
								}
							}
							else{
								if(Lastdeleted->Type == 0){
									Lastdeleted->Output.insert(0, "NOT_");
									Stack_temp.pop();
									if(!Lastdeleted->next){Stack_temp.push(Lastdeleted->Output);}else{Stack_temp.push(Lastdeleted->next->Output);}
								}
								Lastdeleted->invert = 1;
							}
						}

					}else{
						if(currentPointer->next->invert  == 1){
							currentPointer->next->invert = 0;
							if(currentPointer->Type == 0){
								currentPointer->Output.erase(0, 4);
								Stack_temp.pop();
								if(!currentPointer->next){Stack_temp.push(currentPointer->Output);}else{Stack_temp.push(currentPointer->next->Output);}
							}
						}
						else{
							currentPointer->next->invert = 1;
							if(currentPointer->Type == 0){
								currentPointer->Output.insert(0, "NOT_");
								Stack_temp.pop();
								if(!currentPointer->next){Stack_temp.push(currentPointer->Output);}else{Stack_temp.push(currentPointer->next->Output);}
							}
						}
					}
				}
			}else{

				Stack_temp.push(Temp);
			}
		}

		if(ValidGate) {
			if(checkingGate(eqNumber)){
				if(!currentPointer->next){
					Lastdeleted = new GateStruct;
					node* nodeTemp = currentPointer->input;
					Lastdeleted->input = new node;
					node* nodeTemp2 =Lastdeleted->input;
					nodeTemp2->s = nodeTemp->s;
					nodeTemp2->next = NULL;
					nodeTemp = nodeTemp->next;
					while(nodeTemp){
						nodeTemp2->next = new node;
						nodeTemp2->next->s = nodeTemp->s;
						nodeTemp2->next->next = NULL;
						nodeTemp = nodeTemp->next;
						nodeTemp2 = nodeTemp2->next;
					}
					Lastdeleted->invert = currentPointer->invert;
					Lastdeleted->next = NULL;
					Lastdeleted->Output= currentPointer->Output;
					Lastdeleted->Type = currentPointer->Type;
				}else{
					Lastdeleted = new GateStruct;
					node* nodeTemp = currentPointer->next->input;
					Lastdeleted->input = new node;
					node* nodeTemp2 =Lastdeleted->input;
					nodeTemp2->s = nodeTemp->s;
					nodeTemp2->next = NULL;
					nodeTemp = nodeTemp->next;
					while(nodeTemp){
						nodeTemp2->next = new node;
						nodeTemp2->next->s = nodeTemp->s;
						nodeTemp2->next->next = NULL;
						nodeTemp = nodeTemp->next;
						nodeTemp2 = nodeTemp2->next;
					}
					Lastdeleted->invert = currentPointer->next->invert;
					Lastdeleted->next = NULL;
					Lastdeleted->Output= currentPointer->next->Output;
					Lastdeleted->Type = currentPointer->next->Type;
				}
				deleteGate();
				delete_flag = 1;
			}
			else {
				delete_flag = 0;
				if(!currentPointer->next){}else{
					delete_flag = 0;
					ValidGate = 0; currentPointer = currentPointer->next;
				}
			}
			ValidGate = 0;
		}
		Stack1.pop();

	}//End of While

	if(Stack_temp.size() == 1){
		if(delete_flag){
			currentPointer->next = Lastdeleted;
			currentPointer = currentPointer->next;
			currentPointer->Output = Output_temp;
		}else{
			delete(Lastdeleted);

			if(ValidGate) {
					if(checkingGate(eqNumber)){
					if(!currentPointer->next){}else{
						currentPointer = currentPointer->next;
						currentPointer->Output = Output_temp;
					}
				} else{
					if(!currentPointer->next){}else{
						currentPointer = currentPointer->next;
						currentPointer->Output = Output_temp;
					}
				}
			} else{
				currentPointer->Output = Output_temp;
			}
		}

		ValidGate = 0;
		return;
	}

	if(Stack_temp.size() > 1){
	node* currentInput;
	string Temp2 = Stack_temp.top();
		if(!currentPointer){
			currentPointer = new GateStruct;
			Gatehead = currentPointer;
			currentPointer->input = new node;
			currentInput = currentPointer->input;
			currentPointer->input->next = NULL;
			currentPointer->input->s = Stack_temp.top();
			currentPointer->Output = Stack_temp.top();
			currentPointer->invert = 0;currentPointer->Type = 0;
			currentPointer->next=NULL;
			currentPointer->INS = "";
			ValidGate = 1;
		}else{
			currentPointer->next = new GateStruct;
			currentPointer->next->input = new node;
			currentInput = currentPointer->next->input;
			currentPointer->next->input->next = NULL;
			currentPointer->next->input->s = Stack_temp.top();
			currentPointer->next->Output = Stack_temp.top();
			currentPointer->next->invert = 0;currentPointer->next->Type = 0;
			currentPointer->next->next=NULL;
			currentPointer->next->INS = "";
			ValidGate = 1;
		}

		while(Stack_temp.size() > 1){

			Temp2 = Stack_temp.top();
			Stack_temp.pop();


			//-------------------------------------------------------------------------
			if (Temp2 == "&&"){
				if(!currentPointer->next){
					currentPointer->Type = 1;
					currentPointer->Output.insert(0, "_AND_");
					currentInput->next = new node;
					currentInput = currentInput->next;
					currentInput->s = Stack_temp.top();
					currentInput->next = NULL;
					currentPointer->Output.insert(0, Stack_temp.top());
					currentPointer->next=NULL;
					Stack_temp.pop();
					Stack_temp.push(currentPointer->Output);
				}else{
					currentPointer->next->Type = 1;
					currentPointer->next->Output.insert(0, "_AND_");
					currentInput->next = new node;
					currentInput = currentInput->next;
					currentInput->s = Stack_temp.top();
					currentInput->next = NULL;
					currentPointer->next->Output.insert(0, Stack_temp.top());
					currentPointer->next->next=NULL;
					Stack_temp.pop();
					Stack_temp.push(currentPointer->next->Output);
				}
			}
			//--------------------------------------------------------------------------
			//-------------------------------------------------------------------------
			if (Temp2 == "||"){
				if(!currentPointer->next){
					currentPointer->Type = 2;
					currentPointer->Output.insert(0, "_OR_");
					currentInput->next = new node;
					currentInput = currentInput->next;
					currentInput->s = Stack_temp.top();
					currentInput->next = NULL;
					currentPointer->Output.insert(0, Stack_temp.top());
					currentPointer->next=NULL;
					Stack_temp.pop();
					Stack_temp.push(currentPointer->Output);
				}else{
					currentPointer->next->Type = 2;
					currentPointer->next->Output.insert(0, "_OR_");
					currentInput->next = new node;
					currentInput = currentInput->next;
					currentInput->s = Stack_temp.top();
					currentInput->next = NULL;
					currentPointer->next->Output.insert(0, Stack_temp.top());
					currentPointer->next->next=NULL;
					Stack_temp.pop();
					Stack_temp.push(currentPointer->next->Output);
				}
			}
			//--------------------------------------------------------------------------
			//-------------------------------------------------------------------------
			if (Temp2 == "^"){
				if(!currentPointer->next){
					currentPointer->Type = 3;
					currentPointer->Output.insert(0, "_XOR_");
					currentInput->next = new node;
					currentInput = currentInput->next;
					currentInput->s = Stack_temp.top();
					currentInput->next = NULL;
					currentPointer->Output.insert(0, Stack_temp.top());
					currentPointer->next=NULL;
					Stack_temp.pop();
					Stack_temp.push(currentPointer->Output);
				}else{
					currentPointer->next->Type = 3;
					currentPointer->next->Output.insert(0, "_XOR_");
					currentInput->next = new node;
					currentInput = currentInput->next;
					currentInput->s = Stack_temp.top();
					currentInput->next = NULL;
					currentPointer->next->Output.insert(0, Stack_temp.top());
					currentPointer->next->next=NULL;
					Stack_temp.pop();
					Stack_temp.push(currentPointer->next->Output);
				}
			}
		//--------------------------------------------------------------------------
		}

		if(!currentPointer->next){Stack_temp.push(currentPointer->Output);}else{Stack_temp.push(currentPointer->next->Output);}


		if(ValidGate) {
			if(checkingGate(eqNumber)){
				if(!currentPointer->next){}else{
					currentPointer = currentPointer->next;
					currentPointer->Output = Output_temp;
				}
			} //deleteGate();
			else{
				if(!currentPointer->next){}else{
					currentPointer = currentPointer->next;
					currentPointer->Output = Output_temp;
				}
			}
		} else{
			currentPointer->Output = Output_temp;
		}
		ValidGate = 0;
		return;
	}


return;


}//end getfile





void netlistGenerator::printGate(){
	GateStruct* Temp = Gatehead;
	ofstream FO_TEMP("Test.txt");
	while(Temp){
		FO_TEMP<<Temp->Output<<" = ";
		if(Temp->Type ==0 && Temp->invert == 0){FO_TEMP<<"BUF(";}
		if(Temp->Type ==0 && Temp->invert == 1){FO_TEMP<<"NOT(";}
		if(Temp->Type ==1 && Temp->invert == 0){FO_TEMP<<"AND(";}
		if(Temp->Type ==1 && Temp->invert == 1){FO_TEMP<<"NAND(";}
		if(Temp->Type ==2 && Temp->invert == 0){FO_TEMP<<"OR(";}
		if(Temp->Type ==2 && Temp->invert == 1){FO_TEMP<<"NOR(";}
		if(Temp->Type ==3 && Temp->invert == 0){FO_TEMP<<"XOR(";}
		if(Temp->Type ==3 && Temp->invert == 1){FO_TEMP<<"XNOR(";}


		FO_TEMP<<Temp->input->s;

		node* currentInput = Temp->input->next;

		while(currentInput){
			FO_TEMP<<", "<<currentInput->s;
			currentInput = currentInput->next;
		}

		FO_TEMP<<")"<<endl;
		Temp = Temp->next;
	}
	return;


}

void netlistGenerator::printGate_preGateWireAnalyzer(){
	GateStruct* Temp = Gatehead;
	ofstream FO_TEMP("pre_Test.txt");
	while(Temp){
		FO_TEMP<<Temp->Output<<" = ";
		if(Temp->Type ==0 && Temp->invert == 0){FO_TEMP<<"BUF(";}
		if(Temp->Type ==0 && Temp->invert == 1){FO_TEMP<<"NOT(";}
		if(Temp->Type ==1 && Temp->invert == 0){FO_TEMP<<"AND(";}
		if(Temp->Type ==1 && Temp->invert == 1){FO_TEMP<<"NAND(";}
		if(Temp->Type ==2 && Temp->invert == 0){FO_TEMP<<"OR(";}
		if(Temp->Type ==2 && Temp->invert == 1){FO_TEMP<<"NOR(";}
		if(Temp->Type ==3 && Temp->invert == 0){FO_TEMP<<"XOR(";}
		if(Temp->Type ==3 && Temp->invert == 1){FO_TEMP<<"XNOR(";}


		FO_TEMP<<Temp->input->s;

		node* currentInput = Temp->input->next;

		while(currentInput){
			FO_TEMP<<", "<<currentInput->s;
			currentInput = currentInput->next;
		}

		FO_TEMP<<")"<<endl;
		Temp = Temp->next;
	}
	return;


}

void netlistGenerator::clkEnableHn(){//removing CE

	GateStruct* gateTemp = Gatehead;
	while(gateTemp){

		if(gateTemp->CE != "" && gateTemp->Type == 4){
			
			int aTemp = 0; //just for polymorphism
			
			string strTemp1 = gateTemp->CE;
			strTemp1.insert(0, "NOT_");
			string OstrTemp1 = strTemp1;
			OstrTemp1.insert(OstrTemp1.length(), "_AND_");
			OstrTemp1.insert(OstrTemp1.length(), gateTemp->Q);
			
			if(!checkingWire(OstrTemp1, aTemp)){
				//creating AND1
				GateStruct* gateTemp2 = currentPointer;
				gateTemp2->next = new GateStruct;
				gateTemp2->next->Type = 1;
				gateTemp2->next->invert = 0;
				gateTemp2->next->input = new node;
				gateTemp2->next->input->s = gateTemp->Q;
				gateTemp2->next->input->next = new node;
				gateTemp2->next->input->next->s = strTemp1;
				gateTemp2->next->input->next->next = NULL;
				gateTemp2->next->Output = OstrTemp1;
				gateTemp2 = gateTemp2->next;
				currentPointer = currentPointer->next;
			}


			string strTemp2 = gateTemp->CE;
			string OstrTemp2 = strTemp2;
			OstrTemp2.insert(OstrTemp2.length(), "_AND_");
			OstrTemp2.insert(OstrTemp2.length(), gateTemp->D);
			

			if(!checkingWire(OstrTemp2, aTemp)){
				//creating AND2
				GateStruct* gateTemp2 = currentPointer;
				gateTemp2->next = new GateStruct;
				gateTemp2->next->Type = 1;
				gateTemp2->next->invert = 0;
				gateTemp2->next->input = new node;
				gateTemp2->next->input->s = gateTemp->CE;
				gateTemp2->next->input->next = new node;
				gateTemp2->next->input->next->s = gateTemp->D;
				gateTemp2->next->input->next->next = NULL;
				gateTemp2->next->Output = OstrTemp2;
				gateTemp2 = gateTemp2->next;
				currentPointer = currentPointer->next;
			}

			

			string strTemp3 = OstrTemp1;
			string OstrTemp3 = strTemp3;
			OstrTemp3.insert(OstrTemp3.length(), "_OR_");
			OstrTemp3.insert(OstrTemp3.length(), OstrTemp2);
			

			if(!checkingWire(OstrTemp3, aTemp)){
				//Creating OR1
				GateStruct* gateTemp2 = currentPointer;
				gateTemp2->next = new GateStruct;
				gateTemp2->next->Type = 2;
				gateTemp2->next->invert = 0;
				gateTemp2->next->input = new node;
				gateTemp2->next->input->s = OstrTemp1;
				gateTemp2->next->input->next = new node;
				gateTemp2->next->input->next->s = OstrTemp2;
				gateTemp2->next->input->next->next = NULL;
				gateTemp2->next->Output = OstrTemp3;
				gateTemp2 = gateTemp2->next;
				currentPointer = currentPointer->next;
			}


			string OstrTemp4 = gateTemp->CE;
			OstrTemp4.insert(0, "NOT_");
			

			if(!checkingWire(OstrTemp4, aTemp)){
				//Creating NOT
				GateStruct* gateTemp2 = currentPointer;
				gateTemp2->next = new GateStruct;
				gateTemp2->next->Type = 0;
				gateTemp2->next->invert = 1;
				gateTemp2->next->input = new node;
				gateTemp2->next->input->s = gateTemp->CE;
				gateTemp2->next->input->next = NULL;
				gateTemp2->next->Output = OstrTemp4;
				gateTemp2 = gateTemp2->next;
				currentPointer = currentPointer->next;
			}

			gateTemp->D = OstrTemp3;

			gateTemp->CE = "1'b1";
		}
		
		gateTemp = gateTemp->next;
	}
	return;
}

int netlistGenerator::checkingFloatBuf(string s){
	
	GateStruct* gateTemp = Gatehead;
	
	while(gateTemp){
		
		if(gateTemp->Output == s && gateTemp->Type != 5 && gateTemp->Type != 6  && gateTemp->Type != 7)
			return 1;
		if(gateTemp->Q == s && (gateTemp->Type == 5 || gateTemp->Type == 6  || gateTemp->Type == 7))
				return 1;

		gateTemp = gateTemp->next;
	}

	node* nodeTemp = EX_Inputs_1;
	
	while(nodeTemp){
		if(nodeTemp->s == s) return 1;
		nodeTemp = nodeTemp->next;
	}

	return 0;

}




void netlistGenerator::bufferRmv(){//removing buffer
	
	
	
	GateStruct* Temp = Gatehead;
	GateStruct* Temp2 = Gatehead;
	cout<<"\nAnalyzing the gate...";
	
	
	while(Temp){
		if(Temp->Type == 0 && Temp->invert == 0){

			string inputwire = "";
			if(Temp->input->next){
				node* nodeTemp2 = Temp->input;

				while(nodeTemp2){
					if(checkingFloatBuf(nodeTemp2->s)){
						inputwire = nodeTemp2->s;
						//node* nodeTemp = EX_Inputs_1;
						//int Flag = 0;
						//while(nodeTemp){
						//	if(nodeTemp->s == inputwire) {Flag = 1; break;}
						//	nodeTemp = nodeTemp->next;
						//}
						//if(!Flag)
						//	netReplace(inputwire, Temp->Output);
						//else
						//	netReplace(Temp->Output, inputwire);
						
						//if(Temp2 == Temp){
						//	Gatehead = Temp->next;
						//	delete(Temp);
						//	Temp = Gatehead;
						//	Temp2 = Gatehead;
						//}else{
						//	Temp2->next = Temp->next;
						//		delete(Temp);
						//	Temp = Temp2->next;
						//}	

						break;
					}
					nodeTemp2 = nodeTemp2->next;
				}
				if(inputwire == ""){
					cout<<"Warnning : A floating wire detected that do not have any driver...\n";
					stack<node*> ST1;
					nodeTemp2 = Temp->input;
					while(nodeTemp2){
						ST1.push(nodeTemp2);
						nodeTemp2 = nodeTemp2->next;
					}
					while(ST1.size() >0){
						delete(ST1.top());
						ST1.pop();
					}

					Temp->input->s = inputwire;
					Temp->input->next = NULL;
				}
			}
		}
		//}else{
		//	Temp2 = Temp;
		//	Temp = Temp->next;
		//}
		Temp = Temp->next;
	}


	
	Temp = Gatehead;
	
	cout<<"\nCreating supply..."<<endl;
	while(Temp){
		if(Temp->Type == 10 && Temp->invert == 0){
			if(!Temp->input){
				Temp->input = new node;
				Temp->input->s = "1'b0";
				Temp->input->next = NULL;
			}
		}
		
		if(Temp->Type == 11 && Temp->invert == 0){
			if(!Temp->input){
				Temp->input = new node;
				Temp->input->s = "1'b1";
				Temp->input->next = NULL;
			}
		}
		
		Temp = Temp->next;
		
	}
	return;
}


int netlistGenerator::CommandReader(){

	long int eqCounter = 0;
	FDCPEcnt = 0;
	FDDCPEcnt = 0;
	LDCPcnt = 0;
	FTDCPEcnt = 0;
	FTCPEcnt = 0;

	node* TempNode = nodeHead;

	while(TempNode){
		if( TempNode->s == "FDCPE"){
			FDCPEcnt++;
			string Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7;
			TempNode = TempNode->next;
			string INS = TempNode->s;
			TempNode = TempNode->next;
			if(TempNode->s == "("){
				TempNode = TempNode->next;
				//---------
				string Temp_t = "";
				while(TempNode->s != ","){
					Temp_t.insert(Temp_t.length(), TempNode->s);
					TempNode = TempNode->next;
				}
				Temp_t.insert(Temp_t.length(), ";");
				Arg1 = logicExtractor(Temp_t);
				//----------
				if(TempNode->s == ","){
					TempNode = TempNode->next;
					//---------
					string Temp_t = "";
					while(TempNode->s != ","){
						Temp_t.insert(Temp_t.length(), TempNode->s);
						TempNode = TempNode->next;
					}
					Temp_t.insert(Temp_t.length(), ";");
					Arg2 = logicExtractor(Temp_t);
					//----------
					if(TempNode->s == ","){
						TempNode = TempNode->next;
						//---------
						string Temp_t = "";
						while(TempNode->s != ","){
							Temp_t.insert(Temp_t.length(), TempNode->s);
							TempNode = TempNode->next;
						}
						Temp_t.insert(Temp_t.length(), ";");
						Arg3 = logicExtractor(Temp_t);
						//----------
						if(TempNode->s == ","){
							TempNode = TempNode->next;
							//---------
							string Temp_t = "";
							while(TempNode->s != ","){
								Temp_t.insert(Temp_t.length(), TempNode->s);
								TempNode = TempNode->next;
							}
							Temp_t.insert(Temp_t.length(), ";");
							Arg4 = logicExtractor(Temp_t);
							//----------
							if(TempNode->s == ","){
								TempNode = TempNode->next;
								//---------
								string Temp_t = "";
								while(TempNode->s != ","){
									Temp_t.insert(Temp_t.length(), TempNode->s);
									TempNode = TempNode->next;
								}
								Temp_t.insert(Temp_t.length(), ";");
								Arg5 = logicExtractor(Temp_t);
								//----------

								if(TempNode->s == ","){
									TempNode = TempNode->next;
									//---------
									string Temp_t = "";
									while(TempNode->s != ","){
										Temp_t.insert(Temp_t.length(), TempNode->s);
										TempNode = TempNode->next;
									}
									Temp_t.insert(Temp_t.length(), ";");
									Arg6 = logicExtractor(Temp_t);
									//----------
									if(TempNode->s == ","){
										TempNode = TempNode->next;
										//---------
										string Temp_t = "";
										while(TempNode->s != ")"){
											Temp_t.insert(Temp_t.length(), TempNode->s);
											TempNode = TempNode->next;
										}
										Temp_t.insert(Temp_t.length(), ";");
										Arg7 = logicExtractor(Temp_t);
										//----------
										if(TempNode->s == ")"){
											TempNode = TempNode->next;
											if(TempNode->s == ";"){
												TempNode = TempNode->next;
												FDCPEHandler(INS, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7);
											}else{
												error_type8_FDCPE(FDCPEcnt);return -1; break;
											}
										}else{
											error_type8_FDCPE(FDCPEcnt);return -1; break;
										}
									}else{
										error_type8_FDCPE(FDCPEcnt);return -1; break;
									}
								}else{
									error_type8_FDCPE(FDCPEcnt);return -1; break;
								}
							}else{
								error_type8_FDCPE(FDCPEcnt);return -1; break;
							}
						}else{
							error_type8_FDCPE(FDCPEcnt);return -1; break;
						}
					}else{
						error_type8_FDCPE(FDCPEcnt);return -1; break;
					}
				}else{
					error_type8_FDCPE(FDCPEcnt);return -1; break;
				}
			}else{
				error_type8_FDCPE(FDCPEcnt);return -1; break;
			}
		}else{
			if( TempNode->s == "FTCPE"){
				FTCPEcnt++;
				string Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7;
				TempNode = TempNode->next;
				string INS = TempNode->s;
				TempNode = TempNode->next;
				if(TempNode->s == "("){
					TempNode = TempNode->next;
					//---------
					string Temp_t = "";
					while(TempNode->s != ","){
						Temp_t.insert(Temp_t.length(), TempNode->s);
						TempNode = TempNode->next;
					}
					Temp_t.insert(Temp_t.length(), ";");
					Arg1 = logicExtractor(Temp_t);
					//----------
					if(TempNode->s == ","){
						TempNode = TempNode->next;
						//---------
						string Temp_t = "";
						while(TempNode->s != ","){
							Temp_t.insert(Temp_t.length(), TempNode->s);
							TempNode = TempNode->next;
						}
						Temp_t.insert(Temp_t.length(), ";");
						Arg2 = logicExtractor(Temp_t);
						//----------
						if(TempNode->s == ","){
							TempNode = TempNode->next;
							//---------
							string Temp_t = "";
							while(TempNode->s != ","){
								Temp_t.insert(Temp_t.length(), TempNode->s);
								TempNode = TempNode->next;
							}
							Temp_t.insert(Temp_t.length(), ";");
							Arg3 = logicExtractor(Temp_t);
							//----------
							if(TempNode->s == ","){
								TempNode = TempNode->next;
								//---------
								string Temp_t = "";
								while(TempNode->s != ","){
									Temp_t.insert(Temp_t.length(), TempNode->s);
									TempNode = TempNode->next;
								}
								Temp_t.insert(Temp_t.length(), ";");
								Arg4 = logicExtractor(Temp_t);
								//----------
								if(TempNode->s == ","){
									TempNode = TempNode->next;
									//---------
									string Temp_t = "";
									while(TempNode->s != ","){
										Temp_t.insert(Temp_t.length(), TempNode->s);
										TempNode = TempNode->next;
									}
									Temp_t.insert(Temp_t.length(), ";");
									Arg5 = logicExtractor(Temp_t);
									//----------
									if(TempNode->s == ","){
										TempNode = TempNode->next;
										//---------
										string Temp_t = "";
										while(TempNode->s != ","){
											Temp_t.insert(Temp_t.length(), TempNode->s);
											TempNode = TempNode->next;
										}
										Temp_t.insert(Temp_t.length(), ";");
										Arg6 = logicExtractor(Temp_t);
										//----------

										if(TempNode->s == ","){
											TempNode = TempNode->next;
											//---------
											string Temp_t = "";
											while(TempNode->s != ")"){
												Temp_t.insert(Temp_t.length(), TempNode->s);
												TempNode = TempNode->next;
											}
											Temp_t.insert(Temp_t.length(), ";");
											Arg7 = logicExtractor(Temp_t);
											//----------

											if(TempNode->s == ")"){
												TempNode = TempNode->next;
												if(TempNode->s == ";"){
													TempNode = TempNode->next;
													string X1_temp = Arg2;
													Arg2.insert(0, "_XOR_");
													Arg2.insert(0, Arg1);
													FDCPEHandler(INS, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7);
													string str_temp = "";
													str_temp.insert(str_temp.length(), Arg2);
													str_temp.insert(str_temp.length(), "=");
													str_temp.insert(str_temp.length(), Arg1);
													str_temp.insert(str_temp.length(), "^");
													str_temp.insert(str_temp.length(), X1_temp);
													str_temp.insert(str_temp.length(), ";");
													assignHandler(str_temp, eqCounter);

												}else{
													error_type8_FTCPE(FTCPEcnt);return -1; break;
												}
											}else{
												error_type8_FTCPE(FTCPEcnt);return -1; break;
											}
										}else{
											error_type8_FTCPE(FTCPEcnt);return -1; break;
										}
									}else{
										error_type8_FTCPE(FTCPEcnt);return -1; break;
									}
								}else{
									error_type8_FTCPE(FTCPEcnt);return -1; break;
								}
							}else{
								error_type8_FTCPE(FTCPEcnt);return -1; break;
							}
						}else{
							error_type8_FTCPE(FTCPEcnt);return -1; break;
						}
					}else{
						error_type8_FTCPE(FTCPEcnt);return -1; break;
					}
				}else{
					error_type8_FTCPE(FTCPEcnt);return -1; break;
				}
			}else{
				if( TempNode->s == "FDDCPE"){
					FDDCPEcnt++;
					string Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7;
					TempNode = TempNode->next;
					string INS = TempNode->s;
					TempNode = TempNode->next;
					if(TempNode->s == "("){
						TempNode = TempNode->next;
						//---------
						string Temp_t = "";
						while(TempNode->s != ","){
							Temp_t.insert(Temp_t.length(), TempNode->s);
							TempNode = TempNode->next;
						}
						Temp_t.insert(Temp_t.length(), ";");
						Arg1 = logicExtractor(Temp_t);
						//----------
						if(TempNode->s == ","){
							TempNode = TempNode->next;
							//---------
							string Temp_t = "";
							while(TempNode->s != ","){
								Temp_t.insert(Temp_t.length(), TempNode->s);
								TempNode = TempNode->next;
							}
							Temp_t.insert(Temp_t.length(), ";");
							Arg2 = logicExtractor(Temp_t);
							//----------
							if(TempNode->s == ","){
								TempNode = TempNode->next;
								//---------
								string Temp_t = "";
								while(TempNode->s != ","){
									Temp_t.insert(Temp_t.length(), TempNode->s);
									TempNode = TempNode->next;
								}
								Temp_t.insert(Temp_t.length(), ";");
								Arg3 = logicExtractor(Temp_t);
								//----------
								if(TempNode->s == ","){
									TempNode = TempNode->next;
									//---------
									string Temp_t = "";
									while(TempNode->s != ","){
										Temp_t.insert(Temp_t.length(), TempNode->s);
										TempNode = TempNode->next;
									}
									Temp_t.insert(Temp_t.length(), ";");
									Arg4 = logicExtractor(Temp_t);
									//----------
									if(TempNode->s == ","){
										TempNode = TempNode->next;
										//---------
										string Temp_t = "";
										while(TempNode->s != ","){
											Temp_t.insert(Temp_t.length(), TempNode->s);
											TempNode = TempNode->next;
										}
										Temp_t.insert(Temp_t.length(), ";");
										Arg5 = logicExtractor(Temp_t);
										//----------
										if(TempNode->s == ","){
											TempNode = TempNode->next;
											//---------
											string Temp_t = "";
											while(TempNode->s != ","){
												Temp_t.insert(Temp_t.length(), TempNode->s);
												TempNode = TempNode->next;
											}
											Temp_t.insert(Temp_t.length(), ";");
											Arg6 = logicExtractor(Temp_t);
											//----------

											if(TempNode->s == ","){
												TempNode = TempNode->next;
												//---------
												string Temp_t = "";
												while(TempNode->s != ")"){
													Temp_t.insert(Temp_t.length(), TempNode->s);
													TempNode = TempNode->next;
												}
												Temp_t.insert(Temp_t.length(), ";");
												Arg7 = logicExtractor(Temp_t);
												//----------
												if(TempNode->s == ")"){
													TempNode = TempNode->next;
													if(TempNode->s == ";"){
														TempNode = TempNode->next;
														FDDCPEHandler(INS, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7);
													}else{
														error_type8_FDDCPE(FDDCPEcnt);return -1; break;
													}
												}else{
													error_type8_FDDCPE(FDDCPEcnt);return -1; break;
												}
											}else{
												error_type8_FDDCPE(FDDCPEcnt);return -1; break;
											}
										}else{
											error_type8_FDDCPE(FDDCPEcnt);return -1; break;
										}
									}else{
										error_type8_FDDCPE(FDDCPEcnt);return -1; break;
									}
								}else{
									error_type8_FDDCPE(FDDCPEcnt);return -1; break;
								}
							}else{
								error_type8_FDDCPE(FDDCPEcnt);return -1; break;
							}
						}else{
							error_type8_FDDCPE(FDDCPEcnt);return -1; break;
						}
					}else{
						error_type8_FDDCPE(FDDCPEcnt);return -1; break;
					}
				}else{
					if( TempNode->s == "FTDCPE"){
					}else{
						if( TempNode->s == "LDCP"){
							LDCPcnt++;
							string Arg1, Arg2, Arg3, Arg4, Arg5, Arg6;
							TempNode = TempNode->next;
							string INS = TempNode->s;
							TempNode = TempNode->next;
							if(TempNode->s == "("){
								TempNode = TempNode->next;
								//---------
								string Temp_t = "";
								while(TempNode->s != ","){
									Temp_t.insert(Temp_t.length(), TempNode->s);
									TempNode = TempNode->next;
								}
								Temp_t.insert(Temp_t.length(), ";");
								Arg1 = logicExtractor(Temp_t);
								//----------
								if(TempNode->s == ","){
									TempNode = TempNode->next;
									//---------
									string Temp_t = "";
									while(TempNode->s != ","){
										Temp_t.insert(Temp_t.length(), TempNode->s);
										TempNode = TempNode->next;
									}
									Temp_t.insert(Temp_t.length(), ";");
									Arg2 = logicExtractor(Temp_t);
									//---------
									if(TempNode->s == ","){
										TempNode = TempNode->next;
										//---------
										string Temp_t = "";
										while(TempNode->s != ","){
											Temp_t.insert(Temp_t.length(), TempNode->s);
											TempNode = TempNode->next;
										}
										Temp_t.insert(Temp_t.length(), ";");
										Arg3 = logicExtractor(Temp_t);
										//---------
										if(TempNode->s == ","){
											TempNode = TempNode->next;
											//---------
											string Temp_t = "";
											while(TempNode->s != ","){
												Temp_t.insert(Temp_t.length(), TempNode->s);
												TempNode = TempNode->next;
											}
											Temp_t.insert(Temp_t.length(), ";");
											Arg4 = logicExtractor(Temp_t);
											//---------
											if(TempNode->s == ","){
												TempNode = TempNode->next;
												//---------
												string Temp_t = "";
												while(TempNode->s != ","){
													Temp_t.insert(Temp_t.length(), TempNode->s);
													TempNode = TempNode->next;
												}
												Temp_t.insert(Temp_t.length(), ";");
												Arg5 = logicExtractor(Temp_t);
												//---------
												if(TempNode->s == ","){
													TempNode = TempNode->next;
													//---------
													string Temp_t = "";
													while(TempNode->s != ")"){
														Temp_t.insert(Temp_t.length(), TempNode->s);
														TempNode = TempNode->next;
													}
													Temp_t.insert(Temp_t.length(), ";");
													Arg6 = logicExtractor(Temp_t);
													//---------
													if(TempNode->s == ")"){
														TempNode = TempNode->next;
														if(TempNode->s == ";"){
															TempNode = TempNode->next;
															LDCPHandler(INS, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6);
														}else{
															error_type8_LDCP(LDCPcnt);return -1; break;
														}
													}else{
														error_type8_LDCP(LDCPcnt);return -1; break;
													}
												}else{
													error_type8_LDCP(LDCPcnt);return -1; break;
												}
											}else{
												error_type8_LDCP(LDCPcnt);return -1; break;
											}
										}else{
											error_type8_LDCP(LDCPcnt);return -1; break;
										}
									}else{
										error_type8_LDCP(LDCPcnt);return -1; break;
									}
								}else{
									error_type8_LDCP(LDCPcnt);return -1; break;
								}
							}else{
								error_type8_LDCP(LDCPcnt);return -1; break;
							}
						}else{
							string commandline = "";
							int flag = 0;
							while(TempNode->s !=";"){
								commandline.insert(commandline.length(), TempNode->s);
								if(TempNode->s == "?") flag = 1;
								TempNode = TempNode->next;
							}
							if(TempNode->s == ";"){
								TempNode = TempNode->next;
								commandline.insert(commandline.length(), ";");
							}
							eqCounter++;
							if(flag) commandline = QHandler(commandline);
							assignHandler(commandline, eqCounter);
						}
					}
				}
			}
		}
	}

	cout<<"\n**************************************************\n";
	cout<<"The process of extracting equations is completed.\n";
	cout<<"This design has "<<eqCounter<<" non extracted equations.\n";
	cout<<"This design has "<<FDCPEcnt<<" FDCPE(s) storage memory.\n";
	cout<<"This design has "<<FDDCPEcnt<<" FDDCPE(s) storage memory."<<endl;
	cout<<"This design has "<<FTCPEcnt<<" FTCPE(s) storage memory."<<endl;
	cout<<"This design has "<<FTDCPEcnt<<" FTDCPE(s) storage memory."<<endl;
	cout<<"This design has "<<LDCPcnt<<" LDCP(s) storage memory.\n";
	cout<<"you can check the equations in "<<equationFile<<" File.\n";




	return 0;
}

string netlistGenerator::logicExtractor(string equation){



	stack<string> Stack1;
	stack<string> Stack_temp;
	string commandline = equation;

	while( commandline.substr(0, 1) != ";"){
		if((commandline.substr(0, 1) == "!") || (commandline.substr(0, 1) == "(") || (commandline.substr(0, 1) == ")") || (commandline.substr(0, 1) == "^")){
			Stack1.push(commandline.substr(0, 1));
			commandline.erase(0, 1);
		} else{
			if((commandline.substr(0, 2) == "&&") || (commandline.substr(0, 2) == "||")){
				Stack1.push(commandline.substr(0, 2));
				commandline.erase(0, 2);
			}else{
				if((commandline.substr(0, 4) == "1'b0") || (commandline.substr(0, 4) == "1'b1")  || (commandline.substr(0, 4) == "1'bZ")){
				Stack1.push(commandline.substr(0, 4));
				commandline.erase(0, 4);
				} else{
					int x1 = commandline.find("(");
					int x2 = commandline.find(")");
					int x3 = commandline.find("!");
					int x4 = commandline.find("&&");
					int x5 = commandline.find("||");
					int x6 = commandline.find("^");
					int x7 = commandline.find("?");
					int x8 = commandline.find(":");
					int x9 = commandline.find(";");
					if(minNum(x1, x2, x3, x4, x5, x6, x7, x8, x9) == -1) {return commandline;}
					string strTemp = commandline.substr(0, minNum(x1, x2, x3, x4, x5, x6, x7, x8, x9));
					Stack1.push(strTemp);
					commandline.erase(0, minNum(x1, x2, x3, x4, x5, x6, x7, x8, x9));
				}
			}
		}
	}


	//For one option
	if(Stack1.size() == 1){

		return Stack1.top();
	}

	stack<string> Stack_temp2;

	while(Stack1.size() != 0){
		Stack_temp2.push(Stack1.top());
		Stack1.pop();
	}

	//Handling inverters
	while(Stack_temp2.size() != 0){
		if(Stack_temp2.top() == "!"){
			Stack_temp2.pop();
			if(Stack_temp2.top() != "(" && Stack_temp2.top() != "1'b0" && Stack_temp2.top() != "1'b1" && Stack_temp2.top() != "1'bZ" && Stack_temp2.top() != "!"){

				string st_temp = Stack_temp2.top();
				Stack_temp2.pop();
				Stack_temp2.push(creat_NOT(st_temp, -1));

			}else{
				Stack1.push("!");
			}
		}
		Stack1.push(Stack_temp2.top());
		Stack_temp2.pop();
	}

	bool ValidGate = 0;

	int k = Stack1.size();

	int delete_flag = 0; GateStruct* Lastdeleted = NULL;

	while (Stack1.size() != 0){

		string Temp = Stack1.top();

		if(Temp == "(") {
			string Temp2 = Stack_temp.top();

			node* currentInput;

			if(!currentPointer){
				currentPointer = new GateStruct;
				Gatehead = currentPointer;
				currentPointer->input = new node;
				currentInput = currentPointer->input;
				currentPointer->input->next = NULL;
				currentPointer->input->s = Stack_temp.top();
				currentPointer->Output = Stack_temp.top();
				currentPointer->invert = 0;currentPointer->Type = 0;
				currentPointer->next=NULL;
				currentPointer->INS = "";
				ValidGate = 1;
			}else{
				currentPointer->next = new GateStruct;
				currentPointer->next->input = new node;
				currentInput = currentPointer->next->input;
				currentPointer->next->input->next = NULL;
				currentPointer->next->input->s = Stack_temp.top();
				currentPointer->next->Output = Stack_temp.top();
				currentPointer->next->invert = 0;currentPointer->next->Type = 0;
				currentPointer->next->next=NULL;
				currentPointer->next->INS = "";
				ValidGate = 1;
			}

			while(Temp2 != ")"){

				Temp2 = Stack_temp.top();
				Stack_temp.pop();


				//-------------------------------------------------------------------------
				if (Temp2 == "&&"){
					if(!currentPointer->next){
						currentPointer->Type = 1;
						currentPointer->Output.insert(0, "_AND_");
						currentInput->next = new node;
						currentInput = currentInput->next;
						currentInput->s = Stack_temp.top();
						currentInput->next = NULL;
						currentPointer->Output.insert(0, Stack_temp.top());
						currentPointer->next=NULL;
						Stack_temp.pop();
						Stack_temp.push(currentPointer->Output);
					}else{
						currentPointer->next->Type = 1;
						currentPointer->next->Output.insert(0, "_AND_");
						currentInput->next = new node;
						currentInput = currentInput->next;
						currentInput->s = Stack_temp.top();
						currentInput->next = NULL;
						currentPointer->next->Output.insert(0, Stack_temp.top());
						currentPointer->next->next=NULL;
						Stack_temp.pop();
						Stack_temp.push(currentPointer->next->Output);
					}
				}
				//--------------------------------------------------------------------------
				//-------------------------------------------------------------------------
				if (Temp2 == "||"){
					if(!currentPointer->next){
						currentPointer->Type = 2;
						currentPointer->Output.insert(0, "_OR_");
						currentInput->next = new node;
						currentInput = currentInput->next;
						currentInput->s = Stack_temp.top();
						currentInput->next = NULL;
						currentPointer->Output.insert(0, Stack_temp.top());
						currentPointer->next=NULL;
						Stack_temp.pop();
						Stack_temp.push(currentPointer->Output);
					}else{
						currentPointer->next->Type = 2;
						currentPointer->next->Output.insert(0, "_OR_");
						currentInput->next = new node;
						currentInput = currentInput->next;
						currentInput->s = Stack_temp.top();
						currentInput->next = NULL;
						currentPointer->next->Output.insert(0, Stack_temp.top());
						currentPointer->next->next=NULL;
						Stack_temp.pop();
						Stack_temp.push(currentPointer->next->Output);
					}
				}
				//--------------------------------------------------------------------------
				//-------------------------------------------------------------------------
				if (Temp2 == "^"){
					if(!currentPointer->next){
						currentPointer->Type = 3;
						currentPointer->Output.insert(0, "_XOR_");
						currentInput->next = new node;
						currentInput = currentInput->next;
						currentInput->s = Stack_temp.top();
						currentInput->next = NULL;
						currentPointer->Output.insert(0, Stack_temp.top());
						currentPointer->next=NULL;
						Stack_temp.pop();
						Stack_temp.push(currentPointer->Output);
					}else{
						currentPointer->next->Type = 3;
						currentPointer->next->Output.insert(0, "_XOR_");
						currentInput->next = new node;
						currentInput = currentInput->next;
						currentInput->s = Stack_temp.top();
						currentInput->next = NULL;
						currentPointer->next->Output.insert(0, Stack_temp.top());
						currentPointer->next->next=NULL;
						Stack_temp.pop();
						Stack_temp.push(currentPointer->next->Output);
					}
				}
				//--------------------------------------------------------------------------
			}
			if(!currentPointer->next){Stack_temp.push(currentPointer->Output);}else{Stack_temp.push(currentPointer->next->Output);}
		} else{
			if(Temp == "!")	{
				if(!currentPointer){
					string st_temp = creat_NOT(Stack_temp.top(), -1);
					Stack_temp.pop();
					Stack_temp.push(st_temp);

				}else{
					if(!currentPointer->next){
						if(!delete_flag){
							if(currentPointer->invert  == 1){
								currentPointer->invert = 0;
								if(currentPointer->Type == 0){
									currentPointer->Output.erase(0, 4);
									Stack_temp.pop();
									if(!currentPointer->next){Stack_temp.push(currentPointer->Output);}else{Stack_temp.push(currentPointer->next->Output);}
								}
							}
							else{
								if(currentPointer->Type == 0){
									currentPointer->Output.insert(0, "NOT_");
									Stack_temp.pop();
									if(!currentPointer->next){Stack_temp.push(currentPointer->Output);}else{Stack_temp.push(currentPointer->next->Output);}
								}
								currentPointer->invert = 1;
							}
						}else{
							if(Lastdeleted->invert  == 1){
								Lastdeleted->invert = 0;
								if(Lastdeleted->Type == 0){
									Lastdeleted->Output.erase(0, 4);
									Stack_temp.pop();
									if(!Lastdeleted->next){Stack_temp.push(Lastdeleted->Output);}else{Stack_temp.push(Lastdeleted->next->Output);}
								}
							}
							else{
								if(Lastdeleted->Type == 0){
									Lastdeleted->Output.insert(0, "NOT_");
									Stack_temp.pop();
									if(!Lastdeleted->next){Stack_temp.push(Lastdeleted->Output);}else{Stack_temp.push(Lastdeleted->next->Output);}
								}
								Lastdeleted->invert = 1;
							}
						}
					}
				}
			}else{

				Stack_temp.push(Temp);
			}
		}

		if(ValidGate) {
			if(checkingGate(-1)){
				if(!currentPointer->next){
					Lastdeleted = new GateStruct;
					node* nodeTemp = currentPointer->input;
					Lastdeleted->input = new node;
					node* nodeTemp2 =Lastdeleted->input;
					nodeTemp2->s = nodeTemp->s;
					nodeTemp2->next = NULL;
					nodeTemp = nodeTemp->next;
					while(nodeTemp){
						nodeTemp2->next = new node;
						nodeTemp2->next->s = nodeTemp->s;
						nodeTemp2->next->next = NULL;
						nodeTemp = nodeTemp->next;
						nodeTemp2 = nodeTemp2->next;
					}
					Lastdeleted->invert = currentPointer->invert;
					Lastdeleted->next = NULL;
					Lastdeleted->Output= currentPointer->Output;
					Lastdeleted->Type = currentPointer->Type;
				}else{
					Lastdeleted = new GateStruct;
					node* nodeTemp = currentPointer->next->input;
					Lastdeleted->input = new node;
					node* nodeTemp2 =Lastdeleted->input;
					nodeTemp2->s = nodeTemp->s;
					nodeTemp2->next = NULL;
					nodeTemp = nodeTemp->next;
					while(nodeTemp){
						nodeTemp2->next = new node;
						nodeTemp2->next->s = nodeTemp->s;
						nodeTemp2->next->next = NULL;
						nodeTemp = nodeTemp->next;
						nodeTemp2 = nodeTemp2->next;
					}
					Lastdeleted->invert = currentPointer->next->invert;
					Lastdeleted->next = NULL;
					Lastdeleted->Output= currentPointer->next->Output;
					Lastdeleted->Type = currentPointer->next->Type;
				}
				deleteGate();
				delete_flag = 1;
			}
			else {
				delete_flag = 0;
				if(!currentPointer->next){}else{
					delete_flag = 0;
					ValidGate = 0; currentPointer = currentPointer->next;
				}
			}
			ValidGate = 0;
		}
		Stack1.pop();

	}//End of While

	if(Stack_temp.size() == 1){
		if(delete_flag){
			currentPointer->next = Lastdeleted;
			currentPointer = currentPointer->next;
			return(Stack_temp.top());
		}else{
			delete(Lastdeleted);

			if(ValidGate) {
					if(checkingGate(-1)){
					if(!currentPointer->next){}else{
						currentPointer = currentPointer->next;
						return(Stack_temp.top());
					}
				} else{
					if(!currentPointer->next){}else{
						currentPointer = currentPointer->next;
						return(Stack_temp.top());
					}
				}
			} else{
				return(Stack_temp.top());
			}
		}

		ValidGate = 0;
	}

	if(Stack_temp.size() > 1){
	node* currentInput;
	string Temp2 = Stack_temp.top();
		if(!currentPointer){
			currentPointer = new GateStruct;
			Gatehead = currentPointer;
			currentPointer->input = new node;
			currentInput = currentPointer->input;
			currentPointer->input->next = NULL;
			currentPointer->input->s = Stack_temp.top();
			currentPointer->Output = Stack_temp.top();
			currentPointer->invert = 0;currentPointer->Type = 0;
			currentPointer->next=NULL;
			currentPointer->INS = "";
			ValidGate = 1;
		}else{
			currentPointer->next = new GateStruct;
			currentPointer->next->input = new node;
			currentInput = currentPointer->next->input;
			currentPointer->next->input->next = NULL;
			currentPointer->next->input->s = Stack_temp.top();
			currentPointer->next->Output = Stack_temp.top();
			currentPointer->next->invert = 0;currentPointer->next->Type = 0;
			currentPointer->next->next=NULL;
			currentPointer->next->INS = "";
			ValidGate = 1;
		}

		while(Stack_temp.size() > 1){

			Temp2 = Stack_temp.top();
			Stack_temp.pop();


			//-------------------------------------------------------------------------
			if (Temp2 == "&&"){
				if(!currentPointer->next){
					currentPointer->Type = 1;
					currentPointer->Output.insert(0, "_AND_");
					currentInput->next = new node;
					currentInput = currentInput->next;
					currentInput->s = Stack_temp.top();
					currentInput->next = NULL;
					currentPointer->Output.insert(0, Stack_temp.top());
					currentPointer->next=NULL;
					Stack_temp.pop();
					Stack_temp.push(currentPointer->Output);
				}else{
					currentPointer->next->Type = 1;
					currentPointer->next->Output.insert(0, "_AND_");
					currentInput->next = new node;
					currentInput = currentInput->next;
					currentInput->s = Stack_temp.top();
					currentInput->next = NULL;
					currentPointer->next->Output.insert(0, Stack_temp.top());
					currentPointer->next->next=NULL;
					Stack_temp.pop();
					Stack_temp.push(currentPointer->next->Output);
				}
			}
			//--------------------------------------------------------------------------
			//-------------------------------------------------------------------------
			if (Temp2 == "||"){
				if(!currentPointer->next){
					currentPointer->Type = 2;
					currentPointer->Output.insert(0, "_OR_");
					currentInput->next = new node;
					currentInput = currentInput->next;
					currentInput->s = Stack_temp.top();
					currentInput->next = NULL;
					currentPointer->Output.insert(0, Stack_temp.top());
					currentPointer->next=NULL;
					Stack_temp.pop();
					Stack_temp.push(currentPointer->Output);
				}else{
					currentPointer->next->Type = 2;
					currentPointer->next->Output.insert(0, "_OR_");
					currentInput->next = new node;
					currentInput = currentInput->next;
					currentInput->s = Stack_temp.top();
					currentInput->next = NULL;
					currentPointer->next->Output.insert(0, Stack_temp.top());
					currentPointer->next->next=NULL;
					Stack_temp.pop();
					Stack_temp.push(currentPointer->next->Output);
				}
			}
			//--------------------------------------------------------------------------
			//-------------------------------------------------------------------------
			if (Temp2 == "^"){
				if(!currentPointer->next){
					currentPointer->Type = 3;
					currentPointer->Output.insert(0, "_XOR_");
					currentInput->next = new node;
					currentInput = currentInput->next;
					currentInput->s = Stack_temp.top();
					currentInput->next = NULL;
					currentPointer->Output.insert(0, Stack_temp.top());
					currentPointer->next=NULL;
					Stack_temp.pop();
					Stack_temp.push(currentPointer->Output);
				}else{
					currentPointer->next->Type = 3;
					currentPointer->next->Output.insert(0, "_XOR_");
					currentInput->next = new node;
					currentInput = currentInput->next;
					currentInput->s = Stack_temp.top();
					currentInput->next = NULL;
					currentPointer->next->Output.insert(0, Stack_temp.top());
					currentPointer->next->next=NULL;
					Stack_temp.pop();
					Stack_temp.push(currentPointer->next->Output);
				}
			}
		//--------------------------------------------------------------------------
		}

		if(!currentPointer->next){Stack_temp.push(currentPointer->Output);}else{Stack_temp.push(currentPointer->next->Output);}


		if(ValidGate) {
			if(checkingGate(-1)){
				if(!currentPointer->next){}else{
					currentPointer = currentPointer->next;
					return(Stack_temp.top());
				}
			} //deleteGate();
			else{
				if(!currentPointer->next){}else{
					currentPointer = currentPointer->next;
					return(Stack_temp.top());
				}
			}
		} else{
			return(Stack_temp.top());
		}
		ValidGate = 0;
	}



	return commandline;

}

string netlistGenerator::QHandler(string commandline){


	//assign Res = Arg1 ? Arg2 : Arg3;
	string Arg1;
	string Arg2;
	string Arg3;
	string Res;


	commandline = erase_space(commandline);
	int i= commandline.find("=");

	Res = commandline.substr(0, i);
	commandline.erase(0, i+1);


	Arg1 = commandline.substr(0, commandline.find("?"));
	commandline = commandline.erase(0, commandline.find("?")+1);

	Arg2 = commandline.substr(0, commandline.find(":"));
	commandline = commandline.erase(0, commandline.find(":")+1);

	Arg3 = commandline.substr(0, commandline.find(";"));
	commandline = commandline.erase(0, commandline.find(";")+1);


	string Temp = "";


	Temp.insert(Temp.length(), Res);
	Temp.insert(Temp.length(), "=(");
	Temp.insert(Temp.length(), Arg1);
	Temp.insert(Temp.length(), "&&");
	Temp.insert(Temp.length(), Arg2);
	Temp.insert(Temp.length(), ")||(!");
	Temp.insert(Temp.length(), Arg1);
	Temp.insert(Temp.length(), "&&");
	Temp.insert(Temp.length(), Arg3);
	Temp.insert(Temp.length(), ");");

	return Temp;

}


void netlistGenerator::FanoutGenerator(){

	cout<<"\n**************************************************\n";
	cout<<"Generating FAN OUT..."<<endl;
	fanoutCnt = 0;

	node* nodeTemp = EX_Inputs_1;
	while(nodeTemp){
		GateStruct* Fan = findingFanout(nodeTemp->s);

		if(Fan){
			currentPointer = currentPointer->next;
			node* currentInput = NULL;
			int Fan_t_Cnt = 0;
			GateStruct* Temp1 = Gatehead;
			while(Temp1){
				node* Temp2 = Temp1->input;
				while(Temp2){
					if(Temp2->s == Fan->Output){
						if(!Fan->input){
							Fan->input = new node;
							string sTemp = Fan->Output;
							sTemp.insert(sTemp.length(), "_");
							sTemp.insert(sTemp.length(), int2str(Fan_t_Cnt));
							Fan_t_Cnt++;
							Fan->input->next = NULL;
							Temp2->s = sTemp;
							Fan->input->s = sTemp;
							currentInput = Fan->input;
							if(!currentGateWire){
								currentGateWire = new node;
								currentGateWire->s = sTemp;
								currentGateWire->next = NULL;
								wiresHeadGate = currentGateWire;
							}else{
								currentGateWire->next = new node;
								currentGateWire->next->s = sTemp;
								currentGateWire->next->next = NULL;
								currentGateWire = currentGateWire->next;
							}
						}else{
							currentInput->next = new node;
							string sTemp = Fan->Output;
							sTemp.insert(sTemp.length(), "_");
							sTemp.insert(sTemp.length(), int2str(Fan_t_Cnt));
							Fan_t_Cnt++;
							currentInput->next->next = NULL;
							Temp2->s = sTemp;
							currentInput->next->s = sTemp;
							currentInput = currentInput->next;
							if(!currentGateWire){
								currentGateWire = new node;
								currentGateWire->s = sTemp;
								currentGateWire->next = NULL;
								wiresHeadGate = currentGateWire;
							}else{
								currentGateWire->next = new node;
								currentGateWire->next->s = sTemp;
								currentGateWire->next->next = NULL;
								currentGateWire = currentGateWire->next;
							}
						}
					}
					Temp2 = Temp2->next;
				}
				Temp1 = Temp1->next;
			}

			Fan->INS = "FAN_OUT_";
			Fan->INS.insert(Fan->INS.length(), int2str(fanoutCnt));
			fanout_information(int2str(fanoutCnt), Fan->Output, int2str(Fan_t_Cnt));
			fanoutCnt++;
		}
		nodeTemp = nodeTemp->next;
	}


	GateStruct* Temp = Gatehead;

	while (Temp){

		GateStruct* Fan = findingFanout(Temp->Output);

		if(Fan){
			currentPointer = currentPointer->next;
			node* currentInput = NULL;
			int Fan_t_Cnt = 0;
			GateStruct* Temp1 = Gatehead;
			while(Temp1){
				node* Temp2 = Temp1->input;
				while(Temp2){
					if(Temp2->s == Fan->Output){
						if(!Fan->input){
							Fan->input = new node;
							string sTemp = Fan->Output;
							sTemp.insert(sTemp.length(), "_");
							sTemp.insert(sTemp.length(), int2str(Fan_t_Cnt));
							Fan_t_Cnt++;
							Fan->input->next = NULL;
							Temp2->s = sTemp;
							Fan->input->s = sTemp;
							currentInput = Fan->input;
							if(!currentGateWire){
								currentGateWire = new node;
								currentGateWire->s = sTemp;
								currentGateWire->next = NULL;
								wiresHeadGate = currentGateWire;
							}else{
								currentGateWire->next = new node;
								currentGateWire->next->s = sTemp;
								currentGateWire->next->next = NULL;
								currentGateWire = currentGateWire->next;
							}
						}else{
							currentInput->next = new node;
							string sTemp = Fan->Output;
							sTemp.insert(sTemp.length(), "_");
							sTemp.insert(sTemp.length(), int2str(Fan_t_Cnt));
							Fan_t_Cnt++;
							currentInput->next->next = NULL;
							Temp2->s = sTemp;
							currentInput->next->s = sTemp;
							currentInput = currentInput->next;
							if(!currentGateWire){
								currentGateWire = new node;
								currentGateWire->s = sTemp;
								currentGateWire->next = NULL;
								wiresHeadGate = currentGateWire;
							}else{
								currentGateWire->next = new node;
								currentGateWire->next->s = sTemp;
								currentGateWire->next->next = NULL;
								currentGateWire = currentGateWire->next;
							}
						}
					}
					Temp2 = Temp2->next;
				}
				Temp1 = Temp1->next;
			}

			node* Temp2 = EX_Outputs_1;

			while(Temp2){
				if(Temp2->s == Fan->Output){
					if(!Fan->input){
						Fan->input = new node;
						string sTemp = Fan->Output;
						sTemp.insert(sTemp.length(), "_");
						sTemp.insert(sTemp.length(), int2str(Fan_t_Cnt));
						Fan_t_Cnt++;
						Fan->input->next = NULL;
						Temp2->s = sTemp;

						Fan->input->s = sTemp;
						currentInput = Fan->input;
						if(!currentGateWire){
							currentGateWire = new node;
							currentGateWire->s = sTemp;
							currentGateWire->next = NULL;
							wiresHeadGate = currentGateWire;
						}else{
							currentGateWire->next = new node;
							currentGateWire->next->s = sTemp;
							currentGateWire->next->next = NULL;
							currentGateWire = currentGateWire->next;
						}
					}else{
						currentInput->next = new node;
						string sTemp = Fan->Output;
						sTemp.insert(sTemp.length(), "_");
						sTemp.insert(sTemp.length(), int2str(Fan_t_Cnt));
						Fan_t_Cnt++;
						currentInput->next->next = NULL;

						Temp2->s = sTemp;
						currentInput->next->s = sTemp;
						currentInput = currentInput->next;
						if(!currentGateWire){
							currentGateWire = new node;
							currentGateWire->s = sTemp;
							currentGateWire->next = NULL;
							wiresHeadGate = currentGateWire;
						}else{
							currentGateWire->next = new node;
							currentGateWire->next->s = sTemp;
							currentGateWire->next->next = NULL;
							currentGateWire = currentGateWire->next;
						}
					}
				}
				Temp2 = Temp2->next;
			}

			Fan->INS = "FAN_OUT_";
			Fan->INS.insert(Fan->INS.length(), int2str(fanoutCnt));
			fanout_information(int2str(fanoutCnt), Fan->Output, int2str(Fan_t_Cnt));
			fanoutCnt++;
		}
		Temp = Temp->next;
	}
	return;
}


int netlistGenerator::verilogGeneratorV1(){

	cout<<"\n\nGenerating Verilog netlist...\n";

	string verilogFile = ModuleName;
	verilogFile.insert(0, "netlist_");
	verilogFile.insert(verilogFile.length(), "_V1");
	verilogFile.insert(verilogFile.length(), ".v");
	char verilog[256];
	for(int i=0; i<256; i++) verilog[i] = verilogFile[i];
	verilog[verilogFile.length()] = 0;

	remove(verilog);


	ofstream FO_TEMP(verilog);
	FO_TEMP<<endl;
	FO_TEMP<<"//Verilog file of module "<<ModuleName<<endl;
	//FO_TEMP<<"//CAD RESEARCH GROUP"<<endl;
	//FO_TEMP<<"//UNIVERSITY OF TEHRAN";
	FO_TEMP<<endl;
	FO_TEMP<<endl<<"`timescale 1 ns / 1ns"<<endl;


	if (InOutOrder) {FO_TEMP<<endl<<"module "<<ModuleName<<"_net"<<"("<<GlobalReset<<", ";}
	else {FO_TEMP<<endl<<"module "<<ModuleName<<"_net"<<"("<<GlobalReset<<", ";}

	node* Temp = InOutOrder;
	if (Temp){
		while(Temp->next){
			FO_TEMP<<Temp->s<<","<<endl;
			Temp = Temp->next;
		}
		FO_TEMP<<Temp->s<<");"<<endl;
	}


	/*
	node* Temp = Inputs;
	while(Temp->next){
		FO_TEMP<<Temp->s<<","<<endl;
		Temp = Temp->next;
	}
	if(Outputs) FO_TEMP<<Temp->s<<","<<endl;

	Temp = Outputs;
	while(Temp->next){
		FO_TEMP<<Temp->s<<","<<endl;
		Temp = Temp->next;
	}
	if(Outputs) FO_TEMP<<Temp->s<<");"<<endl;*/

	FO_TEMP<<"input "<<GlobalReset<<";"<<endl;

	Temp = InputsStr;

	if(Temp){ while(Temp->next){
			FO_TEMP<<Temp->s<<";"<<endl;
			Temp = Temp->next;
		}
	}
	if(InputsStr) FO_TEMP<<Temp->s<<";"<<endl<<endl;

	Temp = OutputsStr;

	if(Temp){
		while(Temp->next){
			FO_TEMP<<Temp->s<<";"<<endl;
			Temp = Temp->next;
		}
	}
	if(OutputsStr) FO_TEMP<<Temp->s<<";"<<endl<<endl;





	Temp = wiresHeadGate;
	if(Temp){FO_TEMP<<"wire"<<endl;}
	if(Temp){
		while(Temp->next){
			FO_TEMP<<Temp->s<<","<<endl;
			Temp = Temp->next;
		}
	}
	if(wiresHeadGate) FO_TEMP<<Temp->s<<";"<<endl<<endl;

	string str_temp;
	str_temp = "";
	node* Temp1 = EX_Inputs2;
	node* Temp2 = EX_Inputs_1;
	int input1Cnt = 0;
	if(Temp1) str_temp.insert(0, "({");
	while(Temp1){
		if(Temp1->s != Temp2->s){
			input1Cnt++;
			str_temp.insert(str_temp.length(), Temp1->s);
			str_temp.insert(str_temp.length(), ", ");
		}
		Temp1 = Temp1->next;
		Temp2 = Temp2->next;
	}
	str_temp.erase(str_temp.length()-2, 2);
	str_temp.insert(str_temp.length(), "}, ");

	Temp1 = EX_Inputs2;
	Temp2 = EX_Inputs_1;
	int input2Cnt = 0;
	if(Temp2) str_temp.insert(str_temp.length(), "{");
	while(Temp2){
		if(Temp1->s != Temp2->s){
			input2Cnt++;
			str_temp.insert(str_temp.length(), Temp2->s);
			str_temp.insert(str_temp.length(), ", ");
		}
		Temp1 = Temp1->next;
		Temp2 = Temp2->next;
	}
	str_temp.erase(str_temp.length()-2, 2);
	str_temp.insert(str_temp.length(), "});");

	if(input1Cnt !=0){
		str_temp.insert(0, ") pin_0 ");
		str_temp.insert(0, int2str(input1Cnt));
		str_temp.insert(0, "pin #(");
		if(input1Cnt == input2Cnt){
			FO_TEMP<<str_temp<<endl<<endl;
		}else{
			error_type9_input();
			exit(0);
		}
	}


	str_temp = "";
	Temp1 = EX_Outputs_1;
	Temp2 = EX_Outputs2;
	int output1Cnt = 0;
	if(Temp1) str_temp.insert(0, "({");
	while(Temp1){
		if(Temp1->s != Temp2->s){
			output1Cnt++;
			str_temp.insert(str_temp.length(), Temp1->s);
			str_temp.insert(str_temp.length(), ", ");
		}
		Temp1 = Temp1->next;
		Temp2 = Temp2->next;
	}
	str_temp.erase(str_temp.length()-2, 2);
	str_temp.insert(str_temp.length(), "}, ");

	Temp1 = EX_Outputs_1;
	Temp2 = EX_Outputs2;
	int output2Cnt = 0;
	if(Temp2) str_temp.insert(str_temp.length(), "{");
	while(Temp2){
		if(Temp1->s != Temp2->s){
			output2Cnt++;
			str_temp.insert(str_temp.length(), Temp2->s);
			str_temp.insert(str_temp.length(), ", ");
		}
		Temp1 = Temp1->next;
		Temp2 = Temp2->next;
	}
	str_temp.erase(str_temp.length()-2, 2);
	str_temp.insert(str_temp.length(), "});");

	if(output1Cnt !=0){
		str_temp.insert(0, ") pout_0 ");
		str_temp.insert(0, int2str(output1Cnt));
		str_temp.insert(0, "pout #(");
		if(output1Cnt == output2Cnt){
			FO_TEMP<<str_temp<<endl<<endl;
		}else{
			error_type9_output();
			exit(0);
		}
	}

	GateStruct* gateTemp = Gatehead;

	fanoutCnt = 0;

	while(gateTemp){
		if(gateTemp->Type == 9){
			string strTemp = "";
			strTemp.insert(0, "(");
			strTemp.insert(strTemp.length(), gateTemp->Output);
			strTemp.insert(strTemp.length(), ", {");
			node* Temp = gateTemp->input;
			int inputCnt = 0;
			while(Temp){
				inputCnt++;
				strTemp.insert(strTemp.length(), Temp->s);
				strTemp.insert(strTemp.length(), ", ");
				Temp = Temp->next;
			}
			strTemp.erase(strTemp.length()-2, 2);
			strTemp.insert(strTemp.length(), "});");
			string strTTemp = "FANOUT_";
			fanoutCnt++;
			strTTemp.insert(strTTemp.length(), int2str(fanoutCnt));
			strTemp.insert(0, " ");
			strTemp.insert(0, strTTemp);
			strTemp.insert(0, " ");
			strTemp.insert(0, ", 0, 0)");
			strTemp.insert(0, int2str(inputCnt));
			strTemp.insert(0, "fanout_n #(");
			FO_TEMP<<strTemp<<endl;
		}
		gateTemp = gateTemp->next;
	}

	FO_TEMP<<endl<<endl;

	gateTemp = Gatehead;
	andCnt = 0;
	nandCnt = 0;
	orCnt = 0;
	norCnt = 0;
	xorCnt = 0;
	xnorCnt = 0;
	notCnt = 0;
	bufferCnt = 0;

	while(gateTemp){
		if(gateTemp->Type == 1){
			if(gateTemp->invert == 0){
				string strTemp = "";
				strTemp.insert(0, "(");
				strTemp.insert(strTemp.length(), gateTemp->Output);
				strTemp.insert(strTemp.length(), ", {");
				node* Temp = gateTemp->input;
				int inputCnt = 0;
				while(Temp){
					inputCnt++;
					strTemp.insert(strTemp.length(), Temp->s);
					strTemp.insert(strTemp.length(), ", ");
					Temp = Temp->next;
				}
				strTemp.erase(strTemp.length()-2, 2);
				strTemp.insert(strTemp.length(), "});");
				string strTTemp = "AND_";
				andCnt++;
				strTTemp.insert(strTTemp.length(), int2str(andCnt));
				strTemp.insert(0, " ");
				strTemp.insert(0, strTTemp);
				strTemp.insert(0, " ");
				strTemp.insert(0, ", 0, 0)");
				strTemp.insert(0, int2str(inputCnt));
				strTemp.insert(0, "and_n #(");
				FO_TEMP<<strTemp<<endl;
			}
			if(gateTemp->invert == 1){
				string strTemp = "";
				strTemp.insert(0, "(");
				strTemp.insert(strTemp.length(), gateTemp->Output);
				strTemp.insert(strTemp.length(), ", {");
				node* Temp = gateTemp->input;
				int inputCnt = 0;
				while(Temp){
					inputCnt++;
					strTemp.insert(strTemp.length(), Temp->s);
					strTemp.insert(strTemp.length(), ", ");
					Temp = Temp->next;
				}
				strTemp.erase(strTemp.length()-2, 2);
				strTemp.insert(strTemp.length(), "});");
				string strTTemp = "NAND_";
				nandCnt++;
				strTTemp.insert(strTTemp.length(), int2str(nandCnt));
				strTemp.insert(0, " ");
				strTemp.insert(0, strTTemp);
				strTemp.insert(0, " ");
				strTemp.insert(0, ", 0, 0)");
				strTemp.insert(0, int2str(inputCnt));
				strTemp.insert(0, "nand_n #(");
				FO_TEMP<<strTemp<<endl;
			}
		}

		if(gateTemp->Type == 2){
			if(gateTemp->invert == 0){
				string strTemp = "";
				strTemp.insert(0, "(");
				strTemp.insert(strTemp.length(), gateTemp->Output);
				strTemp.insert(strTemp.length(), ", {");
				node* Temp = gateTemp->input;
				int inputCnt = 0;
				while(Temp){
					inputCnt++;
					strTemp.insert(strTemp.length(), Temp->s);
					strTemp.insert(strTemp.length(), ", ");
					Temp = Temp->next;
				}
				strTemp.erase(strTemp.length()-2, 2);
				strTemp.insert(strTemp.length(), "});");
				string strTTemp = "OR_";
				orCnt++;
				strTTemp.insert(strTTemp.length(), int2str(orCnt));
				strTemp.insert(0, " ");
				strTemp.insert(0, strTTemp);
				strTemp.insert(0, " ");
				strTemp.insert(0, ", 0, 0)");
				strTemp.insert(0, int2str(inputCnt));
				strTemp.insert(0, "or_n #(");
				FO_TEMP<<strTemp<<endl;
			}
			if(gateTemp->invert == 1){
				string strTemp = "";
				strTemp.insert(0, "(");
				strTemp.insert(strTemp.length(), gateTemp->Output);
				strTemp.insert(strTemp.length(), ", {");
				node* Temp = gateTemp->input;
				int inputCnt = 0;
				while(Temp){
					inputCnt++;
					strTemp.insert(strTemp.length(), Temp->s);
					strTemp.insert(strTemp.length(), ", ");
					Temp = Temp->next;
				}
				strTemp.erase(strTemp.length()-2, 2);
				strTemp.insert(strTemp.length(), "});");
				string strTTemp = "NOR_";
				norCnt++;
				strTTemp.insert(strTTemp.length(), int2str(norCnt));
				strTemp.insert(0, " ");
				strTemp.insert(0, strTTemp);
				strTemp.insert(0, " ");
				strTemp.insert(0, ", 0, 0)");
				strTemp.insert(0, int2str(inputCnt));
				strTemp.insert(0, "nor_n #(");
				FO_TEMP<<strTemp<<endl;
			}
		}

		if(gateTemp->Type == 3){
			if(gateTemp->invert == 0){
				string strTemp = "";
				strTemp.insert(0, "(");
				strTemp.insert(strTemp.length(), gateTemp->Output);
				strTemp.insert(strTemp.length(), ", {");
				node* Temp = gateTemp->input;
				int inputCnt = 0;
				while(Temp){
					inputCnt++;
					strTemp.insert(strTemp.length(), Temp->s);
					strTemp.insert(strTemp.length(), ", ");
					Temp = Temp->next;
				}
				strTemp.erase(strTemp.length()-2, 2);
				strTemp.insert(strTemp.length(), "});");
				string strTTemp = "XOR_";
				xorCnt++;
				strTTemp.insert(strTTemp.length(), int2str(xorCnt));
				strTemp.insert(0, " ");
				strTemp.insert(0, strTTemp);
				strTemp.insert(0, " ");
				strTemp.insert(0, ", 0, 0)");
				strTemp.insert(0, int2str(inputCnt));
				strTemp.insert(0, "xor_n #(");
				FO_TEMP<<strTemp<<endl;
			}
			if(gateTemp->invert == 1){
				string strTemp = "";
				strTemp.insert(0, "(");
				strTemp.insert(strTemp.length(), gateTemp->Output);
				strTemp.insert(strTemp.length(), ", {");
				node* Temp = gateTemp->input;
				int inputCnt = 0;
				while(Temp){
					inputCnt++;
					strTemp.insert(strTemp.length(), Temp->s);
					strTemp.insert(strTemp.length(), ", ");
					Temp = Temp->next;
				}
				strTemp.erase(strTemp.length()-2, 2);
				strTemp.insert(strTemp.length(), "});");
				string strTTemp = "XNOR_";
				xnorCnt++;
				strTTemp.insert(strTTemp.length(), int2str(xnorCnt));
				strTemp.insert(0, " ");
				strTemp.insert(0, strTTemp);
				strTemp.insert(0, " ");
				strTemp.insert(0, ", 0, 0)");
				strTemp.insert(0, int2str(inputCnt));
				strTemp.insert(0, "xnor_n #(");
				FO_TEMP<<strTemp<<endl;
			}
		}

		if(gateTemp->Type == 0){
			if(gateTemp->invert == 0){
				string strTemp = "";
				strTemp.insert(0, "(");
				strTemp.insert(strTemp.length(), gateTemp->Output);
				strTemp.insert(strTemp.length(), ", ");
				strTemp.insert(strTemp.length(), gateTemp->input->s);
				strTemp.insert(strTemp.length(), ");");
				string strTTemp = "BUF_";
				bufferCnt++;
				strTTemp.insert(strTTemp.length(), int2str(bufferCnt));
				strTemp.insert(0, " ");
				strTemp.insert(0, strTTemp);
				strTemp.insert(0, " ");
				strTemp.insert(0, "bufg #(0, 0)");
				FO_TEMP<<strTemp<<endl;
			}
			if(gateTemp->invert == 1){
				string strTemp = "";
				strTemp.insert(0, "(");
				strTemp.insert(strTemp.length(), gateTemp->Output);
				strTemp.insert(strTemp.length(), ", ");
				strTemp.insert(strTemp.length(), gateTemp->input->s);
				strTemp.insert(strTemp.length(), ");");
				string strTTemp = "NOT_";
				notCnt++;
				strTTemp.insert(strTTemp.length(), int2str(notCnt));
				strTemp.insert(0, " ");
				strTemp.insert(0, strTTemp);
				strTemp.insert(0, " ");
				strTemp.insert(0, "notg #(0, 0)");
				FO_TEMP<<strTemp<<endl;
			}
		}
		gateTemp = gateTemp->next;
	}

	gateTemp = Gatehead;

	fanoutCnt = 0;

	while(gateTemp){
		if(gateTemp->Type == 4){
			string strTemp = "";
			strTemp.insert(0, "dff ");
			strTemp.insert(strTemp.length(), gateTemp->INS);
			strTemp.insert(strTemp.length(), " (");
			strTemp.insert(strTemp.length(), gateTemp->Output);
			strTemp.insert(strTemp.length(), ", ");
			node* Temp = gateTemp->input;
			int inputCnt = 0;
			while(Temp){
				inputCnt++;
				if(Temp->s == GlobalReset){strTemp.insert(strTemp.length(), NbarT); strTemp.insert(strTemp.length(), ", ");strTemp.insert(strTemp.length(), serialInwire);strTemp.insert(strTemp.length(), ", ");}
				strTemp.insert(strTemp.length(), Temp->s);
				strTemp.insert(strTemp.length(), ", ");
				Temp = Temp->next;
			}
			strTemp.erase(strTemp.length()-2, 2);
			strTemp.insert(strTemp.length(), ");");
			FO_TEMP<<strTemp<<endl;
		}

		if(gateTemp->Type == 5){
			/*string strTemp = "";
			strTemp.insert(0, "tff ");
			strTemp.insert(strTemp.length(), gateTemp->INS);
			strTemp.insert(strTemp.length(), " (");
			strTemp.insert(strTemp.length(), gateTemp->Output);
			strTemp.insert(strTemp.length(), ", ");
			node* Temp = gateTemp->input;
			int inputCnt = 0;
			while(Temp){
				inputCnt++;
				if(Temp->s == GlobalReset){strTemp.insert(strTemp.length(), NbarT); strTemp.insert(strTemp.length(), ", ");strTemp.insert(strTemp.length(), serialInwire);strTemp.insert(strTemp.length(), ", ");}
				strTemp.insert(strTemp.length(), Temp->s);
				strTemp.insert(strTemp.length(), ", ");
				Temp = Temp->next;
			}
			strTemp.erase(strTemp.length()-2, 2);
			strTemp.insert(strTemp.length(), ");");
			FO_TEMP<<strTemp<<endl;*/

			string strTemp = "";
			strTemp.insert(strTemp.length(), "dff ");
			strTemp.insert(strTemp.length(), gateTemp->INS);
			strTemp.insert(strTemp.length(), " (");
			strTemp.insert(strTemp.length(), gateTemp->Output);
			strTemp.insert(strTemp.length(), ", T_");
			node* Temp = gateTemp->input;
			int inputCnt = 0;
			while(Temp){
				inputCnt++;
				if(Temp->s == GlobalReset){strTemp.insert(strTemp.length(), NbarT); strTemp.insert(strTemp.length(), ", ");strTemp.insert(strTemp.length(), serialInwire);strTemp.insert(strTemp.length(), ", ");}
				strTemp.insert(strTemp.length(), Temp->s);
				strTemp.insert(strTemp.length(), ", ");
				Temp = Temp->next;
			}
			strTemp.erase(strTemp.length()-2, 2);
			strTemp.insert(strTemp.length(), ");");
			FO_TEMP<<strTemp<<endl;

			strTemp = "";
			strTemp.insert(0, "(");
			strTemp.insert(strTemp.length(), "T_");
			strTemp.insert(strTemp.length(), gateTemp->input->s);
			strTemp.insert(strTemp.length(), ", {");
			strTemp.insert(strTemp.length(), gateTemp->Output);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), gateTemp->input->s);
			strTemp.insert(strTemp.length(), "});");
			string strTTemp = "XOR_";
			xorCnt++;
			strTTemp.insert(strTTemp.length(), int2str(xorCnt));
			strTemp.insert(0, " ");
			strTemp.insert(0, strTTemp);
			strTemp.insert(0, " ");
			strTemp.insert(0, "2, 0, 0)");
			strTemp.insert(0, "xor_n #(");
			FO_TEMP<<strTemp<<endl;

		}

		if(gateTemp->Type == 6){
			string strTemp = "";
			strTemp.insert(0, "FDDCPE ");
			strTemp.insert(strTemp.length(), gateTemp->INS);
			strTemp.insert(strTemp.length(), " (");
			strTemp.insert(strTemp.length(), gateTemp->Output);
			strTemp.insert(strTemp.length(), ", ");

			node* Temp = gateTemp->input;
			int inputCnt = 0;
			while(Temp){
				inputCnt++;
				strTemp.insert(strTemp.length(), Temp->s);
				strTemp.insert(strTemp.length(), ", ");
				Temp = Temp->next;
			}
			strTemp.erase(strTemp.length()-2, 2);
			strTemp.insert(strTemp.length(), ");");
			ERR<<"//Can not handle latches in test application//"<<endl;
			cout<<"//Can not handle latches in test application//"<<endl;
			FO_TEMP<<"//Can not handle latches in test application//"<<endl;
			FO_TEMP<<strTemp<<endl;
		}

		if(gateTemp->Type == 7){
			string strTemp = "";
			strTemp.insert(0, "FTDCPE ");
			strTemp.insert(strTemp.length(), gateTemp->INS);
			strTemp.insert(strTemp.length(), " (");
			strTemp.insert(strTemp.length(), gateTemp->Output);
			strTemp.insert(strTemp.length(), ", ");

			node* Temp = gateTemp->input;
			int inputCnt = 0;
			while(Temp){
				inputCnt++;
				strTemp.insert(strTemp.length(), Temp->s);
				strTemp.insert(strTemp.length(), ", ");
				Temp = Temp->next;
			}
			strTemp.erase(strTemp.length()-2, 2);
			strTemp.insert(strTemp.length(), ");");
			ERR<<"//Can not handle latches in test application//"<<endl;
			cout<<"//Can not handle latches in test application//"<<endl;
			FO_TEMP<<"//Can not handle latches in test application//"<<endl;
			FO_TEMP<<strTemp<<endl;
		}

		if(gateTemp->Type == 8){
			string strTemp = "";
			strTemp.insert(0, "LDCP ");
			strTemp.insert(strTemp.length(), gateTemp->INS);
			strTemp.insert(strTemp.length(), " (");
			strTemp.insert(strTemp.length(), gateTemp->Output);
			strTemp.insert(strTemp.length(), ", ");

			node* Temp = gateTemp->input;
			int inputCnt = 0;
			while(Temp){
				inputCnt++;
				strTemp.insert(strTemp.length(), Temp->s);
				strTemp.insert(strTemp.length(), ", ");
				Temp = Temp->next;
			}
			strTemp.erase(strTemp.length()-2, 2);
			strTemp.insert(strTemp.length(), ");");
			ERR<<"//Can not handle latches in test application//"<<endl;
			cout<<"//Can not handle latches in test application//"<<endl;
			FO_TEMP<<"//Can not handle latches in test application//"<<endl;
			FO_TEMP<<strTemp<<endl;
		}
		gateTemp = gateTemp->next;
	}




	FO_TEMP<<endl<<"endmodule"<<endl;






	//printEQ_Components();
	printNet_Components();
	printMem_module();

	cout<<"\nThe netlist Verilog file generated. It's name is : "<<verilogFile<<".\n";
	cout<<"**************************************************\n";
	//cout<<"Thanks.\nCAD RESEARCH GROUP\nUNIVERSITY OF TEHRAN\n";
	cout<<"hashem@cad.ut.ac.ir\n\n\n";
	cout<<"Press any key to end...";
	getch();

	INF<<"\n**************************************************\n";
	INF<<"\n\nThanks...\n";
	INF<<"hashem@cad.ut.ac.ir"<<endl;

	return 0;
}

int netlistGenerator::verilogGeneratorV2(){

	cout<<"\n\nGenerating Verilog netlist...\n";

	string verilogFile = ModuleName;
	verilogFile.insert(0, "netlist_");
	verilogFile.insert(verilogFile.length(), "_V1");
	verilogFile.insert(verilogFile.length(), ".v");
	char verilog[256];
	for(int i=0; i<verilogFile.length(); i++) verilog[i] = verilogFile[i];
	verilog[verilogFile.length()] = 0;

	remove(verilog);


	ofstream FO_TEMP(verilog);
	FO_TEMP<<endl;
	FO_TEMP<<"//Verilog file of module "<<ModuleName<<endl;
	//FO_TEMP<<"//CAD RESEARCH GROUP"<<endl;
	//FO_TEMP<<"//UNIVERSITY OF TEHRAN";
	FO_TEMP<<endl;
	FO_TEMP<<endl<<"`timescale 1 ns / 1ns"<<endl;


	/*if (InOutOrder) {FO_TEMP<<endl<<"module "<<ModuleName<<"_net"<<"("<<GlobalReset<<", ";}
	else            {FO_TEMP<<endl<<"module "<<ModuleName<<"_net"<<"("<<GlobalReset<<", ";}*/

	if (InOutOrder) {FO_TEMP<<endl<<"module "<<ModuleName<<"_net"<<"(";}
	else            {FO_TEMP<<endl<<"module "<<ModuleName<<"_net"<<"(";}

	node* Temp = InOutOrder;
	if (Temp){
		while(Temp->next){
			FO_TEMP<<Temp->s<<","<<endl;
			Temp = Temp->next;
		}
		FO_TEMP<<Temp->s<<");"<<endl;
	}


	/*
	node* Temp = Inputs;
	while(Temp->next){
		FO_TEMP<<Temp->s<<","<<endl;
		Temp = Temp->next;
	}
	if(Outputs) FO_TEMP<<Temp->s<<","<<endl;

	Temp = Outputs;
	while(Temp->next){
		FO_TEMP<<Temp->s<<","<<endl;
		Temp = Temp->next;
	}
	if(Outputs) FO_TEMP<<Temp->s<<");"<<endl;*/

	//FO_TEMP<<"input "<<GlobalReset<<";"<<endl;

	Temp = InputsStr;

	if(Temp){ while(Temp->next){
			if(Temp->s.find("[") != -1){
				string MAXbound = Temp->s.substr(Temp->s.find("[")+1, Temp->s.find(":")-Temp->s.find("[")-1);
				string MINbound = Temp->s.substr(Temp->s.find(":")+1, Temp->s.find("]")-Temp->s.find(":")-1);
				string variable = Temp->s.substr(Temp->s.find("]")+1, Temp->s.length()-Temp->s.find("]")-1);
				MAXbound = erase_space(MAXbound);
				MINbound = erase_space(MINbound);
				variable = erase_space(variable);
				if(str2int(MAXbound) < str2int(MINbound)){
					string strTemp = MAXbound;
					MAXbound = MINbound;
					MINbound = strTemp;
				}
				FO_TEMP<<"input ["<<MINbound<<":"<<MAXbound<<"]"<<variable<<";"<<endl;
				Temp = Temp->next;
			}else{
				FO_TEMP<<Temp->s<<";"<<endl;
				Temp = Temp->next;
			}
		}
	}

	if(InputsStr){
		if(Temp->s.find("[") != -1){
			string MAXbound = Temp->s.substr(Temp->s.find("[")+1, Temp->s.find(":")-Temp->s.find("[")-1);
			string MINbound = Temp->s.substr(Temp->s.find(":")+1, Temp->s.find("]")-Temp->s.find(":")-1);
			string variable = Temp->s.substr(Temp->s.find("]")+1, Temp->s.length()-Temp->s.find("]")-1);
			MAXbound = erase_space(MAXbound);
			MINbound = erase_space(MINbound);
			variable = erase_space(variable);
			if(str2int(MAXbound) < str2int(MINbound)){
				string strTemp = MAXbound;
				MAXbound = MINbound;
				MINbound = strTemp;
			}
			FO_TEMP<<"input ["<<MINbound<<":"<<MAXbound<<"]"<<variable<<";"<<endl;

		}else{
			FO_TEMP<<Temp->s<<";"<<endl<<endl;

		}
	}
	
	Temp = OutputsStr;

	if(Temp){ while(Temp->next){
			if(Temp->s.find("[") != -1){
				string MAXbound = Temp->s.substr(Temp->s.find("[")+1, Temp->s.find(":")-Temp->s.find("[")-1);
				string MINbound = Temp->s.substr(Temp->s.find(":")+1, Temp->s.find("]")-Temp->s.find(":")-1);
				string variable = Temp->s.substr(Temp->s.find("]")+1, Temp->s.length()-Temp->s.find("]")-1);
				MAXbound = erase_space(MAXbound);
				MINbound = erase_space(MINbound);
				variable = erase_space(variable);
				if(str2int(MAXbound) < str2int(MINbound)){
					string strTemp = MAXbound;
					MAXbound = MINbound;
					MINbound = strTemp;
				}
				FO_TEMP<<"output ["<<MINbound<<":"<<MAXbound<<"]"<<variable<<";"<<endl;
				Temp = Temp->next;
			}else{
				FO_TEMP<<Temp->s<<";"<<endl;
				Temp = Temp->next;
			}
		}
	}

	if(InputsStr){
		if(Temp->s.find("[") != -1){
			string MAXbound = Temp->s.substr(Temp->s.find("[")+1, Temp->s.find(":")-Temp->s.find("[")-1);
			string MINbound = Temp->s.substr(Temp->s.find(":")+1, Temp->s.find("]")-Temp->s.find(":")-1);
			string variable = Temp->s.substr(Temp->s.find("]")+1, Temp->s.length()-Temp->s.find("]")-1);
			MAXbound = erase_space(MAXbound);
			MINbound = erase_space(MINbound);
			variable = erase_space(variable);
			if(str2int(MAXbound) < str2int(MINbound)){
				string strTemp = MAXbound;
				MAXbound = MINbound;
				MINbound = strTemp;
			}
			FO_TEMP<<"output ["<<MINbound<<":"<<MAXbound<<"]"<<variable<<";"<<endl;

		}else{
			FO_TEMP<<Temp->s<<";"<<endl<<endl;

		}
	}





	Temp = wiresHeadGate;
	if(Temp){FO_TEMP<<"wire"<<endl;}
	if(Temp){
		while(Temp->next){
			FO_TEMP<<Temp->s<<","<<endl;
			Temp = Temp->next;
		}
	}
	if(wiresHeadGate) FO_TEMP<<Temp->s<<";"<<endl<<endl;

	string str_temp;
	str_temp = "";
	node* Temp1 = EX_Inputs2;
	node* Temp2 = EX_Inputs_1;
	int input1Cnt = 0;
	if(Temp1) str_temp.insert(0, "({");
	while(Temp1){
		if(Temp1->s != Temp2->s){
			input1Cnt++;
			str_temp.insert(str_temp.length(), Temp1->s);
			str_temp.insert(str_temp.length(), ", ");
		}else{
			input1Cnt++;
			str_temp.insert(str_temp.length(), Temp2->s);
			str_temp.insert(str_temp.length(), ", ");
		}



		Temp1 = Temp1->next;
		Temp2 = Temp2->next;
	}
	str_temp.erase(str_temp.length()-2, 2);
	str_temp.insert(str_temp.length(), "}, ");

	Temp1 = EX_Inputs2;
	Temp2 = EX_Inputs_1;
	int input2Cnt = 0;
	if(Temp2) str_temp.insert(str_temp.length(), "{");
	while(Temp2){
		if(Temp1->s != Temp2->s){
			input2Cnt++;
			str_temp.insert(str_temp.length(), Temp2->s);
			str_temp.insert(str_temp.length(), ", ");
		}else{
			string str_tempIO = Temp1->s;
			int k = 0;
			str_tempIO.insert(str_tempIO.length(), "_net_");
			string str_temp_IO2 = str_tempIO;
			str_temp_IO2.insert(str_temp_IO2.length(), int2str(k));
			while(checkingWire(str_temp_IO2, EX_Inputs_1) || checkingWire(str_temp_IO2, EX_Outputs_1)){
				k++;
				str_temp_IO2 = str_tempIO;
				str_temp_IO2.insert(str_temp_IO2.length(), int2str(k));
			}
			replaceGatewire(str_temp_IO2, Temp1->s);
			input2Cnt++;
			str_temp.insert(str_temp.length(), str_temp_IO2);
			str_temp.insert(str_temp.length(), ", ");
		}
		Temp1 = Temp1->next;
		Temp2 = Temp2->next;
	}
	str_temp.erase(str_temp.length()-2, 2);
	str_temp.insert(str_temp.length(), "});");

	if(input1Cnt !=0){
		str_temp.insert(0, ") pin_0 ");
		str_temp.insert(0, int2str(input1Cnt));
		str_temp.insert(0, "pin #(");
		if(input1Cnt == input2Cnt){
			FO_TEMP<<str_temp<<endl<<endl;
		}else{
			error_type9_input();
			exit(0);
		}
	}


	str_temp = "";
	Temp1 = EX_Outputs_1;
	Temp2 = EX_Outputs2;
	int output1Cnt = 0;
	if(Temp1) str_temp.insert(0, "({");
	while(Temp1){
		if(Temp1->s != Temp2->s){
			output1Cnt++;
			str_temp.insert(str_temp.length(), Temp1->s);
			str_temp.insert(str_temp.length(), ", ");
		}else{
			string str_tempIO = Temp1->s;
			int k = 0;
			str_tempIO.insert(str_tempIO.length(), "_net_");
			string str_temp_IO2 = str_tempIO;
			str_temp_IO2.insert(str_temp_IO2.length(), int2str(k));
			while(checkingWire(str_temp_IO2, EX_Inputs_1) || checkingWire(str_temp_IO2, EX_Outputs_1)){
				k++;
				str_temp_IO2 = str_tempIO;
				str_temp_IO2.insert(str_temp_IO2.length(), int2str(k));
			}
			replaceGatewire(str_temp_IO2, Temp1->s);
			output1Cnt++;
			str_temp.insert(str_temp.length(), str_temp_IO2);
			str_temp.insert(str_temp.length(), ", ");
		}
		Temp1 = Temp1->next;
		Temp2 = Temp2->next;
	}
	str_temp.erase(str_temp.length()-2, 2);
	str_temp.insert(str_temp.length(), "}, ");

	Temp1 = EX_Outputs_1;
	Temp2 = EX_Outputs2;
	int output2Cnt = 0;
	if(Temp2) str_temp.insert(str_temp.length(), "{");
	while(Temp2){
		if(Temp1->s != Temp2->s){
			output2Cnt++;
			str_temp.insert(str_temp.length(), Temp2->s);
			str_temp.insert(str_temp.length(), ", ");
		}else{
			output2Cnt++;
			str_temp.insert(str_temp.length(), Temp2->s);
			str_temp.insert(str_temp.length(), ", ");
		}
		Temp1 = Temp1->next;
		Temp2 = Temp2->next;
	}
	str_temp.erase(str_temp.length()-2, 2);
	str_temp.insert(str_temp.length(), "});");

	if(output1Cnt !=0){
		str_temp.insert(0, ") pout_0 ");
		str_temp.insert(0, int2str(output1Cnt));
		str_temp.insert(0, "pout #(");
		if(output1Cnt == output2Cnt){
			FO_TEMP<<str_temp<<endl<<endl;
		}else{
			error_type9_output();
			exit(0);
		}
	}

	GateStruct* gateTemp = Gatehead;

	fanoutCnt = 0;

	while(gateTemp){
		if(gateTemp->Type == 9){
			string strTemp = "";
			strTemp.insert(0, "(");
			strTemp.insert(strTemp.length(), gateTemp->Output);
			strTemp.insert(strTemp.length(), ", {");
			node* Temp = gateTemp->input;
			int inputCnt = 0;
			while(Temp){
				inputCnt++;
				strTemp.insert(strTemp.length(), Temp->s);
				strTemp.insert(strTemp.length(), ", ");
				Temp = Temp->next;
			}
			strTemp.erase(strTemp.length()-2, 2);
			strTemp.insert(strTemp.length(), "});");
			string strTTemp = "FANOUT_";
			fanoutCnt++;
			strTTemp.insert(strTTemp.length(), int2str(fanoutCnt));
			strTemp.insert(0, " ");
			strTemp.insert(0, strTTemp);
			strTemp.insert(0, " ");
			strTemp.insert(0, ", 0, 0)");
			strTemp.insert(0, int2str(inputCnt));
			strTemp.insert(0, "fanout_n #(");
			FO_TEMP<<strTemp<<endl;
		}
		gateTemp = gateTemp->next;
	}

	FO_TEMP<<endl<<endl;

	gateTemp = Gatehead;
	andCnt = 0;
	nandCnt = 0;
	orCnt = 0;
	norCnt = 0;
	xorCnt = 0;
	xnorCnt = 0;
	notCnt = 0;
	bufferCnt = 0;
	dffCnt = 0;

	while(gateTemp){
		if(gateTemp->Type == 1){
			if(gateTemp->invert == 0){
				string strTemp = "";
				strTemp.insert(0, "(");
				strTemp.insert(strTemp.length(), gateTemp->Output);
				strTemp.insert(strTemp.length(), ", {");
				node* Temp = gateTemp->input;
				int inputCnt = 0;
				while(Temp){
					inputCnt++;
					strTemp.insert(strTemp.length(), Temp->s);
					strTemp.insert(strTemp.length(), ", ");
					Temp = Temp->next;
				}
				strTemp.erase(strTemp.length()-2, 2);
				strTemp.insert(strTemp.length(), "});");
				string strTTemp = "AND_";
				andCnt++;
				strTTemp.insert(strTTemp.length(), int2str(andCnt));
				strTemp.insert(0, " ");
				strTemp.insert(0, strTTemp);
				strTemp.insert(0, " ");
				strTemp.insert(0, ", 0, 0)");
				strTemp.insert(0, int2str(inputCnt));
				strTemp.insert(0, "and_n #(");
				FO_TEMP<<strTemp<<endl;
			}
			if(gateTemp->invert == 1){
				string strTemp = "";
				strTemp.insert(0, "(");
				strTemp.insert(strTemp.length(), gateTemp->Output);
				strTemp.insert(strTemp.length(), ", {");
				node* Temp = gateTemp->input;
				int inputCnt = 0;
				while(Temp){
					inputCnt++;
					strTemp.insert(strTemp.length(), Temp->s);
					strTemp.insert(strTemp.length(), ", ");
					Temp = Temp->next;
				}
				strTemp.erase(strTemp.length()-2, 2);
				strTemp.insert(strTemp.length(), "});");
				string strTTemp = "NAND_";
				nandCnt++;
				strTTemp.insert(strTTemp.length(), int2str(nandCnt));
				strTemp.insert(0, " ");
				strTemp.insert(0, strTTemp);
				strTemp.insert(0, " ");
				strTemp.insert(0, ", 0, 0)");
				strTemp.insert(0, int2str(inputCnt));
				strTemp.insert(0, "nand_n #(");
				FO_TEMP<<strTemp<<endl;
			}
		}

		if(gateTemp->Type == 2){
			if(gateTemp->invert == 0){
				string strTemp = "";
				strTemp.insert(0, "(");
				strTemp.insert(strTemp.length(), gateTemp->Output);
				strTemp.insert(strTemp.length(), ", {");
				node* Temp = gateTemp->input;
				int inputCnt = 0;
				while(Temp){
					inputCnt++;
					strTemp.insert(strTemp.length(), Temp->s);
					strTemp.insert(strTemp.length(), ", ");
					Temp = Temp->next;
				}
				strTemp.erase(strTemp.length()-2, 2);
				strTemp.insert(strTemp.length(), "});");
				string strTTemp = "OR_";
				orCnt++;
				strTTemp.insert(strTTemp.length(), int2str(orCnt));
				strTemp.insert(0, " ");
				strTemp.insert(0, strTTemp);
				strTemp.insert(0, " ");
				strTemp.insert(0, ", 0, 0)");
				strTemp.insert(0, int2str(inputCnt));
				strTemp.insert(0, "or_n #(");
				FO_TEMP<<strTemp<<endl;
			}
			if(gateTemp->invert == 1){
				string strTemp = "";
				strTemp.insert(0, "(");
				strTemp.insert(strTemp.length(), gateTemp->Output);
				strTemp.insert(strTemp.length(), ", {");
				node* Temp = gateTemp->input;
				int inputCnt = 0;
				while(Temp){
					inputCnt++;
					strTemp.insert(strTemp.length(), Temp->s);
					strTemp.insert(strTemp.length(), ", ");
					Temp = Temp->next;
				}
				strTemp.erase(strTemp.length()-2, 2);
				strTemp.insert(strTemp.length(), "});");
				string strTTemp = "NOR_";
				norCnt++;
				strTTemp.insert(strTTemp.length(), int2str(norCnt));
				strTemp.insert(0, " ");
				strTemp.insert(0, strTTemp);
				strTemp.insert(0, " ");
				strTemp.insert(0, ", 0, 0)");
				strTemp.insert(0, int2str(inputCnt));
				strTemp.insert(0, "nor_n #(");
				FO_TEMP<<strTemp<<endl;
			}
		}

		if(gateTemp->Type == 3){
			if(gateTemp->invert == 0){
				string strTemp = "";
				strTemp.insert(0, "(");
				strTemp.insert(strTemp.length(), gateTemp->Output);
				strTemp.insert(strTemp.length(), ", {");
				node* Temp = gateTemp->input;
				int inputCnt = 0;
				while(Temp){
					inputCnt++;
					strTemp.insert(strTemp.length(), Temp->s);
					strTemp.insert(strTemp.length(), ", ");
					Temp = Temp->next;
				}
				strTemp.erase(strTemp.length()-2, 2);
				strTemp.insert(strTemp.length(), "});");
				string strTTemp = "XOR_";
				xorCnt++;
				strTTemp.insert(strTTemp.length(), int2str(xorCnt));
				strTemp.insert(0, " ");
				strTemp.insert(0, strTTemp);
				strTemp.insert(0, " ");
				strTemp.insert(0, ", 0, 0)");
				strTemp.insert(0, int2str(inputCnt));
				strTemp.insert(0, "xor_n #(");
				FO_TEMP<<strTemp<<endl;
			}
			if(gateTemp->invert == 1){
				string strTemp = "";
				strTemp.insert(0, "(");
				strTemp.insert(strTemp.length(), gateTemp->Output);
				strTemp.insert(strTemp.length(), ", {");
				node* Temp = gateTemp->input;
				int inputCnt = 0;
				while(Temp){
					inputCnt++;
					strTemp.insert(strTemp.length(), Temp->s);
					strTemp.insert(strTemp.length(), ", ");
					Temp = Temp->next;
				}
				strTemp.erase(strTemp.length()-2, 2);
				strTemp.insert(strTemp.length(), "});");
				string strTTemp = "XNOR_";
				xnorCnt++;
				strTTemp.insert(strTTemp.length(), int2str(xnorCnt));
				strTemp.insert(0, " ");
				strTemp.insert(0, strTTemp);
				strTemp.insert(0, " ");
				strTemp.insert(0, ", 0, 0)");
				strTemp.insert(0, int2str(inputCnt));
				strTemp.insert(0, "xnor_n #(");
				FO_TEMP<<strTemp<<endl;
			}
		}

		if(gateTemp->Type == 0){
			if(gateTemp->invert == 0){
				string strTemp = "";
				strTemp.insert(0, "(");
				strTemp.insert(strTemp.length(), gateTemp->Output);
				strTemp.insert(strTemp.length(), ", ");
				strTemp.insert(strTemp.length(), gateTemp->input->s);
				strTemp.insert(strTemp.length(), ");");
				string strTTemp = "BUF_";
				bufferCnt++;
				strTTemp.insert(strTTemp.length(), int2str(bufferCnt));
				strTemp.insert(0, " ");
				strTemp.insert(0, strTTemp);
				strTemp.insert(0, " ");
				strTemp.insert(0, "bufg #(0, 0)");
				FO_TEMP<<strTemp<<endl;
			}
			if(gateTemp->invert == 1){
				string strTemp = "";
				strTemp.insert(0, "(");
				strTemp.insert(strTemp.length(), gateTemp->Output);
				strTemp.insert(strTemp.length(), ", ");
				strTemp.insert(strTemp.length(), gateTemp->input->s);
				strTemp.insert(strTemp.length(), ");");
				string strTTemp = "NOT_";
				notCnt++;
				strTTemp.insert(strTTemp.length(), int2str(notCnt));
				strTemp.insert(0, " ");
				strTemp.insert(0, strTTemp);
				strTemp.insert(0, " ");
				strTemp.insert(0, "notg #(0, 0)");
				FO_TEMP<<strTemp<<endl;
			}
		}

		if(gateTemp->Type == 10){
			if(gateTemp->invert == 0){
				string strTemp = "";
				strTemp.insert(0, "(");
				strTemp.insert(strTemp.length(), gateTemp->Output);
				strTemp.insert(strTemp.length(), ", ");
				strTemp.insert(strTemp.length(), gateTemp->input->s);
				strTemp.insert(strTemp.length(), ");");
				string strTTemp = "BUF_";
				bufferCnt++;
				strTTemp.insert(strTTemp.length(), int2str(bufferCnt));
				strTemp.insert(0, " ");
				strTemp.insert(0, strTTemp);
				strTemp.insert(0, " ");
				strTemp.insert(0, "bufg #(0, 0)");
				FO_TEMP<<strTemp<<endl;
			}
			if(gateTemp->invert == 1){
				string strTemp = "";
				strTemp.insert(0, "(");
				strTemp.insert(strTemp.length(), gateTemp->Output);
				strTemp.insert(strTemp.length(), ", ");
				strTemp.insert(strTemp.length(), gateTemp->input->s);
				strTemp.insert(strTemp.length(), ");");
				string strTTemp = "NOT_";
				notCnt++;
				strTTemp.insert(strTTemp.length(), int2str(notCnt));
				strTemp.insert(0, " ");
				strTemp.insert(0, strTTemp);
				strTemp.insert(0, " ");
				strTemp.insert(0, "notg #(0, 0)");
				FO_TEMP<<strTemp<<endl;
			}
		}

		if(gateTemp->Type == 11){
			if(gateTemp->invert == 0){
				string strTemp = "";
				strTemp.insert(0, "(");
				strTemp.insert(strTemp.length(), gateTemp->Output);
				strTemp.insert(strTemp.length(), ", ");
				strTemp.insert(strTemp.length(), gateTemp->input->s);
				strTemp.insert(strTemp.length(), ");");
				string strTTemp = "BUF_";
				bufferCnt++;
				strTTemp.insert(strTTemp.length(), int2str(bufferCnt));
				strTemp.insert(0, " ");
				strTemp.insert(0, strTTemp);
				strTemp.insert(0, " ");
				strTemp.insert(0, "bufg #(0, 0)");
				FO_TEMP<<strTemp<<endl;
			}
			if(gateTemp->invert == 1){
				string strTemp = "";
				strTemp.insert(0, "(");
				strTemp.insert(strTemp.length(), gateTemp->Output);
				strTemp.insert(strTemp.length(), ", ");
				strTemp.insert(strTemp.length(), gateTemp->input->s);
				strTemp.insert(strTemp.length(), ");");
				string strTTemp = "NOT_";
				notCnt++;
				strTTemp.insert(strTTemp.length(), int2str(notCnt));
				strTemp.insert(0, " ");
				strTemp.insert(0, strTTemp);
				strTemp.insert(0, " ");
				strTemp.insert(0, "notg #(0, 0)");
				FO_TEMP<<strTemp<<endl;
			}
		}
		gateTemp = gateTemp->next;
	}

	gateTemp = Gatehead;

	fanoutCnt = 0;

	while(gateTemp){
		if(gateTemp->Type == 4){//FDCE
			string strTemp = "";
			strTemp.insert(0, "dff ");
			dffCnt++;
			strTemp.insert(strTemp.length(), "DFF_");
			strTemp.insert(strTemp.length(), int2str(dffCnt));
			strTemp.insert(strTemp.length(), " ");
			strTemp.insert(strTemp.length(), gateTemp->INS);
			strTemp.insert(strTemp.length(), " (");
			strTemp.insert(strTemp.length(), gateTemp->Q);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), gateTemp->D);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), gateTemp->C);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), gateTemp->CLR);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), "1'b0");
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), gateTemp->CE);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), NbarT);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), serialInwire);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), "1'b0");
			strTemp.insert(strTemp.length(), ");");
			FO_TEMP<<strTemp<<endl;
		}

		if(gateTemp->Type == 5){//FD
			string strTemp = "";
			strTemp.insert(0, "dff ");//Q, D, C, CLR, PRE, CE, NbarT, Si, global_reset
			dffCnt++;
			strTemp.insert(strTemp.length(), "DFF_");
			strTemp.insert(strTemp.length(), int2str(dffCnt));
			strTemp.insert(strTemp.length(), " ");
			strTemp.insert(strTemp.length(), gateTemp->INS);
			strTemp.insert(strTemp.length(), " (");
			strTemp.insert(strTemp.length(), gateTemp->Q);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), gateTemp->D);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), gateTemp->C);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), "1'b0");
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), "1'b0");
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), "1'b1");
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), NbarT);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), serialInwire);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), "1'b0");
			strTemp.insert(strTemp.length(), ");");
			FO_TEMP<<strTemp<<endl;
		}

		

		if(gateTemp->Type == 6){//FDC
			string strTemp = "";
			strTemp.insert(0, "dff ");//Q, D, C, CLR, PRE, CE, NbarT, Si, global_reset
			dffCnt++;
			strTemp.insert(strTemp.length(), "DFF_");
			strTemp.insert(strTemp.length(), int2str(dffCnt));
			strTemp.insert(strTemp.length(), " ");
			strTemp.insert(strTemp.length(), gateTemp->INS);
			strTemp.insert(strTemp.length(), " (");
			strTemp.insert(strTemp.length(), gateTemp->Q);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), gateTemp->D);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), gateTemp->C);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), gateTemp->CLR);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), "1'b0");
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), "1'b1");
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), NbarT);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), serialInwire);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), "1'b0");
			strTemp.insert(strTemp.length(), ");");
			FO_TEMP<<strTemp<<endl;
		}

		if(gateTemp->Type == 7){//FDCPE
			string strTemp = "";
			strTemp.insert(0, "dff ");//Q, D, C, CLR, PRE, CE, NbarT, Si, global_reset
			dffCnt++;
			strTemp.insert(strTemp.length(), "DFF_");
			strTemp.insert(strTemp.length(), int2str(dffCnt));
			strTemp.insert(strTemp.length(), " ");
			strTemp.insert(strTemp.length(), gateTemp->INS);
			strTemp.insert(strTemp.length(), " (");
			strTemp.insert(strTemp.length(), gateTemp->Q);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), gateTemp->D);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), gateTemp->C);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), gateTemp->CLR);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), gateTemp->PRE);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), gateTemp->CE);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), NbarT);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), serialInwire);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), "1'b0");
			strTemp.insert(strTemp.length(), ");");
			FO_TEMP<<strTemp<<endl;
		}

		

		if(gateTemp->Type == 8){
			string strTemp = "";
			strTemp.insert(0, "LDCP ");
			strTemp.insert(strTemp.length(), gateTemp->INS);
			strTemp.insert(strTemp.length(), " (");
			strTemp.insert(strTemp.length(), gateTemp->Output);
			strTemp.insert(strTemp.length(), ", ");

			node* Temp = gateTemp->input;
			int inputCnt = 0;
			while(Temp){
				inputCnt++;
				strTemp.insert(strTemp.length(), Temp->s);
				strTemp.insert(strTemp.length(), ", ");
				Temp = Temp->next;
			}
			strTemp.erase(strTemp.length()-2, 2);
			strTemp.insert(strTemp.length(), ");");
			ERR<<"//Can not handle latches in test application//"<<endl;
			cout<<"//Can not handle latches in test application//"<<endl;
			FO_TEMP<<"//Can not handle latches in test application//"<<endl;
			FO_TEMP<<strTemp<<endl;
		}
		gateTemp = gateTemp->next;
	}




	FO_TEMP<<endl<<"endmodule"<<endl;






	//printEQ_Components();
	printNet_Components();
	//printMem_module();

	FSNetlistGeneratorV2();
	

	cout<<"**************************************************\n";
	cout<<"*****                                         ****\n";
	cout<<"*****                                         ****\n";
	cout<<"*****                Report                   ****\n";
	cout<<"*****                                         ****\n";
	cout<<"*****                                         ****\n";
	cout<<"**************************************************\n";
	cout<<"\n#AND gates : "<<andCnt;
	cout<<"\n#NAND gates : "<<nandCnt;
	cout<<"\n#OR gates : "<<orCnt;
	cout<<"\n#NOR gates : "<<norCnt;
	cout<<"\n#XOR gates : "<<xorCnt;
	cout<<"\n#XNOR gates : "<<xnorCnt;
	cout<<"\n#NOT gates : "<<notCnt;
	cout<<"\n#BUF gates : "<<bufferCnt;
	cout<<"\n#DFFs : "<<dffCnt;

	cout<<"\nThe netlist Verilog file generated. It's name is : "<<verilogFile<<".\n";
	cout<<"**************************************************\n";
	//cout<<"Thanks.\nCAD RESEARCH GROUP\nUNIVERSITY OF TEHRAN\n";
	cout<<"M. Hashem Haghbayan\nhashem@cad.ut.ac.ir\n\n\n";
	
	
	INF<<"**************************************************\n";
	INF<<"*****                                         ****\n";
	INF<<"*****                                         ****\n";
	INF<<"*****                Report                   ****\n";
	INF<<"*****                                         ****\n";
	INF<<"*****                                         ****\n";
	INF<<"**************************************************\n";
	INF<<"\n#AND gates : "<<andCnt;
	INF<<"\n#NAND gates : "<<nandCnt;
	INF<<"\n#OR gates : "<<orCnt;
	INF<<"\n#NOR gates : "<<norCnt;
	INF<<"\n#XOR gates : "<<xorCnt;
	INF<<"\n#XNOR gates : "<<xnorCnt;
	INF<<"\n#NOT gates : "<<notCnt;
	INF<<"\n#BUF gates : "<<bufferCnt;
	INF<<"\n#DFFs : "<<dffCnt;

	INF<<"\nThe netlist Verilog file generated. It's name is : "<<verilogFile<<".\n";
	cout<<"\n**************************************************\n";
	//INF<<"Thanks.\nCAD RESEARCH GROUP\nUNIVERSITY OF TEHRAN\n";
	INF<<"M. Hashem Haghbayan\nhashem@cad.ut.ac.ir\n\n\n";
		
	cout<<"Press any key to end...";
	getch();
	exit(0);


	return 0;
}





int netlistGenerator::FSNetlistGeneratorV2(){

	cout<<"\n\nGenerating netlist for fault simulation...\n";

	string verilogFile = ModuleName;
	verilogFile.insert(0, "FSnetlist_");
	verilogFile.insert(verilogFile.length(), "_V1");
	//verilogFile.insert(verilogFile.length(), ".v");
	char verilog[256];
	for(int i=0; i<verilogFile.length(); i++) verilog[i] = verilogFile[i];
	verilog[verilogFile.length()]  = 0;

	remove(verilog);


	ofstream FO_TEMP(verilog);
	
	
	node* nodeTemp = EX_Inputs_1;
	FO_TEMP<<"//Inputs"<<endl;
	while(nodeTemp){
		FO_TEMP<<nodeTemp->s<<endl;
		nodeTemp = nodeTemp->next;
	}

	nodeTemp = EX_Outputs_1;
	FO_TEMP<<"//Outputs"<<endl;
	while(nodeTemp){
		FO_TEMP<<nodeTemp->s<<endl;
		nodeTemp = nodeTemp->next;
	}

	GateStruct* gateTemp = Gatehead;
	

	while(gateTemp){
		if(gateTemp->Type == 1){
			if(gateTemp->invert == 0){
				FO_TEMP<<"//AND"<<endl;
				FO_TEMP<<gateTemp->Output<<endl;
				node* Temp = gateTemp->input;
				while(Temp){
					FO_TEMP<<Temp->s<<endl;
					Temp = Temp->next;
				}
			}
			if(gateTemp->invert == 1){
				FO_TEMP<<"//NAND"<<endl;
				FO_TEMP<<gateTemp->Output<<endl;
				node* Temp = gateTemp->input;
				while(Temp){
					FO_TEMP<<Temp->s<<endl;
					Temp = Temp->next;
				}
			}
		}

		if(gateTemp->Type == 2){
			if(gateTemp->invert == 0){
				FO_TEMP<<"//OR"<<endl;
				FO_TEMP<<gateTemp->Output<<endl;
				node* Temp = gateTemp->input;
				while(Temp){
					FO_TEMP<<Temp->s<<endl;
					Temp = Temp->next;
				}
			}
			if(gateTemp->invert == 1){
				FO_TEMP<<"//NOR"<<endl;
				FO_TEMP<<gateTemp->Output<<endl;
				node* Temp = gateTemp->input;
				while(Temp){
					FO_TEMP<<Temp->s<<endl;
					Temp = Temp->next;
				}
			}
		}

		if(gateTemp->Type == 3){
			if(gateTemp->invert == 0){
				FO_TEMP<<"//XOR"<<endl;
				FO_TEMP<<gateTemp->Output<<endl;
				node* Temp = gateTemp->input;
				while(Temp){
					FO_TEMP<<Temp->s<<endl;
					Temp = Temp->next;
				}
			}
			if(gateTemp->invert == 1){
				FO_TEMP<<"//XNOR"<<endl;
				FO_TEMP<<gateTemp->Output<<endl;
				node* Temp = gateTemp->input;
				while(Temp){
					FO_TEMP<<Temp->s<<endl;
					Temp = Temp->next;
				}
			}
		}

		if(gateTemp->Type == 0){
			if(gateTemp->invert == 0){
				FO_TEMP<<"//BUFF"<<endl;
				FO_TEMP<<gateTemp->Output<<endl;
				node* Temp = gateTemp->input;
				while(Temp){
					FO_TEMP<<Temp->s<<endl;
					Temp = Temp->next;
				}
			}
			if(gateTemp->invert == 1){
				FO_TEMP<<"//NOT"<<endl;
				FO_TEMP<<gateTemp->Output<<endl;
				node* Temp = gateTemp->input;
				while(Temp){
					FO_TEMP<<Temp->s<<endl;
					Temp = Temp->next;
				}
			}
		}

		if(gateTemp->Type == 10){
			if(gateTemp->invert == 0){
				FO_TEMP<<"//BUFF"<<endl;
				FO_TEMP<<gateTemp->Output<<endl;
				node* Temp = gateTemp->input;
				while(Temp){
					FO_TEMP<<Temp->s<<endl;
					Temp = Temp->next;
				}
			}
			if(gateTemp->invert == 1){
				FO_TEMP<<"//NOT"<<endl;
				FO_TEMP<<gateTemp->Output<<endl;
				node* Temp = gateTemp->input;
				while(Temp){
					FO_TEMP<<Temp->s<<endl;
					Temp = Temp->next;
				}
			}
		}

		if(gateTemp->Type == 11){
			if(gateTemp->invert == 0){
				FO_TEMP<<"//BUFF"<<endl;
				FO_TEMP<<gateTemp->Output<<endl;
				node* Temp = gateTemp->input;
				while(Temp){
					FO_TEMP<<Temp->s<<endl;
					Temp = Temp->next;
				}
			}
			if(gateTemp->invert == 1){
				FO_TEMP<<"//NOT"<<endl;
				FO_TEMP<<gateTemp->Output<<endl;
				node* Temp = gateTemp->input;
				while(Temp){
					FO_TEMP<<Temp->s<<endl;
					Temp = Temp->next;
				}
			}
		}

		if(gateTemp->Type == 9){
			if(gateTemp->invert == 0){
				FO_TEMP<<"//FANOUT"<<endl;
				FO_TEMP<<gateTemp->Output<<endl;
				node* Temp = gateTemp->input;
				while(Temp){
					FO_TEMP<<Temp->s<<endl;
					Temp = Temp->next;
				}
			}
			if(gateTemp->invert == 1){
				FO_TEMP<<"//FANOUT"<<endl;
				FO_TEMP<<gateTemp->Output<<endl;
				node* Temp = gateTemp->input;
				while(Temp){
					FO_TEMP<<Temp->s<<endl;
					Temp = Temp->next;
				}
			}
		}


		gateTemp = gateTemp->next;
	}

	gateTemp = Gatehead;

	while(gateTemp){
		if(gateTemp->Type == 4 || gateTemp->Type == 5 || gateTemp->Type == 6 || gateTemp->Type == 7){//FDCE
			FO_TEMP<<"//DFF"<<endl;
			if(gateTemp->Q != "")
				FO_TEMP<<gateTemp->Q<<endl;
			
			if(gateTemp->D != "")
				FO_TEMP<<gateTemp->D<<endl;
			
			if(gateTemp->C != "")
				FO_TEMP<<gateTemp->C<<endl;
			
			if(gateTemp->CLR != "")
				FO_TEMP<<gateTemp->CLR<<endl;
			else
				FO_TEMP<<"1'b0"<<endl;
			
			if(gateTemp->PRE != "")
				FO_TEMP<<gateTemp->PRE<<endl;
			else
				FO_TEMP<<"1'b0"<<endl;
			
			if(gateTemp->CE != "")
				FO_TEMP<<gateTemp->CE<<endl;
			else
				FO_TEMP<<"1'b1"<<endl;
		}
		gateTemp = gateTemp->next;
	}

	FO_TEMP<<"//END"<<endl;

	/*gateTemp = Gatehead;

	fanoutCnt = 0;

	while(gateTemp){
		if(gateTemp->Type == 4){//FDCE
			string strTemp = "";
			strTemp.insert(0, "dff ");
			dffCnt++;
			strTemp.insert(strTemp.length(), "DFF_");
			strTemp.insert(strTemp.length(), int2str(dffCnt));
			strTemp.insert(strTemp.length(), " ");
			strTemp.insert(strTemp.length(), gateTemp->INS);
			strTemp.insert(strTemp.length(), " (");
			strTemp.insert(strTemp.length(), gateTemp->Q);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), gateTemp->D);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), gateTemp->C);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), gateTemp->CLR);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), "1'b0");
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), gateTemp->CE);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), NbarT);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), serialInwire);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), "1'b0");
			strTemp.insert(strTemp.length(), ");");
			FO_TEMP<<strTemp<<endl;
		}

		if(gateTemp->Type == 5){//FD
			string strTemp = "";
			strTemp.insert(0, "dff ");//Q, D, C, CLR, PRE, CE, NbarT, Si, global_reset
			dffCnt++;
			strTemp.insert(strTemp.length(), "DFF_");
			strTemp.insert(strTemp.length(), int2str(dffCnt));
			strTemp.insert(strTemp.length(), " ");
			strTemp.insert(strTemp.length(), gateTemp->INS);
			strTemp.insert(strTemp.length(), " (");
			strTemp.insert(strTemp.length(), gateTemp->Q);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), gateTemp->D);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), gateTemp->C);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), "1'b0");
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), "1'b0");
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), "1'b1");
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), NbarT);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), serialInwire);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), "1'b0");
			strTemp.insert(strTemp.length(), ");");
			FO_TEMP<<strTemp<<endl;
		}

		

		if(gateTemp->Type == 6){//FDC
			string strTemp = "";
			strTemp.insert(0, "dff ");//Q, D, C, CLR, PRE, CE, NbarT, Si, global_reset
			dffCnt++;
			strTemp.insert(strTemp.length(), "DFF_");
			strTemp.insert(strTemp.length(), int2str(dffCnt));
			strTemp.insert(strTemp.length(), " ");
			strTemp.insert(strTemp.length(), gateTemp->INS);
			strTemp.insert(strTemp.length(), " (");
			strTemp.insert(strTemp.length(), gateTemp->Q);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), gateTemp->D);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), gateTemp->C);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), gateTemp->CLR);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), "1'b0");
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), "1'b1");
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), NbarT);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), serialInwire);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), "1'b0");
			strTemp.insert(strTemp.length(), ");");
			FO_TEMP<<strTemp<<endl;
		}

		if(gateTemp->Type == 7){//FDCPE
			string strTemp = "";
			strTemp.insert(0, "dff ");//Q, D, C, CLR, PRE, CE, NbarT, Si, global_reset
			dffCnt++;
			strTemp.insert(strTemp.length(), "DFF_");
			strTemp.insert(strTemp.length(), int2str(dffCnt));
			strTemp.insert(strTemp.length(), " ");
			strTemp.insert(strTemp.length(), gateTemp->INS);
			strTemp.insert(strTemp.length(), " (");
			strTemp.insert(strTemp.length(), gateTemp->Q);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), gateTemp->D);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), gateTemp->C);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), gateTemp->CLR);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), gateTemp->PRE);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), gateTemp->CE);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), NbarT);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), serialInwire);
			strTemp.insert(strTemp.length(), ", ");
			strTemp.insert(strTemp.length(), "1'b0");
			strTemp.insert(strTemp.length(), ");");
			FO_TEMP<<strTemp<<endl;
		}

		

		if(gateTemp->Type == 8){
			string strTemp = "";
			strTemp.insert(0, "LDCP ");
			strTemp.insert(strTemp.length(), gateTemp->INS);
			strTemp.insert(strTemp.length(), " (");
			strTemp.insert(strTemp.length(), gateTemp->Output);
			strTemp.insert(strTemp.length(), ", ");

			node* Temp = gateTemp->input;
			int inputCnt = 0;
			while(Temp){
				inputCnt++;
				strTemp.insert(strTemp.length(), Temp->s);
				strTemp.insert(strTemp.length(), ", ");
				Temp = Temp->next;
			}
			strTemp.erase(strTemp.length()-2, 2);
			strTemp.insert(strTemp.length(), ");");
			ERR<<"//Can not handle latches in test application//"<<endl;
			cout<<"//Can not handle latches in test application//"<<endl;
			FO_TEMP<<"//Can not handle latches in test application//"<<endl;
			FO_TEMP<<strTemp<<endl;
		}
		gateTemp = gateTemp->next;
	}
	*/


	cout<<"\nGenerating fault simulation file finished...\n\n";


	return 0;
}




void netlistGenerator::printingTestForNetlist(){

	cout<<"\n\nGenerating Verilog netlist...\n";

	string verilogFile = ModuleName;
	verilogFile.insert(0, "test_netlist_");
	verilogFile.insert(verilogFile.length(), "_V1");
	verilogFile.insert(verilogFile.length(), ".v");
	char verilog[256];
	for(int i=0; i<256; i++) verilog[i] = verilogFile[i];

	remove(verilog);


	ofstream FO_TEMP(verilog);

	FO_TEMP<<endl<<"`timescale 1 ns / 1ns"<<endl;

	FO_TEMP<<"module Test_"<<ModuleName<<"();"<<endl;

	FO_TEMP<<"reg "<<GlobalReset<<";"<<endl;

	node* Temp = InputsStr;

	if(Temp){
			while(Temp->next){
			string strTemp = Temp->s;
			strTemp = erase_space(strTemp);
			strTemp.erase(0, 5);
			strTemp.insert(0, "reg ");
			FO_TEMP<<strTemp<<";"<<endl;
			Temp = Temp->next;
		}
	}
	if(InputsStr) {
		string strTemp = Temp->s;
		strTemp = erase_space(strTemp);
		strTemp.erase(0, 5);
		strTemp.insert(0, "reg ");
		FO_TEMP<<strTemp<<";"<<endl<<endl;
	}

	Temp = OutputsStr;

	//For main module---------------------------------------
	if(Temp){
		while(Temp->next){
			string strTemp = Temp->s;
			strTemp = erase_space(strTemp);
			strTemp.erase(0, 6);
			strTemp.insert(0, "wire ");
			FO_TEMP<<strTemp<<";"<<endl;
			Temp = Temp->next;
		}
	}

	if(OutputsStr) {
		string strTemp = Temp->s;
		strTemp = erase_space(strTemp);
		strTemp.erase(0, 6);
		strTemp.insert(0, "wire ");
		FO_TEMP<<strTemp<<";"<<endl<<endl;
	}

	//For Equations------------------------------------------
	if(Temp){
		while(Temp->next){
			string strTemp = Temp->s;
			strTemp = erase_space(strTemp);
			strTemp.erase(0, 6);
			strTemp.insert(0, "wire ");
			strTemp.insert(strTemp.length(), "_eq");
			FO_TEMP<<strTemp<<";"<<endl;
			Temp = Temp->next;
		}
	}

	if(OutputsStr) {
		string strTemp = Temp->s;
		strTemp = erase_space(strTemp);
		strTemp.erase(0, 6);
		strTemp.insert(0, "wire ");
		strTemp.insert(strTemp.length(), "_eq");
		FO_TEMP<<strTemp<<";"<<endl<<endl;
	}

	//For netlist----------------------------------------------
	if(Temp){
		while(Temp->next){
			string strTemp = Temp->s;
			strTemp = erase_space(strTemp);
			strTemp.erase(0, 6);
			strTemp.insert(0, "wire ");
			strTemp.insert(strTemp.length(), "_net");
			FO_TEMP<<strTemp<<";"<<endl;
			Temp = Temp->next;
		}
	}

	if(OutputsStr) {
		string strTemp = Temp->s;
		strTemp = erase_space(strTemp);
		strTemp.erase(0, 6);
		strTemp.insert(0, "wire ");
		strTemp.insert(strTemp.length(), "_net");
		FO_TEMP<<strTemp<<";"<<endl<<endl;
	}

	//for dataBus input file
	Temp = EX_Inputs2;
	int inputCntT = 0;
	if(Temp){
		while(Temp->next){
			Temp = Temp->next;
			inputCntT++;
		}
		inputCntT++;
	}

	if (inputCntT == 1) FO_TEMP<<"reg MHMH_dataBus;"<<endl;
	else
		FO_TEMP<<"reg [0:"<<inputCntT-1<<"]MHMH_dataBus;"<<endl;

	FO_TEMP<<"reg [31:0] addBus = 32'h00000000;"<<endl;
	FO_TEMP<<"reg MW;"<<endl;


	//For main module---------------------------------------------
	FO_TEMP<<endl<<ModuleName<<" INS_test(";

	Temp = Inputs;

	if(Temp){
		while(Temp->next){
			FO_TEMP<<Temp->s<<","<<endl;
			Temp = Temp->next;
		}
		FO_TEMP<<Temp->s;

	}



	Temp = Outputs;

	if(Temp){
		FO_TEMP<<","<<endl;
		while(Temp->next){
			FO_TEMP<<Temp->s<<","<<endl;
			Temp = Temp->next;
		}
		FO_TEMP<<Temp->s;

	}


	FO_TEMP<<");"<<endl;

	//For equations----------------------------------------------
	FO_TEMP<<endl<<ModuleName<<"_eq INS_test_eq("<<GlobalReset<<", ";

	Temp = Inputs;

	if(Temp){
		while(Temp->next){
			FO_TEMP<<Temp->s<<","<<endl;
			Temp = Temp->next;
		}
		FO_TEMP<<Temp->s;

	}



	Temp = Outputs;

	if(Temp){
		FO_TEMP<<","<<endl;
		while(Temp->next){
			FO_TEMP<<Temp->s<<"_eq,"<<endl;
			Temp = Temp->next;
		}
		FO_TEMP<<Temp->s<<"_eq";

	}


	FO_TEMP<<");"<<endl;


	//For netlist----------------------------------------------------
	FO_TEMP<<endl<<ModuleName<<"_net INS_test_net("<<GlobalReset<<", ";

	Temp = Inputs;

	if(Temp){
		while(Temp->next){
			FO_TEMP<<Temp->s<<","<<endl;
			Temp = Temp->next;
		}
		FO_TEMP<<Temp->s;

	}



	Temp = Outputs;

	if(Temp){
		FO_TEMP<<","<<endl;
		while(Temp->next){
			FO_TEMP<<Temp->s<<"_net,"<<endl;
			Temp = Temp->next;
		}
		FO_TEMP<<Temp->s<<"_net";

	}


	FO_TEMP<<");"<<endl<<endl<<endl<<endl;

	//For memory--------------------------------------------------

	FO_TEMP<<"initial begin"<<endl;
	FO_TEMP<<"MW = 0;"<<endl;
	FO_TEMP<<"addBus = 32'h00000000;"<<endl;
	FO_TEMP<<"end"<<endl;




	FO_TEMP<<"always @(";

	Temp = Inputs;

	if(Temp){
		while(Temp->next){
			FO_TEMP<<Temp->s<<","<<endl;
			Temp = Temp->next;
		}
		FO_TEMP<<Temp->s;

	}

	FO_TEMP<<") begin"<<endl;
	FO_TEMP<<"addBus = addBus+1;"<<endl<<endl;

	Temp = EX_Inputs2;

	inputCntT = 0;
	if(Temp){
		while(Temp->next){
			FO_TEMP<<"MHMH_dataBus["<<inputCntT<<"] = "<<Temp->s<<";"<<endl;
			Temp = Temp->next;
			inputCntT++;
		}
		FO_TEMP<<"MHMH_dataBus["<<inputCntT<<"] = "<<Temp->s<<";"<<endl;
		inputCntT++;
	}

	FO_TEMP<<"end"<<endl<<endl;

	FO_TEMP<<"memModule #("<<inputCntT<<") INS_MEM ("<<GlobalReset<<", MW, addBus, MHMH_dataBus);"<<endl<<endl<<endl<<endl;




	//---------------------------------------------------------

	FO_TEMP<<"endmodule"<<endl;


	FO_TEMP.close();


	return;
}

void netlistGenerator::printEQ_Components(){



	ofstream FO_TEMP1("Eq_components.v");





		//FO_TEMP1<<"//Components for smulation the equation file"<<endl;
		//FO_TEMP1<<"//M.H. Haghbayan"<<endl;
		//FO_TEMP1<<"//CAD RESEARCH GROUP"<<endl;
		//FO_TEMP1<<"//UNIVERSITY OF TEHRAN"<<endl;
		FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
		FO_TEMP1<<"//    D Flip Flop"<<endl;
		FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
		FO_TEMP1<<"module FDCPE"<<endl;
		FO_TEMP1<<"#(parameter tphl = 0, tplh = 0)"<<endl;
		FO_TEMP1<<"(Q,D,C,CLR,PRE,CE, global_reset);"<<endl;
		FO_TEMP1<<""<<endl;
		FO_TEMP1<<"output reg Q;"<<endl;
		FO_TEMP1<<"input D, C, CLR, PRE, CE, global_reset;"<<endl;
		FO_TEMP1<<""<<endl;
		FO_TEMP1<<"always @ (posedge CLR or posedge PRE or posedge C or posedge global_reset) begin"<<endl;
		FO_TEMP1<<"   if (CLR || global_reset)"<<endl;
		FO_TEMP1<<"      Q <= 0;"<<endl;
		FO_TEMP1<<"   else if (PRE)"<<endl;
		FO_TEMP1<<"      Q <= 1;"<<endl;
		FO_TEMP1<<"   else if (CE)"<<endl;
		FO_TEMP1<<"      Q <= D;"<<endl;
		FO_TEMP1<<"end"<<endl;
		FO_TEMP1<<"endmodule"<<endl;
		FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
		FO_TEMP1<<"//    T Flip Flop"<<endl;
		FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
		FO_TEMP1<<"module FTCPE"<<endl;
		FO_TEMP1<<"#(parameter tphl = 0, tplh = 0)"<<endl;
		FO_TEMP1<<"(Q,D,C,CLR,PRE,CE, global_reset);"<<endl;
		FO_TEMP1<<""<<endl;
		FO_TEMP1<<"output reg Q;"<<endl;
		FO_TEMP1<<"input D, C, CLR, PRE, CE, global_reset;"<<endl;
		FO_TEMP1<<""<<endl;
		FO_TEMP1<<"always @ (posedge CLR or posedge PRE or posedge C or posedge global_reset) begin"<<endl;
		FO_TEMP1<<"   if (CLR || global_reset)"<<endl;
		FO_TEMP1<<"     Q <= 0;"<<endl;
		FO_TEMP1<<"   else if (PRE)"<<endl;
		FO_TEMP1<<"      Q <= 1;"<<endl;
		FO_TEMP1<<"   else if (CE)"<<endl;
		FO_TEMP1<<"      if (D)"<<endl;
		FO_TEMP1<<"      	Q <= ~Q;"<<endl;
		FO_TEMP1<<"end"<<endl;
		FO_TEMP1<<"endmodule"<<endl;
		FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
		FO_TEMP1<<"//    T Latch"<<endl;
		FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
		FO_TEMP1<<"module FTDCPE"<<endl;
		FO_TEMP1<<"#(parameter tphl = 0, tplh = 0)"<<endl;
		FO_TEMP1<<"(Q,D,C,CLR,PRE,CE, global_reset);"<<endl;
		FO_TEMP1<<""<<endl;
		FO_TEMP1<<"output reg Q;"<<endl;
		FO_TEMP1<<"input D, C, CLR, PRE, CE, global_reset;"<<endl;
		FO_TEMP1<<""<<endl;
		FO_TEMP1<<"always @ (posedge CLR or posedge PRE or posedge C or negedge C or posedge global_reset) begin"<<endl;
		FO_TEMP1<<"   if (CLR || global_reset)"<<endl;
		FO_TEMP1<<"     Q <= 0;"<<endl;
		FO_TEMP1<<"   else if (PRE)"<<endl;
		FO_TEMP1<<"      Q <= 1;"<<endl;
		FO_TEMP1<<"   else if (CE)"<<endl;
		FO_TEMP1<<"      if (D)"<<endl;
		FO_TEMP1<<"      	Q <= ~Q;"<<endl;
		FO_TEMP1<<"end"<<endl;
		FO_TEMP1<<"endmodule"<<endl;
		FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
		FO_TEMP1<<"//    D Latch"<<endl;
		FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
		FO_TEMP1<<"module FDDCPE"<<endl;
		FO_TEMP1<<"#(parameter tphl = 0, tplh = 0)"<<endl;
		FO_TEMP1<<"(Q,D,C,CLR,PRE,CE, global_reset);"<<endl;
		FO_TEMP1<<""<<endl;
		FO_TEMP1<<"output reg Q;"<<endl;
		FO_TEMP1<<"input D, C, CLR, PRE, CE, global_reset;"<<endl;
		FO_TEMP1<<""<<endl;
		FO_TEMP1<<"always @ (posedge C or negedge C or posedge CLR or posedge PRE or posedge global_reset)"<<endl;
		FO_TEMP1<<"begin"<<endl;
		FO_TEMP1<<"if (CLR || global_reset)"<<endl;
		FO_TEMP1<<"Q <= 1'b0;"<<endl;
		FO_TEMP1<<"else"<<endl;
		FO_TEMP1<<"if (PRE)"<<endl;
		FO_TEMP1<<"Q <= 1'b1;"<<endl;
		FO_TEMP1<<"else"<<endl;
		FO_TEMP1<<"if (CE)"<<endl;
		FO_TEMP1<<"Q <= D ;"<<endl;
		FO_TEMP1<<"end"<<endl;
		FO_TEMP1<<"endmodule"<<endl;
		FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
		FO_TEMP1<<"//    Latch"<<endl;
		FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
		FO_TEMP1<<"module LDCP"<<endl;
		FO_TEMP1<<"#(parameter tphl = 0, tplh = 0)"<<endl;
		FO_TEMP1<<"(Q,D,G,CLR,PRE, global_reset);"<<endl;
		FO_TEMP1<<""<<endl;
		FO_TEMP1<<"output reg Q;"<<endl;
		FO_TEMP1<<"input D, G, CLR, PRE, global_reset;"<<endl;
		FO_TEMP1<<""<<endl;
		FO_TEMP1<<"always @ (CLR or PRE or D or G or posedge global_reset) begin"<<endl;
		FO_TEMP1<<"if (CLR || global_reset)"<<endl;
		FO_TEMP1<<"Q <= 0;"<<endl;
		FO_TEMP1<<"else if (PRE)"<<endl;
		FO_TEMP1<<"Q <= 1;"<<endl;
		FO_TEMP1<<"else if (G)"<<endl;
		FO_TEMP1<<"Q <= D;"<<endl;
		FO_TEMP1<<"end"<<endl;
		FO_TEMP1<<"endmodule"<<endl;
		FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
		FO_TEMP1<<"//    Not"<<endl;
		FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
		FO_TEMP1<<"module notg"<<endl;
		FO_TEMP1<<"#(parameter tphl = 0, tplh = 0)"<<endl;
		FO_TEMP1<<"(out,in);"<<endl;
		FO_TEMP1<<""<<endl;
		FO_TEMP1<<"input in;"<<endl;
		FO_TEMP1<<"output reg out;"<<endl;
		FO_TEMP1<<"reg val;"<<endl;
		FO_TEMP1<<""<<endl;
		FO_TEMP1<<"always@(in) begin"<<endl;
		FO_TEMP1<<"val = ~ in;"<<endl;
		FO_TEMP1<<"end "<<endl;
		FO_TEMP1<<"always@(posedge val) #tplh out =  val;"<<endl;
		FO_TEMP1<<"always@(negedge val) #tphl out =  val;"<<endl;
		FO_TEMP1<<"endmodule"<<endl;
		FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
		FO_TEMP1<<"//    And: Parametrized on the number of inputs"<<endl;
		FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
		FO_TEMP1<<"module and_n"<<endl;
		FO_TEMP1<<"#(parameter n = 2, tphl = 0, tplh = 0)"<<endl;
		FO_TEMP1<<"(out,in);"<<endl;
		FO_TEMP1<<""<<endl;
		FO_TEMP1<<"input [n-1:0] in;"<<endl;
		FO_TEMP1<<"output reg out;"<<endl;
		FO_TEMP1<<"reg val;"<<endl;
		FO_TEMP1<<" "<<endl;
		FO_TEMP1<<"always@(in) begin     "<<endl;
		FO_TEMP1<<"val=&in;"<<endl;
		FO_TEMP1<<"end "<<endl;
		FO_TEMP1<<"always@(posedge val) #tplh out =  val;"<<endl;
		FO_TEMP1<<"always@(negedge val) #tphl out =  val;"<<endl;
		FO_TEMP1<<"endmodule"<<endl;
		FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
		FO_TEMP1<<"//    Or: Parametrized on the number of inputs       "<<endl;
		FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
		FO_TEMP1<<"module or_n"<<endl;
		FO_TEMP1<<"#(parameter n = 2, tphl = 0, tplh = 0)"<<endl;
		FO_TEMP1<<"(out,in);             "<<endl;
		FO_TEMP1<<""<<endl;
		FO_TEMP1<<"input [n-1:0] in;"<<endl;
		FO_TEMP1<<"output reg out;"<<endl;
		FO_TEMP1<<"reg val;"<<endl;
		FO_TEMP1<<""<<endl;
		FO_TEMP1<<"always@(in) begin     "<<endl;
		FO_TEMP1<<"val=|in;"<<endl;
		FO_TEMP1<<"end "<<endl;
		FO_TEMP1<<"always@(posedge val) #tplh out =  val;"<<endl;
		FO_TEMP1<<"always@(negedge val) #tphl out =  val;"<<endl;
		FO_TEMP1<<"endmodule"<<endl;
		FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
		FO_TEMP1<<"//    Nand: Parametrized on the number of inputs       "<<endl;
		FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
		FO_TEMP1<<"module nand_n"<<endl;
		FO_TEMP1<<"#(parameter n = 2, tphl = 0, tplh = 0)"<<endl;
		FO_TEMP1<<"(out,in);               "<<endl;
		FO_TEMP1<<""<<endl;
		FO_TEMP1<<"input [n-1:0] in;"<<endl;
		FO_TEMP1<<"output reg out;"<<endl;
		FO_TEMP1<<"reg val;"<<endl;
		FO_TEMP1<<" "<<endl;
		FO_TEMP1<<"always@(in) begin     "<<endl;
		FO_TEMP1<<"val=&in;"<<endl;
		FO_TEMP1<<"end "<<endl;
		FO_TEMP1<<"always@(posedge val) #tplh out =  ~val;"<<endl;
		FO_TEMP1<<"always@(negedge val) #tphl out =  ~val;"<<endl;
		FO_TEMP1<<"endmodule"<<endl;
		FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
		FO_TEMP1<<"//    Nor: Parametrized on the number of inputs       "<<endl;
		FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
		FO_TEMP1<<"module nor_n"<<endl;
		FO_TEMP1<<"#(parameter n = 2, tphl = 0, tplh = 0)"<<endl;
		FO_TEMP1<<"(out,in);              "<<endl;
		FO_TEMP1<<""<<endl;
		FO_TEMP1<<"input [n-1:0] in;"<<endl;
		FO_TEMP1<<"output reg out;"<<endl;
		FO_TEMP1<<"reg val;"<<endl;
		FO_TEMP1<<""<<endl;
		FO_TEMP1<<"always@(in) begin     "<<endl;
		FO_TEMP1<<"val=|in;"<<endl;
		FO_TEMP1<<"end "<<endl;
		FO_TEMP1<<"always@(posedge val) #tplh out =  ~val;"<<endl;
		FO_TEMP1<<"always@(negedge val) #tphl out =  ~val;"<<endl;
		FO_TEMP1<<"endmodule"<<endl;
		FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
		FO_TEMP1<<"//    Xor: Parametrized on the number of inputs       "<<endl;
		FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
		FO_TEMP1<<"module xor_n"<<endl;
		FO_TEMP1<<"#(parameter n = 2, tphl = 0, tplh = 0)"<<endl;
		FO_TEMP1<<"(out,in);              "<<endl;
		FO_TEMP1<<""<<endl;
		FO_TEMP1<<"input [n-1:0] in;"<<endl;
		FO_TEMP1<<"output reg out;"<<endl;
		FO_TEMP1<<"reg val;"<<endl;
		FO_TEMP1<<" "<<endl;
		FO_TEMP1<<"always@(in) begin     "<<endl;
		FO_TEMP1<<"val=^in;"<<endl;
		FO_TEMP1<<"end "<<endl;
		FO_TEMP1<<"always@(posedge val) #tplh out =  val;"<<endl;
		FO_TEMP1<<"always@(negedge val) #tphl out =  val;"<<endl;
		FO_TEMP1<<"endmodule"<<endl;
		FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
		FO_TEMP1<<"//    Xnor: Parametrized on the number of inputs       "<<endl;
		FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
		FO_TEMP1<<"module xnor_n"<<endl;
		FO_TEMP1<<"#(parameter n = 2, tphl = 0, tplh = 0)"<<endl;
		FO_TEMP1<<"(out,in);              "<<endl;
		FO_TEMP1<<""<<endl;
		FO_TEMP1<<"input [n-1:0] in;"<<endl;
		FO_TEMP1<<"output reg out;"<<endl;
		FO_TEMP1<<"reg val;"<<endl;
		FO_TEMP1<<" "<<endl;
		FO_TEMP1<<"always@(in) begin     "<<endl;
		FO_TEMP1<<"val=^in;"<<endl;
		FO_TEMP1<<"end "<<endl;
		FO_TEMP1<<"always@(posedge val) #tplh out =  ~val;"<<endl;
		FO_TEMP1<<"always@(negedge val) #tphl out =  ~val;"<<endl;
		FO_TEMP1<<"endmodule"<<endl;
		FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
		FO_TEMP1<<"//    Fan_Out: Parametrized on the number of inputs       "<<endl;
		FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
		FO_TEMP1<<"module fanout_n"<<endl;
		FO_TEMP1<<"#(parameter n = 2,tphl = 3, tplh = 5)"<<endl;
		FO_TEMP1<<"(in, out);"<<endl;
		FO_TEMP1<<""<<endl;
		FO_TEMP1<<"input in;"<<endl;
		FO_TEMP1<<"output reg [n-1 :0] out;"<<endl;
		FO_TEMP1<<"reg [n-1:0] val;"<<endl;
		FO_TEMP1<<"integer i;"<<endl;
		FO_TEMP1<<""<<endl;
		FO_TEMP1<<"always@(in) begin    "<<endl;
		FO_TEMP1<<"for(i = 0; i <n; i = i + 1 )"<<endl;
		FO_TEMP1<<"val[i] <= in;"<<endl;
		FO_TEMP1<<"end	"<<endl;
		FO_TEMP1<<"always@(posedge val) #tplh out =  val;"<<endl;
		FO_TEMP1<<"always@(negedge val) #tphl out =  val;"<<endl;
		FO_TEMP1<<"endmodule"<<endl;
		FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
		FO_TEMP1<<"//    Primary Input      "<<endl;
		FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
		FO_TEMP1<<"module pin #(parameter n = 1)"<<endl;
		FO_TEMP1<<"(in, out);"<<endl;
		FO_TEMP1<<"input [n-1:0] in;"<<endl;
		FO_TEMP1<<"output reg [n-1:0] out;"<<endl;
		FO_TEMP1<<"   "<<endl;
		FO_TEMP1<<"always@(in)out =  in;"<<endl;
		FO_TEMP1<<"endmodule"<<endl;
		FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
		FO_TEMP1<<"//    Primary Output "<<endl;
		FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
		FO_TEMP1<<"module pout #(parameter n = 1)"<<endl;
		FO_TEMP1<<"(in, out);"<<endl;
		FO_TEMP1<<""<<endl;
		FO_TEMP1<<"input [n-1:0] in;"<<endl;
		FO_TEMP1<<"output reg [n-1:0] out;"<<endl;
		FO_TEMP1<<""<<endl;
		FO_TEMP1<<"always@(in)out =  in;"<<endl;
		FO_TEMP1<<"endmodule"<<endl;



	FO_TEMP1.close();

	return;
}

void netlistGenerator::printMem_module(){



	ofstream FO_TEMP1("Mem_module.vhd");

	FO_TEMP1<<"--Memory package"<<endl;
	FO_TEMP1<<"--By: D. Amerion, A. Alaghi, and M. H. Haghbayan"<<endl;
	FO_TEMP1<<"--CAD RESEARCH GROUP"<<endl;
	FO_TEMP1<<"--UNIVERSITY OF TEHRAN"<<endl;
	FO_TEMP1<<"LIBRARY IEEE;"<<endl;
	FO_TEMP1<<"USE IEEE.std_logic_1164.ALL;"<<endl;
	FO_TEMP1<<"USE IEEE.std_logic_unsigned.ALL;"<<endl;
	FO_TEMP1<<"use std.textio.all;"<<endl;
	FO_TEMP1<<"------------------------------------------------------"<<endl;
	FO_TEMP1<<"-- memory unit"<<endl;
	FO_TEMP1<<"entity memModule is"<<endl;
	FO_TEMP1<<"	generic(m : integer);"<<endl;
	FO_TEMP1<<"	port( reset : IN std_logic;"<<endl;
	FO_TEMP1<<"	MW: in std_logic;"<<endl;
	FO_TEMP1<<"	addBus: in std_logic_vector (31 downto 0);"<<endl;
	FO_TEMP1<<"	dataBusIn: in std_logic_vector (0 to m-1));"<<endl;
	FO_TEMP1<<"end entity;"<<endl;
	FO_TEMP1<<"--------------------"<<endl;
	FO_TEMP1<<"architecture proc of memModule is"<<endl;
	FO_TEMP1<<"file fo: text; "<<endl;
	FO_TEMP1<<"signal flag : bit := '0';"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"--------------------"<<endl;
	FO_TEMP1<<"begin"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"process(MW, addBus)-- initial read of opcode to memory buffer"<<endl;
	FO_TEMP1<<"VARIABLE L:LINE;"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"begin"<<endl;
	FO_TEMP1<<"	--if (flag = '0') then"<<endl;
	char temp = '"';
	FO_TEMP1<<"	--	file_open (fo,"<<temp<<"OPC_M_W.txt"<<temp<<", write_mode); "<<endl;
	FO_TEMP1<<"	--	flag <= '1';"<<endl;
	FO_TEMP1<<"	--else"<<endl;
	FO_TEMP1<<"	"<<endl;
	FO_TEMP1<<"		if(MW ='0' AND reset = '0') then"<<endl;
	FO_TEMP1<<"			file_open (fo,"<<temp<<"TestData"<<temp<<", append_mode);"<<endl;
	FO_TEMP1<<"		"<<endl;
	FO_TEMP1<<"			----now refresh output file"<<endl;
	FO_TEMP1<<"			write(L, conv_integer(addBus), left);"<<endl;
	FO_TEMP1<<"			write(L, string'("<<temp<<" : "<<temp<<"), left);		"<<endl;
	FO_TEMP1<<"			write(L, To_Bitvector(dataBusIn), left, m);"<<endl;
	FO_TEMP1<<"			writeline(fo, L);"<<endl;
	FO_TEMP1<<"			file_close(fo);"<<endl;
	FO_TEMP1<<"			"<<endl;
	FO_TEMP1<<"		"<<endl;
	FO_TEMP1<<"	--	end if;"<<endl;
	FO_TEMP1<<"	end if;"<<endl;
	FO_TEMP1<<"	"<<endl;
	FO_TEMP1<<"	------read signal driver for data bus -----------"<<endl;
	FO_TEMP1<<"end process;"<<endl;
	FO_TEMP1<<"end architecture;"<<endl;
	FO_TEMP1<<"-------"<<endl;

	FO_TEMP1.close();

	return;
}


void netlistGenerator::printNet_Components(){



	ofstream FO_TEMP1("component_library.v");

	FO_TEMP1<<" //Verilog Library for Test Applications"<<endl;
	FO_TEMP1<<" //CAD Research Group"<<endl;
	FO_TEMP1<<" //School of ECE, University of Tehran"<<endl;
	FO_TEMP1<<" /////////////////////////////////////////////////////////////////////////////////////"<<endl;
	FO_TEMP1<<" //    Buf"<<endl;
	FO_TEMP1<<" /////////////////////////////////////////////////////////////////////////////////////"<<endl;
	FO_TEMP1<<" "<<endl;
	FO_TEMP1<<" `timescale 1 ns / 1ns"<<endl;
	FO_TEMP1<<" "<<endl;
	FO_TEMP1<<" module bufg #(parameter tphl = 1, tplh = 1)"<<endl;
	FO_TEMP1<<"     (out,in);               "<<endl;
	FO_TEMP1<<" input in;"<<endl;
	FO_TEMP1<<" output out;"<<endl;
	FO_TEMP1<<"  "<<endl;
	FO_TEMP1<<"	bufif1 #(tplh, tphl) inst(out, in, 1'b1);"<<endl;
	FO_TEMP1<<"    "<<endl;
	FO_TEMP1<<" endmodule"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<" /////////////////////////////////////////////////////////////////////////////////////"<<endl;
	FO_TEMP1<<" //    Not"<<endl;
	FO_TEMP1<<" /////////////////////////////////////////////////////////////////////////////////////"<<endl;
	FO_TEMP1<<" "<<endl;
	FO_TEMP1<<" `timescale 1 ns / 1ns"<<endl;
	FO_TEMP1<<" "<<endl;
	FO_TEMP1<<" module notg #(parameter tphl = 1, tplh = 1)"<<endl;
	FO_TEMP1<<"     (out,in);             "<<endl;
	FO_TEMP1<<" input in;"<<endl;
	FO_TEMP1<<" output out;"<<endl;
	FO_TEMP1<<"  "<<endl;
	FO_TEMP1<<"	not #(tplh, tphl) inst (out, in);"<<endl;
	FO_TEMP1<<"    "<<endl;
	FO_TEMP1<<" endmodule"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<" /////////////////////////////////////////////////////////////////////////////////////"<<endl;
	FO_TEMP1<<" //    And: Parametrized on the number of inputs      "<<endl;
	FO_TEMP1<<" /////////////////////////////////////////////////////////////////////////////////////"<<endl;
	FO_TEMP1<<" "<<endl;
	FO_TEMP1<<" `timescale 1 ns / 1ns"<<endl;
	FO_TEMP1<<" "<<endl;
	FO_TEMP1<<"module and_n"<<endl;
	FO_TEMP1<<"    #(parameter n = 2, tphl = 1, tplh = 1)"<<endl;
	FO_TEMP1<<"    (out,in);           "<<endl;
	FO_TEMP1<<"input [n-1:0] in;"<<endl;
	FO_TEMP1<<"output out;"<<endl;
	FO_TEMP1<<"wire [n-2:0] mwire;"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"genvar i;"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"	and and_0 (mwire [0], in [0], in [1]);"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"  generate"<<endl;
	FO_TEMP1<<"    for (i=1; i <= n-2; i=i+1) begin : AND_N"<<endl;
	FO_TEMP1<<"       and inst (mwire [i], mwire [i-1], in [i+1]);"<<endl;
	FO_TEMP1<<"    end"<<endl;
	FO_TEMP1<<"  endgenerate"<<endl;
	FO_TEMP1<<"	bufif1 #(tplh, tphl) inst(out, mwire [n-2], 1'b1);	"<<endl;
	FO_TEMP1<<"	"<<endl;
	FO_TEMP1<<"endmodule"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
	FO_TEMP1<<"//    Or: Parametrized on the number of inputs       "<<endl;
	FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"`timescale 1 ns / 1ns"<<endl;
	FO_TEMP1<<" "<<endl;
	FO_TEMP1<<"module or_n"<<endl;
	FO_TEMP1<<"    #(parameter n = 2, tphl = 1, tplh = 1)"<<endl;
	FO_TEMP1<<"    (out,in);             "<<endl;
	FO_TEMP1<<"input [n-1:0] in;"<<endl;
	FO_TEMP1<<"output out;"<<endl;
	FO_TEMP1<<"wire [n-2:0] mwire;"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"genvar i;"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"	or or_0 (mwire [0], in [0], in [1]);"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"  generate"<<endl;
	FO_TEMP1<<"    for (i=1; i <= n-2; i=i+1) begin : OR_N"<<endl;
	FO_TEMP1<<"       or inst (mwire [i], mwire [i-1], in [i+1]);"<<endl;
	FO_TEMP1<<"    end"<<endl;
	FO_TEMP1<<"  endgenerate"<<endl;
	FO_TEMP1<<"	bufif1 #(tplh, tphl) inst(out, mwire [n-2], 1'b1);	"<<endl;
	FO_TEMP1<<"   "<<endl;
	FO_TEMP1<<"endmodule"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
	FO_TEMP1<<"//    Nand: Parametrized on the number of inputs       "<<endl;
	FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"`timescale 1 ns / 1ns"<<endl;
	FO_TEMP1<<" "<<endl;
	FO_TEMP1<<"module nand_n"<<endl;
	FO_TEMP1<<"    #(parameter n = 2, tphl = 1, tplh = 1)"<<endl;
	FO_TEMP1<<"    (out,in);               "<<endl;
	FO_TEMP1<<"input [n-1:0] in;"<<endl;
	FO_TEMP1<<"output out;"<<endl;
	FO_TEMP1<<"wire [n-2:0] mwire;"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"genvar i;"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"	and and_0 (mwire [0], in [0], in [1]);"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"  generate"<<endl;
	FO_TEMP1<<"    for (i=1; i <= n-2; i=i+1) begin : NAND_N"<<endl;
	FO_TEMP1<<"       and inst (mwire [i], mwire [i-1], in [i+1]);"<<endl;
	FO_TEMP1<<"    end"<<endl;
	FO_TEMP1<<"  endgenerate"<<endl;
	FO_TEMP1<<"	not #(tplh, tphl) inst(out, mwire [n-2]);	"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"endmodule"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
	FO_TEMP1<<"//    Nor: Parametrized on the number of inputs       "<<endl;
	FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"`timescale 1 ns / 1ns"<<endl;
	FO_TEMP1<<" "<<endl;
	FO_TEMP1<<"module nor_n"<<endl;
	FO_TEMP1<<"    #(parameter n = 2, tphl = 1, tplh = 1)"<<endl;
	FO_TEMP1<<"    (out,in);              "<<endl;
	FO_TEMP1<<"input [n-1:0] in;"<<endl;
	FO_TEMP1<<"output out;"<<endl;
	FO_TEMP1<<"wire [n-2:0] mwire;"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"genvar i;"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"	or nor_0 (mwire [0], in [0], in [1]);"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"  generate"<<endl;
	FO_TEMP1<<"    for (i=1; i <= n-2; i=i+1) begin : NOR_N"<<endl;
	FO_TEMP1<<"       or inst (mwire [i], mwire [i-1], in [i+1]);"<<endl;
	FO_TEMP1<<"    end"<<endl;
	FO_TEMP1<<"  endgenerate"<<endl;
	FO_TEMP1<<"	not #(tplh, tphl) inst(out, mwire [n-2]);	"<<endl;
	FO_TEMP1<<"      "<<endl;
	FO_TEMP1<<"endmodule"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
	FO_TEMP1<<"//    Xor: Parametrized on the number of inputs       "<<endl;
	FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"`timescale 1 ns / 1ns"<<endl;
	FO_TEMP1<<" "<<endl;
	FO_TEMP1<<"module xor_n"<<endl;
	FO_TEMP1<<"    #(parameter n = 2, tphl = 1, tplh = 1)"<<endl;
	FO_TEMP1<<"    (out,in);              "<<endl;
	FO_TEMP1<<"input [n-1:0] in;"<<endl;
	FO_TEMP1<<"output out;"<<endl;
	FO_TEMP1<<"wire [n-2:0] mwire;"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"genvar i;"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"	xor xor_0 (mwire [0], in [0], in [1]);"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"  generate"<<endl;
	FO_TEMP1<<"    for (i=1; i <= n-2; i=i+1) begin : XOR_N"<<endl;
	FO_TEMP1<<"       xor inst (mwire [i], mwire [i-1], in [i+1]);"<<endl;
	FO_TEMP1<<"    end"<<endl;
	FO_TEMP1<<"  endgenerate"<<endl;
	FO_TEMP1<<"	bufif1 #(tplh, tphl) inst(out, mwire [n-2], 1'b1);	"<<endl;
	FO_TEMP1<<"   "<<endl;
	FO_TEMP1<<"endmodule"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
	FO_TEMP1<<"//    Xnor: Parametrized on the number of inputs       "<<endl;
	FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"`timescale 1 ns / 1ns"<<endl;
	FO_TEMP1<<" "<<endl;
	FO_TEMP1<<"module xnor_n"<<endl;
	FO_TEMP1<<"    #(parameter n = 2, tphl = 1, tplh = 1)"<<endl;
	FO_TEMP1<<"    (out,in);              "<<endl;
	FO_TEMP1<<"input [n-1:0] in;"<<endl;
	FO_TEMP1<<"output out;"<<endl;
	FO_TEMP1<<"wire [n-2:0] mwire;"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"genvar i;"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"	xor xnor_0 (mwire [0], in [0], in [1]);"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"  generate"<<endl;
	FO_TEMP1<<"    for (i=1; i <= n-2; i=i+1) begin : XNOR_N"<<endl;
	FO_TEMP1<<"       xor inst (mwire [i], mwire [i-1], in [i+1]);"<<endl;
	FO_TEMP1<<"    end"<<endl;
	FO_TEMP1<<"  endgenerate"<<endl;
	FO_TEMP1<<"	not #(tplh, tphl) inst(out, mwire [n-2]);	"<<endl;
	FO_TEMP1<<"    "<<endl;
	FO_TEMP1<<"endmodule"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
	FO_TEMP1<<"//    Fan_Out: Parametrized on the number of inputs       "<<endl;
	FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"`timescale 1 ns / 1ns"<<endl;
	FO_TEMP1<<" "<<endl;
	FO_TEMP1<<"module fanout_n"<<endl;
	FO_TEMP1<<"    #(parameter n = 2,tphl = 3, tplh = 5)"<<endl;
	FO_TEMP1<<"    (in, out);"<<endl;
	FO_TEMP1<<"input in;"<<endl;
	FO_TEMP1<<"output [n-1 :0] out;"<<endl;
	FO_TEMP1<<"genvar i;"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"  generate"<<endl;
	FO_TEMP1<<"    for (i=0; i < n; i=i+1) begin : fanout"<<endl;
	FO_TEMP1<<"       bufif1 #(tplh, tphl) inst (out [i], in, 1'b1);"<<endl;
	FO_TEMP1<<"    end"<<endl;
	FO_TEMP1<<"  endgenerate		"<<endl;
	FO_TEMP1<<"   "<<endl;
	FO_TEMP1<<"endmodule"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
	FO_TEMP1<<"//    Primary Input      "<<endl;
	FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"`timescale 1 ns / 1ns"<<endl;
	FO_TEMP1<<" "<<endl;
	FO_TEMP1<<"module pin #(parameter n = 1)"<<endl;
	FO_TEMP1<<"    (in, out);"<<endl;
	FO_TEMP1<<"input [n-1:0] in;"<<endl;
	FO_TEMP1<<"output [n-1:0] out;"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"genvar i;"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"  generate"<<endl;
	FO_TEMP1<<"    for (i=0; i < n; i=i+1) begin : PI"<<endl;
	FO_TEMP1<<"       bufif1 inst (out [i], in[i], 1'b1);"<<endl;
	FO_TEMP1<<"    end"<<endl;
	FO_TEMP1<<"  endgenerate"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"endmodule"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
	FO_TEMP1<<"//    Primary Output      "<<endl;
	FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"`timescale 1 ns / 1ns"<<endl;
	FO_TEMP1<<" "<<endl;
	FO_TEMP1<<"module pout #(parameter n = 1)"<<endl;
	FO_TEMP1<<"    (in, out);"<<endl;
	FO_TEMP1<<"input [n-1:0] in;"<<endl;
	FO_TEMP1<<"output [n-1:0] out;"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"genvar i;"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"  generate"<<endl;
	FO_TEMP1<<"    for (i=0; i < n; i=i+1) begin : PO"<<endl;
	FO_TEMP1<<"       bufif1 inst (out [i], in[i], 1'b1);"<<endl;
	FO_TEMP1<<"    end"<<endl;
	FO_TEMP1<<"  endgenerate"<<endl;
	FO_TEMP1<<"   "<<endl;
	FO_TEMP1<<"endmodule"<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
	FO_TEMP1<<"//    D Flip Flop "<<endl;
	FO_TEMP1<<"/////////////////////////////////////////////////////////////////////////////////////"<<endl;
	FO_TEMP1<<"module dff "<<endl;
	FO_TEMP1<<"#(parameter tphl = 0, tplh = 0)"<<endl;
	FO_TEMP1<<"(Q, D, C, CLR, PRE, CE, NbarT, Si, global_reset); "<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"input D, C, CLR, PRE, CE, NbarT, Si, global_reset; "<<endl;
	FO_TEMP1<<"output reg Q; "<<endl;
	FO_TEMP1<<"reg val; "<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<""<<endl;
	FO_TEMP1<<"always @(posedge C or posedge PRE or posedge CLR or posedge global_reset) begin"<<endl;
	FO_TEMP1<<"	if(CLR || global_reset)"<<endl;
	FO_TEMP1<<"		val = 1'b0;"<<endl;
	FO_TEMP1<<"	else	if(PRE)"<<endl;
	FO_TEMP1<<"		val = 1'b1;"<<endl;
	FO_TEMP1<<"	else	if(NbarT)"<<endl;
	FO_TEMP1<<"		val = Si; "<<endl;
	FO_TEMP1<<"	else	if(CE)	val = D; "<<endl;
	FO_TEMP1<<"end"<<endl;
	FO_TEMP1<<"   "<<endl;
	FO_TEMP1<<"always@(posedge val) #tplh Q =  val;"<<endl;
	FO_TEMP1<<"always@(negedge val) #tphl Q =  val;    "<<endl;
	FO_TEMP1<<"endmodule"<<endl;

	FO_TEMP1.close();

	return;
}

void netlistGenerator::AddingGlobalReset2(){
	GlobalReset = "global_reset";
	long int counter = 0;

	int replaceFlag = 0;
	int atemp = 0;
	while(!replaceFlag){
		if(!checkingWire(GlobalReset, atemp) && !checkingWire(GlobalReset, EX_Inputs2) && !checkingWire(GlobalReset, EX_Outputs2) && !checkingWire(GlobalReset, Inputs) && !checkingWire(GlobalReset, Outputs)&& !checkingWire(GlobalReset, EX_Inputs_1) && !checkingWire(GlobalReset, EX_Outputs_1)) replaceFlag = 1;
		else{
			GlobalReset.insert(GlobalReset.length(), int2str(counter));
			counter++;
		}
	}

	cout<<endl<<"The global reset is "<<GlobalReset<<".\n";

	serialInwire = "Si";
	NbarT = "NbarT";
	counter = 0;
	replaceFlag = 0;

	while(!replaceFlag){
		if(!checkingWire(serialInwire, atemp) && !checkingWire(serialInwire, EX_Inputs2) && !checkingWire(serialInwire, EX_Outputs2) && !checkingWire(serialInwire, Inputs) && !checkingWire(serialInwire, Outputs)&& !checkingWire(serialInwire, EX_Inputs_1) && !checkingWire(serialInwire, EX_Outputs_1)) replaceFlag = 1;
		else{
			serialInwire.insert(serialInwire.length(), int2str(counter));
			counter++;
		}
	}

	counter = 0;
	replaceFlag = 0;

	while(!replaceFlag){
		if(!checkingWire(NbarT, atemp) && !checkingWire(NbarT, EX_Inputs2) && !checkingWire(NbarT, EX_Outputs2) && !checkingWire(NbarT, Inputs) && !checkingWire(NbarT, Outputs)&& !checkingWire(NbarT, EX_Inputs_1) && !checkingWire(NbarT, EX_Outputs_1)) replaceFlag = 1;
		else{
			NbarT.insert(NbarT.length(), int2str(counter));
			counter++;
		}
	}

	cout<<endl<<"The serial input wire for flip flops is "<<serialInwire<<".\n";
	cout<<endl<<"The NbarT wire is "<<NbarT<<".\n";

	return;
}



GateStruct* netlistGenerator::findingFanout(string str_temp){


	GateStruct* Temp1 = Gatehead;
	int flag = 0;
	while(Temp1){
		node* Temp2 = Temp1->input;
		while(Temp2){
			if(str_temp == Temp2->s && flag == 0){
				if(!(Temp1->input != Temp2 && (Temp1->Type == 4 || Temp1->Type == 5 || Temp1->Type == 6 || Temp1->Type == 7 || Temp1->Type == 8)))
					flag = 1;
			}else{
				if(str_temp == Temp2->s && flag == 1){
					if(!currentPointer) {
						currentPointer = new GateStruct;
						currentPointer->input = NULL;
						currentPointer->next = NULL;
						currentPointer->Output = str_temp;
						currentPointer->Type = 9;
						currentPointer->invert = 0;
						Gatehead = currentPointer;
						currentPointer->INS = "";
						return currentPointer;
					}
					else{
						currentPointer->next = new GateStruct;
						currentPointer->next->input = NULL;
						currentPointer->next->next = NULL;
						currentPointer->next->Output = str_temp;
						currentPointer->next->Type = 9;
						currentPointer->next->invert = 0;
						currentPointer->next->INS = "";
						return currentPointer->next;
					}
				}
			}
			Temp2 = Temp2->next;
		}
		Temp1 = Temp1->next;
	}
	return NULL;
}

void netlistGenerator::outputFanoutHandller(){

	GateStruct* Temp1 = Gatehead;
	int flag = 0;
	while(Temp1){

		if(Temp1->Type == 9 && checkingWire(Temp1->Output, EX_Outputs2)){
			//--Output manager
			string OutputIntermediate = createIntermediatewire(Temp1->Output);
			insertIntermediate(OutputIntermediate, Temp1->Output);
		}

		Temp1 = Temp1->next;
	}
	return;
}





string netlistGenerator::createIntermediatewire(string wire){

	wire.insert(0, "intermediate_");
	long int counter = 0;

	int replaceFlag = 0;
	while(!replaceFlag){
		if(!checkingWire(wire, nodeHead) && !checkingWire(wire, EX_Inputs2) && !checkingWire(wire, EX_Outputs2) && !checkingWire(wire, Inputs) && !checkingWire(wire, Outputs)&& !checkingWire(wire, EX_Inputs_1) && !checkingWire(wire, EX_Outputs_1)) replaceFlag = 1;
		else{
			GlobalReset.insert(wire.length(), int2str(counter));
			counter++;
		}
	}

	cout<<endl<<"An intermediate wire "<<wire<<" generated.\n";


	return wire;
}



void netlistGenerator::insertIntermediate(string des, string source){

	GateStruct* Temp1 = Gatehead;
	while(Temp1){

		if(Temp1->Output == source) Temp1->Output = des;
		node* Temp2 = Temp1->input;
		while(Temp2){
			if(Temp2->s == source) Temp2->s = des;
			Temp2 = Temp2->next;
		}
		Temp1 = Temp1->next;
	}

	node* Temp3 = EX_Inputs_1;
	while(Temp3){
		if(Temp3->s == source) Temp3->s = des;
		Temp3 = Temp3->next;
	}

	Temp3 = EX_Outputs_1;
	while(Temp3){
		if(Temp3->s == source) Temp3->s = des;
		Temp3 = Temp3->next;
	}
	return;

}

int netlistGenerator::checkInOut(){

	node* Temp3 = EX_Inputs_1;
	while(Temp3){
		if(Temp3->s.find("intermediateXXXwire") == 0){
			cout<<"\n\nWe use |intermediateXXXwire| name az an input keyword.";
			cout<<"\nPlease change the name of the top module I/O to an another name.\n";
			cout<<"\nPress any key to terminate the program..."; return -1; 
		}
		Temp3 = Temp3->next;
	}

	Temp3 = EX_Outputs_1;
	while(Temp3){
		if(Temp3->s.find("intermediateXXXwire") == 0){
			cout<<"\n\nWe use |intermediateXXXwire| name az an input keyword.";
			cout<<"\nPlease change the name of the top module I/O to an another name.\n";
			cout<<"\nPress any key to terminate the program..."; return -1; 
		}
		Temp3 = Temp3->next;
	}
	return 0;

}

GateStruct* netlistGenerator::IsSoloNotOutput(string s){

	GateStruct* Temp1 = Gatehead;
	GateStruct* Temp2 = NULL;
	int intTemp = 0;
	while(Temp1){
		node* nodeTemp = Temp1->input;
		while(nodeTemp){
			if(nodeTemp->s == s){
				intTemp++;
				
				if(intTemp>2) return NULL;
			}
			nodeTemp = nodeTemp->next;
		}

		if(Temp1->Output == s && Temp1->Type == 0 && Temp1->invert == 1){ Temp2 = Temp1; intTemp++;}
		Temp1 = Temp1->next;
	}

	if(Temp2 && (intTemp == 2)) return Temp2;
	return NULL;
}

void netlistGenerator::deleteGate(GateStruct* gatePtr){

	GateStruct* Temp1 = Gatehead;
	GateStruct* Temp1_t = Gatehead;
	while(Temp1){

		if(Temp1 == gatePtr){

			if(Temp1 == Gatehead){
				replaceGatewire(Gatehead->input->s, Gatehead->Output);
				Gatehead = Temp1->next;
				delete(Temp1);
				return;
			}else{
				replaceGatewire(Temp1->input->s, Temp1->Output);
				Temp1_t->next = Temp1->next;
				delete(Temp1);
				return;
			}
		}

		if(Temp1 == Gatehead)
			Temp1 = Temp1->next;
		else{
			Temp1 = Temp1->next;
			Temp1_t = Temp1_t->next;
		}
	}

	return;
}
	





void netlistGenerator::optimizingNetlist(){

	cout<<"\n**************************************************\n";
	cout<<"Optimizing generated netlist...\n";

	stack<GateStruct*> ST1;
	stack<GateStruct*> deleteST1;

	GateStruct* Temp1 = Gatehead;
	while(Temp1){
		if(Temp1->Type != 4 && Temp1->Type != 5 && Temp1->Type != 6 && Temp1->Type != 7 && Temp1->Type != 8){

			if((Temp1->Type == 1) && (Temp1->invert == 0)){
				node* nodeTemp = Temp1->input;
				int inputFlag = 0;
				while(nodeTemp){
					if(GateStruct* gateTemp = IsSoloNotOutput(nodeTemp->s)){
						ST1.push(gateTemp);
					}else{
						while(ST1.size()) ST1.pop();
						inputFlag = 1;
						break;
					}
					nodeTemp = nodeTemp->next;
				}

				if(!inputFlag){

					Temp1->Type = 2;
					Temp1->invert = 1;
					while(ST1.size()){
						deleteST1.push(ST1.top());
						ST1.pop();

					}						
				}
			}
			if((Temp1->Type == 2) && (Temp1->invert == 0)){
				node* nodeTemp = Temp1->input;
				int inputFlag = 0;
				while(nodeTemp){
					if(GateStruct* gateTemp = IsSoloNotOutput(nodeTemp->s)){
						ST1.push(gateTemp);
					}else{
						while(ST1.size()) ST1.pop();
						inputFlag = 1;
						break;
					}
					nodeTemp = nodeTemp->next;
				}

				if(!inputFlag){

					Temp1->Type = 1;
					Temp1->invert = 1;
					while(ST1.size()){
						deleteST1.push(ST1.top());
						ST1.pop();

					}						
				}
			}

			if((Temp1->Type == 3) && (Temp1->invert == 0)){
				node* nodeTemp = Temp1->input;
				bool inputFlag = true;
				while(nodeTemp){
					if(GateStruct* gateTemp = IsSoloNotOutput(nodeTemp->s)){
						ST1.push(gateTemp);
						inputFlag = !inputFlag;
						if(inputFlag){
							while(ST1.size()){
								deleteST1.push(ST1.top());
								ST1.pop();
							}
						}
					}
					nodeTemp = nodeTemp->next;
				}

				if(!inputFlag){

					Temp1->Type = 3;
					Temp1->invert = 1;
					while(ST1.size()){
						deleteST1.push(ST1.top());
						ST1.pop();

					}						
				}
			}
		}

			
		Temp1 = Temp1->next;
	}

	while(deleteST1.size()){
		deleteGate(deleteST1.top());
		deleteST1.pop();
	}
	return;
}





