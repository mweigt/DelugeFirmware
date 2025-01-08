/*
 * Copyright (c) 2014-2023 Synthstrom Audible Limited
 *
 * This file is part of The Synthstrom Audible Deluge Firmware.
 *
 * The Synthstrom Audible Deluge Firmware is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program.
 * If not, see <https://www.gnu.org/licenses/>.
 */
#pragma once
#include "definitions_cxx.hpp"
#include "gui/l10n/l10n.h"
#include "gui/menu_item/arpeggiator/note_mode.h"
#include "gui/menu_item/selection.h"
#include "gui/ui/sound_editor.h"
#include "model/song/song.h"

namespace deluge::gui::menu_item::arpeggiator {
class OctaveMode : public Selection {
public:
	using Selection::Selection;
	void readCurrentValue() override { this->setValue(soundEditor.currentArpSettings->octaveMode); }
	void writeCurrentValue() override {
		soundEditor.currentArpSettings->octaveMode = this->getValue<ArpOctaveMode>();
		soundEditor.currentArpSettings->updatePresetFromCurrentSettings();
		soundEditor.currentArpSettings->flagForceArpRestart = true;
	}
	bool isRelevant(ModControllableAudio* modControllable, int32_t whichThing) override {
		return !soundEditor.editingGateDrumRow();
	}

	deluge::vector<std::string_view> getOptions(OptType optType) override {
		(void)optType;
		using enum l10n::String;
		return {
		    l10n::getView(STRING_FOR_UP),        //<
		    l10n::getView(STRING_FOR_DOWN),      //<
		    l10n::getView(STRING_FOR_UP_DOWN),   //<
		    l10n::getView(STRING_FOR_ALTERNATE), //<
		    l10n::getView(STRING_FOR_RANDOM),    //<
		};
	}
};

class OctaveModeToNoteMode final : public OctaveMode {
public:
	using OctaveMode::OctaveMode;
	void readCurrentValue() override {
		if (display->have7SEG()) {
			display->displayPopup(deluge::l10n::get(deluge::l10n::String::STRING_FOR_OCTAVE_MODE));
		}
		OctaveMode::readCurrentValue();
	}
	MenuItem* selectButtonPress() override { return &arpeggiator::arpNoteModeFromOctaveModeMenu; }
};

extern OctaveModeToNoteMode arpOctaveModeToNoteModeMenu;
} // namespace deluge::gui::menu_item::arpeggiator
