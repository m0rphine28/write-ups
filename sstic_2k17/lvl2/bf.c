#include <stdio.h>
#include <stdlib.h>

char charset[16] = {'\x00','\x01','\x02','\x03','\x04','\x05','\x06','\x07','\x08','\x09','\x0a','\x0b','\x0c','\x0d','\x0e','\x0f'};


unsigned long long mul64(unsigned long long x, unsigned long long y){
	return ((x*y) & 0xffffffffffffffff);
}


unsigned long long div64(unsigned long long x, unsigned long long y){
	return ((x/y) & 0xffffffffffffffff);
}


unsigned long long add64(unsigned long long x, unsigned long long y){
	return ((x+y) & 0xffffffffffffffff);
}


unsigned long long sub64(unsigned long long x, unsigned long long y){
	return ((x-y) & 0xffffffffffffffff);
}


unsigned long long mov64(unsigned long long x, unsigned long long y){
	return y;
}


unsigned long long xor64(unsigned long long x, unsigned long long y){
	return ((x^y) & 0xffffffffffffffff);
}


unsigned int solve_block3(unsigned int x){
	unsigned long long r1 = 0xbd89a8ae;
	unsigned long long r8 = x;
	unsigned long long r2 = 0;
	unsigned long long r3 = 0;
	r8 = xor64(r8, r1); 
	r8 = r8 & 0xffffffff;
	r1 = r8;
	r1 = mul64(r1, r1);
	r2 = 0x8a46a52d;
	r3 = r1;
	r3 = div64(r3, r2);
	r3 = mul64(r3, r2);
	r1 = sub64(r1, r3);
	r1 = mul64(r1, r1);
	r3 = mov64(r3, r1);
	r3 = div64(r3, r2);
	r3 = mul64(r3, r2);
	r1 = sub64(r1, r3);
	r3 = r8; 		//TODO
	r3 = div64(r3, r2);
	r3 = mul64(r3, r2);
	r8 = sub64(r8, r3);
	r1 = mul64(r1, r1);
	r3 = mov64(r3, r1);
	r3 = div64(r3, r2);
	r3 = mul64(r3, r2);
	r1 = sub64(r1, r3);
	r1 = mul64(r1, r1);
	r3 = mov64(r3, r1);
	r3 = div64(r3, r2);
	r3 = mul64(r3, r2);
	r1 = sub64(r1, r3);
	r1 = mul64(r1, r1);
	r3 = mov64(r3, r1);
	r3 = div64(r3, r2);
	r3 = mul64(r3, r2);
	r1 = sub64(r1, r3);
	r1 = mul64(r1, r1);
	r3 = mov64(r3, r1);
	r3 = div64(r3, r2);
	r3 = mul64(r3, r2);
	r1 = sub64(r1, r3);
	r1 = mul64(r1, r1);
	r3 = mov64(r3, r1);
	r3 = div64(r3, r2);
	r3 = mul64(r3, r2);
	r1 = sub64(r1, r3);
	r1 = mul64(r1, r1);
	r3 = mov64(r3, r1);
	r3 = div64(r3, r2);
	r3 = mul64(r3, r2);
	r1 = sub64(r1, r3);
	r1 = mul64(r1, r8);
	r3 = mov64(r3, r1);
	r3 = div64(r3, r2);
	r3 = mul64(r3, r2);
	r1 = sub64(r1, r3);
	return r1;
} 	


unsigned int solve_block4(unsigned int x){
	unsigned long long r1 = 0xba9bbc8d;
	unsigned long long r2 = 0;
	unsigned long long r3 = 0;
	unsigned long long r4 = x;
	r4 = xor64(r4, r1); 
	r4 = r4 & 0xffffffff;
	r1 = r4;
	r1 = mul64(r1, r1);
	r2 = 0xdc6c88e1;
	r3 = mov64(r3, r1);
	r3 = div64(r3, r2);
	r3 = mul64(r3, r2);
	r1 = sub64(r1, r3);
	r1 = mul64(r1, r1);
	r3 = mov64(r3, r1);
	r3 = div64(r3, r2);
	r3 = mul64(r3, r2);
	r1 = sub64(r1, r3);
	r3 = mov64(r3, r4);
	r3 = div64(r3, r2);
	r3 = mul64(r3, r2);
	r4 = sub64(r4, r3);
	r1 = mul64(r1, r1);
	r3 = mov64(r3, r1);
	r3 = div64(r3, r2);
	r3 = mul64(r3, r2);
	r1 = sub64(r1, r3);
	r1 = mul64(r1, r1);
	r3 = mov64(r3, r1);
	r3 = div64(r3, r2);
	r3 = mul64(r3, r2);
	r1 = sub64(r1, r3);
	r1 = mul64(r1, r1);
	r3 = mov64(r3, r1);
	r3 = div64(r3, r2);
	r3 = mul64(r3, r2);
	r1 = sub64(r1, r3);
	r1 = mul64(r1, r1);
	r3 = mov64(r3, r1);
	r3 = div64(r3, r2);
	r3 = mul64(r3, r2);
	r1 = sub64(r1, r3);
	r1 = mul64(r1, r1);
	r3 = mov64(r3, r1);
	r3 = div64(r3, r2);
	r3 = mul64(r3, r2);
	r1 = sub64(r1, r3);
	r1 = mul64(r1, r1);
	r3 = mov64(r3, r1);
	r3 = div64(r3, r2);
	r3 = mul64(r3, r2);
	r1 = sub64(r1, r3);
	r1 = mul64(r1, r4);
	r3 = mov64(r3, r1);
	r3 = div64(r3, r2);
	r3 = mul64(r3, r2);
	r1 = sub64(r1, r3);
	return r1;
} 	




int main(){
	int i=0;
	int j=0;
	int k=0;
	int l=0;
	int m=0;
	int n=0;
	int o=0;
	int p=0;
	unsigned long long x = 0;
	unsigned int res = 0;
	for(i=0; i<16; i++){
		for(j=0; j<16; j++){
			for(k=0; k<16; k++){
				for(l=0; l<16; l++){
					for(m=0; m<16; m++){
						for(n=0; n<16; n++){
							for(o=0; o<16; o++){
								for(p=0; p<16; p++){
									x = (charset[i] << 0x1c) + (charset[j] << 0x18) + (charset[k] << 0x14) + (charset[l] << 0x10) + (charset[m] << 0xc) + (charset[n] << 0x8) + (charset[o] << 0x4) + charset[p];
									res = solve_block3(x);
									//res = solve_block4(x);
									if(res == 0x204c4c41) {
									//if(res == 0x37313032) {
										printf("BLOCK 4 FOUND : %x\n", x);
										//exit(0);
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
