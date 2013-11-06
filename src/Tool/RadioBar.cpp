///////////////////////////////////////////////////////////////
//    Extended BeOS Api © Gergely Rózsahegyi (RoGer) 2002    //
//                       roger@beos.hu                       //
//                                                           //
//                   BRadioBar Source File                   //
//                                                           //
//                  Last Update: 2002.06.29                  //
///////////////////////////////////////////////////////////////

// - Includes
#include <Message.h>
#include "RadioBar.h"

#include <stdlib.h>
#include <stdio.h>

const uint32 B_ORI_T_MSG = 'BOTM';
const uint32 B_ORI_B_MSG = 'BOBM';
const uint32 B_ORI_L_MSG = 'BOLM';
const uint32 B_ORI_R_MSG = 'BORM';
const uint32 B_UPDATE_VIEW_MSG = 'BUVM';
const uint32 B_UPDATE_SIZE_MSG = 'BUSM';
const uint32 B_MOUSE_DOWN_MSG = 'BMDM';
const uint32 B_MOUSE_UP_MSG = 'BMUM';
const uint32 B_ITEM_CLICK_MSG = 'BICM';
const uint32 B_RADIOITEM_CHECKED_MSG = 'BRCM';

// --------------------------------------------------------------------------------- B_Radio_Bar - BRadioBar -
BRadioBar::BRadioBar(const char *name, BRect hmargin, BRect vmargin, float hspace, float vspace, bool autoresize, float hwidth, float vheight):BToolBar(name,hmargin,vmargin,hspace,vspace,autoresize,hwidth,vheight)
{
}


// --------------------------------------------------------------------------- B_Radio_Bar - MessageReceived -
void BRadioBar::AddToolItem(BRadioItem *toolitem)
{
 toolitem->SetNum(BToolBar::AddToolItem(toolitem));
}

// --------------------------------------------------------------------------- B_Radio_Bar - MessageReceived -
void BRadioBar::MessageReceived(BMessage *msg)
{
 switch (msg->what)
 {
  case B_ITEM_CLICK_MSG:
  {
   int32 fNum;
   msg->FindInt32("fNum",&fNum);
   for (uint32 i=0;i<ToolItemCount();i++)
   {
    BMessage msg(B_RADIOITEM_CHECKED_MSG);
    msg.AddBool("Checked",false);
    ToolItem(i)->MessageReceived(&msg);
   }
    BMessage msg(B_RADIOITEM_CHECKED_MSG);
    msg.AddBool("Checked",true);
    ToolItem(fNum)->MessageReceived(&msg);
  }
  break;
  default:
  {
   BToolBar::MessageReceived(msg);
  }
  break;
 }
}

// - End - BRadioBar - Source File ---------------------------------------------------------------------------
