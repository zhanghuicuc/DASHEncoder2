/*
 * aacEncoder.cpp
 *****************************************************************************
 * Copyright (C) 2011 Klagenfurt University
 *
 * Created on: Jun 01, 2011
 * Authors: Stefan Lederer <stefan.lederer@aau.at>
 *          Christian Timmerer  <christian.timmerer@itec.uni-klu.ac.at> 
 * 
 * Modified on: Dec 28, 2015
 * Authors: Hui Zhang <zhanghuicuc@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#include "aacEncoder.h"


std::string     AACEncoder::encode(std::string input){
    this->setInput(input);
    return this->encode();
}

//do the audio encoding stuff and return the encoded file name (together with the outputdir)
std::string     AACEncoder::encode(){
	std::cout << "AAC Audio FFmpeg Encoding @ " << this->bitrate << "kbps\n";

    std::string ffmpeg = "ffmpeg -y ";

	ffmpeg.append("-i \"");
	ffmpeg.append(this->input);
	ffmpeg.append("\" ");

	ffmpeg.append("-acodec ");
	ffmpeg.append(this->acodec);
	ffmpeg.append(" ");

	ffmpeg.append("-ac ");
	ffmpeg.append(DASHHelper::itos(this->channels));
	ffmpeg.append(" ");

	ffmpeg.append("-ar ");
	ffmpeg.append(DASHHelper::itos(this->srate));
	ffmpeg.append(" ");

	ffmpeg.append("-ab ");
	ffmpeg.append(DASHHelper::itos(this->bitrate));
	ffmpeg.append("k ");

	std::string transcoded_audio_file = "";
	transcoded_audio_file.append(this->outputDir);
	transcoded_audio_file.append(this->input.substr(this->input.find_last_of("/") + 1, this->input.find_last_of(".") - this->input.find_last_of("/") - 1));
	transcoded_audio_file.append("_");
	transcoded_audio_file.append(DASHHelper::itos(this->channels));
	transcoded_audio_file.append("ch_");
	transcoded_audio_file.append(DASHHelper::itos(this->getSamplingRate()));
	transcoded_audio_file.append("sr_");
	transcoded_audio_file.append(DASHHelper::itos(this->bitrate));
	transcoded_audio_file.append("k.aac");
	
	ffmpeg.append("\"");
	ffmpeg.append(transcoded_audio_file+'\"');
	
	ffmpeg.append(" >audio_transcoding_log.txt 2>&1 ");
	std::cout << "ffmpeg: " << ffmpeg << "\n";
	if (system(ffmpeg.c_str()) < 0)
		return DASHHelper::itos(-1);	

	return transcoded_audio_file;
}

