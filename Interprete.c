/*
 * Malbolge interpreter.
 * 2012, 2017 Matthias Lutter.
 * Please visit <http://www.matthias-ernst.eu/malbolge.html>
 *
 * To the extent possible under law, the author has dedicated all copyright
 * and related and neighboring rights to this software to the public domain 
 * worldwide. This software is distributed without any warranty.
 *
 * See <http://creativecommons.org/publicdomain/zero/1.0/>.
 */

#include <stdio.h>

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

int main(int argc, char* argv[]) {
	unsigned int memory[59050];
	unsigned int a, c, d, result;
	//Abrir archivo
	FILE* file;
	file = fopen("HelloWorld.txt","r");
	if (file == NULL) {
		fprintf(stderr, "File not found: %s\n",argv[1]);
		return -1;
	}
	//
	a=0;
	c=0;
	d=0;	
	result = 0;
	//Evaluar precondiciones
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
			return -1; //Caracteres invalidos no son aceptados
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
	//
	//Terminar de llenar el vector memory
	while (d < 59049){
		memory[d] = crazy(memory[d-1], memory[d-2]);
		d++;
	}
	//
	d = 0;
	//Ejecutar el programa
	while (1) {
		unsigned int instruction = memory[c];
		if (instruction < 33 || instruction > 126) {
			fprintf(stderr, "Invalid command 0x%05x at 0x%05x.  c=%i\n",instruction,c,c);
			return -1;
		}
		instruction = (instruction+c)%94;
		switch (instruction){
			case 4:
				c = memory[d];
				break;
			case 5:
				printf("%c",(char)(a));
				break;
			case 23:
				a = getchar();
				if (a == EOF)
					a = 59048;
				break;
			case 39:
				a = (memory[d] = rotate_r(memory[d]));
				break;
			case 40:
				d = memory[d];
				break;
			case 62:
				a = (memory[d] = crazy(a, memory[d]));
				break;
			case 81:
				return 0;
			case 68:
			default:
				break;
		}
		/* Si memory[c]* ha sido modificado, las siguientes instrucciones lo volveran a poner en un rango valido
		o el interprete podria fallar*/
		if (memory[c] < 33)
			memory[c] += 94;
		memory[c]-=33;
		if (memory[c] > 93)
			memory[c] %= 94;
		//
		//Comando de encriptacion
		memory[c] = translation[memory[c]];
		//
		c = (c+1)%59049;
		d = (d+1)%59049;
	}
	return 0;
}
