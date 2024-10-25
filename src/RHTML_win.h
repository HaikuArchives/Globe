//////////////////////////////////////////////////////////////////////
//      Globe Webeditor © Gergely Rózsahegyi (RoGer) 2001-2003      //
//                                                                  //
//                         Window  Header File                      //
//                                                                  //
//                       Last Update: 2003.03.04					//
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

#ifndef _RHTMLWIN_H_
#define _RHTMLWIN_H_

// - Includes
#include <Window.h>
#include <FilePanel.h>
#include <MenuItem.h>
#include <Messenger.h>
#include "RHTML_fmenu.h"
#include "RHTML_buttonmenu.h"
#include "RHTML_bmenu.h"
#include "RHTML_amenu.h"
#include "RHTML_textview.h"
#include "RHTML_prefwin.h"
#include "RHTML_options.h"
#include "RHTML_savewin.h"
#include "RTimer.h"
#include "ToolView.h"
#include "ToolBar.h"
#include "ToolItem.h"
#include "TToolTip.h"

enum {
	NUL = 0,
	LF = 10,
	CR = 13,
	ESC = 27,
	SS2 = 142
};

class RHTMLamenu;
class RHTMLfmenu;
class RHTMLbuttonmenu;
class RHTMLPreferencesWin;
class RHTMLbmenu;
class RHTMLparammenu;
class RHTMLtext;
class RHTMLtext;
class RTimer;

// ----------------------------------------------------------------------------------------- RHTML_Win Class -
class RHTMLWin : public BWindow
{
 public:
								RHTMLWin(BRect fRect,int argc,char **argv);
				virtual bool	QuitRequested();
				virtual void	MessageReceived(BMessage *msg);
				virtual	void	FrameResized(float new_width, float new_height);
				virtual	void	FrameMoved(BPoint new_position);
				void			AddText(const char *name,uint ft);
				void			RemoveText(uint8 num);
				void			ChangeText(uint8 num);
				void			Open(const char *name,const char *path);
				void			Save(uint8 num,const char * FileName);
				void			Save_All(int num);
				void			Command(const char *cmd,const char *param);
				
				char			Version[50];
				char			Name[30];
				char			Web[50];
				char			Email1[50];
				char			Email2[50];
				RHTMLfmenu		*fmenu;
				RHTMLbuttonmenu	*buttonmenu;
				RHTMLtext		**textview;
				RHTMLamenu		*amenu;
				uint8			textdb;
				uint8			current;
				bool			fSaveWinShow;
				bool			fPrefWinShow;
				bool			savequit;
				bool			saveclose;
				bool			saverun;
				BFilePanel		*fSavePanel;

 private:
 				BToolView		*fToolView;
 				BView			*fView;
				RHTMLbmenu		*bmenu;
				float			fmenuheight;
				float			bmenuwidth;
				uint8			untitled;
				BFilePanel		*fOpenPanel;
				BMenuItem		*save;
				BMenuItem		*save_as;
				BMenuItem		*save_all;
				BMenuItem		*close;
				BMenuItem		*undo;
				BMenuItem		*copy;
				BMenuItem		*paste;
				BMenuItem		*cut;
				BMenuItem		*selectall;
				BMenuItem		*fTB;
				BMenuItem		*fLB;
				BMenuItem		*fNW;
				BMenuItem		*fCE[25];
				BMenuItem		*fCRLF_LF;
				BMenuItem		*fLF_CRLF;
				BMenuItem		*fWordWrap;
				RTimer			*fUpdateTimer;
				
				int				saveall;
				RHTMLPreferencesWin	*fPrefWin;
				RHTMLOptions	*Options;
			    RHTMLSaveAllWin	*fSaveWin;
			    RHTMLSaveCloseWin	*fSaveCloseWin;
	    
};

#endif

// - End - Window - Header File ------------------------------------------------------------------------------
