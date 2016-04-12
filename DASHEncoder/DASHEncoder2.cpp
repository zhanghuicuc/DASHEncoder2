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

int parse(AnyOption *opt, int argc, char* argv[]);
void setHelp(AnyOption*);
void setFileOptions(AnyOption*);
void setCommandOptions(AnyOption*);
IEncoder::EncoderType getEncoder(std::string);

int TransAudio(AnyOption *opt, AbstractAudioEncoder* a_enc, std::map<int, std::string> transcoded_audio_files);
int TransVideo(AnyOption *opt, AbstractVideoEncoder* v_enc, std::map<int, std::string> transcoded_video_files);
int AVMux(AnyOption *opt, std::map<int, int> av_mux_mapping, MP4BoxMultiplexer* muxer,
	std::map<int, std::string> transcoded_video_files,
	std::map<int, std::string> transcoded_audio_files,
	std::vector<std::string> muxed_files);
int MpdGen(MPDGenerator* mpdgen, std::vector<std::string> muxed_files, std::string mpd_path);

int main(int argc, char* argv[])
{
	std::cout << "======================DASH ENCODER======================\n" << std::endl;
	std::cout << "Usage: " << std::endl;
	std::cout << " -h  Print help " << std::endl;
	std::cout << " -i  Input config file" << std::endl;

	AnyOption *opt = new AnyOption();

	int ret = parse(opt, argc, argv);
	if (ret < 0)
	{
		std::cout << "ERROR! Cant Parsing Cmd Line and Config File\n";
		return -1;
	}


	EncoderFactory* encoder_factory = new EncoderFactory();
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
	AbstractVideoEncoder* v_enc = (AbstractVideoEncoder*)encoder_factory->getEncoder(opt, video_encoder_type);
	AbstractAudioEncoder* a_enc = (AbstractAudioEncoder*)encoder_factory->getEncoder(opt, audio_encoder_type);

	MPDGenerator* mpdgen = new MPDGenerator();
	mpdgen->setOutputDir(opt->getValue("dest-directory"));
	mpdgen->setRAPAligned(opt->getValue("rap-aligned"));
	mpdgen->setTemplateUrl(opt->getValue("use-template-url"));
	mpdgen->setFragSize(atoi(opt->getValue("fragment-size")));
	mpdgen->setSegSize(atoi(opt->getValue("segment-size")));
	mpdgen->setMpdName(opt->getValue("mpd-name"));
	mpdgen->setBaseUrl(opt->getValue("base-url"));
	mpdgen->setminBufferTime(atoi(opt->getValue("minBufferTime")));
	v_enc->setGOPSize(v_enc->getFps()*mpdgen->getSegSize() / 1000);

	MP4BoxMultiplexer* muxer = new MP4BoxMultiplexer();
	muxer->setOutputDir(opt->getValue("dest-directory"));
	std::string outdir = muxer->getOutputDir();
	outdir.append(opt->getValue("mpd-name"));
	std::string mk = "mkdir \"";
	mk.append(outdir.substr(0, outdir.find_last_of("/")));
	mk.append("\"");
	system(mk.c_str());

	//video encoding
	std::map<int, std::string> transcoded_video_files;
	TransVideo(opt, v_enc, transcoded_video_files);

	//audio encoding
	std::map<int, std::string> transcoded_audio_files;
	TransAudio(opt, a_enc, transcoded_audio_files);

	//multiplexing
	std::map<int, int> av_mux_mapping;
	std::vector<std::string> muxed_files;
	AVMux(opt, av_mux_mapping, muxer, transcoded_video_files, transcoded_audio_files,muxed_files);
	
	//mpdgenerating
	std::string mpd_path;
	MpdGen(mpdgen, muxed_files, mpd_path);

	std::cout << "\nFINISHED! Success! Mpd Path: "<< mpd_path<<std::endl;

	delete opt;
	delete encoder_factory;
	delete v_enc;
	delete a_enc;
	delete mpdgen;
	delete muxer;
	return 0;
}

int parse(AnyOption *opt, int argc, char* argv[])
{
	/* COMMAND LINE PREFERENCES  */
	opt->setVerbose(); /* print warnings about unknown options */
	opt->autoUsagePrint(true); /* print usage for bad options */

	/* SET THE USAGE/HELP   */
	setHelp(opt);

	/* SET THE OPTION STRINGS/CHARACTERS */
	setCommandOptions(opt);

	if (argc > 1){
		/* PROCESS THE COMMANDLINE */
		opt->processCommandArgs(argc, argv);
		bool show_help = false;
		string config_file;
		show_help = opt->getValue("help");
		if (show_help){
			opt->printUsage();
			return -1;
		}
		else{
			config_file = opt->getValue("input_config");
			setFileOptions(opt);
			/* PROCESS THE RESOURCE FILE */
			if (!opt->processFile(config_file.c_str())){
				std::cout << "ERROR! Cant process input config file\n";
				return -1;
			}
		}
	}
	else{
		opt->printUsage();
		return -1;
	}
}


void setHelp(AnyOption* opt)
{
	opt->addUsage("Usage: ");
	opt->addUsage("");
	opt->addUsage(" -h  Print help ");
	opt->addUsage(" -i  Input config file");
	opt->addUsage("");
}

void setCommandOptions(AnyOption* opt){
	opt->setCommandFlag("help", 'h');
	opt->setCommandOption("input_config", 'i');
}

void setFileOptions(AnyOption* opt)
{
	opt->setFileOption("dest-directory");
	opt->setFileOption("video-encoder");
	opt->setFileOption("audio-encoder");
	opt->setFileOption("multiplexer");
	//Video options
	opt->setFileOption("input");
	opt->setFileOption("input-width");
	opt->setFileOption("input-height");
	opt->setFileOption("bitrate");
	opt->setFileOption("fps");
	opt->setFileOption("profile");
	opt->setFileOption("preset");
	opt->setFileOption("passes");
	opt->setFileOption("video-codec");
	//Audio options
	opt->setFileOption("audio-rep");
	opt->setFileOption("audio-input");
	opt->setFileOption("audio-codec");
	//MP4box options
	//opt->setFileOption("mux-combi");
	//MPD options
	opt->setFileOption("rap-aligned");
	opt->setFileOption("use-template-url");
	opt->setFileOption("mpd-name");
	opt->setFileOption("base-url");
	opt->setFileOption("fragment-size");
	opt->setFileOption("segment-size");
	opt->setFileOption("minBufferTime");
}

IEncoder::EncoderType getEncoder(std::string e){
	if (e.compare("VP8") == 0)
		return IEncoder::VP8;
	else if (e.compare("AAC") == 0)
		return IEncoder::AAC;
	else if (e.compare("H264") == 0)
		return IEncoder::H264;
	else
		return IEncoder::ERROR;
}


int TransAudio(AnyOption* opt, AbstractAudioEncoder* a_enc, std::map<int, std::string> transcoded_audio_files){
	/*
	 *audio-rep : 2,44100,48|2,44100,128	#[channels, samplerate, bitrate], or simply set audio-quality : 2,44100,48
	 */
	std::string audio_reps = " ";
	audio_reps = opt->getValue("audio-rep");
	if (audio_reps == " "){
		std::cout << "ERROR£¡Audio Rep Setting Not Found\n";
		return -1;
	}
	char* c2 = (char*)malloc(audio_reps.length() * sizeof(char));
	strcpy(c2, audio_reps.c_str());
	char* p;
	char* a_rep = strtok_s(c2, "|", &p);
	std::string a_rep_str = " ";

	while (a_rep != NULL)
	{
		a_rep_str = a_rep;
		a_enc->setChannels(atoi(a_rep_str.substr(0, a_rep_str.find(',')).c_str()));

		a_rep_str = a_rep_str.substr(a_rep_str.find(',') + 1);
		a_enc->setSamplingRate(atoi(a_rep_str.substr(0, a_rep_str.find(',')).c_str()));

		a_enc->setBitrate(atoi(a_rep_str.substr(a_rep_str.find(',') + 1).c_str()));

		std::string encoded_audio_file = a_enc->encode();
		if ("-1" == encoded_audio_file)
		{
			std::cout << "ERROR£¡Audio Transcoding Failed\n";
			return -1;
		}

		transcoded_audio_files[a_enc->getBitrate()] = encoded_audio_file;
		a_rep = strtok_s(NULL, "|", &p);
	}

	return 0;
}

int TransVideo(AnyOption* opt, AbstractVideoEncoder* v_enc, std::map<int, std::string> transcoded_video_files){
	/*
	video-rep : 250@480x272|500@640x360			#or simply set video-rep : 200|400|600|1000
	*/
	std::string video_reps  = opt->getValue("video-rep");
	if (video_reps.size() == 0){
		std::cout << "ERROR£¡Video Rep Setting Not Found\n";
		return -1;
	}
	char* c1 = (char*)malloc(video_reps.length() * sizeof(char));
	strcpy(c1, video_reps.c_str());
	char* p;
	char* v_rep = strtok_s(c1, "|", &p);
	std::string v_rep_str = "";

	while (v_rep != NULL)
	{
		/************VIDEO ENCODING *****************************/
		v_rep_str = v_rep;

		if (v_rep_str.find('@') != std::string::npos)
		{
			v_enc->setoWidth(atoi(v_rep_str.substr(v_rep_str.find('@') + 1, v_rep_str.find('x') - v_rep_str.find('@') - 1).c_str()));
			v_enc->setoHeight(atoi(v_rep_str.substr(v_rep_str.find('x') + 1).c_str()));
			v_enc->setBitrate(atoi(v_rep_str.substr(0, v_rep_str.find('@')).c_str()));
		}
		else
		{
			v_enc->setBitrate(atoi(v_rep));
		}

		std::string encoded_video_file = v_enc->encode();
		if ("-1" == encoded_video_file)
		{
			std::cout << "ERROR£¡Video Transcoding Failed\n";
			return -1;
		}

		transcoded_video_files[v_enc->getBitrate()] = encoded_video_file;
		v_rep = strtok_s(NULL, "|", &p);
		v_enc->setoWidth(v_enc->getiWidth());
		v_enc->setoHeight(v_enc->getiHeight());
	}

	return 0;
}

int AVMux(AnyOption *opt, std::map<int, int> av_mux_mapping, MP4BoxMultiplexer* muxer,
	std::map<int, std::string> transcoded_video_files,
	std::map<int, std::string> transcoded_audio_files,
	std::vector<std::string> muxed_files){
	/*
		mux-combi : 250@48|500,1000@128
	*/
		std::string muxconfig = opt->getValue("mux-combi");
		if (muxconfig.size() == 0){
			std::cout << "ERROR£¡Muxing Setting Not Found\n";
			return -1;
		}
		std::string abitrate;
		std::string vbitrates;
		int i = 0;
		do{
			vbitrates = muxconfig.substr(0, muxconfig.find('@'));
			if (muxconfig.find('|') == std::string::npos){
				abitrate = muxconfig.substr(muxconfig.find('@') + 1);
				muxconfig = "";
			}
			else{
				abitrate = muxconfig.substr(muxconfig.find('@') + 1, muxconfig.find('|') - muxconfig.find('@') - 1);
				muxconfig = muxconfig.substr(muxconfig.find('|') + 1);
			}
			do{
				if (vbitrates.find(',') == std::string::npos){
					av_mux_mapping[atoi(vbitrates.c_str())] = atoi(abitrate.c_str());
					std::string muxed_file=muxer->multiplex(transcoded_video_files[atoi(vbitrates.c_str())], transcoded_audio_files[atoi(abitrate.c_str())]);
					if (muxed_file.size() == 0){
						std::cout << "ERROR£¡Muxing Failed\n";
						return -1;
					}
					else
						muxed_files.push_back(muxed_file);
					vbitrates = "";
				}
				else{
					av_mux_mapping[atoi(vbitrates.substr(0, ',').c_str())] = atoi(abitrate.c_str());
					std::string muxed_file=muxer->multiplex(transcoded_video_files[atoi(vbitrates.substr(0, ',').c_str())], transcoded_audio_files[atoi(abitrate.c_str())]);
					if (muxed_file.size() == 0){
						std::cout << "ERROR£¡Muxing Failed\n";
						return -1;
					}
					else
						muxed_files.push_back(muxed_file);
					vbitrates = vbitrates.substr(vbitrates.find(',') + 1);
				}
			} while (vbitrates.size() > 0);
		} while (muxconfig.size() > 0);
	return 0;
}

int MpdGen(MPDGenerator* mpdgen, std::vector<std::string> muxed_files, std::string mpd_path){
	/* MPD Gneration */
	std::cout << "\nWriting final MPD...\n";
	mpd_path = mpdgen->Segment(muxed_files);
	if ("-1" == mpd_path)
	{
		std::cout << "ERROR£¡MPD Generate Failed\n";
		return -1;
	}
	return 0;
}
