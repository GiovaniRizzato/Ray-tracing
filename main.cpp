#include "rtweekend.h"

#include "hittable_list.h"
#include "camera.h"
#include "scenes.h"

#include <iostream>
#include <fstream>

double hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;
    auto discriminant = half_b*half_b - a*c;

    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-half_b - sqrt(discriminant) ) / a;
    }
}

color ray_color(const ray& r, const color& background, const hittable& world, int depth) {
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0,0,0);

    // If the ray hits nothing, return the background color.
    if (!world.hit(r, 0.001, infinity, rec))
        return background;

    ray scattered;
    color attenuation;
    color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        return emitted;

    return emitted + attenuation * ray_color(scattered, background, world, depth-1);
}

int main() {
    // Image
    auto aspect_ratio = 1.0;
    int image_width = 300;
    int samples_per_pixel = 200;
    const int max_depth = 30;
    auto background = color(0,0,0);

    // Camera
    point3 lookfrom_start(478, 278, -600);
    point3 lookat_start(278, 278, 0);
    point3 lookfrom_end(500, 300, -650);
    point3 lookat_end(250, 250, 0);
    vec3 vup(0,1,0);
    double vfov = 40.0;
    auto aperture = 0.01;

    //Animation
    double tempoTotal = 1.0;
    int framesPorSegundo = 12;

    // World
    auto world = final_scene(tempoTotal);
    //auto world = moving_test(point3(0.0, 0.0, -1.0), point3(0.0, 0.0, 1.0), tempoTotal);

    /** --------------------------------- **/
    int numeroTotalFrames = (int)(tempoTotal * (double)framesPorSegundo);
    double tempoPorFrame = tempoTotal / (double)numeroTotalFrames;
    int image_height = static_cast<int>(image_width / aspect_ratio);

    for(double n_frame=0.0; n_frame<=numeroTotalFrames; n_frame+= 1.0) {
        // File
        std::string fileName = "example" + std::to_string((int) n_frame) + ".ppm";
        std::ofstream imagem;
        imagem.open (fileName);

        // Render
        point3 lookfrom = lookfrom_start + ((lookfrom_end - lookfrom_start) * (n_frame / numeroTotalFrames));
        point3 lookat = lookat_start + ((lookat_end - lookat_start) * (n_frame / numeroTotalFrames));
        auto dist_to_focus = (lookfrom-lookat).length();
        camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, tempoPorFrame * n_frame, tempoPorFrame * (n_frame + 1.0));

        std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
        imagem << "P3\n" << image_width << " " << image_height << "\n255\n";

        for (int j = image_height-1; j >= 0; --j) {
            std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
            for (int i = 0; i < image_width; ++i) {
                color pixel_color(0, 0, 0);
                for (int s = 0; s < samples_per_pixel; ++s) {
                    auto u = (i + random_double()) / (image_width-1);
                    auto v = (j + random_double()) / (image_height-1);
                    ray r = cam.get_ray(u, v);
                    pixel_color += ray_color(r, background, world, max_depth);
                }
                //write_color(std::cout, pixel_color, samples_per_pixel);
                write_color(imagem, pixel_color, samples_per_pixel);
            }
        }

        imagem.close();
    }
    std::cerr << "\nDone.\n";
}
