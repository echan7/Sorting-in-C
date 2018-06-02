#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <limits.h>
/* See `man 3 getopt()` for how to use the getopt function. */
#include <getopt.h>
#define error(s) {perror((s)); exit(EXIT_FAILURE);}
#define MALLOC(s,t) {if(((s) = malloc(t)) == NULL) {error("cant be allocated");}}
#define INCREMENT 10

 int cstring_cmp(const void *a, const void *b)
            {
                const char *ia = (const char *)a;
                const char *ib = (const char *)b;
                return strcmp(ia, ib);
                /* strcmp functions works exactly as expected from
                comparison function */
            }

int cstring_revCmp(const void *a, const void *b)
            {
                const char *ia = (const char *)a;
                const char *ib = (const char *)b;
                return strcmp(ib, ia);
                /* strcmp functions works exactly as expected from
                comparison function */
            }

int main(int argc, char** argv)
{
	/* Your code goes here! */
	extern char *optarg;
	extern int optind;
	char* lines;
	int c;
	int rflag = 0;
	int nflag = 0;
	long value = 0;
	char *endptr;
	
	while((c = getopt(argc, argv, "rn:")) != -1){
		switch(c){
		case 'r':
			rflag = 1;
			/*do this stuff in reverse*/
			break;
		case 'n':
			lines =  optarg;
			nflag = 1;
			errno = 0;
			value = strtol(lines, &endptr, 10); 
			//printf("numlines: %d\n", (int)value);
			if ((errno == ERANGE && (value == LONG_MAX || value == LONG_MIN))
                  	 || (errno != 0 && value == 0)) {
              		 	perror("strtol");
              		 	exit(1);
          		 }
			if(endptr == lines){
				fprintf(stderr, "No digits were found\n");
				exit(1);
			}
			if(*endptr != '\0'){
				printf("There are further characters after number: %s\n", endptr);
				exit(1);
			}
			break;
		case '?':
			if(optopt == 'c'){
				fprintf(stderr, "-%c requires an argument.\n", optopt);
			}else if(isprint (optopt)){
				fprintf(stderr, "invalid option -%c.\n", optopt);
			}else{
				printf("error for invalid argument\n");
			}
			exit(1);
			break;	
		default:
			abort();
		}
	}
	
	//read file
	FILE *fp;
	if(optind == argc){
		fp = stdin;
	}else{
		fp = fopen(argv[optind], "r");
		if(fp == NULL){
			printf("Unreadable");
			exit(1);
		}
	}
	
	char buffer[1024];
	char **sort;
	int meow = 0;
	int temporary = 0;
	int incrementCounter = 1;
	
	//alloc an array after reading text
		int i;
		MALLOC(sort, sizeof(char *) * INCREMENT);
		for(i =0; i< INCREMENT; i++){
			MALLOC(sort[i], sizeof(char) * 1024);
		}

		char **tmp;
		int length = 0;
		int n = 10;
		
		int j =0;
		for(j = 0; j<=n && meow ==0; j++){
			char* ret = fgets(buffer, 1024, fp);
			if(ret == NULL){
				meow = 1;
			}else{
				strcpy(sort[j], buffer);
				length++;
			}

			if(length >= n){
				tmp = realloc(sort, sizeof(char*)*(n+INCREMENT));
				if(tmp == NULL){
					error("realloc failed");
				}
				int y;
				for(y =n; y< n+ INCREMENT; y++){
					MALLOC(tmp[y], sizeof(char) * 1024);
				}
				n += INCREMENT;
				incrementCounter++;
				sort = tmp;
			}
		}
		temporary = value;
		value = length;
	
	//choosing how to sort
	char tmpList[value][1024];
	int l;
	for(l =0; l < value; l++){
		if((strcpy(tmpList[l], sort[l]) == NULL)){
			printf("strcpy error");
		}
	}
	size_t stringLength = value;
	if(rflag == 0){
		qsort(tmpList, stringLength, 1024,cstring_cmp);
	}else{
		qsort(tmpList, stringLength, 1024, cstring_revCmp);
	}
	//choosing how to print
	int m;
	if(nflag == 0){
		for(m=0; m<value; m++){
			printf("%s", tmpList[m]);
		}
	}else{
		int g;
		for(g=0; g<temporary; g++){
			printf("%s", tmpList[g]);
		}
	}
 	
	int p;
	for(p =0; p < INCREMENT*incrementCounter; p++){
		free(sort[p]);
	}
	free(sort);
	
	/* If successful, exit with a zero status. */
	return 0;
}
