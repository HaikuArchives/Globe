//////////////////////////////////////////////////////////////////////
//     Extended BeOS Api © Gergely Rózsahegyi (RoGer) 2002-2003     //
//                           roger@beos.hu                          //
//                                                                  //
//                       BToolView Source File                      //
//                                                                  //
//                      Last Update: 2003.05.13                     //
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
#include <Message.h>
#include <stdlib.h>
#include "ToolView.h"

#include <stdio.h>

const uint32 B_ORI_T_MSG = 'BOTM';
const uint32 B_ORI_B_MSG = 'BOBM';
const uint32 B_ORI_L_MSG = 'BOLM';
const uint32 B_ORI_R_MSG = 'BORM';
const uint32 B_UPDATE_VIEW_MSG = 'BUVM';
const uint32 B_MOUSE_DOWN_MSG = 'BMDM';
const uint32 B_MOUSE_UP_MSG = 'BMUM';
const uint32 B_MOUSE_MOVED_MSG = 'BMMM';
const uint32 B_TOOL_MOVE_MSG = 'BTMM';

// --------------------------------------------------------------------------------- B_Tool_View - BToolView -
BToolView::BToolView(BRect frame,BView *view, uint32 resizeMask, uint32 flags):BView(frame,"Extended BeOS Api",resizeMask,flags)
{
 fView=view;
 isdown=false;
 SetEventMask(B_NO_POINTER_HISTORY);
 fView->ResizeTo(Bounds().right,Bounds().bottom);
 fView->MoveTo(0,0);
 AddChild(fView);
 
 BBDbL=0;
 BBDbR=0;
 BBDbT=0;
 BBDbB=0;
 fBBT = (BToolBar**) calloc(1,sizeof(BToolBar*));
 fBBB = (BToolBar**) calloc(1,sizeof(BToolBar*));
 fBBL = (BToolBar**) calloc(1,sizeof(BToolBar*));
 fBBR = (BToolBar**) calloc(1,sizeof(BToolBar*));
}

// -------------------------------------------------------------------------------- B_Tool_View - AddToolBar -
void BToolView::AddToolBar(BToolBar *fBar, orient ori)
{
 if (ori==B_TOP)
 {
  float oldtop=GetTop();
  BBDbT++;
  fBBT = (BToolBar**) realloc(fBBT,sizeof(BToolBar*)*BBDbT);
  fBBT[BBDbT-1]=fBar;
  AddChild(fBar);
  fBar->MessageReceived(new BMessage(B_ORI_T_MSG));
  fBar->ResizeTo(Bounds().right,fBar->HorizontalHeight());
  fBar->MoveTo(0,oldtop);
  fBar->SetIndex(BBDbT-1);
  UpdateView();
 }

 if (ori==B_BOTTOM)
 {
  float oldbottom=GetBottom();
  BBDbB++;

  fBBB = (BToolBar**) realloc(fBBB,sizeof(BToolBar*)*BBDbB);
  fBBB[BBDbB-1]=fBar;
  AddChild(fBar);
  fBar->MessageReceived(new BMessage(B_ORI_B_MSG));
  fBar->ResizeTo(Bounds().right,fBar->HorizontalHeight());
  fBar->MoveTo(0,oldbottom-fBar->Bounds().bottom);
  fBar->SetIndex(BBDbB-1);
  UpdateView();
 }

 if (ori==B_LEFT)
 {
  float oldleft=GetLeft();
  BBDbL++;
  fBBL = (BToolBar**) realloc(fBBL,sizeof(BToolBar*)*BBDbL);
  fBBL[BBDbL-1]=fBar;
  AddChild(fBar);
  fBar->MessageReceived(new BMessage(B_ORI_L_MSG));
  fBar->ResizeTo(fBar->VerticalWidth(),GetBottom()-GetTop());
  fBar->MoveTo(oldleft,GetTop());
  fBar->SetIndex(BBDbL-1);
  UpdateView();
 }

 if (ori==B_RIGHT)
 {
  float oldright=GetRight();
  BBDbR++;
  fBBR = (BToolBar**) realloc(fBBR,sizeof(BToolBar*)*BBDbR);
  fBBR[BBDbR-1]=fBar;
  AddChild(fBar);
  fBar->MessageReceived(new BMessage(B_ORI_R_MSG));
  fBar->ResizeTo(fBar->VerticalWidth(),GetBottom()-GetTop());
  fBar->MoveTo(oldright-fBar->Bounds().right,GetTop());
  fBar->SetIndex(BBDbR-1);
  UpdateView();
 }
  UpdateLeftRight();
}

// ----------------------------------------------------------------------------- B_Tool_View - RemoveToolBar -
void BToolView::RemoveToolBar(BToolBar *fBar)
{
 uint32 i;
 for (i=0;i<BBDbT;i++)
 {
  if (fBBT[i]==fBar)
  {
   BBDbT--;
   float oldtop=fBar->Frame().top;
   for (uint32 o=i;o<BBDbT;o++)
   {
    fBBT[o]=fBBT[o+1];
    fBBT[o]->MoveTo(0,oldtop);
    fBBT[o]->SetIndex(o);
    oldtop=fBBT[o]->Frame().bottom+1;
   }
    RemoveChild(fBar);
    fBBT = (BToolBar**) realloc(fBBT,sizeof(BToolBar*)*BBDbT);
    UpdateView();
    UpdateLeftRight();
    Update();
  }
 }
 
 for (i=0;i<BBDbB;i++)
 {
  if (fBBB[i]==fBar)
  {
   BBDbB--;
   float oldbottom=fBar->Frame().bottom;
   for (uint32 o=i;o<BBDbB;o++)
   {
    fBBB[o]=fBBB[o+1];
    fBBB[o]->MoveTo(0,oldbottom-fBBB[o]->Bounds().bottom);
    fBBB[o]->SetIndex(o);
    oldbottom=fBBB[o]->Frame().top-1;
   }
    RemoveChild(fBar);
    fBBB = (BToolBar**) realloc(fBBB,sizeof(BToolBar*)*BBDbB);
    UpdateView();
    UpdateLeftRight();
    Update();
  }
 }
 
 for (i=0;i<BBDbL;i++)
 {
  if (fBBL[i]==fBar)
  {
   BBDbL--;
   float oldleft=fBar->Frame().left;
   for (uint32 o=i;o<BBDbL;o++)
   {
    fBBL[o]=fBBL[o+1];
    fBBL[o]->MoveTo(oldleft,GetTop());
    fBBL[o]->SetIndex(o);
    oldleft=fBBL[o]->Frame().right+1;
   }
    RemoveChild(fBar);
    fBBL = (BToolBar**) realloc(fBBL,sizeof(BToolBar*)*BBDbL);
    UpdateView();
    UpdateLeftRight();
    Update();
  }
 }

 for (i=0;i<BBDbR;i++)
 {
  if (fBBR[i]==fBar)
  {
   BBDbR--;
   float oldright=fBar->Frame().right;
   for (uint32 o=i;o<BBDbR;o++)
   {
    fBBR[o]=fBBR[o+1];
    fBBR[o]->MoveTo(oldright-fBBR[o]->Bounds().right,GetTop());
    fBBR[o]->SetIndex(o);
    oldright=fBBR[o]->Frame().left-1;
   }
    RemoveChild(fBar);
    fBBR = (BToolBar**) realloc(fBBR,sizeof(BToolBar*)*BBDbR);
    UpdateView();
    UpdateLeftRight();
    Update();
  }
 }
}
 
// ------------------------------------------------------------------------------------- B_Tool_View - Index -
uint32 BToolView::Index(BToolBar *fBar)
{
 uint32 i;
 for (i=0;i<BBDbT;i++)
  if (fBBT[i]==fBar)
   return i;
 for (i=0;i<BBDbB;i++)
  if (fBBB[i]==fBar)
   return i;
 for (i=0;i<BBDbL;i++)
  if (fBBL[i]==fBar)
   return i;
 for (i=0;i<BBDbR;i++)
  if (fBBR[i]==fBar)
   return i;
 return 0;
}

// ---------------------------------------------------------------------------------- B_Tool_View - SetIndex -
void BToolView::SetIndex(BToolBar *fBar,uint32 index)
{
 uint32 i;
 while (i<BBDbT)
 {
  if (fBBT[i]==fBar)
  {
   fBBT[i]=fBBT[index];
   fBBT[index]=fBar;
   i=BBDbT;
  }
  i++;
 }
 i=0;
 while (i<BBDbB)
 {
  if (fBBB[i]==fBar)
  {
   fBBB[i]=fBBB[index];
   fBBB[index]=fBar;
   i=BBDbB;
  }
  i++;
 }
 i=0;
 while (i<BBDbL)
 {
  if (fBBL[i]==fBar)
  {
   fBBL[i]=fBBL[index];
   fBBL[index]=fBar;
   i=BBDbL;
  }
  i++;
 }
 i=0;
 while (i<BBDbR)
 {
  if (fBBR[i]==fBar)
  {
   fBBR[i]=fBBR[index];
   fBBR[index]=fBar;
   i=BBDbR;
  }
  i++;
 }
 Update();
} 
 
// -------------------------------------------------------------------------------- B_Tool_View - UpdateView -
void BToolView::UpdateView()
{
 fView->ResizeTo(GetRight()-GetLeft(),GetBottom()-GetTop());
 fView->MoveTo(GetLeft(),GetTop());
}

// --------------------------------------------------------------------------- B_Tool_View - UpdateLeftRight -
void BToolView::UpdateLeftRight()
{
 for (uint32 i=0;i<BBDbL;i++)
 {
  fBBL[i]->MoveTo(fBBL[i]->Frame().left,GetTop());
  fBBL[i]->ResizeTo(fBBL[i]->VerticalWidth(),GetBottom()-GetTop());
 }
 for (uint32 i=0;i<BBDbR;i++)
 {
  fBBR[i]->MoveTo(fBBR[i]->Frame().left,GetTop());
  fBBR[i]->ResizeTo(fBBR[i]->VerticalWidth(),GetBottom()-GetTop());
 }
}

// ------------------------------------------------------------------------------------ B_Tool_View - Update -
void BToolView::Update()
{
 float tmp;

 tmp=0;
 for (uint32 i=0;i<BBDbT;i++)
 {
  fBBT[i]->MoveTo(0,tmp);
  tmp=fBBT[i]->Frame().bottom+1;
 }

 tmp=Bounds().bottom;
 for (uint32 i=0;i<BBDbB;i++)
 {
  fBBB[i]->MoveTo(0,tmp-fBBB[i]->Bounds().bottom);
  tmp=fBBB[i]->Frame().top-1;
 }

 tmp=0;
 for (uint32 i=0;i<BBDbL;i++)
 {
  fBBL[i]->MoveTo(tmp,GetTop());
  fBBL[i]->ResizeTo(fBBL[i]->VerticalWidth(),GetBottom()-GetTop());
  tmp=fBBL[i]->Frame().right+1;
 }

 tmp=Bounds().right;
 for (uint32 i=0;i<BBDbR;i++)
 {
  fBBR[i]->MoveTo(tmp-fBBR[i]->Bounds().right,GetTop());
  fBBR[i]->ResizeTo(fBBR[i]->VerticalWidth(),GetBottom()-GetTop());
  tmp=fBBR[i]->Frame().left-1;
 }
}

// ----------------------------------------------------------------------------------- B_Tool_View - GetLeft -
float BToolView::GetLeft()
{
 if (BBDbL>0)
  return fBBL[BBDbL-1]->Frame().right+1;
 else
  return 0;
}

// ---------------------------------------------------------------------------------- B_Tool_View - GetRight -
float BToolView::GetRight()
{
 if (BBDbR>0)
  return fBBR[BBDbR-1]->Frame().left-1;
 else
  return Bounds().right;
}

// ------------------------------------------------------------------------------------ B_Tool_View - GetTop -
float BToolView::GetTop()
{
 if (BBDbT>0)
  return fBBT[BBDbT-1]->Frame().bottom+1;
 else
  return 0;
}

// --------------------------------------------------------------------------------- B_Tool_View - GetBottom -
float BToolView::GetBottom()
{
 if (BBDbB>0)
  return fBBB[BBDbB-1]->Frame().top-1;
 else
  return Bounds().bottom;
}

// ----------------------------------------------------------------------------------- B_Tool_View - MouseUp -
void BToolView::MouseUp(BPoint where)
{
 BPoint point;
 uint32 button;
 if (Window())
  GetMouse(&point,&button,false);
  BMessage *msg = new BMessage(B_MOUSE_UP_MSG);
  msg->AddPoint("pont",ConvertToScreen(where));
  msg->AddBool("gomb",button==B_PRIMARY_MOUSE_BUTTON);
 
   for (uint32 i=0;i<BBDbT;i++)
   fBBT[i]->MessageReceived(msg);
  for (uint32 i=0;i<BBDbB;i++)
   fBBB[i]->MessageReceived(msg);
  for (uint32 i=0;i<BBDbL;i++)
   fBBL[i]->MessageReceived(msg);
  for (uint32 i=0;i<BBDbR;i++)
   fBBR[i]->MessageReceived(msg);
 
 if (button!=B_PRIMARY_MOUSE_BUTTON)
 {
  isdown=false;
  fBBar=NULL;
 }
 

 if (button!=B_PRIMARY_MOUSE_BUTTON)
 {
  SetEventMask(B_NO_POINTER_HISTORY);
  BMessage *msg = new BMessage(B_MOUSE_UP_MSG);
  msg->AddPoint("pont",ConvertToScreen(where));
  if (Parent())
   Parent()->MessageReceived(msg);
 }

}

// -------------------------------------------------------------------------------- B_Tool_View - MouseMoved -
void BToolView::MouseMoved(BPoint where,uint32 code,const BMessage *a_message)
{
 BPoint point;
 uint32 button;
 if (Window())
  GetMouse(&point,&button,false);

 if (isdown && where.y<GetTop()+20 && where.y>GetTop() && where.x>GetLeft()+20 && where.x<GetRight()-20 && fBBar->EnabledOrientation(B_TOP) && fBBT[BBDbT-1]!=fBBar)
 {
  RemoveToolBar(fBBar);
  AddToolBar(fBBar,B_TOP);
  UpdateLeftRight();
  UpdateView();
 }
 if (isdown && where.y>GetBottom()-20 && where.y<GetBottom() && where.x>GetLeft()+20 && where.x<GetRight()-20 && fBBar->EnabledOrientation(B_BOTTOM) && fBBB[BBDbB-1]!=fBBar)
 {
  RemoveToolBar(fBBar);
  AddToolBar(fBBar,B_BOTTOM);
  UpdateLeftRight();
  UpdateView();
 }
 if (isdown && where.x<GetLeft()+20 && where.x>GetLeft() && where.y>GetTop()+20 && where.y<GetBottom()-20 && fBBar->EnabledOrientation(B_LEFT) && fBBL[BBDbL-1]!=fBBar)
 {
  RemoveToolBar(fBBar);
  AddToolBar(fBBar,B_LEFT);
  UpdateLeftRight();
  UpdateView();
 }
 if (isdown && where.x>GetRight()-20 && where.x<GetRight() && where.y>GetTop()+20 && where.y<GetBottom()-20 && fBBar->EnabledOrientation(B_RIGHT) && fBBR[BBDbR-1]!=fBBar)
 {
  RemoveToolBar(fBBar);
  AddToolBar(fBBar,B_RIGHT);
  UpdateLeftRight();
  UpdateView();
 }
}

// --------------------------------------------------------------------------- B_Tool_View - MessageReceived -
void BToolView::MessageReceived(BMessage *msg)
{
 switch (msg->what)
 {
  case B_UPDATE_VIEW_MSG:
  {
   Update();
   UpdateView();
   UpdateLeftRight();
  }
  break;
  case B_MOUSE_DOWN_MSG:
  {
  }
  break;
  case B_MOUSE_MOVED_MSG:
  {
  }
  break;
  case B_MOUSE_UP_MSG:
  {
   isdown=false;
   fBBar=NULL;
  }
  break;
  case B_TOOL_MOVE_MSG:
  {
   BPoint where;
   uint32 button;
   if (Window())
    GetMouse(&where,&button,false);
 
   if (button==B_PRIMARY_MOUSE_BUTTON)
   {
    uint32 i;
    for (i=0;i<BBDbT;i++)
     if (fBBT[i]->IsButton(BPoint(where.x-fBBT[i]->Frame().left,where.y-fBBT[i]->Frame().top)))
     {
      isdown=true;
      fBBar=fBBT[i];
     }
    for (i=0;i<BBDbB;i++)
     if (fBBB[i]->IsButton(BPoint(where.x-fBBB[i]->Frame().left,where.y-fBBB[i]->Frame().top)))
     {
      isdown=true;
      fBBar=fBBB[i];
     }
    for (i=0;i<BBDbL;i++)
     if (fBBL[i]->IsButton(BPoint(where.x-fBBL[i]->Frame().left,where.y-fBBL[i]->Frame().top)))
     {
      isdown=true;
      fBBar=fBBL[i];
     }
    for (i=0;i<BBDbR;i++)
     if (fBBR[i]->IsButton(BPoint(where.x-fBBR[i]->Frame().left,where.y-fBBR[i]->Frame().top)))
     {
      isdown=true;
      fBBar=fBBR[i];
     }
   }   
   SetEventMask(B_POINTER_EVENTS);
  }
  break;
  default:
  {
   BView::MessageReceived(msg);
  }
  break;
 }
}

// - End - BToolView - Source File ---------------------------------------------------------------------------
