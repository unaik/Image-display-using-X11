#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>


int main()
{
  FILE *fpt;
  char header[80];
  char filename[50];
  char ans[1];
  int ROWS,COLS,Bytes; 
  int i,j;  
  unsigned char *image,*displaydata;
  unsigned char *red,*green1,*green2,*blue;
  
  red =  (unsigned char*)malloc(sizeof(unsigned char));
  green1 =  (unsigned char*)malloc(sizeof(unsigned char));
  green2 =  (unsigned char*)malloc(sizeof(unsigned char));
  blue =  (unsigned char*)malloc(sizeof(unsigned char));
  
  Display *Monitor;
  Window ImageWindow;
  GC ImageGC;
  XWindowAttributes Atts;
  XImage *Picture;
  
  while(1)
 {
  printf("Enter filename :\n");
  scanf("%s",filename);
  fpt = fopen(filename,"r");
  fscanf(fpt,"%s %d %d %d",header,&COLS,&ROWS,&Bytes);
  header[79] = fgetc(fpt);

  if(strcmp("P6",header) == 0)
  {
  image = (unsigned char *)malloc(3*ROWS*COLS); 
  displaydata = (unsigned char *)malloc(2*ROWS*COLS);

  fread(image,1,3*ROWS*COLS,fpt); 

  for(i=0,j=0;i<=3*ROWS*COLS;i+=3,j+=2)
  {
  *red = image[i] & 248;
  *green1 = image[i+1] & 224;
  *green2 = image[i+1] & 28;
  *blue = image[i+2] & 248;

  displaydata[j] = (*green2)<<3|(*blue)>>3;
  displaydata[j+1] = (*red)|(*green1)>>5;
  }

  fclose(fpt);
  printf("done !!\n");
  }

  else if(strcmp("P5",header) == 0)
   {
    image = (unsigned char *)malloc(ROWS*COLS);
    displaydata = (unsigned char *)malloc(2*ROWS*COLS);

    fread(image,1,ROWS*COLS,fpt); 

    for(i=0,j=0;i<=ROWS*COLS;i++,j+=2)
    {
    *red = image[i] & 248;
    *green1 = image[i] & 224;
    *green2 = image[i] & 28;
    *blue = image[i] & 248;

    displaydata[j] = (*green2)<<3|(*blue)>>3;
    displaydata[j+1] = (*red)|(*green1)>>5;
    }

    fclose(fpt);
    printf("done !!\n");
   }

  Monitor = XOpenDisplay(NULL);
   if (Monitor == NULL)
    {
    printf("Unable to open graphics display\n");
    exit(0);
    }

  ImageWindow = XCreateSimpleWindow(Monitor,RootWindow(Monitor,0),
		50,10, 
		COLS,ROWS,
		2, 	
		BlackPixel(Monitor,0),
		WhitePixel(Monitor,0));

  ImageGC = XCreateGC(Monitor,ImageWindow,0,NULL);

  XMapWindow(Monitor,ImageWindow);
  XFlush(Monitor);

  while(1)
   {
   XGetWindowAttributes(Monitor,ImageWindow,&Atts);
   if (Atts.map_state == IsViewable )
   break;
   }

  Picture = XCreateImage(Monitor,DefaultVisual(Monitor,0),
		DefaultDepth(Monitor,0),
		ZPixmap, 
		0,	 
		displaydata,
		COLS,ROWS, 
		16,  
		0); 

  XPutImage(Monitor, ImageWindow, ImageGC, Picture,
		0,0,0,0, 
		COLS,ROWS);

  XFlush(Monitor);
  sleep(2);		
  XCloseDisplay(Monitor);
    

  printf("continue? y or n\n");
  scanf("%s",ans);
   if(strcmp("n",ans) == 0)
    { 
    exit(0);
    }

 }
}