## YUV简介

- YUV文件结构： Y(w*h) + U(w*h/2) + V(w*h/2)
- RGB文件结构： RGB [间隔]
- YUV图像亮度调整：Y分量表示亮度
- YUV图像灰度图片：U/V分量表示色度
	```sh
	# 将U/V分量设置为128.
	#
	# U、V是图像中的经过偏置处理的色度分量
	# 色度分量在偏置处理前的取值范围是-128至127，这时候的无色对应的是“0”值。
	# 经过偏置后色度分量取值变成了0至255，因而此时的无色对应的就是128了。
	```
- YUV峰值信噪比:图片压缩后损失度
	```sh
	# 求均方差，然后用分呗表示 
	# PSNR取值通常情况下都在20-50的范围内，取值越高，代表两张图像越接近
	
	for(int i=0;i<num;i++){
		fread(pic1,1,w*h,fp1);
		fread(pic2,1,w*h,fp2);
	 
		double mse_sum=0,mse=0,psnr=0;
		for(int j=0;j<w*h;j++){
			mse_sum+=pow((double)(pic1[j]-pic2[j]),2);
		}
		mse=mse_sum/(w*h);
		psnr=10*log10(255.0*255.0/mse); /*公式*/
		printf("%5.3f\n",psnr);
		 
		fseek(fp1,w*h/2,SEEK_CUR);
		fseek(fp2,w*h/2,SEEK_CUR);
	}
	
	```
