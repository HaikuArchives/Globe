//////////////////////////////////////////////////////////////////////
//      Globe Webeditor © Gergely Rózsahegyi (RoGer) 2001-2003      //
//                                                                  //
//                        ParamMenu Header File                     //
//                                                                  //
//                       Last Update: 2003.06.01                    //
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

#ifndef _RHTMLPARAMMENU_H_
#define _RHTMLPARAMMENU_H_

// - Includes
#include "RHTML_win.h"
#include "RHTML_colorwin.h"
#include "RHTML_tagmenu.h"
#include "ToolBar.h"
#include "RadioBar.h"
#include "ToolItem.h"
#include <View.h>
#include <CheckBox.h>
#include <TextControl.h>
#include <TextView.h>
#include <ListView.h>
#include <PopUpMenu.h>
#include <MenuItem.h>

#include <vector>
#include <map>

class RHTMLWin;
class RHTMLplistview;
class RHTMLplistitem;
class RHTMLptextview;
class RHTMLcolorwin;
class RHTMLTagMenu;

enum ParamType
{
 R_TEXT_PARAM,
 R_INT_PARAM,
 R_INT_SZ_PARAM,
 R_COLOR_PARAM,
 R_RADIO_PARAM
};

struct Params
{
 BString name;
 BString value;
};

struct Tag;

// ----------------------------------------------------------------------------------- RHTML_parammenu Class -
class RHTMLparammenu : public BView
{
 public:
								RHTMLparammenu(BRect fRect, RHTMLWin *fwin);
				virtual void	MessageReceived(BMessage *msg);
				RHTMLWin 		*fWin;
				RHTMLplistview	*fParamList;
 private:
};

// ------------------------------------------------------------------------------- RHTML_paramlistview Class -
class RHTMLplistview : public BListView
{
 public:
								RHTMLplistview(BRect rect, BWindow *win);
				virtual	void	MouseDown(BPoint where);
				virtual	void	AllAttached();
				void			SetTagParams(BString tag, std::vector<Params> params);
				void			SelectItem(BListItem *item);
				virtual void	SelectionChanged();
				int				GetItemNum(BListItem *item);
				std::map<BString,Tag>	GetTag();		
 				BScrollView		*fScrollView;
 				BStringView		*fTagLabel;
 				BString			fTagName;
				std::map<BString,Tag>	fTag;
				BWindow			*fWindow;
 private:
				std::vector<RHTMLplistitem*> fItems;
};

// ------------------------------------------------------------------------------- RHTML_paramlistitem Class -
class RHTMLplistitem : public BListItem
{
 public:
								RHTMLplistitem(BString name, BString value, ParamType type, BString tag, RHTMLplistview *listview);
								~RHTMLplistitem();
				virtual	void	AttachedToView();
				virtual	void	DrawItem(BView *owner, BRect bounds, bool complete = false);
				void			SetValue(BString value);
				void			ModifyValue(BString value);
				BString			GetValue();
				BString			GetName();
				void			SetSelected(bool select = true);
 private:
				RHTMLplistview	*fListView;
				RHTMLptextview	*fTextView;
				RHTMLcolorwin	*fColorWin;
				BPopUpMenu		*fPopUpMenu;
				BString			fName;
				BString			fValue;
				ParamType		fType;
};

// ----------------------------------------------------------------------------------- RHTML_ptextview Class -
class RHTMLptextview : public BTextView
{
 public:
								RHTMLptextview(BString value, ParamType type, RHTMLplistview *listview, RHTMLplistitem *listitem);
				virtual	void	MakeFocus(bool focusState = true);
				virtual	void	KeyDown(const char *bytes, int32 numBytes);
				virtual	void	Paste(BClipboard *clipboard);
 private:
				ParamType		fType;
				RHTMLplistitem	*fListItem;
				RHTMLplistview	*fListView;
				BString 		fValue;
};

#endif

// - End - ParamMenu - Header File ---------------------------------------------------------------------------
