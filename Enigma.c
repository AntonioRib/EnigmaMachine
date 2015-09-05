/********************************************************************************
 *  Enigma.c																	*
 * 	Antonio Ribeiro - n� 41674													*
 * 	Vasco Coelho - n� 41825														*
 *																				*
 *		Felizmente, pensamos que todos os objectivos do trabalho foram 			*
 *	atingidos. Para alem de conseguirmos cumprir o objectivo de 				*
 *	implementar uma EnigmaMachine tambem conseguimos que esta fosse 			*
 *	extens�vel bastando entao mudar certas constantes para mudar o 				*
 *	�formato� da maquina. (n�mero de rotores, tamanhos, ate o alfabeto			*
 *	entre outros).																*
 *	 	Tambem conseguimos modular o nosso programa, podendo muito bem 			*
 *	 ser utilizado como biblioteca (faltando para isso criar o ficheiro .h,		*
 *	 etc.), alem de tentarmos simplificar e dar o maximo de escolhas poss�veis	*
 *	 para poss�veis implementa�oes desta maquina.								*
 *	 	 Para o input optamos para utilizar as fun�oes fgets ou getchar para	*
 *	 obter o input do stdin e depois tratar do input com a fun�ao sscanf. 		*
 *	 Optamos esta abordagem pois a fun�ao scanf nao nos dar seguran�a quanto 	*
 *	 a buffer Overflows pois so parara aquando do aparecimento de um �/n� 		*
 *	 independentemente do tamanho do buffer de armazenamento. As fun�oes 		*
 *	 utilizadas nao tem este problema e a partir destas podemos entao sim 		*
 *	 tratar o input com uma variante (sscanf). 									*
 *	 	De inicio implementamos os rotores com doubleStepping para estar igual 	*
 *	 ao simulador disponibilizado, mas depois de ler melhor o enunciado e com 	*
 *	 o realcar desse tema no forum da disciplina implementamos as rotacoes como	*
 *	 o trabalho pedia.															*
 *	 	 O nosso trabalho esta organizado tendo primeiro as declara�oes dos 	*
 *	 tipos, depois as Fun�oes para tratar de 1 reflector, as fun�oes para		*
 *	 tratar de 1 rotor e por fim as fun�oes para tratar da enigma machine 		*
 *	 no seu todo.																*
 *	 	 Por fim, optamos para na nossa implementa�ao trabalhar com ponteiros	*
 *	 para rotores e reflectores e nao com os rotores e reflectores em si. 		*
 *	 As nossas declara�oes permitem ambas as implementa�oes.					*
 *																				*
 ********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ROTORLENGTH 26
#define ALPHABETLENGTH 26
#define REFLECTORLENGTH 26
#define ROTORSNUMBER 3
#define REFLECTORNUMBER 1
#define MAXINPUT 32
#define ALPHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define SCAPELETTER "*"
#define TERMINATOR '\0'
/*
 *	@desc Um rotor possui, um vector para guardar a sua sequencia,
 *	 a posi�ao em que faz rodar o rotor seguinte,
 *	 e quandas rota�oes ja efectuou
 *	 rotorO e o rotor em si,
 *	 rotor e um ponteiro para um rotorO
 */
typedef struct rotor{
	char rotor[ROTORLENGTH+1];
	int rotationPos;
	int rotations;
} rotorO, *rotor;

/*
 * @desc Um reflector possui a sua sequencia
 * 	 reflectorO e o reflector em si,
 *	 reflector e um ponteiro para um reflectorO
 */
typedef struct reflector{
	char reflector[REFLECTORLENGTH+1];
} reflectorO, *reflector;


/***  FUN�oES PARA 1 REFLECTOR ***/

/*
 * @desc Cria um novo reflector, alocando a quantidade de memoria necessaria
 * @param char* conf - a sequencia do rotor
 * @param int length - o tamanho da sequencia
 * @return reflector - Reflector alocado
 * 					   ou NULL se nao houverem recursos disponiveis
 */
reflector newReflector(char* conf){
	reflector n = malloc(sizeof(reflectorO));
	if(n == NULL){
		return NULL;
	}
	memmove(n->reflector, conf, REFLECTORLENGTH);
	n->reflector[REFLECTORLENGTH] = TERMINATOR;
	return n;
}


/*
 * @desc Fornece uma nova posi�ao a partir da informa�ao do reflector.
 * @param reflector reflector - O reflector a usar.
 * @param int pos - A posi�ao de entrada
 * @return int - A posi�ao de sa�da obtida atraves da informa�ao do reflector.
 */
int reflect(reflector reflector, int pos){
	int i = -1;
	for(i=0; i<REFLECTORLENGTH; i++){
		if((reflector->reflector[i] == reflector->reflector[pos])
													&& (i != pos)){
			return i;
		}
	}
	return -1;
}

/*
 * @desc Liberta os recursos alocados por um reflector
 * @param reflector reflector - o reflector
 */
void freeReflector(reflector reflector){
	free(reflector);
}


/*** FUNCOES PARA 1 ROTOR ***/

/*
 * @desc Cria um novo rotor, alocando a quantidade de memoria necessaria
 * @param char* conf - a sequencia do rotor
 * @param int length - o tamanho da sequencia
 * @param int rotationPos - a posi�ao que causa a rota�ao do rotor seguinte
 * @return rotor - Rotor alocado ou NULL se nao houverem recursos disponiveis
 */
rotor newRotor(char* conf, int rotationPos){
	rotor n = malloc(sizeof(rotorO));
	if(n == NULL){
		return NULL;
	}
	n->rotationPos = rotationPos;
	memmove(n->rotor, conf, ROTORLENGTH);
	n->rotor[ROTORLENGTH] = TERMINATOR;
	n->rotations = 0;
	return n;
}

/*
 * @desc Liberta os recursos alocados por um rotor
 * @param rotor rotor - o rotor
 */
void freeRotor(rotor rotor){
	free(rotor);
}

/*
 * @desc Roda um rotor um dado numero de posi�oes
 * @param rotor rotor - O rotor a rodar
 * @param int rotations - o numero de rota�oes
 */
static void rotateRotor(rotor rotor, int rotations){
	rotor->rotations = (rotor->rotations+rotations)%ROTORLENGTH;
}

/*
 * @desc Roda um rotor uma posi�ao
 * @param rotor rotor - O rotor a rodar
 */
void rotateRotorBy1(rotor rotor){
	rotateRotor(rotor, 1);
}

/*
 * @desc Encontra a posi�ao de dado char no rotor
 * 		Funcao auxiliar.
 * @param rotor rotor - O rotor onde procurar
 * @param char x - o char a procurar
 * @return int - A posi�ao do char no rotor
 */
static int findPositionOnRotor(rotor rotor, char x){
	int i = -1;
	for(i = 0; i<ROTORLENGTH; i++){
		if(rotor->rotor[i] == x){
			break;
		}
	}
	return (ROTORLENGTH+(i-rotor->rotations))%ROTORLENGTH;
}

/*
 * @desc Coloca um rotor na sua posicao original, isto e
 * 		com a letra em que come�a em primeiro.
 * @param rotor rotor - O rotor a fazer o reset.
 */
void resetRotor(rotor rotor){
	rotor->rotations = 0;
}

/*
 * @desc Encontra a posi�ao de dado char no alfabeto
 * @param char in - o char a procurar
 * @return int - A posi�ao do char no alfabeto
 */
int findOnAlphabet(char in){
	return in-ALPHABET[0];
}

/*
 * @desc Inicializa o rotor na letra dada.
 * @param rotor rotor - O rotor a inicializar
 * @param char x - A letra do alfabeto que dara a posi�ao de come�o
 */
void initializeRotor(rotor rotor, char x){
	rotateRotor(rotor, findOnAlphabet(x));
}

/*
 * @desc Encontra a letra de uma dada posi�ao num dado rotor
 * @param rotor rotor - O rotor a procurar
 * @param int pos - A posi�ao que se quer
 * @return char - A letra do alfabeto na posi�ao,
 * 					ou NULL caso a posi�ao seja invalida.
 */
char getCharAtPosOnRotor(rotor rotor, int pos){
	return rotor->rotor[(pos+rotor->rotations)%ALPHABETLENGTH];
}

/*
 * @desc Encontra a posi�ao de dado char no rotor
 * @param rotor rotor - O rotor onde procurar
 * @param char in - o char a procurar
 * @return int - A posi�ao do char no rotor, ou NULL caso nao encontre.
 */
int getPosByCharOnRotor(rotor rotor, char in){
	return findPositionOnRotor(rotor, in);
}

/*
 * @desc Encontra a posi�ao de dado char num alfabeto com rota�oes
 * @param char x - o char a procurar
 * @param int rotations - o numero de rota�oes no alfabeto
 * @return int - A posi�ao do char no alfabeto
 */
int getPosOnRotatedAlphabet(char x, int rotations){
	return (ALPHABETLENGTH+findOnAlphabet(x)-rotations)%ALPHABETLENGTH;
}

/*
 * @desc Encontra a letra numa dada posi�ao no alfabeto com rota�oes
 * @param int position - a posi�ao pedida
 * @param int rotations - o numero de rota�oes no alfabeto
 * @return char - A letra na dada posi�ao do alfabeto rodado
 */
char getCharOnRotatedAlphabet(int pos, int rotations){
	return ALPHABET[(pos+rotations)%ALPHABETLENGTH];
}

/*
 * @desc Retorna o caracter codificado num dado rotor.
 * 	Procura o caracter no alfabeto rodado com o numero de rota�oes
 * 	do rotor anterior, e da o caracter correscondente a esse posi�ao
 * 	no rotor dado.
 * 	Feito nas codifica�oes do input ate ao reflector.
 * @param rotor rotor - O rotor que vai codificar o caracter.
 * @param char in - O caracter
 * @param int rotations - a posi�ao de entrada no rotor.
 * @return char - A letra codificada.
 */
char encodeChar(rotor rotor, char in, int rotations){
	return getCharAtPosOnRotor(rotor, getPosOnRotatedAlphabet(in, rotations));
}

/*
 * @desc Retorna o caracter descodificado num dado rotor.
 * 	Procura a posi�ao do caracter no rotor dado e de seguida vai buscar o caracter
 * 	correspondente a posi�ao no alfabeto rodado com as rota�oes do rotor seguinte.
 * 	Feito nas codifica�oes do reflector ate ao output.
 * @param rotor rotor - O rotor que vai descodificar o caracter
 * @param char in - O caracter
 * @param int rotations - a posi�ao de entrada no rotor.
 * @return char - A letra descodificada
 * 				(codificada no sentido relfector -> output)
 */
char decodeChar(rotor rotor, char in, int rotations){
	return getCharOnRotatedAlphabet(getPosByCharOnRotor(rotor, in), rotations);
}

/*** FUNCOES PARA ENIGMA MACHINE (VaRIOS ROTORES E 1 REFLECTOR) ***/

/*
 * @desc Faz a rota�ao normal dos rotores, rodando obrigatoriamente o rotor
 * 	mais perto do Input em uma casa, e os seguintes se for caso disso.
 * @param rotor* r - Vector com os rotores utilizados.
 * @param int mayRotate - Flag para saber se os rotores deverao ou nao rodar.
 */
void rotateRotors(rotor* r, int mayRotate){
	int i;
	if(mayRotate){
		for(i=ROTORSNUMBER-1; i>0; i--){
			if(r[i]->rotations == r[i]->rotationPos){
				rotateRotorBy1(r[i-1]);
			}
		}
		rotateRotorBy1(r[ROTORSNUMBER-1]);
	}
}

/*
 * @desc Poe todos os rotores de volta a sua posi�ao inicial.
 * @param rotor* r - Vector com os rotores utilizados.
 */
void resetRotors(rotor* rotors){
	int i;
	for(i=0; i<ROTORSNUMBER;i++){
		resetRotor(rotors[i]);
	}
}

/*
 * @desc Inicializa todos os rotores a partir numa determinada posi�ao
 * @param rotor* r - Vector com os rotores a inicializar.
 * @param char* begin - String com as posi�oes
 */
void initializeRotors(rotor* rotors, char* begin){
	int i;
	for(i=0; i<ROTORSNUMBER;i++){
		initializeRotor(rotors[i], begin[i]);
	}
}

/*
 * @desc Liberta os recursos alocados por varios rotores num vector
 * @param rotor* rotors - ponteiro para o primeiro rotor
 */
void freeRotors(rotor* rotors){
	int i;
	for(i=0; i<ROTORSNUMBER; i++){
		freeRotor(rotors[i]);
	}
}

/*
 * @desc Faz o percurso do input ate ao output de um caracter.
 * 	A fun�ao esta feita para utilizar qualquer numero de rotores poss�vel.
 * @param rotor* r - Vector com todos os rotores a utilizar.
 * @param reflector ref - Reflector a utilizar
 * @param char character - Caracter a ser encoded
 * @return char - caracter ja encoded
 */
char encode(rotor* r, reflector ref, char character, int mayRotate){
	int i, var;
	char c;
		rotateRotors(r, mayRotate);
        var =  findOnAlphabet(character);
        c = getCharAtPosOnRotor(r[ROTORSNUMBER-1], var);
		for(i=ROTORSNUMBER-2; i>=0; i--){
			c = encodeChar(r[i], c, r[i+1]->rotations);
		}
		var = reflect(ref, getPosOnRotatedAlphabet(c, r[0]->rotations));
		c = getCharOnRotatedAlphabet(var, r[0]->rotations);
		for(i=0; i+1<ROTORSNUMBER; i++){
			c = decodeChar(r[i], c, r[i+1]->rotations);
		}
		var = getPosByCharOnRotor(r[ROTORSNUMBER-1],c);
		c = ALPHABET[var];
		return c;
}

/*
 * Pede os inputs ao utilizador, interpreta o input,
 *  delega para outras fun�oes e no final manda o output.
 *  Aqui sao unicamente declarados ponteiros para os rotores e reflectores
 */
int main(int argc, char *argv[] ){
	int sent = 0, i = 0, turnsPosition = 0;
	char sequences[ROTORLENGTH];
	char character; 									//caracter a codificar
	char input[MAXINPUT]; //string para guardar os inputs com as configura�oes
	rotor rotors[ROTORSNUMBER]; 			//Vector de ponteiros para rotores
	reflector ref = NULL; 							 //Ponteiro para reflector
	while(i<ROTORSNUMBER+REFLECTORNUMBER){
		fgets(input, MAXINPUT, stdin);
		sscanf(input, "%s %d", sequences, &turnsPosition);
		if(i < ROTORSNUMBER){
			rotors[i] = newRotor(sequences, turnsPosition);
		} else {
			ref = newReflector(sequences);
		}
		i++;
	}
	fgets(input, MAXINPUT, stdin);
	sent = atoi(input);
	while(sent){
		fgets(input, MAXINPUT, stdin);
		sscanf(input, "%s", input);
		resetRotors(rotors);
		initializeRotors(rotors, input);
		while((character = getchar()) != '\n'){
			printf("%c", encode(rotors, ref, character,
					strcmp(&input[ROTORSNUMBER], SCAPELETTER)));
		}
		printf("\n");
		sent--;
	}
	freeRotors(rotors);
	freeReflector(ref);
	return 0;
}



