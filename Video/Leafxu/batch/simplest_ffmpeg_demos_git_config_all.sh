#! /bin/sh
#��򵥵Ļ���FFmpegʾ������ϼ� ---- Git�������ýű�
#simplest ffmpeg demos list ---- git config all
#
#������ Lei Xiaohua
#leixiaohua1020@126.com
#�й���ý��ѧ/���ֵ��Ӽ���
#Communication University of China / Digital TV Technology
#http://blog.csdn.net/leixiaohua1020
#
#�ýű�������������[��򵥵Ļ���FFmpeg��ʾ������ϼ�]�Ĵ����һЩ���ԡ�
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
#This Batch file is used to set some attributes of all the source code of 
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
echo "simplest ffmpeg demos list ---- git config all"
echo "Lei Xiaohua"
echo "Communication University of China / Digital TV Technology"
echo "leixiaohua1020@126.com"
echo "http://blog.csdn.net/leixiaohua1020"
echo "============================================="


cmd1="git config core.ignorecase false"

cd simplest_ffmpeg_player
$cmd1
cd ..

cd simplest_ffmpeg_audio_player
$cmd1
cd ..

cd simplest_ffmpeg_picture_encoder
$cmd1
cd ..

cd simplest_ffmpeg_video_encoder
$cmd1
cd ..

cd simplest_ffmpeg_audio_encoder
$cmd1
cd ..

cd simplest_ffmpeg_streamer
$cmd1
cd ..

cd simplest_ffmpeg_mem_handler
$cmd1
cd ..

cd simplest_ffmpeg_device
$cmd1
cd ..

cd simplest_ffmpeg_format
$cmd1
cd ..

cd simplest_ffmpeg_video_filter
$cmd1
cd ..

cd simplest_ffmpeg_swscale
$cmd1
cd ..



