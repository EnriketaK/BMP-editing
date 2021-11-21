// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }

    // get resize value & remember filenames
    int resize_value = atoi(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];

    //ensures resize value is within limits
    while (resize_value < 1 || resize_value > 100)
    {
        fprintf("Usage: ./resize n infile outfile\n");
        return 1;
    }

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    //resized image's height & width
    bi.biWidth *= resize_value;
    bi.biHeight *= resize_value;

    // determine padding for infile & outfile for scanlines
    int padding_infile = (4 - ((bi.biWidth / resize_value) * sizeof(RGBTRIPLE)) % 4) % 4;
    int padding_outfile = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    //resized image's size of the image & its size of the file in bytes
    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + padding_outfile) * abs(bi.biHeight);
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0, biHeight = (abs(bi.biHeight) / resize_value); i < biHeight; i++)//b.h - r_v
    {
        //iterate over outfile's scanlines
        for (int resize_rows = 0; resize_rows < resize_value; resize_rows++)
        {
            //iterate over infile's pixels
            for (int j = 0, biWidth = bi.biWidth / resize_value; j < biWidth; j++) //writing out the new nr of pixels
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                //iterate each pixel the required amount of time
                for (int resize_pixels = 0; resize_pixels < resize_value; resize_pixels++)
                {
                    // write RGB triple to outfile
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);

                }
            }
            // skip over padding, if any
            fseek(inptr, padding_infile, SEEK_CUR);

            // then add it back (to demonstrate how)
            for (int k = 0; k < padding_outfile; k++)
            {
                fputc(0x00, outptr);

            }
            //go to the start of intfile's scanline
            fseek(inptr, -((bi.biWidth / resize_value) * (sizeof(RGBTRIPLE)) + padding_infile), SEEK_CUR);
        }
        //go to the end of infile's scanline
        fseek(inptr, ((bi.biWidth / resize_value) * (sizeof(RGBTRIPLE)) + padding_infile), SEEK_CUR);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
