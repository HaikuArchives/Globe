//////////////////////////////////////////////////////////////////////
//      Globe Webeditor © Gergely Rózsahegyi (RoGer) 2001-2003      //
//                                                                  //
//                        ParamMenu Source File                     //
//                                                                  //
//                       Last Update: 2003.06.01                    //
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
#include "RHTML_parammenu.h"
#include "RHTML_colorwin.h"
#include "RHTML_options.h"
#include "RadioBar.h"
#include <Button.h>
#include <Clipboard.h>
#include <StringView.h>
#include <stdio.h>
#include <math.h>

// ------------------------------------------------------------------------ RHTML_parammenu - RHTMLparammenu -
RHTMLparammenu::RHTMLparammenu(BRect fRect,RHTMLWin *fwin): BView(fRect,"Search",B_FOLLOW_ALL,B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE),fWin(fwin)
{
 SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
 fParamList = new RHTMLplistview(BRect(1,22,100,100), fwin);

 fParamList->fScrollView = new BScrollView("SpecCharsScrollView", fParamList, B_FOLLOW_ALL, B_WILL_DRAW, false, true, B_PLAIN_BORDER);//B_FANCY_BORDER
 AddChild(fParamList->fScrollView);
 fParamList->TargetedByScrollView(fParamList->fScrollView);
 
 fParamList->SetFontSize(12);
 
 fParamList->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
 fParamList->SetLowColor(ui_color(B_PANEL_BACKGROUND_COLOR));

 fParamList->fTagLabel = new BStringView(BRect(Bounds().right /2 - 110,3, Bounds().right /2 + 80,20),"dd","", B_FOLLOW_H_CENTER | B_FOLLOW_TOP);
 fParamList->fTagLabel->SetAlignment(B_ALIGN_CENTER);
 fParamList->fTagLabel->SetFont(be_bold_font);
 fParamList->fTagLabel->SetFontSize(12);
 AddChild(fParamList->fTagLabel);
}

// ----------------------------------------------------------------------- RHTML_parammenu - MessageReceived -
void RHTMLparammenu::MessageReceived(BMessage *msg)
{
 switch (msg->what)
 {
  case 'SETV':
  {
   fWin->Lock();
   const char *tag;
   const char *param;
   const char *value;
   msg->FindString("tag",&tag);
   msg->FindString("param",&param);
   msg->FindString("value",&value);
   fWin->textview[fWin->current]->fTextView->SetParamValue(BString(tag),BString(param),BString(value));
   fWin->Unlock();
  }
  break;
  default:
  {
   BView::MessageReceived(msg);
  }
  break;
 }
}

// ------------------------------------------------------------------------ RHTML_plistview - RHTMLplistview -
RHTMLplistview::RHTMLplistview(BRect rect, BWindow *win): BListView(rect,"List", B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL)
{
 fWindow = win;
 // - Load HTMLtags.rdat
 BString dir=fOptions->SettingsDir;
 dir << "tags/htmltags.rdat";
 FILE *f;
 char c;
 BString line;
 int i=0;
 if( (f = fopen(dir.String(),"rt")) == NULL); else
 {
  while (feof(f)==false) 
  {
   line = "";
   fscanf(f,"%c",&c);
   while ((c!='\n')&&(!feof(f)))
   {
    line+=c;
    fscanf(f,"%c",&c);
   }
   line=rgetcomment(line);

   BString rtag, str3, str4, name, text, tag, param, type;
   std::vector<Cmd> cmd;

   int32 x,y;

   x=line.FindFirst('<');
   y=line.FindFirst(' ',x);
   if ((x>=0) && (y>=0) && (y>=x))
    line.CopyInto(rtag,x+1,y-x-1);
  
   i=line.FindFirst('<')+1;
   while (i<=line.FindFirst('>',i)-1)
   {
    x=line.FindFirst(' ',i)+1;
    y=line.FindFirst('=',x);
    if ((x>=0) && (y>=0) && ((y-x)>=0))
    line.CopyInto(str3,x,y-x);
 
    x=line.FindFirst('\'',i)+1;
    y=line.FindFirst('\'',x);
    if ((x>=0) && (y>=0) && ((y-x)>=0))
    line.CopyInto(str4,x,y-x);

    i=y+1;

    if (str3 == "NAME") name = str4;
    if (str3 == "TEXT") text = str4;
    if (str3 == "TAG") tag = str4;
    if (str3 == "PARAM") param = str4;
    if (str3 == "TYPE") type = str4;
    if ((str3 == "NEWLINE") || (str3 == "INSERTBACK") || (str3 == "INSERTFOR") || (str3 == "INSERT") || (str3 == "PUT"))
    {
     Cmd tmp;
     tmp.type = str3;
     tmp.param = str4;
     cmd.push_back(tmp);
    }
   }
   
   name.ToLower();
   type.ToLower();
   param.ToLower();
   tag.ToLower();
   
   if (rtag == "TAG")
   {
    Tag tmp;
    tmp.text=text;
    tmp.cmd=cmd;
    fTag[name] = tmp;
   }

   if (rtag == "PARAM")
   {
    Param tmp;
    tmp.text=text;
    tmp.type=type;
    tmp.cmd=cmd;
    fTag[tag].param[name] = tmp;
   }

   if (rtag == "VALUE")
   {
    Value tmp;
    tmp.cmd=cmd;
    fTag[tag].param[param].value[text] = tmp;
   }
  }
  fclose(f);
 }
}

// ----------------------------------------------------------------------------- RHTML_plistview - MouseDown -
void RHTMLplistview::MouseDown(BPoint where)
{
	for (int i = 0; (uint) i < fItems.size(); i++)
		if (ItemFrame(i).left <= where.x && ItemFrame(i).top <= where.y && ItemFrame(i).right >= where.x && ItemFrame(i).bottom >= where.y)
			if (CurrentSelection() != i)
			{
				if (CurrentSelection() >= 0)
					fItems[CurrentSelection()]->SetSelected(false);
				Select(i);
			} else
				fItems[i]->SetSelected();
}

// --------------------------------------------------------------------------- RHTML_plistview - AllAttached -
void RHTMLplistview::AllAttached()
{
	for (int i=0; (uint) i < fItems.size(); i++)
		RemoveItem(fItems[i]);

	for (int i=0; (uint) i < fItems.size(); i++)
		AddItem(fItems[i]);

	for (int i=0; (uint) i < fItems.size(); i++)
		fItems[i]->AttachedToView();
}

// ------------------------------------------------------------------------------------------ StrToParamType -
ParamType StrToParamType(BString str)
{
	if (str.ICompare("text")==0)
		return R_TEXT_PARAM;
	if (str.ICompare("int")==0)
		return R_INT_PARAM;
	if (str.ICompare("int%")==0)
		return R_INT_SZ_PARAM;
	if (str.ICompare("color")==0)
		return R_COLOR_PARAM;
	if (str.ICompare("radio")==0)
		return R_RADIO_PARAM;

	return R_TEXT_PARAM;
}


// -------------------------------------------------------------------------- RHTML_plistview - SetTagParams -
void RHTMLplistview::SetTagParams(BString tag, std::vector<Params> params)
{

 if (fTagName.ICompare(tag) == 0)
 {
  for (uint i = 0; i < fItems.size(); i++)
  {
   bool talalt = false;
   for (uint o=0;o<params.size();o++)
    if (params[o].name.ICompare(fItems[i]->GetName()) == 0)
    {
     fItems[i]->SetValue(params[o].value);
     talalt = true;
    }
   if (!talalt)
    fItems[i]->SetValue(BString(""));
  }
  Draw(Bounds());
 } else
 {
  fTagName = tag;
  for (uint i=0;i<fItems.size();i++)
  {
   RemoveItem(fItems[i]);
   delete fItems[i];
  }
  MakeEmpty();
  fItems.clear();

  std::map<BString,Param>::const_iterator tmp;
  for (tmp = fTag[tag].param.begin();tmp!=fTag[tag].param.end();tmp++)
  {
   BString value;
   for (uint i = 0; i < params.size(); i++)
    if (params[i].name.ICompare(tmp->second.text)==0)
     value = params[i].value;
   fItems.push_back(new RHTMLplistitem(tmp->second.text, value, StrToParamType(tmp->second.type), tag, this));

   AddItem(fItems[fItems.size() - 1]);
   fItems[fItems.size() - 1]->AttachedToView();
  }
 
  tag.ToUpper();
  fTagLabel->SetText(tag.String());
 }
}

// ---------------------------------------------------------------------------- RHTML_plistview - SelectItem -
void RHTMLplistview::SelectItem(BListItem *item)
{
 int tmp = GetItemNum(item);
 if (tmp>=0)
 Select(tmp);
}

// ---------------------------------------------------------------------- RHTML_plistview - SelectionChanged -
void RHTMLplistview::SelectionChanged()
{
 if (CurrentSelection()>=0)
  fItems[CurrentSelection()]->SetSelected();
}

// ---------------------------------------------------------------------------- RHTML_plistview - GetItemNum -
int RHTMLplistview::GetItemNum(BListItem *item)
{
 for (uint i=0;i<fItems.size();i++)
  if (fItems[i] == item)
   return i;
 return -1;
}

// -------------------------------------------------------------------------------- RHTML_plistview - GetTag -
std::map<BString,Tag> RHTMLplistview::GetTag()
{
 return fTag;
}

// ------------------------------------------------------------------------ RHTML_plistitem - RHTMLplistitem -
RHTMLplistitem::RHTMLplistitem(BString name, BString value, ParamType type, BString tag, RHTMLplistview *listview): BListItem()
{
 fName = name;
 fValue = value;
 fType = type;
 fListView = listview;
 
 if (fType == R_TEXT_PARAM || fType == R_INT_PARAM || fType == R_INT_SZ_PARAM)
 {
  fTextView = new RHTMLptextview(value,type,fListView,this);
  fListView->AddChild(fTextView);
  fTextView->MoveTo(69,fListView->GetItemNum(this)*Height()+2);
 } else
 if (fType == R_COLOR_PARAM)
 {
  fColorWin = new RHTMLcolorwin();
  fColorWin->Show();
  fListView->fWindow->Activate();
   
 } else
 if (fType == R_RADIO_PARAM)
 {
  fPopUpMenu = new BPopUpMenu("PopUpMenu");

  BString param = name;
  param.ToLower();

  std::map<BString,Value>::const_iterator tmp;

  for (tmp = fListView->fTag[tag].param[param].value.begin();tmp!=fListView->fTag[tag].param[param].value.end();tmp++)
  {
   BMessage *msg = new BMessage('RAIO');
   msg->AddString("value", tmp->first.String());
   fPopUpMenu->AddItem(new BMenuItem(tmp->first.String(), msg));
  }

 }
}

// ----------------------------------------------------------------------- RHTML_plistitem - ~RHTMLplistitem -
RHTMLplistitem::~RHTMLplistitem()
{
	if (fType == R_TEXT_PARAM || fType == R_INT_PARAM || fType == R_INT_SZ_PARAM)
	{
		fListView->RemoveChild(fTextView);
		delete fTextView;
	}
	if (fType == R_COLOR_PARAM)
	{
		fColorWin->Lock();
		fColorWin->Quit();
	}
}

// ------------------------------------------------------------------------ RHTML_plistitem - AttachedToView -
void RHTMLplistitem::AttachedToView()
{
	if (fType == R_TEXT_PARAM || fType == R_INT_PARAM || fType == R_INT_SZ_PARAM)
		fTextView->MoveTo(fListView->Bounds().right / 2 + 2, fListView->GetItemNum(this) * (ceil(Height()) + 1));
//  fTextView->MoveTo(fListView->Bounds().right /2 + 2, fListView->GetItemNum(this)*(ceil(14)+1));

//  fTextView->MoveTo(69,fListView->GetItemNum(this)*(ceil(Height())+1));
}

// ------------------------------------------------------------------------------ RHTML_plistitem - DrawItem -
void RHTMLplistitem::DrawItem(BView *owner, BRect bounds, bool complete)
{
 float kozep = (bounds.right - bounds.left) /2;
 if (fType == R_TEXT_PARAM || fType == R_INT_PARAM || fType == R_INT_SZ_PARAM)
  if (fTextView->Frame().left != fListView->Bounds().right /2 +10)
  {
   fTextView->MoveTo(kozep + 3, fTextView->Frame().top);
   fTextView->ResizeTo(fListView->Bounds().right - (kozep + 3 + 1),fTextView->Bounds().bottom);
  }
 rgb_color color = owner->HighColor();

 if (IsSelected())
 {
  owner->SetViewColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR),B_DARKEN_2_TINT));
  owner->SetLowColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR),B_DARKEN_2_TINT));

  owner->SetHighColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR),B_DARKEN_2_TINT));
//  owner->FillRect(BRect(bounds.left,bounds.top,bounds.left+67,bounds.bottom));
  owner->FillRect(BRect(bounds.left,bounds.top,bounds.left + kozep,bounds.bottom));
  owner->MovePenTo(bounds.left+4, bounds.bottom-2);
  owner->SetHighColor(color);
  owner->DrawString(fName.String());
    owner->SetHighColor(ui_color(B_PANEL_BACKGROUND_COLOR));
    owner->FillRect(BRect(bounds.left + kozep + 1, bounds.top, bounds.right ,bounds.bottom));

  owner->SetHighColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR),B_DARKEN_1_TINT));
  owner->FillRect(BRect(bounds.left + kozep + 1,bounds.top,bounds.left + kozep + 1,bounds.bottom));

  owner->SetHighColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR),B_DARKEN_2_TINT));
  owner->FillRect(BRect(bounds.left,bounds.bottom,bounds.right,bounds.bottom));
  owner->SetHighColor(color);
 } else
 {
  owner->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
  owner->SetLowColor(ui_color(B_PANEL_BACKGROUND_COLOR));

  owner->SetHighColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR),B_DARKEN_1_TINT));
  owner->FillRect(BRect(bounds.left,bounds.top,bounds.left + kozep,bounds.bottom));
  owner->MovePenTo(bounds.left+4, bounds.bottom-2);
  owner->SetHighColor(color);
  owner->DrawString(fName.String());
    owner->SetHighColor(ui_color(B_PANEL_BACKGROUND_COLOR));
    owner->FillRect(BRect(bounds.left + kozep + 1, bounds.top, bounds.right ,bounds.bottom));

  owner->SetHighColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR),B_DARKEN_2_TINT));
  owner->FillRect(BRect(bounds.left + kozep + 1,bounds.top,bounds.left + kozep + 1,bounds.bottom));
 
  owner->SetHighColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR),B_DARKEN_2_TINT));
  owner->FillRect(BRect(bounds.left,bounds.bottom,bounds.right,bounds.bottom));
  owner->SetHighColor(color);
 }
 if (fType == R_COLOR_PARAM)
 {
  owner->SetHighColor(ui_color(B_PANEL_BACKGROUND_COLOR));
  owner->FillRect(BRect(bounds.left + kozep + 2,bounds.top,bounds.right,bounds.bottom-1));

  if (fValue.Length()==0)
   owner->SetHighColor(ui_color(B_PANEL_BACKGROUND_COLOR));
  else
   owner->SetHighColor(BStringToRGB(fValue));
  if (IsSelected())
   owner->FillRect(BRect(bounds.left + kozep + 1,bounds.top,bounds.left + kozep + 6,bounds.bottom-1));
  else
   owner->FillRect(BRect(bounds.left + kozep + 2,bounds.top,bounds.left + kozep + 6,bounds.bottom-1));
  if (owner->StringWidth(fValue.String())+bounds.left + kozep + 3<=bounds.right)
   owner->FillRect(BRect(bounds.left + kozep + 7 + owner->StringWidth(fValue.String())+1,bounds.top,bounds.right,bounds.bottom-1));
  owner->FillRect(BRect(bounds.left + kozep + 2,bounds.top,bounds.right,bounds.top));
  owner->FillRect(BRect(bounds.left + kozep + 2,bounds.bottom-1,bounds.right,bounds.bottom-1));
  
  owner->MovePenTo(bounds.left + kozep + 8, bounds.bottom-3);
  owner->SetHighColor(color);
  owner->DrawString(fValue.String());
 }
 
 if (fType == R_RADIO_PARAM)
 {
  owner->SetHighColor(ui_color(B_PANEL_BACKGROUND_COLOR));
  owner->FillRect(BRect(bounds.left + kozep + 2,bounds.top,bounds.right,bounds.bottom-1));

   owner->SetHighColor(ui_color(B_PANEL_BACKGROUND_COLOR));
  if (IsSelected())
   owner->FillRect(BRect(bounds.left + kozep + 1,bounds.top,bounds.left + kozep + 6,bounds.bottom-1));
  else
   owner->FillRect(BRect(bounds.left + kozep + 2,bounds.top,bounds.left + kozep + 6,bounds.bottom-1));
  
  owner->MovePenTo(bounds.left + kozep + 8, bounds.bottom-3);
  owner->SetHighColor(color);
  owner->DrawString(fValue.String());
 }
}

// ------------------------------------------------------------------------------ RHTML_plistitem - SetValue -
void RHTMLplistitem::SetValue(BString value)
{
	fValue.SetTo(value);
	if (fType == R_TEXT_PARAM || fType == R_INT_PARAM || fType == R_INT_SZ_PARAM)
		fTextView->SetText(fValue.String());
}

// --------------------------------------------------------------------------- RHTML_plistitem - ModifyValue -
void RHTMLplistitem::ModifyValue(BString value)
{
	fValue.SetTo(value);
	fListView->Window()->Lock();
	fListView->Draw(fListView->Bounds());
	fListView->Window()->Unlock();
	BMessage msg('SETV');
	msg.AddString("tag", fListView->fTagName.String());
	msg.AddString("param", fName.String());
	msg.AddString("value", fValue.String());
	fListView->MessageReceived(&msg);
}

// ------------------------------------------------------------------------------ RHTML_plistitem - GetValue -
BString RHTMLplistitem::GetValue()
{
	return fValue;
}

// ------------------------------------------------------------------------------- RHTML_plistitem - GetName -
BString RHTMLplistitem::GetName()
{
	return fName;
}

// --------------------------------------------------------------------------- RHTML_plistitem - SetSelected -
void RHTMLplistitem::SetSelected(bool select)
{
	if (select)
	{
		if (fType == R_COLOR_PARAM)
		{
			BPoint point = fListView->ConvertToScreen(BPoint(fListView->ItemFrame(fListView->GetItemNum(this)).right,fListView->ItemFrame(fListView->GetItemNum(this)).top));
			fColorWin->Start(point,this,fValue);
		} else
			if (fType == R_RADIO_PARAM)
			{
				BPoint point = fListView->ConvertToScreen(BPoint(fListView->ItemFrame(fListView->GetItemNum(this)).right,fListView->ItemFrame(fListView->GetItemNum(this)).top));
				BMenuItem *item;
				if ((item = fPopUpMenu->Go(point)) != NULL)
					ModifyValue(item->Label());
			}
	}
	if (fType == R_TEXT_PARAM || fType == R_INT_PARAM || fType == R_INT_SZ_PARAM)
	{
		fTextView->MakeFocus(select);
	}
}

// ------------------------------------------------------------------------ RHTML_ptextview - RHTMLptextview -
RHTMLptextview::RHTMLptextview(BString value, ParamType type, RHTMLplistview *listview, RHTMLplistitem *listitem) : BTextView(BRect(0,0,63,13),"name",BRect(3,0,3000,13),B_FOLLOW_NONE,B_WILL_DRAW)
{
	fType = type;
	fListItem = listitem;
	fListView = listview;
	SetText(value.String());
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	SetWordWrap(false);
}

// ----------------------------------------------------------------------------- RHTML_ptextview - MakeFocus -
void RHTMLptextview::MakeFocus(bool focus)
{
	BTextView::MakeFocus(focus);
	if (focus)
	{
		fListView->SelectItem(fListItem);
		SelectAll();
	} else
	{
		SetText(fListItem->GetValue().String());
	}
}

// ------------------------------------------------------------------------------- RHTML_ptextview - KeyDown -
void RHTMLptextview::KeyDown(const char *bytes, int32 numBytes)
{
 switch(bytes[0])
 {
  case B_LEFT_ARROW:	BTextView::KeyDown(bytes,numBytes); break;
  case B_RIGHT_ARROW:	BTextView::KeyDown(bytes,numBytes); break;
  case B_UP_ARROW:		BTextView::KeyDown(bytes,numBytes); break;
  case B_DOWN_ARROW:	BTextView::KeyDown(bytes,numBytes); break;
  case B_DELETE:		BTextView::KeyDown(bytes,numBytes); break;
  case B_HOME:			BTextView::KeyDown(bytes,numBytes); break;
  case B_END:			BTextView::KeyDown(bytes,numBytes); break;
  case B_PAGE_UP:		BTextView::KeyDown(bytes,numBytes); break;
  case B_PAGE_DOWN:		BTextView::KeyDown(bytes,numBytes); break;
  case B_BACKSPACE:		BTextView::KeyDown(bytes,numBytes); break;
  case B_INSERT:		BTextView::KeyDown(bytes,numBytes); break;
  case '0':				BTextView::KeyDown(bytes,numBytes); break;
  case '1':				BTextView::KeyDown(bytes,numBytes); break;
  case '2':				BTextView::KeyDown(bytes,numBytes); break;
  case '3':				BTextView::KeyDown(bytes,numBytes); break;
  case '4':				BTextView::KeyDown(bytes,numBytes); break;
  case '5':				BTextView::KeyDown(bytes,numBytes); break;
  case '6':				BTextView::KeyDown(bytes,numBytes); break;
  case '7':				BTextView::KeyDown(bytes,numBytes); break;
  case '8':				BTextView::KeyDown(bytes,numBytes); break;
  case '9':				BTextView::KeyDown(bytes,numBytes); break;
  case B_ESCAPE:
  {
  }
  break;
  case B_ENTER:
  {
   fListItem->ModifyValue(BString(Text()));
  }
  break;
  case '%':
  {
   if (fType == R_TEXT_PARAM || fType == R_INT_SZ_PARAM)
   {
    BTextView::KeyDown(bytes,numBytes);
   }
  }
  break;
  default:
  {
   if (fType == R_TEXT_PARAM)
   {
    BTextView::KeyDown(bytes,numBytes);
   }
  }
 }
}

// --------------------------------------------------------------------------------- RHTML_ptextview - Paste -
void RHTMLptextview::Paste(BClipboard *clipboard)
{
	const char *text;
	ssize_t textlen;
	BMessage *clip = (BMessage *) NULL;

	if (clipboard->Lock())
	{
		if ((clip = clipboard->Data()));
			clip->FindData("text/plain", B_MIME_TYPE, (const void **)&text, &textlen);
		clipboard->Unlock();
	}
	for (int i=0;i<textlen;i++)
	{
		KeyDown(&text[i], 1);
	}
}

// - End - ParamMenu - Source File ---------------------------------------------------------------------------
