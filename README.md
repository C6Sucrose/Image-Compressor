# Image Compressor
This project consists of 3 programs that perform image compression, decompression and similarity calculation on grayscale bitmap (.bmp) images:

* Image Compressor
* Image Decompressor
* Similarity Calculator

# Image Compressor
The image compressor takes a grayscale .bmp image as input and compresses it using a quadtree compression algorithm. It works by:

* Dividing the image into linked lists to track the color of each row
* Constructing a quadtree from the image and its linked lists
* Storing the quadtree nodes in a "QuadTree" folder

The compressor has been tested on grayscale images and should work for color images as well. Compression results in some quality loss.

# Image Decompressor
The decompressor reads the quadtree node files stored in the "QuadTree" folder and reconstructs the compressed image. It saves the decompressed image as "new_image.bmp".

# Similarity Calculator
The similarity calculator takes two images as input and calculates their similarity score from 0-100 using the Dice coefficient metric. A score of 100 indicates identical images while 0 indicates no similarity.

# Instructions
* Place a grayscale .bmp image in the project directory
* Run the Image Compressor
* Run the Image Decompressor
* Run the Similarity Calculator, providing the original and decompressed image names

### The decompressed image will be in "new_image.bmp"

The similarity score will indicate compression quality loss

The programs have only been tested on grayscale images so far. Let me know if you have any other questions!
