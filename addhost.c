#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>
#include <unistd.h>

const char HOSTS_FILE_PATH[11] = "/etc/hosts";

int error(char text[])
{
    fprintf(stderr, "ERROR: %s\n", text);
    exit(1);
}

char *concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

char *create_new_hosts_file_content(char ip_address[], char hostname[], FILE *hosts_fp)
{
    char file_buffer[255];
    regex_t regex;
    char *new_hosts_content = "";

    char *host_regex_pattern = concat("[ ]*", hostname);
    int compiled = regcomp(&regex, host_regex_pattern, 0);
    if (compiled != 0)
    {
        error("Failed to compile regex");
    }

    while (fgets(file_buffer, 255, hosts_fp) != NULL)
    {
        bool found_entry = regexec(&regex, file_buffer, 0, NULL, REG_EXTENDED) == 0;
        if (!found_entry)
        {
            new_hosts_content = concat(new_hosts_content, file_buffer);
        }
    }
    regfree(&regex);

    char *entry_to_add = concat(concat(concat(ip_address, " "), hostname), "\n");
    new_hosts_content = concat(new_hosts_content, entry_to_add);
    return new_hosts_content;
}

void check_ran_as_root()
{
    if (getuid() != 0)
    {
        error("You need to run this program as root.");
    }
}

void update_hosts_file(char *content)
{
    FILE *output_fp = fopen(HOSTS_FILE_PATH, "w");
    fputs(content, output_fp);
    fclose(output_fp);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        error(
            "Incorrect number of args. \n"
            "Example usage:\nadd 127.0.0.1 example.com"
        );
    }
    check_ran_as_root();

    char *ip_address = argv[1];
    char *hostname = argv[2];
    printf("Setting hosts file entry: %s %s\n", ip_address, hostname);

    FILE *input_fp = fopen(HOSTS_FILE_PATH, "r");
    if (input_fp == NULL)
    {
        error("Cannot open hosts file!");
    }
    char *new_hosts_content = create_new_hosts_file_content(ip_address, hostname, input_fp);
    fclose(input_fp);

    update_hosts_file(new_hosts_content);
    printf("Done!\n");
    return 0;
}
