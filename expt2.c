#include<stdio.h>
#include<stdlib.h>
#include<string.h>


void readData(int* table, int num_pro, int num_res, char* string) {

  printf("\nEnter the details of %s\n",string);
  if(strcmp(string,"avail")) {
    for(int i=0; i<num_pro; i++) {
      printf("Resource instances of process %d ",i+1);
      for(int j=0; j<num_res; j++)
        scanf("%d",(table + i*num_res + j));
    }
  }
  else {
    for(int j=0; j<num_res; j++)
      scanf("%d",(table + j));
  }
}

void displayTable(int* table, int num_pro, int num_res, char* string) {
  printf("\nDetails of %s\n",string);
  for(int i=0; i<num_pro; i++) {
    for(int j=0; j<num_res; j++)
      printf("%d ",*(table + num_res*i + j));
    printf("\n");
  }
}

void defineNeed(int* need, int* max, int* alloc, int num_pro, int num_res) {
  for(int i=0; i<num_pro; i++)
    for(int j=0; j<num_res; j++)
      *(need + num_res*i + j) = *(max + num_res*i + j) - *(alloc + num_res*i + j);
}

int* callBankersAlgorithm(int* need, int* avail, int num_pro, int num_res) {
  int dl_flag=1, op_flag=0, pos=0, order_pos=-1;

  int* comp_pro = (int*)malloc(sizeof(int)*num_pro);
  for(int i=0; i<num_pro; i++)
    comp_pro[i] = 0;

  while(1) {
    if(pos==0)
      dl_flag=1;
    op_flag=1;
    if(comp_pro[pos]==0) {
      for(int j=0; j<num_res; j++) {
        if( *(need + pos*num_res + j) > *(avail + j) ) {
          op_flag=0;
          break;
        }
      }

      if(op_flag==1) {
        dl_flag=0;
        for(int j=0; j<num_res; j++) {
          *(avail + j) += *(need + num_res*pos + j);

        }
        *(comp_pro + pos) = 1;

      }
    }

    if(pos == (num_pro-1) && dl_flag == 1)
      break;
    pos = (pos+1)%num_pro;

  }
  return comp_pro;
}

void main() {
  int num_res, num_pro, dl_flag, *avail, *max, *need, *alloc, *comp_pro;
  printf("Enter the number of resources ");
  scanf("%d",&num_res);
  printf("Enter the number of processes ");
  scanf("%d",&num_pro);

  max = (int*)malloc(sizeof(int)*num_pro*num_res);
  alloc = (int*)malloc(sizeof(int)*num_pro*num_res);
  need = (int*)malloc(sizeof(int)*num_pro*num_res);
  avail = (int*)malloc(sizeof(int)*num_res);

  readData(max, num_pro, num_res, "max");
  readData(alloc, num_pro, num_res, "alloc");
  readData(avail, num_pro, num_res, "avail");

  defineNeed(need, max, alloc, num_pro, num_res);

  displayTable(max, num_pro, num_res, "max");
  displayTable(alloc, num_pro, num_res, "alloc");
  displayTable(avail, 1, num_res, "avail");
  displayTable(need, num_pro, num_res, "need");

  comp_pro = callBankersAlgorithm(need, avail, num_pro, num_res);
  displayTable(comp_pro, 1, num_pro, "comp_pro");
  dl_flag = 0;
  for(int i=0; i<num_pro; i++)
    if(comp_pro[i] == 0) {
      dl_flag = 1;
      break;
    }

  if(dl_flag == 0)
    printf("\nNo deadlock occurs\n");
  else
    printf("\nDeadlock occured");

}
