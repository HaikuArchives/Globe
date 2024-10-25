//////////////////////////////////////////////////////////////////////
//      Globe Webeditor © Gergely Rózsahegyi (RoGer) 2001-2004      //
//                                                                  //
//                        Text View Source File                     //
//                                                                  //
//                       Last Update: 2004.10.21					//
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
#include <Font.h>
#include <stdio.h>
#include <string.h>
#include <Path.h>
#include <File.h>
#include <Entry.h>
#include <NodeInfo.h>
#include "RHTML_textview.h"

// ---------------------------------------------------------------------------------- RHTML_text - RHTMLtext -
RHTMLtext::RHTMLtext(BRect fRect, BLooper *plooper, uint ft, RHTMLparammenu *parammenu): BView(fRect,"textview",B_FOLLOW_ALL,B_WILL_DRAW)
{
 fPLooper = plooper;
 fParamMenu = parammenu;
 modify=false;

 if (fOptions->NumView)
  fTextView = new RHTMLtextview(BRect(32,1,Bounds().right-15,Bounds().bottom-15),this,ft);
 else
  fTextView = new RHTMLtextview(BRect(0,1,Bounds().right-15,Bounds().bottom-15),this,ft);
 
 AddChild(fTextView->fScrollView=new BScrollView("scroll_view", fTextView,
 				B_FOLLOW_ALL, B_WILL_DRAW, true, true,B_NO_BORDER));
 fNumView= new RHTMLNumView(BRect(0,0,30,50000),fTextView);
 AddChild(fNumView);
 fFileName = new BString();
 
 fNumView->UpdateNum();
 if (fTextView->PointAt(fTextView->TextLength()-1).y+50>fNumView->Bounds().bottom) fNumView->ResizeTo(fNumView->Bounds().right,fTextView->PointAt(fTextView->TextLength()-1).y+50);
 SetNumView(fOptions->NumView);
}
// - End - RHTML_text - RHTMLtext ----------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------------
RHTMLtext::~RHTMLtext()
{
	delete fFileName;
}

// -----------------------------------------------------------------------------------------------------------
RHTMLtextview::~RHTMLtextview()
{
	fTagMenu->Lock();
	fTagMenu->Quit();
	delete fFileTypes;
}

// ------------------------------------------------------------------------- RHTML_text_view - RHTMLtextview -
RHTMLtextview::RHTMLtextview(BRect fRect, RHTMLtext *target, uint ft): BTextView(fRect,"fTextView",BRect(3,3,fRect.right-32,fRect.bottom-fRect.top),B_FOLLOW_ALL,B_WILL_DRAW)
{
 SetWordWrap(fOptions->WordWrap);
 fFont=new BFont(fOptions->DefaultFont);
 fTarget=target;
 fUpdateTimer = new RTimer("TextViewTimer", this, new BMessage('TIMR'));
 
 fFileTypes = new RHTMLFileTypes(ft,this);
 
 MakeFocus(true);
 SetDoesUndo(true);
 SetStylable(true);

 rgb_color fekete={0,0,0};

 SetFontAndColor(fFont,B_FONT_ALL,&fekete);

 fTagMenu= new RHTMLTagMenu(this);
 fTagMenu->Show();
 fTagMenu->Activate(false);
// fTagMenu->Hide();
 Window()->Activate(true);
 MakeFocus(true);

 fTimer = new RTimer("PopUpTimer", this, new BMessage('PPOP'));
 }
// - End - RHTML_text_view - RHTMLtextview -------------------------------------------------------------------

// ------------------------------------------------------------------------------- RHTML_text_view - KeyDown -
void RHTMLtextview::KeyDown(const char *bytes, int32 numBytes)
{
	pushmenu = false;
	int32 c,d;
	GetSelection(&c, &d);
	BString *oldtext = new BString(Text());
	BString *newtext = 0;
	switch(bytes[0])
	{
		case B_ESCAPE:			// - ESC
		{
		}
		break;
		case B_ENTER:				// - ENTER
		{
			fTarget->SetModify(true);
			BString tmp = "";
			if (d - OffsetAt(CurrentLine()) > 0)
			{
				for (int i = OffsetAt(CurrentLine()); i < d; i++)
				{
					if (Text()[i] == B_SPACE || Text()[i] == B_TAB)
					{
						tmp << Text()[i];
					} else
						break;
				}
			}
			BTextView::KeyDown(bytes, numBytes);
			Insert(tmp.String());

			fFileTypes->SetFontAndColor(oldtext, newtext = new BString(Text()), 0, c);
   
			if (fUpdateTimer->IsActive())
				fUpdateTimer->ResetTimer();
			else
				fUpdateTimer->StartTimer(fOptions->ReakcioIdo);
   
			if (PointAt(TextLength() - 1).y + 50 > fTarget->fNumView->Bounds().bottom) fTarget->fNumView->ResizeTo(fTarget->fNumView->Bounds().right, PointAt(TextLength() - 1).y + 50);
		}
		break;
		case '>':					// - >
		{
			fTagMenu->Stop();
   fTarget->SetModify(true);
   BTextView::KeyDown(bytes,numBytes);
   fFileTypes->SetFontAndColor(oldtext, newtext = new BString(Text()),0,c);

   if (fUpdateTimer->IsActive())
    fUpdateTimer->ResetTimer();
   else
    fUpdateTimer->StartTimer(fOptions->ReakcioIdo);

   if (PointAt(TextLength()-1).y+50>fTarget->fNumView->Bounds().bottom) fTarget->fNumView->ResizeTo(fTarget->fNumView->Bounds().right,PointAt(TextLength()-1).y+50);
  }
  break;
  case '<':					// - <
  {
   BTextView::KeyDown(bytes,numBytes);
   fTarget->SetModify(true);
   pushmenu=true;
   ScrollToSelection();
   fFileTypes->SetFontAndColor(oldtext, newtext = new BString(Text()),0,c);

   if (fUpdateTimer->IsActive())
    fUpdateTimer->ResetTimer();
   else
    fUpdateTimer->StartTimer(fOptions->ReakcioIdo);

   if (PointAt(TextLength()-1).y+50>fTarget->fNumView->Bounds().bottom) fTarget->fNumView->ResizeTo(fTarget->fNumView->Bounds().right,PointAt(TextLength()-1).y+50);
   mode=1;
   fTimer->StartTimer(fOptions->ReakcioIdoTM);
  }
  break;
  case B_SPACE:				// - SPACE
  {
   fTarget->SetModify(true);
   BTextView::KeyDown(bytes,numBytes);
   fFileTypes->SetFontAndColor(oldtext, newtext = new BString(Text()),0,c);

   if (fUpdateTimer->IsActive())
    fUpdateTimer->ResetTimer();
   else
    fUpdateTimer->StartTimer(fOptions->ReakcioIdo);

   if (PointAt(TextLength()-1).y+50>fTarget->fNumView->Bounds().bottom) fTarget->fNumView->ResizeTo(fTarget->fNumView->Bounds().right,PointAt(TextLength()-1).y+50);
   buf1 = "";
   int32 o;
   
   for (int i=c;i>=0;i--)
   {
    if (Text()[i]=='>') break;
    if (Text()[i]=='<')
    {
     for (o=i+1;o<=c+1;o++)
     {
      if (Text()[o]=='>') break;
      if (Text()[o]=='<') break;

      if (Text()[o]==' ')
      {
       pushmenu=true;
       mode=2;
       fTimer->StartTimer(fOptions->ReakcioIdoTM);
       break;
       i=-1;
      }
      buf1 << Text()[o];
     }
    }
   }
   
   BString str(Text());
   int32 a,b;
   tags.clear();

   a=str.FindLast('<',c);

   if (a>str.FindLast('>',c))
   {
    b=str.FindFirst('>',c);
    int32 e=str.FindFirst('<',c);
    if (b>=0 && ((b<e && e>=0) || (e<0)))
    {
     while (str[a]!=' ')
      a++;
     a++;
     int32 i=a;
     while (i<b)
     {
      if (str[i]==' ')
      {
       BString tmp;
       str.CopyInto(tmp,a,i-a);
       tmp.ToLower();
       if (tmp!=" " && tmp!="")
        tags.push_back(tmp);
       a=i+1;
      }

      if (str[i]=='=')
      {
       BString tmp;
       str.CopyInto(tmp,a,i-a);
       tmp.ToLower();
       tags.push_back(tmp);
       if (str[i+1]=='"')
       {
        a=str.FindFirst('"',i+2)+1;
        i=a-1;
       }
       else
        a=i=str.FindFirst(' ',i+1);
      }

      i++;
     }
    }   
   }
  }
  break;
  case B_UP_ARROW:			// - UP
  {
   BTextView::KeyDown(bytes,numBytes);
   if (fUpdateTimer->IsActive())
    fUpdateTimer->ResetTimer();
   else
    fUpdateTimer->StartTimer(fOptions->ReakcioIdo);
  }
  break;
  case B_DOWN_ARROW:		// - DOWN
  {
   BTextView::KeyDown(bytes,numBytes);
   if (fUpdateTimer->IsActive())
    fUpdateTimer->ResetTimer();
   else
    fUpdateTimer->StartTimer(fOptions->ReakcioIdo);
  }
  break;
  case B_LEFT_ARROW:		// - LEFT
  {
   BTextView::KeyDown(bytes,numBytes);
   if (fUpdateTimer->IsActive())
    fUpdateTimer->ResetTimer();
   else
    fUpdateTimer->StartTimer(fOptions->ReakcioIdo);
  }
  break;
  case B_RIGHT_ARROW:		// - RIGHT
  {
   BTextView::KeyDown(bytes,numBytes);
   if (fUpdateTimer->IsActive())
    fUpdateTimer->ResetTimer();
   else
    fUpdateTimer->StartTimer(fOptions->ReakcioIdo);
  }
  break;
  case B_PAGE_UP:			// - PUP
  {
   BTextView::KeyDown(bytes,numBytes);
   if (fUpdateTimer->IsActive())
    fUpdateTimer->ResetTimer();
   else
    fUpdateTimer->StartTimer(fOptions->ReakcioIdo);
  }
  break;
  case B_PAGE_DOWN:			// - PDOWN
  {
   BTextView::KeyDown(bytes,numBytes);
   if (fUpdateTimer->IsActive())
    fUpdateTimer->ResetTimer();
   else
    fUpdateTimer->StartTimer(fOptions->ReakcioIdo);
  }
  break;
  case B_HOME:				// - HOME
  {
   BTextView::KeyDown(bytes,numBytes);
   if (fUpdateTimer->IsActive())
    fUpdateTimer->ResetTimer();
   else
    fUpdateTimer->StartTimer(fOptions->ReakcioIdo);
  }
  break;
  case B_END:				// - END
  {
   BTextView::KeyDown(bytes,numBytes);
   if (fUpdateTimer->IsActive())
    fUpdateTimer->ResetTimer();
   else
    fUpdateTimer->StartTimer(fOptions->ReakcioIdo);
  }
  break;
  case B_BACKSPACE:			// - BackSpace
  {
   fTarget->SetModify(true);
   BTextView::KeyDown(bytes,numBytes);
   fFileTypes->SetFontAndColor(oldtext, newtext = new BString(Text()),0,c-1);

   if (fUpdateTimer->IsActive())
    fUpdateTimer->ResetTimer();
   else
    fUpdateTimer->StartTimer(fOptions->ReakcioIdo);

   if (PointAt(TextLength()-1).y+50>fTarget->fNumView->Bounds().bottom) fTarget->fNumView->ResizeTo(fTarget->fNumView->Bounds().right,PointAt(TextLength()-1).y+50);
  }
  break;
  default:
  {
   fTarget->SetModify(true);
   BTextView::KeyDown(bytes,numBytes);
   fFileTypes->SetFontAndColor(oldtext, newtext = new BString(Text()),0,c);

   if (fUpdateTimer->IsActive())
    fUpdateTimer->ResetTimer();
   else
    fUpdateTimer->StartTimer(fOptions->ReakcioIdo);

   if (PointAt(TextLength()-1).y+50>fTarget->fNumView->Bounds().bottom) fTarget->fNumView->ResizeTo(fTarget->fNumView->Bounds().right,PointAt(TextLength()-1).y+50);

   if (fUpdateTimer->IsActive())
    fUpdateTimer->ResetTimer();
   else
    fUpdateTimer->StartTimer(fOptions->ReakcioIdo);

   if (PointAt(TextLength()-1).y+50>fTarget->fNumView->Bounds().bottom) fTarget->fNumView->ResizeTo(fTarget->fNumView->Bounds().right,PointAt(TextLength()-1).y+50);
  }
 }
	if (oldtext)
 		delete oldtext;
	if (newtext)
 		delete newtext;
// ScrollToSelection();
}
// - End - RHTML_text_view - KeyDown -------------------------------------------------------------------------

// ----------------------------------------------------------------------------------- RHTML_text_view - Cut -
void RHTMLtextview::Cut(BClipboard *clipboard)
{
	fTarget->SetModify(true);
	BString *oldtext = new BString(Text());
	BString *newtext = 0;
	BTextView::Cut(clipboard);
	int32 c, d;
	GetSelection(&c, &d);
	fFileTypes->SetFontAndColor(oldtext, newtext = new BString(Text()), 0, c);

	if (fUpdateTimer->IsActive())
		fUpdateTimer->ResetTimer();
	else
		fUpdateTimer->StartTimer(fOptions->ReakcioIdo);

	if (PointAt(TextLength() - 1).y + 50>fTarget->fNumView->Bounds().bottom)
		fTarget->fNumView->ResizeTo(fTarget->fNumView->Bounds().right, PointAt(TextLength() - 1).y + 50);

	delete oldtext;
	delete newtext;
}
// - End - RHTML_text_view - Cut -----------------------------------------------------------------------------

// --------------------------------------------------------------------------------- RHTML_text_view - Paste -
void RHTMLtextview::Paste(BClipboard *clipboard)
{
	fTarget->SetModify(true);
	BString *oldtext = new BString(Text());
	BString *newtext = 0;
	int32 c, d, e;
	GetSelection(&c, &d);
	BTextView::Paste(clipboard);
	GetSelection(&d, &e);
	fFileTypes->SetFontAndColor(oldtext, newtext = new BString(Text()), 0, c, d);

	if (fUpdateTimer->IsActive())
		fUpdateTimer->ResetTimer();
	else
		fUpdateTimer->StartTimer(fOptions->ReakcioIdo);

	if (PointAt(TextLength() - 1).y + 50>fTarget->fNumView->Bounds().bottom)
		fTarget->fNumView->ResizeTo(fTarget->fNumView->Bounds().right, PointAt(TextLength() - 1).y + 50);

	delete oldtext;
	delete newtext;
}
// - End - RHTML_text_view - Paste ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------------- RHTML_text_view - Undo -
void RHTMLtextview::Undo(BClipboard *clipboard)
{
	fTarget->SetModify(true);
	BString *oldtext=new BString(Text());
	BString *newtext = 0;
	BTextView::Undo(clipboard);
	int32 c, d;
	GetSelection(&c, &d);
	fFileTypes->SetFontAndColor(oldtext, newtext = new BString(Text()), 0, c, d);

	if (fUpdateTimer->IsActive())
		fUpdateTimer->ResetTimer();
	else
		fUpdateTimer->StartTimer(fOptions->ReakcioIdo);

	if (PointAt(TextLength() - 1).y + 50>fTarget->fNumView->Bounds().bottom)
		fTarget->fNumView->ResizeTo(fTarget->fNumView->Bounds().right, PointAt(TextLength() - 1).y + 50);
		
	delete oldtext;
	delete newtext;
}
// - End - RHTML_text_view - Undo ----------------------------------------------------------------------------

// ------------------------------------------------------------------------------ RHTML_text_view - DropFile -
void RHTMLtextview::DropFile(const char *filename, const char *type, int32 offset)
{
	if (strncmp(type, "image", 5) == 0)
	{
		Select(offset, offset);
		BString tmp("<IMG SRC=\"");

		tmp << GetRelativePath(filename).String() << "\">";
		Insert(tmp.String());
		Select(offset, offset + tmp.Length());
	}
	if (strncmp(type, "text/html", 9) == 0 || strncmp(type, "text/php", 8) == 0)
	{
		Select(offset, offset);
		BString tmp("<A HREF=\"");

		tmp << GetRelativePath(filename).String() << "\">";
		Insert(tmp.String());
		Select(offset, offset + tmp.Length());
	}
}
// - End - RHTML_text_view - DropFile ------------------------------------------------------------------------

// ----------------------------------------------------------------------- RHTML_text_view - GetRelativePath -
BString RHTMLtextview::GetRelativePath(const char *path)
{
	BString pth, ori, tmp;
	tmp.SetTo(path);
	ori.SetTo(tmp, tmp.FindLast("/") + 1);

	if (fTarget->fFileName->Compare("") != 0)
		pth.SetTo(fTarget->fFileName->String(), fTarget->fFileName->FindLast("/"));
	else
		pth = fOptions->DefaultFolder;
	pth << "/";
 
	uint i,db,last;
	i = db = last = 0;
	i = ori.FindFirst("/", 0);
	tmp.SetTo(ori, i);
 
	while (pth.Compare(tmp, i) == 0)
	{
		last = i;
		if (ori.FindFirst("/", i + 1) >= 0)
		{
			i = ori.FindFirst("/", i + 1);
			tmp.SetTo(ori, i);
		} else
			break;
	}
 
	i = last;
	while (pth.FindFirst("/", i + 1) >= 0)
	{
		db++;
		i = pth.FindFirst("/", i + 1);
	}

	tmp.SetTo("");
	for (i = 0; i < db; i++)
		tmp << "../";
 
	ori.SetTo(path);
	ori.CopyInto(pth, last + 1, ori.Length() - last - 1);
	tmp << pth;

	return tmp;
}
// - End - RHTML_text_view - GetRelativePath -----------------------------------------------------------------

// ----------------------------------------------------------------------------- RHTML_text_view - MouseDown -
void RHTMLtextview::MouseDown(BPoint where)
{
	fTagMenu->Stop();
	MakeFocus(true);

	BTextView::MouseDown(where);
	if (fUpdateTimer->IsActive())
		fUpdateTimer->ResetTimer();
	else
		fUpdateTimer->StartTimer(fOptions->ReakcioIdo);
}

// -------------------------------------------------------------------------- RHTML_text_view - FrameResized -
void RHTMLtextview::FrameResized(float new_width, float new_height)
{
	if (DoesWordWrap() && fTarget->IsEnabled())
	{
		SetTextRect(BRect(3, 3, Bounds().right - 2, Bounds().bottom - Bounds().top));
	}
	BTextView::FrameResized(new_width, new_height);
}
// - End - RHTML_text_view - FrameResized --------------------------------------------------------------------

// ---------------------------------------------------------------------------------- RHTML_text_view - Draw -
void RHTMLtextview::Draw(BRect inRect)
{
	BTextView::Draw(inRect);
	if (fTarget->fNumView->Frame().top != fScrollView->ScrollBar(B_VERTICAL)->Value())
	fTarget->fNumView->MoveTo(BPoint(0, 0 - fScrollView->ScrollBar(B_VERTICAL)->Value()));

	if (fTarget->IsEnabled())
		fTarget->fNumView->UpdateNum();
	if (PointAt(TextLength() - 1).y + 50 > fTarget->fNumView->Bounds().bottom)
		fTarget->fNumView->ResizeTo(fTarget->fNumView->Bounds().right, PointAt(TextLength() - 1).y + 50);
}
// - End - RHTML_text_view - Draw ----------------------------------------------------------------------------

// ----------------------------------------------------------------------- RHTML_text_view - UpdateParamMenu -
void RHTMLtextview::UpdateParamMenu()
{
	BString tag = "";
	std::vector<Params> params;
	if (TextLength() > 0)
	{
		int32 c, d;
		GetSelection(&c, &d);
		int32 kezd, veg;

		BString text(Text());

		if ((kezd = text.FindLast("<",c)) > -1 || text[0] == '<')
		{
			if (kezd == -1)
				kezd = 0;
			if ((veg = text.FindFirst(">",c)) > -1)
				if ((veg < text.FindFirst("<",c)) || (text.FindFirst("<",c) == -1))
				{
					if ((veg > text.FindFirst(" ",kezd+1)) && (text.FindFirst(" ",kezd+1) > -1))
						veg = text.FindFirst(" ", kezd);
					text.CopyInto(tag, kezd + 1, veg - kezd - 1);
					veg = text.FindFirst(">",c);
					params.clear();

					while (text[kezd]!=' ' && kezd < veg )
						kezd++;
					kezd++;
					int32 i = kezd;
					while (i < veg)
					{
						if (text[i] == ' ')
						{
							struct Params tmp;
							text.CopyInto(tmp.name, kezd, i - kezd);
							tmp.name.ToLower();
							if (tmp.name != " " && tmp.name != "")
							{
								params.push_back(tmp);
							}
							kezd = i + 1;
						}

						if (text[i] == '=')
						{
							struct Params tmp;
							text.CopyInto(tmp.name, kezd, i - kezd);
							tmp.name.ToLower();
							if (text[i + 1] == '"')
							{
								if ((kezd = text.FindFirst('"', i + 2) + 1) > 0)
								{
									text.CopyInto(tmp.value, i + 2, kezd - i - 3);
									params.push_back(tmp);
									i = kezd - 1;
								} else
								{
									i++;
									break;
								}
							}
							else
							{
								if ((kezd = text.FindFirst(' ', i + 1 ) + 1) > 0)
								{
									text.CopyInto(tmp.value, i + 1, kezd - i - 2);
									i = text.FindFirst(' ', i + 1 );
									params.push_back(tmp);
								} else
								{
									i++;
									break;
								}
							}
						}
						i++;
					}
				}
		}
		tag.ToLower();
	}
	fTarget->fParamMenu->fParamList->SetTagParams(tag,params);
}
// - End - RHTML_text_view - UpdateParamMenu -----------------------------------------------------------------

// ------------------------------------------------------------------------- RHTML_text_view - SetParamValue -
void RHTMLtextview::SetParamValue(BString Tag, BString Param, BString Value)
{
 BString tag="";
 bool talalt = false;
 int32 c,d;
 GetSelection(&c,&d);
 int32 kezd,veg,pkezd;

 BString text(Text());
 BString *newtext = 0;

 if ((kezd = text.FindLast("<",c)) > -1 || text[0] == '<')
 {
  if (kezd == -1)
   kezd = 0;
  if ((veg = text.FindFirst(">",c)) > -1)
   if ((veg < text.FindFirst("<",c)) || (text.FindFirst("<",c) == -1))
   {
    if ((veg > text.FindFirst(" ",kezd+1)) && (text.FindFirst(" ",kezd+1) > -1))
     veg = text.FindFirst(" ", kezd);
    text.CopyInto(tag, kezd + 1, veg - kezd -1);
    if (tag.ICompare(Tag)==0)
    {
     veg = text.FindFirst(">",c);
 
     while (text[kezd]!=' ' && kezd < veg)
      kezd++;
     kezd++;
     int32 i=kezd;
     while (i<veg && !talalt)
     {
      if (text[i]==' ')
       kezd=i+1;
      if (text[i] == '=')
      {
       struct Params tmp;
       text.CopyInto(tmp.name, kezd, i - kezd);
       pkezd = kezd - 1;
       tmp.name.ToLower();
       if (text[i+1]=='"')
       {
        kezd = text.FindFirst('"', i + 2) + 1;
        if (tmp.name.ICompare(Param)==0)
        {
         if (Value.Length() == 0)
         {
          Select(pkezd,pkezd);
          Delete(pkezd, kezd);
         } else
         {
          Delete(i + 2, kezd - 1);
          Select(i + 2, i + 2);
          Insert(Value.String());
          fFileTypes->SetFontAndColor(&text, newtext = new BString(Text()), i + 2, i + 2 + Value.Length());
         }
         talalt = true;
        }
        i = kezd - 1;
       }
       else
       {
        kezd = text.FindFirst(' ', i + 1 )+1;
        if (tmp.name.ICompare(Param)==0)
        {
         if (Value.Length() == 0)
         {
          Select(pkezd,pkezd);
          Delete(pkezd, kezd - 1);
         } else
         {
          Delete(i + 1, kezd - 1);
          Select(i + 1, i + 1);
          Insert(Value.String());
          fFileTypes->SetFontAndColor(&text, newtext = new BString(Text()), i + 1, i + 1 + Value.Length());
         }
         talalt = true;
        }
        i = text.FindFirst(' ', i + 1 );
       }
      }
      i++;
     }
     if (!talalt && Value.Length() > 0)
     {
      Select(veg,veg);
      Insert(" ");
      Insert(Param.String());
      Insert("=\"");
      Insert(Value.String());
      Insert("\"");
      fFileTypes->SetFontAndColor(&text, newtext = new BString(Text()), i + 1, i + 1 + Value.Length());
     }
    }
   }
 }
	if (newtext)
		delete newtext;
}
// - End - RHTML_text_view - SetParamValue -------------------------------------------------------------------

// ----------------------------------------------------------------------- RHTML_text_view - MessageReceived -
void RHTMLtextview::MessageReceived(BMessage *msg)
{
 switch (msg->what)
  {
   case 'PPOP':
   {
	Window()->Lock();
	if (pushmenu)
    {
     int32 c,d;
     GetSelection(&c,&d);
     pushmenu=false;
     if (mode==1) fTagMenu->Start(ConvertToScreen(PointAt(c)),c,R_TAG_MENU,tags);
     if (mode==2) fTagMenu->Start(ConvertToScreen(PointAt(c)),c,R_PARAM_MENU,tags,BString(buf1));
     if (mode==3) fTagMenu->Start(ConvertToScreen(PointAt(c)),c,R_VALUE_MENU,tags,BString(buf1),BString(buf2));

    }
	Window()->Unlock();
	fTimer->EndTimer();
   }
   break;
   case 'TIMR':
   {
	Window()->Lock();
	UpdateParamMenu();
    fTarget->fNumView->UpdateNum();
	Window()->Unlock();
	fUpdateTimer->EndTimer();
   }
   break;
   case 'PTFC':
   {
    int32 c,d;
    BString oldstring;
    msg->FindInt32("sel1",&c);
    msg->FindInt32("sel2",&d);
    msg->FindString("oldstring",&oldstring);
    BString *newtext = new BString(Text());
    fFileTypes->SetFontAndColor(&oldstring, newtext,0,c,d);
    delete newtext;
   }
   break;
   case 'CWFC':
   {
    int32 c,d;
    GetSelection(&c,&d);
    BString *newtext = new BString(Text());
    fFileTypes->SetFontAndColor(newtext, newtext, 0, c);
    delete newtext;
   }
   break;
   case 'VALE':
   {
    msg->FindString("tag",&buf1);
    msg->FindString("param",&buf2);
    pushmenu=true;
    mode=3;
    fTimer->StartTimer(fOptions->ReakcioIdoTM);
   }
   break;
   case 'UPPM':
   {
    if (fUpdateTimer->IsActive())
     fUpdateTimer->ResetTimer();
    else
     fUpdateTimer->StartTimer(fOptions->ReakcioIdo);
   }
   break;
   default:
   {
    if (msg->WasDropped())
    {
     int32		num;
     uint32		type;
     char*		name;
     BPoint		aPoint;
     BPoint		point;
     entry_ref ref;
     status_t err;
     
     msg->FindPoint("_drop_point_", &aPoint);

     point = aPoint;
     this->ConvertFromScreen(&point);

     const int32 offset = OffsetAt(point);
    
     if (msg->GetInfo(B_REF_TYPE, 0, &name, &type, &num) == B_NO_ERROR && num >= 1)
     {
		// - File
      for(int32 i = 0; i < num; i++)
      {
       BEntry entry;
       BPath path;
       BFile fFile;
       BNodeInfo fNodeInfo;
       entry_ref ref;
       char fFileType[B_MIME_TYPE_LENGTH];
       if(msg->FindRef("refs", i, &ref) != B_NO_ERROR)
       {
        continue;
       }
       if ((err=entry.SetTo(&ref)) == B_OK)
       {
        entry.GetPath(&path);
       } 
       fFile.SetTo(&ref,B_READ_ONLY);
       fNodeInfo.SetTo(&fFile);
       fNodeInfo.GetType(fFileType);
       DropFile(path.Path(), fFileType, offset);
      }
     }
    } else
    {
     BTextView::MessageReceived(msg);
    }
   }
   break;
  }
}
// - End - RHTML_text_view - MessageReceived -----------------------------------------------------------------

// --------------------------------------------------------------------------------- RHTML_text - SetNumView -
void RHTMLtext::SetNumView(bool kibe)
{
	if (kibe)
	{
		if (fNumView->IsHidden())
		{
			fNumView->Show();
			fTextView->fScrollView->MoveTo(32, 0);
			fTextView->fScrollView->ResizeTo(Bounds().right - 32, Bounds().bottom);
			fNumView->ResizeTo(31, Bounds().bottom);
		}
	} else
	{
		if (!fNumView->IsHidden())
		{
			fNumView->Hide();
			fTextView->fScrollView->MoveTo(0, 0);
			fTextView->fScrollView->ResizeTo(Bounds().right, Bounds().bottom);
			fNumView->ResizeTo(0, Bounds().bottom);
		}
	}
}
// - End - RHTML_text - SetNumView ---------------------------------------------------------------------------

// -------------------------------------------------------------------------------- RHTML_text - SetWordWrap -
void RHTMLtext::SetWordWrap(bool kibe)
{
	fTextView->SetWordWrap(kibe);
	if (kibe)
		fTextView->SetTextRect(BRect(3, 3, fTextView->fScrollView->Bounds().right - 17, fTextView->Bounds().bottom - fTextView->Bounds().top));
}
// - End - RHTML_text - SetWordWrap --------------------------------------------------------------------------

// --------------------------------------------------------------------------------- RHTML_text - SetEnabled -
void RHTMLtext::SetEnabled(bool kibe)
{
	fEnabled=kibe;
}

// ---------------------------------------------------------------------------------- RHTML_text - IsEnabled -
bool RHTMLtext::IsEnabled()
{
	return fEnabled;
}
// - End - RHTML_text - SetEnabled ---------------------------------------------------------------------------

// ------------------------------------------------------------------------------------- RHTML_text - Modify -
bool RHTMLtext::Modify()
{
	return modify;
}

// ---------------------------------------------------------------------------------- RHTML_text - SetModify -
void RHTMLtext::SetModify(bool kibe)
{
	if (modify != kibe)
	{
		modify = kibe;
		MessageReceived(new BMessage('RTMM'));
	}
	else
		modify=kibe;
}

// ---------------------------------------------------------------------------- RHTML_text - MessageReceived -
void RHTMLtext::MessageReceived(BMessage *msg)
{
	switch (msg->what)
	{
		default:
		{
			BView::MessageReceived(msg);
		}
		break;
	}
}
// - End - RHTML_text - MessageReceived ----------------------------------------------------------------------

// - End - Text View - Source File ---------------------------------------------------------------------------
