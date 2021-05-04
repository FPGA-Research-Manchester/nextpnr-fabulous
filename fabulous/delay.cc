/*
 *  nextpnr -- Next Generation Place and Route
 *
 *  Copyright (C) 2018  Clifford Wolf <clifford@symbioticeda.com>
 *  Copyright (C) 2018  Serge Bazanski <q3k@symbioticeda.com>
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

#include "nextpnr.h"
#include "router1.h"

NEXTPNR_NAMESPACE_BEGIN

#define NUM_FUZZ_ROUTES 100000

delay_t Arch::estimateDelay(WireId src, WireId dst) const
{
    const auto &src_tile = chip_info->wire_to_tile[src.index];
    const auto &src_loc = chip_info->tile_to_xy[src_tile];
    const auto &dst_tile = chip_info->wire_to_tile[dst.index];
    const auto &dst_loc = chip_info->tile_to_xy[dst_tile];

    auto abs_delta_x = abs(dst_loc.first - src_loc.first);
    auto abs_delta_y = abs(dst_loc.second - src_loc.second);
    //auto div_LH = std::div(abs_delta_x, 12);
    //auto div_LV = std::div(abs_delta_y, 18);
    //auto div_LVB = std::div(div_LV.rem, 12);
    //auto div_H6 = std::div(div_LH.rem, 6);
    //auto div_V6 = std::div(div_LVB.rem, 6);
    auto div_H4 = std::div(/*div_H6.rem*/abs_delta_x, 4);
    auto div_V4 = std::div(/*div_V6.rem*/abs_delta_y, 4);
    auto div_H2 = std::div(div_H4.rem, 2);
    auto div_V2 = std::div(div_V4.rem, 2);
    auto num_H1 = div_H2.rem;
    auto num_V1 = div_V2.rem;
    return /*div_LH.quot * 360 + div_LVB.quot * 300 + div_LV.quot * 350 +*/
           (/*div_H6.quot +*/ div_H4.quot + /*div_V6.quot +*/ div_V4.quot) * 300 + (div_H2.quot + div_V2.quot) * 250 +
           (num_H1 + num_V1) * 200;
}

delay_t Arch::predictDelay(const NetInfo *net_info, const PortRef &sink) const
{
    const auto &driver = net_info->driver;
    auto driver_loc = getBelLocation(driver.cell->bel);
    auto sink_loc = getBelLocation(sink.cell->bel);
    auto abs_delta_x = abs(driver_loc.x - sink_loc.x);
    auto abs_delta_y = abs(driver_loc.y - sink_loc.y);
    //auto div_LH = std::div(abs_delta_x, 12);
    //auto div_LV = std::div(abs_delta_y, 18);
    //auto div_LVB = std::div(div_LV.rem, 12);
    //auto div_H6 = std::div(div_LH.rem, 6);
    //auto div_V6 = std::div(div_LVB.rem, 6);
    auto div_H4 = std::div(/*div_H6.rem*/abs_delta_x, 4);
    auto div_V4 = std::div(/*div_V6.rem*/abs_delta_y, 4);
    auto div_H2 = std::div(div_H4.rem, 2);
    auto div_V2 = std::div(div_V4.rem, 2);
    auto num_H1 = div_H2.rem;
    auto num_V1 = div_V2.rem;
    return /*div_LH.quot * 360 + div_LVB.quot * 300 + div_LV.quot * 350 +*/
           (/*div_H6.quot +*/ div_H4.quot + /*div_V6.quot +*/ div_V4.quot) * 210 + (div_H2.quot + div_V2.quot) * 170 +
           (num_H1 + num_V1) * 150;
}

NEXTPNR_NAMESPACE_END
