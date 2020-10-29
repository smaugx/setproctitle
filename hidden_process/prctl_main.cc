#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/prctl.h>

int main(int argc, char* argv[], char *envp[])
{
    const char *new_title = "prctl_new_name";
    prctl(PR_SET_NAME, new_title, NULL, NULL, NULL);
    while (true) {
        sleep(2);
    }
    return 0;
}
