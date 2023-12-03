#include <iostream>
#include <opencv2/opencv.hpp>
#include <chrono>
#include <future>

using namespace cv;
using namespace std;
using namespace chrono;

void processSection(Mat& image, int startRow, int endRow) {
    for (int r = startRow; r < endRow; r++) {
        cvtColor(image.row(r), image.row(r), COLOR_BGR2GRAY);
    }
}

int main(int argc, char** argv) {
    if (argc != 4) {
        cout << "Uso: " << argv[0] << " <imagen_entrada> <imagen_salida> <numero_hebras>" << endl;
        return -1;
    }

    string inputImageName = argv[1];
    string outputImageName = argv[2];
    int numThreads = 0;

    try {
        numThreads = stoi(argv[3]);
    } catch (const invalid_argument& e) {
        cerr << "Error: El número de hilos no es válido." << endl;
        return -1;
    } catch (const out_of_range& e) {
        cerr << "Error: El número de hilos está fuera de rango." << endl;
        return -1;
    }

    auto start = high_resolution_clock::now();
    cout << "Cargando Imagen..." << endl;

    Mat image = imread(inputImageName, IMREAD_COLOR);

    if (image.empty()) {
        cout << "Error al cargar la imagen." << endl;
        return -1;
    } else {
        cout << "Imagen Cargada" << endl;
    }

    cout << "Filas: " << image.rows << "        Columnas: " << image.cols << endl;
    cout << "Comenzando Conversion..." << endl;

    vector<future<void>> futures;

    int rowsPerTask = image.rows / numThreads;

    // Iniciar las tareas asíncronas
    for (int i = 0; i < numThreads; i++) {
        int startRow = i * rowsPerTask;
        int endRow = (i == numThreads - 1) ? image.rows : (i + 1) * rowsPerTask;

        futures.emplace_back(async(processSection, ref(image), startRow, endRow));
    }

    for (auto& future : futures) {
        future.wait();
    }

 
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "Finalizando..." << endl;
    cout << "Tiempo de ejecucion: " << duration.count() << " ms" << endl;

    imwrite(outputImageName, image);
    return 0;
}