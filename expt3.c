#include<stdio.h>
#include<stdlib.h>
void menu() {
  printf("\n\t\t\tMENU\n");
  printf("1)FCFS\n");
  printf("2)SCAN\n");
  printf("3)CSCAN\n");
  printf("4)Exit\n");
  printf("Select option ");
}

void swap(int *a, int *b) {
  int temp=*a;
  *a=*b;
  *b=temp;
}

int partition(int* reqs, int beg, int end) {
  int pivot=reqs[end];
  int i=beg-1;
  for(int j=beg;j<end;j++) {
    if(reqs[j]<pivot) {
      i++;
      swap(&reqs[i],&reqs[j]);
    }
  }
  swap(&reqs[++i],&reqs[end]);
  return i;
}

void quickSort(int* reqs, int beg, int end) {
  if(beg<end) {
    int pivot = partition(reqs, beg, end);
    quickSort(reqs, beg, pivot-1);
    quickSort(reqs, pivot+1, end);
  }
}

int findIndex(int* reqs, int cur_pos, int no_req) {
  int i=0;
  int index=no_req-1;
  for(;i<no_req;i++)
    if(cur_pos<reqs[i]) {
      index=i-1;
      break;
    }
  return index;
}
int fcfs(int* reqs,int cur_pos, int max, int no_req) {
  int seek_time=0;
  for(int i=0;i<no_req; i++) {
    seek_time += abs(reqs[i]-cur_pos);
    cur_pos=reqs[i];
    printf("Request %d: %d\n",(i+1),cur_pos);
  }
  return seek_time;

}
int scan(int* reqs, int cur_pos, int max, int no_req) {
  int i=1;
  int seek_time=0;
  int index_pos;
  int temp_pos;
  quickSort(reqs, 0, no_req-1);
  index_pos = findIndex(reqs, cur_pos, no_req);
  temp_pos=index_pos;
  while(temp_pos>=0) {
    seek_time+= abs(cur_pos-reqs[temp_pos]);
    cur_pos=reqs[temp_pos--];
    printf("Request %d: %d\n",i,cur_pos);
    i++;
  }
  if(reqs[temp_pos+1]!=0) {
    seek_time+= abs(cur_pos-0);
    cur_pos=0;
    printf("Request %d: %d\n",i,cur_pos);
    i++;
  }
  temp_pos=index_pos+1;
  while(temp_pos<=no_req-1) {
    seek_time+= abs(cur_pos-reqs[temp_pos]);
    cur_pos=reqs[temp_pos++];
    printf("Request %d: %d\n",i,cur_pos);
    i++;
  }
  return seek_time;
}
int cscan(int* reqs, int cur_pos, int max, int no_req) {
  int i=1;
  int seek_time=0;
  int index_pos;
  int temp_pos;
  quickSort(reqs, 0, no_req-1);
  index_pos = findIndex(reqs, cur_pos, no_req);
  temp_pos=index_pos;
  printf("Index is %d\n",index_pos);
  while(temp_pos>=0) {
    seek_time+= abs(cur_pos-reqs[temp_pos]);
    cur_pos=reqs[temp_pos--];
    printf("Request %d: %d\n",i,cur_pos);
    i++;
  }
  if(reqs[temp_pos+1]!=0) {
    seek_time+= abs(cur_pos-0);
    cur_pos=0;
    printf("Request %d: %d\n",i,cur_pos);
    i++;
  }
  seek_time+= abs(cur_pos-max);
  cur_pos=max;
  temp_pos=no_req-1;
  printf("Request %d: %d\n",i,cur_pos);
  i++;
  while(temp_pos>index_pos) {
    seek_time+= abs(cur_pos-reqs[temp_pos]);
    cur_pos=reqs[temp_pos--];
    printf("Request %d: %d\n",i,cur_pos);
    i++;
  }
  return seek_time;
}
void main() {
  int opt;
  int no_req;
  int max;
  int cur_pos;
  int* reqs;
  int seek_time;

  while(1) {
    menu();
    scanf("%d",&opt);
    if(opt!=4) {
      printf("Enter the number of requests ");
      scanf("%d", &no_req);
      printf("Enter the maximum ");
      scanf("%d",&max);
      printf("Enter the current position ");
      scanf("%d",&cur_pos);
      reqs=(int*)malloc(sizeof(int)*no_req);
      printf("Enter the requests ");
      for(int i=0;i<no_req;i++)
        scanf("%d",&reqs[i]);
      int ar[10]={8,1,5,3,4};
      max--;
    }
    switch(opt) {
      case 1:
        seek_time=fcfs(reqs,cur_pos,max,no_req);
        break;
      case 3:
        seek_time=cscan(reqs, cur_pos, max, no_req);
        break;
      case 2:
        seek_time=scan(reqs, cur_pos, max, no_req);
        break;
      case 4:
        exit(0);
        break;
      default:
        printf("Enter a correct option\n");
    }
    printf("\nSeek time is %d",seek_time);
  }
}
