#include <stdio.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
    if(argc != 3) {
        printf("ERROR: Incorrect number of args.\n");
        return 1;
    }
    char *ip_address = argv[1];
    char *host_name = argv[2];
    printf("Got IP %s\n", ip_address);
    printf("Got Host %s\n", host_name);
    sleep(100);
    return 0;
}
