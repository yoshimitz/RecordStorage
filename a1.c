/* a1.c 
   Yashar Nesvaderani
   Program takes in user input and displays it.
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define LINESIZE 512

void append(FILE*);
void display(int, FILE*);
void displayAll(int, FILE*);
int isID(const char[]);
int isEmpty(const char[]);
int isName(const char[], const char[]);

/* main functions that displays options and takes user input */
int main(int argc, char* argv[]) {
    
    FILE *fp;
    char line[LINESIZE];
    int n;
    
    if (argc < 2) {
        fprintf(stderr, "usage: %s [file]\n", argv[0]);
        return 1;
    }
    
    if ((fp = fopen(argv[1], "wb+")) == 0) {
        perror("fopen");
        return 2;
    }
    
    while (1) {
        
         fprintf(stderr, "Enter your input:\n0 to add new record.\npositive integer to display specific record\nnegative integer to display range of records\n");
        
        if (!fgets(line, LINESIZE, stdin)) {
            clearerr(stdin);
            break;
        }

        if (sscanf(line, "%d", &n) == 1){
            if (n > 0) {
                display(n, fp);
            } else if (n < 0) {
                displayAll(n, fp);
            } else if (n == 0) {
                append(fp);
            }
        }
    }
    
    if (fclose(fp) != 0) {
        perror("fclose");
        return 3;
    }
    
    
    return 0;
}

/* appends records to file if valid input given */
void append(FILE *a) {
    char line [LINESIZE];
    char id[LINESIZE];
    char name[LINESIZE];
    char first[21];
    char last[21];
    char mark[LINESIZE];
    int score;
    size_t i;
    
    fseek(a, 0, SEEK_END);
    
    while (1) {
        fprintf(stderr, "Enter the student's id:\n");

        if (!fgets(line, LINESIZE, stdin) || isEmpty(line)) {
            clearerr(stdin);
            return;
        }
        
        if (sscanf(line, "%s", id) == 1) {
            if (isID(id)) {
            break;
            }   
        }
        
    }
    
    fseek(a, 0, SEEK_END);
    
    while(1) {
        fprintf(stderr, "Enter the student's name:\n");

        if (!fgets(name, LINESIZE, stdin) || isEmpty(name)) {
            clearerr(stdin);
            return;
        }
              
        if (sscanf(name, "%s%s", first, last) == 2) {
            
            strtok(last, "\n");
            
            if (isName(first, last)) {
                for (i = 0; first[i] != '\0'; i++) {
                    first [i] = tolower(first[i]);
                }
                for (i = 0; last[i] != '\0'; i++) {
                    last[i] = tolower(last[i]);
                }
                break;
            }
        }
    }
    
    fseek(a, 0 , SEEK_END);
    
    while(1) {
        fprintf(stderr, "Enter the student's score:\n");
        
        if (!fgets(mark, LINESIZE, stdin) || isEmpty(mark)) {
            clearerr(stdin);
            return;
        }
        
        if (sscanf(mark, "%d", &score) == 1) {
            if (score >= 0 && score <= 100) {
                break;
            }
        }
    }
    
    fprintf(a, "%-10s", id);
    fprintf(a, "%-20s%-20s", first, last);
    fprintf(a, "%-4d", score);
}

/* displays specified record */
void display(int n, FILE *b) {
    char line[LINESIZE];
    char id[10];
    char first[21];
    char last[21];
    int score;
    
    fseek(b, (n - 1) * 54 , SEEK_SET);
    
    if(!fgets(line, 55, b)) {
        n = 0;
    } else {
        n = 1;
    }
    
    fseek(b, (n - 1) * 54 , SEEK_SET);

    if (sscanf(line, "%s%s%s %d", id, first, last, &score) == 4) {
        printf("%s : %s, %s : %d\n", id, last, first, score);
        fflush(stdout);
    }
    
    printf("%d\n", n);
    fflush(stdout);
}

/* displays specified range of records */
void displayAll(int n, FILE *c) {
    char line[LINESIZE];
    char id[10];
    char first[21];
    char last[21];
    int score;
    
    n = n * -1;
    
    fseek(c, (n - 1) * 54 , SEEK_SET);
    
    
    if(!fgets(line, 55, c)) {
        printf("%d\n", 0);
        return;
    }
    

    fseek(c, (n - 1) * 54 , SEEK_SET);
    
    n = 0;
    while(fgets(line, 55, c)){
        if (sscanf(line, "%s%s%s %d", id, first, last, &score) == 4) {
            printf("%s : %s, %s : %d\n", id, last, first, score);
        }
        n++;
    }
    printf("%d\n", n);
    fflush(stdout);
}

/* checks if ID is valid */
int isID(const char id[]) {
    size_t i;
    
    if (id[0] != 'a') {
        return 0;
    } 
    
    for (i = 1; i < 9; i++) {
        if (!isdigit(id[i])) {
            return 0;
        }
    }
    
    return 1;
}

/* checks if name is valid */
int isName (const char first[], const char last[]) {
    size_t i;
    
    if (first[0] == '-' || last[0] == '-'){
        return 0;
    }
    
    for (i = 0; first[i] != '\0'; i++) {
        if (!isalpha(first[i]) && first[i] != '-') {
            return 0;
        }
    }
    
    if (i < 2 || i >= 20) {
        return 0;
    }
    
    if (first[i - 1] == '-') {
        return 0;
    }
    
    for (i = 0; last[i] != '\0'; i++) {
        if (!isalpha(last[i]) && last[i] != '-') {
            return 0;
        }
    }
    
    if (i < 2 || i >= 20) {
        return 0;
    }
    
    if (last[i - 1] == '-') {
        return 0;
    } 
    
    
    return 1;
}

/* checks if input line is empty */
int isEmpty(const char c[]) {
    size_t i = 0;
    
    while (c[i] != '\0') {
        if (!isspace(c[i])) {
            return 0;
        }
        i++;
    }
    return 1;
}