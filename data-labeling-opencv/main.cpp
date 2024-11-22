#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>

using namespace std;
using namespace cv;

namespace fs = std::filesystem;

// Global variables
bool drawing = false;
vector<tuple<int, int, int, int, int>> boxes; // Stores (x_min, y_min, x_max, y_max, class)
Mat image, img_copy;
string image_name;
const string output_dir = "labels";
const string processed_dir = "already_labeled";
int current_class = 0; // Default class label
int ix = -1, iy = -1;

const int RESIZE_DIM = 512; // Resizing dimension

void redrawImage() {
    img_copy = image.clone();
    for (auto &box : boxes) {
        int x_min, y_min, x_max, y_max, cls;
        tie(x_min, y_min, x_max, y_max, cls) = box;
        Scalar color = (cls == 0) ? Scalar(0, 255, 0) : Scalar(0, 0, 255);
        rectangle(img_copy, Point(x_min, y_min), Point(x_max, y_max), color, 2);
    }
    imshow("Image", img_copy);
}

void mouseCallback(int event, int x, int y, int, void *) {
    if (event == EVENT_LBUTTONDOWN) {
        drawing = true;
        ix = x;
        iy = y;
    } else if (event == EVENT_MOUSEMOVE && drawing) {
        img_copy = image.clone();
        Scalar color = (current_class == 0) ? Scalar(0, 255, 0) : Scalar(0, 0, 255);
        rectangle(img_copy, Point(ix, iy), Point(x, y), color, 2);
        imshow("Image", img_copy);
    } else if (event == EVENT_LBUTTONUP) {
        drawing = false;
        int x_min = min(ix, x), y_min = min(iy, y);
        int x_max = max(ix, x), y_max = max(iy, y);
        boxes.emplace_back(x_min, y_min, x_max, y_max, current_class);
        redrawImage();
    }
}

void saveLabels(int image_width, int image_height) {
    if (!fs::exists(output_dir))
        fs::create_directories(output_dir);
    if (!fs::exists(processed_dir))
        fs::create_directories(processed_dir);

    string label_path = output_dir + "/" + image_name.substr(0, image_name.find_last_of('.')) + ".txt";
    ofstream label_file(label_path);
    if (label_file.is_open()) {
        for (auto &box : boxes) {
            int x_min, y_min, x_max, y_max, cls;
            tie(x_min, y_min, x_max, y_max, cls) = box;

            // Convert to YOLO format
            float x_center = (x_min + x_max) / 2.0 / image_width;
            float y_center = (y_min + y_max) / 2.0 / image_height;
            float width = (x_max - x_min) / (float)image_width;
            float height = (y_max - y_min) / (float)image_height;
            label_file << cls << " " << x_center << " " << y_center << " " << width << " " << height << endl;
        }
        label_file.close();
        cout << "Labels saved to " << label_path << endl;
    }

    // Save resized image in "already_labeled" directory
    string processed_path = processed_dir + "/" + image_name;
    imwrite(processed_path, image);
    cout << "Resized image saved to " << processed_path << endl;
}

bool hasExtension(const string &path, const vector<string> &extensions) {
    for (const auto &ext : extensions) {
        if (path.size() >= ext.size() && path.compare(path.size() - ext.size(), ext.size(), ext) == 0) {
            return true;
        }
    }
    return false;
}

int main() {
    string image_dir = "data/images";
    vector<string> image_files;
    vector<string> valid_extensions = {".png", ".jpg"};

    for (const auto &entry : fs::directory_iterator(image_dir)) {
        string path = entry.path().string();
        if (hasExtension(path, valid_extensions))
            image_files.push_back(path);
    }

    for (const auto &img_path : image_files) {
        image_name = fs::path(img_path).filename().string();
        boxes.clear();

        Mat orig_image = imread(img_path);
        if (orig_image.empty()) {
            cerr << "Failed to load " << img_path << endl;
            continue;
        }

        // Resize the image
        resize(orig_image, image, Size(RESIZE_DIM, RESIZE_DIM));
        int image_height = image.rows;
        int image_width = image.cols;

        img_copy = image.clone();
        namedWindow("Image");
        setMouseCallback("Image", mouseCallback);

        imshow("Image", image);

        while (true) {
            int key = waitKey(1) & 0xFF;

            if (key == '0') {
                current_class = 0;
                cout << "Set class to 0 (Green)" << endl;
            } else if (key == '1') {
                current_class = 1;
                cout << "Set class to 1 (Red)" << endl;
            } else if (key == 's') {
                saveLabels(image_width, image_height);
                break;
            } else if (key == 'q') {
                cout << "Exiting..." << endl;
                return 0;
            } else if (key == 27) { // Esc key
                cout << "Skipping image..." << endl;
                break;
            }
        }
        destroyAllWindows();
    }

    return 0;
}
