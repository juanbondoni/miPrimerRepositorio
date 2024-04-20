#include "Operaciones.h"
#include <stdio.h>
#include <time.h>

void MOV(comp* componentes, tamYOpe A, tamYOpe B){
    int aux = getVALOR(*componentes,B);
    setVALOR(componentes,A,aux);
}

void ADD(comp* componentes, tamYOpe A, tamYOpe B){
   int aux1;
   aux1=getVALOR(*componentes,A) + getVALOR(*componentes,B);
   if(aux1 == 0)
        componentes->registros[CC]=0x40000000;
   else
    if(aux1<0)
        componentes->registros[CC]=0x80000000;
    else
      componentes->registros[CC]=0;

   setVALOR(componentes,A,aux1);
}

void SUB(comp* componentes, tamYOpe A, tamYOpe B){
   int aux1;
   aux1=getVALOR(*componentes,A) - getVALOR(*componentes,B);
   if(aux1 == 0)
        componentes->registros[CC]=0x40000000;
   else
    if(aux1<0)
        componentes->registros[CC]=0x80000000;
    else
      componentes->registros[CC]=0;
   setVALOR(componentes,A,aux1);
}

void SWAP(comp* componentes, tamYOpe A, tamYOpe B){
    int aux;
    aux=getVALOR(*componentes,A);
    setVALOR(componentes,A,getVALOR(*componentes,B));
    setVALOR(componentes,B,aux);
}

void MUL(comp* componentes, tamYOpe A, tamYOpe B){
   int aux1;
   aux1=getVALOR(*componentes,A) * getVALOR(*componentes,B);
   if(aux1 == 0)
        componentes->registros[CC]=0x40000000;
   else
    if(aux1<0)
        componentes->registros[CC]=0x80000000;
    else
      componentes->registros[CC]=0;
   setVALOR(componentes,A,aux1);
}

void DIV(comp* componentes, tamYOpe A, tamYOpe B){
   int aux1,aux2;
   aux2=getVALOR(*componentes,B);
   if(aux2 != 0)
   {
        aux1=getVALOR(*componentes,A) / getVALOR(*componentes,B);
        aux2=getVALOR(*componentes,A) % getVALOR(*componentes,B);
        if(aux1 == 0)
          componentes->registros[CC]=0x40000000;
        else
          if(aux1<0)
            componentes->registros[CC]=0x80000000;
          else
            componentes->registros[CC]=0;

        setVALOR(componentes,A,aux1);
        componentes->registros[AC]=aux2;
   }
   else
   {
       printf("no se permite una division por cero\n");
       printf("%d",32/0);
   }
}

void CMP(comp* componentes, tamYOpe A, tamYOpe B){
   int aux1;
   aux1=getVALOR(*componentes,A) - getVALOR(*componentes,B);
   if(aux1 == 0)
        componentes->registros[CC]=0x40000000;
   else
      if(aux1<0)
        componentes->registros[CC]=0x80000000;
      else
        componentes->registros[CC]=0;
}

void SHL(comp* componentes, tamYOpe A, tamYOpe B){
    int aux;
    aux=getVALOR(*componentes,B);
    setVALOR(componentes,A,getVALOR(*componentes,A)<<aux);
}

void SHR(comp* componentes, tamYOpe A, tamYOpe B){
    int aux;
    aux=getVALOR(*componentes,B);
    setVALOR(componentes,A,getVALOR(*componentes,A)>>aux);
}

void AND(comp* componentes, tamYOpe A, tamYOpe B){
   int aux1;
   aux1=getVALOR(*componentes,A) & getVALOR(*componentes,B);
   if(aux1 == 0)
      componentes->registros[CC]=0x40000000;
   else
    if(aux1<0)
      componentes->registros[CC]=0x80000000;
    else
      componentes->registros[CC]=0;
   setVALOR(componentes,A,aux1);
}

void OR(comp* componentes, tamYOpe A, tamYOpe B){
       int aux1;
  aux1=getVALOR(*componentes,A) | getVALOR(*componentes,B);
  if(aux1 == 0)
    componentes->registros[CC]=0x40000000;
   else
    if(aux1<0)
      componentes->registros[CC]=0x80000000;
    else
      componentes->registros[CC]=0;
   setVALOR(componentes,A,aux1);
}

void XOR(comp* componentes, tamYOpe A, tamYOpe B){
       int aux1;
   aux1=getVALOR(*componentes,A) ^ getVALOR(*componentes,B);
   if(aux1 == 0)
      componentes->registros[CC]=0x40000000;
   else
    if(aux1<0)
      componentes->registros[CC]=0x80000000;
    else
      componentes->registros[CC]=0;
   setVALOR(componentes,A,aux1);
}

void RND(comp* componentes, tamYOpe A, tamYOpe B){
    int aux,aletorio;
    aux=getVALOR(*componentes,B);
    srand(time(NULL));
    if(aux<0)
    {
        aletorio=0+rand() % (-1*aux);
        aletorio*=-1;
    }
    else
        aletorio=0+rand() % aux;
    setVALOR(componentes,A,aletorio);
}

void SYS(comp* componentes, tamYOpe A, tamYOpe B){
    int valorSYS,valorEDX,valorAL,valorCL,valorCH,i,j,k,mascara,valorCad,aux,contMem,valorCel,mascara2;

    valorSYS=getVALOR(*componentes,A);
    valorCH = ((componentes->registros[ECX] & 0x0000FF00)<< 16)>>24;
    valorCL = ((componentes->registros[ECX] & 0x000000FF)<< 24)>>24;
    valorAL = ((componentes->registros[EAX] & 0x000000FF)<< 24)>>24;
    valorEDX = componentes->registros[EDX];

    if(valorSYS == 1) //se almacenan los datos leídos desde el teclado a partir de la posición de memoria apuntada por EDX
    {
      //Se debe leer un string variable por pantalla y se lo almacena de forma SECUENCIAL
      //en la memoria de acuerdo al valor del reg AL

      char *cadena = (char *) malloc(100*sizeof(char));
      int puntero=valorEDX;

      for(k=0;k<valorCL;k++){
        switch(valorAL){
          case 8: printf("[%04X] Ingrese un numero en hexadecimal: ",posMemoria(componentes->tablaSeg,puntero));
            break;
          case 4: printf("[%04X] Ingrese un numero en octal: ",posMemoria(componentes->tablaSeg,puntero));
            break;
          case 2: printf("[%04X] Ingrese un caracter: ",posMemoria(componentes->tablaSeg,puntero));
            break;
          case 1: printf("[%04X] Ingrese un numero en decimal: ",posMemoria(componentes->tablaSeg,puntero));
            break;
        }
        gets(cadena);

        switch(valorAL){
          case 8: valorCad = convierteHexa(cadena,valorCH,puntero);
            break;
          case 4: valorCad = convierteOct(cadena,valorCH,puntero);
            break;
          case 2: valorCad = *cadena;
            break;
          case 1: valorCad = convierteDec(cadena,valorCH,puntero);
            break;
        }

        mascara=0x000000FF;
        for(i=(valorCH-1);i>-1;i--){
          componentes->memoria[posMemoria(componentes->tablaSeg,puntero)+i]=valorCad & mascara;
          mascara = mascara << 8;
        }
        puntero+=valorCH;
      }
    }
    else //muestra en pantalla los valores contenidos a partir de la posición de memoria apuntada por EDX
    {
      //Se debe leer, de forma SECUENCIAL, la memoria CL celdas de CH bytes cada una
      //en la memoria.
      contMem=0;
      for(i=0;i<valorCL;i++){

        valorCel=0;
        for(j=0;j<valorCH;j++){

          aux = valorRegistro(componentes->registros,((valorEDX+contMem) & 0x000F0000)>>16) & 0xFFFF0000; //Me quedo con el puntero a la tabla de segmentos
          aux += valorRegistro(componentes->registros,(valorEDX+contMem) >> 16) & 0x0000FFFF; //Almaceno el offset
          aux += (valorEDX+contMem) & 0x0000FFFF; // Le sumo el offset del operando de memoria

          valorCel = valorCel << 8; //Se usa acumulador y se lo desplaza 1 byte a la derecha
          valorCel += componentes->memoria[posMemoria(componentes->tablaSeg,aux)] & 0x000000FF; //Se almacena el byte en la posicion menos significativa

          contMem++;
        }

        aux = valorRegistro(componentes->registros,((valorEDX+contMem) & 0x000F0000)>>16) & 0xFFFF0000; //Me quedo con el puntero a la tabla de segmentos
        aux += valorRegistro(componentes->registros,(valorEDX+contMem) >> 16) & 0x0000FFFF; //Almaceno el offset
        aux += (valorEDX+contMem) & 0x0000FFFF; // Le sumo el offset del operando de memoria

        mascara2=0x00000001;
        printf("celda [%d]: ",posMemoria(componentes->tablaSeg,aux-valorCH));
        for(int t=0;t<4;t++)
        {
            switch(valorAL & mascara2){
              case 8: printf("%%%x ",valorCel);
                break;
              case 4:printf("@%o ",valorCel);
                break;
              case 2:{
                char *cadena = (char *) malloc(4*sizeof(char));
                if((valorCel>=32 && valorCel<=126) || valorCel>=128){ //Se imprime todo caracter que pertenezca a la tabla ASCII
                   mascara=0x000000FF; k=3;
                  for(j=0;j<valorCH;j++){ //Se carga una cadena de caracteres con el valor de la celda
                    *(cadena+k) = valorCel & mascara;
                    valorCel = valorCel >> 8;
                    k--;
                  }
                  k=4-valorCH;
                  for(j=0;j<valorCH;j++){ //Se muestran los caracteres de la cadena cargada desde la posicion 4-valorCH hasta la 3
                    printf("%c",*(cadena+k));
                    k++;
                  }
                  printf(" ");
                }
                else
                  printf("%c ",'.');
              }
                break;
              case 1: printf("%d ",valorCel);
                break;
            }
            mascara2=mascara2<<1;
        }
        printf("\n");
      }
    }
}

void JMP(comp* componentes, tamYOpe A, tamYOpe B){
    int aux;
    aux=getVALOR(*componentes,A);
    componentes->registros[IP]=componentes->registros[IP] & 0xFFFF0000;

    componentes->registros[IP]+=aux;
}

void JZ(comp* componentes, tamYOpe A, tamYOpe B){
   int aux;
   if((componentes->registros[CC]& 0x40000000) == 0x40000000)
   {
    aux=getVALOR(*componentes,A);
    componentes->registros[IP]=componentes->registros[IP] & 0xFFFF0000;
    componentes->registros[IP]+=aux;
   }
}

void JP(comp* componentes, tamYOpe A, tamYOpe B){
    int aux;
    if((componentes->registros[CC] & 0xC0000000) == 0)
    {
        aux=getVALOR(*componentes,A);
        componentes->registros[IP]=componentes->registros[IP] & 0xFFFF0000;
        componentes->registros[IP]+=aux;
    }
}

void JN(comp* componentes, tamYOpe A, tamYOpe B){
    int aux;
    if((componentes->registros[CC] & 0x80000000)==0x80000000)
    {
        aux=getVALOR(*componentes,A);
        componentes->registros[IP]=componentes->registros[IP] & 0xFFFF0000;
        componentes->registros[IP]+=aux;
    }
}

void JNZ(comp* componentes, tamYOpe A, tamYOpe B){
    JN(componentes,A,B);
    JP(componentes,A,B);
}

void JNP(comp* componentes, tamYOpe A, tamYOpe B){
    JN(componentes,A,B);
    JZ(componentes,A,B);
}

void JNN(comp* componentes, tamYOpe A, tamYOpe B){
    JZ(componentes,A,B);
    JP(componentes,A,B);
}

void LDL(comp* componentes, tamYOpe A, tamYOpe B){
    int aux;
    aux=(getVALOR(*componentes,A)&0x0000FFFF);
    componentes->registros[AC]=componentes->registros[AC] & 0xFFFF0000;
    componentes->registros[AC]+=aux;
}

void LDH(comp* componentes, tamYOpe A, tamYOpe B){
     int aux;
    aux=(getVALOR(*componentes,A)&0x0000FFFF)<<16;
    componentes->registros[AC]=componentes->registros[AC] & 0x0000FFFF;
    componentes->registros[AC]+=aux;
}

void NOT(comp* componentes, tamYOpe A, tamYOpe B){
   int aux1;
   aux1=~getVALOR(*componentes,A);
   if(aux1 == 0)
      componentes->registros[CC]=0x40000000;
   else
    if(aux1<0)
      componentes->registros[CC]=0x80000000;
    else
      componentes->registros[CC]=0;
   setVALOR(componentes,A,aux1);
}
