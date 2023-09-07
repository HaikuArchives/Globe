//////////////////////////////////////////////////////////////////////
//      Globe Webeditor © Gergely Rózsahegyi (RoGer) 2001-2004      //
//                                                                  //
//                         TagMenu Source File                      //
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

#include "RHTML_tagmenu.h"
#include "RHTML_options.h"
#include <ScrollView.h>
#include <Screen.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

// --------------------------------------------------------------------------- RHTML_Tag_Menu - RHTMLTagMenu -
RHTMLTagMenu::RHTMLTagMenu(BTextView *textview): BWindow(BRect(-110,-210,-10,-10),"RHTMLTagMenuWindow",B_NO_BORDER_WINDOW_LOOK,B_FLOATING_APP_WINDOW_FEEL,B_AVOID_FRONT)
{
 fTextView=textview;

 fColorWin = new RHTMLcolorwin();
 fColorWin->Show();
 fColorWin->Activate(false);
// Activate();

 fListView = new RHTMLTagMenuList(this,textview);
 
 BScrollView *fScrollView = new BScrollView("TagMenuScrollView",fListView,B_FOLLOW_ALL,B_WILL_DRAW, false, true,B_FANCY_BORDER);
 AddChild(fScrollView);
 fListView->TargetedByScrollView(fScrollView);
 fListView->SetInvocationMessage(new BMessage('SEL'));

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
    if (type.ICompare("color") == 0)
    {
     Value vtmp;
     Cmd ctmp;
     ctmp.type = BString("SPECIAL");
     ctmp.param = BString("#Color");
     std::vector<Cmd> ccmd;
     ccmd.push_back(ctmp);
     vtmp.cmd = ccmd;
     fTag[tag].param[name].value["#Custom"] = vtmp;
    }
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

// -----------------------------------------------------------------------------------------------------------
RHTMLTagMenu::~RHTMLTagMenu()
{
	fColorWin->Lock();
	fColorWin->Quit();
}

// ---------------------------------------------------------------------------------- RHTML_Tag_Menu - Start -
void RHTMLTagMenu::Start(BPoint point, int32 offset, MenuType _menutype, std::vector<BString> &_tags, BString _tag, BString _param)
{
 menutype=_menutype;
 tag=_tag;
 tags=_tags;
 param=_param;
 tag.ToLower();
 param.ToLower();
 Lock();

 if (_menutype==R_TAG_MENU)
  fListView->str = "<";
 if (_menutype==R_PARAM_MENU)
  fListView->str = " ";
 if (_menutype==R_VALUE_MENU)
  fListView->str = "";
 
 LoadList();
 if (fListView->CountItems()>0)
 {
  fListView->sel1=offset-1;
  fListView->sel2=offset;
  
  if (_menutype==R_VALUE_MENU)
   fListView->sel1++;

  SetFeel(B_MODAL_APP_WINDOW_FEEL);
  Activate(true);
  fListView->MakeFocus(true);
  fListView->SetEventMask(B_POINTER_EVENTS);
  float left,top;
  BScreen screen;

  if (screen.Frame().bottom>point.y+221)
   top=point.y+20;
  else
   top=point.y-210;

  if (screen.Frame().right>point.x+Bounds().right+1)
   left=point.x;
  else
   left=point.x-Bounds().right;

  MoveTo(left,top);
 }
 Unlock();
}

// ----------------------------------------------------------------------------------- RHTML_Tag_Menu - Stop -
void RHTMLTagMenu::Stop()
{
 Lock();
 fListView->SetEventMask(B_NO_POINTER_HISTORY);
 MoveTo(-Bounds().right-5,-Bounds().bottom-5);
 SetFeel(B_FLOATING_APP_WINDOW_FEEL);
 Activate(false);
 Unlock();
}

// ------------------------------------------------------------------------------- RHTML_Tag_Menu - LoadList -
void RHTMLTagMenu::LoadList(BString _str)
{
 fListView->MakeEmpty();
 float width=74;
 if (menutype==R_VALUE_MENU)
  _str=fListView->str;

 if (menutype==R_TAG_MENU) // - R_TAG_MENU
 {
  std::map<BString,Tag>::const_iterator tmp;
  for (tmp = fTag.begin();tmp!=fTag.end();tmp++)
  {
   if (tmp->second.text.ICompare(_str,_str.Length())==0)
   {
    fListView->AddItem(new BStringItem(tmp->second.text.String()));
    if (fListView->StringWidth(tmp->second.text.String())>width)
     width=fListView->StringWidth(tmp->second.text.String());
   }
  }
 }

 if (menutype==R_PARAM_MENU) // - R_PARAM_MENU
 {
  std::map<BString,Param>::const_iterator tmp;
  for (tmp = fTag[tag].param.begin();tmp!=fTag[tag].param.end();tmp++)
  {
   if (tmp->second.text.ICompare(_str,_str.Length())==0 && std::find(tags.begin(),tags.end(),tmp->first)==tags.end())
   {
    fListView->AddItem(new BStringItem(tmp->second.text.String()));
    if (fListView->StringWidth(tmp->second.text.String())>width)
     width=fListView->StringWidth(tmp->second.text.String());
   }
  }
 }

 if (menutype==R_VALUE_MENU) // - R_VALUE_MENU
 {
  std::map<BString,Value>::const_iterator tmp;
  for (tmp = fTag[tag].param[param].value.begin();tmp!=fTag[tag].param[param].value.end();tmp++)
  {
   if (tmp->first.ICompare(_str,_str.Length())==0)
   {
    fListView->AddItem(new BStringItem(tmp->first.String()));
    if (fListView->StringWidth(tmp->first.String())>width)
     width=fListView->StringWidth(tmp->first.String());
   }
  }
 }
 
 if (fListView->CountItems()>0)
 {
  fListView->Select(0);
  ResizeTo(width+26,Bounds().bottom);
 } 
}

// ------------------------------------------------------------------------------------ RHTML_Tag_Menu - Put -
void RHTMLTagMenu::Put()
{
 fTextView->Window()->Lock();
 BString _str;

 if (menutype!=R_VALUE_MENU)
  fListView->str.CopyInto(_str,1,fListView->str.Length()-1);
 else
  _str=fListView->str;

 fTextView->Delete(fListView->sel1,fListView->sel2);
 fTextView->Select(fListView->sel1,fListView->sel1);
 int32 cursor=fListView->sel1;
 int32 origcursor=fListView->sel1;
 BString oldstring(fTextView->Text());
 std::vector<Cmd> cmd;
 
 if (menutype==R_TAG_MENU)	 // - R_TAG_MENU
 {
  int32 i=0;
  std::map<BString,Tag>::const_iterator tmp;
  for (tmp = fTag.begin();tmp!=fTag.end();tmp++)
   if (tmp->first.ICompare(_str,_str.Length())==0)
   {
    if (i==fListView->CurrentSelection())
    {
     cmd=tmp->second.cmd;
    }
    i++;
   }
 }

 if (menutype==R_PARAM_MENU) // - R_PARAM_MENU
 {
  int32 i=0;
  std::map<BString,Param>::const_iterator tmp;
  for (tmp = fTag[tag].param.begin();tmp!=fTag[tag].param.end();tmp++)
   if (tmp->first.ICompare(_str,_str.Length())==0 && std::find(tags.begin(),tags.end(),tmp->first)==tags.end())
   {
    if (i==fListView->CurrentSelection())
    {
     cmd=tmp->second.cmd;
     BMessage *msg = new BMessage('VALE');
     msg->AddString("tag",tag);
     msg->AddString("param",tmp->first);
     fTextView->MessageReceived(msg);
    }
    i++;
   }
 }

 if (menutype==R_VALUE_MENU) // - R_VALUE_MENU
 {
  int32 i=0;
  std::map<BString,Value>::const_iterator tmp;
  for (tmp = fTag[tag].param[param].value.begin();tmp!=fTag[tag].param[param].value.end();tmp++)
   if (tmp->first.ICompare(_str,_str.Length())==0)
   {
    if (i==fListView->CurrentSelection())
    {
     cmd=tmp->second.cmd;
    }
    i++;
   }
 }
 
 std::vector<Cmd>::const_iterator tmp;
 for (tmp = cmd.begin();tmp!=cmd.end();tmp++)
 {
  if (tmp->type.ICompare("INSERTBACK")==0)
  {
   fTextView->Select(origcursor,origcursor);
   fTextView->Insert(tmp->param.String());
   cursor+=tmp->param.Length();
   origcursor=origcursor+tmp->param.Length();
  }
  if (tmp->type.ICompare("INSERTFOR")==0)
  {
   fTextView->Select(cursor,cursor);
   fTextView->Insert(tmp->param.String());
   cursor+=tmp->param.Length();
  }
  if (tmp->type.ICompare("NEWLINE")==0)
  {
   fTextView->Select(cursor,cursor);
   fTextView->Insert("\n");
   cursor++;
  }
  if (tmp->type.ICompare("SPECIAL")==0)
  {
   if (tmp->param.ICompare("#Color")==0)
   {
    fColorWin->Start(BPoint(Frame().left,Frame().top),fTextView);
   }
  }
  if (tmp->type.ICompare("PUT")==0)
  {
   BString tmp1,tmp2,tmp3;
   tmp1=tmp->param;
   int32 newline=-1;
   int32 pos=-1;
   int32 strpos=0;
   int32 pluspos=0;

   newline=tmp1.FindFirst("\\n",strpos);
   pos=tmp1.FindFirst("%c",strpos);
   if ((pos>=0) && (newline>=0))
   {
    if (pos>newline)
    {
     pos=-1;
     pluspos++;
    }
    else
    {
     newline=-1;
    }
   }

   while (pos>=0 || newline>=0)
   {
    if (newline>=0)
    {
     fTextView->Insert((tmp1.CopyInto(tmp2,strpos,newline-strpos)).String());
     int32 c,d;
     fTextView->GetSelection(&c,&d);
     BString temp="";
     if (d-fTextView->OffsetAt(fTextView->CurrentLine())>0)
     {
      for (int i=fTextView->OffsetAt(fTextView->CurrentLine());i<d;i++)
      {
       if (fTextView->Text()[i]==B_SPACE||fTextView->Text()[i]==B_TAB)
       {
        temp << fTextView->Text()[i];
       } else break;
      }
     }
     fTextView->Insert("\n");
     fTextView->Insert(temp.String());
     tmp1.CopyInto(tmp2,0,newline);
     tmp1.CopyInto(tmp3,newline+2,tmp1.Length()-newline-2);
     tmp1=tmp2;
     tmp1+=tmp3;
     cursor+=newline-strpos+temp.Length()+1;
     strpos=newline;
    }
    if (pos>=0)
    {
     fTextView->Insert((tmp1.CopyInto(tmp2,strpos,pos-strpos)).String());
     tmp1.CopyInto(tmp2,0,pos);
     tmp1.CopyInto(tmp3,pos+2,tmp1.Length()-pos-2);
     tmp1=tmp2;
     tmp1+=tmp3;
     cursor+=pos-strpos+pluspos;
     strpos=pos;
     origcursor+=pos+pluspos;
    }
    newline=-1;
    pos=-1;
    newline=tmp1.FindFirst("\\n",strpos);
    pos=tmp1.FindFirst("%c",strpos);
    if ((pos>=0) && (newline>=0))
    {
     if (pos>newline)
     {
      pos=-1;
      pluspos++;
     }
     else
     {
      newline=-1;
     }
    }
   }
   fTextView->Insert((tmp1.CopyInto(tmp2,strpos,tmp1.Length()-strpos)).String());
   cursor+=tmp1.Length()-strpos;
  }
 }
 fTextView->Select(origcursor,origcursor);
 BMessage *msg= new BMessage('PTFC');
 msg->AddInt32("sel1",fListView->sel1);
 msg->AddInt32("sel2",cursor);
 msg->AddString("oldstring",oldstring);
 fTextView->MessageReceived(msg);
 Stop();
 fTextView->MessageReceived(new BMessage('UPPM'));
 fTextView->Window()->Unlock();
}

// ------------------------------------------------------------------------ RHTML_Tag_Menu - MessageReceived -
void RHTMLTagMenu::MessageReceived(BMessage *msg)
{
 switch (msg->what)
 {
  case 'SEL':
   Put();
  break;  
  default:
  {
   BWindow::MessageReceived(msg);
  }
  break;
 }
}

// ------------------------------------------------------------------ RHTML_Tag_Menu_List - RHTMLTagMenuList -
RHTMLTagMenuList::RHTMLTagMenuList(RHTMLTagMenu *tagmenu, BTextView *textview) : BListView(BRect(1,1,tagmenu->Bounds().right-15,tagmenu->Bounds().bottom-1),"TagMenuList",B_SINGLE_SELECTION_LIST,B_FOLLOW_ALL)
{
 fTagMenu=tagmenu;
 fTextView=textview;
}

// ------------------------------------------------------------------------- RHTML_Tag_Menu_List - MouseDown -
void RHTMLTagMenuList::MouseDown(BPoint where)
{
 if (where.x<Bounds().left || where.x>Bounds().right || where.y<Bounds().top || where.y>Bounds().bottom)
 {
  fTagMenu->Stop();
 } else
 BListView::MouseDown(where);
}


// --------------------------------------------------------------------------- RHTML_Tag_Menu_List - KeyDown -
void RHTMLTagMenuList::KeyDown(const char *bytes, int32 numBytes)
{
 fTextView->Window()->Lock();
 switch(bytes[0])
 {
  case B_ESCAPE:		// - ESC
  {
   fTagMenu->Stop();
  }
  break;
  case '>':				// - >
  {
   fTextView->Select(sel2,sel2);
   fTextView->Insert(">");
   fTagMenu->Stop();
  }
  break;
  case '=':				// - =
  {
   fTextView->Select(sel2,sel2);
   fTextView->Insert("=");
   fTagMenu->Stop();
  }
  break;
  case B_SPACE:			// - SPACE
  {
   fTextView->Select(sel2,sel2);
   fTextView->Insert(" ");
   fTagMenu->Stop();
  }
  break;
  case 0x08:			// - BackSpace
  {
   if (str.Length()>0)
   {
    str.Truncate(str.Length()-1);
    fTextView->Delete(sel1,sel2);
    fTextView->Select(sel1,sel1);
    fTextView->Insert(str.String());
    sel2=sel1+str.Length();
    BString tmp = "";
    if (str.Length()>1)
     str.CopyInto(tmp,1,str.Length()-1);
    fTagMenu->LoadList(tmp);
   }
   if (str.Length()==0)
    fTagMenu->Stop();
  }
  break;
  case 0x1c:			// - Left
   fTagMenu->Stop();
  break; 
  case 0x1d:			// - Right
   fTagMenu->Stop();
  break;
  case 0x1e:			// - Up
   BListView::KeyDown(bytes,numBytes);
  break;
  case 0x1f:			// - Down
   BListView::KeyDown(bytes,numBytes);
  break;
  case 0x01:			// - Home
   BListView::KeyDown(bytes,numBytes);
  break;
  case 0x04:			// - End
   BListView::KeyDown(bytes,numBytes);
  break;
  case 0x0b:			// - PageUp
  {
   if (CurrentSelection()>13) Select(CurrentSelection()-13,false); else
    Select(0,false);
   ScrollToSelection();
  } 
  break;
  case 0x0c:			// - PageDown
  {
   if (CurrentSelection()<CountItems()-14) Select(CurrentSelection()+13,false); else
    Select(CountItems()-1,false);

   ScrollToSelection();
  } 
  break;
  case B_ENTER:			// - Enter
  {
   fTextView->Window()->Unlock();
   fTagMenu->Put();
   fTextView->Window()->Lock();
  }
  break;
  default:				// - Default
  {
   str+=bytes[0];
   fTextView->Delete(sel1,sel2);
   fTextView->Select(sel1,sel1);
   fTextView->Insert(str.String());
   sel2=sel1+str.Length();
   BString tmp;
   str.CopyInto(tmp,1,str.Length()-1);
   fTagMenu->LoadList(tmp);
  }
 }
 fTextView->MessageReceived(new BMessage('UPPM'));
 fTextView->Window()->Unlock();
}

// - End - TagMenu - Source File -----------------------------------------------------------------------------
