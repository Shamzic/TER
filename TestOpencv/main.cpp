#include <stdio.h>
#include <stdlib.h>
#include <opencv/highgui.h>

int main (int argc, char* argv[])
{
  IplImage* img = NULL; 
  const char* window_title = "Hello, OpenCV!";

  /* Vérification: au moins un argument doit être passé au programme.*/
  if (argc < 2)
  {
    fprintf (stderr, "usage: %s IMAGE\n", argv[0]);
    return EXIT_FAILURE;
  }

  /* Chargement de l'image passée en argument */
  img = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);

  if (!img)
  {
    fprintf (stderr, "couldn't open image file: %s\n", argv[1]);
    return EXIT_FAILURE;
  }

  /* Création d'une fenêtre intitulée "Hello, OpenCV!" */
  cvNamedWindow (window_title, CV_WINDOW_AUTOSIZE);

  /* Affichage de l'image */
  cvShowImage (window_title, img);

  /* Pause le temps que l'utilisateur appuie sur une touche */
  cvWaitKey (0);

  /* Destruction de la fenêtre */
  cvDestroyAllWindows ();
  
  /* Libération de la mémoire */
  cvReleaseImage (&img);

  return EXIT_SUCCESS;
}
