/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef PINK_MODULE_H
#define PINK_MODULE_H

#include "../archive.h"
#include <common/str.h>
#include "object.h"
#include "named_object.h"
#include <common/debug.h>
#include <engines/pink/utils.h>
#include <common/hash-str.h>
#include "inventory.h"

namespace Pink {

class ModuleProxy : public NamedObject {
public:
    ModuleProxy(){};
    ModuleProxy(const Common::String &name)
            : NamedObject(name)
    {}


};

class PinkEngine;

class Module : public NamedObject {
public:
    Module(PinkEngine *game, const Common::String &name)
        : NamedObject(name), _game(game), _page(nullptr)
    {}

    void deserialize(Archive &archive);
    void initPage(bool isLoadingSave, const Common::String *pageName);

    void OnLeftButtonDown();
    void OnMouseMove();
    void OnKeyboardButtonClick();

private:
    PinkEngine *_game;
    //Common::String _directory; doesn't need this because it was used when game had data in directories
    GamePage *_page;
    PagesArray _pages;
    InventoryMgr _invMgr;
    Common::StringMap _map; // used for saves and maybe for smth else
};


} // End of namespace Pink

#endif