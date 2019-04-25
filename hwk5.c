/*  
*  Name:       Parikshit Sharma 
*  Login:      SP_19_CPS536_12
*  Purpose:    C program to list through directories and sub directories while printing files > size provided
*  Bug report: No bugs reported, and all features were implemented
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#define SIZE 1024

//Initialize modular function to scan directories recursively
void dir_scan(char *, size_t );

//Declare global variables
int size_limit, cnt = 0;

int main(int argc, char* argv[]) {
    char path[SIZE];
    char d[] = "/";
    char *p;

    //Check for correct usage
    if (argc != 3){
        fprintf(stderr, "Usage <program-name> <root> <limit>\n");
        return(-1);
    }
    
    //Assign size limit
    size_limit = atoi(argv[2]);
    size_limit *= 1024;

    //Print the number of links with size greater than limit
    fprintf(stderr, "Links with size >= %d\n", size_limit);
    
    //Check if path is relative or absolute
    p = strstr(argv[1], d);

    //If relative path
    if(p){
        dir_scan(argv[1], sizeof path);
    }
    //If path is absolute - first char match to "/"
    else{
        char cwd[PATH_MAX];
        if(getcwd(cwd, sizeof(cwd)) != NULL) {       
                strcat(cwd, "/");
                strcat(cwd, argv[1]);
            }
        dir_scan(cwd, sizeof path);
    }

    //Print the counter
    fprintf(stderr, "Total number of links found = %d\n", cnt);
    return 0;
}

void dir_scan(char *p, size_t s) {
    DIR *dir;
    struct dirent *parent;
    struct stat temp;
    size_t l = strlen(p);

    //Check if path exists
    if (!(dir = opendir(p))){
            fprintf(stderr, "path not found: %s: %s\n", p, strerror(errno));
            return;
        }

    //Iterate through contents of the directory
    while ((parent = readdir(dir)) != NULL) {
        char *n = parent->d_name;
        char buffer[512];
        
        //If directory is found
        if (parent->d_type == DT_DIR){
            if(!strcmp(n, ".") || !strcmp(n, ".."))
                continue;
            if(l + strlen(n) + 2 > s) {
                    fprintf(stderr, "Path is too long: %s/%s\n", p, n);
                }
            else{   
                    //Append path
                    p[l] = '/';
                    strcpy(p + l + 1, n);

                    //Call function recursively
                    dir_scan(p, s);
                    p[l] = '\0';
                }
            }
        
        else{
            //Copy tp temp
            sprintf(buffer, "%s/%s",p, n);
            stat(buffer, &temp);

            //Check if greater than size limit and print
            if (temp.st_size >= size_limit){   
                printf("%zu\t",temp.st_size);
                printf("%s/%s\n", p, n);

                //Increment counter
                cnt++;
            }
        }
    }
    //Close directory and handle error
    if (closedir(dir) == -1) {
        perror("Error closing directory");
        exit(1);
    }
}
