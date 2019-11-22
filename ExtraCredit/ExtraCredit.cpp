// Kevin Vu
// 1525357
#include <iostream>
#include <string>
#include <cmath>
#include <ctime>

// libpng++-dev Library
#include <png++/png.hpp>

#define image png::image<png::rgb_pixel>
#define pixel png::rgb_pixel

// Final Window
int renderWidth = 100;
int renderHeight = 100;

int sampleSize = 25;
int sampleCount = 200;

// 2D array of Render Progress
int ** progress;

int difference_pixel(pixel &p1, pixel &p2)
{
	// Minimum diff = 0
	// Maximum diff = 768
	int diff = 0;
	diff += abs(p1.red - p2.red);
	diff += abs(p1.green - p2.green);
	diff += abs(p1.blue - p2.blue);
	
	return diff;
}

int difference_area(image &source, image &render, int x, int y, int a, int b)
{
	int total_diff = 0;
	
	for (int i = -sampleSize / 2; i < sampleSize / 2; i++)
	{
		for (int j = -sampleSize / 2; j < sampleSize / 2; j++)
		{
			if (a + i < 0 || b + j < 0 || a + i >= renderWidth || b + j >= renderHeight)
				continue;
			if (progress[a + i][b + j] == 0)
				continue;
			
			pixel source_pixel = source.get_pixel(x + i, y + j);
			pixel render_pixel = render.get_pixel(a + i, b + j);
			
			total_diff += difference_pixel(source_pixel, render_pixel);
		}
	}
	
	return total_diff;
}

pixel find_match(image &source, image &render, int a, int b)
{
	// Prevents from taking from outside source
	int w = source.get_width() - sampleSize;
	int h = source.get_height() - sampleSize;
	
	int copy_x = 0;
	int copy_y = 0;
	int copy_diff = sampleSize * sampleSize * 768;
	
	// Select random canidate
	for (int i = 0; i < sampleCount; i++)
	{
		int x = std::rand() % w + sampleSize / 2;
		int y = std::rand() % h + sampleSize / 2;
		int diff = difference_area(source, render, x, y, a, b);
		// Get Minumum
		if (diff < copy_diff)
		{
			copy_x = x;
			copy_y = y;
			copy_diff = diff;
		}
	}

	return source.get_pixel(copy_x, copy_y);
}

void process_pixel(image &source, image &render, int a, int b)
{
	if (a < 0 || b < 0 || a >= renderWidth || b >= renderHeight)
		return;
	if (progress[a][b] == 1)
		return;
	
	// Find mathch and copy pixel
	render.set_pixel(a, b, find_match(source, render, a, b));
	
	// Mark pixel as 'Rendered'
	progress[a][b] = 1;
}

image generate(image &source, image &render)
{
	
	// Seed Random ints
    std::srand(std::time(0));

    //Instantiate Render Progress 2D Array
    progress = new int*[renderWidth];
    for (int a = 0; a < renderWidth; a++)
    {
        progress[a] = new int[renderHeight];
        for (int b = 0; b < renderHeight; b++)
            progress[a][b] = 0;
    }

    int b = 0;

    while (b <= renderHeight)
    {
        // Render Row
        for (int i = 0; i < renderWidth; i++)
        {
            process_pixel(source, render, i, b);
        }

        // Next Row
        b++;
    }
    return render;
}

int main(int argc, char ** argv)
{
	std::string inputName = "input.png";
	std::string outputName = "output.png";
	
	// Change second argument to inputName
	if (argc >= 2)
	{
		inputName = argv[1];
	}
	
	// Load Source Image
	image source(inputName);
	
	// Load Final Image
	image render(renderWidth, renderHeight);
	
	// Begin Generating pixels
	generate(source, render);
	
	// Save Image
	render.write(outputName);
	
	return 0;
}
