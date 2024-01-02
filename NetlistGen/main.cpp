
#include "NetlistGen.h"







int main(){

	netlistGenerator net1;
	int continu = 1;
	while(continu){
		int k = net1.readFile();
		switch(k){
			case 0:{
				if(net1.readInOut() == -1) {
					cout<<"\nThere is an error while reading your Verilog file.\n";
					cout<<"Please check the ERROR.txt file for more information and be sure that your Verilog file is compilable.\n";
					cout<<"Press any key to end...\n";
					getch();
					return 1;
				}
				if(net1.checkInOut() == -1) { getch(); return 1;}
				string chtemp1;
				cout<<"\n**************************************************\nYou can synthsize module "<<net1.getModuleName()<< " by ISE if the EDIF file is not ready. \nDo you want to use ISE?(Y/N)";	
				while(chtemp1 != "y" && chtemp1 != "Y" && chtemp1 != "n" && chtemp1 != "N"){
					cin>>chtemp1;
					if(chtemp1 == "Y" || chtemp1 == "y"){
						net1.PrintTCLFile();
						net1.runISE();
						if(net1.RunBatchFile() == -1) return -1;
					}else if(chtemp1 != "N" && chtemp1 != "n")
						cout<<"\nYou can synthsize module "<<net1.getModuleName()<< " by ISE if the EDIF file is not ready. \nDo you want to use ISE?(Y/N)";
				}
				net1.readNDFfile();
				net1.designHn();
				//net1.clkEnableHn();
				net1.bufferRmv();
				net1.optimizingNetlist();
				net1.wireAnalyzer_Gate2();
				net1.AddingGlobalReset2();
				//net1.changingInOut();
				net1.benchGeneratorV2();
				net1.FanoutGenerator();
				net1.outputFanoutHandller();
				net1.verilogGeneratorV2();
				continu = 0;
				break;
			}	
			case 1: {
				break;
			}
			case -1:{
				continu = 0;
				break;
			}
			default : return 1;
		}
	}
	return 1;

}
