//////////////////////////////////////////////////////////////////////
//      Globe Webeditor © Gergely Rózsahegyi (RoGer) 2001-2002      //
//                                                                  //
//                       Button Menu Header File                    //
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

#ifndef _RHTMLBUTTONMENU_H_
#define _RHTMLBUTTONMENU_H_

#include <View.h>
#include <StringView.h>
#include <String.h>
#include <PopUpMenu.h>
#include "RHTML_fmenu.h"
#include "RHTML_options.h"
#include "ToolBar.h"
#include "ToolItem.h"

class RHTMLbuttonmenu;
class RHTMLmenubutton;
class ButtonView;


// ------------------------------------------------------------------------ RHTML_button_menu_str_view Class -
class RHTMLbuttonmenustrview : public BStringView
{
public:
								RHTMLbuttonmenustrview(BRect fRect,const char *name,BString *alt,RHTMLmenubutton *target);
				virtual void	MouseMoved(BPoint point, uint32 transit,const BMessage *message);
						void	MouseDown(BPoint where);
				RHTMLmenubutton	*fTarget;
				bool			fAltWinShow;
				bool			fIn;
				BString			*fAlt;			
};

// --------------------------------------------------------------------------------- RHTML_menu_button Class -
class RHTMLmenubutton : public BView
{
 public:
								RHTMLmenubutton(BRect fRect,const char *name,const char *text,const char *alt,uint8 num,RHTMLbuttonmenu *target);
						void	SetON();
						void	SetOFF();
						void	MouseDown(BPoint where);
						void	BalGomb();
 private:
				char 	*name;
				BView	*j1view;
				BView	*j2view;
				RHTMLbuttonmenustrview *fstrview;
				uint8	fNum;
				BPopUpMenu		*fPopUpMenu;
				RHTMLbuttonmenu *fTarget;
				BString			*fName;			
				BString			*fText;			
};

// --------------------------------------------------------------------------------- RHTML_button_menu Class -
class RHTMLbuttonmenu : public BView
{
 public:
								RHTMLbuttonmenu(BRect fRect,BToolBar *fmenu);
						void	AddTab(const char *name,const char *text,const char *alt);
						void	AddMenuBar(const char *name);
						void	fChange(uint8 fnum);
				virtual void	MessageReceived(BMessage *msg);
				virtual	void	ResizeTo(float width, float height);
 private:
				BView			*fview;
				BView			*ffview;
				BView			*fffview;
				BView			*aview;
				BView			*bview;
				BView			*bbview;
				BView			*felsoview;
				BView			*alsoview;
				uint8			current;

	
				BToolBar		**menubar;
				uint8			menubardb;
				float			fx;
				BToolBar		*fMenu;

				RHTMLmenubutton	**tb;
				uint8			tbdb;
};

#endif

// - End - Button Menu - Header File -------------------------------------------------------------------------
