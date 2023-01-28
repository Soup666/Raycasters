#include <iostream>
#include <vector>
#include <cmath>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

struct Ray {
    double x, y;
    double angle;
};

struct Map {
    int width, height;
    std::vector<std::vector<int>> data;
};

void initMap(Map& map) {
    map.width = 10;
    map.height = 10;
    map.data = std::vector<std::vector<int>>(map.height, std::vector<int>(map.width));
    for (int y = 0; y < map.height; y++) {
        for (int x = 0; x < map.width; x++) {
            map.data[y][x] = (x == 0 || x == map.width - 1 || y == 0 || y == map.height - 1);
        }
    }
}

int main() {
    Map map;
    initMap(map);

    std::vector<Ray> rays;
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        double angle = (3.14159265 / 3) * (x / (double)SCREEN_WIDTH - 0.5);
        rays.push_back({x, SCREEN_HEIGHT / 2, angle});
    }

    for (const Ray& ray : rays) {
        double distance = 0;
        double x = ray.x;
        double y = ray.y;
        double angle = ray.angle;

        while (distance < SCREEN_WIDTH) {
            int mapX = x / SCREEN_WIDTH * map.width;
            int mapY = y / SCREEN_HEIGHT * map.height;
            if (map.data[mapY][mapX]) {
                break;
            }

            distance += 0.1;
            x = ray.x + distance * cos(angle);
            y = ray.y + distance * sin(angle);
        }

        std::cout << x << " " << y << std::endl;
    }

    return 0;
}
