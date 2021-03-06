/*
 $Author: All $
 $RCSfile: select.h,v $
 $Date: 2003/11/29 03:31:16 $
 $Revision: 2.3 $
 */

#ifndef SELECT_H
#define SELECT_H

#include "essential.h"
#include "queue.h"

#ifdef _WINDOWS
#include <winsock.h>
#else
#include <sys/select.h>
#endif

extern int bPipeSignal;

int PipeRead(int fd, char *buf, size_t count);
int PipeWrite(int fd, char *buf, size_t count);

#define SELECT_READ 0x01
#define SELECT_WRITE 0x02
#define SELECT_EXCEPT 0x04

class cCaptainHook;

class cHook
{
  friend class cCaptainHook;

public:
  cHook(void);
  virtual ~cHook(void);

  int tfd(void);
  virtual int IsHooked(void);
  virtual void Unhook(void);

  virtual void Write(ubit8 *pData, ubit32 nLen, int bCopy = TRUE);
  int ReadToQueue(void);

  cQueue qRX;

protected:
  void PushWrite(void);
  virtual void Input(int nFlags) = 0;

  cQueue qTX;

private:
  int fd;
  int id;
};

class cCaptainHook
{
  friend class cHook;

public:
  cCaptainHook(void);
  ~cCaptainHook(void);

  void Close(void);
  void Hook(int nHandle, cHook *hook);
  int Wait(struct timeval *timeout);

private:
  void Unhook(cHook *hook);

  fd_set read_set, write_set;

  cHook *pfHook[256];

  int nIdx[256];
  int nMax;
  int nTop;
};

extern cCaptainHook CaptainHook;

#endif
