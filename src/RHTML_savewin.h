//////////////////////////////////////////////////////////////////////
//      Globe Webeditor © Gergely Rózsahegyi (RoGer) 2001-2002      //
//                                                                  //
//                       Save Window Header File                    //
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

#ifndef _RHTMLSAVEWIN_H_
#define _RHTMLSAVEWIN_H_

// - Includes
#include <Window.h>
#include <ListView.h>
#include <View.h>
#include <Button.h>
#include <Message.h>
#include <ScrollView.h>
#include <Application.h>

// -------------------------------------------------------------------------------- RHTML_Save_All_Win Class -
class RHTMLSaveAllWin : public BWindow, public BInvoker
{
public:
								RHTMLSaveAllWin(BRect fRect,RHTMLWin *parent);
				virtual void	MessageReceived(BMessage *msg);
				virtual bool	QuitRequested();
				int				GetListItem(int num);
				int				GetListItemCount();
private:
				int				*list;
				int				listdb;
				int				*list2;
				int				list2db;
				BListView		*fList;
				RHTMLWin		*fParent;
				BView			*fView;
				BButton			*fSave;
				BButton			*fCancel;
				BButton			*fQuit;
				BScrollView		*fScrollView;
};

// ------------------------------------------------------------------------------ RHTML_Save_Close_Win Class -
class RHTMLSaveCloseWin : public BWindow, public BInvoker
{
 public:
								RHTMLSaveCloseWin(BRect fRect,RHTMLWin *parent,int num);
				virtual void	MessageReceived(BMessage *msg);
 private:
				RHTMLWin		*fParent;
				BView			*fView;
				BButton			*fSave;
				BButton			*fCancel;
				BButton			*fClose;
				int				fNum;
};

#endif

// - End - Save Window - Header File ------------------------------------------------------------------------
