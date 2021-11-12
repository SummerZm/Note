::��򵥵Ļ���FFmpegʾ������ϼ� ---- CL��������
::simplest ffmpeg demos list ---- CL compile all
::
::������ Lei Xiaohua
::leixiaohua1020@126.com
::�й���ý��ѧ/���ֵ��Ӽ���
::Communication University of China / Digital TV Technology
::http://blog.csdn.net/leixiaohua1020
::
::�ýű�ʹ��cl.exe��������[��ý����Ŀʾ������ϼ�]�Ĵ��롣
::[��ý����Ŀʾ������ϼ�]Ŀǰ����������Ŀ��
::  simplest media play:          ��򵥵�ʹ�ø���API��Direct3D��OpenGL��GDI��
::                                DirectSound��SDL2�����Ŷ�ý�������
::  simplest encoder:             ��򵥵���Ƶ����������
::  simplest libRTMP example:     ��򵥵Ļ���libRTMP��ʹ��ʾ��
::  simplest libVLC example:      ��򵥵Ļ���libVLC��ʾ������
::  simplest directshow example:  ��򵥵Ļ���DirectShow��ʾ��
::
::This Batch file uses CL.exe to compile all the source code of 
::[multimedia projects examples list].
::[multimedia projects examples list] contains following projects:
::  simplest media play:          Examples about video/audio's playing
::  simplest encoder:             Examples about video encoders
::  simplest libRTMP example:     Examples about usage of libRTMP
::  simplest libVLC example:      Examples about libVLC
::  simplest directshow example:  Simple Examples about DirectShow

@echo off
echo ---------------------------------------------
echo simplest ffmpeg demos list ---- CL compile all
echo Lei Xiaohua
echo Communication University of China / Digital TV Technology
echo leixiaohua1020@126.com
echo http://blog.csdn.net/leixiaohua1020
echo ---------------------------------------------
::=====================================================================================

echo.
echo Compile simplest_ffmpeg_player...
cd simplest_ffmpeg_player
cd simplest_ffmpeg_player
start /wait compile_cl.bat
cd ..
cd simplest_ffmpeg_player_su
start /wait compile_cl.bat
cd ..
cd simplest_ffmpeg_decoder_pure
start /wait compile_cl.bat
cd ..
cd ..

::=====================================================================================
echo.
echo Compile simplest_ffmpeg_audio_player...
cd simplest_ffmpeg_audio_player
cd simplest_ffmpeg_audio_player
start /wait compile_cl.bat
cd ..
cd ..

::=====================================================================================
echo.
echo Compile simplest_ffmpeg_picture_encoder...
cd simplest_ffmpeg_picture_encoder
cd simplest_ffmpeg_picture_encoder
start /wait compile_cl.bat
cd ..
cd ..

::=====================================================================================
echo.
echo Compile simplest_ffmpeg_video_encoder...
cd simplest_ffmpeg_video_encoder
cd simplest_ffmpeg_video_encoder
start /wait compile_cl.bat
cd ..
cd simplest_ffmpeg_video_encoder_pure
start /wait compile_cl.bat
cd ..
cd ..

::=====================================================================================
echo.
echo Compile simplest_ffmpeg_audio_encoder...
cd simplest_ffmpeg_audio_encoder
cd simplest_ffmpeg_audio_encoder
start /wait compile_cl.bat
cd ..
cd ..

::=====================================================================================
echo.
echo Compile simplest_ffmpeg_streamer...
cd simplest_ffmpeg_streamer
cd simplest_ffmpeg_streamer
start /wait compile_cl.bat
cd ..
cd ..

::=====================================================================================
echo.
echo Compile simplest_ffmpeg_mem_handler...
cd simplest_ffmpeg_mem_handler
cd simplest_ffmpeg_mem_player
start /wait compile_cl.bat
cd ..
cd simplest_ffmpeg_mem_transcoder
start /wait compile_cl.bat
cd ..
cd ..

::=====================================================================================
echo.
echo Compile simplest_ffmpeg_device...
cd simplest_ffmpeg_device
cd simplest_ffmpeg_grabdesktop
start /wait compile_cl.bat
cd ..
cd simplest_ffmpeg_readcamera
start /wait compile_cl.bat
cd ..
cd ..

::=====================================================================================
echo.
echo Compile simplest_ffmpeg_format...
cd simplest_ffmpeg_format
cd simplest_ffmpeg_demuxer
start /wait compile_cl.bat
cd ..
cd simplest_ffmpeg_demuxer_simple
start /wait compile_cl.bat
cd ..
cd simplest_ffmpeg_muxer
start /wait compile_cl.bat
cd ..
cd simplest_ffmpeg_remuxer
start /wait compile_cl.bat
cd ..
cd ..

::=====================================================================================
echo.
echo Compile simplest_ffmpeg_video_filter...
cd simplest_ffmpeg_video_filter
cd simplest_ffmpeg_video_filter
start /wait compile_cl.bat
cd ..
cd ..

::=====================================================================================
echo.
echo Compile simplest_ffmpeg_swscale...
cd simplest_ffmpeg_swscale
cd simplest_ffmpeg_swscale
start /wait compile_cl.bat
cd ..
cd simplest_pic_gen
start /wait compile_cl.bat
cd ..
cd ..

::=====================================================================================