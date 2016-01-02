/*
* AbstractEncoder.cpp
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
#include "AbstractVideoEncoder.h"

AbstractVideoEncoder::AbstractVideoEncoder()
{
	this->fps = 0;
	this->gopsize = 0;
    this->iwidth = 0;
    this->iheight = 0;
	this->owidth = 0;
	this->oheight = 0;
}

AbstractVideoEncoder::~AbstractVideoEncoder()
{
}
void            AbstractVideoEncoder::setFps			  (int fps){
	this->fps = fps;
}
int				AbstractVideoEncoder::getFps		      (){
	return this->fps;
}
void            AbstractVideoEncoder::setGOPSize          (int gop){
    this->gopsize = gop;
}
int             AbstractVideoEncoder::getGOPSize          (){
    return this->gopsize;
}
void            AbstractVideoEncoder::setiWidth            (int iw){
	this->iwidth = iw;
}
int             AbstractVideoEncoder::getiWidth            (){
    return this->iwidth;
}
void            AbstractVideoEncoder::setiHeight           (int ih){
    this->iheight = ih;
}
int             AbstractVideoEncoder::getiHeight           (){
    return this->iheight;
}
void            AbstractVideoEncoder::setoWidth(int ow){
	this->owidth = ow;
}
int             AbstractVideoEncoder::getoWidth(){
	return this->owidth;
}
void            AbstractVideoEncoder::setoHeight(int oh){
	this->oheight = oh;
}
int             AbstractVideoEncoder::getoHeight(){
	return this->oheight;
}


