::��򵥵Ļ���FFmpegʾ������ϼ� ---- Devenv��������
::simplest ffmpeg demos list ---- Devenv compile all
::
::������ Lei Xiaohua
::leixiaohua1020@126.com
::�й���ý��ѧ/���ֵ��Ӽ���
::Communication University of China / Digital TV Technology
::http://blog.csdn.net/leixiaohua1020
::
::�ýű�������������[��򵥵Ļ���FFmpeg��ʾ������ϼ�]�Ĵ��롣
::ʹ��Devenv��Ϊ������
::[��򵥵Ļ���FFmpeg��ʾ������ϼ�]Ŀǰ����������Ŀ: 
::-------------------------------------------------------------------------------------
::  *simplest ffmpeg player            | ��򵥵Ļ���FFMPEG����Ƶ������
::  ���������¼����ӹ���: 
::    simplest_ffmpeg_player:          ��׼�棬FFmpegѧϰ�Ŀ�ʼ��
::    simplest_ffmpeg_player_su:       SU��SDL Update���棬�����˼򵥵�SDL��Event��
::    simplest_ffmpeg_decoder_pure:    һ�������Ľ�����ֻʹ��libavcodec��û��ʹ��libavformat����
::-------------------------------------------------------------------------------------
::  *simplest ffmpeg audio player      | ��򵥵Ļ���FFMPEG����Ƶ������
::-------------------------------------------------------------------------------------
::  *simplest ffmpeg picture encoder   | ��򵥵Ļ���FFMPEG��ͼ�������
::-------------------------------------------------------------------------------------
::  *simplest ffmpeg video encoder     | ��򵥵Ļ���FFMPEG����Ƶ������
::  ���������¼����ӹ���: 
::    simplest_ffmpeg_video_encoder:   ��ͨ���������ʹ��libavcodec��libavformat���벢�ҷ�װ��Ƶ��
::    simplest_ffmpeg_video_encoder_pure: ���������������ʹ��libavcodec������Ƶ����ʹ��libavformat��
::-------------------------------------------------------------------------------------
::  *simplest ffmpeg audio encoder     | ��򵥵Ļ���FFMPEG����Ƶ������
::-------------------------------------------------------------------------------------
::  *simplest ffmpeg format            | ��򵥵Ļ���FFMPEG�ķ�װ��ʽ����
::  ���������¼����ӹ���: 
::    simplest_ffmpeg_demuxer:         ����Ƶ��������
::    simplest_ffmpeg_demuxer_simple:  ����Ƶ���������򻯰棩��
::    simplest_ffmpeg_muxer:           ����Ƶ��������
::    simplest_ffmpeg_remuxer:         ��װ��ʽת������
::-------------------------------------------------------------------------------------
::  *simplest ffmpeg streamer          | ��򵥵Ļ���FFmpeg��������������RTMP��
::-------------------------------------------------------------------------------------
::  *simplest ffmpeg video filter      | ��򵥵Ļ���FFmpeg��AVfilter���ӣ�ˮӡ���ӣ�
::-------------------------------------------------------------------------------------
::  *simplest ffmpeg swscale           | ��򵥵Ļ���FFmpeg��libswscale��ʾ��
::  ���������¼����ӹ���: 
::    simplest_ffmpeg_swscale:         ��򵥵�libswscale�Ľ̡̳�
::    simplest_pic_gen:                ���ɸ��ֲ���ͼƬ�Ĺ��ߡ�
::-------------------------------------------------------------------------------------
::  *simplest ffmpeg device            | ��򵥵Ļ���FFmpeg��AVDevice����
::  ���������¼����ӹ���: 
::    simplest_ffmpeg_grabdesktop:     ��Ļ¼�ơ�
::    simplest_ffmpeg_readcamera:      ��ȡ����ͷ��
::-------------------------------------------------------------------------------------
::  *simplest ffmpeg mem handler       | ��򵥵Ļ���FFmpeg���ڴ��д����
::  ���������¼����ӹ���: 
::    simplest_ffmpeg_mem_player:      �����ڴ�����Ƶ���ݵĲ�������
::    simplest_ffmpeg_mem_transcoder:  ת���ڴ������ݵ�ת������
::
::=====================================================================================
::
::
::This Batch file is used to compile all the source code of 
::[simplest ffmpeg demos list]. It uses Devenv as compiler.
::[simplest ffmpeg demos list] contains following projects:
::-------------------------------------------------------------------------------------
::  *simplest ffmpeg player
::  It contains following projects:
::    simplest_ffmpeg_player:          Standard Version, suitable for biginner.
::    simplest_ffmpeg_player_su:       SU��SDL Update��Version, Add SDL Event.
::    simplest_ffmpeg_decoder_pure:    A pure decoder.It only uses libavcodec (without libavformat).
::-------------------------------------------------------------------------------------
::  *simplest ffmpeg audio player
::-------------------------------------------------------------------------------------
::  *simplest ffmpeg picture encoder
::-------------------------------------------------------------------------------------
::  *simplest ffmpeg video encoder
::  It contains following projects:
::    simplest_ffmpeg_video_encoder:   A video encoder. It uses libavcodec and libavformat.
::    simplest_ffmpeg_video_encoder_pure: Pure video encoder. It only uses libavcodec (without libavformat).
::-------------------------------------------------------------------------------------
::  *simplest ffmpeg audio encoder
::-------------------------------------------------------------------------------------
::  *simplest ffmpeg format
::  It contains following projects:
::    simplest_ffmpeg_demuxer:         Split Audio and Video bitstreams.
::    simplest_ffmpeg_demuxer_simple:  Split Audio and Video bitstreams.
::    simplest_ffmpeg_muxer:           Put Audio and Video bitstreams together.
::    simplest_ffmpeg_remuxer:         Change Container Fomat of Video file.
::-------------------------------------------------------------------------------------
::  *simplest ffmpeg streamer
::-------------------------------------------------------------------------------------
::  *simplest ffmpeg video filter
::-------------------------------------------------------------------------------------
::  *simplest ffmpeg swscale
::  It contains following projects:
::    simplest_ffmpeg_swscale:         Convert pixel data's format (from YUV420P to RGB24)
::    simplest_pic_gen:                Generate some test pictures (Gray Bar, Color Bar ...)
::-------------------------------------------------------------------------------------
::  *simplest ffmpeg device
::  It contains following projects:
::    simplest_ffmpeg_grabdesktop:     Screen Capture.
::    simplest_ffmpeg_readcamera:      Read Camera.
::-------------------------------------------------------------------------------------
::  *simplest ffmpeg mem handler
::  It contains following projects:
::    simplest_ffmpeg_mem_player:      Video Player that play video data in memory.
::    simplest_ffmpeg_mem_transcoder:  Video Converter that convert video data in memory.
::=====================================================================================
@echo off
echo ---------------------------------------------
echo simplest ffmpeg demos list ---- Devenv compile all
echo Lei Xiaohua
echo Communication University of China / Digital TV Technology
echo leixiaohua1020@126.com
echo http://blog.csdn.net/leixiaohua1020
echo ---------------------------------------------

set logfile=compile_log.txt
::VS2010 Environment
call "D:\Program Files\Microsoft Visual Studio 10.0\VC\vcvarsall.bat"
::=====================================================================================
echo.
echo Compile simplest_ffmpeg_player...
cd simplest_ffmpeg_player
devenv.exe simplest_ffmpeg_player2.sln /rebuild Debug /Out ../sf_player_debug_%logfile%
devenv.exe simplest_ffmpeg_player2.sln /rebuild Release /Out ../sf_player_release_%logfile%
cd ..

::=====================================================================================
echo.
echo Compile simplest_ffmpeg_audio_player...
cd simplest_ffmpeg_audio_player
devenv.exe simplest_ffmpeg_audio_player2.sln /rebuild Debug /Out ../sf_audio_player_debug_%logfile%
devenv.exe simplest_ffmpeg_audio_player2.sln /rebuild Release /Out ../sf_audio_player_release_%logfile%
cd ..

::=====================================================================================
echo.
echo Compile simplest_ffmpeg_picture_encoder...
cd simplest_ffmpeg_picture_encoder
devenv.exe simplest_ffmpeg_picture_encoder.sln /rebuild Debug /Out ../sf_picture_encoder_debug_%logfile%
devenv.exe simplest_ffmpeg_picture_encoder.sln /rebuild Release /Out ../sf_picture_encoder_release_%logfile%
cd ..

::=====================================================================================
echo.
echo Compile simplest_ffmpeg_video_encoder...
cd simplest_ffmpeg_video_encoder
devenv.exe simplest_ffmpeg_video_encoder.sln /rebuild Debug /Out ../sf_video_encoder_debug_%logfile%
devenv.exe simplest_ffmpeg_video_encoder.sln /rebuild Release /Out ../sf_video_encoder_release_%logfile%
cd ..

::=====================================================================================
echo.
echo Compile simplest_ffmpeg_audio_encoder...
cd simplest_ffmpeg_audio_encoder
devenv.exe simplest_ffmpeg_audio_encoder.sln /rebuild Debug /Out ../sf_audio_encoder_debug_%logfile%
devenv.exe simplest_ffmpeg_audio_encoder.sln /rebuild Release /Out ../sf_audio_encoder_release_%logfile%
cd ..

::=====================================================================================
echo.
echo Compile simplest_ffmpeg_streamer...
cd simplest_ffmpeg_streamer
devenv.exe simplest_ffmpeg_streamer.sln /rebuild Debug /Out ../sf_streamer_debug_%logfile%
devenv.exe simplest_ffmpeg_streamer.sln /rebuild Release /Out ../sf_streamer_release_%logfile%
cd ..

::=====================================================================================
echo.
echo Compile simplest_ffmpeg_mem_handler...
cd simplest_ffmpeg_mem_handler
devenv.exe simplest_ffmpeg_mem_handler.sln /rebuild Debug /Out ../sf_mem_handler_debug_%logfile%
devenv.exe simplest_ffmpeg_mem_handler.sln /rebuild Release /Out ../sf_mem_handler_release_%logfile%
cd ..

::=====================================================================================
echo.
echo Compile simplest_ffmpeg_device...
cd simplest_ffmpeg_device
devenv.exe simplest_ffmpeg_device.sln /rebuild Debug /Out ../sf_device_debug_%logfile%
devenv.exe simplest_ffmpeg_device.sln /rebuild Release /Out ../sf_device_release_%logfile%
cd ..

::=====================================================================================
echo.
echo Compile simplest_ffmpeg_format...
cd simplest_ffmpeg_format
devenv.exe simplest_ffmpeg_format.sln /rebuild Debug /Out ../sf_format_debug_%logfile%
devenv.exe simplest_ffmpeg_format.sln /rebuild Release /Out ../sf_format_release_%logfile%
cd ..

::=====================================================================================
echo.
echo Compile simplest_ffmpeg_video_filter...
cd simplest_ffmpeg_video_filter
devenv.exe simplest_ffmpeg_video_filter.sln /rebuild Debug /Out ../sf_video_filter_debug_%logfile%
devenv.exe simplest_ffmpeg_video_filter.sln /rebuild Release /Out ../sf_video_filter_release_%logfile%
cd ..

::=====================================================================================
echo.
echo Compile simplest_ffmpeg_swscale...
cd simplest_ffmpeg_swscale
devenv.exe simplest_ffmpeg_swscale.sln /rebuild Debug /Out ../sf_swscale_debug_%logfile%
devenv.exe simplest_ffmpeg_swscale.sln /rebuild Release /Out ../sf_swscale_release_%logfile%
cd ..

::=====================================================================================