/*
 * AbstractMultiplexer.cpp
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

#include "AbstractMultiplexer.h"

AbstractMultiplexer::AbstractMultiplexer()
{
    this->audioFile = "";
}

AbstractMultiplexer::~AbstractMultiplexer()
{
    // TODO Auto-generated destructor stub
}
void AbstractMultiplexer::setInput(std::string in)
{
    this->input = in;
}
std::string AbstractMultiplexer::getInput()
{
    return this->input;
}

void            AbstractMultiplexer::setOutputDir        (std::string out){
    this->outputDir = out;
}
std::string     AbstractMultiplexer::getOutputDir        (){
    return this->outputDir;
}
void            AbstractMultiplexer::setAudioFile            (std::string file){
    this->audioFile = file;
}
std::string     AbstractMultiplexer::getAudioFile            (){
    return this->audioFile;
}

