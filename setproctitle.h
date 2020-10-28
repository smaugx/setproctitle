// author: smaug

#pragma once


#ifndef _SMAUG_SETPROCTITLE_H_INCLUDED_
#define _SMAUG_SETPROCTITLE_H_INCLUDED_

#define SMAUG_PROCTITLE_ERROR  -1
#define SMAUG_PROCTITLE_OK   0

int smaug_init_setproctitle();
void smaug_setproctitle(const char *title);

#endif /* _SMAUG_SETPROCTITLE_H_INCLUDED_ */
