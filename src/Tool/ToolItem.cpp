//////////////////////////////////////////////////////////////////////
//     Extended BeOS Api © Gergely Rózsahegyi (RoGer) 2002-2003     //
//                           roger@beos.hu                          //
//                                                                  //
//                       ToolItem  Source File                      //
//                                                                  //
//                      Last Update: 2003.05.14                     //
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
#include <TranslationUtils.h>
#include <Window.h>
#include <stdio.h>
#include <ostream.h>

#include "ToolItem.h"

const uint32 B_UPDATE_SIZE_MSG = 'BUSM';
const uint32 B_MOUSE_DOWN_MSG = 'BMDM';
const uint32 B_MOUSE_UP_MSG = 'BMUM';
const uint32 B_ITEM_CLICK_MSG = 'BICM';
const uint32 B_RADIOITEM_CHECKED_MSG = 'BRCM';

// --------------------------------------------------------------------------------- B_Tool_Item - BToolItem -
BToolItem::BToolItem(const char *name, BMessage *msg):BView(BRect(15,5,60,45),name,B_FOLLOW_TOP|B_FOLLOW_LEFT,B_FULL_UPDATE_ON_RESIZE|B_WILL_DRAW|B_PULSE_NEEDED)
{
 SetMouseEventMask(B_LOCK_WINDOW_FOCUS);
 fMessage=msg;
 SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

 Status=0;
 isDown=false;
 IsEnabled=true;
}

// -------------------------------------------------------------------------------------- B_Tool_Item - Draw -
void BToolItem::Draw(BRect updateRect)
{
 BPoint point;
 uint32 button;
 if (Window())
  GetMouse(&point,&button,false);
 if (Enabled() && point.x>=Bounds().left && point.x<=Bounds().right && point.y>=Bounds().top && point.y<=Bounds().bottom)
 {
  if (button==B_PRIMARY_MOUSE_BUTTON && isDown)
  {
   SetHighColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR),B_LIGHTEN_2_TINT));
   StrokeLine(BPoint(1,Bounds().bottom),BPoint(Bounds().right,Bounds().bottom));
   StrokeLine(BPoint(Bounds().right,1),BPoint(Bounds().right,Bounds().bottom));
   SetHighColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR),B_DARKEN_2_TINT));
   StrokeLine(BPoint(0,0),BPoint(Bounds().right,0));
   StrokeLine(BPoint(0,0),BPoint(0,Bounds().bottom));
  } else
  {
   SetHighColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR),B_LIGHTEN_2_TINT));
   StrokeLine(BPoint(0,0),BPoint(Bounds().right,0));
   StrokeLine(BPoint(0,0),BPoint(0,Bounds().bottom));
   SetHighColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR),B_DARKEN_2_TINT));
   StrokeLine(BPoint(1,Bounds().bottom),BPoint(Bounds().right,Bounds().bottom));
   StrokeLine(BPoint(Bounds().right,1),BPoint(Bounds().right,Bounds().bottom));
  }
 } else
 {
  SetHighColor(ui_color(B_PANEL_BACKGROUND_COLOR));
  StrokeRect(Bounds());
 }
}

// --------------------------------------------------------------------------------- B_Tool_Item - MouseDown -
void BToolItem::MouseDown(BPoint where)
{
 if (Enabled() && Window()->IsActive())
 {
 BPoint point;
 uint32 button;
 if (Window())
  GetMouse(&point,&button,false);
 if (Window()->IsActive())
 {
  if (point.x>=Bounds().left && point.x<=Bounds().right && point.y>=Bounds().top && point.y<=Bounds().bottom)
  {
   if (button==B_PRIMARY_MOUSE_BUTTON)
   {
    isDown=true;
    if (Status!=1)
    {
     Draw(Bounds());
     Status=1;
    }
   } else
   {
    if (Status!=2)
    {
     Draw(Bounds());
     Status=2;
    }
   }
  } else
  {
   if (Status!=3)
   {
    Draw(Bounds());
    Status=3;
   }
  }
 }}
}

// ----------------------------------------------------------------------------------- B_Tool_Item - MouseUp -
void BToolItem::MouseUp(BPoint where)
{
 BPoint point;
 uint32 button;
 if (Window())
  GetMouse(&point,&button,false);
 if (Window()->IsActive())
 {
  if (point.x>=Bounds().left && point.x<=Bounds().right && point.y>=Bounds().top && point.y<=Bounds().bottom)
  {
   if (button==B_PRIMARY_MOUSE_BUTTON)
   {
    if (Status!=1)
    {
     Draw(Bounds());
     Status=1;
    }
   } else
   {
    if (Status!=2)
    {
     Draw(Bounds());
     Status=2;
    }
   }
  } else
  {
   if (Status!=3)
   {
    Draw(Bounds());
    Status=3;
   }
  }

  if (button!=B_PRIMARY_MOUSE_BUTTON)
  {
   if (isDown && Parent())
   {
    isDown=false;
    if (Enabled())
     MessageReceived(fMessage);
   } else
   {
    if (Parent())
     Parent()->MessageReceived(new BMessage(B_MOUSE_DOWN_MSG));
   }
  }
 } 
}

// -------------------------------------------------------------------------------- B_Tool_Item - MouseMoved -
void BToolItem::MouseMoved(BPoint where, uint32 code, const BMessage *a_message)
{
 if (Window()->IsActive())
 {
  BPoint point;
  uint32 button;
  if (Window())
   GetMouse(&point,&button,false);
  if (where.x>=Bounds().left && where.x<=Bounds().right && where.y>=Bounds().top && where.y<=Bounds().bottom)
  {
   if (button==B_PRIMARY_MOUSE_BUTTON && isDown)
   {
    if (Status!=1)
    {
     Draw(Bounds());
     Status=1;
    }
   } else
   {
    if (Status!=2)
    {
     Draw(Bounds());
     Status=2;
    }
   }
  } else
  {
   if (Status!=3)
   {
    Draw(Bounds());
    Status=3;
   }
  }
 }
}

// ----------------------------------------------------------------------------------- B_Tool_Item - Enabled -
bool BToolItem::Enabled()
{
 return IsEnabled;
}

// -------------------------------------------------------------------------------- B_Tool_Item - SetEnabled -
void BToolItem::SetEnabled(bool enabled)
{
 IsEnabled=enabled;
}

// --------------------------------------------------------------------------- B_Tool_Item - MessageReceived -
void BToolItem::MessageReceived(BMessage *msg)
{
 switch (msg->what)
 {
  case B_MOUSE_UP_MSG:
  {
   BPoint pont;
   bool gomb;
   msg->FindPoint("pont",&pont);
   msg->FindBool("gomb",&gomb);
   if (!gomb) isDown=false;
  }
  break;
  default:
  {
   BView::MessageReceived(msg);
  }
  break;
 }
}

// ------------------------------------------------------------------------------- B_Image_Item - BImageItem -
BImageItem::BImageItem(const char *name, const char *filename, BMessage *msg, bool enablecaption, const char *caption, alignment halign):BToolItem(name,msg)
{
 HAlign=halign;
 fCaption = new BString(caption);
 Enablecaption=enablecaption;
 SetImage(filename);
 SetCaption(fCaption->String());
 Enableborder=true;
}

// ------------------------------------------------------------------------------------ B_Image_Item - Image -
BBitmap *BImageItem::Image() const
{
 return fBitmap;
}

// --------------------------------------------------------------------------------- B_Image_Item - SetImage -
void BImageItem::SetImage(const char *filename)
{
 fBitmap = BTranslationUtils::GetBitmapFile(filename);
 if (fBitmap)
 {
  uint32 *bits = new uint32[fBitmap->BitsLength()/4];
  bits = (uint32*) fBitmap->Bits();

  if (fBitmap->ColorSpace()==B_RGBA32)
  {
   for (int i = 0; i < fBitmap->BitsLength()/4; i++)
   {
    if (bits[i] == 4292401368)
     bits[i] = B_TRANSPARENT_MAGIC_RGBA32;
   }
   fBitmap->SetBits(bits, fBitmap->BitsLength(), 0, B_RGBA32);
  }
 
  Update();
 } else
 {
  cout << "hiba" << endl;
 }
}

// --------------------------------------------------------------------------------- B_Image_Item - SetImage -
void BImageItem::SetImage(BBitmap *bitmap)
{
 fBitmap=bitmap;
 Update();
}

// ---------------------------------------------------------------------------------- B_Image_Item - Caption -
const char *BImageItem::Caption() const
{
 return fCaption->String();
}

// ------------------------------------------------------------------------------- B_Image_Item - SetCaption -
void BImageItem::SetCaption(const char *caption)
{
 if (fCaption)
  delete fCaption;
 fCaption = new BString(caption);
 Update();
}

// ----------------------------------------------------------------------------------- B_Image_Item - Update -
void BImageItem::Update()
{
 float width,height=0;
 if (fBitmap)
 {
  width=fBitmap->Bounds().right;
  height=height+fBitmap->Bounds().bottom;
 }
 if (Enablecaption)
 {
  if (width<StringWidth(fCaption->String())+1)
   width=StringWidth(fCaption->String())+1;
  font_height fheight;
  GetFontHeight(&fheight);
  height=height+fheight.ascent+fheight.descent+fheight.leading;
 }
 ResizeTo(width+3,height+3);
 if (Parent())
  Parent()->MessageReceived(new BMessage(B_UPDATE_SIZE_MSG)); 
}

// ------------------------------------------------------------------------------------- B_Image_Item - Draw -
void BImageItem::Draw(BRect updateRect)
{
 BPoint point;
 uint32 button;
 if (Window())
  GetMouse(&point,&button,false);
 float fontleft,bmpleft,fonttop=0;
 font_height height;
 GetFontHeight(&height);
 
 if (fBitmap)
  fonttop=fBitmap->Bounds().bottom+height.ascent+height.descent+height.leading+1;
 else
  fonttop=height.ascent+height.descent+height.leading+1;
 
 if (HAlign==B_ALIGN_CENTER)
 {
  if (fBitmap)
   bmpleft=(Bounds().right-3-fBitmap->Bounds().right) / 2 +2;
  if (Enablecaption)
   fontleft=(Bounds().right-3-StringWidth(fCaption->String())) / 2 +3;
 }
 if (HAlign==B_ALIGN_LEFT)
 {
  if (fBitmap)
   bmpleft=2;
  if (Enablecaption)
   fontleft=3;
 }
 if (HAlign==B_ALIGN_RIGHT)
 {
  if (fBitmap)
   bmpleft=Bounds().right-3-fBitmap->Bounds().right+2;
  if (Enablecaption)
   fontleft=Bounds().right-3-StringWidth(fCaption->String())+3;
 }

 if (point.x>=Bounds().left && point.x<=Bounds().right && point.y>=Bounds().top && point.y<=Bounds().bottom)
 {
  if (button)
  {
   if (fBitmap)
   {
    SetHighColor(ui_color(B_PANEL_BACKGROUND_COLOR));
    FillRect(BRect(bmpleft, 2, fBitmap->Bounds().right + bmpleft, fBitmap->Bounds().bottom + 2));
    drawing_mode drawmode = DrawingMode();
    SetDrawingMode(B_OP_ALPHA);
    DrawBitmapAsync(fBitmap,BPoint(bmpleft,2));
    SetDrawingMode(drawmode);
    
    SetHighColor(ui_color(B_PANEL_BACKGROUND_COLOR));
    StrokeLine(BPoint(bmpleft-1,1),BPoint(fBitmap->Bounds().right+bmpleft,1));
    StrokeLine(BPoint(bmpleft-1,1),BPoint(bmpleft-1,fBitmap->Bounds().bottom+2));
   }
   if (Enablecaption)
   {  
    SetHighColor(ui_color(B_PANEL_BACKGROUND_COLOR));
    DrawString(fCaption->String(),BPoint(fontleft,fonttop));
    SetHighColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR), B_DARKEN_MAX_TINT));
    DrawString(fCaption->String(),BPoint(fontleft-1,fonttop-1));
   }
  } else
  {
   if (fBitmap)
   {
    SetHighColor(ui_color(B_PANEL_BACKGROUND_COLOR));
    FillRect(BRect(bmpleft, 2, fBitmap->Bounds().right + bmpleft - 1, fBitmap->Bounds().bottom + 1));
    drawing_mode drawmode = DrawingMode();
    SetDrawingMode(B_OP_ALPHA);
    DrawBitmapAsync(fBitmap,BPoint(bmpleft-1,1));
    SetDrawingMode(drawmode);
    SetHighColor(ui_color(B_PANEL_BACKGROUND_COLOR));
    StrokeLine(BPoint(fBitmap->Bounds().right+bmpleft,1),BPoint(fBitmap->Bounds().right+bmpleft,fBitmap->Bounds().bottom+2));
    StrokeLine(BPoint(bmpleft-1,fBitmap->Bounds().bottom+2),BPoint(fBitmap->Bounds().right+bmpleft,fBitmap->Bounds().bottom+2));
   }
   if (Enablecaption)
   {  
    SetHighColor(ui_color(B_PANEL_BACKGROUND_COLOR));
    DrawString(fCaption->String(),BPoint(fontleft-1,fonttop-1));
    SetHighColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR), B_DARKEN_MAX_TINT));
    DrawString(fCaption->String(),BPoint(fontleft,fonttop));
   }
  }
 } else
 {
  if (fBitmap)
  {
   SetHighColor(ui_color(B_PANEL_BACKGROUND_COLOR));
   FillRect(BRect(bmpleft, 2, fBitmap->Bounds().right + bmpleft - 1, fBitmap->Bounds().bottom + 1));
   drawing_mode drawmode = DrawingMode();
   SetDrawingMode(B_OP_ALPHA);
   DrawBitmapAsync(fBitmap,BPoint(bmpleft-1,1));
   SetDrawingMode(drawmode);
   SetHighColor(ui_color(B_PANEL_BACKGROUND_COLOR));
   StrokeLine(BPoint(fBitmap->Bounds().right+bmpleft,1),BPoint(fBitmap->Bounds().right+bmpleft,fBitmap->Bounds().bottom+2));
   StrokeLine(BPoint(bmpleft-1,fBitmap->Bounds().bottom+2),BPoint(fBitmap->Bounds().right+bmpleft,fBitmap->Bounds().bottom+2));
  }
  if (Enablecaption)
  {  
   SetHighColor(ui_color(B_PANEL_BACKGROUND_COLOR));
   DrawString(fCaption->String(),BPoint(fontleft-1,fonttop-1));
   SetHighColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR), B_DARKEN_MAX_TINT));
   DrawString(fCaption->String(),BPoint(fontleft,fonttop));
  }
 }
 if (Enableborder)
  BToolItem::Draw(updateRect);
}

// -------------------------------------------------------------------------- B_Image_Item - SetEnableBorder -
void BImageItem::SetEnableBorder(bool enableborder)
{
 Enableborder=enableborder;
}

// -------------------------------------------------------------------------- B_Image_Item - MessageReceived -
void BImageItem::MessageReceived(BMessage *msg)
{
 switch (msg->what)
 {
  case '0':
  {
  }
  break;
  default:
  {
   BToolItem::MessageReceived(msg);
  }
  break;
 }
}

// ------------------------------------------------------------------------------- B_Radio_Item - BRadioItem -
BRadioItem::BRadioItem(const char *name, const char *filename, BMessage *msg):BToolItem(name,msg)
{
 SetImage(filename);
 Checked=false;
 Firstitem=false;
 fMessage=msg;
}

// ------------------------------------------------------------------------------------ B_Radio_Item - Image -
BBitmap *BRadioItem::Image() const
{
 return fBitmap;
}

// --------------------------------------------------------------------------------- B_Radio_Item - SetImage -
void BRadioItem::SetImage(const char *filename)
{
 fBitmap = BTranslationUtils::GetBitmapFile(filename);
 if (fBitmap)
 {
  uint32 *bits = new uint32[fBitmap->BitsLength()/4];
  bits = (uint32*) fBitmap->Bits();

  if (fBitmap->ColorSpace()==B_RGBA32)
  {
   for (int i = 0; i < fBitmap->BitsLength()/4; i++)
   {
    if (bits[i] == 4292401368)
     bits[i] = B_TRANSPARENT_MAGIC_RGBA32;
   }
   fBitmap->SetBits(bits, fBitmap->BitsLength(), 0, B_RGBA32);
  }
 
  Update();
 } else
 {
  cout << "hiba" << endl;
 }
}

// ---------------------------------------------------------------------------------- B_Radio_Item - MouseUp -
void BRadioItem::MouseUp(BPoint where)
{
 if (Window()->IsActive())
 {
  BPoint point;
  uint32 button;
  if (Window())
   GetMouse(&point,&button,false);
  if (button!=B_PRIMARY_MOUSE_BUTTON)
  {
   if (isDown && Parent())
   {
    isDown=false;
    BMessage *msg=new BMessage(B_ITEM_CLICK_MSG);
    msg->AddInt32("fNum",fNum);
    if (Parent())
    {
     MessageReceived(msg);
     MessageReceived(fMessage);
    }
   }
  }
 }
// BToolItem::MouseUp(where);
}

// ----------------------------------------------------------------------------------- B_Radio_Item - Update -
void BRadioItem::Update()
{
 float width=0;
 float height=0;
 if (fBitmap)
 {
  width=fBitmap->Bounds().right;
  height=height+fBitmap->Bounds().bottom;
 }
 if (Firstitem)
  width--;
 ResizeTo(width+3,height+3);
 if (Parent())
  Parent()->MessageReceived(new BMessage(B_UPDATE_SIZE_MSG)); 
}

// ------------------------------------------------------------------------------- B_Radio_Item - SetChecked -
void BRadioItem::SetChecked(bool checked)
{
 Checked=checked;
 Draw(Bounds());
}

// ----------------------------------------------------------------------------- B_Radio_Item - SetFirstItem -
void BRadioItem::SetFirstItem(bool firstitem)
{
 Firstitem=firstitem;
}

// ----------------------------------------------------------------------------------- B_Radio_Item - SetNum -
void BRadioItem::SetNum(uint32 fnum)
{
 fNum=fnum;
}

// ------------------------------------------------------------------------------------- B_Radio_Item - Draw -
void BRadioItem::Draw(BRect updateRect)
{
 if (fBitmap)
 {
  SetHighColor(ui_color(B_PANEL_BACKGROUND_COLOR));
  FillRect(BRect(2, 2, fBitmap->Bounds().right + 2, fBitmap->Bounds().bottom + 2));
  drawing_mode drawmode = DrawingMode();
  SetDrawingMode(B_OP_ALPHA);
  DrawBitmapAsync(fBitmap,BPoint(2,2));
  SetDrawingMode(drawmode);
 }
 if (Checked)
 {
  SetHighColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR),B_LIGHTEN_2_TINT));
  StrokeLine(BPoint(0,0),BPoint(Bounds().right,0));//
  if (!Firstitem) 
   StrokeLine(BPoint(0,0),BPoint(0,Bounds().bottom-2));
  
  SetHighColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR),B_DARKEN_1_TINT));
  StrokeLine(BPoint(Bounds().right,1),BPoint(Bounds().right,Bounds().bottom-1));
  SetHighColor(ui_color(B_PANEL_BACKGROUND_COLOR));
  StrokeLine(BPoint(0,Bounds().bottom-1),BPoint(Bounds().right,Bounds().bottom-1));
  StrokeLine(BPoint(0,Bounds().bottom),BPoint(Bounds().right,Bounds().bottom));
 } else
 {
  SetHighColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR),B_LIGHTEN_2_TINT));
  StrokeLine(BPoint(0,0),BPoint(Bounds().right,0));//
  if (!Firstitem) 
   StrokeLine(BPoint(0,0),BPoint(0,Bounds().bottom-2));
  StrokeLine(BPoint(0,Bounds().bottom),BPoint(Bounds().right,Bounds().bottom));
  
  SetHighColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR),B_DARKEN_1_TINT));
  StrokeLine(BPoint(0,Bounds().bottom-1),BPoint(Bounds().right,Bounds().bottom-1));
  StrokeLine(BPoint(Bounds().right,1),BPoint(Bounds().right,Bounds().bottom-1));
 
 }
}

// -------------------------------------------------------------------------- B_Radio_Item - MessageReceived -
void BRadioItem::MessageReceived(BMessage *msg)
{
 switch (msg->what)
 {
  case B_RADIOITEM_CHECKED_MSG:
  {
   bool checked;
   msg->FindBool("Checked",&checked);
   SetChecked(checked);
  }
  break;
  default:
  {
   BToolItem::MessageReceived(msg);
  }
  break;
 }
}

// ------------------------------------------------------------------------------- B_Check_Item - BCheckItem -
BCheckItem::BCheckItem(const char *name, const char *filename1, const char *filename2, BMessage *msg, bool enablecaption, const char *caption, alignment halign):BImageItem(name,filename1,msg,enablecaption,caption,halign)
{
 fEnableBitmap = BTranslationUtils::GetBitmapFile(filename1);
 fDisableBitmap = BTranslationUtils::GetBitmapFile(filename2);
}

// ------------------------------------------------------------------------------- B_Check_Item - SetEnabled -
void BCheckItem::SetEnabled(bool enabled)
{
 BToolItem::SetEnabled(enabled);
 if (enabled)
  SetImage(fEnableBitmap);
 else
  SetImage(fDisableBitmap);
 Update();
 Draw(Bounds());
}

// - End - ToolItem - Source File ----------------------------------------------------------------------------
