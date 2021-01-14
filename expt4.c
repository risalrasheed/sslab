#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct Single_Directories {
  char name[20];
  char files[20][20];
  int file_count;
} Single_Dir;

typedef struct Two_Directories {
  char mfd_name[20];
  Single_Dir ufd[20];
  int user_count;
} Two_Dir;


typedef struct Directory {
  char file[20][20];
  char dir_name[20];
  struct Directory* next;
  struct Directory* previous;
  struct Directory* sub_dir;
  int file_count;
}Dir;

typedef struct User{
  char ufd_name[20];
  Dir* start;
}User;
typedef struct Hierarchial_MFD {
  char mfd_name[20];
  User ufd[20];
  int ufd_count;
}Hir_MFD;

void hierarchialMenu() {
  printf("\n\t\t\tHierarchial Directory\n");
  printf("1)Create New User\n");
  printf("2)Select User\n");
  printf("3)Delete User\n");
  printf("4)Go Back\n");
  printf("5)Exit\n");
  printf("Select option ");
}
void hirUserMenu() {
  printf("\nHierarchial File System\n");
  printf("1)Add File\n");
  printf("2)Add Directory\n");
  printf("3)Select Directory\n");
  printf("4)List All Directories & Files\n");
  printf("5)Remove a file\n");
  printf("6)Remove Directory\n");
  printf("7)Go Back\n");
  printf("8)Exit\n");
  printf("Select option ");
}
/*
void hierarchialUser(User* ufd_add, int new) {
  int opt;
  if(new==1) {
    fgets((*ufd_add).ufd_name,20,stdin);
    (*ufd_add).start=(Dir*)malloc(sizeof(Dir));
    (*ufd_add).start.previous=NULL;
    (*ufd_add).start.next=NULL;
  }
  while(1) {
    hirUserMenu();
    scanf("%d",&opt);
    switch(opt) {
      case 1:
        if((*ufd_add).start==NULL) {
          printf("No subdirectories found")
        }
    }
  }
}*/
int checkFileExist(char* file_name,char file[][20],int file_count) {
  for(int i=0;i<=file_count;i++) {
    if(strcmp(file_name,file[i])==0)
      return 1;

  }
  return 0;
}
int checkDirExist(char* dir_name, Dir* dir_add) {
  Dir* ptr=dir_add->sub_dir;
  while(ptr!=NULL) {
    if(strcmp(ptr->dir_name,dir_name)==0)
      return 1;
    ptr=ptr->next;
  }
  return 0;
}
int directoryOperate(Dir* dir_add) {
  while(1) {
    int opt;
    char dir_name[20];
    char file_name[20];
    int file_exist;
    int dir_exist;
    Dir* ptr=NULL;
    int pos=-1;
    hirUserMenu();
    scanf("%d",&opt);
    switch(opt) {
      case 1:
      //printf("dir_add->file_count-%d\n",dir_add->file_count);
    //  if((dir_add->file_count)==2) {
        //if(strcmp(dir_add->file[0],dir_add->file[1])==0)
        //  printf("No problem\n");
        //else
        //  printf("Problem\n");
      //}
        if(dir_add->file_count==19)
          printf("No more file can be added\n");
        else {
          printf("Enter the file name ");
          fgets(file_name,20,stdin);
          if(strcmp(file_name,"\n"));
            fgets(file_name,20,stdin);
          file_exist=checkFileExist(file_name,dir_add->file,dir_add->file_count);
          //printf("file exist:%d\n",file_exist);
          if(file_exist==1)
            printf("File Already Exist\n");
          else {
            strcpy(dir_add->file[++(dir_add->file_count)],file_name);
            printf("File added\n");
          }
          //printf("dir_add->file_count-%d\n",dir_add->file_count);

        }
        break;

      case 2:
        printf("Enter the directory Name ");
        fgets(dir_name,20,stdin);
        //fgets(,20,stdin);
        if(strcmp(dir_name,"\n")==0);
          fgets(dir_name,20,stdin);
        dir_exist=checkDirExist(dir_name,dir_add);
        if(dir_exist==0) {

          Dir* new_dir = (Dir*)malloc(sizeof(Dir));
          strcpy(new_dir->dir_name,dir_name);
          new_dir->next=NULL;
          new_dir->previous=NULL;
          new_dir->sub_dir=NULL;
          new_dir->file_count=-1;

          //here
          if(dir_add->sub_dir==NULL)
            dir_add->sub_dir=new_dir;
          else {
            ptr=dir_add->sub_dir;
            while(ptr->next!=NULL) {
              ptr=ptr->next;
            }
            ptr->next=new_dir;
            new_dir->previous=ptr;
          }
          printf("Directory Created\n");
        }
        else
          printf("Directory already exist\n");
        break;

      case 3:
        printf("Enter directory name ");
        fgets(dir_name,20,stdin);
        if(strcmp(dir_name,"\n"));
          fgets(dir_name,20,stdin);
        ptr=dir_add->sub_dir;
        while(ptr!=NULL && strcmp(ptr->dir_name,dir_name)!=0) {
          ptr=ptr->next;
        }
        if(ptr==NULL)
          printf("No such directory\n");
        else
        directoryOperate(ptr);
        break;

      case 4:
        printf("%s\n",dir_add->dir_name);
        ptr=dir_add->sub_dir;
        while(ptr!=NULL) {
          printf("|-+%s\n",ptr->dir_name);
          ptr=ptr->next;
        }

        for(int i=0;i<=dir_add->file_count;i++)
          printf("|-%s\n",dir_add->file[i]);
        break;

      case 5:
        printf("Enter the file name ");
        fgets(file_name,20,stdin);
        if(strcmp(file_name,"\n"));
          fgets(file_name,20,stdin);
        pos=-1;
        for(int i=0;i<=dir_add->file_count;i++) {
          if(strcmp(file_name,dir_add->file[i])==0) {
            pos=i;
            break;
          }
        }
        if(pos!=-1) {
          for(int i=pos;i<dir_add->file_count;i++) {
            strcpy(dir_add->file[i],dir_add->file[i+1]);
          }
          dir_add->file_count--;

          printf("Successfully deleted\n");
          break;
        }
        else
          printf("No file found\n");
        break;

      case 6:
        printf("Enter the directory name ");
        fgets(dir_name,20,stdin);
        if(strcmp(dir_name,"\n"));
          fgets(dir_name,20,stdin);
        ptr=dir_add->sub_dir;
        int del_flag=0;
        if(ptr==NULL)
          printf("No sub directories exist\n");
        else {
          if(strcmp(ptr->dir_name,dir_name)==0) {
            dir_add->sub_dir=ptr->next;
            if(dir_add->sub_dir!=NULL)
              dir_add->sub_dir->previous=NULL;
            del_flag=1;
          }
          else {
            while(ptr!=NULL && strcmp(ptr->dir_name,dir_name)!=0)
              ptr=ptr->next;

            if(ptr!=NULL) {
              ptr->previous->next=ptr->next;
              if(ptr->next!=NULL)
                ptr->next->previous=ptr->previous;
              del_flag=1;
            }
          }
          if(del_flag==1)
            printf("Directory deleted\n");
          else
            printf("Directory not found\n");

        }
          break;

        case 7:
          return 0;

        case 8:
          exit(0);

        default:
          printf("Invalid choice\n");
      }


  }
}
int checkUserExist(char* user_name, User* ufd_add, int user_count) {
  for(int i=0;i<=user_count;i++) {
    if(strcmp(user_name,ufd_add[i].ufd_name)==0)
      return 1;
  }
  return 0;
}
int hierarchial(Hir_MFD* mfd_add) {
  int opt;
  mfd_add->ufd_count=-1;
  char user_name[20];
  int user_flag=0;
  int user_exist;
  int pos=-1;
  while(1) {
    hierarchialMenu();
    scanf("%d",&opt);
    switch(opt) {
      case 1:
        if(mfd_add->ufd_count==19) {
          printf("Limit of users reached\n");
        }
        else {
          printf("Enter the username ");
          fgets(user_name,20,stdin);
          //fgets(mfd_add->ufd[++mfd_add->ufd_count].ufd_name,20,stdin);
          if(strcmp(user_name,"\n"));
            fgets(user_name,20,stdin);
          user_exist=checkUserExist(user_name,mfd_add->ufd,mfd_add->ufd_count);
          if(user_exist==0) {
            strcpy(mfd_add->ufd[++mfd_add->ufd_count].ufd_name,user_name);
            printf("Here %s",mfd_add->ufd[mfd_add->ufd_count].ufd_name);
            mfd_add->ufd[mfd_add->ufd_count].start=(Dir*)malloc(sizeof(Dir));
            strcpy(mfd_add->ufd[mfd_add->ufd_count].start->dir_name,mfd_add->ufd[mfd_add->ufd_count].ufd_name);
            mfd_add->ufd[mfd_add->ufd_count].start->previous=NULL;
            mfd_add->ufd[mfd_add->ufd_count].start->next=NULL;
            mfd_add->ufd[mfd_add->ufd_count].start->sub_dir=NULL;
            mfd_add->ufd[mfd_add->ufd_count].start->file_count=-1;
            printf("Successfully added\n");
          }
          else
            printf("Username already exist\n");
        }
        break;
      case 2:
        printf("Enter the user name ");
        fgets(user_name,20,stdin);
        if(strcmp(user_name,"\n"));
          fgets(user_name,20,stdin);
        for(int i=0;i<=mfd_add->ufd_count;i++) {
          if(strcmp(mfd_add->ufd[i].ufd_name,user_name)==0) {
            user_flag=1;
            directoryOperate(mfd_add->ufd[i].start);
          }
        }
        if(user_flag!=1)
          printf("User not found\n");
        //else
        //  printf("here\n");
        //hierarchialUser()
        break;

      case 3:
        printf("Enter username to delete ");
        fgets(user_name,20,stdin);
        if(strcmp(user_name,"\n"));
          fgets(user_name,20,stdin);
        pos=-1;
        for(int i=0;i<=mfd_add->ufd_count;i++) {
          if(strcmp(user_name,mfd_add->ufd[i].ufd_name)==0) {
            pos=i;
            break;
          }
        }
        if(pos==-1)
          printf("No such user found\n");
        else {
          for(int i=pos;i<mfd_add->ufd_count;i++)
            mfd_add->ufd[i]=mfd_add->ufd[i+1];
          printf("User Deleted\n");
          --(mfd_add->ufd_count);
        }
        break;

      case 4:
        return 0;

      case 5:
        exit(0);

      default:
        printf("Invalid choice\n");
    }
  }
}

void generalMenu() {
  printf("\n\t\t\tSelect Directory Structure\n");
  printf("1)Single Level Directory\n");
  printf("2)Two level Directory\n");
  printf("3)Hierarchial Directory\n");
  printf("4)Exit\n");
  printf("Select option ");
}
int nameSearchSingle(Single_Dir root, char* string) {
  for(int i=0;i<=root.file_count;i++)
    if(strcmp(root.files[i],string)==0)
      return 1;
  return 0;
}
void singleLevelMenu() {
  printf("\nSingle Level File System\n");
  printf("1)Add File\n");
  printf("2)List All Files\n");
  printf("3)Remove a file\n");
  printf("4)Go Back\n");
  printf("5)Exit\n");
  printf("Select option ");
}
int singleLevel(Single_Dir* root_add, int new_flag) {
  char del_name[20];
  char file_name[20];
  int opt;
  int pos=-1;
  int existing_flag=0;
  //Single_Dir root;
  if(new_flag==1) {
    printf("Enter name for directory ");
    fgets((*root_add).name,20,stdin);
    if(strcmp((*root_add).name,"\n")==0)
      fgets((*root_add).name,20,stdin);
    (*root_add).file_count=-1;
  }
  while(1) {
    singleLevelMenu();
    scanf("%d",&opt);
    switch(opt) {
      case 1:
        if((*root_add).file_count==19)
          printf("Cannot add more files\n");
        else {
          while(1){
            printf("Enter the name of the new file ");
            fgets(file_name,20,stdin);
            if(strcmp(file_name,"\n")==0)
              fgets(file_name,20,stdin);
            existing_flag=nameSearchSingle(*root_add,file_name);
            if(existing_flag==1)
              printf("File already exist\n");
            else
              break;
          }
          strcpy((*root_add).files[++(*root_add).file_count],file_name);
          printf("Added successfully\n");
        }
        break;
      case 2:
        printf("%s\n",(*root_add).name);
        for(int i=0; i<=(*root_add).file_count;i++) {
          printf("\t|- %s\n",(*root_add).files[i]);
        }
        break;
      case 3:
        printf("Enter file name to delete ");
        fgets(del_name,20,stdin);
        if(strcmp(del_name,"\n")==0)
          fgets(del_name,20,stdin);

        for(int i=0;i<=(*root_add).file_count;i++) {
          if(strcmp(del_name,(*root_add).files[i])==0) {
            pos=i;
            break;
          }
        }
        if(pos==-1)
          printf("Couldn't find file\n");
        else {
          for(int i=pos+1;i<=(*root_add).file_count;i++) {
            strcpy((*root_add).files[i-1],(*root_add).files[i]);
          }
          (*root_add).file_count--;
          pos=-1;
          printf("Successfully Deleted\n");
        }
        break;
      case 4:
        return 0;
      case 5:
        exit(0);
      default:
        printf("Wrong choice. Try Again!\n");
    }
  }
}
void delTwoLevelUser(Two_Dir* root_add,int pos) {
  for(int i=pos;i<(*root_add).user_count;i++) {
    (*root_add).ufd[i]=(*root_add).ufd[i+1];
  }
  (*root_add).user_count--;
  printf("User Deleted\n");
}
void twoLevelMenu() {
  printf("\n\t\t\tTwo Level Directory\n");
  printf("1)Create New User\n");
  printf("2)Select User\n");
  printf("3)Delete User\n");
  printf("4)Go Back\n");
  printf("5)Exit\n");
  printf("Select option ");
}
int findUser(Two_Dir root, char* string) {
  for(int i=0;i<=root.user_count;i++) {
    if(strcmp(root.ufd[i].name,string)==0)
      return i;
  }
  return -1;
}
int twoLevel() {

  int opt;
  Two_Dir root;
  char user_name[20];
  int user_pos=-1;
  root.user_count=-1;
  printf("Enter the name of MFD ");
  fgets(root.mfd_name,20,stdin);
  if(strcmp(root.mfd_name,"\n")==0)
    fgets(root.mfd_name,20,stdin);


  while(1) {
    twoLevelMenu();
    scanf("%d",&opt);
    switch(opt) {
      case 1:
        singleLevel(&root.ufd[++root.user_count],1);
        break;
      case 2:
        printf("Enter the user name ");
        fgets(user_name,20,stdin);
        if(strcmp(user_name,"\n")==0)
          fgets(user_name,20,stdin);
        user_pos=findUser(root,user_name);
        if(user_pos==-1)
          printf("No such user\n");
        else {
          printf("Logging  in as %s\n",user_name);
          singleLevel(&root.ufd[user_pos],0);
        }
        break;
      case 3:
        printf("Enter user's name to delete ");
        fgets(user_name,20,stdin);
        if(strcmp(user_name,"\n")==0)
          fgets(user_name,20,stdin);
        user_pos=findUser(root,user_name);
        if(user_pos==-1) {
          printf("No such user\n");
        }
        else {
          delTwoLevelUser(&root,user_pos);
        }
        break;
      case 4:
        return 0;
      case 5:
        exit(0);
    }
  }

}

void main() {
  int opt;


  while(1) {
    generalMenu();
    scanf("%d",&opt);
    switch(opt) {
      case 1:
        ;
        Single_Dir root;
        singleLevel(&root,1);
        break;
      case 2:
        twoLevel();
        break;
      case 3:
        ;
        Hir_MFD mfd;
        printf("Enter the mfd name ");
        fgets(mfd.mfd_name,20,stdin);
        if(strcmp(mfd.mfd_name,"\n")==0)
          fgets(mfd.mfd_name,20,stdin);
        hierarchial(&mfd);
        break;
      case 4:
        exit(0);
      default:
        printf("Wrong input. Try Again!");
    }
  }
}
