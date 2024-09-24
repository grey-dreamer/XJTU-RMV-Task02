//    ./OpenCV_Project
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;
void greyimage(Mat image);
void HSVimage(Mat image);
void filteredimage(Mat image);
void Gaussimage(Mat image);
void RedFeature(Mat image);
void Highlight(Mat image);
void Draw(Mat image);
void Process(Mat image);

int main()
{
// 加载图片
    Mat image = imread("../resources/test_image.png");
    if (image.empty()){
        cout << "Could not open or find the image" << endl;
        return -1;
    }
// 图像颜色空间转换
    // greyimage(image); // 灰度图
    // HSVimage(image); // HSV 

// 滤波操作
    // filteredimage(image); // 均值滤波
    // Gaussimage(image); // 高斯滤波 
    
// 特征提取
    // RedFeature(image); // 红色区域提取与操作
    // Highlight(image); // 高亮区域及图形化处理

// 图像绘制(轮廓与bounding box在特征提取时已绘制，见4.2, 4.3)
    // Draw(image); // 蓝圆(填充)，绿矩形(不填充)，红字Text

// 图像处理(旋转与裁剪)
    Process(image);

    return 0;
}

void greyimage(Mat image)
{
    // 创建一个新Mat对象来存储灰度图像，然后转换彩色图像(也可直接读取灰度图像)
    Mat grayImage;
    cvtColor(image, grayImage, COLOR_BGR2GRAY);
    // 展示并保存图片
    namedWindow("Grey Image", WINDOW_NORMAL);
    resizeWindow("Grey Image", 800, 800);
    imshow("Grey Image", grayImage);
    waitKey(0);
    imwrite("../resources/1.1Grey_Image.png", grayImage);
}

void HSVimage(Mat image)
{
    Mat HSVImage;
    cvtColor(image, HSVImage, COLOR_BGR2HSV);
    namedWindow("HSV Image", WINDOW_NORMAL);
    resizeWindow("HSV Image", 800, 800);
    imshow("HSV Image", HSVImage);
    waitKey(0);
    imwrite("../resources/1.2HSV_Image.png", HSVImage);
}

void filteredimage(Mat image)
{
    Mat filtered_image;
    // blur函数或boxFilter函数
    blur(image, filtered_image, Size(5, 5)); // 选用5x5的核
    namedWindow("filtered_image", WINDOW_NORMAL);
    resizeWindow("filtered_image", 800, 800);
    imshow("filtered_image", filtered_image);
    waitKey(0);
    imwrite("../resources/2.1filtered_image.png", filtered_image);
}

void Gaussimage(Mat image)
{
    Mat Gauss_image;
    GaussianBlur(image, Gauss_image, Size(5, 5), 0, 0); // 0表示自动计算sigma大小
    namedWindow("Gauss_image", WINDOW_NORMAL);
    resizeWindow("Gauss_image", 800, 800);
    imshow("Gauss_image", Gauss_image);
    waitKey(0);
    imwrite("../resources/2.2Gauss_image.png", Gauss_image);
}

void RedFeature(Mat image)
{
// 提取红色颜色区域
    Mat hsvimage;
    cvtColor(image, hsvimage, COLOR_BGR2HSV); // 转为HSV图片
    // 定义红色的HSV范围
    Scalar lower_red1(0, 100, 100);
    Scalar upper_red1(10, 255, 255);
    Scalar lower_red2(160, 100, 100);
    Scalar upper_red2(180, 255, 255);
    // 创建掩膜
    Mat mask1, mask2, redarea;
    inRange(hsvimage, lower_red1, upper_red1, mask1);
    inRange(hsvimage, lower_red2, upper_red2, mask2);
    bitwise_or(mask1, mask2, redarea); // 合并
    // 展示
    namedWindow("redarea", WINDOW_NORMAL);resizeWindow("redarea", 800, 800);
    imshow("redarea", redarea);waitKey(0);
    imwrite("../resources/3.1redarea.png", redarea);

// 红色外轮廓
    // 寻找轮廓
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(redarea, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    // 创建一个空白图像来绘制轮廓
    Mat contour_image = Mat::zeros(image.size(), CV_8UC3);
    // 计算并输出轮廓面积
    for (size_t i = 0; i < contours.size(); ++i) {
        double area = contourArea(contours[i]);
        if(area!=0){
            cout << "Contour " << i << " area: " << area << endl;
        }
        // 绘制每个轮廓
        drawContours(contour_image, contours, i, Scalar(0, 0, 255), 2);
    }
    // 展现并保存红色轮廓
    namedWindow("redcontour", WINDOW_NORMAL);resizeWindow("redcontour", 800, 800);
    imshow("redcontour", contour_image);waitKey(0);
    imwrite("../resources/4.2redcontour.png", contour_image);

// 红色bounding box
    Mat boximage=image;
    for (size_t i = 0; i < contours.size(); ++i) {
        // 计算边界框
        Rect bounding_box = boundingRect(contours[i]);
        // 绘制边界框
        rectangle(boximage, bounding_box, Scalar(0, 255, 0), 2);
        // 输出边界框信息
        cout << "Bounding Box " << i << ": x=" << bounding_box.x
             << ", y=" << bounding_box.y
             << ", width=" << bounding_box.width
             << ", height=" << bounding_box.height << endl;
    }
    namedWindow("boximage", WINDOW_NORMAL);resizeWindow("boximage", 800, 800);
    imshow("boximage", boximage);waitKey(0);
    imwrite("../resources/4.3boximage.png", boximage);

    destroyAllWindows();
}

void Highlight(Mat image)
{
    // 将BGR图像转换为灰度图像
    Mat gray_image;
    cvtColor(image, gray_image, COLOR_BGR2GRAY);
    namedWindow("gray_image", WINDOW_NORMAL);resizeWindow("gray_image", 800, 800);
    imshow("gray_image", gray_image);waitKey(0);
    // 提取高亮区域并二值化
    threshold(gray_image, gray_image, 160, 255, THRESH_BINARY); // 定义160以上为高亮
    namedWindow("Highlight", WINDOW_NORMAL);resizeWindow("Highlight", 800, 800);
    imshow("Highlight", gray_image);waitKey(0);imwrite("../resources/3.2Highlight.png", gray_image);
    // 定义腐蚀和膨胀的核
    Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
    // 腐蚀
    erode(gray_image, gray_image, kernel, Point(-1, -1), 1);
    namedWindow("eroded_image", WINDOW_NORMAL);resizeWindow("eroded_image", 800, 800);
    imshow("eroded_image", gray_image);waitKey(0);imwrite("../resources/3.3eroded_image.png", gray_image);
    // 膨胀
    dilate(gray_image, gray_image, kernel, Point(-1, -1), 1);
    namedWindow("dilated_image", WINDOW_NORMAL);resizeWindow("dilated_image", 800, 800);
    imshow("dilated_image", gray_image);waitKey(0);imwrite("../resources/3.4dilated_image.png", gray_image);
    // 漫水处理(漫水区域颜色设为灰色)
    Point start = Point(gray_image.cols / 2, gray_image.rows / 2);  // 中心点作为起始点
    Mat flood_filled_image;
    gray_image.copyTo(flood_filled_image);
    int loDiff = 3;  // 低差异
    int upDiff = 3;  // 高差异
    floodFill(flood_filled_image, start, Scalar(128), nullptr, Scalar(loDiff), Scalar(upDiff), 4 | (255 << 8) | FLOODFILL_FIXED_RANGE); // 限定漫水范围可将nullptr换为&Rect()参数
    namedWindow("flood_filled_image", WINDOW_NORMAL);resizeWindow("flood_filled_image", 800, 800);
    imshow("flood_filled_image", flood_filled_image);waitKey(0);imwrite("../resources/3.5flood_filled_image.png", flood_filled_image);

    destroyAllWindows();
}

void Draw(Mat image)
{
    Scalar red(0, 0, 255);   // BGR格式颜色
    Scalar green(0, 255, 0);
    Scalar blue(255, 0, 0);

    // 绘制圆形
    circle(image, Point(150, 200), 100, blue, -1); // Point为位置，100为半径，-1表示填充(否则为轮廓)

    // 绘制矩形
    Rect rect(1050, 200, 300, 200); // (x, y, width, height)
    rectangle(image, rect, green, 30); // 线条粗细为30，不填充

    // 添加文字
    string text = "Text";
    int font_face = FONT_HERSHEY_SIMPLEX; // 字体
    double font_scale = 20; // 字号大小，1.0时为默认值
    int thickness = 30; // 字体线条粗细
    int baseline = 0; // 无下划线
    Size text_size = getTextSize(text, font_face, font_scale, thickness, &baseline);
    Point org((image.cols - text_size.width) / 2, (image.rows + text_size.height) / 2); // 居中
    putText(image, text, org, font_face, font_scale, red, thickness, LINE_AA);

    namedWindow("drawn_image", WINDOW_NORMAL);resizeWindow("drawn_image", 800, 800);
    imshow("drawn_image", image);waitKey(0);imwrite("../resources/4.1drawn_image.png", image);
}

void Process(Mat image)
{
// 图像旋转
    double angle = 35.0;  // 旋转角度
    double scale = 1.0;   // 缩放因子
    Point center(image.cols / 2, image.rows / 2);  // 旋转中心点
    // 旋转矩阵
    Mat rotation_matrix = getRotationMatrix2D(center, angle, scale);
    // 计算旋转后的新边界
    Rect bbox = RotatedRect(center, image.size(), angle).boundingRect();
    // 调整旋转矩阵以考虑新的边界
    rotation_matrix.at<double>(0, 2) += bbox.width / 2.0 - center.x;
    rotation_matrix.at<double>(1, 2) += bbox.height / 2.0 - center.y;
    // 创建旋转后的图像
    Mat rotated_image;
    warpAffine(image, rotated_image, rotation_matrix, bbox.size());
    namedWindow("Rotated Image", WINDOW_NORMAL);resizeWindow("Rotated Image", 800, 800);
    imshow("Rotated Image", rotated_image);waitKey(0);imwrite("../resources/5.1rotated_image.png", rotated_image);

// 图像裁剪
    Rect roi(0, 0, image.cols/2, image.rows/2);  // 定义感兴趣区域 (x, y, width, height)
    Mat cropped_image = image(roi);
    namedWindow("Cropped Image", WINDOW_NORMAL);resizeWindow("Cropped Image", 800, 800);
    imshow("Cropped Image", cropped_image);waitKey(0);imwrite("../resources/5.2cropped_image.png", cropped_image);
}