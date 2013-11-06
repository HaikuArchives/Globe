//////////////////////////////////////////////////////////////////////
//    Extended BeOS Api © Gergely Rózsahegyi (RoGer) 2002           //
//                          roger@beos.hu                           //
//                                                                  //
//                      BRadioBar Header File                       //
//                                                                  //
//                     Last Update: 2002.06.29                      //
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

#ifndef _BRADIOBAR_H_
#define _BRADIOBAR_H_

// - Includes
#include <View.h>
#include "ToolView.h"
#include "ToolBar.h"
#include "ToolItem.h"

class BToolItem;
class BToolBar;

// ------------------------------------------------------------------------------------------ BRadioBar Class -
class BRadioBar : public BToolBar
{
 public:
								BRadioBar(const char *name, BRect hmargin = BRect(10,5,5,5), BRect vmargin = BRect(5,10,5,5), float hspace = 0, float vspace =0, bool autoresize = true, float hwidth = 50, float vheight = 30);
				void			AddToolItem(BRadioItem *toolitem);
				virtual void	MessageReceived(BMessage *msg);
 private:
};

#endif

// - End - BRadioBar - Header File ----------------------------------------------------------------------------
