#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>
#include <unistd.h>

int error(char text[])
{
    fprintf(stderr, "%s\n", text);
    exit(1);
}

char *concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); //+1 for the null-terminator
    //in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

bool host_entry_already(char host[], FILE *file_pointer)
{
    char buffer[255];
    regex_t regex;
    char *pattern = concat("[ ]*", host);
    int compiled_regex = regcomp(&regex, pattern, 0);
    if (compiled_regex)
    {
        error("Failed to compile regex");
    }

    while (fgets(buffer, 255, file_pointer) != NULL)
    {
        int result = regexec(&regex, buffer, 0, NULL, REG_EXTENDED);
        if (result == 0)
        {
            regfree(&regex);
            return true;
        }
    }
    regfree(&regex);
    return false;
}


void add_hosts_file_entry(char ip[], char host[])
{
    printf("Adding entry\n");
}


int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        error("ERROR: Incorrect number of args.");
    }
    char *ip_address = argv[1];
    char *host_name = argv[2];
    // printf("Got IP %s\n", ip_address);
    // printf("Got Host %s\n", host_name);

    FILE *file_pointer = fopen("./hosts", "r");

    if (file_pointer == NULL)
    {
        error("ERROR:Cannot open hosts file!");
    }

    bool already_host_entry_already = host_entry_already("localhost", file_pointer);
    if (already_host_entry_already == true)
    {
        printf("Entry in hosts file already\n");
    }
    else
    {
        printf("Entry NOT in hosts file already\n");
    }

    add_hosts_file_entry(ip_address, host_name);
    // while (fscanf(file_pointer, "%s %s", f_ip, f_host) != EOF) {
    //     printf("File IP %s", f_ip);
    //     printf("File Host %s", f_host);
    // }

    return 0;
}
