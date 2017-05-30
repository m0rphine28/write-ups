#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include <zlib.h>
#include <inttypes.h>

char * _WIN_HASH = "\x91\x28\x13\x51\x29\xd2\xbe\x65\x28\x09\xf5\xa1\xd3\x37\x21\x1a\xff\xad\x91\xed\x58\x27\x47\x4b\xf9\xbd\x7e\x28\x5e\xce\xf3\x21";

long fsize = 0;

unsigned char _T4_var1 = 0;

unsigned char _T4(unsigned char _STATE[]){
	//loc_4D8
	int i;
	for(i=0; i < 12; i++){
		_T4_var1 += _STATE[i];
		_T4_var1 &= 0xFF;
	}
	return _T4_var1;
}

unsigned char _T5_var1 = 0;

unsigned char _T5(unsigned char _STATE[]){
	//loc_540
	int i;
	for(i=0; i<12; i++){
		_T5_var1 ^= _STATE[i];
		_T5_var1 &= 0xFF;
	}
	return _T5_var1;
}

int _T6_var3 = 0;
short _T6_var2 = 0;

unsigned char _T6(unsigned char _STATE[]){
	unsigned char var1 = 0;
	int var0;
		
	if(_T6_var3 == 0){
		for(var0=0; var0 < 12; var0++){
			_T6_var2 += _STATE[var0];
			_T6_var2 &= 0xFFFF;
		_T6_var3 = 1;
		}
	}
	//loc_5C5
	_T6_var2 = ((((_T6_var2 & 0x8000)) >> 15 ^ ((_T6_var2 & 0x4000) >> 14)) & 0xFFFF) ^ ((_T6_var2 << 1) & 0xFFFF);
	return _T6_var2 & 0xFF;
}


unsigned char _T7(unsigned char _STATE[]){
	unsigned char var1 = 0;
	unsigned char var3 = 0;
	unsigned char var2 = 0;
	int var0;
	for(var0=0; var0 < 6; var0++){
		var1 += _STATE[var0];
		var1 &= 0xFF;
		var2 += _STATE[var0+6];
		var2 &= 0xFF;
	}
	var3 = var1 ^ var2;
	return var3;
}


// init tab
unsigned char _T8_var5[4][12]; // = [[0] * 12, [0] * 12, [0] * 12, [0] * 12]
int _T8_var4 = 0;

unsigned char _T8(unsigned char _STATE[]){
	unsigned char var1 = 0;
	int  i, j;
	for(i=0; i < 12; i++)
		_T8_var5[_T8_var4][i] = _STATE[i];
	_T8_var4 += 1;
	_T8_var4 %= 4;
	unsigned char var3 = 0;
	
	//loc_6FD
	for(i=0; i < 4; i++){
		//loc_6F9
		var1 = 0;
		for(j=0; j < 12; j++){
			var1 += _T8_var5[i][j];
			var1 &= 0xFF;
		}
		var3 ^= var1;
		var3 &= 0xFF;
	}
	return var3;
}


unsigned char _T9(unsigned char _STATE[]){ 
	unsigned char var1 = 0;
	//loc_77C
	int i;
	for(i=0; i < 12; i++){
		var1 ^= _STATE[i] << (i % 8);
		var1 &= 0xFF;
	}
	return var1;
}



// init tab
unsigned char _T10_var4[4][12]; // = [[0] * 12, [0] * 12, [0] * 12, [0] * 12]
int _T10_var2 = 0;

unsigned char _T10(unsigned char _STATE[]){
	unsigned char var3 = 0;
	unsigned char tmp = 0;
	//for i in xrange(len(_STATE)):
	int i;
	for(i=0; i < 12; i++)	
		_T10_var4[_T10_var2][i] = _STATE[i];
	_T10_var2 += 1;
	_T10_var2 %= 4;

	unsigned char var1 = 0;

	//loc_8F1
	// prend le premier byte de chaque tableau?
	for(i=0; i < 4; i++){
		var1 += _T10_var4[i][0];
		var1 &= 0xFF;
	}
	tmp = (var1 >> 4) % 12;
	return _T10_var4[var1 % 4][tmp];
}


// init tab
//_T12_PREVIOUS_STATE = [0] *12
unsigned char _T12_PREVIOUS_STATE[12];
unsigned char _T11(unsigned char _STATE[]){
	// from T12
	unsigned char _INDEX = _T12_PREVIOUS_STATE[1] ^ _T12_PREVIOUS_STATE[5] ^ _T12_PREVIOUS_STATE[9];
	//print _STATE[_INDEX % 12]
	return _STATE[_INDEX % 12];
}

unsigned char _T12(unsigned char _STATE[]){
	int i=0;
	for(i=0; i < 12; i++)
		_T12_PREVIOUS_STATE[i] = _STATE[i];
	// from T11
	unsigned char _INDEX = (_STATE[0] ^ _STATE[3] ^ _STATE[7]) & 0xFF;
	return _STATE[_INDEX % 12];
}

unsigned char _T1(unsigned char _STATE[]){
	unsigned char _B_FROM_T4 = _T4(_STATE);
	unsigned char _B_FROM_T5 = _T5(_STATE);
	unsigned char _B_FROM_T6 = _T6(_STATE);
	//printf("%d\n", _B_FROM_T6);
	return _B_FROM_T4 ^ _B_FROM_T5 ^ _B_FROM_T6;
}

unsigned char _T2(unsigned char _STATE[]){
	unsigned char _B_FROM_T7 = _T7(_STATE);
	unsigned char _B_FROM_T8 = _T8(_STATE);
	unsigned char _B_FROM_T9 = _T9(_STATE);
	//printf("%d\n", _B_FROM_T8);
	return _B_FROM_T7 ^ _B_FROM_T8 ^ _B_FROM_T9;
}

unsigned char _T3(unsigned char _STATE[]){
	unsigned char _B_FROM_T10 = _T10(_STATE);
	unsigned char _B_FROM_T11 = _T11(_STATE);
	unsigned char _B_FROM_T12 = _T12(_STATE);
	//printf("%d\n", _B_FROM_T12);
	return _B_FROM_T10 ^ _B_FROM_T11 ^ _B_FROM_T12;
}

unsigned char _T0(unsigned char _STATE[]){
	unsigned char _B_FROM_T1 = _T1(_STATE);
	unsigned char _B_FROM_T2 = _T2(_STATE);
	unsigned char _B_FROM_T3 = _T3(_STATE);
	return _B_FROM_T1 ^ _B_FROM_T2 ^ _B_FROM_T3;
}

void _INIT_TABS(){
	int tmp1, tmp2;

	for(tmp1=0; tmp1 < 4; tmp1++){
		for(tmp2=0; tmp2 < 12; tmp2++)
			_T8_var5[tmp1][tmp2] = 0;
	}

	for(tmp1=0; tmp1 < 4; tmp1++){
		for(tmp2=0; tmp2 < 12; tmp2++)
			_T10_var4[tmp1][tmp2] = 0;
	}

	for(tmp1=0; tmp1 < 12; tmp1++)
		_T12_PREVIOUS_STATE[tmp1] = 0;

}


void _RESET_VARS(){
	_T4_var1 = 0;
	_T5_var1 = 0;
	_T6_var3 = 0;
	_T6_var2 = 0;
	_T8_var4 = 0;
	_T10_var2 = 0;
	_INIT_TABS();
}

int _PROGRESS = 0;


int _DECRYPT(unsigned char * _KEY, unsigned char * _CIPHER, unsigned char * _PLAIN){
	unsigned char _STATE[12];
	int i,c;

	for(i=0; i < 12; i++)
		_STATE[i] = _KEY[i];
	
	i = 0;
	unsigned char _PRNG;

	/* 
	.magic:16                       = 'BZ' signature/magic number
	.version:8                      = 'h' for Bzip2 ('H'uffman coding), '0' for Bzip1 (deprecated)
	.hundred_k_blocksize:8          = '1'..'9' block-size 100 kB-900 kB (uncompressed)

	.compressed_magic:48            = 0x314159265359 (BCD (pi))
	.crc:32                         = checksum for this block
	.randomised:1                   = 0=>normal, 1=>randomised (deprecated)
	*/

	for(c=0; c < fsize; c++){
		_PLAIN[c] = _CIPHER[c] ^ ((_STATE[i] * 2 + i) & 0xFF);
		if(c == 14 && _PLAIN[c] != '\x00')
			return 1;
		_PRNG = _T0(_STATE);
		_STATE[i] = _PRNG;
		i += 1;
		i %= 12;
	}
	return 0;
}

int _BF(unsigned char * _CIPHER){ 
	
	unsigned char * _DECRYPTED = malloc(fsize);
	
	
	unsigned char _KEY_BUFFER[12];
	unsigned char * _KEY;
	
	//int _CINQPOURCENT = 9*256*256*0.05;
	//int _PROGRESS = 0;
	int _PALLIER = 1;	
	//int j,k
	int l,_NB_TROISIEME_B;
	int debug = 1;
	FILE * f;
	unsigned char w = 0;
	unsigned char _PLAIN_CONNU[10];
	unsigned char _TROISIEME_B[9];
	
	_PLAIN_CONNU[0] = '\x42';
	_PLAIN_CONNU[1] = '\x5a';
	_PLAIN_CONNU[2] = '\x68';
	_PLAIN_CONNU[3] = 0;
	_PLAIN_CONNU[4] = '\x31';
	_PLAIN_CONNU[5] = '\x41';
	_PLAIN_CONNU[6] = '\x59';
	_PLAIN_CONNU[7] = '\x26';
	_PLAIN_CONNU[8] = '\x53';
	_PLAIN_CONNU[9] = '\x59';
	
	_TROISIEME_B[0] = '\x31';	
	_TROISIEME_B[1] = '\x32';	
	_TROISIEME_B[2] = '\x33';	
	_TROISIEME_B[3] = '\x34';	
	_TROISIEME_B[4] = '\x35';	
	_TROISIEME_B[5] = '\x36';	
	_TROISIEME_B[6] = '\x37';	
	_TROISIEME_B[7] = '\x38';	
	_TROISIEME_B[8] = '\x39';	

	unsigned char _KEY_I[12][20];

	int j,k;
	printf("[+] Génération des différents bytes à tester...\n");
	l = 0; // nombre de valeurs possibles pour chaque key[i]
	_NB_TROISIEME_B = 0; // pour affichage des pourcentages
	//for(j=0; j < 10; j++){
	for(j=0; j < 10; j++){
		//printf("J = %di\n", j);
		if( j!= 3 ){
			for(k=0; k < 256; k++){
				w = _CIPHER[j] ^ ((k*2 + j) & 0xFF);
				//printf("%d\n", w);
				if( w == _PLAIN_CONNU[j] ){ 
					_KEY_I[j][l] = k;
					l++;
				}
			}
			/*if(debug == 1)
				printf(" |- %d valeurs possibles pour key[%d]\n", l, j);*/
			l=0;
		}
	}

	if(debug == 1){
		for(j=0; j < 10; j++){
			if( j!= 3){
				printf(" |- valeurs possibles pour key[%d] : %d, %d\n", j, _KEY_I[j][0], _KEY_I[j][1]);
			}
		}
	}


	// traitement du cas particulier du 3eme byte
	for(j=9; j > 0 ; j--){
		for(k=0; k < 256; k++){
			w = _CIPHER[3] ^ ((k*2 + 3) & 0xFF);
			if( w == _TROISIEME_B[j] ) {
				_KEY_I[3][l] = k;
				l++;
			}
		}
		/*if(debug == 1)
			printf(" |- %d valeurs possibles pour key[3][%d]\n", l, j);*/
	}

	if(debug == 1)
		printf(" |- valeurs possibles pour key[3] : ", _NB_TROISIEME_B);
	for(j=0; j < l; j++)
		printf("%d ", _KEY_I[3][j]);
	printf("\n");
	//int _CINQPOURCENT = _NB_TROISIEME_B*256*256*0.05;
	int _TOTAL_A_BF = 2*2*2*l*2*2*2*2*2*2*256*256;
	int _CINQPOURCENT = _TOTAL_A_BF*0.05;
	
	printf("%d", _TOTAL_A_BF);
	//printf("\n%d", _NB_TROISIEME_B);
/*
	_KEY = _KEY_TMP;
	_KEY[0] = ('\x42' ^ _CIPHER[0]) / 2;
	_KEY[1] = (('\x5a' ^ _CIPHER[1]) - 1) / 2;
	_KEY[2] = (('\x68' ^ _CIPHER[2]) - 2) / 2;
	_KEY[4] = (('\x31' ^ _CIPHER[4]) - 4) / 2;
	_KEY[5] = (('\x41' ^ _CIPHER[5]) - 5) / 2;
	_KEY[6] = (('\x59' ^ _CIPHER[6]) - 6) / 2;
	_KEY[7] = (('\x26' ^ _CIPHER[7]) - 7) / 2;
	_KEY[8] = (('\x53' ^ _CIPHER[8]) - 8) / 2;
	_KEY[9] = (('\x59' ^ _CIPHER[9]) - 9) / 2;
	unsigned char i;i*/
	unsigned char _HASH[32];
	
	_KEY = _KEY_BUFFER;
	//int test = 0;
	int _FAIL = 0;	

	int _B0, _B1, _B2, _B3, _B4, _B5, _B6, _B7, _B8, _B9, _B10, _B11;
	for(_B0=0; _B0 < 2; _B0++){
		_KEY[0] = _KEY_I[0][_B0];
		for(_B1=0; _B1 < 2; _B1++){
			_KEY[1] = _KEY_I[1][_B1];
			for(_B2=0; _B2 < 2; _B2++){
				_KEY[2] = _KEY_I[2][_B2];
				for(_B3=0; _B3 < 10; _B3++){
					_KEY[3] = _KEY_I[3][_B3];
					for(_B4=0; _B4 < 2; _B4++){
						_KEY[4] = _KEY_I[4][_B4];
						for(_B5=0; _B5 < 2; _B5++){
							_KEY[5] = _KEY_I[5][_B5];
							for(_B6=0; _B6 < 2; _B6++){
								_KEY[6] = _KEY_I[6][_B6];
								for(_B7=0; _B7 < 2; _B7++){
									_KEY[7] = _KEY_I[7][_B7];
									for(_B8=0; _B8 < 2; _B8++){
										_KEY[8] = _KEY_I[8][_B8];
										for(_B9=0; _B9 < 2; _B9++){
											_KEY[9] = _KEY_I[9][_B9];
											for(_B10=0; _B10 < 256; _B10++){
												_KEY[10] = _B10;
												for(_B11=0; _B11 < 256; _B11++){
													_KEY[11] = _B11;
													_PROGRESS++;
													if(_PROGRESS == _PALLIER*_CINQPOURCENT){
														printf("[+] %d%\n", _PALLIER*5);
														_PALLIER += 1;
													}
													_FAIL = _DECRYPT(_KEY, _CIPHER, _DECRYPTED);    
													if(_FAIL == 0){
														SHA256(_DECRYPTED, fsize, _HASH);
														if(memcmp(_HASH, _WIN_HASH, 32) == 0){
															printf("[+] GG\n");
															f = fopen("stage6", "w");
															fwrite(_DECRYPTED, fsize, 1, f);
															fclose(f);
															return 0;
														}
													}
													_RESET_VARS();
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	return 1;
}


int main(int argc, char** argv){
	_INIT_TABS();
	/*
	unsigned char key[12] = "*SSTIC-2015*";
	unsigned char* data = "\x1d\x87\xc4\xc4\xe0\xee\x40\x38\x3c\x59\x44\x7f\x23\x79\x8d\x9f\xef\xe7\x4f\xb8\x24\x80\x76\x6e";
	unsigned char * decrypted = malloc(strlen(data)+1);
	_DECRYPT(key,data,decrypted);
	printf("decrypted : %s\n", decrypted);
	*/
	FILE *f = fopen("cipher", "r");
	fseek(f, 0, SEEK_END);
	fsize = ftell(f);
	fseek(f, 0, SEEK_SET);

	unsigned char * _CIPHER = malloc(fsize + 1);
	fread(_CIPHER, fsize, 1, f);
	fclose(f);

	_BF(_CIPHER);

	return 0;
}
