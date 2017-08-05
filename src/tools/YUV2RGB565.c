/*
 * YUV2RGB565.c
 *
 *  Created on: 2017年8月4日
 *      Author: Cotyledon
 */

void convert_yuyv_to_rgb(unsigned char *inBuf, unsigned char *outBuf,int imgWidth, int imgHeight)
{
    int rows, cols;
    int y, u, v, r, g, b;
    unsigned char *YUVdata, *RGBdata;
    int Ypos, Upos, Vpos;//in the inBuf's postion

    YUVdata = inBuf;
    RGBdata = outBuf;
    Ypos = 0;
    Upos = Ypos + 1;
    Vpos = Upos + 2;

    for(rows = 0; rows < imgHeight; rows++) {
        for(cols = 0; cols < imgWidth; cols++) {
            y = YUVdata[Ypos];
            u = YUVdata[Upos] - 128;
            v = YUVdata[Vpos] - 128;

            //r= y+1.4075*v;
            //g= y-0.3455*u-0.7169*v;
            //b= y+1.779*u;
            r = y + v + ((v * 103) >> 8);
            g = y - ((u * 88) >> 8) - ((v * 183) >> 8);
            b = y + u + ((u * 198) >> 8);

            r = (r > 255) ? 255 : ((r < 0) ? 0 : r);
            g = (g > 255) ? 255 : ((g < 0) ? 0 : g);
            b = (b > 255) ? 255 : ((b < 0) ? 0 : b);

            *(RGBdata++) = ( ((g & 0x1C) << 3) | (r >> 3) );
            *(RGBdata++) = ( (b & 0xF8) | (g >> 5) );

            Ypos += 2;
            if(!(cols & 0x01)) {//if cols%2!=0
                Upos = Ypos + 1;
                Vpos = Upos + 2;
            }
        }
    }
}
