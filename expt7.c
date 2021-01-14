#include<stdio.h>
#include<string.h>
#include<stdlib.h>

//optable
char opcode[26][5]={"ADD", "AND", "COMP", "DIV", "J", "JEQ", "JGT", "JLT", "JSUB", "LDA", "LDCH", "LDL", "LDX", "MUL", "OR", "RD",
"RSUB", "STA", "STCH", "STL", "STSW", "STX", "SUB", "TD", "TIX", "WD"};
int code[26]={0x18, 0x40, 0x28, 0x24, 0x3C, 0x30, 0x34, 0x38, 0x48, 0x00, 0x50, 0x08, 0x04, 0x20, 0x44, 0xD8, 0x4C, 0x0C, 0x54,
0x14, 0xE8, 0x10, 0x1C, 0xE0, 0x2C, 0xDC};
//optable end
char prog_name[7];
int starting_address=0;
int execution_adress=0;
int end_address=0;
int ifOpcodeExist(char* inst) { //For returning if opcode exist, if doesnot exist return 0xFF
  int beg=0;
  int mid=12;
  int end=25;
  int flag=0;
  while(beg<=end) {
    flag=strcmp(inst,opcode[mid]);
    if(flag==0)
      return code[mid];
    else {
      if(flag<0) {
        end=mid-1;
        mid=(beg+end)/2;
      }
      else {
        beg=mid+1;
        mid=(beg+end)/2;
      }
    }
  }
  return 0xFF;
}

typedef struct entry{ //Symbol Table Each Row
  char symbol[20];
  int address;
  struct entry* next;
}symtabentry;

symtabentry *start; //Symbol Table

void displaySymtab() {
  symtabentry *ptr=start;
  while(ptr!=NULL) {
    printf("%04x %s\n",ptr->address,ptr->symbol);
    ptr=ptr->next;
  }
}

int findSymbol(char* symbol) { //return address stored in symbol table
  symtabentry* ptr=start;
  while(ptr!=NULL) {
    if(strcmp(symbol,ptr->symbol)==0) {
      return ptr->address;
    }

    ptr=ptr->next;
  }
  return 0xFFFF;
}

int addSymbol(char* symbol, int address) { //Adding Rows to Symbol Table
  if(findSymbol(symbol)==0xFFFF) {
    symtabentry* row = (symtabentry*)malloc(sizeof(symtabentry));
    strcpy(row->symbol,symbol);
    row->address=address;
    row->next=NULL;
    if(start==NULL)
      start=row;
    else {
      symtabentry* ptr=start;
      while(ptr->next!=NULL)
        ptr=ptr->next;
      ptr->next=row;
    }
    return 1;
  }
  else {
    return 0;
  }

}



int countByte(char* operand) { //incrementing locctr for byte
  int len=strlen(operand);
  if(operand[0]=='C')
    return len-3;
  else
    return (len-3)/2;
}

int countResw(char* operand) { //incrementing locctr for resw
  int val=0;
  int len=strlen(operand);
  if(operand[len-1]=='\n') {
    operand[len-1]='\0';
    len--;
  }
  for(int i=0;i<len;i++) {
    val*=10;
    val+=(operand[i]-'0');
  }
  return val*3;
}

int countResb(char* operand) {//incrementing locctr for resw
  int val=0;
  int len=strlen(operand);
  for(int i=0;i<len;i++) {
    val*=10;
    val+=(operand[i]-'0');
  }
  return val;
}

int toInt(char* operand) { // get locctr for START
  int val=0;
  int temp=0;
  int len=strlen(operand);
  for(int i=0;i<len;i++) {
    val*=10;
    val+=(operand[i]-'0');
  }
  return val;
}
int toHex(char* operand) {
  int val=0;
  int temp=0;
  int len=strlen(operand);
  for(int i=0;i<len;i++) {
    val*=16;
    temp=(operand[i]-'0');
    if(temp>=10) {
      temp=10+operand[i]-'a';
    }
    val+=temp;
  }
  return val;
}
int pass1() {
  FILE *fpprog, *fpintr;
  char stmt[40];
  char instr[20];
  char label[8];
  char operand[20];
  int i=0;
  int k=0;
  int opcodevalue=0xFF;
  int wordcount=0;
  int locctr = 0; // Location counter
  int errorflag=0; // to know if already error has occured
  int pc=0; //program counter
  int successpass1=1;
  addSymbol("",0x0000);
  fpprog=fopen("prog.txt","r");
  fpintr=fopen("intr.txt","w");

  if(fpprog==NULL) {
    printf("Cannot find the program\n");
  }
  else {
    while(fgets(stmt,40,fpprog)) {
      errorflag=0;
      strcpy(label,"");
      strcpy(instr,"");
      strcpy(operand,"");
      wordcount=0;
      i=0;
      k=0;
      //Finding label opcode operand
      while(stmt[i]==' ')
        i++;
      for(;(stmt[i]!=' ')&&(stmt[i]!=';');i++) {
        label[k]=stmt[i];
        k++;
        if(k==7)
          break;
      }
      label[k]='\0';
      if(label[k-1]=='\n') {
        label[k-1]='\0';
        k--;
      }

      k=0;
      while(stmt[i]==' ')
        i++;
      for(;(stmt[i]!=' ')&&(stmt[i]!=';');i++) {
        instr[k]=stmt[i];
        k++;
        if(k==7)
          break;
      }
      instr[k]='\0';
      if(instr[k-1]=='\n') {
        instr[k-1]='\0';
        k--;
      }

      k=0;
      while(stmt[i]==' ')
        i++;
      for(;(stmt[i]!=' ')&&(stmt[i]!=';');i++) {
        operand[k]=stmt[i];
        k++;
        if(k==7)
          break;
      }
      operand[k]='\0';
      if(operand[k-1]=='\n') {
        operand[k-1]='\0';
        k--;
      }

      //finding number of words
      if(strcmp(instr,"")!=0)
        wordcount++;
      if(strcmp(operand,"")!=0)
        wordcount++;
      if(strcmp(label,"")!=0)
        wordcount++;

      //assigning label, opcode and operand

      if(wordcount==1) {//then RSUB
        if(strcmp(label,"RSUB")==0) {
          strcpy(instr,label);
          strcpy(label,"");
        }
        else {
          fprintf(fpintr,"Error - Operand missing;\n");
          errorflag=1;
          successpass1=0;
        }
      }
      else {
        if(wordcount==2) {
          if((strcmp(label,"RSUB")!=0)&&strcmp(instr,"RSUB")!=0) { //rejecting RSUB OPERAND/ LABEL RSUB
            strcpy(operand,instr);
            strcpy(instr,label);
            strcpy(label,"");
            printf("%04x %s %s %s\n",locctr,label,instr,operand);
          }
          else { //else of (strcmp(label,"RSUB")!=0)&&strcmp(instr,"RSUB")!=0
            if(strcmp(instr,"RSUB")!=0) { //rejecting RSUB OPERAND
              fprintf(fpintr,"Error - RSUB shouldnot have any operand;\n");
              errorflag=1;
              successpass1=0;
            }
          }
        }
        else {//else of wordcount==2
          if(wordcount==3 && strcmp(instr,"RSUB")==0) { // Rejecting label RSUB operand
            fprintf(fpintr,"Error - RSUB shouldnot have any operand;\n");
            errorflag=1;
            successpass1=0;
          }
        }
      }
      if(errorflag==0) {//If no error

        opcodevalue=ifOpcodeExist(instr);
        if(opcodevalue==0xFF) { //psuedo codes / error
          if(wordcount==3) {
            if(strcmp(instr,"WORD")==0) {
              if(addSymbol(label,locctr)==0) {
                fprintf(fpintr,"Error - Label already exist;\n");
                successpass1=0;
              }
              else {
                fprintf(fpintr,"%04x %s %s %s;\n",locctr,label, instr, operand);
                locctr+=3;
              }

            }
            else {
              if(strcmp(instr,"BYTE")==0) {
                if(addSymbol(label,locctr)==0) {
                  fprintf(fpintr,"Error - Label already exist;\n");
                  successpass1=0;
                }
                else {
                  fprintf(fpintr,"%04x %s %s %s;\n",locctr,label, instr, operand);
                  locctr+=countByte(operand);
                }

              }
              else {
                if(strcmp(instr,"RESW")==0) {
                  if(addSymbol(label,locctr)==0) {
                    fprintf(fpintr,"Error - Label already exist;\n");
                    successpass1=0;
                  }
                  else {
                    fprintf(fpintr,"%04x %s %s %s;\n",locctr,label, instr, operand);
                    locctr+=countResw(operand);
                  }
                }
                else {
                  if(strcmp(instr,"RESB")==0) {
                    if(addSymbol(label,locctr)==0) {
                      fprintf(fpintr,"Error - Label laready exist;\n");
                      successpass1=0;
                    }
                    else {
                      fprintf(fpintr,"%04x %s %s %s;\n",locctr,label, instr, operand);
                      locctr+=countResb(operand);
                    }
                  }
                  else {
                    if(strcmp(instr,"START")==0) {
                      strcpy(prog_name,label);
                      locctr=toInt(operand);
                    }
                    else {
                      fprintf(fpintr,"Error - unrecongised instruction;\n");
                      successpass1=0;
                    }
                  }
                }
              }
            }
          }
          else { //else of if(wordcount==3)
            if(wordcount==2) {
              if(strcmp(instr,"RSUB")==0) {
                if(addSymbol(label,locctr)==0) {
                  fprintf(fpintr,"Error - Label already exist;\n");
                  successpass1=0;
                }
                else {
                  fprintf(fpintr,"%04x %s %s ;\n",locctr,label, instr);
                  locctr+=3;
                }
              }
              else {
                if(strcmp(instr,"END")==0) {
                  execution_adress=toInt(operand);
                }
                else {
                  fprintf(fpintr,"Error - unrecongnised instruction;\n");
                  successpass1=0;
                }
              }

            }
            else {
              fprintf(fpintr,"Error - unrecongnised instruction;;\n");
              successpass1=0;
            }
          }

        }
        else { //else of opcodevalue==0xFF
          if(strcmp(label,"")!=0) {
            if(addSymbol(label,locctr)==0) {
              fprintf(fpintr,"Error - Symbol already exist;\n");
              successpass1=0;
              locctr+=3;
            }
            else { //else of if(addSymbol(label,locctr)==0
              fprintf(fpintr,"%04x %s %s %s;\n",locctr,label, instr, operand); //instr is a valid opcode
              locctr+=3;
            }
          }
          else {
            fprintf(fpintr,"%04x %s %s %s;\n",locctr,label, instr, operand); //instr is a valid opcode
            locctr+=3;
          }
        }
      }

    }
  }
  fclose(fpprog);
  fclose(fpintr);
  end_address=locctr;
  return successpass1;
}


int isIndexed(char* operand) { //To check if indexed addressing mode iis present
  if(strcmp(operand,"")!=0) {

    int len=strlen(operand);
    for(int i=0;i<len-1;i++) {
      if(operand[i]==',') {
        if(operand[i+1]=='X') {
          return 1;
        }
      }
    }

  }
  return 0;
}

char* getBytes(char* operand,char *temp) { //To get the hexa codes to constants
  int i=2;
  int j=0;
  char str[3];
  if(operand[0]=='C') {
    i=2;
    while(operand[i]!='\'') {
      sprintf(str,"%02x",operand[i]);
      strcat(temp,str);
      i++;
    }
  }
  else {
    i=2;
    while(operand[i]!='\'') {
      temp[j]=operand[i];
      j++;
      i++;
    }
    temp[j]='\0';
  }
  return temp;
}

void pass2() {
  FILE *fpintr, *fpfinal;
  char stmt[41];
  int pc;
  int i=0;
  int k=0;
  int wordcount=0;
  char str_addr[5];
  int addr=0;
  char instr[8];
  char label[8];
  char operand[8];
  char text_record[71];
  int text_record_ch_count=0;
  int operand_address=0;
  int text_record_length=0;



  fpintr=fopen("intr.txt","r");
  fpfinal=fopen("final.txt","w");

  fgets(stmt,40,fpintr); //getting the address of each statement
  while(stmt[i]!=' ') {
    str_addr[i]=stmt[i];
    i++;
  }
  str_addr[i]='\0';
  while(strlen(prog_name)!=6) {
    strcat(prog_name," ");
  }
  fprintf(fpfinal,"H|%s|%s|%04x|\n",prog_name,str_addr,end_address-starting_address);
  fseek(fpintr,0,0);

  strcpy(text_record,"T|"); // creating first text record
  char temp[5];
  strcat(text_record,str_addr);

  strcat(text_record,"|__|");
  text_record_ch_count+=10;


  while(fgets(stmt,40,fpintr)) {
    if(stmt[strlen(stmt)-1]=='\n')
      stmt[strlen(stmt)-1]='\0';
    strcpy(label,"");
    strcpy(instr,"");
    strcpy(operand,"");
    wordcount=0;

    i=0;
    k=0;
    //Finding label opcode operand
    while(stmt[i]!=' ') {
      str_addr[i]=stmt[i];
      i++;
    }
    str_addr[i++]='\0';
    addr=toHex(str_addr);
    k=0;
    if(stmt[i]!=' ') {
      for(;(stmt[i]!=' ');i++) {
        label[k]=stmt[i];
        k++;
      }
    }

    label[k]='\0';
    if(label[k-1]=='\n') {
      label[k-1]='\0';
      k--;
    }

    k=0;
    i++;
    for(;(stmt[i]!=' ')&&(stmt[i]!=';');i++) {
      instr[k]=stmt[i];
      k++;
    }
    instr[k]='\0';
    if(instr[k-1]=='\n') {
      instr[k-1]='\0';
      k--;
    }

    k=0;
    i++;

    if(stmt[i]!=' ') {
      for(;(stmt[i]!=';');i++) {
        operand[k]=stmt[i];
        k++;
      }
    }
    operand[k]='\0';
    if(operand[k-1]=='\n') {
      operand[k-1]='\0';
      k--;
    }
    operand_address=0;
    if(isIndexed(operand)) { //if indexed adressing mode is present
      int j=0;
      while(operand[j]!=',')
        j++;
      operand[j]='\0';
      operand_address+=0x8000;
    }
    operand_address+=findSymbol(operand);

    //finding number of words
    if(strcmp(instr,"")!=0)
      wordcount++;
    if(strcmp(operand,"")!=0)
      wordcount++;
    if(strcmp(label,"")!=0)
      wordcount++;

    if(ifOpcodeExist(instr)!=0xFF) { // if valid opcode
      if(text_record_ch_count+6>70) {
        char temp[5];
        sprintf(temp,"%02x",(text_record_length/2));//adding length
        text_record[7]=temp[0];
        text_record[8]=temp[1];
        fprintf(fpfinal,"%s\n",text_record);
        strcpy(text_record,"T|");
        /*
        if(text_record[2]=='f'&&text_record[3]=='f'&&text_record[4]=='f'&&text_record[5]=='f') {
          char temp_2[5];
          sprintf(temp,"%04x",addr);
          text_record[2]=temp_2[0];
          text_record[3]=temp_2[1];
          text_record[4]=temp_2[2];
          text_record[5]=temp_2[3];
        }*/
        //sprintf(temp,"%04x",addr);
        //strcat(text_record,temp);
        strcat(text_record,str_addr);
        strcat(text_record,"|__|");
        text_record_ch_count=10;
      }

      char temp[5];
      if(text_record[2]=='f'&&text_record[3]=='f'&&text_record[4]=='f'&&text_record[5]=='f') { // used to assign starting address of text record
        sprintf(temp,"%04x",addr);
        text_record[2]=temp[0];
        text_record[3]=temp[1];
        text_record[4]=temp[2];
        text_record[5]=temp[3];
      }
      sprintf(temp,"%02x",ifOpcodeExist(instr));
      strcat(text_record,temp);
      sprintf(temp,"%04x",operand_address);
      strcat(text_record,temp);
      strcat(text_record,"|");
      text_record_ch_count+=7;
      text_record_length+=6;


    }
    else {
      if(strcmp(instr,"WORD")==0) {
        if(text_record_ch_count+6>70) {
          char temp[5];
          sprintf(temp,"%02x",(text_record_length/2));
          text_record[7]=temp[0];
          text_record[8]=temp[1];
          fprintf(fpfinal,"%s\n",text_record);
          strcpy(text_record,"T|");
          //sprintf(temp,"%04x",addr);
          //strcat(text_record,temp);
          strcat(text_record,str_addr);
          strcat(text_record,"|__|");
          text_record_ch_count=10;
          text_record_length=0;
        }

        char temp[7];
        if(text_record[2]=='f'&&text_record[3]=='f'&&text_record[4]=='f'&&text_record[5]=='f') {
          //char temp_2[5];
          sprintf(temp,"%04x",addr);
          text_record[2]=temp[0];
          text_record[3]=temp[1];
          text_record[4]=temp[2];
          text_record[5]=temp[3];
        }
        sprintf(temp,"%06x",toInt(operand));
        strcat(text_record,temp);
        strcat(text_record,"|");
        text_record_ch_count+=7;
        text_record_length+=6;

      }
      else {
        if(strcmp(instr,"BYTE")==0) {
          strcpy(temp,"");
          getBytes(operand,temp);
          if(text_record_ch_count+strlen(temp)>70) {
            char temp[5];
            sprintf(temp,"%02x",(text_record_length/2)); //adding length of text record
            text_record[7]=temp[0];
            text_record[8]=temp[1];
            fprintf(fpfinal,"%s\n",text_record);
            strcpy(text_record,"T|");
            //sprintf(temp,"%04x",addr);
            //strcat(text_record,temp);
            strcat(text_record,str_addr);
            strcat(text_record,"|__|");
            text_record_ch_count=10;
            text_record_length=0;
          }
          if(text_record[2]=='f'&&text_record[3]=='f'&&text_record[4]=='f'&&text_record[5]=='f') {
            //char temp_2[5];
            sprintf(temp,"%04x",addr);
            text_record[2]=temp[0];
            text_record[3]=temp[1];
            text_record[4]=temp[2];
            text_record[5]=temp[3];
          }
          printf("HE before %s\n",text_record);
          strcat(text_record,temp);
          strcat(text_record,"|");
          text_record_ch_count+=(strlen(temp)+1);
          text_record_length+=strlen(temp);
          printf("HE after %s\n",text_record);
        }
        else {//if RESW or RESB is encountered finish the text record and add it to file
          if(text_record_ch_count>10) { //if text record exist
            char temp[5];
            sprintf(temp,"%02x",(text_record_length/2));
            text_record[7]=temp[0];
            text_record[8]=temp[1];
            fprintf(fpfinal,"%s\n",text_record);
            strcpy(text_record,"T|");

            //sprintf(temp,"%04x",0xFFFF);
            strcat(text_record,"ffff");
            strcat(text_record,"|__|");
            text_record_ch_count=10;
            text_record_length=0;
          }
        }
      }
    }

  }
  if(text_record_ch_count>10){//if text record exist

    char temp[5];
    sprintf(temp,"%02x",(text_record_length/2));
    text_record[7]=temp[0];
    text_record[8]=temp[1];
    fprintf(fpfinal,"%s\n",text_record);
  }
  fprintf(fpfinal,"E|%04x|",execution_adress);
  fclose(fpintr);
  fclose(fpfinal);
}

void main() {
  start=NULL; // initialise global symbol table
  int successpass1=pass1();
  if(successpass1=1) {
    pass2();
  }
  printf("displayTable\n");
  displaySymtab();
}
