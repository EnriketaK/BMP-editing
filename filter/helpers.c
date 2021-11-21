#include "helpers.h"
#include <stdio.h>
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float avg = (image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0;

            image[i][j].rgbtBlue = round(avg);
            image[i][j].rgbtGreen = round(avg);
            image[i][j].rgbtRed = round(avg);
        }
    }
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float sepiaRed = 0.393 * image[i][j].rgbtRed + 0.769 * image[i][j].rgbtGreen + 0.189 * image[i][j].rgbtBlue;
            float sepiaGreen = 0.349 * image[i][j].rgbtRed + 0.686 * image[i][j].rgbtGreen + 0.168 * image[i][j].rgbtBlue;
            float sepiaBlue = 0.272 * image[i][j].rgbtRed + 0.534 * image[i][j].rgbtGreen + 0.131 * image[i][j].rgbtBlue;

            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }

            if (sepiaGreen > 255)
            {
                sepiaGreen  = 255;
            }

            if (sepiaBlue > 255)
            {
                sepiaBlue  = 255;
            }

            image[i][j].rgbtBlue = round(sepiaBlue);
            image[i][j].rgbtGreen = round(sepiaGreen);
            image[i][j].rgbtRed = round(sepiaRed);
        }
    }

}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0, midwidth = width / 2; j < midwidth; j++)
        {
            int a = image[i][j].rgbtBlue;
            image[i][j].rgbtBlue = image[i][width - (j + 1)].rgbtBlue;
            image[i][width - (j + 1)].rgbtBlue = a;

            int b = image[i][j].rgbtGreen;
            image[i][j].rgbtGreen = image[i][width - (j + 1)].rgbtGreen;
            image[i][width - (j + 1)].rgbtGreen = b;

            int c = image[i][j].rgbtRed;
            image[i][j].rgbtRed = image[i][width - (j + 1)].rgbtRed;
            image[i][width - (j + 1)].rgbtRed = c;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE unchanged[height][width];

    for (int a = 0; a < height; a++)
    {
        for (int b = 0; b < width; b++)
        {
            unchanged[a][b].rgbtBlue = image[a][b].rgbtBlue;
            unchanged[a][b].rgbtGreen = image[a][b].rgbtGreen;
            unchanged[a][b].rgbtRed = image[a][b].rgbtRed;
        }
    }

    float sum_red, sum_blue, sum_green;
    int counter;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            sum_red = 0;
            sum_blue = 0;
            sum_green = 0;
            counter = 0;

            if (i > 0 && j > 0 && i != height - 1 && j != width - 1) //middle pixels
            {
                for (int l = i - 1; l < i + 2; l++)
                {
                    for (int k = j - 1; k < j + 2; k++)
                    {
                        sum_blue += unchanged[l][k].rgbtBlue;
                        sum_green += unchanged[l][k].rgbtGreen;
                        sum_red += unchanged[l][k].rgbtRed;
                        counter++;
                    }
                }
            }

            if (i == 0 && j == 0) //left upper corner pixel
            {
                for (int l = i; l < i + 2; l++)
                {
                    for (int k = j; k < j + 2; k++)
                    {
                        sum_blue += unchanged[l][k].rgbtBlue;
                        sum_green += unchanged[l][k].rgbtGreen;
                        sum_red += unchanged[l][k].rgbtRed;
                        counter++;
                    }
                }
            }

            if (i > 0 && i != height - 1 && j == 0) //left edge pixel
            {
                for (int l = i - 1; l < i + 2; l++)
                {
                    for (int k = j; k < j + 2; k++)
                    {
                        sum_blue += unchanged[l][k].rgbtBlue;
                        sum_green += unchanged[l][k].rgbtGreen;
                        sum_red += unchanged[l][k].rgbtRed;
                        counter++;
                    }
                }
            }

            if (i ==  height - 1 && j == 0) //left lower corner pixel
            {
                for (int l = i - 1; l < i + 1; l++)
                {
                    for (int k = j; k < j + 2; k++)
                    {
                        sum_blue += unchanged[l][k].rgbtBlue;
                        sum_green += unchanged[l][k].rgbtGreen;
                        sum_red += unchanged[l][k].rgbtRed;
                        counter++;
                    }
                }
            }

            if (i == 0  && j > 0 && j != width - 1) //top edge pixel
            {
                for (int l = i; l < i + 2; l++)
                {
                    for (int k = j - 1; k < j + 2; k++)
                    {
                        sum_blue += unchanged[l][k].rgbtBlue;
                        sum_green += unchanged[l][k].rgbtGreen;
                        sum_red += unchanged[l][k].rgbtRed;
                        counter++;
                    }
                }
            }

            if (i == height - 1  && j > 0 && j != width - 1) //bottom edge pixel
            {
                for (int l = i - 1; l < i + 1; l++)
                {
                    for (int k = j - 1; k < j + 2; k++)
                    {
                        sum_blue += unchanged[l][k].rgbtBlue;
                        sum_green += unchanged[l][k].rgbtGreen;
                        sum_red += unchanged[l][k].rgbtRed;
                        counter++;
                    }
                }
            }

            if (i == height - 1  && j == width - 1) //lower right corner pixel
            {
                for (int l = i - 1; l < i + 1; l++)
                {
                    for (int k = j - 1; k < j + 1; k++)
                    {
                        sum_blue += unchanged[l][k].rgbtBlue;
                        sum_green += unchanged[l][k].rgbtGreen;
                        sum_red += unchanged[l][k].rgbtRed;
                        counter++;
                    }
                }
            }

            if (i > 0 && i != height - 1  && j == width - 1) //right edge pixel
            {
                for (int l = i - 1; l < i + 2; l++)
                {
                    for (int k = j - 1; k < j + 1; k++)
                    {
                        sum_blue += unchanged[l][k].rgbtBlue;
                        sum_green += unchanged[l][k].rgbtGreen;
                        sum_red += unchanged[l][k].rgbtRed;
                        counter++;
                    }
                }
            }

            if (i == 0 && j == width - 1) //upper right corner pixel
            {
                for (int l = i; l < i + 2; l++)
                {
                    for (int k = j - 1; k < j + 1; k++)
                    {
                        sum_blue += unchanged[l][k].rgbtBlue;
                        sum_green += unchanged[l][k].rgbtGreen;
                        sum_red += unchanged[l][k].rgbtRed;
                        counter++;
                    }
                }
            }

            image[i][j].rgbtBlue = round(sum_blue / counter);
            image[i][j].rgbtGreen = round(sum_green / counter);
            image[i][j].rgbtRed = round(sum_red / counter);
        }
    }
}
