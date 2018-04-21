
****************
* INTRODUCTION *
****************

Thank you for downloading this authentic/spliced image dataset. The goal of this dataset is to provide a benchmark set that people can compare algorithms on. It was created for trustworthy/tampered photo detection, but its use is not just limited to this application.

In this dataset, there are 2 directories: 4cam_auth & 4cam_splc. 4cam_auth containts authentic images, and 4cam_splc contains spliced images. By the term 'authentic', we mean an image that is taken using just one camera.

In 4cam_auth, there are 183 images, and in 4cam_splc, there are 180. The image sizes range from 757x568 to 1152x768 and are uncompressed. The spliced images are created using the authentic images, without any post processing. Full EXIF information is included in authentic images.

The images are mostly indoor scenes: labs, desks, books ...etc. Only 27 images, or 15%, are taken outdoors on a cloudy day (which makes the outdoor illumination similar to indoor conditions).

************************
* FILENAME CONVENTIONS *
************************

The filename conventions are as follows:

---- AUTHENTIC IMAGES ----

(camera_string)_(image_index)_sub_(sub_index).tif

There are 4 cameras: 'canong3', 'nikond70', 'canonxt', 'kodakdcs330'.

canong3 has image #2 to image #8. Because we took images with bracketing (different exposures) and we are only using 1 exposure here, the image indices are 2, 5, and 8.

nikond70 has 2, 5, 8, and 11.

canonxt has 2, 5, 8, 11, ..., 38.

kodakdcs330 is an exception. It has image indices 1, 2, and 3.

sub_index ranges from 01~09 (an originally bigger image was broken down to 9 small images), except for kodakdcs330, where sub_index = 01 always.

---- SPLICED IMAGES ----

(camera1_string)_(camera2_string)_sub_(sub_index).tif

There are 4x3/2 = 6 camera pairs, times 30 sub indices for each pair, we get 180 images in total.

---- EDGEMASKS ----

Under each directory there is a sub directory storing edgemasks for test images. These edgemasks label regions within each image, indicating them as parts that come from different cameras.

There are 4 colors in the edgemask images: bright red (255,0,0), bright green (0,255,0), regular red (200,0,0), and regular green (0,200,0).

Bright red indicates the part near the suspicious splicing boundary that comes from camera 1.
Bright green indicates the part near the suspicious splicing boundary that comes from camera 2.
Regular red indicates the part far from the suspicious splicing boundary that comes from camera 1.
Regular green indicates the part far from the suspicious splicing boundary that comes from camera 2.

**********************************
* ADDITIONAL INFORMATION & LINKS *
**********************************

1. Any questions/suggestions feel free to contact

Yu-Feng Hsu [yfhsu@ee.columbia.edu]
Prof Shih-Fu Chang [sfchang@ee.columbia.edu]

2. Please kindly cite our ICME 2006 paper for your use of this dataset:

Yu-Feng Hsu, Shih-Fu Chang. Detecting Image Splicing Using Geometry Invariants And Camera Characteristics Consistency. In Interational Conference on Multimedia and Expo (ICME), Toronto, Canada, July 2006

3. We also have another authentic/spliced image dataset. This set contains grayscale images collected over the internet and are of smaller sizes (128x128). It suits your need if you want to test algorithms of a more general purpose as the image content is more diverse. The drawback is that the EXIF data is not stored. The set is available for download at

http://www.ee.columbia.edu/dvmm/downloads/AuthSplicedDataSet/AuthSplicedDataSet.htm

4. Please visit our website for further information on Trustfoto project:

http://www.ee.columbia.edu/dvmm/trustfoto/


*********************
* THANKS AND ENJOY!!*
*********************