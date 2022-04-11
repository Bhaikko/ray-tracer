#include <iostream>

#include "./../include/vec3.h"
#include "./../include/color.h"

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
            // Setting Color range from [0, 1]
            color pixel_color(
                double(i) / (IMAGE_WIDTH - 1),
                double(j) / (IMAGE_HEIGHT - 1),
                0.25
            );

            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";

    return 0;
}