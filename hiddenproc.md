---
title: Linux上隐藏进程名（初级版）
date: 2020/10/25 22:23:58
tags: 
	- c++
	- setproctitle
	- fork
	- linux
	- ps
	- cmdline
categories:
	- c++
comments: 
---

# 缘起
上一篇博文 [模仿nginx修改进程名](https://rebootcat.com/2020/10/25/setproctitle/) 中提到了一种修改进程名的方法，就像 nginx 一样，给不同进程命名为 master 以及 worker 等。那么能不能把新进程名设置为空字符串呢？如果能，又会有哪些应用场景呢？

答案可能是能的，设置新进程的名字为空，通常用来隐藏进程，用于攻击或者反攻击。

# prctl 函数
上一篇博文 [模仿nginx修改进程名](https://rebootcat.com/2020/10/25/setproctitle/) 文章末尾提到了 `prctl` 这个函数，它也可以用来修改进程名。

只不过如果单单使用 prctl 来修改进程名的话，使用 ps 或者 top 等工具看到的可能还是原来的名字。

源代码可以在我的 github 找到：

[https://github.com/smaugx/setproctitle/blob/main/hidden_process/prctl_main.cc](https://github.com/smaugx/setproctitle/blob/main/hidden_process/prctl_main.cc)

```
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
```

编译运行：

```
#  g++ prctl_main.cc   -o  prctl_main -std=c++11
# ./prctl_main
```

然后我们查看一下进程的名字：

```
# ps -ef |grep prctl
root     20758 12289  0 17:39 pts/3    00:00:00 ./prctl_main
root     20791 20422  0 17:39 pts/1    00:00:00 grep --color=auto prctl
```

可以看到 ps 看到的进程名依然是 `prctl_main` 而不是 `prctl_new_name`。那么 `prctl` 函数到底修改了哪里呢？ ps 命令又是从哪里读取的进程名呢？

## /proc/\<pid\> 虚拟文件夹

linux 上一切皆文件，启动一个进程，就会在系统的 `/proc` 这个虚拟文件系统下创建这个进程相关的文件夹，里面记录了这个进程的数据。

```
# ls /proc/20758
attr       cgroup      comm             cwd      fd       io        map_files  mountinfo   net        oom_adj        pagemap      projid_map  schedstat  smaps  statm    task     wchan
autogroup  clear_refs  coredump_filter  environ  fdinfo   limits    maps       mounts      ns         oom_score      patch_state  root        sessionid  stack  status   timers
auxv       cmdline     cpuset           exe      gid_map  loginuid  mem        mountstats  numa_maps  oom_score_adj  personality  sched       setgroups  stat   syscall  uid_map
```

关注一下这两个虚拟文件:

```
# cat /proc/20758/cmdline
./prctl_main
```

以及

```
# cat /proc/20758/status
Name:	prctl_new_name
Umask:	0022
State:	S (sleeping)
Tgid:	20758
Ngid:	0
Pid:	20758
PPid:	12289
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0

...(省略）
```

细心的同学应该发现上面的不一致了吧， /proc/\<pid\>/cmdline 这个文件记录的进程名是 `prctl_main`，而 /proc/\<pid\>/status 中 Name 值记录的进程名是 `prctl_new_name`。而 ps 命令正好是读取了 cmdline 这个文件，导致即便使用 prctl 修改了进程名，但 ps 依然看到的是老的进程名。

另外要注意，**prctl() 这个函数有个限制，新进程的名字长度不能超过 16 字节（包括最后的 '\0'）**，详见手册：

[https://man7.org/linux/man-pages/man2/prctl.2.html](https://man7.org/linux/man-pages/man2/prctl.2.html)


# prctl 结合 argv[0]

上面的分析看到，不论是修改 argv[0] 还是使用 prctl，均有其局限性，那么通常可以结合两者来进行。

源码可以在我的 github 找到：

[https://github.com/smaugx/setproctitle/blob/main/hidden_process/hidden_main.cc](https://github.com/smaugx/setproctitle/blob/main/hidden_process/hidden_main.cc)

```
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
    const char *new_title = "hidden_main_new";
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
```

编译运行：

```
# ps -ef |grep hidd
root     21753 12289  0 17:55 pts/3    00:00:00 hidden_main_new
root     21760 20422  0 17:55 pts/1    00:00:00 grep --color=auto hidd

# cat /proc/21753/cmdline
hidden_main_new[root@Jiao ~]#

# cat /proc/21753/status |grep Name
Name:	hidden_main_new
```

可以看到，无论是通过 ps 命令还是直接查看 /proc/\<pid\>/ 下的文件的方式，均能看到修改后的名字： `hidden_main_new`。


# 隐藏进程
经过上一步，已经可以完美的修改进程名了，那么再进一步，如何隐藏进程呢？

```
const char *new_title = "";

```

只需要修改上述的一行代码，重新编译即可，然后用 ps 或者 top 看一下，能不能找到这个进程：

```
# ps -ef |grep hidden
root     22022 20422  0 17:59 pts/1    00:00:00 grep --color=auto hidden
```
可以看到 ps 无法找到 hidden* 相关的进程，那么 top 呢？

```
top - 18:01:06 up 16 days,  4:16,  9 users,  load average: 0.00, 0.01, 0.05
Tasks: 121 total,   1 running, 120 sleeping,   0 stopped,   0 zombie
%Cpu(s):  0.0 us,  0.0 sy,  0.0 ni,100.0 id,  0.0 wa,  0.0 hi,  0.0 si,  0.0 st
KiB Mem :  3879952 total,  3579624 free,    95660 used,   204668 buff/cache
KiB Swap:  5242876 total,  5164352 free,    78524 used.  3272224 avail Mem

  PID USER      PR  NI    VIRT    RES    SHR S  %CPU %MEM     TIME+ COMMAND
31038 root      20   0  115800    372    368 S   0.0  0.0   0:00.70 bash
22081 root      20   0       0      0      0 S   0.0  0.0   0:00.00 kworker/1:0
22078 root      20   0  162140   2252   1548 R   0.0  0.1   0:00.04 top
22013 root      20   0   11124   1068    908 S   0.0  0.0   0:00.00
21859 root      20   0  115892    244    240 S   0.0  0.0   0:00.46 bash
21725 root      20   0       0      0      0 S   0.0  0.0   0:00.00 kworker/1:2
20811 root      20   0       0      0      0 S   0.0  0.0   0:00.00 kworker/0:0
```

运行 top 命令，并且以 pid 倒叙排序，注意第四行的进程，可以看到 COMMAND 为空，这个进程就是刚才的这个进程，但是看不到进程名了，达到了简单的、初级的隐藏进程的目的。


# 扩展一下

上述相关代码均可以在我的 github 找到：

[https://github.com/smaugx/setproctitle/tree/main/hidden_process](https://github.com/smaugx/setproctitle/tree/main/hidden_process)

上面的讨论可以看到，能实现初级的，简单的进程隐藏，但是使用 top 命令还是能看到这个无名进程，那么这点改怎么解决呢？

这里就不展开了，我没有这方面的经验。不过通常来说有两种办法：

+ 修改进程名为常见的一些进程名，比如 bash, top, nginx 等以达到混淆的目的
+ 想办法把 /proc/\<pid\>/ 这个虚拟文件夹隐藏或者达到隐藏类的效果（不太擅长）





Blog:
 
+ [rebootcat.com](http://rebootcat.com)

+ email: <linuxcode2niki@gmail.com>

2020-10-25 于杭州   
*By  [史矛革](https://github.com/smaugx)*