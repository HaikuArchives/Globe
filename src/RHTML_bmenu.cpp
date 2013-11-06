//////////////////////////////////////////////////////////////////////
//      Globe Webeditor © Gergely Rózsahegyi (RoGer) 2001-2003      //
//                                                                  //
//                          BMenu Source File                       //
//                                                                  //
//                       Last Update: 2003.04.23					//
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
#include "RHTML_bmenu.h"
#include "RadioBar.h"
#include <Button.h>
#include <StringView.h>
#include <stdio.h>

// -------------------------------------------------------------------------------- RHTML_bmenu - RHTMLbmenu -
RHTMLbmenu::RHTMLbmenu(BRect fRect, RHTMLWin *fwin): BToolBar("name")
{
// SetBorderStyle(B_NONE);
 SetVerticalWidth(fOptions->LeftViewWidth);
// cout << fOptions->LeftViewWidth << endl;
 fIttNyomta = false; 
 fIrany = 0;
 fRadioBar = new BRadioBar("RadioBar",BRect(0,0,0,0),BRect(0,0,0,0),0,0,false,Bounds().right-2,28);
 fRadioBar->SetMoveable(false);
 fRadioBar->SetBorderStyle(B_SUB);
 AddChild(fRadioBar);
 fRadioBar->MoveTo(1,10);

 fView = new BView(BRect(3, fRadioBar->Frame().bottom-1,Bounds().right - 3,Bounds().bottom-1), "_bmenuview_", B_FOLLOW_ALL, B_FRAME_EVENTS|B_WILL_DRAW);
 AddChild(fView);
 fView->SetViewColor(216,216,216);

 // - Search
 BString dir = fOptions->SettingsDir;
 dir << "menu/pic/kereso.tga";
 fSearchItem = new BRadioItem("name",dir.String(),new BMessage('Tab1'));
 fRadioBar->AddToolItem(fSearchItem);
 
 fSearchView= new RHTMLsearch(fView->Bounds(),fwin);
 fView->AddChild(fSearchView);

 // - ParamMenu
 dir = fOptions->SettingsDir;
// dir <<  "menu/pic/proba.gif";
 dir <<  "menu/pic/tag.tga";

 fParamMenuItem = new BRadioItem("name",dir.String(),new BMessage('Tab2'));
 fRadioBar->AddToolItem(fParamMenuItem);
 fParamMenuItem->SetChecked(false);
  
 fParamMenuView= new RHTMLparammenu(fView->Bounds(),fwin);
 fView->AddChild(fParamMenuView);

 // - Tags
// fTagItem = new BRadioItem("name",dir.String(),new BMessage('Tab3'));
// fRadioBar->AddToolItem(fTagItem);
// fSearchItem->SetChecked(false);
//
// fTagView= new BView(fView->Bounds(),"Tab",B_FOLLOW_ALL,B_WILL_DRAW);
// fView->AddChild(fTagView);
// fTagView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

 // - 
 MessageReceived(new BMessage('Tab1'));
 dir = fOptions->SettingsDir;
 dir << "menu/pic/toolclose.tga";
 fImageItem = new BImageItem("Close",dir.String(),new BMessage('LB'));
 fRadioBar->AddChild(fImageItem);
 fImageItem->MoveTo(Bounds().right - 25,2);
 fImageItem->SetResizingMode(B_FOLLOW_RIGHT | B_FOLLOW_TOP);
 fTimer = new RTimer("BMenuTimer", this, new BMessage('TIMR'));
}

void RHTMLbmenu::FrameResized(float width, float height)
{
}


// ---------------------------------------------------------------------------- RHTML_bmenu - SetOrientation -
void RHTMLbmenu::SetOrientation(orient ori)
{
 fOptions->LeftViewOrient=ori;
 fOptions->Save();
 BToolBar::SetOrientation(ori);
}

// ------------------------------------------------------------------------- RHTML_bmenu - SetVerticalWidth -
void RHTMLbmenu::SetVerticalWidth(float width)
{
 BToolBar::SetVerticalWidth(width);
}

// -------------------------------------------------------------------------------- RHTML_bmenu - RHTMLbmenu -
void RHTMLbmenu::MessageReceived(BMessage *msg)
{
 switch (msg->what)
  {
   case 'Tab1':
   {
    fSearchItem->SetChecked(true);
    fSearchView->SetResizingMode(B_FOLLOW_ALL);
    fSearchView->ResizeTo(fView->Bounds().right,fView->Bounds().bottom);

    fParamMenuItem->SetChecked(false);
    fParamMenuView->SetResizingMode(B_FOLLOW_NONE);
//    fParamMenuView->fParamList->fScrollView->SetResizingMode(B_FOLLOW_NONE);
    fParamMenuView->ResizeTo(0,0);

//    fTagItem->SetChecked(false);
//    fTagView->SetResizingMode(B_FOLLOW_NONE);
//    fTagView->ResizeTo(0,0);
   }
   break;
   case 'Tab2':
   {
    fSearchItem->SetChecked(false);
    fSearchView->SetResizingMode(B_FOLLOW_NONE);
    fSearchView->ResizeTo(0,0);

    fParamMenuItem->SetChecked(true);
    fParamMenuView->SetResizingMode(B_FOLLOW_ALL);

//    fParamMenuView->fParamList->fScrollView->SetResizingMode(B_FOLLOW_ALL);

    fParamMenuView->ResizeTo(fView->Bounds().right,fView->Bounds().bottom);
    fParamMenuView->fParamList->fScrollView->ResizeTo(fView->Bounds().right-0,fView->Bounds().bottom - 23);

/*    if ((fParamMenuView->fParamList->CountItems()>0) && (fParamMenuView->fParamList->CountItems()*(1+(int32)fParamMenuView->fParamList->FirstItem()->Height())+fParamMenuView->fParamList->Frame().top-fParamMenuView->fParamList->Frame().bottom>0))
     fParamMenuView->fParamList->fScrollView->ScrollBar(B_VERTICAL)->SetRange(0,fParamMenuView->fParamList->CountItems()*(ceil(fParamMenuView->fParamList->FirstItem()->Height()))+fParamMenuView->fParamList->Frame().top-fParamMenuView->fParamList->Frame().bottom+2);
    else
     fParamMenuView->fParamList->fScrollView->ScrollBar(B_VERTICAL)->SetRange(0,0);
     
*/
//    fTagItem->SetChecked(false);
//    fTagView->SetResizingMode(B_FOLLOW_NONE);
//    fTagView->ResizeTo(0,0);
   }
   break;
/*   case 'Tab3':
   {
    fSearchItem->SetChecked(false);
    fSearchView->SetResizingMode(B_FOLLOW_NONE);
    fSearchView->ResizeTo(0,0);

    fParamMenuItem->SetChecked(false);
    fParamMenuView->SetResizingMode(B_FOLLOW_NONE);
    fParamMenuView->fParamList->fScrollView->SetResizingMode(B_FOLLOW_NONE);
    fParamMenuView->ResizeTo(0,0);

    fTagItem->SetChecked(true);
    fTagView->SetResizingMode(B_FOLLOW_ALL);
    fTagView->ResizeTo(fView->Bounds().right,fView->Bounds().bottom);
   }
   break;
*/   case 'TIMR':
   {
    Window()->Lock();
    fParamMenuView->fParamList->Draw(fParamMenuView->fParamList->Bounds());
    fOptions->LeftViewWidth = VerticalWidth();
    fOptions->Save();
    Window()->Unlock();
    fTimer->EndTimer();
   }
   break;
   default:
   {
    BToolBar::MessageReceived(msg);
   }
   break;
  }
}

// ------------------------------------------------------------------------------ RHTML_search - RHTMLsearch -
RHTMLsearch::RHTMLsearch(BRect fRect,RHTMLWin *fwin): BView(fRect,"Search",B_FOLLOW_ALL,B_WILL_DRAW),fWin(fwin)
{
 SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

 fSearch = new RHTMLtextcontrol(BRect(10,10,Bounds().right - 10,30),"SearchText",fOptions->GetLocaleString("Globe_Window_Search","Find"),"",new BMessage('SERC'), B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP);
 AddChild(fSearch);
 fSearch->SetDivider(50);

 fReplace = new RHTMLtextcontrol(BRect(10,40,Bounds().right - 10,60),"ReplaceText",fOptions->GetLocaleString("Globe_Window_Search","Replace"),"",new BMessage('SERC'), B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP);
 AddChild(fReplace);
 fReplace->SetDivider(50);

 fCaseCheck = new BCheckBox(BRect(Bounds().right /2 - 50,70,Bounds().right /2 + 51,90),"CaseCheck",fOptions->GetLocaleString("Globe_Window_Search","Ignore Case"),new BMessage('SCCK'), B_FOLLOW_H_CENTER | B_FOLLOW_TOP);
 AddChild(fCaseCheck);

 BButton *fSearchButton = new BButton(BRect(Bounds().right /2 - 55,100,Bounds().right /2 + 55,120),"SearchButton",fOptions->GetLocaleString("Globe_Window_Search_Button","Find"),new BMessage('SERC'), B_FOLLOW_H_CENTER | B_FOLLOW_TOP);
 AddChild(fSearchButton);
 fCaseCheck->SetValue(fOptions->IgnoreCase);

 BButton *fReplaceButton = new BButton(BRect(Bounds().right /2 - 55,135,Bounds().right /2 + 55,155),"ReplaceButton",fOptions->GetLocaleString("Globe_Window_Search_Button","Replace"),new BMessage('REPL'), B_FOLLOW_H_CENTER | B_FOLLOW_TOP);
 AddChild(fReplaceButton);

 BButton *fReplaceAndFindButton = new BButton(BRect(Bounds().right /2 - 55,170,Bounds().right /2 + 55,190),"ReplaceAndFindButton",fOptions->GetLocaleString("Globe_Window_Search_Button","Replace & Find"),new BMessage('REAF'), B_FOLLOW_H_CENTER | B_FOLLOW_TOP);
 AddChild(fReplaceAndFindButton);

 BButton *fReplaceAllButton = new BButton(BRect(Bounds().right /2 - 55,205,Bounds().right /2 + 55,225),"ReplaceAllButton",fOptions->GetLocaleString("Globe_Window_Search_Button","Replace All"),new BMessage('REAL'), B_FOLLOW_H_CENTER | B_FOLLOW_TOP);
 AddChild(fReplaceAllButton);

 fLine = new RHTMLtextcontrol(BRect(10,245,Bounds().right - 10,265),"GoToLine",fOptions->GetLocaleString("Globe_Window_Search","Go To Line"),"",new BMessage('GOTO'), B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP);
 AddChild(fLine);
 fLine->SetDivider(65);
}

// -------------------------------------------------------------------------- RHTML_search - MessageReceived -
void RHTMLsearch::MessageReceived(BMessage *msg)
{
 switch (msg->what)
 {
  case 'TABB':
  {
   void *ptr;
   msg->FindPointer("where",&ptr);
   if (ptr==fSearch)
    fReplace->MakeFocus();
   if (ptr==fReplace)
    fCaseCheck->MakeFocus();
   if (ptr==fLine)
    fSearch->MakeFocus();
  }
  break;
  default:
  {
   BView::MessageReceived(msg);
  }
  break;
 }
}

// ------------------------------------------------------------------- RHTML_text_control - RHTMLtextcontrol -
RHTMLtextcontrol::RHTMLtextcontrol(BRect fRect,const char *name,const char *label, const char *text, BMessage *msg, uint32 rmask, uint32 flags):BView(fRect,name,rmask,flags)
{
 divider=Bounds().right/2;
 fTextView = new RHTMLsearchtext(BRect(divider+2,2,Bounds().right-2,Bounds().bottom-2),"",msg, rmask);
 AddChild(fTextView);
 AddChild(fStringView = new BStringView(BRect(0,0,divider-1,Bounds().bottom),name,label));
 SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
}

// ------------------------------------------------------------------------------- RHTML_text_control - Draw -
void RHTMLtextcontrol::Draw(BRect inRect)
{
 SetHighColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR),B_DARKEN_1_TINT));
 StrokeLine(BPoint(divider+1,0),BPoint(Bounds().right,0));
 StrokeLine(BPoint(divider+0,0),BPoint(divider+0,Bounds().bottom));

 SetHighColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR),B_DARKEN_4_TINT));
 StrokeLine(BPoint(divider+2,1),BPoint(Bounds().right-1,1));
 StrokeLine(BPoint(divider+1,1),BPoint(divider+1,Bounds().bottom-1));

 SetHighColor(255,255,255);
 StrokeLine(BPoint(divider+1,Bounds().bottom),BPoint(Bounds().right,Bounds().bottom));
 StrokeLine(BPoint(Bounds().right,1),BPoint(Bounds().right,Bounds().bottom-1));

 SetHighColor(ui_color(B_PANEL_BACKGROUND_COLOR));
 StrokeLine(BPoint(divider+2,Bounds().bottom-1),BPoint(Bounds().right-1,Bounds().bottom-1));
 StrokeLine(BPoint(Bounds().right-1,2),BPoint(Bounds().right-1,Bounds().bottom-1));

 if (fTextView->IsFocus())
 {
  SetHighColor(0,0,229);
  StrokeRect(BRect(divider+1,1,Bounds().right-1,Bounds().bottom-1));
 }
}

// -------------------------------------------------------------------------- RHTML_text_control - MakeFocus -
void RHTMLtextcontrol::MakeFocus(bool focusState)
{
 fTextView->MakeFocus(focusState);
}

// ------------------------------------------------------------------------- RHTML_text_control - SetDivider -
void RHTMLtextcontrol::SetDivider(float dividing_line)
{
 divider=dividing_line;
 fTextView->MoveTo(divider+2,fTextView->Frame().top);
 fTextView->ResizeTo(Bounds().right-divider-4,fTextView->Bounds().bottom);
 fStringView->ResizeTo(divider-1,fStringView->Bounds().bottom);
 Draw(Bounds());
}

// ------------------------------------------------------------------------------- RHTML_text_control - Text -
const char* RHTMLtextcontrol::Text() const
{
 return fTextView->Text();
}

BTextView* RHTMLtextcontrol::TextView() const
{
 return fTextView;
}

// --------------------------------------------------------------------- RHTML_search_text - RHTMLsearchtext -
RHTMLsearchtext::RHTMLsearchtext(BRect fRect,const char *name, BMessage *msg, uint32 resizeMask, uint32	flags):BTextView(fRect,name,BRect(1,1,1000,fRect.bottom-fRect.top-1), resizeMask, flags)
{
 SetWordWrap(true);
 fMessage=msg;
}

// ----------------------------------------------------------------------------- RHTML_search_text - KeyDown -
void RHTMLsearchtext::KeyDown(const char *bytes, int32 numBytes)
{
 switch(bytes[0])
 {
  case B_ENTER:			// - ENTER
  {
   MessageReceived(fMessage);
  }
  break;
  case B_TAB:			// - TAB
  {
   BMessage *msg = new BMessage('TABB');
   msg->AddPointer("where",Parent());
   MessageReceived(msg);
  }
  break;
  default:
  {
   BTextView::KeyDown(bytes,numBytes);
  }
 }
}

// --------------------------------------------------------------------------- RHTML_search_text - MakeFocus -
void RHTMLsearchtext::MakeFocus(bool focusState)
{
 BTextView::MakeFocus(focusState);
 SelectAll();
 if (Parent())
  Parent()->Draw(Parent()->Bounds());
}

// ---------------------------------------------------------------------------- RHTML_bmenu_view - MouseDown -
void RHTMLbmenu::MouseDown(BPoint where)
{
 uint32	buttons;
 this->Window()->CurrentMessage()->FindInt32("buttons", (int32*)&buttons);
 if (buttons & B_PRIMARY_MOUSE_BUTTON && ((Orientation() == B_LEFT && where.x > Bounds().right - 10)||(Orientation() == B_RIGHT && where.x < 10)))
 {
  fMousePos = where;
  fIttNyomta = true;
  fIrany = 0;
  SetEventMask(B_POINTER_EVENTS);
 }
 BToolBar::MouseDown(where);
}

// ------------------------------------------------------------------------------ RHTML_bmenu_view - MouseUp -
void RHTMLbmenu::MouseUp(BPoint where)
{
 SetEventMask(B_NO_POINTER_HISTORY);
 fIttNyomta = false;
 BToolBar::MouseUp(where);
}

// --------------------------------------------------------------------------- RHTML_bmenu_view - MouseMoved -
void RHTMLbmenu::MouseMoved(BPoint where, uint32 code, const BMessage *a_message)
{
 uint32	buttons;
 this->Window()->CurrentMessage()->FindInt32("buttons", (int32*)&buttons);

 if (buttons & B_PRIMARY_MOUSE_BUTTON && fIttNyomta)
 {
  if (Orientation() == B_LEFT)
  {
   SetVerticalWidth(Bounds().right - (fMousePos.x - where.x));
   if (fTimer->IsActive())
    fTimer->ResetTimer();
   else
    fTimer->StartTimer(fOptions->ReakcioIdo);
   fMousePos = where;
  }
  if (Orientation() == B_RIGHT)
  {

   if (fIrany == 0)
   {
    if (fMousePos.x < where.x)
    {
     fIrany = 1;
    } else
    {
     fIrany = 2;
    } 
   }
   
   if ((fMousePos.x < where.x) && (fIrany == 1))
   {
    SetVerticalWidth(Bounds().right + (fMousePos.x - where.x));
   }
   if ((fMousePos.x > where.x) && (fIrany == 2))
   {
    SetVerticalWidth(Bounds().right + (fMousePos.x - where.x));
   }

  }
 }

 char cursordata[] = {
		0x10,0x01,0x07,0x07,
		0x02,0x80,
		0x02,0x80,
		0x02,0x80,
		0x02,0x80,
		0x0a,0xa0,
		0x1a,0xb0,
		0x3a,0xb8,
		0x7a,0xbc,
		0x7a,0xbc,// - 
		0x3a,0xb8,
		0x1a,0xb0,
		0x0a,0xa0,
		0x02,0x80,
		0x02,0x80,
		0x02,0x80,
		0x02,0x80,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
				
 if (fIttNyomta || (Orientation() == B_LEFT && where.x > Bounds().right - 10) || (Orientation() == B_RIGHT && where.x < 10))
  SetViewCursor(new BCursor((void *)cursordata));
//  be_app->SetCursor((void *)cursordata);
 else
  SetViewCursor(B_CURSOR_SYSTEM_DEFAULT);
//  be_app->SetCursor(B_CURSOR_SYSTEM_DEFAULT);
			
  
 BToolBar::MouseMoved(where, code, a_message);
}


// - End - BMenu - Source File -------------------------------------------------------------------------------

