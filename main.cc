#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

#include "setproctitle.h"

char **smaug_os_argv;

void dump_argv_env(const char* process_id, int argc, char *argv[], char *envp[]) {
    int i;
    for (i = 0; i < argc; ++i) {
        printf("%s mem:%p len:%d argv[%d]: %s\n", process_id, argv[i], strlen(argv[i]), i, argv[i]);
    }
    printf("\n");
    for (i = 0; envp[i] != NULL; ++i) {
        printf("%s mem:%p len:%d envp[%d]: %s\n", process_id, envp[i], strlen(envp[i]), i, envp[i]);
    }
}

int worker_work() {
    while (true) {
        // block here
        getchar();
    }
    return 0;
}

int master_work() {
    while (true) {
        // block here
        getchar();
    }
    return 0;
}
 
int main(int argc, char* argv[], char *envp[])
{
    smaug_os_argv = argv;
    pid_t pid = fork();
    switch (pid) {
        case -1:
            {
                printf("fork failed\n");
                return -1;
            }
         case 0:
            {
                printf("\n\n#######this is child process###########\n");
                const char *new_title = "setproctitle: worker";
                if (smaug_init_setproctitle() == SMAUG_PROCTITLE_OK) {
                    smaug_setproctitle(new_title);
                }
                dump_argv_env("worker", argc, argv, envp);
                return worker_work();
            }
         default:
            {
                printf("\n\n#######this is father process###########\n");
                const char *new_title = "setproctitle: master";
                if (smaug_init_setproctitle() == SMAUG_PROCTITLE_OK) {
                    smaug_setproctitle(new_title);
                }
                dump_argv_env("master", argc, argv, envp);
                return master_work();
            }
    }

    printf("father work done, will exit\n");
    return 0;
}
