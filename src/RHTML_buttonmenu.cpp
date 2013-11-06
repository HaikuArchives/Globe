//////////////////////////////////////////////////////////////////////
//      Globe Webeditor © Gergely Rózsahegyi (RoGer) 2001-2003      //
//                                                                  //
//                       Button Menu Source File                    //
//                                                                  //
//                       Last Update: 2003.04.27					//
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

#include "RHTML_buttonmenu.h"

#include <Application.h>
#include <Bitmap.h>
#include <TranslationUtils.h>
#include <Alert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

const uint32 B_UPDATE_VIEW_MSG = 'BUVM';

// --------------------------------------------------------------------- RHTML_button_menu - RHTMLbuttonmenu -
RHTMLbuttonmenu::RHTMLbuttonmenu(BRect fRect,BToolBar *fmenu): BView(fRect,"name",B_FOLLOW_LEFT_RIGHT|B_FOLLOW_TOP,B_WILL_DRAW)
{
 fMenu=fmenu;
 rgb_color vszurke={239,239,239};
 rgb_color szurke={216,216,216};
 rgb_color sszurke={184,184,184};
 rgb_color ssszurke={152,152,152};

 fview = new BView(BRect(0,0,Bounds().right,0),"fview",B_FOLLOW_LEFT_RIGHT,B_WILL_DRAW);
 AddChild(fview);
 fview->SetViewColor(vszurke);

 bview = new BView(BRect(1,0,1,Bounds().bottom-1),"fview",B_FOLLOW_TOP_BOTTOM,B_WILL_DRAW);
 AddChild(bview);
 bview->SetViewColor(vszurke);

 bbview = new BView(BRect(0,0,0,Bounds().bottom-1),"fview",B_FOLLOW_TOP_BOTTOM,B_WILL_DRAW);
 AddChild(bbview);
 bbview->SetViewColor(ssszurke);

 ffview = new BView(BRect(1,17,Bounds().right,17),"fview",B_FOLLOW_LEFT_RIGHT,B_WILL_DRAW);
 AddChild(ffview);
 ffview->SetViewColor(vszurke);

 fffview = new BView(BRect(1,16,Bounds().right,16),"fview",B_FOLLOW_LEFT_RIGHT,B_WILL_DRAW);
 AddChild(fffview);
 fffview->SetViewColor(sszurke);

 felsoview = new BView(BRect(2,1,Bounds().right,15),"fview",B_FOLLOW_LEFT_RIGHT,B_WILL_DRAW);
 AddChild(felsoview);
 felsoview->SetViewColor(szurke);

 SetViewColor(szurke);

 tbdb=0;
 tb = (RHTMLmenubutton**) calloc(1,sizeof(RHTMLmenubutton));
 fx=0;

 menubardb=0;
 menubar = (BToolBar**) calloc(1,sizeof(BToolBar));

 current=0;

 BString dir=fOptions->SettingsDir;
 dir << "menu/menubar.rdat";
 FILE *f;
 char c;
 char cha[1000];
 int i=0;
 if( (f = fopen(dir.String(),"rt")) == NULL); else
 {
  while (feof(f)==false) 
  {
   cha[0]=0;
   c=0;
   i=0;
   while ((c!='\n')&&(!feof(f)))
   {
    fscanf(f,"%c",&c);
    cha[i]=c;
    i++;
    cha[i]=0;
   }
   cha[i-1]=0;
   BString str1,str2,str3,str4;
   str1.SetTo(cha);
   int x,y;

   x=str1.FindFirst('<');
   y=str1.FindFirst(' ',x);
   if ((x>=0) && (y>=0) && ((y-x)>=0))
   str1.CopyInto(str2,x,y-x);
  
   if (str2.ICompare("<MENUBAR")==0)
   {
    i=str1.FindFirst('<')+1;
    BString name,text,alt;
    while (i<=str1.FindFirst('>',i)-1)
    {
     x=str1.FindFirst(' ',i)+1;
     y=str1.FindFirst('=',x);
     if ((x>=0) && (y>=0) && ((y-x)>=0))
     str1.CopyInto(str3,x,y-x);

     x=str1.FindFirst('\'',i)+1;
     y=str1.FindFirst('\'',x);
     if ((x>=0) && (y>=0) && ((y-x)>=0))
     str1.CopyInto(str4,x,y-x);

     i=y+1;

     if (str3.ICompare("HEIGHT")==0)
     {
      float height=0;
      
      sscanf(str4.String(),"%f",&height);
      ResizeTo(Bounds().right,height);
      fMenu->SetHorizontalHeight(height);
     }
    }  
   }
   if (str2.ICompare("<MENU")==0)
   {
    i=str1.FindFirst('<')+1;
    BString name,text,alt;
    while (i<=str1.FindFirst('>',i)-1)
    {
     x=str1.FindFirst(' ',i)+1;
     y=str1.FindFirst('=',x);
     if ((x>=0) && (y>=0) && ((y-x)>=0))
     str1.CopyInto(str3,x,y-x);

     x=str1.FindFirst('\'',i)+1;
     y=str1.FindFirst('\'',x);
     if ((x>=0) && (y>=0) && ((y-x)>=0))
     str1.CopyInto(str4,x,y-x);

     i=y+1;

     if (str3.ICompare("TEXT")==0)
     {
      if (str4.FindFirst(':')>=0)
      {
       BString tmp1,tmp2;
       str4.CopyInto(tmp1,0,str4.FindFirst(':'));
       str4.CopyInto(tmp2,str4.FindFirst(':')+1,str4.Length()-str4.FindFirst(':')-1);
       text.SetTo(fOptions->GetLocaleString(tmp1.String(),tmp2.String()));
      } else text.SetTo("");
     }
     if (str3.ICompare("NAME")==0) name.SetTo(str4.String());
     if (str3.ICompare("ALT")==0)
     {
      BString tmp1,tmp2;
      str4.CopyInto(tmp1,0,str4.FindFirst(':'));
      str4.CopyInto(tmp2,str4.FindFirst(':')+1,str4.Length()-str4.FindFirst(':')-1);
      alt.SetTo(fOptions->GetLocaleString(tmp1.String(),tmp2.String()));
     }
    }  
    AddTab(name.String(),text.String(),alt.String());
   }
   if (str2.ICompare("<BUTTON")==0)
   {
    i=str1.FindFirst('<')+1;
    BString menu,img,alt;
    int cmddb=0;
    BString cmd[10];
    BString param[10];
    while (i<=str1.FindFirst('>',i)-1)
    {
     x=str1.FindFirst(' ',i)+1;
     y=str1.FindFirst('=',x);
     if ((x>=0) && (y>=0) && ((y-x)>=0))
     str1.CopyInto(str3,x,y-x);
 
     x=str1.FindFirst('\'',i)+1;
     y=str1.FindFirst('\'',x);
     if ((x>=0) && (y>=0) && ((y-x)>=0))
     str1.CopyInto(str4,x,y-x);

     i=y+1;

     if (str3.ICompare("MENU")==0) menu.SetTo(str4.String());
     if (str3.ICompare("IMG")==0) img.SetTo(str4.String());
     if (str3.ICompare("ALT")==0)
     {
      BString tmp1,tmp2;
      str4.CopyInto(tmp1,0,str4.FindFirst(':'));
      str4.CopyInto(tmp2,str4.FindFirst(':')+1,str4.Length()-str4.FindFirst(':')-1);
      alt.SetTo(fOptions->GetLocaleString(tmp1.String(),tmp2.String()));
     }
     if ((str3.ICompare("NEW")==0)||
         (str3.ICompare("OPEN")==0)||
         (str3.ICompare("SAVE")==0)||
         (str3.ICompare("SAVE_ALL")==0)||
         (str3.ICompare("CUT")==0)||
         (str3.ICompare("COPY")==0)||
         (str3.ICompare("PASTE")==0)||
         (str3.ICompare("UNDO")==0)||
         (str3.ICompare("REDO")==0)||
         (str3.ICompare("INSERTBACK")==0)||
         (str3.ICompare("INSERTFOR")==0)||
         (str3.ICompare("INSERT")==0)||
         (str3.ICompare("REDO")==0)||
         (str3.ICompare("QUIT")==0)||
         (str3.ICompare("CLOSE")==0))
     {
      cmddb++;
      cmd[cmddb-1].SetTo(str3.String());
      param[cmddb-1].SetTo(str4.String());
     }
    }
    for (int o=0;o<menubardb;o++)
    {
     if (menu.ICompare(menubar[o]->Name())==0)
     {
      BString dir = fOptions->SettingsDir;
      dir << "menu/pic/" << img;
      ButtonView *fItem = new ButtonView("name",dir.String(),dir.String(),new BMessage('MUDN'),alt.String());
      menubar[o]->AddToolItem(fItem);
      
      for (int p=0;p<cmddb;p++)
      {
       fItem->AddCmd(cmd[p].String(),param[p].String());
      }
     }
    }
   }
  }
  fclose(f);
 }
 if (tbdb>0) fChange(0);
}
// - End - RHTML_button_menu - RHTMLbuttonmenu ---------------------------------------------------------------

// ------------------------------------------------------------------------------ RHTML_button_menu - AddTab -

void RHTMLbuttonmenu::AddTab(const char *name,const char *text,const char *alt)
{
 tbdb++;
 tb = (RHTMLmenubutton**) realloc(tb,sizeof(RHTMLmenubutton)*tbdb);
 tb[tbdb-1] = new RHTMLmenubutton(BRect(fx+2,0,fx+100,felsoview->Bounds().bottom),name,text,alt,tbdb-1,this);
 felsoview->AddChild(tb[tbdb-1]);
 fx=fx+tb[tbdb-1]->Bounds().right+2;
 AddMenuBar(name);
}
// - End - RHTML_button_menu - AddTab ------------------------------------------------------------------------

// ----------------------------------------------------------------------------- RHTML_button_menu - fChange -
void RHTMLbuttonmenu::fChange(uint8 fnum)
{
 if (tbdb>fnum)
 {
  for (int i=0;i<tbdb;i++) tb[i]->SetOFF();
  for (int i=0;i<tbdb;i++) menubar[i]->ResizeTo(-1,-1);
  tb[fnum]->SetON();
  menubar[fnum]->ResizeTo(Bounds().right-2,Bounds().bottom-18);
  current=fnum;
 } 
}
// - End - RHTML_button_menu - fChange -----------------------------------------------------------------------

// -------------------------------------------------------------------------- RHTML_button_menu - AddMenuBar -
void RHTMLbuttonmenu::AddMenuBar(const char *name)
{
 menubardb++;
 menubar = (BToolBar**) realloc(menubar,sizeof(BToolBar)*menubardb);
 menubar[menubardb-1] = new BToolBar(name,BRect(4,2,2,2),BRect(2,10,2,2),2);
 menubar[menubardb-1]->SetEnabledOrientation(false,true,false,false);
 menubar[menubardb-1]->SetBorderStyle(B_NONE);
 menubar[menubardb-1]->SetMoveable(false);
 menubar[menubardb-1]->SetOrientation(B_BOTTOM);
 AddChild(menubar[menubardb-1]);
 menubar[menubardb-1]->MoveTo(2,18);
}
// - End - RHTML_button_menu - AddMenuBar --------------------------------------------------------------------

// ---------------------------------------------------------------------------- RHTML_button_menu - ResizeTo -
void RHTMLbuttonmenu::ResizeTo(float width, float height)
{
 BView::ResizeTo(width,height);
 fChange(current);
}
// - End - RHTML_button_menu - AddMenuBar --------------------------------------------------------------------

// --------------------------------------------------------------------- RHTML_menu_button - RHTMLmenubutton -
RHTMLmenubutton::RHTMLmenubutton(BRect fRect,const char *name,const char *text,const char *alt,uint8 num,RHTMLbuttonmenu *target): BView(fRect,"tabbutton",B_FOLLOW_TOP_BOTTOM,B_WILL_DRAW)
{
 fName = new BString(name);
 fText = new BString(text);

 fNum=num;
 fTarget=target;
 rgb_color vszurke={239,239,239};
 rgb_color szurke={216,216,216};
 rgb_color sszurke={184,184,184};


 BFont *font= new BFont(be_plain_font);
 font->SetSize(11);
 fstrview= new RHTMLbuttonmenustrview(BRect(2,1,Bounds().right-4,Bounds().bottom),fText->String(),new BString(alt),this);
 fstrview->SetAlignment(B_ALIGN_CENTER);
 fstrview->SetFont(font);
 AddChild(fstrview);
 SetViewColor(szurke);
 ResizeTo(fstrview->StringWidth(fText->String())+12,Bounds().bottom);
 fstrview->ResizeTo(fstrview->StringWidth(fText->String())+8,fstrview->Bounds().bottom);
 j1view = new BView(BRect(Bounds().right-1,2,Bounds().right-1,Bounds().bottom-2),"j1view",B_FOLLOW_TOP_BOTTOM,B_WILL_DRAW);
 AddChild(j1view);
 j1view->SetViewColor(sszurke);

 j2view = new BView(BRect(Bounds().right,2,Bounds().right,Bounds().bottom-2),"j2view",B_FOLLOW_TOP_BOTTOM,B_WILL_DRAW);
 AddChild(j2view);
 j2view->SetViewColor(vszurke);
}
// - End - RHTML_menu_button - RHTMLmenubutton ---------------------------------------------------------------

// ------------------------------------------------------------------------------- RHTML_menu_button - SetON -
void RHTMLmenubutton::SetON()
{
 BFont *font= new BFont(be_bold_font);
 font->SetSize(11);
 fstrview->SetFont(font);
 fstrview->Hide();
 fstrview->Show();
}
// - End - RHTML_menu_button - SetON -------------------------------------------------------------------------

// ------------------------------------------------------------------------------ RHTML_menu_button - SetOFF -
void RHTMLmenubutton::SetOFF()
{
 BFont *font= new BFont(be_plain_font);
 font->SetSize(11);
 fstrview->SetFont(font);
 fstrview->Hide();
 fstrview->Show();
}
// - End - RHTML_menu_button - SetOFF ------------------------------------------------------------------------

// ------------------------------------------------------------------ RHTML_button_menu_strview - MouseMoved -
void RHTMLbuttonmenustrview::MouseMoved(BPoint point, uint32 transit,const BMessage *message)
{
 if ((Bounds().Contains(point)) && (Window()->IsActive())) {
  if (!fIn)
  {
   BMessage	msg(eToolTipStart);
   msg.AddPoint("start", ConvertToScreen(point));
   msg.AddRect("bounds", ConvertToScreen(Bounds()));
   msg.AddString("string", fAlt->String());
   be_app->PostMessage(&msg);
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
// - End - RHTML_button_menu_strview - MouseMoved ------------------------------------------------------------

// --------------------------------------------------------------------------- RHTML_menu_button - MouseDown -
void RHTMLmenubutton::MouseDown(BPoint where)
{
 uint32 button;
 this->Window()->CurrentMessage()->FindInt32("buttons", (int32*)&button);
 if (button==0x01) BalGomb();
 if (button==0x02)
 {
  ConvertToScreen(&where);
 }
}
// - End - RHTML_menu_button - MouseDown ---------------------------------------------------------------------

// ----------------------------------------------------------------------------- RHTML_menu_button - BalGomb -
void RHTMLmenubutton::BalGomb()
{
 fTarget->fChange(fNum);
}
// - End - RHTML_menu_button - BalGomb -----------------------------------------------------------------------

// ------------------------------------------------------ RHTML_button_menu_strview - RHTMLbuttonmenustrview -
RHTMLbuttonmenustrview::RHTMLbuttonmenustrview(BRect fRect,const char *name,BString *alt,RHTMLmenubutton *target): BStringView(fRect,"strview",name)
{
 fTarget=target;
 fIn=false;
 fAlt=alt;
}
// - End - RHTML_button_menu_strview - RHTMLbuttonmenustrview ------------------------------------------------

// ------------------------------------------------------------------- RHTML_button_menu_strview - MouseDown -
void RHTMLbuttonmenustrview::MouseDown(BPoint where)
{
 uint32 button;
 this->Window()->CurrentMessage()->FindInt32("buttons", (int32*)&button);
 if (button==0x01) fTarget->BalGomb();
 if (button==0x02)
 {
  ConvertToScreen(&where);
 }
}
// - End - RHTML_button_menu_strview - MouseDown -------------------------------------------------------------

// --------------------------------------------------------------------- RHTML_button_menu - MessageReceived -
void RHTMLbuttonmenu::MessageReceived(BMessage *msg)
{
 switch (msg->what)
  {
   case B_UPDATE_VIEW_MSG:
   {
    float height=0;
    for (uint8 i=0;i<menubardb;i++)
    {
     if (menubar[i]->Frame().bottom>height) height=menubar[i]->Frame().bottom;
    }
    fMenu->SetHorizontalHeight(height+1);
    ResizeTo(Bounds().right,fMenu->Bounds().bottom-1);
   }
   break;
   case B_MOUSE_MOVED:
   {
   }
   break;
   default:
   {
    BView::MessageReceived(msg);
   }
   break;
  }
}
// - End - RHTML_button_menu - MessageReceived ---------------------------------------------------------------

// - End - Button Menu - Source File -------------------------------------------------------------------------
