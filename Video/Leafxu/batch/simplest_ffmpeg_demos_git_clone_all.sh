#! /bin/sh
#��򵥵Ļ���FFmpegʾ������ϼ� ---- Git������ȡ�ű�
#simplest ffmpeg demos list ---- git clone all
#
#������ Lei Xiaohua
#leixiaohua1020@126.com
#�й���ý��ѧ/���ֵ��Ӽ���
#Communication University of China / Digital TV Technology
#http://blog.csdn.net/leixiaohua1020
#
#�ýű�����������ȡ[��򵥵Ļ���FFmpeg��ʾ������ϼ�]�Ĵ��롣
#[��򵥵Ļ���FFmpeg��ʾ������ϼ�]Ŀǰ����������Ŀ��
#  simplest ffmpeg player:          ��򵥵Ļ���FFmpeg����Ƶ������
#  simplest ffmpeg audio player:    ��򵥵Ļ���FFmpeg����Ƶ������
#  simplest ffmpeg pic encoder:     ��򵥵Ļ���FFmpeg��ͼ�������
#  simplest ffmpeg video encoder:   ��򵥵Ļ���FFmpeg����Ƶ������
#  simplest ffmpeg audio encoder:   ��򵥵Ļ���FFmpeg����Ƶ������
#  simplest ffmpeg streamer:        ��򵥵Ļ���FFmpeg��������������RTMP��
#  simplest ffmpeg mem handler:     ��򵥵Ļ���FFmpeg���ڴ��д����
#  simplest ffmpeg device:          ��򵥵Ļ���FFmpeg��AVDevice����
#  simplest ffmpeg format:          ��򵥵Ļ���FFmpeg�ķ�װ��ʽ���������
#  simplest ffmpeg video filter:    ��򵥵Ļ���FFmpeg��AVfilter���ӣ�ˮӡ���ӣ�
#  simplest ffmpeg swscale:         ��򵥵Ļ���FFmpeg��libswscale��ʾ��
#
#This Batch file is used to get all the source code of 
#[simplest ffmpeg demos list].
#[simplest ffmpeg demos list] contains following projects:
#  simplest ffmpeg player:          The simplest video player based on FFmpeg
#  simplest ffmpeg audio player:    The simplest audio player based on FFmpeg
#  simplest ffmpeg pic encoder:     The simplest picture encoder based on FFmpeg
#  simplest ffmpeg video encoder:   The simplest video encoder based on FFmpeg
#  simplest ffmpeg audio encoder:   The simplest audio encoder based on FFmpeg
#  simplest ffmpeg streamer:        The simplest streamer based on FFmpeg
#  simplest ffmpeg mem handler:     FFmpeg's example about read/write from memory
#  simplest ffmpeg device:          FFmpeg's libavdevice examples
#  simplest ffmpeg format:          FFmpeg's mux/demux/remux examples
#  simplest ffmpeg video filter:    use FFmpeg's AVFilter to add watermark in a video file
#  simplest ffmpeg swscale:         Examples about handle pixel data using libswscale in FFmpeg

echo "============================================="
echo "simplest ffmpeg demos list ---- git clone all"
echo "Lei Xiaohua"
echo "Communication University of China / Digital TV Technology"
echo "leixiaohua1020@126.com"
echo "http://blog.csdn.net/leixiaohua1020"
echo "============================================="

originstr="github"
#3 kinds of origins:
githubstr="github"
sourceforgestr="sourceforge"
oscstr="osc"

#Git Clone
#Github
#http://leixiaohua1020.github.com/
if [ "$originstr" = "$githubstr" ]
then
git clone https://github.com/leixiaohua1020/simplest_ffmpeg_player.git simplest_ffmpeg_player
git clone https://github.com/leixiaohua1020/simplest_ffmpeg_audio_player.git simplest_ffmpeg_audio_player
git clone https://github.com/leixiaohua1020/simplest_ffmpeg_picture_encoder.git simplest_ffmpeg_picture_encoder
git clone https://github.com/leixiaohua1020/simplest_ffmpeg_video_encoder.git simplest_ffmpeg_video_encoder
git clone https://github.com/leixiaohua1020/simplest_ffmpeg_audio_encoder.git simplest_ffmpeg_audio_encoder
git clone https://github.com/leixiaohua1020/simplest_ffmpeg_streamer.git simplest_ffmpeg_streamer
git clone https://github.com/leixiaohua1020/simplest_ffmpeg_mem_handler.git simplest_ffmpeg_mem_handler
git clone https://github.com/leixiaohua1020/simplest_ffmpeg_device.git simplest_ffmpeg_device
git clone https://github.com/leixiaohua1020/simplest_ffmpeg_format.git simplest_ffmpeg_format
git clone https://github.com/leixiaohua1020/simplest_ffmpeg_video_filter.git simplest_ffmpeg_video_filter
git clone https://github.com/leixiaohua1020/simplest_ffmpeg_swscale.git simplest_ffmpeg_swscale
#SourceForge
#https://sourceforge.net/u/leixiaohua1020/wiki/
elif [ "$originstr" = "$sourceforgestr" ]
then
git clone git://git.code.sf.net/p/simplestffmpegplayer/code simplest_ffmpeg_player
git clone git://git.code.sf.net/p/simplestffmpegaudioplayer/code simplest_ffmpeg_audio_player
git clone git://git.code.sf.net/p/simplestffmpegpictureencoder/code simplest_ffmpeg_picture_encoder
git clone git://git.code.sf.net/p/simplestffmpegvideoencoder/code simplest_ffmpeg_video_encoder
git clone git://git.code.sf.net/p/simplestffmpegaudioencoder/code simplest_ffmpeg_audio_encoder
git clone git://git.code.sf.net/p/simplestffmpegstreamer/code simplest_ffmpeg_streamer
git clone git://git.code.sf.net/p/simplestffmpegmemhandler/code simplest_ffmpeg_mem_handler
git clone git://git.code.sf.net/p/simplestffmpegdevice/code simplest_ffmpeg_device
git clone git://git.code.sf.net/p/simplestffmpegformat/code simplest_ffmpeg_format
git clone git://git.code.sf.net/p/simplestffmpegvideofilter/code simplest_ffmpeg_video_filter
git clone git://git.code.sf.net/p/simplestffmpegswscale/code simplest_ffmpeg_swscale
#OSC
#http://git.oschina.net/leixiaohua1020
else
git clone https://git.oschina.net/leixiaohua1020/simplest_ffmpeg_player.git simplest_ffmpeg_player
git clone https://git.oschina.net/leixiaohua1020/simplest_ffmpeg_audio_player.git simplest_ffmpeg_audio_player
git clone https://git.oschina.net/leixiaohua1020/simplest_ffmpeg_picture_encoder.git simplest_ffmpeg_picture_encoder
git clone https://git.oschina.net/leixiaohua1020/simplest_ffmpeg_video_encoder.git simplest_ffmpeg_video_encoder
git clone https://git.oschina.net/leixiaohua1020/simplest_ffmpeg_audio_encoder.git simplest_ffmpeg_audio_encoder
git clone https://git.oschina.net/leixiaohua1020/simplest_ffmpeg_streamer.git simplest_ffmpeg_streamer
git clone https://git.oschina.net/leixiaohua1020/simplest_ffmpeg_mem_handler.git simplest_ffmpeg_mem_handler
git clone https://git.oschina.net/leixiaohua1020/simplest_ffmpeg_device.git simplest_ffmpeg_device
git clone https://git.oschina.net/leixiaohua1020/simplest_ffmpeg_format.git simplest_ffmpeg_format
git clone https://git.oschina.net/leixiaohua1020/simplest_ffmpeg_video_filter.git simplest_ffmpeg_video_filter
git clone https://git.oschina.net/leixiaohua1020/simplest_ffmpeg_swscale.git simplest_ffmpeg_swscale
fi

#set remote
cd simplest_ffmpeg_player
git remote rm origin
git remote add origin git://git.code.sf.net/p/simplestffmpegplayer/code
git remote add github https://github.com/leixiaohua1020/simplest_ffmpeg_player.git
git remote add osc https://git.oschina.net/leixiaohua1020/simplest_ffmpeg_player.git
cd ..

cd simplest_ffmpeg_audio_player
git remote rm origin
git remote add origin git://git.code.sf.net/p/simplestffmpegaudioplayer/code
git remote add github https://github.com/leixiaohua1020/simplest_ffmpeg_audio_player.git
git remote add osc https://git.oschina.net/leixiaohua1020/simplest_ffmpeg_audio_player.git
cd ..

cd simplest_ffmpeg_picture_encoder
git remote rm origin
git remote add origin git://git.code.sf.net/p/simplestffmpegpictureencoder/code
git remote add github https://github.com/leixiaohua1020/simplest_ffmpeg_picture_encoder.git
git remote add osc https://git.oschina.net/leixiaohua1020/simplest_ffmpeg_picture_encoder.git
cd ..

cd simplest_ffmpeg_video_encoder
git remote rm origin
git remote add origin git://git.code.sf.net/p/simplestffmpegvideoencoder/code
git remote add github https://github.com/leixiaohua1020/simplest_ffmpeg_video_encoder.git
git remote add osc https://git.oschina.net/leixiaohua1020/simplest_ffmpeg_video_encoder.git
cd ..

cd simplest_ffmpeg_audio_encoder
git remote rm origin
git remote add origin git://git.code.sf.net/p/simplestffmpegaudioencoder/code
git remote add github https://github.com/leixiaohua1020/simplest_ffmpeg_audio_encoder.git
git remote add osc https://git.oschina.net/leixiaohua1020/simplest_ffmpeg_audio_encoder.git
cd ..

cd simplest_ffmpeg_streamer
git remote rm origin
git remote add origin git://git.code.sf.net/p/simplestffmpegstreamer/code
git remote add github https://github.com/leixiaohua1020/simplest_ffmpeg_streamer.git
git remote add osc https://git.oschina.net/leixiaohua1020/simplest_ffmpeg_streamer.git
cd ..

cd simplest_ffmpeg_mem_handler
git remote rm origin
git remote add origin git://git.code.sf.net/p/simplestffmpegmemhandler/code
git remote add github https://github.com/leixiaohua1020/simplest_ffmpeg_mem_handler.git
git remote add osc https://git.oschina.net/leixiaohua1020/simplest_ffmpeg_mem_handler.git
cd ..

cd simplest_ffmpeg_device
git remote rm origin
git remote add origin git://git.code.sf.net/p/simplestffmpegdevice/code
git remote add github https://github.com/leixiaohua1020/simplest_ffmpeg_device.git
git remote add osc https://git.oschina.net/leixiaohua1020/simplest_ffmpeg_device.git
cd ..

cd simplest_ffmpeg_format
git remote rm origin
git remote add origin git://git.code.sf.net/p/simplestffmpegformat/code
git remote add github https://github.com/leixiaohua1020/simplest_ffmpeg_format.git
git remote add osc https://git.oschina.net/leixiaohua1020/simplest_ffmpeg_format.git
cd ..

cd simplest_ffmpeg_video_filter
git remote rm origin
git remote add origin git://git.code.sf.net/p/simplestffmpegvideofilter/code
git remote add github https://github.com/leixiaohua1020/simplest_ffmpeg_video_filter.git
git remote add osc https://git.oschina.net/leixiaohua1020/simplest_ffmpeg_video_filter.git
cd ..

cd simplest_ffmpeg_swscale
git remote rm origin
git remote add origin git://git.code.sf.net/p/simplestffmpegswscale/code
git remote add github https://github.com/leixiaohua1020/simplest_ffmpeg_swscale.git
git remote add osc https://git.oschina.net/leixiaohua1020/simplest_ffmpeg_swscale.git
cd ..