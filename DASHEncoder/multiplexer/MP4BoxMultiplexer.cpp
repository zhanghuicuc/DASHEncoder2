/*
 * MP4BoxMultiplexer.cpp
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

#include "MP4BoxMultiplexer.h"


MP4BoxMultiplexer::MP4BoxMultiplexer()
{
    // TODO Auto-generated constructor stub

}

MP4BoxMultiplexer::~MP4BoxMultiplexer()
{
    // TODO Auto-generated destructor stub
}

std::string     MP4BoxMultiplexer::multiplex(std::string vinput, std::string ainput)
{
    this->vinput = vinput;
	this->ainput = ainput;
	return this->multiplex();
}

std::string     MP4BoxMultiplexer::multiplex                  (){

    std::cout << "MP4Box multiplexing : " << this->vinput << "and " << this->ainput<< " \n";

    std::string mp4box = "MP4Box ";
    mp4box.append("-add \"");
    mp4box.append(this->vinput);
    mp4box.append("\" \"");
	mp4box.append("-add \"");
	mp4box.append(this->ainput);
	mp4box.append("\" \"");
	mp4box.append(this->vinput.substr(0, this->vinput.find_last_of("_")));
	std::string vbitrate = this->vinput.substr(this->vinput.find_last_of("_"), this->vinput.find_last_of("_") - this->vinput.find_last_of(".") - 1);
	std::string abitrate = this->ainput.substr(this->ainput.find_last_of("_"), this->ainput.find_last_of("_") - this->ainput.find_last_of(".") - 1);
	mp4box.append("_" + vbitrate + "_" + abitrate);
	mp4box.append(".mp4\"");

    std::cout << "mp4box: " <<mp4box << "\n";
    system(mp4box.c_str());

   std::cout << "MP4Box Cleaning ... \n";

   mp4box = "MP4Box ";
   mp4box.append("-no-sys ");
   mp4box.append(" \"");
   mp4box.append(this->vinput.substr(0, this->vinput.find_last_of("_")));
   mp4box.append("_" + vbitrate + "_" + abitrate);
   mp4box.append(".mp4\"");

   std::cout << "mp4box: " <<mp4box << "\n";
   if (system(mp4box.c_str()) < 0)
	   return DASHHelper::itos(-1);

   std::string out = this->vinput.substr(0, this->vinput.find_last_of("."));
   out.append("_" + vbitrate + "_" + abitrate);
   out.append(".mp4");
   return out;
}

