//////////////////////////////////////////////////////////////////////
//      Globe Webeditor © Gergely Rózsahegyi (RoGer) 2001-2003      //
//                                                                  //
//                          BMenu Header File                       //
//                                                                  //
//                       Last Update: 2003.03.18					//
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

#ifndef _RHTMLBMENU_H_
#define _RHTMLBMENU_H_

// - Includes
#include "RHTML_win.h"
#include "RHTML_options.h"
#include "RHTML_parammenu.h"
#include "ToolBar.h"
#include "RadioBar.h"
#include "ToolItem.h"
#include "RTimer.h"
#include <View.h>
#include <CheckBox.h>
#include <TextControl.h>
#include <ListView.h>

class RHTMLsearch;
class RHTMLspecchars;
class RHTMLWin;
class RHTMLtextcontrol;
class RHTMLparammenu;
class RHTMLbmenuview;
class RTimer;

// --------------------------------------------------------------------------------------- RHTML_bmenu Class -
class RHTMLbmenu : public BToolBar
{
 public:
								RHTMLbmenu(BRect fRect, RHTMLWin *fwin);
				virtual void	SetOrientation(orient ori);
				virtual void	SetVerticalWidth(float width);
				virtual void	FrameResized(float width, float height);
				virtual	void	MouseDown(BPoint where);
				virtual	void	MouseUp(BPoint where);
				virtual	void	MouseMoved(BPoint where, uint32 code, const BMessage *a_message);
				virtual void	MessageReceived(BMessage *msg);
				RHTMLsearch		*fSearchView;
				RHTMLparammenu	*fParamMenuView;
				BView			*fTagView;
 private:
				BRadioBar		*fRadioBar;
				BRadioItem		*fSearchItem;
				BRadioItem		*fParamMenuItem;
				BRadioItem		*fTagItem;
				BView			*fView;
				BImageItem 		*fImageItem;
				BPoint			fMousePos;
				bool			fIttNyomta;
				int				fIrany;
				int				fMozgas;
				RTimer			*fTimer;
};

// -------------------------------------------------------------------------------------- RHTML_search Class -
class RHTMLsearch : public BView
{
 public:
								RHTMLsearch(BRect fRect, RHTMLWin *fwin);
				virtual void	MessageReceived(BMessage *msg);
			RHTMLtextcontrol	*fSearch;
			RHTMLtextcontrol	*fReplace;
			RHTMLtextcontrol	*fLine;
				BCheckBox		*fCaseCheck;
				RHTMLWin 		*fWin;
 private:
};

// --------------------------------------------------------------------------------- RHTML_search_text Class -
class RHTMLsearchtext : public BTextView
{
 public:
								RHTMLsearchtext(BRect fRect, const char *name, BMessage *msg, uint32 resizeMask, uint32	flags = B_WILL_DRAW | B_PULSE_NEEDED);
				virtual	void	KeyDown(const char *bytes, int32 numBytes);
				virtual	void	MakeFocus(bool focusState = true);
 private:
 				BMessage		*fMessage;
};

// -------------------------------------------------------------------------------- RHTML_text_control Class -
class RHTMLtextcontrol : public BView
{
 public:
								RHTMLtextcontrol(BRect fRect, const char *name, const char *label, const char *text, BMessage *msg, uint32 rmask = B_FOLLOW_LEFT | B_FOLLOW_TOP, uint32 flags = B_WILL_DRAW | B_NAVIGABLE);
				virtual	void	Draw(BRect inRect);
				const char 		*Text() const;
				BTextView		*TextView() const;
				virtual	void	MakeFocus(bool focusState = true);
				virtual	void	SetDivider(float dividing_line);
 private:
				RHTMLsearchtext *fTextView;
				BStringView		*fStringView;
				float			divider;
};

#endif

// - End - BMenu - Header File -------------------------------------------------------------------------------
