#��ý����Ŀʾ������ϼ� ---- Git�ϴ��ű�
#multimedia projects examples list ---- git push all
#
#������ Lei Xiaohua
#leixiaohua1020@126.com
#�й���ý��ѧ/���ֵ��Ӽ���
#Communication University of China / Digital TV Technology
#http://blog.csdn.net/leixiaohua1020
#
#�ýű������ϴ�[��ý����Ŀʾ������ϼ�]�Ĵ��롣
#[��ý����Ŀʾ������ϼ�]Ŀǰ����������Ŀ��
#  simplest media play:          ��򵥵�ʹ�ø���API��Direct3D��OpenGL��GDI��
#                                DirectSound��SDL2�����Ŷ�ý�������
#  simplest encoder:             ��򵥵���Ƶ����������
#  simplest libRTMP example:     ��򵥵Ļ���libRTMP��ʹ��ʾ��
#  simplest libVLC example:      ��򵥵Ļ���libVLC��ʾ������
#  simplest directshow example:  ��򵥵Ļ���DirectShow��ʾ��
#
#This Batch file is used to push all the source code of 
#[multimedia projects examples list].
#[multimedia projects examples list] contains following projects:
#  simplest media play:          Examples about video/audio's playing
#  simplest encoder:             Examples about video encoders
#  simplest libRTMP example:     Examples about usage of libRTMP
#  simplest libVLC example:      Examples about libVLC
#  simplest directshow example:  Simple Examples about DirectShow

echo "============================================="
echo "multimedia projects examples list ---- git push all"
echo "Lei Xiaohua"
echo "Communication University of China / Digital TV Technology"
echo "leixiaohua1020@126.com"
echo "http://blog.csdn.net/leixiaohua1020"
echo "============================================="

#vars
commitstr="fix"
#remotestr="github"
remotestr=""

#git push
cd simplest_media_play
git add .
git commit -m $commitstr
git push $remotestr
cd ..

cd simplest_encoder
git add .
git commit -m $commitstr
git push $remotestr
cd ..

cd simplest_librtmp_example
git add .
git commit -m $commitstr
git push $remotestr
cd ..

cd simplest_libvlc_example
git add .
git commit -m $commitstr
git push $remotestr
cd ..

cd simplest_directshow_example
git add .
git commit -m $commitstr
git push $remotestr
cd ..
