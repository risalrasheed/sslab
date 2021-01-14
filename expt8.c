#include<stdio.h>
#include<string.h>
#include<stdlib.h>

char prog_name[7];
int starting_address=0;
int execution_address=0;
int end_address=0;

struct OperandTableEntry {
  char opcode[7];
  int code;
  int no_operands;
  int no_registers;
  int format;
  //number of non-operand operands = no_operands-no_register
}OpTab[59];
int optabpointer=-1;

struct RegisterBank {
  char register_name;
  int register_code;
}registers[9];

void createRegisterBank() {
  registers[0].register_name='A';
  registers[0].register_code=0;

  registers[1].register_name='X';
  registers[1].register_code=1;

  registers[2].register_name='L';
  registers[2].register_code=2;

  registers[3].register_name='B';
  registers[3].register_code=3;

  registers[4].register_name='S';
  registers[4].register_code=4;

  registers[5].register_name='T';
  registers[5].register_code=5;

  registers[6].register_name='F';
  registers[6].register_code=6;

  registers[7].register_name='P'; //for PC
  registers[7].register_code=8;

  registers[8].register_name='W'; //for SW
  registers[8].register_code=9;


}

int getRegisterNumber(char register_name) {
  for(int i=0;i<=9;i++) {
    if(registers[i].register_name=register_name) {
      return registers[i].register_code;
    }
  }
  return -1;
}

void addToOpTab(char* opcode, int code, int format, int no_operands, int no_registers) {
  strcpy(OpTab[++optabpointer].opcode,opcode);
  OpTab[optabpointer].code=code;
  OpTab[optabpointer].no_operands=no_operands;
  OpTab[optabpointer].no_registers=no_registers;
  OpTab[optabpointer].format=format;
}
void createOpTtab() {
  addToOpTab("ADD",0x18,3,1,0);
  addToOpTab("ADDF",0X58,3,1,0);
  addToOpTab("ADDR",0X90,2,2,2);
  addToOpTab("AND",0x40,3,1,0);
  addToOpTab("CLEAR",0XB4,2,1,1);
  addToOpTab("COMP",0x28,3,1,0);
  addToOpTab("COMPF",0X88,3,1,0);
  addToOpTab("COMPR",0XA0,2,2,2);
  addToOpTab("DIV",0x24,3,1,0);
  addToOpTab("DIVF",0X64,3,1,0);
  addToOpTab("DIVR",0X9C,2,2,2);
  addToOpTab("FIX",0XC4,1,0,0);
  addToOpTab("FLOAT",0XC0,1,0,0);
  addToOpTab("HIO",0XF4,1,0,0);
  addToOpTab("J",0x3C,3,1,0);
  addToOpTab("JEQ",0x30,3,1,0);
  addToOpTab("JGT",0x34,3,1,0);
  addToOpTab("JLT",0x38,3,1,0);
  addToOpTab("JSUB",0x48,3,1,0);
  addToOpTab("LDA",0x00,3,1,0);
  addToOpTab("LDB",0X68,3,1,0);
  addToOpTab("LDCH",0x50,3,1,0);
  addToOpTab("LDF",0X78,3,1,0);
  addToOpTab("LDL",0x08,3,1,0);
  addToOpTab("LDS",0X6C,3,1,0);
  addToOpTab("LDT",0X74,3,1,0);
  addToOpTab("LDX",0x04,3,1,0);
  addToOpTab("LPS",0XD0,3,1,0);
  addToOpTab("MUL",0x20,3,1,0);
  addToOpTab("MULF",0X60,3,1,0);
  addToOpTab("MULR",0X98,2,2,2);
  addToOpTab("NORM",0XC8,1,0,0);
  addToOpTab("OR",0x44,3,1,0);
  addToOpTab("RD",0xD8,3,1,0);
  addToOpTab("RMO",0XAC,2,2,2);
  addToOpTab("RSUB",0x4C,3,0,0);
  addToOpTab("SHIFTL",0XA4,2,2,2);
  addToOpTab("SHIFTR",0XA8,2,2,2);
  addToOpTab("SIO",0XF0,1,0,0);
  addToOpTab("SSK",0XEC,3,1,0);
  addToOpTab("STA",0x0C,3,1,0);
  addToOpTab("STB",0X78,3,1,0);
  addToOpTab("STCH",0x54,3,1,0);
  addToOpTab("STF",0X80,3,1,0);
  addToOpTab("STI",0XD4,3,1,0);
  addToOpTab("STL",0x14,3,1,0);
  addToOpTab("STS",0X7C,3,1,0);
  addToOpTab("STSW",0xE8,3,1,0);
  addToOpTab("STT",0X84,3,1,0);
  addToOpTab("STX",0x10,3,1,0);
  addToOpTab("SUB",0x1C,3,1,0);
  addToOpTab("SUBF",0X5C,3,1,0);
  addToOpTab("SUBR",0X94,2,2,2);
  addToOpTab("SVC",0XB0,2,1,1);
  addToOpTab("TD",0xE0,3,1,0);
  addToOpTab("TIO",0XF8,1,0,0);
  addToOpTab("TIX",0XB8,3,1,0);
  addToOpTab("TIXR",0x2C,2,1,1);
  addToOpTab("WD",0xDC,3,1,0);
}

//optable end


int ifOpcodeExist(char* instr) { //For returning if opcode exist, if doesnot exist return 0xFF
  int beg=0;
  int mid=29;
  int end=59;
  int flag=0;
  int i=0;
  int j=0;
  char temp_instr[8];
  if(instr[i]=='+') {
    i++;
  }
  while(instr[i]!='\0')
    temp_instr[j++]=instr[i++];

  temp_instr[j]='\0';
  while(beg<=end) {
    flag=strcmp(temp_instr,OpTab[mid].opcode);
    if(flag==0) {
      return mid;
    }

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

  return -1;
}

typedef struct entry{ //Symbol Table Each Row
  char symbol[20];
  int address;
  int relative;
  struct entry* next;
}symtabentry;

symtabentry *start; //Symbol Table

void displaySymtab() {
  symtabentry *ptr=start;
  while(ptr!=NULL) {
    printf("%04x %s %d\n",ptr->address,ptr->symbol,ptr->relative);
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
  return 0xFFFFFF;
}

int addSymbol(char* symbol, int address, int relative) { //Adding Rows to Symbol Table

  if(findSymbol(symbol)==0xFFFFFF) {
    symtabentry* row = (symtabentry*)malloc(sizeof(symtabentry));
    strcpy(row->symbol,symbol);
    row->address=address;
    row->next=NULL;
    row->relative=relative;
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

int pass1() { //instruct format label opcode operand1,X operand2;
  FILE *fpprog, *fpintr;
  char stmt[40];
  char label[8];
  char instr[8];
  char operand1[20];
  char operand2[20];
  int locctr=0;
  int isIndexed=0;
  int i=0;
  int k=0;
  int opcode_number=-1;
  int success=1;
  char c;


  fpprog=fopen("prog1.txt","r");
  fpintr=fopen("intr1.txt","w");
  if(fpprog==NULL) {
    printf("Cannot find the program\n");
  }
  else {

    addSymbol("",0x000000,0);
    while(fgets(stmt,40,fpprog)) {
      i=0;
      k=0;

      if(stmt[strlen(stmt)-1]=='\n')
        stmt[strlen(stmt)-1]='\0';


      while(stmt[i]!=' ') {
        label[k]=stmt[i];
        k++;
        i++;
      }
      label[k]='\0';
      i++;
      k=0;

      while(stmt[i]!=' ') {
        instr[k]=stmt[i];
        k++;
        i++;
      }
      instr[k]='\0';
      i++;
      k=0;

      while(stmt[i]!=' '&&stmt[i]!=',') {
        operand1[k]=stmt[i];
        k++;
        i++;
      }
      operand1[k]='\0';
      if(stmt[i]==',') {
        isIndexed=1;
        i+=2;
      }
      i++;
      k=0;

      while(stmt[i]!=';') {
        operand2[k]=stmt[i];
        k++;
        i++;
      }
      operand2[k]='\0';
      i++;


      //checking instr
      opcode_number = ifOpcodeExist(instr);
      if(opcode_number!=-1) {
        if(strcmp(label,"")!=0) {
          addSymbol(label,locctr,1);
        }
        fprintf(fpintr,"%06x %s\n",locctr,stmt);
        locctr+=OpTab[opcode_number].format;
        if(instr[0]=='+')
          locctr++;

      }
      else {
        if(strcmp(instr,"WORD")==0) {
          if(addSymbol(label,locctr,1)==1) {
            fprintf(fpintr,"%06x %s\n",locctr,stmt);
          }
          else {
            fprintf(fpintr,"Error - Label name already exist;");
            success=0;
          }
          locctr+=3;
        }
        else {
          if(strcmp(instr,"BYTE")==0) {
            if(addSymbol(label,locctr,1)==1) {
              fprintf(fpintr,"%06x %s\n",locctr,stmt);
            }
            else {
              fprintf(fpintr,"Error - Label name already exist;");
              success=0;
            }
            locctr+=countByte(operand1);
          }
          else {
            if(strcmp(instr,"RESW")==0) {
              if(addSymbol(label,locctr,1)==1) {
                fprintf(fpintr,"%06x %s\n",locctr,stmt);
              }
              else {
                fprintf(fpintr,"Error - Label name already exist;");
                success=0;
              }
              locctr+=countResw(operand1);
            }
            else {
              if(strcmp(instr,"RESB")==0) {
                if(addSymbol(label,locctr,1)==1) {
                  fprintf(fpintr,"%06x %s\n",locctr,stmt);
                }
                else {
                  fprintf(fpintr,"Error - Label name already exist;");
                  success=0;
                }
                locctr+=countByte(operand1);
              }
              else {
                if(strcmp(instr,"BASE")==0) {
                  fprintf(fpintr,"%06x %s\n",locctr,stmt);

                }
                else {
                  if(strcmp(instr,"EQU")==0) {
                    int equ_val=0;
                    if(operand1[0]-'0'<10) {
                      equ_val=toInt(operand1);
                    }
                    else {
                      equ_val=findSymbol(operand1);
                    }
                    if(addSymbol(label,equ_val,0)==1) {
                      fprintf(fpintr,"%06x %s\n",locctr,stmt);
                    }
                    else {
                      fprintf(fpintr,"Error - Label name already exist;");
                      success=0;
                    }
                  }
                  else {
                    if(strcmp(instr,"START")==0) {
                      locctr=toInt(operand1);
                      starting_address=locctr;
                      strcpy(prog_name,label);
                    }
                    else {
                      if(strcmp(instr,"END")==0) {
                        end_address=locctr;
                        execution_address=toInt(operand1);
                      }
                      else {
                        fprintf(fpintr,"Error - unidentified command;\n");
                        success=0;
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
  }
  fclose(fpintr);
  fclose(fpprog);
  return success;
}

char* getBytes(char* operand,char *temp) { //To get the hexa codes to constants
  int i=2;
  int j=0;
  char str[3];
  strcpy(temp,"");
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

int pass2() {
  FILE *fpintr, *fpfinal;
  char stmt[40];
  char label[8];
  char instr[8];
  char operand1[20];
  char operand2[20];
  char str_addr[7];
  int isIndexed=0;
  int i=0;
  int k=0;
  int opcode_number=-1;
  char text_record[79];
  char temp[10];
  int text_record_ch_count=0;
  int operand_address=0;
  int text_record_length=0;
  int base_register=0;
  char c;
  fpintr=fopen("intr1.txt","r");
  fpfinal=fopen("final1.txt","w");

  fprintf(fpfinal,"H|%s|%06x|%06x|\n",prog_name,starting_address,(end_address-starting_address));
  sprintf(text_record,"T|%06x|__|",starting_address);

  text_record_ch_count+=12;
  while(fgets(stmt,40,fpintr)) {
    isIndexed=0;
    if(stmt[strlen(stmt)-1]=='\n')
      stmt[strlen(stmt)-1]='\0';

    i=0;
    k=0;
    while(stmt[i]!=' ') {
      str_addr[k++]=stmt[i++];
    }
    str_addr[k]='\0';
    i++;
    k=0;
    while(stmt[i]!=' ') {
      label[k++]=stmt[i++];
    }
    label[k]='\0';
    i++;
    k=0;
    while(stmt[i]!=' ') {
      instr[k++]=stmt[i++];
    }
    instr[k]='\0';
    i++;
    k=0;
    while(stmt[i]!=' ' && stmt[i]!=',') {
      operand1[k++]=stmt[i++];
    }
    operand1[k]='\0';
    if(stmt[i]==',') {
      isIndexed=1;
      i+=2;
    }
    i++;
    k=0;
    while(stmt[i]!=';') {
      operand2[k++]=stmt[i++];
    }
    operand2[k]='\0';

    int opcode_num=ifOpcodeExist(instr);
    if(opcode_num!=-1) {
      if(OpTab[opcode_num].format==1) {
        if(text_record_ch_count+3>78){
          sprintf(temp,"%02x",text_record_length/2);
          text_record[9]=temp[0];
          text_record[10]=temp[1];
          fprintf(fpfinal,"%s\n",text_record);
          sprintf(text_record,"T|%s|__|",str_addr);
          text_record_ch_count=12;
          text_record_length=0;
        }
        sprintf(temp,"%02x|",OpTab[opcode_num].code);
        strcat(text_record,temp);
        text_record_ch_count+=3;
        text_record_length+=2;
      }
      else {
          if(OpTab[opcode_num].format==2) {
            if(text_record_ch_count+5>78){
              sprintf(temp,"%02x",text_record_length/2);
              text_record[9]=temp[0];
              text_record[10]=temp[1];
              fprintf(fpfinal,"%s\n",text_record);
              sprintf(text_record,"T|%s|__|",str_addr);
              text_record_ch_count=12;
              text_record_length=0;
            }
            sprintf(temp,"%02x",OpTab[opcode_num].code);
            strcat(text_record,temp);
            if(OpTab[opcode_num].no_operands==1) {
              if(OpTab[opcode_num].no_registers==1) {
                sprintf(temp,"%01x0|",getRegisterNumber(operand1[0]));
                strcat(text_record,temp);
              }
            }
            else {
              if(OpTab[opcode_num].no_operands==2) {
                if(OpTab[opcode_num].no_registers==2) {
                  sprintf(temp,"%01x%01x|",getRegisterNumber(operand1[0]),getRegisterNumber(operand2[0]));
                  strcat(text_record,temp);
                }

              }
            }
            text_record_ch_count+=5;
            text_record_length+=4;
          }
          else {
              if(OpTab[opcode_num].format==3) {
                if(text_record_ch_count+3>78){
                  sprintf(temp,"%02x",text_record_length/2);
                  text_record[9]=temp[0];
                  text_record[10]=temp[1];
                  fprintf(fpfinal,"%s\n",text_record);
                  sprintf(text_record,"T|%s|__|",str_addr);
                  text_record_ch_count=12;
                  text_record_length=0;
                }
                if(instr[0]=='+') { //type 4
                  if(text_record_ch_count+4>78){
                    sprintf(temp,"%02x",text_record_length/2);
                    text_record[9]=temp[0];
                    text_record[10]=temp[1];
                    fprintf(fpfinal,"%s\n",text_record);
                    sprintf(text_record,"T|%s|__|",str_addr);
                    text_record_ch_count=12;
                    text_record_length=0;
                  }
                  int add_ni=3;
                  int add_xbpe=1;
                  int add_addr=0;
                  char remove_hash[20];
                  if(operand1[0]=='#') {
                    add_ni=1;
                    for(int j=1;1;j++) {
                      remove_hash[j-1]=operand1[j];
                      if(operand1[j]=='\0')
                        break;
                    }
                    add_addr=toHex(remove_hash);
                  }
                  else {
                    if(operand1[0]=='@') {
                      add_ni=2;
                      add_addr=findSymbol(operand1);
                    }
                    else {
                      if(isIndexed==1)
                        add_xbpe+=8;
                        add_addr=findSymbol(operand1);
                    }
                  }

                  sprintf(temp,"%02x%01x%05x|",OpTab[opcode_num].code+add_ni,add_xbpe,add_addr);
                  strcat(text_record,temp);
                  text_record_ch_count+=9;
                  text_record_length+=8;
                }
                else {

                  int add_ni=3;
                  int add_xbpe=0;
                  int disp=0;
                  if(operand1[0]=='#') {
                    add_ni=1;
                    if(operand1[1]-'0'>=10) {
                      add_xbpe=2;
                    }
                  }
                  else {
                    if(operand1[0]=='@') {
                      add_ni=2;
                    }
                  }
                  if(add_ni!=1) {
                    if(isIndexed==1)
                      add_xbpe+=8;
                    //sprintf(temp,"%03x",findSymbol())
                    if(strcmp(instr,"RSUB")==0) {
                      disp=0;
                      add_xbpe=-2; //to cancel the 2 that is added in if(disp>=-248 && disp<248)
                    }
                    else {
                        disp=findSymbol(operand1)-(toHex(str_addr)+3);
                    }

                    printf("disp %03x %s\n",disp,operand1);
                    if(disp>=-248 && disp<248) {
                      add_xbpe+=2;
                      sprintf(temp,"%02x%01x%03x|",OpTab[opcode_num].code+add_ni,add_xbpe,disp);

                      strcat(text_record,temp);
                      text_record_ch_count+=7;
                      text_record_length+=6;
                    }
                    else {
                      disp=findSymbol(operand1) - base_register;
                      if(disp>=0&&disp<4096) {
                        add_xbpe+=4;
                        sprintf(temp,"%02x%01x%03x|",OpTab[opcode_num].code+add_ni,add_xbpe,disp);
                        strcat(text_record,temp);
                        text_record_ch_count+=7;
                        text_record_length+=6;
                      }
                      else {

                        strcat(text_record,"Error|");
                        text_record_ch_count+=7;
                        text_record_length+=6;
                      }
                    }

                  }
                  else {

                    //sprintf(temp,"%03x",findSymbol())
                    char remove_hash[20];
                    for(int j=1;1;j++) {
                      remove_hash[j-1]=operand1[j];
                      if(operand1[j]=='\0')
                        break;
                    }
                    sprintf(temp,"%02x%01x%03x|",OpTab[opcode_num].code+add_ni,add_xbpe,toInt(remove_hash));
                    //sprintf(temp,"%02x|",OpTab[opcode_num].code+add_ni);
                    strcat(text_record,temp);
                    text_record_ch_count+=7;
                    text_record_length+=6;
                  }
                }
              }
          }
      }
    }
    else {

      if(strcmp(instr,"WORD")==0) {
        if(text_record_ch_count+3>78) {
          sprintf(temp,"%02x",text_record_length/2);
          text_record[9]=temp[0];
          text_record[10]=temp[1];
          fprintf(fpfinal,"%s\n",text_record);
          sprintf(text_record,"T|%s|__|",str_addr);
          text_record_ch_count=12;
          text_record_length=0;
        }
        sprintf(temp,"%06x|",toInt(operand1));
        strcat(text_record,temp);
        text_record_ch_count+=7;
        text_record_length+=6;
      }
      else {
        if(strcmp(instr,"BYTE")==0) {

          if(text_record_ch_count+strlen(temp)>78) {
            sprintf(temp,"%02x",text_record_length/2);
            text_record[9]=temp[0];
            text_record[10]=temp[1];
            fprintf(fpfinal,"%s\n",text_record);
            sprintf(text_record,"T|%s|__|",str_addr);
            text_record_ch_count=12;
            text_record_length=0;
          }
          getBytes(operand1,temp);
          strcat(temp,"|");
          strcat(text_record,temp);
          text_record_ch_count+=strlen(temp);
          text_record_length+=(strlen(temp)-1);
        }
        else {
          if(strcmp(instr,"RESW")==0) {
            if(text_record_ch_count>12) {
              sprintf(temp,"%02x",text_record_length/2);
              text_record[9]=temp[0];
              text_record[10]=temp[1];
              fprintf(fpfinal,"%s\n",text_record);
            }
            sprintf(text_record,"T|%06x|__|",toHex(str_addr)+countResw(operand1));
            text_record_ch_count=12;
            text_record_length=0;

          }
          else {
            if(strcmp(instr,"RESB")==0) {
              if(text_record_ch_count>12) {
                sprintf(temp,"%02x",text_record_length/2);
                text_record[9]=temp[0];
                text_record[10]=temp[1];
                fprintf(fpfinal,"%s\n",text_record);
              }
              sprintf(text_record,"T|%06x|__|",toHex(str_addr)+countByte(operand1));
              text_record_ch_count=12;
              text_record_length=0;
            }
            else {
              if(strcmp(instr,"BASE")==0) {
                base_register=toInt(operand1);
              }
              else {
                if(strcmp(instr,"EQU")!=0) {
                  if(text_record_ch_count+strlen(temp)>78) {
                    sprintf(temp,"%02x",text_record_length/2);
                    text_record[9]=temp[0];
                    text_record[10]=temp[1];
                    fprintf(fpfinal,"%s\n",text_record);
                    sprintf(text_record,"T|%s|__|",str_addr);
                    text_record_ch_count=12;
                    text_record_length=0;
                  }
                  strcat(text_record,"Error|");
                  text_record_ch_count+=7;
                  text_record_length+=6;
                }
              }
            }
          }
        }
      }
    }

  }
  if(text_record_ch_count>12) {
    sprintf(temp,"%02x",text_record_length/2);
    text_record[9]=temp[0];
    text_record[10]=temp[1];
    fprintf(fpfinal,"%s\n",text_record);
  }
  fprintf(fpfinal,"E|%06x|",execution_address);
  fclose(fpfinal);
  fclose(fpintr);

}



void main() {
  start=NULL; // initialise global symbol table
  createOpTtab();
  createRegisterBank();
  int successpass1=pass1();
  if(successpass1==1) {
    pass2();
  }
  /*
  printf("starting address %x\n",starting_address);
  printf("ending adress %x\n",end_address);
  printf("execution_address %x\n",execution_address);
  printf("prog %s\n",prog_name);


  printf("displayTable\n");
  displaySymtab();
  */
}
