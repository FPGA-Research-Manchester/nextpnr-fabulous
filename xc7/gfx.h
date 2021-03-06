/*
 *  nextpnr -- Next Generation Place and Route
 *
 *  Copyright (C) 2018  Clifford Wolf <clifford@symbioticeda.com>
 *
 *  Permission to use, copy, modify, and/or distribute this software for any
 *  purpose with or without fee is hereby granted, provided that the above
 *  copyright notice and this permission notice appear in all copies.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#ifndef GFX_H
#define GFX_H

#include "nextpnr.h"

NEXTPNR_NAMESPACE_BEGIN

const float main_swbox_x1 = 0.35;
const float main_swbox_x2 = 0.60;
const float main_swbox_y1 = 0.05;
const float main_swbox_y2 = 0.73;

const float local_swbox_x1 = 0.63;
const float local_swbox_x2 = 0.73;
const float local_swbox_y1 = 0.05;
const float local_swbox_y2 = 0.55;

const float lut_swbox_x1 = 0.76;
const float lut_swbox_x2 = 0.80;

const float logic_cell_x1 = 0.83;
const float logic_cell_x2 = 0.95;
const float logic_cell_y1 = 0.05;
const float logic_cell_y2 = 0.10;
const float logic_cell_pitch = 0.0625;

NEXTPNR_NAMESPACE_END

#endif // GFX_H
