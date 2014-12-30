//////////////////////////////////////////////////////////////////////
//      Globe Webeditor © Gergely Rózsahegyi (RoGer) 2001-2003      //
//                                                                  //
//                         Window  Source File                      //
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
#include <stdlib.h>
#include <stdio.h>
#include <Path.h>
#include <File.h>
#include <Entry.h>
#include <UTF8.h>
#include <String.h>
#include <string.h>
#include <Alert.h>
#include <Font.h>
#include <Beep.h>
#include "RHTML_win.h"

// - End - RHTML_Win - RHTMLWin ------------------------------------------------------------------------------
RHTMLWin::RHTMLWin(BRect fRect, int argc, char **argv): BWindow(fRect, "RHTML", B_TITLED_WINDOW, B_UNTYPED_WINDOW)
{
	Options = new RHTMLOptions();
	fSaveWinShow = false;
	fPrefWinShow = false;
	fAboutWinShow = false;
	savequit = false;
	saveclose = false;
	saverun = false;
	Version[0] = 0;
	strcpy(Version, fOptions->GetVer());
	strcat(Version, " (");
	strcat(Version, fOptions->GetDate());
	strcat(Version, ")");
	strcpy(Name, fOptions->GetName());
	strcpy(Web, "http://www.beos.hu/globe");
	strcpy(Email1, "roger@beos.hu");
	strcpy(Email2, "kesigomu@beos.hu");
	char tmp[50];
// sprintf(tmp,"%s v%s",Name,Version);
	sprintf(tmp, "%s", Name);
	SetTitle(tmp);
	fUpdateTimer = new RTimer("fWinTimer", this, new BMessage('TIMR'));
	MoveTo(fOptions->WinFrameLeft, fOptions->WinFrameTop);
	ResizeTo(fOptions->WinFrameRight - fOptions->WinFrameLeft, fOptions->WinFrameBottom - fOptions->WinFrameTop);
	BMenuBar *fMenuBar;
	BMenu *fMenu;
	BMenu *fSubMenu;
	BMenuItem *item;

	fMenuBar = new BMenuBar(BRect(0,0,100,300),"RHTMLMenuBar",B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP);

	fMenu = new BMenu(fOptions->GetLocaleString("Globe_Window_Menu","File"));
	fMenu->AddItem(item=new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_File","New"),new BMessage('NEW'),'N'));
	fMenu->AddItem(item=new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_File","Open"),new BMessage('OPEN'),'O'));
	fMenu->AddSeparatorItem();
	fMenu->AddItem(save=new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_File","Save"),new BMessage('SAVE'),'S'));
	fMenu->AddItem(save_as=new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_File","Save As"),new BMessage('SAVA')));
	fMenu->AddItem(save_all=new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_File","Save All"),new BMessage('SAVL')));
	fMenu->AddItem(close=new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_File","Close File"),new BMessage('REM'),'R'));
	fMenu->AddSeparatorItem();
	fMenu->AddItem(item=new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_File","Quit"),new BMessage('QUIT'),'Q'));
	fMenuBar->AddItem(fMenu);

	fMenu = new BMenu(fOptions->GetLocaleString("Globe_Window_Menu","Edit"));
	fMenu->AddItem(undo=new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_Edit","Undo"), new BMessage('UNDO'),'Z'));
	fMenu->AddSeparatorItem();
	fMenu->AddItem(cut=new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_Edit","Cut"), new BMessage('CUT')));
	fMenu->AddItem(copy=new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_Edit","Copy"), new BMessage('COPY')));
	fMenu->AddItem(paste=new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_Edit","Paste"), new BMessage('PATE')));
	fMenu->AddSeparatorItem();
	fMenu->AddItem(selectall=new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_Edit","Select All"), new BMessage('SLAL')));
	fMenuBar->AddItem(fMenu);

	fMenu = new BMenu(fOptions->GetLocaleString("Globe_Window_Menu", "Search"));
	fMenu->AddItem(undo = new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_Search", "Find"), new BMessage('FIND'), 'F'));
	fMenu->AddItem(cut = new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_Search", "Find Next"), new BMessage('FINE'), 'G'));
	fMenu->AddSeparatorItem();
	fMenu->AddItem(copy = new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_Search","Replace"), new BMessage('RELA'), 'R'));
	fMenu->AddItem(paste = new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_Search","Replace & Find"), new BMessage('RAFN')));
	fMenu->AddItem(paste = new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_Search","Replace All"), new BMessage('RLAL')));
	fMenu->AddSeparatorItem();
	fMenu->AddItem(paste = new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_Search","Go To Line"), new BMessage('MGTO'), ','));
	fMenuBar->AddItem(fMenu);

  fMenu = new BMenu(fOptions->GetLocaleString("Globe_Window_Menu","Options"));
   fMenu->AddItem(fWordWrap=new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_Options","WordWrap"),new BMessage('STWW')));
   fWordWrap->SetMarked(true);

   fSubMenu = new BMenu(fOptions->GetLocaleString("Globe_Window_Menu_Options","View"));
    fSubMenu->AddItem(fTB=new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_Options_View","Top Bar"), new BMessage('TB')));
    fSubMenu->AddItem(fLB=new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_Options_View","Left Bar"), new BMessage('LB')));
    fSubMenu->AddSeparatorItem();
    fSubMenu->AddItem(fNW=new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_Options_View","NumView"), new BMessage('NW')));
    fTB->SetMarked(true);
    fLB->SetMarked(true);
    fNW->SetMarked(true);

 if (!fOptions->TopView) MessageReceived(new BMessage('TW'));
 if (!fOptions->LeftView) {
 MessageReceived(new BMessage('LW'));
 }
 if (!fOptions->NumView) MessageReceived(new BMessage('NW'));
 if (!fOptions->WordWrap) MessageReceived(new BMessage('STWW'));
    
   fMenu->AddItem(fSubMenu);

   fSubMenu = new BMenu(fOptions->GetLocaleString("Globe_Window_Menu_Options","Encoding"));
    fSubMenu->AddItem(fCE[0]=new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_Options_Encoding","EUC Packed Japanese"), new BMessage('CE0')));
    fSubMenu->AddItem(fCE[1]=new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_Options_Encoding","EUC Korean"), new BMessage('CE1')));
    fSubMenu->AddItem(fCE[2]=new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_Options_Encoding","ISO 8859-1"), new BMessage('CE2')));
    fSubMenu->AddItem(fCE[3]=new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_Options_Encoding","ISO 8859-2"), new BMessage('CE3')));
    fSubMenu->AddItem(fCE[4]=new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_Options_Encoding","ISO 8859-3"), new BMessage('CE4')));
    fSubMenu->AddItem(fCE[5]=new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_Options_Encoding","ISO 8859-4"), new BMessage('CE5')));
    fSubMenu->AddItem(fCE[6]=new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_Options_Encoding","ISO 8859-5"), new BMessage('CE6')));
    fSubMenu->AddItem(fCE[7]=new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_Options_Encoding","ISO 8859-6"), new BMessage('CE7')));
    fSubMenu->AddItem(fCE[8]=new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_Options_Encoding","ISO 8859-7"), new BMessage('CE8')));
    fSubMenu->AddItem(fCE[9]=new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_Options_Encoding","ISO 8859-8"), new BMessage('CE9')));
    fSubMenu->AddItem(fCE[10]=new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_Options_Encoding","ISO 8859-9"), new BMessage('CE10')));
    fSubMenu->AddItem(fCE[11]=new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_Options_Encoding","ISO 8859-10"), new BMessage('CE11')));
    fSubMenu->AddItem(fCE[12]=new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_Options_Encoding","ISO 8859-13"), new BMessage('CE12')));
    fSubMenu->AddItem(fCE[13]=new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_Options_Encoding","ISO 8859-14"), new BMessage('CE13')));
    fSubMenu->AddItem(fCE[14]=new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_Options_Encoding","ISO 8859-14"), new BMessage('CE14')));
    fSubMenu->AddItem(fCE[15]=new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_Options_Encoding","JIS X 0208-1990"), new BMessage('CE15')));
    fSubMenu->AddItem(fCE[16]=new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_Options_Encoding","KOI8-R"), new BMessage('CE16')));
    fSubMenu->AddItem(fCE[17]=new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_Options_Encoding","Macintosh Roman"), new BMessage('CE17')));
    fSubMenu->AddItem(fCE[18]=new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_Options_Encoding","MS-DOS Codepage 866"), new BMessage('CE18')));
    fSubMenu->AddItem(fCE[19]=new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_Options_Encoding","MS-DOS Codepage 437"), new BMessage('CE19')));
    fSubMenu->AddItem(fCE[20]=new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_Options_Encoding","MS-Windows Codepage 1252"), new BMessage('CE20')));
    fSubMenu->AddItem(fCE[21]=new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_Options_Encoding","MS-Windows Codepage 1251"), new BMessage('CE21')));
    fSubMenu->AddItem(fCE[22]=new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_Options_Encoding","Unicode 2.0"), new BMessage('CE22')));
    fSubMenu->AddItem(fCE[23]=new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_Options_Encoding","UTF8"), new BMessage('CE23')));
    fSubMenu->AddItem(fCE[24]=new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_Options_Encoding","Shift-JIS"), new BMessage('CE24')));
    fSubMenu->AddSeparatorItem();
    fSubMenu->AddItem(fCRLF_LF=new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_Options_Encoding","CRLF->LF"), new BMessage('toro')));
    fSubMenu->AddItem(fLF_CRLF=new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_Options_Encoding","LF->CRLF"), new BMessage('tors')));
    if (fOptions->Encoding==B_EUC_CONVERSION) fCE[0]->SetMarked(true);
    if (fOptions->Encoding==B_EUC_KR_CONVERSION) fCE[1]->SetMarked(true);
    if (fOptions->Encoding==B_ISO1_CONVERSION) fCE[2]->SetMarked(true);
    if (fOptions->Encoding==B_ISO2_CONVERSION) fCE[3]->SetMarked(true);
    if (fOptions->Encoding==B_ISO3_CONVERSION) fCE[4]->SetMarked(true);
    if (fOptions->Encoding==B_ISO4_CONVERSION) fCE[5]->SetMarked(true);
    if (fOptions->Encoding==B_ISO5_CONVERSION) fCE[6]->SetMarked(true);
    if (fOptions->Encoding==B_ISO6_CONVERSION) fCE[7]->SetMarked(true);
    if (fOptions->Encoding==B_ISO7_CONVERSION) fCE[8]->SetMarked(true);
    if (fOptions->Encoding==B_ISO8_CONVERSION) fCE[9]->SetMarked(true);
    if (fOptions->Encoding==B_ISO9_CONVERSION) fCE[10]->SetMarked(true);
    if (fOptions->Encoding==B_ISO10_CONVERSION) fCE[11]->SetMarked(true);
    if (fOptions->Encoding==B_ISO13_CONVERSION) fCE[12]->SetMarked(true);
    if (fOptions->Encoding==B_ISO14_CONVERSION) fCE[13]->SetMarked(true);
    if (fOptions->Encoding==B_ISO15_CONVERSION) fCE[14]->SetMarked(true);
    if (fOptions->Encoding==B_JIS_CONVERSION) fCE[15]->SetMarked(true);
    if (fOptions->Encoding==B_KOI8R_CONVERSION) fCE[16]->SetMarked(true);
    if (fOptions->Encoding==B_MAC_ROMAN_CONVERSION) fCE[17]->SetMarked(true);
    if (fOptions->Encoding==B_MS_DOS_866_CONVERSION) fCE[18]->SetMarked(true);
    if (fOptions->Encoding==B_MS_DOS_CONVERSION) fCE[19]->SetMarked(true);
    if (fOptions->Encoding==B_MS_WINDOWS_CONVERSION) fCE[20]->SetMarked(true);
    if (fOptions->Encoding==B_MS_WINDOWS_1251_CONVERSION) fCE[21]->SetMarked(true);
    if (fOptions->Encoding==B_UNICODE_CONVERSION) fCE[22]->SetMarked(true);
    if (fOptions->Encoding==B_UTF8_CONVERSION) fCE[23]->SetMarked(true);
    if (fOptions->Encoding==B_SJIS_CONVERSION) fCE[24]->SetMarked(true);

    fCRLF_LF->SetMarked(fOptions->CRLF_LF);
    fLF_CRLF->SetMarked(fOptions->LF_CRLF);
   fMenu->AddItem(fSubMenu);
  
    fMenu->AddItem(new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_Options","Preferences"), new BMessage('Pref')));

   fMenuBar->AddItem(fMenu);
  fMenu = new BMenu(fOptions->GetLocaleString("Globe_Window_Menu","Help"));
   fMenu->AddItem(item=new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_Help","Reference"),new BMessage('Refe')));
   fMenu->AddItem(item=new BMenuItem(fOptions->GetLocaleString("Globe_Window_Menu_Help","About"),new BMessage('Abo')));
  fMenuBar->AddItem(fMenu);
  AddChild(fMenuBar);	

// - new
 fView = new BView(BRect(0,0,1,1),"TextView",B_FOLLOW_ALL,B_FULL_UPDATE_ON_RESIZE|B_WILL_DRAW);
 fToolView = new BToolView(BRect(0,fMenuBar->Frame().bottom+1,Bounds().right,Bounds().bottom),fView);
 AddChild(fToolView);

 fmenu = new RHTMLfmenu(BRect(0,fMenuBar->Bounds().bottom+1,0,fMenuBar->Bounds().bottom+19));
 fmenu->SetAutoResize(false);
 fmenu->SetAlignCenter(true);
 fmenu->SetEnabledOrientation(true,true,false,false);

 buttonmenu = new RHTMLbuttonmenu(BRect(fmenu->GetLast()+2,0,800,120),fmenu);
 fmenu->AddChild(buttonmenu);			

 amenu = new RHTMLamenu(BRect(0,Bounds().bottom-20,Bounds().right,Bounds().bottom));
 amenu->SetHorizontalHeight(20);
 amenu->SetEnabledOrientation(true,true,false,false);

 if (fOptions->TopViewIndex==0)
 {
  fToolView->AddToolBar(fmenu,fOptions->TopViewOrient);
  fToolView->AddToolBar(amenu,fOptions->BottomViewOrient);
 } else
 {
  fToolView->AddToolBar(amenu,fOptions->BottomViewOrient);
  fToolView->AddToolBar(fmenu,fOptions->TopViewOrient);
 }

 bmenu = new RHTMLbmenu(BRect(0,fmenu->Frame().bottom+1,180,amenu->Frame().top-1),this);
 fToolView->AddToolBar(bmenu,fOptions->LeftViewOrient);
 bmenu->SetEnabledOrientation(false,false,true,true);
		
 textdb=0;
 current=0;
 untitled=0;
 textview = (RHTMLtext**) calloc(1,sizeof(RHTMLtext*));
 AddText("",1);
 ChangeText(0);
 
 fSavePanel = new BFilePanel(B_SAVE_PANEL, new BMessenger(be_app), NULL,
				B_FILE_NODE, false,new BMessage('SAV'));

 fOpenPanel = new BFilePanel(B_OPEN_PANEL, new BMessenger(be_app), NULL,
				  B_FILE_NODE, false,new BMessage('OPN'));

 fSavePanel->SetPanelDirectory(fOptions->DefaultFolder.String());
 fOpenPanel->SetPanelDirectory(fOptions->DefaultFolder.String());
 saveall=false;
 
 
 if (!fOptions->LeftView) MessageReceived(new BMessage('LB'));
 if (!fOptions->TopView) MessageReceived(new BMessage('TB'));

 for (int i=1;i<argc;i++)
 {
  BString tmp1,tmp2;
  tmp1.SetTo(argv[i]);
  if (tmp1.FindLast('/')!=-1)
  {
   tmp1.CopyInto(tmp2,tmp1.FindLast('/')+1,tmp1.Length()-tmp1.FindLast('/')-1);
  } else
  {
   tmp2=tmp1;
  }
  Open(tmp2.String(),argv[i]);
 }
}
// - End - RHTML_Win - RHTMLWin ------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------- RHTML_Win - AddText -
void RHTMLWin::AddText(const char *name,uint ft)
{
 textdb++;
 textview = (RHTMLtext**) realloc(textview,sizeof(RHTMLtext*)*textdb);
// textview[textdb-1] = new RHTMLtext(BRect(bmenu->Frame().right+1,fmenu->Frame().bottom+1,Bounds().right,amenu->Frame().top),this,ft,fOptions);
 textview[textdb-1] = new RHTMLtext(fView->Bounds(),this,ft,bmenu->fParamMenuView);
 fView->AddChild(textview[textdb-1]);
 if (name=="") 
 {
  untitled++;
  char a[10];
  sprintf(a,"%d",untitled);  
  BString buf="";
  buf << fOptions->GetLocaleString("New_File_Name","untitled") << a << ".html";
  amenu->AddTab(buf.String());
 } else amenu->AddTab(name);
 ChangeText(textdb-1);
 textview[textdb-1]->fFileName->SetTo("");
 if (textdb>0)
 {
  close->SetEnabled(true);
 }
}
// - End - RHTML_Win - AddText -------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------- RHTML_Win - RemoveText -
void RHTMLWin::RemoveText(uint8 num)
{
	if (textdb>0)
	{
		textdb--;
		fView->RemoveChild(textview[num]);
		delete textview[num];
		for(int i = num; i < textdb; i++)
			textview[i] = textview[i + 1];
		textview = (RHTMLtext**) realloc(textview,sizeof(RHTMLtext*)*textdb);
		amenu->RemoveTab(num);
		if (textdb>0) ChangeText(textdb-1);
	}
	if (textdb<=0)
	{
		close->SetEnabled(false);
	}
}
// - End - RHTML_Win - RemoveText ----------------------------------------------------------------------------

// ---------------------------------------------------------------------------------- RHTML_Win - ChangeText -
void RHTMLWin::ChangeText(uint8 num)
{
 for (uint i=0;i<textdb;i++)
 {
  textview[i]->SetEnabled(false);
  textview[i]->ResizeTo(0,0);
 }
 textview[num]->ResizeTo(fView->Bounds().right,fView->Bounds().bottom);
 textview[num]->fTextView->MakeFocus(true);
 textview[num]->SetEnabled(true);
 textview[num]->fTextView->UpdateParamMenu();
 amenu->ChangeTab(num);
 current=num;
 copy->SetTarget(textview[num]->fTextView);
 cut->SetTarget(textview[num]->fTextView);
// cut->SetTarget(this);
 undo->SetTarget(textview[num]->fTextView);
 paste->SetTarget(textview[num]->fTextView);
 selectall->SetTarget(textview[num]->fTextView);
 MessageReceived(new BMessage('RTMM'));
}
// - End - RHTML_Win - ChangeText ----------------------------------------------------------------------------

// -------------------------------------------------------------------------------- RHTML_Win - FrameResized -
void RHTMLWin::FrameResized(float new_width, float new_height)
{
 for (uint i=0;i<textdb;i++) textview[i]->ResizeTo(0,0);

// if (fLB->IsMarked())
//  textview[current]->ResizeTo(Bounds().right-bmenu->Bounds().right-1,amenu->Frame().top-fmenu->Frame().bottom-2);
// else
//  textview[current]->ResizeTo(Bounds().right,amenu->Frame().top-fmenu->Frame().bottom-2);

 textview[current]->ResizeTo(fView->Bounds().right, fView->Bounds().bottom);

 if (fUpdateTimer->IsActive())
  fUpdateTimer->ResetTimer();
 else
  fUpdateTimer->StartTimer(fOptions->ReakcioIdo);
}
// - End - RHTML_Win - FrameResized --------------------------------------------------------------------------

// ---------------------------------------------------------------------------------- RHTML_Win - FrameMoved -
void RHTMLWin::FrameMoved(BPoint new_position)
{
 if (fUpdateTimer->IsActive())
  fUpdateTimer->ResetTimer();
 else
  fUpdateTimer->StartTimer(fOptions->ReakcioIdo);
}
// - End - RHTML_Win - FrameResized --------------------------------------------------------------------------

// ------------------------------------------------------------------------------- RHTML_Win - QuitRequested -
bool RHTMLWin::QuitRequested()
{
 int o=0;
 for (uint i=0;i<textdb;i++)
 {
  if (textview[i]->Modify()) o++;
 }
 if (!fSaveWinShow&&!savequit&&o>0)
 {
  fSaveWinShow=true;
  fSaveWin = new RHTMLSaveAllWin(BRect(300,200,530,305),this);
  fSaveWin->Show();
  return 0;
 } else
 {
  if (fPrefWinShow) fPrefWin->SetQuit(true);
  if (fAboutWinShow) fAboutWin->SetQuit(true);
  be_app->PostMessage(B_QUIT_REQUESTED);
  return BWindow::QuitRequested();
 }
}
// - End - RHTML_Win - QuitRequested -------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------- RHTML_Win - Open -
void RHTMLWin::Open(const char *name,const char *path)
{
 FILE *f;
 char *buffer;
 buffer = (char*) calloc(5,sizeof(char));
 int32 length=0;
 if(!((f = fopen(path,"r")) == NULL))
 {
  while (feof(f)==false)
  {
   length++;
   buffer = (char*) realloc(buffer,sizeof(char)*length+1);
   buffer[length-1]=fgetc(f);
   buffer[length]=0;
  }
  length--;
  buffer[length]='\0';

  char *buff = new char[length*4];

  BString tmp,tmp2;
  uint FT=0;
  tmp.SetTo(name);
  
  if (tmp.FindLast('.')!=-1)
   tmp.CopyInto(tmp2,tmp.FindLast('.')+1,tmp.Length()-tmp.FindLast('.')-1); else tmp.SetTo("");

  for (uint i=0;i<fOptions->FileTypesDb;i++)
   for (int o=0;o<fOptions->FileTypes[i]->KitDb;o++)
   {
    if (tmp2.ICompare(fOptions->FileTypes[i]->Kit[o]->String())==0) FT=i;
   }
  AddText(name,FT);  

  int32 dest_length=length*4;
  int32 state=0;
 
  if (fOptions->Encoding==B_UTF8_CONVERSION)
  {
   strcpy(buff,buffer);
   dest_length=strlen(buff);
  } else
  {
   convert_to_utf8 (fOptions->Encoding,buffer,&length,buff,&dest_length,&state);
  }

  buff[dest_length] = 0;
  if (fCRLF_LF->IsMarked())
  {
   for(int32 i = 0, j = 0; true; i++, j++)
   {
    if(*(buff + i) == CR)
    {
     *(buff+ j) = LF;
     if(*(buff + i + 1) == LF) i++;
    } else
    {
     *(buff + j) = *(buff + i);
     if(*(buff + j) == '\0') break;
    }
   }
  } 

  textview[textdb-1]->fTextView->SetText(buff);
  textview[textdb-1]->fTextView->fFileTypes->SetFontAndColor(new BString(""),new BString(textview[textdb-1]->fTextView->Text()),1);
  textview[textdb-1]->fFileName->SetTo(path);
  fclose(f);
 }
}
// - End - RHTML_Win - Open ----------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------- RHTML_Win - Save -
void RHTMLWin::Save(uint8 num,const char * FileName)
{
BString data (textview[num]->fTextView->Text());

int32 length = data.Length() + 1;

   char *buffer;
   buffer = new char [length*5];
int32 state (0);

char * buff = new char [length*4];
   strcpy(buff,data.String());

 if (fLF_CRLF->IsMarked())
 {
  int32 i = 0, j = 0;
   for(i = 0, j = 0; true; i++)
   {
    if(buff[i] == CR) ; else 
    if(buff[i] == LF)
    {
     buffer[j]=CR;
     buffer[j+1]=LF;
     j++;j++;
    }
     else
    if(buff[i] == '\0') break; else
    {
     buffer[j]=buff[i];
     j++;
    }
   }
   buffer[j]='\0';
  length=j;

 } else strcpy(buffer,buff);
 int32 dest_length (length*4);

  if (fOptions->Encoding==B_UTF8_CONVERSION)
  {
   strcpy(buff,buffer);
   dest_length=strlen(buff);
  } else
  {
   convert_from_utf8 (fOptions->Encoding,buffer,&length,buff,&dest_length,&state, '?');
   buff[dest_length] = '\0';
  }

 FILE *f;
 if ((f = fopen(FileName, "w"))) {
  fwrite(buff, 1,dest_length, f);
  fclose(f);
  textview[num]->fFileName->SetTo(FileName);
 }
 
 textview[num]->SetModify(false);
   BString tmp,tmp2;
  uint FT=0;
  tmp.SetTo(FileName);
  
  if (tmp.FindLast('.')!=-1)
   tmp.CopyInto(tmp2,tmp.FindLast('.')+1,tmp.Length()-tmp.FindLast('.')-1); else tmp.SetTo("");

  for (uint i=0;i<fOptions->FileTypesDb;i++)
   for (int o=0;o<fOptions->FileTypes[i]->KitDb;o++)
   {
    if (tmp2.ICompare(fOptions->FileTypes[i]->Kit[o]->String())==0) FT=i;
   }

 if (FT!=textview[num]->fTextView->fFileTypes->GetFileType())
 {
  textview[num]->fTextView->fFileTypes->SetFileType(FT);
  textview[textdb-1]->fTextView->fFileTypes->SetFontAndColor(new BString(""),new BString(textview[textdb-1]->fTextView->Text()),1);
 }
}
// - End - RHTML_Win - Save ----------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------ RHTML_Win - Save_All -
void RHTMLWin::Save_All(int num)
{
 if (textview[num]->fFileName->String()!="")
 {
  ChangeText(num);
  Save(num,textview[num]->fFileName->String());
  MessageReceived(new BMessage(B_CANCEL));
 }
  else
  {
   {
    ChangeText(num);
    fSavePanel->SetSaveText(amenu->GetTabText(current));
    Command("SAVAS","");
   }
  }
}
// - End - RHTML_Win - Save_All ------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------- RHTML_Win - Command -
void RHTMLWin::Command(const char *cmd,const char *param)
{
 if (strcasecmp(cmd,"NEW")==0)
 {
  AddText("",1);
 } else
 if (strcasecmp(cmd,"OPEN")==0)
 {
  if (!fOptions->DFOnlyStart)
   fOpenPanel->SetPanelDirectory(fOptions->DefaultFolder.String());
  fOpenPanel->Show();
  be_app->PostMessage(new BMessage('LOCK'));
 } else
 if (strcasecmp(cmd,"QUIT")==0)
 {
  QuitRequested();
 } else
 if (strcasecmp(cmd,"CLOSE")==0)
 {
  uint8 num=current;
  if (param!="")
  {
   num=atoi(param);
   if (num>=textdb) num=current;
  }
  if (textdb>0)
  {
   if (textview[num]->Modify())
   {
    ChangeText(num);
    if (textview[num]->fFileName->String()!="")
    {
     fSaveCloseWin = new RHTMLSaveCloseWin(BRect(300,200,530,305),this,num);
     fSaveCloseWin->Show();
    } else
    {
     saveclose=true;
     Command("SAVAS","");
    }
   } else RemoveText(num);
  } 
 } else
 if (strcasecmp(cmd,"SAVE")==0)
 {
  if (textdb>0)
  {
   if (textview[current]->fFileName->String()!="")
   {
    Save(current,textview[current]->fFileName->String());
    if (saverun)
    {
     Command("RUN","");
     saverun=false;
    }
   } else
   {
    Command("SAVAS","");
   }
  } 
 } else
 if (strcasecmp(cmd,"SAVE_ALL")==0)
 {
  saveall=true;
  if (textdb>0)
   Save_All(0);
 } else
 if (strcasecmp(cmd,"SAVAS")==0)
 {
  if (!fOptions->DFOnlyStart)
   fSavePanel->SetPanelDirectory(fOptions->DefaultFolder.String());
  fSavePanel->Show();
  be_app->PostMessage(new BMessage('LOCK'));
 } else
  if (strcasecmp(cmd,"UNDO")==0)
 {
  textview[current]->fTextView->MessageReceived(new BMessage(B_UNDO));
 } else
 if (strcasecmp(cmd,"CUT")==0)
 {
  textview[current]->fTextView->MessageReceived(new BMessage(B_CUT));
 } else
 if (strcasecmp(cmd,"PASTE")==0)
 {
  textview[current]->fTextView->MessageReceived(new BMessage(B_PASTE));
 } else
 if (strcasecmp(cmd,"COPY")==0)
 {
  textview[current]->fTextView->MessageReceived(new BMessage(B_COPY));
 } else
 if (strcasecmp(cmd,"RUN")==0)
 {
  if (textview[current]->Modify() || textview[current]->fFileName->String()=="")
  {
   BAlert *fAlert = new BAlert("title",
   fOptions->GetLocaleString("Globe_Window_RUN_Alert","Save changes?"),
    fOptions->GetLocaleString("Globe_Window_RUN_Alert","Cancel"),
    fOptions->GetLocaleString("Globe_Window_RUN_Alert","No"),
    fOptions->GetLocaleString("Globe_Window_RUN_Alert","Yes"),B_WIDTH_AS_USUAL, B_OFFSET_SPACING, B_WARNING_ALERT); 
   fAlert->SetShortcut(0, B_ESCAPE);
   int32 button_index = fAlert->Go();
   if (button_index==2)
   {
    saverun=true;
    Command("SAVE","");
   } else
   if (button_index==1)
   {
    BString buf ="";
    buf << "/boot/beos/system/Tracker" << " " << textview[current]->fFileName->String();
    system(buf.String());
   }
  } else
  {
   BString buf ="";
   buf << "/boot/beos/system/Tracker" << " " << textview[current]->fFileName->String();
   system(buf.String());
  }
 } else
 if (strcasecmp(cmd,"INSERTBACK")==0)
 {
  BString *oldtext= new BString(textview[current]->fTextView->Text());
  int32 c,d,e,f;
  textview[current]->fTextView->GetSelection(&c,&d);
  textview[current]->fTextView->Select(c,c);
  textview[current]->fTextView->Insert(param);
  textview[current]->fTextView->GetSelection(&e,&f);
  textview[current]->fTextView->Select(c+strlen(param),d+strlen(param));
  textview[current]->fTextView->ScrollToSelection();
  textview[current]->fTextView->fFileTypes->SetFontAndColor(oldtext,new BString(textview[current]->fTextView->Text()),0,c,f);
  textview[current]->fNumView->UpdateNum();
  textview[current]->SetModify(true);
 } else
 if (strcasecmp(cmd,"INSERTFOR")==0)
 {
  BString *oldtext= new BString(textview[current]->fTextView->Text());
  int32 c,d,e,f;
  textview[current]->fTextView->GetSelection(&c,&d);
  textview[current]->fTextView->Select(d,d);
  textview[current]->fTextView->Insert(param);
  textview[current]->fTextView->GetSelection(&e,&f);
  textview[current]->fTextView->Select(c,d);
  textview[current]->fTextView->ScrollToSelection();
  textview[current]->fTextView->fFileTypes->SetFontAndColor(oldtext,new BString(textview[current]->fTextView->Text()),0,d,f);
  textview[current]->fNumView->UpdateNum();
  textview[current]->SetModify(true);
 } else
 if (strcasecmp(cmd,"INSERT")==0)
 {
  BString *oldtext= new BString(textview[current]->fTextView->Text());
  int32 c,d,e,f;
  textview[current]->fTextView->GetSelection(&c,&d);
  textview[current]->fTextView->Delete(c,d);
  textview[current]->fTextView->Select(c,c);
  textview[current]->fTextView->Insert(param);
  textview[current]->fTextView->GetSelection(&e,&f);
  textview[current]->fTextView->Select(c+strlen(param),c+strlen(param));
  textview[current]->fTextView->ScrollToSelection();
  textview[current]->fTextView->fFileTypes->SetFontAndColor(oldtext,new BString(textview[current]->fTextView->Text()),0,c,f);
  textview[current]->fNumView->UpdateNum();
  textview[current]->SetModify(true);
 } else
 if (strcasecmp(cmd,"ChangeText")==0) // - Belso
 {
  if (current!=atoi(param)) ChangeText(atoi(param));
 } else
 {
  textview[current]->fTextView->Insert("--- Ismeretlen parancs ---");
 }
}
// - End - RHTML_Win - Command -------------------------------------------------------------------------------

// ----------------------------------------------------------------------------- RHTML_Win - MessageReceived -
void RHTMLWin::MessageReceived(BMessage *msg)
{
 switch (msg->what)
  {
   case 'QSAV':
   {
    Save_All(fSaveWin->GetListItem(0));
   }
   break;
   case 'Cmd':
   {
    BString cmd,param;
    msg->FindString("cmd",&cmd);
    msg->FindString("param",&param);
    Command(cmd.String(),param.String());
   }
   break;
   case 'STT':
   {
    int8 a;
	BPoint point;
	BRect bounds;
    msg->FindInt8("num",&a);
    msg->FindPoint("start",&point);
    msg->FindRect("bounds",&bounds);
   
    BMessage msg_(eToolTipStart);
    msg_.AddString("string",textview[a]->fFileName->String());
    msg_.AddPoint("start",point);
    msg_.AddRect("bounds",bounds);
    be_app->PostMessage(&msg_);
   }
   break;
   case 'CSRM':
   {
    RemoveText(current);
   }
   break;
   case 'CSSC':
   {
    Save(current,textview[current]->fFileName->String());
    RemoveText(current);
   }
   break;
   case B_CANCEL:
   {
    int num=current;
     if (saveclose)
     {
      saveclose=false;
      RemoveText(current);
     }
     if (saverun)
     {
      saverun=false;
      Command("RUN","");
     }
     if (saveall)
     if (num<textdb-1)
     {
      num++;
      Save_All(num);
     } else
     saveall=false;

     if (savequit)
     {
      for (int q=0;q<fSaveWin->GetListItemCount();q++)
      {
       if (fSaveWin->GetListItem(q)==num)
       {
        if (q<fSaveWin->GetListItemCount()-1)
        {
         Save_All(fSaveWin->GetListItem(q+1));
        } else
        {
         be_app->PostMessage(B_QUIT_REQUESTED);
         fSaveWin->BWindow::QuitRequested();
         BWindow::QuitRequested();
        }
       }
      }
     }

   }
   break;
   case 'QUIT':
   {
    QuitRequested();
   }
   break;
   case 'NEW':
   {
    AddText("",1);
   }
   break;
   case 'REM':
   {
    Command("CLOSE","");
   }
   break;
   case 'OPEN':
   {
    Command("OPEN","");
   }
   break;
   case 'SAVE':
   {
     Command("SAVE","");
   }
   break;
   case 'SAVA':
   {
    Command("SAVAS","");
   }
   break;
   case 'SAVL':
   {
    saveall=true;
    if (textdb>0)
     Save_All(0);
   }
   break;
   case 'OPN':
   {
    entry_ref ref;
    status_t err;
    int32 ref_num;
    BPath path;
    BEntry entry;
    ref_num = 0;
    do
    {
     if ((err = msg->FindRef("refs", ref_num, &ref)) != B_OK) { return; }
     if ((err=entry.SetTo(&ref)) == B_OK)
     {
      entry.GetPath(&path);
      Open(ref.name,path.Path());
     } 

     ref_num++;
    } while (1);
   }
   break;
  case 'SAV':
  {
   entry_ref ref;
   status_t err;
   const char *name;
   BPath path;
   BEntry entry;
 
   if ((err=msg->FindRef("directory", &ref)) == B_OK)
   {
    if ((err=msg->FindString("name", &name)) == B_OK)
    {
     if ((err=entry.SetTo(&ref)) == B_OK)
     {
      entry.GetPath(&path);
      path.Append(name);
      Save(current,path.Path());
      amenu->SetTabText(current,name);
     }
    }
   }
  }
  break;   
  case 'HUST':
  {
   textview[current]->fTextView->MessageReceived(new BMessage('PPOP'));
  }
  break;
  case 'BMTR':
  {
   int8 tmp;
   tmp=0;
   msg->FindInt8("button",&tmp);
   BMessage *msg_=new BMessage('BMTF');
   msg_->AddInt8("button",tmp);
   fmenu->MessageReceived(msg_);
  }
  break;
  case 'TB':
  {
   if(fTB->IsMarked())
   {
    fTB->SetMarked(false);
    fToolView->RemoveToolBar(fmenu);
    fOptions->TopView=false;
    fOptions->Save();
   } else
   {
    fTB->SetMarked(true);
    fToolView->AddToolBar(fmenu,fOptions->TopViewOrient);
    fOptions->TopView=true;
    fOptions->Save();
   }
  }
  break;
  case 'LB':
  {
   if(fLB->IsMarked())
   {
    fLB->SetMarked(false);
    fToolView->RemoveToolBar(bmenu);
    fOptions->LeftView=false;
    fOptions->Save();
   } else
   {
    fLB->SetMarked(true);
    fToolView->AddToolBar(bmenu,fOptions->LeftViewOrient);
    fOptions->LeftView=true;
    fOptions->Save();
   }
  }
  break;
  case 'NW':
  {
   if(fNW->IsMarked())
   {
    fNW->SetMarked(false);
    for (int i=0;i<textdb;i++) textview[i]->SetNumView(false);
    fOptions->NumView=false;
    fOptions->Save();
    
   } else
   {
    fNW->SetMarked(true);
    for (int i=0;i<textdb;i++) textview[i]->SetNumView(true);
    fOptions->NumView=true;
    fOptions->Save();
   }
  }
  break;
  case 'STWW':
  {
   if(fWordWrap->IsMarked())
   {
    fWordWrap->SetMarked(false);
    for (int i=0;i<textdb;i++) textview[i]->SetWordWrap(false);
    fOptions->WordWrap=false;
    fOptions->Save();
    
   } else
   {
    fWordWrap->SetMarked(true);
    for (int i=0;i<textdb;i++) textview[i]->SetWordWrap(true);
    fOptions->WordWrap=true;
    fOptions->Save();
   }
  }
  break;
  case 'FONT':
  {
   const char *family;
   const char *style;
   msg->FindString("font_family",&family);
   msg->FindString("font_style",&style);
   BFont *font= new BFont();
   font->SetFamilyAndStyle(family, style);
   font->SetSize(12);
   textview[current]->fTextView->fFont=font;
   textview[current]->fTextView->SetFontAndColor(0,textview[current]->fTextView->TextLength()-1,font);
  }
  break;
  case 'Pref':
  {
   if (!fPrefWinShow)
   {
    fPrefWinShow=true;
    fPrefWin = new RHTMLPreferencesWin(BRect(150,100,650,580),this);
    fPrefWin->Show();
   }
  }
  break;
  case 'CE0':
  { for (int i=0;i<25;i++) fCE[i]->SetMarked(false); fCE[0]->SetMarked(true); fOptions->Encoding=B_EUC_CONVERSION; fOptions->Save(); }
  break;
  case 'CE1':
  { for (int i=0;i<25;i++) fCE[i]->SetMarked(false); fCE[1]->SetMarked(true); fOptions->Encoding=B_EUC_KR_CONVERSION; fOptions->Save(); }
  break;
  case 'CE2':
  { for (int i=0;i<25;i++) fCE[i]->SetMarked(false); fCE[2]->SetMarked(true); fOptions->Encoding=B_ISO1_CONVERSION; fOptions->Save(); }
  break;
  case 'CE3':
  { for (int i=0;i<25;i++) fCE[i]->SetMarked(false); fCE[3]->SetMarked(true); fOptions->Encoding=B_ISO2_CONVERSION; fOptions->Save(); }
  break;
  case 'CE4':
  { for (int i=0;i<25;i++) fCE[i]->SetMarked(false); fCE[4]->SetMarked(true); fOptions->Encoding=B_ISO3_CONVERSION; fOptions->Save(); }
  break;
  case 'CE5':
  { for (int i=0;i<25;i++) fCE[i]->SetMarked(false); fCE[5]->SetMarked(true); fOptions->Encoding=B_ISO4_CONVERSION; fOptions->Save(); }
  break;
  case 'CE6':
  { for (int i=0;i<25;i++) fCE[i]->SetMarked(false); fCE[6]->SetMarked(true); fOptions->Encoding=B_ISO5_CONVERSION; fOptions->Save(); }
  break;
  case 'CE7':
  { for (int i=0;i<25;i++) fCE[i]->SetMarked(false); fCE[7]->SetMarked(true); fOptions->Encoding=B_ISO6_CONVERSION; fOptions->Save(); }
  break;
  case 'CE8':
  { for (int i=0;i<25;i++) fCE[i]->SetMarked(false); fCE[8]->SetMarked(true); fOptions->Encoding=B_ISO7_CONVERSION; fOptions->Save(); }
  break;
  case 'CE9':
  { for (int i=0;i<25;i++) fCE[i]->SetMarked(false); fCE[9]->SetMarked(true); fOptions->Encoding=B_ISO8_CONVERSION; fOptions->Save(); }
  break;
  case 'CE10':
  { for (int i=0;i<25;i++) fCE[i]->SetMarked(false); fCE[10]->SetMarked(true); fOptions->Encoding=B_ISO9_CONVERSION; fOptions->Save(); }
  break;
  case 'CE11':
  { for (int i=0;i<25;i++) fCE[i]->SetMarked(false); fCE[11]->SetMarked(true); fOptions->Encoding=B_ISO10_CONVERSION; fOptions->Save(); }
  break;
  case 'CE12':
  { for (int i=0;i<25;i++) fCE[i]->SetMarked(false); fCE[12]->SetMarked(true); fOptions->Encoding=B_ISO13_CONVERSION; fOptions->Save(); }
  break;
  case 'CE13':
  { for (int i=0;i<25;i++) fCE[i]->SetMarked(false); fCE[13]->SetMarked(true); fOptions->Encoding=B_ISO14_CONVERSION; fOptions->Save(); }
  break;
  case 'CE14':
  { for (int i=0;i<25;i++) fCE[i]->SetMarked(false); fCE[14]->SetMarked(true); fOptions->Encoding=B_ISO15_CONVERSION; fOptions->Save(); }
  break;
  case 'CE15':
  { for (int i=0;i<25;i++) fCE[i]->SetMarked(false); fCE[15]->SetMarked(true); fOptions->Encoding=B_JIS_CONVERSION; fOptions->Save(); }
  break;
  case 'CE16':
  { for (int i=0;i<25;i++) fCE[i]->SetMarked(false); fCE[16]->SetMarked(true); fOptions->Encoding=B_KOI8R_CONVERSION; fOptions->Save(); }
  break;
  case 'CE17':
  { for (int i=0;i<25;i++) fCE[i]->SetMarked(false); fCE[17]->SetMarked(true); fOptions->Encoding=B_MAC_ROMAN_CONVERSION; fOptions->Save(); }
  break;
  case 'CE18':
  { for (int i=0;i<25;i++) fCE[i]->SetMarked(false); fCE[18]->SetMarked(true); fOptions->Encoding=B_MS_DOS_866_CONVERSION; fOptions->Save(); }
  break;
  case 'CE19':
  { for (int i=0;i<25;i++) fCE[i]->SetMarked(false); fCE[19]->SetMarked(true); fOptions->Encoding=B_MS_DOS_CONVERSION; fOptions->Save(); }
  break;
  case 'CE20':
  { for (int i=0;i<25;i++) fCE[i]->SetMarked(false); fCE[20]->SetMarked(true); fOptions->Encoding=B_MS_WINDOWS_CONVERSION; fOptions->Save(); }
  break;
  case 'CE21':
  { for (int i=0;i<25;i++) fCE[i]->SetMarked(false); fCE[21]->SetMarked(true); fOptions->Encoding=B_MS_WINDOWS_1251_CONVERSION; fOptions->Save(); }
  break;
  case 'CE22':
  { for (int i=0;i<25;i++) fCE[i]->SetMarked(false); fCE[22]->SetMarked(true); fOptions->Encoding=B_UNICODE_CONVERSION; fOptions->Save(); }
  break;
  case 'CE23':
  { for (int i=0;i<25;i++) fCE[i]->SetMarked(false); fCE[23]->SetMarked(true); fOptions->Encoding=B_UTF8_CONVERSION; fOptions->Save(); }
  break;
  case 'CE24':
  { for (int i=0;i<25;i++) fCE[i]->SetMarked(false); fCE[24]->SetMarked(true); fOptions->Encoding=B_SJIS_CONVERSION; fOptions->Save(); }
  break;
  case 'toro':
  {
   if (fCRLF_LF->IsMarked())
    {
     fCRLF_LF->SetMarked(false);
     fOptions->CRLF_LF=false;
     fOptions->Save();
    } else
    {
     fCRLF_LF->SetMarked(true);
     fOptions->CRLF_LF=true;
     fOptions->Save();
    }
  }
  break;
  case 'tors':
  {
   if (fLF_CRLF->IsMarked())
   {
    fLF_CRLF->SetMarked(false);
    fOptions->LF_CRLF=false;
     fOptions->Save();
   } else
   {
    fOptions->LF_CRLF=true;
    fLF_CRLF->SetMarked(true);
     fOptions->Save();
   }
  }
  break;
  case 'PRWQ':
  {
   fPrefWinShow=false;
  }
  break;
  case 'RTMM':
  {
   save->SetEnabled(textview[current]->Modify());
//   save_as->SetEnabled(textview[current]->Modify());
   bool mod=false;
   for (int i=0;i<textdb;i++)
    if (textview[i]->Modify())
     mod=true;
   save_all->SetEnabled(mod);
  }
  break;
  case 'ABWQ':
  {
   fAboutWinShow=false;
  }
  break;
  case 'SERC':
  {
   if (textdb>0)
   {
    int32 c,d,first;
    textview[current]->fTextView->GetSelection(&c,&d);
    if (bmenu->fSearchView->fCaseCheck->Value())
     first = BString(textview[current]->fTextView->Text()).IFindFirst(bmenu->fSearchView->fSearch->Text(),c+1);
    else
     first = BString(textview[current]->fTextView->Text()).FindFirst(bmenu->fSearchView->fSearch->Text(),c+1);
    if (first>=0)
    {
     textview[current]->fTextView->Select(first,first+strlen(bmenu->fSearchView->fSearch->Text()));
    } else
    {
     if (bmenu->fSearchView->fCaseCheck->Value())
      first = BString(textview[current]->fTextView->Text()).IFindFirst(bmenu->fSearchView->fSearch->Text(),0);
     else
      first = BString(textview[current]->fTextView->Text()).FindFirst(bmenu->fSearchView->fSearch->Text(),0);
     if (first>=0)
     {
      textview[current]->fTextView->Select(first,first+strlen(bmenu->fSearchView->fSearch->Text()));
     } else
     {
      system_beep("Beep");
     }
    }
    textview[current]->fTextView->MakeFocus(true);
    textview[current]->fTextView->ScrollToSelection();    
   }
  }
  break;
  case 'REPL':
  {
   if (textdb>0)
   {
    int32 c,d;
    textview[current]->fTextView->GetSelection(&c,&d);
    BString tmp,tmp2;
    tmp2.SetTo(textview[current]->fTextView->Text());
    tmp2.CopyInto(tmp,c,d-c);
    if ((tmp.ICompare(bmenu->fSearchView->fSearch->Text())==0 && bmenu->fSearchView->fCaseCheck->Value()) || (tmp.Compare(bmenu->fSearchView->fSearch->Text())==0 && !bmenu->fSearchView->fCaseCheck->Value()))
    {
     textview[current]->fTextView->Delete(c,d);
     textview[current]->fTextView->Insert(bmenu->fSearchView->fReplace->Text());
     textview[current]->fTextView->Select(c,c+strlen(bmenu->fSearchView->fReplace->Text()));
     textview[current]->SetModify(true);
    } else
    {
     system_beep("Beep");
    }
   }
   textview[current]->fTextView->MakeFocus(true);
   textview[current]->fTextView->ScrollToSelection();   
  }
  break;
  case 'REAF':
  {
   if (textdb>0)
   {
    int32 c,d;
    textview[current]->fTextView->GetSelection(&c,&d);
    BString tmp,tmp2;
    tmp2.SetTo(textview[current]->fTextView->Text());
    tmp2.CopyInto(tmp,c,d-c);
    if ((tmp.ICompare(bmenu->fSearchView->fSearch->Text())==0 && bmenu->fSearchView->fCaseCheck->Value()) || (tmp.Compare(bmenu->fSearchView->fSearch->Text())==0 && !bmenu->fSearchView->fCaseCheck->Value()))
    {
     textview[current]->fTextView->Delete(c,d);
     textview[current]->fTextView->Insert(bmenu->fSearchView->fReplace->Text());
     textview[current]->fTextView->Select(c,c+strlen(bmenu->fSearchView->fReplace->Text()));
     MessageReceived(new BMessage('SERC'));
     textview[current]->SetModify(true);
    } else
    {
     system_beep("Beep");
     textview[current]->fTextView->MakeFocus(true);
     textview[current]->fTextView->ScrollToSelection();
    }
   }
  }
  break;
  case 'REAL':
  {
   if (textdb>0)
   {
    textview[current]->fTextView->Select(0,0);
    int32 first=0;
    while (((first = BString(textview[current]->fTextView->Text()).IFindFirst(bmenu->fSearchView->fSearch->Text(),first))>=0 && bmenu->fSearchView->fCaseCheck->Value())
        || ((first = BString(textview[current]->fTextView->Text()).FindFirst(bmenu->fSearchView->fSearch->Text(),first))>=0 && !bmenu->fSearchView->fCaseCheck->Value()))
    {
     textview[current]->fTextView->Select(first,first);
     textview[current]->fTextView->Delete(first,first+strlen(bmenu->fSearchView->fSearch->Text()));
     textview[current]->fTextView->Insert(bmenu->fSearchView->fReplace->Text());
     textview[current]->fTextView->Select(first,first+strlen(bmenu->fSearchView->fReplace->Text()));
     first=first+strlen(bmenu->fSearchView->fReplace->Text());
     textview[current]->SetModify(true);
    }
    system_beep("Beep");
    textview[current]->fTextView->MakeFocus(true);
    textview[current]->fTextView->ScrollToSelection();
   }
  }
  break;
  case 'FIND':
  {
   if (!fLB->IsMarked())
    MessageReceived(new BMessage('LB'));
   bmenu->MessageReceived(new BMessage('Tab1'));
   bmenu->fSearchView->fSearch->MakeFocus(true);
  }
  break;
  case 'FINE':
  {
   if (!fLB->IsMarked())
    MessageReceived(new BMessage('LB'));
   bmenu->MessageReceived(new BMessage('Tab1'));
   MessageReceived(new BMessage('SERC'));
  }
  break;
  case 'RELA':
  {
   if (!fLB->IsMarked())
    MessageReceived(new BMessage('LB'));
   bmenu->MessageReceived(new BMessage('Tab1'));
   MessageReceived(new BMessage('REPL'));
  }
  break;
  case 'RAFN':
  {
   if (!fLB->IsMarked())
    MessageReceived(new BMessage('LB'));
   bmenu->MessageReceived(new BMessage('Tab1'));
   MessageReceived(new BMessage('REAF'));
  }
  break;
  case 'RLAL':
  {
   if (!fLB->IsMarked())
    MessageReceived(new BMessage('LB'));
   bmenu->MessageReceived(new BMessage('Tab1'));
   MessageReceived(new BMessage('REAL'));
  }
  break;
  case 'SCCK':
  {
   fOptions->IgnoreCase=bmenu->fSearchView->fCaseCheck->Value();
   fOptions->Save();
  }
  break;
  case 'COPY':
  {
   textview[current]->fTextView->MessageReceived(new BMessage(B_COPY));
  }
  break;
  case 'CUT':
  {
   textview[current]->fTextView->MessageReceived(new BMessage(B_CUT));
  }
  break;
  case 'PATE':
  {
   textview[current]->fTextView->MessageReceived(new BMessage(B_PASTE));
  }
  break;
  case 'SLAL':
  {
   textview[current]->fTextView->MessageReceived(new BMessage(B_SELECT_ALL));
  }
  break;
  case 'UNDO':
  {
   textview[current]->fTextView->MessageReceived(new BMessage(B_UNDO));
  }
  break;
  case 'MGTO':
  {
   if (!fLB->IsMarked())
    MessageReceived(new BMessage('LB'));
   bmenu->fSearchView->fLine->MakeFocus(true);
  }
  break;
  case 'GOTO':
  {
   if (textdb>0)
   {
    int32 num=atoi(bmenu->fSearchView->fLine->Text())-1;
    if (textview[current]->fTextView->CountLines()>num)
    {
     int32 tmp;
     tmp=textview[current]->fNumView->UpdateNum(num)-1;
     textview[current]->fTextView->Select(textview[current]->fTextView->OffsetAt(tmp),textview[current]->fTextView->OffsetAt(tmp));
    } else
    {
     textview[current]->fTextView->Select(textview[current]->fTextView->OffsetAt(textview[current]->fTextView->CountLines()-1),textview[current]->fTextView->OffsetAt(textview[current]->fTextView->CountLines()-1));
     system_beep("Beep");
    }
    textview[current]->fTextView->ScrollToSelection();
    textview[current]->fTextView->MakeFocus(true);
   }
  }
  break;
  case 'Refe':
  {
   BString dir = "/boot/beos/system/Tracker ";
   dir << fOptions->SettingsDir << "reference/index.html";
   system(dir.String());
  }
  break;
  case 'TIMR':
  {
   fOptions->WinFrameLeft = Frame().left;
   fOptions->WinFrameTop = Frame().top;
   fOptions->WinFrameRight = Frame().right;
   fOptions->WinFrameBottom = Frame().bottom;
   fOptions->Save();
   fUpdateTimer->EndTimer();
  }
  break;
  case 'Abo':
  {
   if (!fAboutWinShow)
   {
    fAboutWinShow=true;
    fAboutWin= new RHTMLAboutWin(BRect(250,300,549,499),this);
    fAboutWin->Show();
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
// - End - RHTML_Win - MessageReceived -----------------------------------------------------------------------

// - End - Window - Source File ------------------------------------------------------------------------------
