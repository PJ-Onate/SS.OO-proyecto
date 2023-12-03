#include <iostream>
#include <opencv2/opencv.hpp>
#include <chrono>

using namespace cv;
using namespace std;
using namespace chrono;

int main(int argc, char** argv) {
    if (argc != 3) {
        cout << "Uso: " << argv[0] << " <imagen_entrada> <imagen_salida>" << endl;
        return -1;
    }

    string inputImageName = argv[1];
    string outputImageName = argv[2];

    auto start = high_resolution_clock::now();
    cout << "Cargando Imagen..." << endl;

    Mat image = imread(inputImageName, IMREAD_COLOR);

    if (image.empty()) {
        cout << "Error al cargar la imagen desde " << inputImageName << endl;
        return -1;
    } else {
        cout << "Imagen Cargada" << endl;
    }

    cout << "Filas: " << image.rows << "    Columnas: " << image.cols << endl;
    cout << "Comenzando conversion..." << endl;

    cvtColor(image, image, COLOR_BGR2GRAY);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "Finalizando..." << endl;
    cout << "Tiempo de ejecucion: " << duration.count() << " ms" << endl;

    imwrite(outputImageName, image);

    return 0;
}