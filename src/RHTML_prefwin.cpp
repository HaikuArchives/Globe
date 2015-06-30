//////////////////////////////////////////////////////////////////////
//      Globe Webeditor © Gergely Rózsahegyi (RoGer) 2001-2004      //
//                                                                  //
//                   Preferences Window  Source File                //
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
#include <stdio.h>
#include <Path.h>
#include "RHTML_prefwin.h"

// ------------------------------------------------------------- RHTML_Preferences_Win - RHTMLPreferencesWin -
RHTMLPreferencesWin::RHTMLPreferencesWin(BRect fRect,BWindow *parent): BWindow(fRect,fOptions->GetLocaleString("Preferences_Window","Preferences"),B_FLOATING_WINDOW,B_UNTYPED_WINDOW)
{
 fParent=parent;
 IsQuit=false;
 fList = new BOutlineListView(BRect(0,0,150,Bounds().bottom),"List",B_SINGLE_SELECTION_LIST,B_FOLLOW_TOP_BOTTOM,B_WILL_DRAW);
 BView *view1= new BView(BRect(151,0,151,Bounds().bottom),"view",B_FOLLOW_TOP_BOTTOM,B_WILL_DRAW);
 fView=new BView(BRect(152,0,Bounds().right,Bounds().bottom),"view",B_FOLLOW_ALL,B_WILL_DRAW);
 AddChild(fList);
 AddChild(view1);
 AddChild(fView);
 fView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
 view1->SetViewColor(184,184,184,0);
 fList->SetSelectionMessage(new BMessage('LIST'));

 JViewDb=5;
 fJView = (BView**) calloc(JViewDb,sizeof(BView));

// - Create Languages
 fList->AddItem(new BStringItem(fOptions->GetLocaleString("Preferences_Window_List","Languages")));

 fJView[0] = new BView(fView->Bounds(),"LangView",B_FOLLOW_TOP_BOTTOM,B_WILL_DRAW);
 fJView[0]->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
 fView->AddChild(fJView[0]);
 fLangMenu = new BMenu(fOptions->Locale[fOptions->CurrentLocale]->Lang.String());
 fLangMenu->SetLabelFromMarked(true);
 fLangMenu->SetRadioMode(true);
 for (int32 i=0;i<(int32)fOptions->LocaleDb;i++)
 {
  BMessage* msg = new BMessage('LANG');
  msg->AddInt32("Lang",i);
  BMenuItem *item;
  fLangMenu->AddItem(item=new BMenuItem(fOptions->Locale[i]->Lang.String(),msg));
  item->SetTarget(this);
 }
 fLanguages= new BMenuField(BRect(10,10,250,30),"LanguagesMenu",fOptions->GetLocaleString("Preferences_Window_Languages","Languages"),fLangMenu);
 fJView[0]->AddChild(fLanguages);
 
 fView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

 fStrView1 = new BStringView(BRect(10,50,250,100),"strview1","Language:");
 fJView[0]->AddChild(fStrView1);

 fStrView2 = new BStringView(BRect(10,50,250,120),"strview1","");
 fJView[0]->AddChild(fStrView2);

 fStrView3 = new BStringView(BRect(10,50,250,150),"strview1","Author:");
 fJView[0]->AddChild(fStrView3);

 author = new URLView( BRect(0,50,250,170),"author","","");
 fJView[0]->AddChild(author);

 fStrView4 = new BStringView(BRect(10,50,250,200),"strview4","Language version:");
 fJView[0]->AddChild(fStrView4);
 
 fStrView5 = new BStringView(BRect(10,50,300,220),"strview5","");
 fJView[0]->AddChild(fStrView5);

 fStrView6 = new BStringView(BRect(10,50,300,250),"strview6","Language create to:");
 fJView[0]->AddChild(fStrView6);
 
 fStrView7 = new BStringView(BRect(10,50,300,270),"strview7","");
 fJView[0]->AddChild(fStrView7);

 LSetAuthor();

// - Create FileTypes

 fJView[1] = new BView(fView->Bounds(),"FTView",B_FOLLOW_TOP_BOTTOM,B_WILL_DRAW);
 fJView[1]->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
 fView->AddChild(fJView[1]);

// - Create FileTypes/Fonts

 fFTFontsBox= new BBox(BRect(10,10,200,120));
 fFTFontsBox->SetLabel(fOptions->GetLocaleString("Preferences_Window_FileTypes","Default font"));
 fFTFontsBox->SetFont(new BFont(be_plain_font));
 fJView[1]->AddChild(fFTFontsBox);


 BString stmp;
 stmp << (int32) fOptions->DefaultFont->Size();
 BMenu *fSizeMenu = new BMenu(stmp.String());
 fSizeMenu->SetRadioMode(false);
 fSizeMenu->SetLabelFromMarked(true);
 
 for (int32 o=8;o<15;o++)
 {
  BMessage *msg2;
  msg2 = new BMessage('FTSZ');
  msg2->AddInt32("font_size",o);
  BString tmp;
  tmp << o;
  fSizeMenu->AddItem(new BMenuItem(tmp.String(),msg2));
 }
 fFTDFSize= new BMenuField(BRect(10,75,180,105),"Font Size",fOptions->GetLocaleString("Preferences_Window_FileTypes","Size"),fSizeMenu);
 fFTDFSize->SetDivider(60);
 fFTFontsBox->AddChild(fFTDFSize);

 font_family fFamily;
 font_style	fStyle;

 fOptions->DefaultFont->GetFamilyAndStyle(&fFamily,&fStyle);
 fFTDFStyle= new BMenuField(BRect(10,45,180,75),"Font Style",fOptions->GetLocaleString("Preferences_Window_FileTypes","Style"),new BMenu("Style"));
 fFTDFStyle->SetDivider(60);
 fFTFontsBox->AddChild(fFTDFStyle);
 FTSetFont(fFamily,fStyle);
 BMenu *fMenu = new BMenu(fFamily);
 fMenu->SetRadioMode(false);
 fMenu->SetLabelFromMarked(true);

 const int32 fontNum = count_font_families();

 for(int32 i = 0; i < fontNum; i++)
 {
  get_font_family(i,&fFamily);
  BMessage*	msg = new BMessage('FTDF');
  msg->AddString("font_family", fFamily);
  fMenu->AddItem(new BMenuItem(fFamily,msg));
 }

 fFTDFont= new BMenuField(BRect(10,15,180,35),"Default font",fOptions->GetLocaleString("Preferences_Window_FileTypes","Font"),fMenu);
 fFTDFont->SetDivider(60);
 fFTFontsBox->AddChild(fFTDFont);

// - Create FileTypes/Extensions

 fFTBox= new BBox(BRect(10,130,200,290));
 fFTBox->SetLabel(fOptions->GetLocaleString("Preferences_Window_FileTypes","Extension"));
 fFTBox->SetFont(new BFont(be_plain_font));
 fJView[1]->AddChild(fFTBox);

 BListItem *FTUitem;
 fList->AddItem(FTUitem=new BStringItem(fOptions->GetLocaleString("Preferences_Window_List","Filetypes")));

 fFTFTMenu = new BMenu(fOptions->FileTypes[0]->Name->String());
 fFTFTMenu->SetLabelFromMarked(true);
 fFTFTMenu->SetRadioMode(true);
 for (int32 i=0;i<(int32)fOptions->FileTypesDb;i++)
 {
  BMessage* msg = new BMessage('FTFT');
  msg->AddInt32("FileType",i);
  BMenuItem *item;
  fFTFTMenu->AddItem(item=new BMenuItem(fOptions->FileTypes[i]->Name->String(),msg));
  item->SetTarget(this);
 }
 fFTFT= new BMenuField(BRect(10,15,180,35),"FileTypesMenu",fOptions->GetLocaleString("Preferences_Window_FileTypes","Filetype"),fFTFTMenu);
 fFTFT->SetDivider(80);
 fFTBox->AddChild(fFTFT); 
 
 fFTKit = new BListView(BRect(10,40,54,120),"KitList",B_SINGLE_SELECTION_LIST,B_FOLLOW_NONE,B_WILL_DRAW);
 BScrollView *fScrollView = new BScrollView("List",fFTKit,B_FOLLOW_ALL_SIDES,B_WILL_DRAW, false, true,B_FANCY_BORDER);
 fFTBox->AddChild(fScrollView);
 
 fFTKit->TargetedByScrollView(fScrollView);
 fFTKit->SetSelectionMessage(new BMessage('FTSL'));
 
 fFTAddButton = new BButton(BRect(90,40,170,60),"Add",fOptions->GetLocaleString("Preferences_Window_FileTypes","Add"),new BMessage('FTAD'));
 fFTEditButton = new BButton(BRect(90,80,170,100),"Edit",fOptions->GetLocaleString("Preferences_Window_FileTypes","Edit"),new BMessage('FTED'));
 fFTRemoveButton = new BButton(BRect(90,120,170,140),"Remove",fOptions->GetLocaleString("Preferences_Window_FileTypes","Remove"),new BMessage('FTRM'));
 fFTBox->AddChild(fFTAddButton);
 fFTBox->AddChild(fFTEditButton);
 fFTBox->AddChild(fFTRemoveButton);
 
 fFTStrView = new BTextControl(BRect(5,130,70,150),"Név","","",new BMessage('FTST'));
 fFTStrView->SetDivider(0);
 fFTBox->AddChild(fFTStrView);
 FTCreateKitList(0);

// - Create FileTypes - Highlighting

 fList->AddUnder(new BStringItem(fOptions->GetLocaleString("Preferences_Window_List","Highlighting")), FTUitem);

 fJView[2] = new BView(fView->Bounds(),"FTView",B_FOLLOW_TOP_BOTTOM,B_WILL_DRAW);
 fJView[2]->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
 fView->AddChild(fJView[2]);

  // - FT
 fFTHFileTypes=0;
 fFTHFTBox= new BBox(BRect(10,10,320,260));
 fFTHFTBox->SetLabel(fOptions->GetLocaleString("Preferences_Window_FileTypes","Highlighting"));
 fFTHFTBox->SetFont(new BFont(be_plain_font));
 fJView[2]->AddChild(fFTHFTBox);

 fFTFTMenu = new BMenu(fOptions->FileTypes[0]->Name->String());
 fFTFTMenu->SetLabelFromMarked(true);
 fFTFTMenu->SetRadioMode(true);
 for (int32 i=0;i<(int32)fOptions->FileTypesDb;i++)
 {
  BMessage* msg = new BMessage('FTHF');
  msg->AddInt32("FileType",i);
  BMenuItem *item;
  fFTFTMenu->AddItem(item=new BMenuItem(fOptions->FileTypes[i]->Name->String(),msg));
  item->SetTarget(this);
 }

 fFTHFT= new BMenuField(BRect(10,15,210,30),"FileTypesMenu",fOptions->GetLocaleString("Preferences_Window_FileTypes","Filetype"),fFTFTMenu);
 fFTHFT->SetDivider(100);
 fFTHFTBox->AddChild(fFTHFT);

 fFTHFC= new BMenuField(BRect(10,40,210,65),"FileTypesMenu",fOptions->GetLocaleString("Preferences_Window_FileTypes","Syntax styles"),new BMenu(""));
 fFTHFC->SetDivider(100);
 fFTHFTBox->AddChild(fFTHFC);
 
  // - Font
 fFTHFontColor=0;
 fFTHFontBox= new BBox(BRect(10,80,230,130));
 fFTHFontBox->SetLabel(fOptions->GetLocaleString("Preferences_Window_FileTypes","Font"));
 fFTHFontBox->SetFont(new BFont(be_plain_font));
 fFTHFTBox->AddChild(fFTHFontBox);

 fFTHStyle= new BMenuField(BRect(10,15,210,40),"Size",fOptions->GetLocaleString("Preferences_Window_FileTypes","Style"),new BMenu("Style"));
 fFTHFontBox->AddChild(fFTHStyle);


  // - Color
 
 fFTHColorBox= new BBox(BRect(10,150,300,240));
 fFTHColorBox->SetLabel(fOptions->GetLocaleString("Preferences_Window_FileTypes","Color"));
 fFTHColorBox->SetFont(new BFont(be_plain_font));
 fFTHFTBox->AddChild(fFTHColorBox);

 fFTHCC = new BColorControl(BPoint(10,20),B_CELLS_32x8,1,"CC",new BMessage(R_FTH_Color_MSG));
 fFTHColorBox->AddChild(fFTHCC);

// - Default Folder
 fList->AddItem(new BStringItem(fOptions->GetLocaleString("Preferences_Window_List","Default Folder")));

 fJView[3] = new BView(fView->Bounds(),"OpenSaveView",B_FOLLOW_TOP_BOTTOM,B_WILL_DRAW);
 fJView[3]->SetViewColor(216,216,216,0);
 fView->AddChild(fJView[3]);

 fDFDefaultFolder = new BTextControl(BRect(10,10,260,30),"DFDefaultFolder",fOptions->GetLocaleString("Preferences_Window_DefaultFolder","Default Folder"),fOptions->DefaultFolder.String(),new BMessage('DFFN'));
 fDFDefaultFolder->SetDivider(100);
 fJView[3]->AddChild(fDFDefaultFolder);

 BButton *fDFButton = new BButton(BRect(270,8,330,30),"DFButton",fOptions->GetLocaleString("Preferences_Window_DefaultFolder","Browse"),new BMessage('DFBB'));
 fJView[3]->AddChild(fDFButton);
 fDFFilePanel = new BFilePanel(B_OPEN_PANEL,new BMessenger(this,this), NULL,
				   B_DIRECTORY_NODE, false,new BMessage('DFFP'));
 fDFCheckBox = new BCheckBox(BRect(150,40,280,60),"DFCheckBox",fOptions->GetLocaleString("Preferences_Window_DefaultFolder","Only Start"),new BMessage('DFCK'));
 fJView[3]->AddChild(fDFCheckBox);
 fDFCheckBox->SetValue(fOptions->DFOnlyStart);

// - Advanced Prefs
 fList->AddItem(new BStringItem(fOptions->GetLocaleString("Preferences_Window_List","Advanced preferences")));

 fJView[4] = new BView(fView->Bounds(),"AdvancedPrefs",B_FOLLOW_TOP_BOTTOM,B_WILL_DRAW);
 fJView[4]->SetViewColor(216,216,216,0);
 fView->AddChild(fJView[4]);
 
 fAPSlider = new BSlider(BRect(10,10,330,50),"slider",fOptions->GetLocaleString("Preferences_Window_AdvancedPrefs","Parameter-Menu reaction time"),new BMessage('APRT'), 0, 1000);
 fAPSlider->SetLimitLabels("0ms", "1000ms");
 fAPSlider->SetValue(fOptions->ReakcioIdo);
 fAPSlider->SetHashMarks(B_HASH_MARKS_BOTTOM);
 fAPSlider->SetHashMarkCount(5);
 fJView[4]->AddChild(fAPSlider);

 fAPTMSlider = new BSlider(BRect(10,100,330,150),"slider",fOptions->GetLocaleString("Preferences_Window_AdvancedPrefs","Tag-Menu reaction time"),new BMessage('APTM'), 0, 3000);
 fAPTMSlider->SetLimitLabels("0ms", "3000ms");
 fAPTMSlider->SetValue(fOptions->ReakcioIdo);
 fAPTMSlider->SetHashMarks(B_HASH_MARKS_BOTTOM);
 fAPTMSlider->SetHashMarkCount(5);
 fJView[4]->AddChild(fAPTMSlider);

 // -
 FTHSetFT(0);

 fList->Select(0);
 for (uint i=0;i<JViewDb;i++)fJView[i]->ResizeTo(0,0);
 fJView[0]->ResizeTo(fView->Bounds().right,fView->Bounds().bottom);
}
// - End - RHTML_Preferences_Win - RHTMLPreferencesWin -------------------------------------------------------

// ---------------------------------------------------------------------- RHTML_Preferences_Win - LSetAuthor -
void RHTMLPreferencesWin::LSetAuthor()
{
 fJView[0]->RemoveChild(fStrView1);
 fJView[0]->RemoveChild(fStrView2);
 fJView[0]->RemoveChild(fStrView3);
 fJView[0]->RemoveChild(fStrView4);
 fJView[0]->RemoveChild(fStrView5);
 fJView[0]->RemoveChild(fStrView6);
 fJView[0]->RemoveChild(fStrView7);
 fJView[0]->RemoveChild(author);

 BFont *fFont1= new BFont(be_plain_font);
 fFont1->SetSize(12);
 fFont1->SetFace(B_BOLD_FACE);

 BFont *fFont2= new BFont(be_plain_font);
 fFont2->SetSize(12);
 
 rgb_color fekete={0,0,0,0};

 BString tmp1,tmp2;

 tmp1.SetTo("");
 tmp1 << fOptions->GetLocaleString("Preferences_Window_Languages","Language") << ":";
 fStrView1 = new BStringView(BRect(10,50,350,100),"strview1",tmp1.String());
 fStrView1->SetAlignment(B_ALIGN_CENTER);
 fStrView1->SetFont(fFont1);
 fJView[0]->AddChild(fStrView1);

 fStrView2 = new BStringView(BRect(10,50,350,120),"strview1",fOptions->Locale[fOptions->CurrentLocale]->Lang.String());
 fStrView2->SetAlignment(B_ALIGN_CENTER);
 fStrView2->SetFont(fFont2);
 fJView[0]->AddChild(fStrView2);

 tmp1.SetTo("");
 tmp1 << fOptions->GetLocaleString("Preferences_Window_Languages","Author") << ":";

 fStrView3 = new BStringView(BRect(10,50,350,150),"strview1",tmp1.String());
 fStrView3->SetAlignment(B_ALIGN_CENTER);
 fStrView3->SetFont(fFont1);
 fJView[0]->AddChild(fStrView3);

 tmp1.SetTo("");
 tmp2.SetTo("");
 tmp1 << fOptions->Locale[fOptions->CurrentLocale]->Author.String() << " - " << fOptions->Locale[fOptions->CurrentLocale]->Email.String();
 tmp2 << fOptions->Locale[fOptions->CurrentLocale]->Email.String();

 author = new URLView( BRect(0,50,350,170),"author",tmp1.String(),tmp2.String());
 fJView[0]->AddChild(author);
 author->SetHoverEnabled(false);
 author->AddAttribute("META:nickname",fOptions->Locale[fOptions->CurrentLocale]->Author.String());
 author->AddAttribute("META:url","http://www.beos.hu/");
 author->SetFont(fFont2);
 author->SetAlignment(B_ALIGN_CENTER);
 author->SetColor(fekete);
 author->SetClickColor(fekete);
 author->SetUnderlineThickness(false);
}
// - End - RHTML_Preferences_Win - LSetAuthor ----------------------------------------------------------------

// ----------------------------------------------------------------- RHTML_Preferences_Win - FTCreateKitList -
void RHTMLPreferencesWin::FTCreateKitList(uint FileType)
{
 fFT=FileType;
 fFTKit->MakeEmpty();
 if (fOptions->FileTypesDb>0)
 {
  for (int32 i=0;i<(int32)fOptions->FileTypes[FileType]->KitDb;i++)
  {
   fFTKit->AddItem(new BStringItem(fOptions->FileTypes[FileType]->Kit[i]->String()));
  }
  if (fOptions->FileTypes[FileType]->KitDb>0)
  {
   fFTKit->Select(0);
   fFTStrView->SetText(fOptions->FileTypes[FileType]->Kit[0]->String());
  } else fFTStrView->SetText("");
 }
}
// - End - RHTML_Preferences_Win - FTCreateKitList -----------------------------------------------------------

// ----------------------------------------------------------------------- RHTML_Preferences_Win - FTSetFont -
void RHTMLPreferencesWin::FTSetFont(const char *font,const char *style)
{
 fFTFontsBox->RemoveChild(fFTDFStyle);

 font_family fFamily;
 font_style fStyle;
 BMenu *fSMenu;
 
 strcpy(fFamily,font);
 int32 styleNum = count_font_styles(fFamily);
 if (style!="")
 {
  fSMenu = new BMenu(fOptions->GetLocaleString("Preferences_Window_FileTypes",style));
 } else
 if (styleNum>0)
 {
  get_font_style(fFamily, 0, &fStyle);
  fSMenu = new BMenu(fOptions->GetLocaleString("Preferences_Window_FileTypes",fStyle));
 } else fSMenu = new BMenu("");

 fSMenu->SetRadioMode(false);
 fSMenu->SetLabelFromMarked(true);

 for (int32 ii = 0; ii < styleNum; ii++)
 {
  get_font_style(fFamily, ii, &fStyle);
  BMessage*	msg = new BMessage('FTFS');
  msg->AddString("font_style", fStyle);  
  fSMenu->AddItem(new BMenuItem(fOptions->GetLocaleString("Preferences_Window_FileTypes",fStyle),msg));
 }

 fFTDFStyle= new BMenuField(BRect(10,45,180,75),"Font Style",fOptions->GetLocaleString("Preferences_Window_FileTypes","Style"),fSMenu);
 fFTDFStyle->SetDivider(60);
 fFTFontsBox->AddChild(fFTDFStyle);
}
// - End - RHTML_Preferences_Win - FTSetFont -----------------------------------------------------------------

// ------------------------------------------------------------------------ RHTML_Preferences_Win - FTHSetFT -
void RHTMLPreferencesWin::FTHSetFT(uint FileType)
{
 fFTHFTBox->RemoveChild(fFTHFC);

 BMenu *fFCMenu = new BMenu(fOptions->GetLocaleString("FileTypes_FontColorName",fOptions->FileTypes[FileType]->FCName[0]->String()));
 fFCMenu->SetLabelFromMarked(true);
 fFCMenu->SetRadioMode(true);
 for (int32 i=0;i<(int32)fOptions->FileTypes[FileType]->FontColorDb;i++)
 {
  BMessage* msg = new BMessage('FTHT');
  msg->AddInt32("FontColor",i);
  BMenuItem *item;
  fFCMenu->AddItem(item=new BMenuItem(fOptions->GetLocaleString("FileTypes_FontColorName",fOptions->FileTypes[FileType]->FCName[i]->String()),msg));
  item->SetTarget(this);
 }
 
 fFTHFC= new BMenuField(BRect(10,40,210,65),"FileTypesMenu",fOptions->GetLocaleString("Preferences_Window_FileTypes","Element"),fFCMenu);
 fFTHFC->SetDivider(100);
 fFTHFTBox->AddChild(fFTHFC);
 FTHSetFSize(0);
}
// - End - RHTML_Preferences_Win - FTHSetFT ------------------------------------------------------------------

// --------------------------------------------------------------------- RHTML_Preferences_Win - FTHSetFSize -
void RHTMLPreferencesWin::FTHSetFSize(uint FontColor)
{
 fFTHFontBox->RemoveChild(fFTHStyle);

 font_family fFamily;
 font_style	fStyle;
  BMessage *msg;

// - Style

 BMenu *fStyleMenu;
 
 fOptions->DefaultFont->GetFamilyAndStyle(&fFamily,&fStyle);
 int32 styleNum = count_font_styles(fFamily);

 if (fOptions->FileTypes[fFTHFileTypes]->Font[FontColor]->Style.ICompare("Default style")==0)
 {
  fStyleMenu = new BMenu(fOptions->GetLocaleString("Preferences_Window_FileTypes","Default style"));
 } else
  fStyleMenu = new BMenu(fOptions->GetLocaleString("Preferences_Window_FileTypes",fOptions->FileTypes[fFTHFileTypes]->Font[FontColor]->Style.String()));

 fStyleMenu->SetRadioMode(false);
 fStyleMenu->SetLabelFromMarked(true);

 msg = new BMessage(R_FTH_Style_MSG);
 msg->AddString("font_style","Default style");
 fStyleMenu->AddItem(new BMenuItem(fOptions->GetLocaleString("Preferences_Window_FileTypes","Default style"),msg));

 for (int32 ii = 0; ii < styleNum; ii++)
 {
  get_font_style(fFamily, ii, &fStyle);
  msg = new BMessage(R_FTH_Style_MSG);
  msg->AddString("font_style", fStyle);  
  fStyleMenu->AddItem(new BMenuItem(fOptions->GetLocaleString("Preferences_Window_FileTypes",fStyle),msg));
 }
 
 fFTHStyle= new BMenuField(BRect(10,15,210,40),"Style",fOptions->GetLocaleString("Preferences_Window_FileTypes","Style"),fStyleMenu);
 fFTHStyle->SetDivider(80);
 fFTHFontBox->AddChild(fFTHStyle);
  
// - Color

 rgb_color rgb;
 rgb.alpha=fOptions->FileTypes[fFTHFileTypes]->Color[FontColor]->alpha;
 rgb.red=fOptions->FileTypes[fFTHFileTypes]->Color[FontColor]->red;
 rgb.green=fOptions->FileTypes[fFTHFileTypes]->Color[FontColor]->green;
 rgb.blue=fOptions->FileTypes[fFTHFileTypes]->Color[FontColor]->blue;
 fFTHCC->SetValue(rgb);
}
// - End - RHTML_Preferences_Win - FTHSetFSize ---------------------------------------------------------------

// -------------------------------------------------------------------- RHTML_Preferences_Win - FrameResized -
void RHTMLPreferencesWin::FrameResized(float new_width, float new_height)
{
 for (uint i=0;i<JViewDb;i++)fJView[i]->ResizeTo(0,0);
 if (fList->CurrentSelection()>=0) fJView[fList->CurrentSelection()]->ResizeTo(fView->Bounds().right,fView->Bounds().bottom);
}
// - End - RHTML_Preferences_Win - FrameResized --------------------------------------------------------------


// ------------------------------------------------------------------------- RHTML_Preferences_Win - SetQuit -
void RHTMLPreferencesWin::SetQuit(bool quit)
{
 IsQuit=quit;
}
// - End - RHTML_Preferences_Win - SetQuit -------------------------------------------------------------------

// ------------------------------------------------------------------- RHTML_Preferences_Win - QuitRequested -
bool RHTMLPreferencesWin::QuitRequested()
{
 MessageReceived(new BMessage('DFFN'));
 
 if (!IsQuit) fParent->MessageReceived(new BMessage('PRWQ'));
 return BWindow::QuitRequested();
}
// - End - RHTML_Preferences_Win - QuitRequested -------------------------------------------------------------
 
// ----------------------------------------------------------------- RHTML_Preferences_Win - MessageReceived -
void RHTMLPreferencesWin::MessageReceived(BMessage *msg)
{
 switch (msg->what)
  {
   case 'LANG':
   {
    int32 lang;
    msg->FindInt32("Lang",&lang);
    fOptions->CurrentLocale=lang;
    BAlert *fAlert= new BAlert("Alert",fOptions->GetLocaleString("Preferences_Window_Alert","To take effect your changes you must restart Globe!"),"OK");
    LSetAuthor();
    fAlert->Go();
    fOptions->Save();
   }
   break;
   case 'FTFT':
   {
    int32 filetype;
    msg->FindInt32("FileType",&filetype);
    FTCreateKitList(filetype);
   }
   break;
   case 'FTHF':
   {
    int32 filetype;
    msg->FindInt32("FileType",&filetype);
    fFTHFileTypes= (uint) filetype;
    FTHSetFT(filetype);
   }
   break;
   case 'FTHT':
   {
    int32 fontcolor;
    msg->FindInt32("FontColor",&fontcolor);
    fFTHFontColor= (uint) fontcolor;    
    FTHSetFSize(fontcolor);
   }
   break;
   case R_FTH_Family_MSG:
   {
    const char *family;
    msg->FindString("font_family",&family);
    fOptions->FileTypes[fFTHFileTypes]->Font[fFTHFontColor]->Family.SetTo(family);
    if (fOptions->FileTypes[fFTHFileTypes]->Font[fFTHFontColor]->Style.ICompare("Default style")!=0)
    {
     font_family fFamily;
     font_style fStyle;
     if (strcmp(family,"Default font")==0)
     {
      fOptions->DefaultFont->GetFamilyAndStyle(&fFamily,&fStyle);     
     } else strcpy(fFamily,family);
     get_font_style(fFamily, 0, &fStyle);
     fOptions->FileTypes[fFTHFileTypes]->Font[fFTHFontColor]->Style.SetTo(fStyle);
     FTHSetFSize(fFTHFontColor);
    }
    fOptions->Save();
   }
   break;
   case R_FTH_Style_MSG:
   {
    const char *style;
    msg->FindString("font_style",&style);
    fOptions->FileTypes[fFTHFileTypes]->Font[fFTHFontColor]->Style.SetTo(style);
    fOptions->Save();
   }
   break;
   case R_FTH_Size_MSG:
   {
    int32 size;
    msg->FindInt32("font_size",&size);
    fOptions->FileTypes[fFTHFileTypes]->Font[fFTHFontColor]->Size=size;
    fOptions->Save();
   }
   break;
   case R_FTH_Color_MSG:
   {
    fOptions->FileTypes[fFTHFileTypes]->Color[fFTHFontColor]->alpha=fFTHCC->ValueAsColor().alpha;
    fOptions->FileTypes[fFTHFileTypes]->Color[fFTHFontColor]->red=fFTHCC->ValueAsColor().red;
    fOptions->FileTypes[fFTHFileTypes]->Color[fFTHFontColor]->green=fFTHCC->ValueAsColor().green;
    fOptions->FileTypes[fFTHFileTypes]->Color[fFTHFontColor]->blue=fFTHCC->ValueAsColor().blue;
    fOptions->Save();
   }
   break;
   case 'FTSL':
   {
    if (fFTKit->CurrentSelection()>=0)
     fFTStrView->SetText(fOptions->FileTypes[fFT]->Kit[fFTKit->CurrentSelection()]->String());
   }
   break;
   case 'FTAD':
   {
    uint num=fOptions->AddFTKit(fFT,fFTStrView->Text());
    FTCreateKitList(fFT);
    fFTKit->Select(num);
    fOptions->Save();
   }
   break;
   case 'FTRM':
   {
    if (fFTKit->CurrentSelection()>=0)
    {
     fOptions->RemoveFTKit(fFT,fFTKit->CurrentSelection());
     FTCreateKitList(fFT);
     fOptions->Save();     
    }
   }
   break;
   case 'FTED':
   {
    if (fFTKit->CurrentSelection()>=0)
    {
     fOptions->EditFTKit(fFT,fFTKit->CurrentSelection(),fFTStrView->Text());
     FTCreateKitList(fFT);
     fOptions->Save();     
    }
   }
   break;
   case 'FTDF':
   {
    const char *family;
    msg->FindString("font_family",&family);
    FTSetFont(family);
    font_style fStyle;
    get_font_style((char *) family, 0, &fStyle);
    fOptions->DefaultFont->SetFamilyAndStyle(family,fStyle);
    fOptions->Save();
   }
   break;
   case 'FTFS':
   {
    const char *style;
    msg->FindString("font_style",&style);
    font_family family;
    font_style fstyle;
    fOptions->DefaultFont->GetFamilyAndStyle(&family,&fstyle);
    fOptions->DefaultFont->SetFamilyAndStyle(family, style);
    fOptions->Save();
   }
   break;
   case 'FTSZ':
   {
    int32 size;
    msg->FindInt32("font_size",&size);
    fOptions->DefaultFont->SetSize((float) size);
    fOptions->Save();
   }
   break;
   case 'LIST':
   {
    for (uint i=0;i<JViewDb;i++)fJView[i]->ResizeTo(0,0);
    if (fList->CurrentSelection()>=0)
    {
     fJView[fList->CurrentSelection()]->ResizeTo(fView->Bounds().right,fView->Bounds().bottom);
     FTHSetFSize(0);
    }
   }
   break;
   case 'DFBB':
   {
    if (!IsHidden())
     Hide();
    fDFFilePanel->Show();
   }
   break;
   case 'DFFP':
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
      fDFDefaultFolder->SetText(path.Path());
      fOptions->DefaultFolder.SetTo(path.Path());
      fOptions->Save();
     } 
     ref_num++;
    } while (1);
   }
   break;
   case 'DFFN':
   {
    fOptions->DefaultFolder.SetTo(fDFDefaultFolder->Text());
    fOptions->Save();
   }
   break;
   case 'DFCK':
   {
    fOptions->DFOnlyStart=fDFCheckBox->Value();
    fOptions->Save();
   }
   break;
   case 'APRT':
   {
    fOptions->ReakcioIdo = (uint32) (fAPSlider->Position() * 1000);
    fOptions->Save();
   }
   break;
   case 'APTM':
   {
    fOptions->ReakcioIdoTM = (uint32) (fAPTMSlider->Position() * 1000);
    fOptions->Save();
   }
   break;
   case B_CANCEL:
   {
    if (IsHidden())
     Show();
   }
   break;
   default:
   {
    BWindow::MessageReceived(msg);
   }
   break;
  }
}
// - End - RHTML_Preferences_Win - MessageReceived -----------------------------------------------------------

// - End - Preferences Window - Source File ------------------------------------------------------------------
