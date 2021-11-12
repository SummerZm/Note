#! /bin/sh
#��򵥵Ļ���FFmpegʾ������ϼ� ---- Git�����鵵�ű�
#simplest ffmpeg demos list ---- git archive all
#
#������ Lei Xiaohua
#leixiaohua1020@126.com
#�й���ý��ѧ/���ֵ��Ӽ���
#Communication University of China / Digital TV Technology
#http://blog.csdn.net/leixiaohua1020
#
#�ýű����������鵵[��򵥵Ļ���FFmpeg��ʾ������ϼ�]�Ĵ��롣
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
#This Batch file is used to archive all the source code of 
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
echo "simplest ffmpeg demos list ---- git archive all"
echo "Lei Xiaohua"
echo "Communication University of China / Digital TV Technology"
echo "leixiaohua1020@126.com"
echo "http://blog.csdn.net/leixiaohua1020"
echo "============================================="



#save in a folder named with date
#file suffix
#fsuffix="latest"
fsuffix=$(date +%y%m%d)
#folder to save files
archivefolder="simplest_ffmpeg_demos_list_"${fsuffix}
echo "Archive Folder : "${archivefolder}

#if folder not exist, create it
if [ ! -d ${archivefolder} ]; then
  mkdir ${archivefolder}
fi

#Git Archive
echo Archive simplest_ffmpeg_player...
cd simplest_ffmpeg_player
git archive -o "../"${archivefolder}"/simplest_ffmpeg_player_"${fsuffix}".zip" HEAD
cd ..


echo Archive simplest_ffmpeg_audio_player...
cd simplest_ffmpeg_audio_player
git archive -o "../"${archivefolder}"/simplest_ffmpeg_audio_player_"${fsuffix}".zip" HEAD
cd ..

echo Archive simplest_ffmpeg_picture_encoder...
cd simplest_ffmpeg_picture_encoder
git archive -o "../"${archivefolder}"/simplest_ffmpeg_picture_encoder_"${fsuffix}".zip" HEAD
cd ..

echo Archive simplest_ffmpeg_video_encoder...
cd simplest_ffmpeg_video_encoder
git archive -o "../"${archivefolder}"/simplest_ffmpeg_video_encoder_"${fsuffix}".zip" HEAD
cd ..

echo Archive simplest_ffmpeg_audio_encoder...
cd simplest_ffmpeg_audio_encoder
git archive -o "../"${archivefolder}"/simplest_ffmpeg_audio_encoder_"${fsuffix}".zip" HEAD
cd ..

echo Archive simplest_ffmpeg_streamer...
cd simplest_ffmpeg_streamer
git archive -o "../"${archivefolder}"/simplest_ffmpeg_streamer_"${fsuffix}".zip" HEAD
cd ..

echo Archive simplest_ffmpeg_mem_handler...
cd simplest_ffmpeg_mem_handler
git archive -o "../"${archivefolder}"/simplest_ffmpeg_mem_handler_"${fsuffix}".zip" HEAD
cd ..

echo Archive simplest_ffmpeg_device...
cd simplest_ffmpeg_device
git archive -o "../"${archivefolder}"/simplest_ffmpeg_device_"${fsuffix}".zip" HEAD
cd ..

echo Archive simplest_ffmpeg_format...
cd simplest_ffmpeg_format
git archive -o "../"${archivefolder}"/simplest_ffmpeg_format_"${fsuffix}".zip" HEAD
cd ..

echo Archive simplest_ffmpeg_video_filter...
cd simplest_ffmpeg_video_filter
git archive -o "../"${archivefolder}"/simplest_ffmpeg_video_filter_"${fsuffix}".zip" HEAD
cd ..

echo Archive simplest_ffmpeg_swscale...
cd simplest_ffmpeg_swscale
git archive -o "../"${archivefolder}"/simplest_ffmpeg_swscale_"${fsuffix}".zip" HEAD
cd ..



