//////////////////////////////////////////////////////////////////////
//      Globe Webeditor © Gergely Rózsahegyi (RoGer) 2001-2002      //
//                                                                  //
//                          AMenu Source File                       //
//                                                                  //
//                       Last Update: 2002.11.27					//
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
#include <stdio.h>
#include <stdlib.h>
#include "RHTML_amenu.h"

// -------------------------------------------------------------------------------- RHTML_amenu - RHTMLamenu -
RHTMLamenu::RHTMLamenu(BRect fRect): BToolBar("name")
{
 tbdb=0;
 tb = (RHTMLtabbutton**) calloc(1,sizeof(RHTMLtabbutton));
}
// - End - RHTML_amenu - RHTMLamenu --------------------------------------------------------------------------

// ---------------------------------------------------------------------------- RHTML_amenu - SetOrientation -
void RHTMLamenu::SetOrientation(orient ori)
{
 fOptions->BottomViewOrient=ori;
 fOptions->Save();
 BToolBar::SetOrientation(ori);
}

// ---------------------------------------------------------------------------------- RHTML_amenu - SetIndex -
void RHTMLamenu::SetIndex(uint32 index)
{
 fOptions->BottomViewIndex=index;
 fOptions->Save();
 BToolBar::SetIndex(index);
}

// ------------------------------------------------------------------------------------ RHTML_amenu - AddTab -
void RHTMLamenu::AddTab(const char *name)
{
 tbdb++;
 tb = (RHTMLtabbutton**) realloc(tb,sizeof(RHTMLtabbutton)*tbdb);
 tb[tbdb-1] = new RHTMLtabbutton(BRect(10+(tbdb-1)*110,2,110+(tbdb-1)*110,Bounds().bottom-1),name,tbdb-1,this);
 AddChild(tb[tbdb-1]);
}
// - End - RHTML_amenu - AddTab ------------------------------------------------------------------------------

// --------------------------------------------------------------------------------- RHTML_amenu - RemoveTab -
void RHTMLamenu::RemoveTab(uint8 num)
{
 for(int i=num+1;i<tbdb;i++)
 {
  tb[i]->MoveTo(BPoint(10+(i-1)*110,2));//2 100
  tb[i]->SetNum(i-1);
 }
 tbdb--;
 RemoveChild(tb[num]);
 for(int i=num;i<tbdb;i++) tb[i]=tb[i+1];
 tb = (RHTMLtabbutton**) realloc(tb,sizeof(RHTMLtabbutton)*tbdb);
}
// - End - RHTML_amenu - RemoveTab ---------------------------------------------------------------------------

// --------------------------------------------------------------------------------- RHTML_amenu - ChangeTab -
void RHTMLamenu::ChangeTab(uint8 num)
{
 for(int i=0;i<tbdb;i++) tb[i]->SetOFF();
 tb[num]->SetON();
}
// - End - RHTML_amenu - ChangeTab ---------------------------------------------------------------------------

// ----------------------------------------------------------------------------------- RHTML_amenu - Changed -
void RHTMLamenu::Changed(uint8 num)
{
 BString tmp;
 tmp.SetTo("");
 tmp << (int) num;

 BMessage msg('Cmd');
 msg.AddString("cmd","ChangeText");
 msg.AddString("param",tmp.String());
 MessageReceived(&msg);
}
// - End - RHTML_amenu - Changed -----------------------------------------------------------------------------

// ----------------------------------------------------------------------------------- RHTML_amenu - Removed -
void RHTMLamenu::Removed(uint8 num)
{
 BString tmp;
 tmp.SetTo("");
 tmp << (int) num;

 BMessage msg('Cmd');
 msg.AddString("cmd","CLOSE");
 msg.AddString("param",tmp.String());
 MessageReceived(&msg);
}
// - End - RHTML_amenu - Removed -----------------------------------------------------------------------------

// -------------------------------------------------------------------------------- RHTML_amenu - SetTabText -
void RHTMLamenu::SetTabText(uint8 num,const char *text)
{
 tb[num]->SetText(text);
}
// - End - RHTML_amenu - SetTabText --------------------------------------------------------------------------

// -------------------------------------------------------------------------------- RHTML_amenu - GetTabText -
const char * RHTMLamenu::GetTabText(uint8 num)
{
 return tb[num]->GetText();
}
// - End - RHTML_amenu - GetTabText --------------------------------------------------------------------------

// --------------------------------------------------------------------------- RHTML_amenu - MessageReceived -
void RHTMLamenu::MessageReceived(BMessage *msg)
{
 switch (msg->what)
  {
   case 'EXIT':
   {
   }
   break;
  default:
   {
    BToolBar::MessageReceived(msg);
   }
   break;
  }
}
// - End - RHTML_amenu - MessageReceived ---------------------------------------------------------------------

// - End - AMenu - Source File -------------------------------------------------------------------------------
