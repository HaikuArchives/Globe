//////////////////////////////////////////////////////////////////////
//      Globe Webeditor © Gergely Rózsahegyi (RoGer) 2001-2002      //
//                                                                  //
//                       fmenu View  Header File                    //
//                                                                  //
//                       Last Update: 2002.07.15					//
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

#ifndef _RHTMLFMENU_H_
#define _RHTMLFMENU_H_

// - Includes
#include <View.h>
#include <String.h>
#include "ToolBar.h"
#include "TToolTip.h"
#include "RHTML_options.h"

class RHTMLfmenu;

// - Button_View Class ---------------------------------------------------------------------------------------
class ButtonView : public BCheckItem
{
 public:
								ButtonView(const char *name, const char *filename1, const char *filename2, BMessage *msg,const char *alt="");
				void			AddCmd(const char *cmd,const char *param = NULL);
				virtual void	MouseMoved(BPoint point, uint32 transit,const BMessage *message);
				virtual void	MessageReceived(BMessage *msg);
 private:
				uint8			fNum;
				uint8			cmddb;
				BString			fCmd[10];
				BString			fParam[10];
				BString			*fAlt;
				RHTMLOptions	*fOptions;
				bool			fIn;				
};


// - RHTML_fmenu Class ---------------------------------------------------------------------------------------
class RHTMLfmenu : public BToolBar
{
 public:
								RHTMLfmenu(BRect fRect);
				virtual void	SetOrientation(orient ori);								
				virtual void	SetIndex(uint32 index);
				virtual void	MessageReceived(BMessage *msg);
 private:
				BView			*fview;
				BView			*aview;
				BView			*bview;
				ButtonView		**buttons;
				uint8			buttondb;
				float			x;
};

#endif

// - End - fmenu View - Header File --------------------------------------------------------------------------
