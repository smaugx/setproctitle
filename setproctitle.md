---
title: 模仿nginx修改进程名
date: 2020/10/25 11:23:58
tags: 
	- nginx
	- setproctitle
	- fork
	- c++
categories:
	- c++
comments: 
---


# nginx 进程名
使用 nginx 的过程中，我们经常看到 nginx 的进程名是不同的，如下：

```
$ ps -ef |grep nginx 
smaug    1183  1115  0 05:46 pts/2    00:00:00 grep --color=auto nginx
root     14201     1  0  2019 ?        00:00:00 nginx: master process ./sbin/nginx
nobody   28887 14201  0 Oct14 ?        00:00:00 nginx: worker process
nobody   28888 14201  0 Oct14 ?        00:00:00 nginx: worker process
```

可以看到 nginx 的进程名是不同的，那么它是怎么做到的呢？

# argv[0]
首先来看一下 C 语言中的 main 函数的定义：

```
int main(int argc, char *argv[]);
```

这个应该大家都是比较熟悉的，argc 表示命令行参数个数， argv 保存了各个命令行参数的内容。**其中 `argv[0]` 表示的是进程的名字**，这就是修改进程名的关键点所在。

**只需要修改  argv[0] 的值即可完成修改进程名**。

## hello world

下面以程序员经典入门代码为例说明：

```
// filename: hello_world_setproctitle.cc
// build: g++ hello_world_setproctitle.cc -o hello_world_setproctitle

#include <cstdio>
#include <cstring>

int main(int argc, char *argv[]) {
    printf("hello world\n");
    while (true) {
        // block here
        char c = getchar();
    }
    return 0;
}
```

编译运行：

```
g++ hello_world_setproctitle.cc -o hello_world_setproctitle
./hello_world_setproctitle
```

查看一下进程名：

```
# ps -ef |grep hello_world
root     26356 12289  0 14:17 pts/3    00:00:00 ./hello_world_setproctitle
root     26366 20422  0 14:18 pts/1    00:00:00 grep --color=auto hello_world
```

可以看到进程名是 `hello_world_setproctitle`，接下来我们修改一下 argv[0] 的值，代码如下：

```
// filename: hello_world_setproctitle.cc
// build: g++ hello_world_setproctitle.cc -o hello_world_setproctitle

#include <cstdio>
#include <cstring>

int main(int argc, char *argv[]) {
    // new process name
    const char* new_name = "new_new_hello_world_setproctitle";
    strcpy(argv[0], new_name);
    printf("hello world\n");
    while (true) {
        // block here
        char c = getchar();
    }
    return 0;
}

```

编译运行之后，查看进程名：

```
# ps -ef |grep hello_world
root     26750 12289  0 14:23 pts/3    00:00:00 new_new_hello_world_setproctitle
root     26754 20422  0 14:23 pts/1    00:00:00 grep --color=auto hello_world
```
可以看到进程名已经修改为 `new_new_hello_world_setproctitle` 了。

是不是很简单？

**不过上面的代码是有一定的风险的，如果新的进程名超过了原来 argv[0] 的长度，就可能会影响到后面的 environ 的内容**。

# 环境变量 environ
C 语言中 main 函数的定义还有一个:

```
int main(int argc, char *argv[], char *envp[]);
```

这个版本提供了第三个参数，大多数 Unix 系统支持，但是  POSIX.1 不建议这么做，如果要访问环境变量建议使用 `getenv` 和 `putenv` 接口。这里就不展开讲了。

envp 这个参数表示环境变量，**每一个进程都有与之相关的环境变量，其中每个字符串都以(name=value)形式定义，并且 envp 的地址紧跟在 argv 之后**。

## hello world environ

接下来我们打印一下 envp 这个参数的值，基于上面的代码，简单修改一下：

```
// filename: hello_world_setproctitle.cc
// build: g++ hello_world_setproctitle.cc -o hello_world_setproctitle

#include <cstdio>
#include <cstring>

int main(int argc, char *argv[], char *envp[]) {
    int i;
    for (i = 0; i < argc; ++i) {
        printf("mem:%p len:%d argv[%d]: %s\n", argv[i], strlen(argv[i]), i, argv[i]);
    }
    printf("\n");
    for (i = 0; envp[i] != NULL; ++i) {
        printf("mem:%p len:%d envp[%d]: %s\n", envp[i], strlen(envp[i]), i, envp[i]);
    }

    const char* new_name = "new_new_hello_world_setproctitle";
    strcpy(argv[0], new_name);
    printf("hello world\n");
    while (true) {
        // block here
        char c = getchar();
    }
    return 0;
}

```

上面的代码同时也打印了每个参数的地址以及长度，编译并执行：

```
# ./hello_world_setproctitle 1 22
mem:0x7ffc84cf7544 len:26 argv[0]: ./hello_world_setproctitle
mem:0x7ffc84cf755f len:1 argv[1]: 1
mem:0x7ffc84cf7561 len:2 argv[2]: 22

mem:0x7ffc84cf7564 len:19 envp[0]: XDG_SESSION_ID=2554
mem:0x7ffc84cf7578 len:13 envp[1]: HOSTNAME=Jiao
mem:0x7ffc84cf7586 len:23 envp[2]: SELINUX_ROLE_REQUESTED=
mem:0x7ffc84cf759e len:19 envp[3]: TERM=xterm-256color
mem:0x7ffc84cf75b2 len:15 envp[4]: SHELL=/bin/bash
mem:0x7ffc84cf75c2 len:13 envp[5]: HISTSIZE=1000
mem:0x7ffc84cf75d0 len:31 envp[6]: SSH_CLIENT=192.168.1.3 61311 22
mem:0x7ffc84cf75f0 len:26 envp[7]: SELINUX_USE_CURRENT_RANGE=
mem:0x7ffc84cf760b len:75 envp[8]: ANDROID_NDK_TOOLCHAIN_ROOT=/root/smaug/software/android-ndk-r16b/toolchains
mem:0x7ffc84cf7657 len:12 envp[9]: OLDPWD=/root
mem:0x7ffc84cf7664 len:18 envp[10]: SSH_TTY=/dev/pts/3
mem:0x7ffc84cf7677 len:9 envp[11]: USER=root
mem:0x7ffc84cf7681 len:75 envp[12]: LD_LIBRARY_PATH=/usr/local/lib:/usr/local/python3/lib::/usr/local/topio/lib
mem:0x7ffc84cf76cd len:1719 envp[13]: LS_COLORS=rs=0:di=38;5;27:ln=38;5;51:mh=44;38;5;15:pi=40;38;5;11:so=38;5;13:do=38;5;5:bd=48;5;232;38;5;11:cd=48;5;232;38;5;3:or=48;5;232;38;5;9:mi=05;48;5;232;38;5;15:su=48;5;196;38;5;15:sg=48;5;11;38;5;16:ca=48;5;196;38;5;226:tw=48;5;10;38;5;16:ow=48;5;10;38;5;21:st=48;5;21;38;5;15:ex=38;5;34:*.tar=38;5;9:*.tgz=38;5;9:*.arc=38;5;9:*.arj=38;5;9:*.taz=38;5;9:*.lha=38;5;9:*.lz4=38;5;9:*.lzh=38;5;9:*.lzma=38;5;9:*.tlz=38;5;9:*.txz=38;5;9:*.tzo=38;5;9:*.t7z=38;5;9:*.zip=38;5;9:*.z=38;5;9:*.Z=38;5;9:*.dz=38;5;9:*.gz=38;5;9:*.lrz=38;5;9:*.lz=38;5;9:*.lzo=38;5;9:*.xz=38;5;9:*.bz2=38;5;9:*.bz=38;5;9:*.tbz=38;5;9:*.tbz2=38;5;9:*.tz=38;5;9:*.deb=38;5;9:*.rpm=38;5;9:*.jar=38;5;9:*.war=38;5;9:*.ear=38;5;9:*.sar=38;5;9:*.rar=38;5;9:*.alz=38;5;9:*.ace=38;5;9:*.zoo=38;5;9:*.cpio=38;5;9:*.7z=38;5;9:*.rz=38;5;9:*.cab=38;5;9:*.jpg=38;5;13:*.jpeg=38;5;13:*.gif=38;5;13:*.bmp=38;5;13:*.pbm=38;5;13:*.pgm=38;5;13:*.ppm=38;5;13:*.tga=38;5;13:*.xbm=38;5;13:*.xpm=38;5;13:*.tif=38;5;13:*.tiff=38;5;13:*.png=38;5;13:*.svg=38;5;13:*.svgz=38;5;13:*.mng=38;5;13:*.pcx=38;5;13:*.mov=38;5;13:*.mpg=38;5;13:*.mpeg=38;5;13:*.m2v=38;5;13:*.mkv=38;5;13:*.webm=38;5;13:*.ogm=38;5;13:*.mp4=38;5;13:*.m4v=38;5;13:*.mp4v=38;5;13:*.vob=38;5;13:*.qt=38;5;13:*.nuv=38;5;13:*.wmv=38;5;13:*.asf=38;5;13:*.rm=38;5;13:*.rmvb=38;5;13:*.flc=38;5;13:*.avi=38;5;13:*.fli=38;5;13:*.flv=38;5;13:*.gl=38;5;13:*.dl=38;5;13:*.xcf=38;5;13:*.xwd=38;5;13:*.yuv=38;5;13:*.cgm=38;5;13:*.emf=38;5;13:*.axv=38;5;13:*.anx=38;5;13:*.ogv=38;5;13:*.ogx=38;5;13:*.aac=38;5;45:*.au=38;5;45:*.flac=38;5;45:*.mid=38;5;45:*.midi=38;5;45:*.mka=38;5;45:*.mp3=38;5;45:*.mpc=38;5;45:*.ogg=38;5;45:*.ra=38;5;45:*.wav=38;5;45:*.axa=38;5;45:*.oga=38;5;45:*.spx=38;5;45:*.xspf=38;5;45:
mem:0x7ffc84cf7d85 len:25 envp[14]: MAIL=/var/spool/mail/root
mem:0x7ffc84cf7d9f len:193 envp[15]: PATH=/root/.cargo/bin:/root/.cargo/bin:/usr/local/vim/bin:/usr/local/bin:/usr/loca/python3/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/usr/local/go/bin:/root/bin:/usr/local/topio/bin
mem:0x7ffc84cf7e61 len:21 envp[16]: MARKPATH=/root/.marks
mem:0x7ffc84cf7e77 len:14 envp[17]: PWD=/root/temp
mem:0x7ffc84cf7e86 len:16 envp[18]: LANG=zh_CN.UTF-8
mem:0x7ffc84cf7e97 len:38 envp[19]: PS1=\[\e[32;1m\][\u@\h \w]\$ \[\e[0m\]
mem:0x7ffc84cf7ebe len:24 envp[20]: SELINUX_LEVEL_REQUESTED=
mem:0x7ffc84cf7ed7 len:22 envp[21]: HISTCONTROL=ignoredups
mem:0x7ffc84cf7eee len:7 envp[22]: SHLVL=1
mem:0x7ffc84cf7ef6 len:10 envp[23]: HOME=/root
mem:0x7ffc84cf7f01 len:12 envp[24]: LOGNAME=root
mem:0x7ffc84cf7f0e len:47 envp[25]: SSH_CONNECTION=192.168.1.3 61311 192.168.1.4 22
mem:0x7ffc84cf7f3e len:16 envp[26]: GOPATH=/root/.go
mem:0x7ffc84cf7f4f len:34 envp[27]: LESSOPEN=||/usr/bin/lesspipe.sh %s
mem:0x7ffc84cf7f72 len:49 envp[28]: ANDROID_NDK=/root/smaug/software/android-ndk-r16b
mem:0x7ffc84cf7fa4 len:27 envp[29]: XDG_RUNTIME_DIR=/run/user/0
mem:0x7ffc84cf7fc0 len:28 envp[30]: _=./hello_world_setproctitle
hello world


```

可以看到上述各个 argv 的值以及 envp 参数的内容。

这里需要重点注意一下最后一个 argv[2] 参数以及第一个 envp[0] 参数的地址：

```
mem:0x7ffc84cf7561 len:2 argv[2]: 22

mem:0x7ffc84cf7564 len:19 envp[0]: XDG_SESSION_ID=2554
```

0x7ffc84cf7564 正好等于 0x7ffc84cf7561 + 3 （argv[2] 的长度加上最后一个 '\0')。可以多试几次，不同的参数个数验证下这个。

所以 environ 的地址（envp[0] 的地址）是紧跟在 argv 后面的，那么前面提到的如果当新的进程名长度超出 argv 的长度后，可能就会覆盖后面的 environ 内容，导致其他一些问题。

## 新进程名过长，覆盖 envp 

修改如上代码：

```
// filename: hello_world_setproctitle.cc
// build: g++ hello_world_setproctitle.cc -o hello_world_setproctitle

#include <cstdio>
#include <cstring>

int main(int argc, char *argv[], char *envp[]) {
    const char* new_name = "new_new_hello_world_setproctitlexxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
    strcpy(argv[0], new_name);
    int i;
    for (i = 0; i < argc; ++i) {
        printf("mem:%p len:%d argv[%d]: %s\n", argv[i], strlen(argv[i]), i, argv[i]);
    }
    printf("\n");
    for (i = 0; envp[i] != NULL; ++i) {
        printf("mem:%p len:%d envp[%d]: %s\n", envp[i], strlen(envp[i]), i, envp[i]);
    }

    printf("hello world\n");
    while (true) {
        // block here
        char c = getchar();
    }
    return 0;
}
```

编译运行：

```
# ./hello_world_setproctitle
mem:0x7fff872c3549 len:125 argv[0]: new_new_hello_world_setproctitlexxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

mem:0x7fff872c3564 len:98 envp[0]: titlexxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
mem:0x7fff872c3578 len:78 envp[1]: xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
mem:0x7fff872c3586 len:64 envp[2]: xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
mem:0x7fff872c359e len:40 envp[3]: xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
mem:0x7fff872c35b2 len:20 envp[4]: xxxxxxxxxxxxxxxxxxxx
mem:0x7fff872c35c2 len:4 envp[5]: xxxx
mem:0x7fff872c35d0 len:31 envp[6]: SSH_CLIENT=192.168.1.3 55187 22
mem:0x7fff872c35f0 len:26 envp[7]: SELINUX_USE_CURRENT_RANGE=
mem:0x7fff872c360b len:75 envp[8]: ANDROID_NDK_TOOLCHAIN_ROOT=/root/smaug/software/android-ndk-r16b/toolchains
mem:0x7fff872c3657 len:12 envp[9]: OLDPWD=/root
mem:0x7fff872c3664 len:18 envp[10]: SSH_TTY=/dev/pts/1
mem:0x7fff872c3677 len:9 envp[11]: USER=root
```

可以看到，上面打印出来的 envp[0], envp[1].. envp[5] 都已经被覆盖了。

所以，**通过 argv[0] 修改进程名，如果新进程名过长，需要考虑到 envp 的覆盖问题，通常做法是把 envp 的内容先保存，然后指向新的内存，再把保存的环境变量复制到新的内存，然后再去修改 argv[0]**。

# setproctitle 修改进程名

可以参考 nginx 的源码: [https://github.com/nginx/nginx/blob/master/src/os/unix/ngx_setproctitle.c](https://github.com/nginx/nginx/blob/master/src/os/unix/ngx_setproctitle.c)


下面直接上源码，源码可以在我的 github 找到：

[https://github.com/smaugx/setproctitle](https://github.com/smaugx/setproctitle)

> setproctitle.h

```
// author: smaug

#pragma once


#ifndef _SMAUG_SETPROCTITLE_H_INCLUDED_
#define _SMAUG_SETPROCTITLE_H_INCLUDED_

#define SMAUG_PROCTITLE_ERROR  -1
#define SMAUG_PROCTITLE_OK   0

int smaug_init_setproctitle();
void smaug_setproctitle(const char *title);

#endif /* _SMAUG_SETPROCTITLE_H_INCLUDED_ */

```

> setproctitle.cc

```
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
```

编译运行：

```
# sh build.sh
# ./main

#######this is father process###########
master mem:0x7ffce4714573 len:20 argv[0]: setproctitle: master

master mem:0x12e0c20 len:19 envp[0]: XDG_SESSION_ID=2554
master mem:0x12e0c34 len:13 envp[1]: HOSTNAME=Jiao
master mem:0x12e0c42 len:23 envp[2]: SELINUX_ROLE_REQUESTED=
master mem:0x12e0c5a len:19 envp[3]: TERM=xterm-256color
master mem:0x12e0c6e len:15 envp[4]: SHELL=/bin/bash
master mem:0x12e0c7e len:13 envp[5]: HISTSIZE=1000
master mem:0x12e0c8c len:31 envp[6]: SSH_CLIENT=192.168.1.3 61311 22
master mem:0x12e0cac len:26 envp[7]: SELINUX_USE_CURRENT_RANGE=
master mem:0x12e0cc7 len:75 envp[8]: ANDROID_NDK_TOOLCHAIN_ROOT=/root/smaug/software/android-ndk-r16b/toolchains
master mem:0x12e0d13 len:17 envp[9]: OLDPWD=/root/temp
master mem:0x12e0d25 len:18 envp[10]: SSH_TTY=/dev/pts/3
master mem:0x12e0d38 len:9 envp[11]: USER=root
master mem:0x12e0d42 len:75 envp[12]: LD_LIBRARY_PATH=/usr/local/lib:/usr/local/python3/lib::/usr/local/topio/lib
master mem:0x12e0d8e len:1719 envp[13]: LS_COLORS=rs=0:di=38;5;27:ln=38;5;51:mh=44;38;5;15:pi=40;38;5;11:so=38;5;13:do=38;5;5:bd=48;5;232;38;5;11:cd=48;5;232;38;5;3:or=48;5;232;38;5;9:mi=05;48;5;232;38;5;15:su=48;5;196;38;5;15:sg=48;5;11;38;5;16:ca=48;5;196;38;5;226:tw=48;5;10;38;5;16:ow=48;5;10;38;5;21:st=48;5;21;38;5;15:ex=38;5;34:*.tar=38;5;9:*.tgz=38;5;9:*.arc=38;5;9:*.arj=38;5;9:*.taz=38;5;9:*.lha=38;5;9:*.lz4=38;5;9:*.lzh=38;5;9:*.lzma=38;5;9:*.tlz=38;5;9:*.txz=38;5;9:*.tzo=38;5;9:*.t7z=38;5;9:*.zip=38;5;9:*.z=38;5;9:*.Z=38;5;9:*.dz=38;5;9:*.gz=38;5;9:*.lrz=38;5;9:*.lz=38;5;9:*.lzo=38;5;9:*.xz=38;5;9:*.bz2=38;5;9:*.bz=38;5;9:*.tbz=38;5;9:*.tbz2=38;5;9:*.tz=38;5;9:*.deb=38;5;9:*.rpm=38;5;9:*.jar=38;5;9:*.war=38;5;9:*.ear=38;5;9:*.sar=38;5;9:*.rar=38;5;9:*.alz=38;5;9:*.ace=38;5;9:*.zoo=38;5;9:*.cpio=38;5;9:*.7z=38;5;9:*.rz=38;5;9:*.cab=38;5;9:*.jpg=38;5;13:*.jpeg=38;5;13:*.gif=38;5;13:*.bmp=38;5;13:*.pbm=38;5;13:*.pgm=38;5;13:*.ppm=38;5;13:*.tga=38;5;13:*.xbm=38;5;13:*.xpm=38;5;13:*.tif=38;5;13:*.tiff=38;
5;13:*.png=38;5;13:*.svg=38;5;13:*.svgz=38;5;13:*.mng=38;5;13:*.pcx=38;5;13:*.mov=38;5;13:*.mpg=38;5;13:*.mpeg=38;5;13:*.m2v=38;5;13:*.mkv=38;5;13:*.webm=38;5;13:*.ogm=38;5;13:*.mp4=38;5;13:*.m4v=38;5;13:*.mp4v=38;5;13:*.vob=38;5;13:*.qt=38;5;13:*.nuv=38;5;13:*.wmv=38;5;13:*.asf=38;5;13:*.rm=38;5;13:*.rmvb=38;5;13:*.flc=38;5;13:*.avi=38;5;13:*.fli=38;5;13:*.flv=38;5;13:*.gl=38;5;13:*.dl=38;5;13:*.xcf=38;5;13:*.xwd=38;5;13:*.yuv=38;5;13:*.cgm=38;5;13:*.emf=38;5;13:*.axv=38;5;13:*.anx=38;5;13:*.ogv=38;5;13:*.ogx=38;5;13:*.aac=38;5;45:*.au=38;5;45:*.flac=38;5;45:*.mid=38;5;45:*.midi=38;5;45:*.mka=38;5;45:*.mp3=38;5;45:*.mpc=38;5;45:*.ogg=38;5;45:*.ra=38;5;45:*.wav=38;5;45:*.axa=38;5;45:*.oga=38;5;45:*.spx=38;5;45:*.xspf=38;5;45:

master mem:0x12e1446 len:25 envp[14]: MAIL=/var/spool/mail/root
master mem:0x12e1460 len:193 envp[15]: PATH=/root/.cargo/bin:/root/.cargo/bin:/usr/local/vim/bin:/usr/local/bin:/usr/loca/python3/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/usr/local/go/bin:/root/bin:/usr/local/topio/bin
master mem:0x12e1522 len:21 envp[16]: MARKPATH=/root/.marks
#######this is child process###########
master mem:0x12e1538 len:27 envp[17]: PWD=/root/temp/setproctitle
master mem:0x12e1554 len:16 envp[18]: LANG=zh_CN.UTF-8
worker mem:0x7ffce4714573 len:20 argv[0]: setproctitle: worker

worker mem:0x12e0c20 len:19 envp[0]: XDG_SESSION_ID=2554
worker mem:0x12e0c34 len:13 envp[1]: HOSTNAME=Jiao
worker mem:0x12e0c42 len:23 envp[2]: SELINUX_ROLE_REQUESTED=
worker mem:0x12e0c5a len:19 envp[3]: TERM=xterm-256color
worker mem:0x12e0c6e len:15 envp[4]: SHELL=/bin/bash
worker mem:0x12e0c7e len:13 envp[5]: HISTSIZE=1000
worker mem:0x12e0c8c len:31 envp[6]: SSH_CLIENT=192.168.1.3 61311 22
worker mem:0x12e0cac len:26 envp[7]: SELINUX_USE_CURRENT_RANGE=
worker mem:0x12e0cc7 len:75 envp[8]: ANDROID_NDK_TOOLCHAIN_ROOT=/root/smaug/software/android-ndk-r16b/toolchains
worker mem:0x12e0d13 len:17 envp[9]: OLDPWD=/root/temp
worker mem:0x12e0d25 len:18 envp[10]: SSH_TTY=/dev/pts/3
worker mem:0x12e0d38 len:9 envp[11]: USER=root
worker mem:0x12e0d42 len:75 envp[12]: LD_LIBRARY_PATH=/usr/local/lib:/usr/local/python3/lib::/usr/local/topio/lib
worker mem:0x12e0d8e len:1719 envp[13]: LS_COLORS=rs=0:di=38;5;27:ln=38;5;51:mh=44;38;5;15:pi=40;38;5;11:so=38;5;13:do=38;5;5:bd=48;5;232;38;5;11:cd=48;5;232;38;5;3:or=48;5;232;38;5;9:mi=05;48;5;232;38;5;15:su=48;5;196;38;5;15:sg=48;5;11;38;5;16:ca=48;5;196;38;5;226:tw=48;5;10;38;5;16:ow=48;5;10;38;5;21:st=48;5;21;38;5;15:ex=38;5;34:*.tar=38;5;9:*.tgz=38;5;9:*.arc=38;5;9:*.arj=38;5;9:*.taz=38;5;9:*.lha=38;5;9:*.lz4=38;5;9:*.lzh=38;5;9:*.lzma=38;5;9:*.tlz=38;5;9:*.txz=38;5;9:*.tzo=38;5;9:*.t7z=38;5;9:*.zip=38;5;9:*.z=38;5;9:*.Z=38;5;9:*.dz=38;5;9:*.gz=38;5;9:*.lrz=38;5;9:*.lz=38;5;9:*.lzo=38;5;9:*.xz=38;5;9:*.bz2=38;5;9:*.bz=38;5;9:*.tbz=38;5;9:*.tbz2=38;5;9:*.tz=38;5;9:*.deb=38;5;9:*.rpm=38;5;9:*.jar=38;5;9:*.war=38;5;9:*.ear=38;5;9:*.sar=38;5;9:*.rar=38;5;9:*.alz=38;5;9:*.ace=38;5;9:*.zoo=38;5;9:*.cpio=38;5;9:*.7z=38;5;9:*.rz=38;5;9:*.cab=38;5;9:*.jpg=38;5;13:*.jpeg=38;5;13:*.gif=38;5;13:*.bmp=38;5;13:*.pbm=38;5;13:*.pgm=38;5;13:*.ppm=38;5;13:*.tga=38;5;13:*.xbm=38;5;13:*.xpm=38;5;13:*.tif=38;5;13:*.tiff=38;5;13:*.png=38;5;13:*.svg=38;5;13:*.svgz=38;5;13:*.mng=38;5;13:*.pcx=38;5;13:*.mov=38;5;13:*.mpg=38;5;13:*.mpeg=38;5;13:*.m2v=38;5;13:*.mkv=38;5;13:*.webm=38;5;13:*.ogm=38;5;13:*.mp4=38;5;13:*.m4v=38;5;13:*.mp4v=38;5;13:*.vob=38;5;13:*.qt=38;5;13:*.nuv=38;5;13:*.wmv=38;5;13:*.asf=38;5;13:*.rm=38;5;13:*.rmvb=38;5;13:*.flc=38;5;13:*.avi=38;5;13:*.fli=38;5;13:*.flv=38;5;13:*.gl=38;5;13:*.dl=38;5;13:*.xcf=38;5;13:*.xwd=38;5;13:*.yuv=38;5;13:*.cgm=38;5;13:*.emf=38;5;13:*.axv=38;5;13:*.anx=38;5;13:*.ogv=38;5;13:*.ogx=38;5;13:*.aac=38;5;45:*.au=38;5;45:*.flac=38;5;45:*.mid=38;5;45:*.midi=38;5;45:*.mka=38;5;45:*.mp3=38;5;45:*.mpc=38;5;45:*.ogg=38;5;45:*.ra=38;5;45:*.wav=38;5;45:*.axa=38;5;45:*.oga=38;5;45:*.spx=38;5;45:*.xspf=38;5;45:
worker mem:0x12e1446 len:25 envp[14]: MAIL=/var/spool/mail/root
worker mem:0x12e1460 len:193 envp[15]: PATH=/root/.cargo/bin:/root/.cargo/bin:/usr/local/vim/bin:/usr/local/bin:/usr/loca/python3/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/usr/local/go/bin:/root/bin:/usr/local/topio/bin
worker mem:0x12e1522 len:21 envp[16]: MARKPATH=/root/.marks
worker mem:0x12e1538 len:27 envp[17]: PWD=/root/temp/setproctitle
worker mem:0x12e1554 len:16 envp[18]: LANG=zh_CN.UTF-8
worker mem:0x12e1565 len:38 envp[19]: PS1=\[\e[32;1m\][\u@\h \w]\$ \[\e[0m\]
worker mem:0x12e158c len:24 envp[20]: SELINUX_LEVEL_REQUESTED=
worker mem:0x12e15a5 len:22 envp[21]: HISTCONTROL=ignoredups
worker mem:0x12e15bc len:7 envp[22]: SHLVL=1
worker mem:0x12e15c4 len:10 envp[23]: HOME=/root
worker mem:0x12e15cf len:12 envp[24]: LOGNAME=root
worker mem:0x12e15dc len:47 envp[25]: SSH_CONNECTION=192.168.1.3 61311 192.168.1.4 22
worker mem:0x12e160c len:16 envp[26]: GOPATH=/root/.go
master mem:0x12e1565 len:38 envp[19]: PS1=\[\e[32;1m\][\u@\h \w]\$ \[\e[0m\]
master mem:0x12e158c len:24 envp[20]: SELINUX_LEVEL_REQUESTED=
master mem:0x12e15a5 len:22 envp[21]: HISTCONTROL=ignoredups
master mem:0x12e15bc len:7 envp[22]: SHLVL=1
worker mem:0x12e161d len:34 envp[27]: LESSOPEN=||/usr/bin/lesspipe.sh %s
worker mem:0x12e1640 len:49 envp[28]: ANDROID_NDK=/root/smaug/software/android-ndk-r16b
worker mem:0x12e1672 len:27 envp[29]: XDG_RUNTIME_DIR=/run/user/0
master mem:0x12e15c4 len:10 envp[23]: HOME=/root
master mem:0x12e15cf len:12 envp[24]: LOGNAME=root
master mem:0x12e15dc len:47 envp[25]: SSH_CONNECTION=192.168.1.3 61311 192.168.1.4 22
worker mem:0x12e168e len:8 envp[30]: _=./main
master mem:0x12e160c len:16 envp[26]: GOPATH=/root/.go
master mem:0x12e161d len:34 envp[27]: LESSOPEN=||/usr/bin/lesspipe.sh %s
master mem:0x12e1640 len:49 envp[28]: ANDROID_NDK=/root/smaug/software/android-ndk-r16b
master mem:0x12e1672 len:27 envp[29]: XDG_RUNTIME_DIR=/run/user/0
master mem:0x12e168e len:8 envp[30]: _=./main

```

可以看到上述的命令行参数以及环境变量在父子进程中都是正确的，查看一下进程名:

```
# ps -ef |grep setproc
root      2584 12289  0 16:38 pts/3    00:00:00 setproctitle: master
root      2585  2584  0 16:38 pts/3    00:00:00 setproctitle: worker
root      2590 20422  0 16:38 pts/1    00:00:00 grep --color=auto setproc

```


# 扩展
上述代码可以完美的修改进程名，但是如果你使用查看进程信息可能还会看到旧的进程名：

```
# ps -ef |grep setproc
root      2584 12289  0 16:38 pts/3    00:00:00 setproctitle: master
root      2585  2584  0 16:38 pts/3    00:00:00 setproctitle: worker
root      2670 20422  0 16:39 pts/1    00:00:00 grep --color=auto setproc

# cat /proc/2585/status |grep Name
Name:	main

# cat /proc/2584/status |grep Name
Name:	main
```

这个时候可以结合 prctl 使用：

```
prctl(PR_SET_NAME, new_name);
```

具体可以查看相关资料。