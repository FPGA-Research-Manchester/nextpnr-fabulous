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

enum GfxTileWireId
{
    TILE_WIRE_GLB2LOCAL_0,
    TILE_WIRE_GLB2LOCAL_1,
    TILE_WIRE_GLB2LOCAL_2,
    TILE_WIRE_GLB2LOCAL_3,

    TILE_WIRE_GLB_NETWK_0,
    TILE_WIRE_GLB_NETWK_1,
    TILE_WIRE_GLB_NETWK_2,
    TILE_WIRE_GLB_NETWK_3,
    TILE_WIRE_GLB_NETWK_4,
    TILE_WIRE_GLB_NETWK_5,
    TILE_WIRE_GLB_NETWK_6,
    TILE_WIRE_GLB_NETWK_7,

    TILE_WIRE_LOCAL_G0_0,
    TILE_WIRE_LOCAL_G0_1,
    TILE_WIRE_LOCAL_G0_2,
    TILE_WIRE_LOCAL_G0_3,
    TILE_WIRE_LOCAL_G0_4,
    TILE_WIRE_LOCAL_G0_5,
    TILE_WIRE_LOCAL_G0_6,
    TILE_WIRE_LOCAL_G0_7,

    TILE_WIRE_LOCAL_G1_0,
    TILE_WIRE_LOCAL_G1_1,
    TILE_WIRE_LOCAL_G1_2,
    TILE_WIRE_LOCAL_G1_3,
    TILE_WIRE_LOCAL_G1_4,
    TILE_WIRE_LOCAL_G1_5,
    TILE_WIRE_LOCAL_G1_6,
    TILE_WIRE_LOCAL_G1_7,

    TILE_WIRE_LOCAL_G2_0,
    TILE_WIRE_LOCAL_G2_1,
    TILE_WIRE_LOCAL_G2_2,
    TILE_WIRE_LOCAL_G2_3,
    TILE_WIRE_LOCAL_G2_4,
    TILE_WIRE_LOCAL_G2_5,
    TILE_WIRE_LOCAL_G2_6,
    TILE_WIRE_LOCAL_G2_7,

    TILE_WIRE_LOCAL_G3_0,
    TILE_WIRE_LOCAL_G3_1,
    TILE_WIRE_LOCAL_G3_2,
    TILE_WIRE_LOCAL_G3_3,
    TILE_WIRE_LOCAL_G3_4,
    TILE_WIRE_LOCAL_G3_5,
    TILE_WIRE_LOCAL_G3_6,
    TILE_WIRE_LOCAL_G3_7,

    TILE_WIRE_LUTFF_0_IN_0,
    TILE_WIRE_LUTFF_0_IN_1,
    TILE_WIRE_LUTFF_0_IN_2,
    TILE_WIRE_LUTFF_0_IN_3,

    TILE_WIRE_LUTFF_1_IN_0,
    TILE_WIRE_LUTFF_1_IN_1,
    TILE_WIRE_LUTFF_1_IN_2,
    TILE_WIRE_LUTFF_1_IN_3,

    TILE_WIRE_LUTFF_2_IN_0,
    TILE_WIRE_LUTFF_2_IN_1,
    TILE_WIRE_LUTFF_2_IN_2,
    TILE_WIRE_LUTFF_2_IN_3,

    TILE_WIRE_LUTFF_3_IN_0,
    TILE_WIRE_LUTFF_3_IN_1,
    TILE_WIRE_LUTFF_3_IN_2,
    TILE_WIRE_LUTFF_3_IN_3,

    TILE_WIRE_LUTFF_4_IN_0,
    TILE_WIRE_LUTFF_4_IN_1,
    TILE_WIRE_LUTFF_4_IN_2,
    TILE_WIRE_LUTFF_4_IN_3,

    TILE_WIRE_LUTFF_5_IN_0,
    TILE_WIRE_LUTFF_5_IN_1,
    TILE_WIRE_LUTFF_5_IN_2,
    TILE_WIRE_LUTFF_5_IN_3,

    TILE_WIRE_LUTFF_6_IN_0,
    TILE_WIRE_LUTFF_6_IN_1,
    TILE_WIRE_LUTFF_6_IN_2,
    TILE_WIRE_LUTFF_6_IN_3,

    TILE_WIRE_LUTFF_7_IN_0,
    TILE_WIRE_LUTFF_7_IN_1,
    TILE_WIRE_LUTFF_7_IN_2,
    TILE_WIRE_LUTFF_7_IN_3,

    TILE_WIRE_LUTFF_0_IN_0_LUT,
    TILE_WIRE_LUTFF_0_IN_1_LUT,
    TILE_WIRE_LUTFF_0_IN_2_LUT,
    TILE_WIRE_LUTFF_0_IN_3_LUT,

    TILE_WIRE_LUTFF_1_IN_0_LUT,
    TILE_WIRE_LUTFF_1_IN_1_LUT,
    TILE_WIRE_LUTFF_1_IN_2_LUT,
    TILE_WIRE_LUTFF_1_IN_3_LUT,

    TILE_WIRE_LUTFF_2_IN_0_LUT,
    TILE_WIRE_LUTFF_2_IN_1_LUT,
    TILE_WIRE_LUTFF_2_IN_2_LUT,
    TILE_WIRE_LUTFF_2_IN_3_LUT,

    TILE_WIRE_LUTFF_3_IN_0_LUT,
    TILE_WIRE_LUTFF_3_IN_1_LUT,
    TILE_WIRE_LUTFF_3_IN_2_LUT,
    TILE_WIRE_LUTFF_3_IN_3_LUT,

    TILE_WIRE_LUTFF_4_IN_0_LUT,
    TILE_WIRE_LUTFF_4_IN_1_LUT,
    TILE_WIRE_LUTFF_4_IN_2_LUT,
    TILE_WIRE_LUTFF_4_IN_3_LUT,

    TILE_WIRE_LUTFF_5_IN_0_LUT,
    TILE_WIRE_LUTFF_5_IN_1_LUT,
    TILE_WIRE_LUTFF_5_IN_2_LUT,
    TILE_WIRE_LUTFF_5_IN_3_LUT,

    TILE_WIRE_LUTFF_6_IN_0_LUT,
    TILE_WIRE_LUTFF_6_IN_1_LUT,
    TILE_WIRE_LUTFF_6_IN_2_LUT,
    TILE_WIRE_LUTFF_6_IN_3_LUT,

    TILE_WIRE_LUTFF_7_IN_0_LUT,
    TILE_WIRE_LUTFF_7_IN_1_LUT,
    TILE_WIRE_LUTFF_7_IN_2_LUT,
    TILE_WIRE_LUTFF_7_IN_3_LUT,

    TILE_WIRE_LUTFF_0_LOUT,
    TILE_WIRE_LUTFF_1_LOUT,
    TILE_WIRE_LUTFF_2_LOUT,
    TILE_WIRE_LUTFF_3_LOUT,
    TILE_WIRE_LUTFF_4_LOUT,
    TILE_WIRE_LUTFF_5_LOUT,
    TILE_WIRE_LUTFF_6_LOUT,

    TILE_WIRE_LUTFF_0_OUT,
    TILE_WIRE_LUTFF_1_OUT,
    TILE_WIRE_LUTFF_2_OUT,
    TILE_WIRE_LUTFF_3_OUT,
    TILE_WIRE_LUTFF_4_OUT,
    TILE_WIRE_LUTFF_5_OUT,
    TILE_WIRE_LUTFF_6_OUT,
    TILE_WIRE_LUTFF_7_OUT,

    TILE_WIRE_LUTFF_0_COUT,
    TILE_WIRE_LUTFF_1_COUT,
    TILE_WIRE_LUTFF_2_COUT,
    TILE_WIRE_LUTFF_3_COUT,
    TILE_WIRE_LUTFF_4_COUT,
    TILE_WIRE_LUTFF_5_COUT,
    TILE_WIRE_LUTFF_6_COUT,
    TILE_WIRE_LUTFF_7_COUT,

    TILE_WIRE_LUTFF_GLOBAL_CEN,
    TILE_WIRE_LUTFF_GLOBAL_CLK,
    TILE_WIRE_LUTFF_GLOBAL_S_R,

    TILE_WIRE_CARRY_IN,
    TILE_WIRE_CARRY_IN_MUX,

    TILE_WIRE_NEIGH_OP_BNL_0,
    TILE_WIRE_NEIGH_OP_BNL_1,
    TILE_WIRE_NEIGH_OP_BNL_2,
    TILE_WIRE_NEIGH_OP_BNL_3,
    TILE_WIRE_NEIGH_OP_BNL_4,
    TILE_WIRE_NEIGH_OP_BNL_5,
    TILE_WIRE_NEIGH_OP_BNL_6,
    TILE_WIRE_NEIGH_OP_BNL_7,

    TILE_WIRE_NEIGH_OP_BNR_0,
    TILE_WIRE_NEIGH_OP_BNR_1,
    TILE_WIRE_NEIGH_OP_BNR_2,
    TILE_WIRE_NEIGH_OP_BNR_3,
    TILE_WIRE_NEIGH_OP_BNR_4,
    TILE_WIRE_NEIGH_OP_BNR_5,
    TILE_WIRE_NEIGH_OP_BNR_6,
    TILE_WIRE_NEIGH_OP_BNR_7,

    TILE_WIRE_NEIGH_OP_BOT_0,
    TILE_WIRE_NEIGH_OP_BOT_1,
    TILE_WIRE_NEIGH_OP_BOT_2,
    TILE_WIRE_NEIGH_OP_BOT_3,
    TILE_WIRE_NEIGH_OP_BOT_4,
    TILE_WIRE_NEIGH_OP_BOT_5,
    TILE_WIRE_NEIGH_OP_BOT_6,
    TILE_WIRE_NEIGH_OP_BOT_7,

    TILE_WIRE_NEIGH_OP_LFT_0,
    TILE_WIRE_NEIGH_OP_LFT_1,
    TILE_WIRE_NEIGH_OP_LFT_2,
    TILE_WIRE_NEIGH_OP_LFT_3,
    TILE_WIRE_NEIGH_OP_LFT_4,
    TILE_WIRE_NEIGH_OP_LFT_5,
    TILE_WIRE_NEIGH_OP_LFT_6,
    TILE_WIRE_NEIGH_OP_LFT_7,

    TILE_WIRE_NEIGH_OP_RGT_0,
    TILE_WIRE_NEIGH_OP_RGT_1,
    TILE_WIRE_NEIGH_OP_RGT_2,
    TILE_WIRE_NEIGH_OP_RGT_3,
    TILE_WIRE_NEIGH_OP_RGT_4,
    TILE_WIRE_NEIGH_OP_RGT_5,
    TILE_WIRE_NEIGH_OP_RGT_6,
    TILE_WIRE_NEIGH_OP_RGT_7,

    TILE_WIRE_NEIGH_OP_TNL_0,
    TILE_WIRE_NEIGH_OP_TNL_1,
    TILE_WIRE_NEIGH_OP_TNL_2,
    TILE_WIRE_NEIGH_OP_TNL_3,
    TILE_WIRE_NEIGH_OP_TNL_4,
    TILE_WIRE_NEIGH_OP_TNL_5,
    TILE_WIRE_NEIGH_OP_TNL_6,
    TILE_WIRE_NEIGH_OP_TNL_7,

    TILE_WIRE_NEIGH_OP_TNR_0,
    TILE_WIRE_NEIGH_OP_TNR_1,
    TILE_WIRE_NEIGH_OP_TNR_2,
    TILE_WIRE_NEIGH_OP_TNR_3,
    TILE_WIRE_NEIGH_OP_TNR_4,
    TILE_WIRE_NEIGH_OP_TNR_5,
    TILE_WIRE_NEIGH_OP_TNR_6,
    TILE_WIRE_NEIGH_OP_TNR_7,

    TILE_WIRE_NEIGH_OP_TOP_0,
    TILE_WIRE_NEIGH_OP_TOP_1,
    TILE_WIRE_NEIGH_OP_TOP_2,
    TILE_WIRE_NEIGH_OP_TOP_3,
    TILE_WIRE_NEIGH_OP_TOP_4,
    TILE_WIRE_NEIGH_OP_TOP_5,
    TILE_WIRE_NEIGH_OP_TOP_6,
    TILE_WIRE_NEIGH_OP_TOP_7,

    TILE_WIRE_SP4_V_B_0,
    TILE_WIRE_SP4_V_B_1,
    TILE_WIRE_SP4_V_B_2,
    TILE_WIRE_SP4_V_B_3,
    TILE_WIRE_SP4_V_B_4,
    TILE_WIRE_SP4_V_B_5,
    TILE_WIRE_SP4_V_B_6,
    TILE_WIRE_SP4_V_B_7,
    TILE_WIRE_SP4_V_B_8,
    TILE_WIRE_SP4_V_B_9,
    TILE_WIRE_SP4_V_B_10,
    TILE_WIRE_SP4_V_B_11,
    TILE_WIRE_SP4_V_B_12,
    TILE_WIRE_SP4_V_B_13,
    TILE_WIRE_SP4_V_B_14,
    TILE_WIRE_SP4_V_B_15,
    TILE_WIRE_SP4_V_B_16,
    TILE_WIRE_SP4_V_B_17,
    TILE_WIRE_SP4_V_B_18,
    TILE_WIRE_SP4_V_B_19,
    TILE_WIRE_SP4_V_B_20,
    TILE_WIRE_SP4_V_B_21,
    TILE_WIRE_SP4_V_B_22,
    TILE_WIRE_SP4_V_B_23,
    TILE_WIRE_SP4_V_B_24,
    TILE_WIRE_SP4_V_B_25,
    TILE_WIRE_SP4_V_B_26,
    TILE_WIRE_SP4_V_B_27,
    TILE_WIRE_SP4_V_B_28,
    TILE_WIRE_SP4_V_B_29,
    TILE_WIRE_SP4_V_B_30,
    TILE_WIRE_SP4_V_B_31,
    TILE_WIRE_SP4_V_B_32,
    TILE_WIRE_SP4_V_B_33,
    TILE_WIRE_SP4_V_B_34,
    TILE_WIRE_SP4_V_B_35,
    TILE_WIRE_SP4_V_B_36,
    TILE_WIRE_SP4_V_B_37,
    TILE_WIRE_SP4_V_B_38,
    TILE_WIRE_SP4_V_B_39,
    TILE_WIRE_SP4_V_B_40,
    TILE_WIRE_SP4_V_B_41,
    TILE_WIRE_SP4_V_B_42,
    TILE_WIRE_SP4_V_B_43,
    TILE_WIRE_SP4_V_B_44,
    TILE_WIRE_SP4_V_B_45,
    TILE_WIRE_SP4_V_B_46,
    TILE_WIRE_SP4_V_B_47,

    TILE_WIRE_SP4_V_T_36,
    TILE_WIRE_SP4_V_T_37,
    TILE_WIRE_SP4_V_T_38,
    TILE_WIRE_SP4_V_T_39,
    TILE_WIRE_SP4_V_T_40,
    TILE_WIRE_SP4_V_T_41,
    TILE_WIRE_SP4_V_T_42,
    TILE_WIRE_SP4_V_T_43,
    TILE_WIRE_SP4_V_T_44,
    TILE_WIRE_SP4_V_T_45,
    TILE_WIRE_SP4_V_T_46,
    TILE_WIRE_SP4_V_T_47,

    TILE_WIRE_SP4_R_V_B_0,
    TILE_WIRE_SP4_R_V_B_1,
    TILE_WIRE_SP4_R_V_B_2,
    TILE_WIRE_SP4_R_V_B_3,
    TILE_WIRE_SP4_R_V_B_4,
    TILE_WIRE_SP4_R_V_B_5,
    TILE_WIRE_SP4_R_V_B_6,
    TILE_WIRE_SP4_R_V_B_7,
    TILE_WIRE_SP4_R_V_B_8,
    TILE_WIRE_SP4_R_V_B_9,
    TILE_WIRE_SP4_R_V_B_10,
    TILE_WIRE_SP4_R_V_B_11,
    TILE_WIRE_SP4_R_V_B_12,
    TILE_WIRE_SP4_R_V_B_13,
    TILE_WIRE_SP4_R_V_B_14,
    TILE_WIRE_SP4_R_V_B_15,
    TILE_WIRE_SP4_R_V_B_16,
    TILE_WIRE_SP4_R_V_B_17,
    TILE_WIRE_SP4_R_V_B_18,
    TILE_WIRE_SP4_R_V_B_19,
    TILE_WIRE_SP4_R_V_B_20,
    TILE_WIRE_SP4_R_V_B_21,
    TILE_WIRE_SP4_R_V_B_22,
    TILE_WIRE_SP4_R_V_B_23,
    TILE_WIRE_SP4_R_V_B_24,
    TILE_WIRE_SP4_R_V_B_25,
    TILE_WIRE_SP4_R_V_B_26,
    TILE_WIRE_SP4_R_V_B_27,
    TILE_WIRE_SP4_R_V_B_28,
    TILE_WIRE_SP4_R_V_B_29,
    TILE_WIRE_SP4_R_V_B_30,
    TILE_WIRE_SP4_R_V_B_31,
    TILE_WIRE_SP4_R_V_B_32,
    TILE_WIRE_SP4_R_V_B_33,
    TILE_WIRE_SP4_R_V_B_34,
    TILE_WIRE_SP4_R_V_B_35,
    TILE_WIRE_SP4_R_V_B_36,
    TILE_WIRE_SP4_R_V_B_37,
    TILE_WIRE_SP4_R_V_B_38,
    TILE_WIRE_SP4_R_V_B_39,
    TILE_WIRE_SP4_R_V_B_40,
    TILE_WIRE_SP4_R_V_B_41,
    TILE_WIRE_SP4_R_V_B_42,
    TILE_WIRE_SP4_R_V_B_43,
    TILE_WIRE_SP4_R_V_B_44,
    TILE_WIRE_SP4_R_V_B_45,
    TILE_WIRE_SP4_R_V_B_46,
    TILE_WIRE_SP4_R_V_B_47,

    TILE_WIRE_SP4_H_L_36,
    TILE_WIRE_SP4_H_L_37,
    TILE_WIRE_SP4_H_L_38,
    TILE_WIRE_SP4_H_L_39,
    TILE_WIRE_SP4_H_L_40,
    TILE_WIRE_SP4_H_L_41,
    TILE_WIRE_SP4_H_L_42,
    TILE_WIRE_SP4_H_L_43,
    TILE_WIRE_SP4_H_L_44,
    TILE_WIRE_SP4_H_L_45,
    TILE_WIRE_SP4_H_L_46,
    TILE_WIRE_SP4_H_L_47,

    TILE_WIRE_SP4_H_R_0,
    TILE_WIRE_SP4_H_R_1,
    TILE_WIRE_SP4_H_R_2,
    TILE_WIRE_SP4_H_R_3,
    TILE_WIRE_SP4_H_R_4,
    TILE_WIRE_SP4_H_R_5,
    TILE_WIRE_SP4_H_R_6,
    TILE_WIRE_SP4_H_R_7,
    TILE_WIRE_SP4_H_R_8,
    TILE_WIRE_SP4_H_R_9,
    TILE_WIRE_SP4_H_R_10,
    TILE_WIRE_SP4_H_R_11,
    TILE_WIRE_SP4_H_R_12,
    TILE_WIRE_SP4_H_R_13,
    TILE_WIRE_SP4_H_R_14,
    TILE_WIRE_SP4_H_R_15,
    TILE_WIRE_SP4_H_R_16,
    TILE_WIRE_SP4_H_R_17,
    TILE_WIRE_SP4_H_R_18,
    TILE_WIRE_SP4_H_R_19,
    TILE_WIRE_SP4_H_R_20,
    TILE_WIRE_SP4_H_R_21,
    TILE_WIRE_SP4_H_R_22,
    TILE_WIRE_SP4_H_R_23,
    TILE_WIRE_SP4_H_R_24,
    TILE_WIRE_SP4_H_R_25,
    TILE_WIRE_SP4_H_R_26,
    TILE_WIRE_SP4_H_R_27,
    TILE_WIRE_SP4_H_R_28,
    TILE_WIRE_SP4_H_R_29,
    TILE_WIRE_SP4_H_R_30,
    TILE_WIRE_SP4_H_R_31,
    TILE_WIRE_SP4_H_R_32,
    TILE_WIRE_SP4_H_R_33,
    TILE_WIRE_SP4_H_R_34,
    TILE_WIRE_SP4_H_R_35,
    TILE_WIRE_SP4_H_R_36,
    TILE_WIRE_SP4_H_R_37,
    TILE_WIRE_SP4_H_R_38,
    TILE_WIRE_SP4_H_R_39,
    TILE_WIRE_SP4_H_R_40,
    TILE_WIRE_SP4_H_R_41,
    TILE_WIRE_SP4_H_R_42,
    TILE_WIRE_SP4_H_R_43,
    TILE_WIRE_SP4_H_R_44,
    TILE_WIRE_SP4_H_R_45,
    TILE_WIRE_SP4_H_R_46,
    TILE_WIRE_SP4_H_R_47,

    TILE_WIRE_SP12_V_B_0,
    TILE_WIRE_SP12_V_B_1,
    TILE_WIRE_SP12_V_B_2,
    TILE_WIRE_SP12_V_B_3,
    TILE_WIRE_SP12_V_B_4,
    TILE_WIRE_SP12_V_B_5,
    TILE_WIRE_SP12_V_B_6,
    TILE_WIRE_SP12_V_B_7,
    TILE_WIRE_SP12_V_B_8,
    TILE_WIRE_SP12_V_B_9,
    TILE_WIRE_SP12_V_B_10,
    TILE_WIRE_SP12_V_B_11,
    TILE_WIRE_SP12_V_B_12,
    TILE_WIRE_SP12_V_B_13,
    TILE_WIRE_SP12_V_B_14,
    TILE_WIRE_SP12_V_B_15,
    TILE_WIRE_SP12_V_B_16,
    TILE_WIRE_SP12_V_B_17,
    TILE_WIRE_SP12_V_B_18,
    TILE_WIRE_SP12_V_B_19,
    TILE_WIRE_SP12_V_B_20,
    TILE_WIRE_SP12_V_B_21,
    TILE_WIRE_SP12_V_B_22,
    TILE_WIRE_SP12_V_B_23,

    TILE_WIRE_SP12_V_T_22,
    TILE_WIRE_SP12_V_T_23,

    TILE_WIRE_SP12_H_R_0,
    TILE_WIRE_SP12_H_R_1,
    TILE_WIRE_SP12_H_R_2,
    TILE_WIRE_SP12_H_R_3,
    TILE_WIRE_SP12_H_R_4,
    TILE_WIRE_SP12_H_R_5,
    TILE_WIRE_SP12_H_R_6,
    TILE_WIRE_SP12_H_R_7,
    TILE_WIRE_SP12_H_R_8,
    TILE_WIRE_SP12_H_R_9,
    TILE_WIRE_SP12_H_R_10,
    TILE_WIRE_SP12_H_R_11,
    TILE_WIRE_SP12_H_R_12,
    TILE_WIRE_SP12_H_R_13,
    TILE_WIRE_SP12_H_R_14,
    TILE_WIRE_SP12_H_R_15,
    TILE_WIRE_SP12_H_R_16,
    TILE_WIRE_SP12_H_R_17,
    TILE_WIRE_SP12_H_R_18,
    TILE_WIRE_SP12_H_R_19,
    TILE_WIRE_SP12_H_R_20,
    TILE_WIRE_SP12_H_R_21,
    TILE_WIRE_SP12_H_R_22,
    TILE_WIRE_SP12_H_R_23,

    TILE_WIRE_SP12_H_L_22,
    TILE_WIRE_SP12_H_L_23,

    TILE_WIRE_PLLIN,
    TILE_WIRE_PLLOUT_A,
    TILE_WIRE_PLLOUT_B
};

void gfxTileWire(std::vector<GraphicElement> &g, int x, int y, GfxTileWireId id, GraphicElement::style_t style);
void gfxTilePip(std::vector<GraphicElement> &g, int x, int y, GfxTileWireId src, GfxTileWireId dst,
                GraphicElement::style_t style);

NEXTPNR_NAMESPACE_END

#endif // GFX_H
