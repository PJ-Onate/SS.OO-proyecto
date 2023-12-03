#include <iostream>
#include <opencv2/opencv.hpp>
#include <chrono>
#include <omp.h>

using namespace cv;
using namespace std;
using namespace chrono;

void processSectionOpenMP(Mat& image, int startRow, int endRow) {
    #pragma omp parallel for
    for (int r = startRow; r < endRow; r++) {
        for (int c = 0; c < image.cols; c++) {
            Point3_<uchar>* p = image.ptr<Point3_<uchar>>(r, c);
            uchar grayValue = static_cast<uchar>(0.299 * p->z + 0.587 * p->y + 0.114 * p->x);
            p->x = p->y = p->z = grayValue;
        }
    }
}

int main(int argc, char** argv) {
    if (argc != 3) {
        cout << "Uso: " << argv[0] << " <imagen_entrada> <numero_hebras>" << endl;
        return -1;
    }

    string inputImageName = argv[1];
    int numThreads = atoi(argv[2]);

    auto startLoading = high_resolution_clock::now();
    cout << "Cargando Imagen..." << endl;

    Mat image = imread("20231129-110000234_id40115361.png", IMREAD_COLOR);

    if (image.empty()) {
        cout << "Error al cargar la imagen." << endl;
        return -1;
    } else {
        cout << "Imagen Cargada" << endl;
    }

    auto stopLoading = high_resolution_clock::now();
    auto loadingDuration = duration_cast<milliseconds>(stopLoading - startLoading);
    cout << "Tiempo de carga de imagen: " << loadingDuration.count() << " ms" << endl;

    cout << "Filas: " << image.rows << "        Columnas: " << image.cols << endl;
    cout << "Comenzando Conversion..." << endl;

    auto start = high_resolution_clock::now();

    #pragma omp parallel num_threads(numThreads)
    {
        int startRow = omp_get_thread_num() * (image.rows / numThreads);
        int endRow = (omp_get_thread_num() + 1) * (image.rows / numThreads);

        processSectionOpenMP(image, startRow, endRow);
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "Finalizando Conversion..." << endl;
    cout << "Tiempo de ejecucion: " << duration.count() << " ms" << endl;

    imwrite("Zampedri_Paralel_OpenMP.jpg", image);

    return 0;
}
