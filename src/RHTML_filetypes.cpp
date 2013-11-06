//////////////////////////////////////////////////////////////////////
//      Globe Webeditor © Gergely Rózsahegyi (RoGer) 2001-2002      //
//                                                                  //
//                       File Types  Source File                    //
//                                                                  //
//                       Last Update: 2002.08.01					//
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
#include <TextView.h>
#include <stdio.h>
#include "RHTML_filetypes.h"

// ----------------------------------------------------------------------- RHTML_File_Types - RHTMLFileTypes -
RHTMLFileTypes::RHTMLFileTypes(uint ft,BTextView *textview)
{
 FileType=ft;
 fTextView=textview;
}
// - End - RHTML_File_Types - RHTMLFileTypes -----------------------------------------------------------------

// ---------------------------------------------------------------------- RHTML_File_Types - SetFontAndColor -
void RHTMLFileTypes::SetFontAndColor(BString *oldtext,BString *currenttext,int mode=0,int offset1=-1,int offset2=-1)
{
 if (fOptions->FileTypes[FileType]->Name->ICompare("HTML")==0)
 {
  SetHTMLFC(oldtext,currenttext,offset1,offset2,mode);
 } else 
 if (fOptions->FileTypes[FileType]->Name->ICompare("PHP (Dev.)")==0)
 {
  SetPHPFC(oldtext,currenttext,offset1,offset2,mode);
 }
}
// - End - RHTML_File_Types - SetFontAndColor ----------------------------------------------------------------

// -------------------------------------------------------------------------- RHTML_File_Types - GetFileType -

uint RHTMLFileTypes::GetFileType()
{
 return FileType; 
}
// - End - RHTML_File_Types - GetFileType --------------------------------------------------------------------

// -------------------------------------------------------------------------- RHTML_File_Types - SetFileType -
void RHTMLFileTypes::SetFileType(uint FT)
{
 FileType=FT;
}
// - End - RHTML_File_Types - SetFileType --------------------------------------------------------------------

// ---------------------------------------------------------------------------- RHTML_File_Types - SetHTMLFC -
void RHTMLFileTypes::SetHTMLFC(BString *oldtext,BString *currenttext,int offset1,int offset2,int mode)
{
 int oldcmt1db=0;
 int curcmt1db=0;
 int oldcmt2db=0;
 int curcmt2db=0;
 int tmp=0;
  
 while ((tmp=oldtext->FindFirst("<!--",tmp+1))>-1) oldcmt1db++;
 tmp=0;
 while ((tmp=currenttext->FindFirst("<!--",tmp+1))>-1) curcmt1db++;
 tmp=0;
 while ((tmp=oldtext->FindFirst("-->",tmp+1))>-1) oldcmt2db++;
 tmp=0;
 while ((tmp=currenttext->FindFirst("-->",tmp+1))>-1) curcmt2db++;

 if ((oldcmt1db==curcmt1db) && (oldcmt2db==curcmt2db)&&(!mode))
 {
  if ((offset1!=-1)&&(offset2!=-1))
  {
   FindHTMLTAGs(offset1,offset2,currenttext);
  } else
  if (offset1!=-1)
  {
   FindHTMLTAGs(offset1,offset1,currenttext);
  } else
  {
   FindHTMLTAG(0,currenttext);
  }  
 } else                                   // - Végignézi az egész HTML -t
 {
  FindHTMLTAGs(0,currenttext->Length(),currenttext);
 }
}
// - End - RHTML_File_Types - SetHTMLFC ----------------------------------------------------------------------

// ------------------------------------------------------------------------- RHTML_File_Types - SetHTMLTAGFC -
void RHTMLFileTypes::SetHTMLTAGFC(int x,int y,BString *currenttext)
{
 int i=0;
 int p=0;
 int q=0;
 int elso=0;

 PutColor(x,x,2);
 PutColor(y,y,2);

 if (((p=currenttext->FindFirst(" ",x+1))>-1)&&((currenttext->FindFirst(" ",x+1))<y))
 {
  PutColor(x+1,p-1,3);
  elso=p+1;
  p++;
  while (((i=currenttext->FindFirst("=",p))>-1)&&((currenttext->FindFirst("=",p))<y))
  {
   p=i+1;
   PutColor(elso,i-1,2);
   PutColor(i,i,2);
   if (currenttext->String()[i+1]=='"')
   {
    if (((q=currenttext->FindFirst("\"",i+2))>-1)&&((currenttext->FindFirst("\"",i+2))<y))
    {
     PutColor(i+1,i+1,2);
     PutColor(q,q,2);
     PutColor(i+2,q-1,4);
     elso=q+1;
     p=q+1;
    }else
    {
     PutColor(i+1,i+1,2);
     PutColor(i+2,y-1,4);
     p=y;
    }
   } else
   {
    if (((q=currenttext->FindFirst(" ",i+1))>-1)&&((currenttext->FindFirst(" ",i+1))<y))
    {
     PutColor(i+1,q-1,4);
     elso=q+1;
     p=q+1;
    } else
    {
     PutColor(i+1,y-1,4);
     p=y;
     elso=y;
    }
   }
  }
  PutColor(elso,y-1,2);
 } else
 {
  PutColor(x+1,y-1,3);
 }
}
// - End - RHTML_File_Types - SetHTMLTAGFC -------------------------------------------------------------------

// ------------------------------------------------------------------------- RHTML_File_Types - FindHTMLTAGs -
void RHTMLFileTypes::FindHTMLTAGs(int x,int y,BString *currenttext)
{
 int i=0;
 int o=0;
 int p=x;
 int elso=x;
 int utolso=y;
 if ((currenttext->FindLast("-->",x)>=currenttext->FindLast("<!--",x+3))||(currenttext->FindFirst("-->",x)==-1))
 {
  if ((p=currenttext->FindLast(">",x))!=-1)
  {
   elso=++p;
  } else p=0;

  if ((utolso=currenttext->FindFirst("<",y))==-1)
  {
   utolso=currenttext->Length();
  } else utolso--;
  
  while (((i=currenttext->FindFirst("<",p))>-1)&&((currenttext->FindFirst("<",p))<=utolso))
  {
   if ((currenttext->ByteAt(i+1)=='!')&&(currenttext->ByteAt(i+2)=='-')&&(currenttext->ByteAt(i+3)=='-')&&(currenttext->FindFirst("-->",i+3)!=-1))
   {
    PutColor(elso,i-1,0);
    o=currenttext->FindFirst("-->",i+3);
    PutColor(i,o+3,1);
    elso=o+3;
    p=o+3;
   } else
   {
    p=i+1;
    PutColor(elso,i-1,0);
    elso=i;
    if (((o=currenttext->FindFirst(">",i+1))>-1)&&((currenttext->FindFirst(">",i+1))<=utolso))
    {
     SetHTMLTAGFC(i,o,currenttext);
     elso=o+1;
     p=o+1;
    } else
    {
     p=y;
    }
   }
  }
  PutColor(elso,utolso,0);
 }else
 {
  PutColor(currenttext->FindLast("<!--",x+3),currenttext->FindFirst("-->",x)+2,1);
 }
}
// - End - RHTML_File_Types - FindHTMLTAGs -------------------------------------------------------------------

// -------------------------------------------------------------------------- RHTML_File_Types - FindHTMLTAG -
void RHTMLFileTypes::FindHTMLTAG(int x,BString *currenttext)
{
 int i=x;
 int o=x;
 if ((currenttext->FindLast("-->",x)>=currenttext->FindLast("<!--",x+3))||(currenttext->FindFirst("-->",x)==-1))
 {
  while ((i>=0)&&(currenttext->String()[i]!='>')&&(currenttext->String()[i]!='<'))
  {
   i--;
  }
  if (i>=0)
  {
   if (currenttext->String()[i]=='>') // - '>'
   {
    while ((o<currenttext->Length())&&(currenttext->String()[o]!='>')&&(currenttext->String()[o]!='<'))
    {
     o++;
    }
    if (currenttext->String()[o]=='<')
    {
     PutColor(i+1,o-1,0);
    }
    if (currenttext->String()[o]=='>')
    {
     PutColor(i+1,o,0);
    }
   }
   if (currenttext->String()[i]=='<') // - '<'
   {
    while ((o<currenttext->Length())&&(currenttext->String()[o]!='>')&&(currenttext->String()[o]!='<'))
    {
     o++;
    }
    if (currenttext->String()[o]=='<')
    {
     PutColor(i,o-1,0);
    }
    if (currenttext->String()[o]=='>')
    {
     SetHTMLTAGFC(i,o,currenttext);
    }
   }
  } else
 {
  while ((o<currenttext->Length())&&(currenttext->String()[o]!='>')&&(currenttext->String()[o]!='<'))
  {
   o++;
  }
   if (currenttext->String()[o]=='<')
   {
    PutColor(0,o-1,0);
   }
   if (currenttext->String()[o]=='>')
   {
    PutColor(0,o,0);
   }
 }
 } else
 {
  PutColor(currenttext->FindLast("<!--",x+2),currenttext->FindFirst("-->",x)+2,1);
 }
}
// - End - RHTML_File_Types - FindHTMLTAG --------------------------------------------------------------------

// ----------------------------------------------------------------------------- RHTML_File_Types - PutColor -
void RHTMLFileTypes::PutColor(int32 x,int32 y,int fontcolor)
{;
	font_family family;
	font_style style;
	float size;
	fOptions->DefaultFont->GetFamilyAndStyle(&family, &style);
	size = fOptions->DefaultFont->Size();

	if (fOptions->FileTypes[FileType]->Font[fontcolor]->Style.ICompare("Default Style") != 0)
		strcpy(style, fOptions->FileTypes[FileType]->Font[fontcolor]->Style.String());
 
	BFont font;
	font.SetFamilyAndStyle(family, style);

	font.SetSize(size);


	bool kell = false;

	for (int i = x; i <= y; i++)
	{
		rgb_color col;
		BFont fnt;
		fTextView->GetFontAndColor(i, &fnt, &col);
		if (fnt != font)
		kell = true;
		if (col.red != fOptions->FileTypes[FileType]->Color[fontcolor]->red)
			kell = true;
		if (col.green != fOptions->FileTypes[FileType]->Color[fontcolor]->green)
			kell = true;
		if (col.blue != fOptions->FileTypes[FileType]->Color[fontcolor]->blue)
			kell = true;
	}

	if (kell)
		fTextView->SetFontAndColor(x,y+1,&font,B_FONT_ALL,fOptions->FileTypes[FileType]->Color[fontcolor]);
}
// - End - RHTML_File_Types - PutColor -----------------------------------------------------------------------

// ----------------------------------------------------------------------------- RHTML_File_Types - SetPHPFC -
void RHTMLFileTypes::SetPHPFC(BString *oldtext,BString *currenttext,int offset1,int offset2,int mode)
{
 FindPHPBE(currenttext);
 int oldcmt1db=0;
 int curcmt1db=0;
 int oldcmt2db=0;
 int curcmt2db=0;
 int tmp=0;
  
 while ((tmp=oldtext->FindFirst("<?",tmp+1))>-1) oldcmt1db++;
 tmp=0;
 while ((tmp=currenttext->FindFirst("<?",tmp+1))>-1) curcmt1db++;
 tmp=0;
 while ((tmp=oldtext->FindFirst("?>",tmp+1))>-1) oldcmt2db++;
 tmp=0;
 while ((tmp=currenttext->FindFirst("?>",tmp+1))>-1) curcmt2db++;

 if ((oldcmt1db==curcmt1db) && (oldcmt2db==curcmt2db)&&(!mode))
 {
  if ((offset1!=-1)&&(offset2!=-1))
  {
   FindPHPTAGs(offset1,offset2,currenttext);
  } else
  if (offset1!=-1)
  {
   FindPHPTAGs(offset1,offset1,currenttext);
  } else
  {
   FindPHPTAGs(0,0,currenttext);
  }  
 } else                                   // - Végignézi az egész PHP -t
 {
  FindPHPTAGs(0,currenttext->Length(),currenttext);
 }
}
// - End - RHTML_File_Types - SetPHPFC -----------------------------------------------------------------------

// ---------------------------------------------------------------------------- RHTML_File_Types - FindPHPBE -
void RHTMLFileTypes::FindPHPBE(BString *currenttext)
{
 begindb=0;
 enddb=0;
 int a,b,elso,utolso;
 elso=0;
 utolso=currenttext->Length();
 a=0;
 while ((a=currenttext->FindFirst("<?",elso))>-1)
 {
  begin[begindb]=a;
  begindb++;
  if ((b=FindPHPFEnd_old(a,currenttext))>-1)
  {
   end[begindb]=b;
   enddb++;
   elso=b;
  } else
  {
   elso=utolso;
  }
 }
}
// - End - RHTML_File_Types - FindPHPBE ----------------------------------------------------------------------

// ------------------------------------------------------------------------ RHTML_File_Types - FindPHPBBegin -
int RHTMLFileTypes::FindPHPBBegin(int x,BString *currenttext)
{
 for (int i=begindb-1;i>=0;i--)
 {
  if (begin[i]<x) return begin[i];
 }
 return -1;
}
// - End - RHTML_File_Types - FindPHPBBegin ------------------------------------------------------------------

// -------------------------------------------------------------------------- RHTML_File_Types - FindPHPBEnd -
int RHTMLFileTypes::FindPHPBEnd(int x,BString *currenttext)
{
 for (int i=enddb-1;i>=0;i--)
 {
  if (end[i]<x) return end[i];
 }
 return -1;
}
// - End - RHTML_File_Types - FindPHPBEnd --------------------------------------------------------------------

// ------------------------------------------------------------------------ RHTML_File_Types - FindPHPFBegin -
int RHTMLFileTypes::FindPHPFBegin(int x,BString *currenttext)
{
 for (int i=0;i<begindb;i++)
 {
  if (begin[i]>x) return begin[i];
 }
 return -1;
}
// - End - RHTML_File_Types - FindPHPFBegin ------------------------------------------------------------------

// -------------------------------------------------------------------------- RHTML_File_Types - FindPHPFEnd -
int RHTMLFileTypes::FindPHPFEnd(int x,BString *currenttext)
{
 for (int i=0;i<enddb;i++)
 {
  if (end[i]>x) return end[i];
 }
 return -1;
}
// - End - RHTML_File_Types - FindPHPFEnd --------------------------------------------------------------------

// ---------------------------------------------------------------------- RHTML_File_Types - FindPHPFEnd_old -
int RHTMLFileTypes::FindPHPFEnd_old(int x,BString *currenttext)
{
 int i,o,p,s;
 i=0;
 s=x;
 while (x<=currenttext->Length())
 {
  if ((o=currenttext->FindFirst("?>",s))>-1)
  {
   int q,r,t;
   q=x;
   t=0; 
   while (q<o)
   {
    if (((r=currenttext->FindFirst('"',q))!=-1)&&(r<o))
    {
     if ((t%2)==1)
     {
      if ((currenttext->ByteAt(r-1)!='\\'))
      {
       t++;
       q=r+1;
      } else
      {
       q=r+1;
      }
     } else 
     {
      t++;
      q=r+1;
     }
    } else q=o;
   }
   if ((t%2)==0)
   {
    return o;
    p=o+2;
    q=o;
   } else
   {
    s=o+1;
   }
  } else
  {
   return -1;
  }
 }
  return -1;
}
// - End - RHTML_File_Types - FindPHPFEnd_old ----------------------------------------------------------------

// -------------------------------------------------------------------------- RHTML_File_Types - FindPHPTAGs -
void RHTMLFileTypes::FindPHPTAGs(int x,int y,BString *currenttext)
{
 int i=0;
 int o=0;
 int p=x;
 int elso=x;
 int utolso=y;

  if ((p=FindPHPBEnd(x,currenttext))!=-1)
  {
   elso=p+2;
  } else
  {
   elso=0;
   p=0;
  }

  if ((utolso=FindPHPFBegin(y,currenttext))==-1)
  {
   utolso=currenttext->Length();
  } else utolso--;

   p=elso;
  while (((i=FindPHPFBegin(p,currenttext))!=-1)&&(FindPHPFBegin(p,currenttext)+1<=utolso))
  {
  FindPHPHTMLTAGs(elso,i-1,currenttext);
   if ((o=FindPHPFEnd(i+2,currenttext))!=-1)
   {
    PutColor(i,o+1,5); // - PHP
    p=o+1;
    elso=o+2;
   } else
   {
    p=utolso;
   }
  }
  FindPHPHTMLTAGs(elso,utolso,currenttext);
}
// - End - RHTML_File_Types - FindPHPTAGs --------------------------------------------------------------------

// ---------------------------------------------------------------------- RHTML_File_Types - FindPHPHTMLTAGs -
void RHTMLFileTypes::FindPHPHTMLTAGs(int x,int y,BString *currenttext)
{
 int i=0;
 int o=0;
 int p=x;
 int q;
 int elso=x;
 int utolso=y;
 if ((FindPHPBEnd(x,currenttext)>FindPHPBBegin(x,currenttext))||(FindPHPFEnd(x,currenttext)==-1))
{

}// - - - - - ITTVOLTAM
 if ((currenttext->FindLast("-->",x)>=currenttext->FindLast("<!--",x+3))||(currenttext->FindFirst("-->",x)==-1))
 {
  if ((p=currenttext->FindLast(">",x))!=-1)
  {
   elso=++p;
  } else
  {
   p=0;
   elso=0;
  }

  if ((utolso=currenttext->FindFirst("<",y))==-1)
  {
   utolso=currenttext->Length();
  } else ;//utolso;

  if ((((i=currenttext->FindFirst(">",p))<currenttext->FindFirst("<",p))||(currenttext->FindFirst("<",p)==-1))&&(currenttext->FindFirst(">",p)>-1)&&((o=currenttext->FindLast(">",p))>-1)&&(currenttext->ByteAt(o-1)=='?'))
  {
   elso=i+1;
   p=i+1;
   SetPHPHTMLTAGFC(o+1,i,currenttext);
  }

  while (((i=currenttext->FindFirst("<",p))>-1)&&((currenttext->FindFirst("<",p))<=utolso))
  {
   if ((currenttext->ByteAt(i+1)=='!')&&(currenttext->ByteAt(i+2)=='-')&&(currenttext->ByteAt(i+3)=='-')&&((currenttext->FindFirst("-->",i+3)!=-1)||(FindPHPFBegin(i+3,currenttext)!=-1)))
   { // - Comment
    PutColor(elso,i-1,0);
    int q;
    if ((o=currenttext->FindFirst("-->",i+3))>-1)
    {
     if ((q=FindPHPFBegin(i,currenttext))>-1)
     {
      if (q<o)
      {
       PutColor(i,q,1);
       elso=q;
       p=q;
      } else
      {
       PutColor(i,o+3,1);
       elso=o+3;
       p=o+3;
      }
     } else
     {
      PutColor(i,o+3,1);
      elso=o+3;
      p=o+3;
     }
    } else
    {
     if ((q=FindPHPFBegin(i,currenttext))>-1)
     {
      PutColor(i,q,1);
      elso=q;
      p=q;
     }
    }
   } else // - PHP
   if ((currenttext->ByteAt(i+1)=='?')&&((q=FindPHPFEnd(i,currenttext))>-1))
   {
    PutColor(elso,q+1,5);
    elso=q+2;
    p=q+2;
   } else // - HTML
   {
    p=i+1;
    PutColor(elso,i-1,0);
    elso=i;
    if (((o=currenttext->FindFirst(">",i+1))>-1)&&((currenttext->FindFirst(">",i+1)<currenttext->FindFirst("<",i+1))||(currenttext->FindFirst("<",i+1)==-1))&&((currenttext->FindFirst(">",i+1))<=utolso))
    {
     SetPHPHTMLTAGFC(i,o,currenttext);
     elso=o+1;
     p=o+1;
    } else
    if (((currenttext->FindFirst(">",i+1))>-1)&&((q=currenttext->FindFirst("<",i+1))>-1)&&(currenttext->FindFirst(">",i+1)>currenttext->FindFirst("<",i+1))&&(currenttext->FindFirst("<",i+1)<=utolso)&&(currenttext->ByteAt(q+1)=='?'))
    {
     elso=q;
     SetPHPHTMLTAGFC(i,q-1,currenttext);
     p=q;
    } else
    {
     p=o;
    }
   }
  }
  PutColor(elso,utolso,0);
 }else
 {
  PutColor(currenttext->FindLast("<!--",x+3),currenttext->FindFirst("-->",x)+2,1);
 }
}
// - End - RHTML_File_Types - FindPHPHTMLTAGs ----------------------------------------------------------------

// ---------------------------------------------------------------------- RHTML_File_Types - SetPHPHTMLTAGFC -
void RHTMLFileTypes::SetPHPHTMLTAGFC(int x,int y,BString *currenttext)
{
 int i=0;
 int p=0;
 int q=0;
 int elso=0;

 PutColor(x,x,2);
 if (currenttext->ByteAt(y)=='>') PutColor(y,y,2); else y++;

 if (((p=currenttext->FindFirst(" ",x+1))>-1)&&((currenttext->FindFirst(" ",x+1))<y))
 {
  PutColor(x+1,p-1,3);
  elso=p+1;
  p++;

  while (((i=currenttext->FindFirst("=",p))>-1)&&((currenttext->FindFirst("=",p))<y))
  {
   p=i+1;
   PutColor(elso,i-1,2);
   PutColor(i,i,2);

   if (currenttext->String()[i+1]=='"')
   {
    if (((q=currenttext->FindFirst("\"",i+2))>-1)&&((currenttext->FindFirst("\"",i+2))<y))
    {
     PutColor(i+1,i+1,2);
     PutColor(q,q,2);
     PutColor(i+2,q-1,4);
     elso=q+1;
     p=q+1;
    }else
    {
     PutColor(i+1,i+1,2);
     PutColor(i+2,y-1,4);
     elso=y;
     p=y;
    }
   } else
   {

    if (((q=currenttext->FindFirst(" ",i+1))>-1)&&((currenttext->FindFirst(" ",i+1))<y))
    {
     PutColor(i+1,q-1,4);
     elso=q+1;
     p=q+1;
    } else
    {
     PutColor(i+1,y-1,4);
     p=y;
     elso=y;
    }
   }
  }

  PutColor(elso,y-1,2);
 } else
 {
  PutColor(x+1,y-1,3);
 }
}
// - End - RHTML_File_Types - SetPHPHTMLTAGFC ----------------------------------------------------------------

// - End - File Types - Source File --------------------------------------------------------------------------
