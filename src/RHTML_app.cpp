//////////////////////////////////////////////////////////////////////
//      Globe Webeditor © Gergely Rózsahegyi (RoGer) 2001-2002      //
//                                                                  //
//                       Application Source File                    //
//                                                                  //
//                       Last Update: 2002.05.18					//
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
#include <Path.h>
#include <stdio.h>
#include "RHTML_app.h"
#include "RHTML_win.h"

// ---------------------------------------------------------------------------------------------------- main -
int main(int argc,char* argv[])
{ 
 char **tmp;
 tmp = (char**) calloc(argc,sizeof(char*));
 for (int i=0;i<argc;i++) tmp[i]=argv[i];
 RHTMLApp* app = new RHTMLApp(argc,tmp);
 app->Run();
 return(0);
}
// - End - main ----------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------ RHTML_App - RHTMLApp -
RHTMLApp::RHTMLApp(int argc,char **argv) : BApplication(RHTML_APP_SIG)
{
 fWindow = new RHTMLWin(BRect(40,60,800,600),argc,argv);
 fWindow->Show();
 fToolTip = new TToolTip();
}
// - End - RHTML_App - RHTMLApp ------------------------------------------------------------------------------

// -------------------------------------------------------------------------------- RHTML_App - RefsReceived -
void RHTMLApp::RefsReceived(BMessage* msg)
{
 entry_ref ref;
 type_code type;
 int32 count;
 msg->GetInfo("refs", &type, &count);
 if(type != B_REF_TYPE) 
  return;

 int32 ref_num;
 BPath path;
 status_t err;
 BEntry entry;
 ref_num = 0;
 do
 {
  if ((err = msg->FindRef("refs", ref_num, &ref)) != B_OK) { return; }
  if ((err=entry.SetTo(&ref)) == B_OK)
  {
   entry.GetPath(&path);
   fWindow->Lock();
   fWindow->Open(ref.name,path.Path());
   fWindow->Unlock();
  } 
  ref_num++;
 }
 while (1);

}
// - End - RHTML_App - RefsReceived --------------------------------------------------------------------------

// ----------------------------------------------------------------------------- RHTML_App - MessageReceived -
void RHTMLApp::MessageReceived(BMessage *msg)
{
 switch (msg->what)
 {
  case B_CANCEL:
  {
   fWindow->MessageReceived(msg);
   if (fWindow->IsLocked())fWindow->Unlock();
  }
  break;
  case 'LOCK':
  {
   fWindow->Lock();
  }
  break;
  case 'SAV':
  {
   if (!fWindow->IsLocked()) fWindow->Lock();
   fWindow->MessageReceived(msg);
   if (fWindow->IsLocked()) fWindow->Unlock();
  }
  break;
  case 'OPN':
  {
   if (!fWindow->IsLocked()) fWindow->Lock();
   fWindow->MessageReceived(msg);
   if (fWindow->IsLocked()) fWindow->Unlock();
  }
  break;
  case eToolTipStart:
  {
   msg->AddInt32("WorkSpaces",fWindow->Workspaces());
   fToolTip->PostMessage(msg);
  }
  break;
  case eToolTipStop:
  {
   fToolTip->PostMessage(msg);
  }
  break;
  case 'SET ':
  {
   tool_tip_settings *settings;
   ssize_t len;
   msg->FindData("settings", B_RAW_TYPE,(const void **)&settings,&len);
   fToolTip->SetSettings(settings);
  }
  break;   
  default:
  {
   BApplication::MessageReceived(msg);
  }
  break;
 }
}
// - End - RHTML_App - MessageReceived -----------------------------------------------------------------------

// - End - Application - Source File -------------------------------------------------------------------------
