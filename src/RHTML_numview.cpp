//////////////////////////////////////////////////////////////////////
//      Globe Webeditor © Gergely Rózsahegyi (RoGer) 2001-2002      //
//                                                                  //
//                        Num View  Source File                     //
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

// - Includes
#include <stdio.h>
#include "RHTML_numview.h"
#include "RHTML_options.h"

// --------------------------------------------------------------------------- RHTML_Num_View - RHTMLNumView -
RHTMLNumView::RHTMLNumView(BRect rect,BTextView *textview): BView(rect,"fmenu",B_FOLLOW_TOP_BOTTOM,B_FRAME_EVENTS)
{ 
 oldnewline=0;
 fTextView = textview;
 SetViewColor(235,235,235);
  rgb_color black={0,0,0,0};

 fNumView= new BTextView(BRect(2,0,34,50000),"fTextView",BRect(1,4,30,Bounds().bottom-1),fOptions->DefaultFont,&black,B_FOLLOW_TOP_BOTTOM,B_WILL_DRAW);
 fNumView->SetText("1");
 AddChild(fNumView);
 fNumView->SetViewColor(235,235,235);
 
 fNumView->SetFont(be_plain_font);
 fNumView->SetFontSize(fOptions->DefaultFont->Size());

 fNumView->MakeFocus(false);
 fNumView->SetDoesUndo(false);
 fNumView->SetStylable(true);
 fNumView->MakeSelectable(false);
 fNumView->MakeEditable(false);
}
// - End - RHTML_Num_View - RHTMLNumView ---------------------------------------------------------------------

// ------------------------------------------------------------------------------ RHTML_Num_View - UpdateNum -
int32 RHTMLNumView::UpdateNum(int32 num)
{
 int32 tmpnum=1; 
 if (!fTextView->DoesWordWrap())
 {
  for (int i=0;i<fTextView->CountLines();i++)
  {
   if (fTextView->LineWidth(i)>fTextView->TextRect().right-fTextView->TextRect().left)
   {
    fTextView->SetTextRect(BRect(fTextView->TextRect().left,fTextView->TextRect().top,fTextView->LineWidth(i)+100,fTextView->TextRect().bottom));
    fTextView->ScrollToSelection();
   }
  }
 }
 
 int32 i=0;
 int32 a=0;
 BString tmp,tmp1;
 tmp1.SetTo(fTextView->Text());
 if (fTextView->CountLines()!=oldnewline || num!=-1)
 {
  oldnewline=fTextView->CountLines();
  fNumView->SetText("");
  int32 b=-1;
  i=1;
  a=0;
  fNumView->Insert("1",strlen("1")); 

  while ((a=tmp1.FindFirst("\n",b+1))>-1)
  {
   tmp.SetTo("");
   if (b==-1)
   {
    if (0<fTextView->LineAt(a))
    {
     for (int o=0;o<(fTextView->LineAt(a));o++) tmp << "\n";
    }
   } else
   {
    if (fTextView->LineAt(b)+1<fTextView->LineAt(a)) for (int o=0;o<(fTextView->LineAt(a)-fTextView->LineAt(b)-1);o++) tmp << "\n";
   }
   tmp << "\n" <<i+1;
   fNumView->Insert(tmp.String(),tmp.Length());
   if (i==num)
    tmpnum=fNumView->CountLines();
   b=a;
   i++;
  }
 }
 return tmpnum;
}
// - End - RHTML_Num_View - UpdateNum ------------------------------------------------------------------------

// - End - Num View - Source File ----------------------------------------------------------------------------
