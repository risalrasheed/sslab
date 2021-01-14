#include<stdio.h>
#include<stdlib.h>
int ifExist(int* frames, int page, int f_count) {
  for(int i=0;i<f_count;i++)
    if(frames[i]==page)
      return 1;
  return 0;
}
void displayFrames(int* frames, int f_count) {
  for(int i=0; i<f_count; i++)
    printf("%d ",frames[i]);
  printf("\n");
}
int fcfs(int* pages, int* frames, int p_count, int f_count) {
  int ptr=f_count-1;
  int exist_flag=0;
  int fault_count=0;
  for(int i=0;i<f_count;i++) {
    frames[i] = -1;
  }
  for(int i=0; i<p_count; i++) {
    exist_flag=ifExist(frames, pages[i], f_count);
    if(exist_flag == 0) {
      frames[(++ptr)%f_count] = pages[i];
      fault_count++;
    }
    displayFrames(frames, f_count);
  }
  printf("Number of faults is %d\n",fault_count);
}
int giveLRU(int* lru_ar, int f_count) {
  if(lru_ar[f_count-1]==-1) {

  }
  int ret_val = lru_ar[0];
  for(int i=0;i<f_count-1;i++)
    lru_ar[i]=lru_ar[i+1];
  return ret_val;
}
void replaceLRU(int* lru_ar, int page, int f_count) {
  lru_ar[f_count-1]=page;
}
void updateLRU(int* frames, int lru_val, int page, int f_count) {
  int i;
  for(i=0;frames[i]!=lru_val;i++);

  frames[i]=page;
}
void rearrangeLRU(int* lru_ar, int page, int f_count) {
  int pos,i;
  for(pos=0;lru_ar[pos]!=page;pos++);

  for(i=pos; i<f_count-1; i++)
    lru_ar[i]=lru_ar[i+1];

  lru_ar[i]=page;
}
int lru(int* pages, int* frames, int p_count, int f_count) {
  int ptr=f_count-1;
  int exist_flag=0;
  int fault_count=0;
  int* lru_ar =(int*) malloc(f_count*sizeof(int));
  int lru_value;
  for(int i=0;i<f_count;i++) {
    frames[i] = -1;
    lru_ar[i] = -1;
  }
  for(int i=0; i<p_count; i++) {
    exist_flag=ifExist(frames, pages[i], f_count);
    if(exist_flag == 0) {
      lru_value = giveLRU(lru_ar,f_count);
      replaceLRU(lru_ar,pages[i],f_count);
      updateLRU(frames,lru_value,pages[i],f_count);
      fault_count++;
    }
    else {
      rearrangeLRU(lru_ar,pages[i],f_count);
    }
    displayFrames(frames, f_count);
    printf("Lru values ");
    for(int i=0;i<f_count;i++)
      printf("%d ",lru_ar[i]);
    printf("\n");
  }
  printf("Number of fault is %d\n",fault_count);
}
void addOptimal(int* frames, int* pages, int f_count, int p_count, int pos) {
  int i;
  int j;
  int farthest=-1;
  int farthest_dist=0;
  int flag=0;
  if(frames[f_count-1]==-1) {
    for(i=0;frames[i]!=-1;i++);

    frames[i]=pages[pos];
  }
  else {
    for(i=0;i<f_count;i++) {
      flag=0;
      for(j=pos;j<p_count;j++) {
        if(frames[i]==pages[j]) {
          if(j>farthest_dist) {
            farthest_dist=j;
            farthest=frames[i];
            break;
          }
          else {
            flag=1;
          }
        }

      }
      if(j==p_count && flag==0) {
        farthest=frames[i];

        break;
      }
    }
    for(i=0;frames[i]!=farthest;i++);

    frames[i]=pages[pos];

  }
}
int optimal(int* pages, int* frames, int p_count, int f_count) {
  int ptr=f_count-1;
  int exist_flag=0;
  int fault_count=0;

  for(int i=0;i<f_count;i++) {
    frames[i] = -1;
  }
  for(int i=0; i<p_count; i++) {
    exist_flag=ifExist(frames, pages[i], f_count);
    if(exist_flag == 0) {
      addOptimal(frames, pages, f_count, p_count, i);
      fault_count++;
    }

    displayFrames(frames, f_count);

  }
  printf("Number of fault is %d\n",fault_count);
}
void main() {
  int p_count;
  int f_count;
  int opt;
  int* pages;
  int* frames;
  while(1) {
    printf("Select page replacement algorithm\n");
    printf("1)FCFS\n");
    printf("2)LRU\n");
    printf("3)Optimal\n");
    printf("4)Exit ");
    scanf("%d",&opt);
    if(opt==4) exit(0);
    printf("Enter the number of frames ");
    scanf("%d",&f_count);
    frames = (int*)malloc(f_count*sizeof(int));
    printf("Enter the number of pages ");
    scanf("%d",&p_count);
    pages = (int*)malloc(p_count*sizeof(int));
    printf("Enter the sequence ");
    for(int i=0;i<p_count;i++)
      scanf("%d",&pages[i]);

    switch(opt) {
      case 1:
        fcfs(pages, frames, p_count, f_count);
        break;
      case 2:
        lru(pages, frames, p_count, f_count);
        break;
      case 3:
        optimal(pages, frames, p_count, f_count);
        break;
      default:
        printf("Invalid option\n");
    }
  }
}
