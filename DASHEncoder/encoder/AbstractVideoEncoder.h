/*
 * AbstractEncoder.h
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

#ifndef ABSTRACTVIDEOENCODER_H_
#define ABSTRACTVIDEOENCODER_H_

#include "AbstractEncoder.h"


class AbstractVideoEncoder : public AbstractEncoder
{

    protected:

		int					fps;
        int                 gopsize;
		int					iwidth;
		int					iheight;
        int                 owidth;
        int                 oheight;     

    public:
        AbstractVideoEncoder();
        virtual ~AbstractVideoEncoder();

        virtual std::string encode(std::string) =0;
        virtual std::string encode()            =0;

		void            setFps				(int fps);
		int				getFps				();
        void            setGOPSize          (int gop);
        int             getGOPSize          ();
        void            setiWidth           (int iw);
        int             getiWidth           ();
        void            setiHeight          (int ih);
        int             getiHeight          ();
		void            setoWidth			(int ow);
		int             getoWidth			();
		void            setoHeight			(int oh);
		int             getoHeight			();
		
};
#endif /* ABSTRACTVIDEOENCODER_H_ */
