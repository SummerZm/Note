#��ý����Ŀʾ������ϼ� ---- Git�������½ű�
#multimedia projects examples list ---- git pull all
#
#������ Lei Xiaohua
#leixiaohua1020@126.com
#�й���ý��ѧ/���ֵ��Ӽ���
#Communication University of China / Digital TV Technology
#http://blog.csdn.net/leixiaohua1020
#
#�ýű�������������[��ý����Ŀʾ������ϼ�]�Ĵ��롣
#[��ý����Ŀʾ������ϼ�]Ŀǰ����������Ŀ��
#  simplest media play:          ��򵥵�ʹ�ø���API��Direct3D��OpenGL��GDI��
#                                DirectSound��SDL2�����Ŷ�ý�������
#  simplest encoder:             ��򵥵���Ƶ����������
#  simplest libRTMP example:     ��򵥵Ļ���libRTMP��ʹ��ʾ��
#  simplest libVLC example:      ��򵥵Ļ���libVLC��ʾ������
#  simplest directshow example:  ��򵥵Ļ���DirectShow��ʾ��
#
#This Batch file is used to update all the source code of 
#[multimedia projects examples list].
#[multimedia projects examples list] contains following projects:
#  simplest media play:          Examples about video/audio's playing
#  simplest encoder:             Examples about video encoders
#  simplest libRTMP example:     Examples about usage of libRTMP
#  simplest libVLC example:      Examples about libVLC
#  simplest directshow example:  Simple Examples about DirectShow

echo "============================================="
echo "multimedia projects examples list ---- git pull all"
echo "Lei Xiaohua"
echo "Communication University of China / Digital TV Technology"
echo "leixiaohua1020@126.com"
echo "http://blog.csdn.net/leixiaohua1020"
echo "============================================="
#remotestr="github"
remotestr=""

#Git Pull
cd simplest_media_play
git pull $remotestr
cd ..

cd simplest_encoder
git pull $remotestr
cd ..

cd simplest_librtmp_example
git pull $remotestr
cd ..

cd simplest_libvlc_example
git pull $remotestr
cd ..

cd simplest_directshow_example
git pull $remotestr
cd ..
