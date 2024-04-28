#include <stdio.h>
#include <stdlib.h>
#include "Operaciones.h"
#include <string.h>
#include <ctype.h>

///carga las memorias y carga la tabla de segemento
int cargaMemoria(comp* componetes,FILE* archB){
  char reg[8],aux;
  int i=0,x;
  componetes->tablaSeg[0]=0x00000000;//

  if(archB==NULL)
    printf("NO existe el archivo");
  else{
    fread(&reg,8*sizeof(char),1,archB); //se lee la cabecera del archivo binario
    if(!feof(archB)){
      i=reg[6]; //En i se almacena el A->tamño en bytes del codigo
      i= i << 8;
      x=reg[7];
      i=(i & 0xffffff00) + (x & 0x000000ff);
      if(MEM >= i){//Se verifica que el segmento de codigo entre en memoria
        i=0;
        while(fread(&aux,sizeof(char),1,archB)==1){
          componetes->memoria[i]=aux;
          i++;
        }
        componetes->tablaSeg[CS]=i&0xFFFF;//donde finaliza el code segment
        componetes->tablaSeg[DS]=i;
        componetes->tablaSeg[DS]=componetes->tablaSeg[DS] << 16;//donde inicia el data segment
        componetes->tablaSeg[DS]+=MEM&0xFFFF; // finaliza el data segment
        fclose(archB);
        return 1;
      }
    }
  }
  return 0;
}

//setea el valor en el operando A
void setVALOR(comp *componentes,tamYOpe A,int valor)
{
    int aux,i,x;
    if(A.tam == 3)
    {
      aux = valorRegistro(componentes->registros,(A.OP & 0x000F0000)>>16) & 0xFFFF0000; //Me quedo con el puntero a la tabla de segmentos
      aux += valorRegistro(componentes->registros,A.OP >> 16) & 0x0000FFFF; //Almaceno el offset
      aux += A.OP & 0x0000FFFF; // Le sumo el offset del operando de memoria
      for(i=0;i<4;i++)
      {
        x=8*(3-i);
        componentes->memoria[posMemoria(componentes->tablaSeg,aux & 0x000FFFFF)+i]=(valor&(0x000000FF<<x))>>x;
      }
    }
    else if(A.tam == 1)
        {
           switch(A.OP>>4)
           {
               case 1: { //guardar en low (4to byte)
                   aux=componentes->registros[posRegistro(componentes->registros,A.OP)] & 0xFFFFFF00;
                   aux+=valor & 0x000000FF;
                   componentes->registros[posRegistro(componentes->registros,A.OP)]=aux;
               }break;
               case 2:{ //guardar en high
                   aux=componentes->registros[posRegistro(componentes->registros,A.OP)] & 0xFFFF00FF;
                   aux+=(valor & 0x000000FF)<<8;
                   componentes->registros[posRegistro(componentes->registros,A.OP)]=aux;
               }break;
               case 3:{
                   aux=componentes->registros[posRegistro(componentes->registros,A.OP)] & 0xFFFF0000;
                   aux+=valor & 0x0000FFFF;
                   componentes->registros[posRegistro(componentes->registros,A.OP)]=aux;
               }break;
               default:componentes->registros[posRegistro(componentes->registros,A.OP)]=valor;
            }
        }
}

//devuelve el valor que indica el operando
int getVALOR(comp componentes,tamYOpe OP)
{
  int aux;

  if(OP.tam == 3)
  {
    aux = valorRegistro(componentes.registros,(OP.OP & 0x000F0000)>>16) & 0xFFFF0000; //Me quedo con el puntero a la tabla de segmentos
    aux += valorRegistro(componentes.registros,OP.OP >> 16) & 0x0000FFFF; //Almaceno el offset
    aux += OP.OP & 0x0000FFFF; // Le sumo el offset del operando de memoria
    return valorMemoria(componentes.memoria,componentes.tablaSeg,aux);
  }
  else if(OP.tam == 1)
  {
    return valorRegistro(componentes.registros,OP.OP);
  }
  else
      return ((OP.OP)<<16)>>16;
}

//recibe un registro y devuelve su posicion verdadera(fisica) en memoria
int posMemoria(int tablaSeg[], int x){
  int aux1, aux2;

  aux1=x&0xFFFF0000;//saco los dos primeros byte para saber donde apunta la tabla de segmentos
  aux1=aux1 >> 16;//los seteo en los dos byte menos significativos
  x=x&0x0000FFFF;//agarro el offset del registro
  aux2=tablaSeg[aux1]&0xFFFF0000;//me almaceno los dos byte mas significativos a donde apunta el registro en memoria
  if((aux2>>16)+x<=MEM && aux1<8){
    return (aux2>>16)+x;//le sumo el offset
  }
  else{
    printf("Se accedio a una parte fuera del segmento recorrido en memoria.\n\n");
    printf("%d",5/0);
    return 0;
  }
}


//recibe un operando de registro y devuelve su posicion verdadera(fisica) en los registros
int posRegistro(int registros[],int x)
{
  return x & 0x0000000f;
}


int convierteHexa(char *cadena, int valorCH, int puntero){
  int j,valorCad=0;

  j=strlen(cadena);
  for(j=0;j<strlen(cadena);j++){
    switch(toupper(*(cadena+j))){
      case 'F': valorCad+=0xf;
        break;
      case 'E': valorCad+=0xe;
          break;
      case 'D': valorCad+=0xd;
          break;
      case 'C': valorCad+=0xc;
          break;
      case 'B': valorCad+=0xb;
          break;
      case 'A': valorCad+=0xa;
          break;
      case '9': valorCad+=0x9;
          break;
      case '8': valorCad+=0x8;
          break;
      case '7': valorCad+=0x7;
          break;
      case '6': valorCad+=0x6;
          break;
      case '5': valorCad+=0x5;
          break;
      case '4': valorCad+=0x4;
          break;
      case '3': valorCad+=0x3;
          break;
      case '2': valorCad+=0x2;
          break;
      case '1': valorCad+=0x1;
          break;
      case '0': valorCad+=0x0;
          break;
    }
    valorCad=valorCad<<4;
  }
  valorCad=valorCad>>4;

  return valorCad;
}

int convierteOct(char *cadena, int valorCH, int puntero){
  int valorCad,j;

  j=strlen(cadena);
  for(j=0;j<strlen(cadena);j++){
    switch(toupper(*(cadena+j))){
      case '7': valorCad+=07;
          break;
      case '6': valorCad+=06;
          break;
      case '5': valorCad+=05;
          break;
      case '4': valorCad+=04;
          break;
      case '3': valorCad+=03;
          break;
      case '2': valorCad+=02;
          break;
      case '1': valorCad+=01;
          break;
      case '0': valorCad+=00;
          break;
    }
    valorCad=valorCad<<3;
  }
  valorCad=valorCad>>3;

  return valorCad;
}


int convierteDec(char *cadena, int valorCH, int puntero){
  int valorCad;

  valorCad=atoi(cadena);
  return (valorCad<<24)>>24; //Se expande el signo
}


//recibe un operando de memoria y devuelve su valor
int valorMemoria(unsigned char memoria[], int tablaSeg[], int x)
{
  int aux=0,i,aux2=0;
  for(i=0;i<4;i++)
  {
    aux=memoria[posMemoria(tablaSeg,x & 0x000FFFFF)+i];
    aux2+=aux<<(8*(3-i));
  }
    return aux2;
}


//recibe un registro y devuelve su valor almacenado dependiendo de la seccion indicada en el operando
int valorRegistro(int registros[],int x)
{
  int aux1,sec_reg;

  aux1=registros[x & 0x0000000f];
  sec_reg= (x & 0x000000f0) >> 4; //Se calcula la seccion del registro a obtener
  if(sec_reg==0)
    return aux1;
  else
    if(sec_reg==1)
      return ((aux1 & 0x000000FF)<<24)>>24;
    else
      if(sec_reg==2)
        return ((aux1 & 0x0000FF00) << 16)>>24;
      else
        return ((aux1 & 0x0000FFFF)<<16)>>16;
}


int buscarMnemonico(char mnemonico[MNE], char x){
  int i=0;

  x=x&0x1F;
  while(i<MNE && x!=mnemonico[i])//buscarlo en el vector de mnemonicos
        i++;
  if(i<MNE)
    return i;
  else{
    printf("El codigo de operacion de la instruccion a ejecutar no existe.\n\n");
    printf("%d",5/0);
    return 0;
  }
}


//carga la siguiente istrucion en registros IP
void siguiente_instrucion(int tablaSeg[],int registros[])
{
    int aux;
    /* primero se posiciona donde apunta en la tabla de segmentos.
       luego le aplica una mascara a los dos primeros byte y luego lo corre 16 bits
       luego lo suma al offset de registro IP
       entonces aplico una mascara a registros IP si solo modifica el OffSET
       y se le suma uno al mismo*/
    aux=(tablaSeg[(registros[IP] & 0xFFFF0000)>>16] & 0xFFFF0000)>>16;
    aux+=registros[IP]&0x0000FFFF;
    registros[IP]=registros[IP]& 0xFFFF0000;
    registros[IP]+=aux+1;
}

void calculaOperandos(comp* componentes, int bandera, tamYOpe *A, tamYOpe *B, char instruc){
  if(bandera)
  {
    A->tam=((~instruc)&0x30)>>4;//obtengo los A->tamños de los operandos
    B->tam=((~instruc)&0xC0)>>6;
  }
  else //codigo de un operando
  {
    A->tam=((~instruc)&0xC0)>>6;
    B->tam=0;

  }
    if(A->tam<0)
      A->tam*=-1;

    if(B->tam<0)
      B->tam*=-1;

    A->OP=B->OP=0;
    for(int i=1;i<=B->tam;i++) //Se completa el int desde el byte menos significativo al mas significativo.
    {
      siguiente_instrucion(componentes->tablaSeg,componentes->registros);
      B->OP=B->OP<<8;
      B->OP+=0xFF & componentes->memoria[posMemoria(componentes->tablaSeg,componentes->registros[IP])]; //cargo los operando B en B->OP
    }

    for(int i=1;i<=A->tam;i++){
      siguiente_instrucion(componentes->tablaSeg,componentes->registros);
      A->OP=A->OP<<8;
      A->OP+=0xFF & componentes->memoria[posMemoria(componentes->tablaSeg,componentes->registros[IP])];//cargo el operando A en A->OP
    }
    siguiente_instrucion(componentes->tablaSeg,componentes->registros);//apunta a la siguiente instruccion
}


void disassebler(comp componentes, char mnemonico[MNE]){
  componentes.registros[CS]=0;
  componentes.registros[IP]=componentes.registros[CS];
  char cod_op=0, instruc;
  char espdefault[] = "   ";
  cad pal_mnemonico[MNE]={"MOV","ADD","SUB","SWAP","MUL","DIV","CMP","SHL","SHR","AND","OR","XOR","RND","SYS","JMP","JZ","JP","JN","JNZ","JNP","JNN","LDL","LDH","NOT","STOP"};
  cad registos[64]={"CS","DS","","","","IP","","","CC","AC","EAX","EBX","ECX","EDX","EEX","EFX","","","","","","","","","","","AL","BL","CL","DL","EL","FL","","","","","","","","","","","AH","BH","CH","DH","EH","FH","","","","","","","","","","","AX","BX","CX","DX","EX","FX"};
  tamYOpe A, B;
  int bandera,i,contEsp=0,posMeMIP=0;

  instruc=componentes.memoria[posMemoria(componentes.tablaSeg,componentes.registros[IP])];//se carga la istruccion en un char
  cod_op=mnemonico[buscarMnemonico(mnemonico,instruc)];
  while(cod_op != 0x1F && (posMeMIP < (componentes.tablaSeg[(componentes.registros[CS] & 0xFFFF0000)>>16] & 0x0000FFFF))){

    printf("[%04X] %02X",posMemoria(componentes.tablaSeg,componentes.registros[IP]),instruc & 0x000000FF);

    bandera=(cod_op & 0x10) == 0x00;

    if(bandera)
    {
      A.tam=((~instruc)&0x30)>>4;//obtengo los A->tamños de los operandos
      B.tam=((~instruc)&0xC0)>>6;
    }
    else //codigo de un operando
    {
      A.tam=((~instruc)&0xC0)>>6;
      B.tam=0;
    }

    if(A.tam<0)
      A.tam*=-1;

    if(B.tam<0)
      B.tam*=-1;

     A.OP=B.OP=0;
    for(i=1;i<=B.tam;i++) //Se completa el int desde el byte menos significativo al mas significativo.
    {
      siguiente_instrucion(componentes.tablaSeg,componentes.registros);
      printf(" %02X",(componentes.memoria[posMemoria(componentes.tablaSeg,componentes.registros[IP])]) & 0x000000FF);
      B.OP=B.OP<<8;
      B.OP+=0xFF & componentes.memoria[posMemoria(componentes.tablaSeg,componentes.registros[IP])]; //cargo los operando B en B->OP
    }

    for(i=1;i<=A.tam;i++){
      siguiente_instrucion(componentes.tablaSeg,componentes.registros);
      printf(" %02X",(componentes.memoria[posMemoria(componentes.tablaSeg,componentes.registros[IP])])& 0x000000FF);
      A.OP=A.OP<<8;
      A.OP+=0xFF & componentes.memoria[posMemoria(componentes.tablaSeg,componentes.registros[IP])];//cargo el operando A en A->OP
    }
    siguiente_instrucion(componentes.tablaSeg,componentes.registros);//apunta a la siguiente instruccion

    contEsp=7-(1+A.tam+B.tam); //Se calculan los espacios restantes para que tenga mejor visualizacion el dissasembler

    for(i=0;i<contEsp;i++)
      printf("   ");

    printf("%s |   ",espdefault);

    i=buscarMnemonico(mnemonico,instruc);

    printf(" %s   ",pal_mnemonico[i]);

    if(A.tam==3)
      printf("[%s+%d] ,",registos[(A.OP & 0x00FF0000)>>16], A.OP & 0x0000FFFF);
    else
      if(A.tam==1)
        printf("%s ,",registos[A.OP]);
      else
        if(A.tam==2)
          printf("%d",A.OP);


    if(B.tam==3)
      printf(" [%s+%d]",registos[(B.OP & 0x00FF0000)>>16], B.OP & 0x0000FFFF);
    else
      if(B.tam==2)
        printf(" %d",B.OP);
      else
        if(B.tam==1)
          printf(" %s",registos[B.OP]);

    printf("\n");

    posMeMIP=posMemoria(componentes.tablaSeg,componentes.registros[IP]);
    if((posMeMIP < (componentes.tablaSeg[(componentes.registros[CS] & 0xFFFF0000)>>16] & 0x0000FFFF))){
      instruc=componentes.memoria[posMemoria(componentes.tablaSeg,componentes.registros[IP])];
      cod_op=mnemonico[buscarMnemonico(mnemonico,instruc)];
    }
  }
}

void buscaArchs(char *nomArchVMX, int *d, int argc, char *argv[]){
  int i, longitudArch,longitudExt,j,k;
  char *extension = (char *) malloc(100*sizeof(char));
  char *cadenaD = (char *) malloc(100*sizeof(char));

  strcpy(extension,".vmx");
  strcpy(cadenaD,"-d");
  longitudExt=strlen(extension);

  for(i=1;i<argc;i++){
    longitudArch=strlen(argv[i]);
    j=longitudExt;
    k=longitudArch;
    while((*(argv[i]+k) == *(extension+j)) && *(extension+j)!= '.'){ //Se verifica que la extension de alguno de los parametros sea ".vmx"
      j--;
      k--;
    }

    if(*(extension+j)== '.')
      strcpy(nomArchVMX,argv[i]);

    if(strcmp(argv[i],cadenaD)==0)
      *d=1;
  }
}

///actua como CPU  leyendo cada istruccion y enviando a donde ejecutar dependiendo de cuantos operadores tiene
void cpu(comp* componentes, char mnemonico[MNE], func* mis_func[]){
  //Inicializamos memoria
  componentes->registros[CS]=0;
  componentes->registros[DS]=0x00010000;
  componentes->registros[IP]=componentes->registros[CS];
  char cod_op, instruc;
  tamYOpe A, B;
  int bandera,i,posMeMIP=0;

  instruc=componentes->memoria[posMemoria(componentes->tablaSeg,componentes->registros[IP])];//se carga la istruccion en un char
  cod_op=mnemonico[buscarMnemonico(mnemonico,instruc)];
  /*ejecuta la CPU hasta que encuentra una istruccion stop o el IP supera el CS*/
  while(cod_op != 0x1F && (posMeMIP < (componentes->tablaSeg[(componentes->registros[CS] & 0xFFFF0000)>>16] & 0x0000FFFF)))
  { //consultar si esta bien "mnemonico[buscarMnemonico()]!=0x1F"
      bandera=(cod_op & 0x10) == 0x00; //bandera= 1 si es un codigo de dos operandos, si no bendera=0;
      calculaOperandos(componentes,bandera,&A,&B,instruc);

      i=buscarMnemonico(mnemonico,cod_op);
      (*mis_func[i])(componentes,A,B);

      posMeMIP=posMemoria(componentes->tablaSeg,componentes->registros[IP]);
      if((posMeMIP < (componentes->tablaSeg[(componentes->registros[CS] & 0xFFFF0000)>>16] & 0x0000FFFF))){
        instruc=componentes->memoria[posMemoria(componentes->tablaSeg,componentes->registros[IP])];
        cod_op=mnemonico[buscarMnemonico(mnemonico,instruc)];
      }
  }
}
//componentes del pc arriba

int main(int argc, char *argv[])
{
  comp componentes;
  func* mis_func[MNE-1];
  char mnemonico[MNE]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1F}; //vector de nmemonicos
  int d=0;
  char *nomArchVMX = (char *) malloc(100*sizeof(char));

  FILE* archB=fopen("prueba.vmx","rb");

  if(cargaMemoria(&componentes,archB))
    printf("Memoria cargada con el archivo binario.\n\n");
  else{
    printf("Error en la carga de datos en memoria.\n\n");
    return 0;
  }

  mis_func[0]=&MOV;
  mis_func[1]=&ADD;
  mis_func[2]=&SUB;
  mis_func[3]=&SWAP;
  mis_func[4]=&MUL;
  mis_func[5]=&DIV;
  mis_func[6]=&CMP;
  mis_func[7]=&SHL;
  mis_func[8]=&SHR;
  mis_func[9]=&AND;
  mis_func[10]=&OR;
  mis_func[11]=&XOR;
  mis_func[12]=&RND;
  mis_func[13]=&SYS;
  mis_func[14]=&JMP;
  mis_func[15]=&JZ;
  mis_func[16]=&JP;
  mis_func[17]=&JN;
  mis_func[18]=&JNZ;
  mis_func[19]=&JNP;
  mis_func[20]=&JNN;
  mis_func[21]=&LDL;
  mis_func[22]=&LDH;
  mis_func[23]=&NOT;

  cpu(&componentes,mnemonico,mis_func);
  //disassebler(componentes,mnemonico);
 //printf("\n\n");

  /*strcpy(nomArchVMX,"");
  buscaArchs(nomArchVMX,&d,argc,argv);

  if(strcmp(nomArchVMX,"")!=0){
    FILE* archB=fopen(nomArchVMX,"rb");

    if(cargaMemoria(&componentes,archB))
      printf("Memoria cargada con el archivo binario.\n\n");
    else{
      printf("Error en la carga de datos en memoria.\n\n");
      return 0;
    }
  }
  else
    printf("Error en la extension del archivo.\n\n");

  if(d){
    disassebler(componentes,mnemonico);
    printf("\n\n");
  }
  else
    cpu(&componentes,mnemonico,mis_func);*/

  return 0;
}
