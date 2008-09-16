#ifndef __incl_ImageFile_h
#define __incl_ImageFile_h

struct Image
{
    unsigned int  sizeX, sizeY;
    unsigned int  bpp;
    
    enum Type {
        TP_UNSIGNED_BYTE = 1
    } type;

    enum Format {
        FT_RGB8  = 1,
        FT_RGBA8 = 2
    } format;

    enum ColorOrder {
        CO_RGB  = 1,
        CO_RGBA = 2,
        CO_BGR  = 3,
        CO_BGRA = 4
    } colorOrder;

    unsigned char *data;

    Image() : data(0) {}
    ~Image()
    { if (data) delete[] data; }
};

Image *LoadTGA(const char *filename);
Image *LoadBMP(const char *filename);

#endif

