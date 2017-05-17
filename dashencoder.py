#! /usr/bin/env python
from optparse import OptionParser
import os
import os.path as path
import sys
import platform

# setup main options
VERSION = "1.0.0"
SVN_REVISION = "$Revision: 539 $"
TempFiles = []
ISOFF_LIVE_PROFILE          = 'urn:mpeg:dash:profile:isoff-live:2011'
ISOFF_ON_DEMAND_PROFILE     = 'urn:mpeg:dash:profile:isoff-on-demand:2011'
HBBTV_15_ISOFF_LIVE_PROFILE = 'urn:hbbtv:dash:profile:isoff-live:2012'

SCRIPT_PATH = path.abspath(path.dirname(__file__))

def PrintErrorAndExit(message):
    sys.stderr.write(message+'\n')
    sys.exit(1)

def main():
    # determine the platform binary name
    host_platform = ''
    if platform.system() == 'Linux':
        if platform.processor() == 'x86_64':
            host_platform = 'linux-x86_64'
        else:
            host_platform = 'linux-x86'
    elif platform.system() == 'Darwin':
        host_platform = 'macosx'
    elif platform.system() == 'Windows':
        host_platform = 'win32'
    default_exec_dir = path.join(SCRIPT_PATH, 'bin', host_platform)
    if not path.exists(default_exec_dir):
        default_exec_dir = path.join(SCRIPT_PATH, 'bin')
    if not path.exists(default_exec_dir):
        default_exec_dir = path.join(SCRIPT_PATH, '..', 'bin')

    # parse options
    global Options
    parser = OptionParser(usage="%prog [options] <media-file>",
                          description="Use this tool to produce dash content from a signle media file, <media-file> is the path to the source video file. Version " + VERSION )
    print "Based on Bento4, written by Hui Zhang(zhanghuicuc@gmail.com)"
    #common options                        
    parser.add_option('-v', '--verbose', dest="verbose", action='store_true', default=False,
                      help="Be verbose")
    parser.add_option('-d', '--debug', dest="debug", action='store_true', default=False,
                      help="Print out debugging information")
    parser.add_option('-f', '--force', dest="force_output", action="store_true",
                      help="Overwrite output files if they already exist", default=False)                  
    #mp4-dash-encode options
    parser.add_option('-k', '--keep-files', dest="keep_files", action='store_true', default=False,
                      help="Keep intermediate files")
    parser.add_option('', '--encfiles-output-dir', dest="encfiles_output_dir",
                      help="Output directory", metavar="<encfiles-output-dir>", default='')
    parser.add_option('-b', '--bitrates', dest="bitrates",
                      help="Video Bitrates (comma-separated list of video bitrates")
    parser.add_option('-r', '--resolutions', dest='resolutions',
                      help="Video Resolutions (comma-separated list of resolutions, set as wxh)")
    parser.add_option('-c', '--video-codec', dest='video_codec', default='libx264',
                      help="Video Codec: libx264 (default) or libx265")
    parser.add_option('-a', '--audio-bitrates', dest='audio_bitrates',
                      help="Audio Bitrates (comma-separated list of audio bitrates")
    parser.add_option('', '--select-streams', dest='select_streams',
                      help="Only encode these streams (comma-separated list of stream indexes or stream specifiers)")
    parser.add_option('-s', '--segment-size', dest='segment_size', type='int',
                      help="Video segment size in frames (default: 3*fps)")
    parser.add_option('-t', '--text-overlay', dest='text_overlay', action='store_true', default=False,
                      help="Add a text overlay with the bitrate")
    parser.add_option('', '--text-overlay-font', dest='text_overlay_font', default=None,
                      help="Specify a TTF font file to use for the text overlay")
    parser.add_option('-e', '--encoder-params', dest='encoder_params',
                      help="Extra encoder parameters")
    #mp4-dash options                 
    parser.add_option('-o', '--output-dir', dest="output_dir",
                      help="Output directory", metavar="<output-dir>", default='output')
    parser.add_option('', '--mpd-name', dest="mpd_filename",
                      help="MPD file name", metavar="<filename>", default='stream.mpd')
    parser.add_option('', '--profiles', dest='profiles',
                      help="Comma-separated list of one or more profile(s). Complete profile names can be used, or profile aliases ('live'='"+ISOFF_LIVE_PROFILE+"', 'on-demand'='"+ISOFF_ON_DEMAND_PROFILE+"', 'hbbtv-1.5='"+HBBTV_15_ISOFF_LIVE_PROFILE+"')", default='live',
                      metavar="<profiles>")
    parser.add_option('', '--no-media', dest="no_media", action='store_true', default=False,
                      help="Do not output media files (MPD/Manifests only)")
    parser.add_option('', '--rename-media', dest='rename_media', action='store_true', default=False,
                      help = 'Use a file name pattern instead of the base name of input files for output media files.')
    parser.add_option('', '--media-prefix', dest='media_prefix', metavar='<prefix>', default='media',
                      help='Use this prefix for prefixed media file names (instead of the default prefix "media")')
    parser.add_option('', '--init-segment', dest="init_segment",
                      help="Initialization segment name", metavar="<filename>", default='init.mp4')
    parser.add_option('', "--no-split", action="store_false", dest="split", default=True,
                      help="Do not split the file into individual segment files")
    parser.add_option('', "--use-segment-list", action="store_true", dest="use_segment_list", default=False,
                      help="Use segment lists instead of segment templates")
    parser.add_option('', '--use-segment-template-number-padding', action='store_true', dest='segment_template_padding', default=False,
                      help="Use padded numbers in segment URL/filename templates")
    parser.add_option('', "--use-segment-timeline", action="store_true", dest="use_segment_timeline", default=False,
                      help="Use segment timelines (necessary if segment durations vary)")
    parser.add_option('', "--min-buffer-time", metavar='<duration>', dest="min_buffer_time", type="float", default=0.0,
                      help="Minimum buffer time (in seconds)")
    parser.add_option('', "--max-playout-rate", metavar='<strategy>', dest='max_playout_rate_strategy',
                      help="Max Playout Rate setting strategy for trick-play support. Supported strategies: lowest:X"),
    parser.add_option('', "--language-map", dest="language_map", metavar="<lang_from>:<lang_to>[,...]",
                      help="Remap language code <lang_from> to <lang_to>. Multiple mappings can be specified, separated by ','")
    parser.add_option('', "--always-output-lang", dest="always_output_lang", action='store_true', default=False,
                      help="Always output an @lang attribute for audio tracks even when the language is undefined"),
    parser.add_option('', "--subtitles", dest="subtitles", action="store_true", default=False,
                      help="Enable Subtitles")
    parser.add_option('', "--attributes", dest="attributes", action="append", metavar='<attributes-definition>', default=[],
                      help="Specify the attributes of a set of tracks. This option may be used multiple times, once per attribute set.")
    parser.add_option('', "--exec-dir", metavar="<exec_dir>", dest="exec_dir", default=default_exec_dir,
                      help="Directory where the Bento4 executables are located")
    
    (options, args) = parser.parse_args()
    Options = options
    if len(args) == 0:
        parser.print_help()
        sys.exit(1)
    
    dash_encode_cmd = 'mp4dashencode.bat'
    dash_cmd = 'mp4dash.bat'
    
    #work with common options
    if options.verbose:
        dash_encode_cmd += ' -v'
        dash_cmd += ' -v'
        
    if options.debug:
        dash_encode_cmd += ' -d'
        dash_cmd += ' -d'
        
    if options.force_output:
        dash_encode_cmd += ' -f'
        dash_cmd += ' -f'   
    
    #work with dash encoder options    
    if options.keep_files:
        dash_encode_cmd += ' -k'
      
    if options.encfiles_output_dir:
        dash_encode_cmd += ' -o ' + str(options.encfiles_output_dir)
        
    if options.bitrates:
        dash_encode_cmd += ' -b ' + str(options.bitrates)
        
    if options.resolutions:
        dash_encode_cmd += ' -r ' + str(options.resolutions)
        
    if options.video_codec:
        dash_encode_cmd += ' -c ' + str(options.video_codec)
        
    if options.audio_bitrates:
        dash_encode_cmd += ' -a ' + str(options.audio_bitrates)
        
    if options.select_streams:
        dash_encode_cmd += ' --select-streams=' + str(options.select_streams)
        
    if options.segment_size:
        dash_encode_cmd += ' -s ' + str(options.segment_size)
        dash_cmd += ' -s ' + str(options.segment_size)
        
    if options.text_overlay:
        dash_encode_cmd += ' -t'
        
    if options.text_overlay_font:
        dash_encode_cmd += ' --text-overlay-font=' + str(options.text_overlay_font)
        
    #if options.encoder_params:
    #    dash_encode_cmd += ' -e ' + str(options.encoder_params)
        
    dash_encode_cmd += ' ' + args[0]
    
    #conduct dash encode process
    print dash_encode_cmd
    os.system(dash_encode_cmd)
    
    #work with dash options
    if options.output_dir:
        dash_cmd += ' -o ' + str(options.output_dir)
        
    if options.mpd_filename:
        dash_cmd += ' --mpd-name=' + str(options.mpd_filename)
        
    if options.profiles:
        dash_cmd += ' --profiles=' + options.profiles
        
    if options.no_media:
        dash_cmd += ' --no-media'
        
    if options.rename_media:
        dash_cmd += ' --rename-media'
        
    if options.media_prefix:
        dash_cmd += ' --media-prefix=' + str(options.media_prefix)
        
    if not options.split:
        dash_cmd += ' --no-split'
    
    if options.init_segment:
        dash_cmd += ' --init-segment=' + str(options.init_segment)
        
    if options.use_segment_list:
        dash_cmd += ' --use-segment-list'
        
    if options.segment_template_padding:
        dash_cmd += ' --use-segment-template-number-padding'
        
    if options.use_segment_timeline:
        dash_cmd += ' --use-segment-timeline'
        
    if options.min_buffer_time:
        dash_cmd += ' --min-buffer-time=' + str(options.min_buffer_time)
    
    if options.max_playout_rate_strategy:
        dash_cmd += ' --max-playout-rate=' + options.max_playout_rate_strategy
        
    if options.language_map:
        dash_cmd += ' --language-map=' + options.language_map
        
    if options.always_output_lang:
        dash_cmd += ' --always-output-lang'
        
    if options.subtitles:
        dash_cmd += ' --subtitles'
        
    if options.attributes:
        dash_cmd += ' --attributes=' + str(options.attributes)
        
    if options.exec_dir:
        dash_cmd += ' --exec-dir=' + str(options.exec_dir)
    
    FileList=[]  
    FileNames=os.listdir(options.encfiles_output_dir)
    if (len(FileNames)>0):  
       for fn in FileNames:     
            fullfilename=os.path.join(options.encfiles_output_dir,fn)  
            dash_cmd += ' ' + fullfilename 
    
    #conduct dash process
    print dash_cmd
    os.system(dash_cmd)
    

###########################    
if __name__ == '__main__':
    global Options
    Options = None
    try:
        main()
    except Exception, err:
        if Options is None or Options.debug:
            raise
        else:
            PrintErrorAndExit('ERROR: %s\n' % str(err))
    finally:
        for f in TempFiles:
            os.unlink(f)