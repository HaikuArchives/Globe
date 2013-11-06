//////////////////////////////////////////////////////////////////////
//       Extended BeOS Api © Gergely Rózsahegyi (RoGer) 2002        //
//                          roger@beos.hu                           //
//                                                                  //
//                      BToolBar  Source File                       //
//                                                                  //
//                     Last Update: 2002.06.11                      //
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
#include "ToolBar.h"

#include <stdlib.h>
#include <stdio.h>

const uint32 B_ORI_T_MSG = 'BOTM';
const uint32 B_ORI_B_MSG = 'BOBM';
const uint32 B_ORI_L_MSG = 'BOLM';
const uint32 B_ORI_R_MSG = 'BORM';
const uint32 B_UPDATE_VIEW_MSG = 'BUVM';
const uint32 B_UPDATE_SIZE_MSG = 'BUSM';
const uint32 B_MOUSE_DOWN_MSG = 'BMDM';
const uint32 B_MOUSE_UP_MSG = 'BMUM';
const uint32 B_MOUSE_MOVED_MSG = 'BMMM';
const uint32 B_TOOL_MOVE_MSG = 'BTMM';

// ----------------------------------------------------------------------------------- B_Tool_Bar - BToolBar -
BToolBar::BToolBar(const char *name, BRect hmargin, BRect vmargin, float hspace, float vspace, bool autoresize, float hwidth, float vheight):BView(BRect(0,0,hwidth,vheight),name,B_FOLLOW_TOP|B_FOLLOW_LEFT_RIGHT,B_FULL_UPDATE_ON_RESIZE|B_WILL_DRAW|B_PULSE_NEEDED)
{
 SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
 
 HMargin=hmargin;
 VMargin=vmargin;
 HSpace=hspace;
 VSpace=vspace;
 Width=hwidth;
 Height=vheight;
 Autoresize=autoresize;
 Top=Bottom=Left=Right=true;
 TIDb=0;
 fToolItem = (BToolItem**) calloc(1,sizeof(BToolItem*));
 Borderstyle=B_NORMAL;
 SetOrientation(B_TOP);
 ACenter=false;
}

// --------------------------------------------------------------------------------------- B_Tool_Bar - Draw -
void BToolBar::Draw(BRect updateRect)
{
 if (Borderstyle==B_NORMAL)
 {
  if ((Ori==B_LEFT ||Ori==B_RIGHT))
  {
   SetHighColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR),B_DARKEN_2_TINT));
   StrokeLine(BPoint(3,2),BPoint(Bounds().right-3,2));
   StrokeLine(BPoint(3,5),BPoint(Bounds().right-3,5));
   SetHighColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR),B_LIGHTEN_2_TINT));
   StrokeLine(BPoint(3,3),BPoint(Bounds().right-3,3));
   StrokeLine(BPoint(3,6),BPoint(Bounds().right-3,6));
 
   SetHighColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR),B_LIGHTEN_2_TINT));
   StrokeLine(BPoint(0,0),BPoint(0,Bounds().bottom-1));
   StrokeLine(BPoint(1,0),BPoint(Bounds().right-1,0));
   SetHighColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR),B_DARKEN_1_TINT));
   StrokeLine(BPoint(0,Bounds().bottom),BPoint(Bounds().right-1,Bounds().bottom));
   StrokeLine(BPoint(Bounds().right,0),BPoint(Bounds().right,Bounds().bottom));
  }
  if ((Ori==B_TOP ||Ori==B_BOTTOM))
  {
   SetHighColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR),B_DARKEN_2_TINT));
   StrokeLine(BPoint(2,3),BPoint(2,Bounds().bottom-3));
   StrokeLine(BPoint(5,3),BPoint(5,Bounds().bottom-3));
   SetHighColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR),B_LIGHTEN_2_TINT));
   StrokeLine(BPoint(3,3),BPoint(3,Bounds().bottom-3));
   StrokeLine(BPoint(6,3),BPoint(6,Bounds().bottom-3));
 
   SetHighColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR),B_LIGHTEN_2_TINT));
   StrokeLine(BPoint(0,0),BPoint(Bounds().right-1,0));
   StrokeLine(BPoint(0,0),BPoint(0,Bounds().bottom-1));
   SetHighColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR),B_DARKEN_1_TINT));
   StrokeLine(BPoint(0,Bounds().bottom),BPoint(Bounds().right,Bounds().bottom));
//   StrokeLine(BPoint(Bounds().right,0),BPoint(Bounds().right,Bounds().bottom));
  }
 } else
 if (Borderstyle==B_SUB)
 {
  if ((Ori==B_LEFT ||Ori==B_RIGHT))
  {
   if (Moveable)
   {
    SetHighColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR),B_DARKEN_2_TINT));
    StrokeLine(BPoint(3,2),BPoint(Bounds().right-3,2));
    StrokeLine(BPoint(3,5),BPoint(Bounds().right-3,5));
    SetHighColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR),B_LIGHTEN_2_TINT));
    StrokeLine(BPoint(3,3),BPoint(Bounds().right-3,3));
    StrokeLine(BPoint(3,6),BPoint(Bounds().right-3,6));
   }
 
   SetHighColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR),B_LIGHTEN_2_TINT));
   StrokeLine(BPoint(0,0),BPoint(0,Bounds().bottom-1));
   StrokeLine(BPoint(1,0),BPoint(Bounds().right-1,0));
   SetHighColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR),B_DARKEN_1_TINT));
   StrokeLine(BPoint(0,Bounds().bottom),BPoint(Bounds().right-1,Bounds().bottom));
   StrokeLine(BPoint(Bounds().right,0),BPoint(Bounds().right,Bounds().bottom));
  }
  if ((Ori==B_TOP ||Ori==B_BOTTOM))
  {
   if (Moveable)
   {
    SetHighColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR),B_DARKEN_2_TINT));
    StrokeLine(BPoint(2,3),BPoint(2,Bounds().bottom-3));
    StrokeLine(BPoint(5,3),BPoint(5,Bounds().bottom-3));
    SetHighColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR),B_LIGHTEN_2_TINT));
    StrokeLine(BPoint(3,3),BPoint(3,Bounds().bottom-3));
    StrokeLine(BPoint(6,3),BPoint(6,Bounds().bottom-3));
   }
 
   SetHighColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR),B_LIGHTEN_2_TINT));
//   StrokeLine(BPoint(0,0),BPoint(Bounds().right-1,0));
//   StrokeLine(BPoint(0,0),BPoint(0,Bounds().bottom-1));
   StrokeLine(BPoint(0,Bounds().bottom),BPoint(Bounds().right,Bounds().bottom));
   SetHighColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR),B_DARKEN_1_TINT));
   StrokeLine(BPoint(0,Bounds().bottom-1),BPoint(Bounds().right,Bounds().bottom-1));
//   StrokeLine(BPoint(Bounds().right,0),BPoint(Bounds().right,Bounds().bottom));
  }
 }
}

// ---------------------------------------------------------------------------------- B_Tool_Bar - MouseDown -
void BToolBar::MouseDown(BPoint where)
{
 if (IsButton(where) && Parent())
  Parent()->MessageReceived(new BMessage(B_TOOL_MOVE_MSG));
}

// ---------------------------------------------------------------------- B_Tool_Bar - SetEnabledOrientation -
void BToolBar::SetEnabledOrientation(bool top, bool bottom, bool left, bool right)
{
 Top=top;
 Bottom=bottom;
 Left=left;
 Right=right;
}

// ------------------------------------------------------------------------- B_Tool_Bar - EnabledOrientation -
bool BToolBar::EnabledOrientation(orient ori)
{
 if (ori==B_TOP) return Top;
 if (ori==B_BOTTOM) return Bottom;
 if (ori==B_LEFT) return Left;
 if (ori==B_RIGHT) return Right;
 return true;
}

// -------------------------------------------------------------------------------- B_Tool_Bar - Orientation -
orient BToolBar::Orientation()
{
 return Ori; 
}

// -------------------------------------------------------------------------------- B_Tool_Bar - SetOrientation -
void BToolBar::SetOrientation(orient ori)
{
 Ori=ori;
 if (Ori==B_LEFT)
 {
  SetResizingMode(B_FOLLOW_TOP_BOTTOM);
 }
 if (Ori==B_RIGHT)
 {
  SetResizingMode(B_FOLLOW_TOP_BOTTOM|B_FOLLOW_RIGHT);
 }
 if (Ori==B_TOP)
 {
  SetResizingMode(B_FOLLOW_LEFT_RIGHT);
 }
 if (Ori==B_BOTTOM)
 {
  SetResizingMode(B_FOLLOW_LEFT_RIGHT|B_FOLLOW_BOTTOM);
 }
 Update();
 Draw(Bounds());
}

// --------------------------------------------------------------------------------- B_Tool_Bar - AutoResize -
bool BToolBar::AutoResize()
{
 return Autoresize;
}

// ------------------------------------------------------------------------------ B_Tool_Bar - SetAutoResize -
void BToolBar::SetAutoResize(bool autoresize)
{
 Autoresize=autoresize;
 Update();
}

// --------------------------------------------------------------------------- B_Tool_Bar - HorizontalHeight -
float BToolBar::HorizontalHeight()
{
 return Height;
}

// ------------------------------------------------------------------------ B_Tool_Bar - SetHorizontalHeight -
void BToolBar::SetHorizontalHeight(float height)
{
 Height=height;
 Update();
}

// ------------------------------------------------------------------------------ B_Tool_Bar - VerticalWidth -
float BToolBar::VerticalWidth()
{
 return Width;
}

// --------------------------------------------------------------------------- B_Tool_Bar - SetVerticalWidth -
void BToolBar::SetVerticalWidth(float width)
{
 Width=width;
 Update();
}

// -------------------------------------------------------------------------------- B_Tool_Bar - AddToolItem -
uint32 BToolBar::AddToolItem(BToolItem *fTool)
{
 float oldleft=GetLeft();
 float oldtop=GetTop();
 TIDb++;
 fToolItem = (BToolItem**) realloc(fToolItem,sizeof(BToolItem*)*TIDb);
 fToolItem[TIDb-1]=fTool;
 AddChild(fTool);
  if (Ori==B_TOP || Ori==B_BOTTOM)
 {
  fTool->MoveTo(oldleft,HMargin.top); 
 }
  if (Ori==B_LEFT || Ori==B_RIGHT)
 {
  fTool->MoveTo(VMargin.left,oldtop);
 }
 Update();
 return(TIDb-1);
}

// ----------------------------------------------------------------------------- B_Tool_Bar - RemoveToolItem -
void BToolBar::RemoveToolItem(BToolItem *fTool)
{
 uint32 i;
 for (i=0;i<TIDb;i++)
 {
  if (fToolItem[i]==fTool)
  {
   TIDb--;
   for (uint32 o=i;o<TIDb;o++)
   {
    float oldleft=fToolItem[o]->Frame().right+HSpace+1;
    float oldtop=fToolItem[o]->Frame().bottom+VSpace+1;
    fToolItem[o]=fToolItem[o+1];
    if (Ori==B_TOP || Ori==B_BOTTOM)
     fToolItem[o]->MoveTo(oldleft,HMargin.top);
    if (Ori==B_LEFT || Ori==B_RIGHT)
     fToolItem[o]->MoveTo(VMargin.left,oldtop);
   }
    RemoveChild(fTool);
    fToolItem = (BToolItem**) realloc(fToolItem,sizeof(BToolItem*)*TIDb);
    Update();
  }
 }
}

// ------------------------------------------------------------------------------------- B_Tool_Bar - Update -
void BToolBar::Update()
{
 float max=0;
 float tmp=0;
 if ((Ori==B_TOP || Ori==B_BOTTOM) && TIDb>0)
 {
  tmp=HMargin.left;
  for (uint32 i=0;i<TIDb;i++)
  {
   if (ACenter)
    fToolItem[i]->MoveTo(tmp,HMargin.top+(Bounds().bottom-HMargin.bottom-HMargin.top-fToolItem[i]->Bounds().bottom)/2);
   else
    fToolItem[i]->MoveTo(tmp,HMargin.top);
   tmp=fToolItem[i]->Frame().right+HSpace+1;
   if (fToolItem[i]->Bounds().bottom>max) max=fToolItem[i]->Bounds().bottom;
  }
  if (Autoresize)
  {
   ResizeTo(Bounds().right,max+HMargin.top+HMargin.bottom);
   Height=max+HMargin.top+HMargin.bottom;
  }
 }
 if ((Ori==B_LEFT || Ori==B_RIGHT) && TIDb>0)
 {
  tmp=VMargin.top;
  for (uint32 i=0;i<TIDb;i++)
  {
   fToolItem[i]->MoveTo(VMargin.left,tmp);
   tmp=fToolItem[i]->Frame().bottom+VSpace+1;
   if (fToolItem[i]->Bounds().right>max) max=fToolItem[i]->Bounds().right;
  }
  if (Autoresize)
  {
   ResizeTo(max+VMargin.left+VMargin.right,Bounds().bottom);
   Width=max+VMargin.left+VMargin.right;
  }
 }
 if (!Autoresize)
 {
  if (Ori==B_LEFT || Ori==B_RIGHT)
   ResizeTo(Width,Bounds().bottom);
  if (Ori==B_TOP || Ori==B_BOTTOM)
  {
   ResizeTo(Bounds().right,Height);
  } 
 }
 if (Parent())
  Parent()->MessageReceived(new BMessage(B_UPDATE_VIEW_MSG));
}

// ------------------------------------------------------------------------ B_Tool_Bar - SetHorizontalMargin -
void BToolBar::SetHorizontalMargin(BRect margin)
{
 HMargin=margin;
}

// --------------------------------------------------------------------------- B_Tool_Bar - HorizontalMargin -
BRect BToolBar::HorizontalMargin()
{
 return HMargin;
}

// ------------------------------------------------------------------------------------ B_Tool_Bar - GetLeft -
float BToolBar::GetLeft()
{
 if (TIDb>0)
  return fToolItem[TIDb-1]->Frame().right+HSpace+1;
 else
  return HMargin.left;
}

// ------------------------------------------------------------------------------------ B_Tool_Bar - GetLeft -
float BToolBar::GetTop()
{
 if (TIDb>0)
  return fToolItem[TIDb-1]->Frame().bottom+1;
 else
  return 10;
}
// ----------------------------------------------------------------------------------- B_Tool_Bar - IsButton -
bool BToolBar::IsButton(BPoint point)
{
 if ((Ori==B_LEFT ||Ori==B_RIGHT) && point.y<8 && point.y>=0 && point.x>=0 && point.x<=Bounds().right) return true;
 if ((Ori==B_TOP ||Ori==B_BOTTOM) && point.x<8 && point.x>=0 && point.y>=0 && point.y<=Bounds().bottom) return true;
 return false;
}

void BToolBar::HideAll()
{
 for (uint32 i=0;i<TIDb;i++)
  fToolItem[i]->Hide();
}
void BToolBar::ShowAll()
{
 for (uint32 i=0;i<TIDb;i++)
  fToolItem[i]->Show();
}

// ----------------------------------------------------------------------------- B_Tool_Bar - SetBorderStyle -
void BToolBar::SetBorderStyle(borderstyle bst)
{
 Borderstyle=bst;
}

// -------------------------------------------------------------------------------- B_Tool_Bar - SetMoveable -
void BToolBar::SetMoveable(bool moveable)
{
 Moveable=moveable;
}

// ----------------------------------------------------------------------------- B_Tool_Bar - SetAlignCenter -
void BToolBar::SetAlignCenter(bool acenter)
{
 ACenter=acenter;
 Update();
}


// ------------------------------------------------------------------------------------ B_Tool_Bar - GetLast -
float BToolBar::GetLast()
{
 if (Ori==B_TOP || Ori==B_BOTTOM)
  if (TIDb>0) return fToolItem[TIDb-1]->Frame().right+1;
 if (Ori==B_LEFT || Ori==B_RIGHT)
  if (TIDb>0) return fToolItem[TIDb-1]->Frame().bottom+1;
 return 0;
}

// ----------------------------------------------------------------------------------- B_Tool_Bar - ToolItem -
BToolItem* BToolBar::ToolItem(uint32 num)
{
 return fToolItem[num];
}

// ------------------------------------------------------------------------------- B_Tool_Bar - FindToolItem -
BToolItem* BToolBar::FindToolItem(const char *name)
{
 for (uint32 i=0;i<TIDb;i++)
  if (strcmp(fToolItem[i]->Name(),name)==0)
   return fToolItem[i];
 return NULL;
}

// ------------------------------------------------------------------------------ B_Tool_Bar - ToolItemCount -
uint32 BToolBar::ToolItemCount()
{
 return TIDb;
}

// ----------------------------------------------------------------------------------- B_Tool_Bar - SetIndex -
void BToolBar::SetIndex(uint32 index)
{
}


// ---------------------------------------------------------------------------- B_Tool_Bar - MessageReceived -
void BToolBar::MessageReceived(BMessage *msg)
{
 switch (msg->what)
 {
  case B_ORI_T_MSG:
  {
   SetOrientation(B_TOP);
  }
  break;
  case B_ORI_B_MSG:
  {
   SetOrientation(B_BOTTOM);
  }
  break;
  case B_ORI_L_MSG:
  {
   SetOrientation(B_LEFT);
  }
  break;
  case B_ORI_R_MSG:
  {
   SetOrientation(B_RIGHT);
  }
  break;
  case B_UPDATE_SIZE_MSG:
  {
   Update();
  }
  break;
  case B_MOUSE_UP_MSG:
  {
   if (Parent())
    Parent()->MessageReceived(msg);
  }
  break;
  default:
  {
   BView::MessageReceived(msg);
  }
  break;
 }
}

// - End - BToolBar - Source File ----------------------------------------------------------------------------
