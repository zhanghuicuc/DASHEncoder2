/*
 * MPDGnerator.h
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
#ifndef MPDGENERATOR_H_
#define MPDGENERATOR_H_

#include "../DASHHelper.h"
#include <vector>

class MPDGenerator
{
	std::string     outputDir;
	bool            rapAligned;
	bool			template_url;
	std::string		mpdName;
	std::string		baseurl;
	int             fragSize;
	int             segSize;
	int				minBufferTime;

    public:
        MPDGenerator();
        virtual ~MPDGenerator();

		std::string		Segment(std::vector<std::string> muxed_files);

		void            setOutputDir(std::string out);
		std::string     getOutputDir();
		void            setRAPAligned(std::string rap);
		bool            isRAPAligned();
		void            setTemplateUrl(std::string templated);
		bool            isTemplateUrl();
		void            setMpdName(std::string name);
		std::string     getMpdName();
		void            setBaseUrl(std::string url);
		std::string     getBaseUrl();
		void            setFragSize(int frag);
		int             getFragSize();
		void            setSegSize(int seg);
		int             getSegSize();		
		void            setminBufferTime(int buffer);
		int				getminBufferTime();		
};

#endif /* MPDGENERATOR_H_ */
