Weed Detection System
=====================

An **Android app** for **real-time weed detection** in agriculture using **YOLOv8**. This project is designed to streamline weed management processes, offering precise detection and deployment through modern machine learning and Android technologies.

Overview
--------

This project is divided into three parts:

1.  **Labeling Data** (using OpenCV).
    
2.  **Training the Model** (using YOLOv8).
    
3.  **Deploying the Model** (to an Android app using Kotlin).
    

Part 1: Labeling Data
---------------------

To label your data for model training, follow these steps:

1.  Navigate to the data-labeling-opencv folder in the repository.
    
2.  Use the labeling tool provided in this folder to annotate your images.

3.  This tool supports bounding box annotation for your dataset in YOLO format.
    

**Tool Features:**

*   Developed in C++ with OpenCV.
    
*   Allows users to draw bounding boxes around weeds.
    
*   Exports labels in YOLO-compatible format.
    

[Folder : data-labeling-opencv](https://github.com/samir5636/weeds-detection/tree/main/data-labeling-opencv)

Part 2: Training the Model
--------------------------

To train the YOLOv8 model with your custom data:

1.  Navigate to the [Weeds-Detection.ipynb](https://github.com/samir5636/weeds-detection/blob/main/Weeds-Detection.ipynb)file in the root of the repository.
    
2.  Open the notebook and follow the step-by-step guide provided.
    
3.  Customize the YOLO model with your labeled data from **Part 1**.
    

**Steps in the Notebook:**

*   Data preparation and formatting.
    
*   Training the YOLOv8 model with your dataset.
    
*   Evaluating model performance and exporting the trained model.
    

**Output:**After training, export the model as a **TensorFlow Lite** (.tflite) file for deployment.

Part 3: Deploying to an Android App
-----------------------------------

To integrate the trained model into the Android app:
1.  Open android_app folder.
2. Put your .tflite model and .txt label file inside the assets folder. You can find assets folder at this location: android_app\android_app\app\src\main\assets
3. Rename paths of your model and labels file in Constants.kt file. You can find Constants.kt at this location: android_app\android_app\app\src\main\java\com\surendramaran\yolov8tflite
4. Download and install Android Studio from the official website (https://developer.android.com/studio)
5. Once installed, open Android Studio from your applications menu.
When Android Studio opens, you'll see a welcome screen. Here, you'll find options to create a new project, open an existing project, or check out project from version control.Since you already have a project, click on "Open an existing Android Studio project".
6. Navigate to the directory where your project is located and select the project's root folder.


**Features of the App:**

*   Real-time weed detection using the trained YOLOv8 model.
    
*   User-friendly interface built with Kotlin.
    
*   Supports integration with TensorFlow Lite for efficient inference on mobile devices.
    

Prerequisites
-------------

### General Requirements

*   Python 3.8+ (for YOLOv8 training).
    
*   OpenCV 4.6+ (for data labeling).
    
*   Android Studio, Vscode (for app development).
    

### Libraries and Frameworks

*   **YOLOv8** (Ultralytics).
    
*   TensorFlow Lite.
    
How to Contribute
-----------------

We welcome contributions! To contribute:

1.  Fork this repository.
    
2.  Create a new branch for your feature/bug fix.
    
3.  Submit a pull request.
    

Author
------

Developed by **Samir Ziani**. Feel free to connect on [GitHub](https://github.com/samir5636).

License
-------

This project is licensed under the MIT License. See the [LICENSE](https://github.com/samir5636/weeds-detection/blob/main/LICENSE)file for details.

Let me know if you’d like further tweaks! 😊