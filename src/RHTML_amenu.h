//////////////////////////////////////////////////////////////////////
//      Globe Webeditor © Gergely Rózsahegyi (RoGer) 2001-2002      //
//                                                                  //
//                          AMenu Header File                       //
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

#ifndef _RHTMLAMENU_H_
#define _RHTMLAMENU_H_

// - Includes
#include <View.h>
#include <Window.h>
#include <Message.h>
#include "RHTML_tabbutton.h"
#include "RHTML_options.h"
#include "ToolBar.h"

class RHTMLWin;
class RHTMLtabbutton;

// --------------------------------------------------------------------------------------- RHTML_amenu Class -
class RHTMLamenu : public BToolBar
{
 public:
								RHTMLamenu(BRect fRect);
				virtual void	SetOrientation(orient ori);								
				virtual void	SetIndex(uint32 index);
				void			AddTab(const char *name);
				void			RemoveTab(uint8 num);
				void			ChangeTab(uint8 num);				
				void			Changed(uint8 num);				
				void			Removed(uint8 num);				
				void			SetTabText(uint8 num,const char *text);			
				const char *	GetTabText(uint8 num);
				virtual void	MessageReceived(BMessage *msg);
 private:
				BView			*fview;
				BView			*f2view;
				BView			*bview;
				RHTMLtabbutton	**tb;
				uint8			tbdb;
};

#endif

// - End - AMenu - Header File -------------------------------------------------------------------------------
