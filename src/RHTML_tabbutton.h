//////////////////////////////////////////////////////////////////////
//      Globe Webeditor © Gergely Rózsahegyi (RoGer) 2001-2002      //
//                                                                  //
//                       Tab Button  Header File                    //
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

#ifndef _RHTMLTABBUTTON_H_
#define _RHTMLTABBUTTON_H_

// - Includes
#include <View.h>
#include <StringView.h>
#include <PopUpMenu.h>
#include "TToolTip.h"
#include "RHTML_amenu.h"
#include "RHTML_options.h"

class RHTMLtabbutton;
class RHTMLamenu;

// ------------------------------------------------------------------------- RHTML_tab_button_str_view Class -
class RHTMLtabbuttonstrview : public BStringView
{
 public:
								RHTMLtabbuttonstrview(BRect fRect,const char *name,RHTMLtabbutton *target);
				virtual void	MouseMoved(BPoint point, uint32 transit,const BMessage *message);
				void			MouseDown(BPoint where);
						
				RHTMLtabbutton	*fTarget;
 private:				
				bool			fIn;

};

// ---------------------------------------------------------------------------------- RHTML_tab_button Class -
class RHTMLtabbutton : public BView
{
 public:
								RHTMLtabbutton(BRect fRect,const char *name,uint8 num,RHTMLamenu *target);
				virtual void	MessageReceived(BMessage *msg);
				void			SetON();
				void			SetOFF();
				void			MouseDown(BPoint where);
				void			BalGomb();
				void			JobbGomb(BPoint where);
				void			SetNum(uint8 num);
				void			SetText(const char *text);
				const char		*GetText();
				RHTMLamenu		*fTarget;
				uint8			fNum;

 private:
				char 			*name;
				BView			*j1view;
				BView			*j2view;
				RHTMLtabbuttonstrview *fstrview;
				BPopUpMenu		*fPopUpMenu;
			
};

#endif

// - End - Tab Button - Header File --------------------------------------------------------------------------
