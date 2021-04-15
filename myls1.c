#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>
#include "secret_headers.h"
void getAndPrintUserName(char* dir);
#define MAXLINE 5000
char* date_time(const char *dir);
long int findSize(const char *dir);
int is_hidden(const char *name);
void getAndPrintGroup(char *dir);
void getFilePermission(const char *dir);
int countFiles(const char *dir);
void sortNames(char *dir);
int stringCompare(char *string1, char *string2);
void getdino(const char *dir, char*name);
void cur_dirent_list(const char *dir, int signal)
{

  /*struct dirent **dirlist;
  int n;
  n=scandir(&dir,&dirlist,0,alphasort);
  if (n == -1){
      printf("Error reading the directory\n");
      exit(1);
  }*/
    DIR *d = opendir(dir);
    struct dirent *directory;
    if (!d){
    	perror(dir);
    	return;
    }
    else{
    while((directory = readdir(d))!=NULL)
      {
	char *dir_name = directory->d_name;
    	if(signal == 0){
	    printf("%s\n",dir_name);}
	else if(signal == 1){
      if( directory->d_name[0] != '.'){
	    printf("%lu\t%s\n",directory->d_ino, dir_name);
	    }
      }
	else if(signal == 2 && dir_name[0]!='.'){
	    getFilePermission(dir_name);
	    getAndPrintGroup(dir_name);
	    getAndPrintUserName(dir_name);
	    printf("%ld     %s     %s\n",findSize(dir_name),date_time(dir_name),dir_name);
	    }
	else if((signal == 3 && dir_name[0]!='.') ){
            printf("%lu\t", directory->d_ino);
	    getFilePermission(dir_name);
	    getAndPrintGroup(dir_name);
	    getAndPrintUserName(dir_name);
	    printf("%ld\t%s     %s\n",findSize(dir_name),date_time(dir_name),dir_name);
	    }
       }
     }
    closedir(d);
}


void traverseRecursive(const char *dlist){
  struct dirent **dirlist;
  int n;
  n=scandir(".",&dirlist,0,alphasort);

  if (n == -1){
      printf("Error reading the directory\n");
      exit(1);
  }

  int i = 0,listc = 0;
  char *list_dir[100];
  printf(".:\n");
  while(i < n){
      if(dirlist[i]->d_name[0] != '.') {
          if(dirlist[i]->d_type == DT_DIR) {
              list_dir[listc] = calloc(sizeof(char),1024);
              strcat(list_dir[listc],"./");
              strcat(list_dir[listc++],dirlist[i]->d_name);
              printf("%s ", dirlist[i]->d_name);
          }else{
              printf("%s ",dirlist[i]->d_name);
          }
      }
      free(dirlist[i]);
      i++;
  }
  free(dirlist);
  printf("\n");
  printf("\n");
  Dirrecursor(list_dir,listc);
}
void Dirrecursor(char **list, int size) {
    int i = 0,n,listc = 0;
    struct dirent **dirlist;
    char *list_dir[100];
    if(size == 0){
        return;
    }
    while(i < size){
        printf("%s:\n",list[i]);
        n=scandir(list[i],&dirlist,0,alphasort);
        if (n == -1){
            return;
        }
        int j =0;
        while(j < n) {
            if (dirlist[j]->d_name[0] != '.') {
                if (dirlist[j]->d_type == DT_DIR) {
                    list_dir[listc] = calloc(sizeof(char), 1024);
                    strcat(list_dir[listc], list[i]);
                    strcat(list_dir[listc],"/");
                    strcat(list_dir[listc++], dirlist[j]->d_name);
                    printf("%s ", dirlist[j]->d_name);
                } else {
                    printf("%s ", dirlist[j]->d_name);
                }
            }
            free(dirlist[j]);
            j++;
        }
        printf("\n");
        printf("\n");
        i++;
    }
    free(dirlist);
    //printf("\n");
    Dirrecursor(list_dir,listc);
}
    /*char *buffer = malloc(sizeof(char)*MAXLINE);
    DIR *d;
    struct dirent *directory;
    d = opendir(dir);
    if(!d)
    {
    	//perror(dir);
    	return;
    }
    while((directory = readdir(d))!=NULL){
    	if (directory->d_name[0]!='.')
    	{
    	   printf("%s/%s\n",dir, directory->d_name);
    	   strcpy(buffer, dir);
    	   strcat(buffer, "/");
    	   strcat(buffer,  directory->d_name);
    	   traverseRecursive(buffer);
    	  }
  	}
    closedir(d); */



void pathListing (char *dir, int signal){
    DIR *d = opendir(dir);
    struct dirent *directory;
    strcat(dir,"/");
    printf("%s\n",dir);
    char* buffer = malloc(sizeof(char)*MAXLINE);
    while((directory = readdir(d))!=NULL){
    strcpy(buffer, dir);
    strcat(buffer, directory->d_name);
    	if (signal == 2 && directory->d_name[0]!='.'){
	    getFilePermission(buffer);
	    getAndPrintGroup(buffer);
	    getAndPrintUserName(buffer);
	    printf("%ld\t%s     %s\n",findSize(buffer),date_time(buffer),directory->d_name);
         }
        else if (signal == 3 && directory->d_name[0]!='.'){
            printf("%lu\t", directory->d_ino);
	    getFilePermission(buffer);
	    getAndPrintGroup(buffer);
	    getAndPrintUserName(buffer);
	    printf("%ld\t%s     %s\n",findSize(buffer),date_time(buffer),directory->d_name);
         }

      }
     closedir(d);
}

void singleNameListing(char *dir, int signal){
  struct stat attrib;
  stat(dir, &attrib);
    if(signal==2){
     getFilePermission(dir);
     getAndPrintGroup(dir);
     getAndPrintUserName(dir);
     printf("%ld\t%s     %s\n",findSize(dir),date_time(dir),dir);
    }
  else if(signal ==3){
     printf("%lu\t", attrib.st_ino);
     getFilePermission(dir);
     getAndPrintGroup(dir);
     getAndPrintUserName(dir);
     printf("%ld\t%s     %s\n",findSize(dir),date_time(dir),dir);
    }
  else{
  printf("%lu\t%s\n", attrib.st_ino, dir);
  }
}

int countFiles(const char *dir){
    int count = 0;
    DIR *d = opendir(dir);
    struct dirent *directory;
    while((directory = readdir(d))!=NULL){
    if(directory->d_name[0]!='.'){
        count++;}
    }
  closedir(d);
  return count;
}

void swapFunction(char *string1, char *string2){
    char *buf = malloc(sizeof(char)*MAXLINE);
    strcpy(buf, string1);
    strcpy(string1, string2);
    strcpy(string2, buf);
}

int stringCompare(char *string1, char *string2){
  char *buf = malloc(sizeof(char)*MAXLINE);
  char *buf2 = malloc(sizeof(char)*MAXLINE);
  strcpy(buf, string1);
  strcpy(buf2, string2);
  int i = 0;
  while(buf[i]!='\0' && buf2[i] != '\0'){
    if (buf[i] >='a' && buf[i] <='z'){
    	buf[i] -= 32;
    	}
    if(buf2[i] >='a' && buf2[i] <='z'){
        buf2[i] -= 32;
        }
        i++;
  }
   if (strcmp(buf,buf2) > 0)
   {
   	return 1;
   }
   else if (strcmp(buf,buf2) < 0)
   {
   	return 2;
   }
   else
   {
   	return 0;
   }
}


void sortNames(char *dir)
{   char*names[MAXLINE];
    DIR *d = opendir(dir);
    struct dirent *directory;
    int fileNumbers = countFiles(dir);
    int i =0;
    while((directory = readdir(d))!=NULL)
      {
        if(directory->d_name[0] != '.'){
        names[i] = directory->d_name;
        i++;
        }
      }
    for (int i =0; i < fileNumbers; i++){
    	for (int j =i+1 ; j <fileNumbers; j++){
    	   if(stringCompare(names[i],names[j])==1){
    	      swapFunction(names[i],names[j]);
    	   }
    	}
    }
    for(int i =0; i < fileNumbers; i++){
       printf("%s\n",names[i]);
    }
}

char* date_time(const char *dir)
{
    char *datime = malloc(sizeof(char)*MAXLINE);
    char *datime2 = malloc(sizeof(char)*MAXLINE);
    struct stat attrib;
    stat(dir, &attrib);
    strftime(datime, MAXLINE, "%b %d %Y %T", localtime(&attrib.st_mtime));
    strncpy(datime2,datime, strlen(datime)-3);
    return datime2;

}

long int findSize(const char *dir){
   struct stat st;
   if(stat(dir,&st)==0){
   return (st.st_size);
   }
   else
   	return -1;
}

void getFilePermission(const char *dir){
   struct stat perm;
   stat(dir, &perm);
   char *cPerm = malloc(sizeof(char)*MAXLINE);
   for (int i =0; i < 10;i++){
     if (i == 0){
     	cPerm[i]=(perm.st_mode & S_IRUSR) ?'r':'-';
     }
     else if(i==1){
     	cPerm[i]=(perm.st_mode & S_IWUSR) ?'w':'-';
     }
     else if(i==2){
     	cPerm[i]=(perm.st_mode & S_IXUSR) ?'x':'-';
     }
     else if(i==3){
     	cPerm[i]=(perm.st_mode & S_IRGRP) ?'r':'-';
     }
     else if(i==4){
     	cPerm[i]=(perm.st_mode & S_IWGRP) ?'w':'-';
     }
     else if(i==5){
     	cPerm[i]=(perm.st_mode & S_IXGRP) ?'x':'-';
     }
     else if(i==6){
     	cPerm[i]=(perm.st_mode & S_IROTH) ?'r':'-';
     }
     else if(i==7){
     	cPerm[i]=(perm.st_mode & S_IWOTH) ?'w':'-';
     }
     else if(i==8){
     	cPerm[i]=(perm.st_mode & S_IXOTH) ?'x':'-';
     }
   }

   printf("%s %lu  ", cPerm,perm.st_nlink);
}


int main (int argc, char *argv[]){
    int signal =0;
    if(argc == 1)
      {
      	cur_dirent_list(".",signal);
      }

    int signalI ,signalL ,signalR , nonOption ;
    for(int i = 1; argv[i]!=NULL;i++){
        // case of the not an option i.e doesnt have '-'
        if((argv[i])[0]!='-'){
             nonOption = i;
        }
        else {
            int j = 1;
            while(j < strlen(argv[i])) {
                char x = (argv[i])[j];
                if (x =='i') {
                    signalI = 1;
                } else if (x == 'l') {
                    signalL = 1;
                } else if (x == 'R') {
                    signalR = 1;
                }else
                {
                    printf("Invalid Input\n");
                    exit(1);
                }
                j++;
            }
        }
    }
    if(signalI == 1 && signalL == 0 && signalR ==0)
    {
      signal = 1;
      cur_dirent_list(".",signal);
    }
    else if(signalI == 0 && signalL == 1 && signalR ==0)
    {
      signal = 2;
      cur_dirent_list(".",signal);
    }
    else if(signalI == 1 && signalL == 1 && signalR ==0)
    {
      signal = 3;
      cur_dirent_list(".",signal);
    }
    else if(signalI == 0 && signalL == 0 && signalR ==1)
    {
      signal = 4;
      traverseRecursive(".");
    }
    else if(signalI == 1 && signalL == 0 && signalR ==1)
    {
      signal = 5;
    }
    else if(signalI == 0 && signalL == 1 && signalR ==1)
    {
      signal = 6;
    }
    else if(signalI == 0 && signalL == 0 && signalR ==1)
    {
      signal = 7;
    }
  }


  /*  if(argc == 1)
    {
    	cur_dirent_list(".",signal);
    }
    else if (argc == 2)
    {
	if (argv[1][0] == '-'&& strlen(argv[1]) == 2){
		if(argv[1][1] == 'i'){
		   signal = 1;
		   cur_dirent_list(".",signal);
		}
		else if (argv[1][1] == 'l'){
		   signal = 2;
		   cur_dirent_list(".",signal);
		}
		else if (argv[1][1] == 'R'){
		   traverseRecursive("/home/john");
		}
    	}
    	else if(argv[1][0] == '-' && strlen(argv[1]) == 3){
    		if(argv[1][1] =='i' && argv[1][2] =='l'){
		   signal = 3;
		   cur_dirent_list(".",signal);
		}
		else if(argv[1][1] =='l' && argv[1][2] =='i'){
		   signal = 4;
		   cur_dirent_list(".",signal);
		}
    	}
    	else{
    	    char *buf = malloc(sizeof(char)*MAXLINE);
    	    strcpy(buf, argv[1]);
    	    cur_dirent_list(buf,signal);
    	}
    }
    else if(argc == 3)
    {
    	char *buf = malloc(sizeof(char)*MAXLINE);
    	char *buf2 = malloc(sizeof(char)*MAXLINE);
    	char *buf3;
    	strcpy(buf2,argv[1]);
    	strcpy(buf, argv[2]);
    	buf3 = strchr(buf, '/');
    	if (buf3 != NULL){
    	if (strcmp(buf2,"-i") == 0){
    	   signal =1;
    	   cur_dirent_list(buf, signal);
    	}
    	else if (strcmp(buf2,"-l") == 0)
    	{
	    signal =2;
	    pathListing(buf, signal);
	}
	else if (strcmp(buf2,"-il") == 0)
	{
	   signal =3;
	   pathListing(buf, signal);
	}
	else if (strcmp(buf2,"-li") == 0)
	{
	   signal =3;
	   pathListing(buf, signal);
	}
	}
	else{
	    if (strcmp(buf2,"-i") == 0){
    	   signal =1;
    	   singleNameListing(buf,signal);
    	   }
    	   else if (strcmp(buf2,"-l") == 0)
    	   {
	    signal =2;
	    singleNameListing(buf,signal);
	   }
	   else if (strcmp(buf2,"-il") == 0 || strcmp(buf2,"-li") == 0 )
	   {
	   signal =3;
	   singleNameListing(buf,signal);
	   }
	  }
     }
 }  */


  /* struct dirent **directory;
    int n = scandir("/home/john", &directory, NULL, alphasort);
    if(n < 0){perror("scandir");}
    else
    {
    	for(int i = 0; i < n; i++){
    	if(signal == 0){
	    printf("%s\n",directory[i]->d_name);
	    free(directory[i]);
	    }
    	}
    }*/
    //free(directory);
