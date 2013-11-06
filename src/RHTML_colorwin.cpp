//////////////////////////////////////////////////////////////////////
//      Globe Webeditor © Gergely Rózsahegyi (RoGer) 2001-2003      //
//                                                                  //
//                        ColorWin  Source File                     //
//                                                                  //
//                       Last Update: 2004.10.21                    //
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
#include "RHTML_colorwin.h"
#include <Screen.h>
#include <ScrollView.h>

#include <stdio.h>

struct colortype htmlcolors[htmlcolorcount] = 
{
 BString("aqua"),{0,255,255},
 BString("black"),{0,0,0},
 BString("blue"),{0,0,255},
 BString("fuchsia"),{255,0,255},
 BString("gray"),{128,128,128},
 BString("green"),{0,128,0},
 BString("lime"),{0,255,0},
 BString("maroon"),{128,0,0},
 BString("navy"),{0,0,128},
 BString("olive"),{128,128,0},
 BString("purple"),{128,0,128},
 BString("red"),{255,0,0},
 BString("silver"),{192,192,192},
 BString("teal"),{0,128,128},
 BString("white"),{255,255,255},
 BString("yellow"),{255,255,0}
};

// -------------------------------------------------------------------------- RHTML_colorwin - RHTMLcolorwin -
RHTMLcolorwin::RHTMLcolorwin(): BWindow(BRect(-359,-123,-10,-10),"RHTMLColorWindow",B_NO_BORDER_WINDOW_LOOK,B_FLOATING_APP_WINDOW_FEEL,B_AVOID_FRONT)
{
 fBox = new RHTMLcolorview(BRect(0,0,Bounds().right+1,Bounds().bottom+1));
 AddChild(fBox);
 
 BBox *fColorBox = new BBox(BRect(79,61,180,81));
 fBox->AddChild(fColorBox);

 fColorView = new BView(BRect(2,2,fColorBox->Bounds().right-2,fColorBox->Bounds().bottom-2),"colorview",B_FOLLOW_NONE,B_WILL_DRAW|B_WILL_DRAW);
 fColorBox->AddChild(fColorView);
 
 fColorControl = new BColorControl(BPoint(79,5),B_CELLS_32x8,1,"colorcontrol",new BMessage('COLR'));
 fBox->AddChild(fColorControl);
 
 fListView = new BListView(BRect(6,6,60,108),"listview",B_SINGLE_SELECTION_LIST);
 BScrollView *fScrollView = new BScrollView("listviewscrollview",fListView,B_FOLLOW_ALL,B_WILL_DRAW, false, true,B_PLAIN_BORDER);
 fBox->AddChild(fScrollView);
 fListView->TargetedByScrollView(fScrollView);

 fListView->SetSelectionMessage(new BMessage('LIST'));
 fListView->AddItem(new BStringItem("#Custom"));
 for (int i=0;i<htmlcolorcount;i++)
  fListView->AddItem(new BStringItem(htmlcolors[i].name.String()));
 
 fTextControl= new BTextControl(BRect(200,62,345,82),"hexatext",fOptions->GetLocaleString("Color_Window","Color in Hexa:"),"#000000",new BMessage('HEXA'));
 fBox->AddChild(fTextControl);
 fTextControl->SetDivider(80);

 fCancelButton = new BButton(BRect(78,84,148,100),"cancel",fOptions->GetLocaleString("Color_Window","Cancel"),new BMessage('CNCL'));
 fBox->AddChild(fCancelButton);

 fOkButton = new BButton(BRect(276,84,346,100),"cancel",fOptions->GetLocaleString("Color_Window","Ok"),new BMessage('OK'));
 fBox->AddChild(fOkButton);
}

// -----------------------------------------------------------------------------------------------------------
RHTMLcolorwin::~RHTMLcolorwin()
{
}

// ---------------------------------------------------------------------------------- RHTML_colorwin - Start -
void RHTMLcolorwin::Start(BPoint pos, RHTMLplistitem *item, BString value)
{
 fValue = value;
 fItem = item;
 fTextView = NULL;
 
 Lock();
 fTextControl->SetText(RGBToBString(BStringToRGB(fValue)).String());
 fColorControl->SetValue(BStringToRGB(fValue));
 fListView->Select(0);
 for (int i=0;i<htmlcolorcount;i++)
 {
  if (htmlcolors[i].name == fValue)
  fListView->Select(i + 1);
 }
 fColorView->SetViewColor(BStringToRGB(fValue));
 fColorView->Hide();
 fColorView->Show(); 
   
 SetFeel(B_MODAL_APP_WINDOW_FEEL);
 Activate(true);
 fTextControl->MakeFocus(true);
 fBox->SetEventMask(B_POINTER_EVENTS);

 float left,top;
 BScreen screen;

 if (screen.Frame().bottom > pos.y + Bounds().bottom+1)
  top = pos.y-1;
 else
  top = screen.Frame().bottom - Bounds().bottom;

 if (screen.Frame().right > pos.x + Bounds().right + 1)
  left = pos.x + 1;
 else
 {
  left = screen.Frame().right - Bounds().right;
  if (screen.Frame().bottom <= pos.y + Bounds().bottom + 1)
  top = pos.y - Bounds().bottom;
 }

 MoveTo(left,top);
 Unlock();
}

// ---------------------------------------------------------------------------------- RHTML_colorwin - Start -
void RHTMLcolorwin::Start(BPoint pos, BTextView *text)
{
 fValue = "";
 fItem = NULL;
 fTextView = text;
 Lock();

 fTextControl->SetText(RGBToBString(BStringToRGB(fValue)).String());
 fColorControl->SetValue(BStringToRGB(fValue));
 fListView->Select(0);
 for (int i=0;i<htmlcolorcount;i++)
 {
  if (htmlcolors[i].name == fValue)
  fListView->Select(i + 1);
 }
 fColorView->SetViewColor(BStringToRGB(fValue));
 fColorView->Hide();
 fColorView->Show(); 
   
 SetFeel(B_MODAL_APP_WINDOW_FEEL);
 Activate(true);
 fTextControl->MakeFocus(true);
 fBox->SetEventMask(B_POINTER_EVENTS);

 float left,top;
 BScreen screen;

 if (screen.Frame().bottom > pos.y + Bounds().bottom+1)
  top = pos.y-1;
 else
  top = screen.Frame().bottom - Bounds().bottom;

 if (screen.Frame().right > pos.x + Bounds().right + 1)
  left = pos.x + 1;
 else
 {
  left = screen.Frame().right - Bounds().right;
  if (screen.Frame().bottom <= pos.y + Bounds().bottom + 1)
  top = pos.y - Bounds().bottom;
 }

 MoveTo(left,top);
 Unlock();
}

// ----------------------------------------------------------------------------------- RHTML_colorwin - Stop -
void RHTMLcolorwin::Stop()
{
 Lock();
 fBox->SetEventMask(B_NO_POINTER_HISTORY);
 MoveTo(-Bounds().right-5,-Bounds().bottom-5);
 SetFeel(B_FLOATING_APP_WINDOW_FEEL);
 Activate(false);
 Unlock();
}

// ------------------------------------------------------------------------ RHTML_colorwin - MessageReceived -
void RHTMLcolorwin::MessageReceived(BMessage *msg)
{
 switch (msg->what)
 {
  case 'STOP':
  {
   Stop();
  }
  break;
  case 'LIST':
  {
   if (fListView->CurrentSelection()>0)
   {
    fValue = htmlcolors[fListView->CurrentSelection()-1].name;
    fTextControl->SetText(RGBToBString(BStringToRGB(fValue)).String());
    fColorControl->SetValue(BStringToRGB(fValue));
    fColorView->SetViewColor(BStringToRGB(fValue));
    fColorView->Hide();
    fColorView->Show();
   }
  }
  break;
  case 'COLR':
  {
   fValue = RGBToBString(fColorControl->ValueAsColor());
   fTextControl->SetText(fValue.String());
   fListView->Select(0);
   fColorView->SetViewColor(BStringToRGB(fValue));
   fColorView->Hide();
   fColorView->Show();
  }
  break;
  case 'HEXA':
  {
   fTextControl->SetText(RGBToBString(BStringToRGB(BString(fTextControl->Text()))).String());
   fValue = fTextControl->Text();
   fColorControl->SetValue(BStringToRGB(fValue));
   fListView->Select(0);
   fColorView->SetViewColor(BStringToRGB(fValue));
   fColorView->Hide();
   fColorView->Show();
  }
  break;
  case 'CNCL':
  {
   Stop();
  }
  break;
  case 'OK':
  {
   Stop();
   if (fItem)
    fItem->ModifyValue(fValue);
   else
    if (fTextView)
    {
     if (!fTextView->Window()->IsLocked())
     {
      fTextView->Window()->Lock();
      fTextView->Insert(fValue.String());
      fTextView->MessageReceived(new BMessage('UPPM'));
      fTextView->MessageReceived(new BMessage('CWFC'));
      fTextView->Window()->Unlock();
     } else
     {
      fTextView->Insert(fValue.String());
      fTextView->MessageReceived(new BMessage('UPPM'));
      fTextView->MessageReceived(new BMessage('CWFC'));
     }
    }
  }
  break;
  default:
  {
   BWindow::MessageReceived(msg);
  }
  break;
 }
}

// ------------------------------------------------------------------------ RHTML_colorview - RHTMLcolorview -
RHTMLcolorview::RHTMLcolorview(BRect rect): BBox(rect)
{

}

// ----------------------------------------------------------------------------- RHTML_colorview - MouseDown -
void RHTMLcolorview::MouseDown(BPoint where)
{
 if (where.x<Bounds().left || where.x>Bounds().right || where.y<Bounds().top || where.y>Bounds().bottom)
 {
  MessageReceived(new BMessage('STOP'));
 } else
 BBox::MouseDown(where);
}

// - End - ColorWin - Source File ----------------------------------------------------------------------------
