::��ý����Ŀʾ������ϼ� ---- Devenv��������
::multimedia projects examples list ---- Devenv compile all
::
::������ Lei Xiaohua
::leixiaohua1020@126.com
::�й���ý��ѧ/���ֵ��Ӽ���
::Communication University of China / Digital TV Technology
::http://blog.csdn.net/leixiaohua1020
::
::�ýű�ʹ��devenv��������[��ý����Ŀʾ������ϼ�]�Ĵ��롣
::[��ý����Ŀʾ������ϼ�]Ŀǰ����������Ŀ��
::  simplest media play:          ��򵥵�ʹ�ø���API��Direct3D��OpenGL��GDI��
::                                DirectSound��SDL2�����Ŷ�ý�������
::  simplest encoder:             ��򵥵���Ƶ����������
::  simplest libRTMP example:     ��򵥵Ļ���libRTMP��ʹ��ʾ��
::  simplest libVLC example:      ��򵥵Ļ���libVLC��ʾ������
::  simplest directshow example:  ��򵥵Ļ���DirectShow��ʾ��
::
::This Batch file uses devenv to compile all the source code of 
::[multimedia projects examples list].
::[multimedia projects examples list] contains following projects:
::  simplest media play:          Examples about video/audio's playing
::  simplest encoder:             Examples about video encoders
::  simplest libRTMP example:     Examples about usage of libRTMP
::  simplest libVLC example:      Examples about libVLC
::  simplest directshow example:  Simple Examples about DirectShow
::=====================================================================================
@echo off
echo ---------------------------------------------
echo multimedia projects examples list ---- Devenv compile all
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
echo Compile simplest_media_play...
cd simplest_media_play
devenv.exe simplest_media_play.sln /rebuild Debug /Out ../simplest_media_play_debug_%logfile%
devenv.exe simplest_media_play.sln /rebuild Release /Out ../simplest_media_play_release_%logfile%
cd ..

::=====================================================================================
echo.
echo Compile simplest_encoder...
cd simplest_encoder
devenv.exe simplest_encoder.sln /rebuild Debug /Out ../simplest_encoder_debug_%logfile%
devenv.exe simplest_encoder.sln /rebuild Release /Out ../simplest_encoder_release_%logfile%
cd ..

::=====================================================================================
echo.
echo Compile simplest_librtmp_example...
cd simplest_librtmp_example
devenv.exe simplest_librtmp_example.sln /rebuild Debug /Out ../simplest_librtmp_example_debug_%logfile%
devenv.exe simplest_librtmp_example.sln /rebuild Release /Out ../simplest_librtmp_example_release_%logfile%
cd ..

::=====================================================================================
echo.
echo Compile simplest_libvlc_example...
cd simplest_libvlc_example
devenv.exe simplest_libvlc_example.sln /rebuild Debug /Out ../simplest_libvlc_example_debug_%logfile%
devenv.exe simplest_libvlc_example.sln /rebuild Release /Out ../simplest_libvlc_example_release_%logfile%
cd ..

::=====================================================================================
echo.
echo Compile simplest_directshow_example...
cd simplest_directshow_example
devenv.exe simplest_directshow_example.sln /rebuild Debug /Out ../simplest_directshow_example_debug_%logfile%
devenv.exe simplest_directshow_example.sln /rebuild Release /Out ../simplest_directshow_example_release_%logfile%
cd ..

::=====================================================================================