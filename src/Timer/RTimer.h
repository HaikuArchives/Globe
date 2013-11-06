//////////////////////////////////////////////////////////////////////
//      Globe Webeditor © Gergely Rózsahegyi (RoGer) 2001-2003      //
//                                                                  //
//                         RTimer  Header File                      //
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

#ifndef __RTIMER_H__
#define __RTIMER_H__

// - Includes
#include <Locker.h>
#include <Looper.h>
#include <Message.h>
#include <time.h>
#include <String.h>

// --------------------------------------------------------------------------------------- RHTML_Timer Class -
class RTimer {
 public:
								RTimer(const char *name, BHandler *target, BMessage *msg);
				virtual			~RTimer();
				void			StartTimer(uint32 interval);
				void 			ResetTimer();
				void			EndTimer();
				bool			IsActive();

 protected:
				static int32	ThreadFunc(void* data);
				virtual void	Timer();
				uint32	  		fInterval;
				BHandler		*fTarget;
				BMessage		*fMessage;
				uint32			fCurrent;
				thread_id		fThread;
				bool			fCancel;
				BLocker			fLocker;
				BString			fName;
};
// - End - RTimeru - Header File ------------------------------------------------------------------------------

#endif
