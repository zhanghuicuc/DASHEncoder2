/*
 * EncoderFactory.cpp
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

#include "EncoderFactory.h"

EncoderFactory::EncoderFactory()
{
    // TODO Auto-generated constructor stub

}

EncoderFactory::~EncoderFactory()
{
    // TODO Auto-generated destructor stub
}

IEncoder* EncoderFactory::getEncoder (AnyOption* opt, IEncoder::EncoderType type){
    IEncoder* retval = NULL;
    if(type == IEncoder::H264){
		H264Encoder* e = new H264Encoder();
        e->setProfile(opt->getValue("profile"));
        e->setPreset(opt->getValue("preset"));       
        e->setFps(atoi(opt->getValue("fps")));
        e->setPasses(atoi(opt->getValue("passes")));
        e->setInput(opt->getValue("input"));
        //e->setiWidth(atoi(opt->getValue("input-width")));
		//e->setiHeight(atoi(opt->getValue("input-height")));
        e->setOutputDir(opt->getValue("dest-directory"));
		e->setCodec(opt->getValue("video-codec"));

        retval = e;

    }
    else if(type == IEncoder::VP8){

    }
    else if(type == IEncoder::AAC){
        AACEncoder* a = new AACEncoder();
        a->setInput(opt->getValue("audio-input"));
        a->setOutputDir(opt->getValue("dest-directory"));
        a->setCodec(opt->getValue("audio-codec"));
        retval = a;
    }

    return retval;
}
