#include <stdio.h>
#include <stdlib.h>
//posicion donde los registros
#define CS 0
#define DS 1
#define IP 5
#define CC 8
#define AC 9
#define EAX 10
#define EBX 11
#define ECX 12
#define EDX 13
#define EEX 14
#define EFX 15

#define AL %000000FF
#define AH %0000FF00
#define AX %0000FFFF

#define REG 16 //cantidad de registros
#define MEM 16834 //cantidad de memoria
#define TS 8 //tamaño de la tabla de segmento
#define MNE 25 //cantidad de mnemonicos

typedef struct{
  int registros[REG],tablaSeg[TS];
  unsigned char memoria[MEM];
}comp;

typedef struct{
  int OP,tam;
}tamYOpe;

typedef void func(comp* componentes, tamYOpe A, tamYOpe B);

typedef char cad [5];

int posMemoria(int tablaSeg[], int x);
int posRegistro(int registros[],int x);
int valorMemoria(unsigned char memoria[], int tablaSeg[], int x);
int valorRegistro(int registros[],int x);
int getVALOR(comp componentes,tamYOpe OP);
void setVALOR(comp *componentes,tamYOpe A,int valor);
int convierteHexa(char *cadena, int valorCH, int puntero);
int convierteDec(char *cadena, int valorCH, int puntero);
int convierteOct(char *cadena, int valorCH, int puntero);


void MOV(comp* componentes, tamYOpe A, tamYOpe B);
void ADD(comp* componentes, tamYOpe A, tamYOpe B);
void SUB(comp* componentes, tamYOpe A, tamYOpe B);
void SWAP(comp* componentes, tamYOpe A, tamYOpe B);
void MUL(comp* componentes, tamYOpe A, tamYOpe B);
void DIV(comp* componentes, tamYOpe A, tamYOpe B);
void DIV(comp* componentes, tamYOpe A, tamYOpe B);
void CMP(comp* componentes, tamYOpe A, tamYOpe B);
void SHL(comp* componentes, tamYOpe A, tamYOpe B);
void SHR(comp* componentes, tamYOpe A, tamYOpe B);
void AND(comp* componentes, tamYOpe A, tamYOpe B);
void OR(comp* componentes, tamYOpe A, tamYOpe B);
void XOR(comp* componentes, tamYOpe A, tamYOpe B);
void RND(comp* componentes, tamYOpe A, tamYOpe B);
void SYS(comp* componentes, tamYOpe A, tamYOpe B);
void JMP(comp* componentes, tamYOpe A, tamYOpe B);
void JZ(comp* componentes, tamYOpe A, tamYOpe B);
void JP(comp* componentes, tamYOpe A, tamYOpe B);
void JN(comp* componentes, tamYOpe A, tamYOpe B);
void JNZ(comp* componentes, tamYOpe A, tamYOpe B);
void JNP(comp* componentes, tamYOpe A, tamYOpe B);
void JNN(comp* componentes, tamYOpe A, tamYOpe B);
void LDL(comp* componentes, tamYOpe A, tamYOpe B);
void LDH(comp* componentes, tamYOpe A, tamYOpe B);
void NOT(comp* componentes, tamYOpe A, tamYOpe B);

