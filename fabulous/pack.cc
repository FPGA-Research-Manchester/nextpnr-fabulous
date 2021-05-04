/*
 *  nextpnr -- Next Generation Place and Route
 *
 *  Copyright (C) 2018  Clifford Wolf <clifford@symbioticeda.com>
 *  Copyright (C) 2018  David Shah <david@symbioticeda.com>
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

#include <algorithm>
#include <iterator>
#include <unordered_set>
#include "cells.h"
#include "chains.h"
#include "design_utils.h"
#include "log.h"
#include "util.h"
#include <iostream>

NEXTPNR_NAMESPACE_BEGIN

// Pack LUTs and LUT-FF pairss
static void pack_lut_lutffs(Context *ctx)
{
    log_info("Packing LUT-FFs..\n");

    std::unordered_set<IdString> packed_cells;
    std::vector<std::unique_ptr<CellInfo>> new_cells;
    for (auto cell : sorted(ctx->cells)) {
        CellInfo *ci = cell.second;
        if (ctx->verbose)
            log_info("cell '%s' is of type '%s'\n", ci->name.c_str(ctx), ci->type.c_str(ctx));
        
        // if (is_lut(ctx, ci)) {
        //     std::unique_ptr<CellInfo> packed = create_diko_cell(ctx, ctx->id("diko_LC"), ci->name.str(ctx) + "_LC");
        //     std::copy(ci->attrs.begin(), ci->attrs.end(), std::inserter(packed->attrs, packed->attrs.begin()));
        //     packed_cells.insert(ci->name);
        //     if (ctx->verbose)
        //         log_info("packed cell %s into %s\n", ci->name.c_str(ctx), packed->name.c_str(ctx));
        //     // See if we can pack into a DFF
        //     bool packed_dff = false;
        //     if (get_net_or_empty(ci, ctx->id("O"))) {
        //         NetInfo *o = ci->ports.at(ctx->id("O")).net;
        //         CellInfo *dff = net_only_drives(ctx, o, is_ff, ctx->id("D"), true);
        //         auto lut_bel = ci->attrs.find(ctx->id("BEL"));
        //         if (dff) {
        //             if (ctx->verbose)
        //                 log_info("found attached dff %s\n", dff->name.c_str(ctx));
        //             auto dff_bel = dff->attrs.find(ctx->id("BEL"));
        //             if (lut_bel != ci->attrs.end() && dff_bel != dff->attrs.end() && lut_bel->second != dff_bel->second) {
        //                 // Locations don't match, can't pack
        //             } else {
        //                 lut_to_lc(ctx, ci, packed.get(), false);
        //                 dff_to_lc(ctx, dff, packed.get(), false);
        //                 packed->params[ctx->id("FF")] = 1;
        //                 log(o->name.c_str(ctx));
        //                 ctx->nets.erase(o->name);
        //                 //replace_port(ci, ctx->id("O"), dff, ctx->id("O"));
        //                 if (dff_bel != dff->attrs.end())
        //                     packed->attrs[ctx->id("BEL")] = dff_bel->second;
        //                 packed_cells.insert(dff->name);
        //                 if (ctx->verbose)
        //                     log_info("packed cell %s into %s\n", dff->name.c_str(ctx), packed->name.c_str(ctx));
        //                 packed_dff = true;
        //             }
        //         }
        //     }
        //     if (!packed_dff) {
        //         lut_to_lc(ctx, ci, packed.get(), true);
        //     }
        //     new_cells.push_back(std::move(packed));
        // }



        if (is_lut(ctx, ci)) {
            std::unique_ptr<CellInfo> packed = create_diko_cell(ctx, ctx->id("diko_LC"), ci->name.str(ctx) + "_LC");
            std::copy(ci->attrs.begin(), ci->attrs.end(), std::inserter(packed->attrs, packed->attrs.begin()));
            packed_cells.insert(ci->name);
            if (ctx->verbose)
                log_info("packed cell %s into %s\n", ci->name.c_str(ctx), packed->name.c_str(ctx));
            // See if we can pack into a DFF
            bool has_dff = false;

            if (get_net_or_empty(ci, ctx->id("O"))) {
                NetInfo *o = ci->ports.at(ctx->id("O")).net;
                CellInfo *dff = net_only_drives(ctx, o, is_ff, ctx->id("D"), true);
                //std::cout << "DFF:" << dff << std::endl;
                //auto lut_bel = ci->attrs.find(ctx->id("BEL"));
                if (dff) {
                    if (ctx->verbose)
                        log_info("found attached dff %s\n", dff->name.c_str(ctx));
                    //disconnect_port(ctx, ci, ctx->id("O"));
                    replace_port(ci, ctx->id("O"), dff, ctx->id("O"));

                    ctx->nets.erase(o->name);

                    //Rearrange nets

                    for (auto &n: ctx->nets){
                        auto ni = n.second.get();
                        if (ni->driver.cell == dff){
                            ni->driver.cell = ci;
                            ni->driver.cell->ports.at(ni->driver.port).net = ni;
                            //n.first = packed->name;
                        }
                    }

                    packed_cells.insert(dff->name);
                    has_dff = true;
                }
            }
                lut_to_lc(ctx, ci, packed.get(), true);
                if (has_dff){
                    packed->params[ctx->id("FF")] = "1";
                    packed->params[ctx->id("DFF_ENABLE")] = "1";
                }


            // log("IO");
            // replace_port(ci, ctx->id("I0"), packed.get(), ctx->id("I0"));
            //             log("I1");

            // replace_port(ci, ctx->id("I1"), packed.get(), ctx->id("I1"));
            //             log("I2");

            // replace_port(ci, ctx->id("I2"), packed.get(), ctx->id("I2"));
            //             log("I3");

            // replace_port(ci, ctx->id("I3"), packed.get(), ctx->id("I3"));
            //             log("O");


            // replace_port(ci, ctx->id("O"), packed.get(), ctx->id("O"));
            new_cells.push_back(std::move(packed));


        }
    }
    for (auto pcell : packed_cells) {
        ctx->cells.erase(pcell);
    }
    for (auto &ncell : new_cells) {
        ctx->cells[ncell->name] = std::move(ncell);
    }
}

//Diko specific mux pack
static void pack_muxes(Context *ctx)
{
    log_info("Packing muxs..\n");

    std::unordered_set<IdString> packed_cells;
    std::vector<std::unique_ptr<CellInfo>> new_cells;
    for (auto cell : sorted(ctx->cells)) {
        CellInfo *ci = cell.second;
//        if (ctx->verbose)
//            log_info("cell '%s' is of type '%s'\n", ci->name.c_str(ctx), ci->type.c_str(ctx));
        if (ci->type == ctx->id("MUXF7") || ci->type == ctx->id("MUXF8") ) {
            std::unique_ptr<CellInfo> packed = create_diko_cell(ctx, ctx->id("diko_mux"), ci->name.str(ctx) + "_mux");
            std::copy(ci->attrs.begin(), ci->attrs.end(), std::inserter(packed->attrs, packed->attrs.begin()));
            packed_cells.insert(ci->name);
            if (ctx->verbose)
                log_info("packed cell %s into %s\n", ci->name.c_str(ctx), packed->name.c_str(ctx));
            new_cells.push_back(std::move(packed));
        }
    }
    for (auto pcell : packed_cells) {
        ctx->cells.erase(pcell);
    }
    for (auto &ncell : new_cells) {
        ctx->cells[ncell->name] = std::move(ncell);
    }
}

//Diko specific DSP pack
static void pack_DSP(Context *ctx)
{
    log_info("Packing DSP blocks..\n");

    std::unordered_set<IdString> packed_cells;
    std::vector<std::unique_ptr<CellInfo>> new_cells;
    for (auto cell : sorted(ctx->cells)) {

        CellInfo *ci = cell.second;
//        if (ctx->verbose)
//            log_info("cell '%s' is of type '%s'\n", ci->name.c_str(ctx), ci->type.c_str(ctx));
        if (ci->type == ctx->id("MULADD")) {

            std::unique_ptr<CellInfo> packed = create_diko_cell(ctx, ctx->id("diko_MULADD"), ci->name.str(ctx) + "_mul");
            std::copy(ci->attrs.begin(), ci->attrs.end(), std::inserter(packed->attrs, packed->attrs.begin()));
            packed_cells.insert(ci->name);

            // for (auto &port : ci->ports) {
            //      auto portName = port.first;

            //      replace_port(ci, portName, packed.get(), portName);
            // }

            replace_port(ci, ctx->id("Q[0]"), packed.get(), ctx->id("Q[0]"));
            replace_port(ci, ctx->id("Q[1]"), packed.get(), ctx->id("Q[1]"));
            replace_port(ci, ctx->id("Q[2]"), packed.get(), ctx->id("Q[2]"));
            replace_port(ci, ctx->id("Q[3]"), packed.get(), ctx->id("Q[3]"));
            replace_port(ci, ctx->id("Q[4]"), packed.get(), ctx->id("Q[4]"));
            replace_port(ci, ctx->id("Q[5]"), packed.get(), ctx->id("Q[5]"));
            replace_port(ci, ctx->id("Q[6]"), packed.get(), ctx->id("Q[6]"));
            replace_port(ci, ctx->id("Q[7]"), packed.get(), ctx->id("Q[7]"));
            replace_port(ci, ctx->id("Q[8]"), packed.get(), ctx->id("Q[8]"));
            replace_port(ci, ctx->id("Q[9]"), packed.get(), ctx->id("Q[9]"));
            replace_port(ci, ctx->id("Q[10]"), packed.get(), ctx->id("Q[10]"));
            replace_port(ci, ctx->id("Q[11]"), packed.get(), ctx->id("Q[11]"));
            replace_port(ci, ctx->id("Q[12]"), packed.get(), ctx->id("Q[12]"));
            replace_port(ci, ctx->id("Q[13]"), packed.get(), ctx->id("Q[13]"));
            replace_port(ci, ctx->id("Q[14]"), packed.get(), ctx->id("Q[14]"));
            replace_port(ci, ctx->id("Q[15]"), packed.get(), ctx->id("Q[15]"));
            replace_port(ci, ctx->id("Q[16]"), packed.get(), ctx->id("Q[16]"));
            replace_port(ci, ctx->id("Q[17]"), packed.get(), ctx->id("Q[17]"));
            replace_port(ci, ctx->id("Q[18]"), packed.get(), ctx->id("Q[18]"));
            replace_port(ci, ctx->id("Q[19]"), packed.get(), ctx->id("Q[19]"));

            replace_port(ci, ctx->id("C[0]"), packed.get(), ctx->id("C[0]"));
            replace_port(ci, ctx->id("C[1]"), packed.get(), ctx->id("C[1]"));
            replace_port(ci, ctx->id("C[2]"), packed.get(), ctx->id("C[2]"));
            replace_port(ci, ctx->id("C[3]"), packed.get(), ctx->id("C[3]"));
            replace_port(ci, ctx->id("C[4]"), packed.get(), ctx->id("C[4]"));
            replace_port(ci, ctx->id("C[5]"), packed.get(), ctx->id("C[5]"));
            replace_port(ci, ctx->id("C[6]"), packed.get(), ctx->id("C[6]"));
            replace_port(ci, ctx->id("C[7]"), packed.get(), ctx->id("C[7]"));
            replace_port(ci, ctx->id("C[8]"), packed.get(), ctx->id("C[8]"));
            replace_port(ci, ctx->id("C[9]"), packed.get(), ctx->id("C[9]"));
            replace_port(ci, ctx->id("C[10]"), packed.get(), ctx->id("C[10]"));
            replace_port(ci, ctx->id("C[11]"), packed.get(), ctx->id("C[11]"));
            replace_port(ci, ctx->id("C[12]"), packed.get(), ctx->id("C[12]"));
            replace_port(ci, ctx->id("C[13]"), packed.get(), ctx->id("C[13]"));
            replace_port(ci, ctx->id("C[14]"), packed.get(), ctx->id("C[14]"));
            replace_port(ci, ctx->id("C[15]"), packed.get(), ctx->id("C[15]"));
            replace_port(ci, ctx->id("C[16]"), packed.get(), ctx->id("C[16]"));
            replace_port(ci, ctx->id("C[17]"), packed.get(), ctx->id("C[17]"));
            replace_port(ci, ctx->id("C[18]"), packed.get(), ctx->id("C[18]"));
            replace_port(ci, ctx->id("C[19]"), packed.get(), ctx->id("C[19]"));

            replace_port(ci, ctx->id("A[0]"), packed.get(), ctx->id("A[0]"));
            replace_port(ci, ctx->id("A[1]"), packed.get(), ctx->id("A[1]"));
            replace_port(ci, ctx->id("A[2]"), packed.get(), ctx->id("A[2]"));
            replace_port(ci, ctx->id("A[3]"), packed.get(), ctx->id("A[3]"));
            replace_port(ci, ctx->id("A[4]"), packed.get(), ctx->id("A[4]"));
            replace_port(ci, ctx->id("A[5]"), packed.get(), ctx->id("A[5]"));
            replace_port(ci, ctx->id("A[6]"), packed.get(), ctx->id("A[6]"));
            replace_port(ci, ctx->id("A[7]"), packed.get(), ctx->id("A[7]"));

            replace_port(ci, ctx->id("B[0]"), packed.get(), ctx->id("B[0]"));
            replace_port(ci, ctx->id("B[1]"), packed.get(), ctx->id("B[1]"));
            replace_port(ci, ctx->id("B[2]"), packed.get(), ctx->id("B[2]"));
            replace_port(ci, ctx->id("B[3]"), packed.get(), ctx->id("B[3]"));
            replace_port(ci, ctx->id("B[4]"), packed.get(), ctx->id("B[4]"));
            replace_port(ci, ctx->id("B[5]"), packed.get(), ctx->id("B[5]"));
            replace_port(ci, ctx->id("B[6]"), packed.get(), ctx->id("B[6]"));
            replace_port(ci, ctx->id("B[7]"), packed.get(), ctx->id("B[7]"));

            replace_port(ci, ctx->id("clr"), packed.get(), ctx->id("clr"));
            log_info("packed cell %s into %s\n", ci->name.c_str(ctx), packed->name.c_str(ctx));
            new_cells.push_back(std::move(packed));


        }
    }
    for (auto pcell : packed_cells) {
        ctx->cells.erase(pcell);
    }
    for (auto &ncell : new_cells) {
        ctx->cells[ncell->name] = std::move(ncell);
    }
}

// Pack FFs not packed as LUTFFs
static void pack_nonlut_ffs(Context *ctx)
{
    log_info("Packing non-LUT FFs..\n");

    std::unordered_set<IdString> packed_cells;
    std::vector<std::unique_ptr<CellInfo>> new_cells;

    for (auto cell : sorted(ctx->cells)) {
        CellInfo *ci = cell.second;
        if (is_ff(ctx, ci)) {
            log_error("FF not attached to a LUT found. This cannot be packed, as all flip-flops in generated architectures are connected to LUTs. This may be due to assignment to IO pins in sequential logic - please do this with combinatorial logic.");
            std::unique_ptr<CellInfo> packed = create_diko_cell(ctx, ctx->id("diko_LC"), ci->name.str(ctx) + "_DFFLC");
            std::copy(ci->attrs.begin(), ci->attrs.end(), std::inserter(packed->attrs, packed->attrs.begin()));
            if (ctx->verbose)
                log_info("packed cell %s into %s\n", ci->name.c_str(ctx), packed->name.c_str(ctx));
            packed_cells.insert(ci->name);
            dff_to_lc(ctx, ci, packed.get(), true);
            new_cells.push_back(std::move(packed));
        }
    }
    for (auto pcell : packed_cells) {
        ctx->cells.erase(pcell);
    }


    for (auto &ncell : new_cells) {
        ctx->cells[ncell->name] = std::move(ncell);
    }
}

static bool net_is_constant(const Context *ctx, NetInfo *net, bool &value)
{
    if (net == nullptr)
        return false;
    if (net->name == ctx->id("$PACKER_GND_NET") || net->name == ctx->id("$PACKER_VCC_NET")) {
        value = (net->name == ctx->id("$PACKER_VCC_NET"));
        return true;
    } else {
        return false;
    }
}


// Pack carry logic (copied from ice40 as the carry chain design was too)
static void pack_carries(Context *ctx)
{
    log_info("Packing carries..\n");
    std::unordered_set<IdString> exhausted_cells;
    std::unordered_set<IdString> packed_cells;
    std::vector<std::unique_ptr<CellInfo>> new_cells;

    for (auto cell : sorted(ctx->cells)) {
        CellInfo *ci = cell.second;
        if (is_carry(ctx, ci)) {
            log("packed");

            packed_cells.insert(cell.first);

            CellInfo *carry_ci_lc;
            bool ci_value;
            bool ci_const = net_is_constant(ctx, ci->ports.at(ctx->id("CI")).net, ci_value);
            if (ci_const) {
                carry_ci_lc = nullptr;
            } else {
                carry_ci_lc = net_only_drives(ctx, ci->ports.at(ctx->id("CI")).net, is_lc, ctx->id("I3"), false);
            }

            std::set<IdString> i0_matches, i1_matches;
            NetInfo *i0_net = ci->ports.at(ctx->id("I0")).net;
            NetInfo *i1_net = ci->ports.at(ctx->id("I1")).net;
            // Find logic cells connected to both I0 and I1
            if (i0_net) {
                for (auto usr : i0_net->users) {
                    if (is_lc(ctx, usr.cell) && usr.port == ctx->id("I1")) {
                        if (ctx->cells.find(usr.cell->name) != ctx->cells.end() &&
                            exhausted_cells.find(usr.cell->name) == exhausted_cells.end()) {
                            // This clause stops us double-packing cells
                            i0_matches.insert(usr.cell->name);
                            if (!i1_net && !usr.cell->ports.at(ctx->id("I2")).net) {
                                // I1 is don't care when disconnected, duplicate I0
                                i1_matches.insert(usr.cell->name);
                            }
                        }
                    }
                }
            }
            if (i1_net) {
                for (auto usr : i1_net->users) {
                    if (is_lc(ctx, usr.cell) && usr.port == ctx->id("I2")) {
                        if (ctx->cells.find(usr.cell->name) != ctx->cells.end() &&
                            exhausted_cells.find(usr.cell->name) == exhausted_cells.end()) {
                            // This clause stops us double-packing cells
                            i1_matches.insert(usr.cell->name);
                            if (!i0_net && !usr.cell->ports.at(ctx->id("I1")).net) {
                                // I0 is don't care when disconnected, duplicate I1
                                i0_matches.insert(usr.cell->name);
                            }
                        }
                    }
                }
            }

            std::set<IdString> carry_lcs;
            std::set_intersection(i0_matches.begin(), i0_matches.end(), i1_matches.begin(), i1_matches.end(),
                                  std::inserter(carry_lcs, carry_lcs.end()));
            CellInfo *carry_lc = nullptr;
            if (carry_ci_lc && carry_lcs.find(carry_ci_lc->name) != carry_lcs.end()) {
                carry_lc = carry_ci_lc;
            } else { 
                // No LC to pack into matching I0/I1, insert a new one
                std::unique_ptr<CellInfo> created_lc =
                        create_diko_cell(ctx, ctx->id("diko_LC"), cell.first.str(ctx) + "$CARRY");
                carry_lc = created_lc.get();
                created_lc->ports.at(ctx->id("I1")).net = i0_net;
                if (i0_net) {
                    PortRef pr;
                    pr.cell = created_lc.get();
                    pr.port = ctx->id("I1");
                    i0_net->users.push_back(pr);
                }
                created_lc->ports.at(ctx->id("I2")).net = i1_net;
                if (i1_net) {
                    PortRef pr;
                    pr.cell = created_lc.get();
                    pr.port = ctx->id("I2");
                    i1_net->users.push_back(pr);
                }
                new_cells.push_back(std::move(created_lc));
            }
            carry_lc->params[ctx->id("CARRY_ENABLE")] = "1";
            replace_port(ci, ctx->id("CI"), carry_lc, ctx->id("CIN"));
            replace_port(ci, ctx->id("CO"), carry_lc, ctx->id("COUT"));
            if (i0_net) {
                auto &i0_usrs = i0_net->users;
                i0_usrs.erase(std::remove_if(i0_usrs.begin(), i0_usrs.end(), [ci, ctx](const PortRef &pr) {
                    return pr.cell == ci && pr.port == ctx->id("I0");
                }));
            }
            if (i1_net) {
                auto &i1_usrs = i1_net->users;
                i1_usrs.erase(std::remove_if(i1_usrs.begin(), i1_usrs.end(), [ci, ctx](const PortRef &pr) {
                    return pr.cell == ci && pr.port == ctx->id("I1");
                }));
            }

            // Check for constant driver on CIN
            if (carry_lc->ports.at(ctx->id("CIN")).net != nullptr) {
                IdString cin_net = carry_lc->ports.at(ctx->id("CIN")).net->name;
                if (cin_net == ctx->id("$PACKER_GND_NET") || cin_net == ctx->id("$PACKER_VCC_NET")) {
                    carry_lc->params[ctx->id("CIN_CONST")] = "1";
                    carry_lc->params[ctx->id("CIN_SET")] = cin_net == ctx->id("$PACKER_VCC_NET") ? "1" : "0";
                    carry_lc->ports.at(ctx->id("CIN")).net = nullptr;
                    auto &cin_users = ctx->nets.at(cin_net)->users;
                    cin_users.erase(
                            std::remove_if(cin_users.begin(), cin_users.end(), [carry_lc, ctx](const PortRef &pr) {
                                return pr.cell == carry_lc && pr.port == ctx->id("CIN");
                            }));
                }
            }
            exhausted_cells.insert(carry_lc->name);
        }
    }
    for (auto pcell : packed_cells) {
        ctx->cells.erase(pcell);
    }
    for (auto &ncell : new_cells) {
        ctx->cells[ncell->name] = std::move(ncell);
    }
}
// "Pack" RAMs
static void pack_ram(Context *ctx)
{
    //log_info("Packing RAMs..\n");
    // TODO
}

// Merge a net into a constant net
static void set_net_constant(const Context *ctx, NetInfo *orig, NetInfo *constnet, bool constval)
{
    orig->driver.cell = nullptr;
    for (auto user : orig->users) {
        if (user.cell != nullptr) {
            CellInfo *uc = user.cell;
            if (ctx->verbose)
                log_info("%s user %s\n", orig->name.c_str(ctx), uc->name.c_str(ctx));
            if ((is_lut(ctx, uc) || is_lc(ctx, uc) || is_carry(ctx, uc)) && (user.port.str(ctx).at(0) == 'I') &&
                !constval) {
                uc->ports[user.port].net = nullptr;
            } else {
                uc->ports[user.port].net = constnet;
                constnet->users.push_back(user);
            }
        }
    }
    orig->users.clear();
}

// Pack constants (simple implementation)
static void pack_constants(Context *ctx)
{
    log_info("Packing constants..\n");

    std::unique_ptr<CellInfo> gnd_cell = create_diko_cell(ctx, ctx->id("diko_LC"), "$PACKER_GND");
    gnd_cell->params[ctx->id("INIT")] = "0";
    std::unique_ptr<NetInfo> gnd_net = std::unique_ptr<NetInfo>(new NetInfo);
    gnd_net->name = ctx->id("$PACKER_GND_NET");
    gnd_net->driver.cell = gnd_cell.get();
    gnd_net->driver.port = id_O;
    gnd_cell->ports.at(id_O).net = gnd_net.get();

    std::unique_ptr<CellInfo> vcc_cell = create_diko_cell(ctx, ctx->id("diko_LC"), "$PACKER_VCC");
    vcc_cell->params[ctx->id("INIT")] = "1";
    std::unique_ptr<NetInfo> vcc_net = std::unique_ptr<NetInfo>(new NetInfo);
    vcc_net->name = ctx->id("$PACKER_VCC_NET");
    vcc_net->driver.cell = vcc_cell.get();
    vcc_net->driver.port = id_O;
    vcc_cell->ports.at(id_O).net = vcc_net.get();

    std::vector<IdString> dead_nets;

    bool gnd_used = false;

    for (auto net : sorted(ctx->nets)) {
        NetInfo *ni = net.second;
        if (ni->driver.cell != nullptr && ni->driver.cell->type == ctx->id("GND")) {
            IdString drv_cell = ni->driver.cell->name;
            set_net_constant(ctx, ni, gnd_net.get(), false);
            gnd_used = true;
            dead_nets.push_back(net.first);
            ctx->cells.erase(drv_cell);
        } else if (ni->driver.cell != nullptr && ni->driver.cell->type == ctx->id("VCC")) {
            IdString drv_cell = ni->driver.cell->name;
            set_net_constant(ctx, ni, vcc_net.get(), true);
            dead_nets.push_back(net.first);
            ctx->cells.erase(drv_cell);
        }
    }

    if (gnd_used) {
        ctx->cells[gnd_cell->name] = std::move(gnd_cell);
        ctx->nets[gnd_net->name] = std::move(gnd_net);
    }
    // Vcc cell always inserted for now, as it may be needed during carry legalisation (TODO: trim later if actually
    // never used?)
    ctx->cells[vcc_cell->name] = std::move(vcc_cell);
    ctx->nets[vcc_net->name] = std::move(vcc_net);

    for (auto dn : dead_nets) {
        ctx->nets.erase(dn);
    }
}

static bool is_nextpnr_iob(Context *ctx, CellInfo *cell)
{
//    return cell->type == ctx->id("$nextpnr_ibuf") || cell->type == ctx->id("$nextpnr_obuf") ||
//           cell->type == ctx->id("$nextpnr_iobuf") || cell->type == ctx->id("IOB18") || cell->type == ctx->id("IOB33");
    return cell->type == ctx->id("$nextpnr_iobuf");

}

static bool is_monodir_iob(Context *ctx, CellInfo *cell)
{
//    return cell->type == ctx->id("$nextpnr_ibuf") || cell->type == ctx->id("$nextpnr_obuf") ||
//           cell->type == ctx->id("$nextpnr_iobuf") || cell->type == ctx->id("IOB18") || cell->type == ctx->id("IOB33");

    return (cell->type == ctx->id("IBUF") || cell->type == ctx->id("OBUF"));

}


// Pack IO buffers
static void pack_io(Context *ctx)
{
    std::unordered_set<IdString> packed_cells;
    std::vector<std::unique_ptr<CellInfo>> new_cells;
    log_info("Packing IOs..\n");

    for (auto cell : sorted(ctx->cells)) {
        CellInfo *ci = cell.second;
        if (is_nextpnr_iob(ctx, ci)) {
            log("WOO");
            CellInfo *sb = nullptr;
            if (ci->type == ctx->id("$nextpnr_ibuf") || ci->type == ctx->id("$nextpnr_iobuf")) {
                sb = net_only_drives(ctx, ci->ports.at(ctx->id("O")).net, is_sb_io, ctx->id("PACKAGE_PIN"), true, ci);

            } else if (ci->type == ctx->id("$nextpnr_obuf")) {
                sb = net_only_drives(ctx, ci->ports.at(ctx->id("I")).net, is_sb_io, ctx->id("PACKAGE_PIN"), true, ci);
            }
            if (sb != nullptr) {
                log("SB case");
                // Trivial case, IOBUF used. Just destroy the net and the
                // iobuf
                log_info("%s feeds IOBUF %s, removing %s %s.\n", ci->name.c_str(ctx), sb->name.c_str(ctx),
                         ci->type.c_str(ctx), ci->name.c_str(ctx));
                NetInfo *net = sb->ports.at(ctx->id("PACKAGE_PIN")).net;
                if (net != nullptr) {
                    ctx->nets.erase(net->name);
                    sb->ports.at(ctx->id("PACKAGE_PIN")).net = nullptr;
                }
                if (ci->type == ctx->id("$nextpnr_iobuf")) {
                    NetInfo *net2 = ci->ports.at(ctx->id("I")).net;
                    if (net2 != nullptr) {
                        ctx->nets.erase(net2->name);
                    }
                }
            } else {
                // std::unique_ptr<CellInfo> diko_cell;
                // Create a IOBUF buffer
                // if (ci->type == ctx->id("$nextpnr_iobuf")){
                    std::unique_ptr<CellInfo> diko_cell = create_diko_cell(ctx, ctx->id("IO_1_bidirectional_frame_config_pass"), ci->name.str(ctx));
                    nxio_to_sb(ctx, ci, diko_cell.get());
                // } else if (ci->type == ctx->id("$nextpnr_ibuf")){
                    // std::unique_ptr<CellInfo> diko_cell = create_diko_cell(ctx, ctx->id("InPass4_frame_config"), ci->name.str(ctx));
                // }
                // else if (ci->type == ctx->id("$nextpnr_obuf")){
                    // std::unique_ptr<CellInfo> diko_cell = create_diko_cell(ctx, ctx->id("OutPass4_frame_config"), ci->name.str(ctx));
                // }

                new_cells.push_back(std::move(diko_cell));
                sb = new_cells.back().get();
            }
            packed_cells.insert(ci->name);
            std::copy(ci->attrs.begin(), ci->attrs.end(), std::inserter(sb->attrs, sb->attrs.begin()));
        }
        if (ci->type == ctx->id("IO_1_bidirectional_frame_config_pass")){
                log("starting");
                std::unique_ptr<CellInfo> packed = create_diko_cell(ctx, ctx->id("IO_1_bidirectional_frame_config_pass"), ci->name.str(ctx));// + "_mul");

                // log("I");
                // //replace_port(ci, ctx->id("I"), packed.get(), ctx->id("I"));
                // log("O");
                // //replace_port(ci, ctx->id("O"), packed.get(), ctx->id("O"));
                // log("T");
                // //replace_port(ci, ctx->id("T"), packed.get(), ctx->id("T"));
                // log("Q");
                //replace_port(ci, ctx->id("Q"), packed.get(), ctx->id("Q"));
        }
    }
    for (auto pcell : packed_cells) {
        ctx->cells.erase(pcell);


    // for (const auto &[key, value]: ctx->cells){
    //     std::cout << "key: " << key.c_str(ctx) << ", value: " << value->name.c_str(ctx) << std::endl;
    // }

    }
    for (auto &ncell : new_cells) {
        //std::cout << "Addr of map" << &ctx->cells << std::endl;
        ctx->cells[ncell->name] = std::move(ncell);
        //ctx->cells.insert(std::make_pair(ncell->name, ncell));
    }
}

static void pack_monodir_io(Context *ctx)
{
    log_info("Packing input & output ports..\n");

    std::unordered_set<IdString> packed_cells;
    std::vector<std::unique_ptr<CellInfo>> new_cells;
    for (auto cell : sorted(ctx->cells)) {
        CellInfo *ci = cell.second;
//        if (ctx->verbose)
//            log_info("cell '%s' is of type '%s'\n", ci->name.c_str(ctx), ci->type.c_str(ctx));
        if (is_monodir_iob(ctx, ci)) {
            std::unique_ptr<CellInfo> packed;
            if (ci->type == ctx->id("InPass4_frame_config")){
                packed = create_diko_cell(ctx, ctx->id("InPass4_frame_config"), ci->name.str(ctx));// + "_mul");
                replace_port(ci, ctx->id("O1"), packed.get(), ctx->id("O1"));
                replace_port(ci, ctx->id("O2"), packed.get(), ctx->id("O2"));
                replace_port(ci, ctx->id("O3"), packed.get(), ctx->id("O3"));
                replace_port(ci, ctx->id("O3"), packed.get(), ctx->id("O3"));
            } else {
                packed = create_diko_cell(ctx, ctx->id("OutPass4_frame_config"), ci->name.str(ctx));// + "_mul");
                replace_port(ci, ctx->id("I1"), packed.get(), ctx->id("I1"));
                replace_port(ci, ctx->id("I2"), packed.get(), ctx->id("I2"));
                replace_port(ci, ctx->id("I3"), packed.get(), ctx->id("I3"));
                replace_port(ci, ctx->id("I3"), packed.get(), ctx->id("I3"));
            }
            std::copy(ci->attrs.begin(), ci->attrs.end(), std::inserter(packed->attrs, packed->attrs.begin()));
            packed_cells.insert(ci->name);
            if (ctx->verbose)
                log_info("packed cell %s into %s\n", ci->name.c_str(ctx), packed->name.c_str(ctx));
            new_cells.push_back(std::move(packed));


        }


    }
    for (auto pcell : packed_cells) {
        ctx->cells.erase(pcell);
    }
    for (auto &ncell : new_cells) {
        ctx->cells[ncell->name] = std::move(ncell);
    }
}



// Return true if a port counts as "logic" for global promotion
static bool is_logic_port(BaseCtx *ctx, const PortRef &port)
{
    if (is_clock_port(ctx, port) || is_reset_port(ctx, port) || is_enable_port(ctx, port))
        return false;
    return !is_sb_io(ctx, port.cell) && port.cell->type != id_BUFGCTRL;
}

static void insert_global(Context *ctx, NetInfo *net, bool is_reset, bool is_cen, bool is_logic)
{
    std::string glb_name = net->name.str(ctx) + std::string("_$glb_") + (is_reset ? "sr" : (is_cen ? "ce" : "clk"));
    std::unique_ptr<CellInfo> gb = create_diko_cell(ctx, id_BUFGCTRL, "$bufg_" + glb_name);
    gb->ports[ctx->id("I0")].net = net;
    PortRef pr;
    pr.cell = gb.get();
    pr.port = ctx->id("I0");
    net->users.push_back(pr);

    pr.cell = gb.get();
    pr.port = ctx->id("O");
    std::unique_ptr<NetInfo> glbnet = std::unique_ptr<NetInfo>(new NetInfo());
    glbnet->name = ctx->id(glb_name);
    glbnet->driver = pr;
    gb->ports[ctx->id("O")].net = glbnet.get();
    std::vector<PortRef> keep_users;
    for (auto user : net->users) {
        if (is_clock_port(ctx, user) || (is_reset && is_reset_port(ctx, user)) ||
            (is_cen && is_enable_port(ctx, user)) || (is_logic && is_logic_port(ctx, user))) {
            user.cell->ports[user.port].net = glbnet.get();
            glbnet->users.push_back(user);
        } else {
            keep_users.push_back(user);
        }
    }
    net->users = keep_users;
    ctx->nets[glbnet->name] = std::move(glbnet);
    ctx->cells[gb->name] = std::move(gb);
}

// Simple global promoter (clock only)
static void promote_globals(Context *ctx)
{
    log_info("Promoting globals..\n");
    const int logic_fanout_thresh = 15;
    const int enable_fanout_thresh = 5;
    std::map<IdString, int> clock_count, reset_count, cen_count, logic_count;
    for (auto net : sorted(ctx->nets)) {
        NetInfo *ni = net.second;
        if (ni->driver.cell != nullptr && !ctx->isGlobalNet(ni)) {
            clock_count[net.first] = 0;
            reset_count[net.first] = 0;
            cen_count[net.first] = 0;

            for (auto user : ni->users) {
                if (is_clock_port(ctx, user))
                    clock_count[net.first]++;
                if (is_reset_port(ctx, user))
                    reset_count[net.first]++;
                if (is_enable_port(ctx, user))
                    cen_count[net.first]++;
                if (is_logic_port(ctx, user))
                    logic_count[net.first]++;
            }
        }
    }
    int prom_globals = 0, prom_resets = 0, prom_cens = 0, prom_logics = 0;
    int gbs_available = 8;
    for (auto &cell : ctx->cells)
        if (is_gbuf(ctx, cell.second.get()))
            --gbs_available;
    while (prom_globals < gbs_available) {
        auto global_clock = std::max_element(clock_count.begin(), clock_count.end(),
                                             [](const std::pair<IdString, int> &a, const std::pair<IdString, int> &b) {
                                                 return a.second < b.second;
                                             });

        auto global_reset = std::max_element(reset_count.begin(), reset_count.end(),
                                             [](const std::pair<IdString, int> &a, const std::pair<IdString, int> &b) {
                                                 return a.second < b.second;
                                             });
        auto global_cen = std::max_element(cen_count.begin(), cen_count.end(),
                                           [](const std::pair<IdString, int> &a, const std::pair<IdString, int> &b) {
                                               return a.second < b.second;
                                           });
        auto global_logic = std::max_element(logic_count.begin(), logic_count.end(),
                                             [](const std::pair<IdString, int> &a, const std::pair<IdString, int> &b) {
                                                 return a.second < b.second;
                                             });
        if (global_clock->second == 0 && prom_logics < 4 && global_logic->second > logic_fanout_thresh &&
            (global_logic->second > global_cen->second || prom_cens >= 4) &&
            (global_logic->second > global_reset->second || prom_resets >= 4)) {
            NetInfo *logicnet = ctx->nets[global_logic->first].get();
            insert_global(ctx, logicnet, false, false, true);
            ++prom_globals;
            ++prom_logics;
            clock_count.erase(logicnet->name);
            reset_count.erase(logicnet->name);
            cen_count.erase(logicnet->name);
            logic_count.erase(logicnet->name);
        } else if (global_reset->second > global_clock->second && prom_resets < 4) {
            NetInfo *rstnet = ctx->nets[global_reset->first].get();
            insert_global(ctx, rstnet, true, false, false);
            ++prom_globals;
            ++prom_resets;
            clock_count.erase(rstnet->name);
            reset_count.erase(rstnet->name);
            cen_count.erase(rstnet->name);
            logic_count.erase(rstnet->name);
        } else if (global_cen->second > global_clock->second && prom_cens < 4 &&
                   global_cen->second > enable_fanout_thresh) {
            NetInfo *cennet = ctx->nets[global_cen->first].get();
            insert_global(ctx, cennet, false, true, false);
            ++prom_globals;
            ++prom_cens;
            clock_count.erase(cennet->name);
            reset_count.erase(cennet->name);
            cen_count.erase(cennet->name);
            logic_count.erase(cennet->name);
        } else if (global_clock->second != 0) {
            NetInfo *clknet = ctx->nets[global_clock->first].get();
            insert_global(ctx, clknet, false, false, false);
            ++prom_globals;
            clock_count.erase(clknet->name);
            reset_count.erase(clknet->name);
            cen_count.erase(clknet->name);
            logic_count.erase(clknet->name);
        } else {
            break;
        }
    }
}

// spliceLUT adds a pass-through LUT LC between the given cell's output port
// and either all users or only non_LUT users.
static std::unique_ptr<CellInfo> spliceLUT(Context *ctx, CellInfo *ci, IdString portId, bool onlyNonLUTs)
{
    auto port = ci->ports[portId];

    NPNR_ASSERT(port.net != nullptr);

    // Create pass-through LUT.
    std::unique_ptr<CellInfo> pt =
            create_diko_cell(ctx, ctx->id("diko_LC"), ci->name.str(ctx) + "$nextpnr_" + portId.str(ctx) + "_lut_through");
    pt->params[ctx->id("INIT")] = "65280"; // output is always I3

    // Create LUT output net.
    std::unique_ptr<NetInfo> out_net = std::unique_ptr<NetInfo>(new NetInfo);
    out_net->name = ctx->id(ci->name.str(ctx) + "$nextnr_" + portId.str(ctx) + "_lut_through_net");
    out_net->driver.cell = pt.get();
    out_net->driver.port = ctx->id("O");
    pt->ports.at(ctx->id("O")).net = out_net.get();

    // New users of the original cell's port
    std::vector<PortRef> new_users;
    for (const auto &user : port.net->users) {
        if (onlyNonLUTs && user.cell->type == ctx->id("diko_LC")) {
            new_users.push_back(user);
            continue;
        }
        // Rewrite pointer into net in user.
        user.cell->ports[user.port].net = out_net.get();
        // Add user to net.
        PortRef pr;
        pr.cell = user.cell;
        pr.port = user.port;
        out_net->users.push_back(pr);
    }

    // Add LUT to new users.
    PortRef pr;
    pr.cell = pt.get();
    pr.port = ctx->id("I3");
    new_users.push_back(pr);
    pt->ports.at(ctx->id("I3")).net = port.net;

    // Replace users of the original net.
    port.net->users = new_users;

    ctx->nets[out_net->name] = std::move(out_net);
    return pt;
}

// Pack special functions
static void pack_special(Context *ctx)
{
    log_info("Packing special functions..\n");

    std::unordered_set<IdString> packed_cells;
    std::vector<std::unique_ptr<CellInfo>> new_cells;

    for (auto cell : sorted(ctx->cells)) {
        CellInfo *ci = cell.second;
        if (ci->type == id_BUFGCTRL) {
            ci->params.emplace(ctx->id("PRESELECT_I0"), "FALSE");
            ci->params.emplace(ctx->id("CE0INV"), "CE0");
            ci->params.emplace(ctx->id("S0INV"), "S0");
            ci->params.emplace(ctx->id("IGNORE0INV"), "IGNORE0");
            ci->params.emplace(ctx->id("CE1INV"), "CE1");
            ci->params.emplace(ctx->id("S1INV"), "S1");
            ci->params.emplace(ctx->id("IGNORE1INV"), "IGNORE1");
        } else if (ci->type == id_MMCME2_ADV) {
            ci->params.emplace(ctx->id("BANDWIDTH"), "OPTIMIZED");
            ci->params.emplace(ctx->id("CLKBURST_ENABLE"), "FALSE");
            ci->params.emplace(ctx->id("CLKBURST_REPEAT"), "FALSE");
            ci->params.emplace(ctx->id("CLKFBIN_EDGE"), "FALSE");
            ci->params.emplace(ctx->id("CLKFBIN_NOCOUNT"), "TRUE");
            ci->params.emplace(ctx->id("CLKFBOUT_EDGE"), "FALSE");
            ci->params.emplace(ctx->id("CLKFBOUT_EN"), "TRUE");
            ci->params.emplace(ctx->id("CLKFBOUT_FRAC_EN"), "FALSE");
            ci->params.emplace(ctx->id("CLKFBOUT_FRAC_WF_FALL"), "FALSE");
            ci->params.emplace(ctx->id("CLKFBOUT_FRAC_WF_RISE"), "FALSE");
            ci->params.emplace(ctx->id("CLKFBOUT_NOCOUNT"), "TRUE");
            ci->params.emplace(ctx->id("CLKFBOUT_USE_FINE_PS"), "FALSE");
            ci->params.emplace(ctx->id("CLKINSELINV"), "CLKINSEL");
            ci->params.emplace(ctx->id("CLKOUT0_EDGE"), "FALSE");
            ci->params.emplace(ctx->id("CLKOUT0_EN"), "FALSE");
            ci->params.emplace(ctx->id("CLKOUT0_FRAC_EN"), "FALSE");
            ci->params.emplace(ctx->id("CLKOUT0_FRAC_WF_FALL"), "FALSE");
            ci->params.emplace(ctx->id("CLKOUT0_FRAC_WF_RISE"), "FALSE");
            ci->params.emplace(ctx->id("CLKOUT0_NOCOUNT"), "TRUE");
            ci->params.emplace(ctx->id("CLKOUT0_USE_FINE_PS"), "FALSE");
            ci->params.emplace(ctx->id("CLKOUT1_EDGE"), "FALSE");
            ci->params.emplace(ctx->id("CLKOUT1_EN"), "FALSE");
            ci->params.emplace(ctx->id("CLKOUT1_NOCOUNT"), "TRUE");
            ci->params.emplace(ctx->id("CLKOUT1_USE_FINE_PS"), "FALSE");
            ci->params.emplace(ctx->id("CLKOUT2_EDGE"), "FALSE");
            ci->params.emplace(ctx->id("CLKOUT2_EN"), "FALSE");
            ci->params.emplace(ctx->id("CLKOUT2_NOCOUNT"), "TRUE");
            ci->params.emplace(ctx->id("CLKOUT2_USE_FINE_PS"), "FALSE");
            ci->params.emplace(ctx->id("CLKOUT3_EDGE"), "FALSE");
            ci->params.emplace(ctx->id("CLKOUT3_EN"), "FALSE");
            ci->params.emplace(ctx->id("CLKOUT3_NOCOUNT"), "TRUE");
            ci->params.emplace(ctx->id("CLKOUT3_USE_FINE_PS"), "FALSE");
            ci->params.emplace(ctx->id("CLKOUT4_CASCADE"), "FALSE");
            ci->params.emplace(ctx->id("CLKOUT4_EDGE"), "FALSE");
            ci->params.emplace(ctx->id("CLKOUT4_EN"), "FALSE");
            ci->params.emplace(ctx->id("CLKOUT4_NOCOUNT"), "TRUE");
            ci->params.emplace(ctx->id("CLKOUT4_USE_FINE_PS"), "FALSE");
            ci->params.emplace(ctx->id("CLKOUT5_EDGE"), "FALSE");
            ci->params.emplace(ctx->id("CLKOUT5_EN"), "FALSE");
            ci->params.emplace(ctx->id("CLKOUT5_NOCOUNT"), "TRUE");
            ci->params.emplace(ctx->id("CLKOUT5_USE_FINE_PS"), "FALSE");
            ci->params.emplace(ctx->id("CLKOUT6_EDGE"), "FALSE");
            ci->params.emplace(ctx->id("CLKOUT6_EN"), "FALSE");
            ci->params.emplace(ctx->id("CLKOUT6_NOCOUNT"), "TRUE");
            ci->params.emplace(ctx->id("CLKOUT6_USE_FINE_PS"), "FALSE");
            ci->params.emplace(ctx->id("COMPENSATION"), "INTERNAL");
            ci->params.emplace(ctx->id("DIRECT_PATH_CNTRL"), "FALSE");
            ci->params.emplace(ctx->id("DIVCLK_EDGE"), "FALSE");
            ci->params.emplace(ctx->id("DIVCLK_NOCOUNT"), "TRUE");
            ci->params.emplace(ctx->id("EN_VCO_DIV1"), "FALSE");
            ci->params.emplace(ctx->id("EN_VCO_DIV6"), "FALSE");
            ci->params.emplace(ctx->id("GTS_WAIT"), "FALSE");
            ci->params.emplace(ctx->id("HVLF_CNT_TEST_EN"), "FALSE");
            ci->params.emplace(ctx->id("INTERP_TEST"), "FALSE");
            ci->params.emplace(ctx->id("IN_DLY_EN"), "TRUE");
            ci->params.emplace(ctx->id("LF_LOW_SEL"), "FALSE");
            ci->params.emplace(ctx->id("MMCM_EN"), "TRUE");
            ci->params.emplace(ctx->id("PERF0_USE_CLK"), "FALSE");
            ci->params.emplace(ctx->id("PERF1_USE_CLK"), "FALSE");
            ci->params.emplace(ctx->id("PERF2_USE_CLK"), "FALSE");
            ci->params.emplace(ctx->id("PERF3_USE_CLK"), "FALSE");
            ci->params.emplace(ctx->id("PSENINV"), "PSEN");
            ci->params.emplace(ctx->id("PSINCDECINV"), "PSINCDEC");
            ci->params.emplace(ctx->id("PWRDWNINV"), "PWRDWN");
            ci->params.emplace(ctx->id("RSTINV"), "RST");
            ci->params.emplace(ctx->id("SEL_HV_NMOS"), "FALSE");
            ci->params.emplace(ctx->id("SEL_LV_NMOS"), "FALSE");
            ci->params.emplace(ctx->id("SEL_SLIPD"), "FALSE");
            ci->params.emplace(ctx->id("SS_EN"), "FALSE");
            ci->params.emplace(ctx->id("SS_MODE"), "CENTER_HIGH");
            ci->params.emplace(ctx->id("STARTUP_WAIT"), "FALSE");
            ci->params.emplace(ctx->id("SUP_SEL_AREG"), "FALSE");
            ci->params.emplace(ctx->id("SUP_SEL_DREG"), "FALSE");
            ci->params.emplace(ctx->id("TMUX_MUX_SEL"), "00");
            ci->params.emplace(ctx->id("VLF_HIGH_DIS_B"), "TRUE");
            ci->params.emplace(ctx->id("VLF_HIGH_PWDN_B"), "TRUE");
            //ci->params.emplace(ctx->id("MMCME2_ADV:mmcm_adv_inst:");
            ci->params.emplace(ctx->id("ANALOG_MISC"), "0000");
            ci->params.emplace(ctx->id("AVDD_COMP_SET"), "011");
            ci->params.emplace(ctx->id("AVDD_VBG_PD"), "110");
            ci->params.emplace(ctx->id("AVDD_VBG_SEL"), "1001");
            ci->params.emplace(ctx->id("CLKBURST_CNT"), "1");
            ci->params.emplace(ctx->id("CLKFBIN_HT"), "1");
            ci->params.emplace(ctx->id("CLKFBIN_LT"), "1");
            ci->params.emplace(ctx->id("CLKFBIN_MULT"), "1");
            ci->params.emplace(ctx->id("CLKFBOUT_DT"), "0");
            ci->params.emplace(ctx->id("CLKFBOUT_FRAC"), "0");
            ci->params.emplace(ctx->id("CLKFBOUT_HT"), "1");
            ci->params.emplace(ctx->id("CLKFBOUT_LT"), "1");
            ci->params.emplace(ctx->id("CLKFBOUT_MULT_F"), "40.5");
            ci->params.emplace(ctx->id("CLKFBOUT_MX"), "00");
            ci->params.emplace(ctx->id("CLKFBOUT_PHASE"), "0.0");
            ci->params.emplace(ctx->id("CLKFBOUT_PM_FALL"), "000");
            ci->params.emplace(ctx->id("CLKFBOUT_PM_RISE"), "000");
            ci->params.emplace(ctx->id("CLKFB_MUX_SEL"), "000");
            ci->params.emplace(ctx->id("CLKIN1_MUX_SEL"), "000");
            ci->params.emplace(ctx->id("CLKIN1_PERIOD"), "8");
            ci->params.emplace(ctx->id("CLKIN2_MUX_SEL"), "000");
            ci->params.emplace(ctx->id("CLKIN2_PERIOD"), "0");
            ci->params.emplace(ctx->id("CLKOUT0_DIVIDE_F"), "16.875");
            ci->params.emplace(ctx->id("CLKOUT0_DT"), "0");
            ci->params.emplace(ctx->id("CLKOUT0_DUTY_CYCLE"), "0.5");
            ci->params.emplace(ctx->id("CLKOUT0_FRAC"), "0");
            ci->params.emplace(ctx->id("CLKOUT0_HT"), "1");
            ci->params.emplace(ctx->id("CLKOUT0_LT"), "1");
            ci->params.emplace(ctx->id("CLKOUT0_MX"), "00");
            ci->params.emplace(ctx->id("CLKOUT0_PHASE"), "0.0");
            ci->params.emplace(ctx->id("CLKOUT0_PM_FALL"), "000");
            ci->params.emplace(ctx->id("CLKOUT0_PM_RISE"), "000");
            ci->params.emplace(ctx->id("CLKOUT1_DIVIDE"), "1");
            ci->params.emplace(ctx->id("CLKOUT1_DT"), "0");
            ci->params.emplace(ctx->id("CLKOUT1_DUTY_CYCLE"), "0.5");
            ci->params.emplace(ctx->id("CLKOUT1_HT"), "1");
            ci->params.emplace(ctx->id("CLKOUT1_LT"), "1");
            ci->params.emplace(ctx->id("CLKOUT1_MX"), "00");
            ci->params.emplace(ctx->id("CLKOUT1_PHASE"), "0.0");
            ci->params.emplace(ctx->id("CLKOUT1_PM"), "000");
            ci->params.emplace(ctx->id("CLKOUT2_DIVIDE"), "1");
            ci->params.emplace(ctx->id("CLKOUT2_DT"), "0");
            ci->params.emplace(ctx->id("CLKOUT2_DUTY_CYCLE"), "0.5");
            ci->params.emplace(ctx->id("CLKOUT2_HT"), "1");
            ci->params.emplace(ctx->id("CLKOUT2_LT"), "1");
            ci->params.emplace(ctx->id("CLKOUT2_MX"), "00");
            ci->params.emplace(ctx->id("CLKOUT2_PHASE"), "0.0");
            ci->params.emplace(ctx->id("CLKOUT2_PM"), "000");
            ci->params.emplace(ctx->id("CLKOUT3_DIVIDE"), "1");
            ci->params.emplace(ctx->id("CLKOUT3_DT"), "0");
            ci->params.emplace(ctx->id("CLKOUT3_DUTY_CYCLE"), "0.5");
            ci->params.emplace(ctx->id("CLKOUT3_HT"), "1");
            ci->params.emplace(ctx->id("CLKOUT3_LT"), "1");
            ci->params.emplace(ctx->id("CLKOUT3_MX"), "00");
            ci->params.emplace(ctx->id("CLKOUT3_PHASE"), "0.0");
            ci->params.emplace(ctx->id("CLKOUT3_PM"), "000");
            ci->params.emplace(ctx->id("CLKOUT4_DIVIDE"), "1");
            ci->params.emplace(ctx->id("CLKOUT4_DT"), "0");
            ci->params.emplace(ctx->id("CLKOUT4_DUTY_CYCLE"), "0.5");
            ci->params.emplace(ctx->id("CLKOUT4_HT"), "1");
            ci->params.emplace(ctx->id("CLKOUT4_LT"), "1");
            ci->params.emplace(ctx->id("CLKOUT4_MX"), "00");
            ci->params.emplace(ctx->id("CLKOUT4_PHASE"), "0.0");
            ci->params.emplace(ctx->id("CLKOUT4_PM"), "000");
            ci->params.emplace(ctx->id("CLKOUT5_DIVIDE"), "1");
            ci->params.emplace(ctx->id("CLKOUT5_DT"), "0");
            ci->params.emplace(ctx->id("CLKOUT5_DUTY_CYCLE"), "0.5");
            ci->params.emplace(ctx->id("CLKOUT5_HT"), "1");
            ci->params.emplace(ctx->id("CLKOUT5_LT"), "1");
            ci->params.emplace(ctx->id("CLKOUT5_MX"), "00");
            ci->params.emplace(ctx->id("CLKOUT5_PHASE"), "0.0");
            ci->params.emplace(ctx->id("CLKOUT5_PM"), "000");
            ci->params.emplace(ctx->id("CLKOUT6_DIVIDE"), "1");
            ci->params.emplace(ctx->id("CLKOUT6_DT"), "0");
            ci->params.emplace(ctx->id("CLKOUT6_DUTY_CYCLE"), "0.5");
            ci->params.emplace(ctx->id("CLKOUT6_HT"), "1");
            ci->params.emplace(ctx->id("CLKOUT6_LT"), "1");
            ci->params.emplace(ctx->id("CLKOUT6_MX"), "00");
            ci->params.emplace(ctx->id("CLKOUT6_PHASE"), "0.0");
            ci->params.emplace(ctx->id("CLKOUT6_PM"), "000");
            ci->params.emplace(ctx->id("CONTROL_0"), "1111001101111100");
            ci->params.emplace(ctx->id("CONTROL_1"), "0111110101001101");
            ci->params.emplace(ctx->id("CONTROL_2"), "0101000001000010");
            ci->params.emplace(ctx->id("CONTROL_3"), "1110101111001000");
            ci->params.emplace(ctx->id("CONTROL_4"), "1101010011011111");
            ci->params.emplace(ctx->id("CONTROL_5"), "1010110111111011");
            ci->params.emplace(ctx->id("CONTROL_6"), "1011001011000011");
            ci->params.emplace(ctx->id("CONTROL_7"), "0100110000101110");
            ci->params.emplace(ctx->id("CP"), "0000");
            ci->params.emplace(ctx->id("CP_BIAS_TRIP_SET"), "0");
            ci->params.emplace(ctx->id("CP_RES"), "01");
            ci->params.emplace(ctx->id("DIVCLK_DIVIDE"), "5");
            ci->params.emplace(ctx->id("DIVCLK_HT"), "1");
            ci->params.emplace(ctx->id("DIVCLK_LT"), "1");
            ci->params.emplace(ctx->id("DVDD_COMP_SET"), "011");
            ci->params.emplace(ctx->id("DVDD_VBG_PD"), "110");
            ci->params.emplace(ctx->id("DVDD_VBG_SEL"), "1001");
            ci->params.emplace(ctx->id("EN_CURR_SINK"), "11");
            ci->params.emplace(ctx->id("FINE_PS_FRAC"), "0");
            ci->params.emplace(ctx->id("FREQ_BB_USE_CLK0"), "0");
            ci->params.emplace(ctx->id("FREQ_BB_USE_CLK1"), "0");
            ci->params.emplace(ctx->id("FREQ_BB_USE_CLK2"), "0");
            ci->params.emplace(ctx->id("FREQ_BB_USE_CLK3"), "0");
            ci->params.emplace(ctx->id("FREQ_COMP"), "01");
            ci->params.emplace(ctx->id("HROW_DLY_SET"), "0");
            ci->params.emplace(ctx->id("HVLF_CNT_TEST"), "0");
            ci->params.emplace(ctx->id("INTERP_EN"), "00010000");
            ci->params.emplace(ctx->id("IN_DLY_MX_CVDD"), "011000");
            ci->params.emplace(ctx->id("IN_DLY_MX_DVDD"), "000001");
            ci->params.emplace(ctx->id("IN_DLY_SET"), "38");
            ci->params.emplace(ctx->id("LFHF"), "11");
            ci->params.emplace(ctx->id("LF_NEN"), "10");
            ci->params.emplace(ctx->id("LF_PEN"), "00");
            ci->params.emplace(ctx->id("LOCK_CNT"), "128");
            ci->params.emplace(ctx->id("LOCK_FB_DLY"), "3");
            ci->params.emplace(ctx->id("LOCK_REF_DLY"), "3");
            ci->params.emplace(ctx->id("LOCK_SAT_HIGH"), "160");
            ci->params.emplace(ctx->id("MAN_LF"), "000");
            ci->params.emplace(ctx->id("MVDD_SEL"), "11");
            ci->params.emplace(ctx->id("PERF0_MUX_SEL"), "000");
            ci->params.emplace(ctx->id("PERF1_MUX_SEL"), "000");
            ci->params.emplace(ctx->id("PERF2_MUX_SEL"), "000");
            ci->params.emplace(ctx->id("PERF3_MUX_SEL"), "000");
            ci->params.emplace(ctx->id("PFD"), "0100001");
            ci->params.emplace(ctx->id("REF_JITTER1"), "0.01");
            ci->params.emplace(ctx->id("REF_JITTER2"), "0.01");
            ci->params.emplace(ctx->id("RES"), "0000");
            ci->params.emplace(ctx->id("SKEW_FLOP_INV"), "0000");
            ci->params.emplace(ctx->id("SPARE_ANALOG"), "00000");
            ci->params.emplace(ctx->id("SPARE_DIGITAL"), "00000");
            ci->params.emplace(ctx->id("SS_MOD_PERIOD"), "10000");
            ci->params.emplace(ctx->id("SS_STEPS"), "011");
            ci->params.emplace(ctx->id("SS_STEPS_INIT"), "010");
            ci->params.emplace(ctx->id("SYNTH_CLK_DIV"), "11");
            ci->params.emplace(ctx->id("UNLOCK_CNT"), "64");
            ci->params.emplace(ctx->id("VREF_START"), "01");

            ci->params[ctx->id("COMPENSATION")] = "INTERNAL";
        }
    }

    for (auto pcell : packed_cells) {
        ctx->cells.erase(pcell);
    }
    for (auto &ncell : new_cells) {
        ctx->cells[ncell->name] = std::move(ncell);
    }
}

// Main pack function
bool Arch::pack()
{
    Context *ctx = getCtx();
    try {
        log_break();
        pack_constants(ctx);
        // TODO
        // promote_globals(ctx);
        pack_io(ctx);
        pack_monodir_io(ctx);
        pack_lut_lutffs(ctx);
        pack_nonlut_ffs(ctx);
        pack_muxes(ctx);
        pack_DSP(ctx);
        pack_carries(ctx);
        pack_ram(ctx);
        pack_special(ctx);
        ctx->assignArchInfo();
        constrain_chains(ctx);
        ctx->assignArchInfo();
        log_info("Checksum: 0x%08x\n", ctx->checksum());

        return true;
    } catch (log_execution_error_exception) {
        return false;
    }
}

NEXTPNR_NAMESPACE_END
