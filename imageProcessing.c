#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imageProcessing.h"

int main() {
	unsigned char   R[WIDTH][HEIGHT];
	unsigned char   G[WIDTH][HEIGHT];
	unsigned char   B[WIDTH][HEIGHT];

	int flag;
	do
	{
		flag = 0;

		char fname[SLEN];
		char output_name[SLEN];
		char color[SLEN];
		int t_R, t_G, t_B, threshold, r_R, r_G, r_B, border_width, choice;

//choice
		PrintMenu();
		printf("Please make your choice: ");
		scanf("%d",&choice);
		printf("\n");

		switch(choice)
		{
			case 1:
			printf("Please input the file name to load: ");
			scanf("%s", fname);
			printf("\n");
			ReadImage(fname, R, G, B);
			printf("-------------------------------------------\n");
			break;

			case 2:
			printf("Please input the file name to save: ");
			scanf("%s",output_name);
			SaveImage(output_name, R, G, B);
			printf("-------------------------------------------\n");
			break;

			case 3:
			Negative(R, G, B);
			printf("\"Negative\" operation is done!\n");
			printf("-------------------------------------------\n");
			break;

			case 4:
			printf("Enter Red component for the target color: ");
			scanf("%d\n", &t_R);
			printf("Enter Green component for the target color: ");
			scanf("%d\n", &t_G);
			printf("Enter Blue component for the target color:  ");
			scanf("%d\n", &t_B);
			printf("Enter threshold for the color difference:  ");
			scanf("%d\n", &threshold);
			printf("Enter value for Red component in the target color:  ");
			scanf("%d\n", &r_R);
			printf("Enter value for Green component in the target color: ");
			scanf("%d\n", &r_G);
			printf("Enter value for Blue component in the target color: ");
			scanf("%d\n", &r_B);
			ColorFilter(R, G, B, t_R, t_G, t_B, threshold, r_R, r_G, r_B);
			printf("\"Color Filter\" operation is done!\n");
			printf("-------------------------------------------\n");
			break;

			case 5:
			Edge(R, G, B);
			printf("\"Edge\" operation is done!\n");
			printf("-------------------------------------------\n");
			break;

			case 6:
			HFlip(R, G, B);
			printf("\"HFlip\" operation is done!\n");
			printf("-------------------------------------------\n");
			break;

			case 7:
			VMirror(R, G, B);
			printf("\"VMirror\" operation is done!\n");
			printf("-------------------------------------------\n");
			break;

			case 8:
			printf("Enter border width: ");
			scanf("%d",&border_width);
			printf("Available border colors are : black, white, red, green, blue, yellow, cyan, pink, orange\n");
			printf("Select border colors from the options: ");
			scanf("%s", color);
			AddBorder(R, G, B, color, border_width);
			printf("\"Border\" operation is done!");
			printf("-------------------------------------------\n");
			break;

			case 9:
			Zoom(R, G, B);
			printf("\"Zoom\" operation is done!");
			printf("-------------------------------------------\n");
			break;

			case 10:
			AutoTest(R, G, B);
			printf("-------------------------------------------\n");
			break;

			case 11:
			flag = 1;
			printf("-------------------------------------------\n");
			break;

			default:
			printf("Please enter a valid choice\n");
		}
	}while(!flag);

	return 0;
}

int ReadImage(char fname[SLEN], unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]) {
	FILE           *File;
	char            Type[SLEN];
	int             Width, Height, MaxValue;
	int             x, y;
	char            ftype[] = ".ppm";
	char            fname_tmp[SLEN];
	strcpy(fname_tmp, fname);
	strcat(fname_tmp, ftype);

	File = fopen(fname_tmp, "r");
	if (!File) {
		printf("Cannot open file \"%s\" for reading!\n", fname);
		return 1;
	}
	fscanf(File, "%79s", Type);
	if (Type[0] != 'P' || Type[1] != '6' || Type[2] != 0) {
		printf("\nUnsupported file format!\n");
		return 2;
	}
	fscanf(File, "%d", &Width);
	if (Width != WIDTH) {
		printf("\nUnsupported image width %d!\n", Width);
		return 3;
	}
	fscanf(File, "%d", &Height);
	if (Height != HEIGHT) {
		printf("\nUnsupported image height %d!\n", Height);
		return 4;
	}
	fscanf(File, "%d", &MaxValue);
	if (MaxValue != 255) {
		printf("\nUnsupported image maximum value %d!\n", MaxValue);
		return 5;
	}
	if ('\n' != fgetc(File)) {
		printf("\nCarriage return expected!\n");
		return 6;
	}
	for (y = 0; y < HEIGHT; y++)
		for (x = 0; x < WIDTH; x++) {
			R[x][y] = fgetc(File);
			G[x][y] = fgetc(File);
			B[x][y] = fgetc(File);
		}
		if (ferror(File)) {
			printf("\nFile error while reading from file!\n");
			return 7;
		}
		printf("%s was read successfully!\n", fname_tmp);
		fclose(File);
		return 0;
	}

	int SaveImage(char fname[SLEN], unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]) {
		FILE	*File;
		int	x, y;
		char	SysCmd[SLEN * 5];

		char	ftype[] = ".ppm";
		char	fname_tmp[SLEN];
		char	fname_tmp2[SLEN];

		strcpy(fname_tmp, fname);
		strcpy(fname_tmp2, fname);
		strcat(fname_tmp2, ftype);

		File = fopen(fname_tmp2, "w");
		if (!File) {
			printf("Cannot open file \"%s\" for writing!\n", fname);
			return 1;
		}
		fprintf(File, "P6\n");
		fprintf(File, "%d %d\n", WIDTH, HEIGHT);
		fprintf(File, "255\n");

		for (y = 0; y < HEIGHT; y++)
			for (x = 0; x < WIDTH; x++) {
				fputc(R[x][y], File);
				fputc(G[x][y], File);
				fputc(B[x][y], File);
			}

			if (ferror(File)) {
				printf("\nFile error while writing to file!\n");
				return 2;
			}
			fclose(File);
			printf("%s was saved successfully. \n", fname_tmp2);
			return (0);
		}

/* age the image to make it look like old picture */
void Aging(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]) {
	int x;
	int y;
	for(y = 0; y < HEIGHT; y++ ) {
		for(x = 0; x < WIDTH; x++ ) {
			B[x][y] = (R[x][y] + G[x][y] + B[x][y]) / 5;
			R[x][y] = (unsigned char) (B[x][y] * 1.6);
			G[x][y] = (unsigned char) (B[x][y] * 1.6);
		}
	}
}

/* menu */
void PrintMenu()
{
	printf("1: Load a PPM image\n2: Save an image in PPM and JPEG format\n3: Make a negative of an image\n4: Color filter an image\n5: Sketch the edge of an image\n6: Flip an image horizontally\n7: Mirror an image vertically\n8: Add Border to an image\n9: Zoom an image\n10: Test all functions\n11: Exit\n");
}

/* negative */
void Negative(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
{
	int x, y;
	for(y = 0; y < HEIGHT; y++ ) {
		for(x = 0; x < WIDTH; x++ ) {
			R[x][y] = MAX_INTENSITY - R[x][y];
			G[x][y] = MAX_INTENSITY - G[x][y];
			B[x][y] = MAX_INTENSITY - B[x][y];
		}
	}
}
/* colorFilter */
void ColorFilter(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT], int target_r, int target_g, int target_b, int threshold, int replace_r, int replace_g, int replace_b)
{
	int x, y;
	for(y = 0; y < HEIGHT; y++ ) {
		for(x = 0; x < WIDTH; x++ ) {
			if((R[x][y] >= target_r - threshold && R[x][y] <= target_r + threshold) && (G[x][y] >= target_g - threshold && G[x][y] <= target_g + threshold) && (B[x][y] >= target_b - threshold && B[x][y] <= target_b + threshold))
			{
				R[x][y] = replace_r;
				G[x][y] = replace_g;
				B[x][y] = replace_b;
			}
		}
	}
}

/* edge */
void Edge(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
{
	int x, y;
	unsigned char RT[WIDTH][HEIGHT];
	unsigned char GT[WIDTH][HEIGHT];
	unsigned char BT[WIDTH][HEIGHT];

	for(y = 0; y < HEIGHT; y++ ) {
		for(x = 0; x < WIDTH; x++ ) {
			if(y == 0 || y == HEIGHT-1 || x == 0 || x == WIDTH-1)
			{
				RT[x][y] = 0;
				GT[x][y] = 0;
				BT[x][y] = 0;
				continue;
			}
	//setting central pixal values according to mask
			int l, m, sumR = 0, sumG = 0, sumB = 0;
			for(l = y - 1; l <= y + 1; ++l)
			{
				for(m = x - 1; m <= x + 1; ++m)
				{
					if(y == l && x == m)
					{
						sumR += 8*R[x][y];
						sumG += 8*G[x][y];
						sumB += 8*B[x][y];
					}
					else
					{
						sumR -= R[m][l];
						sumG -= G[m][l];
						sumB -= B[m][l];
					}
				}
			}
			if(sumR < 0) sumR = 0;
			else if(sumR > 255) sumR = 255;

			if(sumG < 0) sumG = 0;
			else if(sumG > 255) sumG = 255;

			if(sumB < 0) sumB = 0;
			else if(sumB > 255) sumB = 255;

			RT[x][y] = sumR;
			GT[x][y] = sumG;
			BT[x][y] = sumB;
		}
	}
	for(y = 0; y < HEIGHT; y++ ) {
		for(x = 0; x < WIDTH; x++ ) {
			R[x][y] = RT[x][y];
			G[x][y] = GT[x][y];
			B[x][y] = BT[x][y];
		}
	}
}

/* hFlip */
void swap(char *a, char *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

void HFlip(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
{
	int x;
	for(x = 0; x < HEIGHT; x++ ) {
		{
			int low = 0, end = WIDTH-1;
			while(low < end)
			{
				swap(&R[low][x], &R[end][x]);
				swap(&G[low][x], &G[end][x]);
				swap(&B[low][x], &B[end][x]);

				low++;
				end--;
			}
		}
	}
}

/* vMirror */
void VMirror(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
{
	int x, y, l;
	for(x = 0, l = HEIGHT - 1; x < HEIGHT/2; x++, l-- ) {
		for(y = 0; y < WIDTH; y++ ) {
			R[y][l] = R[y][x];
			G[y][l] = G[y][x];
			B[y][l] = B[y][x];
		}
	}
}

/* addBorder */
void AddBorder(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT], char color[SLEN], int border_width)
{
	int vertical_border_width = (border_width/16)*9;
	int r, g, b;

	if(strcmp(color, "red") == 0)
	{
		r = 255; g = 0; b = 0;
	}
	else if(!strcmp(color, "green"))
	{
		r = 0; g = 255; b = 0;
	}
	else if(!strcmp(color, "blue"))
	{
		r = 0; g = 0; b = 255;
	}
	else if(!strcmp(color, "yello"))
	{
		r = 255; g = 255; b = 0;
	}
	else if(!strcmp(color, "cyan"))
	{
		r = 0; g = 255; b = 255;
	}
	else if(!strcmp(color, "pink"))
	{
		r = 255; g = 192; b = 203;
	}
	else if(!strcmp(color, "orange"))
	{
		r = 255; g = 165; b = 0;
	}
	else if(!strcmp(color, "white"))
	{
		r = 255; g = 255; b = 255;
	}
	else if(!strcmp(color, "black"))
	{
		r = 0; g = 0; b = 0;
	}
	else
	{
		printf("Please enter the color from above list only\n");
		return;
	}

	int x, y, f = 0;
	for(x = 0; x < WIDTH; x++ ) {
		if(x == vertical_border_width && f == 0)
		{
			f = 1;
			x = WIDTH - vertical_border_width;
			continue;
		}

		for(y = 0; y < HEIGHT; y++ ) {
			R[x][y] = r;
			G[x][y] = g;
			B[x][y] = b;
		}
	}

	for(x = vertical_border_width; x <= WIDTH - vertical_border_width; x++ ) {
		f = 0;
		for(y = 0; y < HEIGHT; y++ ) {
			if(y == border_width && f == 0)
			{
				f = 1;
				y = HEIGHT - border_width;
				continue;
			}

			R[x][y] = r;
			G[x][y] = g;
			B[x][y] = b;
		}
	}
}

/* zoom */
void Zoom(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
{
	unsigned char RT[2*WIDTH-1][2*HEIGHT-1];
	unsigned char GT[2*WIDTH-1][2*HEIGHT-1];
	unsigned char BT[2*WIDTH-1][2*HEIGHT-1];

	int x, y, l, m;
	for(x = 0, l = 0; x < WIDTH; ++x, l += 2)
	{
		for(y = 0, m = 0; y < HEIGHT; ++y, m += 2)
		{
			RT[l][m] = R[x][y];
			GT[l][m] = G[x][y];
			BT[l][m] = B[x][y];
		}
	}

	for(x = 1; x < 2*WIDTH - 1; x += 2)
	{
		for(y = 1; y < 2*HEIGHT - 1; y += 2)
		{
			RT[x][y] = RT[x][y-1] + (RT[x][y+1] - RT[x][y-1])/2;
			GT[x][y] = GT[x][y-1] + (GT[x][y+1] - GT[x][y-1])/2;
			BT[x][y] = BT[x][y-1] + (BT[x][y+1] - BT[x][y-1])/2;
		}
	}

	for(l = 1; l < 2*HEIGHT - 1; l += 2)
	{
		for(m = 1; m < 2*WIDTH - 1; m += 2)
		{
			RT[m][l] = RT[m-1][l] + (RT[m+1][l] - RT[m-1][l])/2;
			GT[m][l] = GT[m-1][l] + (GT[m+1][l] - GT[m-1][l])/2;
			BT[m][l] = BT[m-1][l] + (BT[m+1][l] - BT[m-1][l])/2;
		}
	}

	for(x = 0; x < WIDTH; ++x)
	{
		for(y = 0; y < HEIGHT; ++y)
		{
			R[x][y] = RT[x][y];
			G[x][y] = GT[x][y];
			B[x][y] = BT[x][y];
		}
	}
}

/* test all DIP operations */
void AutoTest(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]) {
	char fname[SLEN] = "EH";
	char sname[SLEN];

	ReadImage(fname, R, G, B);
	Negative(R, G, B);
	strcpy(sname, "negative");
	SaveImage(sname, R, G, B);
	printf("Negative tested!\n\n");

	ReadImage(fname, R, G, B);
	ColorFilter(R, G, B, 190, 100, 150, 60, 0, 0, 255);
	strcpy(sname, "colorfilter");
	SaveImage(sname, R, G, B);
	printf("Color Filter tested!\n\n");

	ReadImage(fname, R, G, B);
	Edge(R, G, B);
	strcpy(sname, "edge");
	SaveImage(sname, R, G, B);
	printf("Edge tested!\n\n");

	ReadImage(fname, R, G, B);
	HFlip(R, G, B);
	strcpy(sname, "hflip");
	SaveImage(sname, R, G, B);
	printf("HFlip tested!\n\n");

	ReadImage(fname, R, G, B);
	VMirror(R, G, B);
	strcpy(sname, "vmirror");
	SaveImage(sname, R, G, B);
	printf("VMirror tested!\n\n");


	ReadImage(fname, R, G, B);
	char color[] = "red";
	AddBorder(R, G, B, color, 64);
	strcpy(sname, "border");
	SaveImage(sname, R, G, B);
	printf("AddBorder tested!\n\n");

	ReadImage(fname, R, G, B);
	Zoom(R, G, B);
	strcpy(sname, "zoom");
	SaveImage(sname, R, G, B);
	printf("Zoom tested!\n\n");
}
