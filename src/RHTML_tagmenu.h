//////////////////////////////////////////////////////////////////////
//      Globe Webeditor © Gergely Rózsahegyi (RoGer) 2001-2004      //
//                                                                  //
//                         TagMenu Header File                      //
//                                                                  //
//                       Last Update: 2004.10.21					//
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

#ifndef _RHTMLTAGMENU_H_
#define _RHTMLTAGMENU_H_

// - Includes
#include <ListView.h>
#include <Window.h>
#include <ScrollView.h>
#include <TextView.h>
#include <String.h>

#include <vector>
#include <map>

#include "RHTML_colorwin.h"

struct Cmd
{
 BString type;
 BString param;
};

struct Value
{
 std::vector<Cmd> cmd;
};

struct Param
{
 BString text;
 BString type;
 std::vector<Cmd> cmd;
 std::map<BString,Value> value;
};

struct Tag
{
 BString text;
 std::vector<Cmd> cmd;
 std::map<BString,Param> param;
};

enum MenuType
{
 R_TAG_MENU,
 R_PARAM_MENU,
 R_VALUE_MENU
};

class RHTMLTagMenuList;
class RHTMLcolorwin;

// ------------------------------------------------------------------------------------ RHTML_Tag_Menu Class -
class RHTMLTagMenu : public BWindow
{
 public:
								RHTMLTagMenu(BTextView *textview);
				virtual			~RHTMLTagMenu();
				void			Start(BPoint point, int32 offset, MenuType menutype, std::vector<BString>& tags, BString tag="", BString param="");
				void			Stop();
				void			LoadList(BString str="");
				void			Put();
				virtual void	MessageReceived(BMessage *msg);
 private:
				BTextView		*fTextView;
				std::map<BString,Tag>	fTag;
				std::vector<BString> tags;
				RHTMLTagMenuList *fListView;
				MenuType		menutype;
				BString			tag;
				BString			param;
				RHTMLcolorwin	*fColorWin;
};


// ------------------------------------------------------------------------------------ RHTML_Tag_Menu Class -
class RHTMLTagMenuList : public BListView
{
 public:
								RHTMLTagMenuList(RHTMLTagMenu *tagmenu, BTextView *textview);
				virtual	void	MouseDown(BPoint where);
				virtual	void 	KeyDown(const char *bytes, int32 numBytes);
				int32			sel1;
				int32			sel2;
				BString			str;
 private:
				RHTMLTagMenu	*fTagMenu;
				BTextView		*fTextView;
};


#endif

// - End - TagMenu - Header File -----------------------------------------------------------------------------
