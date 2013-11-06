//////////////////////////////////////////////////////////////////////
//      Globe Webeditor © Gergely Rózsahegyi (RoGer) 2001-2004      //
//                                                                  //
//                        Text View Header File                     //
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

#ifndef _RHTMLTEXTVIEW_H_
#define _RHTMLTEXTVIEW_H_

// - Includes
#include <TextView.h>
#include <ScrollView.h>
#include <String.h>
#include <Looper.h>
#include "RHTML_tagmenu.h"
#include "RHTML_options.h"
#include "RHTML_filetypes.h"
#include "RHTML_numview.h"
#include "RHTML_parammenu.h"
//#include "HUserTimer.h"
#include "RTimer.h"

class RHTMLpmenu;
class RHTMLtext;
class RHTMLpomenu;
class HUserTimer;
class RHTMLtextLooper;
class RHTMLparammenu;
class RHTMLTagMenu;
class RTimer;

// ----------------------------------------------------------------------------------- RHTML_text_view Class -
class RHTMLtextview : public BTextView
{
 public:
								RHTMLtextview(BRect fRect, RHTMLtext *target, uint ft);
				virtual			~RHTMLtextview();
				virtual void	MessageReceived(BMessage *msg);
			    virtual	void	KeyDown(const char *bytes, int32 numBytes);
				virtual	void	MouseDown(BPoint where);
				virtual	void	FrameResized(float new_width, float new_height);
				virtual	void	Draw(BRect inRect);
			    void			UpdateParamMenu();
				void			SetParamValue(BString tag, BString param, BString value);

				RHTMLTagMenu	*fTagMenu;
				int				mode;
				BString			buf1;
				BString			buf2;
				std::vector<BString> tags;
				RTimer			*fTimer;
				BScrollView		*fScrollView;
				bool			pushmenu;
				BFont			*fFont;
				RHTMLFileTypes	*fFileTypes;
				RTimer			*fUpdateTimer;
 private:
				virtual	void	Cut(BClipboard *clipboard);
				virtual	void	Paste(BClipboard *clipboard);
				virtual	void	Undo(BClipboard *clipboard);
				void			DropFile(const char *filename, const char *type, int32 offset);
				BString			GetRelativePath(const char *path);
				RHTMLtext		*fTarget;
};


// ---------------------------------------------------------------------------------------- RHTML_text Class -
class RHTMLtext : public BView
{
 public:
								RHTMLtext(BRect fRect, BLooper *plooper, uint ft, RHTMLparammenu *parammenu);
				virtual			~RHTMLtext();
				void			SetNumView(bool kibe);
				void			SetWordWrap(bool kibe);
				void			SetEnabled(bool kibe);
				bool			IsEnabled();
				bool			Modify();
				void			SetModify(bool kibe);
				virtual void	MessageReceived(BMessage *msg);
				RHTMLtextview	*fTextView;
				BString			*fFileName;
				BLooper			*fPLooper;
				RHTMLNumView	*fNumView;
				RHTMLparammenu	*fParamMenu;
 private:
				bool			modify;
				bool			fEnabled;
};

#endif

// - End - Text View - Header File ---------------------------------------------------------------------------
