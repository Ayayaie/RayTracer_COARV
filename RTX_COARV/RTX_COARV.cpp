// RTX_COARV.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittablelist.h"
#include "material.h"
#include "sphere.h"

using namespace std;


int main() {


    hittable_list world;

    /*auto material1 = make_shared<dielectric>(1.5);

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    for (int a = -4; a < 2; a++) {
        for (int b = -2; b < 2; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.3, b + 0.9 * random_double());

            if ((center - point3(4, 0.5, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.2) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.3, material1));
                }
                else if (choose_mat < 0.6) {
                    // metal
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.3, sphere_material));
                }
                else {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.3, sphere_material));
                }
            }
        }
    }

    // metal
    auto albedo = color::random(0.5, 1);
    auto fuzz = random_double(0, 0.5);
    auto sphere_material = make_shared<metal>(albedo, fuzz);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, sphere_material));

    auto material2 = make_shared<dielectric>(1.2); 
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2)); 

    auto material3 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));*/

    // ----- ground -----
    auto ground = make_shared<lambertian>(color(0.45, 0.45, 0.45));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground));


    // ----- materials -----
    auto glass = make_shared<dielectric>(1.5);
    auto glass2 = make_shared<dielectric>(1.2);

    auto red = make_shared<lambertian>(color(0.8, 0.2, 0.2));
    auto blue = make_shared<lambertian>(color(0.2, 0.3, 0.8));
    auto green = make_shared<lambertian>(color(0.2, 0.8, 0.3));

    auto gold = make_shared<metal>(color(0.9, 0.7, 0.3), 0.05);
    auto rough = make_shared<metal>(color(0.8, 0.8, 0.8), 0.4);


    // ----- composition (only 6 spheres) -----

    // big center glass
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, glass));

    // hollow glass bubble (cool refractions)
    world.add(make_shared<sphere>(point3(0, 1, 0), 0.5, glass2));

    // left matte sphere
    world.add(make_shared<sphere>(point3(-2.2, 0.6, 1.0), 0.6, red));

    // right shiny metal
    world.add(make_shared<sphere>(point3(2.2, 0.6, 1.0), 0.6, gold));

    // small floating rough metal
    world.add(make_shared<sphere>(point3(0.5, 0.35, -1.2), 0.35, rough));

    // small colored diffuse
    world.add(make_shared<sphere>(point3(-1.2, 0.35, -1.5), 0.35, blue));

    // small colored diffuse
    world.add(make_shared<sphere>(point3(-0.6, 0.35, 1.5), 0.35, green));



    const int frame_count = 96;     // nombre d'images de l'animation
    const double radius = 10.0;      // distance au centre
    const double height = 2.0;

    for (int f = 0; f < frame_count; ++f) {

        // ---- nom du fichier : frame_000.ppm, frame_001.ppm, ...
        ostringstream name;
        name << "frame_" << setw(3) << setfill('0') << f << ".ppm";
        ofstream out(name.str());

        camera cam;

        cam.aspect_ratio = 16.0 / 9.0;
        cam.image_width = 1000;
        cam.samples_per_pixel = 100;
        cam.max_depth = 50;

        cam.vfov = 20;
        cam.lookat = point3(0, 0.6, 0);
        cam.vup = vec3(0, 1, 0);

        cam.defocus_angle = 0;
        cam.focus_dist = 10.0;

        // ---- rotation orbitale ----
        double theta = 2.0 * pi * f / frame_count;

        double x = radius * cos(theta);
        double z = radius * sin(theta);

        cam.lookfrom = point3(x, height, z);

        clog << "Rendering frame " << f + 1 << "/" << frame_count << "\n";

        cam.render(world, out);
        out.close();
    }
}