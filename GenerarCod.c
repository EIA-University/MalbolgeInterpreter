/* Este programa funciona analogo a Interprete.c , pero con la diferencia que en el se puede generar codigo malbolge facilmente.
Fue creado orignilamente para facilitar la generacion de codigo 
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

int main(int argc, char* argv[]){
    unsigned int memory[59050];
    unsigned int a, c, d, result;
    FILE* file;
    file = fopen("HelloWorld.txt","r"); //Puedes usar cualquier archivo .txt (Siempre y cuando tenga codigo de Malbolge valido), ya que este
    //codigo no se ejecutara
    if (file == NULL){
        fprintf(stderr, "File not found: %s\n",argv[1]);
        return -1;
    }
    a=0;
    c=0;
    d=0;
    result = 0;
/*Ingresa numeros entre 33 y 126 (Numeros que representan caracteres ASCII) directamente en memory.
Se debe tener en cuenta que (memory[c]+c)%94 debe ser igual a un instruccion, por tanto debes tener presente en que posicion te conviene 
guardar el numero o los numeros que necesitas */

//El siguiente ejemplo realiza 5 NOP y luego un End Program
	memory[0]=68;
	memory[1]=67;
	memory[2]=66;
	memory[3]=65;
	memory[4]=64;
	memory[5]=76;
 //
 //Esta parte del codigo es directamente para convertir las instrucciones que programaste en codigo Malbolge, (ASCII)
    unsigned int i;
    for(i=0; i<6; i++){
    	printf("%c", (char)memory[i]);
	}
	//Es muy posible que necesites utilizar ese codigo varias veces
	d=6; // debes dejar el puntero de datos en el valor siguiente en memory, para poder terminar de llenar el vector
    while (d < 59049){
        memory[d] = crazy(memory[d-1], memory[d-2]);
        d++;
    }
    d = 0;
    //El programa termina ejecutanto el codigo que acabas de generar
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
        if (memory[c] < 33)
            memory[c] += 94;
        memory[c]-=33;
        if (memory[c] > 93)
            memory[c] %= 94;
        memory[c] = translation[memory[c]];

        c = (c+1)%59049;
        d = (d+1)%59049;
    }
    return 0;
}
