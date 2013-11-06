//////////////////////////////////////////////////////////////////////
//       Extended BeOS Api © Gergely Rózsahegyi (RoGer) 2002        //
//                          roger@beos.hu                           //
//                                                                  //
//                      BToolView Header File                       //
//                                                                  //
//                     Last Update: 2002.06.11                      //
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

#ifndef _BTOOLVIEW_H_
#define _BTOOLVIEW_H_

// - Enum
enum orient
{
 B_LEFT,
 B_RIGHT,
 B_TOP,
 B_BOTTOM
};

// - Includes
#include <View.h>
#include "ToolBar.h"

class BToolBar;

// ----------------------------------------------------------------------------------------- BToolView Class -
class BToolView : public BView
{
 public:
								BToolView(BRect frame, BView *view, uint32 resizeMask = B_FOLLOW_ALL, uint32 flags = B_FULL_UPDATE_ON_RESIZE | B_WILL_DRAW | B_NAVIGABLE);
				virtual	void	MouseUp(BPoint where);
				virtual	void	MouseMoved(BPoint where, uint32 code, const BMessage *a_message);
				void			AddToolBar(BToolBar *fBar, orient ori);
				void			RemoveToolBar(BToolBar *fBar);
				uint32			Index(BToolBar *fBar);
				void			SetIndex(BToolBar *fBar,uint32 index);
				virtual void	MessageReceived(BMessage *msg);
 private:
 				void			UpdateView();
 				void			UpdateLeftRight();
 				void			Update();
				float			GetLeft();
				float			GetRight();
				float			GetTop();
				float			GetBottom();
 				BToolBar		**fBBL;
 				BToolBar		**fBBR;
 				BToolBar		**fBBT;
 				BToolBar		**fBBB;
 				uint32			BBDbL;
 				uint32			BBDbR;
 				uint32			BBDbT;
 				uint32			BBDbB;
 				bool			isdown;
 				BView			*fView;
				BToolBar		*fBBar;
};

#endif

// - End - BToolView - Header File ---------------------------------------------------------------------------
