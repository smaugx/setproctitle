// author: smaug

#include "setproctitle.h"

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>

#include <cstdint>

/*
 * To change the process title in Linux and Solaris we have to set argv[1]
 * to NULL and to copy the title to the same place where the argv[0] points to.
 * However, argv[0] may be too small to hold a new title.  Fortunately, Linux
 * and Solaris store argv[] and environ[] one after another.  So we should
 * ensure that is the continuous memory and then we allocate the new memory
 * for environ[] and copy it.  After this we could use the memory starting
 * from argv[0] for our process title.
 */

extern char **environ;
// same as argv
extern char** smaug_os_argv;
static char *smaug_os_argv_last;

int smaug_init_setproctitle()
{
    char *p;
    int i = 0;
    uint32_t size = 0;

    // 统计所有 environ 环境变量的长度(+1 是表示每个字符串后的 '\0')
    for (i = 0; environ[i]; i++) {
        size += strlen(environ[i]) + 1;
    }

    // 重新分配一段内存，待会用来保存 environ 指向的环境变量
    p = (char*)malloc(size);
    if (p == NULL) {
        return SMAUG_PROCTITLE_ERROR;
    }

    smaug_os_argv_last = smaug_os_argv[0];

    // 遍历 argv，完成后指向 argv 最后一个参数的 '\0' 的下一个位置，也就是 envp[0] 的地址
    for (i = 0; smaug_os_argv[i]; i++) {
        if (smaug_os_argv_last == smaug_os_argv[i]) {
            smaug_os_argv_last = smaug_os_argv[i] + strlen(smaug_os_argv[i]) + 1;
        }
    }

    for (i = 0; environ[i]; i++) {
        if (smaug_os_argv_last == environ[i]) {

            // 包括 '\0'
            size = strlen(environ[i]) + 1;
            // smaug_os_argv_last 接着往前递进, 经过每一个环境变量
            smaug_os_argv_last = environ[i] + size;

            // 拷贝每一个 环境变量到上面刚申请的新内存中
            strncpy(p, environ[i], size);
            // 修改 environ[i] 指向这块新的内存地址（避免后续修改 argv[0] 造成覆盖)
            environ[i] = (char *) p;
            // 新内存指针 p 前移，称为下一轮 environ[i+1] 的值
            p += size;
        }
    }

    // 指针往前移 1 个字节，指向 最后一个环境变量的 '\0' 位置
    smaug_os_argv_last--;

    return SMAUG_PROCTITLE_OK;
}


void smaug_setproctitle(const char *title)
{
    // 设置 argv[1] = NULL 通常不必要，加上更安全
    smaug_os_argv[1] = NULL;
    /*
    char new_title[1024];
    bzero(new_title, sizeof(new_title));
    sprintf(new_title, "%s%s", "smaug: ", title);
    */

    // smaug_os_argv_last - smaug_os_argv[0] 就是最初 argv + envp 所有字符串的长度, 拷贝新进程名到 argv[0]
    strncpy(smaug_os_argv[0], title, smaug_os_argv_last - smaug_os_argv[0]);

#ifdef DEBUG
    printf("set title:%s\n", new_title);
#endif
}
