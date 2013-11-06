//////////////////////////////////////////////////////////////////////
//      Globe Webeditor © Gergely Rózsahegyi (RoGer) 2001-2002      //
//                                                                  //
//                       File Types  Header File                    //
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

#ifndef _RHTMLFILETYPES_H_
#define _RHTMLFILETYPES_H_

// - Includes
#include <TextView.h>
#include <Font.h>
#include <String.h>
#include <string.h>
#include "RHTML_options.h"

// ----------------------------------------------------------------------------------- RHTML_FileTypes Class -
class RHTMLFileTypes
{
 public:
								RHTMLFileTypes(uint ft,BTextView *textview);
				void			SetFontAndColor(BString *oldtext,BString *currenttext,int mode=0,int offset1=-1,int offset2=-1);
				uint			GetFileType();
				void			SetFileType(uint FT);
 private:
								// - HTML
				void			SetHTMLFC(BString *oldtext,BString *currenttext,int offset1,int offset2,int mode);
				void			SetHTMLTAGFC(int x,int y,BString *currenttext);
				void			FindHTMLTAGs(int x,int y,BString *currenttext);
				void			FindHTMLTAG(int x,BString *currenttext);
								// - PHP
				void			SetPHPFC(BString *oldtext,BString *currenttext,int offset1,int offset2,int mode);
				void			SetPHPHTMLTAGFC(int x,int y,BString *currenttext);
				void			FindPHPHTMLTAGs(int x,int y,BString *currenttext);
				void			FindPHPTAGs(int x,int y,BString *currenttext);
				int				FindPHPBBegin(int x,BString *currenttext);
				int				FindPHPBEnd(int x,BString *currenttext);
				int				FindPHPFBegin(int x,BString *currenttext);
				int				FindPHPFEnd(int x,BString *currenttext);
				int				FindPHPFEnd_old(int x,BString *currenttext);
				void			FindPHPBE(BString *currenttext);
								
								// -
				void			PutColor(int32 x,int32 y,int fontcolor);
				uint			FileType;
				BTextView		*fTextView;
				int				begin[10000];
				int				end[10000];
				int				begindb;
				int				enddb;
};

#endif

// - End - File Types - Header File --------------------------------------------------------------------------
