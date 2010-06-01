/*
 * This file is part of libbluray
 * Copyright (C) 2009-2010  cRTrn13
 * Copyright (C) 2009-2010  John Stebbins
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * In addition, as a special exception, the copyright holders of libbluray
 * gives permission to link the code of its release of libbluray with the
 * OpenSSL project's "OpenSSL" library (or with modified versions of it
 * that use the same license as the "OpenSSL" library), and distribute
 * the linked executables.  You must obey the GNU General Public License
 * in all respects for all of the code used other than "OpenSSL".  If you
 * modify this file, you may extend this exception to your version of the
 * file, but you are not obligated to do so.  If you do not wish to do
 * so, delete this exception statement from your version.
 */

#ifndef BLURAY_H_
#define BLURAY_H_

#include "file/file.h"
#include "bdnav/navigation.h"

struct indx_root_s;

typedef int (*fptr_int)();
typedef int32_t (*fptr_int32)();
typedef void* (*fptr_p_void)();

typedef struct bluray BLURAY;
struct bluray {
    char *device_path;
    NAV_TITLE_LIST *title_list;
    NAV_TITLE *title;
    uint64_t s_size;
    uint64_t s_pos;
    NAV_CLIP *clip;
    FILE_H *fp;
    uint64_t clip_size;
    uint64_t clip_block_pos;
    uint64_t clip_pos;
    void *aacs, *bdplus;
    fptr_int32 bdplus_seek; // frequently called
    fptr_int32 bdplus_fixup; // frequently called
    void *h_libaacs, *h_libbdplus, *h_libbdnav;
    fptr_int libaacs_decrypt_unit;
    uint8_t int_buf[6144];
    uint16_t int_buf_off;
    int      seamless_angle_change;
    uint32_t angle_change_pkt;
    uint32_t angle_change_time;
    int      request_angle;
    int      angle;

    struct indx_root_s *index;
};

typedef struct bd_chapter {
    uint32_t    idx;
    uint64_t    start;
    uint64_t    duration;
    uint64_t    offset;
} BD_TITLE_CHAPTER;

typedef struct bd_title_info {
    uint32_t    idx;
    uint64_t    duration;
    uint32_t    angle_count;
    uint32_t    chapter_count;
    BD_TITLE_CHAPTER *chapters;
} BD_TITLE_INFO;

uint32_t bd_get_titles(BLURAY *bd, uint8_t flags);
BD_TITLE_INFO* bd_get_title_info(BLURAY *bd, uint32_t title_idx);
void bd_free_title_info(BD_TITLE_INFO *title_info);

BLURAY *bd_open(const char* device_path, const char* keyfile_path); // Init libbluray objs
void bd_close(BLURAY *bd);                                          // Free libbluray objs

int64_t bd_seek(BLURAY *bd, uint64_t pos);              // Seek to pos in currently selected title file
int64_t bd_seek_time(BLURAY *bd, uint64_t tick); // Seek to a specific time in 90Khz ticks
int bd_read(BLURAY *bd, unsigned char *buf, int len);   // Read from currently selected title file, decrypt if possible

int bd_select_title(BLURAY *bd, uint32_t title);    // Select the title from the list created by bd_get_titles()
int bd_select_angle(BLURAY *bd, int angle);         // Set the angle to play
void bd_seamless_angle_change(BLURAY *bd, int angle); // Initiate seamless angle change
uint64_t bd_get_title_size(BLURAY *bd);             // Returns file size in bytes of currently selected title, 0 in no title selected

uint64_t bd_tell(BLURAY *bd);       // Return current pos

#endif /* BLURAY_H_ */
