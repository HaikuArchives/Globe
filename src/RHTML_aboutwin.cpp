//////////////////////////////////////////////////////////////////////
//      Globe Webeditor © Gergely Rózsahegyi (RoGer) 2001-2003      //
//                                                                  //
//                       About Window Source File                   //
//                                                                  //
//                       Last Update: 2003.05.14					//
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
#include <StringView.h>
#include <Bitmap.h>
#include <stdio.h>

#include "RHTML_aboutwin.h"
#include "RHTML_aboutpic.h"
#include "URLView.h"

// ------------------------------------------------------------------------- RHTML_About_Win - RHTMLAboutWin -
RHTMLAboutWin::RHTMLAboutWin(BRect fRect,BWindow *parent): BWindow(fRect,fOptions->GetLocaleString("About_Window","About"),B_FLOATING_WINDOW,B_NOT_ZOOMABLE|B_NOT_MINIMIZABLE|B_NOT_RESIZABLE)
{ 
 fParent=parent;
 IsQuit=false;
 BBitmap *image;
 BView *fViewj = new BView(BRect(200,0,299,199),"view1",B_FOLLOW_LEFT_RIGHT,B_WILL_DRAW);
 BView *fView = new BView(BRect(0,0,199,199),"view2",B_FOLLOW_LEFT_RIGHT,B_WILL_DRAW);
 AddChild(fViewj);
 AddChild(fView);
 image = new BBitmap(BRect(0,0,99,199),  B_RGB32 );
 image->SetBits(AboutPicture, 60000, 0,  B_RGB32);
 //fViewj->SetViewBitmap(image,BRect(0,0,99,199),BRect(0,0,99,199),B_FOLLOW_TOP,B_FOLLOW_TOP);

 fView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
 BFont *fFont1= new BFont(be_plain_font);
 fFont1->SetSize(16);
 fFont1->SetFace(B_BOLD_FACE);

 BFont *fFont3= new BFont(be_plain_font);
 fFont3->SetSize(12);
 fFont3->SetFace(B_BOLD_FACE);

 rgb_color fekete={0,0,0,0};
 BString tmp;
 tmp.SetTo("");
 tmp << fOptions->GetLocaleString("About_Window","Ver") <<": " << fOptions->GetVer();

 BStringView *fStrView2 = new BStringView(BRect(10,1,190,20),"strview2",tmp.String());
 BFont *fFont2= new BFont(be_plain_font);
 fFont2->SetSize(12);
 fStrView2->SetAlignment(B_ALIGN_CENTER);
 fStrView2->SetFont(fFont2);
 fView->AddChild(fStrView2);

 tmp.SetTo("(");
 tmp << fOptions->GetDate() << ")";

 BStringView *fStrView3 = new BStringView(BRect(0,21,199,35),"strview3",tmp.String());
 fStrView3->SetAlignment(B_ALIGN_CENTER);
 fStrView3->SetFont(fFont2);
 fView->AddChild(fStrView3);

 // - Webpage
 URLView *web = new URLView( BRect(0,36,199,55),"web","http://globe.beos.hu/", "http://globe.beos.hu/");
 fView->AddChild(web);
 web->SetHoverEnabled(false);
 fFont2->SetFace(B_BOLD_FACE);
 web->SetFont(fFont2);
 fFont2->SetFace(B_REGULAR_FACE);
 web->SetAlignment(B_ALIGN_CENTER);
 web->SetColor(fekete);
 web->SetClickColor(fekete);
 web->SetUnderlineThickness(false);

 tmp.SetTo(fOptions->GetLocaleString("About_Window","Developer"));
 tmp << ":";

 BStringView *fStrView5 = new BStringView(BRect(0,45,199,80),"strview3",tmp.String());
 fStrView5->SetAlignment(B_ALIGN_CENTER);
 fStrView5->SetFont(fFont3);
 fView->AddChild(fStrView5);

 // - RoGer
 URLView *roger = new URLView( BRect(0,45,199,95),"roger","Gergely Rózsahegyi (RoGer)", "roger@beos.hu");
 fView->AddChild(roger);
 roger->SetHoverEnabled(false);
 roger->AddAttribute("META:name","Gergely Rózsahegyi");
 roger->AddAttribute("META:nickname","RoGer");
 roger->AddAttribute("META:url","http://www.beos.hu/");
 roger->AddAttribute("META:country","Hungary");
 roger->SetFont(fFont2);
 roger->SetAlignment(B_ALIGN_CENTER);
 roger->SetColor(fekete);
 roger->SetClickColor(fekete);
 roger->SetUnderlineThickness(false);

 URLView *roger2 = new URLView( BRect(0,45,199,110),"roger","roger@beos.hu", "roger@beos.hu");
 fView->AddChild(roger2);
 roger2->SetHoverEnabled(false);
 roger2->AddAttribute("META:name","Gergely Rózsahegyi");
 roger2->AddAttribute("META:nickname","RoGer");
 roger2->AddAttribute("META:url","http://www.beos.hu/");
 roger2->AddAttribute("META:country","Hungary");
 roger2->SetFont(fFont2);
 roger2->SetAlignment(B_ALIGN_CENTER);
 roger2->SetColor(fekete);
 roger2->SetClickColor(fekete);
 roger2->SetUnderlineThickness(false);

 tmp.SetTo(fOptions->GetLocaleString("About_Window","Graphics, Tags, etc..."));
 tmp <<":";

 BStringView *fStrView7 = new BStringView(BRect(0,45,199,135),"strview3",tmp.String());
 fStrView7->SetAlignment(B_ALIGN_CENTER);
 fStrView7->SetFont(fFont3);
 fView->AddChild(fStrView7);

 // - kesigomu
 URLView *kesi = new URLView( BRect(0,45,199,150),"kesigomu","Attila Ökrös (kesigomu)", "kesigomu@beos.hu");
 fView->AddChild(kesi);
 kesi->SetHoverEnabled(false);
 kesi->SetAlignment(B_ALIGN_CENTER);
 kesi->AddAttribute("META:name","Attila Ökrös");
 kesi->AddAttribute("META:nickname","kesigomu");
 kesi->AddAttribute("META:url","http://www.beos.hu/");
 kesi->AddAttribute("META:country","Hungary");
 kesi->SetFont(fFont2);
 kesi->SetColor(fekete);
 kesi->SetClickColor(fekete);
 kesi->SetUnderlineThickness(false);

 URLView *kesi2 = new URLView( BRect(0,45,199,165),"kesigomu","kesigomu@beos.hu", "kesigomu@beos.hu");
 fView->AddChild(kesi2);
 kesi2->SetHoverEnabled(false);
 kesi2->SetAlignment(B_ALIGN_CENTER);
 kesi2->AddAttribute("META:name","Attila Ökrös");
 kesi2->AddAttribute("META:nickname","kesigomu");
 kesi2->AddAttribute("META:url","http://www.beos.hu/");
 kesi2->AddAttribute("META:country","Hungary");
 kesi2->SetFont(fFont2);
 kesi2->SetColor(fekete);
 kesi2->SetClickColor(fekete);
 kesi2->SetUnderlineThickness(false);
 
 tmp.SetTo("TToolTip By: Robert Polic");

 BStringView *fStrView8 = new BStringView(BRect(0,45,199,185),"strview8",tmp.String());
 fStrView8->SetAlignment(B_ALIGN_CENTER);
 fStrView8->SetFont(fFont2);
 fView->AddChild(fStrView8);

 tmp.SetTo("URLView By: William Kakes");

 BStringView *fStrView9 = new BStringView(BRect(0,45,199,200),"strview8",tmp.String());
 fStrView9->SetAlignment(B_ALIGN_CENTER);
 fStrView9->SetFont(fFont2);
 fView->AddChild(fStrView9);

}
// - End - RHTML_About_Win - RHTMLAboutWin -------------------------------------------------------------------

// ------------------------------------------------------------------------------- RHTML_About_Win - SetQuit -
void RHTMLAboutWin::SetQuit(bool quit)
{
 IsQuit=quit;
}
// - End - RHTML_About_Win - SetQuit -------------------------------------------------------------------------
 
// ------------------------------------------------------------------------- RHTML_About_Win - QuitRequested -
bool RHTMLAboutWin::QuitRequested()
{
 if(!IsQuit) fParent->MessageReceived(new BMessage('ABWQ'));
 return BWindow::QuitRequested();
}
// - End - RHTML_About_Win - QuitRequested -------------------------------------------------------------------

// - End - About Window - Source File ------------------------------------------------------------------------
