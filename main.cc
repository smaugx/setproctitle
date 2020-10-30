#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

#include <string>
#include <iostream>

#include "setproctitle.h"

char **smaug_os_argv;

void dump_argv_env(const char* process_id, int argc, char *argv[], char *envp[]) {
    int i;
    for (i = 0; i < argc && argv[i]; ++i) {
        printf("%s mem:%p len:%d argv[%d]: %s\n", process_id, argv[i], strlen(argv[i]), i, argv[i]);
    }
    printf("\n");
    for (i = 0; envp[i] != NULL; ++i) {
        printf("%s mem:%p len:%d envp[%d]: %s\n", process_id, envp[i], strlen(envp[i]), i, envp[i]);
    }
    printf("\n");
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
                sleep(1);
                printf("\n\n#######this is child process###########\n");
                dump_argv_env("old_worker", argc, argv, envp);

                const char *new_process_name = "setproctitle: worker";
                std::string new_title;
                for (int i = 0; i < argc && argv[i]; ++i) {
                    if (i == 0) {
                        new_title += new_process_name;
                        new_title += " ";
                    } else {
                        new_title += argv[i];
                        new_title += " ";
                    }
                }
                if (new_title.back() == ' ') {
                    new_title.pop_back();
                }

                if (smaug_init_setproctitle() == SMAUG_PROCTITLE_OK) {
                    smaug_setproctitle(new_title.c_str());
                }

                dump_argv_env("new_worker", argc, argv, envp);
                return worker_work();
            }
         default:
            {
                printf("\n\n#######this is father process###########\n");
                dump_argv_env("old_master", argc, argv, envp);

                const char *new_process_name = "setproctitle: master";
                std::string new_title;
                for (int i = 0; i < argc && argv[i]; ++i) {
                    if (i == 0) {
                        new_title += new_process_name;
                        new_title += " ";
                    } else {
                        new_title += argv[i];
                        new_title += " ";
                    }
                }
                if (new_title.back() == ' ') {
                    new_title.pop_back();
                }

                if (smaug_init_setproctitle() == SMAUG_PROCTITLE_OK) {
                    smaug_setproctitle(new_title.c_str());
                }
                dump_argv_env("new_master", argc, argv, envp);
                return master_work();
            }
    }

    printf("father work done, will exit\n");
    return 0;
}
