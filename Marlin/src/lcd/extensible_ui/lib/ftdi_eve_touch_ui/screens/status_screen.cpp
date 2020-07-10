/*********************
 * status_screen.cpp *
 *********************/

/****************************************************************************
 *   Written By Mark Pelletier  2017 - Aleph Objects, Inc.                  *
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   To view a copy of the GNU General Public License, go to the following  *
 *   location: <http://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#include "../config.h"

#if ENABLED(TOUCH_UI_FTDI_EVE) && DISABLED(TOUCH_UI_LULZBOT_BIO) && DISABLED(TOUCH_UI_COCOA_PRESS)

#include "screens.h"
#include "screen_data.h"

#include "../archim2-flash/flash_storage.h"

using namespace FTDI;
using namespace Theme;

#ifdef TOUCH_UI_PORTRAIT
  #define GRID_ROWS 8
#else
  #define GRID_ROWS 8
#endif

void StatusScreen::draw_axis_position(draw_mode_t what) {
  CommandProcessor cmd;

  #define GRID_COLS 3

  if (what & BACKGROUND) {
    cmd.tag(6)
    #ifdef TOUCH_UI_PORTRAIT
      .fgcolor(Theme::axis_label)
        .font(Theme::font_large)
                         .button( BTN_POS(1,5), BTN_SIZE(2,1), F(""), OPT_FLAT)
                         .button( BTN_POS(1,6), BTN_SIZE(2,1), F(""), OPT_FLAT)
                         .button( BTN_POS(1,7), BTN_SIZE(2,1), F(""), OPT_FLAT)

        .font(Theme::font_small)
                         .text  ( BTN_POS(1,5), BTN_SIZE(1,1), GET_TEXT_F(MSG_AXIS_X))
                         .text  ( BTN_POS(1,6), BTN_SIZE(1,1), GET_TEXT_F(MSG_AXIS_Y))
                         .text  ( BTN_POS(1,7), BTN_SIZE(1,1), GET_TEXT_F(MSG_AXIS_Z))

        .font(Theme::font_medium)
        .fgcolor(Theme::x_axis) .button( BTN_POS(2,5), BTN_SIZE(2,1), F(""), OPT_FLAT)
        .fgcolor(Theme::y_axis) .button( BTN_POS(2,6), BTN_SIZE(2,1), F(""), OPT_FLAT)
        .fgcolor(Theme::z_axis) .button( BTN_POS(2,7), BTN_SIZE(2,1), F(""), OPT_FLAT);
    #else
      .fgcolor(Theme::axis_label)
        .font(Theme::font_large)
                         .button( BTN_POS(1,5), BTN_SIZE(1,2), F(""),  OPT_FLAT)
                         .button( BTN_POS(2,5), BTN_SIZE(1,2), F(""),  OPT_FLAT)
                         .button( BTN_POS(3,5), BTN_SIZE(1,2), F(""),  OPT_FLAT)

        .font(Theme::font_small)
                         .text  ( BTN_POS(1,5), BTN_SIZE(1,1), GET_TEXT_F(MSG_AXIS_X))
                         .text  ( BTN_POS(2,5), BTN_SIZE(1,1), GET_TEXT_F(MSG_AXIS_Y))
                         .text  ( BTN_POS(3,5), BTN_SIZE(1,1), GET_TEXT_F(MSG_AXIS_Z))
                         .font(Theme::font_medium)

        .fgcolor(Theme::x_axis) .button( BTN_POS(1,6), BTN_SIZE(1,1), F(""), OPT_FLAT)
        .fgcolor(Theme::y_axis) .button( BTN_POS(2,6), BTN_SIZE(1,1), F(""), OPT_FLAT)
        .fgcolor(Theme::z_axis) .button( BTN_POS(3,6), BTN_SIZE(1,1), F(""), OPT_FLAT);
    #endif
  }

  if (what & FOREGROUND) {
    using namespace ExtUI;
    char x_str[15];
    char y_str[15];
    char z_str[15];

    strcpy_P(x_str, PSTR("7"));
    strcpy_P(y_str, PSTR("7"));
    strcpy_P(z_str, PSTR("7"));

    if (isAxisPositionKnown(X))
      format_position(x_str, getAxisPosition_mm(X));
    else
      strcpy_P(x_str, PSTR("?"));

    if (isAxisPositionKnown(Y))
      format_position(y_str, getAxisPosition_mm(Y));
    else
      strcpy_P(y_str, PSTR("?"));

    if (isAxisPositionKnown(Z))
      format_position(z_str, getAxisPosition_mm(Z));
    else
      strcpy_P(z_str, PSTR("?"));
    cmd.tag(6).font(Theme::font_medium)
    #ifdef TOUCH_UI_PORTRAIT
         .text  ( BTN_POS(2,5), BTN_SIZE(2,1), x_str)
         .text  ( BTN_POS(2,6), BTN_SIZE(2,1), y_str)
         .text  ( BTN_POS(2,7), BTN_SIZE(2,1), z_str);
    #else
         .text  ( BTN_POS(1,6), BTN_SIZE(1,1), x_str)
         .text  ( BTN_POS(2,6), BTN_SIZE(1,1), y_str)
         .text  ( BTN_POS(3,6), BTN_SIZE(1,1), z_str);
    #endif
  }

  #undef GRID_COLS
}

#ifdef TOUCH_UI_PORTRAIT
  #define GRID_COLS 8
#else
  #define GRID_COLS 12
#endif

void StatusScreen::draw_temperature(draw_mode_t what) {
  using namespace Theme;

  CommandProcessor cmd;

  if (what & BACKGROUND) {
    cmd.font(Theme::font_small)
    #ifdef TOUCH_UI_PORTRAIT
       .tag(5)
       .fgcolor(temp)      .button( BTN_POS(1,1), BTN_SIZE(4,2), F(""), OPT_FLAT)
                                  .button( BTN_POS(1,1), BTN_SIZE(8,1), F(""), OPT_FLAT)
       .fgcolor(fan_speed) .button( BTN_POS(5,2), BTN_SIZE(4,1), F(""), OPT_FLAT)
       .tag(0)
       .fgcolor(progress)  .button( BTN_POS(1,3), BTN_SIZE(4,1), F(""), OPT_FLAT)
                                  .button( BTN_POS(5,3), BTN_SIZE(4,1), F(""), OPT_FLAT);
    #else
       .tag(5)
       .fgcolor(temp)      .button( BTN_POS(1,1), BTN_SIZE(4,2), F(""), OPT_FLAT)
                                  .button( BTN_POS(1,1), BTN_SIZE(8,1), F(""), OPT_FLAT)
       .fgcolor(fan_speed) .button( BTN_POS(5,2), BTN_SIZE(4,1), F(""), OPT_FLAT)
       .tag(0)
       .fgcolor(progress)  .button( BTN_POS(9,1), BTN_SIZE(4,1), F(""), OPT_FLAT)
                                  .button( BTN_POS(9,2), BTN_SIZE(4,1), F(""), OPT_FLAT);
    #endif

    // Draw Extruder Bitmap on Extruder Temperature Button

    cmd.tag(5)
       .cmd(BITMAP_SOURCE(Extruder_Icon_Info))
       .cmd(BITMAP_LAYOUT(Extruder_Icon_Info))
       .cmd(BITMAP_SIZE  (Extruder_Icon_Info))
       .icon (BTN_POS(1,1), BTN_SIZE(1,1),  Extruder_Icon_Info, icon_scale)
       .icon (BTN_POS(5,1), BTN_SIZE(1,1),  Extruder_Icon_Info, icon_scale);

    // Draw Bed Heat Bitmap on Bed Heat Button
    cmd.cmd(BITMAP_SOURCE(Bed_Heat_Icon_Info))
       .cmd(BITMAP_LAYOUT(Bed_Heat_Icon_Info))
       .cmd(BITMAP_SIZE  (Bed_Heat_Icon_Info))
       .icon (BTN_POS(1,2), BTN_SIZE(1,1), Bed_Heat_Icon_Info, icon_scale);

    // Draw Fan Percent Bitmap on Bed Heat Button

    cmd.cmd(BITMAP_SOURCE(Fan_Icon_Info))
       .cmd(BITMAP_LAYOUT(Fan_Icon_Info))
       .cmd(BITMAP_SIZE  (Fan_Icon_Info))
       .icon  (BTN_POS(5,2), BTN_SIZE(1,1), Fan_Icon_Info, icon_scale);

    #ifdef TOUCH_UI_USE_UTF8
      load_utf8_bitmaps(cmd); // Restore font bitmap handles
    #endif
  }

  if (what & FOREGROUND) {
    using namespace ExtUI;
    char e0_str[20];
    char e1_str[20];
    char bed_str[20];
    char fan_str[20];

    sprintf_P(
      fan_str,
      PSTR("%-3d %%"),
      int8_t(getActualFan_percent(FAN0))
    );

    if (isHeaterIdle(BED))
      format_temp_and_idle(bed_str, getActualTemp_celsius(BED));
    else
      format_temp_and_temp(bed_str, getActualTemp_celsius(BED), getTargetTemp_celsius(BED));

    if (isHeaterIdle(H0))
      format_temp_and_idle(e0_str, getActualTemp_celsius(H0));
    else
      format_temp_and_temp(e0_str, getActualTemp_celsius(H0), getTargetTemp_celsius(H0));

    #if EXTRUDERS == 2
      if (isHeaterIdle(H1))
        format_temp_and_idle(e1_str, getActualTemp_celsius(H1));
      else
        format_temp_and_temp(e1_str, getActualTemp_celsius(H1), getTargetTemp_celsius(H1));
    #else
      strcpy_P(
        e1_str,
        PSTR("-")
      );
    #endif
    cmd.tag(5)
       .font(font_medium)
       .text(BTN_POS(2,1), BTN_SIZE(3,1), e0_str)
       .text(BTN_POS(6,1), BTN_SIZE(3,1), e1_str)
       .text(BTN_POS(2,2), BTN_SIZE(3,1), bed_str)
       .text(BTN_POS(6,2), BTN_SIZE(3,1), fan_str);
  }
}

void StatusScreen::draw_progress(draw_mode_t what) {
  using namespace ExtUI;
  using namespace Theme;

  CommandProcessor cmd;

  if (what & BACKGROUND) {
    cmd.tag(0).font(font_medium)
    #ifdef TOUCH_UI_PORTRAIT
       .fgcolor(progress) .button(BTN_POS(1,3), BTN_SIZE(4,1), F(""), OPT_FLAT)
                                 .button(BTN_POS(5,3), BTN_SIZE(4,1), F(""), OPT_FLAT);
    #else
       .fgcolor(progress) .button(BTN_POS(9,1), BTN_SIZE(4,1), F(""), OPT_FLAT)
                                 .button(BTN_POS(9,2), BTN_SIZE(4,1), F(""), OPT_FLAT);
    #endif
  }

  if (what & FOREGROUND) {
    const uint32_t elapsed = getProgress_seconds_elapsed();
    const uint8_t hrs = elapsed/3600;
    const uint8_t min = (elapsed/60)%60;

    char time_str[10];
    char progress_str[10];

    sprintf_P(time_str,     PSTR(" %02d : %02d"), hrs, min);
    sprintf_P(progress_str, PSTR("%-3d %%"),      getProgress_percent() );

    cmd.font(font_medium)
    #ifdef TOUCH_UI_PORTRAIT
       .tag(0).text(BTN_POS(1,3), BTN_SIZE(4,1), time_str)
              .text(BTN_POS(5,3), BTN_SIZE(4,1), progress_str);
    #else
       .tag(0).text(BTN_POS(9,1), BTN_SIZE(4,1), time_str)
              .text(BTN_POS(9,2), BTN_SIZE(4,1), progress_str);
    #endif
  }
}

#undef GRID_COLS


void StatusScreen::draw_interaction_buttons(draw_mode_t what) {
  #define GRID_COLS 4
  if (what & FOREGROUND) {
    using namespace ExtUI;

    const bool has_media = isMediaInserted() && !isPrintingFromMedia();

    CommandProcessor cmd;
    cmd.colors(normal_btn)
       .font(Theme::font_medium)
       .enabled(has_media)
       .colors(has_media ? action_btn : normal_btn)
       .tag(3).button(
          #ifdef TOUCH_UI_PORTRAIT
            BTN_POS(1,8), BTN_SIZE(2,1),
          #else
            BTN_POS(1,7), BTN_SIZE(2,2),
          #endif
          isPrintingFromMedia() ? GET_TEXT_F(MSG_PRINTING) : GET_TEXT_F(MSG_BUTTON_MEDIA)
        ).colors(!has_media ? action_btn : normal_btn)
      #ifdef TOUCH_UI_PORTRAIT
       .tag(4).button( BTN_POS(3,8), BTN_SIZE(2,1), GET_TEXT_F(MSG_BUTTON_MENU));
      #else
       .tag(4).button( BTN_POS(3,7), BTN_SIZE(2,2), GET_TEXT_F(MSG_BUTTON_MENU));
    #endif
  }
  #undef  GRID_COLS
}

void StatusScreen::draw_status_message(draw_mode_t what, const char* message) {
  #define GRID_COLS 1
  if (what & BACKGROUND) {
    CommandProcessor cmd;
    cmd.fgcolor(Theme::status_msg)
       .tag(0)
    #ifdef TOUCH_UI_PORTRAIT
       .button( BTN_POS(1,4), BTN_SIZE(1,1), F(""), OPT_FLAT);
    #else
       .button( BTN_POS(1,3), BTN_SIZE(1,2), F(""), OPT_FLAT);
    #endif

    draw_text_box(cmd,
    #ifdef TOUCH_UI_PORTRAIT
      BTN_POS(1,4), BTN_SIZE(1,1),
    #else
      BTN_POS(1,3), BTN_SIZE(1,2),
    #endif
      message, OPT_CENTER, font_large);
  }
  #undef  GRID_COLS
}

void StatusScreen::setStatusMessage(progmem_str message) {
  char buff[strlen_P((const char * const)message)+1];
  strcpy_P(buff, (const char * const) message);
  setStatusMessage((const char *) buff);
}

void StatusScreen::setStatusMessage(const char* message) {
  CommandProcessor cmd;
  cmd.cmd(CMD_DLSTART)
     .cmd(CLEAR_COLOR_RGB(Theme::bg_color))
     .cmd(CLEAR(true,true,true));

  draw_temperature(BACKGROUND);
  draw_progress(BACKGROUND);
  draw_axis_position(BACKGROUND);
  draw_status_message(BACKGROUND, message);
  draw_interaction_buttons(BACKGROUND);

  storeBackground();

  #if ENABLED(TOUCH_UI_DEBUG)
    SERIAL_ECHO_START();
    SERIAL_ECHOLNPAIR("New status message: ", message);
  #endif

  if (AT_SCREEN(StatusScreen)) {
    current_screen.onRefresh();
  }
}

void StatusScreen::loadBitmaps() {
  /*
  // Load the bitmaps for the status screen
  using namespace Theme;
  constexpr uint32_t base = ftdi_memory_map::RAM_G;
  CLCD::mem_write_pgm(base + TD_Icon_Info.RAMG_offset,       TD_Icon,       sizeof(TD_Icon));
  CLCD::mem_write_pgm(base + Extruder_Icon_Info.RAMG_offset, Extruder_Icon, sizeof(Extruder_Icon));
  CLCD::mem_write_pgm(base + Bed_Heat_Icon_Info.RAMG_offset, Bed_Heat_Icon, sizeof(Bed_Heat_Icon));
  CLCD::mem_write_pgm(base + Fan_Icon_Info.RAMG_offset,      Fan_Icon,      sizeof(Fan_Icon));

  // Load fonts for internationalization
  #ifdef TOUCH_UI_USE_UTF8
    load_utf8_data(base + UTF8_FONT_OFFSET);
  #endif
  */
}

void StatusScreen::onStartup() {
  UIFlashStorage::initialize();
}

void StatusScreen::onRedraw(draw_mode_t what) {
  /*
  if (what & FOREGROUND) {
    draw_temperature(FOREGROUND);
    draw_progress(FOREGROUND);
    draw_axis_position(FOREGROUND);
    draw_interaction_buttons(FOREGROUND);
  }
  */
}

void StatusScreen::onEntry() {
  onRefresh();
}

void StatusScreen::onIdle() {
  if (refresh_timer.elapsed(STATUS_UPDATE_INTERVAL)) {
    onRefresh();
    refresh_timer.start();
  }
  BaseScreen::onIdle();
}

bool StatusScreen::onTouchEnd(uint8_t tag) {
  /*
  using namespace ExtUI;

  switch (tag) {
    case 3: GOTO_SCREEN(FilesScreen); break;
    case 4:
      if (isPrinting()) {
        GOTO_SCREEN(TuneMenu);
      } else {
        GOTO_SCREEN(MainMenu);
      }
      break;
    case 5:  GOTO_SCREEN(TemperatureScreen); break;
    case 6:
      if (!isPrinting()) {
        GOTO_SCREEN(MoveAxisScreen);
      }
      break;
    default:
      return true;
  }
  // If a passcode is enabled, the LockScreen will prevent the
  // user from proceeding.
  LockScreen::check_passcode();
  return true;
  */
}

#endif // TOUCH_UI_FTDI_EVE
