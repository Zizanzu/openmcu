/*
 * main.h
 *
 * PWLib application header file for threadex
 *
 * Copyright (c) 2003 Equivalence Pty. Ltd.
 *
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.0 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See
 * the License for the specific language governing rights and limitations
 * under the License.
 *
 * The Original Code is Portable Windows Library.
 *
 * The Initial Developer of the Original Code is Equivalence Pty. Ltd.
 *
 * Contributor(s): ______________________________________.
 *
 * $Log: main.h,v $
 * Revision 1.7  2006/06/21 03:28:43  csoutheren
 * Various cleanups thanks for Frederic Heem
 *
 * Revision 1.6  2006/01/20 00:34:09  dereksmithies
 * Add PTRACE statements to the DelayThread Constructors and Destructor.
 *
 * Revision 1.5  2006/01/13 07:20:22  dereksmithies
 * add option to use threads generated by the PThread::Create call.
 *
 * Revision 1.4  2006/01/11 22:14:48  dereksmithies
 * Add autodelete test option to code.
 * Move Resume() operators out of the constructor for threads.
 * The Resume() operator is "ok" in the constructor, if Resume() is at the end of the constructor,
 * and the thread class does not have a descendant.
 *
 * Revision 1.3  2006/01/06 23:08:24  dereksmithies
 * Add some code, now it crashes on unix with the command args -d 1 -b
 *
 * Revision 1.2  2005/07/26 00:46:22  dereksmithies
 * Commit code to provide two examples of waiting for a thread to terminate.
 * The busy wait method provides a method of testing PWLIB processes for closing
 * a thread. With a delay of 20ms, SMP box, we found some pwlib methods that
 * needed fixing. At the time of committing this change, the pwlib code was correct.
 *
 * Revision 1.1  2004/09/13 01:13:26  dereksmithies
 * Initial release of VERY simple program to test PThread::WaitForTermination
 *
 * Revision 1.1  2004/09/10 01:59:35  dereksmithies
 * Initial release of program to test Dtmf creation and detection.
 *
 *
 */

#ifndef _Threadex_MAIN_H
#define _Threadex_MAIN_H

#include <ptlib/pprocess.h>

/**This class is a simple simple thread that just creates, waits a
   period of time, and exits.It is designed to test the PwLib methods
   for reporting the status of a thread. This class will be created
   over and over- millions of times is possible if left long
   enough. If the pwlib thread status functions are broken, a segfault
   will result. Past enxperience has found a fault in pwlib with the
   BusyWait option on, with SMP machines and a delay period of 20ms */
class DelayThread : public PThread
{
  PCLASSINFO(DelayThread, PThread);
  
public:
  DelayThread(PINDEX _delay);
    
  DelayThread(PINDEX _delay, BOOL);
  
  ~DelayThread();

  void Main();
    
 protected:
  PINDEX delay;
};

////////////////////////////////////////////////////////////////////////////////
/**This thread handles the Users console requests to query the status of 
   the launcher thread. It provides a means for the user to close down this
   program - without having to use Ctrl-C*/
class UserInterfaceThread : public PThread
{
  PCLASSINFO(UserInterfaceThread, PThread);
  
public:
  UserInterfaceThread()
    : PThread(10000, NoAutoDeleteThread)
    { }

  void Main();
    
 protected:
};

///////////////////////////////////////////////////////////////////////////////
/**This thread launches multiple instances of the BusyWaitThread. Each
   thread launched is busy monitored for termination. When the thread
   terminates, the thread is deleted, and a new one is created. This
   process repeats until segfault or termination by the user */
class LauncherThread : public PThread
{
  PCLASSINFO(LauncherThread, PThread);
  
public:
  LauncherThread()
    : PThread(10000, NoAutoDeleteThread)
    { iteration = 0; keepGoing = TRUE; }
  
  void Main();
    
  PINDEX GetIteration() { return iteration; }

  virtual void Terminate() { keepGoing = FALSE; }

  PTimeInterval GetElapsedTime() { return PTime() - startTime; }

  PDECLARE_NOTIFIER(PThread, LauncherThread, AutoCreatedMain);

 protected:
  PINDEX iteration;
  PTime startTime;
  BOOL  keepGoing;
};

////////////////////////////////////////////////////////////////////////////////


class Threadex : public PProcess
{
  PCLASSINFO(Threadex, PProcess)

  public:
    Threadex();
    virtual void Main();

    PINDEX Delay()    { return delay; }

    BOOL AutoDelete() { return doAutoDelete; }

    BOOL BusyWait()   { return doBusyWait; }

    BOOL Create()     { return doCreate; }

   static Threadex & Current()
      { return (Threadex &)PProcess::Current(); }

 protected:

    PINDEX delay;

    BOOL doAutoDelete;

    BOOL doBusyWait;

    BOOL doCreate;
};



#endif  // _Threadex_MAIN_H


// End of File ///////////////////////////////////////////////////////////////