/*
 * AbstractMultiplexer.h
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

#ifndef ABSTRACTMULTIPLEXER_H_
#define ABSTRACTMULTIPLEXER_H_

#include "IMultiplexer.h"
#include "../DASHHelper.h"

class AbstractMultiplexer : public IMultiplexer
{
    protected:
        std::string     vinput;
		std::string     ainput ;
		std::string     outputDir;	

    public:
        AbstractMultiplexer();
        virtual ~AbstractMultiplexer();

		virtual     std::string multiplex(std::string vinput, std::string ainput) = 0;
        virtual     std::string multiplex       () = 0;

		void            setVinput				(std::string vinput);
        std::string     getVinput               ();
		void            setAinput				(std::string ainput);
        std::string     getAinput				();
        void            setOutputDir            (std::string out);
        std::string     getOutputDir            (); 
};
#endif /* ABSTRACTMULTIPLEXER_H_ */
