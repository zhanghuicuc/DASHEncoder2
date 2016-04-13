/*
 * h264Encoder.cpp
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

#include "h264Encoder.h"

std::string     H264Encoder::encode(std::string in){
    this->input = in;
    return this->encode();
}
//do the video encoding stuff and return the encoded file name (together with the outputdir)
std::string     H264Encoder::encode(){
	std::cout << "H264 Video FFmpeg Encoding @ " << this->bitrate << "kbps\n";

    std::string ffmpeg = "ffmpeg -y ";
	
	ffmpeg.append("-i \"");
	ffmpeg.append(this->input);
	ffmpeg.append("\" ");
	
	ffmpeg.append("-s ");
	ffmpeg.append(DASHHelper::itos(this->iwidth));
	ffmpeg.append("x");
	ffmpeg.append(DASHHelper::itos(this->iheight));
	ffmpeg.append(" ");

	ffmpeg.append("-vcodec ");
	ffmpeg.append(this->vcodec);
	ffmpeg.append(" ");

	ffmpeg.append("-preset ");
	ffmpeg.append(this->preset);
	ffmpeg.append(" ");

	ffmpeg.append("-profile:v ");
	ffmpeg.append(this->profile);
	ffmpeg.append(" ");
    
	ffmpeg.append("-r ");
	ffmpeg.append(DASHHelper::itos(this->fps));
	ffmpeg.append(" ");

	ffmpeg.append("-b:v ");
	ffmpeg.append(DASHHelper::itos(this->bitrate));
	ffmpeg.append("k ");
   
	ffmpeg.append("-maxrate ");
	ffmpeg.append(DASHHelper::itos(this->bitrate));
	ffmpeg.append("k ");

	ffmpeg.append("-bufsize ");
	ffmpeg.append(DASHHelper::itos(this->bitrate * 2));
	ffmpeg.append("k ");

	ffmpeg.append("-x264opts keyint=");
	ffmpeg.append(DASHHelper::itos(this->gopsize));
	ffmpeg.append(":min-keyint=");
	ffmpeg.append(DASHHelper::itos(this->gopsize));
	ffmpeg.append(":scenecut=-1 ");

    if(this->owidth>0 && this->oheight>0){
		ffmpeg.append("-s ");
		ffmpeg.append(DASHHelper::itos(this->owidth));
		ffmpeg.append("x");
		ffmpeg.append(DASHHelper::itos(this->oheight));
		ffmpeg.append(" ");
    }

	std::string transcoded_video_file = "";
	transcoded_video_file.append(this->outputDir);
	transcoded_video_file.append(this->input.substr(this->input.find_last_of("/") + 1, this->input.find_last_of(".") - this->input.find_last_of("/") - 1));
	transcoded_video_file.append("_");
	transcoded_video_file.append(DASHHelper::itos(this->owidth));
	transcoded_video_file.append("x");
	transcoded_video_file.append(DASHHelper::itos(this->oheight));
	transcoded_video_file.append("_");
	transcoded_video_file.append(DASHHelper::itos(this->bitrate));
	transcoded_video_file.append("k.h264");

	ffmpeg.append("\"");
	ffmpeg.append(transcoded_video_file + '\"');
	
    
	ffmpeg.append(" >video_transcoding_log.txt 2>&1 ");
    std::cout << "ffmpeg: " <<ffmpeg << "\n";
	if (system(ffmpeg.c_str()) < 0)
		return DASHHelper::itos(-1);

	return transcoded_video_file;
}


void            H264Encoder::setPreset(std::string pre){
    this->preset = pre;
}
std::string     H264Encoder::getPreset(){
    return this->preset;
}
void            H264Encoder::setProfile(std::string pro){
    this->profile = pro;
}
std::string     H264Encoder::getProfile(){
    return this->profile;
}
void            H264Encoder::setPasses(int p){
	this->passes = p;
}
int             H264Encoder::getPasses(){
	return this->passes;
}

