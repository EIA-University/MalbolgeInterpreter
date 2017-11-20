/*Este programa funciona analogo a Interprete.c , pero con la diferencia que te muestra el paso a paso en cada instruccion.
Fue creado con el objetivo de ayudar a comprender y explicar codigo en Malbolge*/

#include <stdio.h>
#include <math.h>

const char* translation = "5z]&gqtyfr$(we4{WP)H-Zn,[%\\3dL+Q;>U!pJS72FhOA1CB6v^=I_0/8|jsb9m<.TVac`uY*MK'X~xDl}REokN:#?G\"i@";

unsigned int crazy(unsigned int a, unsigned int d){
	unsigned int crz[] = {1,0,0,1,0,2,2,2,1};
	int position = 0;
	unsigned int output = 0;
	while (position < 10){
		unsigned int i = a%3;
		unsigned int j = d%3;
		unsigned int out = crz[i+3*j];
		unsigned int multiple = 1;
		int k;
		for (k=0;k<position;k++)
			multiple *= 3;
		output += multiple*out;
		a /= 3;
		d /= 3;
		position++;
	}
	return output;
}

unsigned int rotate_r(unsigned int d){
	unsigned int carry = d%3;
	d /= 3;
	d += 19683 * carry;
	return d;
}


void escribir(unsigned instruction, unsigned flag, int i, int a, int c, int d, int memory){
	if(flag){
		printf("------------------------------------------------------------ \n");
		printf("| Numero | Instruccion |   a   |   c   |   d   | memory[d] | \n");
		printf("------------------------------------------------------------ \n");
	}else{
		int spc,j;
		//calcular espacios i
		spc = (int)(log10(i))+1;
		char ii[8-spc];
		ii[7-spc]='\0';
		for(j=0; j<(7-spc);j++){
			ii[j]=(char)32;
		}
		//
		//calcular espacios a
		if(a==0) {
			spc = 1;
		}else {
			spc = (int) (log10(a)) + 1;
		}
		char aa[7-spc];
		aa[6-spc]='\0';
		for(j=0; j<(6-spc);j++){
			aa[j]=(char)32;
		}
		//
		//calcular espacios c
		if(c==0) {
			spc = 1;
		}else{
			spc = (int)(log10(c))+1;
		}
		char cc[7-spc];
		cc[6-spc]='\0';
		for(j=0; j<(6-spc);j++){
			cc[j]=(char)32;
		}
		//
		//calcular espacios d
		if(d==0){
			spc=1;
		}else{
			spc = (int)(log10(d))+1;
		}
		char dd[7-spc];
		dd[6-spc]='\0';
		for(j=0; j<(6-spc);j++){
			dd[j]=(char)32;
		}
		//
		//calcular espacios memory
		spc = (int)(log10(memory))+1;
		char mm[11-spc];
		mm[10-spc]='\0';
		for(j=0; j<(10-spc);j++){
			mm[j]=(char)32;
		}
		//
		switch (instruction){
			case 4:
				printf("| %i%s| JMP[d]      | %i%s| %i%s| %i%s| %i%s| \n", i, ii, a, aa, c, cc, d, dd, memory, mm);
				break;
			case 5:
				printf("| %i%s| OUT A       | %i%s| %i%s| %i%s| %i%s| \n", i, ii, a, aa, c, cc, d, dd, memory, mm);
				break;
			case 23:
				printf("| %i%s| IN A        | %i%s| %i%s| %i%s| %i%s| \n", i, ii, a, aa, c, cc, d, dd, memory, mm);
				break;
			case 39:
				printf("| %i%s| ROT([d])    | %i%s| %i%s| %i%s| %i%s| \n", i, ii, a, aa, c, cc, d, dd, memory, mm);
				break;
			case 40:
				printf("| %i%s| d=[d]       | %i%s| %i%s| %i%s| %i%s| \n", i, ii, a, aa, c, cc, d, dd, memory, mm);
				break;
			case 62:
				printf("| %i%s| CRZ(a,[d])  | %i%s| %i%s| %i%s| %i%s| \n", i, ii, a, aa, c, cc, d, dd, memory, mm);
				break;
			case 81:
				printf("| %i%s| END         | %i%s| %i%s| %i%s| %i%s| \n", i, ii, a, aa, c, cc, d, dd, memory, mm);
				break;
			case 68:
				printf("| %i%s| NOP         | %i%s| %i%s| %i%s| %i%s| \n", i, ii, a, aa, c, cc, d, dd, memory, mm);
			default:
				break;
		}
	}
}

int main(int argc, char* argv[]) {
	unsigned int memory[59050];
	unsigned int a, c, d, result;
	FILE* file;
	file = fopen("HelloWorld.txt","r");
	if (file == NULL) {
		fprintf(stderr, "File not found: %s\n",argv[1]);
		return -1;
	}
	a=0;
	c=0;
	d=0;	
	result = 0;
	while (!feof(file) && d < 59050){
		unsigned int instr;
		memory[d] = 0;
		result = fread(memory+d,1,1,file);
		if (result > 1)
			return -1;
		if (result == 0 || memory[d] == 0x1a || memory[d] == 0x04)
			break;
		instr = (memory[d] + d)%94;
		if (memory[d]==' ' || memory[d] == '\t' || memory[d] == '\r' || memory[d] == '\n');
		else if (memory[d] >= 33 && memory[d] < 127 && (instr == 4 || instr == 5 || instr == 23 || instr == 39 || instr == 40 || instr == 62 || instr == 68 || instr == 81))
			d+=result;
		else{
			fprintf(stderr, "Invalid character 0x%02x at 0x%05x.\n",(char)memory[d],d);
			return -1;
		}
	}
	if (file != stdin) {
		fclose(file);
	}
	if (d == 59050) {
		fprintf(stderr, "Maximum program length of 59049 exceeded.\n");
		return -1;
	}
	if (d < 2) {
		fprintf(stderr, "Minimal program length of 2 deceeded.\n");
		return -1;
	}

	while (d < 59049){
		memory[d] = crazy(memory[d-1], memory[d-2]);
		d++;
	}
	d = 0;
	
	escribir(1, 1, 1, 1, 1, 1, 1); //Escribir encabezado de la tabla
	
	unsigned int i=1;
	while (1) {
		unsigned int instruction = memory[c];
		if (instruction < 33 || instruction > 126) {
			fprintf(stderr, "Invalid command 0x%05x at 0x%05x.\n",instruction,c);
			return -1;
		}
		instruction = (instruction+c)%94;
		switch (instruction){
			case 4:
				c = memory[d];
				escribir(instruction, 0, i, a, c, d, memory[d]);
				break;
			case 5:
				printf("IMPRIME: %c \n",(char)(a));
				escribir(instruction, 0, i, a, c, d, memory[d]);
				break;
			case 23:
				printf("INGRESA: \n");
				a = getchar();
				if (a == EOF)
					a = 59048;
				escribir(instruction, 0, i, a, c, d, memory[d]);	
				break;
			case 39:
				a = (memory[d] = rotate_r(memory[d]));
				escribir(instruction, 0, i, a, c, d, memory[d]);
				break;
			case 40:
				d = memory[d];
				escribir(instruction, 0, i, a, c, d, memory[d]);
				break;
			case 62:
				a = (memory[d] = crazy(a, memory[d]));
				escribir(instruction, 0, i, a, c, d, memory[d]);
				break;
			case 81:
				escribir(instruction, 0, i, a, c, d, memory[d]);
				return 0;
			case 68:
				escribir(instruction, 0, i, a, c, d, memory[d]);
				break;
			default:
				break;
		}
		if (memory[c] < 33)
			memory[c] += 94;
		memory[c]-=33;
		if (memory[c] > 93)
			memory[c] %= 94;
		memory[c] = translation[memory[c]];
		c = (c+1)%59049;
		d = (d+1)%59049;
		i+=1;
	}
	return 0;
}
