/*
 * Copyright © 2021-2023 Synthstrom Audible Limited
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

#include "model/sample/sample_reader.h"
#include "definitions_cxx.hpp"
#include "model/sample/sample.h"
#include "storage/audio/audio_file.h"
#include "storage/audio/audio_file_manager.h"
#include "storage/cluster/cluster.h"

Error SampleReader::readBytesPassedErrorChecking(char* outputBuffer, int32_t num) {

	while (num--) {
		Error error = advanceClustersIfNecessary();
		if (error != Error::NONE) {
			return error;
		}

		*outputBuffer = currentCluster->data[byteIndexWithinCluster];
		outputBuffer++;
		byteIndexWithinCluster++;
	}

	return Error::NONE;
}

Error SampleReader::readNewCluster() {
	if (currentCluster != nullptr) {
		audioFileManager.removeReasonFromCluster(*currentCluster, "E031");
	}

	auto& sampleFile = static_cast<Sample&>(*audioFile);

	currentCluster = sampleFile.clusters.getElement(currentClusterIndex)
	                     ->getCluster(&sampleFile, currentClusterIndex, CLUSTER_LOAD_IMMEDIATELY);
	if (currentCluster == nullptr) {
		return Error::SD_CARD; // Failed to load cluster from card
	}
	return Error::NONE;
}
