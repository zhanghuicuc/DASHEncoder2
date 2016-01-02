#include "AnyOption.h"
#include "multiplexer/MP4BoxMultiplexer.h"
#include "encoder/IEncoder.h"
#include "encoder/AbstractAudioEncoder.h"
#include "encoder/AbstractVideoEncoder.h"
#include "encoder/EncoderFactory.h"
#include "mpdgenerator/MPDGenerator.h"

#include <vector>
#include <iostream>
#include <map>
//#include "mysql/mysql.h"

int parse(int argc, char* argv[]);
void setHelp(AnyOption*);
void setOptions(AnyOption*);
IEncoder::EncoderType getEncoder(std::string);

AnyOption *opt;

int main(int argc, char* argv[])
{
	int ret= parse(argc, argv);
	if (ret < 0)
	{
		std::cout << "ERROR!\n";
		return -1;
	}
    return 0;
}

int parse(int argc, char* argv[])
{
    std::cout << "==========DASH ENCODER===============\n";
    //set the options and flags,
	//read the config file or command line, 
	//then get the type-value pairs automatically
	AnyOption *opt = new AnyOption();

    /* COMMAND LINE PREFERENCES  */
    opt->setVerbose(); /* print warnings about unknown options */
    opt->autoUsagePrint(true); /* print usage for bad options */

    /* SET THE USAGE/HELP   */
    setHelp(opt);

    /* SET THE OPTION STRINGS/CHARACTERS */
    setOptions(opt);

    /* PROCESS THE RESOURCE FILE */
    opt->processFile("./DASHEncoder.config");
    /* PROCESS THE COMMANDLINE, cover the same option set in config file */
    opt->processCommandArgs(argc, argv);

    /* Run DASH Encoding */
    EncoderFactory* encoder_factory= new EncoderFactory();
	IEncoder::EncoderType video_encoder_type = getEncoder(opt->getValue("video-encoder"));
	if (video_encoder_type == IEncoder::ERROR)
	{
		std::cout << "ERROR£¡Unsupported Video Encoder\n";
		return -1;
	}
	IEncoder::EncoderType audio_encoder_type = getEncoder(opt->getValue("audio-encoder"));
	if (audio_encoder_type == IEncoder::ERROR)
	{
		std::cout << "ERROR£¡Unsupported Audio Encoder\n";
		return -1;
	}

    AbstractVideoEncoder* ve = (AbstractVideoEncoder*)encoder_factory->getEncoder(opt, video_encoder_type);
    AbstractAudioEncoder* ae = (AbstractAudioEncoder*)encoder_factory->getEncoder(opt, audio_encoder_type);
    MPDGenerator* mpdgen = new MPDGenerator();
    MP4BoxMultiplexer* m = new MP4BoxMultiplexer();

    std::map<int, std::string> audio_files;
	std::map<int, std::string> video_files;
    std::map<int, int> av_mux_mapping;

    /******* MySQL Support *******/
    /*MYSQL_RES *result;

    MYSQL_ROW row;

    MYSQL *connection, mysql;

    */

	mpdgen->setOutputDir(opt->getValue("dest-directory"));
	mpdgen->setRAPAligned(opt->getValue("rap-aligned"));// (opt->getFlag("rap-aligned"));
	mpdgen->setTemplateUrl(opt->getValue("use-template-url"));// (opt->getFlag("use-template-url"));
	mpdgen->setFragSize(atoi(opt->getValue("fragment-size")));
	mpdgen->setSegSize(atoi(opt->getValue("segment-size")));
	mpdgen->setMpdName(opt->getValue("mpd-name"));
	mpdgen->setBaseUrl(opt->getValue("base-url"));
	mpdgen->setminBufferTime(atoi(opt->getValue("minBufferTime")));
	ve->setGOPSize(ve->getFps()*mpdgen->getSegSize() / 1000);
	m->setOutputDir(opt->getValue("dest-directory"));
	
    std::string h264file;
	std::string mp4file_h264;
	std::string aac_file;
	std::string mp4file_aac;
    //vector<std::string> mpds;   
    //ifstream infile;

    std::string exp_name = opt->getValue("dest-directory");
    exp_name.append(opt->getValue("mpd-name"));

	std::string mk = "mkdir \"";
	mk.append(exp_name.substr(0, exp_name.find_last_of("/")));
	mk.append("\"");
	system(mk.c_str());

    /************* AUDIO/VIDEO MULTIPLEXING INFORMATION ******************************/
    /*if(opt->getValue("mux-combi")!=NULL){
        std::string muxconfig = opt->getValue("mux-combi");
        std::string currentaudio;
        std::string bitrates;
        do{
            bitrates = muxconfig.substr(0,muxconfig.find('@'));
            if(muxconfig.find('|') == std::string::npos){
                currentaudio = muxconfig.substr(muxconfig.find('@')+1);
                muxconfig = "";
            }
            else{
				currentaudio = muxconfig.substr(muxconfig.find('@') + 1, muxconfig.find('|') - muxconfig.find('@')-1);
                muxconfig = muxconfig.substr(muxconfig.find('|')+1);
            }
            do{
                if(bitrates.find(',') == std::string::npos){
                    av_mux_mapping[atoi(bitrates.c_str())] = atoi(currentaudio.c_str());
                    bitrates = "";
                }
                else{
                    av_mux_mapping[atoi(bitrates.substr(0,',').c_str())] = atoi(currentaudio.c_str());
                    bitrates = bitrates.substr(bitrates.find(',')+1);
                }
            }while(bitrates.size()>0);
        }while(muxconfig.size()>0);
    }*/

    /************************ AUDIO PROCESSING ******************************/
    std::string audio_quality = opt->getValue("audio-quality");
    char*c2 = (char*) malloc(audio_quality.length() * sizeof(char));
    strcpy(c2, audio_quality.c_str());
    char* act_audio = strtok(c2, "|");
    std::string act_quality = "";

    while (act_audio != NULL)
    {

        act_quality = act_audio;
        ae->setChannels(atoi(act_quality.substr(0, act_quality.find(',')).c_str()));

        act_quality = act_quality.substr(act_quality.find(',') + 1);
		ae->setSamplingRate(atoi(act_quality.substr(0, act_quality.find(',')).c_str()));

		ae->setBitrate(atoi(act_quality.substr(act_quality.find(',') + 1).c_str()));

		aac_file = ae->encode();
		if ("-1" == aac_file)
		{
			std::cout << "ERROR£¡Audio Encoding \n";
			return -1;
		}
		mp4file_aac = m->multiplex(aac_file);
		if ("-1" == mp4file_aac)
		{
			std::cout << "ERROR£¡Audio Multiplexing \n";
			return -1;
		}

		audio_files[ae->getBitrate()] = mp4file_aac;

		/************ DO NOT MUX AUDIO AND VIDEO **********************/
        /*if (opt->getValue("mux-combi") == NULL)
        {*/
            /************ AUDIO FOLDER CREATION **********************/
		/*
            foldername = opt->getValue("folder-prefix");
            foldername.append("_");
            foldername.append(DASHHelper::itos(a->getChannels()));
            foldername.append("ch_");
			foldername.append(DASHHelper::itos(a->getSamplingRate()));
			foldername.append("sr_audio_");
            foldername.append(DASHHelper::itos(a->getBitrate()));
            foldername.append("kbit");

            folder = "mkdir ";
            folder.append(opt->getValue("dest-directory"));
            folder.append(foldername);
            system(folder.c_str());

            folder = "mv ";
            //folder.append(opt->getValue("dest-directory"));
            folder.append(act_audio_encoding);
            folder.append(" ");
            folder.append(opt->getValue("dest-directory"));
            folder.append(foldername);
            system(folder.c_str());*/

            /************ AUDIO MULTIPLEXING & SEGMENTATION **************/
		/*
            act_audio_seg = opt->getValue("dest-directory");
            act_audio_seg.append(foldername);
            act_audio_seg.append("\\");
            act_audio_seg.append(opt->getValue("segment-name"));

            m->setSegName(act_audio_seg);

            act_audio_seg = opt->getValue("dest-directory");
            act_audio_seg.append(foldername);
            act_audio_seg.append("\\");
            act_audio_seg.append(act_audio_encoding.substr(act_audio_encoding.find_last_of('\\')+1));

            std::string audio_mpd = m->multiplex(act_audio_seg);

			mpds.push_back(audio_mpd);*/

        act_audio = strtok(NULL, "|");
    }

    /************************ VIDEO PROCESSING ******************************/

    string bitrates = opt->getValue('b');
    char*c1 = (char*) malloc(bitrates.length() * sizeof(char));
    strcpy(c1, bitrates.c_str());
    char* pch = strtok(c1, "|");
	std::string act = "";

    while (pch != NULL)
    {
        /************VIDEO ENCODING *****************************/
        act = pch;

        if (act.find('@') != std::string::npos)
        {
            std::cout << act << "\n";
            ve->setoWidth(atoi(act.substr(act.find('@') + 1, act.find('x') - act.find('@') - 1).c_str()));
            ve->setoHeight(atoi(act.substr(act.find('x') + 1).c_str()));
            ve->setBitrate(atoi(act.substr(0, act.find('@')).c_str()));
        }
        else
        {
            ve->setBitrate(atoi(pch));
        }

        h264file = ve->encode();
		if ("-1" == h264file)
		{
			std::cout << "ERROR£¡Video Encoding \n";
			return -1;
		}

		mp4file_h264 = m->multiplex(h264file);
		if ("-1" == mp4file_h264)
		{
			std::cout << "ERROR£¡Video Multiplexing \n";
			return -1;
		}

		video_files[ve->getBitrate()] = mp4file_h264;

        /************VIDEO FOLDER CREATION **********************/
        /*foldername = opt->getValue("folder-prefix");
        foldername.append("_");
		foldername.append(DASHHelper::itos(e->getWidth()));
		foldername.append("x");
		foldername.append(DASHHelper::itos(e->getHeight()));
		foldername.append("_");
        foldername.append(DASHHelper::itos(e->getBitrate()));
        foldername.append("kbit");

        folder = "mkdir ";
        folder.append(opt->getValue("dest-directory"));
        folder.append(foldername);
        system(folder.c_str());

        folder = "mv ";
        folder.append(opt->getValue("dest-directory"));
        folder.append(h264file);
        folder.append(" ");
        folder.append(opt->getValue("dest-directory"));
        folder.append(foldername);
        system(folder.c_str());

        if(opt->getValue("mux-combi")!=NULL){

            folder = "cp ";
            folder.append(opt->getValue("dest-directory"));
            folder.append(audio_files[av_mux_mapping[e->getBitrate()]]);
            folder.append(" ");
            folder.append(opt->getValue("dest-directory"));
            folder.append(foldername);
            folder.append("\\");
            folder.append(audio_files[av_mux_mapping[e->getBitrate()]]);
            std::cout << "copy audio: " << folder;
            system(folder.c_str());
        }*/

        /************ STORE STATISTICS *********************/
/* MySQL Support Disabled
        if(opt->getFlag("store-psnr")){

           mysql_init(&mysql);

           mysql_options(&mysql, MYSQL_READ_DEFAULT_FILE,  "/opt/lampp/etc/my.cnf");
           //mysql_options(&mysql, MYSQL_OPT_SSL_VERIFY_SERVER_CERT, "/etc/ssl/certs/ComodoIntermediate.pem");
           //mysql_ssl_set(&mysql, NULL, NULL, "/etc/ssl/certs/ComodoIntermediate.pem",NULL, NULL);

           connection = mysql_real_connect(&mysql,opt->getValue("sql-host"),opt->getValue("sql-user"),opt->getValue("sql-pw"),opt->getValue("sql-database"),0,0,0);

           if (connection == NULL)
           {
               std::cout << "MySQL Error: " << mysql_error(&mysql) << "\n";
               return ;
           }
           infile.open("out.txt", ifstream::in);

           act_rep = "";
           std::string query;
           int posStart;
           int posEnd;

           if (infile.is_open())
           {
               while (infile.good())
               {
                   getline(infile, act_line);
                   if(act_line.find("x264 [debug]: frame=") != std::string::npos){
                       //std::cout << act_line << "\n";
                       query = "INSERT INTO frames (framenr, type, ypsnr,upsnr, vpsnr, representation) Values (";

                       posStart = act_line.find("frame=")+6;
                       query.append(act_line.substr(posStart,  act_line.find("QP")-posStart));
                       query.append(", \"");

                       query.append(act_line.substr(act_line.find("Slice:")+6, 1));
                       query.append("\", ");

                       posStart = act_line.find("Y:")+2;
                       query.append(act_line.substr(posStart,  act_line.find("U:")-posStart));
                       query.append(", ");

                       posStart = act_line.find("U:")+2;
                       query.append(act_line.substr(posStart,  act_line.find("V:")-posStart));
                       query.append(", ");

                       query.append(act_line.substr(act_line.find("V:")+2));
                       query.append(", \"");

                       query.append(foldername);
                       query.append("\")");
                       //std::cout << "\n" << query;
                       mysql_query(connection, query.c_str());
                   }
               }
               std::cout << "PSNR data stored in MySQL database!";
               infile.close();
           }
           else
               cout << "Error: Unable to open Log file!";
        }

*/
        /************VIDEO MULTIPLEXING & SEGMENTATION **************/

        /*h264new = opt->getValue("dest-directory");
        h264new.append(foldername);
        h264new.append("\\");
        h264new.append(opt->getValue("segment-name"));

        m->setSegName(h264new);

        h264new = opt->getValue("dest-directory");
        h264new.append(foldername);
        h264new.append("\\");
        h264new.append(h264file);

        if(opt->getValue("mux-combi")!=NULL){

            std::string af =opt->getValue("dest-directory");
            af.append(foldername);
            af.append("\\");
            af.append(audio_files[av_mux_mapping[e->getBitrate()]]);

            m->setAudioFile(af);
        }

        std::string videompd = m->multiplex(h264new);
		mpds.push_back(videompd);*/

        ve->setoWidth(0);
        ve->setoHeight(0);

        pch = strtok(NULL, "|");
    }

	/*video processing done here*/

    /* MPD Gneration */
    std::cout << "\nWriting final MPD...\n";
	std::string mpdname=mpdgen->Segment(audio_files, video_files);
	if ("-1" == mpdname)
	{
		std::cout << "ERROR£¡MPD Generate \n";
		return -1;
	}

    std::cout << "\nFINISHED! Success!\n";
    delete opt;
	return 0;
}

void setHelp(AnyOption* opt)
{
    opt->addUsage("");
    opt->addUsage("Usage: ");
    opt->addUsage("");
    opt->addUsage(" -h  --help              Print help ");
    opt->addUsage(" -d  --dest-directory    Destination directory"); 
	opt->addUsage(" -V  --video-encoder     Video encoder");
    opt->addUsage(" -A  --audio-encoder     Audio encoder");
    opt->addUsage(" -R  --multiplexer       Multiplexing tool");
	//Video options
	opt->addUsage(" -i  --input             Input file");
    //opt->addUsage(" -W  --input-width       width of input video");
	//opt->addUsage(" -H  --input-height      height of input video");
	opt->addUsage(" -b  --bitrate           Video bitrates (exmaple: see config file)");
	opt->addUsage(" -g  --fps               frame per second");
	opt->addUsage(" -p  --profile           h.264 profile");
	opt->addUsage(" -P  --preset            x264 preset");
	opt->addUsage(" -k  --passes            Encoding passes");
	opt->addUsage(" -C  --video-codec       Video Codec");
	//Audio options
	opt->addUsage(" -a  --audio-quality     Audio qualities (see config file)");
    opt->addUsage(" -I  --audio-input       Audio source");
	opt->addUsage(" -c  --audio-codec       Audio codec");
	//MP4box options
	//opt->addUsage(" -M  --mux-combi         A/V muxing combinations");
	//MPD options
	opt->addUsage(" -r  --rap-aligned       Muliplexing at GOP boundaries");
	opt->addUsage(" -D  --use-template-url  Use Segment Template");
	opt->addUsage(" -m  --mpd-name          MPD name");
	opt->addUsage(" -u  --base-url          Base url");	
	opt->addUsage(" -f  --fragment-size     Fragment size in milliseconds");
	opt->addUsage(" -S  --segment-size      DASH segment size in milliseconds");
    opt->addUsage(" -B  --minBufferTime     Minimum Buffer in MPD in milliseconds");
	//SQL options
	/*opt->addUsage(" -o  --store-psnr        Store PSNR statistics to database");
	opt->addUsage(" -y  --sql-host          MySQL host");
	opt->addUsage(" -z  --sql-user          MySQL user");
	opt->addUsage(" -Z  --sql-pw            MySQL password");
	opt->addUsage(" -Y  --sql-database      MySQL database");*/
    opt->addUsage("");
}
void setOptions(AnyOption* opt)
{
    opt->setFlag("help", 'h');
	opt->setOption("dest-directory", 'd');
	opt->setOption("video-encoder", 'V');
	opt->setOption("audio-encoder", 'A');
	opt->setOption("multiplexer", 'R');
	//Video options
    opt->setOption("input", 'i');
	//opt->setOption("input-width", 'W');
	//opt->setOption("input-height", 'H');
    opt->setOption("bitrate", 'b');
    opt->setOption("fps", 'g');
    opt->setOption("profile", 'p');
    opt->setOption("preset", 'P');
	opt->setOption("passes", 'k');
	opt->setOption("video-codec", 'C');
	//Audio options
	opt->setOption("audio-quality", 'a');
	opt->setOption("audio-input", 'I');
	opt->setOption("audio-codec", 'c');
	//MP4box options
	//opt->setOption("mux-combi", 'M');
    //MPD options
	opt->setOption("rap-aligned", 'r');
	opt->setOption("use-template-url", 'D');
	opt->setOption("mpd-name", 'm');
	opt->setOption("base-url", 'u');
	opt->setOption("fragment-size", 'f');
    opt->setOption("segment-size", 'S');
	opt->setOption("minBufferTime", 'B');
	//SQL options
    /*opt->setFlag("store-psnr", 'o');
    opt->setOption("sql-host", 'y');
    opt->setOption("sql-user", 'z');
    opt->setOption("sql-pw", 'Z');
    opt->setOption("sql-database", 'Y');*/
}

IEncoder::EncoderType getEncoder(std::string e){
    if(e.compare("VP8")==0)
        return IEncoder::VP8;
    else if(e.compare("AAC")==0)
        return IEncoder::AAC;
    else if(e.compare("H264")==0)
        return IEncoder::H264;
	else
		return IEncoder::ERROR;
}



