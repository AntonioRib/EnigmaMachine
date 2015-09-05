/********************************************************************************
 *  Enigma.c																	*
 * 	Antonio Ribeiro - nº 41674													*
 * 	Vasco Coelho - nº 41825														*
 *																				*
 *		Felizmente, pensamos que todos os objectivos do trabalho foram 			*
 *	atingidos. Para alem de conseguirmos cumprir o objectivo de 				*
 *	implementar uma EnigmaMachine tambem conseguimos que esta fosse 			*
 *	extensível bastando entao mudar certas constantes para mudar o 				*
 *	“formato” da maquina. (número de rotores, tamanhos, ate o alfabeto			*
 *	entre outros).																*
 *	 	Tambem conseguimos modular o nosso programa, podendo muito bem 			*
 *	 ser utilizado como biblioteca (faltando para isso criar o ficheiro .h,		*
 *	 etc.), alem de tentarmos simplificar e dar o maximo de escolhas possíveis	*
 *	 para possíveis implementaçoes desta maquina.								*
 *	 	 Para o input optamos para utilizar as funçoes fgets ou getchar para	*
 *	 obter o input do stdin e depois tratar do input com a funçao sscanf. 		*
 *	 Optamos esta abordagem pois a funçao scanf nao nos dar segurança quanto 	*
 *	 a buffer Overflows pois so parara aquando do aparecimento de um ‘/n’ 		*
 *	 independentemente do tamanho do buffer de armazenamento. As funçoes 		*
 *	 utilizadas nao tem este problema e a partir destas podemos entao sim 		*
 *	 tratar o input com uma variante (sscanf). 									*
 *	 	De inicio implementamos os rotores com doubleStepping para estar igual 	*
 *	 ao simulador disponibilizado, mas depois de ler melhor o enunciado e com 	*
 *	 o realcar desse tema no forum da disciplina implementamos as rotacoes como	*
 *	 o trabalho pedia.															*
 *	 	 O nosso trabalho esta organizado tendo primeiro as declaraçoes dos 	*
 *	 tipos, depois as Funçoes para tratar de 1 reflector, as funçoes para		*
 *	 tratar de 1 rotor e por fim as funçoes para tratar da enigma machine 		*
 *	 no seu todo.																*
 *	 	 Por fim, optamos para na nossa implementaçao trabalhar com ponteiros	*
 *	 para rotores e reflectores e nao com os rotores e reflectores em si. 		*
 *	 As nossas declaraçoes permitem ambas as implementaçoes.					*
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
 *	 a posiçao em que faz rodar o rotor seguinte,
 *	 e quandas rotaçoes ja efectuou
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


/***  FUNÇoES PARA 1 REFLECTOR ***/

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
 * @desc Fornece uma nova posiçao a partir da informaçao do reflector.
 * @param reflector reflector - O reflector a usar.
 * @param int pos - A posiçao de entrada
 * @return int - A posiçao de saída obtida atraves da informaçao do reflector.
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
 * @param int rotationPos - a posiçao que causa a rotaçao do rotor seguinte
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
 * @desc Roda um rotor um dado numero de posiçoes
 * @param rotor rotor - O rotor a rodar
 * @param int rotations - o numero de rotaçoes
 */
static void rotateRotor(rotor rotor, int rotations){
	rotor->rotations = (rotor->rotations+rotations)%ROTORLENGTH;
}

/*
 * @desc Roda um rotor uma posiçao
 * @param rotor rotor - O rotor a rodar
 */
void rotateRotorBy1(rotor rotor){
	rotateRotor(rotor, 1);
}

/*
 * @desc Encontra a posiçao de dado char no rotor
 * 		Funcao auxiliar.
 * @param rotor rotor - O rotor onde procurar
 * @param char x - o char a procurar
 * @return int - A posiçao do char no rotor
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
 * 		com a letra em que começa em primeiro.
 * @param rotor rotor - O rotor a fazer o reset.
 */
void resetRotor(rotor rotor){
	rotor->rotations = 0;
}

/*
 * @desc Encontra a posiçao de dado char no alfabeto
 * @param char in - o char a procurar
 * @return int - A posiçao do char no alfabeto
 */
int findOnAlphabet(char in){
	return in-ALPHABET[0];
}

/*
 * @desc Inicializa o rotor na letra dada.
 * @param rotor rotor - O rotor a inicializar
 * @param char x - A letra do alfabeto que dara a posiçao de começo
 */
void initializeRotor(rotor rotor, char x){
	rotateRotor(rotor, findOnAlphabet(x));
}

/*
 * @desc Encontra a letra de uma dada posiçao num dado rotor
 * @param rotor rotor - O rotor a procurar
 * @param int pos - A posiçao que se quer
 * @return char - A letra do alfabeto na posiçao,
 * 					ou NULL caso a posiçao seja invalida.
 */
char getCharAtPosOnRotor(rotor rotor, int pos){
	return rotor->rotor[(pos+rotor->rotations)%ALPHABETLENGTH];
}

/*
 * @desc Encontra a posiçao de dado char no rotor
 * @param rotor rotor - O rotor onde procurar
 * @param char in - o char a procurar
 * @return int - A posiçao do char no rotor, ou NULL caso nao encontre.
 */
int getPosByCharOnRotor(rotor rotor, char in){
	return findPositionOnRotor(rotor, in);
}

/*
 * @desc Encontra a posiçao de dado char num alfabeto com rotaçoes
 * @param char x - o char a procurar
 * @param int rotations - o numero de rotaçoes no alfabeto
 * @return int - A posiçao do char no alfabeto
 */
int getPosOnRotatedAlphabet(char x, int rotations){
	return (ALPHABETLENGTH+findOnAlphabet(x)-rotations)%ALPHABETLENGTH;
}

/*
 * @desc Encontra a letra numa dada posiçao no alfabeto com rotaçoes
 * @param int position - a posiçao pedida
 * @param int rotations - o numero de rotaçoes no alfabeto
 * @return char - A letra na dada posiçao do alfabeto rodado
 */
char getCharOnRotatedAlphabet(int pos, int rotations){
	return ALPHABET[(pos+rotations)%ALPHABETLENGTH];
}

/*
 * @desc Retorna o caracter codificado num dado rotor.
 * 	Procura o caracter no alfabeto rodado com o numero de rotaçoes
 * 	do rotor anterior, e da o caracter correscondente a esse posiçao
 * 	no rotor dado.
 * 	Feito nas codificaçoes do input ate ao reflector.
 * @param rotor rotor - O rotor que vai codificar o caracter.
 * @param char in - O caracter
 * @param int rotations - a posiçao de entrada no rotor.
 * @return char - A letra codificada.
 */
char encodeChar(rotor rotor, char in, int rotations){
	return getCharAtPosOnRotor(rotor, getPosOnRotatedAlphabet(in, rotations));
}

/*
 * @desc Retorna o caracter descodificado num dado rotor.
 * 	Procura a posiçao do caracter no rotor dado e de seguida vai buscar o caracter
 * 	correspondente a posiçao no alfabeto rodado com as rotaçoes do rotor seguinte.
 * 	Feito nas codificaçoes do reflector ate ao output.
 * @param rotor rotor - O rotor que vai descodificar o caracter
 * @param char in - O caracter
 * @param int rotations - a posiçao de entrada no rotor.
 * @return char - A letra descodificada
 * 				(codificada no sentido relfector -> output)
 */
char decodeChar(rotor rotor, char in, int rotations){
	return getCharOnRotatedAlphabet(getPosByCharOnRotor(rotor, in), rotations);
}

/*** FUNCOES PARA ENIGMA MACHINE (VaRIOS ROTORES E 1 REFLECTOR) ***/

/*
 * @desc Faz a rotaçao normal dos rotores, rodando obrigatoriamente o rotor
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
 * @desc Poe todos os rotores de volta a sua posiçao inicial.
 * @param rotor* r - Vector com os rotores utilizados.
 */
void resetRotors(rotor* rotors){
	int i;
	for(i=0; i<ROTORSNUMBER;i++){
		resetRotor(rotors[i]);
	}
}

/*
 * @desc Inicializa todos os rotores a partir numa determinada posiçao
 * @param rotor* r - Vector com os rotores a inicializar.
 * @param char* begin - String com as posiçoes
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
 * 	A funçao esta feita para utilizar qualquer numero de rotores possível.
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
 *  delega para outras funçoes e no final manda o output.
 *  Aqui sao unicamente declarados ponteiros para os rotores e reflectores
 */
int main(int argc, char *argv[] ){
	int sent = 0, i = 0, turnsPosition = 0;
	char sequences[ROTORLENGTH];
	char character; 									//caracter a codificar
	char input[MAXINPUT]; //string para guardar os inputs com as configuraçoes
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



