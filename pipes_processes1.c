// C program to demonstrate use of fork() and pipe() 
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
  
int main() 
{ 
    // We use two pipes 
    // First pipe to send input string from parent 
    // Second pipe to send concatenated string from child 
  
    int fd1[2];  // Used to store two ends of first pipe 
    int fd2[2];  // Used to store two ends of second pipe 
  
    char fixed_str[] = "howard.edu";
    char fixed_str2[] = "gobison.org";
    char input_str[100];
    char input_str2[100];
    char concat_str[100];
    char concat_str2[100];
    pid_t p; 
  
    if (pipe(fd1)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
    if (pipe(fd2)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
  
    printf("First Call: Enter a string to concatenate:");
    scanf("%s", input_str);
    input_str[101] = '\0';
    p = fork(); 
  
    if (p < 0) 
    { 
        fprintf(stderr, "fork Failed" ); 
        return 1; 
    } 
  
    // Parent process 
    else if (p > 0) 
    { 
        close(fd1[0]);  // Close reading end of pipes   
        // Write input string and close writing end of first 
        // pipe. 
        write(fd1[1], input_str, strlen(input_str)+1); 

        // Wait for child to send a string 
        wait(NULL);

        read(fd2[0], concat_str2, 100);

        printf("Parent Process Concatenated string: %s\n", concat_str2);

    } 
  
    // child process 
    else
    { 
        close(fd1[1]);  // Close writing end of first pipes 
        close(fd2[0]);

        // Read a string using first pipe 
        char concat_str[100]; 
        read(fd1[0], concat_str, 100); 
        strcat(concat_str, fixed_str); // Concatenate a fixed string with it 
        
        printf("Child Process Concatenated string: %s\n", concat_str);
        
        printf("Enter a string to concatenate: ");
        scanf("%s", input_str2);
        strcat(concat_str, input_str2);
        
        write(fd2[1], concat_str, strlen(concat_str)+1);
        close(fd2[0]);
  
        exit(0); 
    } 
} 