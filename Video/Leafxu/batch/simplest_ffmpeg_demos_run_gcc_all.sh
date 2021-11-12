#! /bin/sh
#��򵥵Ļ���FFmpegʾ������ϼ� ---- GCC��������
#simplest ffmpeg demos list ---- GCC run all
#
#������ Lei Xiaohua
#leixiaohua1020@126.com
#�й���ý��ѧ/���ֵ��Ӽ���
#Communication University of China / Digital TV Technology
#http://blog.csdn.net/leixiaohua1020
#
#�ýű�������������[��򵥵Ļ���FFmpeg��ʾ������ϼ�]�ı����Ŀ�ִ�г���
#ʹ��GCC��Ϊ������
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
#This Batch file is used to run all the executable file of 
#[simplest ffmpeg demos list]. It uses GCC as compiler.
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
echo "============================================="
echo "simplest ffmpeg demos list ---- GCC run all"
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
./simplest_ffmpeg_player.out
cd ..
cd simplest_ffmpeg_player_su
./simplest_ffmpeg_player_su.out
cd ..
cd simplest_ffmpeg_decoder_pure
./simplest_ffmpeg_decoder_pure.out
cd ..
cd ..

#=====================================================================================
echo ""
echo "Compile simplest_ffmpeg_audio_player..."
cd simplest_ffmpeg_audio_player
cd simplest_ffmpeg_audio_player
./simplest_ffmpeg_audio_player.out
cd ..
cd ..

#=====================================================================================
echo ""
echo "Compile simplest_ffmpeg_picture_encoder..."
cd simplest_ffmpeg_picture_encoder
cd simplest_ffmpeg_picture_encoder
./simplest_ffmpeg_picture_encoder.out
cd ..
cd ..

#=====================================================================================
echo ""
echo "Compile simplest_ffmpeg_video_encoder..."
cd simplest_ffmpeg_video_encoder
cd simplest_ffmpeg_video_encoder
./simplest_ffmpeg_video_encoder.out
cd ..
cd simplest_ffmpeg_video_encoder_pure
./simplest_ffmpeg_video_encoder_pure.out
cd ..
cd ..

#=====================================================================================
echo ""
echo "Compile simplest_ffmpeg_audio_encoder..."
cd simplest_ffmpeg_audio_encoder
cd simplest_ffmpeg_audio_encoder
./simplest_ffmpeg_audio_encoder.out
cd ..
cd ..

#=====================================================================================
echo ""
echo "Compile simplest_ffmpeg_streamer..."
cd simplest_ffmpeg_streamer
cd simplest_ffmpeg_streamer
./simplest_ffmpeg_streamer.out
cd ..
cd ..

#=====================================================================================
echo ""
echo "Compile simplest_ffmpeg_mem_handler..."
cd simplest_ffmpeg_mem_handler
cd simplest_ffmpeg_mem_player
./simplest_ffmpeg_mem_player.out
cd ..
cd simplest_ffmpeg_mem_transcoder
./simplest_ffmpeg_mem_transcoder.out
cd ..
cd ..

#=====================================================================================
echo ""
echo "Compile simplest_ffmpeg_device..."
cd simplest_ffmpeg_device
cd simplest_ffmpeg_grabdesktop
./simplest_ffmpeg_grabdesktop.out
cd ..
cd simplest_ffmpeg_readcamera
./simplest_ffmpeg_readcamera.out
cd ..
cd ..

#=====================================================================================
echo ""
echo "Compile simplest_ffmpeg_format..."
cd simplest_ffmpeg_format
cd simplest_ffmpeg_demuxer
./simplest_ffmpeg_demuxer.out
cd ..
cd simplest_ffmpeg_demuxer_simple
./simplest_ffmpeg_demuxer_simple.out
cd ..
cd simplest_ffmpeg_muxer
./simplest_ffmpeg_muxer.out
cd ..
cd simplest_ffmpeg_remuxer
./simplest_ffmpeg_remuxer.out
cd ..
cd ..

#=====================================================================================
echo ""
echo "Compile simplest_ffmpeg_video_filter..."
cd simplest_ffmpeg_video_filter
cd simplest_ffmpeg_video_filter
./simplest_ffmpeg_video_filter.out
cd ..
cd ..

#=====================================================================================
echo ""
echo "Compile simplest_ffmpeg_swscale..."
cd simplest_ffmpeg_swscale
cd simplest_ffmpeg_swscale
./simplest_ffmpeg_swscale.out
cd ..
cd simplest_pic_gen
./simplest_pic_gen.out
cd ..
cd ..

#=====================================================================================