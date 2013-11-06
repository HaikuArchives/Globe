//////////////////////////////////////////////////////////////////////
//      Globe Webeditor © Gergely Rózsahegyi (RoGer) 2001-2003      //
//                                                                  //
//                       Tab Button  Source File                    //
//                                                                  //
//                       Last Update: 2003.04.29					//
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
#include <Application.h>
#include <MenuBar.h>
#include <Menu.h>
#include <MenuItem.h>
#include "RHTML_tabbutton.h"

// ----------------------------------------------------------------------- RHTML_tab_button - RHTMLtabbutton -
RHTMLtabbutton::RHTMLtabbutton(BRect fRect,const char *name_,uint8 num,RHTMLamenu *target): BView(fRect,"tabbutton",B_FOLLOW_TOP_BOTTOM,B_WILL_DRAW)
{
 fNum=num;
 fTarget=target;
// rgb_color vszurke={239,239,239};
// rgb_color szurke={216,216,216};
// rgb_color sszurke={184,184,184};

 rgb_color vszurke=tint_color(ui_color(B_PANEL_BACKGROUND_COLOR),B_LIGHTEN_2_TINT);
 rgb_color szurke=ui_color(B_PANEL_BACKGROUND_COLOR);
 rgb_color sszurke=tint_color(ui_color(B_PANEL_BACKGROUND_COLOR),B_DARKEN_2_TINT);
  SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));


 j1view = new BView(BRect(Bounds().right-1,2,Bounds().right-1,Bounds().bottom-2),"j1view",B_FOLLOW_TOP_BOTTOM,B_WILL_DRAW);
 AddChild(j1view);
 j1view->SetViewColor(sszurke);

 j2view = new BView(BRect(Bounds().right,2,Bounds().right,Bounds().bottom-2),"j2view",B_FOLLOW_TOP_BOTTOM,B_WILL_DRAW);
 AddChild(j2view);
 j2view->SetViewColor(vszurke);

 SetViewColor(szurke);
 
 fstrview= new RHTMLtabbuttonstrview(BRect(2,1,Bounds().right-4,Bounds().bottom),name_,this);
 fstrview->SetAlignment(B_ALIGN_CENTER);
 AddChild(fstrview);
 fPopUpMenu = new BPopUpMenu("PopUpMenu");
 fPopUpMenu->AddItem(new BMenuItem(fOptions->GetLocaleString("File_List_Bar_Pop_Up_Menu","Close"),new BMessage('CLOS')));
}
// - End - RHTML_tab_button - RHTMLtabbutton -----------------------------------------------------------------

// -------------------------------------------------------------------------------- RHTML_tab_button - SetON -
void RHTMLtabbutton::SetON()
{
 fstrview->SetFont(be_bold_font);
 fstrview->Hide();
 fstrview->Show();
}
// - End - RHTML_tab_button - SetON --------------------------------------------------------------------------

// ------------------------------------------------------------------------------- RHTML_tab_button - SetOFF -
void RHTMLtabbutton::SetOFF()
{
 fstrview->SetFont(be_plain_font);
 fstrview->Hide();
 fstrview->Show();
}
// - End - RHTML_tab_button - SetOFF -------------------------------------------------------------------------

// ---------------------------------------------------------------------------- RHTML_tab_button - MouseDown -
void RHTMLtabbutton::MouseDown(BPoint where)
{
 uint32 button;
 this->Window()->CurrentMessage()->FindInt32("buttons", (int32*)&button);
 if (button==0x01) BalGomb();
 if (button==0x02)
 {
  ConvertToScreen(&where);
  JobbGomb(where);
 }
}
// - End - RHTML_tab_button - MouseDown ----------------------------------------------------------------------

// ------------------------------------------------------------------------------ RHTML_tab_button - BalGomb -
void RHTMLtabbutton::BalGomb()
{
 fTarget->Changed(fNum);
}
// - End - RHTML_tab_button - BalGomb ------------------------------------------------------------------------

// ----------------------------------------------------------------------------- RHTML_tab_button - JobbGomb -
void RHTMLtabbutton::JobbGomb(BPoint where)
{
  if (fPopUpMenu->Go(BPoint(where.x+3,where.y)) ) fTarget->Removed(fNum);
}
// - End - RHTML_tab_button - JobbGomb -----------------------------------------------------------------------

// ------------------------------------------------------------------------------- RHTML_tab_button - SetNum -
void RHTMLtabbutton::SetNum(uint8 num)
{
 fNum=num;
}
// - End - RHTML_tab_button - SetNum -------------------------------------------------------------------------

// ------------------------------------------------------------------------------ RHTML_tab_button - SetText -
void RHTMLtabbutton::SetText(const char *text)
{
 fstrview->SetText(text);
}
// - End - RHTML_tab_button - SetText ------------------------------------------------------------------------

// ------------------------------------------------------------------------------ RHTML_tab_button - GetText -
const char * RHTMLtabbutton::GetText()
{
 return fstrview->Text();
}
// - End - RHTML_tab_button - GetText ------------------------------------------------------------------------

// ---------------------------------------------------------------------- RHTML_tab_button - MessageReceived -
void RHTMLtabbutton::MessageReceived(BMessage *msg)
{
 switch (msg->what)
  {
   case 'BG':
   {BalGomb();
   }
   break;
  default:
   {
    BView::MessageReceived(msg);
   }
   break;
  }
}
// - End - RHTML_tab_button - MessageReceived ----------------------------------------------------------------

// ------------------------------------------------------- RHTML_tab_button_str_view - RHTMLtabbuttonstrview -
RHTMLtabbuttonstrview::RHTMLtabbuttonstrview(BRect fRect,const char *name,RHTMLtabbutton *target): BStringView(fRect,"strview",name,B_FOLLOW_TOP)
{
 fTarget=target;
 fIn=false;
}
// - End - RHTML_tab_button_str_view - RHTMLtabbuttonstrview -------------------------------------------------

// ------------------------------------------------------------------ RHTML_tab_button_str_view - MouseMoved -
void RHTMLtabbuttonstrview::MouseMoved(BPoint point, uint32 transit,const BMessage *message)
{
 if ((Bounds().Contains(point)) && (Window()->IsActive())) {
  if (!fIn)
  {
   BMessage	msg('STT');
   msg.AddPoint("start", ConvertToScreen(point));
   msg.AddRect("bounds", ConvertToScreen(Bounds()));
   msg.AddInt8("num",fTarget->fNum);
   MessageReceived(&msg);
   fIn = true;
  }
 } else
 if (fIn)
 {
  be_app->PostMessage(eToolTipStop);
  fIn = false;
 }
 BView::MouseMoved(point, transit, message);
}
// - End - RHTML_tab_button_str_view - MouseMoved ------------------------------------------------------------

// ------------------------------------------------------------------- RHTML_tab_button_str_view - MouseDown -
void RHTMLtabbuttonstrview::MouseDown(BPoint where)
{
 uint32 button;
 this->Window()->CurrentMessage()->FindInt32("buttons", (int32*)&button);
 if (button==0x01) fTarget->BalGomb();
 if (button==0x02)
 {
  ConvertToScreen(&where);
  fTarget->JobbGomb(where);
 }
}
// - End - RHTML_tab_button_str_view - MouseDown -------------------------------------------------------------

// - End - Tab Button - Source File --------------------------------------------------------------------------
