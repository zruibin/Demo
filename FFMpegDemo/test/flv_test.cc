/*
 *
 * h265_test.cc
 *
 * Created by Ruibin.Chow on 2022/12/02.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "foundation/assets.h"

/*
 https://www.jianshu.com/p/419f99be3989
*/

//Important!
#pragma pack(1)


#define TAG_TYPE_SCRIPT 18  //帧tag
#define TAG_TYPE_AUDIO  8   //audio tag
#define TAG_TYPE_VIDEO  9   //video tag

typedef unsigned char byte;
typedef unsigned int uint;

typedef struct {
    byte Signature[3];  //文件标识，总是为FLV
    byte Version;   //版本号，总是为1
    byte Flags;   // 音视频流存在标识
    uint DataOffset;
} FLV_HEADER;   // FLV 文件头

typedef struct {
    byte TagType;  //tag类型
    byte DataSize[3];  //tag data字节数
    byte Timestamp[3];  //时戳
    uint Reserved;  //时间戳扩展字节+streamID
} TAG_HEADER;  //tag头

//翻转字节，大段序转成小段序
uint reverse_bytes(byte *p, char c) {
    int r = 0;
    int i;
    for (i = 0; i<c; i++)
        r |=  ( *(p+i) << (((c-1)*8)-8*i));
    return r;
}

/**
 分析FLV 文件
 @param url FLV文件路径
 */
int simplest_flv_parser(const char *url) {
    //whether output audio/video stream
    int output_a = 1;
    int output_v = 1;
    //-------------
    FILE *ifh = NULL,*vfh = NULL, *afh = NULL;
    
    //FILE *myout = fopen("output_log.txt","wb+");
    FILE *myout = stdout;
    
    FLV_HEADER flv;
    TAG_HEADER tagheader;
    uint previoustagsize, previoustagsize_z = 0;
    uint ts = 0, ts_new = 0;
    
    ifh = fopen(url, "rb+");
    if ( ifh == NULL) {
        printf("Failed to open files!");
        return -1;
    }
    
    //FLV file header 先读FLV头
    fread((char *)&flv,1,sizeof(FLV_HEADER),ifh);
    
    fprintf(myout," = = = = = = = = = = = = = = FLV Header = = = = = = = = = = = = = = \n");
    fprintf(myout,"Signature:  0x %c %c %c\n",flv.Signature[0],flv.Signature[1],flv.Signature[2]);
    fprintf(myout,"Version:    0x %X\n",flv.Version);
    fprintf(myout,"Flags  :    0x %X\n",flv.Flags);
    fprintf(myout,"HeaderSize: 0x %X\n",reverse_bytes((byte *)&flv.DataOffset, sizeof(flv.DataOffset)));
    fprintf(myout," = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =  = \n");
    
    //move the file pointer to the end of the header
    fseek(ifh, reverse_bytes((byte *)&flv.DataOffset, sizeof(flv.DataOffset)), SEEK_SET);
    
    //process each tag
    do {
        //读取Previous tag size
        previoustagsize =  getw(ifh);
        // 读取tag header
        fread((void *)&tagheader,sizeof(TAG_HEADER),1,ifh);
        
        //int temp_datasize1 = reverse_bytes((byte *)&tagheader.DataSize, sizeof(tagheader.DataSize));
        //获取tag data的字节数
        int tagheader_datasize = tagheader.DataSize[0]*pow(2, 16)+tagheader.DataSize[1]*pow(2, 8)+tagheader.DataSize[2];
        
        //获取时戳
        int tagheader_timestamp = tagheader.Timestamp[0]*pow(2, 16)+tagheader.Timestamp[1]*pow(2, 8)+tagheader.Timestamp[2];
        
        char tagtype_str[10];
        //获取tag类型
        switch(tagheader.TagType){
            case TAG_TYPE_AUDIO:sprintf(tagtype_str,"AUDIO");break;
            case TAG_TYPE_VIDEO:sprintf(tagtype_str,"VIDEO");break;
            case TAG_TYPE_SCRIPT:sprintf(tagtype_str,"SCRIPT");break;
            default:sprintf(tagtype_str,"UNKNOWN");break;
        }
        fprintf(myout,"[%6s] %6d %6d |",tagtype_str,tagheader_datasize,tagheader_timestamp);
        
        //if we are not past the end of file, process the tag
        if (feof(ifh)) {
            break;
        }
        
        //process tag by type
        switch (tagheader.TagType) {
            case TAG_TYPE_AUDIO:{  //音频
                char audiotag_str[100] = {0};
                strcat(audiotag_str,"| ");
                char tagdata_first_byte;
                
                //读取一个字符，音频tag data区域的第一个字节，音频的信息
                tagdata_first_byte = fgetc(ifh);
                // &操作获取前四位，代表音频格式
                int x = tagdata_first_byte&0xF0;
                //右移4位
                x = x>>4;
                //判断音频格式
                switch (x) {
                    case 0:strcat(audiotag_str,"Linear PCM, platform endian");break;
                    case 1:strcat(audiotag_str,"ADPCM");break;
                    case 2:strcat(audiotag_str,"MP3");break;
                    case 3:strcat(audiotag_str,"Linear PCM, little endian");break;
                    case 4:strcat(audiotag_str,"Nellymoser 16-kHz mono");break;
                    case 5:strcat(audiotag_str,"Nellymoser 8-kHz mono");break;
                    case 6:strcat(audiotag_str,"Nellymoser");break;
                    case 7:strcat(audiotag_str,"G.711 A-law logarithmic PCM");break;
                    case 8:strcat(audiotag_str,"G.711 mu-law logarithmic PCM");break;
                    case 9:strcat(audiotag_str,"reserved");break;
                    case 10:strcat(audiotag_str,"AAC");break;
                    case 11:strcat(audiotag_str,"Speex");break;
                    case 14:strcat(audiotag_str,"MP3 8-Khz");break;
                    case 15:strcat(audiotag_str,"Device-specific sound");break;
                    default:strcat(audiotag_str,"UNKNOWN");break;
                }
                strcat(audiotag_str,"| ");
                
                //获取5~6位，采样率
                x = tagdata_first_byte&0x0C;
                //右移2位
                x = x>>2;
                //判断采样率
                switch (x)
                {
                    case 0:strcat(audiotag_str,"5.5-kHz");break;
                    case 1:strcat(audiotag_str,"1-kHz");break;
                    case 2:strcat(audiotag_str,"22-kHz");break;
                    case 3:strcat(audiotag_str,"44-kHz");break;
                    default:strcat(audiotag_str,"UNKNOWN");break;
                }
                strcat(audiotag_str,"| ");
                
                //获取第7位，采样精度
                x = tagdata_first_byte&0x02;
                x = x>>1;
                switch (x)
                {
                    case 0:strcat(audiotag_str,"8Bit");break;
                    case 1:strcat(audiotag_str,"16Bit");break;
                    default:strcat(audiotag_str,"UNKNOWN");break;
                }
                strcat(audiotag_str,"| ");
                
                //获取第8位，音频声道数
                x = tagdata_first_byte&0x01;
                switch (x)
                {
                    case 0:strcat(audiotag_str,"Mono");break;
                    case 1:strcat(audiotag_str,"Stereo");break;
                    default:strcat(audiotag_str,"UNKNOWN");break;
                }
                fprintf(myout,"%s",audiotag_str);
                
                //if the output file hasn't been opened, open it.
                if(output_a != 0&&afh ==  NULL){
                    afh = fopen("output.mp3", "wb");
                }
                
                //TagData - First Byte Data
                //获取tag Data字节数,需要减去Tag Data区域的第一个字节
                int data_size = reverse_bytes((byte *)&tagheader.DataSize, sizeof(tagheader.DataSize))-1;
                
                //循环获取字节写入文件
                if (output_a != 0) {
                    //TagData+1
                    for (int i = 0; i<data_size; i++)
                        fputc(fgetc(ifh),afh);
                    
                } else {
                    for (int i = 0; i<data_size; i++)
                        fgetc(ifh);
                }
                break;
            }
            case TAG_TYPE_VIDEO: {   //视频
                char videotag_str[100] = {0};
                strcat(videotag_str,"| ");
                //读取TagData区域第一个字节，取出前4位。包含视频帧类型
                char tagdata_first_byte;
                tagdata_first_byte = fgetc(ifh);
                int x = tagdata_first_byte&0xF0;
                x = x>>4;
                switch (x) {
                    case 1:strcat(videotag_str,"key frame  ");break;
                    case 2:strcat(videotag_str,"inter frame");break;
                    case 3:strcat(videotag_str,"disposable inter frame");break;
                    case 4:strcat(videotag_str,"generated keyframe");break;
                    case 5:strcat(videotag_str,"video info/command frame");break;
                    default:strcat(videotag_str,"UNKNOWN");break;
                }
                strcat(videotag_str,"| ");
                
                //读取TagData区域第一个字节，取出后4位。包含视频编码类型
                x = tagdata_first_byte&0x0F;
                switch (x) {
                    case 1:strcat(videotag_str,"JPEG (currently unused)");break;
                    case 2:strcat(videotag_str,"Sorenson H.263");break;
                    case 3:strcat(videotag_str,"Screen video");break;
                    case 4:strcat(videotag_str,"On2 VP6");break;
                    case 5:strcat(videotag_str,"On2 VP6 with alpha channel");break;
                    case 6:strcat(videotag_str,"Screen video version 2");break;
                    case 7:strcat(videotag_str,"AVC");break;
                    default:strcat(videotag_str,"UNKNOWN");break;
                }
                fprintf(myout,"%s",videotag_str);
                
                fseek(ifh, -1, SEEK_CUR);
                //if the output file hasn't been opened, open it.
                if (vfh ==  NULL && output_v != 0) {
                    //write the flv header (reuse the original file's hdr) and first previoustagsize
                    vfh = fopen("output.flv", "wb");
                    fwrite((char *)&flv,1, sizeof(flv),vfh);
                    fwrite((char *)&previoustagsize_z,1,sizeof(previoustagsize_z),vfh);
                }
#if 0
                //Change Timestamp
                //Get Timestamp
                ts = reverse_bytes((byte *)&tagheader.Timestamp, sizeof(tagheader.Timestamp));
                ts = ts*2;
                //Writeback Timestamp
                ts_new = reverse_bytes((byte *)&ts, sizeof(ts));
                memcpy(&tagheader.Timestamp, ((char *)&ts_new) + 1, sizeof(tagheader.Timestamp));
#endif
                
                
                //TagData + Previous Tag Size
                int data_size = reverse_bytes((byte *)&tagheader.DataSize, sizeof(tagheader.DataSize))+4;
                if (output_v != 0) {
                    //TagHeader
                    fwrite((char *)&tagheader,1, sizeof(tagheader),vfh);
                    //TagData
                    for (int i = 0; i<data_size; i++)
                        fputc(fgetc(ifh),vfh);
                } else {
                    for (int i = 0; i<data_size; i++)
                        fgetc(ifh);
                }
                //rewind 4 bytes, because we need to read the previoustagsize again for the loop's sake
                fseek(ifh, -4, SEEK_CUR);
                
                break;
            }
            default:
                
                //skip the data of this tag
                fseek(ifh, reverse_bytes((byte *)&tagheader.DataSize, sizeof(tagheader.DataSize)), SEEK_CUR);
        }
        
        fprintf(myout,"\n");
        
    } while (!feof(ifh));
    
//
    fclose(ifh);
    fclose(vfh);
    fclose(afh);
    
    return 0;
}

TEST(FLVTest, FLVParseTest) {
    using namespace Foundation;
    std::string flvPath{Assets::AssetsDirPath().append("juren.flv")};
    printf("file: %s\n", flvPath.c_str());
    //解析FLV
    simplest_flv_parser(flvPath.c_str());
}
