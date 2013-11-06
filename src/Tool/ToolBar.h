//////////////////////////////////////////////////////////////////////
//       Extended BeOS Api © Gergely Rózsahegyi (RoGer) 2002        //
//                          roger@beos.hu                           //
//                                                                  //
//                      BToolBar  Header File                       //
//                                                                  //
//                     Last Update: 2002.06.11                      //
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

#ifndef _BTOOLBAR_H_
#define _BTOOLBAR_H_

// - Includes
#include <View.h>
#include "ToolView.h"
#include "ToolItem.h"

enum borderstyle
{
 B_NORMAL,
 B_NONE,
 B_SUB
};

class BToolItem;

// ------------------------------------------------------------------------------------------ BToolBar Class -
class BToolBar : public BView
{
 public:
								BToolBar(const char *name, BRect hmargin = BRect(10,5,5,5), BRect vmargin = BRect(5,10,5,5), float hspace = 5, float vspace = 5, bool autoresize = true, float hwidth = 50, float vheight = 30);
				virtual	void	Draw(BRect updateRect);
				virtual	void	MouseDown(BPoint where);
				virtual void	SetEnabledOrientation(bool top, bool bottom, bool left, bool right);
				bool			EnabledOrientation(orient ori);
				orient			Orientation();
				virtual void	SetOrientation(orient ori);
				bool			AutoResize();
				void			SetAutoResize(bool autoresize);
				float			HorizontalHeight();
				void			SetHorizontalHeight(float height);
				float			VerticalWidth();
				void			SetVerticalWidth(float width);
				bool			IsButton(BPoint point);
				uint32			AddToolItem(BToolItem *toolitem);
				void			RemoveToolItem(BToolItem *toolitem);
				void			SetHorizontalMargin(BRect margin);
				BRect			HorizontalMargin();
				void			HideAll();
				void			ShowAll();
				void			SetBorderStyle(borderstyle bst);
				void			SetMoveable(bool moveable);
				float			GetLast(); 				
				void			SetAlignCenter(bool acenter);
				BToolItem		*ToolItem(uint32 num);
				BToolItem		*FindToolItem(const char *name);
				uint32			ToolItemCount();
				virtual void	SetIndex(uint32 index);
				virtual void	MessageReceived(BMessage *msg);

 private:
 				BToolItem		**fToolItem;
 				uint32			TIDb;
				float			GetLeft();
				float			GetTop();
				void			Update();
				orient			Ori;
				float			Width;
				float			Height;
				bool			Autoresize;
				bool			Top;
				bool			Bottom;
				bool			Left;
				bool			Right;
 				BRect			HMargin;
 				BRect			VMargin;
 				float			HSpace;
 				float			VSpace;
 				borderstyle		Borderstyle;
 				bool			Moveable;
 				bool			ACenter;
};

#endif

// - End - BToolBar - Header File ----------------------------------------------------------------------------
