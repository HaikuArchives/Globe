//////////////////////////////////////////////////////////////////////
//      Globe Webeditor © Gergely Rózsahegyi (RoGer) 2001-2002      //
//                                                                  //
//                        Num View  Header File                     //
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

#ifndef _RHTMLNUMVIEW_H_
#define _RHTMLNUMVIEW_H_

// - Includes
#include <View.h>
#include <StringView.h>
#include <TextView.h>
#include "RHTML_options.h"

// ------------------------------------------------------------------------------------ RHTML_Num_View Class -
class RHTMLNumView : public BView
{
 public:
								RHTMLNumView(BRect rect,BTextView* textview);
				int32			UpdateNum(int32 num=-1);

 private:
				BStringView		**fStrView;
				BTextView		*fTextView;
				BTextView		*fNumView;
				int32			oldnewline;
};

#endif

// - End - Num View - Header File ----------------------------------------------------------------------------
