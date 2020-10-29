#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/prctl.h>

#include "../setproctitle.h"

char **smaug_os_argv;

int main(int argc, char* argv[], char *envp[])
{
    smaug_os_argv = argv;

    // set new process NULL to hide process
    //const char *new_title = "hidden_main_new";
    const char *new_title = "";
    if (smaug_init_setproctitle() == SMAUG_PROCTITLE_OK) {
        smaug_setproctitle(new_title);
    }
    // set new process NULL to hide process
    prctl(PR_SET_NAME, new_title, NULL, NULL, NULL);

    while (true) {
        sleep(1);
    }
    return 0;
}
