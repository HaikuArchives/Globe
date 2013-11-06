//////////////////////////////////////////////////////////////////////
//      Globe Webeditor © Gergely Rózsahegyi (RoGer) 2001-2003      //
//                                                                  //
//                       fmenu View  Source File                    //
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
#include <Bitmap.h>
#include <TranslationUtils.h>
#include <Alert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "RHTML_fmenu.h"

RHTMLfmenu::RHTMLfmenu(BRect fRect): BToolBar("name",BRect(10,2,2,2),BRect(2,10,2,2),2)
{

// SetViewColor(216,216,216);
 SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

 buttondb=0;
 x=5;
 buttons = (ButtonView**) calloc(1,sizeof(ButtonView));

 BString dir=fOptions->SettingsDir;
 dir << "menu/menu.rdat";
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
  
   if (str2.ICompare("<BUTTON")==0)
   {
    i=str1.FindFirst('<')+1;
    BString menu,img,img2,alt,type;
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

     if (str3.ICompare("IMG")==0) img.SetTo(str4.String());
     if (str3.ICompare("IMG2")==0) img2.SetTo(str4.String());
     if (str3.ICompare("TYPE")==0) type.SetTo(str4.String());
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
         (str3.ICompare("RUN")==0)||
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
     BString tmp1 = fOptions->SettingsDir;
     tmp1 << "menu/pic/";
     BString tmp2 = tmp1;
     tmp1 << img;
     tmp2 << img2;
     if (type.ICompare("SAVEBUTTON")!=0 && type.ICompare("SAVEALLBUTTON")!=0 && type.ICompare("CLOSEBUTTON")!=0)
      tmp2=tmp1;
     ButtonView *fItem = new ButtonView(type.String(),tmp1.String(),tmp2.String(),new BMessage('MUDN'),alt.String());
     AddToolItem(fItem);
    for (int p=0;p<cmddb;p++)
    {
     fItem->AddCmd(cmd[p].String(),param[p].String());
    }
  }
 }
 fclose(f);
}

}

void RHTMLfmenu::SetOrientation(orient ori)
{
 fOptions->TopViewOrient=ori;
 fOptions->Save();
 BToolBar::SetOrientation(ori);
}

void RHTMLfmenu::SetIndex(uint32 index)
{
 fOptions->TopViewIndex=index;
 fOptions->Save();
 BToolBar::SetIndex(index);
}

// --- Button View
ButtonView::ButtonView(const char *name,const char *filename1, const char *filename2,BMessage *msg,const char *alt): BCheckItem(name,filename1,filename2,msg)
{

// fCMD = new BString(cmd);
// fParam = new BString(param);
 fAlt = new BString(alt);
 	if (fIn)
		be_app->PostMessage(eToolTipStop);
	// free cached ToolTip
 fIn = false;
 cmddb=0;
}

void ButtonView::AddCmd(const char *cmd,const char *param)
{
 if (cmddb<9)
 {
  cmddb++;
  fCmd[cmddb-1].SetTo(cmd);
  fParam[cmddb-1].SetTo(param);
 }
}

void ButtonView::MouseMoved(BPoint point, uint32 transit,const BMessage *message)
{
 if (Enabled())
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
 }
 BImageItem::MouseMoved(point, transit, message);
}

void RHTMLfmenu::MessageReceived(BMessage *msg)
{
 switch (msg->what)
  {
   case 'BMTF':
   {
    int8 tmp;
    tmp=0;
    msg->FindInt8("button",&tmp);
    buttons[tmp]->MessageReceived(new BMessage('BMTC'));
   }
   break;
  default:
   {
    BToolBar::MessageReceived(msg);
   }
   break;
  }
}

void ButtonView::MessageReceived(BMessage *msg)
{
 switch (msg->what)
  {
   case 'MUDN':
   {
    for (int i=0;i<cmddb;i++)
    {
     BMessage *msg= new BMessage('Cmd');
     msg->AddString("cmd",fCmd[i].String());
     msg->AddString("param",fParam[i].String());
     MessageReceived(msg);
    }
   }
   break;
  default:
   {
    BImageItem::MessageReceived(msg);
   }
   break;
  }
}
