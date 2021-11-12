#! /bin/sh
#��򵥵Ļ���FFmpegʾ������ϼ� ---- MinGW��������
#simplest ffmpeg demos list ---- MinGW compile all
#
#������ Lei Xiaohua
#leixiaohua1020@126.com
#�й���ý��ѧ/���ֵ��Ӽ���
#Communication University of China / Digital TV Technology
#http://blog.csdn.net/leixiaohua1020
#
#�ýű�������������[��򵥵Ļ���FFmpeg��ʾ������ϼ�]�Ĵ��롣
#ʹ��MinGW��g++��Ϊ������
#[��򵥵Ļ���FFmpeg��ʾ������ϼ�]Ŀǰ����������Ŀ: 
#-------------------------------------------------------------------------------------
#  *simplest ffmpeg player            | ��򵥵Ļ���FFMPEG����Ƶ������
#  ���������¼����ӹ���: 
#    simplest_ffmpeg_player:          ��׼�棬FFmpegѧϰ�Ŀ�ʼ��
#    simplest_ffmpeg_player_su:       SU��SDL Update���棬�����˼򵥵�SDL��Event��
#    simplest_ffmpeg_decoder_pure:    һ�������Ľ�����ֻʹ��libavcodec��û��ʹ��libavformat����
#-------------------------------------------------------------------------------------
#  *simplest ffmpeg audio player      | ��򵥵Ļ���FFMPEG����Ƶ������
#-------------------------------------------------------------------------------------
#  *simplest ffmpeg picture encoder   | ��򵥵Ļ���FFMPEG��ͼ�������
#-------------------------------------------------------------------------------------
#  *simplest ffmpeg video encoder     | ��򵥵Ļ���FFMPEG����Ƶ������
#  ���������¼����ӹ���: 
#    simplest_ffmpeg_video_encoder:   ��ͨ���������ʹ��libavcodec��libavformat���벢�ҷ�װ��Ƶ��
#    simplest_ffmpeg_video_encoder_pure: ���������������ʹ��libavcodec������Ƶ����ʹ��libavformat��
#-------------------------------------------------------------------------------------
#  *simplest ffmpeg audio encoder     | ��򵥵Ļ���FFMPEG����Ƶ������
#-------------------------------------------------------------------------------------
#  *simplest ffmpeg format            | ��򵥵Ļ���FFMPEG�ķ�װ��ʽ����
#  ���������¼����ӹ���: 
#    simplest_ffmpeg_demuxer:         ����Ƶ��������
#    simplest_ffmpeg_demuxer_simple:  ����Ƶ���������򻯰棩��
#    simplest_ffmpeg_muxer:           ����Ƶ��������
#    simplest_ffmpeg_remuxer:         ��װ��ʽת������
#-------------------------------------------------------------------------------------
#  *simplest ffmpeg streamer          | ��򵥵Ļ���FFmpeg��������������RTMP��
#-------------------------------------------------------------------------------------
#  *simplest ffmpeg video filter      | ��򵥵Ļ���FFmpeg��AVfilter���ӣ�ˮӡ���ӣ�
#-------------------------------------------------------------------------------------
#  *simplest ffmpeg swscale           | ��򵥵Ļ���FFmpeg��libswscale��ʾ��
#  ���������¼����ӹ���: 
#    simplest_ffmpeg_swscale:         ��򵥵�libswscale�Ľ̡̳�
#    simplest_pic_gen:                ���ɸ��ֲ���ͼƬ�Ĺ��ߡ�
#-------------------------------------------------------------------------------------
#  *simplest ffmpeg device            | ��򵥵Ļ���FFmpeg��AVDevice����
#  ���������¼����ӹ���: 
#    simplest_ffmpeg_grabdesktop:     ��Ļ¼�ơ�
#    simplest_ffmpeg_readcamera:      ��ȡ����ͷ��
#-------------------------------------------------------------------------------------
#  *simplest ffmpeg mem handler       | ��򵥵Ļ���FFmpeg���ڴ��д����
#  ���������¼����ӹ���: 
#    simplest_ffmpeg_mem_player:      �����ڴ�����Ƶ���ݵĲ�������
#    simplest_ffmpeg_mem_transcoder:  ת���ڴ������ݵ�ת������
#
#=====================================================================================
#
#
#This Batch file is used to compile all the source code of 
#[simplest ffmpeg demos list]. It uses MinGW's g++ as compiler.
#[simplest ffmpeg demos list] contains following projects:
#-------------------------------------------------------------------------------------
#  *simplest ffmpeg player
#  It contains following projects:
#    simplest_ffmpeg_player:          Standard Version, suitable for biginner.
#    simplest_ffmpeg_player_su:       SU��SDL Update��Version, Add SDL Event.
#    simplest_ffmpeg_decoder_pure:    A pure decoder. It only uses libavcodec (without libavformat).
#-------------------------------------------------------------------------------------
#  *simplest ffmpeg audio player
#-------------------------------------------------------------------------------------
#  *simplest ffmpeg picture encoder
#-------------------------------------------------------------------------------------
#  *simplest ffmpeg video encoder
#  It contains following projects:
#    simplest_ffmpeg_video_encoder:   A video encoder. It uses libavcodec and libavformat.
#    simplest_ffmpeg_video_encoder_pure: Pure video encoder. It only uses libavcodec (without libavformat).
#-------------------------------------------------------------------------------------
#  *simplest ffmpeg audio encoder
#-------------------------------------------------------------------------------------
#  *simplest ffmpeg format
#  It contains following projects:
#    simplest_ffmpeg_demuxer:         Split Audio and Video bitstreams.
#    simplest_ffmpeg_demuxer_simple:  Split Audio and Video bitstreams.
#    simplest_ffmpeg_muxer:           Put Audio and Video bitstreams together.
#    simplest_ffmpeg_remuxer:         Change Container Fomat of Video file.
#-------------------------------------------------------------------------------------
#  *simplest ffmpeg streamer
#-------------------------------------------------------------------------------------
#  *simplest ffmpeg video filter
#-------------------------------------------------------------------------------------
#  *simplest ffmpeg swscale
#  It contains following projects:
#    simplest_ffmpeg_swscale:         Convert pixel data's format (from YUV420P to RGB24)
#    simplest_pic_gen:                Generate some test pictures (Gray Bar, Color Bar ...)
#-------------------------------------------------------------------------------------
#  *simplest ffmpeg device
#  It contains following projects:
#    simplest_ffmpeg_grabdesktop:     Screen Capture.
#    simplest_ffmpeg_readcamera:      Read Camera.
#-------------------------------------------------------------------------------------
#  *simplest ffmpeg mem handler
#  It contains following projects:
#    simplest_ffmpeg_mem_player:      Video Player that play video data in memory.
#    simplest_ffmpeg_mem_transcoder:  Video Converter that convert video data in memory.
#=====================================================================================
# Get FFmpeg's Lib in MinGW
# Input following command in Command Line Window:
#
# ~~~~~~~~~~~~~~~~~~[Compile]~~~~~~~~~~~~~~~~~~
# #(0)Compile FFmpeg Dependencies
# #Install yasm
# #Visit yasm official site:http://yasm.tortall.net/, download the yasmXXXX.exe
# #Change "yasmXXXX.exe" name to "yasm.exe"
# #Copy yasm.exe to [MinGW Install Folder]/bin/
# #Install SDL
# #Visit SDL official site: http://www.libsdl.org/, download source code
# #cd to source code folder
# ./configure
# make
# make install
# #Compile libx264
# #Visit x264 official site: http://www.videolan.org/developers/x264.html, download source code
# #cd to source code folder
# ./configure
# make
# make install
# #Compile libfaac 
# #Visit libfaac official site: http://www.audiocoding.com/faac.html, download source code
# #cd to source code folder
# ./configure
# make
# make install
#
# #(1)Compile FFmpeg
# #Visit ffmpeg official site: http://ffmpeg.org/, download source code.
# #or use Git:
# git clone git://source.ffmpeg.org/ffmpeg.git ffmpeg
# cd ffmpeg
# ./configure --enable-shared --enable-libfaac --enable-libx264 --enable-gpl --enable-nonfree
# make
# make install
#
# #(2)Compile SDL2 (Some projects needed)
# #Visit SDL official site:http://www.libsdl.org/, download source code
# #cd to source code folder
# ./configure
# make
# make install
#
# ~~~~~~~~~~~~~~[Install Directly]~~~~~~~~~~~~~~
# #Visit ffmpeg Windows Build site: http://ffmpeg.zeranoe.com/, download windows shared/dev version.
# #Create Folder named "local" in [Msys Install Folder]
# #(0)Include
# #copy include folder in dev version to [Msys Install Folder]/local/
# #(1)Lib
# #copy lib folder in dev version to [Msys Install Folder]/local/
# #(2)Dll
# #copy all the file (*.dll and *.exe) in shared version to [MinGW Install Folder]/bin/
# #
echo "============================================="
echo "simplest ffmpeg demos list ---- MinGW compile all"
echo "Lei Xiaohua"
echo "Communication University of China / Digital TV Technology"
echo "leixiaohua1020@126.com"
echo "http://blog.csdn.net/leixiaohua1020"
echo "============================================="
#=====================================================================================

echo ""
echo "Compile simplest_ffmpeg_player..."
cd simplest_ffmpeg_player
cd simplest_ffmpeg_player
sh compile_mingw.sh
cd ..
cd simplest_ffmpeg_player_su
sh compile_mingw.sh
cd ..
cd simplest_ffmpeg_decoder_pure
sh compile_mingw.sh
cd ..
cd ..

#=====================================================================================
echo ""
echo "Compile simplest_ffmpeg_audio_player..."
cd simplest_ffmpeg_audio_player
cd simplest_ffmpeg_audio_player
sh compile_mingw.sh
cd ..
cd ..

#=====================================================================================
echo ""
echo "Compile simplest_ffmpeg_picture_encoder..."
cd simplest_ffmpeg_picture_encoder
cd simplest_ffmpeg_picture_encoder
sh compile_mingw.sh
cd ..
cd ..

#=====================================================================================
echo ""
echo "Compile simplest_ffmpeg_video_encoder..."
cd simplest_ffmpeg_video_encoder
cd simplest_ffmpeg_video_encoder
sh compile_mingw.sh
cd ..
cd simplest_ffmpeg_video_encoder_pure
sh compile_mingw.sh
cd ..
cd ..

#=====================================================================================
echo ""
echo "Compile simplest_ffmpeg_audio_encoder..."
cd simplest_ffmpeg_audio_encoder
cd simplest_ffmpeg_audio_encoder
sh compile_mingw.sh
cd ..
cd ..

#=====================================================================================
echo ""
echo "Compile simplest_ffmpeg_streamer..."
cd simplest_ffmpeg_streamer
cd simplest_ffmpeg_streamer
sh compile_mingw.sh
cd ..
cd ..

#=====================================================================================
echo ""
echo "Compile simplest_ffmpeg_mem_handler..."
cd simplest_ffmpeg_mem_handler
cd simplest_ffmpeg_mem_player
sh compile_mingw.sh
cd ..
cd simplest_ffmpeg_mem_transcoder
sh compile_mingw.sh
cd ..
cd ..

#=====================================================================================
echo ""
echo "Compile simplest_ffmpeg_device..."
cd simplest_ffmpeg_device
cd simplest_ffmpeg_grabdesktop
sh compile_mingw.sh
cd ..
cd simplest_ffmpeg_readcamera
sh compile_mingw.sh
cd ..
cd ..

#=====================================================================================
echo ""
echo "Compile simplest_ffmpeg_format..."
cd simplest_ffmpeg_format
cd simplest_ffmpeg_demuxer
sh compile_mingw.sh
cd ..
cd simplest_ffmpeg_demuxer_simple
sh compile_mingw.sh
cd ..
cd simplest_ffmpeg_muxer
sh compile_mingw.sh
cd ..
cd simplest_ffmpeg_remuxer
sh compile_mingw.sh
cd ..
cd ..

#=====================================================================================
echo ""
echo "Compile simplest_ffmpeg_video_filter..."
cd simplest_ffmpeg_video_filter
cd simplest_ffmpeg_video_filter
sh compile_mingw.sh
cd ..
cd ..

#=====================================================================================
echo ""
echo "Compile simplest_ffmpeg_swscale..."
cd simplest_ffmpeg_swscale
cd simplest_ffmpeg_swscale
sh compile_mingw.sh
cd ..
cd simplest_pic_gen
sh compile_mingw.sh
cd ..
cd ..

#=====================================================================================