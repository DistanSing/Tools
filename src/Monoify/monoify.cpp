/*
 *		Copyright(c) 2021 - Jacob Blomquist
 *
 *		Permission is hereby granted, free of charge, to any person obtaining a copy
 *		of this softwareand associated documentation files(the "Software"), to deal
 *		in the Software without restriction, including without limitation the rights
 *		to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
 *		copies of the Software, and to permit persons to whom the Software is
 *		furnished to do so, subject to the following conditions :
 *
 *		The above copyright noticeand this permission notice shall be included in all
 *		copies or substantial portions of the Software.
 *
 *		THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *		IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *		FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
 *		AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *		LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *		OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *		SOFTWARE.
 */

#include <fstream>
#include <cstdio>
#include <cstdlib>

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		printf("Usage: %s <filename> <num channels>\nInput file is a raw 16 bit, little-endian pcm file.\nOutput is written to \"output.pcm\"", argv[0]);
		return 1;
	}

	int numChannels = atoi(argv[2]);
	if (!numChannels)
	{
		printf("Invalid number of channels!\n");
		return 1;
	}

	std::ifstream inFile;
	inFile.open(argv[1], std::ifstream::binary | std::ifstream::ate);

	if (!inFile.is_open())
	{
		printf("Unable to open: %s\n", argv[1]);
		return 1;
	}

	int filesize = inFile.tellg();
	inFile.seekg(0);


	int numFrames = filesize / numChannels/sizeof(short);
	
	short* inputData = new short[filesize / sizeof(short)];
	inFile.read((char*)inputData, filesize);
	inFile.close();


	short* outputData = new short[numFrames];

	for (int i = 0; i < numFrames; i++)
	{
		int index = i * numChannels;
		outputData[i] = inputData[index];
	}


	std::ofstream outfile;
	outfile.open("output.pcm", std::ofstream::binary);

	outfile.write((const char*)outputData, numFrames * sizeof(short));

	outfile.close();


	return 0;
}