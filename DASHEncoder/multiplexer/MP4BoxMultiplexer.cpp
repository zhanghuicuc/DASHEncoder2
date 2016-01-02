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

std::string     MP4BoxMultiplexer::multiplex                  (std::string in)
{
    this->input = in;
    return this->multiplex();
}

std::string     MP4BoxMultiplexer::multiplex                  (){

    std::cout << "MP4Box multiplexing : " << this->input << " \n";

    std::string mp4box = "MP4Box ";
    mp4box.append("-add \"");
    mp4box.append(this->input);
    mp4box.append("\" \"");
    mp4box.append(this->input.substr(0,this->input.find_last_of(".")));
    mp4box.append(".mp4\"");

    std::cout << "mp4box: " <<mp4box << "\n";
    system(mp4box.c_str());

    if(this->audioFile.size()>0){
        std::cout << "MP4Box multiplexing Audio:" << this->audioFile << " \n";

        std::string mp4box = "MP4Box ";
        mp4box.append("-add \"");
        mp4box.append(this->audioFile);
        mp4box.append("\" \"");
        mp4box.append(this->input.substr(0,this->input.find_last_of(".")));
        mp4box.append(".mp4\"");

        std::cout << "mp4box: " <<mp4box << "\n";
        system(mp4box.c_str());
    }

    std::cout << "MP4Box Cleaning ... \n";

   mp4box = "MP4Box ";
   mp4box.append("-no-sys ");
   mp4box.append(" \"");
   mp4box.append(this->input.substr(0,this->input.find_last_of(".")));
   mp4box.append(".mp4\"");

   std::cout << "mp4box: " <<mp4box << "\n";
   if (system(mp4box.c_str()) < 0)
	   return DASHHelper::itos(-1);

   std::string out = this->input.substr(0, this->input.find_last_of("."));
   out.append(".mp4");
   return out;
}

