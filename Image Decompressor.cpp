#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <direct.h>
#include <cmath>
#include <filesystem>
#include "Image Compressor.cpp"

using namespace cv;
using namespace std;
using namespace std::filesystem;






QuadtreeNode::QuadtreeNode(string path)
{
    

            cout << "Reading File " <<  path << endl;

           

            ifstream read(path);

            string holder;
                
            /*if (!read.is_open()) {
                cout << "Couldnt open file" << endl;
                return;
            }*/
                
            
                getline(read, holder, ',');
                int x = stoi(holder);
                getline(read, holder, ',');
                int y = stoi(holder);
                getline(read, holder, ',');
                int width = stoi(holder);
                getline(read, holder, ',');
                int height = stoi(holder);
                getline(read, holder, ',');
                int color = stoi(holder);
                getline(read, holder, ',');
                int isLeaf = stoi(holder);

                cout << x << " " << y << " " << width << " " << height << " " << color << " " << isLeaf << endl;

            
            
                if(isLeaf != 1)
                {   

                    this->x = x;
                    this->y = y;
                    this->width = width;
                    this->height = height;
                    this->color = color;
                    this->isleaf = isLeaf;
                    
                    getline(read, holder, ',');

                    cout << holder << endl;
                    
                    this->children[0] = new QuadtreeNode("QuadTree\\" + holder + ".txt");

                    getline(read, holder, ',');

                    //cout << holder << endl;

                    this->children[1] = new QuadtreeNode("QuadTree\\" + holder + ".txt");

                    getline(read, holder, ',');

                    cout << holder << endl;

                    this->children[2] = new QuadtreeNode("QuadTree\\" + holder + ".txt");

                    getline(read, holder);

                    cout << holder << endl;

                    this->children[3] = new QuadtreeNode("QuadTree\\" + holder + ".txt");



                    
                }   
                else {
                    std::cout << "Base condition reached in file " << path << '\n';



                    this->x = x;
                    this->y = y;
                    this->width = width;
                    this->height = height;
                    this->color = color;
                    this->isleaf = isLeaf;

                    for (int i = 0; i < 4; i++) {
                        this->children[i] = nullptr;
                    }

                    
                }


        
    
}

void tree_traversal(int**& img_arr, QuadtreeNode* node) {
    if (node->isleaf == 1) {
        int h = node->height;
        int w = node->width;
        if (node->height < 200 && node->width < 200) {
            h = node->height + 1;
            w = node->width + 1;
        }
        for (int i = node->y; i < h; i++) {
            for (int j = node->x; j < w; j++) {
                img_arr[i][j] = node->color;
            }
        }
        /*img_arr[node->y][node->x] = node->color;
        img_arr[node->width][node->height] = node->color;*/
        return;
    }

    tree_traversal(img_arr, node->children[0]);

    tree_traversal(img_arr, node->children[1]);

    tree_traversal(img_arr, node->children[2]);

    tree_traversal(img_arr, node->children[3]);


}


int main()
{
    string path = "QuadTree\\0_root0.txt";
    if (path == NULL) {
        cout << endl << "ERROR : QUADTREE PATH NOT FOUND!!!! \n EXITING!!!!" << endl;
        
        return 0;
    }
    QuadtreeNode* root = new QuadtreeNode(path);

   

    int rows = root->width;
    int cols = root->height;

    int** new_DMA = new int* [rows];

    for (int i = 0; i < rows; i++) {
        new_DMA[i] = new int[cols];
    }



    tree_traversal(new_DMA, root);

    /*for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << new_DMA[i][j] << " ";
        }
        cout << endl;
    }*/
    
    

    Mat image(root->width, root->height, IMREAD_GRAYSCALE);


    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            image.at<uchar>(i, j) = 255;
        }
    }


    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            image.at<uchar>(i, j) = new_DMA[i][j];
        }
    }

    imwrite("new_image.bmp", image);

   /* Mat canvas = Mat::zeros(200, 200, IMREAD_GRAYSCALE);
    root->draw(canvas);

     imshow("Quadtree", canvas);

     waitKey(0);*/

    return 0;
}




