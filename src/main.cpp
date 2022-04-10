#include <iostream>

// Image Configurations
#define IMAGE_WIDTH     256
#define IMAGE_HEIGHT    256

int main()
{
    std::cout << "P3\n" << IMAGE_WIDTH << ' ' << IMAGE_HEIGHT << "\n255\n";

    // Render
    for (int j = IMAGE_HEIGHT - 1; j >= 0; --j) {
        // Using std::cerr since std::cout stream is output to image file
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < IMAGE_WIDTH; ++i) {
            double r = double(i) / (IMAGE_WIDTH - 1);
            double g = double(j) / (IMAGE_HEIGHT - 1);
            double b = 0.25;

            int ir = static_cast<int>(255.999 * r);
            int ig = static_cast<int>(255.999 * g);
            int ib = static_cast<int>(255.999 * b);

            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }

    std::cerr << "\nDone.\n";

    return 0;
}