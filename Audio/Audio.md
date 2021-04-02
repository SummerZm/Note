## **PCM音频处理**

### PCM常见音视频处理
- **分离PCM16LE双声道音频采样数据的左声道和右声道 【左右声道一个采样4个字节并排】**
- **将PCM16LE双声道音频采样数据中左声道的音量降一半 【读取左声道的值除以2】**
- **将PCM16LE双声道音频采样数据的声音速度提高一倍 【抽取其中的某些点】**
- **将PCM16LE双声道音频采样数据转换为PCM8音频采样数据**
    ```sh
    # 第一步是将-32768到32767的16bit有符号数值转换为-128到127的8bit有符号数值.
    # 第二步是将-128到127的8bit有符号数值转换为0到255的8bit无符号数值.
    ```

- **将从PCM16LE单声道音频采样数据中截取一部分数据 【打开文件循环读取某些个问题文件】**
- **将PCM16LE双声道音频采样数据转换为WAVE格式音频数据 【加个WAV音频文件头】**
    ```c
        typedef struct WAVE_HEADER{
            char fccID[4];
            unsigned long dwSize;
            char fccType[4];
        }WAVE_HEADER;
 
        typedef struct WAVE_FMT{
            char  fccID[4];
            unsigned long dwSize;
            unsigned short wFormatTag;
            unsigned short wChannels;
            unsigned long dwSamplesPerSec;
            unsigned long dwAvgBytesPerSec;
            unsigned short wBlockAlign;
            unsigned short uiBitsPerSample;
        }WAVE_FMT;
 
        typedef struct WAVE_DATA{
            char       fccID[4];
            unsigned long dwSize;
        }WAVE_DATA;
    ```