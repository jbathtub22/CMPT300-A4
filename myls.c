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
#define MAXLINE 100000
char* date_time(const char *dir);
long int findSize(const char *dir);
int is_hidden(const char *name);
void getAndPrintGroup(char *dir);
void getFilePermission(const char *dir);
int countFiles(const char *dir);
void sortNames(char *dir);
int stringCompare(char *string1, char *string2);
void getdino(const char *dir, char*name);
void reccurFunc(char *dir, int signal);
void pathListing (char *dir, int signal);
void pathListingR(char *dir, char *dname);
void cur_dirent_list(const char *dir, int signal)
{
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
    	if(signal == 0 && dir_name[0]!='.'){
	    printf("%s\n",dir_name);}
	else if(signal == 1 && dir_name[0]!='.'){
	    printf("%lu\t%s\n",directory->d_ino, dir_name);
	    }
	else if(signal == 2 && dir_name[0]!='.'){
	    getFilePermission(dir_name);
	    getAndPrintGroup(dir_name);
	    getAndPrintUserName(dir_name);
	    printf("%ld     %s     %s\n",findSize(dir_name),date_time(dir_name),dir_name);
	    }
	else if((signal == 3 && dir_name[0]!='.') || (signal == 4 && dir_name[0]!='.')){
            printf("%lu\t", directory->d_ino); 
	    getFilePermission(dir_name);
	    getAndPrintGroup(dir_name);
	    getAndPrintUserName(dir_name);
	    printf("%ld\t%s     %s\n",findSize(dir_name),date_time(dir_name),dir_name);
	    }
     }
    closedir(d);
}
}
void reccurFunc(char *dir, int signal){
    char *buffer = malloc(sizeof(char)*MAXLINE); 
  
	struct dirent **dirr;
	int n = scandir(dir,&dirr, NULL, alphasort);
	if(n<0){perror("scandir");}
	int i =0;
	while(i<n){
	struct dirent *directory = dirr[i]; 
    
    	if (directory->d_name[0]!='.')
    	{
			int isDir = 0;
    	   if(directory->d_type == DT_DIR)
           {
			  isDir = 1;
    	   }
    	   
    	      if(signal == 5 || signal == 9){
              printf("%lu\t", directory->d_ino); 
    	      printf("%s\n ", directory->d_name); 
    	      }
    	      else if(signal == 7){
    	      
    	      getFilePermission(directory->d_name);
	      getAndPrintGroup(directory->d_name);
	      getAndPrintUserName(directory->d_name);
	      printf("%ld\t%s\t%s\n",findSize(directory->d_name),date_time(directory->d_name),directory->d_name);
	    	  
	    	}
	      else if(signal == 8){
	      printf("%lu\t", directory->d_ino); 
	      getFilePermission(directory->d_name);
	      getAndPrintGroup(directory->d_name);
	      getAndPrintUserName(directory->d_name);
	      printf("%ld\t%s\t%s\n",findSize(directory->d_name),date_time(directory->d_name),directory->d_name);
	      }
	      else if(signal == 10){
	      if (isDir ==1){
    	      	 printf("%s/%s: ",dir, directory->d_name);}
    	      else{
	      pathListingR(dir,directory->d_name);
	      }
	      }
	      else if(signal == 11){
	      if (isDir ==1){
    	      	 printf("%s/%s: ",dir, directory->d_name);}
    	      else{
	      printf("%lu\t", directory->d_ino); 
	      pathListingR(dir,directory->d_name);
	          }
	      }
    	      else {
    	      	 if (isDir ==1){
    	      	 printf("%s/%s: ",dir, directory->d_name);}
    	      	 else{
    	      	 printf("%s  ", directory->d_name); 
    	      	 printf("\n");}
    	      }
    	   //} 
	      if(isDir == 1){
				printf("%s/%s: ",dir, directory->d_name);
    	   		strcpy(buffer, dir); 
    	   		strcat(buffer, "/"); 
    	   		strcat(buffer,  directory->d_name);
    	   		printf("\n");
			reccurFunc(buffer, signal); 
	       }
    	  }	
		  i++;
     }
    
	free(buffer);
	free(dirr);
}
void pathListingR(char *dir, char *dname){
	char *buffnine = malloc(sizeof(char)*MAXLINE);
	strcpy(buffnine,dir);
	strcat(buffnine,"/");
	strcat(buffnine, dname); 
    	getFilePermission(buffnine);
	getAndPrintGroup(buffnine);
	getAndPrintUserName(buffnine);
	printf("%ld\t%s\t%s\n",findSize(buffnine),date_time(buffnine),dname);
}

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


int irlSignal(char *sig){
    if(strcmp(sig,"iRl") ==0 ||strcmp(sig,"ilR")==0||strcmp(sig,"ilR")==0||strcmp(sig,"lRi")==0||strcmp(sig, "Ril")==0||strcmp(sig,"Rli")==0){
       return 1; 
    }
    else{
	    if((sig[0] == 'i' && sig[1] == 'l') ||(sig[0] == 'l' && sig[1] == 'i')){
	       return 2; 
	    }
	    
	    else if((sig[0] == 'i' && sig[1] == 'R') ||(sig[0] == 'R' && sig[1] == 'i')){
	       return 3; 
	    }
	    
	    else {
	       return 4; 
	    }
    }
}
char* extractString(char *extr){
           int count = 0; 
           char *extr3 = malloc(sizeof(char)*MAXLINE);
           char *extr2 = malloc(sizeof(char)*MAXLINE);  
           for(int i =0; i < strlen(extr); i++){
              if(extr[i] != ' ' && extr[i]!= '-'){
                 extr2[count++] = extr[i]; 
              }
           }
	   strncpy(extr3, &extr2[6], 3); 
	   return extr3; 
}

int checkDash(char *space){
    int count = 0;
    for(int i =0; i < strlen(space); i++){
    	if(space[i]=='-'){
    	   count++;
    	}
    }
    if (count == 2){return 1;}
    else{return 0;}

}
int main (int argc, char *argv[]){  
    int signal = 0; 
	//For throwing exception
	/*char*bufParse = malloc(sizeof(char)*MAXLINE); 
	for (int i = 2; i <= argc; i++)
	  { 
	     strcat(strcat(bufParse, " "), argv[i]); 
	  }
	char  things[] = "-/~."; 
	char * exceptionStr = strpbrk(bufParse ,things);
	if(exceptionStr == NULL)
	{
		printf("Exiting the program as user input is incorrect....\n");
   		exit(0);
	}*/
    
	
	if(argc == 1)
    {
    	cur_dirent_list(".",signal);
    }
    
    else if (argc == 2)
    {
	if (strlen(argv[1]) == 2 && strcmp(argv[1],"..") != 0 ){
		if(strcmp(argv[1],"-i")==0){
		   signal = 1; 
		   cur_dirent_list(".",signal);
		}
		else if (strcmp(argv[1],"-l")==0){
		   signal = 2;
		   cur_dirent_list(".",signal);
		}
		else if (strcmp(argv[1],"-R")==0){
		   signal = 6; 
		   reccurFunc(".", signal);
		}
    	else{
			printf("Incorrect option chosen ...\n");
		}
		}
    	else if((strlen(argv[1]) == 3 || strlen(argv[1]) == 4) && argv[1][0] =='-'){
    		if(strcmp(argv[1],"-il")==0){
		   signal = 3;
		   cur_dirent_list(".",signal); 
		}
		else if(strcmp(argv[1],"-li")==0){
		   signal = 4;
		   cur_dirent_list(".",signal); 
		}
		else if(strcmp(argv[1],"-iR")==0 ||strcmp(argv[1],"-Ri")==0 ){
		   signal = 5; 
		   reccurFunc(".", signal);
		}
		else if(strcmp(argv[1],"-lR")==0 ||strcmp(argv[1],"-Rl")==0 ){
		   signal = 7; 
		   reccurFunc(".", signal);
		} 
		else if(strcmp(argv[1],"-ilR")==0 ||strcmp(argv[1],"-iRl")==0 || strcmp(argv[1],"-Ril")==0 || strcmp(argv[1],"-Rli")==0 || strcmp(argv[1],"-liR")==0 || strcmp(argv[1],"-lRi")==0){
		   signal = 8; 
		   reccurFunc(".", signal);
		
		}
		else{
			printf("Incorrect option chosen ...\n");   
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
    char*bufParse = malloc(sizeof(char)*MAXLINE); 
	for (int i = 0; i < argc; i++)
	  { 
	     strcat(strcat(bufParse, " "), argv[i]); 
	  }
	
	int dash = checkDash(bufParse); 
    char *buf = malloc(sizeof(char)*MAXLINE);
    char *buf2 = malloc(sizeof(char)*MAXLINE);
    char *buf3;
    strcpy(buf2,argv[1]);
    strcpy(buf, argv[2]);
    buf3 = strchr(buf, '/');
    if(dash == 0){     
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
		else if (strcmp(buf2, "-R")==0){
	    	signal = 6; 
	    	reccurFunc(buf, signal); 
		}
		else if (strcmp(buf2,"-il") == 0 || strcmp(buf2,"-li") == 0)
		{
	   		signal =3; 
	   		pathListing(buf, signal); 
		}
		else if (strcmp(buf2,"-iR")==0 || strcmp(buf2, "-Ri")==0){
	   		signal = 9;
	   		reccurFunc(buf, signal); 
		}
		else if(strcmp(buf2,"-lR")==0 || strcmp(buf2, "-Rl")==0){
	   		signal = 10; 
	   		reccurFunc(buf, signal); 
		}
		else if(strcmp(argv[1],"-ilR")==0 ||strcmp(argv[1],"-iRl")==0 || strcmp(argv[1],"-Ril")==0 || strcmp(argv[1],"-Rli")==0 || strcmp(argv[1],"-liR")==0 || strcmp(argv[1],"-lRi")==0){
	   		signal = 11; 
	   		reccurFunc(buf,signal);
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
	 
	if(argv[1][0] != '-'){
	  char *buflist = malloc(sizeof(char)*MAXLINE);
	  for (int i =1; i <argc; i++){
    	           strcpy(buflist, argv[i]); 
    	           printf("%s:\n", buflist);
    	           cur_dirent_list(buflist,signal); 
    	           printf("\n"); 
    	        }
	   }
	}
	
	
	//option with space
    else{
        char *bufExtract2 = malloc(sizeof(char)*MAXLINE);  
		bufExtract2 = extractString(bufParse);
		int signalIRL = irlSignal(bufExtract2);
        if (signalIRL == 1){
	   		signal = 8;
	   		reccurFunc(".", signal); 
	  	}
	  	else if(signalIRL ==2){
	  		signal = 3;
	  		cur_dirent_list(".",signal);
	  	}
	  	else if(signalIRL ==3){
	  		signal = 9;
	  		reccurFunc(".",signal);
	  	}
	  	else if(signalIRL ==4){
	  		signal = 10;
	  		reccurFunc(".",signal);
	  	}
       }
    } 
      
      
     else if(argc > 3){
		if (argv[1][0]!='-'){
		char *buflist = malloc(sizeof(char)*MAXLINE);
		for(int i = 1; i < argc; i++){
			strcpy(buflist, argv[i]);
			printf("%s:\n", buflist);
			cur_dirent_list(buflist, signal);
			printf("\n");
		   }
	    }
		else{
    	char*bufParse = malloc(sizeof(char)*MAXLINE); 
	   	for (int i = 0; i < argc; i++)
	   	{ 
	   		strcat(strcat(bufParse, " "), argv[i]); 
	   	}
    	char *bufExtract2 = malloc(sizeof(char)*MAXLINE);  
	   	bufExtract2 = extractString(bufParse);
	   	int signalIRL = irlSignal(bufExtract2);
		char *buf = malloc(sizeof(char)*MAXLINE);
    	char *buf3;
    	strcpy(buf, argv[argc-1]);
    	buf3 = strchr(buf, '/');
		if (buf3 != NULL){
    	 	if (signalIRL == 1){
	   			signal = 11;
	   			reccurFunc(buf, signal); 
	  		}
	  		else if(signalIRL ==2){
	  			signal = 3;
	  			pathListing(buf,signal);
	  		}
	  		else if(signalIRL ==3){
	  			signal = 9;
	  			reccurFunc(buf,signal);
	  		}
	  		else if(signalIRL ==4){
	  			signal = 10;
	  			reccurFunc(buf,signal);
	  		} 
	 	}
    	else{
	   		if (signalIRL == 1){
	   			signal = 8;
	   			reccurFunc(".", signal); 
	   		} 
          }
		}  
    }
}               

