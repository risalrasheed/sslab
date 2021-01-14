#include<stdio.h>
#include<stdlib.h>
typedef struct processes {
    int at,bt,priority;
    unsigned short int index,flag;
} process;

void menu() {
  printf("\nSelect Option\n");
  printf("1)FCFS\n");
  printf("2)SJF\n");
  printf("3)RR\n");
  printf("4)PRIORITY\n");
  printf("5)Exit\n");
  printf("Enter option ");
}

process* readvalues(int n) {

  process* pi=(process*)malloc(n*sizeof(process));
  printf("Enter details (AT , BT)\n");
  for(int i=0;i<n;i++) {
    scanf("%d%d",&pi[i].at,&pi[i].bt);
    pi[i].index=i;
    pi[i].flag=0;
  }
  return pi;
}
process* readvalues_prio(int n) {

  process* pi=(process*)malloc(n*sizeof(process));
  printf("Enter details (AT BT P)\n");
  for(int i=0;i<n;i++) {
    scanf("%d%d%d",&pi[i].at,&pi[i].bt,&pi[i].priority);
    pi[i].index=i;
    pi[i].flag=0;
  }
  return pi;
}
void printer(process* pid,int n) {
  for(int i=0;i<n;i++) {
    printf("%d %d %d\n",pid[i].index,pid[i].at,pid[i].bt);
  }
}
void sort(process* pid,int n) {
  for(int i=0;i<n-1;i++) {
    for(int j=0;j<n-i-1;j++) {
      if(pid[j].at>pid[j+1].at) {
        process temp=pid[j];
        pid[j]=pid[j+1];
        pid[j+1]=temp;
      }
    }
  }
}
void fcfs() {
  int n=0,endtime=0,at=0,temp=0;
  float tt=0,wt=0;
  printf("Enter the number of processes ");
  scanf("%d",&n);
  process* pid=readvalues(n);
  //printer(pid,n);
  sort(pid,n);
  for(int i=0;i<n;i++) {
    if(pid[i].at>endtime)
      at=pid[i].at-endtime;
    else
      at=0;
    endtime+=(at+pid[i].bt);
    temp=(endtime-pid[i].at-pid[i].bt);
    tt+=(temp+pid[i].bt);
    wt+=temp;
  }
  printf("\nAvereage Waiting time in FCFS is %f",wt/n);
  printf("\nAvereage Turnaround time in FCFS is %f",tt/n);

  //printer(pid,n);

}

void sjf() {
  int n=0,endtime=0,at=0,temp=0;
  float tt=0,wt=0;
  printf("Enter the number of processes ");
  scanf("%d",&n);
  process* pid=readvalues(n);
  //printer(pid,n);
  sort(pid,n);
  /*
  for(int i=0;i<n;i++) {
    if(pid[i].at>endtime)
      at=pid[i].at-endtime;
    else
      at=0;
    endtime+=(at+pid[i].bt);
    temp=(endtime-pid[i].at-pid[i].bt);
    tt+=(temp+pid[i].bt);
    wt+=temp;
  }
  */
  endtime=pid[0].at;
  while(1){
    int pos=-1;
    int sbt=-1;
    for(int i=0;i<n;i++) {
      if(pid[i].flag==0&&pid[i].at<=endtime) {
        if(sbt==-1) {
          sbt=pid[i].bt;
          pos=i;
        }
        else {
          if(sbt>pid[i].bt) {
            sbt=pid[i].bt;
            pos=i;
          }
        }
      }
    }

    if(pos==-1) {
      //printf("over");
      break;
    }
    else {
      //printf("\nExec %d",pid[pos].bt);
      pid[pos].flag=1;
      endtime+=(pid[pos].bt);//at is removed
      temp=(endtime-pid[pos].at-pid[pos].bt);
      tt+=(temp+pid[pos].bt);
      wt+=temp;

    }

  }
  printf("\nAvereage Waiting time in sjf is %f",wt/n);
  printf("\nAvereage Turnaround time in sjf is %f",tt/n);

  //printer(pid,n);

}

void rr() {
  int n=0,endtime=0,at=0,temp=0,quantum=0,req=0,*ar_wt,*ar_tt,*cycles,*offset,*ar_endtime,*rem_bt,flag=0,pos=-1,rem=0;
  float tt=0,wt=0;
  printf("Enter the number of processes ");
  scanf("%d",&n);
  printf("Enter the time quantum ");
  scanf("%d",&quantum);
  process* pid=readvalues(n);

  ar_wt=(int*)malloc(n*sizeof(int));
  //offset=(int*)malloc(n*sizeof(int));
  //cycles=(int*)malloc(n*sizeof(int));
  ar_endtime=(int*)malloc(n*sizeof(int));
  rem_bt=(int*)malloc(n*sizeof(int));
  ar_tt=(int*)malloc(n*sizeof(int));
  //printer(pid,n);
  sort(pid,n);

  for(int i=0;i<n;i++) {
    rem_bt[i]=pid[i].bt;
  }
  endtime=pid[0].at;
  while(1>0){
    int val=1;
    pos=(pos+1)%n;
    if(pos==0) flag=0;
    if(rem_bt[pos]>0 && pid[pos].at<=endtime) {

      val++;
      flag=1;
      rem=quantum;
      //rem=rem_bt[pos]-quantum;
      if(rem_bt[pos]<rem) rem=rem_bt[pos];
      rem_bt[pos]-=rem;
      endtime+=rem;
      if(rem_bt[pos]==0) ar_endtime[pos]=endtime;


    }
    if(pos==n-1 && flag==0) break;

  }

  for(int i=0;i<n;i++) {
    ar_wt[i]=ar_endtime[i]-pid[i].at-pid[i].bt;
    ar_tt[i]=ar_wt[i]+pid[i].bt;
  }
  for(int i=0;i<n;i++) {
    wt+=ar_wt[i];
    tt+=ar_tt[i];
  }
  printf("\nAverage waiting time of Roundrobin is %f",wt/n);
  printf("\nAverage turnarround time of Roundrobin is %f",tt/n);

}
void priority() {
  int n=0,endtime=0,at=0,temp=0;
  float tt=0,wt=0;
  printf("Enter the number of processes ");
  scanf("%d",&n);
  process* pid=readvalues_prio(n);
  //printer(pid,n);
  sort(pid,n);
  endtime=pid[0].at;
  while(1>0){
    int pos=-1;
    int lp=-1;
    for(int i=0;i<n;i++) {
      if(pid[i].flag==0&&pid[i].at<=endtime) {
        if(lp==-1) {
          lp=pid[i].priority;
          pos=i;
        }
        else {
          if(lp>pid[i].priority) {
            lp=pid[i].priority;
            pos=i;
          }
        }
      }
    }

    if(pos==-1) {
      
      break;
    }
    else {
      pid[pos].flag=1;
      endtime+=(at+pid[pos].bt);
      temp=(endtime-pid[pos].at-pid[pos].bt);
      tt+=(temp+pid[pos].bt);
      wt+=temp;
    }
  }
  printf("\nAvereage Waiting time in FCFS is %f",wt/n);
  printf("\nAvereage Turnaround time in FCFS is %f",tt/n);
}
void main() {
    while(1>0) {
      menu();
      int opt;
      scanf("%d",&opt);
      switch(opt)
      {
        case 1:
          fcfs();
          break;
        case 2:
          sjf();
          break;
        case 3:
          rr();
          break;
        case 4:
          priority();
          break;

        case 5:
          exit(0);

        default:
          printf("Invalid option try again");
      }

    }
}
