//////////////////////////////////////////////////////////////////////
//      Globe Webeditor © Gergely Rózsahegyi (RoGer) 2001-2004      //
//                                                                  //
//                   Preferences Window  Header File                //
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

#ifndef _RHTMLPREFWIN_H_
#define _RHTMLPREFWIN_H_

// - Includes
#include <Window.h>
#include <MenuItem.h>
#include <Messenger.h>
#include <OutlineListView.h>
#include <View.h>
#include <MenuField.h>
#include <Menu.h>
#include <Alert.h>
#include <Button.h>
#include <TextControl.h>
#include <ListView.h>
#include <Box.h>
#include <ScrollView.h>
#include <ColorControl.h>
#include <FilePanel.h>
#include <CheckBox.h>
#include <Slider.h>

#include "RHTML_options.h"
#include "URLView.h"

const	uint32	R_FTH_Family_MSG=	'M001';
const	uint32	R_FTH_Style_MSG=	'M002';
const	uint32	R_FTH_Size_MSG=		'M003';
const	uint32	R_FTH_Color_MSG=	'M004';


// ----------------------------------------------------------------------------- RHTML_Preferences_Win Class -
class RHTMLPreferencesWin : public BWindow
{
 public:
								RHTMLPreferencesWin(BRect fRect,BWindow *parent);
				virtual bool	QuitRequested();
				virtual	void	FrameResized(float new_width, float new_height);
				virtual void	MessageReceived(BMessage *msg);
				void			SetQuit(bool quit);
 private:
				BWindow				*fParent;
				BOutlineListView	*fList;
				BView				*fView;
                BView				**fJView;
                uint				JViewDb;
				bool				IsQuit;

                BMenu				*fLangMenu;					// - Languages
				BMenuField			*fLanguages;
				BStringView			*fStrView1;
				BStringView			*fStrView2;
				BStringView			*fStrView3;
				BStringView			*fStrView4;
				BStringView			*fStrView5;
				BStringView			*fStrView6;
				BStringView			*fStrView7;
				URLView				*author;
				void				LSetAuthor();

                BMenu				*fFTFTMenu;					// - FileTypes
				BMenuField			*fFTFT;
				BMenuField			*fFTDFont;
				BMenuField			*fFTDFStyle;
				BMenuField			*fFTDFSize;
				BMenuItem			*fFTMItem;
				BBox				*fFTBox;
				BBox				*fFTFontsBox;
				BListView			*fFTKit;
				BButton				*fFTAddButton;
				BButton				*fFTEditButton;
				BButton				*fFTRemoveButton;
				BTextControl		*fFTStrView;
				uint				fFT;
				void				FTCreateKitList(uint FileType);
				void				FTSetFont(const char *font,const char *style ="");

				BBox				*fFTHFTBox;					// - FT - Highlighting
				BBox				*fFTHFontBox;
				BBox				*fFTHColorBox;
				BMenuField			*fFTHFT;
				BMenuField			*fFTHFC;
				BMenuField			*fFTHStyle;
				BColorControl		*fFTHCC;
				uint				fFTHFileTypes;
				uint				fFTHFontColor;
				void				FTHSetFT(uint FileType);
				void				FTHSetFSize(uint FontColor);
							
                BTextControl		*fDFDefaultFolder;			// - Default Folder
                BFilePanel			*fDFFilePanel;
                BCheckBox			*fDFCheckBox;

                BTextControl		*fAPAdvancedPrefs;			// - Advanced Prefs
				BSlider				*fAPSlider;
				BSlider				*fAPTMSlider;
				
//				void				LSetAuthor();


};

#endif
// - End - Preferences Window - Header File ------------------------------------------------------------------
