/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file dropdown_type.h Types related to the drop down widget. */

#ifndef WIDGETS_DROPDOWN_TYPE_H
#define WIDGETS_DROPDOWN_TYPE_H

#include "../window_type.h"
#include "../gfx_func.h"
#include "table/strings.h"
#include <vector>

enum DropDownSyncFocus {
	DDSF_NONE = 0,
	DDSF_RECV_FOCUS = 1,
	DDSF_LOST_FOCUS = 2,
	DDSF_ALL = DDSF_RECV_FOCUS | DDSF_LOST_FOCUS,
};

/**
 * Base list item class from which others are derived. If placed in a list it
 * will appear as a horizontal line in the menu.
 */
class DropDownListItem {
public:
	int result;  ///< Result code to return to window on selection
	bool masked; ///< Masked and unselectable item

	DropDownListItem(int result, bool masked) : result(result), masked(masked) {}
	virtual ~DropDownListItem() = default;

	virtual bool Selectable() const { return false; }
	virtual uint Height(uint width) const { return FONT_HEIGHT_NORMAL; }
	virtual uint Width() const { return 0; }
	virtual void Draw(const Rect &r, bool sel, Colours bg_colour) const;
};

/**
 * Common string list item.
 */
class DropDownListStringItem : public DropDownListItem {
public:
	StringID string; ///< String ID of item
	TextColour colour_flags = TC_BEGIN;

	DropDownListStringItem(StringID string, int result, bool masked) : DropDownListItem(result, masked), string(string) {}

	bool Selectable() const override { return true; }
	uint Width() const override;
	void Draw(const Rect &r, bool sel, Colours bg_colour) const override;
	virtual StringID String() const { return this->string; }
	void SetColourFlags(TextColour colour_flags) { this->colour_flags = colour_flags; }

	static bool NatSortFunc(std::unique_ptr<const DropDownListItem> const &first, std::unique_ptr<const DropDownListItem> const &second);
};

/**
 * String list item with parameters.
 */
class DropDownListParamStringItem : public DropDownListStringItem {
public:
	uint64 decode_params[10]; ///< Parameters of the string

	DropDownListParamStringItem(StringID string, int result, bool masked) : DropDownListStringItem(string, result, masked) {}

	StringID String() const override;
	void SetParam(uint index, uint64 value) { decode_params[index] = value; }
	void SetParamStr(uint index, const char *str) { this->SetParam(index, (uint64)(size_t)str); }
};

/**
 * List item containing a C char string.
 */
class DropDownListCharStringItem : public DropDownListStringItem {
public:
	std::string raw_string;

	DropDownListCharStringItem(const std::string &raw_string, int result, bool masked) : DropDownListStringItem(STR_JUST_RAW_STRING, result, masked), raw_string(raw_string) {}

	StringID String() const override;
};

/**
 * List item with icon and string.
 */
class DropDownListIconItem : public DropDownListParamStringItem {
	SpriteID sprite;
	PaletteID pal;
	Dimension dim;
	uint sprite_y;
public:
	DropDownListIconItem(SpriteID sprite, PaletteID pal, StringID string, int result, bool masked);

	uint Height(uint width) const override;
	uint Width() const override;
	void Draw(const Rect &r, bool sel, Colours bg_colour) const override;
	void SetDimension(Dimension d);
};

/**
 * A drop down list is a collection of drop down list items.
 */
typedef std::vector<std::unique_ptr<const DropDownListItem>> DropDownList;

void ShowDropDownListAt(Window *w, DropDownList &&list, int selected, int button, Rect wi_rect, Colours wi_colour, bool instant_close = false, DropDownSyncFocus sync_parent_focus = DDSF_NONE);

void ShowDropDownList(Window *w, DropDownList &&list, int selected, int button, uint width = 0, bool instant_close = false, DropDownSyncFocus sync_parent_focus = DDSF_NONE);

#endif /* WIDGETS_DROPDOWN_TYPE_H */
