//////////////////////////////////////////////////////////////////////
//      Globe Webeditor © Gergely Rózsahegyi (RoGer) 2001-2003      //
//                                                                  //
//                         RTimer  Source File                      //
//                                                                  //
//                       Last Update: 2003.04.18                    //
//////////////////////////////////////////////////////////////////////
//                                                                  //
//  Copyright (C) 2001-2004 Gergely Rózsahegyi <roger@beos.hu>      //
//                                                                  //
//  This program is free software; you can redistribute it and/or   //
//  modify it under the terms of the GNU General Public License as  //
//  published by the Free Software Foundation; either version 2 of  //
//  the License, or (at your option) any later version.             //
//                                                                  //
//  This program is distributed in the hope that it will be useful, //
//  but WITHOUT ANY WARRANTY; without even the implied warranty of  //
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the   //
//  GNU General Public License for more details.                    //
//                                                                  //
//  You should have received a copy of the GNU General Public       //
//  License along with this program; if not, write to the           //
//  Free Software Foundation, Inc.,                                 //
//  59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.        //
//                                                                  //
//////////////////////////////////////////////////////////////////////

// - Includes
#include "RTimer.h"
#include <Autolock.h>
#include <ostream.h>

#define SLEEP_TIME 1000
// ---------------------------------------------------------------------------------------- R_Timer - RTimer -
RTimer::RTimer(const char *name, BHandler *target, BMessage *msg)
{
	fTarget = target;
	fMessage = msg;
	fCancel = true;
	fName.SetTo(name);
}

// --------------------------------------------------------------------------------------- R_Timer - ~RTimer -
RTimer::~RTimer()
{
	if(fCancel == false)
		EndTimer();
}

// ------------------------------------------------------------------------------------ R_Timer - StartTimer -
void RTimer::StartTimer(uint32 interval)
{
	fInterval = interval * 1000;
	fCancel = false;
	fCurrent = 0;
	fThread = ::spawn_thread(ThreadFunc, fName.String(), B_LOW_PRIORITY, this);
	::resume_thread(fThread);
}

// ------------------------------------------------------------------------------------ R_Timer - ThreadFunc -
int32 RTimer::ThreadFunc(void* data)
{
	RTimer *timer = (RTimer*)data;
	while(!timer->fCancel)
	{
		timer->fCurrent += SLEEP_TIME;
		if(timer->fCurrent >= timer->fInterval)
		{
			timer->fCurrent = 0;
			timer->Timer();
		}
		::snooze(SLEEP_TIME);
	}
	return 0;	
}

// ------------------------------------------------------------------------------------ R_Timer - ResetTimer -
void RTimer::ResetTimer()
{
	BAutolock lock(fLocker);
	fCurrent = 0;
}

// -------------------------------------------------------------------------------------- R_Timer - EndTimer -
void RTimer::EndTimer()
{
	fCancel = true;
	status_t exitStatus;
	exitStatus = ::kill_thread(fThread);
//	::wait_for_thread(fThread, &exitStatus);
}

// ----------------------------------------------------------------------------------------- R_Timer - Timer -
void RTimer::Timer()
{
//	cout << fName.String() <<": Alert Timer!" << endl;
	fTarget->MessageReceived(fMessage);
}

// -------------------------------------------------------------------------------------- R_Timer - IsActive -
bool RTimer::IsActive()
{
	return !fCancel;
}
// - End - R_Timer - Source File -----------------------------------------------------------------------------
