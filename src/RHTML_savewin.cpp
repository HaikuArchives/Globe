//////////////////////////////////////////////////////////////////////
//      Globe Webeditor © Gergely Rózsahegyi (RoGer) 2001-2002      //
//                                                                  //
//                       Save Window Source File                    //
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
#include <StringView.h>
#include <stdio.h>
#include "RHTML_win.h"

// -------------------------------------------------------------------- RHTML_Save_All_Win - RHTMLSaveAllWin -
RHTMLSaveAllWin::RHTMLSaveAllWin(BRect fRect,RHTMLWin *parent): BWindow(fRect,fOptions->GetLocaleString("Save&Quit_Window","Save & Quit"),B_MODAL_WINDOW,B_NOT_ZOOMABLE|B_NOT_MINIMIZABLE|B_NOT_RESIZABLE)
{ 
 fParent=parent;

 BView *view1= new BView(BRect(0,0,136,Bounds().bottom),"view",B_FOLLOW_TOP_BOTTOM,B_WILL_DRAW);
 AddChild(view1);
 fView = new BView(BRect(137,0,Bounds().right,Bounds().bottom),"fView",B_FOLLOW_ALL,B_WILL_DRAW);
 AddChild(fView);
 fList = new BListView(BRect(0,0,121,Bounds().bottom),"fList",B_MULTIPLE_SELECTION_LIST,B_FOLLOW_TOP_BOTTOM);
 fScrollView = new BScrollView("Param",fList,B_FOLLOW_ALL,0, false, true,B_FANCY_BORDER);
 view1->AddChild(fScrollView);
 fView->SetViewColor(216,216,216,0);
 fQuit = new BButton(BRect(11,10,80,30),"Q",fOptions->GetLocaleString("Save&Quit_Window_Button","Quit"),new BMessage('Q'));
 fCancel = new BButton(BRect(11,40,80,60),"C",fOptions->GetLocaleString("Save&Quit_Window_Button","Cancel"),new BMessage('C'));
 fSave = new BButton(BRect(11,70,80,90),"SQ",fOptions->GetLocaleString("Save&Quit_Window_Button","Save & Quit"),new BMessage('SQ'));

 float maxwidth= Bounds().right;

 float width = (int)fQuit->StringWidth(fOptions->GetLocaleString("Save&Quit_Window_Button","Quit"));
 if (fQuit->Bounds().right<width+10)
 {
  fQuit->ResizeTo(width+10,fQuit->Bounds().bottom);
  fCancel->ResizeTo(width+10,fCancel->Bounds().bottom);
  fSave->ResizeTo(width+10,fSave->Bounds().bottom);
  if (maxwidth<width+161) maxwidth=width+161;
 }

 width = fCancel->StringWidth(fOptions->GetLocaleString("Save&Quit_Window_Button","Cancel"));
 if (fCancel->Bounds().right<width+10)
 {
  fQuit->ResizeTo(width+10,fQuit->Bounds().bottom);
  fCancel->ResizeTo(width+10,fCancel->Bounds().bottom);
  fSave->ResizeTo(width+10,fSave->Bounds().bottom);
  if (maxwidth<width+161) maxwidth=width+161;
 }

 width = fSave->StringWidth(fOptions->GetLocaleString("Save&Quit_Window_Button","Save & Quit"));
 if (fSave->Bounds().right<width+10)
 {
  fQuit->ResizeTo(width+10,fQuit->Bounds().bottom);
  fCancel->ResizeTo(width+10,fCancel->Bounds().bottom);
  fSave->ResizeTo(width+10,fSave->Bounds().bottom);
  if (maxwidth<width+161) maxwidth=width+161;
 }

 if (maxwidth>Bounds().right) ResizeTo(maxwidth,Bounds().bottom);

 fView->AddChild(fSave);
 fView->AddChild(fCancel);
 fView->AddChild(fQuit);

 fList->MakeEmpty();
 for (uint i=0;i<fParent->textdb;i++)
 {
  if (fParent->textview[i]->Modify())
   fList->AddItem(new BStringItem(fParent->amenu->GetTabText(i)));
 }
 fList->Select(0,fList->CountItems()-1,true);

 list = (int*) calloc(fList->CountItems(),sizeof(int));
 list2 = (int*) calloc(fList->CountItems(),sizeof(int));
 list2db=0;
 for (uint i=0;i<fParent->textdb;i++)
 {
  if (fParent->textview[i]->Modify())
  {
   list2[list2db]=i;
   list2db++;
  }
 }
 SetTarget(fParent);
}
// - End - RHTML_Save_All_Win - RHTMLSaveAllWin --------------------------------------------------------------

// --------------------------------------------------------------------- RHTML_Save_All_Win - QuitRequested -
bool RHTMLSaveAllWin::QuitRequested()
{
 fParent->fSaveWinShow=false;
 return BWindow::QuitRequested();
}
// - End - RHTML_Save_All_Win - QuitRequested ----------------------------------------------------------------

// -------------------------------------------------------------------- RHTML_Save_All_Win - MessageReceived -
void RHTMLSaveAllWin::MessageReceived(BMessage *msg)
{
 switch (msg->what)
  {
   case 'SQ':
   {
    listdb=0;
    for (uint i=0;i<(uint) fList->CountItems();i++)
    {
     if (fList->IsItemSelected(i))
     {
      list[listdb]=list2[i];
      listdb++;
     }
    }
    if (listdb>0)
    {
     Hide();
     fParent->savequit=true;
     Invoke(new BMessage('QSAV'));
    } else
    {
     be_app->PostMessage(B_QUIT_REQUESTED);
     BWindow::QuitRequested();
     fParent->BWindow::QuitRequested();
    }
   
   }
   break;
   case 'C':
   {
    fParent->fSaveWinShow=false;
    Close();
   }
   break;
   case 'Q':
   {
    be_app->PostMessage(B_QUIT_REQUESTED);
    BWindow::QuitRequested();
    fParent->BWindow::QuitRequested();
   }
   break;
  default:
   {
    BWindow::MessageReceived(msg);
   }
   break;
  }
}
// - End - RHTML_Save_All_Win - MessageReceived --------------------------------------------------------------

// ------------------------------------------------------------------------ RHTML_Save_All_Win - GetListItem -
int RHTMLSaveAllWin::GetListItem(int num)
{
 return list[num];
}
// - End - RHTML_Save_All_Win - GetListItem ------------------------------------------------------------------

// ------------------------------------------------------------------- RHTML_Save_All_Win - GetListItemCount -
int RHTMLSaveAllWin::GetListItemCount()
{
 return listdb;
}
// - End - RHTML_Save_All_Win - GetListItemCount -------------------------------------------------------------

// ---------------------------------------------------------------- RHTML_Save_Close_Win - RHTMLSaveCloseWin -
RHTMLSaveCloseWin::RHTMLSaveCloseWin(BRect fRect,RHTMLWin *parent,int num): BWindow(fRect,fOptions->GetLocaleString("Save&Close_Window","Save & Close"),B_MODAL_WINDOW,B_NOT_ZOOMABLE|B_NOT_MINIMIZABLE|B_NOT_RESIZABLE)
{ 
 fParent=parent;
 fNum=num;
 fView = new BView(BRect(137,0,Bounds().right,Bounds().bottom),"fView",B_FOLLOW_ALL,B_WILL_DRAW);
 AddChild(fView);
 fView->SetViewColor(216,216,216,0);
 BStringView *fStrView1= new BStringView(BRect(0,30,136,50),"strview",fOptions->GetLocaleString("Save&Close_Window","Save"));
 BStringView *fStrView2= new BStringView(BRect(0,51,136,70),"strview","xy.html?");
 AddChild(fStrView1);
 AddChild(fStrView2);
 fStrView1->SetAlignment(B_ALIGN_CENTER);
 fStrView2->SetAlignment(B_ALIGN_CENTER);
 BString felirat;
 felirat << fParent->amenu->GetTabText(fNum) << fOptions->GetLocaleString("Save&Close_Window","?");
 fStrView1->SetViewColor(255,255,255,0);
 fStrView1->SetHighColor(0,0,0,0);
 fStrView2->SetText(felirat.String());
 fStrView2->SetViewColor(255,255,255,0);
 fStrView2->SetHighColor(0,0,0,0);
 fClose = new BButton(BRect(11,10,80,30),"Q",fOptions->GetLocaleString("Save&Close_Window_Button","Close"),new BMessage('Q'));
 fCancel = new BButton(BRect(11,40,80,60),"C",fOptions->GetLocaleString("Save&Close_Window_Button","Cancel"),new BMessage('C'));
 fSave = new BButton(BRect(11,70,80,90),"SQ",fOptions->GetLocaleString("Save&Close_Window_Button","Save & Close"),new BMessage('SC'));
 float maxwidth=Bounds().right;

 float width = fClose->StringWidth(fOptions->GetLocaleString("Save&Close_Window_Button","Close"));
 if (fClose->Bounds().right<width+10)
 {
  fClose->ResizeTo(width+10,fClose->Bounds().bottom);
  fCancel->ResizeTo(width+10,fCancel->Bounds().bottom);
  fSave->ResizeTo(width+10,fSave->Bounds().bottom);
  if (maxwidth<width+161) maxwidth=width+161;
 }

 width = fCancel->StringWidth(fOptions->GetLocaleString("Save&Close_Window_Button","Cancel"));
 if (fCancel->Bounds().right<width+10)
 {
  fClose->ResizeTo(width+10,fClose->Bounds().bottom);
  fCancel->ResizeTo(width+10,fCancel->Bounds().bottom);
  fSave->ResizeTo(width+10,fSave->Bounds().bottom);
  if (maxwidth<width+161) maxwidth=width+161;
 }

 width = fSave->StringWidth(fOptions->GetLocaleString("Save&Close_Window_Button","Save & Close"));
 if (fSave->Bounds().right<width+10)
 {
  fClose->ResizeTo(width+10,fClose->Bounds().bottom);
  fCancel->ResizeTo(width+10,fCancel->Bounds().bottom);
  fSave->ResizeTo(width+10,fSave->Bounds().bottom);
  if (maxwidth<width+161) maxwidth=width+161;
 }

 if (maxwidth>Bounds().right) ResizeTo(maxwidth,Bounds().bottom);

 fView->AddChild(fSave);
 fView->AddChild(fCancel);
 fView->AddChild(fClose);
 SetTarget(fParent);
}
// - End - RHTML_Save_Close_Win - RHTMLSaveCloseWin ----------------------------------------------------------

// ------------------------------------------------------------------ RHTML_Save_Close_Win - MessageReceived -
void RHTMLSaveCloseWin::MessageReceived(BMessage *msg)
{
 switch (msg->what)
  {
   case 'Q':
   {
    Invoke(new BMessage('CSRM'));
    Close();
   }
   break;
   case 'C':
   {
    Close();
   }
   break;
   case 'SC':
   {
    Invoke(new BMessage('CSSC'));
    Close();
   }
   break;
   default:
   {
    BWindow::MessageReceived(msg);
   }
   break;
  }
}
// - End - RHTML_Save_Close_Win - MessageReceived ------------------------------------------------------------

// - End - Save Window - Source File -------------------------------------------------------------------------
