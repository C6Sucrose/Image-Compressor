#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <direct.h>
#include <cmath>


using namespace cv;
using namespace std;


struct node {
    int data;
    node* right;
    node* down;
};

// Define the 2D linked list class
class LinkedList {


public:
    node* head;
    int numRows;
    int numCols;

    LinkedList() {
        head = nullptr;
        numRows = 0;
        numCols = 0;
    }

    node* getNode(int rowNum, int index) {
        node* temp = new node;
        temp = head;

        while (rowNum > 0) {
            temp = temp->down;
            rowNum--;
        }

        while (index > 0) {
            temp = temp->right;
        }

        return temp;

    }

    // Insert a new node at the end of the row
    void insert(int data, int row) {
        node* newnode = new node;
        newnode->data = data;
        newnode->right = nullptr;
        newnode->down = nullptr;

        if (head == nullptr) {
            head = newnode;
            numCols = numRows = 1;
            return;
        }

        node* curr = head;
        node* prev = nullptr;

        // Traverse to the end of the row
        while (curr != nullptr && row > 0) {
            prev = curr;
            curr = curr->down;
            row--;
        }

        //If the row doesn't exist yet, create it
        if (curr == nullptr) {
            prev->down = newnode;
            numRows = numRows + 1;
            return;
        }
        numCols = 2;
        // Traverse to the end of the column
        while (curr->right != nullptr) {
            curr = curr->right;
            numCols = numCols + 1;
        }

        //Attach the new node to the end of the column
        curr->right = newnode;
        numCols = numCols + 1;

    }

    // Print the linked list
    void print() {
        node* currRow = head;
        while (currRow != nullptr) {
            node* currCol = currRow;
            while (currCol != nullptr) {
                cout << currCol->data << " ";
                currCol = currCol->right;
            }
            cout << endl;
            currRow = currRow->down;
        }
    }


    void convertTo2DLL(int** arr, int rows, int cols) {

        //int rows = img_read.rows;//sets the rows and cols of the image matrix
        //int cols = img_read.cols;








        for (int i = 0; i < rows; i++) {
            bool white_foun = false;
            bool black_foun = false;
            insert(i, i);//inserts the row number at the head of the row
            for (int j = 0; j < cols; j++) {
                if (arr[i][j] == 255) {//if white pixel is encountered
                    insert(j, i);//inserts the first index of white pixels
                    white_foun = true;
                    // skips all the indices in the middle
                    while (arr[i][j + 1] != 0 && j < cols - 1) {//move forwards until the next pixel is black
                        j++;
                    }

                    //last index 
                    insert(j, i);//inserts the last index of white pixels

                }

            }
            if (white_foun == true) {//only inserts the end if a white pixel is found
                insert(-2, i);//sets the end of white pixels
            }


            for (int j = 0; j < cols; j++) {//for loop checks for black pixels
                if (arr[i][j] == 0) {//if black pixel is encountered
                    insert(j, i);//inserts the first index of black pixels
                    black_foun = true;
                    while (arr[i][j + 1] != 255 && j < cols - 1) {//move forwards until the next pixel is white
                        j++;
                    }

                    insert(j, i);//inserts the last index of black pixels

                }
            }
            if (black_foun == true)//only runs if a black pixel is found
                insert(-1, i);//sets the end of black pixels
        }



    }

};



class QuadtreeNode {
public:
    int x, y, width, height;
    int color;
    bool isleaf;
    string name = "root";
    int designation = 0;
    string child_0_designation;
    string child_1_designation;
    string child_2_designation;
    string child_3_designation;
    QuadtreeNode* children[4];



    QuadtreeNode() {

    }

    QuadtreeNode(string);

    QuadtreeNode(int x, int y, int width, int height, node*& head) {
        // Determine the average color of the region
       /* int sum = 0;
        int count = 0;


        for (int i = x; i < x + width; i++) {
            for (int j = y; j < y + height; j++) {
                uchar pixel = img.at<uchar>(j, i);
                sum += pixel;
                count++;
            }
        }
        color = sum / count;*/


        //Main gist of checking for homogenity:
        //Create a for loop that traverese the rows included in the region
        //Create an inner for loop that traverses the columns of those rows
        //When you find a black or white index, check if that index is inside the region or not
        //if inside the region, bool will be set to true
        //if both bools are true together, the region is not homogeneous
        //else the region is homogeneous
        static int node_counter = 0;
        
        cout << endl << endl << endl << x << " " << y << " " << width + x << " " << y + height  << endl;
        cout << "Node " << node_counter << " is being checked" << endl;
        
        // Check if the region is homogeneous
        node* temp_down_cursor = head;//traverses the list vertically
        node* temp_right_cursor = new node;//traverses the list horizontally
        temp_right_cursor = head->right;
        bool white_holder = false;//checks for black and white indices in the list
        bool black_holder = false;
        bool isHomogeneous = true;
        int current_row = head->data;//stores the row number

        for (int i = y; i < y + height; i++) {//moves the cursor down

            cout << i << "-Down " << temp_right_cursor->data << "-Right" << "  LL-Row Num: "  << current_row << endl;
            cout << "height " << height + y << " width " << width + x << endl;

            //while(temp_right_cursor->right != nullptr) {//checks the columns
            int white_start = -3;
            int white_end = -3;
            int black_start = -4;
            int black_end = -4;
            node* temp_check_black_white = temp_right_cursor;

            while (temp_check_black_white->data != -2 && temp_check_black_white->data != -1) {//checks if the row has only black or only white or both
                temp_check_black_white = temp_check_black_white->right;
            }
            /*if ((x + width) % 3 == 0) {
                width = width + 1;
            }*/
            if (temp_check_black_white->data == -2) {//if white are found in the row
                while (temp_right_cursor->right != nullptr && temp_right_cursor->right->right != nullptr && temp_right_cursor->data != -2) {//checks the columns til -2
                    white_start = temp_right_cursor->data;
                    white_end = temp_right_cursor->right->data;
                    cout << "White Start : " << white_start << " White End : " << white_end << endl;
                    if ((white_start >= x && (white_start < width + x)) || (white_end >= x && white_end < width + x) || (white_start <= x && white_end >= width + x)) {
                        cout << "White True" << endl;
                        white_holder = true;
                    }
                    else {
                        cout << "White False" << endl;
                    }
                    temp_right_cursor = temp_right_cursor->right->right;
                }
            }

            if (temp_check_black_white->data == -2 && temp_check_black_white->right != nullptr) {//since its at -2, checks if there are black nodes after
                while (temp_check_black_white->data != -1) {
                    temp_check_black_white = temp_check_black_white->right;
                }
                if (temp_right_cursor->data == -2 && temp_right_cursor->right != nullptr) {//puts the column cursor at the start of the black nodes
                    temp_right_cursor = temp_right_cursor->right;
                }
            }



            if (temp_check_black_white->data == -1) {//checks for black nodes in the range
                while (temp_right_cursor->data != -1 && temp_right_cursor->right->right != nullptr) {//checks the columns til -1
                    black_start = temp_right_cursor->data;
                    black_end = temp_right_cursor->right->data;
                    cout << "Black Start : " << black_start << " Black End : " << black_end << endl;
                    if ((black_start >= x && black_start < width + x) || (black_end >= x && black_end < width + x) || (black_start <= x && black_end >= width + x)) {
                        cout << "Black True" << endl;
                        black_holder = true;
                    }
                    else {
                        cout << "Black False" << endl;
                    }
                    temp_right_cursor = temp_right_cursor->right->right;
                }
            }

            if (black_holder == true && white_holder == true) {//if both black and white indices are found in the region
                cout << "------------NOT HOMOGENEOUS----------" << endl;
                isHomogeneous = false;
                break;
            }

            //if (temp_right_cursor->right->right->data != -2) {
            //    white_start = temp_right_cursor->data;
            //    white_end = temp_right_cursor->right->data;
            //}

            //if (white_start > width || white_start < x) {//if white start index is out of the region boundary
            //    white_start = -3;
            //}
            //

            //if (white_start != -3) {//if white index is inside the region, set white check to true
            //    white_holder = true;
            //}


            

            //if (temp_right_cursor->data == -2) {//if white pixels are present, true check
            //    cout << "TRUE" << i << endl;
            //    white_holder = true;
            //}
            //if (temp_right_cursor->data == -1 && white_holder == true) {
            //    //if black pixels are present along with white pixels, region is grey
            //    isHomogeneous = false;
            //    cout << "FALSE" << endl;
            //    black_holder = true;
            //}
            //temp_right_cursor = temp_right_cursor->right;//move to the next column
            //if (isHomogeneous == false) {
            //    break;
            //}
       // }
            if (temp_down_cursor->down != nullptr) {
                temp_down_cursor = temp_down_cursor->down;
                temp_right_cursor = temp_down_cursor->right;//move to the next row
            }
        }

        if (isHomogeneous == true) {
            if (white_holder == true) {//if the region is white, the node is white
                this->color = 255;
            }
            else {//else the node is black
                this->color = 0;
            }
        }
        else {
            this->color = -1;//if the area is not homogeneous then set color to -1
        }
        this->x = x;
        this->y = y;
        this->width = x + width;
        this->height = y + height;
        

        /*for (int i = x; i < x + width; i++) {
            for (int j = y; j < y + height; j++) {
                uchar pixel = img.at<uchar>(j, i);
                if (pixel != color) {
                    isHomogeneous = false;
                    break;
                }
            }
            if (!isHomogeneous) {
                break;
            }
        }*/

        // If the region is not homogeneous, split it into four subregions

        if (isHomogeneous == false) {
            this->designation = node_counter;
            node_counter++;
            this->isleaf = false;//to be used in decoding, tells the traversal function when to stop
            cout << "_________________SPLIT_______________" << endl;
            int halfWidth = width / 2;//in case of values in points, takes the ceiling value in order to eliminate negative error
            int halfHeight = height / 2;

            children[0] = new QuadtreeNode(x, y, halfWidth, halfHeight, head);
            this->child_0_designation = "node" + to_string(this->children[0]->designation);

            children[1] = new QuadtreeNode(x + halfWidth, y, halfWidth, halfHeight, head);
            this->child_1_designation = "node" + to_string(this->children[1]->designation);


            node* temp_head = new node;
            temp_head = head;
            for (int i = y; i < y + halfHeight; i++) {//child 3 and child 4 start from the half point of a region so the head must be moved to the half point
                if (temp_head->down != nullptr) {
                    temp_head = temp_head->down;
                }
            }

            children[2] = new QuadtreeNode(x, y + halfHeight, halfWidth, halfHeight, temp_head);
            this->child_2_designation = "node" + to_string(this->children[2]->designation);


            children[3] = new QuadtreeNode(x + halfWidth, y + halfHeight, halfWidth, halfHeight, temp_head);
            this->child_3_designation = "node" + to_string(this->children[3]->designation);


            cout << this->child_0_designation << ", " << this->child_1_designation << ", " << this->child_2_designation << ", " << this->child_3_designation << endl;

            /*ofstream write("QuadTree\\" + this->name + to_string(this->designation) + ".txt");

            if (!write.is_open()) {
                cout << "File not open" << endl;
                return;
            }

            write << this->x << "," << this->y << "," << this->width << "," << this->height << "," << this->color << "," << this->children[0] << "," << this->children[1] << "," << this->children[2] << "," << this->children[3] << "," << endl;

            write.close();

            this->name = "node";
            this->designation = this->designation + 1;
            */

        }
        else {
            this->designation = node_counter;
            node_counter++;
            this->isleaf = true;
            cout << "HOMOGENEOUS" << endl;
            for (int i = 0; i < 4; i++) {
                children[i] = nullptr;
            }

            /*ofstream write("QuadTree\\" + this->name + to_string(this->designation) + ".txt");

            if (!write.is_open()) {
                cout << "File not open" << endl;
                return;
            }

            write << x << "," << y << "," << this->width << "," << this->height << "," << color  << endl;

            write.close();

            this->name = "node";
            this->designation = this->designation + 1;*/

        }
    }

    ~QuadtreeNode() {
        for (int i = 0; i < 4; i++) {
            delete children[i];
        }
    }


    void treeTraverse(string n, int d) {
        this->name = n;

        if (this->isleaf == true) {


            ofstream write("QuadTree\\" + this->name + to_string(this->designation) + ".txt");

            if (!write.is_open()) {
                cout << "File not open" << endl;
                return;
            }

            write << x << "," << y << "," << this->width << "," << this->height << "," << this->color << "," << this->isleaf << "," << this->designation << endl;

            write.close();



            return;
        }

        this->children[0]->treeTraverse("node", this->children[0]->designation);

        this->children[1]->treeTraverse("node", this->children[1]->designation);

        this->children[2]->treeTraverse("node", this->children[2]->designation);

        this->children[3]->treeTraverse("node", this->children[3]->designation);



        ofstream write("QuadTree\\" + this->name + to_string(this->designation) + ".txt");

        if (!write.is_open()) {
            cout << "File not open" << endl;
            return;
        }

        write << this->x << "," << this->y << "," << this->width << "," << this->height << "," << this->color << "," << this->isleaf << "," << this->child_0_designation << "," << this->child_1_designation << "," << this->child_2_designation << "," << this->child_3_designation << endl;

        write.close();

    }




    void draw(Mat& img) {
        // If the node has children, draw them recursively
        if (children[0] != nullptr) {
            for (int i = 0; i < 4; i++) {
                children[i]->draw(img);
            }
        }
        // Otherwise, draw a rectangle with the average color of the region
        else {
            rectangle(img, Rect(x, y, width, height), Scalar(color), FILLED);
        }
    }





};

int main() {


    LinkedList test;
    string read_img;
    cout << "MAKE SURE THE IMAGE NAME IS CORRECT AND THE FILE EXTENSION MUST BE INCLUDED(i.e .bmp)" << endl;
    cout << "Enter Image Name: ";
    cin >> read_img;
    cout << endl;
    

    // Read the input image
    Mat img = imread(read_img, IMREAD_GRAYSCALE);
    if (img.empty()) {
        cerr << "Error: Could not read image file." << endl;
        return -1;
    }


    int rows = img.rows;
    int cols = img.cols;

    int** img_DMA = new int* [rows];

    for (int i = 0; i < rows; i++) {//created DMA
        img_DMA[i] = new int[cols];
    }


    for (int i = 0; i < rows; i++) {//added the pixel values to DMA
        //int count = 0;
       // cout << "Row : " << i << endl;
        for (int j = 0; j < cols; j++) {

            img_DMA[i][j] = img.at<uchar>(i, j);
            //count++;

            //cout << img_DMA[i][j] << " ";
            //img_DMA[i][j] = img_read.at<uchar>(i, j);
        }
        //cout << endl;
    }

    //cout << endl << endl;

    /*int** img_DMA = new int*[8];
    for (int i = 0; i < 8; i++) {
        img_DMA[i] = new int[8];
    }

    img_DMA[0][0] = 0;
    img_DMA[0][1] = 0;
    img_DMA[1][0] = 0;
    img_DMA[1][1] = 0;
    img_DMA[1][2] = 0;
    img_DMA[1][3] = 0;
    img_DMA[2][0] = 0;
    img_DMA[2][1] = 0;
    img_DMA[3][0] = 0;
    img_DMA[3][1] = 0;
    img_DMA[4][0] = 0;
    img_DMA[4][1] = 0;
    img_DMA[4][2] = 0;
    img_DMA[4][3] = 0;
    img_DMA[4][4] = 0;
    img_DMA[4][5] = 0;
    img_DMA[5][0] = 0;
    img_DMA[5][1] = 0;
    img_DMA[5][2] = 0;
    img_DMA[5][3] = 0;
    img_DMA[5][4] = 0;
    img_DMA[5][5] = 0;
    img_DMA[6][0] = 0;
    img_DMA[6][1] = 0;
    img_DMA[6][2] = 0;
    img_DMA[6][3] = 0;
    img_DMA[6][4] = 0;
    img_DMA[6][5] = 0;
    img_DMA[6][6] = 0;
    img_DMA[6][7] = 0;
    img_DMA[7][0] = 0;
    img_DMA[7][1] = 0;
    img_DMA[7][2] = 0;
    img_DMA[7][3] = 0;
    img_DMA[7][4] = 0;
    img_DMA[7][5] = 0;
    img_DMA[7][6] = 0;
    img_DMA[7][7] = 0;
    img_DMA[0][2] = 255;
    img_DMA[0][3] = 255;
    img_DMA[0][4] = 255;
    img_DMA[0][5] = 255;
    img_DMA[0][6] = 255;
    img_DMA[0][7] = 255;
    img_DMA[1][4] = 255;
    img_DMA[1][5] = 255;
    img_DMA[1][6] = 255;
    img_DMA[1][7] = 255;
    img_DMA[2][2] = 255;
    img_DMA[2][3] = 255;
    img_DMA[2][4] = 255;
    img_DMA[2][5] = 255;
    img_DMA[2][6] = 255;
    img_DMA[2][7] = 255;
    img_DMA[3][2] = 255;
    img_DMA[3][3] = 255;
    img_DMA[3][4] = 255;
    img_DMA[3][5] = 255;
    img_DMA[3][6] = 255;
    img_DMA[3][7] = 255;
    img_DMA[4][6] = 255;
    img_DMA[4][7] = 255;
    img_DMA[5][6] = 255;
    img_DMA[5][7] = 255;

    
    int rows = 8;
    int cols = 8;*/
    // test.print();
     //Mat img = imread("image.png", IMREAD_GRAYSCALE);
     //LinkedList list;
    test.convertTo2DLL(img_DMA, rows, cols);
    test.numRows = rows;
    test.numCols = cols;
    cout << test.numRows << endl << test.numCols << endl;
    test.print();
    //QuadTreeNode* root = buildQuadTree(test, 0, 7);
    //printQuadTree(root);

    //creating folder for node files
    //_mkdir("QuadTree");
    //
    //// Initialize the quadtree root node

    QuadtreeNode* root = new QuadtreeNode(0, 0, cols, rows, test.head);

    root->treeTraverse("0_root", 0);

    Mat canvas = Mat::zeros(rows, cols,IMREAD_GRAYSCALE);
    root->draw(canvas);

    namedWindow("Original", WINDOW_NORMAL);
    namedWindow("Quadtree", WINDOW_NORMAL);
    imshow("Original", img);
    imshow("Quadtree", canvas);
    waitKey(0);



    //// Draw the quadtree on a black canvas
    

    //// Display the original image and the quadtree
    //namedWindow("Original", WINDOW_NORMAL);
    //namedWindow("Quadtree", WINDOW_NORMAL);
    //imshow("Original", img);
    //imshow("Quadtree", canvas);
    //waitKey(0);

    //// Clean up
    //delete root;
    return 0;
}

























