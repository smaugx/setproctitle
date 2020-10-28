# setproctitle
set process title just like nginx.

# build

```
sh build.sh
```

# run

```
# ./main


#######this is father process###########


#######this is child process###########
master mem:0x7ffd2ee22573 len:20 argv[0]: setproctitle: master

master mem:0x1445c20 len:19 envp[0]: XDG_SESSION_ID=2428
master mem:0x1445c34 len:13 envp[1]: HOSTNAME=Jiao
master mem:0x1445c42 len:23 envp[2]: SELINUX_ROLE_REQUESTED=
worker mem:0x7ffd2ee22573 len:20 argv[0]: setproctitle: worker

worker mem:0x1445c20 len:19 envp[0]: XDG_SESSION_ID=2428
worker mem:0x1445c34 len:13 envp[1]: HOSTNAME=Jiao
worker mem:0x1445c42 len:23 envp[2]: SELINUX_ROLE_REQUESTED=
worker mem:0x1445c5a len:19 envp[3]: TERM=xterm-256color
worker mem:0x1445c6e len:15 envp[4]: SHELL=/bin/bash
worker mem:0x1445c7e len:13 envp[5]: HISTSIZE=1000
worker mem:0x1445c8c len:31 envp[6]: SSH_CLIENT=192.168.1.3 55187 22
worker mem:0x1445cac len:26 envp[7]: SELINUX_USE_CURRENT_RANGE=
worker mem:0x1445cc7 len:75 envp[8]: ANDROID_NDK_TOOLCHAIN_ROOT=/root/smaug/software/android-ndk-r16b/toolchains
worker mem:0x1445d13 len:17 envp[9]: OLDPWD=/root/temp
worker mem:0x1445d25 len:18 envp[10]: SSH_TTY=/dev/pts/1
worker mem:0x1445d38 len:9 envp[11]: USER=root
worker mem:0x1445d42 len:75 envp[12]: LD_LIBRARY_PATH=/usr/local/lib:/usr/local/python3/lib::/usr/local/topio/lib
worker mem:0x1445d8e len:1719 envp[13]: LS_COLORS=rs=0:di=38;5;27:ln=38;5;51:mh=44;38;5;15:pi=40;38;5;11:so=38;5;13:do=38;5;5:bd=48;5;232;38;5;11:cd=48;5;232;38;5;3:or=48;5;232;38;5;9:mi=05;48;5;232;38;5;15:su=48;5;196;38;5;15:sg=48;5;11;38;5;16:ca=48;5;196;38;5;226:tw=48;5;10;38;5;16:ow=48;5;10;38;5;21:st=48;5;21;38;5;15:ex=38;5;34:*.tar=38;5;9:*.tgz=38;5;9:*.arc=38;5;9:*.arj=38;5;9:*.taz=38;5;9:*.lha=38;5;9:*.lz4=38;5;9:*.lzh=38;5;9:*.lzma=38;5;9:*.tlz=38;5;9:*.txz=38;5;9:*.tzo=38;5;9:*.t7z=38;5;9:*.zip=38;5;9:*.z=38;5;9:*.Z=38;5;9:*.dz=38;5;9:*.gz=38;5;9:*.lrz=38;5;9:*.lz=38;5;9:*.lzo=38;5;9:*.xz=38;5;9:*.bz2=38;5;9:*.bz=38;5;9:*.tbz=38;5;9:*.tbz2=38;5;9:*.tz=38;5;9:*.deb=38;5;9:*.rpm=38;5;9:*.jar=38;5;9:*.war=38;5;9:*.ear=38;5;9:*.sar=38;5;9:*.rar=38;5;9:*.alz=38;5;9:*.ace=38;5;9:*.zoo=38;5;9:*.cpio=38;5;9:*.7z=38;5;9:*.rz=38;5;9:*.cab=38;5;9:*.jpg=38;5;13:*.jpeg=38;5;13:*.gif=38;5;13:*.bmp=38;5;13:*.pbm=38;5;13:*.pgm=38;5;13:*.ppm=38;5;13:*.tga=38;5;13:*.xbm=38;5;13:*.xpm=38;5;13:*.tif=38;5;13:*.tiff=38;5;13:*.png=38;5;13:*.svg=38;5;13:*.svgz=38;5;13:*.mng=38;5;13:*.pcx=38;5;13:*.mov=38;5;13:*.mpg=38;5;13:*.mpeg=38;5;13:*.m2v=38;5;13:*.mkv=38;5;13:*.webm=38;5;13:*.ogm=38;5;13:*.mp4=38;5;13:*.m4v=38;5;13:*.mp4v=38;5;13:*.vob=38;5;13:*.qt=38;5;13:*.nuv=38;5;13:*.wmv=38;5;13:*.asf=38;5;13:*.rm=38;5;13:*.rmvb=38;5;13:*.flc=38;5;13:*.avi=38;5;13:*.fli=38;5;13:*.flv=38;5;13:*.gl=38;5;13:*.dl=38;5;13:*.xcf=38;5;13:*.xwd=38;5;13:*.yuv=38;5;13:*.cgm=38;5;13:*.emf=38;5;13:*.axv=38;5;13:*.anx=38;5;13:*.ogv=38;5;13:*.ogx=38;5;13:*.aac=38;5;45:*.au=38;5;45:*.flac=38;5;45:*.mid=38;5;45:*.midi=38;5;45:*.mka=38;5;45:*.mp3=38;5;45:*.mpc=38;5;45:*.ogg=38;5;45:*.ra=38;5;45:*.wav=38;5;45:*.axa=38;5;45:*.oga=38;5;45:*.spx=38;5;45:*.xspf=38;5;45:
master mem:0x1445c5a len:19 envp[3]: TERM=xterm-256color
worker mem:0x1446446 len:25 envp[14]: MAIL=/var/spool/mail/root
worker mem:0x1446460 len:193 envp[15]: PATH=/root/.cargo/bin:/root/.cargo/bin:/usr/local/vim/bin:/usr/local/bin:/usr/loca/python3/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/usr/local/go/bin:/root/bin:/usr/local/topio/bin
worker mem:0x1446522 len:21 envp[16]: MARKPATH=/root/.marks
worker mem:0x1446538 len:27 envp[17]: PWD=/root/temp/setproctitle
worker mem:0x1446554 len:16 envp[18]: LANG=zh_CN.UTF-8
worker mem:0x1446565 len:38 envp[19]: PS1=\[\e[32;1m\][\u@\h \w]\$ \[\e[0m\]
worker mem:0x144658c len:24 envp[20]: SELINUX_LEVEL_REQUESTED=
worker mem:0x14465a5 len:22 envp[21]: HISTCONTROL=ignoredups
worker mem:0x14465bc len:7 envp[22]: SHLVL=1
worker mem:0x14465c4 len:10 envp[23]: HOME=/root
worker mem:0x14465cf len:12 envp[24]: LOGNAME=root
worker mem:0x14465dc len:47 envp[25]: SSH_CONNECTION=192.168.1.3 55187 192.168.1.4 22
worker mem:0x144660c len:16 envp[26]: GOPATH=/root/.go
worker mem:0x144661d len:34 envp[27]: LESSOPEN=||/usr/bin/lesspipe.sh %s
worker mem:0x1446640 len:49 envp[28]: ANDROID_NDK=/root/smaug/software/android-ndk-r16b
master mem:0x1445c6e len:15 envp[4]: SHELL=/bin/bash
master mem:0x1445c7e len:13 envp[5]: HISTSIZE=1000
master mem:0x1445c8c len:31 envp[6]: SSH_CLIENT=192.168.1.3 55187 22
master mem:0x1445cac len:26 envp[7]: SELINUX_USE_CURRENT_RANGE=
master mem:0x1445cc7 len:75 envp[8]: ANDROID_NDK_TOOLCHAIN_ROOT=/root/smaug/software/android-ndk-r16b/toolchains
master mem:0x1445d13 len:17 envp[9]: OLDPWD=/root/temp
master mem:0x1445d25 len:18 envp[10]: SSH_TTY=/dev/pts/1
master mem:0x1445d38 len:9 envp[11]: USER=root
master mem:0x1445d42 len:75 envp[12]: LD_LIBRARY_PATH=/usr/local/lib:/usr/local/python3/lib::/usr/local/topio/lib
master mem:0x1445d8e len:1719 envp[13]: LS_COLORS=rs=0:di=38;5;27:ln=38;5;51:mh=44;38;5;15:pi=40;38;5;11:so=38;5;13:do=38;5;5:bd=48;5;232;38;5;11:cd=48;5;232;38;5;3:or=48;5;232;38;5;9:mi=05;48;5;232;38;5;15:su=48;5;196;38;5;15:sg=48;5;11;38;5;16:ca=48;5;196;38;5;226:tw=48;5;10;38;5;16:ow=48;5;10;38;5;21:st=48;5;21;38;5;15:ex=38;5;34:*.tar=38;5;9:*.tgz=38;5;9:*.arc=38;5;9:*.arj=38;5;9:*.taz=38;5;9:*.lha=38;5;9:*.lz4=38;5;9:*.lzh=38;5;9:*.lzma=38;5;9:*.tlz=38;5;9:*.txz=38;5;9:*.tzo=38;5;9:*.t7z=38;5;9:*.zip=38;5;9:*.z=38;5;9:*.Z=38;5;9:*.dz=38;5;9:*.gz=38;5;9:*.lrz=38;5;9:*.lz=38;5;9:*.lzo=38;5;9:*.xz=38;5;9:*.bz2=38;5;9:*.bz=38;5;9:*.tbz=38;5;9:*.tbz2=38;5;9:*.tz=38;5;9:*.deb=38;5;9:*.rpm=38;5;9:*.jar=38;5;9:*.war=38;5;9:*.ear=38;5;9:*.sar=38;5;9:*.rar=38;5;9:*.alz=38;5;9:*.ace=38;5;9:*.zoo=38;5;9:*.cpio=38;5;9:*.7z=38;5;9:*.rz=38;5;9:*.cab=38;5;9:*.jpg=38;5;13:*.jpeg=38;5;13:*.gif=38;5;13:*.bmp=38;5;13:*.pbm=38;5;13:*.pgm=38;5;13:*.ppm=38;5;13:*.tga=38;5;13:*.xbm=38;5;13:*.xpm=38;5;13:*.tif=38;5;13:*.tiff=38;5;13:*.png=38;5;13:*.svg=38;5;13:*.svgz=38;5;13:*.mng=38;5;13:*.pcx=38;5;13:*.mov=38;5;13:*.mpg=38;5;13:*.mpeg=38;5;13:*.m2v=38;5;13:*.mkv=38;5;13:*.webm=38;5;13:*.ogm=38;5;13:*.mp4=38;5;13:*.m4v=38;5;13:*.mp4v=38;5;13:*.vob=38;5;13:*.qt=38;5;13:*.nuv=38;5;13:*.wmv=38;5;13:*.asf=38;5;13:*.rm=38;5;13:*.rmvb=38;5;13:*.flc=38;5;13:*.avi=38;5;13:*.fli=38;5;13:*.flv=38;5;13:*.gl=38;5;13:*.dl=38;5;13:*.xcf=38;5;13:*.xwd=38;5;13:*.yuv=38;5;13:*.cgm=38;5;13:*.emf=38;5;13:*.axv=38;5;13:*.anx=38;5;13:*.ogv=38;5;13:*.ogx=38;5;13:*.aac=38;5;45:*.au=38;5;45:*.flac=38;5;45:*.mid=38;5;45:*.midi=38;5;45:*.mka=38;5;45:*.mp3=38;5;45:*.mpc=38;5;45:*.ogg=38;5;45:*.ra=38;5;45:*.wav=38;5;45:*.axa=38;5;45:*.oga=38;5;45:*.spx=38;5;45:*.xspf=38;5;45:
master mem:0x1446446 len:25 envp[14]: MAIL=/var/spool/mail/root
master mem:0x1446460 len:193 envp[15]: PATH=/root/.cargo/bin:/root/.cargo/bin:/usr/local/vim/bin:/usr/local/bin:/usr/loca/python3/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/usr/local/go/bin:/root/bin:/usr/local/topio/bin
master mem:0x1446522 len:21 envp[16]: MARKPATH=/root/.marks
master mem:0x1446538 len:27 envp[17]: PWD=/root/temp/setproctitle
master mem:0x1446554 len:16 envp[18]: LANG=zh_CN.UTF-8
master mem:0x1446565 len:38 envp[19]: PS1=\[\e[32;1m\][\u@\h \w]\$ \[\e[0m\]
master mem:0x144658c len:24 envp[20]: SELINUX_LEVEL_REQUESTED=
master mem:0x14465a5 len:22 envp[21]: HISTCONTROL=ignoredups
master mem:0x14465bc len:7 envp[22]: SHLVL=1
master mem:0x14465c4 len:10 envp[23]: HOME=/root
master mem:0x14465cf len:12 envp[24]: LOGNAME=root
master mem:0x14465dc len:47 envp[25]: SSH_CONNECTION=192.168.1.3 55187 192.168.1.4 22
master mem:0x144660c len:16 envp[26]: GOPATH=/root/.go
master mem:0x144661d len:34 envp[27]: LESSOPEN=||/usr/bin/lesspipe.sh %s
master mem:0x1446640 len:49 envp[28]: ANDROID_NDK=/root/smaug/software/android-ndk-r16b
master mem:0x1446672 len:27 envp[29]: XDG_RUNTIME_DIR=/run/user/0
master mem:0x144668e len:8 envp[30]: _=./main
worker mem:0x1446672 len:27 envp[29]: XDG_RUNTIME_DIR=/run/user/0
worker mem:0x144668e len:8 envp[30]: _=./main

```

验证一下：

```
# ps -ef |grep setpro
root      2332 20422  0 16:33 pts/1    00:00:00 setproctitle: master
root      2333  2332  0 16:33 pts/1    00:00:00 setproctitle: worker
root      2364 12289  0 16:33 pts/3    00:00:00 grep --color=auto setpro
[root@Jiao ~/temp]#
```

