//////////////////////////////////////////////////////////////////////
//       Extended BeOS Api © Gergely Rózsahegyi (RoGer) 2002        //
//                          roger@beos.hu                           //
//                                                                  //
//                      ToolItem  Header File                       //
//                                                                  //
//                     Last Update: 2002.07.10                      //
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

#ifndef _BTOOLITEM_H_
#define _BTOOLITEM_H_

// - Includes
#include <View.h>
#include <String.h>
#include <Bitmap.h>
#include "ToolView.h"

// ----------------------------------------------------------------------------------------- BToolItem Class -
class BToolItem : public BView
{
 public:
								BToolItem(const char *name, BMessage *msg);
				virtual	void	Draw(BRect updateRect);
				virtual	void	MouseDown(BPoint where);
				virtual	void	MouseUp(BPoint where);
				virtual	void	MouseMoved(BPoint where, uint32 code, const BMessage *a_message);
				bool			Enabled();
				virtual void	SetEnabled(bool enabled);
				virtual void	MessageReceived(BMessage *msg);
 				bool			isDown;
 private:
 				int				Status;
 				BMessage		*fMessage;
 				bool			IsEnabled;
};

// ---------------------------------------------------------------------------------------- BImageItem Class -
class BImageItem : public BToolItem
{
 public:
								BImageItem(const char *name,const char *filename, BMessage *msg, bool enablecaption=false, const char *caption="", alignment halign = B_ALIGN_LEFT);
				BBitmap			*Image() const;
				void			SetImage(const char *filename);
				void			SetImage(BBitmap *bitmap);
				const char		*Caption() const;
				void			SetCaption(const char *caption);
				void			Update();
				virtual	void	Draw(BRect updateRect);
				void			SetEnableBorder(bool enableborder);
				virtual void	MessageReceived(BMessage *msg);
 private:
 				BBitmap			*fBitmap;
 				BString			*fCaption;
 				bool			Enablecaption;
				alignment		HAlign;
				bool			Enableborder;
};

// ---------------------------------------------------------------------------------------- BRadioItem Class -
class BRadioItem : public BToolItem
{
 public:
								BRadioItem(const char *name,const char *filename, BMessage *msg);
				BBitmap			*Image() const;
				void			SetImage(const char *filename);
				virtual	void	MouseUp(BPoint where);
				void			Update();
				void			SetChecked(bool checked);
				void			SetFirstItem(bool firstitem);
				void			SetNum(uint32 fnum);
				virtual	void	Draw(BRect updateRect);
				virtual void	MessageReceived(BMessage *msg);
 private:
 				BBitmap			*fBitmap;
				bool			Checked;
				bool			Firstitem;
				uint32			fNum;
				BMessage		*fMessage;
};

// ---------------------------------------------------------------------------------------- BCheckItem Class -
class BCheckItem : public BImageItem
{
 public:
								BCheckItem(const char *name,const char *filename,const char *filename2, BMessage *msg, bool enablecaption=false, const char *caption="", alignment halign = B_ALIGN_LEFT);
				virtual void	SetEnabled(bool enabled);
 private:
 				BBitmap			*fEnableBitmap;
 				BBitmap			*fDisableBitmap;
};


#endif

// - End - BToolItem - Header File ---------------------------------------------------------------------------
