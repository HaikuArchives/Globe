//////////////////////////////////////////////////////////////////////
//      Globe Webeditor © Gergely Rózsahegyi (RoGer) 2001-2003      //
//                                                                  //
//                        ColorWin  Header File                     //
//                                                                  //
//                       Last Update: 2004.10.21                    //
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

#ifndef _RHTMLCOLORWIN_H_
#define _RHTMLCOLORWIN_H_

// - Includes
#include "RHTML_options.h"
#include "RHTML_parammenu.h"
#include <Window.h>
#include <Box.h>
#include <ColorControl.h>
#include <ListView.h>
#include <Button.h>
#include <TextControl.h>
#include <TextView.h>
#include <vector>

int HexaToInt(char a, char f);
BString IntToHexa(int i);
rgb_color BStringToRGB(BString str);
BString RGBToBString(rgb_color color);

struct colortype
{
 BString name;
 rgb_color color;
};

#define htmlcolorcount 16
extern struct colortype htmlcolors[htmlcolorcount];

class RHTMLcolorwin;
class RHTMLcolorview;
class RHTMLplistitem;
class RHTMLcolortext;

// ------------------------------------------------------------------------------------ RHTML_colorwin Class -
class RHTMLcolorwin : public BWindow
{
 public:
								RHTMLcolorwin();
				virtual			~RHTMLcolorwin();
				void			Start(BPoint pos, RHTMLplistitem *item, BString value = "");
				void			Start(BPoint pos, BTextView *text);
				void			Stop();
				virtual void	MessageReceived(BMessage *msg);
 private:
 				RHTMLcolorview	*fBox;
 				BView			*fColorView;
 				BColorControl	*fColorControl;
 				BListView		*fListView;
 				BTextControl	*fTextControl;
 				RHTMLplistitem	*fItem;
 				BString			fValue;
 				BTextView		*fTextView;
 				BButton			*fCancelButton;
 				BButton			*fOkButton;
};

// ----------------------------------------------------------------------------------- RHTML_colorview Class -
class RHTMLcolorview : public BBox
{
 public:
								RHTMLcolorview(BRect rect);
				virtual	void	MouseDown(BPoint where);
 private:
};

// ----------------------------------------------------------------------------------------------- HexaToInt -
inline int HexaToInt(char a, char f)
{
 int i=0;
 if (a >= 'a' && a <= 'f')
  i += (a - 'a' + 10) * 16;
 if (a >= '0' && a <= '9')
  i += (a - '0') * 16;
 if (f >= 'a' && f <= 'f')
  i += f - 'a' + 10;
 if (f >= '0' && f <= '9')
  i += f - '0';
 return i;
}

// ----------------------------------------------------------------------------------------------- HexaToInt -
inline BString IntToHexa(int i)
{
 BString tmp;
 if (i / 16 >= 10)
  tmp << (char)('A' + (i / 16) - 10);
 else
  tmp << (char)('0' + i / 16);
 if (i % 16 >= 10)
  tmp << (char)('A' + (i % 16) -10);
 else
  tmp << (char)('0' + i % 16);
 return tmp;
}

// -------------------------------------------------------------------------------------------- BStringToRGB -
inline rgb_color BStringToRGB(BString str)
{
 BString tmp(str);
 tmp.ToLower();
 rgb_color color={0,0,0,0};
 if (tmp.Length() == 7 && tmp[0] == '#')
 {
  color.red = HexaToInt(tmp[1],tmp[2]);
  color.green = HexaToInt(tmp[3],tmp[4]);
  color.blue = HexaToInt(tmp[5],tmp[6]);
 } else
 for (int i=0;i<htmlcolorcount;i++)
 if (htmlcolors[i].name == tmp)
  color = htmlcolors[i].color;
 return color;
}

// -------------------------------------------------------------------------------------------- RGBToBString -
inline BString RGBToBString(rgb_color color)
{
 BString tmp("#");
 tmp << IntToHexa(color.red);
 tmp << IntToHexa(color.green);
 tmp << IntToHexa(color.blue);
 return tmp;
}

#endif

// - End - ColorWin - Header File ---------------------------------------------------------------------------
