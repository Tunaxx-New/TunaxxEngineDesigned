//--Function-with-color----------------------------------------------------

//
// Funciton converts rgb to hex
//
unsigned int rgbToHex(short r, short g, short b)
{
	if (r > 255)
		r = 255;
	if (g > 255)
		g = 255;
	if (b > 255)
		b = 255;

	if (r < 0)
		r = 0;
	if (g < 0)
		g = 0;
	if (b < 0)
		b = 0;

	return r * 0x010000 + g * 0x000100 + b;
}

//-------------------------------------------------------------------------

//--Complex number struct--------------------------------------------------
// This class have math functions and two fields
struct Complex
{
	float real;
	float imag;
};

//
// initializeComplex initializing complex number
// @param1 number where it changes
// @param2 real part
// @param3 imaginary part
void initializeComplex(struct Complex* c, float r, float i)
{
	c->real = r;
	c->imag = i;
}



//
// sumComplex summarazing two complex numbers
// @param1 number to add (changes)
// @param2 number source add from
void sumComplex(struct Complex* c, struct Complex* cAdd)
{
	c->real += cAdd->real;
	c->imag += cAdd->imag;
}



//
// lengthComplex len
// @param1 number
//
float lengthComplex(struct Complex* c)
{
	return sqrt(c->real * c->real + c->imag * c->imag);
}



//
// absComplex len
// @param1 number
//
float absComplex(struct Complex* c)
{
	return c->real * c->real + c->imag * c->imag;
}



//
// sqrComplex pows complex number in square
// @param1 number to square (changes)
//
void sqrComplex(struct Complex* c)
{
	float temp = c->real;
	c->real = c->real * c->real - c->imag * c->imag;
	c->imag = 2 * temp * c->imag;
}


//
// cubComplex pows complex number in cubic
// @param1 number to cube (changes)
//
void cubComplex(struct Complex* c)
{
	float temp = c->real;
	c->real = c->real * c->real * c->real + 3 * c->imag * c->imag * c->real;
	c->imag = -3 * temp * temp * c->imag - c->imag * c->imag * c->imag;
}
//-------------------------------------------------------------------------



//
// This is function bitmapping the fractal
//
kernel void mandelbrotSet(global unsigned int* bitmap, short width, short height, float paramReal, float paramImag, int offset)
{
	for (int gId = get_global_id(0); gId < height * width; gId += offset)
	{

	//Coordinates, index, complex numbers
	float x = (float)(gId % width - width / 2) / 500.0 / paramReal;
	float y = (float)(gId / width - height / 2) / 500.0 / paramReal;
	struct Complex c;
	struct Complex z;

	//Initializing
	initializeComplex(&c, x - 0.98, y + paramImag / 100000 - 0.3);
	initializeComplex(&z, 0, 0);
	bitmap[gId] = 0;

	//Cycle with alpha paint in complex function increase length
	for (float j = 0; j < 10; j += 1) {
		float i;
		for (i = 0; i < 10; i++)
		{
			sqrComplex(&z);
			sumComplex(&z, &c);
			if (absComplex(&z) >= j)
				break;
		}
		short huer = 255 * i * j / 100;
		short hueg = 255 * i / 60 * absComplex(&z);
		short hueb = 255 * i / 100 * j;

		if (hueg >= 128) {
			huer += hueg;
			hueg = 20;
		}

		//huer /= 20;
		//hueg /= 20;
		hueb /= 20;

		//huer = (huer + hueg + hueb) / 3;

		bitmap[gId] += rgbToHex(huer + hueg / 2, hueg / 2, log(paramReal));
	}

	}
}
//Fractal-Field---------------------------------------------------------------------------------

//
// This is function bitmapping the game of life
//
kernel void conwayGoL(global unsigned int* bitmap, short width, short height, global unsigned int* newBitmap, int offset)
{
	//Coordinates, index
	//int gId = get_global_id(0);
	for (int gId = get_global_id(0); gId < height * width; gId += offset)
	{

	int x = gId % width;
	int y = gId / width;
	if (bitmap[gId] == rgbToHex(64, 64, 256))
		bitmap[gId] = 0;

	//Cycling 9 cells and counting life
	int life = 0;
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			int x0 = x + j;
			int y0 = y + i;
			//If index out of border
			if (x0 < 0) {
				x0 = width - 1;
			}
			else if (x0 >= width) {
				x0 = 0;
			}
			if (y0 < 0) {
				y0 = height - 1;
			}
			else if (y0 >= height) {
				y0 = 0;
			}
			if (bitmap[y0 * width + x0] == rgbToHex(255, 255, 255))
				life++;
		}
	}

	//Rules of cell
	short hue = 0;
	short hue2 = 0;
	if (bitmap[gId] != 0)
		hue = 255;
	newBitmap[gId] = bitmap[gId];
	if (bitmap[gId] == rgbToHex(255, 255, 255)) {
		if (life > 4 || life < 3)
			hue = 64;
	}
	else {
		if (life == 3)
			hue = 255;
	}
	hue2 = hue;
	if (hue == 64)
		hue2 = 256;

	newBitmap[gId] = rgbToHex(hue, hue, hue2);

	}
}
//GOL-Field-----------------------------------------------------------------------

kernel void fill(global unsigned int* bitmap, size_t size, size_t thread_count)
{
	float thread_index = (float)get_global_id(0) / (float)thread_count;
	float thread_color = 255.0 * thread_index;
	for (int gId = get_global_id(0); gId < size; gId += thread_count)
	{

		bitmap[gId] =  rgbToHex(thread_color, thread_color, thread_color);

	}
}

//GPU-ENGINE-RASTER---------------------------------------------------------------
//
// Structs in cl and cpp
//
#define OBJ_COORD_COUNT 3
union vertex
{
    float coordinates[OBJ_COORD_COUNT];
};
//
// Brezenham algorithm for line
//
void line(global unsigned int* bitmap, int x1, int y1, int x2, int y2, short width, short height, int size) {
    const int deltaX = abs(x2 - x1);
    const int deltaY = abs(y2 - y1);
    const int signX = x1 < x2 ? 1 : -1;
    const int signY = y1 < y2 ? 1 : -1;
    int error = deltaX - deltaY;
    int index = y2 * width + x2;
    if (0 <= x2 && x2 < width && 0 <= y2 && y2 < height)
		if (0 <= index && index < size)
			bitmap[index] = rgbToHex(255, 255, 255);
    while(x1 != x2 || y1 != y2) 
   {
        int index = y1 * width + x1;
        if (0 <= x1 && x1 < width && 0 <= y1 && y1 < height)
			if (0 <= index && index < size)
				bitmap[index] = rgbToHex(255, 255, 255);
        int error2 = error + error;
        if(error2 > -deltaY)
        {
            error -= deltaY;
            x1 += signX;
        }
        if(error2 < deltaX) 
        {
            error += deltaX;
            y1 += signY;
        }
    }

}


//
// This function for blitting union object with params
//
kernel void blitLine(
	global unsigned int* bitmap,
	short                width,
	short                height,
	int                  size,
	global union vertex* vertex,
	global int*          lines,
	size_t               lsize,
	int                  offset)
{
	for (int gId = get_global_id(0); gId < lsize; gId += offset)
	{
		//bitmap[(int)((vertex[gId].coordinates[1] + height / 2) * width + vertex[gId].coordinates[0] + width / 2)] = rgbToHex(255, 255, 255);
		//float x  = vertex[gId].coordinates[0];
		//float y  = vertex[gId].coordinates[1];
		//x += width / 2;
		//y += height / 2;
		//bitmap[(int)y * width + (int)x] = rgbToHex(255, 255, 255);
		if (gId > 0) {
		float x  = vertex[lines[gId]].coordinates[0];
		float y  = vertex[lines[gId]].coordinates[1];
		float x0 = vertex[lines[gId - 1]].coordinates[0];
		float y0 = vertex[lines[gId - 1]].coordinates[1];
	
		x += width / 2;
		y += height / 2;
		x0 += width / 2;
		y0 += height / 2;
		//if (0 <= x && x < width)
		//	bitmap[(int)y * width + (int)x] = rgbToHex(255, 255, 255);
		if (vertex[lines[gId]].coordinates[2] < -0.1f && vertex[lines[gId - 1]].coordinates[2] < -0.1f)
			line(bitmap, (int)x0, (int)y0, (int)x, (int)y, width, height, size);
		}
	}
}



//
// This function for blitting union object with params
//
#define MATRIX_SIZE 4
kernel void matrix(
	global union vertex* vertex,
	global float*        matrix,
	global union vertex* result,
	size_t               vsize,
	int                  offset,
	int                  divZ)
{
	for (int gId = get_global_id(0); gId < vsize; gId += offset)
	{

	for (int j = 0; j < MATRIX_SIZE - 1; j++)
	{
		float summarazing = 0;
		for (int k = 0; k < MATRIX_SIZE - 1; k++)
		{
			summarazing += vertex[gId].coordinates[k] * matrix[j * MATRIX_SIZE + k];
		}
		summarazing += matrix[j * MATRIX_SIZE + MATRIX_SIZE - 1];
		result[gId].coordinates[j] = summarazing;
	}

	if (divZ == 5)
		for (int k = 0; k < 2; k++) {
			if (result[gId].coordinates[2] != 0)
				result[gId].coordinates[k] /= result[gId].coordinates[2];
		}

	}
	barrier(CLK_GLOBAL_MEM_FENCE);
}




//
// Is point belongs to poly
//
int pnpoly(int npol, float* xpols, float* ypols, float x, float y)
{
	bool is = 0;
	for (int i = 0, j = npol - 1; i < npol; j = i++)
    {
    	if (((ypols[i] <= y) && (y < ypols[j]) || ((ypols[j] <= y) && (y < ypols[i]))) &&
    		(((ypols[j] - ypols[i]) != 0) &&
    		(x > ((xpols[j] - xpols[i]) * (y - ypols[i]) / (ypols[j] - ypols[i]) + xpols[i]))))
    		is = !is;
    }
	return is;
}




kernel void blitFace(
	global unsigned int* bitmap,
	short                width,
	short                height,
	int                  size,
	global union vertex* vertex,
	global int*          lines,
	size_t               lsize,
	int                  offset)
{
	for (int gId = get_global_id(0); gId < lsize; gId += offset)
	{
		if (gId > 2) {
			float facex[4];
			float facey[4];
			facex[0] = vertex[lines[gId]].coordinates[0] + width / 2;
			facex[1] = vertex[lines[gId - 1]].coordinates[0] + width / 2;
			facex[2] = vertex[lines[gId - 2]].coordinates[0] + width / 2;
			facex[3] = vertex[lines[gId - 3]].coordinates[0] + width / 2;
			facey[0] = vertex[lines[gId]].coordinates[1] + height / 2;
			facey[1] = vertex[lines[gId - 1]].coordinates[1] + height / 2;
			facey[2] = vertex[lines[gId - 2]].coordinates[1] + height / 2;
			facey[2] = vertex[lines[gId - 3]].coordinates[1] + height / 2;
			if (vertex[lines[gId]].coordinates[2] < -1 && vertex[lines[gId - 1]].coordinates[2] < -1 && vertex[lines[gId - 2]].coordinates[2] < -1)
			for (int i = min(facey[3], min(facey[0], min(facey[1], facey[2]))); i <= max(facey[3], max(facey[0], max(facey[1], facey[2]))); i++) {
				for (int j = min(facex[3], min(facex[0], min(facex[1], facex[2]))); j <= max(facex[3], max(facex[0], max(facex[1], facex[2]))); j++) {
					int index = i * width + j;
			        if (0 <= j && i < width && 0 <= i && i < height)
						if ((0 <= index) && (index < size))
							if (pnpoly(3, facex, facey, j, i))
								bitmap[index] = rgbToHex(255, 255, 255);
				}
			}

		}
	}
}
//--------------------------------------------------------------------------------------------------------------------