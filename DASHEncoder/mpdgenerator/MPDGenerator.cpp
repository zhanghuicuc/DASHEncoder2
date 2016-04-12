/*
 * MPDGnerator.cpp
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

#include "MPDGenerator.h"

MPDGenerator::MPDGenerator()
{
	this->rapAligned = true;
	this->template_url = true;
	this->fragSize = 0;
	this->segSize = 0;
	this->minBufferTime = 0;
}

MPDGenerator::~MPDGenerator()
{
    // TODO Auto-generated destructor stub
}
void            MPDGenerator::setOutputDir(std::string out){
	this->outputDir = out;
}
std::string     MPDGenerator::getOutputDir(){
	return this->outputDir;
}
void MPDGenerator::setRAPAligned(std::string rap)
{
	if (rap == "0")
		this->rapAligned = false;
	if (rap == "1")
		this->rapAligned = true;
}
bool MPDGenerator::isRAPAligned()
{
	return this->rapAligned;
}
void MPDGenerator::setTemplateUrl(std::string templated)
{
	if (templated=="0")
		this->template_url = false;
	if (templated == "1")
		this->template_url = true;
}
bool MPDGenerator::isTemplateUrl()
{
	return this->template_url;
}
void MPDGenerator::setMpdName(std::string name)
{
	this->mpdName = name;
}
std::string MPDGenerator::getMpdName()
{
	return this->mpdName;
}
void MPDGenerator::setBaseUrl(std::string url)
{
	this->baseurl = url;
}
std::string MPDGenerator::getBaseUrl()
{
	return this->baseurl;
}
void MPDGenerator::setFragSize(int frag)
{
	this->fragSize = frag;
}
int MPDGenerator::getFragSize()
{
	return this->fragSize;
}
void MPDGenerator::setSegSize(int seg)
{
	this->segSize = seg;
}
int MPDGenerator::getSegSize()
{
	return this->segSize;
}
void MPDGenerator::setminBufferTime(int buffer)
{
	this->minBufferTime = buffer;
}
int MPDGenerator::getminBufferTime()
{
	return this->minBufferTime;
}
std::string     MPDGenerator::Segment(std::vector<std::string> muxed_files){

	std::cout << "MP4Box segmentation: \n" ;

	std::string mp4box = "MP4Box ";

	mp4box.append("-out \"");
	std::string mpdname = this->outputDir;
	mpdname.append(this->mpdName);
	mp4box.append(mpdname);
	mp4box.append("\" ");

	mp4box.append("-tmp \"");
	mp4box.append(this->outputDir);
	mp4box.append("\" ");

	if (baseurl != "0")
	{
		mp4box.append("-base-url ");
		std::string baseurl = this->baseurl;
		baseurl.append(this->outputDir);
		mp4box.append(baseurl);
		mp4box.append(" ");
	}
	
	mp4box.append("-frag ");
	mp4box.append(DASHHelper::itos(this->fragSize));
	mp4box.append(" ");

	mp4box.append("-dash ");
	mp4box.append(DASHHelper::itos(this->segSize));
	mp4box.append(" ");

	if (this->isRAPAligned()){
		mp4box.append("-rap ");
	}

	if (this->isTemplateUrl()){
		mp4box.append("-url-template ");
	}

	mp4box.append("-segment-name ");
	mp4box.append("media_$RepresentationID$_$Bandwidth$/%s");
	mp4box.append(" ");

	std::vector<std::string>::const_iterator it = muxed_files.begin();
	while (it != muxed_files.end())
	{
		mp4box.append("\"");
		mp4box.append(*it);
		++it;
	}

	std::cout << "mp4box: " << mp4box << "\n";
	if (system(mp4box.c_str()) < 0)
		return DASHHelper::itos(-1);

	return mpdname;
}

