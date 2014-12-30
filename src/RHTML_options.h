//////////////////////////////////////////////////////////////////////
//      Globe Webeditor © Gergely Rózsahegyi (RoGer) 2001-2003      //
//                                                                  //
//                         Options Header File                      //
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

#ifndef _RHTMLOPTIONS_H_
#define _RHTMLOPTIONS_H_

// - Includes
#include "ToolView.h"
#include <String.h>
#include <View.h>
#include <stdlib.h>
#include <iostream>

class RHTMLOptions;

extern RHTMLOptions *fOptions;

BString rgetcomment(BString str);

const int B_UTF8_CONVERSION=123456;

struct rfont
	   {
        BString		Family;
        BString		Style;
        float		Size;
       };
       
struct filetype
	   {
        BString		*Name;
        BString		**Kit;
        BString		**FCName;
        struct rfont **Font;
        rgb_color	**Color;
        int 		KitDb;
        int 		FontColorDb;
       };

struct locale_string
	   {
        BString		Name;
        BString		String;
        BString		Locale;
       };

struct locale
	   {
        BString		Lang;
        BString		Author;
        BString		Email;
        struct locale_string **LString;
        uint 		LSDb;
       };

// ------------------------------------------------------------------------------------- RHTML_Options Class -
class RHTMLOptions
{
 public:
								RHTMLOptions();
 			const char *		GetName();
 			const char *		GetVer();
 			const char *		GetDate();
 			const char *		GetLocaleString(const char *name,const char *string);
 			int					AddFTKit(int filetype,const char *kit);
 			void				RemoveFTKit(int filetype,int num);
 			void				EditFTKit(int filetype,int num,const char *kit);
 			int					FindFT(const char *filetype);
 			int					FindFTFC(int filetype,const char *fontcolor);
 			void				Load();
 			void				Save();
			BString				*Name;
			BString				*Ver;
			BString				*Date;
			struct filetype		**FileTypes;
			struct locale		**Locale;
			int					CurrentLocale;
			uint				LocaleDb;
			uint				FileTypesDb;
			BFont				*DefaultFont;
			bool				CRLF_LF;
			bool				LF_CRLF;
			int					Encoding;
			bool				LeftView;
			orient				LeftViewOrient;
			float				LeftViewWidth;
			bool				TopView;
			orient				TopViewOrient;
			orient				BottomViewOrient;
			uint32				TopViewIndex;
			uint32				BottomViewIndex;
			bool				NumView;
			bool				WordWrap;
			bool				IgnoreCase;
			BString				DefaultFolder;
			bool				DFOnlyStart;
			BString				SettingsDir;
			uint32				ReakcioIdo;
			uint32				ReakcioIdoTM;
			float				WinFrameLeft;
			float				WinFrameTop;
			float				WinFrameRight;
			float				WinFrameBottom;

 private:
 			uint				LoadLocale(const char *language);
 			uint				AddLocaleString(uint num,const char *name,const char *string,const char *locale);
 			uint				AddLocale(const char *lang,const char *author,const char *email);

};

#endif

// - End - Options - Header File -----------------------------------------------------------------------------
