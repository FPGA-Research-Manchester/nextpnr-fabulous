/*
 *  nextpnr -- Next Generation Place and Route
 *
 *  Copyright (C) 2018  Miodrag Milanovic <miodrag@symbioticeda.com>
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

#include "project.h"
#include <boost/filesystem/convenience.hpp>
#include <fstream>
#include "log.h"
#include "pcf.h"

NEXTPNR_NAMESPACE_BEGIN

void ProjectHandler::saveArch(Context *ctx, pt::ptree &root, std::string path)
{
    root.put("project.arch.package", ctx->archArgs().package);
    if (ctx->settings.find(ctx->id("project/input/pcf")) != ctx->settings.end()) {
        std::string fn = ctx->settings[ctx->id("project/input/pcf")];
        root.put("project.input.pcf", make_relative(fn, path).string());
    }
}

std::unique_ptr<Context> ProjectHandler::createContext(pt::ptree &root)
{
    ArchArgs chipArgs;
    std::string arch_type = root.get<std::string>("project.arch.type");
    if (arch_type == "z020") {
        chipArgs.type = ArchArgs::Z020;
    }
    chipArgs.package = root.get<std::string>("project.arch.package");

    return std::unique_ptr<Context>(new Context(chipArgs));
}

void ProjectHandler::loadArch(Context *ctx, pt::ptree &root, std::string path)
{
    auto input = root.get_child("project").get_child("input");
    boost::filesystem::path pcf = boost::filesystem::path(path) / input.get<std::string>("pcf");
    std::ifstream f(pcf.string());
    if (!apply_pcf(ctx, input.get<std::string>("pcf"), f))
        log_error("Loading PCF failed.\n");
}

NEXTPNR_NAMESPACE_END
