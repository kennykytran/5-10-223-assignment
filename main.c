#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>
#include <pwd.h>

#define MAX_PATH 1024

#ifndef DIRSIZ
#define DIRSIZ 14
#endif

typedef long Align;

union header {
    struct {
        union header *ptr;
        unsigned size;
    } s;
    Align x;
};

typedef union header Header;

struct wtbheader {
    struct wtbheader *next;
    void *p;
    unsigned n;
};

void dirwalk( char *dir,void (*fcn)(char *)){

	char name[MAX_PATH];
	struct dirent *dp;
	DIR *dfd;

	if((dfd = opendir(dir))==NULL){
		fprintf(stderr, "dirwalk: can't open %s\n", dir);
		return;
	}

	puts(dir);

	while((dp=readdir(dfd)) != NULL){
		if(strcmp(dp->d_name,".") == 0 || strcmp(dp->d_name,"..") ==0 ) continue;
		if(strlen(dir)+strlen(dp->d_name)+2 > sizeof(name))
			fprintf(stderr, "dirwalk: name %s %s too long\n", dir, dp->d_name);
		else{
			sprintf(name, "%s/%s",dir,dp->d_name);
			(*fcn)(name);
		}
	}

	closedir(dfd);
}
//8.5
void fsize(char *name){
	struct stat stbuf;

	if(stat(name,&stbuf) == -1){
		fprintf(stderr, "Could not access %s\n", name);
		return;
	}

	if((stbuf.st_mode & S_IFMT) == S_IFDIR){
		dirwalk(name,fsize);}
	struct passwd *pwd = getpwuid(stbuf.st_uid);
	
	printf("%d %s Data: %s\n",(int)stbuf.st_size, name, pwd->pw_name);
}

//8.6
void *mycalloc(size_t n,size_t size){
    size_t x;
    char *ptr, *result;
    x = n * size;
    
    if((ptr = result = malloc(x)) != NULL){
        for(size_t i = 0; i <x; i++) *ptr++ = 0;
        }
    
    return result;
}


int main(int argc,char *argv[]) {
  int *ptr;

  //8.5
  if(argc==1) fsize(".");
	else{ while(--argc>0) fsize(*++argv);}
  printf("\n");

  //8.6
  ptr = mycalloc(100, sizeof *ptr);
  
  if(ptr == NULL) printf("returned NULL");
  else{
    for(int i = 0; i<=100; i++){
      printf("%08X", ptr[i]);
      if (i%8 == 7) printf("\n\n");
    }
    printf("\n");
    
    free(ptr);
  }

  //8.8
  return 0;
}
