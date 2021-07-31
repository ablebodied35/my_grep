#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MAX_FILE 20000
#define MAX_CHAR 2000

char* PATTERN;
int lines_searched = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void * searchfile (void * filename);

int main(int argc, char* argv[]){
	if(argc < 3 || argc > MAX_FILE){
		perror("Arguments must be >3 and less than 20000");
		exit(EXIT_FAILURE);
	}

	pthread_t * thread_pointer;
	int ret;
	void * void_ptr;
	int tot_lines = 0;
	
	PATTERN = malloc(strlen(argv[1]));
	if( PATTERN == NULL){
		perror("Malloc call failed for PATTERN array. Exiting...");
		exit(EXIT_FAILURE);
	}
	strcpy(PATTERN,argv[1]);
	thread_pointer = malloc((argc - 2) * sizeof (pthread_t));	
	if( thread_pointer == NULL){
                perror("Malloc call failed for thread_pointer array. Exiting...");
                exit(EXIT_FAILURE);
        }

	for(int i = 0; i <(argc - 2);i++){
		ret = pthread_create(&thread_pointer[i],NULL,searchfile,(void*)argv[i+2]);
		if(ret == -1){
			perror("Error at pthread_create(). Exiting...");
			exit(EXIT_FAILURE);
		}
	}

	for(int i =0; i<(argc - 2);i++){
		ret = pthread_join(thread_pointer[i],&void_ptr);
		if(ret == -1){
			perror("Error at pthread_join(). Exiting...");
			exit(EXIT_FAILURE);
		}
		printf("[main] Thread %d returned with value %d\n",i,ret);
		tot_lines = tot_lines + (int)void_ptr; 
	}
	
	printf("[main] Total of %d matched lines among %d lines scanned\n",tot_lines,lines_searched);
	free(thread_pointer);
	free(PATTERN);
	exit(EXIT_SUCCESS);
}


void * searchfile (void * filename){
        char* ptr;
        int buffer_length;
        int pattern_lines=0;
        FILE *file;
        ptr = malloc(MAX_CHAR);
	if(ptr== NULL){
		perror("Malloc call failed for ptr in Searchfile function. Exiting...");
		exit(EXIT_FAILURE);
	}
        file = fopen((char*) filename,"r");
        if(file == NULL){
		perror("File not found. Exiting...");
		exit(EXIT_FAILURE);
	}
	char* ret; //Return value for the strstr() function
        while(fgets(ptr,MAX_CHAR,file)!= NULL){
                pthread_mutex_lock(&mutex);
                lines_searched++;
                pthread_mutex_unlock(&mutex);
                buffer_length = strlen(ptr);
                if(buffer_length > MAX_CHAR){
                        perror("Buffer length too long. Exiting...");
                        exit(EXIT_FAILURE);
                }
                if(buffer_length == 0)
                        continue;
                ret = strstr(ptr,PATTERN);
                if (ret != NULL){
                        printf("%s: %s",(char*)filename,ptr);
                        pattern_lines++;
                }
        }

        printf("--%s has %d matched lines\n",(char*)filename,pattern_lines);
        free(ptr);
        fclose(file);
        pthread_exit((void*)pattern_lines);

}
