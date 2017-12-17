//////////////////////////////////////////////////////////////////////
//      Globe Webeditor © Gergely Rózsahegyi (RoGer) 2001-2004      //
//                                                                  //
//                         Options Source File                      //
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
#include "RHTML_options.h"
#include <stdio.h>
#include <stdlib.h>
#include <UTF8.h>
#include <Entry.h>
#include <Directory.h>
#include <fstream>

using std::ifstream;

RHTMLOptions *fOptions;

// ---------------------------------------------------------------------------- RHTML_Options - RHTMLOptions -
RHTMLOptions::RHTMLOptions()
{
 fOptions=this;
 
 Name= new BString("Globe");
 Ver = new BString("0.4.0");
 Date= new BString("2004.10.21");
 DefaultFont= new BFont(be_plain_font);

 SettingsDir = "/boot/home/config/settings/Globe/";
// - Locale - English (Default)
 LocaleDb=0;
 CurrentLocale=0;
 Locale= (locale**) calloc(LocaleDb,sizeof(locale));
 AddLocale("English","RoGer","roger@beos.hu");
/*
 BString dir = SettingsDir;
 dir << "languages/languages.rdat";

 dir << "languages/languages.rdat";
 FILE *f;
 if( (f = fopen(dir.String(),"rt")) == NULL); else
 {
  int i=0;
  char tmp[100];
  while (feof(f)==false) 
  {
   tmp[0]=0;
   char ch=0;
   i=0;
   while ((ch!='\n')&&(!feof(f)))
   {
    fscanf(f,"%c",&ch);
    tmp[i]=ch;
    i++;
    tmp[i]=0;
   }
   tmp[i-1]=0;
   if (tmp!="")
   {
    LoadLocale(tmp);
   }
  }
 }*/

 BString path = SettingsDir;
 path << "languages";
	BDirectory dir(path.String());
	BNode node;
	node_ref noderef;

	if (dir.InitCheck()==B_OK)
	{
		BEntry entry;
		
		if (dir.GetEntry(&entry)==B_OK)
		{
			while (dir.GetNextEntry(&entry)==B_OK)
			{
				node.SetTo(&entry);	
				node.GetNodeRef(&noderef);				
				char *buffer = new char[255];
				entry.GetName(buffer);
				entry.GetName(buffer);
				LoadLocale(buffer);
					
//					AddDirectoryEntry(&entry);
			}
		}
	}


 

// - FileTypes
 FileTypesDb=3;
 FileTypes = (filetype**) calloc(FileTypesDb,sizeof(filetype));

// - FileTypes - SIMPLE TEXT
 FileTypes[0] = new filetype;
 FileTypes[0]->Name= new BString("TEXT");
 FileTypes[0]->KitDb=0;
 FileTypes[0]->Kit = (BString**) calloc(FileTypes[0]->KitDb,sizeof(BString));
 FileTypes[0]->FontColorDb=1;
 FileTypes[0]->FCName = (BString**) calloc(FileTypes[0]->FontColorDb,sizeof(BString));
 FileTypes[0]->Font = (rfont**) calloc(FileTypes[0]->FontColorDb,sizeof(rfont));
 FileTypes[0]->Color = (rgb_color**) calloc(FileTypes[0]->FontColorDb,sizeof(rgb_color));
 FileTypes[0]->FCName[0]= new BString("Text");
 font_family family;
 font_style style;
 float size;
 be_plain_font->GetFamilyAndStyle(&family,&style);
 size=be_plain_font->Size();
 FileTypes[0]->Font[0]= new rfont;
 FileTypes[0]->Font[0]->Family.SetTo("Default font");
 FileTypes[0]->Font[0]->Style.SetTo("Default style");
 FileTypes[0]->Font[0]->Size=-1;

 FileTypes[0]->Color[0]= new rgb_color;
 FileTypes[0]->Color[0]->red=0;
 FileTypes[0]->Color[0]->green=0;
 FileTypes[0]->Color[0]->blue=0;

// - FileTypes - HTML
 FileTypes[1] = new filetype;
 FileTypes[1]->Name= new BString("HTML");
 FileTypes[1]->KitDb=2;
 FileTypes[1]->Kit = (BString**) calloc(FileTypes[1]->KitDb,sizeof(BString));
 FileTypes[1]->Kit[0]= new BString("html");
 FileTypes[1]->Kit[1]= new BString("htm");
 FileTypes[1]->FontColorDb=5;
 FileTypes[1]->FCName = (BString**) calloc(FileTypes[1]->FontColorDb,sizeof(BString));
 FileTypes[1]->Font = (rfont**) calloc(FileTypes[1]->FontColorDb,sizeof(rfont));
 FileTypes[1]->Color = (rgb_color**) calloc(FileTypes[1]->FontColorDb,sizeof(rgb_color));
 FileTypes[1]->FCName[0]= new BString("Text");
 FileTypes[1]->FCName[1]= new BString("Comment");
 FileTypes[1]->FCName[2]= new BString("Parameter");
 FileTypes[1]->FCName[3]= new BString("Tag");
 FileTypes[1]->FCName[4]= new BString("Value");
 be_plain_font->GetFamilyAndStyle(&family,&style);
 size=be_plain_font->Size();
 FileTypes[1]->Font[0]= new rfont;
 FileTypes[1]->Font[1]= new rfont;
 FileTypes[1]->Font[2]= new rfont;
 FileTypes[1]->Font[3]= new rfont;
 FileTypes[1]->Font[4]= new rfont;
 FileTypes[1]->Font[0]->Family.SetTo("Default font");
 FileTypes[1]->Font[1]->Family.SetTo("Default font");
 FileTypes[1]->Font[2]->Family.SetTo("Default font");
 FileTypes[1]->Font[3]->Family.SetTo("Default font");
 FileTypes[1]->Font[4]->Family.SetTo("Default font");
 FileTypes[1]->Font[0]->Style.SetTo("Default style");
 FileTypes[1]->Font[1]->Style.SetTo("Default style");
 FileTypes[1]->Font[2]->Style.SetTo("Default style");
 FileTypes[1]->Font[3]->Style.SetTo("Default style");
 FileTypes[1]->Font[4]->Style.SetTo("Default style");
 FileTypes[1]->Font[0]->Size=-1;
 FileTypes[1]->Font[1]->Size=-1;
 FileTypes[1]->Font[2]->Size=-1;
 FileTypes[1]->Font[3]->Size=-1;
 FileTypes[1]->Font[4]->Size=-1;

 FileTypes[1]->Color[0]= new rgb_color;
 FileTypes[1]->Color[1]= new rgb_color;
 FileTypes[1]->Color[2]= new rgb_color;
 FileTypes[1]->Color[3]= new rgb_color;
 FileTypes[1]->Color[4]= new rgb_color;
 FileTypes[1]->Color[0]->red=0;
 FileTypes[1]->Color[0]->green=0;
 FileTypes[1]->Color[0]->blue=0;
 FileTypes[1]->Color[1]->red=255;
 FileTypes[1]->Color[1]->green=0;
 FileTypes[1]->Color[1]->blue=0;
 FileTypes[1]->Color[2]->red=0;
 FileTypes[1]->Color[2]->green=0;
 FileTypes[1]->Color[2]->blue=255;
 FileTypes[1]->Color[3]->red=0;
 FileTypes[1]->Color[3]->green=128;
 FileTypes[1]->Color[3]->blue=0;
 FileTypes[1]->Color[4]->red=128;
 FileTypes[1]->Color[4]->green=128;
 FileTypes[1]->Color[4]->blue=128;

// - DEVELOPMENT
 FileTypes[2] = new filetype;
 FileTypes[2]->Name= new BString("PHP (Dev.)");
 FileTypes[2]->KitDb=2;
 FileTypes[2]->Kit = (BString**) calloc(FileTypes[2]->KitDb,sizeof(BString));
 FileTypes[2]->Kit[0]= new BString("php");
 FileTypes[2]->Kit[1]= new BString("php3");
 FileTypes[2]->FontColorDb=6;
 FileTypes[2]->FCName = (BString**) calloc(FileTypes[2]->FontColorDb,sizeof(BString));
 FileTypes[2]->Font = (rfont**) calloc(FileTypes[2]->FontColorDb,sizeof(rfont));
 FileTypes[2]->Color = (rgb_color**) calloc(FileTypes[2]->FontColorDb,sizeof(rgb_color));
 FileTypes[2]->FCName[0]= new BString("Text");
 FileTypes[2]->FCName[1]= new BString("Comment");
 FileTypes[2]->FCName[2]= new BString("Parameter");
 FileTypes[2]->FCName[3]= new BString("Tag");
 FileTypes[2]->FCName[4]= new BString("Value");
 FileTypes[2]->FCName[5]= new BString("PHP");
 FileTypes[2]->Font[0]= new rfont;
 FileTypes[2]->Font[1]= new rfont;
 FileTypes[2]->Font[2]= new rfont;
 FileTypes[2]->Font[3]= new rfont;
 FileTypes[2]->Font[4]= new rfont;
 FileTypes[2]->Font[5]= new rfont;
 FileTypes[2]->Font[0]->Family.SetTo((const char*) family);
 FileTypes[2]->Font[1]->Family.SetTo((const char*) family);
 FileTypes[2]->Font[2]->Family.SetTo((const char*) family);
 FileTypes[2]->Font[3]->Family.SetTo((const char*) family);
 FileTypes[2]->Font[4]->Family.SetTo((const char*) family);
 FileTypes[2]->Font[5]->Family.SetTo((const char*) family);
 FileTypes[2]->Font[0]->Style.SetTo((const char*) style);
 FileTypes[2]->Font[1]->Style.SetTo((const char*) style);
 FileTypes[2]->Font[2]->Style.SetTo((const char*) style);
 FileTypes[2]->Font[3]->Style.SetTo((const char*) style);
 FileTypes[2]->Font[4]->Style.SetTo((const char*) style);
 FileTypes[2]->Font[5]->Style.SetTo((const char*) style);
 FileTypes[2]->Font[0]->Size=size;
 FileTypes[2]->Font[1]->Size=size;
 FileTypes[2]->Font[2]->Size=size;
 FileTypes[2]->Font[3]->Size=size;
 FileTypes[2]->Font[4]->Size=size;
 FileTypes[2]->Font[5]->Size=size;
 FileTypes[2]->Color[0]= new rgb_color;
 FileTypes[2]->Color[1]= new rgb_color;
 FileTypes[2]->Color[2]= new rgb_color;
 FileTypes[2]->Color[3]= new rgb_color;
 FileTypes[2]->Color[4]= new rgb_color;
 FileTypes[2]->Color[5]= new rgb_color;
 FileTypes[2]->Color[0]->red=0;
 FileTypes[2]->Color[0]->green=0;
 FileTypes[2]->Color[0]->blue=0;
 FileTypes[2]->Color[1]->red=255;
 FileTypes[2]->Color[1]->green=0;
 FileTypes[2]->Color[1]->blue=0;
 FileTypes[2]->Color[2]->red=0;
 FileTypes[2]->Color[2]->green=0;
 FileTypes[2]->Color[2]->blue=255;
 FileTypes[2]->Color[3]->red=0;
 FileTypes[2]->Color[3]->green=128;
 FileTypes[2]->Color[3]->blue=0;
 FileTypes[2]->Color[4]->red=128;
 FileTypes[2]->Color[4]->green=128;
 FileTypes[2]->Color[4]->blue=128;
 FileTypes[2]->Color[5]->red=255;
 FileTypes[2]->Color[5]->green=128;
 FileTypes[2]->Color[5]->blue=0;
// - END DEVELOPMENT

// - CR_LF
 CRLF_LF=true;
 LF_CRLF=false;

// - Encoding
 Encoding=B_UTF8_CONVERSION;

// - Views
 LeftView=false;
 TopView=true;
 NumView=true;
 WordWrap=true;
 LeftViewOrient=B_LEFT;
 TopViewOrient=B_TOP;
 BottomViewOrient=B_BOTTOM;
 LeftViewWidth=150;
 TopViewIndex=0;
 BottomViewIndex=0;
// - Search
 IgnoreCase=true;
// - Default Folder
 DefaultFolder.SetTo("/boot/home");
 DFOnlyStart=true;
// - Reakcio
 ReakcioIdo = 300;
 ReakcioIdoTM = 1000;
// - Window Frame
 WinFrameLeft = 40;
 WinFrameTop = 60;
 WinFrameRight = 780;
 WinFrameBottom = 580;
// - Load
 Load();
}
// - End - RHTML_Options - RHTMLOptions ----------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------- rscanfline -
BString rscanfline(FILE *f)
{
 char tmp[1000];
 tmp[0]=0;
 char ch=0;
 int i=0;
 while ((ch!='\n')&&(!feof(f)))
 {
  fscanf(f,"%c",&ch);
  tmp[i]=ch;
  i++;
  tmp[i]=0;
 }
 tmp[i-1]=0;
 return BString(tmp);
}
// - End - rscanfline ----------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------- rgetcomment -
BString rgetcomment(BString str)
{
 BString tmp;
 if (str.FindFirst('#')>=0)
 {
  str.CopyInto(tmp,0,str.FindFirst('#'));
 } else tmp=str;
 return tmp;
}
// - End - rgetcomment ---------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------- rgetstring -
BString rgetstring(BString str)
{
 BString tmp1,tmp2,tmp3,tmp4;
 tmp1=str;
 if (tmp1.Length()>0)
 {
  int i=0;
  while (tmp1[i]==' ') i++;
  tmp1.CopyInto(tmp2,i,str.Length()-i);
 } else tmp2=tmp1;
 if (tmp2.Length()>0)
 {
  int i=tmp2.Length()-1;
  while (tmp2[i]==' ') i--;
  tmp2.CopyInto(tmp3,0,i+1);
 } else tmp3=tmp2;
 return tmp3;
}
// - End - rgetstring ----------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------- rgetstring -
BString rgetstring(BString *str)
{
 BString tmp1,tmp2,tmp3,tmp4;
 tmp1.SetTo(str->String());
 if (tmp1.Length()>0)
 {
  int i=0;
  while (tmp1[i]==' ') i++;
  tmp1.CopyInto(tmp2,i,str->Length()-i);
 } else tmp2=tmp1;
 if (tmp2.Length()>0)
 {
  int i=tmp2.Length()-1;
  while (tmp2[i]==' ') i--;
  tmp2.CopyInto(tmp3,0,i+1);
 } else tmp3=tmp2;
 return tmp3;
}
// - End - rgetstring ----------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------ RHTML_Options - Save -
void RHTMLOptions::Save()
{
 BString dir=SettingsDir;
 dir << "settings.rdat";
 FILE *f;
 if( (f = fopen(dir.String(),"w")) == NULL); else
 {
  // - CurrentLocale
  fprintf(f,"Language=%s\n",Locale[CurrentLocale]->Lang.String());
  // - Default Font
  font_family family;
  font_style style;
  DefaultFont->GetFamilyAndStyle(&family,&style);
  fprintf(f,"Default_Font_Family=%s\n",(const char *) family);
  fprintf(f,"Default_Font_Style=%s\n",(const char *) style);
  fprintf(f,"Default_Font_Size=%f\n",DefaultFont->Size());
  // - FileTypes
  for (uint i=0;i<FileTypesDb;i++)
  {
   for (int o=0;o<FileTypes[i]->KitDb;o++)
   {
    fprintf(f,"FileTypes_KIT=%s/%s\n",rgetstring(FileTypes[i]->Name).String(),rgetstring(FileTypes[i]->Kit[o]).String());
   }
   for (int o=0;o<FileTypes[i]->FontColorDb;o++)
   {
    fprintf(f,"FileTypes_FAMILY=%s/%s\\%s\n",rgetstring(FileTypes[i]->Name).String(),rgetstring(FileTypes[i]->FCName[o]).String(),rgetstring(FileTypes[i]->Font[o]->Family).String());
   }
   for (int o=0;o<FileTypes[i]->FontColorDb;o++)
   {
    fprintf(f,"FileTypes_STYLE=%s/%s\\%s\n",rgetstring(FileTypes[i]->Name).String(),rgetstring(FileTypes[i]->FCName[o]).String(),rgetstring(FileTypes[i]->Font[o]->Style).String());
   }
   for (int o=0;o<FileTypes[i]->FontColorDb;o++)
   {
    fprintf(f,"FileTypes_SIZE=%s/%s\\%f\n",rgetstring(FileTypes[i]->Name).String(),rgetstring(FileTypes[i]->FCName[o]).String(),FileTypes[i]->Font[o]->Size);
   }
   for (int o=0;o<FileTypes[i]->FontColorDb;o++)
   {
    fprintf(f,"FileTypes_COLOR_ALPHA=%s/%s\\%d\n",rgetstring(FileTypes[i]->Name).String(),rgetstring(FileTypes[i]->FCName[o]).String(),FileTypes[i]->Color[o]->alpha);
   }
   for (int o=0;o<FileTypes[i]->FontColorDb;o++)
   {
    fprintf(f,"FileTypes_COLOR_RED=%s/%s\\%d\n",rgetstring(FileTypes[i]->Name).String(),rgetstring(FileTypes[i]->FCName[o]).String(),FileTypes[i]->Color[o]->red);
   }
   for (int o=0;o<FileTypes[i]->FontColorDb;o++)
   {
    fprintf(f,"FileTypes_COLOR_GREEN=%s/%s\\%d\n",rgetstring(FileTypes[i]->Name).String(),rgetstring(FileTypes[i]->FCName[o]).String(),FileTypes[i]->Color[o]->green);
   }
   for (int o=0;o<FileTypes[i]->FontColorDb;o++)
   {
    fprintf(f,"FileTypes_COLOR_BLUE=%s/%s\\%d\n",rgetstring(FileTypes[i]->Name).String(),rgetstring(FileTypes[i]->FCName[o]).String(),FileTypes[i]->Color[o]->blue);
   }
  }
  // - LF_CRLF
  if (LF_CRLF)
   fprintf(f,"LF_CRLF=true\n");
  else
   fprintf(f,"LF_CRLF=false\n");
  // - CRLF_LF
  if (CRLF_LF)
   fprintf(f,"CRLF_LF=true\n");
  else
   fprintf(f,"CRLF_LF=false\n");
  // - Encoding
   fprintf(f,"Encoding=%d\n",Encoding);
  // - NumView
  if (NumView)
   fprintf(f,"NumView=true\n");
  else
   fprintf(f,"NumView=false\n");
  // - LeftView
  if (LeftView)
   fprintf(f,"LeftView=true\n");
  else
   fprintf(f,"LeftView=false\n");
  // - TopView
  if (TopView)
   fprintf(f,"TopView=true\n");
  else
   fprintf(f,"TopView=false\n");
  // - WordWrap
  if (WordWrap)
   fprintf(f,"WordWrap=true\n");
  else
   fprintf(f,"WordWrap=false\n");
  // - TopViewOrient
  if (TopViewOrient==B_TOP)
   fprintf(f,"TopViewOrient=TOP\n");
  if (TopViewOrient==B_BOTTOM)
   fprintf(f,"TopViewOrient=BOTTOM\n");
  if (TopViewOrient==B_LEFT)
   fprintf(f,"TopViewOrient=LEFT\n");
  if (TopViewOrient==B_RIGHT)
   fprintf(f,"TopViewOrient=RIGHT\n");
  // - BottomViewOrient
  if (BottomViewOrient==B_TOP)
   fprintf(f,"BottomViewOrient=TOP\n");
  if (BottomViewOrient==B_BOTTOM)
   fprintf(f,"BottomViewOrient=BOTTOM\n");
  if (BottomViewOrient==B_LEFT)
   fprintf(f,"BottomViewOrient=LEFT\n");
  if (BottomViewOrient==B_RIGHT)
   fprintf(f,"BottomViewOrient=RIGHT\n");
  // - LeftViewOrient
  if (LeftViewOrient==B_TOP)
   fprintf(f,"LeftViewOrient=TOP\n");
  if (LeftViewOrient==B_BOTTOM)
   fprintf(f,"LeftViewOrient=BOTTOM\n");
  if (LeftViewOrient==B_LEFT)
   fprintf(f,"LeftViewOrient=LEFT\n");
  if (LeftViewOrient==B_RIGHT)
   fprintf(f,"LeftViewOrient=RIGHT\n");
  // - LeftViewWidth
   fprintf(f,"LeftViewWidth=%f\n",LeftViewWidth);
  // - TopViewIndex
   fprintf(f,"TopViewIndex=%ld\n",TopViewIndex);
  // - BottomViewIndex
   fprintf(f,"BottomViewIndex=%ld\n",BottomViewIndex);
  // - IgnoreCase
  if (IgnoreCase)
   fprintf(f,"IgnoreCase=true\n");
  else
   fprintf(f,"IgnoreCase=false\n");
  // - DefaultFolder
  // - DFOnlyStart
  if (DFOnlyStart)
   fprintf(f,"DFOnlyStart=true\n");
  else
   fprintf(f,"DFOnlyStart=false\n");
   fprintf(f,"ReakcioIdo=%ld\n",ReakcioIdo);
   fprintf(f,"ReakcioIdoTM=%ld\n",ReakcioIdoTM);
  // - DefaultFolder
  fprintf(f,"DefaultFolder=%s\n",DefaultFolder.String());
  // - WinFrameLeft
   fprintf(f,"WinFrameLeft=%f\n",WinFrameLeft);
  // - WinFrameTop
   fprintf(f,"WinFrameTop=%f\n",WinFrameTop);
  // - WinFrameRight
   fprintf(f,"WinFrameRight=%f\n",WinFrameRight);
  // - WinFrameBottom
   fprintf(f,"WinFrameBottom=%f\n",WinFrameBottom);
  // --
  fclose(f); 
 }
}
// - End - RHTML_Options - RHTMLOptions ----------------------------------------------------------------------

// ------------------------------------------------------------------------------------ RHTML_Options - Load -
void RHTMLOptions::Load()
{
 BString dir=SettingsDir;
 dir << "settings.rdat";
 FILE *f;
 if( (f = fopen(dir.String(),"rt")) == NULL); else
 {
  while (!feof(f))
  {
   BString name,val1,val2,val3,val4,tmp;
   tmp=rgetstring(rgetcomment(rscanfline(f)));
   if (tmp.FindFirst('=')>=0)
   {
    tmp.CopyInto(name,0,tmp.FindFirst('='));
    name=rgetstring(name);
    if (name.ICompare("Language")==0)				// - Language
    {
     tmp.CopyInto(val1,tmp.FindFirst('=')+1,tmp.Length()-tmp.FindFirst('=')-1);
     val1=rgetstring(val1);
     for (int i=0;(uint) i<LocaleDb;i++)
     {
      if (Locale[i]->Lang.ICompare(val1.String())==0) CurrentLocale=i;
     }
    } else
    if (name.ICompare("Default_Font_Family")==0)	// - Default_Font_Family
    {
     tmp.CopyInto(val1,tmp.FindFirst('=')+1,tmp.Length()-tmp.FindFirst('=')-1);
     val1=rgetstring(val1);
     font_style fstyle;
     get_font_style((char *) val1.String(), 0, &fstyle);
     DefaultFont->SetFamilyAndStyle(val1.String(),fstyle);
    } else
    if (name.ICompare("Default_Font_Style")==0)	// - Default_Font_Style
    {
     tmp.CopyInto(val1,tmp.FindFirst('=')+1,tmp.Length()-tmp.FindFirst('=')-1);
     val1=rgetstring(val1);
     font_family family;
     font_style fstyle;
     DefaultFont->GetFamilyAndStyle(&family,&fstyle);
     DefaultFont->SetFamilyAndStyle(family,val1.String());
    } else
    if (name.ICompare("Default_Font_Size")==0)	// - Default_Font_Size
    {
     tmp.CopyInto(val1,tmp.FindFirst('=')+1,tmp.Length()-tmp.FindFirst('=')-1);
     val1=rgetstring(val1);
     DefaultFont->SetSize(atof(val1.String()));
    } else
    if (name.ICompare("FileTypes_KIT")==0)				// - FileTypes_Kit
    {
     tmp.CopyInto(val1,tmp.FindFirst('=')+1,tmp.FindFirst('/')-tmp.FindFirst('=')-1);
     val1=rgetstring(val1);
     tmp.CopyInto(val2,tmp.FindFirst('/')+1,tmp.Length()-tmp.FindFirst('/')-1);
     val2=rgetstring(val2);
     AddFTKit(FindFT(val1.String()),val2.String());
    } else
    if (name.ICompare("FileTypes_FAMILY")==0)			// - FileTypes_FONT_FAMILY
    {
     tmp.CopyInto(val1,tmp.FindFirst('=')+1,tmp.FindFirst('/')-tmp.FindFirst('=')-1);
     val1=rgetstring(val1);
     tmp.CopyInto(val2,tmp.FindFirst('/')+1,tmp.FindFirst('\\')-tmp.FindFirst('/')-1);
     val2=rgetstring(val2);
     tmp.CopyInto(val3,tmp.FindFirst('\\')+1,tmp.Length()-tmp.FindFirst('\\')-1);
     val3=rgetstring(val3);
     FileTypes[FindFT(val1.String())]->Font[FindFTFC(FindFT(val1.String()),val2.String())]->Family.SetTo(val3.String());
    } else
    if (name.ICompare("FileTypes_STYLE")==0)			// - FileTypes_FONT_STYLE
    {
     tmp.CopyInto(val1,tmp.FindFirst('=')+1,tmp.FindFirst('/')-tmp.FindFirst('=')-1);
     val1=rgetstring(val1);
     tmp.CopyInto(val2,tmp.FindFirst('/')+1,tmp.FindFirst('\\')-tmp.FindFirst('/')-1);
     val2=rgetstring(val2);
     tmp.CopyInto(val3,tmp.FindFirst('\\')+1,tmp.Length()-tmp.FindFirst('\\')-1);
     val3=rgetstring(val3);
     FileTypes[FindFT(val1.String())]->Font[FindFTFC(FindFT(val1.String()),val2.String())]->Style.SetTo(val3.String());
    } else
    if (name.ICompare("FileTypes_SIZE")==0)			// - FileTypes_FONT_SIZE
    {
     tmp.CopyInto(val1,tmp.FindFirst('=')+1,tmp.FindFirst('/')-tmp.FindFirst('=')-1);
     val1=rgetstring(val1);
     tmp.CopyInto(val2,tmp.FindFirst('/')+1,tmp.FindFirst('\\')-tmp.FindFirst('/')-1);
     val2=rgetstring(val2);
     tmp.CopyInto(val3,tmp.FindFirst('\\')+1,tmp.Length()-tmp.FindFirst('\\')-1);
     val3=rgetstring(val3);
     FileTypes[FindFT(val1.String())]->Font[FindFTFC(FindFT(val1.String()),val2.String())]->Size=atof(val3.String());
    } else
    if (name.ICompare("FileTypes_COLOR_ALPHA")==0)			// - FileTypes_COLOR_ALPHA
    {
     tmp.CopyInto(val1,tmp.FindFirst('=')+1,tmp.FindFirst('/')-tmp.FindFirst('=')-1);
     val1=rgetstring(val1);
     tmp.CopyInto(val2,tmp.FindFirst('/')+1,tmp.FindFirst('\\')-tmp.FindFirst('/')-1);
     val2=rgetstring(val2);
     tmp.CopyInto(val3,tmp.FindFirst('\\')+1,tmp.Length()-tmp.FindFirst('\\')-1);
     val3=rgetstring(val3);
     FileTypes[FindFT(val1.String())]->Color[FindFTFC(FindFT(val1.String()),val2.String())]->alpha=atoi(val3.String());
    } else
    if (name.ICompare("FileTypes_COLOR_RED")==0)			// - FileTypes_COLOR_RED
    {
     tmp.CopyInto(val1,tmp.FindFirst('=')+1,tmp.FindFirst('/')-tmp.FindFirst('=')-1);
     val1=rgetstring(val1);
     tmp.CopyInto(val2,tmp.FindFirst('/')+1,tmp.FindFirst('\\')-tmp.FindFirst('/')-1);
     val2=rgetstring(val2);
     tmp.CopyInto(val3,tmp.FindFirst('\\')+1,tmp.Length()-tmp.FindFirst('\\')-1);
     val3=rgetstring(val3);
     FileTypes[FindFT(val1.String())]->Color[FindFTFC(FindFT(val1.String()),val2.String())]->red=atoi(val3.String());
    } else
    if (name.ICompare("FileTypes_COLOR_GREEN")==0)			// - FileTypes_COLOR_GREEN
    {
     tmp.CopyInto(val1,tmp.FindFirst('=')+1,tmp.FindFirst('/')-tmp.FindFirst('=')-1);
     val1=rgetstring(val1);
     tmp.CopyInto(val2,tmp.FindFirst('/')+1,tmp.FindFirst('\\')-tmp.FindFirst('/')-1);
     val2=rgetstring(val2);
     tmp.CopyInto(val3,tmp.FindFirst('\\')+1,tmp.Length()-tmp.FindFirst('\\')-1);
     val3=rgetstring(val3);
     FileTypes[FindFT(val1.String())]->Color[FindFTFC(FindFT(val1.String()),val2.String())]->green=atoi(val3.String());
    } else
    if (name.ICompare("FileTypes_COLOR_BLUE")==0)			// - FileTypes_COLOR_BLUE
    {
     tmp.CopyInto(val1,tmp.FindFirst('=')+1,tmp.FindFirst('/')-tmp.FindFirst('=')-1);
     val1=rgetstring(val1);
     tmp.CopyInto(val2,tmp.FindFirst('/')+1,tmp.FindFirst('\\')-tmp.FindFirst('/')-1);
     val2=rgetstring(val2);
     tmp.CopyInto(val3,tmp.FindFirst('\\')+1,tmp.Length()-tmp.FindFirst('\\')-1);
     val3=rgetstring(val3);
     FileTypes[FindFT(val1.String())]->Color[FindFTFC(FindFT(val1.String()),val2.String())]->blue=atoi(val3.String());
    } else
    if (name.ICompare("LF_CRLF")==0)				// - LF_CRLF
    {
     tmp.CopyInto(val1,tmp.FindFirst('=')+1,tmp.Length()-tmp.FindFirst('=')-1);
     val1=rgetstring(val1);
     if (val1.ICompare("TRUE")==0) LF_CRLF=true; else LF_CRLF=false;
    } else
    if (name.ICompare("CRLF_LF")==0)				// - CRLF_LF
    {
     tmp.CopyInto(val1,tmp.FindFirst('=')+1,tmp.Length()-tmp.FindFirst('=')-1);
     val1=rgetstring(val1);
     if (val1.ICompare("TRUE")==0) CRLF_LF=true; else CRLF_LF=false;
    } else
    if (name.ICompare("Encoding")==0)				// - Encoding
    {
     tmp.CopyInto(val1,tmp.FindFirst('=')+1,tmp.Length()-tmp.FindFirst('=')-1);
     val1=rgetstring(val1);
     Encoding=atoi(val1.String());
    } else
    if (name.ICompare("NumView")==0)				// - NumView
    {
     tmp.CopyInto(val1,tmp.FindFirst('=')+1,tmp.Length()-tmp.FindFirst('=')-1);
     val1=rgetstring(val1);
     if (val1.ICompare("TRUE")==0) NumView=true; else NumView=false;
    } else
    if (name.ICompare("LeftView")==0)				// - LeftView
    {
     tmp.CopyInto(val1,tmp.FindFirst('=')+1,tmp.Length()-tmp.FindFirst('=')-1);
     val1=rgetstring(val1);
     if (val1.ICompare("TRUE")==0) LeftView=true; else LeftView=false;
    } else
    if (name.ICompare("TopView")==0)				// - TopView
    {
     tmp.CopyInto(val1,tmp.FindFirst('=')+1,tmp.Length()-tmp.FindFirst('=')-1);
     val1=rgetstring(val1);
     if (val1.ICompare("TRUE")==0) TopView=true; else TopView=false;
    } else
    if (name.ICompare("WordWrap")==0)				// - WordWrap
    {
     tmp.CopyInto(val1,tmp.FindFirst('=')+1,tmp.Length()-tmp.FindFirst('=')-1);
     val1=rgetstring(val1);
     if (val1.ICompare("TRUE")==0) WordWrap=true; else WordWrap=false;
    } else
    if (name.ICompare("TopViewOrient")==0)			// - TopViewOrient
    {
     tmp.CopyInto(val1,tmp.FindFirst('=')+1,tmp.Length()-tmp.FindFirst('=')-1);
     val1=rgetstring(val1);
     if (val1.ICompare("TOP")==0) TopViewOrient=B_TOP;
     if (val1.ICompare("BOTTOM")==0) TopViewOrient=B_BOTTOM;
     if (val1.ICompare("LEFT")==0) TopViewOrient=B_LEFT;
     if (val1.ICompare("RIGHT")==0) TopViewOrient=B_RIGHT;
    } else
    if (name.ICompare("BottomViewOrient")==0)			// - BottomViewOrient
    {
     tmp.CopyInto(val1,tmp.FindFirst('=')+1,tmp.Length()-tmp.FindFirst('=')-1);
     val1=rgetstring(val1);
     if (val1.ICompare("TOP")==0) BottomViewOrient=B_TOP;
     if (val1.ICompare("BOTTOM")==0) BottomViewOrient=B_BOTTOM;
     if (val1.ICompare("LEFT")==0) BottomViewOrient=B_LEFT;
     if (val1.ICompare("RIGHT")==0) BottomViewOrient=B_RIGHT;
    } else
    if (name.ICompare("LeftViewOrient")==0)				// - LeftViewOrient
    {
     tmp.CopyInto(val1,tmp.FindFirst('=')+1,tmp.Length()-tmp.FindFirst('=')-1);
     val1=rgetstring(val1);
     if (val1.ICompare("TOP")==0) LeftViewOrient=B_TOP;
     if (val1.ICompare("BOTTOM")==0) LeftViewOrient=B_BOTTOM;
     if (val1.ICompare("LEFT")==0) LeftViewOrient=B_LEFT;
     if (val1.ICompare("RIGHT")==0) LeftViewOrient=B_RIGHT;
    } else
    if (name.ICompare("LeftViewWidth")==0)				// - LeftViewWidth
    {
     tmp.CopyInto(val1,tmp.FindFirst('=')+1,tmp.Length()-tmp.FindFirst('=')-1);
     val1=rgetstring(val1);
     LeftViewWidth = atof(val1.String());
    } else
    if (name.ICompare("TopViewIndex")==0)				// - TopViewIndex
    {
     tmp.CopyInto(val1,tmp.FindFirst('=')+1,tmp.Length()-tmp.FindFirst('=')-1);
     val1=rgetstring(val1);
     TopViewIndex=atoi(val1.String());
    } else
    if (name.ICompare("BottomViewIndex")==0)			// - BottomViewIndex
    {
     tmp.CopyInto(val1,tmp.FindFirst('=')+1,tmp.Length()-tmp.FindFirst('=')-1);
     val1=rgetstring(val1);
     BottomViewIndex=atoi(val1.String());
    } else
    if (name.ICompare("IgnoreCase")==0)					// - IgnoreCase
    {
     tmp.CopyInto(val1,tmp.FindFirst('=')+1,tmp.Length()-tmp.FindFirst('=')-1);
     val1=rgetstring(val1);
     if (val1.ICompare("TRUE")==0) IgnoreCase=true; else IgnoreCase=false;
    } else
    if (name.ICompare("DFOnlyStart")==0)				// - DFOnlyStart
    {
     tmp.CopyInto(val1,tmp.FindFirst('=')+1,tmp.Length()-tmp.FindFirst('=')-1);
     val1=rgetstring(val1);
     if (val1.ICompare("TRUE")==0) DFOnlyStart=true; else DFOnlyStart=false;
    } else
    if (name.ICompare("ReakcioIdo")==0)					// - ReakcioIdo
    {
     tmp.CopyInto(val1,tmp.FindFirst('=')+1,tmp.Length()-tmp.FindFirst('=')-1);
     val1=rgetstring(val1);
     ReakcioIdo = atoi(val1.String());
    } else
    if (name.ICompare("ReakcioIdoTM")==0)				// - ReakcioIdoTM
    {
     tmp.CopyInto(val1,tmp.FindFirst('=')+1,tmp.Length()-tmp.FindFirst('=')-1);
     val1=rgetstring(val1);
     ReakcioIdoTM = atoi(val1.String());
    } else
    if (name.ICompare("DefaultFolder")==0)				// - DefaultFolder
    {
     tmp.CopyInto(val1,tmp.FindFirst('=')+1,tmp.Length()-tmp.FindFirst('=')-1);
     DefaultFolder.SetTo(rgetstring(val1));
    }
    if (name.ICompare("WinFrameLeft")==0)				// - WinFrameLeft
    {
     tmp.CopyInto(val1,tmp.FindFirst('=')+1,tmp.Length()-tmp.FindFirst('=')-1);
     WinFrameLeft = atof(val1.String());
    }
    if (name.ICompare("WinFrameTop")==0)				// - WinFrameTop
    {
     tmp.CopyInto(val1,tmp.FindFirst('=')+1,tmp.Length()-tmp.FindFirst('=')-1);
     WinFrameTop = atof(val1.String());
    }
    if (name.ICompare("WinFrameRight")==0)				// - WinFrameRight
    {
     tmp.CopyInto(val1,tmp.FindFirst('=')+1,tmp.Length()-tmp.FindFirst('=')-1);
     WinFrameRight = atof(val1.String());
    }
    if (name.ICompare("WinFrameBottom")==0)				// - WinFrameBottom
    {
     tmp.CopyInto(val1,tmp.FindFirst('=')+1,tmp.Length()-tmp.FindFirst('=')-1);
     WinFrameBottom = atof(val1.String());
    }
// --
   }
  }
  fclose(f); 
 }
}
// - End - RHTML_Options - Load ------------------------------------------------------------------------------


// --------------------------------------------------------------------------------- RHTML_Options - FindFT -
int RHTMLOptions::FindFT(const char *filetype)
{
 int o=-1;
 for (uint i=0;i<FileTypesDb;i++)
  if (FileTypes[i]->Name->ICompare(filetype)==0)
   o=i;
 return o;
}
// - End - RHTML_Options - FindFT ----------------------------------------------------------------------------

// -------------------------------------------------------------------------------- RHTML_Options - FindFTFC -
int RHTMLOptions::FindFTFC(int filetype,const char *fontcolor)
{
 int o=-1;
 for (int i=0;i<FileTypes[filetype]->FontColorDb;i++)
  if (FileTypes[filetype]->FCName[i]->ICompare(fontcolor)==0)
   o=i;
 return o;
}
// - End - RHTML_Options - FindFTFC --------------------------------------------------------------------------
 			
// -------------------------------------------------------------------------------- RHTML_Options - AddFTKit -
int RHTMLOptions::AddFTKit(int filetype,const char *kit)
{
 int o=-1;
 for (int i=0;i<FileTypes[filetype]->KitDb;i++)
  if (FileTypes[filetype]->Kit[i]->ICompare(kit)==0)
  {
   o=i;
  }
 if (o==-1) 
 {
  FileTypes[filetype]->KitDb++;
  FileTypes[filetype]->Kit = (BString**) realloc(FileTypes[filetype]->Kit,sizeof(BString)*FileTypes[filetype]->KitDb);
  FileTypes[filetype]->Kit[FileTypes[filetype]->KitDb-1]= new BString(kit);
  return FileTypes[filetype]->KitDb-1;
 } else return o;
}
// - End - RHTML_Options - AddFTKit --------------------------------------------------------------------------

// ----------------------------------------------------------------------------- RHTML_Options - RemoveFTKit -
void RHTMLOptions::RemoveFTKit(int filetype,int num)
{
 if (num<FileTypes[filetype]->KitDb)
 {
  for (int i=num;i<FileTypes[filetype]->KitDb-1;i++)
  FileTypes[filetype]->Kit[i]=FileTypes[filetype]->Kit[i+1];
  FileTypes[filetype]->KitDb--;
  FileTypes[filetype]->Kit = (BString**) realloc(FileTypes[filetype]->Kit,sizeof(BString)*FileTypes[filetype]->KitDb);
 }
}
// - End - RHTML_Options - RemoveFTKit -----------------------------------------------------------------------

// ------------------------------------------------------------------------------- RHTML_Options - EditFTKit -
void RHTMLOptions::EditFTKit(int filetype,int num,const char *kit)
{
 int o=-1;
 for (int i=0;i<FileTypes[filetype]->KitDb;i++)
  if (FileTypes[filetype]->Kit[i]->ICompare(kit)==0)
  {
   o=i;
  }
 if (o==-1) 
 {
  if (num<FileTypes[filetype]->KitDb)
  {
   FileTypes[filetype]->Kit[num]->SetTo(kit);
  }
 }
}
// - End - RHTML_Options - EditFTKit -------------------------------------------------------------------------

// --------------------------------------------------------------------------------- RHTML_Options - GetName -
const char* RHTMLOptions::GetName()
{
 return Name->String();
}
// - End - RHTML_Options - GetName ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------------- RHTML_Options - GetVer -
const char* RHTMLOptions::GetVer()
{
 return Ver->String();
}
// - End - RHTML_Options - GetVer ----------------------------------------------------------------------------

// --------------------------------------------------------------------------------- RHTML_Options - GetDate -
const char* RHTMLOptions::GetDate()
{
 return Date->String();
}
// - End - RHTML_Options - GetDate ---------------------------------------------------------------------------

// ------------------------------------------------------------------------- RHTML_Options - GetLocaleString -
const char* RHTMLOptions::GetLocaleString(const char *name,const char *string)
{
 for (uint i=0;i<Locale[CurrentLocale]->LSDb;i++)
 {
  if ((Locale[CurrentLocale]->LString[i]->Name.ICompare(name)==0)&&(Locale[CurrentLocale]->LString[i]->String.ICompare(string)==0))
  {
   return Locale[CurrentLocale]->LString[i]->Locale.String();
  }
 }
 return string;
}
// - End - RHTML_Options - GetLocaleString -------------------------------------------------------------------

// ------------------------------------------------------------------------- RHTML_Options - AddLocaleString -
uint RHTMLOptions::AddLocaleString(uint num,const char *name,const char *string,const char *locale)
{
 Locale[num]->LSDb++;
 Locale[num]->LString = (locale_string**) realloc(Locale[num]->LString,sizeof(locale_string)*Locale[num]->LSDb);
 Locale[num]->LString[Locale[num]->LSDb-1]= new locale_string;
 Locale[num]->LString[Locale[num]->LSDb-1]->Name.SetTo(name);
 Locale[num]->LString[Locale[num]->LSDb-1]->String.SetTo(string);
 Locale[num]->LString[Locale[num]->LSDb-1]->Locale.SetTo(locale);
 
 return Locale[num]->LSDb-1;
}
// - End - RHTML_Options - GetLocaleString -------------------------------------------------------------------

// ------------------------------------------------------------------------------- RHTML_Options - AddLocale -
uint RHTMLOptions::AddLocale(const char *lang,const char *author,const char *email)
{
 LocaleDb++;
 Locale = (locale**) realloc(Locale,sizeof(locale)*LocaleDb);
 Locale[LocaleDb-1]= new locale;
 Locale[LocaleDb-1]->Lang.SetTo(lang);
 Locale[LocaleDb-1]->Author.SetTo(author);
 Locale[LocaleDb-1]->Email.SetTo(email);
 Locale[LocaleDb-1]->LSDb=0;
 Locale[LocaleDb-1]->LString= (locale_string**) calloc(1,sizeof(locale_string));

 return LocaleDb-1;
}
// - End - RHTML_Options - AddLocale -------------------------------------------------------------------------

// ------------------------------------------------------------------------------ RHTML_Options - LoadLocale -
uint RHTMLOptions::LoadLocale(const char *language)
{
// FILE *f;
 ifstream file;
 int i=0;
 int localenum=-1;
  char tmp[1000];
   BString temp1;
   BString name;
   BString string;
   BString locale;
   BString lang;
   BString author;
   BString email;
   BString filename = SettingsDir;
   filename << "languages/" << language;
 file.open(filename.String());
 if (!file.fail())
// if( (f = fopen(filename.String(),"rt")) == NULL); else
 {
//if (f == NULL) cout << "nulla" << endl;
  tmp[0]=0;
  char ch=0;
  i=0;
//  while ((ch!='\n')&&(!feof(f)))
  while ( ch != '\n' && !file.eof() )
  {
//   fscanf(f,"%c",&ch);
   file.get(ch);
   tmp[i]=ch;
   i++;
   tmp[i]=0;
  }
  tmp[i-1]=0;

  temp1.SetTo(tmp);
  int o1=temp1.FindFirst(':');
  int o2=temp1.FindFirst(':',o1+1);

  if ((o1>=0)&&(o2>=0))
  {
   temp1.CopyInto(lang,0,o1);
   temp1.CopyInto(author,o1+1,o2-o1-1);
   temp1.CopyInto(email,o2+1,temp1.Length()-o2-1);

  localenum=AddLocale(lang.String(),author.String(),email.String());
//   while (feof(f)==false) 
   while (!file.eof()) 
   {
    tmp[0]=0;
    char ch=0;
    i=0;
//    while ((ch!='\n')&&(!feof(f)))
    while (ch!='\n' && !file.eof())
    {
     file.get(ch);
//     fscanf(f,"%c",&ch);
     tmp[i]=ch;
     i++;
     tmp[i]=0;
    }
    tmp[i-1]=0;

    temp1.SetTo(tmp);
    o1=temp1.FindFirst(':');
    o2=temp1.FindFirst('=');
    if ((o1>=0)&&(o2>=0))
    {
     temp1.CopyInto(name,0,o1);
     temp1.CopyInto(string,o1+1,o2-o1-1);
     temp1.CopyInto(locale,o2+1,temp1.Length()-o2-1);
     AddLocaleString(localenum,name.String(),string.String(),locale.String());
    }
   }
  }
//  fclose(f);
 }
 return localenum;
}
// - End - RHTML_Options - LoadLocale ------------------------------------------------------------------------

// - End - Options - Source File -----------------------------------------------------------------------------
