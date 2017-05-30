#include <stdio.h>
#include <stdlib.h>
#define BLOCKSIZE 100000000 

long long total = 740457671;
//long long BLOCKSIZE = total*0.1;
int i = 0;
char io0[BLOCKSIZE];
char io1[BLOCKSIZE];
char io2[BLOCKSIZE];
char io3[BLOCKSIZE];
char io4[BLOCKSIZE];
char io5[BLOCKSIZE];
char io6[BLOCKSIZE];
char io7[BLOCKSIZE];
char re[BLOCKSIZE];
char we[BLOCKSIZE];
char ale[BLOCKSIZE];
char cle[BLOCKSIZE];
char * CURRENT_COMMAND = NULL;
FILE * logFile = NULL;
FILE * firmware = NULL;
FILE* f_io0 = NULL;
FILE* f_io1 = NULL;
FILE* f_io2 = NULL;
FILE* f_io3 = NULL;
FILE* f_io4 = NULL;
FILE* f_io5 = NULL;
FILE* f_io6 = NULL;
FILE* f_io7 = NULL;
FILE* f_re = NULL;
FILE* f_we = NULL;
FILE* f_ale = NULL;
FILE* f_cle = NULL;
int blockID = 0;

void readBlock(){
	/*
		/!\ fgets appends null byte 
		=> BLOCKSIZE-1 = 0x0
	*/
	fgets(io0, BLOCKSIZE, f_io0);
	fgets(io1, BLOCKSIZE, f_io1);
	fgets(io2, BLOCKSIZE, f_io2);
	fgets(io3, BLOCKSIZE, f_io3);
	fgets(io4, BLOCKSIZE, f_io4);
	fgets(io5, BLOCKSIZE, f_io5);
	fgets(io6, BLOCKSIZE, f_io6);
	fgets(io7, BLOCKSIZE, f_io7);
	fgets(re, BLOCKSIZE, f_re);
	fgets(we, BLOCKSIZE, f_we);
	fgets(ale, BLOCKSIZE, f_ale);
	fgets(cle, BLOCKSIZE, f_cle);
	i = 0;
}

int readIOs(){
	return (io0[i]-0x30) + (io1[i]-0x30<<1) + (io2[i]-0x30 << 2) + (io3[i]-0x30 << 3) + (io4[i]-0x30 << 4) + (io5[i]-0x30 << 5) + (io6[i]-0x30 << 6) + (io7[i]-0x30 << 7);
}

char * commands[4] = {"RESET", "READ ID", "ERASE BLOCK", "PROGRAM PAGE"};


void printStatus(){
	printf("[+] i = %i\n", i + BLOCKSIZE*blockID);
	printf("[+] io = %x\n", readIOs());
	printf("[+] ale = %x\n", ale[i]);
	printf("[+] re = %x\n", re[i]);
	printf("[+] we = %x\n", we[i]);
	printf("[+] cle = %x\n", cle[i]);
}

void closeFiles(){
	fclose(f_io0);
	fclose(f_io1);
	fclose(f_io2);
	fclose(f_io3);
	fclose(f_io4);
	fclose(f_io5);
	fclose(f_io6);
	fclose(f_io7);
	fclose(f_re);
	fclose(f_we);
	fclose(f_ale);
	fclose(f_cle);
	fclose(logFile);	
}

int newCommand(){
	int commandID = readIOs();
	switch(readIOs()){
		case 0xff : 
			CURRENT_COMMAND = commands[0];
			fprintf(logFile, "RESET at %i\n", i + BLOCKSIZE*blockID);
			break;
		case 0x90 : // READ ID
			CURRENT_COMMAND = commands[1];
			fprintf(logFile, "READ ID at %i\n", i + BLOCKSIZE*blockID);
			break;		 
		case 0x60 : // ERASE BLOCK
			CURRENT_COMMAND = commands[2];
			fprintf(logFile, "ERASE BLOCK at %i\n", i + BLOCKSIZE*blockID);
			break;
		case 0x80 : // PROGRAM PAGE
			CURRENT_COMMAND = commands[3];
			fprintf(logFile, "PROGRAM PAGE at %i\n", i + BLOCKSIZE*blockID);
			break;
		default: 
			printf("unknown command = %x\n", commandID);
			CURRENT_COMMAND = 0;
			char pause = getchar();
	}
	return commandID;
}

int indexNextCycle(){
	//printf("Cycle : %i\n", (i+blockID*BLOCKSIZE));
	if(i == BLOCKSIZE -1){
		printf("test\n");
		readBlock();
		blockID++;
	}
	int tmp = i;
	if(we[i] == 0x30){
		while(we[tmp] == 0x30){
			tmp++;
			if(tmp == BLOCKSIZE-1){
				readBlock();
				blockID++;
				tmp = 0;
			}
		}
	}
	else if(we[i] == 0x31){
		while(we[tmp] == 0x31){
			tmp++;
			if(tmp == BLOCKSIZE-1){
				readBlock();
				blockID++;
				tmp = 0;
			}
		}	
		while(we[tmp] == 0x30){
			tmp++;
			if(tmp == BLOCKSIZE-1){
				readBlock();
				blockID++;
				tmp = 0;
			}
		}
	}
	else{
		printf("ERROR WE clock at %i, we[i]=%i\n",(i+blockID*BLOCKSIZE),we[i]);
		exit(-1);
	}
	if(tmp + BLOCKSIZE*blockID == total){
		printf("[+] Done\n");
		closeFiles();
		exit(0);
	}
	return tmp;
}

int indexNextRECycle(){
	int tmp = i;
	if(re[i] == 0x30){
		while(re[tmp] == 0x30) tmp++;
	}
	else if(re[i] == 0x31){
		while(re[tmp] == 0x31) tmp++;
		while(re[tmp] == 0x30) tmp++;
	}
	else{
		printf("ERROR RE clock\n");
		exit(-1);
	}
	return tmp;
}


unsigned int getAddress(int mode, int address[]){
	unsigned int _ADDRESS = 0;
	unsigned int columnAddress = 0;
	unsigned int pageAddress = 0;
	unsigned int blockAddress = 0;
	if(mode == 1){ 	// PROGRAM DATA 
		columnAddress = ((address[1] & 0b1111 ) << 8 ) +   address[0];
		//printf("Column address : %x\n", columnAddress);
	}
	pageAddress = (address[2] & 0b111111);
	//printf("address[2] = %i => Page address : %x\n", address[2], pageAddress);
 	blockAddress = (address[3] + ((address[4] & 0b1) << 8));
	//printf("Block address : %x\n", blockAddress);
	//char pause = getchar();
	//exit(0);
	// The 2Gb NAND Flash device is organized as 2048 blocks, with 64 pages per block (see
	// Figure 3). Each page is 2112 bytes, consisting of a 2048-byte data area and a 64-byte
	// spare area.
	return (blockAddress*64*0x2112) + (pageAddress*0x2112) + columnAddress;  
}  


int main(){
	
	f_io0 = fopen("bits/io0_final", "r");
	f_io1 = fopen("bits/io1_final", "r");
	f_io2 = fopen("bits/io2_final", "r");
	f_io3 = fopen("bits/io3_final", "r");
	f_io4 = fopen("bits/io4_final", "r");
	f_io5 = fopen("bits/io5_final", "r");
	f_io6 = fopen("bits/io6_final", "r");
	f_io7 = fopen("bits/io7_final", "r");
	f_re = fopen("bits/re_final", "r");
	f_we = fopen("bits/we_final", "r");
	f_ale = fopen("bits/ale_final", "r");
	f_cle = fopen("bits/cle_final", "r");
	
	readBlock();	

	logFile = fopen("log","w");
	firmware = fopen("firmware2.bin", "w");

	i = indexNextCycle(); 
	int IOValue = 0;
	int tmp = 0;
	char pause;
	int commandOK = 1;
	int cpt=0;
	int address[5];
	int _ADDRESS = 0;

	for(i; i<BLOCKSIZE; i=indexNextCycle()){
		CURRENT_COMMAND = "";
		if(cle[i] == 0x31){
			//printf("New command at index : %i\n", i);
			int c = newCommand();
			//pause = getchar();
		}
		if(CURRENT_COMMAND == "RESET"){
			// Nothing to do
		}	
		else if(CURRENT_COMMAND == "READ ID"){
			i = indexNextCycle();
			// READ ID = DUMMY ADDRESS then READ
			IOValue = readIOs();
			if(IOValue == 0 && ale[i] == 0x31){
				fprintf(logFile, "\tDUMMY ADDRESS 0\n");
				tmp = i;
				while(we[tmp] == 0x31) tmp++;		
				fprintf(logFile, "\tID : ");
				i = indexNextRECycle();
				while(i < tmp){
					char ID = readIOs();
					fprintf(logFile, "%c", ID);
					i = indexNextRECycle();
				}
				fprintf(logFile, "\n");
			}	
			else commandOK = 0;
		}
		else if(CURRENT_COMMAND == "ERASE BLOCK"){
			// ERASE BLOCK = 3 ADDRESS CYCLES + CMD(0xd0)
			cpt=0;
			address[0] = 0;
			address[1] = 0;
			while(cpt<3){
				i = indexNextCycle();
				if(ale[i] == 0x31){
					address[cpt+2] = readIOs();
					cpt++;
				}
				else commandOK = 0;
			}
			_ADDRESS = getAddress(0, address);
			fprintf(logFile, "\tAddressEB %x\n", _ADDRESS);	
			//fseek(firmware, _ADDRESS, SEEK_SET);
			i = indexNextCycle();
			//printf("cle[i]= %i, ios=%x\n", cle[i], readIOs()); 
			if(cle[i] != 0x31 || readIOs() != 0xd0) commandOK = 0;
			i = indexNextCycle();
		}
		else if(CURRENT_COMMAND == "PROGRAM PAGE"){
			// PROGRAM PAGE = 5 ADDRESS CYCLES + DATA + CMD(0x10)
			cpt=0;
			while(cpt<5){
				i = indexNextCycle();
				if(ale[i] == 0x31){
					address[cpt] = readIOs(); 
					cpt++;
				}
				else commandOK = 0;
			}
			_ADDRESS = getAddress(1,address);
			fprintf(logFile, "\tAddress %x\n", _ADDRESS);
			//fprintf(logFile, "\tAddress %i%i%i%i%i\n", address[0], address[1], address[2], address[3], address[4]);	
			// read data until next CLE = 1
			i = indexNextCycle(); // NEXT CYCLE = DATA
			cpt = 0;
			//fseek(firmware, , SEEK_SET)
			while( cle[i] != 0x31 ){ 
				fprintf(firmware, "%c", readIOs());
				i = indexNextCycle();
				cpt++;
			}
			if( readIOs() != 0x10 ) commandOK = 0;
			fprintf(logFile, "\t%i bytes of data\n", cpt);
			i = indexNextCycle();
			//printStatus();
			//fprintf(logFile, "\n");
			//i = indexNextCycle();
		}
		else{
		}
		if(!commandOK){
			printStatus();
			closeFiles();
			printf("Error : %s at %i", CURRENT_COMMAND, i + BLOCKSIZE*blockID);
			exit(-1);	
		}
	}

	closeFiles();
}
